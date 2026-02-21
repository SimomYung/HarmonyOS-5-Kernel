/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : UAPSD hmac层处理
 * 作    者 :
 * 创建日期 : 2013年9月18日
 */

#include "hmac_rx_thread.h"
#include "hmac_wifi_delay.h"
#include "oal_ext_if.h"
#include "hmac_main.h"
#include "hmac_napi.h"
#include "hmac_rx.h"
#ifdef _PRE_WLAN_PKT_TIME_STAT
#include <hwnet/ipv4/wifi_delayst.h>
#endif
#if(_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#include "plat_pm_wlan.h"
#endif
#include "frw_task.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_RX_THREAD_C

oal_bool_enum_uint8 hmac_get_rxthread_enable(void)
{
    hmac_rxdata_thread_stru *rxdata_thread = hmac_get_rxdata_thread_addr();

    return rxdata_thread->en_rxthread_enable;
}

void hmac_rxdata_sched(void)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    hmac_rxdata_thread_stru *rxdata_thread = hmac_get_rxdata_thread_addr();
    up(&rxdata_thread->st_rxdata_sema);
#endif
    return;
}

void hmac_rxdata_netbuf_enqueue(oal_netbuf_stru *pst_netbuf)
{
    oal_netdev_priv_stru *pst_netdev_priv;

    pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_netbuf->dev);
    if (pst_netdev_priv->queue_len_max < oal_netbuf_list_len(&pst_netdev_priv->st_rx_netbuf_queue)) {
        oal_netbuf_free(pst_netbuf);
        return;
    }
    oal_netbuf_list_tail(&pst_netdev_priv->st_rx_netbuf_queue, pst_netbuf);
    g_hisi_softwdt_check.napi_enq_cnt++;
    pst_netdev_priv->period_pkts++;
}

/*
 * 功能描述  : 接收线程非napi出队
 * 1.日    期  : 2020年3月9日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_rxdata_netbuf_delist(oal_netdev_priv_stru *netdev_priv)
{
    oal_netbuf_stru *netbuf = NULL;

    netbuf = oal_netbuf_delist(&netdev_priv->st_rx_netbuf_queue);
    while (netbuf != NULL) {
#ifdef _PRE_WLAN_PKT_TIME_STAT
        if (DELAY_STATISTIC_SWITCH_ON && IS_NEED_RECORD_DELAY(netbuf, TP_SKB_HMAC_RX)) {
            skbprobe_record_time(netbuf, TP_SKB_HMAC_UPLOAD);
        }
#endif
#ifdef CONFIG_ATP_SERVICE_CLASSIFY
        atp_common_skb_proc_hook(netbuf, 0);
#endif
#ifdef _PRE_DELAY_DEBUG
        hmac_wifi_delay_rx_notify(netdev_priv->dev, netbuf);
#endif
        /* 清空cb */
#ifdef _PRE_WLAN_PKT_TIME_STAT
        memset_s(oal_netbuf_cb(netbuf), OAL_NETBUF_CB_ORIGIN, 0, OAL_NETBUF_CB_ORIGIN);
#else
        memset_s(oal_netbuf_cb(netbuf), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
#endif

#ifdef _PRE_WLAN_FEATURE_ESPE
        hmac_espe_recv(netbuf);
#else
        oal_netif_rx_ni(netbuf);
#endif
        netbuf = oal_netbuf_delist(&netdev_priv->st_rx_netbuf_queue);
    }
}

static void hmac_exec_rxdata_schedule_process_by_dev(uint32_t bus_dev)
{
    uint8_t uc_vap_idx, napi_sch;
    hmac_vap_stru *pst_hmac_vap = NULL;
    oal_netdev_priv_stru *pst_netdev_priv = NULL;
    mac_device_stru *pst_mac_device = mac_res_get_dev(bus_dev);

    if (pst_mac_device == NULL) {
        oal_io_print("[E]hmac_exec_rxdata_schedule_process_by_dev::pst_mac_device[%d] is NULL!\n", bus_dev);
        return;
    }

    for (uc_vap_idx = 0; uc_vap_idx < pst_mac_device->uc_vap_num; uc_vap_idx++) {
        pst_hmac_vap = mac_res_get_hmac_vap(pst_mac_device->auc_vap_id[uc_vap_idx]);
        if (oal_unlikely(pst_hmac_vap == NULL)) {
            continue;
        }

        if (pst_hmac_vap->pst_net_device == NULL) {
            continue;
        }

        pst_netdev_priv = (oal_netdev_priv_stru *)oal_net_dev_wireless_priv(pst_hmac_vap->pst_net_device);
        if (pst_netdev_priv == NULL) {
            continue;
        }

        if (oal_netbuf_list_len(&pst_netdev_priv->st_rx_netbuf_queue) == 0) {
            continue;
        }

        napi_sch = pst_netdev_priv->uc_napi_enable;
        if (napi_sch) {
            hmac_rxdata_update_napi_weight(pst_netdev_priv);
#ifdef _PRE_CONFIG_NAPI_DYNAMIC_SWITCH
            /* RR性能优化，当napi权重为1时，不做napi轮询处理 */
            if (pst_netdev_priv->uc_napi_weight <= 1) {
                napi_sch = 0;
            }
#endif
        }
        if (napi_sch) {
            g_hisi_softwdt_check.napi_sched_cnt++;
            oal_napi_schedule(&pst_netdev_priv->st_napi);
        } else {
            g_hisi_softwdt_check.netif_rx_cnt++;
            hmac_rxdata_netbuf_delist(pst_netdev_priv);
        }
    }
    g_hisi_softwdt_check.rxdata_cnt++;
}

static void hmac_exec_rxdata_schedule_process(void)
{
    uint32_t bus_dev = 0;
    uint32_t max_dev_num = oal_bus_get_chip_num();

    for (bus_dev = 0; bus_dev < max_dev_num; bus_dev++) {
        if (bus_dev >= HCC_CHIP_MAX_DEV) {
            oal_io_print("[E]hmac_exec_rxdata_schedule_process::bus_dev[%d] is exceed\n", bus_dev);
            return;
        }
        hmac_exec_rxdata_schedule_process_by_dev(bus_dev);
    }
}

int32_t hmac_rxdata_thread(void *p_data)
{
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
    struct sched_param param;
    hmac_rxdata_thread_stru *rxdata_thread = hmac_get_rxdata_thread_addr();

    param.sched_priority = 1;  /* 线程优先级 1，同hisi_hcc线程一致  */
    oal_set_thread_property(current, SCHED_FIFO, &param, -10); /* 线程静态优先级 nice  -10  范围-20到19  */
    allow_signal(SIGTERM);
    while (oal_likely(!down_interruptible(&rxdata_thread->st_rxdata_sema))) {
#else
    for (;;) {
#endif
#ifdef _PRE_WINDOWS_SUPPORT
        if (oal_kthread_should_stop((PRT_THREAD)p_data)) {
#else
        if (oal_kthread_should_stop()) {
#endif
            break;
        }
        hmac_exec_rxdata_schedule_process();
    }
    return OAL_SUCC;
}

uint32_t hmac_hisi_rxdata_thread_init(void)
{
#ifndef _PRE_WINDOWS_SUPPORT
    hmac_rxdata_thread_stru *rxdata_thread = hmac_get_rxdata_thread_addr();

    oal_wait_queue_init_head(&rxdata_thread->st_rxdata_wq);

    oal_netbuf_list_head_init(&rxdata_thread->st_rxdata_netbuf_head);
    oal_spin_lock_init(&rxdata_thread->st_lock);
    rxdata_thread->en_rxthread_enable = OAL_TRUE;
    rxdata_thread->pkt_loss_cnt = 0;
    rxdata_thread->uc_allowed_cpus = 0;

    rxdata_thread->pst_rxdata_thread = oal_thread_create(hmac_rxdata_thread, NULL, &rxdata_thread->st_rxdata_sema,
        "hisi_rxdata", SCHED_FIFO, 10, -1); // sched 10 prio

    hisi_thread_set_affinity(rxdata_thread->pst_rxdata_thread);
#endif
#if !defined(WIN32) || defined(_PRE_WINDOWS_SUPPORT)
    if (rxdata_thread->pst_rxdata_thread == NULL) {
        oal_io_print("hisi_rxdata thread create failed!\n");
        return OAL_FAIL;
    }
#endif
    return OAL_SUCC;
}

void hmac_hisi_rxdata_thread_exit(void)
{
#ifndef _PRE_WINDOWS_SUPPORT
    hmac_rxdata_thread_stru *rxdata_thread = hmac_get_rxdata_thread_addr();
    if (rxdata_thread->pst_rxdata_thread == NULL) {
        return;
    }
    oal_kthread_stop(rxdata_thread->pst_rxdata_thread);
    rxdata_thread->pst_rxdata_thread = NULL;
#endif
}
