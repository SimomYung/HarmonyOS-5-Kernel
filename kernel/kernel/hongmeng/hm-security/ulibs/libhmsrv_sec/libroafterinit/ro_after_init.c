/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of HM KIP pmalloc
 * Author: Huawei System Security Lab
 * Create: Fri Aug 4 02:21:13 2023
 */

#include <libroafterinit/roafterinit.h>

#define ROAFTER_START "__start_ro_after_init"
#define ROAFTER_END "__end_ro_after_init"
enum hkip_seg_ind {
	ZERO = 0,
	ONE,
	TWO
};

static int lookup_ro_after_init_range(struct dl_phdr_info *info, uint64_t *start, uint64_t *end)
{
	uint64_t *seg_hkip_info = 0;
	for(size_t i = 0; i < info->dlpi_phnum; i++) {
		if(info->dlpi_phdr[i].p_type == PT_HKIP) {
			seg_hkip_info = (uint64_t*)(info->dlpi_phdr[i].p_vaddr + info->dlpi_addr);
			break;
		}
	}
	if(!seg_hkip_info) {
		return E_HM_INVAL;
	}
	if(seg_hkip_info[ONE] == 0U || seg_hkip_info[TWO] == 0U) {
		hm_error("[! %s] library %s do not got roafter segment\n", __FILE__, info->dlpi_name);
		return E_HM_INVAL;
	}
	if(seg_hkip_info[ONE] >= seg_hkip_info[TWO]) {
		hm_error("[! %s] library %s roafter data range error\n", __func__, info->dlpi_name);
		hm_debug("start %lx, end %lx\n", seg_hkip_info[ONE] + info->dlpi_addr, seg_hkip_info[TWO] + info->dlpi_addr);
		return E_HM_INVAL;
	}
	*start = seg_hkip_info[ONE] + info->dlpi_addr;
	*end = seg_hkip_info[TWO] + info->dlpi_addr;
	return E_HM_OK;
}

static int roafterinit_callback_set_ro(struct dl_phdr_info *info, size_t size, void *data)
{
	int ret;
	uint64_t start_addr = 0, end_addr = 0, sz;

	if(lookup_ro_after_init_range(info, &start_addr, &end_addr) == E_HM_OK) {
		start_addr = PAGE_ALIGN_UP(start_addr);
		sz = PAGE_ALIGN_UP(end_addr - start_addr);
		ret = hm_mem_mprotect((void*)start_addr, sz, PROT_READ);
		if (ret != E_HM_OK) {
			hm_error("[! %s] failed to set ro_after_init area to MPROT_READ, err=%s\n", __func__, hmstrerror(ret));
		}
	}
	UNUSED(size, data);
	return 0;
}

void roafterinit_register_ro(void)
{
	dl_iterate_phdr(roafterinit_callback_set_ro, NULL);
	return;
}
