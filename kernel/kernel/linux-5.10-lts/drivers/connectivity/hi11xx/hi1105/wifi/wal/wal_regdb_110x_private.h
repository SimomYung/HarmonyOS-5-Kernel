/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : wal_regdb_110x_private.c 的头文件
 * 作    者 :
 * 创建日期 : 2024年05月15日
 */

#ifndef WAL_REGDB_110X_PRIVATE_H
#define WAL_REGDB_110X_PRIVATE_H

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_WLAN_PRIVATE_REGDD
#include "wal_regdb.h"
#include "hisi_ini.h"
#include "hisi_customize_wifi.h"

int32_t wal_regdb_find_db_from_private_file(int8_t *str);
oal_ieee80211_regdomain_stru *wal_get_private_regdom(void);
#endif
#endif

#endif