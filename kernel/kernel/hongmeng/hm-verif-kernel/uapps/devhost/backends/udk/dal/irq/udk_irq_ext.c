/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: UDK irq extention implementation
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 28 15:25:41 2021
 */
#include <udk/irq_ext.h>
#include <udk/log.h>
#include <udk/kvic.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/bitops.h>
#include <udk/securec.h>
#include <udk/lib/idr.h>
#include <internal/irq.h>
#include <internal/init.h>
#include <devhost/interrupt.h>
#include <hongmeng/errno.h>
#include <internal/of.h>
#include <libalgo/rbtree.h>

#define IRQ_MUX_INVALID		(~0U)

#define UIRQ_MIN_ID		(UDK_KVIC_IRQ_MAX + 1) /* kvic irqs are not configured in dts */
#define UIRQ_MAX_ID		IDR_MAX_ID

#define GIC_IRQ_CELLS		3U

struct irq_ext_node {
	struct udk_irq_ext *ext;
	struct irq_ext_node *parent;
	/*
	 * hw_irq handler idr whose id is hw_irq;
	 * for normal ext node, hw_irq = raw_irq;
	 * for kernel ext node, hw_irq = kirq;
	 */
	struct idr handler_idr;
	struct udk_mutex lock;
	struct dlist_node node;
	/*
	 * For irq ext who does not provide the get_child_irq op,
	 * this map stores the map relations between parent irq and child irq.
	 */
	void *irq_map;
};

struct hwirq_handler {
	irq_handler_t handler;
	/*
	 * for uirq handler, irq = uirq;
	 * for irq_ext_handler, irq = raw_irq
	 */
	int irq;
	/*
	 * for uirq handler, ctx = udk_request_irq's ctx;
	 * for irq_ext_handler, ctx = child ext node
	 */
	void *ctx;
};

struct irq_ext_center {
	struct udk_mutex lock;
	struct dlist_node ext_list;
};

struct uirq_entry {
	unsigned int uirq;
	unsigned int hw_irq;
	struct irq_ext_node *ext_node;
};

static struct irq_ext_center irq_ext_mgr = {
	.ext_list = DLIST_HEAD_INIT(irq_ext_mgr.ext_list),
};

struct uirq_manager {
	struct idr uirq_idr;
	struct udk_mutex lock;
};

struct irq_rbt_node {
	struct rbt_node node;
	int parent_irq;
	int child_irq;
};

DEFINE_rbt_search(irq, struct irq_rbt_node, node, parent_irq)

static struct uirq_manager uirq_mgr;

static struct irq_ext_node kernel_ext_node; /* virtual ext node for GIC or ITS */

#define is_irq_ext_op_valid(ext, op)	\
	((ext) != NULL &&		\
	 ext->ops != NULL &&		\
	 ext->ops->op != NULL)		\

#define IRQ_EXT_OPS_DEFINE(op)					\
static int irq_ext_##op(struct irq_ext_node *node,		\
			    unsigned int raw_irq)		\
{								\
	struct udk_irq_ext *ext = node->ext;			\
	if (is_irq_ext_op_valid(ext, irq_##op)) {		\
		return ext->ops->irq_##op(ext->ctx, raw_irq);	\
	}							\
	return 0;						\
}								\

IRQ_EXT_OPS_DEFINE(get_parent_irq)
IRQ_EXT_OPS_DEFINE(get_child_irq)
IRQ_EXT_OPS_DEFINE(activate)

#define IRQ_EXT_NO_RETURN_OPS_DEFINE(op)			\
static void irq_ext_##op(struct irq_ext_node *node,		\
			     unsigned int raw_irq)		\
{								\
	struct udk_irq_ext *ext = node->ext;			\
	if (is_irq_ext_op_valid(ext, irq_##op)) {		\
		ext->ops->irq_##op(ext->ctx, raw_irq);		\
	}							\
}								\

IRQ_EXT_NO_RETURN_OPS_DEFINE(eoi)
IRQ_EXT_NO_RETURN_OPS_DEFINE(mask)
IRQ_EXT_NO_RETURN_OPS_DEFINE(unmask)
IRQ_EXT_NO_RETURN_OPS_DEFINE(deactivate)

static inline void irq_ext_lock(struct irq_ext_node *node)
{
	udk_mutex_lock(&node->lock);
}

static inline void irq_ext_unlock(struct irq_ext_node *node)
{
	udk_mutex_unlock(&node->lock);
}

static inline void irq_ext_mgr_lock(void)
{
	udk_mutex_lock(&irq_ext_mgr.lock);
}

static inline void irq_ext_mgr_unlock(void)
{
	udk_mutex_unlock(&irq_ext_mgr.lock);
}

static struct irq_ext_node *irq_ext_node_find(const char *name)
{
	struct irq_ext_node *pos = NULL;
	struct irq_ext_node *result = NULL;

	irq_ext_mgr_lock();
	dlist_for_each_entry(pos, &irq_ext_mgr.ext_list,
			     struct irq_ext_node, node) {
		/* find irq extention by name */
		if (strcmp(pos->ext->name, name) == 0) {
			result = pos;
			break;
		}
	}
	irq_ext_mgr_unlock();

	return result;
}

static void irq_ext_node_insert(struct irq_ext_node *node)
{
	irq_ext_mgr_lock();
	dlist_insert(&irq_ext_mgr.ext_list, &(node->node));
	irq_ext_mgr_unlock();
}

static inline void uirq_mgr_lock(void)
{
	udk_mutex_lock(&uirq_mgr.lock);
}

static inline void uirq_mgr_unlock(void)
{
	udk_mutex_unlock(&uirq_mgr.lock);
}

static struct uirq_entry *uirq_entry_search(const struct irq_ext_node *node,
					    unsigned int hw_irq)
{
	struct idr *uirq_idr = &uirq_mgr.uirq_idr;
	struct uirq_entry *entry = NULL;
	unsigned int id = 0;

	do {
		entry = (struct uirq_entry *)idr_get_next(uirq_idr, &id);
		if (entry == NULL) {
			break;
		}
		id++;
	} while (!(entry->ext_node == node && entry->hw_irq == hw_irq));

	return entry;
}

static inline struct uirq_entry *uirq_entry_lookup(unsigned int uirq)
{
	return (struct uirq_entry *)idr_find(&uirq_mgr.uirq_idr, uirq);
}

static struct irq_ext_node *get_ext_node_by_name(const char *ext_name)
{
	if (ext_name != NULL) {
		return irq_ext_node_find(ext_name);
	} else {
		return &kernel_ext_node;
	}
}

static int irq_ext_uirq_alloc(struct irq_ext_node *ext_node, unsigned int hw_irq)
{
	struct uirq_entry *entry = NULL;
	int ret;

	uirq_mgr_lock();
	entry = uirq_entry_search(ext_node, hw_irq);
	if (entry != NULL) {
		uirq_mgr_unlock();
		return (int)entry->uirq;
	}
	entry = (struct uirq_entry *)udk_malloc(sizeof(*entry));
	if (entry == NULL) {
		uirq_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*entry);
	ret = idr_alloc_cyclic(&uirq_mgr.uirq_idr, entry, UIRQ_MIN_ID, UIRQ_MAX_ID);
	if (ret < 0) {
		uirq_mgr_unlock();
		udk_free(entry);
		return -hmerrno2posix(ret);
	}
	entry->uirq = (unsigned int)ret;
	entry->hw_irq = hw_irq;
	entry->ext_node = ext_node;
	uirq_mgr_unlock();

	return (int)entry->uirq;
}

int udk_irq_ext_uirq_alloc(const char *ext_name, unsigned int hw_irq)
{
	struct irq_ext_node *ext_node = NULL;
	int ret;

	ext_node = get_ext_node_by_name(ext_name);
	if (ext_node == NULL) {
		udk_warn("failed to find irq ext:%s\n", ext_name);
		return -EINVAL;
	}
	irq_ext_lock(ext_node);
	ret = irq_ext_uirq_alloc(ext_node, hw_irq);
	if (ret < 0) {
		udk_warn("irq ext uirq alloc failed: %s\n", strerror(-ret));
	}
	irq_ext_unlock(ext_node);

	return ret;
}

static int irq_ext_uirq_map(struct irq_ext_node *ext_node, unsigned int hw_irq, unsigned int target_uirq)
{
	struct uirq_entry *entry = NULL;
	int ret;

	uirq_mgr_lock();
	entry = uirq_entry_search(ext_node, hw_irq);
	if (entry != NULL) {
		if (entry->uirq == target_uirq) {
			uirq_mgr_unlock();
			return (int)target_uirq;
		} else {
			uirq_mgr_unlock();
			return -EEXIST;
		}
	}
	entry = (struct uirq_entry *)udk_malloc(sizeof(*entry));
	if (entry == NULL) {
		uirq_mgr_unlock();
		return -ENOMEM;
	}
	mem_zero_s(*entry);
	ret = idr_set(&uirq_mgr.uirq_idr, entry, target_uirq);
	if (ret < 0) {
		udk_free(entry);
		uirq_mgr_unlock();
		return -hmerrno2posix(ret);
	}
	entry->uirq = target_uirq;
	entry->hw_irq = hw_irq;
	entry->ext_node = ext_node;
	uirq_mgr_unlock();

	return (int)target_uirq;
}

int udk_irq_ext_uirq_map(const char *ext_name, unsigned int hw_irq, unsigned int target_uirq)
{
	struct irq_ext_node *ext_node = NULL;
	int uirq;

	if (target_uirq < UIRQ_MIN_ID || target_uirq >= UIRQ_MAX_ID) {
		udk_warn("map an invalid uirq=%u\n", target_uirq);
		return -EINVAL;
	}
	ext_node = get_ext_node_by_name(ext_name);
	if (ext_node == NULL) {
		udk_warn("failed to find irq ext:%s\n", ext_name);
		return -EINVAL;
	}
	irq_ext_lock(ext_node);
	uirq = irq_ext_uirq_map(ext_node, hw_irq, target_uirq);
	if (uirq < 0) {
		udk_warn("irq ext uirq map failed: %s\n", strerror(-uirq));
	}
	irq_ext_unlock(ext_node);

	return uirq;
}

void udk_irq_ext_uirq_free(unsigned int uirq)
{
	struct uirq_entry *entry = NULL;

	uirq_mgr_lock();
	entry = idr_remove(&uirq_mgr.uirq_idr, uirq);
	if (entry != NULL) {
		udk_free(entry);
	}
	uirq_mgr_unlock();
}

static void irq_ext_uirq_clean(const struct irq_ext_node *node)
{
	struct idr *uirq_idr = &uirq_mgr.uirq_idr;
	struct uirq_entry *entry = NULL;
	unsigned int id = 0;

	uirq_mgr_lock();
	do {
		entry = (struct uirq_entry *)idr_get_next(uirq_idr, &id);
		if (entry != NULL && entry->ext_node == node) {
			(void)idr_remove(uirq_idr, id);
			udk_free(entry);
		}
		id++;
	} while (entry != NULL);
	uirq_mgr_unlock();
}

static int irq_ext_irq_map_lookup(struct irq_ext_node *node,
				  int parent_irq)
{
	struct rbt_tree *irq_tree = (struct rbt_tree *)node->irq_map;
	struct irq_rbt_node *irq_node = NULL;

	irq_node = irq_rbt_search(irq_tree, parent_irq, NULL);
	if (irq_node == NULL) {
		udk_warn("irq rbt node not found\n");
		return -EINVAL;
	}

	return irq_node->child_irq;
}

static void irq_ext_irq_map_remove(struct irq_ext_node *node,
				   int parent_irq)
{
	struct rbt_tree *irq_tree = (struct rbt_tree *)node->irq_map;
	struct irq_rbt_node *irq_node = NULL;

	irq_node = irq_rbt_search(irq_tree, parent_irq, NULL);
	if (irq_node == NULL) {
		return;
	}
	rbt_remove(irq_tree, &irq_node->node);
	udk_free(irq_node);
}

static int irq_ext_irq_map_insert(struct irq_ext_node *node,
				  int parent_irq,
				  int child_irq)
{
	struct rbt_tree *irq_tree = (struct rbt_tree *)node->irq_map;
	struct irq_rbt_node *irq_parent_node = NULL;
	struct irq_rbt_node *irq_node = NULL;
	struct rbt_node *parent_node = NULL;
	bool is_rchild = false;

	irq_node = irq_rbt_search(irq_tree, parent_irq, &parent_node);
	if (irq_node != NULL) {
		return 0;
	}
	if (parent_node != NULL) {
		irq_parent_node = rbt_entry(parent_node, struct irq_rbt_node, node);
		if (irq_parent_node->parent_irq < parent_irq) {
			is_rchild = true;
		}
	}
	irq_node = (struct irq_rbt_node *)udk_malloc(sizeof(*irq_node));
	if (irq_node == NULL) {
		udk_warn("irq rbt node malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*irq_node);
	irq_node->parent_irq = parent_irq;
	irq_node->child_irq = child_irq;
	rbt_insert(irq_tree, &irq_node->node, parent_node, is_rchild);

	return 0;
}

static int irq_ext_irq_map_init(struct irq_ext_node *node)
{
	struct rbt_tree *irq_tree = NULL;

	irq_tree = (struct rbt_tree *)udk_malloc(sizeof(*irq_tree));
	if (irq_tree == NULL) {
		udk_warn("irq rb tree malloc failed\n");
		return -ENOMEM;
	}
	rbt_tree_init(irq_tree);
	node->irq_map = irq_tree;

	return 0;
}

static void irq_rbt_node_free(struct rbt_node *node)
{
	struct irq_rbt_node *irq_node = NULL;

	irq_node = rbt_entry(node, struct irq_rbt_node, node);
	udk_free(irq_node);
}

static void irq_ext_irq_map_destroy(struct irq_ext_node *node)
{
	struct rbt_tree *irq_tree = (struct rbt_tree *)node->irq_map;

	if (irq_tree == NULL) {
		return;
	}
	rbt_destroy_tree(irq_tree, irq_rbt_node_free);
	udk_free(irq_tree);
}

int udk_irq_ext_add(struct udk_irq_ext *ext, const char *parent)
{
	int err;
	struct irq_ext_node *node= NULL;
	struct irq_ext_node *parent_node = NULL;

	if ((ext == NULL) || (ext->name == NULL) || (ext->name[0] == '\0')) {
		udk_error("invalid irq extention\n");
		return -EINVAL;
	}
	if (!is_irq_ext_op_valid(ext, irq_get_parent_irq)) {
		udk_error("invalid irq extention ops\n");
		return -EINVAL;
	}
	if (irq_ext_node_find(ext->name) != NULL) {
		return -EEXIST;
	}
	if (parent != NULL) {
		parent_node = irq_ext_node_find(parent);
		if (parent_node == NULL) {
			return -EINVAL;
		}
	} else {
		parent_node = &kernel_ext_node;
	}
	node = (struct irq_ext_node *)udk_malloc(sizeof(struct irq_ext_node));
	if (node == NULL) {
		return -ENOMEM;
	}
	if (!is_irq_ext_op_valid(ext, irq_get_child_irq)) {
		err = irq_ext_irq_map_init(node);
		if (err < 0) {
			udk_free(node);
			return err;
		}
	}
	node->ext = ext;
	node->parent = parent_node;
	idr_init(&node->handler_idr);
	udk_mutex_init(&node->lock);
	irq_ext_node_insert(node);

	return 0;
}

static int irq_ext_remove(struct udk_irq_ext *ext)
{
	struct irq_ext_node *pos = NULL;
	struct irq_ext_node *dst = NULL;

	irq_ext_mgr_lock();
	dlist_for_each_entry(pos, &irq_ext_mgr.ext_list,
			     struct irq_ext_node, node) {
		if (pos->parent->ext == ext) {
			irq_ext_mgr_unlock();
			return -EINVAL;
		}
		if (pos->ext == ext) {
			dst = pos;
		}
	}
	if (dst == NULL) {
		irq_ext_mgr_unlock();
		return -EINVAL;
	}
	irq_ext_lock(dst);
	if (!idr_is_empty(&dst->handler_idr)) {
		udk_warn("handler idr is not empty\n");
		irq_ext_unlock(dst);
		irq_ext_mgr_unlock();
		return -EINVAL;
	}
	irq_ext_uirq_clean(dst);
	dlist_delete(&dst->node);
	irq_ext_irq_map_destroy(dst);
	irq_ext_unlock(dst);
	irq_ext_mgr_unlock();
	udk_free(dst);

	return 0;
}

int udk_irq_ext_remove(struct udk_irq_ext *ext)
{
	int err;

	if (ext == NULL) {
		udk_error("invalid irq extention\n");
		return -EINVAL;
	}
	/*
	 * The caller should ensure the extention can be safely
	 * removed, that is, no other extentions still depend on it.
	 * If not, the function won't remove the extention and only
	 * return an error message.
	 */
	err = irq_ext_remove(ext);
	if (err < 0) {
		udk_error("remove irq extention failed: %s\n", strerror(-err));
		return err;
	}

	return 0;
}

static bool is_trigger_type_valid(unsigned int type)
{
	return type == UDK_IRQ_TRIGGER_NONE ||
	       type == UDK_IRQ_TRIGGER_RISING ||
	       type == UDK_IRQ_TRIGGER_FALLING ||
	       type == UDK_IRQ_TRIGGER_EDG_BOTH ||
	       type == UDK_IRQ_TRIGGER_HIGH ||
	       type == UDK_IRQ_TRIGGER_LOW ||
	       type == UDK_IRQ_TRIGGER_DEFAULT;
}

static int irq_ext_set_trigger(struct irq_ext_node *node,
			       unsigned int raw_irq,
			       unsigned int flags)
{
	struct udk_irq_ext *ext = node->ext;
	unsigned int type;
	int err;

	type = UDK_MASK(flags, UDK_IRQ_FLAGS_TRIGGER_MASK);
	if (!is_trigger_type_valid(type)) {
		udk_warn("invalid trigger type=%u\n", type);
		return -EINVAL;
	}
	if (is_irq_ext_op_valid(node->ext, irq_set_trigger)) {
		err = ext->ops->irq_set_trigger(ext->ctx, raw_irq, type);
		if (err < 0) {
			udk_warn("ext=%s set trigger type for irq=%d failed\n",
				 ext->name, raw_irq);
			return err;
		}
	}

	return 0;
}

static int irq_ext_kirq_map(struct irq_ext_node *node, unsigned int raw_irq)
{
	struct udk_irq_info *info = node->ext->irq_info;
	unsigned int irq_index, irq_off, irq, arg_index;
	unsigned int irq_args[MAX_INTR_CELLS];
	unsigned int *irq_start;

	if (info == NULL) {
		return -EINVAL;
	}
	irq_start = info->irq_args;
	for (irq_index = 0; irq_index < info->irq_num; irq_index++) {
		irq_off = irq_index * info->irq_cells;
		if (info->irq_cells == GIC_IRQ_CELLS) {
			irq = fdt32_value(&irq_start[irq_off]);
		} else {
			irq = fdt32_value(&irq_start[irq_off + 1]);
		}
		if (irq == raw_irq) {
			break;
		}
	}
	if (irq_index >= info->irq_num) {
		udk_warn("invalid raw irq=%u\n", raw_irq);
		return -EINVAL;
	}
	mem_zero_a(irq_args);
	for (arg_index = 0; arg_index < info->irq_cells; arg_index++) {
		irq_args[arg_index] = fdt32_value(&irq_start[irq_off + arg_index]);
	}

	return udk_irq_kirq_create(node, info->irqctrl_id, irq_args, info->irq_cells);
}

static int irq_ext_hwirq_map(struct irq_ext_node *node, unsigned int raw_irq)
{
	int kirq;

	if (node->parent != &kernel_ext_node) {
		return (int)raw_irq;
	}
	kirq = irq_ext_kirq_map(node, raw_irq);
	if (kirq < 0) {
		udk_warn("kirq map failed:%s\n", strerror(-kirq));
	}

	return kirq;
}

static int irq_ext_hwirq_handler(int hw_irq, void *ctx)
{
	struct irq_ext_node *node = (struct irq_ext_node *)ctx;

	if (node == NULL) {
		return -EINVAL;
	}
	if (!is_irq_ext_op_valid(node->ext, irq_get_child_irq)) {
		return irq_ext_irq_map_lookup(node, hw_irq);
	}

	return irq_ext_get_child_irq(node, (unsigned int)hw_irq);
}

static inline
struct hwirq_handler *irq_ext_handler_find(struct irq_ext_node *node,
					   unsigned int hw_irq)
{
	return (struct hwirq_handler *)idr_find(&node->handler_idr, hw_irq);
}

static int irq_ext_handler_insert(struct irq_ext_node *node,
				  unsigned int hw_irq,
				  unsigned int irq,
				  irq_handler_t handler, void *ctx)
{
	struct hwirq_handler *hw_handler = NULL;
	int err;

	hw_handler = udk_malloc(sizeof(*hw_handler));
	if (hw_handler == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*hw_handler);
	hw_handler->handler = handler;
	hw_handler->irq = (int)irq;
	hw_handler->ctx = ctx;
	err = idr_set(&node->handler_idr, hw_handler, hw_irq);
	if (err < 0) {
		udk_warn("hardware irq handler insert failed\n");
		udk_free(hw_handler);
		return err;
	}

	return 0;
}

static void irq_ext_handler_destroy(struct irq_ext_node *node,
					  unsigned int hw_irq)
{
	struct hwirq_handler *hw_handler = NULL;

	hw_handler = (struct hwirq_handler *)idr_find(&node->handler_idr, hw_irq);
	if (hw_handler == NULL) {
		return;
	}
	irq_ext_mask(node, (unsigned int)hw_handler->irq);
	irq_ext_deactivate(node, (unsigned int)hw_handler->irq);
	if (node == &kernel_ext_node) {
		devhost_free_irq(hw_irq);
	}
	(void)idr_remove(&node->handler_idr, hw_irq);
	udk_free(hw_handler);
}

static int irq_ext_handler_init(struct irq_ext_node *node,
				unsigned int hw_irq,
				unsigned int irq,
				irq_handler_t handler, void *ctx)
{
	struct hwirq_handler *hw_handler = NULL;
	int err;

	err = irq_ext_handler_insert(node, hw_irq, irq, handler, ctx);
	if (err < 0) {
		udk_warn("irq ext handler insert failed: %s\n", strerror(-err));
		return err;
	}
	err = irq_ext_activate(node, hw_irq);
	if (err < 0) {
		udk_warn("irq ext activate irq failed: %s\n", strerror(-err));
		hw_handler = (struct hwirq_handler *)idr_remove(&node->handler_idr, hw_irq);
		udk_free(hw_handler);
		return err;
	}
	irq_ext_unmask(node, hw_irq);

	return 0;
}

static int uirq_handler_insert(struct uirq_entry *entry, irq_handler_t handler,
			       unsigned int flags, void *ctx)
{
	struct irq_ext_node *node = entry->ext_node;
	unsigned int hw_irq = entry->hw_irq;
	int err;

	irq_ext_lock(node);
	if (irq_ext_handler_find(node, hw_irq) != NULL) {
		udk_warn("uirq handler exists\n");
		irq_ext_unlock(node);
		return -EEXIST;
	}
	err = irq_ext_set_trigger(node, hw_irq, flags);
	if (err < 0) {
		udk_warn("irq ext set trigger failed: %s\n", strerror(-err));
		irq_ext_unlock(node);
		return err;
	}
	err = irq_ext_handler_init(node, hw_irq, entry->uirq, handler, ctx);
	if (err < 0) {
		udk_warn("irq ext handler init failed: %s\n", strerror(-err));
		irq_ext_unlock(node);
		return err;
	}
	irq_ext_unlock(node);

	return 0;
}

static void uirq_path_clean(struct uirq_entry *entry)
{
	int hw_irq, raw_irq;
	struct irq_ext_node *node = NULL;

	hw_irq = (int)entry->hw_irq;
	raw_irq = hw_irq;
	node = entry->ext_node;
	while (node != NULL) {
		irq_ext_lock(node);
		irq_ext_handler_destroy(node, (unsigned int)hw_irq);
		if (!idr_is_empty(&node->handler_idr) ||
		    node == &kernel_ext_node) {
			irq_ext_unlock(node);
			return;
		}
		irq_ext_unlock(node);
		raw_irq = irq_ext_get_parent_irq(node, (unsigned int)raw_irq);
		if (raw_irq < 0) {
			return;
		}
		if (!is_irq_ext_op_valid(node->ext, irq_get_child_irq)) {
			irq_ext_irq_map_remove(node, raw_irq);
		}
		hw_irq = irq_ext_hwirq_map(node, (unsigned int)raw_irq);
		if (hw_irq < 0) {
			return;
		}
		node = node->parent;
	}
}

/*
 * Map the child raw irq to the parent raw irq cyclically until reach the
 * root ext node(kernel_ext_node), and record the mapping relations in
 * the parent ext nodes' handler idr.
 * Return 0 if the mapping relation has been created in the middle parent node.
 * Return kirq when the mapping relation between kernel_ext_node and
 * its child ext node is created for the first time.
 */
static int irq_handle_path_build(struct irq_ext_node *start_node,
				 unsigned int start_raw_irq)
{
	int hw_irq, raw_irq, child_raw_irq, err;
	struct irq_ext_node *node = NULL;
	struct irq_ext_node *child = NULL;

	node = start_node;
	raw_irq = (int)start_raw_irq;
	while (node != &kernel_ext_node) {
		child_raw_irq = raw_irq;
		raw_irq = irq_ext_get_parent_irq(node, (unsigned int)raw_irq);
		if (raw_irq < 0) {
			udk_warn("irq ext get parent raw irq failed: %s\n",
				 strerror(-raw_irq));
			return raw_irq;
		}
		hw_irq = irq_ext_hwirq_map(node, (unsigned int)raw_irq);
		if (hw_irq < 0) {
			udk_warn("irq ext map hardware irq failed: %s\n",
				 strerror(-hw_irq));
			return hw_irq;
		}
		child = node;
		node = node->parent;
		irq_ext_lock(node);
		if (irq_ext_handler_find(node, (unsigned int)hw_irq) != NULL) {
			irq_ext_unlock(node);
			return 0;
		}
		err = irq_ext_handler_init(node, (unsigned int)hw_irq,
					   (unsigned int)raw_irq,
					   &irq_ext_hwirq_handler, child);
		if (err < 0) {
			irq_ext_unlock(node);
			return -EFAULT;
		}
		if (!is_irq_ext_op_valid(child->ext, irq_get_child_irq)) {
			err = irq_ext_irq_map_insert(child, raw_irq, child_raw_irq);
			if (err < 0) {
				irq_ext_handler_destroy(node, (unsigned int)hw_irq);
				irq_ext_unlock(node);
				return err;
			}
		}
		irq_ext_unlock(node);
	}

	return hw_irq;
}

static int kirq_handler(int kirq, void *ctx)
{
	struct hwirq_handler *hw_handler = NULL;
	struct irq_ext_node *node = NULL;
	int hw_irq;
	int ret;

	UNUSED(ctx);
	hw_irq = kirq;
	node = &kernel_ext_node;
	while (node != NULL) {
		irq_ext_mask(node, (unsigned int)hw_irq);
		hw_handler = irq_ext_handler_find(node, (unsigned int)hw_irq);
		if (hw_handler == NULL) {
			ret = -ENOENT;
			break;
		}
		ret = hw_handler->handler(hw_handler->irq, hw_handler->ctx);
		if (ret < 0) {
			udk_warn("hardware irq handler failed: %s\n",
				 strerror(-ret));
			break;
		}
		if (hw_handler->handler != &irq_ext_hwirq_handler) {
			break;
		}
		hw_irq = ret;
		node = (struct irq_ext_node *)hw_handler->ctx;
	}
	while (node != &kernel_ext_node) {
		irq_ext_eoi(node, (unsigned int)hw_irq);
		irq_ext_unmask(node, (unsigned int)hw_irq);
		hw_irq = irq_ext_get_parent_irq(node, (unsigned int)hw_irq);
		if (hw_irq < 0) {
			/* get_parent_irq should not fail at this point */
			udk_warn("irq get parent irq failed: %s\n",
				 strerror(-hw_irq));
			return hw_irq;
		}
		node = node->parent;
	}

	return ret;
}

static int kirq_request(unsigned int kirq, const char *devname, void *ctx)
{
	int err;

	err = devhost_request_threaded_irq(kirq, &kirq_handler, 0, devname, ctx);
	if (err < 0) {
		return -hmerrno2posix(err);
	}
	devhost_enable_irq(kirq);

	return 0;
}

int udk_irq_ext_uirq_request(unsigned int uirq, irq_handler_t handler,
			     unsigned int flags, const char *devname,
			     void *ctx)
{
	struct irq_ext_node *node = NULL;
	struct uirq_entry *entry = NULL;
	int hw_irq, kirq, err;

	entry = uirq_entry_lookup(uirq);
	if (entry == NULL) {
		udk_error("invalid uirq\n");
		return -EINVAL;
	}
	err = uirq_handler_insert(entry, handler, flags, ctx);
	if (err < 0) {
		udk_warn("uirq handler insert failed: %s\n", strerror(-err));
		return err;
	}
	node = entry->ext_node;
	hw_irq = (int)entry->hw_irq;
	if (node != &kernel_ext_node) {
		kirq = irq_handle_path_build(node, (unsigned int)hw_irq);
		if (kirq < 0) {
			udk_error("irq handle path build failed\n");
			uirq_path_clean(entry);
			return kirq;
		} else if (kirq == 0) {
			return 0;
		}
	} else {
		kirq = hw_irq;
	}
	err = kirq_request((unsigned int)kirq, devname, ctx);
	if (err < 0) {
		uirq_path_clean(entry);
	}

	return err;
}

void udk_irq_ext_uirq_unrequest(unsigned int uirq)
{
	struct uirq_entry *entry = NULL;

	entry = uirq_entry_lookup(uirq);
	if (entry == NULL) {
		udk_error("invalid uirq\n");
		return;
	}
	uirq_path_clean(entry);
}

void udk_irq_ext_uirq_enable(unsigned int uirq)
{
	struct uirq_entry *entry = NULL;

	entry = uirq_entry_lookup(uirq);
	if (entry == NULL) {
		udk_warn("invalid uirq\n");
		return;
	}
	if (entry->ext_node == &kernel_ext_node) {
		devhost_enable_irq(entry->hw_irq);
	} else {
		irq_ext_unmask(entry->ext_node, entry->hw_irq);
	}
}

void udk_irq_ext_uirq_disable(unsigned int uirq)
{
	struct uirq_entry *entry = NULL;

	entry = uirq_entry_lookup(uirq);
	if (entry == NULL) {
		udk_warn("invalid uirq\n");
		return;
	}
	if (entry->ext_node == &kernel_ext_node) {
		devhost_disable_irq(entry->hw_irq);
	} else {
		irq_ext_mask(entry->ext_node, entry->hw_irq);
	}
}

static int udk_irq_ext_init(void)
{
	udk_mutex_init(&irq_ext_mgr.lock);
	idr_init(&uirq_mgr.uirq_idr);
	udk_mutex_init(&uirq_mgr.lock);
	idr_init(&kernel_ext_node.handler_idr);
	udk_mutex_init(&kernel_ext_node.lock);

	return 0;
}
udk_init_call(udk_irq_ext_init);
