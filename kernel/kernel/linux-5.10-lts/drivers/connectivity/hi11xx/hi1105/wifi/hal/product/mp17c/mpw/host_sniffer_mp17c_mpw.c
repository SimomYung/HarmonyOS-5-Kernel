/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : HOST SNIFFER FUNCTION
 * 作    者 : wifi3
 * 创建日期 : 2021年3月29日
 */

#ifdef _PRE_WLAN_FEATURE_SNIFFER

#include "oal_util.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"

#include "host_hal_dscr.h"
#include "host_hal_ring.h"
#include "host_hal_device.h"
#include "hal_common.h"

#include "host_dscr_mp17c_mpw.h"
#include "host_mac_mp17c_mpw.h"
#include "pcie_host.h"
#include "host_sniffer_mp17c_mpw.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_SNIFFER_MP17C_MPW_C

/*
 * 函数描述：rx ppdu描述符 free ring 初始化
 */
uint32_t mp17c_mpw_rx_ppdu_dscr_free_ring_init(hal_host_device_stru *hal_dev)
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

    rx_ring_ctl = &(hal_dev->host_rx_ppdu_free_ring);
    memset_s(rx_ring_ctl, sizeof(hal_host_ring_ctl_stru), 0, sizeof(hal_host_ring_ctl_stru));

    entries = HAL_RX_PPDU_FREE_RING_COUNT;
    alloc_size = (entries * HAL_RX_ENTRY_SIZE);
    /* 申请ring的内存，非netbuf的内存 */
    rbase_vaddr = dma_alloc_coherent(&pcie_dev->dev, alloc_size, &rbase_dma_addr, GFP_KERNEL);
    if (rbase_vaddr == NULL) {
        oam_error_log0(0, 0, "hal_rx_host_init_ppdu_free_ring alloc pcie_linux_res  null.");
        return OAL_FAIL;
    }

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)rbase_dma_addr, &devva) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_rx_host_init_ppdu_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        return OAL_FAIL;
    }

    rx_ring_ctl->entries    = entries;
    rx_ring_ctl->p_entries  = (uintptr_t *)rbase_vaddr;
    rx_ring_ctl->entry_size = HAL_RX_ENTRY_SIZE;
    rx_ring_ctl->ring_type  = HAL_RING_TYPE_FREE_RING;
    rx_ring_ctl->devva = (uintptr_t)devva;

    if (hal_rx_ring_reg_init(hal_dev, rx_ring_ctl, HAL_RING_TYPE_P_F) == OAL_FALSE) {
        oam_error_log0(0, 0, "hal_rx_host_init_ppdu_free_ring alloc pcie_if_hostca_to_devva fail.");
        dma_free_coherent(&pcie_dev->dev, alloc_size, rbase_vaddr, rbase_dma_addr);
        rx_ring_ctl->p_entries = NULL;
        return OAL_FAIL;
    }

    oal_dlist_init_head(&hal_dev->host_rx_ppdu_alloc_list.list_head);
    oam_warning_log2(0, 0, "hal_rx_host_init_normal_free_ring :base[%x].size[%d]", devva, entries);
    return OAL_SUCC;
}

static uint32_t mp17c_mpw_dma_map_rx_ppdu_dscr(
    uint8_t dma_dir, hal_host_rx_ppdu_dscr_stru *ppdu_dscr, dma_addr_t *dma_addr)
{
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    if (pcie_dev == NULL) {
        oam_error_log0(0, 0, "dma_map_rx_buff: pcie_linux_rsc null");
        return OAL_FAIL;
    }

    *dma_addr = dma_map_single(&pcie_dev->dev, (uint8_t *)&ppdu_dscr->hw_ppdu_data,
        sizeof(mp17c_mpw_rx_ppdu_dscr_stru), dma_dir);

    if (dma_mapping_error(&pcie_dev->dev, *dma_addr)) {
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述：向 free ring添加内存
 */
uint32_t mp17c_mpw_add_rx_ppdu_dscr(hal_host_device_stru *hal_device)
{
    uint64_t devva = 0;
    dma_addr_t pci_dma_addr;
    uint16_t need_add = 0;
    uint16_t succ_add_num = 0;
    uint16_t loop;
    hal_host_ring_ctl_stru *ring_ctl = &hal_device->host_rx_ppdu_free_ring;
    hal_host_rx_ppdu_dscr_list_head_stru *alloc_list = &hal_device->host_rx_ppdu_alloc_list;
    hal_host_rx_ppdu_dscr_stru *ppdu_dscr = NULL;
    uint32_t ret;

    ret = hal_ring_get_entry_count(hal_device, ring_ctl, &need_add);
    if (ret != OAL_SUCC) {
        oam_warning_log1(0, OAM_SF_RX, "{add_rx_ppdu_dscr, return code %d.}", ret);
        return ret;
    }
    for (loop = 0; loop < need_add; loop++) {
        ppdu_dscr = oal_memalloc(MEM_RX_PPDU_DSCR_MPW_SIZE);
        if (ppdu_dscr == NULL) {
            oam_error_log0(0, OAM_SF_RX, "host_rx_add_buff_alloc alloc fail");
            break;
        }
        memset_s(ppdu_dscr, MEM_RX_PPDU_DSCR_MPW_SIZE, 0, MEM_RX_PPDU_DSCR_MPW_SIZE);

        if (mp17c_mpw_dma_map_rx_ppdu_dscr(DMA_FROM_DEVICE, ppdu_dscr, &pci_dma_addr) != OAL_SUCC) {
            oal_free(ppdu_dscr);
            oam_warning_log0(0, OAM_SF_RX, "host_rx_add_buff_alloc::alloc node fail");
            break;
        }

        if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)pci_dma_addr, &devva) != OAL_SUCC) {
            oal_free(ppdu_dscr);
            break;
        }

        ppdu_dscr->paddr.addr = pci_dma_addr;
        oal_dlist_add_tail(&ppdu_dscr->entry, &alloc_list->list_head);
        hal_ring_set_entries(ring_ctl, devva);
        succ_add_num++;
    }
    if (succ_add_num > 0) {
        hal_ring_set_sw2hw(ring_ctl);
    }
    return OAL_SUCC;
}

/*
 * 功能描述：sniffer功能 rx ppdu描述符 free ring 初始化
 */
uint32_t mp17c_mpw_sniffer_rx_ppdu_free_ring_init(hal_host_device_stru *hal_device)
{
    /* ring 初始化 */
    if (mp17c_mpw_rx_ppdu_dscr_free_ring_init(hal_device) != OAL_SUCC) {
        oam_error_log0(0, 0, "sniffer_rx_ppdu_free_ring_init::init_rx_ppdu_free_ring fail.");
        return OAL_FAIL;
    }
    hal_rx_set_ring_regs(&(hal_device->host_rx_ppdu_free_ring));
    /* 向 free ring添加内存 */
    if (mp17c_mpw_add_rx_ppdu_dscr(hal_device) != OAL_SUCC) {
        oam_error_log0(0, 0, "sniffer_rx_ppdu_free_ring_init::add buff fail.");
        return OAL_FAIL;
    }
    return OAL_SUCC;
}

/*
 * 功能描述: 释放rx ppdu ring
 */
void mp17c_mpw_rx_ppdu_from_list_free(hal_host_rx_ppdu_dscr_list_head_stru *ppdu_list)
{
    hal_host_rx_ppdu_dscr_stru *ppdu_dscr = NULL;

    if (ppdu_list == NULL) {
        oam_error_log0(0, OAM_SF_RX, "rx_ppdu_from_list_free::ppdu_list null");
        return;
    }

    /* 循环查找保存的链表，找到该ppdu_dscr, 并从链表中释放 */
    while (!oal_dlist_is_empty(&(ppdu_list->list_head))) {
        ppdu_dscr = (hal_host_rx_ppdu_dscr_stru *)oal_dlist_delete_head(&ppdu_list->list_head);
        oal_mem_free_m(ppdu_dscr, OAL_TRUE);
    }

    ppdu_list->list_num = 0;
}

/*
 * 功能描述：sniffer功能 rx ppdu描述符 free ring 去初始化
 */
void mp17c_mpw_sniffer_rx_ppdu_free_ring_deinit(hal_host_device_stru *hal_device)
{
    hal_host_ring_ctl_stru *free_rctl = &hal_device->host_rx_ppdu_free_ring;
    hal_host_rx_ppdu_dscr_list_head_stru *alloc_list = &hal_device->host_rx_ppdu_alloc_list;

    if (alloc_list == NULL) {
        oam_warning_log0(0, OAM_SF_RX, "sniffer_rx_ppdu_free_ring_deinit::ppdu_list null");
        return;
    }

    /* 释放alloc list */
    mp17c_mpw_rx_ppdu_from_list_free(&hal_device->host_rx_ppdu_alloc_list);
    /* 释放ring本身 */
    free_rctl->un_read_ptr.read_ptr = 0;
    free_rctl->un_write_ptr.write_ptr = 0;
    hal_ring_set_sw2hw(free_rctl);
}

/*
 * 功能描述：rx status statistic 信息填充
 */
void mp17c_mpw_sniffer_set_rx_info(hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *cb,
    mp17c_mpw_rx_ppdu_dscr_stru *ppdu_dscr)
{
    hal_sniffer_rx_status_stru *sniffer_rx_status = &sniffer_rx_info->sniffer_rx_status;
    hal_sniffer_rx_statistic_stru *sniffer_rx_statistic = &sniffer_rx_info->sniffer_rx_statistic;
    sniffer_rx_status->bit_cipher_protocol_type = cb->bit_cipher_type;
    sniffer_rx_status->bit_dscr_status = cb->rx_status;
    sniffer_rx_status->bit_ampdu = ppdu_dscr->rx_ppdu_vector_info.ampdu_en;
    sniffer_rx_status->bit_last_mpdu_flag = 0;
    sniffer_rx_status->bit_gi_type = ppdu_dscr->rx_ppdu_vector_info.gi_type;
    sniffer_rx_status->bit_he_ltf_type = ppdu_dscr->rx_ppdu_vector_info.he_eht_ltf_type;
    sniffer_rx_status->bit_sounding_mode = ppdu_dscr->rx_ppdu_vector_info.sounding_mode;
    sniffer_rx_status->bit_freq_bandwidth_mode = ppdu_dscr->rx_ppdu_vector_info.freq_bw;
    sniffer_rx_status->bit_rx_himit_flag = ppdu_dscr->rx_ppdu_vector_info.preamble;
    sniffer_rx_status->bit_ext_spatial_streams = ppdu_dscr->rx_ppdu_vector_info.spatial_reuse;
    sniffer_rx_status->bit_smoothing = ppdu_dscr->rx_ppdu_vector_info.smoothing;
    sniffer_rx_status->bit_fec_coding = ppdu_dscr->rx_ppdu_user_info.fec_coding;
    sniffer_rx_status->un_nss_rate.st_rate.bit_rate_mcs = ppdu_dscr->rx_ppdu_user_info.nss_mcs_rate;
    sniffer_rx_status->un_nss_rate.st_rate.bit_nss_mode = 0;
    sniffer_rx_status->un_nss_rate.st_rate.bit_protocol_mode = ppdu_dscr->rx_ppdu_vector_info.protocol_mode;
    sniffer_rx_status->un_nss_rate.st_rate.bit_is_rx_vip = 0;
    sniffer_rx_status->un_nss_rate.st_rate.bit_rsp_flag = ppdu_dscr->rx_ppdu_vector_info.response_flag;
    sniffer_rx_status->un_nss_rate.st_rate.bit_mu_mimo_flag = ppdu_dscr->rx_ppdu_vector_info.mu_mimo_flag;
    sniffer_rx_status->un_nss_rate.st_rate.bit_ofdma_flag = ppdu_dscr->rx_ppdu_vector_info.ofdma_flag;
    sniffer_rx_status->un_nss_rate.st_rate.bit_beamforming_flag = 0;
    sniffer_rx_status->un_nss_rate.st_rate.bit_stbc = ppdu_dscr->rx_ppdu_vector_info.stbc_mode;

    sniffer_rx_statistic->c_rssi_dbm = 0;
    sniffer_rx_statistic->uc_code_book = 0;
    sniffer_rx_statistic->uc_grouping = 0;
    sniffer_rx_statistic->uc_row_number = 0;
    sniffer_rx_statistic->c_snr_ant0 = (int8_t)ppdu_dscr->rx_ppdu_vector_info.rpt_snr_0ch;
    sniffer_rx_statistic->c_snr_ant1 = (int8_t)ppdu_dscr->rx_ppdu_vector_info.rpt_snr_1ch;
    sniffer_rx_statistic->c_ant0_rssi = ppdu_dscr->rx_ppdu_vector_info.rpt_rssi_0ch_msb;
    sniffer_rx_statistic->c_ant1_rssi = ppdu_dscr->rx_ppdu_vector_info.rpt_rssi_1ch_msb;
}

/*
 * 功能描述：rx速率信息填充
 */
void mp17c_mpw_sniffer_set_rx_rate_info(hal_sniffer_extend_info *sniffer_rx_info,
    mp17c_mpw_rx_ppdu_dscr_stru *ppdu_dscr)
{
    hal_statistic_stru *per_rate = &sniffer_rx_info->per_rate;
    uint32_t *rate_kbps = &sniffer_rx_info->rate_kbps;

    /* 获取接收帧的速率 */
    per_rate->un_nss_rate.st_ht_rate.bit_ht_mcs = ppdu_dscr->rx_ppdu_user_info.nss_mcs_rate;
    per_rate->un_nss_rate.st_ht_rate.bit_protocol_mode = ppdu_dscr->rx_ppdu_vector_info.protocol_mode;
    per_rate->uc_short_gi = ppdu_dscr->rx_ppdu_vector_info.gi_type;
    per_rate->bit_preamble = ppdu_dscr->rx_ppdu_vector_info.preamble;
    per_rate->bit_stbc = ppdu_dscr->rx_ppdu_vector_info.stbc_mode;
    per_rate->bit_channel_code = ppdu_dscr->rx_ppdu_user_info.fec_coding;
    per_rate->uc_bandwidth = ppdu_dscr->rx_ppdu_vector_info.freq_bw;
    if (hmac_get_rate_kbps(per_rate, rate_kbps) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{sniffer_set_rx_rate_info::get rate failed.}");
        /* 若获取速率失败, 默认速率为6Mbps（6000kbps） */
        *rate_kbps = 6000;
    }
}

#define SNIFFER_INVALIAD_ADDR 0xffffffff
static uintptr_t mp17c_mpw_get_rx_ppdu_dscr_addr(oal_netbuf_stru *netbuf)
{
    uint64_t rx_ppdu_dscr_addr = SNIFFER_INVALIAD_ADDR;
    mp17c_mpw_rx_msdu_dscr_stru *rx_hw_dscr = NULL;

    /* 之前函数中修改data指针指向帧体 为获取帧头部分mpdu描述符中的lsb和msb 修改data指针指向帧头 */
    oal_netbuf_push(netbuf, MP17C_MPW_RX_MSDU_DSCR_LEN);

    rx_hw_dscr = (mp17c_mpw_rx_msdu_dscr_stru *)oal_netbuf_data(netbuf);
    if (rx_hw_dscr == NULL) {
        return rx_ppdu_dscr_addr;
    }
    if (rx_hw_dscr->ppdu_host_buf_addr_lsb != SNIFFER_INVALIAD_ADDR ||
        rx_hw_dscr->ppdu_host_buf_addr_msb != SNIFFER_INVALIAD_ADDR) {
        rx_ppdu_dscr_addr = oal_make_word64(rx_hw_dscr->ppdu_host_buf_addr_lsb, rx_hw_dscr->ppdu_host_buf_addr_msb);
    }

    /* 获取到地址值后 再修改data指针指向帧体部分 */
    oal_netbuf_pull(netbuf, MP17C_MPW_RX_MSDU_DSCR_LEN);

    return rx_ppdu_dscr_addr;
}

hal_host_rx_ppdu_dscr_stru *mp17c_mpw_sniffer_get_rx_ppdu_dscr(hal_host_device_stru *hal_dev, oal_netbuf_stru *netbuf)
{
    uintptr_t rx_ppdu_dscr_addr;
    uint64_t host_iova = 0;
    hal_host_rx_ppdu_dscr_stru *rx_ppdu = NULL;
    oal_dlist_head_stru *entry_ppdu = NULL;
    oal_dlist_head_stru *entry_tmp = NULL;

    rx_ppdu_dscr_addr = mp17c_mpw_get_rx_ppdu_dscr_addr(netbuf);
    if (oal_unlikely(rx_ppdu_dscr_addr == SNIFFER_INVALIAD_ADDR)) {
        return NULL;
    }

    if (oal_unlikely(pcie_if_devva_to_hostca(HCC_EP_WIFI_DEV, (uint64_t)rx_ppdu_dscr_addr, (uint64_t *)&host_iova) !=
        OAL_SUCC)) {
        return NULL;
    }

    oal_dlist_search_for_each_safe(entry_ppdu, entry_tmp, &hal_dev->host_rx_ppdu_alloc_list.list_head) {
        rx_ppdu = oal_dlist_get_entry(entry_ppdu, hal_host_rx_ppdu_dscr_stru, entry);
        if (rx_ppdu->paddr.addr == host_iova) {
            return rx_ppdu;
        }
    }

    return NULL;
}

void mp17c_mpw_sniffer_rx_info_fill_host_rx(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf,
    hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *rx_ctl)
{
    hal_host_rx_ppdu_dscr_stru *rx_ppdu = NULL;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    if (oal_unlikely(pcie_dev == NULL)) {
        oam_error_log0(0, 0, "sniffer_rx_info_fill:pcie_linux_res null");
        return;
    }

    if (!rx_ctl->bit_is_first_buffer) {
        return;
    }

    rx_ppdu = mp17c_mpw_sniffer_get_rx_ppdu_dscr(hal_device, netbuf);
    if (rx_ppdu == NULL) {
        return;
    }

    dma_unmap_single(&pcie_dev->dev, rx_ppdu->paddr.addr,
        sizeof(mp17c_mpw_rx_ppdu_dscr_stru), DMA_FROM_DEVICE);

    mp17c_mpw_sniffer_set_rx_info(sniffer_rx_info, rx_ctl, &rx_ppdu->hw_ppdu_data);
    mp17c_mpw_sniffer_set_rx_rate_info(sniffer_rx_info, &rx_ppdu->hw_ppdu_data);

    oal_dlist_delete_entry(&rx_ppdu->entry);
    oal_free(rx_ppdu);
}

void mp17c_mpw_sniffer_rx_info_fill_device_rx(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf,
    hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *rx_ctl)
{
    hal_sniffer_extend_info *sniffer_info = NULL;
    uint8_t *cb = oal_netbuf_cb(netbuf);
    sniffer_info = (hal_sniffer_extend_info *)(cb + HAL_DEVICE_MAX_CB_LEN);

    if (!rx_ctl->bit_is_first_buffer) {
        return;
    }

    memcpy_s(sniffer_rx_info, sizeof(hal_sniffer_extend_info), sniffer_info, sizeof(hal_sniffer_extend_info));
    if (hmac_get_rate_kbps(&sniffer_rx_info->per_rate, &sniffer_rx_info->rate_kbps) == OAL_FALSE) {
        oam_warning_log0(0, OAM_SF_ANY, "{sniffer_rx_info_fill_device_rx::get rate failed.}");
        /* 若获取速率失败, 默认速率为6Mbps（6000kbps） */
        sniffer_rx_info->rate_kbps = 6000;
    }
}

void mp17c_mpw_sniffer_rx_info_fill(hal_host_device_stru *hal_device, oal_netbuf_stru *netbuf,
    hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *rx_ctl)
{
    if (oal_unlikely(hal_device == NULL)) {
        oam_error_log0(0, 0, "sniffer_rx_info_fill:hal_device null");
        return;
    }

    if (oal_atomic_read(&hal_device->rx_mode) == HAL_DDR_RX) {
        mp17c_mpw_sniffer_rx_info_fill_host_rx(hal_device, netbuf, sniffer_rx_info, rx_ctl);
    } else if (oal_atomic_read(&hal_device->rx_mode) == HAL_RAM_RX) {
        mp17c_mpw_sniffer_rx_info_fill_device_rx(hal_device, netbuf, sniffer_rx_info, rx_ctl);
    } else {
        oam_warning_log1(0, OAM_SF_RX, "sniffer_rx_info_fill::rx_mode[%d]",
            oal_atomic_read(&hal_device->rx_mode));
    }
}
#endif /* _PRE_WLAN_FEATURE_SNIFFER */
