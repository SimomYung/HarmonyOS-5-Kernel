/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : UAPSD hmac层处理
 * 作    者 :
 * 创建日期 : 2013年9月18日
 */

#include "mac_vap.h"
#include "mac_frame.h"
#include "hmac_tx_complete.h"
#include "hmac_encap_frame_ap.h"
#include "hmac_mgmt_bss_comm.h"
#include "hmac_rx_data.h"
#include "hmac_uapsd.h"
#include "hmac_config.h"
#include "securec.h"
#include "hmac_tid_sche.h"
#include "hmac_tx_data.h"
#include "hmac_rx_thread.h"
#if (defined(_PRE_WLAN_FEATURE_WAPI) || defined(_PRE_WLAN_FEATURE_PWL))
#include "hmac_soft_encrypt.h"
#endif
#include "hmac_rx_complete.h"
#include "frw_task.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_THREAD_C

/*
 * 功能描述 : tid调度线程init
 *            本线程用于在host tx时调度各user tid, 将其中缓存的netbuf入ring发送
  * 1.日    期 : 2020年7月29日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC uint32_t hmac_tid_schedule_thread_init(void)
{
#ifndef WIN32
    hmac_tid_schedule_stru *tid_schedule = NULL;

    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        return OAL_SUCC;
    }

    tid_schedule = hmac_get_tid_schedule_list();
#ifdef _PRE_WLAN_WAIT_QUEUE_THREAD_SCHDULE
    oal_wait_queue_init_head(&tid_schedule->tx_sch_wq);
    tid_schedule->tid_schedule_thread = oal_thread_create(hmac_tid_schedule_thread, tid_schedule,
        NULL, "hisi_tx_sch", SCHED_FIFO, 98, -1); // sched 98 prio
#else
    tid_schedule->tid_schedule_thread = oal_thread_create(hmac_tid_schedule_thread, NULL,
        &tid_schedule->tid_sche_sema, "hisi_tx_sch", SCHED_FIFO, 98, -1); // sched 98 prio
#endif
    if (tid_schedule->tid_schedule_thread == NULL) {
        return OAL_FAIL;
    }
#endif

    return OAL_SUCC;
}

/*
 * 功能描述 : tid调度线程exit
  * 1.日    期 : 2020年7月29日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tid_schedule_thread_exit(void)
{
#ifndef WIN32
    hmac_tid_schedule_stru *tid_schedule = NULL;

    if (hmac_ring_tx_enabled() != OAL_TRUE) {
        return;
    }

    tid_schedule = hmac_get_tid_schedule_list();
    if (tid_schedule->tid_schedule_thread == NULL) {
        return;
    }

    oal_kthread_stop(tid_schedule->tid_schedule_thread);
    tid_schedule->tid_schedule_thread = NULL;
#endif
}

#if (defined(_PRE_WLAN_FEATURE_WAPI) || defined(_PRE_WLAN_FEATURE_PWL))
/*
 * 功能描述 : 软件加密调度线程init
 *           本线程用于在host tx时将缓存待加密的netbuf做加密处理后抛事件发送
 * 1.日    期 : 2022年12月1日
 *   修改内容 : 新生成函数
 */
OAL_STATIC uint32_t hmac_soft_encrypt_schedule_thread_init(void)
{
#ifndef WIN32
    hmac_encrypt_schedule_stru *encrypt_schedule = NULL;
    const int32_t prio = 98;
    const int32_t cpu_id = -1;

    encrypt_schedule = hmac_get_encrypt_schedule_list();
    encrypt_schedule->encrypt_schedule_thread =
        oal_thread_create(hmac_soft_encrypt_schedule_thread, NULL, &encrypt_schedule->encrypt_sche_sema,
                          "hisi_encry", SCHED_FIFO, prio, cpu_id);

    if (encrypt_schedule->encrypt_schedule_thread == NULL) {
        return OAL_FAIL;
    }
    oal_netbuf_list_head_init(&encrypt_schedule->netbuf_header);
#endif

    return OAL_SUCC;
}

/*
 * 功能描述 : 软件加密调度线程exit
 * 1.日    期 : 2022年12月1日
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_soft_encrypt_schedule_thread_exit(void)
{
#ifndef WIN32
    hmac_encrypt_schedule_stru *encrypt_schedule = NULL;

    /* 支持hw wapi/pwl时,不需要注册线程做软件处理 */
    if (wlan_chip_is_support_hw_wapi() == OAL_TRUE) {
        return;
    }
    encrypt_schedule = hmac_get_encrypt_schedule_list();
    if (encrypt_schedule->encrypt_schedule_thread == NULL) {
        return;
    }

    oal_thread_stop(encrypt_schedule->encrypt_schedule_thread, &encrypt_schedule->encrypt_sche_sema);
    encrypt_schedule->encrypt_schedule_thread = NULL;
#endif
}
#endif

static uint32_t hmac_tx_comp_thread_init(void)
{
#ifndef WIN32
    hmac_tx_comp_stru *tx_comp = NULL;

    if (!hmac_host_ring_tx_enabled()) {
        return OAL_SUCC;
    }

    tx_comp = hmac_get_tx_comp_mgmt();
    tx_comp->tx_comp_thread = oal_thread_create(hmac_tx_comp_thread, NULL, &tx_comp->tx_comp_sema, "hisi_tx_comp",
                                                SCHED_FIFO, 98, -1); // sched 98 prio

    if (tx_comp->tx_comp_thread == NULL) {
        return OAL_FAIL;
    }

    hmac_tx_comp_init();
#endif

    return OAL_SUCC;
}

static void hmac_tx_comp_thread_exit(void)
{
#ifndef WIN32
    hmac_tx_comp_stru *tx_comp = NULL;

    if (!hmac_host_ring_tx_enabled()) {
        return;
    }

    tx_comp = hmac_get_tx_comp_mgmt();
    if (tx_comp->tx_comp_thread == NULL) {
        return;
    }

    oal_kthread_stop(tx_comp->tx_comp_thread);
    tx_comp->tx_comp_thread = NULL;
#endif
}

static uint32_t hmac_rx_comp_thread_init(void)
{
#ifndef WIN32
    hmac_rx_comp_stru *rx_comp = hmac_get_rx_comp_mgmt();

    rx_comp->rx_comp_thread = oal_thread_create(hmac_rx_comp_thread, NULL, &rx_comp->rx_comp_sema, "hisi_rx_comp",
                                                SCHED_FIFO, 99, -1); // sched 99 prio

    if (rx_comp->rx_comp_thread == NULL) {
        return OAL_FAIL;
    }
    hmac_rx_comp_init();
#endif

    return OAL_SUCC;
}
static void hmac_rx_comp_thread_exit(void)
{
#ifndef WIN32
    hmac_rx_comp_stru *rx_comp = NULL;

    rx_comp = hmac_get_rx_comp_mgmt();
    if (rx_comp->rx_comp_thread == NULL) {
        return;
    }

    oal_thread_stop(rx_comp->rx_comp_thread, &rx_comp->rx_comp_sema);
    rx_comp->rx_comp_thread = NULL;
#endif
}

uint32_t hmac_trx_thread_init(void)
{
    if (hmac_tid_schedule_thread_init() != OAL_SUCC) {
        oal_io_print("hisi_tid_schedule thread create failed!\n");
        return OAL_FAIL;
    }

    if (hmac_tx_comp_thread_init() != OAL_SUCC) {
        oal_io_print("hisi_tx_comp thread create failed!\n");
        return OAL_FAIL;
    }

    if (hmac_rx_comp_thread_init() != OAL_SUCC) {
        oal_io_print("hisi_rx_comp thread create failed!\n");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
uint32_t hmac_hisi_hcc_thread_init(void)
{
    struct hcc_handler *hcc = NULL;
    uint32_t bus_dev = 0;
    uint32_t max_dev_num = oal_bus_get_chip_num();
    int8_t hcc_transfer_thread_name[NUM_32_BYTES] = {"hisi_hcc"};
    int8_t bus_dev_id_str[NUM_5_BYTES] = {0};

    for (bus_dev = 0; bus_dev < max_dev_num; bus_dev++) {
        if (bus_dev >= HCC_CHIP_MAX_DEV) {
            oal_io_print("hcc thread create failed!:bus_dev[%d] is exceed\n", bus_dev);
            return OAL_FAIL;
        }

#if defined(_PRE_WLAN_TCP_OPT) && !defined(WIN32)
        hmac_set_hmac_tcp_ack_process_func(bus_dev, hmac_tcp_ack_process);
        hmac_set_hmac_tcp_ack_need_schedule(bus_dev, hmac_tcp_ack_need_schedule);

        hcc = hcc_get_handler(bus_dev);
        if (hcc == NULL) {
            oal_io_print("hcc thread create failed!:hcc[%d] is NULL!\n", bus_dev);
            return OAL_FAIL;
        }

        if (bus_dev > HCC_EP_WIFI_DEV) {
            oal_itoa(bus_dev, bus_dev_id_str, sizeof(bus_dev_id_str));
            if (strcat_s(hcc_transfer_thread_name, sizeof(hcc_transfer_thread_name), bus_dev_id_str) != EOK) {
                oal_io_print("hcc_transfer_thread_name bus_dev[%d] strcat fail!", bus_dev);
                return OAL_FAIL;
            }
        }

        hcc->hcc_transer_info.hcc_transfer_thread = oal_thread_create(hcc_transfer_thread, hcc,
                NULL, hcc_transfer_thread_name, HCC_TRANS_THREAD_POLICY, HCC_TRANS_THERAD_PRIORITY, -1);
        hisi_thread_set_affinity(hcc->hcc_transer_info.hcc_transfer_thread);
        if (!hcc->hcc_transer_info.hcc_transfer_thread) {
            oal_io_print("hcc thread create failed!:hcc_transfer_thread[%d] is NULL\n", bus_dev);
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
            mutex_destroy(&hcc->tx_transfer_lock);
#endif
            return OAL_FAIL;
        }
#endif
    }
    return OAL_SUCC;
}
static void hmac_hisi_hcc_thread_exit(void)
{
    struct hcc_handler *hcc = NULL;
    uint32_t bus_dev = 0;
    uint32_t max_dev_num = oal_bus_get_chip_num();

    for (bus_dev = 0; bus_dev < max_dev_num; bus_dev++) {
        if (bus_dev >= HCC_CHIP_MAX_DEV) {
            oal_io_print("hmac_hisi_hcc_thread_exit failed!:bus_dev[%d] is exceed\n", bus_dev);
            return;
        }

        hcc = hcc_get_handler(bus_dev);
        if (hcc == NULL) {
            oal_io_print("hmac_hisi_hcc_thread_exit failed!:hcc[%d] is NULL\n", bus_dev);
            return;
        }
        if (hcc->hcc_transer_info.hcc_transfer_thread == NULL) {
            oal_io_print("hmac_hisi_hcc_thread_exit failed!:hcc_transfer_thread[%d] is NULL\n", bus_dev);
            return;
        }
        oal_kthread_stop(hcc->hcc_transer_info.hcc_transfer_thread);
        hcc->hcc_transer_info.hcc_transfer_thread = NULL;
    }
}
uint32_t hmac_hisi_thread_init(void)
{
    if (hmac_hisi_hcc_thread_init() != OAL_SUCC) {
        return OAL_FAIL;
    }
    if (hmac_hisi_rxdata_thread_init() != OAL_SUCC) {
        hmac_hisi_hcc_thread_exit();
        return OAL_FAIL;
    }
    if (hmac_trx_thread_init() != OAL_SUCC) {
        hmac_hisi_hcc_thread_exit();
        hmac_hisi_rxdata_thread_exit();
        hmac_tid_schedule_thread_exit();
        hmac_tx_comp_thread_exit();
        hmac_rx_comp_thread_exit();
        return OAL_FAIL;
    }

#if (defined(_PRE_WLAN_FEATURE_WAPI) || defined(_PRE_WLAN_FEATURE_PWL))
    /* 支持hw wapi/pwl时,不需要注册线程做软件处理 */
    if (wlan_chip_is_support_hw_wapi() != OAL_TRUE) {
        if (hmac_soft_encrypt_schedule_thread_init() != OAL_SUCC) {
            oal_io_print("hisi_soft_encrypt thread create failed!\n");
            hmac_hisi_hcc_thread_exit();
            hmac_hisi_rxdata_thread_exit();
            hmac_tid_schedule_thread_exit();
            hmac_tx_comp_thread_exit();
            hmac_rx_comp_thread_exit();
            return OAL_FAIL;
        }
    }
#endif

    return OAL_SUCC;
}

void hmac_hisi_thread_exit(void)
{
#if defined(_PRE_WLAN_TCP_OPT) && !defined(WIN32)
    struct hcc_handler *hcc = hcc_get_handler(HCC_EP_WIFI_DEV);
    if (hcc == NULL) {
        return;
    }
    if (hcc->hcc_transer_info.hcc_transfer_thread) {
        hmac_hisi_hcc_thread_exit();
    }
#endif
    hmac_hisi_rxdata_thread_exit();
    hmac_tid_schedule_thread_exit();
#if (defined(_PRE_WLAN_FEATURE_WAPI) || defined(_PRE_WLAN_FEATURE_PWL))
    hmac_soft_encrypt_schedule_thread_exit();
#endif
    hmac_tx_comp_thread_exit();
    hmac_rx_comp_thread_exit();
}
