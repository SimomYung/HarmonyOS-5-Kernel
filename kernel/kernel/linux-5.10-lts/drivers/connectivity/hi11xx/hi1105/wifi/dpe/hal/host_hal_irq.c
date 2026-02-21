/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL MAIN
 * 作    者 :
 * 创建日期 : 2020年06月14日
 */

#include "oal_util.h"
#include "oal_net.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "pcie_linux.h"
#include "pcie_host.h"
#include "host_hal_ext_if.h"
#include "host_hal_ops.h"
#include "host_hal_ring.h"
#include "host_hal_dscr.h"
#include "dpe_wlan_device.h"
#include "hmac_host_performance.h"
#include "hmac_rx_complete.h"
#include "hmac_tx_complete.h"
#include "host_hal_irq.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_IRQ_C

uint32_t g_int_state_reg_val = 0;
uint32_t g_int_mask_reg_val = 0;

void hal_clear_host_mac_irq_mask(void)
{
    hal_host_device_stru *hal_dev = hal_pcie_down_get_hal_dev(0);
    dpe_wlan_device_stru *dpe_device = dpe_wlan_device_get(0);
    uint32_t mask = 0xffffffff;

    if (hal_dev == NULL || dpe_device == NULL) {
        return;
    }

    hal_get_host_mac_int_mask(hal_dev, &mask);
    dpe_device->pre_host_mac_irq_mask = mask;
    hal_set_host_mac_int_mask(hal_dev, 0xffffffff);
    oam_warning_log1(0, OAM_SF_ANY, "{clear_host_mac_irq_mask: pcie_switch irq mask[0x%x].}", mask);
}

void hal_recover_host_mac_irq_mask(void)
{
    hal_host_device_stru *hal_dev = NULL;
    dpe_wlan_device_stru *dpe_device = dpe_wlan_device_get(0);

    if (dpe_device == NULL) {
        return;
    }
    hal_dev = hal_pcie_down_get_hal_dev(0);
    if (hal_dev == NULL) {
        return;
    }
    hal_set_host_mac_int_mask(hal_dev, dpe_device->pre_host_mac_irq_mask);
    oam_warning_log1(0, OAM_SF_ANY, "{recover_host_mac_irq_mask: pcie_switch irq mask[0x%x].}",
        dpe_device->pre_host_mac_irq_mask);
}
/*
 * 功能描述   : free ring 空中断服务例程
 * 1.日    期   : 2019年11月20日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void hal_irq_host_rx_ring_empty_isr(hal_host_device_stru *hal_device, uint32_t mask)
{
    if (!hal_device->rx_q_inited) {
        oam_error_log1(0, OAM_SF_RX, "host_rx_ring_empty_isr:: device[%d] not inited", hal_device->device_id);
        oam_warning_log4(0, OAM_SF_RX, "{host_rx_ring_empty_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_device->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_device->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
        return;
    }
    hmac_rx_add_buff_process(hal_device);
}

/*
 * 功能描述   : 发消息到中断下半部对收到的MPDU进行处理
 * 1.日    期   : 2020年3月4日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC void hal_irq_host_rx_complete_notify(hal_host_device_stru *hal_device)
{
    frw_event_stru           *event = NULL;
    frw_event_mem_stru       *event_mem = NULL;
    hal_host_rx_event        *wlan_rx_event = NULL;

    event_mem = frw_event_alloc_m(sizeof(hal_host_rx_event));
    if (event_mem == NULL) {
        return;
    }

    event = frw_get_event_stru(event_mem);
    frw_event_hdr_init(&(event->st_event_hdr),
                       FRW_EVENT_TYPE_HOST_DDR_DRX,
                       HAL_WLAN_DDR_DRX_EVENT_SUBTYPE,
                       sizeof(hal_host_rx_event),
                       FRW_EVENT_PIPELINE_STAGE_0,
                       0, hal_device->device_id, 0);

    wlan_rx_event = (hal_host_rx_event *)event->auc_event_data;
    wlan_rx_event->hal_dev = hal_device;
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
    return;
}
/*
 * 功能描述   : 将临时netbuf链放入乒乓链表
 * 1.日    期   : 2019年7月4日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
OAL_STATIC void hal_host_rx_mpdu_que_push(hal_rx_mpdu_que *rx_mpdu, oal_netbuf_head_stru *netbuf_head,
    uint32_t *ori_skb_num)
{
    oal_netbuf_head_stru *rx_que = NULL;
    unsigned long         lock_flag;

    oal_spin_lock_irq_save(&rx_mpdu->st_spin_lock, &lock_flag);
    rx_que = &(rx_mpdu->ast_rx_mpdu_list[rx_mpdu->cur_idx]);
    *ori_skb_num = oal_netbuf_get_buf_num(rx_que);
    oal_netbuf_queue_splice_tail_init(netbuf_head, rx_que);
    oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);
}

/*
 * 功能描述   : 接收数据完成中断host处理
 * 1.日    期   : 2018年5月13日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void hal_irq_host_rx_complete_isr(hal_host_device_stru *hal_device, uint32_t mask)
{
    uint32_t                     ret;
    oal_netbuf_head_stru         rx_msdu_que;
    hal_rx_mpdu_que             *rx_mpdu = NULL;
    uint32_t                     ori_skb_num = 0;
    host_start_record_performance(RX_ISR_PROC);

    if (!hal_device->rx_q_inited) {
        oam_error_log1(0, OAM_SF_RX, "irq_host_mac_isr:: device[%d] not inited", hal_device->device_id);
        oam_warning_log4(0, OAM_SF_RX, "{hal_irq_host_rx_complete_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_device->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_device->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
        return;
    }
    hmac_rx_add_buff_process(hal_device);

    rx_mpdu = &(hal_device->st_rx_mpdu);
    oal_netbuf_list_head_init(&rx_msdu_que);
    /* 从complete ring上读取元素，并存入临时netbuf上报链表 */
    ret = hal_host_rx_ring_entry_get(hal_device, &hal_device->st_host_rx_complete_ring, &rx_msdu_que);
    if (ret != OAL_SUCC) {
        return;
    }

    /* 将临时链表中的元素存入乒乓队列 */
    hal_host_rx_mpdu_que_push(rx_mpdu, &rx_msdu_que, &ori_skb_num);
    /* 如果乒乓链表没有数据，抛事件到下半部处理 */
    if (!ori_skb_num) {
        hal_irq_host_rx_complete_notify(hal_device);
    } else {
        oam_info_log1(0, OAM_SF_RX, "irq_host_mac_isr:: ori_skb_num[%d] not zero", ori_skb_num);
    }

    host_end_record_performance(ori_skb_num, RX_ISR_PROC);
}

/*
 * 功能描述   : irq_host_tx_complete_isr中断服务例程
 * 1.日    期   : 2020年05月10日
 *   修改内容   : 新生成函数
 */
void hal_irq_host_tx_complete_isr(hal_host_device_stru *hal_dev, uint32_t mask)
{
    host_start_record_performance(TX_ISR_PROC);

    if (!hal_dev->intr_inited) {
        oam_warning_log1(0, OAM_SF_TX, "irq_host_tx_complete_isr:: device[%d] not inited", hal_dev->device_id);
        oam_warning_log4(0, OAM_SF_TX, "{irq_host_tx_complete_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_dev->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_dev->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
    }
    hmac_set_tx_comp_triggered(hal_dev->device_id);
    hmac_tx_comp_scheduled();

    host_end_record_performance(1, TX_ISR_PROC);
}

#ifdef _PRE_WLAN_FEATURE_CSI
/*
 * 功能描述   : mp16c_host_ftm_csi_event FTM/CSI事件上报
 * 1.日    期   : 2020年11月18日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
static void hal_host_ftm_csi_event(hal_host_device_stru *hal_device, hal_host_location_isr_stru *ftm_csi_isr_event)
{
    frw_event_mem_stru *event_mem = NULL;
    frw_event_stru *hmac_ftm_csi_event = NULL;

    /* 抛加入完成事件到WAL */
    event_mem = frw_event_alloc_m(sizeof(hal_host_location_isr_stru));
    if (event_mem == NULL) {
        oam_error_log0(0, OAM_SF_FTM, "{host_ftm_csi_event::alloc event_mem failed.}\r\n");
        return;
    }
    ftm_csi_isr_event->chip_id = hal_device->chip_id;
    ftm_csi_isr_event->hal_device_id = hal_device->device_id;
    /* 填写事件 */
    hmac_ftm_csi_event = frw_get_event_stru(event_mem);

    /* 填写事件头 */
    frw_event_hdr_init(&(hmac_ftm_csi_event->st_event_hdr), FRW_EVENT_TYPE_HOST_DDR_DRX,
                       HAL_WLAN_FTM_IRQ_EVENT_SUBTYPE,
                       sizeof(hal_host_location_isr_stru), FRW_EVENT_PIPELINE_STAGE_0,
                       0, hal_device->device_id, 0);

    if (memcpy_s(hmac_ftm_csi_event->auc_event_data, sizeof(hal_host_location_isr_stru), ftm_csi_isr_event,
        sizeof(hal_host_location_isr_stru)) != EOK) {
        oam_error_log0(0, OAM_SF_FTM, "host_ftm_csi_event::memcpy fail!");
        frw_event_free_m(event_mem);
        return;
    }

    /* 分发事件 */
    frw_event_dispatch_event(event_mem);
    frw_event_free_m(event_mem);
}

static uint32_t hal_host_ftm_csi_addr_report(uintptr_t info_addr_lsb, uintptr_t info_addr_msb, uintptr_t *addr)
{
    *addr = oal_make_word64(hal_host_readl_irq(info_addr_lsb), hal_host_readl_irq(info_addr_msb));

    if (*addr == 0) {
        oam_warning_log0(0, OAM_SF_FTM, "{host_ftm_csi_addr_report:get addr fail.}");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}
/*
 * 功能描述   : mp16c_irq_host_ftm_csi_isr中断服务例程
 * 1.日    期   : 2020年11月13日
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void hal_irq_host_ftm_csi_isr(hal_host_device_stru *hal_dev, uint32_t mask)
{
    hal_host_location_isr_stru ftm_csi_isr_event;

    if (!hal_dev->intr_inited) {
        oam_error_log4(0, OAM_SF_RX, "irq_host_ftm_csi_isr:: device[%d]not inited, csi init[%d], \
            ftm init[%d], mask[0x%x]", hal_dev->device_id, hal_dev->csi_regs.inited, hal_dev->ftm_regs.inited, mask);
        oam_warning_log4(0, OAM_SF_RX, "{irq_host_ftm_csi_isr::int status reg[0x%x], state val[0x%x], \
            mask reg[0x%x], mask val[0x%x]}", hal_dev->mac_regs.irq_status_reg, g_int_state_reg_val,
            hal_dev->mac_regs.irq_status_mask_reg, g_int_mask_reg_val);
        return;
    }

    ftm_csi_isr_event.ftm_csi_info = hal_get_host_ftm_csi_locationinfo(hal_dev);

    // 考虑ftm、csi全上报的情况，取flag bit0 bit1分别进行判断 bit0:csi、bit1:ftm
    if (!ftm_csi_isr_event.ftm_csi_info) {
        oam_error_log0(0, OAM_SF_FTM, "irq_host_ftm_csi_isr::no ftm csi rpt!");
        return;
    }

    if (ftm_csi_isr_event.ftm_csi_info & BIT0) { // CSI上报
        if (hal_host_ftm_csi_addr_report(hal_dev->csi_regs.csi_info_lsb,
            hal_dev->csi_regs.csi_info_msb, &ftm_csi_isr_event.csi_info_addr) != OAL_SUCC) {
            return;
        }
        hal_host_ftm_csi_event(hal_dev, &ftm_csi_isr_event);
    }

    if (ftm_csi_isr_event.ftm_csi_info & BIT1) { // FTM上报
        if (hal_host_ftm_csi_addr_report(hal_dev->ftm_regs.ftm_info_lsb,
            hal_dev->ftm_regs.ftm_info_msb, &ftm_csi_isr_event.ftm_info_addr) != OAL_SUCC) {
            return;
        }
        hal_host_ftm_csi_event(hal_dev, &ftm_csi_isr_event);
    }
}
#endif
