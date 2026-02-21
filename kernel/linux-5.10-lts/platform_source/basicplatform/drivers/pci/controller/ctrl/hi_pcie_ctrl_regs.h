/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller regs define.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_REGS_H
#define HI_PCIE_CTRL_REGS_H

// AP REGISTER CTRL MOMERYMAP
#define AP_IOB_TX_REG	0
#define AP_IOB_RX_REG	0x4000
#define AP_APAT_REG		0x7000
#define AP_GLOBAL_REG	0x8000
#define AP_MCTP_REG		0xA000
#define AP_ENGINE_REG	0xB000
#define AP_INT_REG		0xC000
#define AP_DMA_REG		0x10000
#define AP_P2P_REG		0xC0000
#define PORT_INTX_ASSERT_MASK	0x01B0
#define PORT_INTX_DASSERT_MASK	0x01B4

// CORE REGISTER CTRL MOMERYMAP
#define TL_REG					0x20000
#define TL_CORE_REG				0x30000
#define TL_CORE_PF_REG			0x31000
#define TL_CORE_DFX_REG			0x3E000
#define TL_RPFN_CFGSPACE_REG	0x40000
#define PCS_REG					0x5A000
#define TOP_REG					0x5C000
#define CORE_REG				0x5E000
#define DL_REG					0x60000
#define MAC_REG					0x70000

// CORE GLOBAL CLKRST CTRL REG
#define PORT_RESET	0
#define CORE_PHY_RESET	0x4
#define PCS_SOFT_REST	0xC
#define PCIE_CORE_TL_COM_SFT_RST	0x14
#define PCIE_CORE_PHY_COM_SFT_RST	0x18

// CORE GLOBAL CTRL REG
#define PORT07_LINK_MODE		0x510
#define PORT0_LINK_MODE_X4		0x2
#define PORT02_LINK_MODE_X2		0x101
#define PORT815_LINK_MODE		0x514
#define GLB_PCIEC_MODE_SEL		0x434
#define PCIE_EP_DEVICE			0
#define CORE_GLB_MISC_0			0x700
#define PCIE_MAC_TXBUFF_MODEVAL		0x0
#define PCIE_MISC0_PCIEC_PHYMODE	0
#define PCIE_MISC0_PCIEC_PHYMODEVAL	0x1
#define MAC_CDC_DPTX_FIFO		0x704
#define HILINK_INT_MASK			0x520
#define PCIE_LINK_DOWN_RST_EN 0x75C
#define PCIE_LINK_DOWN_CLR_PORT_EN	0x760
#define CORE_PORT_CLK_ICG_EN_CTRL	0x76C
#define DLPHY_PMT_ICG_EN_CTRL		0x768
#define PORT_EN				0x400

#define CORE_INT_NI_STATUS_0	0x538
#define CORE_INT_NI_STATUS_1	0x548
#define CORE_INT_CE_STATUS_0	0x558
#define CORE_INT_CE_STATUS_1	0x568
#define CORE_INT_NFE_STATUS_0	0x578
#define CORE_INT_NFE_STATUS_1	0x588
#define CORE_INT_FE_STATUS_0	0x598
#define CORE_INT_FE_STATUS_1	0x5A8

// MAC REG
#define MAC_REG_TEST_COUNTER			0x05C
#define WAIT_LINK_NUM_TIMER_DP			0x1F4
#define MAC_ENTER_LPBK_DISABLE			0x2EC
#define MAC_REG_EQ_PHASE23_TIMEOUT_VAL	0x030
#define MAC_REG_WAIT_LINK_NUM_TIMER		0x0CC
#define MAC_WAIT_LANE_NUM_TIMER			0x2C4
#define MAC_REG_USE_FIX_CORE_CLK_EN		0x4EC
#define MAC_TARGET_LINK_WIDTH			0x534
#define MAC_REG_FXIDLE_DELAY_TIME		0x3CC
#define MAC_LTSSM_CTRL_INFEER_ELEC		0x2D8
#define MAC_REQ_EIOS_TO_ELEIDLE_DELAY		0x000
#define MAC_RCV_EIOS_TO_ELEIDLE_DLY		    0x57C
#define MAC_REG_ENTER_L1L2_TIMEOUT_VAL		0x098
#define MAC_REG_SYMBOL_UNLOCK_CNT		0x08C
#define MAC_FRAMING_ERR_CNT			    0x39C
#define MAC_RX_MARGIN_SELF_CTRL         0x33C
#define AUTO_SPEED_CHG_EN				BIT(9)
#define FIRST_AUTO_SPEED_CHG_EN			BIT(8)
#define AUTO_SPEED_DIS_MASK				BIT(7)

#define MAC_INT_TYPE_SEL				0x3A8
#define BUSINESS_INTR					1

#define MAC_CFG_RCV_HOLD_EN				0x310
#define REG_RCV_LOCK_HOLD_EN			BIT(0)
#define EQ_PHASE_TIMEOUT				0x24
#define PHASE3_OFFSET					8
#define MAC_LANE_NUM_WAIT_TIMER			0x2C8

#define WAIT_LINK_NUM_TIMER_UP			0x1F40000
#define WAIT_LANE_TIMER_UP_OFF			16
#define WAIT_LANE_TIMER_UP_VAL			0x1F4

#define FIX_CORE_CLK_EN					BIT(0)
#define CLK_FREQUENCY_LOW_POWER			8

#define MAC_REG_EQ_INIT_COEFF_CFG		0xC8
#define PRESET_COEFF_MODE_SET			28
#define HIPRELUDE_MODE					2

#define DEBOUNCE_EN						BIT(8)
#define MODE_SEL						BIT(9)
#define RXIDLE_DELAY_TIME				0xC

#define REG_STAND_BY_DISABLE			8
#define EIOS_TO_ELECIDLE_DELAY			2

#define MAC_FRAMING_ERR_CTRL			0x3A0
#define MAC_FRAMING_ERR_CTRL_VAL		0xF733

#define MAC_REG_MAC_INT_STATUS			0x54
#define MAC_REG_LINK_INFO				0x60
#define MAC_REG_DEBUG_PIPE2			0x068
#define MAC_REG_DEBUG_PIPE3			0x06C
#define MAC_REG_DEBUG_PIPE7			0x248

#define MAC_PCS_RX_ERR_CNT_REG			0x2BC
#define MAC_TRACE_2BIT_ECC_CNT			0x308
#define MAC_TRACE_1BIT_ECC_CNT			0x30C
#define DFX_APB_LANE_ERROR_STATUS		0x3BC
#define DFX_APB_LANE_ERROR_STATUS1		0x3C0
#define MAC_REG_RPT_MBUS_ERR_CNT		0x518

#define MAC_REG_SYMBOL_UNLOCL_COUNTER	0x8C
#define MAC_LEAVE_L0_INFO				0x3b4

#define MAC_ENTER_L1L2_TIMEOUT			0x0

#define MAC_REG_INT_MASK				0x058
#define MAC_ICG_EN_CTRL					0x4F4

#define MAC_REG_NI_INT_RO						0x03e8
#define MAC_REG_FE_INT_RO						0x03ec
#define MAC_REG_CE_INT_RO						0x03f0
#define MAC_REG_NFE_INT_RO						0x03f4
#define MAC_LTSSM_ST_SHIFT						24
#define MAC_LTSSM_ST_MASK						0x3f
#define MAC_LINK_UP								BIT(16)
#define MAC_CUR_LINK_SPEED_SHIFT				8
#define MAC_CUR_LINK_SPEED_MASK					0x7
#define MAC_CUR_LINK_WIDTH_SHIFT				0
#define MAC_CUR_LINK_WIDTH_MASK					0x3f
#define MAC_REG_LTSSM_TRACER_INPUT				0x050
#define MAC_LTSSM_TRACER_CAP_MODE				BIT(1)
#define MAC_LTSSM_TRACER_RECAP					BIT(0)
#define MAC_REG_LTSSM_TRACER_OUTPUT_1			0x084
#define MAC_LTSSM_TRACER_OUTPUT_1_STATE_MASK	0x3F
#define MAC_REG_LTSSM_TRACER_LAST_ADDR			0x088
#define MAC_LTSSM_TRACER_LAST_ADDR_ROLLBACK		BIT(6)
#define MAC_LTSSM_TRACER_LAST_WADDR_MASK		0x3F
#define MAC_REG_LTSSM_TRACERADDR				0x02F8
#define MAC_LTSSM_TRACER_RADDR_MASK				0x3f
#define LOCAL_LOOPBACK_MODE						0x40C
#define MAC_REG_ENTER_LOOPBACK					0x004
#define MAC_LOOPBACK_MASTER_CHECK_ST_SHIFT		8
#define MAC_LOOPBACK_MASTER_CHECK_ST_MASK		0x3
#define MAC_LOOPBACK_CHECK_EN					BIT(1)
#define MAC_ENTER_LOOPBACK						BIT(0)
#define MAC_DS_L0_LOOPBACK_EN					BIT(3)
#define MAC_REG_EQ_DISABLE						0x01C
#define MAC_EQUALIZATION_DISABLE				BIT(0)
#define MAC_POWERDOWN_VALUE						0x528
#define MAC_TXELEIDLE_TM						0x530
#define REG_RCV_CFG_TXIDLE_EN					BIT(24)

#define MAC_REG_PRESET0				0x200
#define MAC_REG_PRESET1				0x204
#define MAC_REG_PRESET2				0x208
#define MAC_REG_PRESET3				0x20C
#define MAC_REG_PRESET4				0x210
#define MAC_REG_PRESET5				0x214
#define MAC_REG_PRESET6				0x218
#define MAC_REG_PRESET7				0x21C
#define MAC_REG_PRESET8				0x220
#define MAC_REG_PRESET9				0x224
#define MAC_REG_PRESET10			0x228
#define MAC_REG_PRESET11			0x22C
#define MAC_REG_PRESET12			0x230
#define MAC_REG_PRESET13			0x234
#define MAC_REG_PRESET14			0x238
#define MAC_REG_PRESET15			0x23C
#define MAC_CFG_RCV_CFG_HOLD_TIME               0x318

// TL REG
#define TL_RX_CHK_EN					0x8
#define TL_PM_STATE				0x10C
#define SYS_ERR_OVERRIDE_CTRL			0x4E4
#define TL_CFG_HCK_EN					0x4E8
#define TL_NPD_VC0_CDT		0xF
#define TL_NPH_VC0_CDT		0x14
#define TX_CRD_CTRL		    0x14
#define TL_RX_CPL_CREDIT	0xB24
#define TL_CPLD_VC0_CDT_OFF	16
#define TL_CPLD_VC0_CDT		0x5F
#define TL_CPLH_VC0_CDT		0x14
#define TL_RX_CDT_INI_UP	0xB28
#define TL_CDT_INIT_UP		bit(0)
#define TL_RX_POSTED_CREDIT_WATERLINE_CFG 0xBBC
#define RX_PH_VC0_CDT_WL_CFG	0x24
#define RX_PD_VC0_CDT_WL_CFG	0xC0
#define RX_PD_VC0_CDT_WL_CFG_OFF	16
#define TL_RX_NPOSTED_CREDIT_WATERLINE_CFG	0xBC0
#define RX_NPH_VC0_CDT_WL_CFG	0x24
#define RX_NPD_VC0_CDT_WL_CFG	0x3
#define RX_NPD_VC0_CDT_WL_CFG_OFF	16
#define TL_RX_ODR_CTRL_MODE	0x200
#define TL_CFG_ACCESS_CTRL		0xFC
#define tl_tx_tag_ctrl(port_num_max)	(0xA00 + (port_num_max) * 0x4)
#define TL_INT_MASK0		0x574
#define TL_INT_MASK1		0x580
#define TL_INT_MASK2		0x5A4

#define TL_INT_CE_RO					0x0594
#define TL_INT_NFE_RO					0x0598
#define TL_INT_FE_RO					0x059C
#define TL_INT_STATUS0					0x056C
#define TL_INT_STATUS1					0x0570
#define TL_INT_STATUS2					0x05A0
#define TL_RX_VC0_POST_CREDIT_LEFT		0x0B00
#define TL_RX_VC0_NONPOST_CREDIT_LEFT	0x0B04
#define TL_RX_VC0_CPL_CREDIT_LEFT		0x0B08
#define TL_PM_STATUS_FIFO_CONTROL		0x0BE4
#define TL_PM_STATUS					0x0BD8
#define TL_TX_VC0_P_FC_LEFT				0x0A10
#define TL_TX_VC0_NP_FC_LEFT			0x0A14
#define TL_TX_VC0_CPL_FC_LEFT			0x0A18
#define TL_PORT_IDLE_CTRL_STATUS		0x0028
#define PCIE_PORT_IDLE_ST				BIT(19)
#define TL_RX_ERR_STATUS				0x0B9C
#define TL_PM_UC_CTRL					0x0114
#define TL_UC_TURNOFF_MSG_REQ			BIT(25)
#define TL_ENTER_L0_CTRL				0x0118
#define TL_ENTER_L0_EN					BIT(0)
#define TL_TX_DFX_CTRL				0x0A00
#define TL_DFX_PM_TRACER_CAP_MODEBIT            BIT(1)
#define TL_DFX_PM_TRACER_LAST_WADDR_MASK        0x3F00
#define TL_PM_STATUS_MSK                         0x3F
#define TL_DFX_PM_TRACER_RADDR_MASK             0xF8
#define TL_PM_TRACER_WADDR_MASK                 0x1F00
#define TL_PM_STATE_MSK                         0x3F

// TL CORE REG
#define TL_TX_CTRL			0x4
#define TL_TX_INGRESS_CNT	0x8
#define TL_TX_EGRESS_CNT	0xC
#define TL_TX_REQ_TIMER_CTRL    0x14
#define TL_TX_CFG_CNT		0x98
#define TL_TX_MEM_RD_CNT	0x9C
#define TL_TX_MEM_WR_CNT	0xA0
#define TL_TX_MSG_CNT		0xAC
#define TL_TX_CPL_CNT		0xB0
#define TL_RX_MSI_ADDR_WINDOW_L	0x514
#define TL_RX_MSI_ADDR_WINDOW_H	0x518
#define PN_PF_STR_IDX0		0x910
#define PN_PF_STR_IDX1		0x914
#define PN_PF_STR_IDX2		0x918
#define PN_PF_STR_IDX3		0x91C
#define TL_CFG_PF_EN		0x904
#define TL_CFGSPACE_SEL		0x004
#define TL_PF_VF_CFGSPACE_SEL	0x008
#define TL_DFX_PM_CORE_FUNC_EN	0x0A8C
#define TL_CORE_INT_STATUS0		0x814
#define TL_CORE_INT_CE_RO0		0x0828
#define TL_CORE_INT_NFE_RO0		0x082C
#define TL_CORE_INT_FE_RO0		0x0830
#define APB_TIMEOUT_INT_STATUS	0x0008
#define APB_TIMEOUT_INT_RO		0x000C
#define TL_RX_MISC_CFG			0x0508
#define TL_PF_FLR_INT_MASK		0x940
#define TL_CORE_INT_MASK0		0x818

// TL CORE DFX REG
#define TL_ICG_SW_CTRL			0x140

// DL REG
#define ECC_ERR_CNT_1BIT		0x0044
#define ECC_ERR_CNT_2BIT		0x0048
#define DFX_LCRC_ERR_NUM		0x0050
#define DFX_DCRC_ERR_NUM		0x0054
#define DFX_FSM_STATE			0x0058
#define DL_INT_STATUS			0x0088
#define DL_INT_MASK			0x008C
#define INIT_FC_SEND_EN			0x00DC
#define DFX_DLLP_RX_COUNT_NUM		0x00E0
#define DFX_RX_NAK_COUNT		0x0F8
#define DFX_RX_BAD_DLLP_TYPE		0x00FC

#define DL_UP_INT_STATUS		BIT(18)

#define DFX_ICG_CTRL			0x2C8

#define DL_NI_INT_RO			0x014c
#define DL_FE_INT_RO			0x0150
#define DL_CE_INT_RO			0x0154
#define DL_NFE_INT_RO			0x0158

// DL CXL REG
#define HOST_DEVICE_SEL			0xC00

// TOP REG
#define APB_TIMEOUT_INT_MASK	0x0004

// RP_CFGSPACE
#define TL_L1PM_CAPBILITY		0x3C4
#define DEVICE_CAPBILITY		0x044
#define ROOT_CTRL_STATUS		0x05C
#define LINK_CTRL_STATUS		0x050
#define L1PM_CAP_HEADER			0x3C0
#define GEN5_PHY_CAP_REG08              0x784

// AP GLOBAL
#define AP_CFGSPACE_RST			0x0820
#define AP_CAP_TL_REG_RST		0x0828
#define PCIE_AP_NI_STATUS		0x0120
#define PCIE_AP_CE_STATUS		0x0124
#define PCIE_AP_UNF_STATUS		0x0128
#define PCIE_AP_UF_STATUS		0x012C
#define PCIE_MSI_MASK			0x00F4
#define PCIE_MSI_STATUS			0x00F8
#define PCIE_CORE_NI_STATUS		0x0180
#define PCIE_CORE_CE_STATUS		0x0184
#define PCIE_CORE_UNF_STATUS	0x0188
#define PCIE_CORE_UF_STATUS		0x018C
#define PCIE_AP_APB_SYNC_RST	0x810
#define PCIE_AP_AXI_SYNC_RST	0x814
#define AP_SUB_ICG_EN			0x85c
#define AP_SUB_AXI_ICG_EN		0x854
#define AP_PORT_EN				0x800
#define PCIE_ERR_MAPPING	0x0000
#define PCIE_CE_ENA			0x0008
#define PCIE_UNF_ENA		0x0010
#define PCIE_UF_ENA			0x0018
#define PCIE_CE_MASK		0x000C
#define PCIE_UNF_MASK		0x0014
#define PCIE_UF_MASK		0x001C
#define PCIE_CE_STATUS		0x0020
#define PCIE_UNF_STATUS		0x0028
#define PCIE_UF_STATUS		0x0030
#define PCIE_AP_NI_ENA		0x0100
#define PCIE_AP_CE_ENA		0x0104
#define PCIE_AP_UNF_ENA		0x0108
#define PCIE_AP_UF_ENA		0x010C
#define PCIE_AP_NI_MASK		0x0110
#define PCIE_AP_CE_MASK		0x0114
#define PCIE_AP_UNF_MASK	0x0118
#define PCIE_AP_UF_MASK		0x011C
#define PCIE_CORE_NI_ENA	0x0160
#define PCIE_CORE_CE_ENA	0x0164
#define PCIE_CORE_UNF_ENA	0x0168
#define PCIE_CORE_UF_ENA	0x016C
#define PCIE_CORE_NI_MASK	0x0170
#define PCIE_CORE_CE_MASK	0x0174
#define PCIE_CORE_UNF_MASK	0x0178
#define PCIE_CORE_UF_MASK	0x017C
#define CORE_INT_CE_MSK_0		0x554
#define CORE_INT_NI_MSK_0		0x534
#define CORE_INT_NFE_MSK_0		0x574
#define CORE_INT_FE_MSK_0		0x594
#define CORE_INT_CE_MSK_1		0x564
#define CORE_INT_NI_MSK_1		0x544
#define CORE_INT_NFE_MSK_1		0x584
#define CORE_INT_FE_MSK_1		0x5A4
#define CORE_INT_NI_MSK_2		0x5B4
#define CORE_INT_CE_MSK_2		0x5C4
#define CORE_INT_NFE_MSK_2		0x5D4
#define CORE_INT_FE_MSK_2		0x5E4
#define CORE_INT_NI_STATUS_2	0x5B8
#define CORE_INT_CE_STATUS_2	0x5C8
#define CORE_INT_NFE_STATUS_2	0x5D8
#define CORE_INT_FE_STATUS_2	0x5E8
#define AP_IDLE			0xC08

// AP INT
#define INT_SRAM_INIT_CTRL			0x0050
#define INIT_MSI_COAL_TBL			BIT(3)
#define INIT_MSIX_PTR_TBL			BIT(2)
#define INIT_MSIX_COAL_TBL			BIT(1)
#define INIT_MSIX_TBL				BIT(0)

#define DFX_INT_SRAM_INIT_STATUS	0x0A00
#define PTR_TBL_INIT_SEL			BIT(31)
#define MSI_COAL_TBL_INIT_DONE		BIT(3)
#define PTR_TBL_INIT_DONE			BIT(2)
#define MSIX_COAL_TBL_INIT_DONE		BIT(1)
#define MSIX_TBL_INIT_DONE			BIT(0)

#define INIT_PTR_TBL_DATA		0x0060
#define ALLOCATE_BUF_CTRL		0x0B0
#define LOCAL_CE_INT			0x0400
#define LOCAL_NFE_INT			0x0404
#define LOCAL_FE_INT			0x0408
#define ABNORMAL_INT_STS		0x0500
#define ECC_ERR_INT_SRC_STS		0x0604

// AP IOB RX
#define IOB_RX_INT_RO_CE		0x1714
#define IOB_RX_INT_RO_NFE		0x1718
#define IOB_RX_INT_RO_FE		0x171C
#define IOB_RX_INT_STATUS		0x1704
#define IOB_RX_INT_MSK			0x1700
#define IOB_RX_DATA_BUFFER_SIZE		0x10EC
#define IOB_RX_AML_ERR_WDATA_CTRL	0x18e4
#define DFX_IOB_RX_CNT_RX_REQ		0x1500
#define DFX_IOB_RX_CNT_DMA_CPL		0x1518
#define IOB_RX_DAT_BUF_ECC_ERR_CNT	0x1748

#define rx_atu_size_low(n)	(0xC + 0x20 * (n))
#define rx_atu_size_high(n)	(0x8 + 0x20 * (n))
#define rx_atu_src_low(n)	(0x10 + 0x20 * (n))
#define rx_atu_src_high(n)	(0x14 + 0x20 * (n))
#define rx_atu_dst_low(n)	(0x18 + 0x20 * (n))
#define rx_atu_dst_high(n)	(0x1C + 0x20 * (n))
#define rx_atu_ctrl0(n)		(0x0 + 0x20 * (n))
#define rx_atu_ctrl1(n)		(0x4 + 0x20 * (n))
#define RX_ATU_EN_BIT		(0x1 << 0)

// AP IOB TX
#define IOB_TX_INT_RO_CE0		0x8A0
#define IOB_TX_INT_RO_CE1		0x8A4
#define IOB_TX_INT_RO_CE2		0x8A8
#define IOB_TX_INT_RO_CE3		0x8AC
#define IOB_TX_INT_RO_CE4		0x8B0
#define IOB_TX_INT_RO_CE5		0x8B4
#define IOB_TX_INT_RO_NFE0		0x8C0
#define IOB_TX_INT_RO_NFE1		0x8C4
#define IOB_TX_INT_RO_NFE2		0x8C8
#define IOB_TX_INT_RO_NFE3		0x8CC
#define IOB_TX_INT_RO_NFE4		0x8D0
#define IOB_TX_INT_RO_NFE5		0x8D4
#define IOB_TX_INT_RO_FE0		0x8E0
#define IOB_TX_INT_RO_FE1		0x8E4
#define IOB_TX_INT_RO_FE2		0x8E8
#define IOB_TX_INT_RO_FE3		0x8EC
#define IOB_TX_INT_RO_FE4		0x8F0
#define IOB_TX_INT_STATUS0		0x820
#define IOB_TX_INT_STATUS1		0x824
#define IOB_TX_INT_STATUS2		0x828
#define IOB_TX_INT_STATUS3		0x82C
#define IOB_TX_INT_STATUS4		0x830
#define IOB_TX_INT_STATUS5		0x834
#define IOB_TX_INT_SEVERITY3	0x88C
#define iob_tx_dbuf_p_pri_rgn_addr(n)	(0x1000 + (n) * 0x10)
#define IOB_TX_DBUF_ALL_SHR_RGN_ADDR	0x1080
#define IOB_TX_DBUF_P_SHR_RGN_ADDR	0x1084
#define IOB_TX_DBUF_CPL_SHR_RGN_ADDR	0x108C
#define iob_tx_dbuf_src_pri_rgn_addr(n)	(0x10A0 + (n) * 0x4)
#define iob_tx_hbuf_p_pri_rgn_addr(n)	(0x1100 + (n) * 0x10)
#define IOB_TX_HBUF_ALL_SHR_RGN_ADDR	0x1180
#define IOB_TX_HBUF_P_SHR_RGN_ADDR	0x1184
#define IOB_TX_HBUF_CPL_SHR_RGN_ADDR	0x118C
#define iob_tx_hbuf_src_pri_rgn_addr(n)	(0x11A0 + (n) * 0x4)
#define IOB_TX_SRC_PRI_RGN_MAP		0x120C
#define IOB_TX_REQ_TOUT_NOTIFY_MSK	0x1280
#define IOB_TX_REQ_TIMER_CFG		0x1300
#define IOB_TX_INT_SEVERITY0		0x0880
#define IOB_TX_INT_MSK0			0x800
#define IOB_TX_INT_MSK1			0x804
#define IOB_TX_INT_MSK2			0x808
#define IOB_TX_INT_MSK3			0x80C
#define IOB_TX_INT_MSK4			0x810
#define IOB_TX_INT_MSK5                 0x814
#define IOB_TX_ECAM_CTRL0		(0x600)
#define TX_ECAM_EN_BIT		(0x1 << 4)
#define TX_BUS_CNT_MASK		(0xF)
#define IOB_TX_ECAM_CTRL1		(0x604)
#define IOB_TX_ECAM_ADDR_LOW	(0x608)
#define IOB_TX_ECAM_ADDR_HIGH	(0x60C)
#define tx_atu_size_low(n)	(0xC + 0x20 * (n))
#define tx_atu_size_high(n)	(0x8 + 0x20 * (n))
#define tx_atu_src_low(n)	(0x10 + 0x20 * (n))
#define tx_atu_src_high(n)	(0x14 + 0x20 * (n))
#define tx_atu_dst_low(n)	(0x18 + 0x20 * (n))
#define tx_atu_dst_high(n)	(0x1C + 0x20 * (n))
#define tx_atu_ctrl0(n)		(0x0 + 0x20 * (n))
#define TX_ATU_EN_BIT		(0x1 << 0)
#define atu_control_0(idx)			(0x0 + 0x20 * (idx))
#define atu_control_1(idx)			(0x4 + 0x20 * (idx))
#define atu_control_2(idx)			(0x8 + 0x20 * (idx))
#define atu_region_size(idx)		(0xC + 0x20 * (idx))
#define SIZE_4K						(0x1000)
#define atu_size_align_4k(size)		((((size) / SIZE_4K) + 1) * SIZE_4K)
#define DFX_IOB_RX_AML_WR_CNT_0         0x1A20
#define DFX_IOB_RX_AML_WR_CNT_1         0x1A24
#define DFX_IOB_RX_AML_RD_CNT           0x1A28
#define DFX_IOB_TX_IDLE                 0x2000
#define DFX_IOB_TX_AXIS_WR_CNT0         0x2400
#define DFX_IOB_TX_AXIS_WR_CNT1         0x2404
#define DFX_IOB_TX_AXIS_WR_CNT2         0x2408
#define DFX_IOB_TX_AXIS_WR_CNT3         0x240C
#define DFX_IOB_TX_AXIS_WR_CNT4         0x2410
#define DFX_IOB_TX_AXIS_WR_CNT5         0x2414
#define DFX_IOB_TX_AXIS_RD_CNT0         0x2440
#define DFX_IOB_TX_AXIS_RD_CNT1         0x2444
#define DFX_IOB_TX_P_CNT0               0x2800
#define DFX_IOB_TX_P_CNT1               0x2804
#define DFX_IOB_TX_NP_CNT0              0x2880
#define DFX_IOB_TX_NP_CNT1              0x2884
#define DFX_IOB_TX_NP_CNT2              0x2888
#define DFX_IOB_TX_NP_CNT3              0x288C
#define DFX_IOB_TX_NP_CNT4              0x2890
#define DFX_IOB_TX_NP_CNT5              0x2894
#define DFX_IOB_TX_CPL_CNT0             0x2900
#define DFX_IOB_TX_CPL_CNT1             0x2904
#define DFX_IOB_TX_CPL_CNT2             0x2908
#define DFX_IOB_TX_STATUS2              0x2A08
#define DFX_IOB_TX_STATUS3              0x2A0C
#define DFX_IOB_TX_STATUS4              0x2A10

// TL CORE PF
#define PFN_BAR0_MASK			0x0010
#define PFN_BAR1_MASK			0x0014
#define PFN_BAR2_MASK			0x0018
#define PFN_BAR3_MASK			0x001C
#define PFN_BAR4_MASK			0x0020
#define PFN_BAR5_MASK			0x0024
#define PFN_BARN_ENABLE			0x002C
#define PFN_TLRX_ERR_FILT		0x0070

// AP DMA REG
#define DMA_TOTAL_IDX		1
#define dma_queue_com(n)	(0x20 + (n) * 0x100 + 0x2000)
#define dma_queue_fsm_sts(n)	(0x30 + (n) * 0x100 + 0x2000)
#define DMA_CM_RAS_LEVEL	0x0188
#define dma_queue_int_msk(n)	(0x44 + (n) * 0x100 + 0x2000)
#define dma_queue_sq_base_l(q)		(0x2000 + (q) * 0x100)
#define dma_queue_sq_base_h(q)		(0x2000 + (q) * 0x100 + 0x4)
#define dma_queue_sq_depth(q)		(0x2000 + (q) * 0x100 + 0x8)
#define dma_queue_sq_tail_ptr(q)	(0x2000 + (q) * 0x100 + 0xC)

#define dma_queue_cq_base_l(q)		(0x2000 + (q) * 0x100 + 0x10)
#define dma_queue_cq_base_h(q)		(0x2000 + (q) * 0x100 + 0x14)
#define dma_queue_cq_depth(q)		(0x2000 + (q) * 0x100 + 0x18)
#define dma_queue_cq_head_ptr(q)	(0x2000 + (q) * 0x100 + 0x1C)
#define dma_queue_cq_tail_ptr(q)	(0x2000 + (q) * 0x100 + 0x3C)

#define dma_queue_int_sts(q)		(0x0040 + (q) * 0x100 + 0x2000)
#define dma_queue_err_int_sts(q)	(0x0048 + (q) * 0x100 + 0x2000)
#define dma_queue_err_int_msk(q)	(0x004C + (q) * 0x100 + 0x2000)
#define DMA_EP_INT					0x0028
#define DMA_EP_INT_MSK				0x0024

#define DMA_QUEUE_DONE_INT_MSK		(0x1 << 0)

#define dma_queue_ctrl0(q)			(0x2000 + (q) * 0x100 + 0x20)
#define DMA_QUEUE_EN				(0x1 << 0)
#define DMA_QUEUE_PAUSE				(0x1 << 4)
#define DMA_QUEUE_RESET				(0x1 << 0)
#define DMA_QUEUE_CQ_DRCT_SEL		(0x1 << 26)
#define DMA_QUEUE_SQ_DRCT_SEL		(0x1 << 27)

#define dma_queue_ctrl1(q)			(0x2000 + (q) * 0x100 + 0x24)
#define DMA_QUEUE_PORT_NUM			(0x1 << 4)

#define	DMA_FLR_DISABLE				0xA00

#define DMA_PORT_CFG0				0x800
#define LINK_DOWN_MASK				(0x1 << 16)

// AP_INT
#define INT_REQ_CTRL				0x00A0
#define INT_REQ_SET					(0x1 << 0)
#define INT_REQ_VECTOR_SHIFT		1
#define INT_REQ_VECTOR_MASK			0x7ff

// AP_ENGINE
#define RBA_MODE_CFG                0xBC

// config space
#define PCIHDR_ID					0
#define DEVICEID_OFF				16
#define DEVICE_ID					0x6490
#define VENDOR_ID					0x19E5

#define PCIHDR_CMDSTS				0x4
#define CFG_MEM_SPACE_EN			(0x1 << 1)
#define CFG_MASTER_EN				(0x1 << 2)

#define PCIHDR_BAR0					0x10
#define PCIHDR_BAR1					0x14
#define PCIHDR_BAR2					0x18
#define PCIHDR_BAR3					0x1C
#define PCIHDR_BAR4					0x20
#define PCIHDR_BAR5					0x24

#define CFG_BAR_32BIT_MEM_TYPE		0xFFFFFFF8

#define PCIHDR_IO_LIMIT			0x001C
#define PCIHDR_MEM_BASE_LIMIT			0x0020

#define PCIHDR_INT					0x3C
#define CFG_INT_PIN					(0x1 << 8)
#define PIN_INTA					(1 << 8)
#define PIN_INTB					(2 << 8)
#define PIN_INTC					(3 << 8)
#define PIN_INTD					(4 << 8)

#define LINK_CAPBILITY				0x4C
#define MAX_LINK_SPEED				1 /* support 2.5GT/s */
#define MAX_LINK_WIDTH_OFF			4
#define MAX_LINK_WIDTH				4 /* x4 */

#define DEVICE_CTRL_STATUS			0x48
#define AUX_PWR_DETECT				(0x1 << 20)
#define SLOT_CTRL_STATUS			0x58
#define DEVICE_CAPABILITY2			0x64
#define FRS_SUP						(0x1 << 31)
#define SUP_10BIT_REQ_TAG			(0x1 << 17)
#define SUP_10BIT_CPL_TAG			(0x1 << 16)
#define IN_SYS_CLS					((0x1 << 14) | (0x1 << 15))
#define CAS_128BIT_CPL_SUP			(0x1 << 9)
#define ATOMIC_64BIT_CPL_SUP		(0x1 << 8)
#define ATOMIC_32BIT_CPL_SUP		(0x1 << 7)

#define DEVICE_CTRL2				0x68
#define EN_10BIT_REQ_TAG			(0x1 << 12)

#define LINK_CAPBILITY2				0x6C
#define CROSS_LINK_UP				(0x1 << 8)
#define LINK_SPEED					(0x1 << 1) /* support 2.5GT/s */

#define LINK_CTRL_STATUS2			0x70
#define TARGET_LINK_SPEED			1 /* support 2.5GT/s */

#define MSI_CAP_HEADER				0x80
#define MSI_ENABLE					(0x1 << 16)

#define MSI_CAP_LADDR				0x84

#define MSI_DATA					0x88
#define MSI_EXT_DATA				(0x1 << 16)

#define MSI_MASK					0x90

#define MSIX_CAP_HEADER				0xA0
#define MSIX_ENABLE					(0x1 << 31)

#define PME_CAP_HEADER				0xB0
#define IMMEDIATE_RN_RETURN_D0		(0x1 << 20)

#define PME_STATUS					0xB4
#define NO_SOFT_RESET				(0x1 << 3)

#define DEVICE_SERIAL_NUMBER_CAP_HEADER	0x04E0

#define AER_UNCR_STATUS                         0x0104
#define AER_COR_STATUS                          0x0110
#define LINK_CONTROL3_REGISTER01		0x031C
#define LINK_CONTROL3_REGISTER23		0x0320
#define LINK_CONTROL3_REGISTER45		0x0324
#define LINK_CONTROL3_REGISTER67		0x0328
#define LINK_CONTROL3_REGISTER89		0x032C
#define LINK_CONTROL3_REGISTER1011		0x0330
#define LINK_CONTROL3_REGISTER1213		0x0334
#define LINK_CONTROL3_REGISTER1415		0x0338

#define L1PM_CONTROL1				0x3C8
#define L1PM_CONTROL2				0x3CC

#define GEN4_PHY_CAP_REG20				0x08A0
#define GEN4_PHY_CAP_REG24				0x08A4
#define GEN4_PHY_CAP_REG28				0x08A8
#define GEN4_PHY_CAP_REG2C				0x08AC
#define UNCR_ERR_MASK					0x0108
#define SRAM_TIMEING_CFG6		                0x850
#endif
