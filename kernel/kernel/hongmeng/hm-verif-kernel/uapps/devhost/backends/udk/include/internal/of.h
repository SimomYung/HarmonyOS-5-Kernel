/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Declare struture and internal interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 05 15:43:24 2021
 */

#ifndef __UDK_INTERNAL_OF_H__
#define __UDK_INTERNAL_OF_H__

#include <libfdt.h>
#include <udk/types.h>

struct udk_device;

struct udk_dt_prop {
	char		*name;
	size_t		length;
	void		*value;
	struct udk_dt_prop	*next;
};

struct udk_dt_node {
	char		*name;
	char		*full_name;
	const char	*type;
	unsigned int	offset;
	unsigned int	phandle;

	struct udk_device	*dev;
	struct udk_dt_prop	*properties;

	struct udk_dt_node	*parent;
	struct udk_dt_node	*child;
	struct udk_dt_node	*sibling;
};

#define fdt_value(p) fdt32_to_cpu(*((fdt32_t *)(p)))

static inline uint32_t fdt32_value(const void *ptr)
{
	return fdt_value(ptr);
}

#define UINT32_BIT_SHIFT	32U
static inline uint64_t fdt64_value(const void *ptr)
{
	return ((uint64_t)fdt_value(ptr) << UINT32_BIT_SHIFT) |
		(uint64_t)fdt_value((fdt32_t *)ptr + 1);
}

struct udk_dt_node *udk_of_find_root_node(struct udk_dt_node *node);

#endif /* __UDK_INTERNAL_OF_H__ */
