/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Slab redzone function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Oct 12 2023
 */

#include "slab.h"
#include <libmem/redzone.h>
#include <hmsysmgr/dumpstack.h>
#include <libhmsrv_sys/hm_dump.h>

/*
 * slab page memory layout:
 * +------------------------------------------------------------------------+
 * | slab desc | redzone | slot | redzone | ... | redzone | slot | redzone |
 * +------------------------------------------------------------------------+
 */

/* when obj is inactive */
#define REDZONE_INACTIVE	0x09F911029D74E35BULL
/* when obj is active */
#define REDZONE_ACTIVE		0xD84156C5635688C0ULL

static unsigned long long *slab_redzone1(void *slot)
{
	return (unsigned long long*)slot;
}

static unsigned long long *slab_redzone2(struct slab_ator *s_ator, void *slot)
{
	return (unsigned long long *)(ptr_to_type(slot, uintptr_t) + REDZONE_OFFSET +
			ALIGN_UP(s_ator->obj_orig_size, REDZONE_OFFSET));
}

static void slab_set_redzone(unsigned long long *redzone1,
					    unsigned long long *redzone2, unsigned long long value)
{
	int ret = E_HM_OK;

	if (redzone1 == NULL || redzone2 == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		*redzone1 = value;
		*redzone2 = value;
	}
}

void *slab_init_redzone(struct slab_ator *s_ator, void *slot)
{
	int ret = E_HM_OK;
	void *obj = slot;

	if (s_ator == NULL || slot == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		unsigned long long *redzone1 = slab_redzone1(slot);
		unsigned long long *redzone2 = slab_redzone2(s_ator, slot);
		slab_set_redzone(redzone1, redzone2, REDZONE_ACTIVE);

		obj = slab_redzone_slot_to_obj(slot);
	}

	return obj;
}

int slab_verify_redzone(struct slab_ator *s_ator, void *ptr)
{
	int ret = E_HM_OK;
	uint32_t idx;
	void *slot = NULL;
	struct slab_desc *s_desc = NULL;

	if (s_ator == NULL || ptr == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		s_desc = slab_obj_to_slab_desc(ptr, (size_t)SLAB_PAGE_SIZE);
		idx = slot_ptr_to_idx(s_ator, s_desc, ptr);
		slot = (void *)(ptr_to_ulong(s_desc) + sizeof(struct slab_desc) + idx * s_ator->obj_size);

		unsigned long long *redzone1 = slab_redzone1(slot);
		unsigned long long *redzone2 = slab_redzone2(s_ator, slot);

		if (*redzone1 == REDZONE_INACTIVE && *redzone2 == REDZONE_INACTIVE) {
			hm_error("redzone: double free detected\n");
			ret = E_HM_FAULT;
		} else if (*redzone1 != REDZONE_ACTIVE || *redzone2 != REDZONE_ACTIVE) {
			hm_error("redzone: memory outside object was overwritten\n");
			ret = E_HM_FAULT;
		}

		if (ret != E_HM_OK) {
			hm_error("slot %p redzone 1:0x%llx, redzone 2:0x%llx\n", slot, *redzone1, *redzone2);
		} else {
			slab_set_redzone(redzone1, redzone2, REDZONE_INACTIVE);
		}
	}

	return ret;
}
