/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明 : Hmac tx 统计模块
 * 作    者 : wifi
 * 创建日期 : 2024年1月15日
 */
#ifdef _PRE_WLAN_FEATURE_TX_STAT
#include "hmac_tid_update.h"
#include "hmac_tid_sche.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_stat.h"
#include "hmac_tx_complete.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_TX_STATS_C

typedef enum {
    HMAC_TX_STATS_COLLECT_0,
    HMAC_TX_STATS_COLLECT_1,
    HMAC_TX_STATS_COLLECT_2,
    HMAC_TX_STATS_COLLECT_3,
    HMAC_TX_STATS_COLLECT_BUTT
} hmac_tx_stat_collect_enum;

#define HMAC_TX_STATS_PERIOD_MIN 100 /* 上报周期最短100ms */
#define HMAC_TX_STATS_PERIOD_MAX 10000 /* 上报周期最长10s */

typedef struct {
    uint32_t enqueue_msdu_cnt;
    uint32_t tx_queue_sched_cnt;
    uint32_t tx_msdu_cnt;
    uint32_t tx_comp_sched_cnt;
    uint32_t tx_comp_msdu_cnt;
    uint32_t tx_ring_full_cnt;
    uint32_t upate_tid_msdu_cnt[HMAC_TX_STATS_COLLECT_BUTT]; /* 保存4份25ms周期长度发包数量 */
    uint32_t upate_tid_stats_cnt;
} hmac_tid_tx_stats_stru;

typedef struct {
    uint8_t log_on;
    uint8_t resv;
    uint16_t log_th;
    uint16_t log_period;
    uint16_t resv2;
    hmac_tid_tx_stats_stru tid_tx_stats[WLAN_ASSOC_USER_MAX_NUM][WLAN_TIDNO_BUTT];
} hmac_tx_stats_stru;

hmac_tx_stats_stru g_tx_stats = {0};

/* user_id, tid_id有效性由调用者保证 */
static hmac_tid_tx_stats_stru *hmac_get_tid_tx_stats(uint8_t user_id, uint8_t tid_id)
{
    return &g_tx_stats.tid_tx_stats[user_id][tid_id];
}

/* 设置log on开关 */
static void hmac_set_tx_stats_log_on(uint8_t log_on)
{
    g_tx_stats.log_on = log_on;
}

/* 设置log on开关 */
static void hmac_set_tx_stats_log_period(uint16_t log_period)
{
    g_tx_stats.log_period = log_period;
    g_tx_stats.log_th = log_period / HMAC_TID_UPDATE_TIMER_PERIOD;
}

static bool is_hmac_tx_stats_log_on(void)
{
    return (g_tx_stats.log_on > 0 ? OAL_TRUE : OAL_FALSE);
}

void hmac_stat_tx_ring(hmac_tid_info_stru *tid_info, uint32_t avail_size)
{
    uint32_t cur;
    uint32_t last;
    if (tid_info == NULL) {
        return;
    }

    cur = oal_netbuf_list_len(&tid_info->tid_queue);
    last = tid_info->tx_stat.max_queue_len;
    /* 缓存队列最大深度统计 */
    tid_info->tx_stat.max_queue_len = cur > last ? cur : last;

    /* ring满的统计 */
    if (avail_size == 0) {
        tid_info->tx_stat.tx_ring_full_cnt++;
    }
}

/* 入参有效性由调用者保证 */
static void hmac_tx_stats_log_proc(hmac_vap_stru *hmac_vap, hmac_tid_info_stru *tid_info,
    uint8_t vap_id, uint8_t user_id, uint8_t tid_id)
{
    uint32_t id;
    hmac_tid_tx_stats_stru *tid_stats = hmac_get_tid_tx_stats(user_id, tid_id);
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;

    uint32_t enqueue_msdu_cnt = tid_info->tx_stat.enqueue_msdu_cnt - tid_stats->enqueue_msdu_cnt;
    uint32_t tid_schedule_cnt = hmac_get_tid_schedule_cnt() - tid_stats->tx_queue_sched_cnt;
    uint32_t tx_msdu_cnt = tx_ring->tx_msdu_cnt - tid_stats->tx_msdu_cnt;
    uint32_t tx_comp_sched_cnt = hmac_get_tx_comp_scheduled_cnt() - tid_stats->tx_comp_sched_cnt;
    uint32_t tx_comp_msdu_cnt = tid_info->tx_stat.tx_comp_msdu_cnt - tid_stats->tx_comp_msdu_cnt;
    uint32_t tx_ring_full_cnt = tid_info->tx_stat.tx_ring_full_cnt - tid_stats->tx_ring_full_cnt;
    uint32_t max_queue_len = tid_info->tx_stat.max_queue_len;

    tid_stats->enqueue_msdu_cnt = tid_info->tx_stat.enqueue_msdu_cnt;
    tid_stats->tx_queue_sched_cnt = hmac_get_tid_schedule_cnt();
    tid_stats->tx_msdu_cnt = tx_ring->tx_msdu_cnt;
    tid_stats->tx_comp_sched_cnt = hmac_get_tx_comp_scheduled_cnt();
    tid_stats->tx_comp_msdu_cnt = tid_info->tx_stat.tx_comp_msdu_cnt;
    tid_stats->tx_ring_full_cnt = tid_info->tx_stat.tx_ring_full_cnt;
    tid_info->tx_stat.max_queue_len = 0;

    id = (vap_id << 16) + (user_id << 8) + tid_id; /* 16:vap偏移 8:user偏移 */
    oam_warning_log4(vap_id, OAM_SF_STATISTIC, "tx_stat id[0x%06x] enqueue[%u] sched[%u] enring[%u]",
        id, enqueue_msdu_cnt, tid_schedule_cnt, tx_msdu_cnt);
    if (tx_comp_sched_cnt != 0) {
        oam_warning_log4(vap_id, OAM_SF_STATISTIC, "tx_stat id[0x%06x] ampdu[%u] tx_comp_sched[%u] tx_comp_cnt[%u]",
            id, tx_comp_msdu_cnt / tx_comp_sched_cnt, tx_comp_sched_cnt, tx_comp_msdu_cnt);
    }
    oam_warning_log3(vap_id, OAM_SF_STATISTIC, "tx_stat id[0x%06x] queue_max[%u] ring_full[%u]",
        id, max_queue_len, tx_ring_full_cnt);
    /* 25ms粒度检查发包个数 */
    oam_warning_log4(vap_id, OAM_SF_STATISTIC, "tx_stat tx 25ms check [%u] [%u] [%u] [%u]",
        tid_stats->upate_tid_msdu_cnt[HMAC_TX_STATS_COLLECT_0], tid_stats->upate_tid_msdu_cnt[HMAC_TX_STATS_COLLECT_1],
        tid_stats->upate_tid_msdu_cnt[HMAC_TX_STATS_COLLECT_2], tid_stats->upate_tid_msdu_cnt[HMAC_TX_STATS_COLLECT_3]);
}

void hmac_tx_stats_log(hmac_tid_info_stru *tid_info, uint32_t last_period_tx_msdu)
{
    uint8_t vap_id;
    uint8_t user_id;
    uint8_t tid_id;
    hmac_user_stru *hmac_user = NULL;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_tid_tx_stats_stru *tid_stats = NULL;
    uint16_t th = g_tx_stats.log_th;
    uint8_t collect_num;

    if (!is_hmac_tx_stats_log_on()) {
        return;
    }

    tid_id = tid_info->tid_no;
    hmac_user = mac_res_get_hmac_user(tid_info->user_index);
    if (hmac_user == NULL) {
        return;
    }
    user_id = hmac_user->lut_index;
    hmac_vap = mac_res_get_hmac_vap(hmac_user->st_user_base_info.uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }
    vap_id = hmac_vap->st_vap_base_info.uc_vap_id;
    if (vap_id >= mac_chip_get_vap_support_max_num_limit() ||
        user_id >= WLAN_ASSOC_USER_MAX_NUM || tid_id >= WLAN_TIDNO_BUTT) {
        return;
    }

    tid_stats = hmac_get_tid_tx_stats(user_id, tid_id);
    tid_stats->upate_tid_stats_cnt++;
    collect_num = tid_stats->upate_tid_stats_cnt % HMAC_TX_STATS_COLLECT_BUTT;
    tid_stats->upate_tid_msdu_cnt[collect_num] = last_period_tx_msdu;

    if (tid_stats->upate_tid_stats_cnt < th) {
        return;
    }
    tid_stats->upate_tid_stats_cnt = 0;
    hmac_tx_stats_log_proc(hmac_vap, tid_info, vap_id, user_id, tid_id);
}

uint32_t hmac_tx_stats_set_log_on(mac_vap_stru *mac_vap, uint32_t *params)
{
    uint16_t log_period;
    uint8_t log_on = (uint8_t)params[0];
    hmac_set_tx_stats_log_on(log_on);

    log_period = (uint16_t)params[1];
    if (log_period < HMAC_TX_STATS_PERIOD_MIN) {
        log_period = HMAC_TX_STATS_PERIOD_MIN;
    }
    if (log_period > HMAC_TX_STATS_PERIOD_MAX) {
        log_period = HMAC_TX_STATS_PERIOD_MAX;
    }
    hmac_set_tx_stats_log_period(log_period);

    oam_warning_log2(0, 0, "{hmac_tx_stats_set_log_on::log on [%d] period[%d]}", log_on, log_period);
    return OAL_SUCC;
}
#endif