/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : hmac_emlsr.c 的头文件
 * 作    者 :
 * 创建日期 : 2012年9月18日
 */

#ifndef HMAC_EMLSR_H
#define HMAC_EMLSR_H

/* 1 其他头文件包含 */
#include "oam_ext_if.h"
#include "hmac_ext_if.h"
#include "oal_mem.h"
#include "frw_ext_if.h"
#include "frw_event_main.h"

#ifdef __cplusplus // windows ut编译不过，后续下线清理
#if __cplusplus
extern "C" {
#endif
#endif

void hmac_emlsr_ram_init(mac_vap_stru *mac_vap);
uint32_t hmac_emlsr_switch_event(frw_event_mem_stru *event_mem);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of hmac_emlsr */

