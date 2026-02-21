/*
 * Copyright (c) @CompanyNameMagicTag 2021-2025. All rights reserved.
 * 功能说明   : map/unmap batch基本操作
 * 作者       : wifi
 * 创建日期   : 2025年1月4日
 */

#ifndef HOST_HAL_MAP_BATCH_H
#define HOST_HAL_MAP_BATCH_H

#include "oal_types.h"
#include "oal_net.h"

#define DMA_MAP_BATCH_MAX_NUM 128
#define DMA_MAP_ONCE_PROC_NUM_DEFAULT 8

typedef enum {
    DMA_BATCH_HOST_RX_MAP = 0,
    DMA_BATCH_HOST_RX_UNMAP = 1,
    DMA_BATCH_HOST_TX_MAP = 2,
    DMA_BATCH_HOST_TX_UNMAP = 3,
    DMA_BATCH_BUTT,
} dma_batch_enum;

typedef void (*p_hal_host_after_dma_batch_proc_func)(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after);
typedef void (*p_hal_host_dma_set_sgl_buf_func)(struct scatterlist *sg,
    oal_netbuf_stru *netbuf_array[], uint16_t need_map_num, oal_netbuf_stru *netbuf, uint8_t type);
typedef void (*p_hal_host_dma_batch_func)(struct scatterlist *sgl, uint16_t need_map_num,
    oal_netbuf_stru *netbuf_array[], oal_netbuf_head_stru *after, uint8_t type);
typedef uint8_t (*p_hal_host_get_dma_offset_func)(void);

typedef struct {
    uint8_t type;
    uint8_t dma_dir;
    p_hal_host_dma_set_sgl_buf_func hal_host_dma_set_sgl_buf;
    p_hal_host_dma_batch_func hal_host_dma_batch;
    p_hal_host_after_dma_batch_proc_func hal_host_after_dma_batch_proc;
    p_hal_host_get_dma_offset_func hal_host_get_dma_offset;
} dma_batch_table_stru;

void hal_host_dma_proc(oal_netbuf_head_stru *before, oal_netbuf_head_stru *after, uint8_t type);

#endif /* HOST_HAL_MAP_BATCH_H */
