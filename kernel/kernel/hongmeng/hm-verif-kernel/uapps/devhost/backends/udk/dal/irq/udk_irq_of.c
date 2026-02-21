/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: UDK irq dts praser
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 11 20:10:10 2021
 */
#include <udk/of.h>
#include <udk/irq.h>
#include <udk/log.h>
#include <udk/types.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/irq_ext.h>
#include <udk/securec.h>
#include <internal/of.h>
#include <internal/irq.h>
#include <libstrict/strict.h>

#define DEFAULT_INTR_CELLS	3U

/*
 * Because the hierarchy of the nodes in the interrupt tree
 * might not match the devicetree, the interrupt-parent
 * property is available to make the definition of an interrupt
 * parent explicit. The value is the phandle to the
 * interrupt parent. If this property is missing from a device,
 * its interrupt parent is assumed to be its devicetree parent.
 */
static int of_find_irq_parent_node(struct udk_dt_node *node,
				   struct udk_dt_node **parent_node)
{
	int ret;
	uint32_t parent_phandle = 0;
	struct udk_dt_node *np = NULL;

	ret = udk_of_prop_read_u32(node, "interrupt-parent", &parent_phandle);
	if (ret != 0) {
		if (node->parent != NULL) {
			*parent_node = node->parent;
			return 0;
		} else {
			*parent_node = NULL;
			return -ENOENT;
		}
	}
	np = udk_of_find_node_by_phandle(node, parent_phandle);
	if (np == NULL) {
		*parent_node = NULL;
		return -ENOENT;
	}
	*parent_node = np;

	return 0;
}

static bool is_irq_cells_valid(unsigned int irq_cells)
{
	return (irq_cells > 0 && irq_cells <= MAX_INTR_CELLS);
}

static int of_parse_interrupt_cells(const struct udk_dt_node *node,
				    const struct udk_dt_node *parent_node,
				    unsigned int *irq_cells)
{
	uint32_t cells = 0;
	int ret;

	/*
	 * Currently, udk_create_mapping can only map the irqs whose
	 * parent are GIC, and the GIC's #interrupt-cells is 3.
	 * The child irqs of ITS have to define their interrupt-parent_node
	 * as GIC, while ITS's #interrupt-cells is 4.
	 * To resolve the mismatch between #interrupt-cells and interrupt-parent,
	 * irq_of_info supports child node to define their own #interrupt-cells
	 * whose priority is higher than the parent's #interrupt-cells.
	 */
	ret = udk_of_prop_read_u32(node, "#interrupt-cells", &cells);
	if (ret == 0 && is_irq_cells_valid(cells)) {
		*irq_cells = cells;
		return 0;
	}
	ret = udk_of_prop_read_u32(parent_node, "#interrupt-cells", &cells);
	if (ret < 0) {
		udk_info("set irq cells as default val\n");
		*irq_cells = DEFAULT_INTR_CELLS;
		return 0;
	}
	if (!is_irq_cells_valid(cells)) {
		udk_error("#interrupt-cells val=%u is invalid\n", cells);
		return -EINVAL;
	}
	*irq_cells = cells;

	return 0;
}

static inline bool is_irq_prop_valid(const struct udk_dt_prop *irq_prop,
				     uint32_t irq_cells)
{
	return irq_prop->length % (sizeof(uint32_t) * irq_cells) == 0;
}

static inline unsigned int irq_num_calc(const struct udk_dt_prop *irq_prop,
					uint32_t irq_cells)
{
	return (unsigned int)(irq_prop->length / (sizeof(uint32_t) * irq_cells));
}

static void convert_ldk_irq_cells_to_hm(unsigned int *irq_args, unsigned int irq_cells)
{
#define LDK_IRQ_CELL_SIZE		3U
/* ldk irq cells: [is_percpu_irq, irq_no, trigger_type] */
#define LDK_IRQ_IS_PER_CPU_IDX		0U
#define LDK_IRQ_NO_IDX			1U
#define LDK_IRQ_TRIGGER_TYPE_IDX	2U
#define LDK_IRQ_TYPE_EDGE_BOTH		3U

/* hm irq cells: [hw_irq_no, irq_config, irq_affinity] */
#define HM_IRQ_HW_NO_IDX	0U
#define HM_IRQ_CONFIG_IDX	1U
#define HM_IRQ_AFFINITY_IDX	2U
#define HM_IRQ_OFFSET		32U
#define HM_IRQ_PRIORITY		0xE0
#define HM_IRQ_AFFINITY		0xFFFFFFFF
#define HM_IRQ_TYPE_EDGE_MASK	0x02000000

	/*
	 * only support spi, following two conditions are required.
	 * 1) ldk irq cells is 3
	 * 2) the 1st cell value is 0, 0 means non-percpu interrupt
	 */
	if (irq_cells != LDK_IRQ_CELL_SIZE) {
		return;
	}
	if (irq_args[LDK_IRQ_IS_PER_CPU_IDX] != 0U) {
		return;
	}

	irq_args[HM_IRQ_HW_NO_IDX] = irq_args[LDK_IRQ_NO_IDX] + HM_IRQ_OFFSET;
	irq_args[HM_IRQ_CONFIG_IDX] = HM_IRQ_PRIORITY;
	if ((irq_args[LDK_IRQ_TRIGGER_TYPE_IDX] & LDK_IRQ_TYPE_EDGE_BOTH) != 0U) {
		irq_args[HM_IRQ_CONFIG_IDX] |= HM_IRQ_TYPE_EDGE_MASK;
	}
	irq_args[HM_IRQ_AFFINITY_IDX] = HM_IRQ_AFFINITY;
}

/*
 * Get <interrupts> information from dts.
 * The <interrupts> is a two-dimensional array.
 * The ararry's each raw represents a set of irq args,
 * and its column number equals to "irq_cells"
 * For example:
 * <interrupts> = <a1 b1 c1>,
 *		  <a2 b2 c2>,
 *		  <a3 b3 c3>;
 * This <interrupts>'s irq_cells = 3.
 * if want to get <a2, b2, c2>, "index" = 1.
 */
static int of_parse_interrupts_args(const struct udk_dt_node *node,
				    unsigned int index, unsigned int irq_cells,
				    unsigned int *irq_args)
{
	struct udk_dt_prop *irq_prop = NULL;
	uint32_t *irq_start = NULL;
	uint32_t *cellp = NULL;
	uint32_t irq_num = 0;
	unsigned int i;

	irq_prop = udk_of_find_property(node, "interrupts");
	if (irq_prop == NULL || irq_prop->value == NULL) {
		udk_error("find <interrupts> property failed\n");
		return -ENOENT;
	}
	if (!is_irq_prop_valid(irq_prop, irq_cells)) {
		return -EFAULT;
	}
	irq_num = irq_num_calc(irq_prop, irq_cells);
	if (index >= irq_num) {
		udk_error("interrupt index=%u is out of node=%s range\n",
			  index, node->full_name);
		return -ENOENT;
	}
	irq_start = (uint32_t *)irq_prop->value;
	cellp = &irq_start[index * irq_cells];
	for (i = 0U; i < irq_cells; i++) {
		irq_args[i] = fdt32_value(&cellp[i]);
	}

	/*
	 * in LDK+UDK proxy mode, dtb is passed from LDK,
	 * so we need to convert ldk irq cells to hm.
	 */
	convert_ldk_irq_cells_to_hm(irq_args, irq_cells);

	return 0;
}

/*
 * Prefer irqctrl_id property as interrupt-controller id if the
 * property exists, otherwise use node offset.
 * Keep consistent with the kernel irqctrl logic.
 */
static unsigned int of_parse_irqctrl_id(const struct udk_dt_node *node)
{
	unsigned int irqctrl_id = 0U;
	int ret;

	ret = udk_of_prop_read_u32(node, "irqctrl_id", &irqctrl_id);
	if (ret != 0) {
		irqctrl_id = node->offset;
	}

	return irqctrl_id;
}

static int of_parse_kirq(struct udk_dt_node *node,
			 const struct udk_dt_node *parent_node,
			 unsigned int index)
{
	uint32_t irq_args[MAX_INTR_CELLS];
	unsigned int irqctrl_id = 0;
	unsigned int irq_cells = 0;
	int err;

	irqctrl_id = of_parse_irqctrl_id(parent_node);
	err = of_parse_interrupt_cells(node, parent_node, &irq_cells);
	if (err < 0) {
		udk_error("node=%s parse #interrupt-cells failed\n", node->full_name);
		return err;
	}
	mem_zero_a(irq_args);
	err = of_parse_interrupts_args(node, index, irq_cells, irq_args);
	if (err < 0) {
		udk_error("node=%s parse interrupts args failed\n", node->full_name);
		return err;
	}

	return udk_irq_kirq_create(node, irqctrl_id, irq_args, irq_cells);
}

#ifndef CONFIG_UDK_IRQ_EXT

int udk_irq_of_uirq(struct udk_dt_node *node, unsigned int index)
{
	struct udk_dt_node *parent_node = NULL;
	int err;

	if (node == NULL) {
		return -EINVAL;
	}
	err = of_find_irq_parent_node(node, &parent_node);
	if (err < 0 || parent_node == NULL) {
		udk_error("find irq parent node failed\n");
		return -ENOENT;
	}

	/* when there are no irq extensions, uirq equals to kirq */
	return of_parse_kirq(node, parent_node, index);
}

#else

static bool of_is_node_in_kernel(struct udk_dt_node *node)
{
	struct udk_dt_node *root = NULL;
	struct udk_dt_node *kernel = NULL;
	struct udk_dt_node *iter_node = NULL;

	root = udk_of_find_root_node(node);
	kernel = udk_of_find_node_by_fullname(root, "/hmmicrokernel");
	for_each_of_hmnode(kernel, iter_node) {
		if (iter_node == kernel->sibling) {
			break;
		}
		if (iter_node->phandle == node->phandle) {
			return true;
		}
	}

	return false;
}

static int of_parse_raw_irq(const struct udk_dt_node *node,
			    const struct udk_dt_node *parent_node,
			    unsigned int index)
{
	unsigned int irq_args[MAX_INTR_CELLS];
	unsigned int irq_cells;
	int err;

	err = of_parse_interrupt_cells(node, parent_node, &irq_cells);
	if (err < 0) {
		udk_warn("node=%s parse #interrupt-cells failed\n", node->full_name);
		return err;
	}
	mem_zero_a(irq_args);
	err = of_parse_interrupts_args(node, index, irq_cells, irq_args);
	if (err < 0) {
		udk_warn("node=%s parse <interrupts> failed\n", node->full_name);
		return err;
	}

	return (int)irq_args[0];
}

int udk_irq_of_parent_name(struct udk_dt_node *node, const char **val)
{
	struct udk_dt_node *parent_node = NULL;
	const char *parent_name = NULL;
	int ret;

	if (node == NULL || val == NULL) {
		udk_warn("invalid node or val\n");
		return -EINVAL;
	}
	ret = of_find_irq_parent_node(node, &parent_node);
	if (ret < 0 || parent_node == NULL) {
		udk_warn("find interrupt parent node failed\n");
		return -ENOENT;
	}
	if (!of_is_node_in_kernel(parent_node)) {
		ret = udk_of_prop_read_string(parent_node, "irq-extension", &parent_name);
		if (ret < 0) {
			udk_warn("interrupt parent does not has irq-extension\n");
			return -ENOENT;
		}
	}
	*val = parent_name;

	return 0;
}

int udk_irq_of_uirq(struct udk_dt_node *node, unsigned int index)
{
	struct udk_dt_node *parent_node = NULL;
	const char *parent_name = NULL;
	int hwirq, ret;

	if (node == NULL) {
		return -EINVAL;
	}
	ret = of_find_irq_parent_node(node, &parent_node);
	if (ret < 0 || parent_node == NULL) {
		udk_warn("find interrupt parent node failed\n");
		return -ENOENT;
	}
	if (of_is_node_in_kernel(parent_node)) {
		hwirq = of_parse_kirq(node, parent_node, index);
		if (hwirq < 0) {
			udk_warn("parse kirq irq failed\n");
			return hwirq;
		}
	} else {
		ret = udk_of_prop_read_string(parent_node, "irq-extension", &parent_name);
		if (ret < 0) {
			udk_warn("interrupt parent does not has irq-extension\n");
			return -ENOENT;
		}
		hwirq = of_parse_raw_irq(node, parent_node, index);
		if (hwirq < 0) {
			udk_warn("parse raw irq failed\n");
			return hwirq;
		}
	}

	return udk_irq_ext_uirq_alloc(parent_name, (unsigned int)hwirq);
}

int udk_irq_of_irq_info(struct udk_dt_node *node, struct udk_irq_info *info)
{
	struct udk_dt_node *parent_node = NULL;
	struct udk_dt_prop *prop = NULL;
	unsigned int irqctrl_id = 0;
	unsigned int irq_cells = 0;
	unsigned int irq_num;
	int err;

	if (node == NULL || info == NULL) {
		udk_warn("invalid node or irq info\n");
		return -EINVAL;
	}
	err = of_find_irq_parent_node(node, &parent_node);
	if (err < 0 || parent_node == NULL) {
		udk_error("find irq parent node failed\n");
		return -ENOENT;
	}
	irqctrl_id = of_parse_irqctrl_id(parent_node);
	err = of_parse_interrupt_cells(node, parent_node, &irq_cells);
	if (err < 0) {
		udk_error("node=%s parse #interrupt-cells failed\n", node->full_name);
		return err;
	}
	prop = udk_of_find_property(node, "interrupts");
	if (prop == NULL || prop->value == NULL) {
		udk_error("find <interrupts> property failed\n");
		return -ENOENT;
	}
	if (!is_irq_prop_valid(prop, irq_cells)) {
		udk_error("irq prop length=%zu does not match irq_cells=%u\n",
			  prop->length, irq_cells);
		return -EINVAL;
	}
	irq_num = irq_num_calc(prop, irq_cells);
	info->irqctrl_id = irqctrl_id;
	info->irq_cells = irq_cells;
	info->irq_num = irq_num;
	info->irq_args = (uint32_t *)prop->value;

	return 0;
}
#endif /* CONFIG_UDK_IRQ_EXT */
