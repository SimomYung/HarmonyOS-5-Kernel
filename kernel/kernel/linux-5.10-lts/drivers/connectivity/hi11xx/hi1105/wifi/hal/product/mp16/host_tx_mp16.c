/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host侧Shenkuo芯片相关TX功能
 * 作    者 : wifi
 * 创建日期 : 2021年3月1日
 */

#include "host_tx_mp16.h"
#include "host_dscr_mp16.h"
#include "host_hal_access_mgmt.h"
#include "pcie_host.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HOST_HAL_TX_MP16_C

#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
uint32_t mp16_host_tx_clear_msdu_padding(oal_netbuf_stru *netbuf)
{
    uint32_t msdu_len = oal_netbuf_len(netbuf);
    uint8_t padding_len = (HAL_TX_MSDU_MAX_PADDING_LEN - (msdu_len & 0x7)) & 0x7;

    if (oal_unlikely(padding_len > oal_netbuf_tailroom(netbuf) &&
        pskb_expand_head(netbuf, 0, padding_len, GFP_ATOMIC))) {
        oam_error_log3(0, 0, "{host_tx_clear_msdu_padding::clear failed! msdu[%d] padding[%d] tailroom[%d]}",
            msdu_len, padding_len, oal_netbuf_tailroom(netbuf));
        return OAL_FAIL;
    }

    return memset_s(oal_netbuf_data(netbuf) + msdu_len, padding_len, 0, padding_len) == EOK ? OAL_SUCC : OAL_FAIL;
}
#else
uint32_t mp16_host_tx_clear_msdu_padding(oal_netbuf_stru *netbuf)
{
    return OAL_SUCC;
}
#endif

#define MP16_DEVICE_LARGE_TX_RING_NUM 21
#define MP16_DEVICE_SMALL_TX_RING_NUM 0
#define MP16_DEVICE_TX_RING_TOTAL_NUM (MP16_DEVICE_LARGE_TX_RING_NUM + MP16_DEVICE_SMALL_TX_RING_NUM)
uint8_t mp16_host_get_device_tx_ring_num(void)
{
    return MP16_DEVICE_TX_RING_TOTAL_NUM;
}

#define MP16_MSDU_INFO_LEN 12
uint32_t mp16_host_alloc_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint32_t size)
{
    return hal_host_tx_dma_alloc_ring_buf(&msdu_ring->msdu_info_ring, size * MP16_MSDU_INFO_LEN);
}

uint32_t mp16_host_release_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    return hal_host_tx_dma_release_ring_buf(&msdu_ring->msdu_info_ring);
}

uint8_t mp16_host_tx_ring_alloced(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    return msdu_ring->msdu_info_ring.ring_buf != NULL;
}

static mp16_tx_msdu_info_stru *mp16_host_tx_ring_msdu_info(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    return msdu_ring->msdu_info_ring.ring_buf ?
           (mp16_tx_msdu_info_stru *)(msdu_ring->msdu_info_ring.ring_buf + index * MP16_MSDU_INFO_LEN) : NULL;
}

#ifdef _PRE_WLAN_FEATURE_GET_MSDU_RING_PERFORMACE
static mp16_tx_msdu_info_stru *mp16_host_tx_ring_msdu_info_ext(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    return msdu_ring->msdu_info_ring.ring_ext_buf ?
           (mp16_tx_msdu_info_stru *)(msdu_ring->msdu_info_ring.ring_ext_buf + index * MP16_MSDU_INFO_LEN) : NULL;
}
#endif

static inline void mp16_host_tx_reset_msdu_info(mp16_tx_msdu_info_stru *mp16_msdu_info)
{
    memset_s(mp16_msdu_info, sizeof(mp16_tx_msdu_info_stru), 0, sizeof(mp16_tx_msdu_info_stru));
}

static void mp16_host_tx_set_msdu_info(mp16_tx_msdu_info_stru *mp16_msdu_info, msdu_info_stru *msdu_info)
{
#ifdef _PRE_WLAN_FEATURE_SINGLE_CORE_PERFORMANCE
    int32_t ret;
    mp16_tx_msdu_info_stru mp16_msdu_info_tmp;
    mp16_msdu_info_tmp.msdu_addr_lsb = msdu_info->msdu_addr_lsb;
    mp16_msdu_info_tmp.msdu_addr_msb = msdu_info->msdu_addr_msb;
    mp16_msdu_info_tmp.msdu_len = msdu_info->msdu_len;
    mp16_msdu_info_tmp.data_type = msdu_info->data_type;
    mp16_msdu_info_tmp.frag_num = msdu_info->frag_num;
    mp16_msdu_info_tmp.to_ds = msdu_info->to_ds;
    mp16_msdu_info_tmp.from_ds = msdu_info->from_ds;
    mp16_msdu_info_tmp.more_frag = msdu_info->more_frag;
    mp16_msdu_info_tmp.aggr_msdu_num = msdu_info->aggr_msdu_num;
    mp16_msdu_info_tmp.first_msdu = msdu_info->first_msdu;
    mp16_msdu_info_tmp.csum_type = msdu_info->csum_type;
    ret = memcpy_s(mp16_msdu_info, sizeof(mp16_tx_msdu_info_stru), &mp16_msdu_info_tmp, sizeof(mp16_tx_msdu_info_stru));
    if (oal_unlikely(ret != EOK)) {
        oam_error_log1(0, 0, "{mp16_host_tx_set_msdu_info::memcpy failed[%d]", ret);
    }
#else
    mp16_host_tx_reset_msdu_info(mp16_msdu_info);
    mp16_msdu_info->msdu_addr_lsb = msdu_info->msdu_addr_lsb;
    mp16_msdu_info->msdu_addr_msb = msdu_info->msdu_addr_msb;
    mp16_msdu_info->msdu_len = msdu_info->msdu_len;
    mp16_msdu_info->data_type = msdu_info->data_type;
    mp16_msdu_info->frag_num = msdu_info->frag_num;
    mp16_msdu_info->to_ds = msdu_info->to_ds;
    mp16_msdu_info->from_ds = msdu_info->from_ds;
    mp16_msdu_info->more_frag = msdu_info->more_frag;
    mp16_msdu_info->aggr_msdu_num = msdu_info->aggr_msdu_num;
    mp16_msdu_info->first_msdu = msdu_info->first_msdu;
    mp16_msdu_info->csum_type = msdu_info->csum_type;
#endif
}

uint32_t mp16_host_tx_set_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
    mp16_tx_msdu_info_stru *mp16_msdu_info = mp16_host_tx_ring_msdu_info(msdu_ring, index);
#ifdef _PRE_WLAN_FEATURE_GET_MSDU_RING_PERFORMACE
    mp16_tx_msdu_info_stru *mp16_msdu_info_ext = mp16_host_tx_ring_msdu_info_ext(msdu_ring, index);
    if (mp16_msdu_info_ext == NULL) {
        return OAL_FAIL;
    }
#endif

    if (oal_any_null_ptr2(mp16_msdu_info, msdu_info)) {
        return OAL_FAIL;
    }

    mp16_host_tx_set_msdu_info(mp16_msdu_info, msdu_info);
#ifdef _PRE_WLAN_FEATURE_GET_MSDU_RING_PERFORMACE
    mp16_host_tx_set_msdu_info(mp16_msdu_info_ext, msdu_info);
#endif
    return OAL_SUCC;
}

static void mp16_host_tx_get_msdu_info(mp16_tx_msdu_info_stru *mp16_msdu_info, msdu_info_stru *msdu_info)
{
    msdu_info->msdu_addr_lsb = mp16_msdu_info->msdu_addr_lsb;
    msdu_info->msdu_addr_msb = mp16_msdu_info->msdu_addr_msb;
    msdu_info->msdu_len = mp16_msdu_info->msdu_len;
    msdu_info->data_type = mp16_msdu_info->data_type;
    msdu_info->frag_num = mp16_msdu_info->frag_num;
    msdu_info->to_ds = mp16_msdu_info->to_ds;
    msdu_info->from_ds = mp16_msdu_info->from_ds;
    msdu_info->more_frag = mp16_msdu_info->more_frag;
    msdu_info->aggr_msdu_num = mp16_msdu_info->aggr_msdu_num;
    msdu_info->first_msdu = mp16_msdu_info->first_msdu;
    msdu_info->csum_type = mp16_msdu_info->csum_type;
}

uint32_t mp16_host_tx_get_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
#ifdef _PRE_WLAN_FEATURE_GET_MSDU_RING_PERFORMACE
    mp16_tx_msdu_info_stru *mp16_msdu_info = mp16_host_tx_ring_msdu_info_ext(msdu_ring, index);
#else
    mp16_tx_msdu_info_stru *mp16_msdu_info = mp16_host_tx_ring_msdu_info(msdu_ring, index);
#endif

    if (oal_any_null_ptr2(mp16_msdu_info, msdu_info)) {
        return OAL_FAIL;
    }

    mp16_host_tx_get_msdu_info(mp16_msdu_info, msdu_info);

    return OAL_SUCC;
}

uint32_t mp16_host_tx_reset_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    mp16_tx_msdu_info_stru *mp16_msdu_info = mp16_host_tx_ring_msdu_info(msdu_ring, index);

    if (oal_any_null_ptr1(mp16_msdu_info)) {
        return OAL_FAIL;
    }

    mp16_host_tx_reset_msdu_info(mp16_msdu_info);

    return OAL_SUCC;
}

/* 临时接口, mp16 tx ring table & 常发开发完后删除 */
uint32_t mp16_host_tx_get_msdu_info_devva(hal_host_tx_msdu_ring_stru *msdu_ring,
    uint64_t *info_ring_devva, uint64_t *addr_ring_devva)
{
    if (pcie_if_hostca_to_devva(HCC_EP_WIFI_DEV, (uint64_t)msdu_ring->msdu_info_ring.dma_addr, info_ring_devva) !=
        OAL_SUCC) {
        oam_error_log0(0, 0, "{mp16_host_tx_get_msdu_info_devva::hostva to devva failed");
        return OAL_ERR_CODE_ALLOC_MEM_FAIL;
    }

    return OAL_SUCC;
}

typedef struct {
    uint32_t base_addr; /* Device Tx Ring Table基地址 */
    mp16_tx_msdu_ring_table_stru tx_ring_device_info[WLAN_ASSOC_USER_MAX_NUM][WLAN_TIDNO_BUTT];
} mp16_host_tx_ring_table_mgmt_stru;

mp16_host_tx_ring_table_mgmt_stru g_mp16_host_tx_ring_table_mgmt = { 0 };

void mp16_host_tx_set_ring_table_base_addr(uint32_t addr)
{
    g_mp16_host_tx_ring_table_mgmt.base_addr = addr;
}

static inline uint32_t mp16_host_tx_get_tid_ring_table_addr(uint8_t lut_idx, uint8_t tid)
{
    return g_mp16_host_tx_ring_table_mgmt.base_addr + MP16_TX_RING_INFO_LEN * (lut_idx * WLAN_TIDNO_BUTT + tid);
}

static inline mp16_tx_msdu_ring_table_stru *mp16_host_tx_get_tid_ring_table_mgmt(uint8_t lut_idx, uint8_t tid)
{
    return &g_mp16_host_tx_ring_table_mgmt.tx_ring_device_info[lut_idx][tid];
}

static inline uint32_t mp16_host_tx_get_tid_ring_table_hostva(uint8_t lut_idx, uint8_t tid, uint64_t *hostva)
{
    return oal_pcie_devca_to_hostva(HCC_EP_WIFI_DEV, mp16_host_tx_get_tid_ring_table_addr(lut_idx, tid), hostva);
}

uint32_t mp16_host_tx_init_tid_ring_table(uint8_t lut_idx, uint8_t tid, uint64_t *tx_ring_table_addr)
{
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt = mp16_host_tx_get_tid_ring_table_mgmt(lut_idx, tid);

    if (mp16_host_tx_get_tid_ring_table_hostva(lut_idx, tid, tx_ring_table_addr) != OAL_SUCC) {
        return OAL_FAIL;
    }

    memset_s(tx_ring_mgmt, sizeof(mp16_tx_msdu_ring_table_stru), 0, sizeof(mp16_tx_msdu_ring_table_stru));

    return OAL_SUCC;
}

static inline void mp16_host_tx_update_ring_table(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t offset)
{
    hal_host_writel(tx_ring_mgmt->words[offset], tx_ring_table_addr + offset * sizeof(uint32_t));
}

#define MP16_RING_TABLE_MSDU_INFO_RING_LSB 0
static inline void mp16_host_tx_update_msdu_info_ring_lsb(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_base_lsb = val;
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_MSDU_INFO_RING_LSB);
}

#define MP16_RING_TABLE_MSDU_INFO_RING_MSB 1
static inline void mp16_host_tx_update_msdu_info_ring_msb(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_base_msb = val;
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_MSDU_INFO_RING_MSB);
}

#define MP16_RING_TABLE_WPTR_OFFSET 2
static inline void mp16_host_tx_update_wptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr = (uint16_t)val;
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_WPTR_OFFSET);
}

#define MP16_RING_TABLE_RING_DEPTH_OFFSET 2
static inline void mp16_host_tx_update_ring_depth(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_depth = (val & 0xf);
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_RING_DEPTH_OFFSET);
}

#define MP16_RING_TABLE_MAX_AGGR_AMSDU_OFFSET 2
static inline void mp16_host_tx_update_max_aggr_amsdu_num(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.max_aggr_amsdu_num = (val & 0xf);
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_MAX_AGGR_AMSDU_OFFSET);
}

#define MP16_RING_TABLE_RPTR_OFFSET 3
static inline void mp16_host_tx_update_rptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_rptr = (uint16_t)val;
    mp16_host_tx_update_ring_table(tx_ring_mgmt, tx_ring_table_addr, MP16_RING_TABLE_RPTR_OFFSET);
}

static inline void mp16_host_tx_update_sw_wptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr = (uint16_t)val;
}

static inline void mp16_host_tx_update_sw_rptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t val)
{
    tx_ring_mgmt->ring_info.tx_msdu_info_ring_rptr = (uint16_t)val;
}

typedef void (*mp16_host_tx_ring_update_ops_func)(mp16_tx_msdu_ring_table_stru *, uint64_t, uint32_t);
static const mp16_host_tx_ring_update_ops_func g_mp16_host_tx_ring_update_ops_table[HAL_HOST_TX_UPDATE_OPS_BUTT] = {
    mp16_host_tx_update_msdu_info_ring_lsb,
    mp16_host_tx_update_msdu_info_ring_msb,
    NULL,
    NULL,
    mp16_host_tx_update_wptr,
    mp16_host_tx_update_rptr,
    NULL,
    mp16_host_tx_update_ring_depth,
    mp16_host_tx_update_max_aggr_amsdu_num,
    mp16_host_tx_update_sw_wptr,
    mp16_host_tx_update_sw_rptr,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

uint32_t mp16_host_tx_ring_update_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt = NULL;

    if (oal_unlikely(type >= HAL_HOST_TX_UPDATE_OPS_BUTT) || !g_mp16_host_tx_ring_update_ops_table[type]) {
        return OAL_FAIL;
    }

    tx_ring_mgmt = mp16_host_tx_get_tid_ring_table_mgmt(param->lut_idx, param->tid);
    g_mp16_host_tx_ring_update_ops_table[type](tx_ring_mgmt, param->tx_ring_table_addr, param->val);

    return OAL_SUCC;
}

static inline uint32_t mp16_host_tx_get_ring_table(uint64_t tx_ring_table_addr, uint32_t offset)
{
    return hal_host_readl(tx_ring_table_addr + offset * sizeof(uint32_t));
}

static uint32_t mp16_host_tx_get_hw_rptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t *val)
{
    uint32_t rptr = mp16_host_tx_get_ring_table(tx_ring_table_addr, MP16_RING_TABLE_RPTR_OFFSET);

    if (oal_unlikely(rptr == HAL_HOST_READL_INVALID_VAL)) {
        return OAL_FAIL;
    }

    tx_ring_mgmt->ring_info.tx_msdu_info_ring_rptr = (uint16_t)rptr;
    *val = (uint16_t)rptr;

    return OAL_SUCC;
}

static uint32_t mp16_host_tx_get_sw_wptr(
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt, uint64_t tx_ring_table_addr, uint32_t *val)
{
    *val = tx_ring_mgmt->ring_info.tx_msdu_info_ring_wptr;

    return OAL_SUCC;
}

typedef uint32_t (*mp16_host_tx_ring_get_ops_func)(mp16_tx_msdu_ring_table_stru *, uint64_t, uint32_t *);
static const mp16_host_tx_ring_get_ops_func g_mp16_host_tx_ring_get_ops_table[HAL_HOST_TX_GET_OPS_BUTT] = {
    mp16_host_tx_get_hw_rptr,
    NULL,
    mp16_host_tx_get_sw_wptr,
    NULL,
    NULL,
    NULL,
};

uint32_t mp16_host_tx_ring_get_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    mp16_tx_msdu_ring_table_stru *tx_ring_mgmt = NULL;

    if (oal_unlikely(type >= HAL_HOST_TX_GET_OPS_BUTT)) {
        return OAL_FAIL;
    }

    tx_ring_mgmt = mp16_host_tx_get_tid_ring_table_mgmt(param->lut_idx, param->tid);
    return g_mp16_host_tx_ring_get_ops_table[type](tx_ring_mgmt, param->tx_ring_table_addr, &param->val);
}

uint16_t mp16_host_get_tx_ring_lut_idx(uint16_t lut_idx, uint16_t mld_user_lut_idx)
{
    return lut_idx;
}
