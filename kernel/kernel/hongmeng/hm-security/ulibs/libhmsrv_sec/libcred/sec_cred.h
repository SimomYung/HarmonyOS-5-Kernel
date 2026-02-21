/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of shared memory list
 * Author: Huawei OS Kernel Lab
 * Create: Thur Feb 2 14:50:00 2023
 */

#ifndef ULIBS_LIBSEC_SECCRED_H
#define ULIBS_LIBSEC_SECCRED_H
#include <libsec/cred.h>

void sec_shared_cred_ref_release(struct raw_refcnt *ref);
void sec_base_cred_ref_release(struct raw_refcnt *ref);
#endif
