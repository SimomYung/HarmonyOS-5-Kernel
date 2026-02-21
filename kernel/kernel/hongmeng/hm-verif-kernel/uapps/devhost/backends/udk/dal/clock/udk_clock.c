/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: UDK clock framework
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 07 20:21:54 2021
 */

#include <stdlib.h>
#include <string.h>

#include <udk/log.h>
#include <udk/sync.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/clock.h>
#include <udk/bitops.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>
#include <internal/init.h>
#include <libalgo/rbtree.h>

enum udk_clock_status {
	UDK_CLOCK_INVALID = -1,
	UDK_CLOCK_DISABLED = 0,
	UDK_CLOCK_ENABLED = 1,
};

struct udk_clock_rbt {
	struct rbt_tree tree;
	struct udk_mutex mutex;
};

struct udk_clock {
	unsigned long rate;
	struct udk_clock *parent;
	const struct udk_clock_desc *desc;
	const struct udk_clock_hw_ops *hw_ops;
	struct rbt_node node;
	struct dlist_node children;
	struct dlist_node child_node;
	enum udk_clock_status status;
};

static struct udk_clock_rbt udk_clock_root;

static inline void clock_lock(void)
{
	udk_mutex_lock(&udk_clock_root.mutex);
}

static inline void clock_unlock(void)
{
	udk_mutex_unlock(&udk_clock_root.mutex);
}

static inline bool clock_is_gate(const struct udk_clock *clock)
{
	return clock->desc->gate != NULL;
}

static inline bool clock_is_mux(const struct udk_clock *clock)
{
	return clock->desc->mux != NULL;
}

static inline bool clock_is_div(const struct udk_clock *clock)
{
	return clock->desc->div != NULL;
}

static inline bool clock_is_enabled(const struct udk_clock *clock)
{
	return clock->status == UDK_CLOCK_ENABLED;
}

static inline bool clock_is_disabled(const struct udk_clock *clock)
{
	return clock->status == UDK_CLOCK_DISABLED;
}

static inline bool clock_validity_check(const struct udk_clock *clock)
{
	return ((clock != NULL) && (clock->desc != NULL) && (clock->hw_ops != NULL));
}

static inline bool clock_name_check(const char *name)
{
	return !(name == NULL || name[0] == '\0');
}

static inline const char *clock_name_of(const struct udk_clock *clock)
{
	return clock->desc->name;
}

#define CLOCK_REG_DEFINE(type)				\
static inline struct udk_clock_reg *			\
clock_##type##_reg_of(const struct udk_clock *clock)	\
{							\
	return &clock->desc->type->reg;			\
}
CLOCK_REG_DEFINE(gate)
CLOCK_REG_DEFINE(mux)
CLOCK_REG_DEFINE(div)

static void clock_gate_enable(const struct udk_clock *clock)
{
	const struct udk_clock_hw_ops *hw = clock->hw_ops;
	struct udk_clock_reg *reg = clock_gate_reg_of(clock);
	unsigned long val, tmp;

	val = hw->reg_read(reg->offset);
	tmp = udk_set_bit(val, reg->shift);
	if (tmp != val) {
		hw->reg_write(reg->offset, tmp);
	}
}

static void clock_gate_disable(const struct udk_clock *clock)
{
	const struct udk_clock_hw_ops *hw = clock->hw_ops;
	struct udk_clock_reg *reg = clock_gate_reg_of(clock);
	unsigned long val, tmp;

	val = hw->reg_read(reg->offset);
	tmp = udk_clear_bit(val, reg->shift);
	if (tmp != val) {
		hw->reg_write(reg->offset, tmp);
	}
}

static bool clock_gate_is_enabled(const struct udk_clock *clock)
{
	const struct udk_clock_hw_ops *hw = clock->hw_ops;
	struct udk_clock_reg *reg = clock_gate_reg_of(clock);
	unsigned long val;

	val = hw->reg_read(reg->offset);

	return udk_test_bit(val, reg->shift);
}

static unsigned long clock_reg_get_bits(const struct udk_clock_hw_ops *hw,
					const struct udk_clock_reg *reg)
{
	unsigned long val;

	val = hw->reg_read(reg->offset);

	return udk_get_bits(val, reg->mask, reg->shift);
}

static void clock_reg_set_bits(const struct udk_clock_hw_ops *hw,
			       struct udk_clock_reg *reg, unsigned long value)
{
	unsigned long val;

	val = hw->reg_read(reg->offset);
	val = udk_unmask_bits(val, reg->mask, reg->shift);
	val |= UDK_BIT_LS(value, reg->shift);
	hw->reg_write(reg->offset, val);
}

static int clock_desc_check(const struct udk_clock_desc *desc)
{
	if (desc == NULL) {
		udk_warn("invalid clock desc\n");
		return -EINVAL;
	}
	if (!clock_name_check(desc->name)) {
		udk_warn("invalid clock name\n");
		return -EINVAL;
	}

	return 0;
}

static int clock_hw_ops_check(const struct udk_clock_hw_ops *hw_ops)
{
	if (hw_ops == NULL) {
		udk_warn("invalid clock hw ops\n");
		return -EINVAL;
	}
	if (hw_ops->reg_read == NULL || hw_ops->reg_write == NULL) {
		udk_warn("invalid reg_read/write callback\n");
		return -EINVAL;
	}

	return 0;
}

static struct udk_clock *clock_lookup(const char *name,
				      struct rbt_node **parent,
				      bool *is_right)
{
	struct rbt_tree *tree = &udk_clock_root.tree;
	struct rbt_node *p = tree->root;
	struct rbt_node *q = NULL;
	bool right = false;

	while (p != NULL) {
		int res;
		struct udk_clock *pos = rbt_entry(p, struct udk_clock, node);

		res = strcmp(name, clock_name_of(pos));
		if (res == 0) {
			return pos;
		}
		if (res > 0) {
			if (p->rbt_right == NULL) {
				q = p;
				right = true;
			}
			p = p->rbt_right;
		} else {
			if (p->rbt_left == NULL) {
				q = p;
				right = false;
			}
			p = p->rbt_left;
		}
	}
	if (parent != NULL) {
		*parent = q;
	}
	if (is_right != NULL) {
		*is_right = right;
	}

	return NULL;
}

static int clock_add(struct udk_clock *clock)
{
	const char *name = clock_name_of(clock);
	bool is_right = false;
	struct rbt_node *parent = NULL;
	struct udk_clock *tmp = NULL;

	tmp = clock_lookup(name, &parent, &is_right);
	if (tmp != NULL) {
		return -EEXIST;
	}
	rbt_insert(&udk_clock_root.tree, &clock->node, parent, is_right);

	return 0;
}

static void clock_remove_from_parent(struct udk_clock *clock)
{
	struct udk_clock *pos = NULL;
	struct udk_clock *n = NULL;
	struct udk_clock *parent = clock->parent;

	dlist_for_each_entry_safe(pos, n, &parent->children,
				  struct udk_clock, child_node) {
		if (pos == clock) {
			dlist_delete(&pos->child_node);
			break;
		}
	}
}

static int clock_remove(struct udk_clock *clock)
{
	const char *name = clock_name_of(clock);
	struct udk_clock *tmp = NULL;
	struct udk_clock *pos = NULL;

	if (!clock_name_check(name)) {
		return -EINVAL;
	}
	tmp = clock_lookup(name, NULL, NULL);
	if (tmp != clock) {
		return -ENOENT;
	} else {
		rbt_remove(&udk_clock_root.tree, &clock->node);
		if (clock->parent != NULL) {
			clock_remove_from_parent(clock);
		}
		/* make all children be orphan */
		dlist_for_each_entry_safe(pos, tmp, &clock->children,
					  struct udk_clock, child_node) {
			dlist_delete(&pos->child_node);
			pos->parent = NULL;
			pos->rate = 0UL;
		}
	}

	return 0;
}

static inline unsigned int mux_get_parent_idx(const struct udk_clock *clock)
{
	return (unsigned int)clock_reg_get_bits(clock->hw_ops,
						clock_mux_reg_of(clock));
}

static inline void mux_set_parent_idx(struct udk_clock *clock,
				      unsigned int index)
{
	clock_reg_set_bits(clock->hw_ops, clock_mux_reg_of(clock),
			   (unsigned long)index);
}

static inline const char *clock_mux_parent_idx_to_name(const struct udk_clock_mux *mux,
						       unsigned int index)
{
	return (index >= mux->parent_num) ? NULL : mux->parents[index];
}

static int clock_mux_parent_name_to_idx(const struct udk_clock_mux *mux,
					const char *name, unsigned int *index)
{
	unsigned int idx;
	unsigned int num = mux->parent_num;
	const char * const *parents = mux->parents;

	for (idx = 0U; idx < num; idx++) {
		if (strcmp(parents[idx], name) == 0) {
			*index = idx;
			return 0;
		}
	}

	return -EINVAL;
}

static const char *clock_mux_get_parent_name(const struct udk_clock *clock)
{
	unsigned int index;

	if (clock->hw_ops->get_parent != NULL) {
		index = clock->hw_ops->get_parent(clock->desc);
	} else {
		/* default get_parent for mux clock */
		index = mux_get_parent_idx(clock);
	}

	return clock_mux_parent_idx_to_name(clock->desc->mux, index);
}

static void clock_set_parent(struct udk_clock *clock, struct udk_clock *parent)
{
	if (clock_is_disabled(parent)) {
		clock->status = UDK_CLOCK_DISABLED;
	}
	dlist_insert(&parent->children, &clock->child_node);
	clock->parent = parent;
}

static void clock_parent_init(struct udk_clock *clock,
			      const struct udk_clock_desc *desc)
{
	struct udk_clock *parent = NULL;
	const char *name = NULL;

	if (desc->mux != NULL) {
		name = clock_mux_get_parent_name(clock);
	} else {
		name = desc->parent;
	}
	if (name == NULL) {
		return;
	}
	/* the parent must be registered before all children if it exists. */
	parent = clock_lookup(name, NULL, NULL);
	if (parent != NULL) {
		clock_set_parent(clock, parent);
	}
}

static inline unsigned int div_get_val(const struct udk_clock *clock)
{
	return clock_reg_get_bits(clock->hw_ops, clock_div_reg_of(clock));
}

static inline void div_set_val(const struct udk_clock *clock, unsigned int val)
{
	clock_reg_set_bits(clock->hw_ops, clock_div_reg_of(clock),
			   (unsigned long)val);
}

static inline unsigned int power2(unsigned int val)
{
	return (1U << val);
}

static inline bool is_power2(unsigned int val)
{
	return (val != 0U) && ((val & (val - 1U)) == 0U);
}

static inline unsigned int logarithm2(unsigned int val)
{
	unsigned int lsb = 0U;
	unsigned int temp = val;

	while (temp > 1U) {
		temp = temp >> 1U;
		lsb++;
	}

	return lsb;
}

static inline unsigned int max_at_zero_ratio(const struct udk_clock_div *div,
					     unsigned int val)
{
	return (val != 0U) ? val : (div->reg.mask + 1U);
}

static inline unsigned int max_at_zero_val(const struct udk_clock_div *div,
					   unsigned int ratio)
{
	return (ratio == (div->reg.mask + 1U)) ? 0U : ratio;
}

static int clock_div_set_ratio(const struct udk_clock *clock,
			       unsigned int ratio)
{
	unsigned int val = 0U;
	struct udk_clock_div *div = NULL;

	div = clock->desc->div;
	switch (div->method) {
	case UDK_CLOCK_DIV_PARALLEL:
		val = ratio;
		break;
	case UDK_CLOCK_DIV_POWER_OF_TWO:
		if (!is_power2(ratio)) {
			return -EINVAL;
		}
		val = logarithm2(ratio);
		break;
	case UDK_CLOCK_DIV_MAX_AT_ZERO:
		val = max_at_zero_val(div, ratio);
		break;
	default:
		udk_error("unknown div method: %d\n", (int)div->method);
		return -EINVAL;
	}
	/* the val can't be greater than the maximum value of the register */
	val = (val < div->reg.mask) ? val : div->reg.mask;
	div_set_val(clock, val);

	return 0;
}

static unsigned int clock_div_get_ratio(const struct udk_clock *clock)
{
	unsigned int val = 0U;
	unsigned int ratio = 0U;
	struct udk_clock_div *div = NULL;

	div = clock->desc->div;
	val = div_get_val(clock);
	/* divider ratio need calculate with the val */
	switch (div->method) {
	case UDK_CLOCK_DIV_PARALLEL:
		ratio = val;
		break;
	case UDK_CLOCK_DIV_POWER_OF_TWO:
		ratio = power2(val);
		break;
	case UDK_CLOCK_DIV_MAX_AT_ZERO:
		ratio = max_at_zero_ratio(div, val);
		break;
	default:
		udk_error("unknown div method: %d\n", (int)div->method);
		break;
	}

	return ratio;
}

static unsigned long clock_div_recalc_rate(const struct udk_clock *clock,
					   unsigned long parent_rate)
{
	unsigned int ratio;

	ratio = clock_div_get_ratio(clock);
	if (ratio == 0U) {
		/* keep parent_rate for zero divisor ratio */
		ratio = 1UL;
	}

	return parent_rate / ratio;
}

static void clock_rate_recalc(struct udk_clock *clock,
			      unsigned long parent_rate)
{
	const struct udk_clock_hw_ops *hw = clock->hw_ops;

	if (hw->recalc_rate == NULL) {
		/* divider clock can use default recalc_rate */
		if (clock_is_div(clock)) {
			clock->rate = clock_div_recalc_rate(clock, parent_rate);
		} else {
			clock->rate = parent_rate;
		}
	} else {
		clock->rate = hw->recalc_rate(clock->desc, parent_rate);
	}
}

static void clock_rate_update(struct udk_clock *clock,
			      unsigned long parent_rate)
{
	struct udk_clock *pos = NULL;

	clock_rate_recalc(clock, parent_rate);
	dlist_for_each_entry(pos, &clock->children,
			     struct udk_clock, child_node) {
		clock_rate_update(pos, clock->rate);
	}
}

static void clock_rate_init(struct udk_clock *clock, unsigned long rate)
{
	struct udk_clock *parent = clock->parent;

	if (parent == NULL) {
		clock->rate = rate;
	} else {
		clock_rate_recalc(clock, parent->rate);
	}
}

static void clock_init(struct udk_clock *clock, const struct udk_clock_desc *desc,
		       const struct udk_clock_hw_ops *hw_ops)
{
	clock->desc = desc;
	clock->hw_ops = hw_ops;
	clock->status = UDK_CLOCK_INVALID;
	dlist_init(&clock->children);
	clock_parent_init(clock, desc);
	clock_rate_init(clock, desc->rate);
}

static struct udk_clock *clock_create(const struct udk_clock_desc *desc,
				      const struct udk_clock_hw_ops *hw_ops)
{
	struct udk_clock *clock = NULL;

	clock = (struct udk_clock *)udk_malloc(sizeof(struct udk_clock));
	if (clock == NULL) {
		udk_error("malloc clock failed\n");
		return NULL;
	}
	mem_zero_s(*clock);
	clock_init(clock, desc, hw_ops);

	return clock;
}

static void clock_destroy(struct udk_clock *clock)
{
	clock->desc = NULL;
	clock->hw_ops = NULL;
	udk_free(clock);
}

struct udk_clock *udk_clock_register(const struct udk_clock_desc *desc,
				     const struct udk_clock_hw_ops *hw_ops)
{
	int err;
	struct udk_clock *clock = NULL;

	err = clock_desc_check(desc);
	if (err < 0) {
		udk_warn("invalid clock desc\n");
		return NULL;
	}
	err = clock_hw_ops_check(hw_ops);
	if (err < 0) {
		udk_warn("invalid clock hw_ops\n");
		return NULL;
	}
	clock = clock_create(desc, hw_ops);
	if (clock == NULL) {
		udk_error("create new clock failed\n");
		return NULL;
	}
	clock_lock();
	err = clock_add(clock);
	if (err < 0) {
		udk_error("add clock failed, err: %d\n", err);
		clock_destroy(clock);
		clock_unlock();
		return NULL;
	}
	clock_unlock();

	return clock;
}

int udk_clock_unregister(struct udk_clock *clock)
{
	int err;

	if ((clock == NULL) || (clock->desc == NULL)) {
		udk_warn("invalid input clock\n");
		return -EINVAL;
	}
	clock_lock();
	err = clock_remove(clock);
	if (err < 0) {
		udk_error("remove clock failed, err: %d\n", err);
		clock_unlock();
		return err;
	}
	clock_destroy(clock);
	clock_unlock();

	return 0;
}

struct udk_clock *udk_clock_lookup(const char *name)
{
	struct udk_clock *clock = NULL;

	if (!clock_name_check(name)) {
		udk_warn("invalid clock name\n");
		return NULL;
	}
	clock_lock();
	clock = clock_lookup(name, NULL, NULL);
	if (clock == NULL) {
		udk_warn("clock '%s' not found\n", name);
	}
	clock_unlock();

	return clock;
}

static void clock_enable(struct udk_clock *clock)
{
	if (clock->parent != NULL) {
		clock_enable(clock->parent);
	}
	/* non-gate clock is enabled by default */
	if (!clock_is_gate(clock)) {
		clock->status = UDK_CLOCK_ENABLED;
		return;
	}
	if (clock_is_enabled(clock)) {
		return;
	}
	clock_gate_enable(clock);
	clock->status = UDK_CLOCK_ENABLED;
}

int udk_clock_enable(struct udk_clock *clock)
{
	if (!clock_validity_check(clock)) {
		udk_error("invalid clock members\n");
		return -EINVAL;
	}
	clock_lock();
	clock_enable(clock);
	clock_unlock();

	return 0;
}

static void do_clock_disable(struct udk_clock *clock)
{
	if (clock_is_gate(clock)) {
		clock_gate_disable(clock);
	}
	clock->status = UDK_CLOCK_DISABLED;
}

static void clock_disable(struct udk_clock *clock)
{
	struct udk_clock *pos = NULL;

	if (clock_is_disabled(clock)) {
		return;
	}
	/* disable children clock recursively */
	dlist_for_each_entry(pos, &clock->children,
			     struct udk_clock, child_node) {
		clock_disable(pos);
	}
	do_clock_disable(clock);
}

static void clock_disable_parent(struct udk_clock *parent)
{
	struct udk_clock *pos = NULL;

	if (parent == NULL) {
		return;
	}
	/* disable parent if all its children had been disabled */
	dlist_for_each_entry(pos, &parent->children,
			     struct udk_clock, child_node) {
		if (!clock_is_disabled(pos)) {
			return;
		}
	}
	do_clock_disable(parent);
	/* disable parent clock recursively */
	clock_disable_parent(parent->parent);
}

int udk_clock_disable(struct udk_clock *clock)
{
	if (!clock_validity_check(clock)) {
		udk_warn("invalid clock\n");
		return -EINVAL;
	}
	if (!clock_is_gate(clock)) {
		udk_warn("can not disable non-gate clock\n");
		return -EINVAL;
	}
	clock_lock();
	clock_disable(clock);
	clock_disable_parent(clock->parent);
	clock_unlock();

	return 0;
}

bool udk_clock_is_enabled(struct udk_clock *clock)
{
	bool enabled = false;

	if (!clock_validity_check(clock)) {
		udk_warn("invalid clock\n");
		return false;
	}
	clock_lock();
	if (clock_is_gate(clock)) {
		enabled = clock_gate_is_enabled(clock);
	} else {
		enabled = !clock_is_disabled(clock);
	}
	if (enabled) {
		clock->status = UDK_CLOCK_ENABLED;
	} else {
		clock->status = UDK_CLOCK_DISABLED;
	}
	clock_unlock();

	return enabled;
}

struct udk_clock *udk_clock_get_parent(struct udk_clock *clock)
{
	struct udk_clock *parent = NULL;

	if (!clock_validity_check(clock)) {
		udk_warn("invalid clock\n");
		return NULL;
	}
	clock_lock();
	parent = clock->parent;
	clock_unlock();

	return parent;
}

static int clock_mux_set_parent(struct udk_clock *clock,
				struct udk_clock *parent)
{
	int err = 0;
	unsigned int index;
	const char *name = NULL;
	struct udk_clock_mux *mux = NULL;

	mux = clock->desc->mux;
	name = clock_name_of(parent);
	err = clock_mux_parent_name_to_idx(mux, name, &index);
	if (err < 0) {
		udk_error("could not found mux parent index\n");
		return err;
	}
	if (clock->hw_ops->set_parent != NULL) {
		err = clock->hw_ops->set_parent(clock->desc, index);
		if (err < 0) {
			udk_error("driver set_parent failed, err: %d\n", err);
			return err;
		}
	} else {
		/* default set_parent for mux clock */
		mux_set_parent_idx(clock, index);
	}

	return 0;
}

static void clock_reparent(struct udk_clock *clock, struct udk_clock *parent)
{
	/* remove clock from children list of old parent */
	if (clock->parent != NULL) {
		clock_remove_from_parent(clock);
	}
	/* add clock to children list of new parent */
	clock_set_parent(clock, parent);
	/* update self and all children clock rate */
	clock_rate_update(clock, parent->rate);
}

int udk_clock_set_parent(struct udk_clock *clock, struct udk_clock *parent)
{
	int err;

	if (!clock_validity_check(clock) || !clock_validity_check(parent)) {
		udk_warn("input invalid clock\n");
		return -EINVAL;
	}
	clock_lock();
	if (clock->parent == parent) {
		clock_unlock();
		return 0;
	}
	if (!clock_is_mux(clock)) {
		udk_error("could not change clock parent\n");
		clock_unlock();
		return -EINVAL;
	}
	err = clock_mux_set_parent(clock, parent);
	if (err < 0) {
		udk_error("mux clock reset parent failed\n");
		clock_unlock();
		return err;
	}
	clock_reparent(clock, parent);
	clock_unlock();

	return 0;
}

unsigned long udk_clock_get_rate(struct udk_clock *clock)
{
	unsigned long rate = 0UL;

	if (clock == NULL) {
		udk_error("invalid udk clock\n");
		return 0;
	}
	clock_lock();
	rate = clock->rate;
	clock_unlock();

	return rate;
}

static int clock_div_set_rate(const struct udk_clock *clock, unsigned long rate,
			      unsigned long parent_rate)
{
	int err;
	unsigned int ratio;

	ratio = (unsigned int)(parent_rate / rate);
	if (ratio == 0U) {
		/* rate of divider can not be greater than parent */
		return -EINVAL;
	}
	err = clock_div_set_ratio(clock, ratio);
	if (err < 0) {
		udk_error("divider set ratio failed: %d\n", err);
		return err;
	}

	return 0;
}

static int clock_default_set_rate(const struct udk_clock *clock,
				  unsigned long rate, unsigned long parent_rate)
{
	if (clock_is_div(clock)) {
		return clock_div_set_rate(clock, rate, parent_rate);
	}

	return -ENOSYS;
}

static int clock_set_rate(struct udk_clock *clock, unsigned long rate)
{
	int err;
	unsigned long parent_rate = 0UL;

	if (clock->rate == rate) {
		return 0;
	}
	if (clock->parent != NULL) {
		parent_rate = clock->parent->rate;
	}
	if (clock->hw_ops->set_rate != NULL) {
		err = clock->hw_ops->set_rate(clock->desc, rate, parent_rate);
	} else {
		err = clock_default_set_rate(clock, rate, parent_rate);
	}
	if (err < 0) {
		udk_error("set rate failed, err: %d\n", err);
		return err;
	}
	clock_rate_update(clock, parent_rate);

	return 0;
}

int udk_clock_set_rate(struct udk_clock *clock, unsigned long rate)
{
	int err;

	if (!clock_validity_check(clock)) {
		udk_warn("invalid clock\n");
		return -EINVAL;
	}
	if (rate == 0UL) {
		udk_warn("invalid rate\n");
		return -EINVAL;
	}
	clock_lock();
	err = clock_set_rate(clock, rate);
	clock_unlock();

	return err;
}

static int udk_clock_init(void)
{
	rbt_tree_init(&udk_clock_root.tree);
	udk_mutex_init(&udk_clock_root.mutex);

	return 0;
}
udk_init_call(udk_clock_init);
