/*
 * Copyright (c) @CompanyNameMagicTag 2024-2025. All rights reserved.
 * Description: nbw frame encap and trx
 * Author: wifi
 * Create: 2024-12-26
 */
#include "hmac_nbw_frame.h"
#include "securec.h"
#include "securectype.h"
#include "oal_cfg80211.h"
#include "wlan_types.h"
#include "mac_device.h"
#include "hmac_mgmt_sta_up.h"
#include "mac_ie.h"
#include "hmac_encap_frame_sta.h"
#include "hmac_encap_frame_ap.h"
#include "hmac_mgmt_bss_comm.h"
#include "mac_frame_inl.h"
#include "mac_function.h"
#include "mac_mib_sta_config.h"
#include "hmac_chba_frame.h"
#include "hmac_chba_function.h"
#include "hmac_nbw_mgmt.h"
#include "hmac_nbw_connect.h"
#include "hmac_chba_fsm.h"
#include "hmac_chba_sync.h"
#include "oam_event_wifi.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_NBW_FRAME_C

#ifdef _PRE_WLAN_FEATURE_NBW
oal_bool_enum_uint8 hmac_nbw_is_nbw_action(uint8_t *payload, uint32_t payload_len)
{
    /*
     *                       NBW Action
     * | category |  nbw oui   | oui type | NBW Attributes |
     * |     1    |     3      |     1    |     variable    |
     * |    127   |   00-E0-FC |   0x99   |                 |
     */
    uint8_t nbw_oui[MAC_OUI_LEN] = { 0x00, 0xE0, 0xFC };

    if (payload_len < MAC_NBW_ACTION_HDR_LEN) {
        return OAL_FALSE;
    }

    if (((payload[0] != MAC_ACTION_CATEGORY_VENDOR) &&
        (payload[0] != MAC_ACTION_CATEGORY_VENDOR_SPECIFIC_PROTECTED)) ||
        oal_memcmp(payload + 1, nbw_oui, MAC_OUI_LEN) != 0  ||
        payload[MAC_NBW_ACTION_OUI_TYPE_LEN] != MAC_OUI_TYPE_NBW) {
        return OAL_FALSE;
    }

    return OAL_TRUE;
}


void mac_set_nbw_action_hdr(uint8_t *action_buf, uint8_t action_type)
{
    if (oal_any_null_ptr1(action_buf)) {
        return;
    }
    /* 目前都是单播帧，所以都protected */
    action_buf[0] = MAC_ACTION_CATEGORY_VENDOR_SPECIFIC_PROTECTED; /* vendor-specific protected action */

    action_buf[BYTE_OFFSET_1] = 0x00; /* 1: oui[0] */
    action_buf[BYTE_OFFSET_2] = 0xE0; /* 2: oui[1] */
    action_buf[BYTE_OFFSET_3] = 0xFC; /* 3: oui[2] */
    action_buf[BYTE_OFFSET_4] = MAC_OUI_TYPE_NBW;
    action_buf[BYTE_OFFSET_5] = action_type;
}

uint32_t hmac_nbw_set_nbw_timeslot_response_action_attr(uint8_t *attr_buf, uint8_t *offset, uint8_t *slave_addr,
    hmac_nbw_uslot_info_stru uslot_info)
{
    uint8_t idx = *offset;
    if (uslot_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
        attr_buf[idx++] = MAC_NBW_REJECT;
        /* 拒绝也带上slave mac地址，在解析时封装好处理 */
        if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return OAL_FAIL;
        }
        idx += WLAN_MAC_ADDR_LEN;
    } else {
        attr_buf[idx++] = MAC_NBW_ACCEPT;
        if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return OAL_FAIL;
        }
        idx += WLAN_MAC_ADDR_LEN;
        attr_buf[idx++] = 0;
        attr_buf[idx++] = 0;
        attr_buf[idx++] = TWELVE_REPEAT;
        attr_buf[idx++] = uslot_info.start_idx;
        attr_buf[idx++] = uslot_info.end_idx;
    }
    attr_buf[MAC_NBW_ATTR_ID_LEN] = idx - MAC_NBW_ATTR_HDR_LEN;
    *offset = idx;
    return OAL_SUCC;
}

uint32_t hmac_nbw_set_nbw_timeslot_request_action_attr(uint8_t *attr_buf, uint8_t *offset, uint8_t *slave_addr,
    hmac_nbw_uslot_info_stru uslot_info)
{
    uint8_t idx = *offset;
    if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    idx += WLAN_MAC_ADDR_LEN;
    attr_buf[idx++] = 0;
    attr_buf[idx++] = 0;
    attr_buf[idx++] = TWELVE_REPEAT;
    attr_buf[idx++] = uslot_info.timeslot_interval;
    attr_buf[idx++] = uslot_info.start_idx;
    attr_buf[idx++] = uslot_info.end_idx;
    attr_buf[MAC_NBW_ATTR_ID_LEN] = idx - MAC_NBW_ATTR_HDR_LEN;
    *offset = idx;
    return OAL_SUCC;
}

uint32_t hmac_nbw_set_nbw_timeslot_indicate_action_attr(uint8_t *attr_buf,
    uint8_t *offset, uint8_t *master_addr, hmac_nbw_uslot_info_stru uslot_info)
{
    uint8_t idx = *offset;
    if (uslot_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
        attr_buf[idx++] = MAC_NBW_REJECT;
    } else {
        attr_buf[idx++] = MAC_NBW_ACCEPT;
        if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, master_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return OAL_FAIL;
        }
        idx += WLAN_MAC_ADDR_LEN;
        attr_buf[idx++] = 0;
        attr_buf[idx++] = TWELVE_REPEAT;
        attr_buf[idx++] = uslot_info.start_idx;
        attr_buf[idx++] = uslot_info.end_idx;
    }
    attr_buf[MAC_NBW_ATTR_ID_LEN] = idx - MAC_NBW_ATTR_HDR_LEN;
    *offset = idx;
    return OAL_SUCC;
}

void hmac_nbw_set_timeslot_neg_attr(uint8_t *attr_buf, uint16_t *attr_len, uint8_t *slave_addr,
    hmac_nbw_uslot_info_stru uslot_info, uint8_t request_type)
{
    uint8_t offset;
    uint32_t ret = 0;
    attr_buf[0] = MAC_NBW_ATTR_TIMESLOT_INFO;
    offset = MAC_NBW_ATTR_HDR_LEN;
    switch (request_type) {
        case MAC_NBW_TIMESLOT_NEG_REQUEST_ACTION:
            ret = hmac_nbw_set_nbw_timeslot_request_action_attr(attr_buf, &offset, slave_addr, uslot_info);
            break;
        case MAC_NBW_TIMESLOT_NEG_RESPONSE_ACTION:
            ret = hmac_nbw_set_nbw_timeslot_response_action_attr(attr_buf, &offset, slave_addr, uslot_info);
            break;
        case MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION:
            ret = hmac_nbw_set_nbw_timeslot_indicate_action_attr(attr_buf, &offset,
                hmac_nbw_get_master_macaddr(), uslot_info);
            break;
        default:
            break;
    }
    if (ret == OAL_FAIL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_set_timeslot_attr: set attr fail!");
        return;
    }
    *attr_len = offset;
    return;
}

uint32_t hmac_nbw_set_nbw_timeslot_adjust_req_action_attr(uint8_t *attr_buf,
    uint8_t *offset, uint8_t *slave_addr, hmac_nbw_uslot_info_stru uslot_info)
{
    uint8_t idx = *offset;
    oam_warning_log3(0, OAM_SF_ANY,
        "{hmac_nbw_set_nbw_timeslot_adjust_req_action_attr:slave mac[%02x:XX:XX:XX:%02x:%02x]}",
        slave_addr[MAC_ADDR_0], slave_addr[MAC_ADDR_4],
        slave_addr[MAC_ADDR_5]);

    if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    idx += WLAN_MAC_ADDR_LEN;
    if (hmac_nbw_get_role() == MAC_NBW_SLAVEAP_ROLE) {
        attr_buf[idx++] = 0; /* 如果是slaveap发送，标志位填0，否则填1，即slave发填1，方便demo实现 */
    } else {
        attr_buf[idx++] = 1;
    }
    attr_buf[idx++] = 0;
    attr_buf[idx++] = TWELVE_REPEAT;
    /* 如果是通过时隙调整帧 做时隙资源释放使用，则uslot_info不要填写 */
    if (uslot_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
        attr_buf[idx++] = INVALID_TIMESLOT_USLOT_IDX;
        attr_buf[idx++] = INVALID_TIMESLOT_USLOT_IDX;
    } else {
        attr_buf[idx++] = uslot_info.start_idx;
        attr_buf[idx++] = uslot_info.end_idx;
    }
    attr_buf[MAC_NBW_ATTR_ID_LEN] = idx - MAC_NBW_ATTR_HDR_LEN;
    *offset = idx;
    return OAL_SUCC;
}

uint32_t hmac_nbw_set_nbw_timeslot_adjust_rsp_action_attr(uint8_t *attr_buf,
    uint8_t *offset, uint8_t *slave_addr, hmac_nbw_uslot_info_stru uslot_info)
{
    uint8_t idx = *offset;
    if (uslot_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
        attr_buf[idx++] = MAC_NBW_REJECT;
        /* 拒绝也带上slave mac地址，在解析时封装好处理 */
        if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return OAL_FAIL;
        }
        idx += WLAN_MAC_ADDR_LEN;
    } else {
        attr_buf[idx++] = MAC_NBW_ACCEPT;
        if (memcpy_s(attr_buf + idx, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
            return OAL_FAIL;
        }
        idx += WLAN_MAC_ADDR_LEN;
        attr_buf[idx++] = 0; /* delayed work beacon 暂时定成立即生效 */
        attr_buf[idx++] = TWELVE_REPEAT;
        attr_buf[idx++] = uslot_info.start_idx;
        attr_buf[idx++] = uslot_info.end_idx;
    }
    attr_buf[MAC_NBW_ATTR_ID_LEN] = idx - MAC_NBW_ATTR_HDR_LEN;
    *offset = idx;
    return OAL_SUCC;
}

void hmac_nbw_set_timeslot_adjust_attr(uint8_t *attr_buf, uint16_t *attr_len, uint8_t *slave_addr,
    hmac_nbw_uslot_info_stru uslot_info, uint8_t request_type)
{
    uint8_t offset;
    uint32_t ret = 0;
    attr_buf[0] = MAC_NBW_ATTR_TIMESLOT_INFO;
    offset = MAC_NBW_ATTR_HDR_LEN;
    switch (request_type) {
        case MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_ACTION:
            ret = hmac_nbw_set_nbw_timeslot_adjust_req_action_attr(attr_buf, &offset, slave_addr, uslot_info);
            break;
        case MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_ACTION:
            ret = hmac_nbw_set_nbw_timeslot_adjust_rsp_action_attr(attr_buf, &offset, slave_addr, uslot_info);
            break;
        default:
            break;
    }
    if (ret == OAL_FAIL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_set_timeslot_adjust_attr: set attr fail!");
        return;
    }
    *attr_len = offset;
    return;
}

void hmac_nbw_set_timeslot_attr(uint8_t *attr_buf, uint16_t *attr_len, uint8_t *slave_addr,
    hmac_nbw_uslot_info_stru uslot_info, uint8_t action_type, uint8_t request_type)
{
    if (oal_any_null_ptr3(attr_buf, attr_len, slave_addr)) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_set_timeslot_attr: ptr is NULL!");
        return;
    }
    if (action_type == MAC_NBW_TIMESLOT_NEG_ACTION) {
        hmac_nbw_set_timeslot_neg_attr(attr_buf, attr_len, slave_addr, uslot_info, request_type);
    } else if (action_type == MAC_NBW_TIMESLOT_ADJUST_ACTION) {
        hmac_nbw_set_timeslot_adjust_attr(attr_buf, attr_len, slave_addr, uslot_info, request_type);
    } else {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_set_timeslot_attr: action type error!");
        return;
    }
}

void hmac_nbw_send_timeslot_frame(uint8_t *peer_addr, uint8_t *slave_addr, uint8_t addr_len,
    hmac_nbw_uslot_info_stru uslot_info, uint8_t action_type, uint8_t request_type)
{
    oal_netbuf_stru *timeslot_action_frame = NULL;
    uint8_t *frame_hdr = NULL, *payload = NULL;
    uint16_t mgmt_len = 0, payload_len = 0, attr_len = 0;
    uint32_t ret;
    hmac_chba_vap_stru *chba_vap_info = hmac_get_chba_vap();
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(chba_vap_info->mac_vap_id);
    mac_vap_stru *mac_vap = NULL;
    /* 先用chba vap来实现， 后续可能改成nbw vap */
    if (oal_any_null_ptr3(hmac_vap, peer_addr, slave_addr) || (addr_len > WLAN_MAC_ADDR_LEN)) {
        return;
    }
    mac_vap = &hmac_vap->st_vap_base_info;
    timeslot_action_frame = oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, WLAN_MEM_NETBUF_SIZE2, OAL_NETBUF_PRIORITY_MID);
    if (timeslot_action_frame == NULL) {
        return;
    }
    oam_warning_log2(0, OAM_SF_NBW, "hmac_nbw_send_timeslot_frame: action type:%d, request type:%d!",
        action_type, request_type);

    oal_mem_netbuf_trace(timeslot_action_frame, OAL_TRUE);
    memset_s(oal_netbuf_cb(timeslot_action_frame), oal_netbuf_cb_size(), 0, oal_netbuf_cb_size());
    /* 封装TIMESLOT Action */
    frame_hdr = (uint8_t *)oal_netbuf_header(timeslot_action_frame);
    /* 将mac header清零 */
    memset_s(frame_hdr, MAC_80211_FRAME_LEN, 0, MAC_80211_FRAME_LEN);
    /* 填写mac头信息 */
    mac_hdr_set_frame_control(frame_hdr, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_ACTION);
    oal_set_mac_addr(frame_hdr + WLAN_HDR_ADDR1_OFFSET, peer_addr);
    oal_set_mac_addr(frame_hdr + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(mac_vap));
    oal_set_mac_addr(frame_hdr + WLAN_HDR_ADDR3_OFFSET, hmac_nbw_get_master_macaddr());

    mgmt_len += MAC_80211_FRAME_LEN;
    payload = frame_hdr + mgmt_len;
    /* 填写Action 头 */
    mac_set_nbw_action_hdr(payload, action_type);
    payload_len += MAC_CHBA_ACTION_HDR_LEN;
    /* 填写type */
    payload[payload_len] = request_type;
    payload_len++;
    /* 当前没有rp值，时隙复用这些 */
    hmac_nbw_set_timeslot_attr(payload + payload_len, &attr_len, slave_addr, uslot_info, action_type, request_type);

    if (attr_len == 0) {
        oal_netbuf_free(timeslot_action_frame);
        return;
    }
    payload_len += attr_len;
    mgmt_len += payload_len;
    oal_netbuf_put(timeslot_action_frame, mgmt_len);
    /* 先复用chba的管理帧cb字段初始化 */
    hmac_chba_mgmt_cb_partial_init(mac_vap, peer_addr, timeslot_action_frame, mgmt_len, OAL_FALSE);
    /* 发送TIMESLOT Action帧 */
    ret = hmac_tx_mgmt_send_event(mac_vap, timeslot_action_frame, mgmt_len);
    if (ret != OAL_SUCC) {
        oal_netbuf_free(timeslot_action_frame);
        return;
    }
}


static void hmac_nbw_timeslot_neg_request_handle(hmac_vap_stru *hmac_vap, uint8_t *payload,
    uint16_t payload_len, uint8_t *peer_addr)
{
    uint8_t time_interval;
    uint8_t slave_addr[WLAN_MAC_ADDR_LEN];
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    if (payload_len < TIMESLOT_REQ_BODY_LEN || hmac_nbw_get_role() != MAC_NBW_MASTER_ROLE) {
        oam_error_log0(0, OAM_SF_NBW,
            "hmac_nbw_timeslot_request_handler::receive timeslot REQ len invalid or is not master no handle.");
        return;
    }
    oam_warning_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_request_handle enter");

    if (memcpy_s(slave_addr, WLAN_MAC_ADDR_LEN, payload + TIMESLOT_REQ_SLAVEMAC_POS, WLAN_MAC_ADDR_LEN) != EOK) {
        return;
    }
    /* 目前只考虑一种申请形式，即12repeat, 每8个uslot作为一个单位 */
    time_interval = *(payload + TIMESLOT_REQ_TIMEINTERVAL_POS);
    /* 先做简化方案，不考虑时隙复用的情况，只要申请时隙的连续长度让当前的uslot-bitmap不满足，则拒绝 */
    uslot_bitmap_info = hmac_nbw_calculate_uslotbitmap(time_interval);
    /* 如果申请不到时隙，则直接发送拒绝帧，结束流程 */
    if (uslot_bitmap_info.uslot_bitmap == INVALID_USLOT_BITMAP) {
        hmac_nbw_send_timeslot_frame(peer_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
            MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_RESPONSE_ACTION);
        return;
    }

    /* 第一版不维护拓扑信息，只维护4个岛的时隙资源情况包括岛的设备信息这些 */
    hmac_update_master_island_info(uslot_bitmap_info, slave_addr, peer_addr);
    /* 特别说明：通过neg req来做的时隙申请，必定不是与master有连接的设备，不然会走时隙申请流程 */
    hmac_nbw_set_available_uslot_bitmap(uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_send_timeslot_frame(peer_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
        MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_RESPONSE_ACTION);
}

static uint32_t hmac_nbw_timeslot_neg_response_resolving(hmac_vap_stru *hmac_vap, uint8_t *payload,
    uint16_t payload_len)
{
    uint8_t slave_addr[WLAN_MAC_ADDR_LEN];
    uint8_t status, slot_idx;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    hmac_user_stru *hmac_user = NULL;
    slot_info_stru *island_slot_info = hmac_nbw_get_slaveap_island_slot_info();
    if (memcpy_s(slave_addr, WLAN_MAC_ADDR_LEN, payload + TIMESLOT_RSP_SLAVEMAC_POS, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    status = *(payload + TIMESLOT_RSP_STATUS_POS);
    /* 如果收到的response报文是拒绝的，则不做处理，直接通知slave拒绝 */
    if (status == MAC_NBW_REJECT) {
        oam_warning_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_response_resolving reject error");
        hmac_nbw_send_timeslot_frame(slave_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
            MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION);
        return OAL_FAIL;
    }
    /* 如果收到的response是接受，则取出对应的时隙，填入岛的时隙管理部分，再通知给slave */
    /* slaverp，单群岛指示等的处理先不考虑 */
    uslot_bitmap_info.start_idx = *(payload + TIMESLOT_RSP_START_IDX_POS);
    uslot_bitmap_info.end_idx = *(payload + TIMESLOT_RSP_END_IDX_POS);
    uslot_bitmap_info.timeslot_interval = uslot_bitmap_info.end_idx - uslot_bitmap_info.start_idx + 1;
    uslot_bitmap_info.uslot_bitmap = hmac_nbw_convert_idx_to_uslotbitmap_level1(uslot_bitmap_info.start_idx,
        uslot_bitmap_info.end_idx);
    oam_warning_log3(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_response_resolving uslotbitmap start_idx: %d, end_idx; %d "
        " bitmap: %d", uslot_bitmap_info.start_idx, uslot_bitmap_info.end_idx, uslot_bitmap_info.uslot_bitmap);
    /* 更新岛主的时隙管理信息 */
    /* 填入时隙管理并配置给固件,   因为  是时隙申请所以必然是新增 */
    slot_idx = hmac_nbw_find_island_unused_slot_idx();
    if (slot_idx == NBW_INVALID_IDX) {
        return OAL_FAIL;
    }
    if (memcpy_s(island_slot_info[slot_idx].slave_device_mac, WLAN_MAC_ADDR_LEN,
        slave_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    island_slot_info[slot_idx].per_slot_info.uslot_info = uslot_bitmap_info;
    island_slot_info[slot_idx].use_flag = OAL_TRUE;
    hmac_nbw_slaveap_link_cnt_add();
    hmac_nbw_send_timeslot_frame(slave_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
        MAC_NBW_TIMESLOT_NEG_ACTION, MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION);
    hmac_user = mac_vap_get_hmac_user_by_addr(&(hmac_vap->st_vap_base_info), slave_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_response_resolving::NO USER ERROR.");
        return OAL_FAIL;
    }
    hmac_nbw_sync_user_bitmap(hmac_vap, hmac_user, mac_nbw_uslot_bitmap_to_bitmap(uslot_bitmap_info.uslot_bitmap));
    hmac_nbw_update_uslot_bitmap_for_slot_alloced(uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info));
    return OAL_SUCC;
}


static void hmac_nbw_encap_sync_beacon_frame(mac_vap_stru *mac_vap, uint8_t *beacon_buf, uint16_t *beacon_len,
    uint8_t *domain_bssid, uint8_t *payload, uint16_t payload_len)
{
    hmac_chba_system_params *system_params = hmac_get_chba_system_params();
    uint16_t total_len, vendor_ie_len;
    hmac_vap_stru *hmac_vap = NULL;
    hmac_chba_vap_stru *chba_vap_info = NULL;
    uint16_t *beacon_interval = NULL;
    uint32_t bcn_int;
    uint8_t *vendor_ie_len_pos = NULL;
    int32_t err;

    if (oal_any_null_ptr5(mac_vap, beacon_buf, beacon_len, domain_bssid, payload)) {
        return;
    }
    hmac_vap = (hmac_vap_stru *)mac_res_get_hmac_vap(mac_vap->uc_vap_id);
    if (hmac_vap == NULL) {
        return;
    }
    chba_vap_info = hmac_vap->hmac_chba_vap_info;
    if (chba_vap_info == NULL) {
        return;
    }

    total_len = 0;
    /* 填写mac头信息 */
    mac_hdr_set_frame_control(beacon_buf, WLAN_PROTOCOL_VERSION | WLAN_FC0_TYPE_MGT | WLAN_FC0_SUBTYPE_BEACON);
    oal_set_mac_addr(beacon_buf + WLAN_HDR_ADDR1_OFFSET, BROADCAST_MACADDR); /* 设置广播发送地址 */
    oal_set_mac_addr(beacon_buf + WLAN_HDR_ADDR2_OFFSET, mac_mib_get_StationID(mac_vap)); /* 设置sa */
    oal_set_mac_addr(beacon_buf + WLAN_HDR_ADDR3_OFFSET, domain_bssid); /* 设置BSSID */

    /* beacon帧没有sequence number */
    mac_hdr_set_fragment_number(beacon_buf, 0);
    total_len += MAC_80211_FRAME_LEN;

    /* Timestamp由硬件填写 */
    memset_s(beacon_buf + total_len, MAC_TIME_STAMP_LEN, 0, MAC_TIME_STAMP_LEN);
    total_len += MAC_TIME_STAMP_LEN;
    /* 填写beacon period字段为512 */
    bcn_int = system_params->schedule_time_ms;
    beacon_interval = (uint16_t *)(beacon_buf + total_len);
    *beacon_interval = (uint16_t)oal_byteorder_to_le32(bcn_int);
    total_len += MAC_BEACON_INTERVAL_LEN;
    /* 填写capinfo字段 */
    mac_set_cap_info_ap(mac_vap, beacon_buf + total_len);
    total_len += MAC_CAP_INFO_LEN;

    vendor_ie_len_pos = beacon_buf + total_len + 1;

    /* 将payload复制到netbuf中 */
    err = memcpy_s(beacon_buf + total_len, payload_len, payload, payload_len);
    if (err != EOK) {
        oam_warning_log0(0, 0, "hmac_chba_encap_sync_beacon_frame::memcpy failed.");
        return;
    }
    total_len += payload_len;
    /* 获取当前的vendor IE Len */
    vendor_ie_len = payload[MAC_CHBA_ATTR_ID_LEN];

    if (mac_vap->nbw_mode == NBW_MODE) {
        /* nbw 不需要封装coex info信息 */
        *vendor_ie_len_pos = vendor_ie_len;
        *beacon_len = total_len;
        return;
    }
}


uint32_t hmac_nbw_sync_beacon_info(hmac_vap_stru *hmac_vap, hmac_chba_vap_stru *chba_vap_info)
{
    mac_vap_stru *mac_vap = NULL;
    chba_params_config_stru chba_params_sync = { 0 };
    master_info master_info = { 0 };
    uint32_t ret;

    if (hmac_vap == NULL || chba_vap_info == NULL) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_chba_sync_chba_info::input pointer is null.");
        return OAL_FAIL;
    }

    mac_vap = &hmac_vap->st_vap_base_info;
    /* 下发同步信息到dmac */
    chba_params_sync.chba_state = hmac_chba_get_sync_state();
    chba_params_sync.chba_role = hmac_nbw_get_role();
    oal_set_mac_addr(chba_params_sync.domain_bssid, hmac_nbw_get_master_macaddr());
    oal_set_mac_addr(master_info.master_addr, hmac_nbw_get_master_macaddr());
    chba_params_sync.master_info = master_info;
    chba_params_sync.info_bitmap = CHBA_MASTER_INFO | CHBA_STATE | CHBA_ROLE | CHBA_BSSID;

    /* 封装beacon帧 */
    hmac_nbw_encap_sync_beacon_frame(mac_vap, chba_params_sync.sync_beacon, &chba_params_sync.sync_beacon_len,
        hmac_nbw_get_master_macaddr(), chba_vap_info->beacon_buf, chba_vap_info->beacon_buf_len);
    chba_params_sync.info_bitmap |= CHBA_BEACON_BUF | CHBA_BEACON_LEN;

    ret = hmac_chba_params_sync(mac_vap, &chba_params_sync);
    oam_warning_log1(0, 0, "hmac_chba_encap_sync_beacon_frame::sync ret:%d.", ret);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_sync_beacon_info::send nbw params to dmac err");
        return ret;
    }

    return OAL_SUCC;
}


static uint32_t hmac_nbw_slaveap_beacon_sync(hmac_vap_stru *hmac_vap)
{
    hmac_chba_vap_stru *chba_vap_info = hmac_get_chba_vap();
    oam_warning_log0(0, 0, "hmac_nbw_slaveap_beacon_sync enter");

    /* 封装beacon vendor信息 */
    hmac_nbw_encap_master_beacon(chba_vap_info, chba_vap_info->beacon_buf, &chba_vap_info->beacon_buf_len,
        MAC_NBW_BEACON_TYPE_BASIC);

    return hmac_nbw_sync_beacon_info(hmac_vap, chba_vap_info);
}


static void hmac_nbw_timeslot_neg_response_handle(hmac_vap_stru *hmac_vap, uint8_t *peer_addr, uint8_t *payload,
    uint16_t payload_len)
{
    uint32_t ret;
    hmac_user_stru *hmac_user = NULL;
    hmac_nbw_current_connect_info_stru *connect_info = hmac_nbw_get_curr_conn_info();
    if (payload_len < TIMESLOT_RSP_MIN_BODY_LEN || (hmac_nbw_get_role() != MAC_NBW_SLAVEAP_ROLE &&
        hmac_nbw_get_role() != MAC_NBW_SLAVEAP_SLAVE_MIX_ROLE)) {
        oam_error_log0(0, OAM_SF_NBW,
            "hmac_nbw_timeslot_response_handle::receive timeslot RSP len invalid or no slaveap handle it.");
        return;
    }

    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, connect_info->peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_timeslot_neg_response_handle::hmac user is null.");
        return;
    }
    oam_warning_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_timeslot_neg_response_handle:: slaveap process slot neg response.");

    ret = hmac_nbw_timeslot_neg_response_resolving(hmac_vap, payload, payload_len);
    /* 未申请到时隙时的本端设备处理 */
    if (ret == OAL_FAIL) {
        /* 建链失败处理 */
        hmac_nbw_connect_fail_handler(hmac_vap);
        return;
    }

    /* 停hmac_nbw_waiting_master_slot_time_response_proc 定时器 */
    if (connect_info->assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(connect_info->assoc_waiting_timer));
    }

    /* 向device配置beacon信息 */
    ret = hmac_nbw_slaveap_beacon_sync(hmac_vap);
    if (ret == OAL_FAIL) {
        /* 配置dmac信息失败的处理 */
        hmac_nbw_connect_fail_handler(hmac_vap);
        return;
    }

    /* 时隙申请完成后，清除当前建链信息 */
    hmac_nbw_clear_curr_conn_info();
}

static uint32_t hmac_nbw_timeslot_neg_indicate_resolving(hmac_vap_stru *hmac_vap, uint8_t *payload,
    uint16_t payload_len, uint8_t *peer_addr)
{
    uint8_t master_addr[WLAN_MAC_ADDR_LEN];
    uint8_t status, slot_idx;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    hmac_user_stru *hmac_user = NULL;
    slave_slot_stru *slave_slot_info = hmac_nbw_get_slave_slot_info();
    status = *(payload + TIMESLOT_INDICATE_STATUS_POS);
    /* 如果收到的response报文是拒绝的，则不做处理 */
    if (status == MAC_NBW_REJECT || (status == MAC_NBW_ACCEPT && payload_len < TIMESLOT_INDICATE_BODY_LEN)) {
        return OAL_FAIL;
    }

    if (memcpy_s(master_addr, WLAN_MAC_ADDR_LEN, payload + TIMESLOT_INDICATE_MASTERMAC_POS,
        WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    oal_set_mac_addr(hmac_nbw_get_master_macaddr(), master_addr);
    oam_warning_log3(0, OAM_SF_ANY, "{hmac_nbw_timeslot_neg_indicate_resolving:master mac[%02x:XX:XX:XX:%02x:%02x]}",
        master_addr[MAC_ADDR_0], master_addr[MAC_ADDR_4], master_addr[MAC_ADDR_5]);
    /* 单群岛指示先不处理 */
    uslot_bitmap_info.start_idx = *(payload + TIMESLOT_INDICATE_START_IDX_POS);
    uslot_bitmap_info.end_idx = *(payload + TIMESLOT_INDICATE_END_IDX_POS);
    uslot_bitmap_info.timeslot_interval = uslot_bitmap_info.end_idx - uslot_bitmap_info.start_idx + 1;
    uslot_bitmap_info.uslot_bitmap = hmac_nbw_convert_idx_to_uslotbitmap_level1(uslot_bitmap_info.start_idx,
        uslot_bitmap_info.end_idx);
    oam_warning_log3(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_indicate_resolving uslotbitmap start_idx: %d, end_idx; %d, "
        "bitmap: %d", uslot_bitmap_info.start_idx, uslot_bitmap_info.end_idx, uslot_bitmap_info.uslot_bitmap);
    /* 填入时隙管理并配置给固件 */
    /* 因为是时隙申请指示，所以肯定是新增 */
    slot_idx = hmac_nbw_find_slave_unused_slot_idx();
    if (slot_idx == NBW_INVALID_IDX) {
        return OAL_FAIL;
    }
    if (memcpy_s(slave_slot_info[slot_idx].slave_ap_mac, WLAN_MAC_ADDR_LEN,
        peer_addr, WLAN_MAC_ADDR_LEN) != EOK) {
        return OAL_FAIL;
    }
    slave_slot_info[slot_idx].slot_info.uslot_info = uslot_bitmap_info;
    slave_slot_info[slot_idx].slot_info.shared_flag = 0;
    slave_slot_info[slot_idx].use_flag = OAL_TRUE;
    hmac_nbw_slave_link_cnt_add();
    hmac_user = mac_vap_get_hmac_user_by_addr(&(hmac_vap->st_vap_base_info), peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_neg_indicate_resolving::NO USER ERROR.");
        return OAL_FAIL;
    }
    hmac_nbw_sync_user_bitmap(hmac_vap, hmac_user, mac_nbw_uslot_bitmap_to_bitmap(uslot_bitmap_info.uslot_bitmap));
    hmac_nbw_update_uslot_bitmap_for_slot_alloced(uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info));
    hmac_nbw_slave_config_sync_slaveap(hmac_vap);

    return OAL_SUCC;
}


static void hmac_nbw_timeslot_neg_indicate_handle(hmac_vap_stru *hmac_vap, uint8_t *payload,
    uint16_t payload_len, uint8_t *peer_addr)
{
    uint32_t ret;
    hmac_user_stru *hmac_user = NULL;
    hmac_nbw_current_connect_info_stru *nbw_curr_info = hmac_nbw_get_curr_conn_info();
    oam_warning_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_timeslot_neg_indicate_handle start.");
    /* 如果不是建链中、或者不是SLAVE角色、或者对端设备地址不是目标SlaveAp, 则不处理 */
    if (nbw_curr_info->connect_status != MAC_NBW_CONNECT_ASSOC ||
        nbw_curr_info->connect_role != MAC_NBW_SLAVE_ROLE ||
        oal_compare_mac_addr(peer_addr, nbw_curr_info->peer_addr) != 0) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_timeslot_indicate_handle:: not need handle slot indicate.");
        return;
    }

    /* 校验帧长的合法性 */
    if (payload_len < TIMESLOT_INDICATE_BODY_LEN_MIN) {
        oam_error_log0(0, OAM_SF_NBW,
            "NBW: hmac_nbw_timeslot_indicate_handle::receive timeslot IND len invalid.");
        return;
    }

    /* 查找hmac user */
    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_timeslot_neg_indicate_handle:: hmac user is null.");
        return;
    }

    /* slave建链初始化完成后，启动定时器等待时隙指示，此处删除定时器 */
    if (hmac_user->chba_user.connect_info.assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(hmac_user->chba_user.connect_info.assoc_waiting_timer));
    }

    ret = hmac_nbw_timeslot_neg_indicate_resolving(hmac_vap, payload, payload_len, peer_addr);
    if (ret == OAL_FAIL) {
        oam_error_log0(0, OAM_SF_NBW,
            "NBW: hmac_nbw_timeslot_neg_indicate_handle::slot apply failed, report connect failed.");
        /* 时隙申请不成功，上报建链失败 */
        hmac_nbw_connect_fail_handler(hmac_vap);
        return;
    }

    /* 如果已同步并时隙申请完成，清除当前建链信息 */
    if (hmac_chba_get_sync_state() == CHBA_WORK) {
        hmac_nbw_clear_curr_conn_info();
    }
}


void hmac_nbw_rx_timeslot_neg_handle(hmac_vap_stru *hmac_vap, uint8_t *peer_addr,
    uint8_t *payload, uint16_t payload_len)
{
    uint8_t request_type;
    if (payload_len < MIN_TIMESLOT_BODY_LEN) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_NBW,
            "hmac_nbw_rx_timeslot_req_handle::receive timeslot action frame len[%d] invalid.", payload_len);
        return;
    }
    request_type = *payload;
    oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_NBW,
        "hmac_nbw_rx_timeslot_req_handle::receive timeslot action frame, subtype is %d.", request_type);
    switch (request_type) {
        case MAC_NBW_TIMESLOT_NEG_REQUEST_ACTION:
            hmac_nbw_timeslot_neg_request_handle(hmac_vap, payload + 1, payload_len - 1, peer_addr);
            break;
        case MAC_NBW_TIMESLOT_NEG_RESPONSE_ACTION:
            hmac_nbw_timeslot_neg_response_handle(hmac_vap, peer_addr, payload + 1, payload_len - 1);
            break;
        case MAC_NBW_TIMESLOT_NEG_INDICATE_ACTION:
            hmac_nbw_timeslot_neg_indicate_handle(hmac_vap, payload + 1, payload_len - 1, peer_addr);
            break;
        default:
            break;
    }
}


static void hmac_nbw_timeslot_adjust_master_slaveap_req_handler(hmac_vap_stru *hmac_vap,
    uint8_t *payload, uint16_t payload_len, uint8_t *peer_addr)
{
    uint8_t ret, flag;
    uint8_t slave_addr[WLAN_MAC_ADDR_LEN];
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    hmac_nbw_uslot_info_stru send_bitmap_info = { 0 };
    if (payload_len < TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_BODY_LEN || hmac_nbw_get_role() != MAC_NBW_MASTER_ROLE) {
        oam_error_log0(0, OAM_SF_NBW,
            "hmac_nbw_timeslot_adjust_master_slaveap_req_handler::len invalid or no master handle it.");
        return;
    }

    if (memcpy_s(slave_addr, WLAN_MAC_ADDR_LEN, payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_SLAVEMAC_POS,
        WLAN_MAC_ADDR_LEN) != EOK) {
        return;
    }
    flag = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_FLAG_POS);
    /* 先做简化方案，不考虑before after，时隙调整直接覆盖，如果start和end都为127，则认为是主动释放资源 */
    uslot_bitmap_info.start_idx = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_START_IDX_POS);
    uslot_bitmap_info.end_idx = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_END_IDX_POS);
    if (uslot_bitmap_info.start_idx == INVALID_TIMESLOT_USLOT_IDX &&
        uslot_bitmap_info.end_idx == INVALID_TIMESLOT_USLOT_IDX) {
        /* 释放该链路的时隙资源 */
        if (flag == 0) {
            hmac_nbw_release_timeslot(slave_addr, WLAN_MAC_ADDR_LEN, peer_addr, WLAN_MAC_ADDR_LEN);
        } else if (flag == 1) {
            hmac_nbw_release_timeslot(peer_addr, WLAN_MAC_ADDR_LEN, slave_addr, WLAN_MAC_ADDR_LEN);
        } else {
            oam_error_log0(0, OAM_SF_NBW,
                "hmac_nbw_timeslot_adjust_master_slaveap_req_handler::flag error.");
        }
        return;
    }
    /* 如果是非127，则需要先检查申请调整的interval是否可以被调整，无论能否调整，均要回复 */
    uslot_bitmap_info.timeslot_interval = uslot_bitmap_info.end_idx - uslot_bitmap_info.start_idx + 1;
    uslot_bitmap_info.uslot_bitmap = hmac_nbw_timeslot_interval_transfrom(uslot_bitmap_info.timeslot_interval);
    ret = hmac_nbw_master_adjust_timeslot(peer_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info);
    /* 如果不符合调整条件，则发拒绝帧 */
    if (ret == OAL_FAIL) {
        /* 拒绝帧和接受帧的区别在于uslot_info，拒绝的话，在组帧时会检查bitmap，如果是0，则知道是拒绝 */
        hmac_nbw_send_timeslot_frame(peer_addr, slave_addr, WLAN_MAC_ADDR_LEN, send_bitmap_info,
            MAC_NBW_TIMESLOT_ADJUST_ACTION, MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_ACTION);
    } else {
        /* 目前调整帧不包括master帮助调整，如果接受就直接将申请的ulsot_info带回去 */
        send_bitmap_info = uslot_bitmap_info;
        hmac_nbw_send_timeslot_frame(peer_addr, slave_addr, WLAN_MAC_ADDR_LEN, send_bitmap_info,
            MAC_NBW_TIMESLOT_ADJUST_ACTION, MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_ACTION);
    }
}


static void hmac_nbw_timeslot_adjust_master_slaveap_rsp_handler(hmac_vap_stru *hmac_vap,
    uint8_t *payload, uint16_t payload_len, uint8_t *peer_addr)
{
    uint8_t status, slot_idx;
    uint8_t slave_addr[WLAN_MAC_ADDR_LEN];
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    hmac_user_stru *hmac_user = NULL;
    slot_info_stru *island_slot_info = hmac_nbw_get_slaveap_island_slot_info();
    if (payload_len < TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_MIN_BODY_LEN ||
        (hmac_nbw_get_role() != MAC_NBW_SLAVEAP_ROLE && hmac_nbw_get_role() != MAC_NBW_SLAVEAP_SLAVE_MIX_ROLE)) {
        oam_error_log0(0, OAM_SF_NBW,
            "hmac_nbw_timeslot_adjust_master_slaveap_rsp_handler::len invalid or no master handle it.");
        return;
    }
    status = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_STATUS_POS);
    /* 如果status是拒绝，则直接结束，说明：后面要加一个标志来指示是slave发起的还是slaveap发起的 */
    /* 如果是slave发起的，即使是拒绝，也要通知给slave */
    if (status == MAC_NBW_REJECT) {
        return;
    }
    if (status == MAC_NBW_ACCEPT && payload_len < TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_BODY_LEN) {
        return;
    }
    if (memcpy_s(slave_addr, WLAN_MAC_ADDR_LEN, payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_SLAVEMAC_POS,
        WLAN_MAC_ADDR_LEN) != EOK) {
        return;
    }
    /* 如果status是成功，则要通知给slave */
    uslot_bitmap_info.start_idx = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_START_IDX_POS);
    uslot_bitmap_info.end_idx = *(payload + TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_END_IDX_POS);
    uslot_bitmap_info.timeslot_interval = uslot_bitmap_info.end_idx - uslot_bitmap_info.start_idx + 1;
    uslot_bitmap_info.uslot_bitmap = hmac_nbw_convert_idx_to_uslotbitmap_level1(uslot_bitmap_info.start_idx,
        uslot_bitmap_info.end_idx);

    /* 更新岛主的时隙管理信息, 因为是时隙调整，找到之前的时隙资源，直接覆盖 */
    slot_idx = hmac_nbw_find_island_link_idx(slave_addr, WLAN_MAC_ADDR_LEN);
    if (slot_idx == NBW_INVALID_IDX) {
        return;
    }
    island_slot_info[slot_idx].per_slot_info.uslot_info = uslot_bitmap_info;
    hmac_nbw_send_timeslot_frame(slave_addr, slave_addr, WLAN_MAC_ADDR_LEN, uslot_bitmap_info,
        MAC_NBW_TIMESLOT_ADJUST_ACTION, MAC_NBW_TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_ACTION);
    hmac_user = mac_vap_get_hmac_user_by_addr(&(hmac_vap->st_vap_base_info), slave_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_adjust_master_slaveap_rsp_handler::NO USER ERROR.");
        return;
    }
    hmac_nbw_sync_user_bitmap(hmac_vap, hmac_user, mac_nbw_uslot_bitmap_to_bitmap(uslot_bitmap_info.uslot_bitmap));
    hmac_nbw_update_uslot_bitmap_for_slot_alloced(uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info));
}


static void hmac_nbw_timeslot_adjust_slave_slaveap_rsp_handler(hmac_vap_stru *hmac_vap,
    uint8_t *payload, uint16_t payload_len, uint8_t *peer_addr)
{
    uint8_t status, slot_idx;
    hmac_nbw_uslot_info_stru uslot_bitmap_info = { 0 };
    slave_slot_stru *slave_slot_info = hmac_nbw_get_slave_slot_info();
    hmac_user_stru *hmac_user = NULL;
    if (payload_len < TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_MIN_BODY_LEN || hmac_nbw_get_role() != MAC_NBW_SLAVE_ROLE) {
        oam_error_log0(0, OAM_SF_NBW,
            "hmac_nbw_timeslot_adjust_slave_slaveap_rsp_handler::len invalid or no master handle it.");
        return;
    }
    status = *(payload + TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_STATUS_POS);
    /* 此次slave发起的时隙调整失败 */
    if (status == MAC_NBW_REJECT) {
        return;
    }
    if (status == MAC_NBW_ACCEPT && payload_len < TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_BODY_LEN) {
        return;
    }
    /* 如果status是成功，则取出新的bitmap info, 填入时隙资源并配给固件 */
    uslot_bitmap_info.start_idx = *(payload + TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_START_IDX_POS);
    uslot_bitmap_info.end_idx = *(payload + TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_END_IDX_POS);
    uslot_bitmap_info.timeslot_interval = uslot_bitmap_info.end_idx - uslot_bitmap_info.start_idx + 1;
    uslot_bitmap_info.uslot_bitmap = hmac_nbw_convert_idx_to_uslotbitmap_level1(uslot_bitmap_info.start_idx,
        uslot_bitmap_info.end_idx);
    slot_idx = hmac_nbw_find_slave_link_slot_idx(peer_addr);
    if (slot_idx == NBW_INVALID_IDX) {
        return;
    }
    slave_slot_info[slot_idx].slot_info.uslot_info = uslot_bitmap_info;
    hmac_user = mac_vap_get_hmac_user_by_addr(&(hmac_vap->st_vap_base_info), peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "hmac_nbw_timeslot_adjust_slave_slaveap_rsp_handler::NO USER ERROR.");
        return;
    }
    hmac_nbw_sync_user_bitmap(hmac_vap, hmac_user, mac_nbw_uslot_bitmap_to_bitmap(uslot_bitmap_info.uslot_bitmap));
    hmac_nbw_update_uslot_bitmap_for_slot_alloced(uslot_bitmap_info.uslot_bitmap);
    hmac_nbw_sync_vap_bitmap_info(&(hmac_vap->st_vap_base_info));
}


void hmac_nbw_rx_timeslot_adjust_handle(hmac_vap_stru *hmac_vap, uint8_t *peer_addr,
    uint8_t *payload, uint16_t payload_len)
{
    uint8_t request_type;
    if (payload_len < MIN_TIMESLOT_BODY_LEN) {
        oam_error_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_NBW,
            "hmac_nbw_rx_timeslot_adjust_handle::receive timeslot action frame len[%d] invalid.", payload_len);
        return;
    }
    request_type = *payload;
    oam_warning_log1(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_NBW,
        "hmac_nbw_rx_timeslot_req_handle::receive timeslot action frame, subtype is %d.", request_type);
    /* 先不考虑before after，一条链对应一个连续时隙段 */
    switch (request_type) {
        case MAC_NBW_TIMESLOT_ADJUST_SLAVE_SLAVEAP_REQ_ACTION:
            break;
        case MAC_NBW_TIMESLOT_ADJUST_SLAVE_SLAVEAP_RSP_ACTION:
            hmac_nbw_timeslot_adjust_slave_slaveap_rsp_handler(hmac_vap, payload + 1, payload_len - 1, peer_addr);
            break;
        case MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_REQ_ACTION:
            hmac_nbw_timeslot_adjust_master_slaveap_req_handler(hmac_vap, payload + 1, payload_len - 1, peer_addr);
            break;
        case MAC_NBW_TIMESLOT_ADJUST_MASTER_SLAVEAP_RSP_ACTION:
            hmac_nbw_timeslot_adjust_master_slaveap_rsp_handler(hmac_vap, payload + 1, payload_len - 1, peer_addr);
            break;
        case MAC_NBW_TIMESLOT_ADJUST_MASTER_INDICATE_ACTION:
            break;
        default:
            break;
    }
}

static void hmac_nbw_slave_rx_beacon_proc(hmac_vap_stru *hmac_vap, uint8_t *peer_addr, uint8_t addr_len)
{
    hmac_nbw_current_connect_info_stru *curr_conn_info = NULL;

    if (addr_len > WLAN_MAC_ADDR_LEN) {
        return;
    }
    /* 建链过程中只处理岛主的beacon帧 */
    curr_conn_info = (hmac_nbw_current_connect_info_stru *)hmac_nbw_get_curr_conn_info();
    if (oal_compare_mac_addr(curr_conn_info->peer_addr, peer_addr) != 0) {
        return;
    }
    /* 如果当前已同步，则返回 */
    if (hmac_chba_get_sync_state() == CHBA_WORK) {
        return;
    }
    /* 配置同步状态 */
    hmac_chba_set_sync_state(CHBA_WORK);
    hmac_nbw_slave_sync_set(hmac_vap);

    /* 如果slave已获取到时隙指示，清除当前建链信息 */
    if (hmac_nbw_find_slave_link_slot_idx(peer_addr) != NBW_INVALID_IDX) {
        hmac_nbw_clear_curr_conn_info();
    }
}

uint32_t hmac_nbw_waiting_master_slot_time_response_proc(void *arg)
{
    hmac_vap_stru *hmac_vap = (hmac_vap_stru *)arg;
    hmac_nbw_current_connect_info_stru *nbw_curr_conn_info = hmac_nbw_get_curr_conn_info();
    hmac_user_stru *hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info,
        nbw_curr_conn_info->peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_error_log0(0, OAM_SF_NBW, "NBW: hmac_nbw_waiting_master_slot_time_response_proc::hmac user is null.");
        return 0;
    }
    hmac_nbw_connect_fail_handler(hmac_vap);
    oam_warning_log0(0, OAM_SF_NBW,
        "NBW: hmac_nbw_waiting_master_slot_time_response_proc::receive timeslot action frame timeout.");
    return 0;
}

static void hmac_nbw_slaveap_rx_beacon_proc(hmac_vap_stru *hmac_vap, uint8_t *payload,
    uint8_t *peer_addr, uint8_t addr_len)
{
    hmac_user_stru *hmac_user = NULL;
    hmac_nbw_current_connect_info_stru *conn_info = hmac_nbw_get_curr_conn_info();
    chba_params_config_stru chba_params_sync = { 0 };

    if (addr_len > WLAN_MAC_ADDR_LEN) {
        return;
    }
    /* 如果不是master beacon，则本函数不处理，如果是master beacon，则在hmac_nbw_master_election_finish_proc定时器内处理 */
    if (payload[MAC_CHBA_ATTR_HDR_LEN] != MASTER_BEACON_INDICATE) {
        oam_info_log0(0, OAM_SF_CHBA, "NBW: {hmac_nbw_slaveap_rx_beacon_proc::is not master beacon.}");
        return;
    }
    /* 如果当前已同步，则返回 */
    if (hmac_chba_get_sync_state() == CHBA_WORK) {
        return;
    }
    oam_warning_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_rx_beacon:: slaveap process beacon.");
    /* 1. slaveap接收master beacon后，停止监听定时器 */
    if (conn_info->assoc_waiting_timer.en_is_registerd == OAL_TRUE) {
        frw_timer_immediate_destroy_timer_m(&(conn_info->assoc_waiting_timer));
    }

    /* 2. 同步master */
    hmac_nbw_set_role(MAC_NBW_SLAVEAP_ROLE);
    oal_set_mac_addr(hmac_nbw_get_master_macaddr(), peer_addr);
    hmac_chba_set_sync_state(CHBA_WORK);
    oam_warning_log3(hmac_vap->st_vap_base_info.uc_vap_id, OAM_SF_NBW,
        "{hmac_nbw_basic_beacon_handler:become slaveap! Master is %02x:XX:XX:XX:%02x:%02x}",
        peer_addr[MAC_ADDR_0], peer_addr[MAC_ADDR_4], peer_addr[MAC_ADDR_5]);

    chba_params_sync.chba_role = MAC_NBW_SLAVEAP_ROLE;
    oal_set_mac_addr(chba_params_sync.domain_bssid, hmac_nbw_get_master_macaddr());
    chba_params_sync.chba_state = CHBA_WORK;
    chba_params_sync.info_bitmap = CHBA_STATE | CHBA_ROLE | CHBA_BSSID;
    hmac_chba_params_sync(&hmac_vap->st_vap_base_info, &chba_params_sync);

    /* 3. 如果已经关联，则向master申请时隙 */
    hmac_user = mac_vap_get_hmac_user_by_addr(&hmac_vap->st_vap_base_info, conn_info->peer_addr, WLAN_MAC_ADDR_LEN);
    if (hmac_user == NULL) {
        oam_warning_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_slaveap_rx_beacon_proc:: hmac user is null.");
        return;
    }
    if (hmac_user->st_user_base_info.en_user_asoc_state == MAC_USER_STATE_ASSOC) {
        hmac_nbw_apply_slot(hmac_vap);
    }
}


uint32_t hmac_nbw_rx_beacon(hmac_vap_stru *hmac_vap, mac_ieee80211_frame_stru *mac_hdr,
    uint8_t *payload, uint16_t payload_len)
{
    uint8_t sa_addr[WLAN_MAC_ADDR_LEN], domain_bssid[WLAN_MAC_ADDR_LEN];
    uint8_t *nbw_ie = NULL;
    uint16_t nbw_ie_len;
    mac_ies_info ies_info = {0};
    hmac_nbw_current_connect_info_stru *curr_conn_info = NULL;

    /* 固定长度检查 */
    if (payload_len < MAC_NBW_BEACON_ELEMENT_OFFSET) {
        oam_error_log1(0, OAM_SF_CHBA, "NBW: {hmac_nbw_rx_beacon::len is too short[%d].}", payload_len);
        return OAL_FAIL;
    }

    /* 解析hdr头地址 */
    mac_get_address2((uint8_t *)mac_hdr, sa_addr, WLAN_MAC_ADDR_LEN);
    mac_get_address3((uint8_t *)mac_hdr, domain_bssid, WLAN_MAC_ADDR_LEN);
    /* 解析nbw 私有IE, 并完成合法性检查 */
    ies_info.frame_ies = payload + MAC_NBW_BEACON_ELEMENT_OFFSET;
    ies_info.frame_len = payload_len - MAC_NBW_BEACON_ELEMENT_OFFSET;
    nbw_ie = mac_find_vendor_ie(MAC_CHBA_VENDOR_IE, MAC_OUI_TYPE_NBW, &ies_info);
    if (nbw_ie == NULL) {
        oam_error_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_rx_beacon::can't find nbw ie in beacon frame.");
        return OAL_FAIL;
    }
    if (((uint16_t)(nbw_ie[1]) + MAC_IE_HDR_LEN) < sizeof(mac_ieee80211_vendor_ie_stru)) {
        oam_error_log1(0, OAM_SF_CHBA, "NBW: hmac_nbw_rx_beacon::find nbw_ie len[%d]", nbw_ie[1]);
        return OAL_FAIL;
    }
    nbw_ie_len = ((uint16_t)(nbw_ie[1]) + MAC_IE_HDR_LEN) - sizeof(mac_ieee80211_vendor_ie_stru);
    nbw_ie = nbw_ie + sizeof(mac_ieee80211_vendor_ie_stru);

    curr_conn_info = (hmac_nbw_current_connect_info_stru *)hmac_nbw_get_curr_conn_info();
    if (curr_conn_info->connect_status == MAC_NBW_CONNECT_NEGOTIATE) {
        /* 建链协商过程中处理beacon帧 */
    } else if (curr_conn_info->connect_status == MAC_NBW_CONNECT_ASSOC) { /* 建链关联过程中处理beacon帧 */
        if (curr_conn_info->connect_role == MAC_NBW_SLAVE_ROLE) {
            hmac_nbw_slave_rx_beacon_proc(hmac_vap, sa_addr, WLAN_MAC_ADDR_LEN);
        } else if (curr_conn_info->connect_role == MAC_NBW_SLAVEAP_ROLE) {
            hmac_nbw_slaveap_rx_beacon_proc(hmac_vap, nbw_ie, sa_addr, WLAN_MAC_ADDR_LEN);
        }
    } else {
        oam_error_log0(0, OAM_SF_CHBA, "NBW: hmac_nbw_rx_beacon:: connect_status is valid.");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/* 功能: NBW Vendor IE Header填写 */
static uint32_t hmac_nbw_set_vendor_ie_hdr(uint8_t *buf)
{
    if (buf == NULL) {
        return 0;
    }

    buf[BYTE_OFFSET_0] = MAC_NBW_VENDOR_EID; /* vendor ie: 221 */
    buf[BYTE_OFFSET_2] = 0x00; /* 2: oui[0] */
    buf[BYTE_OFFSET_3] = 0xE0; /* 3: oui[1] */
    buf[BYTE_OFFSET_4] = 0xFC; /* 4: oui[2] */
    buf[BYTE_OFFSET_5] = MAC_OUI_TYPE_NBW;
    return sizeof(mac_ieee80211_vendor_ie_stru);
}

/*
 * 功能描述  : 封装Master Election Attribute
 * 日    期  : 2021年02月04日
 * 作    者  : wifi
 */
static void hmac_nbw_set_basic_beacon_attr(uint8_t *attr_buf)
{
    uint8_t index;

    if (attr_buf == NULL) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_chba_set_master_election_attr: input pointer is null.");
        return;
    }

    /* Attribute ID */
    attr_buf[0] = MAC_NBW_ATTR_BASIC;
    index = MAC_CHBA_ATTR_HDR_LEN;

    /* Attribute Content */
    /* 根据role信息填写is_master字段 */
    if (hmac_nbw_get_role() == MAC_NBW_MASTER_ROLE) {
        attr_buf[index++] = MASTER_BEACON_INDICATE;
    } else {
        attr_buf[index++] = 0;
    }

    /* 设置Attr Length字段 */
    attr_buf[MAC_CHBA_ATTR_ID_LEN] = index - MAC_CHBA_ATTR_HDR_LEN;
}


void hmac_nbw_encap_master_beacon(hmac_chba_vap_stru *chba_vap_info, uint8_t *beacon_buf, uint16_t *beacon_len,
    uint8_t beacon_type)
{
    uint16_t total_len = 0;
    uint16_t vendor_ie_len;
    uint8_t *vendor_ie_len_pos = NULL;

    if (oal_any_null_ptr3(chba_vap_info, beacon_buf, beacon_len)) {
        oam_warning_log0(0, OAM_SF_CHBA, "hmac_chba_encap_sync_beacon: input pointer is null.");
        return;
    }

    /* 填写NBW Vendor IE Header */
    hmac_nbw_set_vendor_ie_hdr(beacon_buf);
    vendor_ie_len_pos = beacon_buf + 1;
    vendor_ie_len = MAC_CHBA_ACTION_HDR_LEN - MAC_CHBA_ATTR_HDR_LEN;
    total_len += MAC_CHBA_ACTION_HDR_LEN;

    switch (beacon_type) {
        case MAC_NBW_BEACON_TYPE_BASIC:
            hmac_nbw_set_basic_beacon_attr(beacon_buf + total_len);
            total_len += MAC_NBW_BEACON_BASIC_ATTR_LEN;
            vendor_ie_len += MAC_NBW_BEACON_BASIC_ATTR_LEN;
            break;
        default:
            oam_warning_log0(0, OAM_SF_CHBA, "hmac_nbw_encap_master_beacon: beacon type is unknown.");
            break;
    }

    *vendor_ie_len_pos = vendor_ie_len;
    *beacon_len = total_len;
}
#endif

