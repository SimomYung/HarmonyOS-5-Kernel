/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Structure definition and function implementation of aux
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 17 09:53:42 2021
 */
#include <hongmeng/types.h>
#include <elf.h>
#include <libcrt/hmcrt.h>
#include <libhmsrv_sys/hm_auxv.h>

static size_t current_aux[AUX_CNT];
static uint32_t cnode_idx;
static uint32_t tcb_inner_idx;

void hm_setup_vdso_aux(uintptr_t base)
{
	current_aux[AT_SYSINFO_EHDR] = (size_t)base;
}

size_t *hm_crt_current_aux(void)
{
	return current_aux;
}

uint32_t hm_crt_cnode_idx_of(void)
{
	return cnode_idx;
}

uint32_t hm_crt_tcb_inner_idx_of(void)
{
	return tcb_inner_idx;
}

void hm_crt_set_cnode_idx(uint32_t cnode)
{
	cnode_idx = cnode;
}

void hm_crt_set_tcb_inner_idx(uint32_t inner)
{
	tcb_inner_idx = inner;
}
