/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Convert hm_uvmm sysif to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 10 15:25:36 2021
 */
#include <libdevhost/vpipe/uvmm.h>
#include <libhmsrv_sys/hm_uvmm.h>

#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>

int libdh_uvmm_mclone_to_vm(unsigned long src_va, unsigned long tgt_gpa,
			    unsigned long len, unsigned int prot)
{
	int rc = hm_uvmm_mclone_to_vm(src_va, tgt_gpa, len, prot);
	return (rc < 0) ? -hmerrno2posix(rc) : rc;
}
