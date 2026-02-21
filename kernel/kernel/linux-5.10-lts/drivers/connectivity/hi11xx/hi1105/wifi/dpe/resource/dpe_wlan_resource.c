/*
 * Copyright (c) @CompanyNameMagicTag 2021-2024. All rights reserved.
 * 功能说明 : DPE资源池主文件
 * 作    者 : wifi
 * 创建日期 : 2024年11月18日
 */


#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DPE_WLAN_RESOURCE_C

#include "dpe_wlan_resource.h"
#include "oam_ext_if.h"

dpe_wlan_res_stru g_dpe_ast_wlan_res;

oal_wlan_cfg_stru g_wlan_spec_cfg_dpe = {
    .hal_dev_num = WLAN_DEVICE_MAX_NUM_PER_CHIP,
};
const oal_wlan_cfg_stru *g_wlan_spec_cfg = &g_wlan_spec_cfg_dpe;

dpe_wlan_res_stru *dpe_wlan_res_addr_get(void)
{
    return (dpe_wlan_res_stru *)(&g_dpe_ast_wlan_res);
}

static void dpe_wlan_private_spec_cfg(void)
{
#ifdef _PRE_DUAL_CHIP
    /* 兼容单芯片的场景，增加判断总线上是否是双芯片 */
    if (oal_bus_get_chip_num() < WLAN_CHIP_MAX_NUM_PER_BOARD) {
        g_wlan_spec_cfg_dpe.vap_support_max_num_limit = HAL_MAX_VAP_NUM_DUAL_MP15;
        g_wlan_spec_cfg_dpe.hal_max_vap_num = HAL_MAX_VAP_NUM_MP13;
    }
#endif
}

void dpe_wlan_spec_cfg_init(const oal_wlan_cfg_stru *spec_cfg)
{
    /* DPE管理区 */
    memcpy_s(&g_wlan_spec_cfg_dpe, sizeof(oal_wlan_cfg_stru), spec_cfg, sizeof(oal_wlan_cfg_stru));

    /* 私有定制化配置项 */
    dpe_wlan_private_spec_cfg();
}

/* 功能描述: 获取最大hal_vap个数 */
uint8_t dpe_mac_chip_get_hal_max_vap_num(void)
{
    return g_wlan_spec_cfg->hal_max_vap_num;
}

