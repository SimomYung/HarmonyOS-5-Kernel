/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : MinTP
 * 作    者 : wifi
 * 创建日期 : 2023年1月7日
 */

#include "hmac_mintp.h"
#include "hmac_mintp_log.h"
#include "hmac_tx_data.h"
#include "hmac_host_tx_data.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_data.h"
#include "hmac_stat.h"
#include "mac_data.h"
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include <linux/pkt_sched.h>
#include <linux/netdevice.h>
#endif

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_MINTP_C

typedef struct {
    oal_netbuf_stru *netbuf;
    mac_tx_ctl_stru *tx_ctl;
    mac_vap_stru *mac_vap;
    hmac_vap_stru *hmac_vap;
    hmac_user_stru *hmac_user;
} hmac_mintp_tx_context_stru;
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
static uint8_t hmac_mintp_get_tid(uint32_t priority)
{
    if (priority == TC_PRIO_BULK) {
        return WLAN_TIDNO_BACKGROUND;
    } else if (priority == TC_PRIO_INTERACTIVE_BULK) {
        return WLAN_TIDNO_VIDEO;
    } else if (priority == TC_PRIO_INTERACTIVE) {
        return WLAN_TIDNO_VOICE;
    } else {
        return WLAN_TIDNO_BEST_EFFORT;
    }
}
#endif
static void hmac_mintp_tx_cb_init(hmac_mintp_tx_context_stru *ctx)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    uint32_t priority = ctx->netbuf->priority;
#endif
    uint8_t tid = 0;
    mac_ether_header_stru *ether_hdr = (mac_ether_header_stru *)oal_netbuf_data(ctx->netbuf);

    ctx->tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(ctx->netbuf);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    tid = hmac_mintp_get_tid(priority);
#endif

    if (ctx->tx_ctl == NULL) {
        return;
    }
    memset_s(ctx->tx_ctl, oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    mac_get_cb_tx_user_idx(ctx->tx_ctl) = hmac_tx_get_user_id(ctx->mac_vap, ether_hdr);
    mac_get_cb_tx_vap_index(ctx->tx_ctl) = ctx->mac_vap->uc_vap_id;
    mac_get_cb_wme_tid_type(ctx->tx_ctl) = tid;
    mac_get_cb_wme_ac_type(ctx->tx_ctl) = WLAN_WME_TID_TO_AC(tid);
    mac_get_cb_data_type(ctx->tx_ctl) = DATA_TYPE_ETH;
    mac_get_cb_mpdu_num(ctx->tx_ctl) = 1;
    mac_get_cb_netbuf_num(ctx->tx_ctl) = 1;
    mac_get_cb_frame_type(ctx->tx_ctl) = WLAN_CB_FRAME_TYPE_DATA;
    mac_get_cb_frame_subtype(ctx->tx_ctl) = MAC_DATA_NUM;
}

static void hmac_mintp_tx_free_netbuf_list(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;
    oal_netbuf_stru *prev = netbuf;

    do {
        next = hmac_mintp_tx_get_next_netbuf(prev);
        hmac_mintp_tx_set_next_netbuf(prev, NULL);
        oal_netbuf_free(prev);
        prev = next;
    } while (next);
}


static void hmac_mintp_tx_netbuf_init(hmac_mintp_tx_context_stru *ctx, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;
    oal_netbuf_stru *prev = netbuf;

    while ((next = hmac_mintp_tx_get_next_netbuf(prev)) != NULL) {
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0))
        local_bh_disable();
        dev_queue_xmit_nit(next, next->dev);
        local_bh_enable();
#endif
        if (memcpy_s(oal_netbuf_cb(next), oal_netbuf_cb_size(), ctx->tx_ctl, oal_netbuf_cb_size()) != EOK) {
            hmac_mintp_tx_free_netbuf_list(next);
            break;
        }
        hmac_mintp_tx_set_next_netbuf(prev, NULL);
        oal_netbuf_next(prev) = next;
        prev = next;
    }

    oal_netbuf_next(prev) = NULL;
}

static uint32_t hmac_mintp_tx_build_context(
    oal_netbuf_stru *netbuf, oal_net_device_stru *net_device, hmac_mintp_tx_context_stru *ctx)
{
    ctx->mac_vap = (mac_vap_stru *)oal_net_dev_priv(net_device);
    if (oal_unlikely(!ctx->mac_vap)) {
        return OAL_FAIL;
    }

    ctx->hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(ctx->mac_vap->uc_vap_id);
    if (oal_unlikely(!ctx->hmac_vap)) {
        return OAL_FAIL;
    }

    ctx->netbuf = netbuf;
    hmac_mintp_tx_cb_init(ctx);

    if (ctx->tx_ctl == NULL) {
        return OAL_FAIL;
    }
    ctx->hmac_user = mac_res_get_hmac_user(mac_get_cb_tx_user_idx(ctx->tx_ctl));
    if (oal_unlikely(!ctx->hmac_user)) {
        return OAL_FAIL;
    }

    hmac_mintp_log((uint8_t *)netbuf, MINTP_LOG_LVL_2, MINTP_LOG_DATATYPE_SKB_HEAD_LIST, MINTP_LOG_TYPE_TX);
    hmac_mintp_tx_netbuf_init(ctx, netbuf);

    return OAL_SUCC;
}

static uint32_t hmac_mintp_tx_preprocess(hmac_mintp_tx_context_stru *ctx)
{
    uint8_t tid_no = mac_get_cb_wme_tid_type(ctx->tx_ctl);

    if (hmac_tx_filter_security(ctx->hmac_vap, ctx->netbuf, ctx->hmac_user) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (!hmac_get_tx_ring_inited(ctx->hmac_user->st_user_base_info.us_assoc_id, tid_no)) {
        return OAL_FAIL;
    }

    if (hmac_tid_need_ba_session(ctx->hmac_vap, ctx->hmac_user, tid_no) == OAL_TRUE) {
        hmac_tx_ba_setup(ctx->hmac_vap, ctx->hmac_user, tid_no);
    }

    return OAL_SUCC;
}

static void hmac_mintp_tx_stat(hmac_mintp_tx_context_stru *ctx, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;
    oal_netbuf_stru *prev = netbuf;

    do {
        next = oal_netbuf_next(prev);
#ifdef _PRE_WLAN_CHBA_MGMT
        /* 用户帧统计 */
#ifndef _PRE_WLAN_VAP_PROC
        if (hmac_chba_vap_start_check(ctx->hmac_vap) == OAL_TRUE) {
            hmac_stat_user_tx_netbuf(ctx->hmac_user->st_user_base_info.us_assoc_id, prev);
        }
#else
        hmac_stat_user_tx_netbuf(ctx->hmac_user->st_user_base_info.us_assoc_id, prev);
#endif
#endif
        hmac_stat_device_tx_msdu(oal_netbuf_len(prev));
        prev = next;
    } while (next);
}

/* Ring TX使能时的MINTP通路, Ring TX由调用者负责判断 */
oal_net_dev_tx_enum hmac_bridge_vap_xmit_mintp(oal_netbuf_stru *netbuf, oal_net_device_stru *net_device)
{
    hmac_mintp_tx_context_stru ctx = { 0 };

    if (oal_unlikely(oal_any_null_ptr2(netbuf, net_device))) {
        return OAL_NETDEV_TX_OK;
    }

    if (hmac_mintp_tx_build_context(netbuf, net_device, &ctx) != OAL_SUCC) {
        hmac_mintp_tx_free_netbuf_list(netbuf);
        return OAL_NETDEV_TX_OK;
    }

    hmac_mintp_tx_stat(&ctx, ctx.netbuf);

    if (hmac_mintp_tx_preprocess(&ctx) != OAL_SUCC ||
        hmac_host_tx_data(ctx.hmac_vap->st_vap_base_info.uc_vap_id,
            ctx.hmac_user->st_user_base_info.us_assoc_id, ctx.netbuf) != OAL_SUCC) {
        hmac_free_netbuf_list(netbuf);
    }

    return OAL_NETDEV_TX_OK;
}
