/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: This is the header of udk irq extention interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 28 19:11:05 2021
 */

#ifndef __DEVHOST_UDK_IRQ_EXT_H__
#define __DEVHOST_UDK_IRQ_EXT_H__

#include <udk/of.h>
#include <udk/irq.h>
#include <udk/lib/dlist.h>
#include <libstrict/strict.h>

#include <stdbool.h>

struct udk_irq_ext_data {
	unsigned int irq;
	void *private;
};

struct udk_irq_ext_ops {
	int (*irq_get_parent_irq)(void *ctx, unsigned int child_irq);
	int (*irq_get_child_irq)(void *ctx, unsigned int parent_irq);
	int (*irq_activate)(void *ctx, unsigned int irq);
	int (*irq_set_trigger)(void *ctx, unsigned int irq, unsigned int type);
	void (*irq_eoi)(void *ctx, unsigned int irq);
	void (*irq_mask)(void *ctx, unsigned int irq);
	void (*irq_unmask)(void *ctx, unsigned int irq);
	void (*irq_deactivate)(void *ctx, unsigned int irq);
};

struct udk_irq_info {
	unsigned int irqctrl_id;
	unsigned int irq_cells;
	unsigned int irq_num;
	unsigned int *irq_args;
};

struct udk_irq_ext {
	const char *name;
	struct udk_irq_ext_ops *ops;
	struct udk_irq_info *irq_info;
	void *ctx;
};

#ifdef CONFIG_UDK_IRQ_EXT

int udk_irq_ext_add(struct udk_irq_ext *ext, const char *parent);
int udk_irq_ext_remove(struct udk_irq_ext *ext);

int udk_irq_ext_uirq_alloc(const char *ext_name, unsigned int hw_irq);
int udk_irq_ext_uirq_map(const char *ext_name, unsigned int hw_irq,
			 unsigned int uirq);
void udk_irq_ext_uirq_free(unsigned int uirq);

int udk_irq_of_parent_name(struct udk_dt_node *node, const char **val);
int udk_irq_of_irq_info(struct udk_dt_node *node, struct udk_irq_info *info);
#else

static inline int udk_irq_ext_add(struct udk_irq_ext *ext, const char *parent)
{
	UNUSED(ext, parent);
	return 0;
}

static inline int udk_irq_ext_remove(struct udk_irq_ext *ext)
{
	UNUSED(ext);
	return 0;
}

static inline int udk_irq_ext_uirq_alloc(const char *ext_name, unsigned int hw_irq)
{
	UNUSED(ext_name, hw_irq);
	return -ENOENT;
}

static inline int udk_irq_ext_uirq_map(const char *ext_name, unsigned int hw_irq,
				       unsigned int uirq)
{
	UNUSED(ext_name, hw_irq, uirq);
	return -ENOSYS;
}

static inline void udk_irq_ext_uirq_free(unsigned int uirq)
{
	UNUSED(uirq);
}

static inline int udk_irq_of_parent_name(struct udk_dt_node *node, const char **val)
{
	UNUSED(node, val);
	return -ENOENT;
}

static inline int udk_irq_of_irq_info(struct udk_dt_node *node, struct udk_irq_info *info)
{
	UNUSED(node, info);
	return -ENOSYS;
}

#endif /* CONFIG_UDK_IRQ_EXT */

#endif /* __DEVHOST_UDK_IRQ_EXT_H__ */
