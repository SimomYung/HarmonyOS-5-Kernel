/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Export IPC shm Symbols
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 21:13:39 2021
 */
#include <libdevhost/vpipe/ipc_shm.h>

#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <hmkernel/compiler.h>

EXPORT_SYMBOL(libdh_ipc_shmget);
EXPORT_SYMBOL(libdh_ipc_shmat);
EXPORT_SYMBOL(libdh_ipc_shmdt);
EXPORT_SYMBOL(libdh_ipc_shmctl);
EXPORT_SYMBOL(libdh_alloc_static_shm);
EXPORT_SYMBOL(libdh_free_static_shm);
