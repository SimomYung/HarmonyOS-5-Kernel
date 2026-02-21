/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: Export Ulibs Symbols
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 19 16:28:16 2021
 */

#include <hmkernel/compiler.h>
#include <hongmeng/errno.h>

#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <libhmsrv_sys/hm_procmgr.h>

#ifdef CONFIG_EXPORT_HM_PROC_SHOW_PTE
/* Exported only on SD5151T-debug for testing */
EXPORT_SYMBOL(hm_proc_show_pte);
#endif
