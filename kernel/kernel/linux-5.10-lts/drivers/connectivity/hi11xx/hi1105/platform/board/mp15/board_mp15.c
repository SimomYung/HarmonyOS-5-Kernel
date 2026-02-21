/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Driver Entry Function Declaration.
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#include <linux/platform_device.h>
#include "plat_common_clk.h"
#include "plat_firmware.h"
#include "bfgx_core.h"
#include "plat_uart.h"
#include "board_mp15.h"
#ifdef _PRE_DUAL_CHIP
#include "board_dual_mp15.h"
#endif

/* mp15 asic cfg file path */
#ifdef _PRE_WINDOWS_SUPPORT
#define BFGX_AND_WIFI_CFG_MP15_ASIC_PATH       "bfgx_and_wifi_cfg"
#define WIFI_CFG_MP15_ASIC_PATH                "wifi_cfg"
#define BFGX_CFG_MP15_ASIC_PATH                "bfgx_cfg"
#define RAM_CHECK_CFG_MP15_ASIC_PATH           "ram_reg_test_cfg"
#define RAM_BCPU_CHECK_CFG_MP15_ASIC_PATH      "reg_bcpu_mem_test_cfg"
#else
#define BFGX_AND_WIFI_CFG_MP15_ASIC_PATH       "/vendor/firmware/hi1105/pilot/bfgx_and_wifi_cfg"
#if ((defined _PRE_WLAN_EXPORT) || (defined _PRE_DUAL_CHIP))
#define WIFI_ONLT_CFG_MP15_ASIC_PATH                "/vendor/firmware/hi1105/pilot/wifi_only_cfg"
#endif
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
/* 产品单独加载WIFI，BFG不上电时，复用WIFI_COMB_CFG加载wifi_only_cfg */
#if ((defined _PRE_WLAN_EXPORT) || (defined _PRE_DUAL_CHIP))
    {
        .idx = WIFI_COMB_CFG,
        .cfg_path = WIFI_ONLT_CFG_MP15_ASIC_PATH
    },
#endif
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

#ifdef _PRE_WLAN_EXPORT
#ifndef _PRE_CONFIG_USE_DTS
STATIC struct ps_private_data bfgx_ps_private_data = {
    .v = {
        .uart_name = "/dev/ttyAMA1",
        .uart_index = BUART,
        .flow_cntrl = FLOW_CTRL_ENABLE,
        .ldisc_num = N_HW_BFG,
        .baud_rate = UART_BAUD_RATE_1M,
        .paren = OAL_FALSE,
    },
};

STATIC struct platform_device g_ps_platform_device = {
    .name = "hisi_bfgx",
    .id   = 0,
    .dev  = {
        .platform_data = &bfgx_ps_private_data,
    },
};
#endif

void STATIC mp15_export_config(void)
{
#ifdef _FEATURE_NO_BFGX
    g_st_board_info.is_bfgx_disable = 1;
#endif
#ifndef _PRE_CONFIG_USE_DTS
    platform_device_register(&g_ps_platform_device);
#endif
}
#endif

int32_t board_info_init_mp15(void)
{
    board_callback_init_dts();
    board_callback_init_mp13_power();
#ifdef _PRE_DUAL_CHIP
    board_callback_init_dual_mp15_power();
#endif
    board_plat_cust_init_mp13();
#ifdef _PRE_PRODUCT_HI1620S_KUNPENG
    board_info_init_kunpeng();
#endif
    board_clk_init(DET_BY_UART, (CLK_INIT_INI | CLK_INIT_DTS));
    chip_firmware_cfg_register(g_mp15_cfg_str, oal_array_size(g_mp15_cfg_str));
#ifdef _PRE_WLAN_EXPORT
    mp15_export_config();
#endif
    return BOARD_SUCC;
}