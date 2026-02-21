/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Driver Entry Function Declaration.
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#include "plat_common_clk.h"
#include "plat_firmware.h"
#include "board_mp15.h"

/* mp15 asic cfg file path */
#ifdef _PRE_WINDOWS_SUPPORT
#define BFGX_AND_WIFI_CFG_MP15_ASIC_PATH       "bfgx_and_wifi_cfg"
#define WIFI_CFG_MP15_ASIC_PATH                "wifi_cfg"
#define BFGX_CFG_MP15_ASIC_PATH                "bfgx_cfg"
#define RAM_CHECK_CFG_MP15_ASIC_PATH           "ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_MP15_ASIC_PATH      "reg_bcpu_mem_test_cfg"
#else
#define BFGX_AND_WIFI_CFG_MP15_ASIC_PATH       "/vendor/firmware/hi1105/pilot/bfgx_and_wifi_cfg"
#define WIFI_CFG_MP15_ASIC_PATH                "/vendor/firmware/hi1105/pilot/wifi_cfg"
#define BFGX_CFG_MP15_ASIC_PATH                "/vendor/firmware/hi1105/pilot/bfgx_cfg"
#endif

/* mp15 */
STATIC cfg_map_info g_mp15_asic_cfg_patch_in_vendor[] = {
    {
        .idx = BFGX_AND_WIFI_CFG,
        .cfg_path = BFGX_AND_WIFI_CFG_MP15_ASIC_PATH
    }, {
        .idx = WIFI_CFG,
        .cfg_path = WIFI_CFG_MP15_ASIC_PATH
    }, {
        .idx = BFGX_CFG,
        .cfg_path = BFGX_CFG_MP15_ASIC_PATH
    },
};

chip_firmware_cfg_str g_mp15_cfg_str[] = {
    {
        .chip_nr = BOARD_VERSION_MP15,
        .is_asic = VERSION_ASIC,
        .cfg_map_size = oal_array_size(g_mp15_asic_cfg_patch_in_vendor),
        .cfg_map = g_mp15_asic_cfg_patch_in_vendor,
        .pre_directory = NULL
    }
};

int32_t board_info_init_mp15(void)
{
    board_callback_init_dts();
    board_callback_init_mp13_power();
    board_plat_cust_init_mp13();
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    board_info_init_kunpeng();
#endif
    board_clk_init(DET_BY_UART, (CLK_INIT_INI | CLK_INIT_DTS));
    chip_firmware_cfg_register(g_mp15_cfg_str, oal_array_size(g_mp15_cfg_str));
    return BOARD_SUCC;
}