/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : dpe_wlan_device.c
 * 作    者 :
 * 创建日期 : 2024年11月21日
 */

/* 1 头文件包含 */
#include "dpe_wlan_device.h"
#include "dpe_wlan_resource.h"
#include "dpe_wlan_stat.h"
#include "oam_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DPE_WLAN_DEVICE_C

/*
 * 函 数 名  : dpe_wlan_device_get
 * 功能描述  : 获取对应DPE DEVICE
 * 1.日    期  : 2024年11月21日
 *   修改内容  : 新生成函数
 */
dpe_wlan_device_stru *dpe_wlan_device_get(uint16_t us_idx)
{
    if (us_idx >= WLAN_SERVICE_DEVICE_SUPPORT_MAX_NUM_SPEC) {
        oam_error_log1(0, 0, "{dpe_wlan_device_get::dpe_device null,device_idx=%d.}", us_idx);
        return NULL;
    }
    return (dpe_wlan_device_stru *)(&g_dpe_ast_wlan_res.dpe_wlan_device_res[us_idx]);
}

void dpe_wlan_device_init(uint16_t us_idx, uint16_t cfg_vap_id, uint8_t chip_id)
{
    dpe_wlan_device_stru *device = dpe_wlan_device_get(us_idx);
    if (device == NULL) {
        return;
    }

    memset_s(device, sizeof(dpe_wlan_device_stru), 0, sizeof(dpe_wlan_device_stru));
    device->uc_device_id = us_idx;
    device->en_al_trx_flag = OAL_FALSE;
    device->pre_host_mac_irq_mask = 0xffffffff;
    dpe_wlan_stat_init_device_stat(device);
    dpe_wlan_cfg_vap_init(us_idx, cfg_vap_id, chip_id);
    dpe_wlan_lut_index_tbl_init();
}

void dpe_wlan_device_al_trx_flag_sync(uint8_t in_al_trx)
{
    dpe_wlan_device_stru *dpe_device = dpe_wlan_device_get(0);
    dpe_device->en_al_trx_flag = in_al_trx == OAL_TRUE ? OAL_TRUE : dpe_device->en_al_trx_flag;
}

void dpe_wlan_device_init_dyn_pcie(uint8_t dev_idx)
{
    dpe_wlan_device_stru *dpe_dev = dpe_wlan_device_get(dev_idx);
    if (dpe_dev == NULL) {
        return;
    }
    dpe_dev->pre_host_mac_irq_mask = 0xffffffff;
}