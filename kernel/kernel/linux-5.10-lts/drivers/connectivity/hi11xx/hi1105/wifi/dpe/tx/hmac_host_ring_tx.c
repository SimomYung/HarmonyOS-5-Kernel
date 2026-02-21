/*
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 * 功能说明   : HOST ring tx 功能
 */

/* 1 其他头文件包含 */
#include "hmac_tx_msdu_dscr.h"
#include "hmac_tid_sche.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DRIVER_HMAC_HOST_RING_TX_C

/*
 * 功能描述 : Host写寄存器更新ring基地址
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_reg_write_info_ring_base_addr(hmac_tid_info_stru *tid_info, uint64_t devva)
{
    hal_host_tx_ring_ops_param_stru info_ring_lsb_param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = get_low_32_bits(devva),
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };
    hal_host_tx_ring_ops_param_stru info_ring_msb_param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = get_high_32_bits(devva),
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&info_ring_lsb_param, HAL_HOST_TX_UPDATE_MSDU_INFO_RING_LSB);
    hal_host_tx_ring_update_ops(&info_ring_msb_param, HAL_HOST_TX_UPDATE_MSDU_INFO_RING_MSB);
}

OAL_STATIC void hmac_tx_reg_write_addr_ring_base_addr(hmac_tid_info_stru *tid_info, uint64_t devva)
{
    hal_host_tx_ring_ops_param_stru addr_ring_lsb_param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = get_low_32_bits(devva),
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };
    hal_host_tx_ring_ops_param_stru addr_ring_msb_param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = get_high_32_bits(devva),
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&addr_ring_lsb_param, HAL_HOST_TX_UPDATE_MSDU_ADDR_RING_LSB);
    hal_host_tx_ring_update_ops(&addr_ring_msb_param, HAL_HOST_TX_UPDATE_MSDU_ADDR_RING_MSB);
}

OAL_STATIC void hmac_tx_reg_write_base_addr(hmac_tid_info_stru *tid_info,
    uint64_t info_ring_devva, uint64_t addr_ring_devva)
{
    hmac_tx_reg_write_info_ring_base_addr(tid_info, info_ring_devva);
    hmac_tx_reg_write_addr_ring_base_addr(tid_info, addr_ring_devva);
}
/*
 * 功能描述 : Host写寄存器更新ring写指针
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_reg_write_wptr(hmac_tid_info_stru *tid_info, uint16_t wptr)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = wptr,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_WPTR);
}

/*
 * 功能描述 : Host写寄存器更新ring读指针, 主要用于init/deinit时的清零
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_reg_write_rptr(hmac_tid_info_stru *tid_info, uint16_t rptr)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = rptr,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_RPTR);
}

OAL_STATIC void hmac_tx_reg_write_ml1_rptr(hmac_tid_info_stru *tid_info, uint16_t rptr)
{
#ifdef _PRE_WLAN_FEATURE_ML
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = rptr,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_ML1_RPTR);
#endif
}

/*
 * 功能描述 : Host写寄存器更新ring大小
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_reg_write_ring_depth(hmac_tid_info_stru *tid_info, uint16_t depth)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = depth,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_RING_DEPTH);
}

/*
 * 功能描述 : Host写寄存器更新ring最大amsdu聚合数
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_tx_reg_write_max_amsdu_num(hmac_tid_info_stru *tid_info, uint16_t max_amsdu_num)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = max_amsdu_num,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_MAX_AGGR_AMSDU_NUM);
}
#ifdef _PRE_WLAN_FEATURE_ML
OAL_STATIC void hmac_tx_reg_write_ml_mode(hmac_tid_info_stru *tid_info, uint8_t ml_mode)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = ml_mode,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_ML_MODE);
}

OAL_STATIC void hmac_tx_reg_write_ml0_dup_en(hmac_tid_info_stru *tid_info, uint8_t ml0_dup_en)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = ml0_dup_en,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_ML0_DUP_EN);
}

OAL_STATIC void hmac_tx_reg_write_ml1_dup_en(hmac_tid_info_stru *tid_info, uint8_t ml1_dup_en)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = ml1_dup_en,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_ML1_DUP_EN);
}
#endif

OAL_STATIC void hmac_tx_reg_write_multi_link_bitmap(hmac_tid_info_stru *tid_info, uint8_t multi_link_bitmap)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .val = multi_link_bitmap,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_update_ops(&param, HAL_HOST_TX_UPDATE_ML_BITMAP);
}

OAL_STATIC void hmac_tx_reg_write_ml_creat(hmac_tid_info_stru *tid_info, hmac_msdu_info_ring_stru *tx_ring,
    uint8_t d2h_switch_proc)
{
#ifdef _PRE_WLAN_FEATURE_ML
    hmac_tx_reg_write_ml_mode(tid_info, tx_ring->ml_ring_info.ml_mode);
    hmac_tx_reg_write_ml0_dup_en(tid_info, tx_ring->ml_ring_info.ml0_dup_en);
    hmac_tx_reg_write_ml1_dup_en(tid_info, tx_ring->ml_ring_info.ml1_dup_en);
#endif
    if (d2h_switch_proc == OAL_FALSE) {
        hmac_tx_reg_write_multi_link_bitmap(tid_info, tx_ring->ml_ring_info.multi_link_bitmap);
    }
}

OAL_STATIC void hmac_tx_reg_write_ml_reset(hmac_tid_info_stru *tid_info)
{
#ifdef _PRE_WLAN_FEATURE_ML
    hmac_tx_reg_write_ml_mode(tid_info, 0);
    hmac_tx_reg_write_ml0_dup_en(tid_info, 0);
    hmac_tx_reg_write_ml1_dup_en(tid_info, 0);
#endif
    hmac_tx_reg_write_multi_link_bitmap(tid_info, 0);
}

/*
 * 功能描述 : 创建ring时写寄存器更新device ring信息
 * 1.日    期 : 2020年6月1日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_reg_write_create_ring_info(hmac_tid_info_stru *tid_info, uint8_t d2h_switch_proc)
{
    hal_tx_ring_table_addr_stru tx_ring_addr = {0};
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;

    if (hal_host_tx_get_msdu_info_devva(&tx_ring->ring_buf, &tx_ring_addr) != OAL_SUCC) {
        oam_error_log0(0, 0, "{hmac_reg_write_create_ring_info::hostca to devva failed}");
        return;
    }
    oam_warning_log4(0, OAM_SF_TX, "{hmac_reg_write_create_ring_info::tid[%d] release[%d] rptr[%d], wptr[%d]}",
        tid_info->tid_no, tx_ring->release_index, tx_ring->base_ring_info.read_index,
        tx_ring->base_ring_info.write_index);

    tid_info->tx_ring.release_index = tx_ring->base_ring_info.read_index;

    hmac_tx_reg_write_ring_depth(tid_info, tx_ring->base_ring_info.size);
    hmac_tx_reg_write_max_amsdu_num(tid_info, tx_ring->base_ring_info.max_amsdu_num);
    hmac_tx_reg_write_rptr(tid_info, tx_ring->base_ring_info.read_index);
    hmac_tx_reg_write_ml1_rptr(tid_info, tx_ring->base_ring_info.read_index);
    hmac_tx_reg_write_wptr(tid_info, tx_ring->base_ring_info.write_index);
    hmac_tx_reg_write_ml_creat(tid_info, tx_ring, d2h_switch_proc);
    /* info_ring和addr_ring的地址设置放在最后 */
    hmac_tx_reg_write_base_addr(tid_info, tx_ring_addr.info_devva, tx_ring_addr.addr_devva);
}

/*
 * 功能描述 : 删除ring时写寄存器更新device ring信息
 * 1.日    期 : 2020年6月1日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC void hmac_reg_write_del_ring_info(hmac_tid_info_stru *tid_info)
{
    hmac_tx_reg_write_base_addr(tid_info, 0, 0);
    hmac_tx_reg_write_ring_depth(tid_info, 0);
    hmac_tx_reg_write_max_amsdu_num(tid_info, 0);
    hmac_tx_reg_write_rptr(tid_info, 0);
    hmac_tx_reg_write_ml1_rptr(tid_info, 0);
    hmac_tx_reg_write_wptr(tid_info, 0);
    hmac_tx_reg_write_ml_reset(tid_info);
}

OAL_STATIC uint16_t hmac_tx_reg_read_sw_wptr(hmac_tid_info_stru *tid_info)
{
    hal_host_tx_ring_ops_param_stru param = {
        .tx_ring_table_addr = tid_info->tx_ring.tx_ring_table_addr.addr_val,
        .lut_idx = tid_info->lut_index,
        .tid = tid_info->tid_no,
    };

    hal_host_tx_ring_get_ops(&param, HAL_HOST_TX_GET_SW_WPTR);

    return (uint16_t)param.val;
}

/*
 * 功能描述 : msdu入ring时写寄存器更新device ring信息
 * 1.日    期 : 2020年6月1日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE void hmac_reg_write_enque_ring_info(hmac_tid_info_stru *tid_info)
{
    hmac_msdu_info_ring_stru *tx_ring = &tid_info->tx_ring;
    un_rw_ptr write_origin = { .rw_ptr = hmac_tx_reg_read_sw_wptr(tid_info) };
    un_rw_ptr write_new = { .rw_ptr = tx_ring->base_ring_info.write_index };

    if (hal_tx_rw_ptr_compare(write_new, write_origin) != RING_PTR_GREATER) {
        return;
    }

    hmac_tx_reg_write_wptr(tid_info, tx_ring->base_ring_info.write_index);
}

/*
 * 功能描述 : Host写寄存器更新ring信息
 * 1.日    期 : 2020年5月7日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
void hmac_tx_reg_write_ring_info(hmac_tid_info_stru *tid_info, tid_cmd_enum_uint8 cmd, uint8_t d2h_switch_proc)
{
    if (cmd == TID_CMDID_ENQUE) {
        hmac_reg_write_enque_ring_info(tid_info);
    } else if (cmd == TID_CMDID_CREATE) {
        hmac_reg_write_create_ring_info(tid_info, d2h_switch_proc);
    } else if (cmd == TID_CMDID_DEL) {
        hmac_reg_write_del_ring_info(tid_info);
    } else if (cmd == TID_CMDID_STOP_TX) {
        hmac_tx_reg_write_base_addr(tid_info, 0, 0);
    }
}
