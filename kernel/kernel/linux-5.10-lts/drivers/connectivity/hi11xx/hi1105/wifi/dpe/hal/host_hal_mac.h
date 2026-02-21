/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL MAIN
 * 作    者 : wifi
 * 创建日期 : 2020年10月19日
 */

#ifndef HOST_HAL_MAC_H
#define HOST_HAL_MAC_H

#include "oal_ext_if.h"
#include "frw_ext_if.h"
#include "hal_common.h"
#include "host_hal_device.h"

void hal_host_al_rx_fcs_info(uint8_t hal_dev_id);
void hal_host_get_rx_pckt_info(uint8_t hal_dev_id, dmac_atcmdsrv_atcmd_response_event *rx_pkcg_event_info);
#endif
