/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Export Vnotify Symbols
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 21:13:39 2021
 */
#include <libdevhost/vpipe/vnotify.h>

#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <hmkernel/compiler.h>

EXPORT_SYMBOL(libdh_vnotify_init);
EXPORT_SYMBOL(libdh_vnotify_attach_chn);
EXPORT_SYMBOL(libdh_vnotify_detach_chn);
EXPORT_SYMBOL(libdh_vnotify_notify);
EXPORT_SYMBOL(libdh_vnotify_receive);
EXPORT_SYMBOL(libdh_vnotify_update_futex);
EXPORT_SYMBOL(libdh_vnotify_read_status);
