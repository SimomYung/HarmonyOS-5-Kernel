/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exprot hm uvmm sysif symbols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 10 15:01:29 2021
 */
#include <libdevhost/vpipe/uvmm.h>

#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <hmkernel/compiler.h>

EXPORT_SYMBOL(libdh_uvmm_mclone_to_vm);
