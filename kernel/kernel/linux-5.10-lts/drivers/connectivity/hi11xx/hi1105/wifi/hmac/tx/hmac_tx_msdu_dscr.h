/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : hmac_tx_msdu_dscr.c 的头文件
 * 作者       : wifi
 * 创建日期   : 2020年02月12日
 */

#ifndef HMAC_TX_MSDU_DSCR_H
#define HMAC_TX_MSDU_DSCR_H

/* 1 其他头文件包含 */
#include "hmac_tx_data.h"
#include "host_hal_ext_if.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DRIVER_HMAC_TX_MSDU_DSCR_H
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static inline uint32_t get_low_32_bits(uint64_t value)
{
    return ((uint32_t)(((uint64_t)(value)) & 0x00000000FFFFFFFFUL));
}
static inline uint32_t get_high_32_bits(uint64_t value)
{
    return ((uint32_t)((((uint64_t)(value)) & 0xFFFFFFFF00000000UL) >> 32UL));
}
#define HOST_AL_TX_FLAG   0x5a

static inline uint8_t hmac_tid_is_host_ring_tx(hmac_tid_info_stru *tid_info)
{
    uint8_t ring_tx_mode = (uint8_t)oal_atomic_read(&tid_info->ring_tx_mode);

    return ring_tx_mode == HOST_RING_TX_MODE || ring_tx_mode == H2D_SWITCHING_MODE;
}
/*
 * 功能描述 : 获取ring状态, 若ring不在inited状态, 说明ring已去初始化/初始化未完成, 不允许发帧
 * 1.日    期 : 2020年6月1日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_get_tx_ring_inited(hmac_user_stru *hmac_user, uint8_t tid)
{
    hmac_tid_info_stru *tid_info = hmac_get_tx_tid_info(hmac_user, tid);
    if (tid_info == NULL) {
        return OAL_FALSE;
    }
    return oal_atomic_read(&tid_info->tx_ring.inited);
}

/*
 * 功能描述 : 判断ring是否已满
 * 1.日    期 : 2020年6月1日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tid_ring_full(hmac_msdu_info_ring_stru *tx_msdu_ring)
{
    un_rw_ptr write_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.write_index };
    un_rw_ptr release_ptr = { .rw_ptr = tx_msdu_ring->release_index };

    if (!is_rw_ring_full(&release_ptr, &write_ptr)) {
        return OAL_FALSE;
    }

    tx_msdu_ring->push_ring_full_cnt++;
    return OAL_TRUE;
}

static inline uint8_t hmac_tid_ring_empty(hmac_msdu_info_ring_stru *tx_msdu_ring)
{
    return tx_msdu_ring->base_ring_info.write_index == tx_msdu_ring->base_ring_info.read_index;
}

/* ptr1 - ptr2 */
static inline uint16_t hmac_tid_ring_ptr_diff(un_rw_ptr *ptr1, un_rw_ptr *ptr2, uint16_t ring_size)
{
    return ptr1->st_rw_ptr.bit_wrap_flag == ptr2->st_rw_ptr.bit_wrap_flag ?
           ptr1->st_rw_ptr.bit_rw_ptr - ptr2->st_rw_ptr.bit_rw_ptr :
           ptr1->st_rw_ptr.bit_rw_ptr + ring_size - ptr2->st_rw_ptr.bit_rw_ptr;
}

/*
 * 功能描述 : 检查read指针是否在release和write指针之间
 * 1.日    期 : 2020年2月18日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 * 2.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 重构函数
 */
OAL_STATIC OAL_INLINE oal_bool_enum_uint8 hmac_tx_ring_rw_ptr_check(hmac_msdu_info_ring_stru *tx_msdu_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_msdu_ring->release_index };
    un_rw_ptr read_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.read_index };
    un_rw_ptr write_ptr = { .rw_ptr = tx_msdu_ring->base_ring_info.write_index };
    ring_ptr_compare_enum read_release_compare = hal_tx_rw_ptr_compare(read_ptr, release_ptr);
    ring_ptr_compare_enum write_read_compare = hal_tx_rw_ptr_compare(write_ptr, read_ptr);

    return (read_release_compare == RING_PTR_EQUAL || read_release_compare == RING_PTR_GREATER) &&
           (write_read_compare == RING_PTR_EQUAL || write_read_compare == RING_PTR_GREATER);
}

OAL_STATIC OAL_INLINE oal_netbuf_stru *hmac_tx_ring_get_netbuf(
    hmac_msdu_info_ring_stru *tx_ring, un_rw_ptr netbuf_ptr)
{
    uint16_t netbuf_index = netbuf_ptr.st_rw_ptr.bit_rw_ptr;

    if (tx_ring->netbuf_list == NULL) {
        oam_error_log0(0, OAM_SF_TX, "{hmac_tx_ring_get_netbuf::netbuf list is NULL!}");
        return NULL;
    }

    if (tx_ring->netbuf_list[netbuf_index] == NULL) {
        oam_error_log1(0, OAM_SF_TX, "{hmac_tx_ring_get_netbuf::netbuf[%d] is NULL!}", netbuf_index);
    }

    return tx_ring->netbuf_list[netbuf_index];
}

/*
 * 功能描述 : 使用完msdu info后需要清空包括硬件回填字段在内的msdu info内容,
 *            否则硬件下次获取同一位置数据时可能会因为回填字段未清零而产生异常
 * 1.日    期 : 2020年5月29日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_tx_reset_msdu_info(hmac_msdu_info_ring_stru *tx_ring, un_rw_ptr reset_ptr)
{
    if (oal_unlikely(tx_ring->host_tx_flag == HOST_AL_TX_FLAG)) {
        return;
    }

    if (oal_unlikely(hal_host_tx_reset_msdu_ring(&tx_ring->ring_buf, reset_ptr.st_rw_ptr.bit_rw_ptr))) {
        oam_error_log1(0, 0, "{hmac_tx_reset_msdu_info::index[%d] msdu_info null}", reset_ptr.rw_ptr);
    }
}

/*
 * 功能描述 : ring指针++
 * 1.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_tx_rw_ptr_inc(un_rw_ptr *rw_ptr, uint16_t ring_size)
{
    oal_smp_mb();

    if (++rw_ptr->st_rw_ptr.bit_rw_ptr >= ring_size) {
        rw_ptr->st_rw_ptr.bit_rw_ptr = 0;
        rw_ptr->st_rw_ptr.bit_wrap_flag = !rw_ptr->st_rw_ptr.bit_wrap_flag;
    }
}

/*
 * 功能描述 : 更新ring的释放指针
 * 1.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_tx_msdu_ring_inc_release_ptr(hmac_msdu_info_ring_stru *tx_ring)
{
    un_rw_ptr release_ptr = { .rw_ptr = tx_ring->release_index };

    hmac_tx_rw_ptr_inc(&release_ptr, hal_host_tx_tid_ring_depth_get(tx_ring->base_ring_info.size));

    tx_ring->release_index = release_ptr.rw_ptr;
}

/* 10 函数声明 */
uint32_t hmac_tx_ring_push_msdu(mac_vap_stru *mac_vap, hmac_msdu_info_ring_stru *tx_ring, oal_netbuf_stru *netbuf);
uint32_t hmac_tx_hostva_to_devva(uint8_t *hostva, uint32_t alloc_size, uint64_t *devva);
uint8_t *hmac_tx_netbuf_init_msdu_dscr(oal_netbuf_stru *netbuf);
void hmac_tx_reg_write_ring_info(hmac_tid_info_stru *tid_info, tid_cmd_enum_uint8 cmd);
uint32_t hmac_tx_sync_ring_info(hmac_tid_info_stru *tx_tid_info, tid_cmd_enum_uint8 cmd);
void hmac_tx_ring_release_netbuf(hmac_msdu_info_ring_stru *tx_ring, oal_netbuf_stru *netbuf, uint16_t release_index);
void hmac_tx_ring_unmap_netbuf(hmac_msdu_info_ring_stru *tx_ring, oal_netbuf_stru *netbuf, uint16_t release_index);
uint32_t hmac_tx_unmap_msdu_dma_addr(hmac_msdu_info_ring_stru *tx_ring, msdu_info_stru *msdu_info, uint32_t netbuf_len);
void hmac_tx_msdu_ring_inc_write_ptr(hmac_msdu_info_ring_stru *tx_ring);
void hmac_tx_msdu_update_free_cnt(hmac_msdu_info_ring_stru *tx_ring);
uint32_t hmac_get_host_ring_state(frw_event_mem_stru *event_mem);
void hmac_tx_sched_trigger(hmac_vap_stru *hmac_vap, hmac_tid_info_stru *tid_info);
uint32_t hmac_tx_reg_read_rptr(hmac_tid_info_stru *tid_info, uint32_t *rptr);
uint32_t hmac_tx_reg_read_ml1_rptr(hmac_tid_info_stru *tid_info, uint32_t *rptr);
void hmac_tx_reg_write_ml0_dup_en(hmac_tid_info_stru *tid_info, uint8_t ml0_dup_en);
void hmac_tx_reg_write_ml1_dup_en(hmac_tid_info_stru *tid_info, uint8_t ml1_dup_en);
void hmac_tx_reg_write_multi_link_bitmap(hmac_tid_info_stru *tid_info, uint8_t multi_link_bitmap);
uint32_t hmac_tx_reg_read_ml_bitmap(hmac_tid_info_stru *tid_info, uint32_t *ml_bitmap);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
