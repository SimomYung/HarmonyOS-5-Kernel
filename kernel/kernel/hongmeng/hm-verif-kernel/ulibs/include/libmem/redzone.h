/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Internal interfaces of slab redzone
 * Author: Huawei OS Kernel Lab
 * Create: Oct 12 2023
 */

#ifndef _ULIBS_LIBMEM_SLAB_REDZONE_H_
#define _ULIBS_LIBMEM_SLAB_REDZONE_H_

#include <libmem/utils.h>
#include <libstrict/strict.h>

struct slab_ator;

#define REDZONE_LOG_LEVEL	((unsigned int)HMLOG_ERROR)

#ifdef CONFIG_SLAB_REDZONE
#define REDZONE_OFFSET	(sizeof(unsigned long long))
#define REDZONE_SIZE	(2 * sizeof(unsigned long long))

void *slab_init_redzone(struct slab_ator *s_ator, void *slot);
int slab_verify_redzone(struct slab_ator *s_ator, void *slot);

static inline void *slab_redzone_obj_to_slot(const void *obj)
{
	return ptr_to_void(ptr_to_type(obj, uintptr_t) - REDZONE_OFFSET);
}

static inline void *slab_redzone_slot_to_obj(void *slot)
{
	return ptr_to_void(ptr_to_type(slot, uintptr_t) + REDZONE_OFFSET);
}

static inline uint32_t slab_redzone_calc_obj_size(uint32_t obj_size)
{
	return (ALIGN_UP(obj_size, REDZONE_OFFSET) + REDZONE_SIZE);
}

#else
#define REDZONE_SIZE	0

static inline void *slab_init_redzone(struct slab_ator *s_ator, void *slot)
{
	UNUSED(s_ator);
	return slot;
}

static inline int slab_verify_redzone(struct slab_ator *s_ator, void *slot)
{
	UNUSED(s_ator, slot);
	return E_HM_OK;
}

static inline void *slab_redzone_obj_to_slot(const void *obj)
{
	return ptr_to_void(obj);
}

static inline void *slab_redzone_slot_to_obj(void *slot)
{
	return slot;
}

static inline uint32_t slab_redzone_calc_obj_size(uint32_t obj_size)
{
	return obj_size;
}
#endif /* CONFIG_SLAB_REDZONE */

#endif /* _ULIBS_LIBMEM_SLAB_REDZONE_H_ */
