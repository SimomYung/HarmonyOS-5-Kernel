/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL MAIN
 * 作    者 :
 * 创建日期 : 2020年06月14日
 */

#include "pcie_host.h"
#include "oal_util.h"
#include "oal_ext_if.h"
#include "host_hal_access_mgmt.h"
#include "host_hal_ring.h"
#include "host_hal_device.h"
#include "frw_ext_if.h"
#include "wlan_spec.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_MAC_C

/* 功能描述 : 常收维测信息输出 */
void hal_host_al_rx_fcs_info(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);
    if (hal_device == NULL) {
        oam_error_log0(0, 0, "{host_al_rx_fcs_info::hal_device null.}");
        return;
    }

    oam_warning_log3(hal_device->device_id, OAM_SF_CFG,
        "host_al_rx_fcs_info:packets info, mpdu succ[%d], ampdu succ[%d],fail[%d]",
        hal_device->st_alrx.rx_normal_mdpu_succ_num,
        hal_device->st_alrx.rx_ampdu_succ_num,
        hal_device->st_alrx.rx_ppdu_fail_num);

    hal_device->st_alrx.rx_normal_mdpu_succ_num = 0;
    hal_device->st_alrx.rx_ampdu_succ_num = 0;
    hal_device->st_alrx.rx_ppdu_fail_num = 0;
}

/* 功能描述 : 常收维测信息输出 */
void hal_host_get_rx_pckt_info(uint8_t hal_dev_id, dmac_atcmdsrv_atcmd_response_event *rx_pkcg_event_info)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);
    if (hal_device == NULL) {
        oam_error_log0(0, 0, "{host_get_rx_pckt_info::hal_device null.}");
        return;
    }
    if (hal_device->st_alrx.rx_ampdu_succ_num != 0) {
        rx_pkcg_event_info->event_para = hal_device->st_alrx.rx_ampdu_succ_num;
    } else {
        rx_pkcg_event_info->event_para = hal_device->st_alrx.rx_normal_mdpu_succ_num;
    }
}
