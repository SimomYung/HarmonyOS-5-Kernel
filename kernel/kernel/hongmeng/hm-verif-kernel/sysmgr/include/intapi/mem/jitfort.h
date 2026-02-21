/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: jitfort ctrl header
 * Author: Huawei
 * Create: Wen Mar 17 09:43:10 2024
*/

#ifndef SYSMGR_INTAPI_MEM_JITFORT_H
#define SYSMGR_INTAPI_MEM_JITFORT_H

/* prctl jitfort options */
#define JITFORT_SET_MNGT_MODE		0
#define JITFORT_SET_ENABLED			1
#define JITFORT_SET_DISABLED 		2
#define JITFORT_SWITCH_IN			3
#define JITFORT_SWITCH_OUT			4
#define JITFORT_CREATE_COPGTABLE 	5
#define JITFORT_DESTROY_COPGTABLE 	6
#define JITFORT_PAC_SUPPORTED		7
#define JITFORT_QUERY_ENCAPS		'E'	

#define JITFORT_PROTECT_OFF		0
#define JITFORT_PROTECT_ON		1
#define JITFORT_MEM_MNGT_PERMISSIVE	0
#define JITFORT_MEM_MNGT_ENFORCE	1

struct vspace_s;
struct thread_s;
struct process_s;
struct mem_range_s;
struct vregion_s;

void jitfort_protect_enable(void);
void jitfort_protect_disable(void);

int jitfort_enable(void);
int jitfort_disable(void);
int jitfort_mem_mngt_mode_of(void);
int jitfort_set_mem_mngt_mode(int flags);
#ifdef CONFIG_SECURITY_JITFORT
int jitfort_caller_check(struct vspace_s *vspace, bool in_vslock);

int jitfort_mode_of(void);

int jitfort_switch_pgtable(struct thread_s *thread, int option);

void jitfort_mr_prot_mask(struct mem_range_s *mr, uint32_t prot);

int jitfort_prepare_mprotect(struct vregion_s *vr, uint32_t *rprot);

int jitfort_check_map_jit(uint32_t flags);
#else
static inline int jitfort_caller_check(struct vspace_s *vspace, bool in_vslock)
{
	UNUSED(vspace, in_vslock);
	return E_HM_OK;
}

static inline int jitfort_mode_of(void)
{
	return JITFORT_PROTECT_OFF;
}

static inline int jitfort_switch_pgtable(struct thread_s *thread, int option)
{
	UNUSED(thread, option);
	return E_HM_INVAL;
}

static inline void
jitfort_mr_prot_mask(struct mem_range_s *mr, uint32_t prot)
{
	UNUSED(mr, prot);
}

static inline int jitfort_prepare_mprotect(struct vregion_s *vr, uint32_t *rprot)
{
	UNUSED(vr, rprot);
	return E_HM_OK;
}

static inline int jitfort_check_map_jit(uint32_t flags)
{
	UNUSED(flags);
	return E_HM_OK;
}
#endif

#endif
