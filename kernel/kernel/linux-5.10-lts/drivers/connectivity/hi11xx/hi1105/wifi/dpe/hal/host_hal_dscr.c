/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST HAL DSCR FUNCTION
 * 作    者 :
 * 创建日期 : 2021年2月23日
 */

#include "host_hal_dscr.h"
#include "host_hal_ext_if.h"
#include "pcie_linux.h"
#include "host_hal_ring.h"
#include "pcie_host.h"
#include "dpe_wlan_stat.h"
#ifdef _PRE_WLAN_FEATURE_ESPE
#include "hmac_espe.h"
#endif
#include "oam_event_wifi.h"
#include "host_hal_dma.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_DSCR_C

#ifdef _PRE_WLAN_FEATURE_VSP
static inline oal_netbuf_stru *hal_vsp_alloc_rx_netbuf(hal_host_device_stru *hal_device, uint32_t size)
{
    if (oal_netbuf_list_empty(&hal_device->vsp_netbuf_recycle_list) ||
        size != LARGE_NETBUF_SIZE + hal_host_get_rx_msdu_dscr_len()) {
        return NULL;
    }

    return oal_netbuf_delist(&hal_device->vsp_netbuf_recycle_list);
}
#endif

oal_netbuf_stru *hal_host_rx_alloc_netbuf(hal_host_device_stru *hal_device, uint32_t size)
{
#ifdef _PRE_WLAN_FEATURE_VSP
    oal_netbuf_stru *netbuf = hal_vsp_alloc_rx_netbuf(hal_device, size);
    if (netbuf) {
        return netbuf;
    }
#endif

#ifdef _PRE_WLAN_FEATURE_ESPE
    return hmac_espe_alloc_skb(size);
#else
    return oal_mem_netbuf_alloc(OAL_NORMAL_NETBUF, size, OAL_NETBUF_PRIORITY_HIGH);
#endif
}
uint32_t hal_dma_map_rx_buff(oal_netbuf_stru *netbuf, dma_addr_t *dma_addr)
{
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    if (pcie_dev == NULL) {
        oam_error_log0(0, 0, "hal_dma_map_rx_buff: pcie_linux_rsc null");
        return OAL_FAIL;
    }
    // rx receive
    *dma_addr = dma_map_single(&pcie_dev->dev, oal_netbuf_data(netbuf), oal_netbuf_len(netbuf), DMA_FROM_DEVICE);

    if (dma_mapping_error(&pcie_dev->dev, *dma_addr)) {
        oam_warning_log1(0, 0, "hal_dma_map_rx_buff: len[%d]", oal_netbuf_len(netbuf));
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

OAL_STATIC void hal_host_rx_add_buff_alloc_before_dma(hal_host_device_stru *hal_device,
    oal_netbuf_head_stru *before_dma, uint32_t buff_size, uint16_t need_add)
{
    uint16_t loop;
    oal_netbuf_stru *netbuf = NULL;
    size_t mpdu_desc_size = hal_host_get_rx_msdu_dscr_len();

    for (loop = 0; loop < need_add; loop++) {
        netbuf = hal_host_rx_alloc_netbuf(hal_device, buff_size);
        if (netbuf == NULL) {
            oam_warning_log0(0, OAM_SF_RX, "hal_host_rx_add_buff_alloc_before_dma::skb alloc fail");
            break;
        }
        oal_netbuf_set_len(netbuf, buff_size);
        if (g_wlan_spec_cfg->rx_ring_reduce_load_is_open != OAL_TRUE) {
            memset_s(oal_netbuf_data(netbuf), mpdu_desc_size, 0, mpdu_desc_size);
        }
        oal_netbuf_list_tail(before_dma, netbuf);
    }
}

OAL_STATIC void hal_host_rx_add_buff_alloc_after_dma(hal_host_device_stru *hal_device,
    oal_netbuf_head_stru *after_dma, hal_host_ring_ctl_stru *ring_ctl, hal_host_rx_alloc_list_stru *alloc_list)
{
    dma_addr_t pci_dma_addr;
    uint64_t  devva = 0;
    unsigned long lock_flag;
    uint16_t succ_add_num = 0;
    pcie_cb_dma_res *cb_res = NULL;
    oal_netbuf_stru *netbuf = NULL;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    if (pcie_dev == NULL) {
        return;
    }
    while ((netbuf = oal_netbuf_delist_nolock(after_dma)) != NULL) {
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
        pci_dma_addr = cb_res->paddr.addr;

        if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)pci_dma_addr, &devva) != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_RX, "hal_host_rx_add_buff_alloc_after_dma::hostca_to_devva fail");
            dma_unmap_single(&pcie_dev->dev, pci_dma_addr, cb_res->len, DMA_FROM_DEVICE);
            oal_netbuf_free(netbuf);
            continue;
        }
        oal_spin_lock_irq_save(&alloc_list->lock, &lock_flag);
        oal_netbuf_list_tail(&alloc_list->list, netbuf);
        oal_spin_unlock_irq_restore(&alloc_list->lock, &lock_flag);
        hal_ring_set_entries(ring_ctl, devva);
        succ_add_num++;
    }
    if (succ_add_num > 0) {
        hal_ring_set_sw2hw(ring_ctl);
    }
}

void hal_host_rx_add_buff_alloc_dma_proc(hal_host_device_stru *hal_device,
    hal_host_ring_ctl_stru *ring_ctl, uint32_t buff_size, hal_host_rx_alloc_list_stru *alloc_list, uint16_t need_add)
{
    oal_netbuf_head_stru before;
    oal_netbuf_head_stru after;

    /* step0：初始化before和after */
    oal_netbuf_list_head_init(&before);
    oal_netbuf_list_head_init(&after);

    /* step1：申请netbuf并串链到before */
    hal_host_rx_add_buff_alloc_before_dma(hal_device, &before, buff_size, need_add);

    /* step2: before进行dma地址转换，转换后返回after */
    hal_host_rx_dma_map_proc(&before, &after);

    /* step3: 遍历after,逐包写ring */
    hal_host_rx_add_buff_alloc_after_dma(hal_device, &after, ring_ctl, alloc_list);
}

OAL_STATIC void hal_host_rx_add_buff_proc(hal_host_device_stru *hal_device,
    hal_host_ring_ctl_stru *ring_ctl, uint32_t buff_size, hal_host_rx_alloc_list_stru *alloc_list)
{
    uint16_t need_add = 0;
    uint16_t once_proc_num;

    if (hal_ring_get_entry_count(hal_device, ring_ctl, &need_add) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_RX, "{host_rx_add_buff_alloc::return code.}");
        return;
    }
    if (need_add == 0) {
        return;
    }
    once_proc_num = hal_get_host_dma_once_proc_num();
    if (once_proc_num == 0) {
        return;
    }
    while (need_add > once_proc_num) {
        hal_host_rx_add_buff_alloc_dma_proc(hal_device, ring_ctl, buff_size,
            alloc_list, once_proc_num);
        need_add -= once_proc_num;
    }
    hal_host_rx_add_buff_alloc_dma_proc(hal_device, ring_ctl, buff_size,
        alloc_list, need_add);
}

/*
 * 功能描述   : 为防止描述符越来越少，至少要补齐spec
 * 1.日    期   : 2015年1月21日
 *   作    者   : wifi
 */
void hal_host_rx_add_buff(hal_host_device_stru *hal_device, uint8_t q_num)
{
    uint32_t                     buff_size;
    hal_host_ring_ctl_stru      *ring_ctl = NULL;
    hal_host_rx_alloc_list_stru *alloc_list = NULL;

    if (!oal_pcie_link_state_up(HCC_EP_WIFI_DEV)) {
        oam_warning_log0(0, OAM_SF_RX, "{host_rx_add_buff::pcie link down!}");
        return;
    }

    if (q_num == HAL_RX_DSCR_NORMAL_PRI_QUEUE) {
        ring_ctl = &hal_device->st_host_rx_normal_free_ring;
        buff_size = ((hal_device->st_alrx.en_al_rx_flag != 0) ? ALRX_NETBUF_SIZE : LARGE_NETBUF_SIZE) +
            hal_host_get_rx_msdu_dscr_len();
        alloc_list = &hal_device->host_rx_normal_alloc_list;
    } else if (q_num == HAL_RX_DSCR_SMALL_QUEUE) {
        ring_ctl = &hal_device->st_host_rx_small_free_ring;
        buff_size = SMALL_NETBUF_SIZE + hal_host_get_rx_msdu_dscr_len();
        alloc_list = &hal_device->host_rx_small_alloc_list;
    } else {
        oam_warning_log1(0, 0, "invalid queue num[%d] in rx irq", q_num);
        return;
    }
    hal_host_rx_add_buff_proc(hal_device, ring_ctl, buff_size, alloc_list);
}

/*
 * 功能描述   : 从alloc list中删除指定地址的netbuf,锁操作由外部保证
 * 1.日    期   : 2020.6.15
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
oal_netbuf_stru *hal_alloc_list_delete_netbuf(hal_host_device_stru *hal_dev,
    hal_host_rx_alloc_list_stru *alloc_list, dma_addr_t host_iova, uint32_t *pre_num)
{
    oal_netbuf_stru             *next_skb       = NULL;
    oal_netbuf_stru             *skb            = NULL;
    pcie_cb_dma_res             *cb_res         = NULL;
    oal_netbuf_head_stru        *list           = &alloc_list->list;

    oal_netbuf_search_for_each_safe(skb, next_skb, list) {
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(skb);
        if (cb_res == NULL) {
            return NULL;
        }
        /* 复用cb字段 */
        if (cb_res->paddr.addr == host_iova) {
            /* 此处仅从链表中取出，先不进行unmap操作 */
            netbuf_unlink(skb, list);
            return skb;
        }
        /* pre_num计数到达一定门限后，添加自愈机制，TO BE DONE */
        (*pre_num)++;
    }
    return NULL;
}
oal_netbuf_head_stru g_skb_list_temp = { 0 }; // 将局部变量转换全局变量，维测使用。
/*
 * 功能描述   : 释放alloc list中所有元素
 * 1.日    期   : 2020.6.15
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void hal_rx_alloc_list_free(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list)
{
    oal_netbuf_head_stru *list = &alloc_list->list;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    oal_netbuf_stru *skb = NULL;
    oal_netbuf_stru *next_skb = NULL;
    pcie_cb_dma_res *cb_res = NULL;
    unsigned long flags = 0;

    if (!hal_dev->rx_q_inited) {
        oam_warning_log0(hal_dev->device_id, 0, "hal_rx_alloc_list_free:rx res not inited");
        return;
    }
    if (pcie_dev == NULL) {
        oam_warning_log0(hal_dev->device_id, 0, "hal_rx_alloc_list_free:pcie dev is null");
        return;
    }

    oal_netbuf_head_init(&g_skb_list_temp);
    oal_spin_lock_irq_save(&alloc_list->lock, &flags);
    if (oal_netbuf_peek(list) == NULL) {
        oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);
        return;
    }
    oal_netbuf_queue_splice_tail_init(list, &g_skb_list_temp);
    oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);

    oal_netbuf_search_for_each_safe(skb, next_skb, &g_skb_list_temp) {
        if (skb == NULL) {
            oam_error_log0(hal_dev->device_id, 0, "hal_rx_alloc_list_free:skb list is broken");
            oal_netbuf_head_init(&g_skb_list_temp);
            return;
        }
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(skb);
#ifndef _PRE_LINUX_TEST
        if (cb_res != NULL && cb_res->paddr.addr <= DMA_RX_MAP_ERROR_ADDR) {
            oam_error_log2(0, 0, "{hal_rx_alloc_list_free::dma unmap addr error, skb[0x%llx] iova[0x%llx]}",
                (uintptr_t)skb, (uintptr_t)cb_res->paddr.addr);
            return;
        }
#endif
        dma_unmap_single(&pcie_dev->dev, cb_res->paddr.addr, skb->len, DMA_FROM_DEVICE); // rx receive
        netbuf_unlink(skb, &g_skb_list_temp);
        oal_netbuf_free(skb);
    }
}

/*
 * 功能描述   : 初始化alloc list
 * 1.日    期   : 2020.6.15
 *   作    者   : wifi
 *   修改内容   : 新生成函数
 */
void hal_init_alloc_list(hal_host_device_stru *hal_dev, hal_host_rx_alloc_list_stru *alloc_list)
{
    oal_netbuf_head_stru    *list = &alloc_list->list;
    uint32_t                 len;

    oal_spin_lock_init(&alloc_list->lock);

    len = oal_netbuf_list_len(list);
    if ((oal_netbuf_peek(list) != NULL) || (len != 0)) {
        oam_error_log1(hal_dev->device_id, OAM_SF_RX, "init_alloc_list:list len[%d] not empty", len);
        hal_rx_alloc_list_free(hal_dev, alloc_list);
    }
    oal_netbuf_head_init(list);
}
/*
 * 功能描述   : 固件侧reset_smac, host处理总入口
 * 1.日    期   : 2020.6.15
 *   作    者   : wifi
 */
uint32_t hal_host_rx_reset_smac_handler(hal_host_device_stru *hal_dev)
{
    hal_host_rx_alloc_list_stru *alloc_list = &hal_dev->host_rx_normal_alloc_list;

    hal_rx_alloc_list_free(hal_dev, alloc_list);
    hal_dev->rx_alloc_list_inited = OAL_FALSE;
    oam_warning_log1(0, OAM_SF_RX, "{host_rx_reset_smac_handler::succ device[%d]}", hal_dev->device_id);
    return OAL_SUCC;
}

uint32_t hal_rx_mpdu_que_len(hal_host_device_stru *hal_device)
{
    unsigned long lock_flag;
    uint32_t que_len;
    oal_netbuf_head_stru *netbuf_head = NULL;
    hal_rx_mpdu_que *rx_mpdu;

    rx_mpdu = &hal_device->st_rx_mpdu;
    oal_spin_lock_irq_save(&rx_mpdu->st_spin_lock, &lock_flag);

    /* 获取工作队列当前缓存的MPDU数目 */
    netbuf_head = &(rx_mpdu->ast_rx_mpdu_list[(rx_mpdu->cur_idx + 1) % RXQ_NUM]);
    que_len = oal_netbuf_list_len(netbuf_head);
    oal_spin_unlock_irq_restore(&rx_mpdu->st_spin_lock, &lock_flag);

    return que_len;
}

oal_netbuf_stru *hal_rx_get_sub_msdu(hal_host_device_stru *hal_device, uint64_t next_dscr_addr)
{
    dma_addr_t                   host_iova = 0;
    hal_host_rx_alloc_list_stru *alloc_list = &hal_device->host_rx_normal_alloc_list;
    unsigned long                flags = 0;
    uint32_t                     pre_num = 0;
    oal_netbuf_stru             *sub_msdu = NULL;

    if (pcie_if_devva_to_hostca(HCC_EP_WIFI_DEV, next_dscr_addr, (uint64_t *)&host_iova) != OAL_SUCC) {
        oam_warning_log0(0, OAM_SF_RX, "{hal_rx_get_sub_msdu:get next msdu fail.}");
        return NULL;
    }
    oal_spin_lock_irq_save(&alloc_list->lock, &flags);
    sub_msdu = hal_alloc_list_delete_netbuf(hal_device, alloc_list, host_iova, &pre_num);
    oal_spin_unlock_irq_restore(&alloc_list->lock, &flags);
    if (pre_num != 0 && g_wlan_spec_cfg->need_check_netbuf_pre_num == OAL_TRUE) {
        oam_warning_log1(hal_device->device_id, OAM_SF_RX, "hal_rx_get_sub_msdu:pre num[%d]", pre_num);
    }
    if (sub_msdu == NULL) {
        oam_error_log0(hal_device->device_id, OAM_SF_RX, "hal_rx_get_sub_msdu:sub msdu not found");
    }
    return sub_msdu;
}
/*
* 函 数 名   : mp16_host_rx_get_mac_header_len
* 功能描述   : 计算RX数据帧的帧头长度
* 1.日    期   : 2021年2月26日
*   作    者   : wifi
*   修改内容   : 计算mac header len
*/
uint8_t hal_host_rx_get_mac_header_len(mac_rx_ctl_stru *rx_ctl)
{
    uint8_t mac_hdr_len = MAC_80211_FRAME_LEN;
    mac_header_frame_control_stru *fc_ctl_hdr = NULL;

    fc_ctl_hdr = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;
    if (rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) {
        if ((fc_ctl_hdr->bit_type == WLAN_DATA_BASICTYPE) || (fc_ctl_hdr->bit_type == WLAN_MANAGEMENT)) {
            /* 4-Address Header */
            if ((fc_ctl_hdr->bit_to_ds == 1) && (fc_ctl_hdr->bit_from_ds == 1)) {
                mac_hdr_len += OAL_MAC_ADDR_LEN;
            }

            /* HTC+ Header */
            mac_hdr_len += (fc_ctl_hdr->bit_order == 1) ? sizeof(uint32_t) : 0;
            /* QoS Header */
            if ((fc_ctl_hdr->bit_type) == WLAN_DATA_BASICTYPE &&
                ((fc_ctl_hdr->bit_sub_type & WLAN_QOS_DATA) == WLAN_QOS_DATA)) {
                mac_hdr_len += sizeof(uint16_t);
            }
        } else {
            /* ctrl frame */
            mac_hdr_len = MAC_80211_CTL_HEADER_LEN;
            /* 适配monitor ack rts cts等 */
            if (rx_ctl->us_frame_len < mac_hdr_len) {
                mac_hdr_len = (uint8_t)(rx_ctl->us_frame_len);
            }
        }
    } else {
        /* 以太网帧头长度 */
        mac_hdr_len = ETHER_HDR_LEN;
    }

    return mac_hdr_len;
}

void hal_rx_set_ring_regs(hal_host_ring_ctl_stru *rx_ring_ctl)
{
    if (rx_ring_ctl->devva == 0) {
        oam_error_log2(0, 0, "rx_set_ring_regs::ring_type %d, subtype %d!",
            rx_ring_ctl->ring_type, (uintptr_t)rx_ring_ctl->ring_subtype);
        return;
    }

    hal_host_writel(get_low_32bits(rx_ring_ctl->devva), rx_ring_ctl->base_lsb);
    hal_host_writel(get_high_32bits(rx_ring_ctl->devva), rx_ring_ctl->base_msb);
    hal_host_writel(rx_ring_ctl->entries, rx_ring_ctl->ring_size_reg);

    /* hw与sw保持一致 */
    rx_ring_ctl->un_read_ptr.read_ptr = 0;
    rx_ring_ctl->un_write_ptr.write_ptr = 0;

    if (rx_ring_ctl->write_ptr_reg != 0) {
        hal_host_writel(0, rx_ring_ctl->write_ptr_reg);
    }
    if (rx_ring_ctl->cfg_read_ptr_reg != 0) {
        hal_host_writel(0, rx_ring_ctl->cfg_read_ptr_reg);
    }
    if (rx_ring_ctl->read_ptr_reg != 0) {
        hal_host_writel(0, rx_ring_ctl->read_ptr_reg);
    }
    if (rx_ring_ctl->cfg_write_ptr_reg != 0) {
        hal_host_writel(0, rx_ring_ctl->cfg_write_ptr_reg);
    }
}

static uintptr_t hal_rx_ring_regs_addr_transfer(uint8_t device_id, uint32_t reg_addr)
{
    uint64_t host_va = 0;
    uint32_t mac_reg_offset = hal_get_mac_reg_offset(device_id, reg_addr);
    uint8_t dev_num = mac_chip_get_hal_dev_num();
    if (device_id >= dev_num) {
        oam_error_log2(0, OAM_SF_CFG, "{ring_ctl_regs_addr_transfer::device_id[%d],addr[%x], check device_id fail}",
            device_id, reg_addr);
        return 0;
    }
    if (oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, reg_addr + mac_reg_offset, &host_va) == OAL_SUCC) {
        return (uintptr_t)host_va;
    } else {
        oam_error_log2(0, OAM_SF_CFG, "{ring_ctl_regs_addr_transfer::device_id[%d],addr[%x] transfer fail}",
            device_id, reg_addr);
        return 0;
    }
}

uint8_t hal_rx_ring_reg_init(hal_host_device_stru *hal_dev, hal_host_ring_ctl_stru *rctl, uint8_t ring_id)
{
    hal_ring_mac_regs_info* ring_reg_tbl = hal_get_ring_mac_regs_tbl();
    if ((hal_dev == NULL) || (rctl == NULL) || (ring_id >= HAL_RING_ID_BUTT) || (ring_reg_tbl == NULL)) {
        oam_error_log0(0, OAM_SF_CFG, "{hal_rx_ring_reg_init::null input}");
        return OAL_FALSE;
    }

    rctl->base_lsb      = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].base_lsb);
    rctl->base_msb      = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].base_msb);
    rctl->ring_size_reg = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].size);
    rctl->write_ptr_reg = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].wptr_reg);
    rctl->read_ptr_reg  = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].rptr_reg);
    rctl->rx_ring_reset = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].rx_ring_reset);
    if (!((rctl->base_lsb) && (rctl->base_msb) && (rctl->ring_size_reg) &&
        (rctl->write_ptr_reg) && (rctl->read_ptr_reg))) {
        oam_error_log2(0, OAM_SF_CFG, "{rx_ring_reg_init::cfg NULL! base_lsb[0x%x] base_msb[0x%x]}",
            rctl->base_lsb, rctl->base_msb);
        oam_error_log3(0, OAM_SF_CFG, "{rx_ring_reg_init::ring_size_reg[0x%x] wptr_reg[0x%x] rptr_reg[0x%x]}",
            rctl->ring_size_reg, rctl->write_ptr_reg, rctl->read_ptr_reg);
        return OAL_FALSE;
    }
    if (ring_id < HAL_RING_TYPE_COMP) {
        rctl->cfg_read_ptr_reg =
            hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].cfg_rptr_reg);
        return !!(rctl->cfg_read_ptr_reg);
    } else {
        if (ring_reg_tbl[ring_id].len) {
            rctl->len_reg  = hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].len);
            rctl->cfg_write_ptr_reg =
                hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].cfg_wptr_reg);
            return ((rctl->len_reg) && (rctl->cfg_write_ptr_reg));
        } else {
            rctl->cfg_write_ptr_reg =
                hal_rx_ring_regs_addr_transfer(hal_dev->device_id, ring_reg_tbl[ring_id].cfg_wptr_reg);
            return OAL_TRUE;
        }
    }
}

void hal_rx_init_ring_ptr_host_addr(hal_host_device_stru *hal_dev)
{
    void *rbase_vaddr = NULL;
    dma_addr_t rbase_dma_addr = 0;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    hal_host_chip_stru *hal_chip = hal_get_host_chip();

    if (pcie_dev == NULL) {
        oam_error_log0(0, OAM_SF_RX, "hal_rx_init_ring_ptr_host_addr: pcie_linux_rsc null");
        return;
    }

    // 申请MAC上报Host侧指针的内存
    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, sizeof(hal_rx_ring_mac_ptr_info) * HAL_MAC_PTR_RPT_HOST_BUTT,
        &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, OAM_SF_RX, "hal_rx_init_ring_ptr_host_addr::alloc ptr rpt host mem fail.");
        return;
    }
    memset_s(rbase_vaddr, sizeof(hal_rx_ring_mac_ptr_info) * HAL_MAC_PTR_RPT_HOST_BUTT, 0,
        sizeof(hal_rx_ring_mac_ptr_info) * HAL_MAC_PTR_RPT_HOST_BUTT);
    hal_chip->rx_ring_mac_ptr_info = (hal_rx_ring_mac_ptr_info *)rbase_vaddr;
    hal_chip->rx_ring_mac_ptr_addr.rbase_dma_addr = rbase_dma_addr;

    oam_warning_log0(0, OAM_SF_RX, "hal_rx_init_ring_ptr_host_addr::init rpt host addr succ!");
}

void hal_rx_set_ring_ptr_host_addr(hal_host_device_stru *hal_dev)
{
    uint64_t devva = 0;
    uintptr_t host_va_lsb;
    uintptr_t host_va_msb;
    uintptr_t ring_ptr_rpt_mode;
    hal_host_chip_stru *hal_chip = hal_get_host_chip();

    if (hal_chip->rx_ring_mac_ptr_info == NULL) {
        oam_error_log0(0, OAM_SF_RX, "{hal_rx_set_ring_ptr_host_addr::get rx ring mac ptr fail.}");
        return;
    }

    memset_s(hal_chip->rx_ring_mac_ptr_info, sizeof(hal_rx_ring_mac_ptr_info) * HAL_MAC_PTR_RPT_HOST_BUTT, 0,
        sizeof(hal_rx_ring_mac_ptr_info) * HAL_MAC_PTR_RPT_HOST_BUTT);

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)hal_chip->rx_ring_mac_ptr_addr.rbase_dma_addr,
        &devva) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_set_ring_ptr_host_addr::set ring ptr host addr fail.");
        return;
    }

    host_va_lsb = hal_rx_ring_regs_addr_transfer(hal_dev->device_id,
        hal_chip->rx_ring_mac_ptr_addr.mac_ptr_rpt_host_addr_lsb);
    host_va_msb = hal_rx_ring_regs_addr_transfer(hal_dev->device_id,
        hal_chip->rx_ring_mac_ptr_addr.mac_ptr_rpt_host_addr_msb);
    hal_host_writel(get_low_32bits((uintptr_t)devva), host_va_lsb);
    hal_host_writel(get_high_32bits((uintptr_t)devva), host_va_msb);

    /* 配置上报模式寄存器 */
    ring_ptr_rpt_mode = hal_rx_ring_regs_addr_transfer(hal_dev->device_id,
        hal_chip->rx_ring_mac_ptr_addr.ring_ptr_rpt_mode);
    hal_host_set_mac_ptr_rpt_regs(ring_ptr_rpt_mode);

    oam_warning_log0(0, OAM_SF_RX, "hal_rx_set_ring_ptr_host_addr::set rpt host addr succ!");
}

int32_t hal_rx_host_init_small_free_ring(hal_host_device_stru *pst_device)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    void *rbase_vaddr = NULL;
    uint64_t devva = 0;
    int32_t ret;
    oal_pci_dev_stru  *pcie_dev = oal_get_wifi_pcie_dev();

    if (pcie_dev == NULL) {
        return OAL_FAIL;
    }

    rx_ring_ctl = &(pst_device->st_host_rx_small_free_ring);
    memset_s(rx_ring_ctl, sizeof(hal_host_ring_ctl_stru), 0, sizeof(hal_host_ring_ctl_stru));

    entries = (pst_device->device_id == 0) ? HAL_RX_MSDU_SMALL_RING_NUM0 : HAL_RX_MSDU_SMALL_RING_NUM0;
    alloc_size = (entries * HAL_RX_ENTRY_SIZE);
    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log1(0, 0, "normal_free:dma_alloc fail, size[%d]", alloc_size);
        return OAL_FAIL;
    }

    ret = pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)rbase_dma_addr, &devva);
    if (ret != OAL_SUCC) {
        oam_error_log0(0, 0, "rx_host_init_small_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        return OAL_FAIL;
    }

    rx_ring_ctl->entries = entries;
    rx_ring_ctl->p_entries = (uintptr_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_FREE_RING;
    rx_ring_ctl->ring_subtype = HAL_RING_TYPE_S_F;
    rx_ring_ctl->devva = (uintptr_t)devva;

    if (hal_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_S_F) == OAL_FALSE) {
        oam_error_log0(0, 0, "rx_host_init_small_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        rx_ring_ctl->p_entries = NULL;
        return OAL_FAIL;
    }
    hal_init_alloc_list(pst_device, &pst_device->host_rx_small_alloc_list);
    return OAL_SUCC;
}

int32_t hal_rx_host_init_normal_free_ring(hal_host_device_stru *pst_device)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    void *rbase_vaddr = NULL;
    uint64_t devva = 0;
    dma_addr_t rbase_dma_addr = 0;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    if (pcie_dev == NULL) {
        return OAL_FAIL;
    }

    rx_ring_ctl = &(pst_device->st_host_rx_normal_free_ring);
    memset_s(rx_ring_ctl, sizeof(hal_host_ring_ctl_stru), 0, sizeof(hal_host_ring_ctl_stru));

    entries = (pst_device->device_id == 0) ?
        HAL_RX_MSDU_NORMAL_RING_NUM1 : HAL_RX_MSDU_NORMAL_RING_NUM1;
    alloc_size = (entries * HAL_RX_ENTRY_SIZE);

    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_dev  null.");
        return OAL_FAIL;
    }

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)rbase_dma_addr, &devva) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        return OAL_FAIL;
    }

    rx_ring_ctl->entries    = entries;
    rx_ring_ctl->p_entries  = (uintptr_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type  = HAL_RING_TYPE_FREE_RING;
    rx_ring_ctl->ring_subtype = HAL_RING_TYPE_L_F;
    rx_ring_ctl->devva = (uintptr_t)devva;

    if (hal_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_L_F) == OAL_FALSE) {
        oam_error_log0(0, 0, "hal_rx_host_init_normal_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        rx_ring_ctl->p_entries = NULL;
        return OAL_FAIL;
    }
    hal_init_alloc_list(pst_device, &pst_device->host_rx_normal_alloc_list);
    oam_warning_log2(0, 0, "hal_rx_host_init_normal_free_ring :base[%x].size[%d]", devva, entries);
    return OAL_SUCC;
}

int32_t hal_rx_host_init_complete_ring(hal_host_device_stru *pst_device)
{
    uint16_t entries;
    hal_host_ring_ctl_stru *rx_ring_ctl = NULL;
    uint32_t alloc_size;
    dma_addr_t rbase_dma_addr = 0;
    void *rbase_vaddr = NULL;
    uint64_t devva = 0;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    if (pcie_dev == NULL) {
        return OAL_FAIL;
    }

    oal_spin_lock_init(&pst_device->st_free_ring_lock);
    rx_ring_ctl = &(pst_device->st_host_rx_complete_ring);
    memset_s(rx_ring_ctl, sizeof(hal_host_ring_ctl_stru), 0, sizeof(hal_host_ring_ctl_stru));

    entries = HAL_RX_COMPLETE_RING_MAX;
    alloc_size = (entries * HAL_RX_ENTRY_SIZE);
    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        return OAL_FAIL;
    }

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)rbase_dma_addr, &devva) != OAL_SUCC) {
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        return OAL_FAIL;
    }

    rx_ring_ctl->entries = entries;
    rx_ring_ctl->p_entries = (uintptr_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type = HAL_RING_TYPE_COMPLETE_RING;
    rx_ring_ctl->ring_subtype = HAL_RING_TYPE_COMP;
    rx_ring_ctl->devva = (uintptr_t)devva;

    if (hal_rx_ring_reg_init(pst_device, rx_ring_ctl, HAL_RING_TYPE_COMP) == OAL_FALSE) {
        oam_error_log0(0, 0, "hal_rx_host_init_complete_ring alloc pcie_if_hostca_to_devva  fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        rx_ring_ctl->p_entries = NULL;
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

int32_t hal_rx_host_init_dscr_queue(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);

    if ((hal_device == NULL) || (hal_dev_id >= mac_chip_get_hal_dev_num())) {
        oam_error_log1(0, 0, "hal_rx_host_init_dscr_queue::hal dev null. id[%d].", hal_dev_id);
        return OAL_FAIL;
    }

    if (hal_device->rx_q_inited) {
        oam_warning_log0(0, 0, "hal_rx_host_init_dscr_queue::already inited.");
        return OAL_SUCC;
    }

    /* host访问device寄存器的地址映射操作 */
    hal_host_regs_addr_init(hal_device);

    /* 初始化接收硬件队列 */
    if (hal_rx_host_init_normal_free_ring(hal_device) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_normal_free_ring fail.");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DEV,
                             CHR_WIFI_DEV_EVENT_RING_RX, CHR_WIFI_DEV_ERROR_RX_RING_INIT);
        return OAL_FAIL;
    }

    if (hal_rx_host_init_small_free_ring(hal_device) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_small_free_ring fail.");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DEV,
                             CHR_WIFI_DEV_EVENT_RING_RX, CHR_WIFI_DEV_ERROR_RX_RING_INIT);
        return OAL_FAIL;
    }

    if (hal_rx_host_init_complete_ring(hal_device) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_host_init_dscr_queue::init_complete_ring fail.");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_WIFI, CHR_LAYER_DEV,
                             CHR_WIFI_DEV_EVENT_RING_RX, CHR_WIFI_DEV_ERROR_RX_RING_INIT);
        return OAL_FAIL;
    }

    /* rx方向负载优化，初始化ring指针上报host侧地址 */
    if (g_wlan_spec_cfg->rx_ring_reduce_load_is_open == OAL_TRUE) {
        hal_rx_init_ring_ptr_host_addr(hal_device);
    }

    hal_device->rx_q_inited = OAL_TRUE;
    oam_warning_log0(0, 0, "hal_rx_host_init_dscr_queue::inited succ.");

    return OAL_SUCC;
}
uint32_t hal_rx_host_start_dscr_queue(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device  = hal_get_host_device(hal_dev_id);
    uint32_t mpdu_desc_size = hal_host_get_rx_msdu_dscr_len();
    if (hal_device == NULL) {
        oam_error_log0(0, 0, "{rx_host_start_dscr_queue::hal_device null.}");
        return OAL_ERR_CODE_PTR_NULL;
    }
    if (hal_device->rx_alloc_list_inited) {
        return OAL_SUCC;
    }

    if (hal_pm_try_wakeup_dev_lock() != OAL_SUCC) {
        return OAL_FAIL;
    }

    /* 队列的初始化可以放到hal device init函数内做一次 */
    if (hal_rx_host_init_dscr_queue(hal_dev_id) != OAL_SUCC) {
        return OAL_FAIL;
    }
    /* host data ring复位 */
    if (hal_device->st_host_rx_normal_free_ring.rx_ring_reset != 0) {
        hal_host_h2d_rx_ring_reset(hal_device);
    }
    /* LEN REG keep default */
    hal_rx_set_ring_regs(&(hal_device->st_host_rx_normal_free_ring));
    hal_rx_set_ring_regs(&(hal_device->st_host_rx_small_free_ring));
    hal_rx_set_ring_regs(&(hal_device->st_host_rx_complete_ring));

    /* rx方向负载优化，设置ring指针上报host侧地址及上报模式寄存器 */
    if (g_wlan_spec_cfg->rx_ring_reduce_load_is_open == OAL_TRUE) {
        hal_rx_set_ring_ptr_host_addr(hal_device);
    }

    if (hal_device->st_alrx.en_al_rx_flag != 0) {
        if (!hal_device->st_host_rx_complete_ring.len_reg) {
            oam_error_log0(0, 0, "rx_host_start_dscr_queue::len_reg not inited!");
            return OAL_FAIL;
        }
        hal_host_writel(((ALRX_NETBUF_SIZE + mpdu_desc_size) | (mpdu_desc_size << NUM_16_BITS)),
            hal_device->st_host_rx_complete_ring.len_reg);
    }
    /* host data ring解复位 */
    if (hal_device->st_host_rx_normal_free_ring.rx_ring_reset != 0) {
        hal_host_writel(0x0, hal_device->st_host_rx_normal_free_ring.rx_ring_reset);
    }
    /* 向数据帧normal free ring添加内存 */
    hal_host_rx_add_buff(hal_device, HAL_RX_DSCR_NORMAL_PRI_QUEUE);
    hal_device->rx_alloc_list_inited = OAL_TRUE;
    hal_device->dscr_prev = 0;
    oam_warning_log1(0, OAM_SF_RX, "rx_host_start_dscr_queue::dev[%d]succ!", hal_dev_id);
    return OAL_SUCC;
}

OAL_STATIC void hal_rx_host_destory_free_ring(hal_host_ring_ctl_stru *rx_ring_ctl)
{
}

OAL_STATIC void hal_rx_host_destory_comp_ring(hal_host_ring_ctl_stru *rx_ring_ctl)
{
}

int32_t hal_rx_host_stop_dscr_queue(uint8_t hal_dev_id)
{
    hal_host_device_stru   *hal_device  = hal_get_host_device(hal_dev_id);

    if (hal_device == NULL) {
        return OAL_ERR_CODE_PTR_NULL;
    }

    /* 不用host去reset ring cfg regs，避免处理不必要的同步问题 */
    hal_rx_host_destory_free_ring(&hal_device->st_host_rx_small_free_ring);
    hal_rx_host_destory_free_ring(&hal_device->st_host_rx_normal_free_ring);
    hal_rx_host_destory_comp_ring(&hal_device->st_host_rx_complete_ring);

    return OAL_SUCC;
}

void hal_host_intr_regs_init(uint8_t hal_dev_id)
{
    hal_host_device_stru *hal_device = hal_get_host_device(hal_dev_id);

    if (hal_device == NULL) {
        oam_error_log0(0, 0, "{host_ring_tx_init::haldev null!}");
        return;
    }

    oam_warning_log2(0, 0, "host_intr_regs_init::dev_id[%d], initr_inited state[%d]!",
                     hal_device->device_id, hal_device->intr_inited);
    if (hal_device->intr_inited) {
        return;
    }

    if (hal_host_regs_addr_init(hal_device) != OAL_SUCC) {
        return;
    }
    hal_device->intr_inited = OAL_TRUE;
}

void hal_host_rx_proc_msdu_dscr(oal_netbuf_stru *netbuf)
{
    if (oam_ota_get_switch(OAM_OTA_SWITCH_RX_MSDU_DSCR) == OAL_SWITCH_OFF) {
        return;
    }
    while (netbuf != NULL) {
        oam_report_dscr(BROADCAST_MACADDR, (uint8_t *)oal_netbuf_data(netbuf),
            hal_host_get_rx_msdu_dscr_len(), OAM_OTA_TYPE_RX_MSDU_DSCR);
        netbuf = oal_netbuf_next(netbuf);
    }
}

hal_host_device_stru *hal_rx_get_mpdu_link(oal_netbuf_stru *mpdu)
{
    mac_rx_ctl_stru *rx_cb = (mac_rx_ctl_stru *)oal_netbuf_cb(mpdu);
    uint8_t link_id;
    uint8_t dev_num = mac_chip_get_hal_dev_num();

    if (rx_cb == NULL) {
        return NULL;
    }
    link_id = rx_cb->link_id;

    return link_id < dev_num ? hal_get_host_device(link_id) : NULL;
}

uint8_t hal_rx_is_frag_frm(mac_rx_ctl_stru *rx_ctl)
{
    mac_header_frame_control_stru *fc_ctl_hdr = (mac_header_frame_control_stru *)&rx_ctl->us_frame_control;

    return (uint8_t)((rx_ctl->bit_frame_format == MAC_FRAME_TYPE_80211) &&
        (fc_ctl_hdr->bit_more_frag || rx_ctl->bit_fragment_num));
}

/*
 * 功能描述  : cb中长度计算，并调整netbuf数据部分头尾指针
 * 1.日    期  : 2020.7.20
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t hal_rx_adjust_netbuf_len(oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_ctl)
{
    uint32_t rx_msdu_len;
    pcie_cb_dma_res *cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
    if (cb_res == NULL) {
        return OAL_FAIL;
    }

    rx_msdu_len = hal_host_get_rx_msdu_dscr_len();
    /* 1.fcs错误，后续字段信息不需要查看，本身不可信  2.fcs错误的amsdu非首帧信息本身不回填，字段不可信 */
    if ((rx_ctl->rx_status == HAL_RX_SUCCESS) &&
        (rx_ctl->us_frame_len == 0 || (rx_ctl->us_frame_len > (cb_res->len - rx_msdu_len)))) {
        oam_error_log2(0, OAM_SF_RX, "host_rx_adjust_netbuf_len:pkt len:%d, pcie cb len:%d",
            rx_ctl->us_frame_len, cb_res->len);
        return OAL_FAIL;
    }

    oal_netbuf_pull(netbuf, rx_msdu_len);
    skb_trim(netbuf, rx_ctl->us_frame_len);
    return OAL_SUCC;
}

oal_bool_enum_uint8 hal_rx_ip_checksum_is_pass(oal_netbuf_stru *netbuf)
{
    mac_rx_ctl_stru *rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (rx_ctrl == NULL) {
        return OAL_FALSE;
    }
    if  (rx_ctrl->bit_ipv4cs_valid && (rx_ctrl->bit_ipv4cs_pass == 0)) {
        return OAL_FALSE;
    } else {
        return OAL_TRUE;
    }
}

void hal_rx_tcp_udp_checksum(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *cur_netbuf = netbuf;
    oal_netbuf_stru *next_netbuf = NULL;
    mac_rx_ctl_stru *rx_ctrl = NULL;
    while (cur_netbuf != NULL) {
        next_netbuf = cur_netbuf->next;
        rx_ctrl = (mac_rx_ctl_stru *)oal_netbuf_cb(cur_netbuf);
        if (rx_ctrl->bit_ptlcs_valid && rx_ctrl->bit_ptlcs_pass) {
            cur_netbuf->ip_summed = CHECKSUM_UNNECESSARY;
        }
        dpe_device_rx_checksum_stat(rx_ctrl);
        cur_netbuf = next_netbuf;
    }
}

#ifdef _PRE_WLAN_FEATURE_VSP
#define MAX_BUFFER_IN_RECYCLE_LIST 20
uint32_t hal_vsp_rx_buff_recycle(hal_host_device_stru *hal_device, oal_netbuf_head_stru *netbuf_head)
{
    oal_netbuf_stru *netbuf = NULL;
    uint32_t netbuf_total_len, headroom, rx_msdu_len;
    uint32_t recycle_cnt = 0;

    if (hal_device == NULL) {
        while ((netbuf = oal_netbuf_delist_nolock(netbuf_head)) != NULL) {
            oal_netbuf_free(netbuf);
        }
        return 0; // 若vsp已经stop(hal_device=null),直接释放netbuf
    }

    if (oal_netbuf_list_len(&hal_device->vsp_netbuf_recycle_list) > MAX_BUFFER_IN_RECYCLE_LIST) {
        oal_netbuf_list_purge(netbuf_head);
        return 0;
    }
    rx_msdu_len = hal_host_get_rx_msdu_dscr_len();
    while ((netbuf = oal_netbuf_delist_nolock(netbuf_head)) != NULL) {
        headroom = oal_netbuf_headroom(netbuf);
        netbuf_total_len = headroom + oal_netbuf_get_len(netbuf) + oal_netbuf_tailroom(netbuf);
        if (netbuf_total_len < LARGE_NETBUF_SIZE + rx_msdu_len) {
            oam_warning_log1(0, 0, "{host_rx_buff_recycle::buffer room[%d] not sufficent}", netbuf_total_len);
            oal_netbuf_free(netbuf);
            continue;
        }
        oal_netbuf_push(netbuf, headroom);
        oal_netbuf_set_len(netbuf, LARGE_NETBUF_SIZE + rx_msdu_len);
        oal_netbuf_add_to_list_tail(netbuf, &hal_device->vsp_netbuf_recycle_list);
        recycle_cnt++;
    }

    return recycle_cnt;
}
#endif

void hal_host_rx_amsdu_list_build(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next_netbuf = NULL;
    pcie_cb_dma_res *cb_res = NULL;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    while (netbuf != NULL) {
        next_netbuf = hal_rx_get_next_sub_msdu(hal_device, netbuf);
        oal_netbuf_next(netbuf) = next_netbuf;
        netbuf = next_netbuf;
        if (netbuf != NULL) {
            cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
#ifndef _PRE_LINUX_TEST
            if (cb_res != NULL && cb_res->paddr.addr <= DMA_RX_MAP_ERROR_ADDR) {
                oam_error_log2(0, 0, "{hal_host_rx_amsdu_list_build::dma unmap addr error, skb[0x%llx] iova[0x%llx]}",
                    (uintptr_t)netbuf, (uintptr_t)cb_res->paddr.addr);
                return;
            }
#endif
            if (oal_likely((cb_res != NULL && pcie_dev != NULL))) {
                dma_unmap_single(&pcie_dev->dev, cb_res->paddr.addr, netbuf->len, DMA_FROM_DEVICE);
            }
        }
    }
}