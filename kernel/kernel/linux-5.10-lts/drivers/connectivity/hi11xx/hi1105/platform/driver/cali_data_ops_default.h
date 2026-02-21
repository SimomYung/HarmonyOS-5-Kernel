/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: plat_cali.c header file
 * Author: @CompanyNameTag
 */

#ifndef CALI_DATA_OPS_DEFAULT_H
#define CALI_DATA_OPS_DEFAULT_H

/* 头文件包含 */
#include "plat_type.h"
#include "oal_types.h"
#include "oal_util.h"

/* 宏定义 */
/* 以下5个宏定义，如果要修改长度，需要同步修改device的宏定义 */
#define MPXX_BFGX_BT_CALI_DATA_SIZE             492
#define MPXX_WIFI_CALI_DATA_FOR_FM_RC_CODE_SIZE 20
#define MPXX_BFGX_NV_DATA_SIZE                  128
#define MPXX_BFGX_BT_CUST_INI_SIZE              512
#define MPXX_WIFI_CALI_DATA_FOR_BT              768
#define MPXX_BFGX_EXT_NV_DATA_SIZE              128
/* 考虑结构体总体长度考虑SDIO下载长度512对齐特性，这里长度为2048 */
typedef struct {
    uint8_t auc_bfgx_data[MPXX_BFGX_BT_CALI_DATA_SIZE];
    uint8_t auc_wifi_rc_code_data[MPXX_WIFI_CALI_DATA_FOR_FM_RC_CODE_SIZE];
    uint8_t auc_nv_data[MPXX_BFGX_NV_DATA_SIZE];
    uint8_t auc_bt_cust_ini_data[MPXX_BFGX_BT_CUST_INI_SIZE];
    uint8_t auc_wifi_cali_for_bt_data[MPXX_WIFI_CALI_DATA_FOR_BT];
    uint8_t auc_ext_nv_data[MPXX_BFGX_EXT_NV_DATA_SIZE];
} mpxx_bfgx_cali_data_stru;

#define MPXX_BFGX_CALI_DATA_BUF_LEN (sizeof(mpxx_bfgx_cali_data_stru))
extern bfgx_cali_data_ops_stru g_cali_data_ops_mpxx;
#endif /* end of cali_data_ops_default.h */
