/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host/device 校准公共使用头文件
 */

#ifndef WLAN_CALI_H
#define WLAN_CALI_H

#include "mp15/wlan_cali_mp15.h"

#include "mp16/wlan_cali_mp16.h" /* mp16/mp16c 校准数据格式相同，mp16c使用mp16校准数据结构 */

#include "mp17c/wlan_cali_mp17c.h"

#include "mp12/wlan_cali_mp12.h"

/* host校准数据结构体 */
typedef struct {
    mp1x_wlan_cali_data_para_stru cali_data;
    mp1x_wlan_cali_common_ext_stru common_cali_data_ext;
} mp1x_wlan_cali_data_para_ext_stru;

typedef struct {
    mp12_wlan_cali_data_para_stru cali_data;
    mp12_wlan_cali_common_ext_stru common_cali_data_ext;
} mp12_wlan_cali_data_para_ext_stru;

#endif /* end of wlan_cali.h */
