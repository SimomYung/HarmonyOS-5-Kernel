/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host hal 功能接口
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */

#ifndef HOST_HAL_EXT_IF_H
#define HOST_HAL_EXT_IF_H

#include "oal_ext_if.h"
#include "wlan_types.h"
#include "mac_common.h"
#include "host_hal_ring.h"
#include "host_hal_ops.h"
#include "host_hal_dscr.h"
#include "host_hal_access_mgmt.h"
#include "host_hal_irq.h"
#include "host_hal_mac.h"

extern const struct hal_common_ops_stru *g_hal_common_ops;

int32_t hal_main_init(void);

static inline uint32_t hal_get_mac_reg_offset(uint8_t device_id, uint32_t reg_addr)
{
    if (g_hal_common_ops->get_mac_reg_offset != NULL) {
        return g_hal_common_ops->get_mac_reg_offset(device_id, reg_addr);
    }

    return 0;
}
static inline hal_ring_mac_regs_info* hal_get_ring_mac_regs_tbl(void)
{
    if (g_hal_common_ops->get_ring_mac_regs != NULL) {
        return g_hal_common_ops->get_ring_mac_regs();
    }

    return NULL;
}
static inline uint32_t hal_host_regs_addr_init(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_regs_addr_init != NULL) {
        return g_hal_common_ops->host_regs_addr_init(hal_device);
    }

    return OAL_SUCC;
}

static inline hal_host_device_stru *hal_host_rx_get_mpdu_link(hal_host_device_stru *hal_device, oal_netbuf_stru *mpdu)
{
    if (g_hal_common_ops->host_rx_get_mpdu_link != NULL) {
        return g_hal_common_ops->host_rx_get_mpdu_link(mpdu);
    }

    return hal_device;
}

static inline void hal_host_chip_mac_ptr_rpt_init(void)
{
    if (g_hal_common_ops->host_chip_mac_ptr_rpt_init != NULL) {
        g_hal_common_ops->host_chip_mac_ptr_rpt_init();
    }
}
static inline void hal_host_set_mac_ptr_rpt_regs(uintptr_t ring_ptr_rpt_mode)
{
    if (g_hal_common_ops->host_set_mac_ptr_rpt_regs != NULL) {
        g_hal_common_ops->host_set_mac_ptr_rpt_regs(ring_ptr_rpt_mode);
    }
}
static inline void hal_host_chip_irq_init(void)
{
    if (g_hal_common_ops->host_chip_irq_init != NULL) {
        g_hal_common_ops->host_chip_irq_init();
    }
}
static inline void hal_host_mac_clear_rx_irq(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_mac_clear_rx_irq != NULL) {
        g_hal_common_ops->host_mac_clear_rx_irq(hal_device);
    }
}
static inline void hal_host_mac_mask_rx_irq(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_mac_mask_rx_irq != NULL) {
        g_hal_common_ops->host_mac_mask_rx_irq(hal_device);
    }
}
static inline void hal_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *p_mask)
{
    if (g_hal_common_ops->host_get_host_mac_int_mask != NULL) {
        g_hal_common_ops->host_get_host_mac_int_mask(hal_device, p_mask);
    }
}
static inline void hal_set_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t mask)
{
    if (g_hal_common_ops->host_set_host_mac_int_mask != NULL) {
        g_hal_common_ops->host_set_host_mac_int_mask(hal_device, mask);
    }
}

static inline oal_netbuf_stru *hal_rx_get_next_sub_msdu(hal_host_device_stru *hal_device,
    oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->rx_get_next_sub_msdu != NULL) {
        return g_hal_common_ops->rx_get_next_sub_msdu(hal_device, netbuf);
    }
    return NULL;
}

static inline void hal_tx_ba_info_dscr_get(uint8_t *ba_info_data, uint32_t len, hal_tx_ba_info_stru *tx_ba_info)
{
    if (g_hal_common_ops->tx_ba_info_dscr_get != NULL) {
        g_hal_common_ops->tx_ba_info_dscr_get(ba_info_data, tx_ba_info);
    }
}

static inline void hal_tx_msdu_dscr_info_get(oal_netbuf_stru *netbuf, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    if (g_hal_common_ops->tx_msdu_dscr_info_get != NULL) {
        g_hal_common_ops->tx_msdu_dscr_info_get(netbuf, tx_msdu_info);
    }
}

static inline uint8_t hal_host_tx_tid_ring_size_get(uint8_t ring_size_grade, uint8_t tid_no)
{
    if (g_hal_common_ops->host_get_tx_tid_ring_size != NULL) {
        return g_hal_common_ops->host_get_tx_tid_ring_size(ring_size_grade, tid_no);
    }
    return 0;
}
static inline uint32_t hal_host_tx_tid_ring_depth_get(uint8_t size)
{
    if (g_hal_common_ops->host_get_tx_tid_ring_depth != NULL) {
        return g_hal_common_ops->host_get_tx_tid_ring_depth(size);
    }
    return 0;
}

static inline uint8_t hal_tx_complete_ring_rptr_valid(uint8_t size, uint32_t rptr_val)
{
    un_rw_ptr rptr = { .rw_ptr = (uint16_t)rptr_val };
    return (rptr_val != HAL_HOST_READL_INVALID_VAL) &&
           (rptr.st_rw_ptr.bit_rw_ptr < hal_host_tx_tid_ring_depth_get(size));
}

/*
 * 功能描述 : 比较两个ring指针的大小(rw_ptr1较大返回GREATER, 较小SMALLER, 相等EQUAL)
 * 1.日    期 : 2020年4月22日
 *   作    者 : wifi
 *   修改内容 : 新生成函数
 */
OAL_STATIC OAL_INLINE uint8_t hal_tx_rw_ptr_compare(un_rw_ptr rw_ptr1, un_rw_ptr rw_ptr2)
{
    if (rw_ptr1.rw_ptr == rw_ptr2.rw_ptr) {
        return RING_PTR_EQUAL;
    }

    if (rw_ptr1.st_rw_ptr.bit_wrap_flag == rw_ptr2.st_rw_ptr.bit_wrap_flag) {
        return (rw_ptr1.st_rw_ptr.bit_rw_ptr > rw_ptr2.st_rw_ptr.bit_rw_ptr) ? RING_PTR_GREATER : RING_PTR_SMALLER;
    } else {
        return (rw_ptr1.st_rw_ptr.bit_rw_ptr <= rw_ptr2.st_rw_ptr.bit_rw_ptr) ? RING_PTR_GREATER : RING_PTR_SMALLER;
    }
}

static inline void hal_host_ring_tx_init(uint8_t hal_dev_id)
{
    if (g_hal_common_ops->host_ring_tx_init != NULL) {
        g_hal_common_ops->host_ring_tx_init(hal_dev_id);
    }
}

static inline void hal_host_ba_ring_regs_init(uint8_t hal_dev_id)
{
    if (g_hal_common_ops->host_ba_ring_regs_init != NULL) {
        g_hal_common_ops->host_ba_ring_regs_init(hal_dev_id);
    }
}

static inline uint32_t hal_host_ba_ring_depth_get(void)
{
    if (g_hal_common_ops->host_ba_ring_depth_get != NULL) {
        return g_hal_common_ops->host_ba_ring_depth_get();
    }

    return HAL_DEFAULT_BA_INFO_COUNT;
}

static inline int32_t hal_host_init_common_timer(hal_mac_common_timer *mac_timer)
{
    if (g_hal_common_ops->host_init_common_timer != NULL) {
        return g_hal_common_ops->host_init_common_timer(mac_timer);
    }
    return OAL_FAIL;
}

static inline void hal_host_set_mac_common_timer(hal_mac_common_timer *mac_timer)
{
    if (g_hal_common_ops->host_set_mac_common_timer != NULL) {
        g_hal_common_ops->host_set_mac_common_timer(mac_timer);
    }
}

// 该函数内没有唤醒dev的操作，由调用者执行hal_pm_try_wakeup_dev_lock
static inline void hal_host_set_mac_common_timer_no_pm_wakeup(hal_mac_common_timer *mac_timer)
{
    if (g_hal_common_ops->host_set_mac_common_timer_no_pm_wakeup != NULL) {
        g_hal_common_ops->host_set_mac_common_timer_no_pm_wakeup(mac_timer);
    }
}

static inline uint32_t hal_host_rx_get_msdu_info_dscr(hal_host_device_stru *hal_dev,
    oal_netbuf_stru *netbuf, mac_rx_ctl_stru *rx_info, uint8_t first_msdu)
{
    if (g_hal_common_ops->host_rx_get_msdu_info_dscr != NULL) {
        return g_hal_common_ops->host_rx_get_msdu_info_dscr(hal_dev, netbuf, rx_info, first_msdu);
    }
    return OAL_FAIL;
}

#ifdef _PRE_WLAN_FEATURE_CSI
static inline uint32_t hal_get_csi_info(hmac_csi_info_stru *hmac_csi_info, uint8_t *addr)
{
    if (g_hal_common_ops->host_get_csi_info != NULL) {
        return g_hal_common_ops->host_get_csi_info(hmac_csi_info, addr);
    }
    return OAL_FAIL;
}
#endif
static inline uint8_t hal_host_vap_get_by_hw_vap_id(uint8_t hal_device_id, uint8_t hw_vap_id)
{
#ifdef _PRE_WLAN_FEATURE_FTM
    if (g_hal_common_ops->host_vap_get_by_hw_vap_id != NULL) {
        return g_hal_common_ops->host_vap_get_by_hw_vap_id(hal_device_id, hw_vap_id);
    }
#endif
    return WLAN_VAP_MAX_NUM_LIMIT;
}
#ifdef _PRE_WLAN_FEATURE_FTM
static inline uint32_t hal_host_ftm_reg_init(uint8_t hal_dev_id)
{
    if (g_hal_common_ops->host_ftm_reg_init != NULL) {
        return g_hal_common_ops->host_ftm_reg_init(hal_dev_id);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_ftm_get_info(hal_ftm_info_stru *hal_ftm_info, uint8_t *addr)
{
    if (g_hal_common_ops->host_ftm_get_info != NULL) {
        return g_hal_common_ops->host_ftm_get_info(hal_ftm_info, addr);
    }
    return OAL_FAIL;
}

static inline void hal_host_ftm_get_divider(hal_host_device_stru *hal_device, uint8_t *divider)
{
    if (g_hal_common_ops->host_ftm_get_divider != NULL) {
        return g_hal_common_ops->host_ftm_get_divider(hal_device, divider);
    }
}

static inline void hal_host_ftm_set_sample(hal_host_device_stru *hal_device, oal_bool_enum_uint8 ftm_status)
{
    if (g_hal_common_ops->host_ftm_set_sample != NULL) {
        g_hal_common_ops->host_ftm_set_sample(hal_device, ftm_status);
    }
}

static inline void hal_host_ftm_set_enable(hal_host_device_stru *hal_device, oal_bool_enum_uint8 ftm_status)
{
    if (g_hal_common_ops->host_ftm_set_enable != NULL) {
        return g_hal_common_ops->host_ftm_set_enable(hal_device, ftm_status);
    }
}

static inline void hal_host_ftm_set_m2s_phy(hal_host_device_stru *hal_device, uint8_t tx_chain_selection,
                                            uint8_t tx_rssi_selection)
{
    if (g_hal_common_ops->host_ftm_set_m2s_phy != NULL) {
        g_hal_common_ops->host_ftm_set_m2s_phy(hal_device, tx_chain_selection, tx_rssi_selection);
    }
}

static inline void hal_host_ftm_set_buf_base_addr(hal_host_device_stru *hal_device, uintptr_t devva)
{
    if (g_hal_common_ops->host_ftm_set_buf_base_addr != NULL) {
        g_hal_common_ops->host_ftm_set_buf_base_addr(hal_device, devva);
    }
}

static inline void hal_host_ftm_set_buf_size(hal_host_device_stru *hal_device, uint16_t cfg_ftm_buf_size)
{
    if (g_hal_common_ops->host_ftm_set_buf_size != NULL) {
        g_hal_common_ops->host_ftm_set_buf_size(hal_device, cfg_ftm_buf_size);
    }
}

static inline uint32_t hal_host_ftm_set_white_list(hal_host_device_stru *hal_device, uint8_t idx,
                                                   uint8_t *mac_addr)
{
    if (g_hal_common_ops->host_ftm_set_white_list != NULL) {
        return g_hal_common_ops->host_ftm_set_white_list(hal_device, idx, mac_addr);
    }
    return OAL_FAIL;
}

static inline void hal_host_ftm_transfer_device_stru_to_host_device(uint8_t *device_param, uint8_t *host_param)
{
    if (g_hal_common_ops->host_ftm_transfer_device_stru_to_host_stru != NULL) {
        g_hal_common_ops->host_ftm_transfer_device_stru_to_host_stru(device_param, host_param);
    }
}

static inline void hal_host_ftm_get_rssi_selection(uint8_t *hmac_ftm, uint8_t *device_param)
{
    if (g_hal_common_ops->host_ftm_get_rssi_selection != NULL) {
        g_hal_common_ops->host_ftm_get_rssi_selection(hmac_ftm, device_param);
    }
}
#endif
#ifdef _PRE_WLAN_FEATURE_CSI
static inline uint32_t hal_host_csi_config(uint16_t vap_idx, uint16_t len, uint8_t *param)
{
    if (g_hal_common_ops->host_csi_config != NULL) {
        return g_hal_common_ops->host_csi_config(vap_idx, len, param);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_ftm_csi_init(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_ftm_csi_init != NULL) {
        return g_hal_common_ops->host_ftm_csi_init(hal_device);
    }
    return OAL_FAIL;
}
#endif
static inline uint32_t hal_host_tx_clear_msdu_padding(oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->host_tx_clear_msdu_padding != NULL) {
        return g_hal_common_ops->host_tx_clear_msdu_padding(netbuf);
    }

    /* 默认不需要清零padding, 返回成功 */
    return OAL_SUCC;
}

static inline void hal_host_psd_rpt_to_file_func(oal_netbuf_stru *netbuf, uint16_t psd_info_size)
{
#ifdef _PRE_WLAN_FEATURE_PSD_ANALYSIS
    if (g_hal_common_ops->host_psd_rpt_to_file != NULL) {
        g_hal_common_ops->host_psd_rpt_to_file(netbuf, psd_info_size);
    }
#endif
}

#ifdef _PRE_WLAN_FEATURE_SNIFFER
static inline uint32_t hal_host_sniffer_rx_ppdu_free_ring_init(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_sniffer_rx_ppdu_free_ring_init != NULL) {
        return g_hal_common_ops->host_sniffer_rx_ppdu_free_ring_init(hal_device);
    }
    return OAL_FAIL;
}

static inline void hal_host_sniffer_rx_ppdu_free_ring_deinit(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_sniffer_rx_ppdu_free_ring_deinit != NULL) {
        g_hal_common_ops->host_sniffer_rx_ppdu_free_ring_deinit(hal_device);
    }
}

static inline void hal_host_sniffer_rx_info_fill(hal_host_device_stru *hal_device,
    oal_netbuf_stru *netbuf, hal_sniffer_extend_info *sniffer_rx_info, mac_rx_ctl_stru *rx_ctl)
{
    if (g_hal_common_ops->host_sniffer_rx_info_fill != NULL) {
        g_hal_common_ops->host_sniffer_rx_info_fill(hal_device, netbuf, sniffer_rx_info, rx_ctl);
    }
}

static inline void hal_host_sniffer_add_rx_ppdu_dscr(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_sniffer_add_rx_ppdu_dscr != NULL) {
        g_hal_common_ops->host_sniffer_add_rx_ppdu_dscr(hal_device);
    }
}

#endif

#ifdef _PRE_WLAN_FEATURE_VSP
static inline void hal_vsp_msdu_dscr_info_get(uint8_t *buffer, hal_tx_msdu_dscr_info_stru *tx_msdu_info)
{
    if (g_hal_common_ops->host_vsp_msdu_dscr_info_get != NULL) {
        g_hal_common_ops->host_vsp_msdu_dscr_info_get(buffer, tx_msdu_info);
    }
}
#endif

static inline uint32_t hal_host_get_tsf_lo(hal_host_device_stru *hal_device, uint8_t hal_vap_id, uint32_t *tsf)
{
    if (g_hal_common_ops->host_get_tsf_lo != NULL) {
        return g_hal_common_ops->host_get_tsf_lo(hal_device, hal_vap_id, tsf);
    }
    return OAL_FAIL;
}
uint32_t hal_host_device_exit(uint8_t hal_dev_id);

static inline uint8_t hal_host_get_device_tx_ring_num(void)
{
    if (g_hal_common_ops->host_get_device_tx_ring_num != NULL) {
        return g_hal_common_ops->host_get_device_tx_ring_num();
    }

    return 0;
}
static inline void hal_host_rx_tcp_udp_checksum(oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->host_rx_tcp_udp_checksum != NULL) {
        g_hal_common_ops->host_rx_tcp_udp_checksum(netbuf);
    }
}
static inline oal_bool_enum_uint8 hal_host_rx_ipv4_checksum_is_pass(oal_netbuf_stru *netbuf)
{
    if (g_hal_common_ops->host_rx_ip_checksum_is_pass != NULL) {
        return g_hal_common_ops->host_rx_ip_checksum_is_pass(netbuf);
    }
    return OAL_TRUE;
}

static inline void hal_host_mac_phy_irq_mask(void)
{
    if (g_hal_common_ops->host_mac_phy_irq_mask != NULL) {
        g_hal_common_ops->host_mac_phy_irq_mask();
    }
}
static inline uint32_t hal_get_host_ftm_csi_locationinfo(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->get_host_ftm_csi != NULL) {
        return g_hal_common_ops->get_host_ftm_csi(hal_device);
    }
    return 0;
}
static inline uint32_t hal_host_alloc_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint32_t size)
{
    if (g_hal_common_ops->host_alloc_tx_ring != NULL) {
        return g_hal_common_ops->host_alloc_tx_ring(msdu_ring, size);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_release_tx_ring(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    if (g_hal_common_ops->host_release_tx_ring != NULL) {
        return g_hal_common_ops->host_release_tx_ring(msdu_ring);
    }
    return OAL_FAIL;
}

static inline uint8_t hal_host_tx_ring_alloced(hal_host_tx_msdu_ring_stru *msdu_ring)
{
    if (g_hal_common_ops->host_tx_ring_alloced != NULL) {
        return g_hal_common_ops->host_tx_ring_alloced(msdu_ring);
    }
    return OAL_FALSE;
}

static inline uint32_t hal_host_tx_set_msdu_ring(
    hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
    if (g_hal_common_ops->host_tx_set_msdu_ring != NULL) {
        return g_hal_common_ops->host_tx_set_msdu_ring(msdu_ring, msdu_info, index);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_tx_get_msdu_ring(
    hal_host_tx_msdu_ring_stru *msdu_ring, msdu_info_stru *msdu_info, uint16_t index)
{
    if (g_hal_common_ops->host_tx_get_msdu_ring != NULL) {
        return g_hal_common_ops->host_tx_get_msdu_ring(msdu_ring, msdu_info, index);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_tx_reset_msdu_ring(hal_host_tx_msdu_ring_stru *msdu_ring, uint16_t index)
{
    if (g_hal_common_ops->host_tx_reset_msdu_ring != NULL) {
        return g_hal_common_ops->host_tx_reset_msdu_ring(msdu_ring, index);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_tx_get_msdu_info_devva(hal_host_tx_msdu_ring_stru *msdu_ring,
    hal_tx_ring_table_addr_stru *ring_devva)
{
    if (g_hal_common_ops->host_tx_get_msdu_info_devva != NULL) {
        return g_hal_common_ops->host_tx_get_msdu_info_devva(msdu_ring, ring_devva);
    }
    return OAL_FAIL;
}

static inline uint8_t hal_host_tx_get_msdu_dscr_len(void)
{
    if (g_hal_common_ops->host_tx_get_msdu_dscr_len != NULL) {
        return g_hal_common_ops->host_tx_get_msdu_dscr_len();
    }
    return 0;
}

static inline void hal_host_tx_set_ring_table_base_addr(uint32_t addr)
{
    if (g_hal_common_ops->host_tx_set_ring_table_base_addr != NULL) {
        g_hal_common_ops->host_tx_set_ring_table_base_addr(addr);
    }
}

static inline uint32_t hal_host_tx_init_tid_ring_table(uint8_t lut_idx,
    uint8_t tid, hal_tx_ring_table_addr_val_stru *ring_tb_addr)
{
    if (g_hal_common_ops->host_tx_init_tid_ring_table != NULL) {
        return g_hal_common_ops->host_tx_init_tid_ring_table(lut_idx, tid, ring_tb_addr);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_tx_ring_update_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    if (g_hal_common_ops->host_tx_ring_update_ops != NULL) {
        return g_hal_common_ops->host_tx_ring_update_ops(param, type);
    }
    return OAL_FAIL;
}

static inline uint32_t hal_host_tx_ring_get_ops(hal_host_tx_ring_ops_param_stru *param, uint8_t type)
{
    if (g_hal_common_ops->host_tx_ring_get_ops != NULL) {
        return g_hal_common_ops->host_tx_ring_get_ops(param, type);
    }
    return OAL_FAIL;
}

static inline uint16_t hal_host_get_tx_ring_lut_idx(uint16_t lut_idx, uint16_t mld_user_lut_idx)
{
    if (g_hal_common_ops->host_get_tx_ring_lut_idx != NULL) {
        return g_hal_common_ops->host_get_tx_ring_lut_idx(lut_idx, mld_user_lut_idx);
    }
    return lut_idx;
}

static inline uint8_t hal_host_get_rx_msdu_dscr_len(void)
{
    if (g_hal_common_ops->host_get_rx_msdu_dscr_len != NULL) {
        return g_hal_common_ops->host_get_rx_msdu_dscr_len();
    }
    return 0;
}

static inline void hal_host_h2d_rx_ring_reset(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_h2d_rx_ring_reset != NULL) {
        g_hal_common_ops->host_h2d_rx_ring_reset(hal_device);
    }
}

static inline void hal_host_unmask_soc_host_intr(hal_host_device_stru *hal_device)
{
    if (g_hal_common_ops->host_unmask_soc_host_intr != NULL) {
        g_hal_common_ops->host_unmask_soc_host_intr(hal_device);
    }
}

#endif
