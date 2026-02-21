/**
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 *
 * Description: pcie_ctrl_rb_regs.h header file
 * Author: @CompanyNameTag
 */

#ifndef MP17C_PCIE_CTRL_RB_REGS_H
#define MP17C_PCIE_CTRL_RB_REGS_H

/* This file is generated automatically. Do not modify it */
#define PCIE_CTRL_RB_BASE    (0x04980000)

/* SYS CTL ID寄存器 0x000 */
typedef union {
    struct {
        unsigned int pcie_ctrl_sys_ctl_id : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_ctrl_sys_ctl_id;
#define PCIE_CTRL_RB_PCIE_CTRL_SYS_CTL_ID_OFF    0x000
#define PCIE_CTRL_RB_PCIE_CTRL_SYS_CTL_ID_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CTRL_SYS_CTL_ID_OFF)

/* 通用寄存器 0x0010 */
typedef union {
    struct {
        unsigned int pcie_ctrl_gp_reg0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_ctrl_gp_reg0;
#define PCIE_CTRL_RB_PCIE_CTRL_GP_REG0_OFF    0x0010
#define PCIE_CTRL_RB_PCIE_CTRL_GP_REG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CTRL_GP_REG0_OFF)

/* 通用寄存器 0x0014 */
typedef union {
    struct {
        unsigned int pcie_ctrl_gp_reg1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_ctrl_gp_reg1;
#define PCIE_CTRL_RB_PCIE_CTRL_GP_REG1_OFF    0x0014
#define PCIE_CTRL_RB_PCIE_CTRL_GP_REG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CTRL_GP_REG1_OFF)

/* 测试寄存器 0x100 */
typedef union {
    struct {
        unsigned int pcie_ctrl_test_value : 16; /* 0:15 */
        unsigned int reserved0 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_ctrl_test_value;
#define PCIE_CTRL_RB_PCIE_CTRL_TEST_VALUE_OFF    0x100
#define PCIE_CTRL_RB_PCIE_CTRL_TEST_VALUE_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CTRL_TEST_VALUE_OFF)

/* 测试结果寄存器 0x104 */
typedef union {
    struct {
        unsigned int pcie_ctrl_test_result : 16; /* 0:15 */
        unsigned int reserved0 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_ctrl_test_result;
#define PCIE_CTRL_RB_PCIE_CTRL_TEST_RESULT_OFF    0x104
#define PCIE_CTRL_RB_PCIE_CTRL_TEST_RESULT_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CTRL_TEST_RESULT_OFF)

/* PCIE参数控制寄存器 0x108 */
typedef union {
    struct {
        unsigned int pcie_ete_sp_ram_tmod_h : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mem_sp_ram_tmode_h;
#define PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_H_OFF    0x108
#define PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_H_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_H_OFF)

/* PCIE参数控制寄存器 0x10C */
typedef union {
    struct {
        unsigned int pcie_ete_sp_ram_tmod_l : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mem_sp_ram_tmode_l;
#define PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_L_OFF    0x10C
#define PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_L_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MEM_SP_RAM_TMODE_L_OFF)

/* PCIE参数控制寄存器 0x110 */
typedef union {
    struct {
        unsigned int pcie_ete_tp_ram_tmod : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mem_tp_ram_tmode;
#define PCIE_CTRL_RB_PCIE_MEM_TP_RAM_TMODE_OFF    0x110
#define PCIE_CTRL_RB_PCIE_MEM_TP_RAM_TMODE_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MEM_TP_RAM_TMODE_OFF)

/*  0x200 */
typedef union {
    struct {
        unsigned int sc_icg_en_pcie50_clk_pcs_aux : 1; /* 0 */
        unsigned int sc_icg_en_pcie50_clk_tl_apb : 1; /* 1 */
        unsigned int sc_icg_en_pcie50_clk_pcs_apb : 1; /* 2 */
        unsigned int sc_icg_en_pcie50_clk_pcs_rx : 2; /* 3:4 */
        unsigned int sc_icg_en_pcie50_clk_pcs_tx : 2; /* 5:6 */
        unsigned int sc_icg_en_pcie50_clk_pcs_logic_common : 1; /* 7 */
        unsigned int sc_icg_en_pcie50_clk_pcs_logic_div : 2; /* 8:9 */
        unsigned int sc_icg_en_pcie50_clk_pcs_logic : 2; /* 10:11 */
        unsigned int sc_icg_en_pcie50_clk_pipe_lane             : 2; /* 12:13 */
        unsigned int sc_icg_en_pcie50_clk_core_phy_lane         : 2; /* 14:15 */
        unsigned int sc_icg_en_pcie50_clk_core_phy_common : 1; /* 16 */
        unsigned int sc_icg_en_pcie50_clk_core_phy_port_div         : 1; /* 17 */
        unsigned int sc_icg_en_pcie50_clk_core_phy_port         : 1; /* 18 */
        unsigned int sc_icg_en_pcie50_clk_core_tl_common  : 1; /* 19 */
        unsigned int sc_icg_en_pcie50_clk_core_tl_port_div               : 1; /* 20 */
        unsigned int sc_icg_en_pcie50_clk_core_tl_port               : 1; /* 21 */
        unsigned int sc_icg_en_pcie50_clk_tl_axi          : 1; /* 22 */
        unsigned int sc_icg_en_pcie50_clk_ap_axi                  : 5; /* 23:27 */
        unsigned int sc_icg_en_pcie50_clk_apb              : 2; /* 28:29 */
        unsigned int reserved0 : 2; /* 30:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_icg;
#define PCIE_CTRL_RB_PCIE_MAC_ICG_OFF    0x200
#define PCIE_CTRL_RB_PCIE_MAC_ICG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_ICG_OFF)

/*  0x204 */
typedef union {
    struct {
        unsigned int sc_srst_req_pcie50_pcs_rx : 2; /* 0:1 */
        unsigned int sc_srst_req_pcie50_pcs_tx : 2; /* 2:3 */
        unsigned int sc_srst_req_ras_status : 1; /* 4 */
        unsigned int sc_srst_req_pcie50_pipe_lane : 2; /* 5:6 */
        unsigned int sc_srst_req_pcie50_pcs_logic : 1; /* 7 */
        unsigned int sc_srst_req_pcie50_pipe     : 2; /* 8:9 */
        unsigned int sc_srst_req_pcie50_core_phy    : 1; /* 10 */
        unsigned int sc_srst_req_pcie50_core_tl : 1; /* 11 */
        unsigned int sc_srst_req_pcie50_tl_axi : 1; /* 12 */
        unsigned int sc_srst_req_pcie50_ap_axi : 1; /* 13 */
        unsigned int sc_srst_req_pcie50_apb         : 1; /* 14 */
        unsigned int reserved0 : 17; /* 15:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_soft_reset;
#define PCIE_CTRL_RB_PCIE_MAC_SOFT_RESET_OFF    0x204
#define PCIE_CTRL_RB_PCIE_MAC_SOFT_RESET_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_SOFT_RESET_OFF)

/*  0x208 */
typedef union {
    struct {
        unsigned int phy2pcie_pipe_ebuffer_location : 6; /* 0:5 */
        unsigned int reserved0 : 26; /* 6:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_cfg;
#define PCIE_CTRL_RB_PCIE_MAC_CFG_OFF    0x208
#define PCIE_CTRL_RB_PCIE_MAC_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_CFG_OFF)

/* PCIE状态上报寄存器 0x20C */
typedef union {
    struct {
        unsigned int pcie_ctrl_pm_linkst_in_l1sub : 1; /* 0 */
        unsigned int pcie_ctrl_pm_linkst_in_l1 : 1; /* 1 */
        unsigned int pcie_ctrl_pm_linkst_in_l0s : 1; /* 2 */
        unsigned int pcie_ctrl_pm_linkst_in_l0 : 1; /* 3 */
        unsigned int pcie_linkst_in_l1_2 : 1; /* 4 */
        unsigned int reserved0 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status0;
#define PCIE_CTRL_RB_PCIE_STATUS0_OFF    0x20C
#define PCIE_CTRL_RB_PCIE_STATUS0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS0_OFF)

/* PCIE状态上报寄存器 0x210 */
typedef union {
    struct {
        unsigned int pcie_clkreq_n_monitor_en : 1; /* 0 */
        unsigned int pcie_clkreq_n_monitor_clr : 1; /* 1 */
        unsigned int reserved0 : 14; /* 2:15 */
        unsigned int pcie_clkreq_n_monitor_cnt : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status1;
#define PCIE_CTRL_RB_PCIE_STATUS1_OFF    0x210
#define PCIE_CTRL_RB_PCIE_STATUS1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS1_OFF)

/* 低功耗进出配置寄存器 0x214 */
typedef union {
    struct {
        unsigned int soc_app_xfer_pending : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_low_power_cfg0;
#define PCIE_CTRL_RB_LOW_POWER_CFG0_OFF    0x214
#define PCIE_CTRL_RB_LOW_POWER_CFG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_LOW_POWER_CFG0_OFF)

/* 低功耗进出配置寄存器 0x218 */
typedef union {
    struct {
        unsigned int host_xfer_pending : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_low_power_cfg1;
#define PCIE_CTRL_RB_LOW_POWER_CFG1_OFF    0x218
#define PCIE_CTRL_RB_LOW_POWER_CFG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_LOW_POWER_CFG1_OFF)

/* X2X自动cg bypass 0x21C */
typedef union {
    struct {
        unsigned int pciemst_x2x_auto_cg_bypass : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_mstr_x2x_auto_cg_bypass;
#define PCIE_CTRL_RB_MSTR_X2X_AUTO_CG_BYPASS_OFF    0x21C
#define PCIE_CTRL_RB_MSTR_X2X_AUTO_CG_BYPASS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MSTR_X2X_AUTO_CG_BYPASS_OFF)

/* PCIE状态上报寄存器 0x240 */
typedef union {
    struct {
        unsigned int soc_cpl_timeout : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_cpl_timeout_sts;
#define PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_STS_OFF    0x240
#define PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_STS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_STS_OFF)

/* PCIE状态清除寄存器 0x244 */
typedef union {
    struct {
        unsigned int soc_cpl_timeout_clr : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_cpl_timeout_clr;
#define PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_CLR_OFF    0x244
#define PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_CLR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CPL_TIMEOUT_CLR_OFF)

/* PCIE MSG中断屏蔽前上报寄存器 0x2A8 */
typedef union {
    struct {
        unsigned int soc_radm_msg_turnoff : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_msg_intr;
#define PCIE_CTRL_RB_PCIE_MSG_INTR_OFF    0x2A8
#define PCIE_CTRL_RB_PCIE_MSG_INTR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MSG_INTR_OFF)

/* PCIE MSG中断屏蔽后上报寄存器 0x2AC */
typedef union {
    struct {
        unsigned int soc_radm_msg_turnoff_status : 1; /* 0 */
        unsigned int link_down_irq : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_msg_intr_status;
#define PCIE_CTRL_RB_PCIE_MSG_INTR_STATUS_OFF    0x2AC
#define PCIE_CTRL_RB_PCIE_MSG_INTR_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MSG_INTR_STATUS_OFF)

/* PCIE MSG中断清除寄存器 0x2B0 */
typedef union {
    struct {
        unsigned int soc_radm_msg_turnoff_clr : 1; /* 0 */
        unsigned int link_down_irq_clr : 1; /* 1 */
        unsigned int mannual_link_down_clr : 1; /* 2 */
        unsigned int reserved0 : 29; /* 3:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_msg_intr_clr;
#define PCIE_CTRL_RB_PCIE_MSG_INTR_CLR_OFF    0x2B0
#define PCIE_CTRL_RB_PCIE_MSG_INTR_CLR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MSG_INTR_CLR_OFF)

/* PCIE MSG中断屏蔽寄存器 0x2B4 */
typedef union {
    struct {
        unsigned int soc_radm_msg_turnoff_mask : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_msg_intr_mask;
#define PCIE_CTRL_RB_PCIE_MSG_INTR_MASK_OFF    0x2B4
#define PCIE_CTRL_RB_PCIE_MSG_INTR_MASK_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MSG_INTR_MASK_OFF)

/* PCIE配置寄存器 0x2D0 */
typedef union {
    struct {
        unsigned int pcie_linkdown_auto_rstn_disable : 1; /* 0 */
        unsigned int phy_rxvalid_hand_disable : 1; /* 1 */
        unsigned int soc_app_xfer_pending_sel : 1; /* 2 */
        unsigned int reserved0 : 29; /* 3:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_cfg;
#define PCIE_CTRL_RB_PCIE_CFG_OFF    0x2D0
#define PCIE_CTRL_RB_PCIE_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CFG_OFF)

/* OBFF DEC配置寄存器 0x2D4 */
typedef union {
    struct {
        unsigned int host2device_tx_intr_set : 1; /* 0 */
        unsigned int host2device_rx_intr_set : 1; /* 1 */
        unsigned int device2host_tx_intr_set : 1; /* 2 */
        unsigned int device2host_rx_intr_set : 1; /* 3 */
        unsigned int reserved0 : 28; /* 4:31 */
    } bits;
    unsigned int as_dword;
} hreg_host2device_intr_set;
#define PCIE_CTRL_RB_HOST2DEVICE_INTR_SET_OFF    0x2D4
#define PCIE_CTRL_RB_HOST2DEVICE_INTR_SET_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST2DEVICE_INTR_SET_OFF)

/* 核间通信寄存器 0x2D8 */
typedef union {
    struct {
        unsigned int host2device_reg0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_device_reg0;
#define PCIE_CTRL_RB_HOST_DEVICE_REG0_OFF    0x2D8
#define PCIE_CTRL_RB_HOST_DEVICE_REG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_DEVICE_REG0_OFF)

/* 核间通信寄存器 0x2DC */
typedef union {
    struct {
        unsigned int host2device_reg1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_device_reg1;
#define PCIE_CTRL_RB_HOST_DEVICE_REG1_OFF    0x2DC
#define PCIE_CTRL_RB_HOST_DEVICE_REG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_DEVICE_REG1_OFF)

/* PCIE屏蔽前中断上报 0x2E4 */
typedef union {
    struct {
        unsigned int host2device_tx_intr_raw_status : 1; /* 0 */
        unsigned int host2device_rx_intr_raw_status : 1; /* 1 */
        unsigned int device2host_tx_intr_raw_status : 1; /* 2 */
        unsigned int device2host_rx_intr_raw_status : 1; /* 3 */
        unsigned int device2host_intr_raw_status : 1; /* 4 */
        unsigned int reserved0 : 1; /* 5 */
        unsigned int mac_n2_intr_raw_status : 1; /* 6 */
        unsigned int mac_n1_intr_raw_status : 1; /* 7 */
        unsigned int phy_n2_intr_raw_status : 1; /* 8 */
        unsigned int phy_n1_intr_raw_status : 1; /* 9 */
        unsigned int pcie_slv_rresp_intr_raw_status : 1; /* 10 */
        unsigned int mac_n3_intr_raw_status : 1; /* 11 */
        unsigned int reserved1 : 20; /* 12:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_intr_raw_status;
#define PCIE_CTRL_RB_HOST_INTR_RAW_STATUS_OFF    0x2E4
#define PCIE_CTRL_RB_HOST_INTR_RAW_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_INTR_RAW_STATUS_OFF)

/* PCIE中断上报 0x2E8 */
typedef union {
    struct {
        unsigned int host2device_tx_intr_mask : 1; /* 0 */
        unsigned int host2device_rx_intr_mask : 1; /* 1 */
        unsigned int device2host_tx_intr_mask : 1; /* 2 */
        unsigned int device2host_rx_intr_mask : 1; /* 3 */
        unsigned int device2host_intr_mask : 1; /* 4 */
        unsigned int reserved0 : 1; /* 5 */
        unsigned int mac_n2_intr_mask : 1; /* 6 */
        unsigned int mac_n1_intr_mask : 1; /* 7 */
        unsigned int phy_n2_intr_mask : 1; /* 8 */
        unsigned int phy_n1_intr_mask : 1; /* 9 */
        unsigned int pcie_slv_rresp_intr_mask : 1; /* 10 */
        unsigned int mac_n3_intr_mask : 1; /* 11 */
        unsigned int reserved1 : 20; /* 12:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_intr_mask;
#define PCIE_CTRL_RB_HOST_INTR_MASK_OFF    0x2E8
#define PCIE_CTRL_RB_HOST_INTR_MASK_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_INTR_MASK_OFF)

/* PCIE屏蔽后中断上报 0x2EC */
typedef union {
    struct {
        unsigned int host2device_tx_intr_status : 1; /* 0 */
        unsigned int host2device_rx_intr_status : 1; /* 1 */
        unsigned int device2host_tx_intr_status : 1; /* 2 */
        unsigned int device2host_rx_intr_status : 1; /* 3 */
        unsigned int device2host_intr_status : 1; /* 4 */
        unsigned int reserved0 : 1; /* 5 */
        unsigned int mac_n2_intr_status : 1; /* 6 */
        unsigned int mac_n1_intr_status : 1; /* 7 */
        unsigned int phy_n2_intr_status : 1; /* 8 */
        unsigned int phy_n1_intr_status : 1; /* 9 */
        unsigned int pcie_slv_rresp_intr_status : 1; /* 10 */
        unsigned int mac_n3_intr_status : 1; /* 11 */
        unsigned int reserved1 : 20; /* 12:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_intr_status;
#define PCIE_CTRL_RB_HOST_INTR_STATUS_OFF    0x2EC
#define PCIE_CTRL_RB_HOST_INTR_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_INTR_STATUS_OFF)

/* PCIE中断上报 0x2F0 */
typedef union {
    struct {
        unsigned int host2device_tx_intr_clr : 1; /* 0 */
        unsigned int host2device_rx_intr_clr : 1; /* 1 */
        unsigned int device2host_tx_intr_clr : 1; /* 2 */
        unsigned int device2host_rx_intr_clr : 1; /* 3 */
        unsigned int reserved0 : 6; /* 4:9 */
        unsigned int pcie_slv_rresp_intr_clr : 1; /* 10 */
        unsigned int reserved1 : 21; /* 11:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_intr_clr;
#define PCIE_CTRL_RB_HOST_INTR_CLR_OFF    0x2F0
#define PCIE_CTRL_RB_HOST_INTR_CLR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_INTR_CLR_OFF)

/* 性能统计配置寄存器 0x2F4 */
typedef union {
    struct {
        unsigned int pfmc_test_enable : 1; /* 0 */
        unsigned int pfmc_test_finish : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_performance_test_cfg0;
#define PCIE_CTRL_RB_PERFORMANCE_TEST_CFG0_OFF    0x2F4
#define PCIE_CTRL_RB_PERFORMANCE_TEST_CFG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PERFORMANCE_TEST_CFG0_OFF)

/* 性能统计时长配置寄存器 0x2F8 */
typedef union {
    struct {
        unsigned int test_delay_cnt : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_test_delay_cnt_low;
#define PCIE_CTRL_RB_TEST_DELAY_CNT_LOW_OFF    0x2F8
#define PCIE_CTRL_RB_TEST_DELAY_CNT_LOW_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_TEST_DELAY_CNT_LOW_OFF)

/* 性能统计上报寄存器 0x2FC */
typedef union {
    struct {
        unsigned int pcie_slv_write_performance0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_write_performance0;
#define PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE0_OFF    0x2FC
#define PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE0_OFF)

/* 性能统计上报寄存器 0x300 */
typedef union {
    struct {
        unsigned int pcie_slv_write_performance1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_write_performance1;
#define PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE1_OFF    0x300
#define PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_WRITE_PERFORMANCE1_OFF)

/* 性能统计上报寄存器 0x304 */
typedef union {
    struct {
        unsigned int pcie_slv_read_performance0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_read_performance0;
#define PCIE_CTRL_RB_SLV_READ_PERFORMANCE0_OFF    0x304
#define PCIE_CTRL_RB_SLV_READ_PERFORMANCE0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_READ_PERFORMANCE0_OFF)

/* 性能统计上报寄存器 0x308 */
typedef union {
    struct {
        unsigned int pcie_slv_read_performance1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_read_performance1;
#define PCIE_CTRL_RB_SLV_READ_PERFORMANCE1_OFF    0x308
#define PCIE_CTRL_RB_SLV_READ_PERFORMANCE1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_READ_PERFORMANCE1_OFF)

/* 性能统计上报寄存器 0x30C */
typedef union {
    struct {
        unsigned int pcie_mst_write_performance0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_mst_write_performance0;
#define PCIE_CTRL_RB_MST_WRITE_PERFORMANCE0_OFF    0x30C
#define PCIE_CTRL_RB_MST_WRITE_PERFORMANCE0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MST_WRITE_PERFORMANCE0_OFF)

/* 性能统计上报寄存器 0x310 */
typedef union {
    struct {
        unsigned int pcie_mst_write_performance1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_mst_write_performance1;
#define PCIE_CTRL_RB_MST_WRITE_PERFORMANCE1_OFF    0x310
#define PCIE_CTRL_RB_MST_WRITE_PERFORMANCE1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MST_WRITE_PERFORMANCE1_OFF)

/* 性能统计上报寄存器 0x314 */
typedef union {
    struct {
        unsigned int pcie_mst_read_performance0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_mst_read_performance0;
#define PCIE_CTRL_RB_MST_READ_PERFORMANCE0_OFF    0x314
#define PCIE_CTRL_RB_MST_READ_PERFORMANCE0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MST_READ_PERFORMANCE0_OFF)

/* 性能统计上报寄存器 0x318 */
typedef union {
    struct {
        unsigned int pcie_mst_read_performance1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_mst_read_performance1;
#define PCIE_CTRL_RB_MST_READ_PERFORMANCE1_OFF    0x318
#define PCIE_CTRL_RB_MST_READ_PERFORMANCE1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MST_READ_PERFORMANCE1_OFF)

/* 操作采样上报寄存器 0x31C */
typedef union {
    struct {
        unsigned int pcie_slv_awaddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt0;
#define PCIE_CTRL_RB_MONITOR_RPT0_OFF    0x31C
#define PCIE_CTRL_RB_MONITOR_RPT0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT0_OFF)

/* 操作采样上报寄存器 0x320 */
typedef union {
    struct {
        unsigned int pcie_slv_awaddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt1;
#define PCIE_CTRL_RB_MONITOR_RPT1_OFF    0x320
#define PCIE_CTRL_RB_MONITOR_RPT1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT1_OFF)

/* 操作采样上报寄存器 0x324 */
typedef union {
    struct {
        unsigned int pcie_slv_wdata0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt2;
#define PCIE_CTRL_RB_MONITOR_RPT2_OFF    0x324
#define PCIE_CTRL_RB_MONITOR_RPT2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT2_OFF)

/* 操作采样上报寄存器 0x328 */
typedef union {
    struct {
        unsigned int pcie_slv_wdata1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt3;
#define PCIE_CTRL_RB_MONITOR_RPT3_OFF    0x328
#define PCIE_CTRL_RB_MONITOR_RPT3_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT3_OFF)

/* 操作采样上报寄存器 0x32C */
typedef union {
    struct {
        unsigned int pcie_slv_araddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt6;
#define PCIE_CTRL_RB_MONITOR_RPT6_OFF    0x32C
#define PCIE_CTRL_RB_MONITOR_RPT6_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT6_OFF)

/* 操作采样上报寄存器 0x330 */
typedef union {
    struct {
        unsigned int pcie_slv_araddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt7;
#define PCIE_CTRL_RB_MONITOR_RPT7_OFF    0x330
#define PCIE_CTRL_RB_MONITOR_RPT7_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT7_OFF)

/* 操作采样上报寄存器 0x334 */
typedef union {
    struct {
        unsigned int pcie_slv_rdata0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt8;
#define PCIE_CTRL_RB_MONITOR_RPT8_OFF    0x334
#define PCIE_CTRL_RB_MONITOR_RPT8_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT8_OFF)

/* 操作采样上报寄存器 0x338 */
typedef union {
    struct {
        unsigned int pcie_slv_rdata1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt9;
#define PCIE_CTRL_RB_MONITOR_RPT9_OFF    0x338
#define PCIE_CTRL_RB_MONITOR_RPT9_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT9_OFF)

/* 操作采样上报寄存器 0x33C */
typedef union {
    struct {
        unsigned int pcie_mst_awaddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt10;
#define PCIE_CTRL_RB_MONITOR_RPT10_OFF    0x33C
#define PCIE_CTRL_RB_MONITOR_RPT10_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT10_OFF)

/* 操作采样上报寄存器 0x340 */
typedef union {
    struct {
        unsigned int pcie_mst_awaddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt12;
#define PCIE_CTRL_RB_MONITOR_RPT12_OFF    0x340
#define PCIE_CTRL_RB_MONITOR_RPT12_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT12_OFF)

/* 操作采样上报寄存器 0x344 */
typedef union {
    struct {
        unsigned int pcie_mst_wdata0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt14;
#define PCIE_CTRL_RB_MONITOR_RPT14_OFF    0x344
#define PCIE_CTRL_RB_MONITOR_RPT14_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT14_OFF)

/* 操作采样上报寄存器 0x348 */
typedef union {
    struct {
        unsigned int pcie_mst_wdata1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt15;
#define PCIE_CTRL_RB_MONITOR_RPT15_OFF    0x348
#define PCIE_CTRL_RB_MONITOR_RPT15_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT15_OFF)

/* 操作采样上报寄存器 0x34C */
typedef union {
    struct {
        unsigned int pcie_mst_araddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt17;
#define PCIE_CTRL_RB_MONITOR_RPT17_OFF    0x34C
#define PCIE_CTRL_RB_MONITOR_RPT17_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT17_OFF)

/* 操作采样上报寄存器 0x350 */
typedef union {
    struct {
        unsigned int pcie_mst_araddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt18;
#define PCIE_CTRL_RB_MONITOR_RPT18_OFF    0x350
#define PCIE_CTRL_RB_MONITOR_RPT18_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT18_OFF)

/* 操作采样上报寄存器 0x354 */
typedef union {
    struct {
        unsigned int pcie_mst_rdata0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt20;
#define PCIE_CTRL_RB_MONITOR_RPT20_OFF    0x354
#define PCIE_CTRL_RB_MONITOR_RPT20_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT20_OFF)

/* 操作采样上报寄存器 0x358 */
typedef union {
    struct {
        unsigned int pcie_mst_rdata1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt21;
#define PCIE_CTRL_RB_MONITOR_RPT21_OFF    0x358
#define PCIE_CTRL_RB_MONITOR_RPT21_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT21_OFF)

/* 操作采样上报寄存器 0x35C */
typedef union {
    struct {
        unsigned int pcie_slv_arid0 : 13; /* 0:12 */
        unsigned int reserved0 : 19; /* 13:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt22;
#define PCIE_CTRL_RB_MONITOR_RPT22_OFF    0x35C
#define PCIE_CTRL_RB_MONITOR_RPT22_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT22_OFF)

/* 操作采样上报寄存器 0x360 */
typedef union {
    struct {
        unsigned int pcie_slv_awid0 : 13; /* 0:12 */
        unsigned int reserved0 : 19; /* 13:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_rpt23;
#define PCIE_CTRL_RB_MONITOR_RPT23_OFF    0x360
#define PCIE_CTRL_RB_MONITOR_RPT23_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_RPT23_OFF)

/* 操作采样配置寄存器 0x364 */
typedef union {
    struct {
        unsigned int pcie_monitor_enable : 1; /* 0 */
        unsigned int pcie_monitor_mode : 1; /* 1 */
        unsigned int reserved0 : 2; /* 2:3 */
        unsigned int pcie_monitor_clr : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_cfg0;
#define PCIE_CTRL_RB_MONITOR_CFG0_OFF    0x364
#define PCIE_CTRL_RB_MONITOR_CFG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_CFG0_OFF)

/* 性能统计配置寄存器 0x368 */
typedef union {
    struct {
        unsigned int wr_slv_wdata_counter_en : 1; /* 0 */
        unsigned int wr_slv_rdata_counter_en : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_delay_monitor_cfg;
#define PCIE_CTRL_RB_DELAY_MONITOR_CFG_OFF    0x368
#define PCIE_CTRL_RB_DELAY_MONITOR_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_DELAY_MONITOR_CFG_OFF)

/* DELAY采样上报寄存器 0x36C */
typedef union {
    struct {
        unsigned int slv_wdata_latency : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_wdata_latency;
#define PCIE_CTRL_RB_SLV_WDATA_LATENCY_OFF    0x36C
#define PCIE_CTRL_RB_SLV_WDATA_LATENCY_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_WDATA_LATENCY_OFF)

/* DELAY采样上报寄存器 0x370 */
typedef union {
    struct {
        unsigned int slv_rdata_latency : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_slv_rdata_latency;
#define PCIE_CTRL_RB_SLV_RDATA_LATENCY_OFF    0x370
#define PCIE_CTRL_RB_SLV_RDATA_LATENCY_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_SLV_RDATA_LATENCY_OFF)

/*  0x3B0 */
typedef union {
    struct {
        unsigned int soc_stat_cnt_ltssm_enter_rcvry : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_stat_cnt_rcvry;
#define PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_OFF    0x3B0
#define PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_OFF)

/*  0x3B4 */
typedef union {
    struct {
        unsigned int soc_stat_cnt_l1_enter_rcvry : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_stat_cnt_l1_rcvry;
#define PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_OFF    0x3B4
#define PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_OFF)

/*  0x3B8 */
typedef union {
    struct {
        unsigned int soc_pcie_stat_monitor_s1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_stat_monitor1;
#define PCIE_CTRL_RB_PCIE_STAT_MONITOR1_OFF    0x3B8
#define PCIE_CTRL_RB_PCIE_STAT_MONITOR1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STAT_MONITOR1_OFF)

/*  0x3BC */
typedef union {
    struct {
        unsigned int soc_pcie_stat_monitor_s2 : 4; /* 0:3 */
        unsigned int reserved0 : 8; /* 4:11 */
        unsigned int soc_ltssm_stat_samp_en : 1; /* 12 */
        unsigned int soc_ltssm_stat_flag_clr : 1; /* 13 */
        unsigned int reserved1 : 2; /* 14:15 */
        unsigned int soc_ltssm_stat_flag : 1; /* 16 */
        unsigned int reserved2 : 3; /* 17:19 */
        unsigned int soc_stat_cnt_ltssm_enter_rcvry_clr : 1; /* 20 */
        unsigned int soc_stat_cnt_l1_enter_rcvry_clr : 1; /* 21 */
        unsigned int reserved3 : 10; /* 22:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_stat_monitor2;
#define PCIE_CTRL_RB_PCIE_STAT_MONITOR2_OFF    0x3BC
#define PCIE_CTRL_RB_PCIE_STAT_MONITOR2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STAT_MONITOR2_OFF)

/*  0x3C0 */
typedef union {
    struct {
        unsigned int soc_xfer_pending_timer : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_xfer_pending_timer;
#define PCIE_CTRL_RB_PCIE_XFER_PENDING_TIMER_OFF    0x3C0
#define PCIE_CTRL_RB_PCIE_XFER_PENDING_TIMER_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_XFER_PENDING_TIMER_OFF)

/*  0x3C4 */
typedef union {
    struct {
        unsigned int soc_xfer_pending_timer_en : 1; /* 0 */
        unsigned int reserved0 : 3; /* 1:3 */
        unsigned int soc_cpl_timeout_en : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_xfer_pending_ctrl;
#define PCIE_CTRL_RB_PCIE_XFER_PENDING_CTRL_OFF    0x3C4
#define PCIE_CTRL_RB_PCIE_XFER_PENDING_CTRL_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_XFER_PENDING_CTRL_OFF)

/* PCIE PHY配置寄存器1 0x400 */
typedef union {
    struct {
        unsigned int pcie_pcs_tx_deemph_gen2_3p5db : 6; /* 0:5 */
        unsigned int reserved0 : 26; /* 6:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config1;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG1_OFF    0x400
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG1_OFF)

/* PCIE PHY配置寄存器2 0x404 */
typedef union {
    struct {
        unsigned int pcie_pcs_tx_swing_low : 7; /* 0:6 */
        unsigned int reserved0 : 25; /* 7:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config2;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG2_OFF    0x404
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG2_OFF)

/* PCIE PHY配置寄存器3 0x408 */
typedef union {
    struct {
        unsigned int pcie_pcs_tx_deemph_gen2_6db : 6; /* 0:5 */
        unsigned int reserved0 : 26; /* 6:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config3;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG3_OFF    0x408
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG3_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG3_OFF)

/* PCIE PHY配置寄存器4 0x40C */
typedef union {
    struct {
        unsigned int pcie_pcs_tx_deemph_gen1 : 6; /* 0:5 */
        unsigned int reserved0 : 26; /* 6:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config4;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG4_OFF    0x40C
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG4_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG4_OFF)

/* PCIE PHY配置寄存器5 0x410 */
typedef union {
    struct {
        unsigned int pcie_pcs_tx_swing_full : 7; /* 0:6 */
        unsigned int pcie_phy_ref_clkdiv2 : 1; /* 7 */
        unsigned int pcie_vreg_bypass : 1; /* 8 */
        unsigned int reserved0 : 23; /* 9:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config5;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG5_OFF    0x410
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG5_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG5_OFF)

/* PCIE PHY配置寄存器6 0x414 */
typedef union {
    struct {
        unsigned int pcie_pcs_common_clocks : 1; /* 0 */
        unsigned int pcie_phy_tx0_term_offset : 5; /* 1:5 */
        unsigned int clkreq_oen : 1; /* 6 */
        unsigned int pcie_phy_mpll_multiplier : 7; /* 7:13 */
        unsigned int reserved0 : 18; /* 14:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config6;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG6_OFF    0x414
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG6_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG6_OFF)

/* PCIE PHY配置寄存器7 0x418 */
typedef union {
    struct {
        unsigned int pcie_phy_los_bias : 3; /* 0:2 */
        unsigned int pcie_phy_los_level : 5; /* 3:7 */
        unsigned int pcie_phy_rx0_eq : 3; /* 8:10 */
        unsigned int pcie_phy_tx_vboost_lvl : 3; /* 11:13 */
        unsigned int reserved0 : 18; /* 14:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_config7;
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG7_OFF    0x418
#define PCIE_CTRL_RB_PCIE_PHY_CONFIG7_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CONFIG7_OFF)

/* PCIE PHY配置地址寄存器 0x41C */
typedef union {
    struct {
        unsigned int phy_cr_addr : 16; /* 0:15 */
        unsigned int reserved0 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_cfg_addr;
#define PCIE_CTRL_RB_PCIE_PHY_CFG_ADDR_OFF    0x41C
#define PCIE_CTRL_RB_PCIE_PHY_CFG_ADDR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CFG_ADDR_OFF)

/* PCIE PHY数据寄存器 0x420 */
typedef union {
    struct {
        unsigned int phy_cr_wr_data : 16; /* 0:15 */
        unsigned int reserved0 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_wr_data;
#define PCIE_CTRL_RB_PCIE_PHY_WR_DATA_OFF    0x420
#define PCIE_CTRL_RB_PCIE_PHY_WR_DATA_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_WR_DATA_OFF)

/* PCIE PHY数据寄存器 0x424 */
typedef union {
    struct {
        unsigned int phy_cr_rd_data : 16; /* 0:15 */
        unsigned int reserved0 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_rd_data;
#define PCIE_CTRL_RB_PCIE_PHY_RD_DATA_OFF    0x424
#define PCIE_CTRL_RB_PCIE_PHY_RD_DATA_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_RD_DATA_OFF)

/* PCIE PHY配置控制寄存器 0x428 */
typedef union {
    struct {
        unsigned int phy_config_en : 1; /* 0 */
        unsigned int reserved0 : 1; /* 1 */
        unsigned int phy_write : 1; /* 2 */
        unsigned int reserved1 : 12; /* 3:14 */
        unsigned int phy_config_rdy : 1; /* 15 */
        unsigned int reserved2 : 16; /* 16:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_cfg1;
#define PCIE_CTRL_RB_PCIE_PHY_CFG1_OFF    0x428
#define PCIE_CTRL_RB_PCIE_PHY_CFG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CFG1_OFF)

/* PCIE PCS EN 配置 0x42C */
typedef union {
    struct {
        unsigned int soc_phy_txdetectrx_loopback_en0 : 1; /* 0 */
        unsigned int soc_phy_txdetectrx_loopback_en1 : 1; /* 1 */
        unsigned int reserved0 : 2; /* 2:3 */
        unsigned int soc_phy_txdetectrx_loopback_sel : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_en_cfg;
#define PCIE_CTRL_RB_PCIE_EN_CFG_OFF    0x42C
#define PCIE_CTRL_RB_PCIE_EN_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_EN_CFG_OFF)

/* PHY配置寄存器 0x430 */
typedef union {
    struct {
        unsigned int soc_mac_phy_txdeemph0 : 18; /* 0:17 */
        unsigned int reserved0 : 14; /* 18:31 */
    } bits;
    unsigned int as_dword;
} hreg_phy_cfg1;
#define PCIE_CTRL_RB_PHY_CFG1_OFF    0x430
#define PCIE_CTRL_RB_PHY_CFG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PHY_CFG1_OFF)

/* PHY配置寄存器 0x434 */
typedef union {
    struct {
        unsigned int soc_mac_phy_txmargin : 3; /* 0:2 */
        unsigned int soc_mac_phy_txmargin_sel : 1; /* 3 */
        unsigned int soc_mac_phy_txswing : 1; /* 4 */
        unsigned int soc_mac_phy_txswing_sel : 1; /* 5 */
        unsigned int reserved0 : 2; /* 6:7 */
        unsigned int soc_mac_phy_txdeemph_sel : 1; /* 8 */
        unsigned int reserved1 : 3; /* 9:11 */
        unsigned int soc_mac_phy_txdeemph1 : 18; /* 12:29 */
        unsigned int reserved2 : 2; /* 30:31 */
    } bits;
    unsigned int as_dword;
} hreg_phy_cfg2;
#define PCIE_CTRL_RB_PHY_CFG2_OFF    0x434
#define PCIE_CTRL_RB_PHY_CFG2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PHY_CFG2_OFF)

/* pcie维测选择寄存器 0x438 */
typedef union {
    struct {
        unsigned int pcie_diag_sel : 4; /* 0:3 */
        unsigned int reserved0 : 28; /* 4:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_diag_sel;
#define PCIE_CTRL_RB_PCIE_DIAG_SEL_OFF    0x438
#define PCIE_CTRL_RB_PCIE_DIAG_SEL_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_DIAG_SEL_OFF)

/* pcie维测寄存器2 0x43C */
typedef union {
    struct {
        unsigned int pcie_mac_phy_powerdown : 4; /* 0:3 */
        unsigned int pcie_phy_mac_rxelecidle : 2; /* 4:5 */
        unsigned int reserved0 : 2; /* 6:7 */
        unsigned int pcie_phy_mac_rxstatus : 6; /* 8:13 */
        unsigned int reserved1 : 2; /* 14:15 */
        unsigned int pcie_mac_phy_txelecidle : 2; /* 16:17 */
        unsigned int reserved2 : 2; /* 18:19 */
        unsigned int pcie_mac_phy_rate : 2; /* 20:21 */
        unsigned int reserved3 : 2; /* 22:23 */
        unsigned int pcie_phy_mac_phystatus : 2; /* 24:25 */
        unsigned int reserved4 : 2; /* 26:27 */
        unsigned int pcie_mac_phy_clk_req_n : 1; /* 28 */
        unsigned int pcie_phy_mac_pclkack_n : 1; /* 29 */
        unsigned int reserved5 : 2; /* 30:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status2;
#define PCIE_CTRL_RB_PCIE_STATUS2_OFF    0x43C
#define PCIE_CTRL_RB_PCIE_STATUS2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS2_OFF)

/* pcie维测寄存器3 0x440 */
typedef union {
    struct {
        unsigned int pcie_pipe0_rx_elecidle_disable : 1; /* 0 */
        unsigned int pcie_pipe0_tx_common_mode_disable : 1; /* 1 */
        unsigned int pcie_mac0_pclkreq_n : 2; /* 2:3 */
        unsigned int reserved0 : 28; /* 4:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status3;
#define PCIE_CTRL_RB_PCIE_STATUS3_OFF    0x440
#define PCIE_CTRL_RB_PCIE_STATUS3_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS3_OFF)

/*  0x44C */
typedef union {
    struct {
        unsigned int soc_pm_l1_entry_started : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status4;
#define PCIE_CTRL_RB_PCIE_STATUS4_OFF    0x44C
#define PCIE_CTRL_RB_PCIE_STATUS4_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS4_OFF)

/* pcie维测寄存器12 0x464 */
typedef union {
    struct {
        unsigned int pcie_phy_mpll_en : 1; /* 0 */
        unsigned int pcie_phy_tx0_state : 1; /* 1 */
        unsigned int pcie_phy_ref_ssp_en : 1; /* 2 */
        unsigned int pcie_phy_rx0_pll_state : 1; /* 3 */
        unsigned int pcie_phy_mpll_state : 1; /* 4 */
        unsigned int pcie_phy_mpll_word_clk_en : 1; /* 5 */
        unsigned int pcie_phy_mpll_dword_clk_en : 1; /* 6 */
        unsigned int pcie_phy_mpll_qword_clk_en : 1; /* 7 */
        unsigned int pcie_phy_rx0_pll_en : 1; /* 8 */
        unsigned int reserved0 : 23; /* 9:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_status12;
#define PCIE_CTRL_RB_PCIE_STATUS12_OFF    0x464
#define PCIE_CTRL_RB_PCIE_STATUS12_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_STATUS12_OFF)

/* pcie tx abort消息上报 0x47C */
typedef union {
    struct {
        unsigned int tx_app_wr_err : 2; /* 0:1 */
        unsigned int reserved0 : 2; /* 2:3 */
        unsigned int tx_ll_fetch_err : 2; /* 4:5 */
        unsigned int reserved1 : 2; /* 6:7 */
        unsigned int tx_unsupport_err : 2; /* 8:9 */
        unsigned int reserved2 : 2; /* 10:11 */
        unsigned int tx_cpl_abort : 2; /* 12:13 */
        unsigned int reserved3 : 2; /* 14:15 */
        unsigned int tx_cpl_timeout : 2; /* 16:17 */
        unsigned int reserved4 : 2; /* 18:19 */
        unsigned int tx_data_poision : 2; /* 20:21 */
        unsigned int reserved5 : 2; /* 22:23 */
        unsigned int reserved6 : 8; /* 24:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_tx_abort_status;
#define PCIE_CTRL_RB_PCIE_TX_ABORT_STATUS_OFF    0x47C
#define PCIE_CTRL_RB_PCIE_TX_ABORT_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_TX_ABORT_STATUS_OFF)

/* pcie rx abort消息上报 0x480 */
typedef union {
    struct {
        unsigned int rx_app_wr_err : 2; /* 0:1 */
        unsigned int reserved0 : 2; /* 2:3 */
        unsigned int rx_ll_fetch_err : 2; /* 4:5 */
        unsigned int reserved1 : 2; /* 6:7 */
        unsigned int reserved2 : 24; /* 8:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_rx_abort_status;
#define PCIE_CTRL_RB_PCIE_RX_ABORT_STATUS_OFF    0x480
#define PCIE_CTRL_RB_PCIE_RX_ABORT_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_RX_ABORT_STATUS_OFF)

/*  0x484 */
typedef union {
    struct {
        unsigned int reserved0 : 4; /* 0:3 */
        unsigned int soc_pipe_lane_pclk_sel : 1; /* 4 */
        unsigned int reserved1 : 3; /* 5:7 */
        unsigned int soc_pipe_lane_tx2rx_loopbk : 2; /* 8:9 */
        unsigned int reserved2 : 2; /* 10:11 */
        unsigned int soc_pipe_rx_termination : 2; /* 12:13 */
        unsigned int soc_pipe_rx_termination_sel : 1; /* 14 */
        unsigned int reserved3 : 1; /* 15 */
        unsigned int soc_pipe_rx_cmn_refclk_mode : 2; /* 16:17 */
        unsigned int reserved4 : 14; /* 18:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_clk_sel_cfg;
#define PCIE_CTRL_RB_PCIE_PHY_CLK_SEL_CFG_OFF    0x484
#define PCIE_CTRL_RB_PCIE_PHY_CLK_SEL_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CLK_SEL_CFG_OFF)

/* TX SWING手动使能 0x500 */
typedef union {
    struct {
        unsigned int pcs_tx_swing_sel_en : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcs_tx_swing_sel_en;
#define PCIE_CTRL_RB_PCS_TX_SWING_SEL_EN_OFF    0x500
#define PCIE_CTRL_RB_PCS_TX_SWING_SEL_EN_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCS_TX_SWING_SEL_EN_OFF)

/*  0x534 */
typedef union {
    struct {
        unsigned int pcie_max_awaddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_max_awaddr_cfg00;
#define PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG00_OFF    0x534
#define PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG00_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG00_OFF)

/*  0x538 */
typedef union {
    struct {
        unsigned int pcie_max_awaddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_max_awaddr_cfg01;
#define PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG01_OFF    0x538
#define PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG01_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAX_AWADDR_CFG01_OFF)

/*  0x53C */
typedef union {
    struct {
        unsigned int pcie_min_awaddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_min_awaddr_cfg00;
#define PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG00_OFF    0x53C
#define PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG00_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG00_OFF)

/*  0x540 */
typedef union {
    struct {
        unsigned int pcie_min_awaddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_min_awaddr_cfg01;
#define PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG01_OFF    0x540
#define PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG01_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MIN_AWADDR_CFG01_OFF)

/*  0x544 */
typedef union {
    struct {
        unsigned int pcie_max_araddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_max_araddr_cfg00;
#define PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG00_OFF    0x544
#define PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG00_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG00_OFF)

/*  0x548 */
typedef union {
    struct {
        unsigned int pcie_max_araddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_max_araddr_cfg01;
#define PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG01_OFF    0x548
#define PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG01_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAX_ARADDR_CFG01_OFF)

/*  0x54C */
typedef union {
    struct {
        unsigned int pcie_min_araddr0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_min_araddr_cfg00;
#define PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG00_OFF    0x54C
#define PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG00_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG00_OFF)

/*  0x550 */
typedef union {
    struct {
        unsigned int pcie_min_araddr1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_min_araddr_cfg01;
#define PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG01_OFF    0x550
#define PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG01_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MIN_ARADDR_CFG01_OFF)

/*  0x554 */
typedef union {
    struct {
        unsigned int pcie_awaddr_sts0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_lmi_awaddr_sts0;
#define PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS0_OFF    0x554
#define PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS0_OFF)

/*  0x558 */
typedef union {
    struct {
        unsigned int pcie_awaddr_sts1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_lmi_awaddr_sts1;
#define PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS1_OFF    0x558
#define PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_LMI_AWADDR_STS1_OFF)

/*  0x55C */
typedef union {
    struct {
        unsigned int pcie_araddr_sts0 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_lmi_araddr_sts0;
#define PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS0_OFF    0x55C
#define PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS0_OFF)

/*  0x560 */
typedef union {
    struct {
        unsigned int pcie_araddr_sts1 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_lmi_araddr_sts1;
#define PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS1_OFF    0x560
#define PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_LMI_ARADDR_STS1_OFF)

/*  0x564 */
typedef union {
    struct {
        unsigned int pcie_awchan_valid : 1; /* 0 */
        unsigned int pcie_archan_valid : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_cfg_sts;
#define PCIE_CTRL_RB_PCIE_CFG_STS_OFF    0x564
#define PCIE_CTRL_RB_PCIE_CFG_STS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CFG_STS_OFF)

/*  0x568 */
typedef union {
    struct {
        unsigned int pcie_mstr_clr : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_cfg_clr;
#define PCIE_CTRL_RB_PCIE_CFG_CLR_OFF    0x568
#define PCIE_CTRL_RB_PCIE_CFG_CLR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_CFG_CLR_OFF)

/*  0x56C */
typedef union {
    struct {
        unsigned int bus_monitor_enable : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_bus_monitor_enable;
#define PCIE_CTRL_RB_BUS_MONITOR_ENABLE_OFF    0x56C
#define PCIE_CTRL_RB_BUS_MONITOR_ENABLE_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_BUS_MONITOR_ENABLE_OFF)

/*  0x570 */
typedef union {
    struct {
        unsigned int wr_cnt : 6; /* 0:5 */
        unsigned int reserved0 : 2; /* 6:7 */
        unsigned int rd_cnt : 6; /* 8:13 */
        unsigned int reserved1 : 18; /* 14:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_axi_cnt_rpt;
#define PCIE_CTRL_RB_HOST_AXI_CNT_RPT_OFF    0x570
#define PCIE_CTRL_RB_HOST_AXI_CNT_RPT_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_AXI_CNT_RPT_OFF)

/*  0x600 */
typedef union {
    struct {
        unsigned int soc_phy_por_n : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_poweron_cfg;
#define PCIE_CTRL_RB_PCIE_PHY_POWERON_CFG_OFF    0x600
#define PCIE_CTRL_RB_PCIE_PHY_POWERON_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_POWERON_CFG_OFF)

/*  0x604 */
typedef union {
    struct {
        unsigned int soc_phy_reset_n : 1; /* 0 */
        unsigned int reserved0 : 3; /* 1:3 */
        unsigned int soc_phy_reset_sel : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_reset_cfg;
#define PCIE_CTRL_RB_PCIE_PHY_RESET_CFG_OFF    0x604
#define PCIE_CTRL_RB_PCIE_PHY_RESET_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_RESET_CFG_OFF)

/*  0x608 */
typedef union {
    struct {
        unsigned int soc_sym_align_en : 1; /* 0 */
        unsigned int soc_sym_align_en_sel : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_sym_align_en_ctl;
#define PCIE_CTRL_RB_PCIE_SYM_ALIGN_EN_CTL_OFF    0x608
#define PCIE_CTRL_RB_PCIE_SYM_ALIGN_EN_CTL_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_SYM_ALIGN_EN_CTL_OFF)

/*  0x610 */
typedef union {
    struct {
        unsigned int soc_jtag_protect : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_jtag_mode_protect;
#define PCIE_CTRL_RB_PCIE_JTAG_MODE_PROTECT_OFF    0x610
#define PCIE_CTRL_RB_PCIE_JTAG_MODE_PROTECT_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_JTAG_MODE_PROTECT_OFF)

/*  0x614 */
typedef union {
    struct {
        unsigned int soc_pcie_phy_reset_n : 1; /* 0 */
        unsigned int reserved0 : 3; /* 1:3 */
        unsigned int soc_pcie_phy_reset_bypass : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_reset_bypass;
#define PCIE_CTRL_RB_PCIE_PHY_RESET_BYPASS_OFF    0x614
#define PCIE_CTRL_RB_PCIE_PHY_RESET_BYPASS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_RESET_BYPASS_OFF)

/*  0x618 */
typedef union {
    struct {
        unsigned int soc_pcie_phy_apb_reset_n : 1; /* 0 */
        unsigned int reserved0 : 3; /* 1:3 */
        unsigned int soc_pcie_phy_apb_reset_bypass : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_apb_reset_bypass;
#define PCIE_CTRL_RB_PCIE_APB_RESET_BYPASS_OFF    0x618
#define PCIE_CTRL_RB_PCIE_APB_RESET_BYPASS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_APB_RESET_BYPASS_OFF)

/*  0x61C */
typedef union {
    struct {
        unsigned int pcie_phy_por_n_status : 1; /* 0 */
        unsigned int pcie_phy_prst_n_status : 1; /* 1 */
        unsigned int pcie_phy_reset_n_status : 1; /* 2 */
        unsigned int reserved0 : 29; /* 3:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_rst_n_status;
#define PCIE_CTRL_RB_PCIE_PHY_RST_N_STATUS_OFF    0x61C
#define PCIE_CTRL_RB_PCIE_PHY_RST_N_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_RST_N_STATUS_OFF)

/*  0x630 */
typedef union {
    struct {
        unsigned int reserve_phy_eco_in : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_reserve_phy_eco_in;
#define PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_IN_OFF    0x630
#define PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_IN_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_IN_OFF)

/*  0x634 */
typedef union {
    struct {
        unsigned int reserve_phy_eco_out : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_reserve_phy_eco_out;
#define PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_OUT_OFF    0x634
#define PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_OUT_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_RESERVE_PHY_ECO_OUT_OFF)

/*  0x638 */
typedef union {
    struct {
        unsigned int pcs_clk_en_sel : 1; /* 0 */
        unsigned int reserved0 : 3; /* 1:3 */
        unsigned int pcs_clk_en_manual : 1; /* 4 */
        unsigned int reserved1 : 27; /* 5:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_pcs_cg_ctl;
#define PCIE_CTRL_RB_PCIE_PHY_PCS_CG_CTL_OFF    0x638
#define PCIE_CTRL_RB_PCIE_PHY_PCS_CG_CTL_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_PCS_CG_CTL_OFF)

/*  0x640 */
typedef union {
    struct {
        unsigned int ete_clk_en_auto_cg_bypass : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_ete_clk_en_auto_cg_bypass;
#define PCIE_CTRL_RB_ETE_CLK_EN_AUTO_CG_BYPASS_OFF    0x640
#define PCIE_CTRL_RB_ETE_CLK_EN_AUTO_CG_BYPASS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_ETE_CLK_EN_AUTO_CG_BYPASS_OFF)

/*  0x644 */
typedef union {
    struct {
        unsigned int soft_pcie_phy_load_en : 1; /* 0 */
        unsigned int pcie_phy_cfg_sts : 3; /* 1:3 */
        unsigned int soft_pcie_phy_reload_en : 1; /* 4 */
        unsigned int pcie_phy_prst_n_sts : 1; /* 5 */
        unsigned int reserved0 : 26; /* 6:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_phy_cfg;
#define PCIE_CTRL_RB_PCIE_PHY_CFG_OFF    0x644
#define PCIE_CTRL_RB_PCIE_PHY_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PHY_CFG_OFF)

/*  0x648 */
typedef union {
    struct {
        unsigned int host_ete_rx_intr_status : 1; /* 0 */
        unsigned int host_ete_tx_intr_status : 1; /* 1 */
        unsigned int sync_rx_ch_dr_empty_intr_status : 1; /* 2 */
        unsigned int sync_tx_ch_dr_empty_intr_status : 1; /* 3 */
        unsigned int reserved0 : 28; /* 4:31 */
    } bits;
    unsigned int as_dword;
} hreg_ete_intr_status;
#define PCIE_CTRL_RB_ETE_INTR_STATUS_OFF    0x648
#define PCIE_CTRL_RB_ETE_INTR_STATUS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_ETE_INTR_STATUS_OFF)

/* HOST_CRG_PCIE_CLKEN 0x700 */
typedef union {
    struct {
        unsigned int cfg_pcie_pcs_man_clken : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_clken;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_CLKEN_OFF    0x700
#define PCIE_CTRL_RB_HOST_CRG_PCIE_CLKEN_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_CLKEN_OFF)

/* HOST_CRG_PCIE_SOFT_RST_INT_CFG 0x704 */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_int_cfg_n : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_int_cfg;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_INT_CFG_OFF    0x704
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_INT_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_INT_CFG_OFF)

/* HOST_CRG_PCIE_SOFT_RST_PHY_POR 0x708 */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_pcie_phy_por_n : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_phy_por;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_POR_OFF    0x708
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_POR_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_POR_OFF)

/* HOST_CRG_PCIE_SOFT_RST_PROTECT 0x70c */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_pcie_protect_n : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_protect;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PROTECT_OFF    0x70c
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PROTECT_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PROTECT_OFF)

/* HOST_CRG_PCIE_SOFT_RST_PHY_FRM_MAC 0x710 */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_phy_mac_reset_sel : 1; /* 0 */
        unsigned int cfg_pcie_soft_rst_phy_mac_reset_n : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_phy_frm_mac;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_FRM_MAC_OFF    0x710
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_FRM_MAC_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_FRM_MAC_OFF)

/* HOST_CRG_PCIE_SOFT_RST_PHY_PRESET 0x714 */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_phy_preset_sel : 1; /* 0 */
        unsigned int cfg_pcie_soft_rst_phy_preset_n : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_phy_preset;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_PRESET_OFF    0x714
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_PRESET_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_PRESET_OFF)

/* HOST_CRG_PCIE_SOFT_RST_PHY 0x718 */
typedef union {
    struct {
        unsigned int cfg_pcie_soft_rst_phy_reset_sel : 1; /* 0 */
        unsigned int cfg_pcie_soft_rst_phy_reset_n : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_soft_rst_phy;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_OFF    0x718
#define PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_SOFT_RST_PHY_OFF)

/* HOST_CRG_PCIE_AUTOCG_BYPASS 0x71c */
typedef union {
    struct {
        unsigned int cfg_pcie_pcs_autocg_bypass : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_host_crg_pcie_autocg_bypass;
#define PCIE_CTRL_RB_HOST_CRG_PCIE_AUTOCG_BYPASS_OFF    0x71c
#define PCIE_CTRL_RB_HOST_CRG_PCIE_AUTOCG_BYPASS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_HOST_CRG_PCIE_AUTOCG_BYPASS_OFF)

/*  0x750 */
typedef union {
    struct {
        unsigned int soc_ltssm_state : 6; /* 0:5 */
        unsigned int linkdown_sts : 1; /* 6 */
        unsigned int mac_phy_pclk_rate : 3; /* 7:9 */
        unsigned int bus_master_en_sts : 1; /* 10 */
        unsigned int reserved0 : 21; /* 11:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_sts;
#define PCIE_CTRL_RB_PCIE_MAC_STS_OFF    0x750
#define PCIE_CTRL_RB_PCIE_MAC_STS_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_STS_OFF)

/*  0x724 */
typedef union {
    struct {
        unsigned int djtag_ns_allow : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_cfg0;
#define PCIE_CTRL_RB_PCIE_MAC_CFG0_OFF    0x724
#define PCIE_CTRL_RB_PCIE_MAC_CFG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_CFG0_OFF)

/*  0x758 */
typedef union {
    struct {
        unsigned int cldo3_pcieslv_rs_auto_cg_bypass : 1; /* 0 */
        unsigned int cldo3_pciemst_rs_auto_cg_bypass : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_cfg1;
#define PCIE_CTRL_RB_PCIE_MAC_CFG1_OFF    0x758
#define PCIE_CTRL_RB_PCIE_MAC_CFG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_CFG1_OFF)

/*  0x75c */
typedef union {
    struct {
        unsigned int soc_phy2pcie_pipe_refclk_req_n : 1; /* 0 */
        unsigned int soc_phy2pcie_pipe_refclk_req_n_sel : 1; /* 1 */
        unsigned int reserved0 : 30; /* 2:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_mac_cfg2;
#define PCIE_CTRL_RB_PCIE_MAC_CFG2_OFF    0x75c
#define PCIE_CTRL_RB_PCIE_MAC_CFG2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_MAC_CFG2_OFF)

/* 操作采样上报寄存器 0x760 */
typedef union {
    struct {
        unsigned int pcie_slv_wdata2 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_0;
#define PCIE_CTRL_RB_MONITOR_0_OFF    0x760
#define PCIE_CTRL_RB_MONITOR_0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_0_OFF)

/* 操作采样上报寄存器 0x764 */
typedef union {
    struct {
        unsigned int pcie_slv_wdata3 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_1;
#define PCIE_CTRL_RB_MONITOR_1_OFF    0x764
#define PCIE_CTRL_RB_MONITOR_1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_1_OFF)

/* 操作采样上报寄存器 0x768 */
typedef union {
    struct {
        unsigned int pcie_slv_rdata2 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_2;
#define PCIE_CTRL_RB_MONITOR_2_OFF    0x768
#define PCIE_CTRL_RB_MONITOR_2_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_2_OFF)

/* 操作采样上报寄存器 0x76C */
typedef union {
    struct {
        unsigned int pcie_slv_rdata3 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_3;
#define PCIE_CTRL_RB_MONITOR_3_OFF    0x76C
#define PCIE_CTRL_RB_MONITOR_3_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_3_OFF)

/* 操作采样上报寄存器 0x770 */
typedef union {
    struct {
        unsigned int pcie_mst_wdata2 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_4;
#define PCIE_CTRL_RB_MONITOR_4_OFF    0x770
#define PCIE_CTRL_RB_MONITOR_4_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_4_OFF)

/* 操作采样上报寄存器 0x774 */
typedef union {
    struct {
        unsigned int pcie_mst_wdata3 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_5;
#define PCIE_CTRL_RB_MONITOR_5_OFF    0x774
#define PCIE_CTRL_RB_MONITOR_5_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_5_OFF)

/* 操作采样上报寄存器 0x778 */
typedef union {
    struct {
        unsigned int pcie_mst_rdata2 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_6;
#define PCIE_CTRL_RB_MONITOR_6_OFF    0x778
#define PCIE_CTRL_RB_MONITOR_6_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_6_OFF)

/* 操作采样上报寄存器 0x77C */
typedef union {
    struct {
        unsigned int pcie_mst_rdata3 : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_monitor_7;
#define PCIE_CTRL_RB_MONITOR_7_OFF    0x77C
#define PCIE_CTRL_RB_MONITOR_7_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MONITOR_7_OFF)

/* PCIE MEM POWER MODE 0x780 */
typedef union {
    struct {
        unsigned int mem_power_mode : 3; /* 0:2 */
        unsigned int reserved0 : 29; /* 3:31 */
    } bits;
    unsigned int as_dword;
} hreg_mem_power_mode;
#define PCIE_CTRL_RB_MEM_POWER_MODE_OFF    0x780
#define PCIE_CTRL_RB_MEM_POWER_MODE_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MEM_POWER_MODE_OFF)

/*  0x78C */
typedef union {
    struct {
        unsigned int soc_int_req_pf_num : 5; /* 0:4 */
        unsigned int soc_int_req_vf_num : 8; /* 5:12 */
        unsigned int soc_int_vector : 11; /* 13:23 */
        unsigned int reserved0 : 8; /* 24:31 */
    } bits;
    unsigned int as_dword;
} hreg_intx_cfg;
#define PCIE_CTRL_RB_INTX_CFG_OFF    0x78C
#define PCIE_CTRL_RB_INTX_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_INTX_CFG_OFF)

/*  0x790 */
typedef union {
    struct {
        unsigned int msi_mask_sts : 32; /* 0:31 */
    } bits;
    unsigned int as_dword;
} hreg_msi_cfg0;
#define PCIE_CTRL_RB_MSI_CFG0_OFF    0x790
#define PCIE_CTRL_RB_MSI_CFG0_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MSI_CFG0_OFF)

/*  0x794 */
typedef union {
    struct {
        unsigned int msi_enable_sts : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_msi_cfg1;
#define PCIE_CTRL_RB_MSI_CFG1_OFF    0x794
#define PCIE_CTRL_RB_MSI_CFG1_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_MSI_CFG1_OFF)

/*  0x7A4 */
typedef union {
    struct {
        unsigned int pcie_protect_en_cfg : 1; /* 0 */
        unsigned int reserved0 : 31; /* 1:31 */
    } bits;
    unsigned int as_dword;
} hreg_pcie_protect_cfg;
#define PCIE_CTRL_RB_PCIE_PROTECT_CFG_OFF    0x7A4
#define PCIE_CTRL_RB_PCIE_PROTECT_CFG_REG \
    (PCIE_CTRL_RB_BASE + PCIE_CTRL_RB_PCIE_PROTECT_CFG_OFF)

#endif
