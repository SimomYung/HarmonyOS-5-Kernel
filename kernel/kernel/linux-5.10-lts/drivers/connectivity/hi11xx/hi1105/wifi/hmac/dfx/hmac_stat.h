/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : WIFI HOST统计接口文件
 * 作    者 :
 * 创建日期 : 2020年7月21日
 */

#ifndef HMAC_STAT_H
#define HMAC_STAT_H

#include "hmac_stat_stru.h"
#include "hmac_vap.h"
#include "hmac_user.h"
#include "dpe_wlan_stat.h"
#ifdef _PRE_WLAN_CHBA_MGMT
#include "hmac_chba_mgmt.h"
#include "hmac_chba_function.h"
#endif

#define HMAC_VAP_STATS_PKT_INCR(_member, _cnt) ((_member) += (_cnt))

struct hmac_device_tag;

void hmac_stat_init_device_screen_stat(struct hmac_device_tag *hmac_device);

extern hmac_lp_miracast_stat_stru g_hmac_lp_miracast_stats;

OAL_STATIC OAL_INLINE hmac_lp_miracast_stat_stru *hmac_stat_get_lp_miracast_stats(void)
{
    return &g_hmac_lp_miracast_stats;
}

/* 后续需要迁移至dpe目录，否则每包统计，函数的调用会影响mips */
OAL_STATIC OAL_INLINE void hmac_stat_user_rx_netbuf(hmac_user_stru *hmac_user, oal_netbuf_stru *netbuf)
{
#ifndef WIN32
    hmac_vap_stru *hmac_vap = NULL;
    if ((hmac_user == NULL) || (netbuf == NULL)) {
        return;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
#ifndef _PRE_WLAN_VAP_PROC
    if (!hmac_chba_vap_start_check(hmac_vap)) {
        return;
    }
#endif
    dpe_wlan_stat_user_rx_netbuf(hmac_user->st_user_base_info.us_assoc_id, netbuf);
#endif
}
#endif /* HMAC_STAT_H */

