/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Driver Entry Function Declaration.
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#include "plat_firmware.h"
#include "plat_common_clk.h"
#include "plat_debug.h"
#include "plat_cust.h"
#include "board_mp16.h"

#ifndef FIRMWARE_MP16_PILOT_CFG_DIR
#define FIRMWARE_MP16_PILOT_CFG_DIR        "/vendor/firmware/shenkuo/pilot"
#endif

/* 必须和芯片定义的PLAT_CUST_LEN长度相同 */
#define PLAT_CUST_LEN       0x10

typedef union {
    struct {
        uint8_t slave_monitor;
    } cfg;
    uint8_t data[PLAT_CUST_LEN];
} plat_cust_data_mp16;

/* mp16 */
STATIC cfg_map_info g_mp16_asic_cfg_patch_in_vendor[] = {
    {
        .idx = BFGX_AND_WIFI_CFG,
        .cfg_path = BFGX_AND_WIFI_CFG_FILE
    }, {
        .idx = WIFI_CFG,
        .cfg_path = WIFI_CFG_FILE
    }, {
        .idx = BFGX_CFG,
        .cfg_path = BFGX_CFG_FILE
    }, {
        .idx = RAM_REG_TEST_CFG,
        .cfg_path = RAM_WIFI_CHECK_FILE
    }
};

chip_firmware_cfg_str g_mp16_cfg_str[] = {
    {
        .chip_nr = BOARD_VERSION_MP16,
        .is_asic = VERSION_ASIC,
        .cfg_map_size = oal_array_size(g_mp16_asic_cfg_patch_in_vendor),
        .cfg_map = g_mp16_asic_cfg_patch_in_vendor,
        .pre_directory = FIRMWARE_MP16_PILOT_CFG_DIR
    }
};

STATIC void board_get_slave_monitor(plat_cust_data_mp16 *cust)
{
    int32_t cfg_value;
    int32_t ret;

    cust->cfg.slave_monitor = OAL_FALSE;

    /* 获取ini的配置值 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "pcie_slave_monitor_switch", &cfg_value);
    if (ret == INI_FAILED) {
        return;
    }

    cust->cfg.slave_monitor = cfg_value;
    ps_print_info("get slave_monitor =%d\n", cust->cfg.slave_monitor);
}

/*
 * host定制化下发至dev
 */
void board_plat_cust_init_mp16(void)
{
    int32_t ret;
    plat_cust_data_mp16 plat_cust;

    memset_s(&plat_cust, sizeof(plat_cust), 0, sizeof(plat_cust));
    board_get_slave_monitor(&plat_cust);
    oal_print_hex_dump((uint8_t *)&plat_cust, 0x10, HEX_DUMP_GROUP_SIZE, "buff: ");
    ret = set_plat_cust_buf((uint8_t *)&plat_cust, sizeof(plat_cust_data_mp16));
    if (ret != SUCC) {
        ps_print_err("set plat cust buf fail, ret = %d\n", ret);
        return;
    }
    ps_print_info("set plat cust buf succ, vtype=%d\n", hi110x_get_release_type());
}

int32_t board_info_init_mp16(void)
{
    board_callback_init_dts();
    board_callback_init_mp13_power();
    board_plat_cust_init_mp16();
    board_clk_init(DET_BY_UART, (CLK_INIT_INI | CLK_INIT_DTS));
    chip_firmware_cfg_register(g_mp16_cfg_str, oal_array_size(g_mp16_cfg_str));
    return BOARD_SUCC;
}