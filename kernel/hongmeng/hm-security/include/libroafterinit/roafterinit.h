/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header of shared library ro_after_init protection
 * Author: Huawei System Security Technology Lab
 * Create: Fri Oct 7 20:32:13 2023
 */

#ifndef ULIBS_LIBROAFTERINIT_PROTECT_H
#define ULIBS_LIBROAFTERINIT_PROTECT_H

#include <link.h>
#include <elf.h>
#include <dlfcn.h>
#include <libmem/utils.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>

#define PT_HKIP  0x60000008

void roafterinit_register_ro(void);

#endif
