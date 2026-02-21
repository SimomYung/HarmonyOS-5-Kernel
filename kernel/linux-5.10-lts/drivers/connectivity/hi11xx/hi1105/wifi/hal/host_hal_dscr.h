/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host 描述符公共部分
 * 作    者 :
 * 创建日期 : 2021年2月22日
 */

#ifndef HOST_HAL_DSCR_H
#define HOST_HAL_DSCR_H

#include "wlan_spec_host_if.h"
#include "host_hal_device.h"

#define LARGE_NETBUF_SIZE            1580
#define SMALL_NETBUF_SIZE            1500
#define ALRX_NETBUF_SIZE             4112
typedef struct {
    uint32_t bit_user_id     : 12,
             bit_tid         : 4,
             bit_tx_ba_ssn   : 12,
             bit_resv        : 3,
             bit_ba_info_vld : 1;
    uint32_t bit_tx_msdu_info_ring_rptr : 16,
             bit_tx_ba_winsize : 8,
             reserved : 8;
    uint32_t ba_bitmap[HAL_BA_BITMAP_SIZE];
} hal_tx_ba_info_dscr_stru;

typedef enum {
    HAL_TX_RING_SIZE_GRADE_DEFAULT = 0,
    HAL_TX_RING_SIZE_GRADE_DOWNGRADE = 1,
    HAL_TX_RING_SIZE_GRADE_DOWNGRADE_TWICE = 2,

    HAL_TX_RING_SIZE_GRADE_BUTT,
} hal_tx_ring_size_grade;

#define HAL_HOST_USER_TID_TX_RING_SIZE_ERROR 0xff
#define BA_INFO_DATA_SIZE sizeof(hal_tx_ba_info_dscr_stru)
#define HAL_DEFAULT_BA_INFO_COUNT 64

#define HAL_RX_MSDU_NORMAL_RING_NUM0 256
#define HAL_RX_MSDU_NORMAL_RING_NUM1 4095

#define HAL_RX_MSDU_SMALL_RING_NUM0 512
#define HAL_RX_MSDU_SMALL_RING_NUM1 1024
#define HAL_HOST_MONITOR_COUNT_2G   128
#define HAL_HOST_MONITOR_COUNT_5G   256
#define HAL_RX_ENTRY_SIZE             8

#define HAL_RX_RING_RESET_MASK      0x30 // reset free ring

/* 为了防止rx complete ring overrun, complete ring的深度设置为最大值 */
#define HAL_RX_COMPLETE_RING_MAX 4095

static inline uint32_t get_low_32bits(uint64_t a)
{
    return ((uint32_t)(((uint64_t)(a)) & 0x00000000FFFFFFFFUL));
}
static inline uint32_t get_high_32bits(uint64_t a)
{
    return ((uint32_t)((((uint64_t)(a)) & 0xFFFFFFFF00000000UL) >> 32UL));
}

oal_netbuf_stru *hal_alloc_list_delete_netbuf(hal_host_device_stru *hal_dev,
    hal_host_rx_alloc_list_stru *alloc_list, dma_addr_t host_iova, uint32_t *pre_num);
void hal_host_rx_add_buff(hal_host_device_stru *hal_device, uint8_t q_num);
void hal_host_intr_regs_init(uint8_t hal_dev_id);
int32_t hal_rx_host_stop_dscr_queue(uint8_t hal_dev_id);
uint32_t hal_rx_host_start_dscr_queue(uint8_t hal_dev_id);
int32_t hal_rx_host_init_dscr_queue(uint8_t hal_dev_id);
uint32_t hal_host_rx_reset_smac_handler(hal_host_device_stru *hal_dev);
void hal_rx_free_res(hal_host_device_stru *hal_device);
void hal_rx_alloc_list_free(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list);
uint8_t hal_host_rx_get_mac_header_len(mac_rx_ctl_stru *rx_ctl);
uint8_t hal_rx_ring_reg_init(hal_host_device_stru *hal_dev, hal_host_ring_ctl_stru *rctl, uint8_t ring_id);
void hal_rx_set_ring_regs(hal_host_ring_ctl_stru *rx_ring_ctl);
oal_netbuf_stru *hal_rx_get_sub_msdu(hal_host_device_stru *hal_device, uint64_t next_dscr_addr);
void hal_host_rx_proc_msdu_dscr(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf);
#endif /* end of host_hal_dscr.h */
