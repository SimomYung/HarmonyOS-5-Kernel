/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_cali_arr_init.c 的头文件
 * 创建日期 : 2020年10月10日
 */

#ifndef HMAC_CALI_ARR_INIT_H
#define HMAC_CALI_ARR_INIT_H

#include "hmac_cali_mgmt.h"
#include "wlan_cali.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_CALI_ARR_INIT_H


extern uint32_t g_dpd_corram_data_128bits[WLAN_CALI_DPD_CORRAM_DATA_128BITS_LEN];
extern uint32_t g_dpd_corram_data_64bits[WLAN_CALI_DPD_CORRAM_DATA_64BITS_LEN];
extern int16_t g_as_rxiq_h_comp_80m[MP13_CALI_RXIQ_LS_FILTER_TAP_NUM_80M][MP13_CALI_RXIQ_LS_FILTER_FEQ_NUM_80M];
extern int16_t g_as_rxiq_h_comp_320m[MP13_CALI_RXIQ_LS_FILTER_TAP_NUM_320M][MP13_RXIQ_LSFIR_FEQ_NUM_320M];
extern int16_t g_as_rxiq_bw80m_siso_h_comp_320m[MP13_CALI_RXIQ_LS_FILTER_TAP_NUM_320M][MP13_RXIQ_LSFIR_FEQ_NUM_320M];

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
