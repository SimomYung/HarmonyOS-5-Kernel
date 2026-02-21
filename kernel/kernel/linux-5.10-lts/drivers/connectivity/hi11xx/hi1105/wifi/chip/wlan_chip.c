/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : WIFI 芯片差异文件
 * 作    者 :
 * 创建日期 : 2020年6月19日
 */

#include "oneimage.h"
#include "wlan_chip.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "board.h"
#endif
#include "oam_ext_if.h"
#include "dpe_wlan_resource.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_WLAN_CHIP_C

const struct wlan_chip_ops g_wlan_chip_dummy_ops = { NULL, };
const struct wlan_chip_ops *g_wlan_chip_ops = &g_wlan_chip_dummy_ops;

#ifndef _PRE_WLAN_EXPORT
static void wlan_chip_ops_init_mp16c(void)
{
    if (get_mpxx_subchip_version() == CHIP_VERSION_PILOT) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp16c;
    } else {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp17c;
    }
}
#endif

uint32_t wlan_chip_ops_init(void)
{
    /* 根据不同芯片类型，挂接不同回调函数 */
    int32_t chip_type = get_mpxx_subchip_type();
    if (chip_type == BOARD_VERSION_MP13) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp13;
    } else if (chip_type == BOARD_VERSION_MP15) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp15;
#ifndef _PRE_WLAN_EXPORT
    } else if (chip_type == BOARD_VERSION_MP16) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp16;
    } else if (chip_type == BOARD_VERSION_MP16C) {
        wlan_chip_ops_init_mp16c();
    } else if (chip_type == BOARD_VERSION_GF61) {
        g_wlan_chip_ops = &g_wlan_chip_ops_gf61;
    } else if (chip_type == BOARD_VERSION_MP17C) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp17c;
    } else if (chip_type == BOARD_VERSION_MP12) {
        g_wlan_chip_ops = &g_wlan_chip_ops_mp12;
#endif
    } else {
        g_wlan_chip_ops = &g_wlan_chip_dummy_ops;
        oam_error_log1(0, OAM_SF_ANY, "wlan_chip_ops_init: wifi unsupport chitype!%d\n",
            get_mpxx_subchip_type());
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

int32_t wlan_chip_feature_init(void)
{
    const oal_wlan_cfg_stru *spec_cfg = NULL;
    int32_t chip = get_mpxx_subchip_type();
    if (chip == BOARD_VERSION_MP13) {
        spec_cfg = &g_wlan_spec_cfg_mp13;
    } else if (chip == BOARD_VERSION_MP15) {
        spec_cfg = &g_wlan_spec_cfg_mp15;
#ifndef _PRE_WLAN_EXPORT
    } else if (chip == BOARD_VERSION_MP16) {
        spec_cfg = &g_wlan_spec_cfg_mp16;
    } else if (chip == BOARD_VERSION_MP16C) {
        if (get_mpxx_subchip_version() == CHIP_VERSION_PILOT) {
            spec_cfg = &g_wlan_spec_cfg_mp16c;
        } else {
            spec_cfg = &g_wlan_spec_cfg_mp17c_mpw;
        }
    } else if (chip == BOARD_VERSION_GF61) {
        spec_cfg = &g_wlan_spec_cfg_gf61;
    } else if (chip == BOARD_VERSION_MP17C) {
        spec_cfg = &g_wlan_spec_cfg_mp17c;
    } else if (chip == BOARD_VERSION_MP12) {
        spec_cfg = &g_wlan_spec_cfg_mp12;
#endif
    } else {
        oal_io_print("wifi unsupport chitype!\n");
        return -OAL_EFAIL;
    }
    g_wlan_spec_cfg = spec_cfg;
    dpe_wlan_spec_cfg_init(spec_cfg);
    return OAL_SUCC;
}
