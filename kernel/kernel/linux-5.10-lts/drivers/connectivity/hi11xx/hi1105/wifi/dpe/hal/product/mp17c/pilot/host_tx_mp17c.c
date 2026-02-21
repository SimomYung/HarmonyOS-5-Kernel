/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host侧MP17C芯片相关TX功能
 * 作    者 : wifi
 * 创建日期 : 2021年12月26日
 */

#include "host_tx_mp17c.h"
#include "host_dscr_mp17c.h"
#include "host_hal_access_mgmt.h"
#include "pcie_host.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_TX_MP17C_C

#define MP17C_DEVICE_LARGE_TX_RING_NUM 21
#define MP17C_DEVICE_SMALL_TX_RING_NUM 91
#define MP17C_DEVICE_TX_RING_TOTAL_NUM (MP17C_DEVICE_LARGE_TX_RING_NUM + MP17C_DEVICE_SMALL_TX_RING_NUM)
uint8_t mp17c_host_get_device_tx_ring_num(void)
{
    return MP17C_DEVICE_TX_RING_TOTAL_NUM;
}

#define MP17C_MSDU_INFO_LEN 4
#define MP17C_MSDU_ADDR_LEN 8
uint32_t mp17c_host_alloc_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint32_t size)
{
    if (hal_host_tx_dma_alloc_ring_buf(&msdu_ring->msdu_info_ring, size * MP17C_MSDU_INFO_LEN) != OAL_SUCC) {
        return OAL_FAIL;
    }

    if (hal_host_tx_dma_alloc_ring_buf(&msdu_ring->msdu_addr_ring, size * MP17C_MSDU_ADDR_LEN) != OAL_SUCC) {
        hal_host_tx_dma_release_ring_buf(&msdu_ring->msdu_info_ring);
        return OAL_FAIL;
    }

    return OAL_SUCC;
}

uint32_t mp17c_host_release_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    uint32_t ret = OAL_SUCC;

    if (hal_host_tx_dma_release_ring_buf(&msdu_ring->msdu_info_ring) != OAL_SUCC) {
        ret = OAL_FAIL;
    }

    if (hal_host_tx_dma_release_ring_buf(&msdu_ring->msdu_addr_ring) != OAL_SUCC) {
        ret = OAL_FAIL;
    }

    return ret;
}

uint8_t mp17c_host_tx_ring_alloced(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    return msdu_ring->msdu_info_ring.ring_buf != NULL && msdu_ring->msdu_addr_ring.ring_buf != NULL;
}

static mp17c_tx_msdu_addr_stru *mp17c_host_tx_ring_msdu_addr(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    return msdu_ring->msdu_addr_ring.ring_buf ?
           (mp17c_tx_msdu_addr_stru *)(msdu_ring->msdu_addr_ring.ring_buf + index * MP17C_MSDU_ADDR_LEN) : NULL;
}

static mp17c_tx_msdu_info_stru *mp17c_host_tx_ring_msdu_info(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    return msdu_ring->msdu_info_ring.ring_buf ?
           (mp17c_tx_msdu_info_stru *)(msdu_ring->msdu_info_ring.ring_buf + index * MP17C_MSDU_INFO_LEN) : NULL;
}

static inline void mp17c_host_tx_reset_msdu_info(mp17c_tx_msdu_info_stru *mp17c_msdu_info)
{
    memset_s(mp17c_msdu_info, sizeof(mp17c_tx_msdu_info_stru), 0, sizeof(mp17c_tx_msdu_info_stru));
}

static void mp17c_host_tx_set_msdu_info(mp17c_tx_msdu_info_stru *mp17c_msdu_info, msdu_info_stru *msdu_info)
{
    mp17c_host_tx_reset_msdu_info(mp17c_msdu_info);
    mp17c_msdu_info->msdu_len = msdu_info->msdu_len;
    mp17c_msdu_info->data_type = msdu_info->data_type;
    mp17c_msdu_info->frag_num = msdu_info->frag_num;
    mp17c_msdu_info->to_ds = msdu_info->to_ds;
    mp17c_msdu_info->from_ds = msdu_info->from_ds;
    mp17c_msdu_info->more_frag = msdu_info->more_frag;
    mp17c_msdu_info->aggr_msdu_num = msdu_info->aggr_msdu_num;
    mp17c_msdu_info->first_msdu = msdu_info->first_msdu;
    mp17c_msdu_info->csum_type = msdu_info->csum_type;
}

static inline void mp17c_host_tx_reset_msdu_addr(mp17c_tx_msdu_addr_stru *mp17c_msdu_addr)
{
    memset_s(mp17c_msdu_addr, sizeof(mp17c_tx_msdu_addr_stru), 0, sizeof(mp17c_tx_msdu_addr_stru));
}

static void mp17c_host_tx_set_msdu_addr(mp17c_tx_msdu_addr_stru *mp17c_msdu_addr, msdu_info_stru *msdu_info)
{
    mp17c_host_tx_reset_msdu_addr(mp17c_msdu_addr);
    mp17c_msdu_addr->msdu_addr_lsb = msdu_info->msdu_addr_lsb;
    mp17c_msdu_addr->msdu_addr_msb = msdu_info->msdu_addr_msb;
}

uint32_t mp17c_host_tx_set_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
    mp17c_tx_msdu_info_stru *mp17c_msdu_info = mp17c_host_tx_ring_msdu_info(msdu_ring, index);
    mp17c_tx_msdu_addr_stru *mp17c_msdu_addr = mp17c_host_tx_ring_msdu_addr(msdu_ring, index);

    if (oal_any_null_ptr3(mp17c_msdu_info, mp17c_msdu_addr, msdu_info)) {
        return OAL_FAIL;
    }

    mp17c_host_tx_set_msdu_info(mp17c_msdu_info, msdu_info);
    mp17c_host_tx_set_msdu_addr(mp17c_msdu_addr, msdu_info);

    return OAL_SUCC;
}

static void mp17c_host_tx_get_msdu_info(mp17c_tx_msdu_info_stru *mp17c_msdu_info, msdu_info_stru *msdu_info)
{
    msdu_info->msdu_len = mp17c_msdu_info->msdu_len;
    msdu_info->data_type = mp17c_msdu_info->data_type;
    msdu_info->frag_num = mp17c_msdu_info->frag_num;
    msdu_info->to_ds = mp17c_msdu_info->to_ds;
    msdu_info->from_ds = mp17c_msdu_info->from_ds;
    msdu_info->more_frag = mp17c_msdu_info->more_frag;
    msdu_info->aggr_msdu_num = mp17c_msdu_info->aggr_msdu_num;
    msdu_info->first_msdu = mp17c_msdu_info->first_msdu;
    msdu_info->csum_type = mp17c_msdu_info->csum_type;
}

static void mp17c_host_tx_get_msdu_addr(mp17c_tx_msdu_addr_stru *mp17c_msdu_addr, msdu_info_stru *msdu_info)
{
    msdu_info->msdu_addr_lsb = mp17c_msdu_addr->msdu_addr_lsb;
    msdu_info->msdu_addr_msb = mp17c_msdu_addr->msdu_addr_msb;
}

uint32_t mp17c_host_tx_get_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
    mp17c_tx_msdu_info_stru *mp17c_msdu_info = mp17c_host_tx_ring_msdu_info(msdu_ring, index);
    mp17c_tx_msdu_addr_stru *mp17c_msdu_addr = mp17c_host_tx_ring_msdu_addr(msdu_ring, index);

    if (oal_any_null_ptr3(mp17c_msdu_info, mp17c_msdu_addr, msdu_info)) {
        return OAL_FAIL;
    }

    mp17c_host_tx_get_msdu_info(mp17c_msdu_info, msdu_info);
    mp17c_host_tx_get_msdu_addr(mp17c_msdu_addr, msdu_info);

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_reset_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    mp17c_tx_msdu_info_stru *mp17c_msdu_info = mp17c_host_tx_ring_msdu_info(msdu_ring, index);
    mp17c_tx_msdu_addr_stru *mp17c_msdu_addr = mp17c_host_tx_ring_msdu_addr(msdu_ring, index);

    if (oal_any_null_ptr2(mp17c_msdu_info, mp17c_msdu_addr)) {
        return OAL_FAIL;
    }

    mp17c_host_tx_reset_msdu_info(mp17c_msdu_info);
    mp17c_host_tx_reset_msdu_addr(mp17c_msdu_addr);

    return OAL_SUCC;
}

/* 临时接口, mp17c tx ring table & 常发开发完后删除 */
uint32_t mp17c_host_tx_get_msdu_info_devva(hal_host_tx_msdu_ring_stru *msdu_ring,
    hal_tx_ring_table_addr_stru *tx_ring_addr)
{
    if (tx_ring_addr == NULL) {
        oam_error_log0(0, 0, "{mp17c_host_tx_get_msdu_info_devva::ring_devva is NULL");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }
    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)msdu_ring->msdu_info_ring.dma_addr,
        (uint64_t *)(&tx_ring_addr->info_devva)) != OAL_SUCC ||
        pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)msdu_ring->msdu_addr_ring.dma_addr,
        (uint64_t *)(&tx_ring_addr->addr_devva)) != OAL_SUCC) {
        oam_error_log0(0, 0, "{mp17c_host_tx_get_msdu_info_devva::hostva to devva failed");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    return OAL_SUCC;
}

typedef struct {
    uint32_t base_addr; /* Device Tx Ring Table基地址 */
    mp17c_tx_msdu_ring_table_stru tx_ring_device_info[WLAN_ASSOC_USER_MAX_NUM][WLAN_TIDNO_BUTT];
} mp17c_host_tx_ring_table_mgmt_stru;

mp17c_host_tx_ring_table_mgmt_stru g_mp17c_host_tx_ring_table_mgmt = { 0 };

void mp17c_host_tx_set_ring_table_base_addr(uint32_t addr)
{
    g_mp17c_host_tx_ring_table_mgmt.base_addr = addr;
}

static inline uint32_t mp17c_host_tx_get_tid_ring_table_addr(uint8_t lut_idx, uint8_t tid)
{
    return g_mp17c_host_tx_ring_table_mgmt.base_addr + MP17C_TX_RING_INFO_LEN * (lut_idx * WLAN_TIDNO_BUTT + tid);
}

mp17c_tx_msdu_ring_table_stru *mp17c_host_tx_get_tid_ring_table_mgmt(uint8_t lut_idx, uint8_t tid)
{
    return &g_mp17c_host_tx_ring_table_mgmt.tx_ring_device_info[lut_idx][tid];
}

static inline uint32_t mp17c_host_tx_get_tid_ring_table_hostva(uint8_t lut_idx, uint8_t tid,
    hal_tx_ring_table_addr_val_stru *ring_tb_addr)
{
    return oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV,
        mp17c_host_tx_get_tid_ring_table_addr(lut_idx, tid), &ring_tb_addr->addr_val);
}

static inline void mp17c_host_tx_update_ring_table(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t offset)
{
    hal_host_writel(tx_ring_mgmt->words[offset], tx_ring_table_addr + offset * sizeof(uint32_t));
}

uint32_t mp17c_host_tx_init_tid_ring_table(uint8_t lut_idx, uint8_t tid,
    hal_tx_ring_table_addr_val_stru *ring_tb_addr)
{
    uint8_t i;
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt = mp17c_host_tx_get_tid_ring_table_mgmt(lut_idx, tid);

    if (mp17c_host_tx_get_tid_ring_table_hostva(lut_idx, tid, ring_tb_addr) != OAL_SUCC) {
        return OAL_FAIL;
    }

    memset_s(tx_ring_mgmt, sizeof(mp17c_tx_msdu_ring_table_stru), 0, sizeof(mp17c_tx_msdu_ring_table_stru));

    for (i = 0; i < MP17C_TX_RING_INFO_WORD_NUM; i++) {
        mp17c_host_tx_update_ring_table(tx_ring_mgmt, ring_tb_addr->addr_val, i);
    }
    return OAL_SUCC;
}

#define MP17C_RING_TABLE_MSDU_INFO_RING_LSB 0
static inline void mp17c_host_tx_update_msdu_info_ring_lsb(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_base_lsb = val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_MSDU_INFO_RING_LSB);
}

#define MP17C_RING_TABLE_MSDU_INFO_RING_MSB 1
static inline void mp17c_host_tx_update_msdu_info_ring_msb(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_base_msb = val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_MSDU_INFO_RING_MSB);
}

#define MP17C_RING_TABLE_MSDU_ADDR_RING_LSB 2
static inline void mp17c_host_tx_update_msdu_addr_ring_lsb(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_addr_ring_base_lsb = val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_MSDU_ADDR_RING_LSB);
}

#define MP17C_RING_TABLE_MSDU_ADDR_RING_MSB 3
static inline void mp17c_host_tx_update_msdu_addr_ring_msb(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_addr_ring_base_msb = val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_MSDU_ADDR_RING_MSB);
}

#define MP17C_RING_TABLE_WPTR_OFFSET 4
uint32_t mp17c_host_tx_get_hw_tx_ring_word4(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr);
static inline void mp17c_host_tx_update_wptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    volatile uint16_t wrptr = (uint16_t)val;
    if (mp17c_host_tx_get_hw_tx_ring_word4(tx_ring_mgmt, tx_ring_table_addr) != OAL_SUCC) {
        return;
    }

    tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr = (uint16_t)val;

    /* 由于读指针和其他信息（link bitmap）在同一个word，在频繁读写写指针时会读写不同步导致其他比特错误，改为16位只写写指针 */
    hal_host_writew(wrptr, (uintptr_t)(tx_ring_table_addr + MP17C_RING_TABLE_WPTR_OFFSET * sizeof(uint32_t)));
}
#define MP17C_RING_TABLE_RING_DEPTH_OFFSET 4
static inline void mp17c_host_tx_update_ring_depth(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    if (mp17c_host_tx_get_hw_tx_ring_word4(tx_ring_mgmt, tx_ring_table_addr) != OAL_SUCC) {
        return;
    }
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_depth = (val & 0xf);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_RING_DEPTH_OFFSET);
}

#define MP17C_RING_TABLE_MAX_AGGR_AMSDU_OFFSET 4
static inline void mp17c_host_tx_update_max_aggr_amsdu_num(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    if (mp17c_host_tx_get_hw_tx_ring_word4(tx_ring_mgmt, tx_ring_table_addr) != OAL_SUCC) {
        return;
    }
    tx_ring_mgmt->ring_info.max_aggr_amsdu_num = (val & 0xf);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_MAX_AGGR_AMSDU_OFFSET);
}

#define MP17C_RING_TABLE_ML0_RPTR_OFFSET 7
uint32_t mp17c_host_tx_get_hw_ml0_rptr_seq_num(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val);
static inline void mp17c_host_tx_update_ml0_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    uint32_t seq_num;
    if (mp17c_host_tx_get_hw_ml0_rptr_seq_num(tx_ring_mgmt, tx_ring_table_addr, &seq_num) != OAL_SUCC) {
        return;
    }
    tx_ring_mgmt->ring_info.ml0_tx_msdu_info_ring_rptr = (uint16_t)val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML0_RPTR_OFFSET);
}

#define MP17C_RING_TABLE_ML1_RPTR_OFFSET 8
uint32_t mp17c_host_tx_get_hw_ml1_rptr_seq_num(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val);
static inline void mp17c_host_tx_update_ml1_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    uint32_t seq_num;
    if (mp17c_host_tx_get_hw_ml1_rptr_seq_num(tx_ring_mgmt, tx_ring_table_addr, &seq_num) != OAL_SUCC) {
        return;
    }
    tx_ring_mgmt->ring_info.ml1_tx_msdu_info_ring_rptr = (uint16_t)val;
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML1_RPTR_OFFSET);
}

#define MP17C_RING_TABLE_ML_MODE_OFFSET 4
static inline void mp17c_host_tx_update_ml_mode(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.ml_mode = (val & 0x1);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML_MODE_OFFSET);
}

static inline void mp17c_host_tx_update_ml0_dup_en(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.ml0_dup_en = (val & 0x1);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML_MODE_OFFSET);
}

static inline void mp17c_host_tx_update_ml1_dup_en(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.ml1_dup_en = (val & 0x1);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML_MODE_OFFSET);
}

static inline void mp17c_host_tx_update_ml_bitmap(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.multi_link_bitmap = (val & 0xf);
    mp17c_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP17C_RING_TABLE_ML_MODE_OFFSET);
}

static inline void mp17c_host_tx_update_sw_wptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr = (uint16_t)val;
}

static inline void mp17c_host_tx_update_sw_ml0_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.ml0_tx_msdu_info_ring_rptr = (uint16_t)val;
}

static inline void mp17c_host_tx_update_sw_ml1_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.ml1_tx_msdu_info_ring_rptr = (uint16_t)val;
}

typedef void (*mp17c_host_tx_ring_update_ops_func)(mp17c_tx_msdu_ring_table_stru *, uint64_t, uint32_t);
static const mp17c_host_tx_ring_update_ops_func g_mp17c_host_tx_ring_update_ops_table[HAL_HOST_TX_UPDATE_OPS_BUTT] = {
    mp17c_host_tx_update_msdu_info_ring_lsb,
    mp17c_host_tx_update_msdu_info_ring_msb,
    mp17c_host_tx_update_msdu_addr_ring_lsb,
    mp17c_host_tx_update_msdu_addr_ring_msb,
    mp17c_host_tx_update_wptr,
    mp17c_host_tx_update_ml0_rptr,
    mp17c_host_tx_update_ml1_rptr,
    mp17c_host_tx_update_ring_depth,
    mp17c_host_tx_update_max_aggr_amsdu_num,
    mp17c_host_tx_update_sw_wptr,
    mp17c_host_tx_update_sw_ml0_rptr,
    mp17c_host_tx_update_sw_ml1_rptr,
    mp17c_host_tx_update_ml_mode,
    mp17c_host_tx_update_ml0_dup_en,
    mp17c_host_tx_update_ml1_dup_en,
    mp17c_host_tx_update_ml_bitmap,
};

uint32_t mp17c_host_tx_ring_update_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt = NULL;

    if (oal_unlikely(type >= HAL_HOST_TX_UPDATE_OPS_BUTT) || !g_mp17c_host_tx_ring_update_ops_table[type]) {
        return OAL_FAIL;
    }

    tx_ring_mgmt = mp17c_host_tx_get_tid_ring_table_mgmt(param->lut_idx, param->tid);
    g_mp17c_host_tx_ring_update_ops_table[type](tx_ring_mgmt, param->tx_ring_table_addr, param->val);

    return OAL_SUCC;
}

static inline uint32_t mp17c_host_tx_get_ring_table(uint64_t ring_table_addr, uint32_t offset)
{
    return hal_host_readl(ring_table_addr + offset * sizeof(uint32_t));
}

uint32_t mp17c_host_tx_get_hw_ml0_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    volatile uint32_t rptr = mp17c_host_tx_get_ring_table(ring_table_addr, MP17C_RING_TABLE_ML0_RPTR_OFFSET);
    if (oal_unlikely(rptr == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }

    tx_ring_mgmt->ring_info.ml0_tx_msdu_info_ring_rptr = (uint16_t)rptr;
    *val = (uint16_t)rptr;

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_get_hw_ml1_rptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    volatile uint32_t rptr = mp17c_host_tx_get_ring_table(ring_table_addr, MP17C_RING_TABLE_ML1_RPTR_OFFSET);
    if (oal_unlikely(rptr == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }

    tx_ring_mgmt->ring_info.ml1_tx_msdu_info_ring_rptr = (uint16_t)rptr;
    *val = (uint16_t)rptr;

    return OAL_SUCC;
}

static uint32_t mp17c_host_tx_get_sw_wptr(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    *val = tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr;

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_get_hw_ml0_rptr_seq_num(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    volatile uint32_t seq_num = mp17c_host_tx_get_ring_table(ring_table_addr, MP17C_RING_TABLE_ML0_RPTR_OFFSET);
    if (oal_unlikely(seq_num == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }
    seq_num = ((seq_num >> NUM_16_BITS) & 0xFFF);
    tx_ring_mgmt->ring_info.ml0_rptr_seq_num = (uint16_t)seq_num;
    *val = (uint16_t)seq_num;

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_get_hw_ml1_rptr_seq_num(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    volatile uint32_t seq_num = mp17c_host_tx_get_ring_table(ring_table_addr, MP17C_RING_TABLE_ML1_RPTR_OFFSET);
    if (oal_unlikely(seq_num == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }
    seq_num = ((seq_num >> NUM_16_BITS) & 0xFFF);
    tx_ring_mgmt->ring_info.ml1_rptr_seq_num = (uint16_t)seq_num;
    *val = (uint16_t)seq_num;

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_get_hw_multi_link_bitmap(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr, uint32_t *val)
{
    volatile uint32_t bitmap = mp17c_host_tx_get_ring_table(ring_table_addr, MP17C_RING_TABLE_MAX_AGGR_AMSDU_OFFSET);
    if (oal_unlikely(bitmap == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }
    bitmap = ((bitmap >> NUM_28_BITS) & 0xF);
    tx_ring_mgmt->ring_info.multi_link_bitmap = (uint8_t)bitmap;
    *val = (uint8_t)bitmap;

    return OAL_SUCC;
}

uint32_t mp17c_host_tx_get_hw_tx_ring_word4(
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t ring_table_addr)
{
    volatile uint32_t word4 = mp17c_host_tx_get_ring_table(ring_table_addr, BIT_OFFSET_4);
    if (oal_unlikely(word4 == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }
    tx_ring_mgmt->words[BIT_OFFSET_4] = word4;

    return OAL_SUCC;
}

typedef uint32_t (*mp17c_host_tx_ring_get_ops_func)(mp17c_tx_msdu_ring_table_stru *, uint64_t, uint32_t *);
static const mp17c_host_tx_ring_get_ops_func g_mp17c_host_tx_ring_get_ops_table[HAL_HOST_TX_GET_OPS_BUTT] = {
    mp17c_host_tx_get_hw_ml0_rptr,
    mp17c_host_tx_get_hw_ml1_rptr,
    mp17c_host_tx_get_sw_wptr,
    NULL,
    mp17c_host_tx_get_hw_ml0_rptr_seq_num,
    mp17c_host_tx_get_hw_ml1_rptr_seq_num,
    mp17c_host_tx_get_hw_multi_link_bitmap,
};

uint32_t mp17c_host_tx_ring_get_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    mp17c_tx_msdu_ring_table_stru *tx_ring_mgmt = NULL;

    if (oal_unlikely(type >= HAL_HOST_TX_GET_OPS_BUTT)) {
        return OAL_FAIL;
    }

    tx_ring_mgmt = mp17c_host_tx_get_tid_ring_table_mgmt(param->lut_idx, param->tid);
    if (g_mp17c_host_tx_ring_get_ops_table[type] == NULL) {
        return OAL_FAIL;
    }
    return g_mp17c_host_tx_ring_get_ops_table[type](tx_ring_mgmt, param->tx_ring_table_addr, &param->val);
}

uint16_t mp17c_host_get_tx_ring_lut_idx(uint16_t lut_idx, uint16_t mld_user_lut_idx)
{
    return mld_user_lut_idx;
}
