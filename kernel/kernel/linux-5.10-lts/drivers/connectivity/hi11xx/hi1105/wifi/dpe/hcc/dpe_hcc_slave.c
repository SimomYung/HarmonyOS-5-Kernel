/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HMAC模块HCC层适配
 * 作    者 :
 * 创建日期 : 2014年10月8日
 */
#ifdef _PRE_MULTI_CORE_DPE_OFFLOAD

#include "dpe_hcc.h"
#include "mac_resource.h"
#include "oal_hcc_host_if.h"
#include "frw_event_main.h"
#include "hmac_vap.h"
#include "wlan_chip_i.h"
#include "host_hal_device.h"
#include "host_hal_ext_if.h"
#include "hmac_wifi_delay.h"
#include "securec.h"
#include "securectype.h"
#include "oam_event_wifi.h"

int32_t hcc_slave_rx_wifi_post_action_function(
    struct hcc_handler *hcc, uint8_t stype, hcc_netbuf_stru *skb, uint8_t *context)
{
    oal_netbuf_stru *netbuf = skb->pst_netbuf;
    frw_event_stru *frw_event = (frw_event_stru*)oal_sbuf_cb(netbuf); /* 事件信息维护在sbuf cb(128 bytes) */
    struct frw_hcc_extend_hdr *frw_hdr = (struct frw_hcc_extend_hdr *)&(frw_event->hcc_hdr.frw_hdr);

    if (frw_hdr->en_nest_type >= FRW_EVENT_TYPE_BUTT) {
        oal_print_hex_dump(oal_sbuf_cb(netbuf),
            (int32_t)sizeof(frw_event_stru), 32, "invaild frw extend hdr: "); // 32代表长度
        oal_netbuf_free(netbuf);
        return 0;
    }

    /* ACPU->DPA、ACPU->DPA->WCPU依赖FRW继续转发WCPU */
    frw_event_param_stru param = {
        .type = frw_hdr->en_nest_type,
        .subtype = frw_hdr->uc_nest_sub_type,
        .hal_device_id = frw_hdr->device_id,
        .vap_id = frw_hdr->vap_id,
        .pipeline = FRW_EVENT_PIPELINE_STAGE_1,
        .transfer_event_to = FRW_EVENT_TO_WCORE, /* slave hcc接收的去往WCPU */
        .payload_size = oal_netbuf_len(netbuf),
        .payload = NULL,
        .netbuf = netbuf,
    };
    if (frw_post_event(&param) != OAL_SUCC) {
        oal_print_hex_dump(oal_sbuf_cb(netbuf),
            (int32_t)sizeof(frw_event_stru), 32, "invaild frw extend hdr: "); // 32代表长度
        oal_netbuf_free(netbuf);
    }
    return 0;
}

void acpu_rx_register_hcc_action(void)
{
    hcc_slave_rx_registerd(HCC_ACTION_TYPE_WIFI, hcc_slave_rx_wifi_post_action_function, NULL);
}
#endif
