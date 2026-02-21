/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ulibs slab operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 20 15:02:53 2020
 */
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <libmem/slab.h>
#include <libmem/slab_utils.h>

uint32_t slab_page_usable_size(const struct slab_ator *ator)
{
	UNUSED(ator);
	return SLAB_PAGE_SIZE - (uint32_t)sizeof(struct slab_desc);
}

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
		hm_error("slab range invalid start 0x%lx, size %zu, slab_size %zu\n",
			 vaddr_of((void *)start), size, slab_size);
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

uintptr_t slot_idx_to_ptr(const struct slab_ator *s_ator,
			  const struct slab_desc *s_desc,
			  uint32_t idx)
{
	return (uintptr_t)s_desc + sizeof(struct slab_desc) + (s_ator->obj_size * idx);
}

uint32_t slot_ptr_to_idx(const struct slab_ator *s_ator,
			 const struct slab_desc *s_desc,
			 const void *ptr)
{
	return (uint32_t)(((ptr_to_ulong(ptr) - ptr_to_ulong(s_desc)) - sizeof(struct slab_desc)) / s_ator->obj_size);
}

bool slab_ptr_is_valid(struct slab_ator *ator, uintptr_t ptr)
{
	struct slab_ator *ptr_ator = NULL;
	struct slab_desc *s_desc = NULL;
	bool valid = true;

	if (ator == NULL) {
		valid = false;
	}
	/* check ator is equal */
	if (valid) {
		void *obj = slab_redzone_obj_to_slot((void *)ptr);
		s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
		if (s_desc != NULL) {
			ptr_ator = s_desc->ator;
		}
		if (unlikely(ptr_ator != ator)) {
			valid = false;
		}
	}
	/* check ptr is not slab desc */
	if (valid) {
		if (unlikely(ptr < (ptr_to_ulong(s_desc) + sizeof(struct slab_desc)))) {
			valid = false;
		}
	}
	/* check ptr alignment */
	if (valid) {
		size_t obj_sz = ator->obj_size;
		size_t ptr_off = ptr - ptr_to_ulong(s_desc) - sizeof(struct slab_desc);
		if (unlikely(ptr_off % obj_sz != 0)) {
			valid = false;
		}
	}
	return valid;
}
