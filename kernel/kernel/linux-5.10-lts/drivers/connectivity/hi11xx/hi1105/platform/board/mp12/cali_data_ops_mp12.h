/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: plat_cali.c header file
 * Author: @CompanyNameTag
 */

#ifndef CALI_DATA_OPS_MP12_H
#define CALI_DATA_OPS_MP12_H

/* 头文件包含 */
#include "plat_type.h"
#include "oal_types.h"
#include "oal_util.h"
#include "plat_cali.h"

typedef struct {
    bt_wl_cali_data_stru wl_cali_data;
} m12_bfgx_data_stru;

/* 以下宏定义，如果要修改结构体成员，需要同步修改device */
#define MP12_BFGX_BT_CALI_DATA_SIZE             512 // (sizeof(m12_bfgx_data_stru))
#define MP12_BFGX_NV_DATA_SIZE                  128
#define MP12_BFGX_EXT_NV_DATA_SIZE              128
#define MP12_BFGX_BT_CUST_INI_SIZE              512

/* 考虑结构体总体长度考虑SDIO下载长度512对齐特性，这里长度为2048 */
typedef struct {
    uint8_t auc_bfgx_data[MP12_BFGX_BT_CALI_DATA_SIZE]; // bt校准数据
    uint8_t auc_wifi_cali_for_bt_data[MP12_BFGX_BT_CALI_DATA_SIZE]; // wifi校准数据
    uint8_t auc_nv_data[MP12_BFGX_NV_DATA_SIZE];
    uint8_t auc_ext_nv_data[MP12_BFGX_EXT_NV_DATA_SIZE];
    uint8_t auc_bt_cust_ini_data[MP12_BFGX_BT_CUST_INI_SIZE];
} mp12_bfgx_cali_data_stru;

#define MP12_BFGX_CALI_DATA_BUF_LEN  sizeof(mp12_bfgx_cali_data_stru)

extern bfgx_cali_data_ops_stru g_cali_data_ops_mp12;

#endif /* end of cali_data_ops_mp12.h */
