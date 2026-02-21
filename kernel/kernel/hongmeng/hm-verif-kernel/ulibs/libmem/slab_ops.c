/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ulibs slab operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 20 15:02:53 2020
 */
#include "slab.h"
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

struct slab_desc *slab_obj_to_slab_desc(const void *obj, size_t slab_size)
{
	uintptr_t obj_v_off, desc_v_off;
	uintptr_t size_mask;

	obj_v_off = (uintptr_t)obj;
	size_mask = (uintptr_t)(~(slab_size - 1UL));

	desc_v_off = obj_v_off & size_mask;

	return (struct slab_desc *)desc_v_off;
}

int slab_check_range(uintptr_t start, size_t size, size_t slab_size)
{
	int ret = E_HM_OK;

	if (start == 0 || !IS_ALIGNED(start, slab_size) || size < slab_size) {
		hm_error("slab range invalid start 0x%"PRIxPTR", size %zu, slab_size %zu\n",
			 start, size, slab_size);
		ret = E_HM_INVAL;
	}

	return ret;
}

#ifdef CONFIG_SLAB_DEBUG_USAGE
struct process_s *slab_fill_user_info(struct slab_user_info *info, uint32_t s_flags)
{
	UNUSED(s_flags);
	info = NULL;
	UNUSED(info);
	return NULL;
}
#endif

void process_add_slab_size(struct process_s *process, int64_t size, struct slab_ator *s_ator)
{
	UNUSED(process, size, s_ator);
}

void process_sub_slab_size(pid_t pid, int64_t size, struct slab_ator *s_ator)
{
	UNUSED(pid, size, s_ator);
}
