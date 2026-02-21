/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Swap dma memory implement
 * Author: HISI Kirin memory
 * Create: Mon May 13 20:18:20 2024
 */
#include <liblinux/pal.h>

#include <hmasm/page.h>
#include <sys/mman.h>
#include <libmem/flags.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <devhost/memory.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/vmap.h>
#include <hmkernel/mm/pgtable.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libdevhost/mm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>

#include "shrinker.h"
#include <devhost/log.h>

int liblinux_pal_mem_swap_space_init(int swap_id, unsigned long total_size, unsigned int ext_size)
{
	int ret;

	ret = actvxactcall_hmcall_mem_swap_space_init(true, true, swap_id, total_size, ext_size);
	if (ret < 0) {
		dh_error("liblinux swap_space_init failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return ret;
}

int liblinux_pal_mem_swap_alloc_extentid(int swap_id, int *ext_id)
{
	int ret;
	struct __actvret_hmcall_mem_swap_alloc_extentid actv_ret;

	mem_zero_s(actv_ret);
	ret = actvxactcall_hmcall_mem_swap_alloc_extentid(true, true, swap_id, &actv_ret);
	if (ret < 0) {
		dh_error("liblinux swap_alloc_extentid failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	*ext_id = actv_ret.ext_id;

	return 0;
}

void liblinux_pal_mem_swap_free_extentid(int swap_id, int ext_id)
{
	int ret;

	ret = actvxactcall_hmcall_mem_swap_free_extentid(false, false, swap_id, ext_id);
	if (ret < 0)
		dh_error("liblinux swap_free_extentid failed, ret=%s\n", hmstrerror(ret));
}

int liblinux_pal_mem_swap_in_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num)
{
	int ret;

	ret = actvxactcall_hmcall_mem_swap_in_range(false, false, swap_id, (void *)pa_array, (void *)ext_ids, num);
	if (ret < 0) {
		dh_error("liblinux swap_in_range failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

int liblinux_pal_mem_swap_out_range(int swap_id, unsigned long *pa_array, int *ext_ids, int num)
{
	int ret;

	ret = actvxactcall_hmcall_mem_swap_out_range(false, false, swap_id, (void *)pa_array, (void *)ext_ids, num);
	if (ret < 0) {
		dh_error("liblinux swap_out_range failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
