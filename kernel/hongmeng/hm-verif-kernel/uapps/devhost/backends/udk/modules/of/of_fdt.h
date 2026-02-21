/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK devicetree base defination
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 14 14:27:10 2019
 */
#ifndef __UDK_OF_FDT_H__
#define __UDK_OF_FDT_H__

#include <internal/of.h>

extern struct udk_dt_node *udk_of_root;

#define DEFAULT_ADDR_CELLS	2U
#define DEFAULT_SIZE_CELLS	1U
#define MAX_ADDR_CELLS		2U
#define MAX_SIZE_CELLS		2U

#define MAX_BUFFER_LEN		512U
#define MAX_PROP_VALUE_LEN	8192U

struct udk_dt_node *udk_of_unflatten_tree(const void* dtb);
void udk_of_free_tree(struct udk_dt_node *root);

int udk_of_reg_count(const struct udk_dt_node *node, unsigned int *nump);
int udk_of_get_reg_value(const struct udk_dt_node *node, unsigned int reg_index,
			 uint64_t *addr, uint64_t *size);
int udk_of_get_reg_names(const struct udk_dt_node *node, const char **names,
			 size_t length);
void udk_of_delete_dt_node(struct udk_dt_node *np);

#endif /* __UDK_OF_FDT_H__ */
