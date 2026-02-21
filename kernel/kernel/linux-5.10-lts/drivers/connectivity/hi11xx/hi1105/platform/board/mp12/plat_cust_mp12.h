/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: Customization Structure
 * Author: @CompanyNameTag
 */

#ifndef PLAT_CUST_MP12_H
#define PLAT_CUST_MP12_H

/* 其他头文件包含 */
#include "plat_type.h"

/* 必须和芯片定义的DTCM_PLAT_CUST_LEN长度相同 */
#define DTCM_PLAT_CUST_LEN       0x10

typedef union {
    struct {
        uint8_t release_version; // true:relese; false: other
        uint8_t enter_rsmc;
        uint8_t gugong_fac_status;
        uint8_t vmin_mode;
        uint32_t buart_rate;
        uint32_t gnss_dpe_switch;
        uint32_t slave_monitor;
        uint8_t sensorhub_enable;
    } cfg;
    uint8_t data[DTCM_PLAT_CUST_LEN];
} plat_cust_data_mp12;
#endif