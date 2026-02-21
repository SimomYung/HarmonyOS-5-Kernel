/*
 * Copyright (c) @CompanyNameMagicTag 2021-2025. All rights reserved.
 * 功能说明   : MAP BATCH基本操作
 * 作者       : wifi
 * 创建日期   : 2025年1月4日
 */
#ifdef _PRE_WLAN_FEATURE_DMA_MAP_BATCH
#undef THIS_FILE_ID
#define THIS_FILE_ID     OAM_FILE_ID_HOST_HAL_DMA_BATCH

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

OAL_STATIC void hal_host_rx_after_map_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after);
OAL_STATIC void hal_host_dma_map_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset);
OAL_STATIC void hal_host_trx_after_unmap_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after);
OAL_STATIC void hal_host_dma_map_batch(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after, uint8_t type);
OAL_STATIC void hal_host_dma_unmap_batch(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after, uint8_t type);
OAL_STATIC void hal_host_dma_rx_unmap_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset);
OAL_STATIC uint8_t hal_host_dma_get_no_offset(void);
OAL_STATIC void hal_host_tx_after_map_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after);
OAL_STATIC uint8_t hal_host_tx_dma_offset(void);
OAL_STATIC void hal_host_dma_tx_unmap_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset);

OAL_STATIC struct scatterlist g_dma_sgl_array[DMA_BATCH_BUTT][DMA_MAP_BATCH_MAX_NUM];
OAL_STATIC dma_batch_table_stru g_dma_batch_ops[DMA_BATCH_BUTT] = {
    {DMA_BATCH_HOST_RX_MAP, DMA_FROM_DEVICE, hal_host_dma_map_set_sgl_buf,
        hal_host_dma_map_batch, hal_host_rx_after_map_proc, hal_host_dma_get_no_offset},
    {DMA_BATCH_HOST_RX_UNMAP, DMA_FROM_DEVICE, hal_host_dma_rx_unmap_set_sgl_buf,
        hal_host_dma_unmap_batch, hal_host_trx_after_unmap_proc, hal_host_dma_get_no_offset},
    {DMA_BATCH_HOST_TX_MAP, DMA_TO_DEVICE, hal_host_dma_map_set_sgl_buf,
        hal_host_dma_map_batch, hal_host_tx_after_map_proc, hal_host_tx_dma_offset},
    {DMA_BATCH_HOST_TX_UNMAP, DMA_TO_DEVICE, hal_host_dma_tx_unmap_set_sgl_buf,
        hal_host_dma_unmap_batch, hal_host_trx_after_unmap_proc, hal_host_tx_dma_offset},
};

OAL_STATIC struct scatterlist *dma_batch_get_sgl(uint8_t type)
{
    if (type >= DMA_BATCH_BUTT) {
        return NULL;
    }
    return &g_dma_sgl_array[type][0];
}

OAL_STATIC dma_batch_table_stru *dma_batch_get_ops(uint8_t type)
{
    if (type >= DMA_BATCH_BUTT) {
        return NULL;
    }
    return &g_dma_batch_ops[type];
}

OAL_STATIC void hal_dma_batch_free_netbuf_array(oal_netbuf_stru *netbuf_array[], uint16_t need_map_num)
{
    uint16_t i;
    oal_netbuf_stru *netbuf = NULL;
    if (netbuf_array == NULL) {
        return;
    }
    for (i = 0; i < need_map_num; i++) {
        netbuf = netbuf_array[i];
        if (netbuf != NULL) {
            oal_netbuf_free(netbuf);
        }
        netbuf_array[i] = NULL;
    }
}

OAL_STATIC void hal_host_dma_map_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset)
{
    sg_set_buf(sg, oal_netbuf_data(netbuf) - offset, oal_netbuf_len(netbuf) + offset);
    netbuf_array[need_map_num] = netbuf;
}

OAL_STATIC void hal_host_dma_rx_unmap_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset)
{
    pcie_cb_dma_res *cb_res = NULL;
    cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
    sg->dma_address = cb_res->paddr.addr;
    sg->length = cb_res->len;
    netbuf_array[need_map_num] = netbuf;
}

OAL_STATIC void hal_host_dma_tx_unmap_set_sgl_buf(struct scatterlist *sg, oal_netbuf_stru *netbuf_array[],
    uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t offset)
{
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
    sg->dma_address = tx_ctl->st_expand_cb.pcie_dma_addr;
    sg->length = tx_ctl->st_expand_cb.pcie_dma_len;
    netbuf_array[need_map_num] = netbuf;
    if (tx_ctl->st_expand_cb.dma_mapped == OAL_FALSE) {
        oam_error_log0(0, 0, "hal_host_dma_tx_unmap_set_sgl_buf dma not mapped");
    }
    tx_ctl->st_expand_cb.dma_mapped = OAL_FALSE;
}

OAL_STATIC void hal_host_dma_map_batch(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after, uint8_t type)
{
    uint32_t ret;
    dma_batch_table_stru *ops = dma_batch_get_ops(type);
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    if (ops == NULL || pcie_dev == NULL) {
        oam_error_log0(0, 0, "hal_host_dma_map_batch: para is null");
        hal_dma_batch_free_netbuf_array(netbuf_array, need_map_num);
        return;
    }

    sg_init_marker(sgl, need_map_num);
    ret = dma_map_page_batch(&pcie_dev->dev, sgl, need_map_num, ops->dma_dir);
    /* DMA地址映射失败，释放内存 */
    if (ret == 0) {
        oam_error_log0(0, 0, "hal_host_dma_map_batch: fail");
        hal_dma_batch_free_netbuf_array(netbuf_array, need_map_num);
        return;
    }
    ops->hal_host_after_dma_batch_proc(sgl, need_map_num, netbuf_array, after);
}

OAL_STATIC void hal_host_dma_unmap_batch(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after, uint8_t type)
{
    dma_batch_table_stru *ops = dma_batch_get_ops(type);
    oal_pci_dev_stru *pcie_dev = oal_get_wifi_pcie_dev();
    if (ops == NULL || pcie_dev == NULL) {
        oam_error_log0(0, 0, "hal_host_dma_unmap_batch: para is null");
        hal_dma_batch_free_netbuf_array(netbuf_array, need_map_num);
        return;
    }

    sg_init_marker(sgl, need_map_num);
    dma_unmap_page_batch(&pcie_dev->dev, sgl, need_map_num, ops->dma_dir);
    ops->hal_host_after_dma_batch_proc(sgl, need_map_num, netbuf_array, after);
}

void hal_host_dma_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after, uint8_t type)
{
    uint8_t offset;
    uint16_t need_map_num = 0;
    oal_netbuf_stru *netbuf = NULL;
    oal_netbuf_stru *netbuf_array[DMA_MAP_BATCH_MAX_NUM] = {NULL};
    dma_batch_table_stru *ops = dma_batch_get_ops(type);
    struct scatterlist *sgl = dma_batch_get_sgl(type);

    if (sgl == NULL || before == NULL || after == NULL || ops == NULL) {
        oam_error_log0(0, 0, "hal_host_dma_proc:para is null");
        return;
    }

    memset_s(sgl, sizeof(struct scatterlist) * DMA_MAP_BATCH_MAX_NUM,
        0, sizeof(struct scatterlist) * DMA_MAP_BATCH_MAX_NUM);

    while ((netbuf = oal_netbuf_delist_nolock(before)) != NULL) {
        offset = ops->hal_host_get_dma_offset();
        ops->hal_host_dma_set_sgl_buf(&sgl[need_map_num], netbuf_array, need_map_num, netbuf, offset);
        need_map_num++;

        if (need_map_num >= DMA_MAP_BATCH_MAX_NUM) {
            ops->hal_host_dma_batch(sgl, need_map_num, netbuf_array, after, type);
            memset_s(sgl, sizeof(struct scatterlist) * DMA_MAP_BATCH_MAX_NUM,
                0, sizeof(struct scatterlist) * DMA_MAP_BATCH_MAX_NUM);
            memset_s(netbuf_array, sizeof(netbuf_array), 0, sizeof(netbuf_array));
            need_map_num = 0;
        }
    }
    if (need_map_num > 0) {
        ops->hal_host_dma_batch(sgl, need_map_num, netbuf_array, after, type);
    }
}

/* AFTER MAP BATCH 处理函数 */
OAL_STATIC void hal_host_tx_after_map_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after)
{
    uint16_t i;
    oal_netbuf_stru *netbuf = NULL;
    mac_tx_ctl_stru *tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);

    /* DMA地址映射成功，DMA地址写入cb中，同时netbuf串入链表 */
    for (i = 0; i < need_map_num; i++) {
        netbuf = netbuf_array[i];
        if (oal_unlikely(netbuf == NULL)) {
            oam_error_log1(0, 0, "hal_host_tx_after_map_proc i[%u]", i);
            continue;
        }
        tx_ctl = (mac_tx_ctl_stru *)oal_netbuf_cb(netbuf);
        tx_ctl->st_expand_cb.pcie_dma_addr = sg_dma_address(&sgl[i]);
        tx_ctl->st_expand_cb.pcie_dma_len = sgl[i].length;
        tx_ctl->st_expand_cb.dma_mapped = OAL_TRUE;
        oal_netbuf_list_tail(after, netbuf);
    }
}

OAL_STATIC void hal_host_rx_after_map_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after)
{
    uint16_t i;
    oal_netbuf_stru *netbuf = NULL;
    pcie_cb_dma_res *cb_res = NULL;
    /* DMA地址映射成功，DMA地址写入cb中，同时netbuf串入链表 */
    for (i = 0; i < need_map_num; i++) {
        netbuf = netbuf_array[i];
        if (oal_unlikely(netbuf == NULL)) {
            oam_error_log1(0, 0, "hal_host_rx_after_map_proc i[%u]", i);
            continue;
        }
        cb_res = (pcie_cb_dma_res *)oal_netbuf_cb(netbuf);
        cb_res->paddr.addr = sg_dma_address(&sgl[i]);
        cb_res->len = oal_netbuf_len(netbuf);
        oal_netbuf_list_tail(after, netbuf);
    }
}

/* AFTER UNMAP BATCH 处理函数 */
OAL_STATIC void hal_host_trx_after_unmap_proc(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after)
{
    uint16_t i;
    oal_netbuf_stru *netbuf = NULL;

    for (i = 0; i < need_map_num; i++) {
        netbuf = netbuf_array[i];
        if (oal_unlikely(netbuf == NULL)) {
            oam_error_log1(0, 0, "hal_host_trx_after_unmap_proc i[%u]", i);
            continue;
        }
        oal_netbuf_list_tail(after, netbuf);
    }
}

/* DMA OFFSET 处理函数 */
OAL_STATIC uint8_t hal_host_dma_get_no_offset(void)
{
    return 0;
}

OAL_STATIC uint8_t hal_host_tx_dma_offset(void)
{
    return hal_host_tx_get_msdu_dscr_len();
}

#endif