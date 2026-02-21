/*
 * Copyright (c) @CompanyNameMagicTag 2021-2025. All rights reserved.
 * 功能说明   : MAP UNMAP基本操作
 * 作者       : wifi
 * 创建日期   : 2025年1月4日
 */
#include "securec.h"
#include "oal_util.h"
#include "oal_ext_if.h"
#include "oam_ext_if.h"
#include "host_hal_ext_if.h"
#include "host_hal_device.h"
#include "pcie_linux.h"
#include "pcie_host.h"
#include "host_hal_ring.h"
#include "host_hal_dma.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID     OAM_FILE_ID_HOST_HAL_DMA

OAL_STATIC uint16_t g_dma_once_proc_num = DMA_MAP_ONCE_PROC_NUM_DEFAULT;
uint16_t hal_get_host_dma_once_proc_num(void)
{
    return g_dma_once_proc_num;
}

/*
 * 功能描述 : 清零netbuf headroom中的MSDU描述符内存空间, 并返回其地址
 * 1.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
uint8_t *hal_host_tx_netbuf_init_msdu_dscr(oal_netbuf_stru *netbuf)
{
    uint8_t msdu_dscr_len = hal_host_tx_get_msdu_dscr_len();
    uint8_t *msdu_dscr_addr = NULL;
    if (oal_unlikely(netbuf == NULL)) {
        return NULL;
    }
    msdu_dscr_addr = (uint8_t *)((uintptr_t)oal_netbuf_data(netbuf) - msdu_dscr_len);
    memset_s(msdu_dscr_addr, msdu_dscr_len, 0, msdu_dscr_len);
    return msdu_dscr_addr;
}

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
uint32_t hal_host_tx_dma_map_phyaddr(uint8_t *host_addr, uint32_t len, dma_addr_t *physaddr)
{
    oal_pci_dev_stru *pcie_dev = NULL;
    if (oal_unlikely(host_addr == NULL || physaddr == NULL)) {
        return OAL_FAIL;
    }
    pcie_dev = oal_get_wifi_pcie_dev();
    if (oal_unlikely(pcie_dev == NULL)) {
        oam_error_log0(0, 0, "{hal_host_tx_dma_map_phyaddr::pcie_dev NULL}");
        return OAL_FAIL;
    }

    *physaddr = dma_map_single(&pcie_dev->dev, host_addr, len, DMA_TO_DEVICE);
    if (dma_mapping_error(&pcie_dev->dev, *physaddr) != 0) {
        oam_error_log0(0, 0, "{hal_host_tx_dma_map_phyaddr::addr map failed}");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t hal_host_tx_dma_unmap_addr(uint8_t *dma_addr, uint32_t len)
{
    oal_pci_dev_stru *pcie_dev = NULL;
    if (oal_unlikely(dma_addr == NULL)) {
        return OAL_FAIL;
    }
    pcie_dev = oal_get_wifi_pcie_dev();
    if (oal_unlikely(pcie_dev == NULL)) {
        oam_error_log0(0, OAM_SF_TX, "{hal_host_tx_dma_unmap_addr::pcie_dev NULL}");
        return OAL_FAIL;
    }

    dma_unmap_single(&pcie_dev->dev, (dma_addr_t)(uintptr_t)dma_addr, len, DMA_TO_DEVICE);

    return OAL_SUCC;
}
#else
uint32_t hal_host_tx_dma_map_phyaddr(uint8_t *host_addr, uint32_t len, dma_addr_t *physaddr)
{
    *physaddr = (dma_addr_t)host_addr;
    return OAL_SUCC;
}

uint32_t hal_host_tx_dma_unmap_addr(uint8_t *dma_addr, uint32_t len)
{
    return OAL_SUCC;
}
#endif

void hal_host_rx_dma_map_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after)
{
#ifdef _PRE_WLAN_FEATURE_DMA_MAP_BATCH
    hal_host_dma_proc(before, after, DMA_BATCH_HOST_RX_MAP);
#else
    dma_addr_t pci_dma_addr;
    pcie_cb_dma_res *cb_res = NULL;
    oal_netbuf_stru *netbuf = NULL;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    if (oal_unlikely(pcie_dev == NULL)) {
        return;
    }

    while ((netbuf = oal_netbuf_delist_nolock(before)) != NULL) {
        pci_dma_addr = dma_map_single(&pcie_dev->dev, oal_netbuf_data(netbuf), oal_netbuf_len(netbuf), DMA_FROM_DEVICE);
        if ((dma_mapping_error(&pcie_dev->dev, pci_dma_addr) != 0) || (pci_dma_addr <= DMA_RX_MAP_ERROR_ADDR)) {
            oam_error_log2(0, 0, "{hal_host_rx_dma_map_proc::addr map failed, skb[0x%llx] iova[0x%llx]}",
                (uintptr_t)netbuf, (uintptr_t)pci_dma_addr);
            oal_netbuf_free(netbuf);
            continue;
        }
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
        if (oal_likely(cb_res != NULL)) {
            cb_res->paddr.addr = pci_dma_addr;
            cb_res->len = oal_netbuf_len(netbuf);
        }
        oal_netbuf_add_to_list_tail(netbuf, after);
    }
#endif
}

void hal_host_rx_dma_unmap_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after)
{
#ifdef _PRE_WLAN_FEATURE_DMA_MAP_BATCH
    hal_host_dma_proc(before, after, DMA_BATCH_HOST_RX_UNMAP);
#else
    pcie_cb_dma_res *cb_res = NULL;
    oal_netbuf_stru *netbuf = NULL;
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();

    while ((netbuf = oal_netbuf_delist_nolock(before)) != NULL) {
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
#ifndef _PRE_LINUX_TEST
        if (cb_res != NULL && cb_res->paddr.addr <= DMA_RX_MAP_ERROR_ADDR) {
            oam_error_log2(0, 0, "{hal_host_rx_dma_unmap_proc::dma unmap addr error, skb[0x%llx] iova[0x%llx]}",
                (uintptr_t)netbuf, (uintptr_t)cb_res->paddr.addr);
            return;
        }
#endif
        if (oal_likely(cb_res != NULL && pcie_dev != NULL)) {
            dma_unmap_single(&pcie_dev->dev, cb_res->paddr.addr, netbuf->len, DMA_FROM_DEVICE);
        }
        oal_netbuf_add_to_list_tail(netbuf, after);
    }
#endif
}

void hal_host_tx_dma_map_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after)
{
#ifdef _PRE_WLAN_FEATURE_DMA_MAP_BATCH
    hal_host_dma_proc(before, after, DMA_BATCH_HOST_TX_MAP);
#else
    uint32_t ret;
    dma_addr_t addr;
    mac_tx_ctl_stru *tx_ctl = NULL;
    oal_netbuf_stru *netbuf = NULL;

    while ((netbuf = oal_netbuf_delist_nolock(before)) != NULL) {
        ret = hal_host_tx_dma_map_phyaddr(oal_netbuf_data(netbuf) - hal_host_tx_get_msdu_dscr_len(),
            oal_netbuf_len(netbuf) + hal_host_tx_get_msdu_dscr_len(), &addr);
        if (oal_unlikely(ret != OAL_SUCC)) {
            oam_error_log0(0, 0, "hal_host_tx_dma_map_phyaddr fail");
            oal_netbuf_free(netbuf);
            continue;
        }
        tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (oal_likely(tx_ctl != NULL && ret == OAL_SUCC)) {
            tx_ctl->st_expand_cb.pcie_dma_addr = addr;
            tx_ctl->st_expand_cb.pcie_dma_len = oal_netbuf_len(netbuf) + hal_host_tx_get_msdu_dscr_len();
            tx_ctl->st_expand_cb.dma_mapped = OAL_TRUE;
        }
        oal_netbuf_add_to_list_tail(netbuf, after);
    }
#endif
}

void hal_host_tx_dma_unmap_step_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    oal_netbuf_stru *netbuf = NULL;
    if (oal_unlikely(before == NULL || after == NULL)) {
        return;
    }

    while ((netbuf = oal_netbuf_delist_nolock(before)) != NULL) {
        tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        if (oal_likely(tx_ctl != NULL && tx_ctl->st_expand_cb.dma_mapped == OAL_TRUE)) {
            hal_host_tx_dma_unmap_addr((uint8_t *)(uintptr_t)tx_ctl->st_expand_cb.pcie_dma_addr,
                tx_ctl->st_expand_cb.pcie_dma_len);
            tx_ctl->st_expand_cb.dma_mapped = OAL_FALSE;
        } else {
            oam_error_log0(0, 0, "hal_host_tx_dma_unmap_proc dma not mapped");
        }
        oal_netbuf_add_to_list_tail(netbuf, after);
    }
}

void hal_host_tx_dma_unmap_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after)
{
#ifdef _PRE_WLAN_FEATURE_DMA_MAP_BATCH
    hal_host_dma_proc(before, after, DMA_BATCH_HOST_TX_UNMAP);
#else
    hal_host_tx_dma_unmap_step_proc(before, after);
#endif
}

void hal_host_tx_unmap_single_netbuf(oal_netbuf_stru *netbuf)
{
    mac_tx_ctl_stru *tx_ctl = NULL;
    if (oal_unlikely(netbuf == NULL)) {
        return;
    }
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (oal_unlikely(tx_ctl == NULL)) {
        return;
    }
    if (tx_ctl->st_expand_cb.dma_mapped == OAL_TRUE) {
        hal_host_tx_dma_unmap_addr((uint8_t *)(uintptr_t)tx_ctl->st_expand_cb.pcie_dma_addr,
            tx_ctl->st_expand_cb.pcie_dma_len);
        tx_ctl->st_expand_cb.dma_mapped = OAL_FALSE;
    } else {
        oam_error_log0(0, 0, "hal_host_tx_unmap_single_netbuf dma not mapped");
    }
}

void hal_host_tx_unmap_netbuf_list(oal_netbuf_stru *netbuf)
{
    oal_netbuf_stru *next = NULL;
    if (oal_unlikely(!netbuf)) {
        return;
    }

    while (netbuf != NULL) {
        next = oal_netbuf_list_next(netbuf);
        hal_host_tx_unmap_single_netbuf(netbuf);
        netbuf = next;
    }
}

void hal_host_clear_netbuf_list(oal_netbuf_head_stru *netbub_list)
{
    oal_netbuf_stru *netbuf = NULL;
    if (netbub_list == NULL) {
        return;
    }
    while ((netbuf = oal_netbuf_delist_nolock(netbub_list)) != NULL) {
        oal_netbuf_free(netbuf);
    }
}

/*
 * 功能描述 : hostva映射成devva, 供mac访问host ring内容
 * dma_map_batch处理场景，走到此函数是已经map好了，此处仅需要从cb中取出dma地址进行转换
 */
uint32_t hal_host_tx_get_devva(oal_netbuf_stru *netbuf, uint64_t *devva)
{
    uint32_t alloc_size;
    dma_addr_t pcie_dma_addr;
    mac_tx_ctl_stru *tx_ctl = NULL;
    if (oal_unlikely(netbuf == NULL || devva == NULL)) {
        return OAL_FAIL;
    }
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (oal_unlikely(tx_ctl == NULL)) {
        return OAL_FAIL;
    }
    if (oal_unlikely(tx_ctl->st_expand_cb.dma_mapped == OAL_FALSE)) {
        oam_error_log0(0, 0, "hal_host_tx_get_devva dma not mapped");
        return OAL_FAIL;
    }

    pcie_dma_addr = tx_ctl->st_expand_cb.pcie_dma_addr;
    alloc_size = tx_ctl->st_expand_cb.pcie_dma_len;

    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)pcie_dma_addr, devva) != OAL_SUCC) {
        oam_error_log0(0, 0, "hal_host_tx_get_devva hostca_to_devva fail");
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

/*
 * 功能描述 : 单个tx netbuf map接口
 */
uint32_t hal_host_ring_tx_single_map(oal_netbuf_stru *netbuf)
{
    uint32_t ret;
    dma_addr_t addr;
    mac_tx_ctl_stru *tx_ctl = NULL;
    if (oal_unlikely(netbuf == NULL)) {
        return OAL_FAIL;
    }
    tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    if (oal_unlikely(tx_ctl == NULL)) {
        return OAL_FAIL;
    }
    (void)hal_host_tx_netbuf_init_msdu_dscr(netbuf);
    ret = hal_host_tx_dma_map_phyaddr(oal_netbuf_data(netbuf) - hal_host_tx_get_msdu_dscr_len(),
        oal_netbuf_len(netbuf) + hal_host_tx_get_msdu_dscr_len(), &addr);
    if (oal_likely(tx_ctl != NULL && ret == OAL_SUCC)) {
        tx_ctl->st_expand_cb.pcie_dma_addr = addr;
        tx_ctl->st_expand_cb.pcie_dma_len = oal_netbuf_len(netbuf) + hal_host_tx_get_msdu_dscr_len();
        tx_ctl->st_expand_cb.dma_mapped = OAL_TRUE;
    }
    return ret;
}