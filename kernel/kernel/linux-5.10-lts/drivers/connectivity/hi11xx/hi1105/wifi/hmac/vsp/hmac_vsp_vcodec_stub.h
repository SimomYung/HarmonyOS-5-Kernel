/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : vsp打桩头文件
 * 作者       : wifi
 * 创建日期   : 2020年12月24日
 */

#ifndef VCODEC_VSP_BUFFER_H
#define VCODEC_VSP_BUFFER_H

#include "wlan_types.h"
#include "oal_dma_buf.h"

#define VSP_MAX_CB_LEN (48)
#define MAX_LAYER_NUM (3)
#define MAX_PAKET_NUM (2000)
#define PAYLOAD_LENGTH (1472)
#define RESERVE_LENGTH (40)

/* the struct wifi send to vdec */
typedef struct {
    uint32_t fix_len;
    uint32_t payload_offset;
    void *priv[MAX_LAYER_NUM]; /* 接收skb链 */
    uint32_t last_len[MAX_LAYER_NUM];
    uint32_t packet_num[MAX_LAYER_NUM];
    uintptr_t base_addr[MAX_LAYER_NUM][MAX_PAKET_NUM];
} rx_slice_mgmt;

/* the struct for msdu packet ctrl */
typedef struct  {
    uintptr_t next;
    char cb[VSP_MAX_CB_LEN];
    uint8_t mac_ra_address[WLAN_MAC_ADDR_LEN];
    uint8_t mac_sa_address[WLAN_MAC_ADDR_LEN];
    uint16_t qos_type;
    uintptr_t data_addr;
    dma_addr_t iova_addr;
    uint16_t version;
    char reserve[RESERVE_LENGTH];
    uint16_t layer_number;
    uint16_t last_paket_len;
    uint16_t normal_pack_length;
    uint16_t paket_number;
} tx_layer_ctrl;

/* the struct wifi feed back to venc */
typedef struct {
    tx_layer_ctrl *slice_layer;
    uint16_t len;
    uint16_t slice_info;
    uint16_t layer_suc_num[MAX_LAYER_NUM];
    uint16_t slice_layer0_max;
    uint16_t slice_layerall_max;
    char *vendor_spec; // need confirm this variable purpose and length
} send_result;

struct vcodec_venc_callback {
    void (*wifi_tx_pkg_done)(send_result*);
    void (*wifi_tx_send_done)(send_result*);
};

struct vcodec_vdec_callback {
    rx_slice_mgmt *(*alloc_vdec_slice_buffer)(uint32_t);
    void (*wifi_rx_slice_done)(rx_slice_mgmt*);
};

/* vdec call wifi to feed back buffer */
void vdec_rx_slice_done(rx_slice_mgmt *rx_slice);
/* venc call wifi to send slice buffer */
bool wifi_tx_venc_pkg(tx_layer_ctrl *layer_list);
#endif
