/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Implementation for id_generator mcache malloc/free function
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 28 15:31:45 2024
 */
#include <libalgo/id_generator.h>
#include <malloc.h>
#include <hongmeng/compiler.h>

void *struct_id_generator_malloc(void)
{
	return malloc(struct_id_generator_size());
}

void struct_id_generator_free(void *p)
{
	if (likely(p != NULL)) {
		free(p);
	}
}
