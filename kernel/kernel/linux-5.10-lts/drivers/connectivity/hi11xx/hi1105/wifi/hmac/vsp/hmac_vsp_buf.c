/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : vsp dma-buf管理
 * 作    者 : wifi
 * 创建日期 : 2022年3月22日
 */

#include "hmac_vsp_buf.h"
#include "oal_dma_buf.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_VSP_BUF_C

#ifdef _PRE_WLAN_FEATURE_VSP
#ifdef CONFIG_VCODEC_VSP_SUPPORT
typedef struct {
    oal_dlist_head_stru entry;
    struct dma_buf *dmabuf;
    struct dma_buf_attachment *attachment;
    struct sg_table *table;
    oal_pci_dev_stru *pci_dev;
} hmac_vsp_dma_buf_context_stru;

oal_dlist_head_stru g_vsp_dma_buf_head = {NULL, NULL};

void hmac_vsp_dma_list_init(void)
{
    oal_dlist_init_head(&g_vsp_dma_buf_head);
}
void hmac_vsp_dma_list_add_tail(oal_dlist_head_stru *new)
{
    oal_dlist_add_tail(new, &g_vsp_dma_buf_head);
}

hmac_vsp_dma_buf_context_stru *hmac_vsp_get_dma_node(struct dma_buf *dmabuf)
{
    oal_dlist_head_stru *entry = NULL;
    oal_dlist_head_stru *entry_tmp = NULL;
    hmac_vsp_dma_buf_context_stru *dma_buf_context = NULL;

    oal_dlist_search_for_each_safe(entry, entry_tmp, &g_vsp_dma_buf_head) {
        dma_buf_context = oal_dlist_get_entry(entry, hmac_vsp_dma_buf_context_stru, entry);
        if (dma_buf_context->dmabuf == dmabuf) {
            oal_dlist_delete_entry(&dma_buf_context->entry);
            return dma_buf_context;
        }
    }
    return NULL;
}

static uint32_t hmac_vsp_source_init_dma_buf_context(hmac_vsp_dma_buf_context_stru *context)
{
    context->pci_dev = oal_get_wifi_pcie_dev();

    if (!context->pci_dev) {
        return OAL_FAIL;
    }

    context->attachment = oal_dma_buf_attach(context->dmabuf, &context->pci_dev->dev);
    context->table = oal_dma_buf_map_attach(context->attachment, DMA_BIDIRECTIONAL);

    return OAL_SUCC;
}

static void hmac_vsp_source_deinit_dma_buf_context(hmac_vsp_dma_buf_context_stru *context)
{
    oal_dma_buf_unmap_attachment(context->attachment, context->table, DMA_BIDIRECTIONAL);
    oal_dma_buf_detach(context->dmabuf, context->attachment);
}

uint32_t hmac_vsp_source_map_dma_buf(struct dma_buf *dmabuf, uintptr_t *iova)
{
    hmac_vsp_dma_buf_context_stru *context = NULL;
    if (dmabuf == NULL) {
        return OAL_FAIL;
    }

    if (oal_dma_buf_begin_cpu_access(dmabuf, DMA_FROM_DEVICE)) {
        return OAL_FAIL;
    }
    context = oal_memalloc(sizeof(hmac_vsp_dma_buf_context_stru));
    if (context == NULL) {
        return OAL_FAIL;
    }
    context->dmabuf = dmabuf;
    if (hmac_vsp_source_init_dma_buf_context(context) != OAL_SUCC) {
        oal_free(context);
        return OAL_FAIL;
    }

    if (!dma_map_sg(&context->pci_dev->dev, context->table->sgl, context->table->nents, DMA_BIDIRECTIONAL)) {
        oal_free(context);
        return OAL_FAIL;
    }

    *iova = sg_dma_address(context->table->sgl);
    hmac_vsp_dma_list_add_tail(&context->entry);

    return OAL_SUCC;
}

uint32_t hmac_vsp_source_unmap_dma_buf(struct dma_buf *dmabuf)
{
    hmac_vsp_dma_buf_context_stru *context = NULL;
    if (dmabuf == NULL) {
        return OAL_FAIL;
    }

    if (oal_dma_buf_end_cpu_access(dmabuf, DMA_TO_DEVICE)) {
        return OAL_FAIL;
    }
    context = hmac_vsp_get_dma_node(dmabuf);
    if (context == NULL) {
        return OAL_FAIL;
    }

    dma_unmap_sg(&context->pci_dev->dev, context->table->sgl, context->table->nents, DMA_BIDIRECTIONAL);
    hmac_vsp_source_deinit_dma_buf_context(context);
    oal_free(context);

    return OAL_SUCC;
}

void hmac_vsp_clear_dma_buf_list(void)
{
    oal_dlist_head_stru *entry = NULL;
    oal_dlist_head_stru *entry_tmp = NULL;
    hmac_vsp_dma_buf_context_stru *context = NULL;

    if (oal_dlist_is_empty(&g_vsp_dma_buf_head)) {
        return;
    }
    // 这里一般不会走进来，走进来说明VENC侧没有彻底执行dma unmap流程
    oam_error_log0(0, 0, "{hmac_vsp_clear_dma_buf_list::dma buf list is not empty}");
    oal_dlist_search_for_each_safe(entry, entry_tmp, &g_vsp_dma_buf_head) {
        context = oal_dlist_get_entry(entry, hmac_vsp_dma_buf_context_stru, entry);
        if (context->dmabuf != NULL) {
            oal_dlist_delete_entry(&context->entry);
            oal_dma_buf_end_cpu_access(context->dmabuf, DMA_TO_DEVICE);
            dma_unmap_sg(&context->pci_dev->dev, context->table->sgl, context->table->nents, DMA_BIDIRECTIONAL);
            hmac_vsp_source_deinit_dma_buf_context(context);
            oal_free(context);
        }
    }
}
#endif
#endif
