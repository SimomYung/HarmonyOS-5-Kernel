/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : host常发管理功能
 * 作者       : wifi
 * 创建日期   : 2020年05月15日
 */

/* 1 头文件包含 */

#include "hmac_tx_amsdu.h"
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tx_ring_alloc.h"
#include "host_hal_ext_if.h"
#include "hmac_config.h"
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_AL_TX_C

#define AL_TX_MSDU_NETBUF_MAX_NUM (4 * MAX_MAC_AGGR_MPDU_NUM)
/*
 * 功能描述  : 按照配置创建常发报文，包括netbuf申请和初始化
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC oal_netbuf_stru *hmac_create_al_tx_packet(uint32_t size, uint8_t payload_flag)
{
    oal_netbuf_stru *buf = NULL;
    uint32_t loop;
    uint8_t *data = NULL;
    uint8_t msdu_dscr_len = hal_host_tx_get_msdu_dscr_len();

    buf = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, (uint16_t)size + msdu_dscr_len, OAL_NETBUF_PRIORITY_MID);
    if (buf == NULL) {
        oam_error_log0(0, OAM_SF_TX, "dmac_config_create_al_tx_packet::alloc Fail");
        return NULL;
    }
    oal_netbuf_put(buf, size);
    if (oal_netbuf_headroom(buf) < msdu_dscr_len) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_netbuf_expand_headroom::netbuf headroom not enough, expanding...}");
        oal_netbuf_free(buf);
        return NULL;
    }
    data = oal_netbuf_data(buf);
    switch (payload_flag) {
        case RF_PAYLOAD_ALL_ZERO:
            memset_s(data, size, 0, size);
            break;
        case RF_PAYLOAD_ALL_ONE:
            memset_s(data, size, 0xFF, size);
            break;
        case RF_PAYLOAD_RAND:
            data[0] = oal_gen_random(18, 1); /* 18 随机数种子  */
            for (loop = 1; loop < size; loop++) {
                data[loop] = oal_gen_random(18, 0); /* 18 随机数种子  */
            }
            break;
        default:
            break;
    }
    oal_set_netbuf_next(buf, NULL);
    return buf;
}
/*
 * 功能描述  : 初始化ring
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC uint32_t hmac_always_tx_init_tx_ring(hmac_msdu_info_ring_stru *tx_ring)
{
    memset_s(tx_ring, sizeof(hmac_msdu_info_ring_stru), 0, sizeof(hmac_msdu_info_ring_stru));
    tx_ring->base_ring_info.size = hal_host_tx_tid_ring_size_get(HAL_TX_RING_SIZE_GRADE_DEFAULT, WLAN_TIDNO_BCAST);
    tx_ring->host_tx_flag = HOST_AL_TX_FLAG;
    oal_atomic_set(&tx_ring->inited, OAL_TRUE);
    if (hmac_alloc_tx_ring_by_tid(tx_ring, WLAN_TIDNO_BCAST) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_always_tx_init_tx_ring::hmac_alloc_tx_ring_by_tid failed}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
/*
 * 功能描述  : 配置msdu ring info
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_always_tx_set_msdu_info(msdu_info_stru *msdu_info, oal_netbuf_stru *netbuf, uintptr_t devva)
{
    memset_s(msdu_info, sizeof(msdu_info_stru), 0, sizeof(msdu_info_stru));
    msdu_info->msdu_addr_lsb = get_low_32_bits(devva);
    msdu_info->msdu_addr_msb = get_high_32_bits(devva);
    msdu_info->msdu_len = oal_netbuf_len(netbuf);
    msdu_info->data_type = DATA_TYPE_ETH;
    msdu_info->from_ds = 0;
    msdu_info->to_ds = 1;
}
/*
 * 功能描述  : 修改ringbuffer
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_al_tx_set_ring_info(hmac_msdu_info_ring_stru *tx_ring, oal_netbuf_stru *netbuf, uint32_t index,
    uintptr_t info_devva)
{
    msdu_info_stru msdu_info = { 0 };
    hmac_always_tx_set_msdu_info(&msdu_info, netbuf, info_devva);
    hal_host_tx_set_msdu_ring(&tx_ring->ring_buf, &msdu_info, index);
    tx_ring->netbuf_list[index] = netbuf;
    oal_atomic_inc(&tx_ring->msdu_cnt);
    tx_ring->base_ring_info.write_index++;
}
/*
 * 功能描述  : 更新抛事件内容
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
OAL_STATIC void hmac_always_tx_set_update_info(mac_cfg_tx_comp_stru *event_set_al_tx,
    hmac_msdu_info_ring_stru *tx_ring, uint32_t net_buff_num, uintptr_t devva)
{
    event_set_al_tx->base_addr_lsb = get_low_32_bits(devva);
    event_set_al_tx->base_addr_msb = get_high_32_bits(devva);
    event_set_al_tx->en_protocol_mode = tx_ring->base_ring_info.size;
    event_set_al_tx->en_ack_policy = net_buff_num / MAX_MAC_AGGR_MPDU_NUM;
    event_set_al_tx->write_index = net_buff_num;
    event_set_al_tx->uc_param = HAL_ALWAYS_TX_RF;
}
hmac_msdu_info_ring_stru g_always_tx_ring[WLAN_DEVICE_MAX_NUM_PER_CHIP];

/*
 * 功能描述  : host长发根据帧长获取netbuf个数
 * 1.日    期  : 2020年12月14日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_always_tx_get_netbuf_num(uint32_t payload_len)
{
    uint32_t net_buff_num;

    if (payload_len % AL_TX_MSDU_NETBUF_MAX_LEN == 0) {
        net_buff_num = payload_len / AL_TX_MSDU_NETBUF_MAX_LEN;
    } else {
        net_buff_num = payload_len / AL_TX_MSDU_NETBUF_MAX_LEN + 1;
    }
    net_buff_num = oal_min(net_buff_num, AL_TX_MSDU_NETBUF_MAX_NUM);
    return net_buff_num;
}
OAL_STATIC void hmac_always_tx_spin_lock_init(hmac_msdu_info_ring_stru *tx_ring)
{
    mutex_init(&tx_ring->tx_lock);
    mutex_init(&tx_ring->tx_comp_lock);
}

void hmac_al_tx_ring_release(hmac_msdu_info_ring_stru *tx_ring)
{
    oal_atomic_set(&tx_ring->inited, OAL_FALSE);
    hmac_tx_host_ring_release(tx_ring);
}
/*
 * 功能描述  : host常发 同步 msdu info信息 需要在常发启动前同步
 * 1.日    期  : 2025年6月20日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
void hmac_always_tx_set_msdu_info_msg(uintptr_t addr_devva, uintptr_t info_devva, uint32_t net_buff_num, uint8_t *param)
{
    mac_cfg_altx_msdu_info_stru *event_set_al_tx = (mac_cfg_altx_msdu_info_stru *)param;
    event_set_al_tx->msdu_info_addr_lsb = get_low_32_bits(info_devva);
    event_set_al_tx->msdu_info_addr_msb = get_high_32_bits(info_devva);
    event_set_al_tx->msdu_addr_addr_lsb = get_low_32_bits(addr_devva);
    event_set_al_tx->msdu_addr_addr_msb = get_high_32_bits(addr_devva);
    event_set_al_tx->al_tx_netbuf_num = net_buff_num;
    event_set_al_tx->al_tx_flag = HAL_ALWAYS_TX_RF;
}

void hmac_always_tx_send_msdu_info_msg(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param)
{
    uint32_t ret;
    ret = hmac_config_send_event(mac_vap, WLAN_CFGID_SET_ALWAYS_TX_RING_INFO, len, param);
    if (oal_unlikely(ret != OAL_SUCC)) {
        oam_warning_log1(mac_vap->uc_vap_id, OAM_SF_CFG,
                         "{hmac_always_tx_send_msdu_info_msg::hmac_config_send_event failed[%d].}", ret);
    }
}

/*
 * 功能描述  : host常发主流程
 * 1.日    期  : 2020年4月27日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hmac_always_tx_proc(mac_vap_stru *mac_vap, uint16_t len, uint8_t *param, uint8_t hal_dev_id)
{
    mac_cfg_tx_comp_stru *event_set_al_tx = (mac_cfg_tx_comp_stru *)param;
    uint32_t ret;
    uint32_t index;
    uint32_t net_buff_num;
    uint64_t info_devva = 0;
    uint64_t addr_devva = 0;
    uint32_t packet_len;
    hmac_msdu_info_ring_stru *tx_ring = &g_always_tx_ring[hal_dev_id];
    oal_netbuf_stru  *netbuf = NULL;
    uint8_t payload_flag = event_set_al_tx->en_payload_flag;
    uint32_t payload_len = event_set_al_tx->payload_len;

    if (hmac_always_tx_init_tx_ring(tx_ring) != OAL_SUCC) {
        return OAL_FAIL;
    }
    hmac_always_tx_spin_lock_init(tx_ring);
    net_buff_num = hmac_always_tx_get_netbuf_num(payload_len);
    for (index = 0; index < net_buff_num; index++) {
        packet_len = (index == (net_buff_num - 1)) ?
            (payload_len - (index * AL_TX_MSDU_NETBUF_MAX_LEN)) : AL_TX_MSDU_NETBUF_MAX_LEN;
        netbuf = hmac_create_al_tx_packet(packet_len, payload_flag);
        if (oal_unlikely(netbuf == NULL)) {
            hmac_al_tx_ring_release(tx_ring);
            oam_error_log0(0, OAM_SF_TX, "{hmac_always_tx_proc::netbuf is NULL}");
            return OAL_ERR_CODE_PTR_NULL;
        }
        ret = hmac_tx_hostva_to_devva(hmac_tx_netbuf_init_msdu_dscr(netbuf),
            netbuf->len + hal_host_tx_get_msdu_dscr_len(), &info_devva);
        if (ret != OAL_SUCC) {
            oam_error_log1(0, OAM_SF_TX, "{hmac_always_tx_proc::hostva to info_devva failed[%d]}", ret);
            hmac_al_tx_ring_release(tx_ring);
            oal_netbuf_free(netbuf);
            return ret;
        }
        hmac_al_tx_set_ring_info(tx_ring, netbuf, index, (uintptr_t)info_devva);
    }
    if (hal_host_tx_get_msdu_info_devva(&tx_ring->ring_buf, &info_devva, &addr_devva) != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_always_tx_proc::hostva to devva failed");
        hmac_al_tx_ring_release(tx_ring);
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    hmac_always_tx_set_msdu_info_msg((uintptr_t)addr_devva, (uintptr_t)info_devva, net_buff_num, param);
    hmac_always_tx_send_msdu_info_msg(mac_vap, len, param);
    event_set_al_tx->en_payload_flag = payload_flag;
    event_set_al_tx->payload_len = payload_len;
    hmac_always_tx_set_update_info(event_set_al_tx, tx_ring, net_buff_num, (uintptr_t)info_devva);
    return ret;
}
