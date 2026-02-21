/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Convert hm_uvmm sysif to posix errno
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 10 15:06:21 2021
 */
#ifndef LIBDEVHOST_VPIPE_UVMM_H
#define LIBDEVHOST_VPIPE_UVMM_H

/*
 * NOTE: Now the <libdevhost/vpipe/uvmm.h> might be included
 * when compiling a liblinux module. To support this, cannot include
 * any unsupported-for-liblinux-module header files here.
 */
int libdh_uvmm_mclone_to_vm(unsigned long src_va, unsigned long tgt_gpa,
			    unsigned long len, unsigned int prot);

#endif
