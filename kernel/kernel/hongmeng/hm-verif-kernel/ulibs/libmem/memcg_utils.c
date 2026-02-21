/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memcg utilities
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 22 11:44:47 2021
 */

#include <libmem/memcg_utils.h>

#include <libsysif/sysmgr/api.h>

#ifndef CONFIG_PAGE_META_MERGE
static uintptr_t g_memcg_mapping = INVALID_VADDR;

int memcg_utils_init(void)
{
	struct __actvret_hmcall_resmgr_mem_mapping actvret;
	int rc = E_HM_OK;

	if (g_memcg_mapping == INVALID_VADDR) {
		mem_zero_s(actvret);
		rc = actvcall_hmcall_resmgr_mem_mapping(&actvret);
		if (rc == E_HM_OK) {
			g_memcg_mapping = actvret.vaddr;
		}
	}

	return rc;
}

#define memcg_share_read_once(x) ((volatile struct memcg_share_slot_s *)(x))

static int query_memcg_id(uint32_t cnode_idx, unsigned int *memcg_id)
{
	volatile struct memcg_share_slot_s *curr = NULL;
	unsigned int version;
	uintptr_t curr_off;
	int rc;

	do {
		rc = E_HM_NOOBJ;
		curr = memcg_share_read_once(memcg_share_from_off(g_memcg_mapping, 0));
		version = curr->u.version;
		curr_off = curr->next_off;
		while (curr_off != (uintptr_t)0) {
			curr = memcg_share_read_once(memcg_share_from_off(g_memcg_mapping, curr_off));
			if (curr->u.cnode_idx < cnode_idx) {
				curr_off = curr->next_off;
			} else if (curr->u.cnode_idx == cnode_idx) {
				*memcg_id = curr->memcg_id;
				rc = E_HM_OK;
				break;
			} else {
				break;
			}
		}
		if (rc != E_HM_OK) {
			break;
		}
		/* double check verison number */
		curr = memcg_share_read_once(memcg_share_from_off(g_memcg_mapping, 0));
	} while (curr->u.version != version);

	return rc;
}

int memcg_utils_query_id(uint32_t cnode_idx, unsigned int *memcg_id)
{
	unsigned int tmp_id = MEMCG_ROOT_ID;
	int rc = E_HM_OK;

	if (memcg_id == NULL) {
		rc = E_HM_INVAL;
	}
	if ((rc == E_HM_OK) && (g_memcg_mapping == INVALID_VADDR)) {
		rc = E_HM_POSIX_FAULT;
	}
	if (rc == E_HM_OK) {
		rc = query_memcg_id(cnode_idx, &tmp_id);
	}
	if (rc == E_HM_OK) {
		*memcg_id = tmp_id;
	}

	return rc;
}

int memcg_utils_charge(unsigned int memcg_id, unsigned int nr_pages,
		       unsigned int *nr_charged)
{
	struct __actvret_hmcall_resmgr_mem_fs_charge actvret;
	int rc = E_HM_OK;

	if ((memcg_id == MEMCG_ROOT_ID) || (nr_charged == NULL)) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(actvret);
		rc = actvcall_hmcall_resmgr_mem_fs_charge(memcg_id, nr_pages, &actvret);
		if (rc == E_HM_OK) {
			*nr_charged = actvret.nr_charged;
		}
	}

	return rc;
}

int memcg_utils_uncharge(unsigned int memcg_id, unsigned int nr_pages)
{
	int rc = E_HM_OK;

	if (memcg_id == MEMCG_ROOT_ID) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = actvcall_hmcall_resmgr_mem_fs_uncharge(memcg_id, nr_pages);
	}

	return rc;
}
#else
int memcg_utils_init(void)
{
	return E_HM_OK;
}

int memcg_utils_query_id(uint32_t cnode_idx, unsigned int *memcg_id)
{
	UNUSED(cnode_idx, memcg_id);
	return E_HM_OK;
}

int memcg_utils_charge(unsigned int memcg_id, unsigned int nr_pages,
		       unsigned int *nr_charged)
{
	UNUSED(memcg_id, nr_pages, nr_charged);
	return E_HM_OK;
}

int memcg_utils_uncharge(unsigned int memcg_id, unsigned int nr_pages)
{
	UNUSED(memcg_id, nr_pages);
	return E_HM_OK;
}
#endif // CONFIG_PAGE_META_MERGE

