/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Resource acquisition.
 * Author: @CompanyNameTag
 */

#ifndef BOARD_MP17C_H
#define BOARD_MP17C_H

/* 其他头文件包含 */
#include "plat_type.h"
#include "hw_bfg_ps.h"

/* 必须和芯片定义的 SHARE_MEM_PLAT_CUST_LEN 长度相同 */
#define SHARE_MEM_PLAT_CUST_LEN       (0x40 + 0x80)

typedef union {
    struct {
        /* 64 Byte 用于平台定制配置 */
        uint8_t release_version;
        uint8_t enter_rsmc;
        uint8_t gugong_fac_status;
        uint8_t vmin_mode;
        uint32_t buart_rate;
        uint32_t guart_rate;
        uint32_t pm_linkdown;
        uint8_t bg_tcxo_dll;
        uint8_t sensorhub_enable;
        uint8_t reserve1[46];   /* 46 Byte预留 */
        /* 128 Byte 用于 host cust节点数据下发 */
        struct {
            uint64_t config;
            uint64_t addr[8];   /* slv_monitor最大监控4段范围，即最多下发8个地址 */
        } slave_monitor;
        uint8_t reserve2[56];   /* 56 Byte预留 */
    } cfg;
    uint8_t data[SHARE_MEM_PLAT_CUST_LEN];
} plat_cust_data_mp17c;

/* 函数声明 */
int32_t board_info_init_mp17c(void);
uint32_t mp17c_pmu_ver_get(void);
#endif
