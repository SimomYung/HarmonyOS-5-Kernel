/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: API for hm-uvmm
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 02 17:06:43 2021
 */
#ifndef ULIBS_LIBHMSRV_SYS_HM_UVMM_H
#define ULIBS_LIBHMSRV_SYS_HM_UVMM_H

#include <stdint.h>

#include <hmkernel/capability.h>
#include <libvpipe/vnotify.h>

struct uvmm_param_s {
	int vm_type;
};

rref_t hm_uvmm_rref_acquire(const struct uvmm_param_s *uvmm);
void hm_uvmm_rref_release(const struct uvmm_param_s *uvmm, rref_t uvmm_rref);
int hm_uvmm_mclone_to_vm(uint64_t src_va, uint64_t tgt_gpa,
			 uint64_t len, uint32_t prot);
int hm_uvmm_vnotify_send_ctrl_msg(void);

#endif
