/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Ebr radix tree node allocation interfaces.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 6 16:50:01 2024
 */
#include <libalgo/radix_tree.h>
#include <libhwsecurec/securec.h>

void *radix_tree_node_malloc(void)
{
	size_t size = struct_radix_tree_node_size();
	void *node = malloc(size);
	if (likely(node != NULL)) {
		NOFAIL(memset_s(node, size, 0, size));
	}
	return node;
}

void radix_tree_node_free(void *ptr)
{
	if (likely(ptr != NULL)) {
		free(ptr);
	}
}
