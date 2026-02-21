/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exported liblinux APIs
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 13 20:34:41 2021
 */

#include <hmkernel/compiler.h>
#include <liblinux/pal.h>
#include "libdh_linux.h"

/* used in devhost.ko/kvic.c,
 * only exists in 4.4, in 5.10, it is an inline function */
EXPORT_SYMBOL(irq_domain_alloc_fwnode);

/* used in hm-liblinux anon_inode.c */
EXPORT_SYMBOL(liblinux_pal_get_unused_fd_flags);
EXPORT_SYMBOL(liblinux_pal_fd_install);
EXPORT_SYMBOL(liblinux_pal_put_unused_fd);
