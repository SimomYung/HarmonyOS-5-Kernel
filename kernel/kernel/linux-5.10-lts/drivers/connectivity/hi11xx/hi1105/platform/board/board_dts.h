/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: board_dts.c header file
 * Author: @CompanyNameTag
 */

#ifndef BOARD_DTS_H
#define BOARD_DTS_H

/* 其他头文件包含 */
#include "plat_type.h"
#include "hw_bfg_ps.h"

/* 宏定义 */
#define PROC_NAME_GPIO_WLAN_FLOWCTRL  "hi110x_wlan_flowctrl"
#define INI_WLAN_WAKEUP_HOST_REVERSE "wlan_wakeup_host_have_reverser"

/* test ssi write bcpu code */
/* EXTERN VARIABLE */
#ifdef PLATFORM_DEBUG_ENABLE
extern int32_t g_device_monitor_enable;
#endif

/* 函数声明 */
void board_callback_init_dts(void);
int32_t mpxx_get_ini_file_name_from_dts(const char *dts_prop, char *prop_value, uint32_t size);
#if defined(CONFIG_IODIE_HKADC)
bool mpxx_ncid_supported(void);
int32_t mpxx_get_ini_file_name_by_ncid(uint32_t id, const char *dts_prop, char *prop_value, uint32_t size);
int32_t mpxx_get_ncid_from_dts(int32_t *ncid);
int32_t mpxx_get_support_ncid_from_dts(int32_t *supp_ncids, int32_t max_supp_num, int32_t *supp_num);
#endif
#endif
