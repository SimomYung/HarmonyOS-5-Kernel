/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 *
 * Description:sdio master ip driver
 * Author: @CompanyNameTag
 */

#define HISI_LOG_TAG               "[SDIO_MASTER]"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/host.h>
#include "oal_util.h"
#include "oal_sdio.h"
#include "oal_sdio_host_if.h"
#include "sdio_master_v120.h"
#include "sdio_master_v151.h"
#include "sdio_master.h"

/* sdio version registers */
#define HISDIO_WAKEUP_DEV_REG   0xf0
#define HISDIO_IP_VERSION_REG   0xf5
#ifdef CONFIG_MMC
static int32_t sdio_master_ip_version_init(struct oal_sdio *sdio)
{
    int32_t ret;
    sdio_func0_wakeup_reg wakeup_data;
    struct msdio_func0_addr *f0_addr = &sdio->reg_addr.func0_addr;

    /* function0 reg address */
    f0_addr->wakeupdev = HISDIO_WAKEUP_DEV_REG;
    f0_addr->ip_version = HISDIO_IP_VERSION_REG;

    /* sdio ip version check */
    oal_sdio_claim_host(sdio);
    wakeup_data.dword = (uint32_t)sdio_f0_readb(sdio->func, f0_addr->wakeupdev, &ret);
    oal_sdio_release_host(sdio);
    if (ret != 0) {
        oal_print_mpxx_log(MPXX_LOG_INFO, "get sdio master version failed, ret=%d", ret);
        return ret;
    }
    /* 判断IP版本 */
    return (wakeup_data.bits.version == 0) ? sdio_master_ip_init_v120(sdio) : sdio_master_ip_init_v151(sdio);
}

int32_t sdio_master_ip_init(struct oal_sdio *sdio)
{
    return sdio_master_ip_version_init(sdio);
}
#endif