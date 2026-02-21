/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: dma register wr.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:40:22 2024
 */

#include <liblinux/pal.h>

#include <devhost/log.h>
#include <libsec/posix_cap.h>
#include <hongmeng/syscall.h>
#include <libhmsrv_sys/hm_sysctrl.h>

void liblinux_pal_peri_dma_write(struct peri_dma_write_info* info)
{
    int err;

    err = sysfast_peri_dma_write(info);
    if (err < 0) {
        dh_error("sysfast_peri_dma_write failed\n");
    }

    return;
}