/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Ebr radix tree node allocation interfaces.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 19 16:50:01 2024
 */
#include <libhmebr/ebr_radix_tree.h>
#include <libhwsecurec/securec.h>

void *ebr_radix_tree_node_zmalloc(enum ebr_mcache_type mcache_type)
{
	size_t size = ebr_radix_tree_node_size();
	void *node = malloc(size);
	if (likely(node != NULL)) {
		NOFAIL(memset_s(node, size, 0, size));
	}
	return node;
}

void ebr_radix_tree_node_free(void *ptr, enum ebr_mcache_type mcache_type)
{
	if (likely(ptr != NULL)) {
		free(ptr);
	}
}
