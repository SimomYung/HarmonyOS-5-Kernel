/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : HOST MAC OPS接口
 * 作者       : wifi
 * 创建日期   : 2012年10月18日
 */

#ifndef HOST_MAC_MP12_H
#define HOST_MAC_MP12_H

#include "frw_ext_if.h"
#include "hal_common.h"
#include "host_hal_device.h"
#include "oal_ext_if.h"
#include "host_irq_mp12.h"

/* MP12 MAC寄存器基地址 */
#define MAC_COMMON_REG_BASE  (0x04070000)
#define MAC_LUT_REG_BASE     (0x04071000)
#define MAC_PPU_REG_BASE     (0x04072000)
#define MAC_GRP_HAC_REG_BASE (0x04073000)
#define MAC_GLB_REG_BASE     (0x04080000)
#define MAC_CTRL_REG_BASE    (0x04081000)
#define MAC_TPU_REG_BASE     (0x04082000)
#define MAC_RPU_REG_BASE     (0x04083000)
#define MAC_TMU_REG_BASE     (0x04084000)

/* MP12 PHY寄存器基地址 */
#define DFX_REG_BANK_BASE (0x40C4000)
#define RX_CHN_EST_REG_BANK_BASE (0x40F2800)
#define RF_CTRL_REG_BANK_0_BASE (0x40C4800)
#define CALI_TEST_REG_BANK_0_BASE (0x40C1000)
#define PHY_GLB_REG_BANK_BASE (0x40C0800)

#define MP12_GLBA_CTL_RB_BASE (0x4000000)

/* PHY 寄存器 */
#define DFX_REG_BANK_PHY_INTR_RPT_REG (DFX_REG_BANK_BASE + 0x148)              /* PHY中断状态上报寄存器 */
#define DFX_REG_BANK_PHY_INTR_RPT2_REG (DFX_REG_BANK_BASE + 0x14C) /* PHY中断状态上报寄存器2 */
#define DFX_REG_BANK_CFG_INTR_MASK_HOST_REG (DFX_REG_BANK_BASE + 0x24)         /* PHY给host中断MASK寄存器 */
#define DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG (DFX_REG_BANK_BASE + 0x30) /* PHY给host中断MASK寄存器2 */
#define DFX_REG_BANK_CFG_INTR_CLR_REG (DFX_REG_BANK_BASE + 0x2C)               /* PHY中断清除寄存器 */
#define DFX_REG_BANK_CFG_INTR_CLR2_REG (DFX_REG_BANK_BASE + 0x38) /* PHY中断清除寄存器2 */
#define CALI_TEST_REG_BANK_0_FTM_CFG_REG (CALI_TEST_REG_BANK_0_BASE + 0x380)   /* FTM配置寄存器 */
#define RX_CHN_EST_REG_BANK_CHN_EST_COM_REG (RX_CHN_EST_REG_BANK_BASE + 0x24C) /* 信道估计通用寄存器 */
#define RF_CTRL_REG_BANK_0_CFG_ONLINE_CALI_USE_RF_REG_EN_REG (RF_CTRL_REG_BANK_0_BASE + 0x2C)
#define CALI_TEST_REG_BANK_0_FIFO_FORCE_EN_REG (CALI_TEST_REG_BANK_0_BASE + 0x28) /* FIFO使能对应的寄存器配置地址 */
#define PHY_GLB_REG_BANK_PHY_BW_MODE_STA_REG           (PHY_GLB_REG_BANK_BASE + 0x8) /* 频宽模式配置寄存器 */

/* MAC 寄存器 */
#define MAC_GLB_REG_HOST_INTR_STATUS_REG                     (MAC_GLB_REG_BASE + 0x504) /* 上报到Host的中断状态寄存器 */
#define MAC_GLB_REG_HOST_INTR_CLR_REG                        (MAC_GLB_REG_BASE + 0x400) /* Host中断清除寄存器 */
#define MAC_GLB_REG_HOST_INTR_MASK_REG                       (MAC_GLB_REG_BASE + 0x404) /* Host中断屏蔽寄存器 */

#define MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG           (MAC_COMMON_REG_BASE + 0x318) /* 上报到Host的中断状态寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG              (MAC_COMMON_REG_BASE + 0xC4)  /* MAC COMMON Host中断清除寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG             (MAC_COMMON_REG_BASE + 0xC8)  /* MAC COMMON Host中断屏蔽寄存器 */

/* rx ring reset ctrl,每个bit对应1个ring */
#define MP12_RING_RESET_NORM_DATA_FREE_RING_MASK    0x1
#define MP12_RING_RESET_SMALL_DATA_FREE_RING_MASK   0x2
#define MP12_RING_RESET_NORM_MGMT_FREE_RING_MASK    0x4
#define MP12_RING_RESET_SMALL_MGMT_FREE_RING_MASK   0x8
#define MP12_RING_RESET_LARGE_MGMT_FREE_RING_MASK   0x10
#define MP12_RING_RESET_PPDU_FREE_RING_MASK         0x20
#define MP12_RING_RESET_PPDU_HOST_FREE_RING_MASK    0x40
#define MP12_RING_RESET_DATA_COMP_RING_MASK         0x80
#define MP12_RING_RESET_MGMT_COMP_RING_MASK         0x100
#define MP12_RING_RESET_PPDU_COMP_RING_MASK         0x200

#define MAC_COMMON_REG_RX_RING_RESET_CTRL_REG            (MAC_COMMON_REG_BASE + 0x104) /* 接收ring复位配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_REG   (MAC_COMMON_REG_BASE + 0x3A8) /* 接收普通数据帧Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_REG  (MAC_COMMON_REG_BASE + 0x3AC) /* 接收小包数据帧Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_REG         (MAC_COMMON_REG_BASE + 0x3B0) /* 接收数据帧Complete Ring的写指针上报 */
#define MAC_COMMON_REG_RPT_HOST_FREE_RING_STATUS_REG     (MAC_COMMON_REG_BASE + 0x3B4) /* FREE RING上报信息 */
/* RX PPDU HOST描述符Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_REG (MAC_COMMON_REG_BASE + 0x3B8)
/* PPDU HOST FREE RING上报信息 */
#define MAC_COMMON_REG_RPT_PPDU_HOST_FREE_RING_STATUS_REG  (MAC_COMMON_REG_BASE + 0x3BC)

#define MAC_LUT_REG_ML0_TX_INFO_BUF_WPTR_REG           (MAC_LUT_REG_BASE + 0x500) /* BA INFO BUFFER的写指针上报 */
#define MAC_LUT_REG_ML1_TX_INFO_BUF_WPTR_REG           (MAC_LUT_REG_BASE + 0x504) /* BA INFO BUFFER的写指针上报 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_DEPTH_0_REG         (MAC_LUT_REG_BASE + 0x60)  /* TX BA INFO BUFFER深度配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_DEPTH_1_REG         (MAC_LUT_REG_BASE + 0x64)  /* TX BA INFO BUFFER深度配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_LSB_0_REG      (MAC_LUT_REG_BASE + 0x6C)  /* TX BA INFO BUFFER基地址配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_MSB_0_REG      (MAC_LUT_REG_BASE + 0x78)  /* TX BA INFO BUFFER基地址配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_LSB_1_REG      (MAC_LUT_REG_BASE + 0x70)  /* TX BA INFO BUFFER基地址配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_BUF_ADDR_MSB_1_REG      (MAC_LUT_REG_BASE + 0x7C)  /* TX BA INFO BUFFER基地址配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_RPTR_0_REG              (MAC_LUT_REG_BASE + 0x84)  /* TX BA INFO RPTR 配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_RPTR_1_REG              (MAC_LUT_REG_BASE + 0x88)  /* TX BA INFO RPTR 配置寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_WPTR_0_REG              (MAC_LUT_REG_BASE + 0x90)  /* TX BA INFO WPTR 寄存器 */
#define MAC_LUT_REG_TX_BA_INFO_WPTR_1_REG              (MAC_LUT_REG_BASE + 0x94)  /* TX BA INFO WPTR 寄存器 */

/* RX数据帧Buffer大小配置寄存器 */
#define MAC_RPU_REG_RX_DATA_BUFF_LEN_REG                        (MAC_RPU_REG_BASE + 0x5C)
/* 接收普通数据帧Free Ring基地址配置寄存器LSB */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG      (MAC_COMMON_REG_BASE  + 0x108)
/* 接收普通数据帧Free Ring基地址配置寄存器MSB */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG      (MAC_COMMON_REG_BASE  + 0x10C)
/* 接收小包数据帧Free Ring基地址配置寄存器_LSB */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG     (MAC_COMMON_REG_BASE  + 0x110)
/* 接收小包数据帧Free Ring基地址配置寄存器_MSB */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG     (MAC_COMMON_REG_BASE  + 0x114)
/* 接收数据帧Complete Ring基地址配置寄存器LSB */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_LSB_REG            (MAC_COMMON_REG_BASE  + 0x184)
/* 接收数据帧Complete Ring基地址配置寄存器MSB */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_MSB_REG            (MAC_COMMON_REG_BASE  + 0x188)
/* 接收普通数据帧Free Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_SIZE_REG          (MAC_COMMON_REG_BASE  + 0x130)
/* 接收小包数据帧Free Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_SIZE_REG         (MAC_COMMON_REG_BASE  + 0x134)
/* 接收数据帧Complete Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_SIZE_REG                (MAC_COMMON_REG_BASE  + 0x194)
/* 接收普通数据帧Free Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_WPTR_REG          (MAC_COMMON_REG_BASE  + 0x14C)
/* 接收小包数据帧Free Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_WPTR_REG         (MAC_COMMON_REG_BASE  + 0x150)
/* 接收数据帧Complete Ring的读指针配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_RPTR_REG                (MAC_COMMON_REG_BASE  + 0x1AC)

/* RX PPDU HOST描述符Free Ring基地址LSB */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_LSB_REG (MAC_COMMON_REG_BASE  + 0x128)
/* RX PPDU HOST描述符Free Ring基地址MSB */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_MSB_REG (MAC_COMMON_REG_BASE  + 0x12C)
/* RX PPDU HOST描述符Free Ring的大小 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_SIZE_REG     (MAC_COMMON_REG_BASE  + 0x148)
/* RX PPDU HOST描述符Free Ring的写指针 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_WPTR_REG     (MAC_COMMON_REG_BASE  + 0x164)
/* RX PPDU HOST描述符的Free Ring的读指针 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_CFG_REG (MAC_COMMON_REG_BASE  + 0x180)

/* 接收数据帧Complete Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_CFG_REG            (MAC_COMMON_REG_BASE  + 0x1A0)
/* 接收普通数据帧Free Ring的读指针配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG      (MAC_COMMON_REG_BASE  + 0x168)
/* 接收小包数据帧的Free Ring的读指针 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG     (MAC_COMMON_REG_BASE  + 0x16C)

#define MAC_GLB_REG_DMAC_INTR_MASK_REG                    (MAC_GLB_REG_BASE + 0x40C) /* DMAC中断屏蔽寄存器 */

#define MAC_RPU_REG_RX_CTRL_REG                           (MAC_RPU_REG_BASE + 0x8)   /* 接收控制寄存器 */
#define MAC_RPU_REG_RX_FRAMEFILT_REG                      (MAC_RPU_REG_BASE + 0xC)   /* RX过滤控制寄存器 */
#define MAC_RPU_REG_RX_FRAMEFILT2_REG                     (MAC_RPU_REG_BASE + 0x10)   /* RX过滤控制寄存器2 */

#define MAC_PPU_REG_CFG_ML0_CSI_PROCESS_REG               (MAC_PPU_REG_BASE + 0x130) /* CSI采样上报配置寄存器 */
#define MAC_PPU_REG_CFG_ML1_CSI_PROCESS_REG               (MAC_PPU_REG_BASE + 0x248) /* CSI采样上报配置寄存器 */

#define MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_LSB_REG     (MAC_PPU_REG_BASE + 0x134) /* CSI采样上报片外循环BUF基地址LSB */
#define MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_MSB_REG     (MAC_PPU_REG_BASE + 0x138) /* CSI采样上报片外循环BUF基地址MSB */
#define MAC_PPU_REG_CFG_ML1_CSI_BUF_BASE_ADDR_LSB_REG     (MAC_PPU_REG_BASE + 0x24C) /* CSI采样上报片外循环BUF基地址LSB */
#define MAC_PPU_REG_CFG_ML1_CSI_BUF_BASE_ADDR_MSB_REG     (MAC_PPU_REG_BASE + 0x250) /* CSI采样上报片外循环BUF基地址MSB */

#define MAC_PPU_REG_CFG_ML0_CSI_BUF_SIZE_REG              (MAC_PPU_REG_BASE + 0x13C) /* CSI采样上报片外循环BUF大小 */
#define MAC_PPU_REG_CFG_ML1_CSI_BUF_SIZE_REG              (MAC_PPU_REG_BASE + 0x254) /* CSI采样上报片外循环BUF大小 */

#define MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_LSB_0_REG  (MAC_PPU_REG_BASE + 0x148) /* CSI采样白名单低32位 */
#define MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_MSB_0_REG  (MAC_PPU_REG_BASE + 0x1C8) /* CSI采样白名单高16位 */
#define MAC_PPU_REG_CFG_ML1_CSI_WHITELIST_ADDR_LSB_0_REG  (MAC_PPU_REG_BASE + 0x260) /* CSI采样白名单低32位 */
#define MAC_PPU_REG_CFG_ML1_CSI_WHITELIST_ADDR_MSB_0_REG  (MAC_PPU_REG_BASE + 0x2E0) /* CSI采样白名单高16位 */

#define MAC_PPU_REG_LOCATION_INFO_MASK_REG                (MAC_PPU_REG_BASE + 0x508) /* 定位信息上报 */
#define MAC_PPU_REG_CSI_INFO_ADDR_MSB_REG                 (MAC_PPU_REG_BASE + 0x50C) /* CSI上报地址高32bit */
#define MAC_PPU_REG_CSI_INFO_ADDR_LSB_REG                 (MAC_PPU_REG_BASE + 0x510) /* CSI上报地址低32bit */
#define MAC_PPU_REG_FTM_INFO_ADDR_MSB_REG                 (MAC_PPU_REG_BASE + 0x514) /* FTM上报地址高32bit */
#define MAC_PPU_REG_FTM_INFO_ADDR_LSB_REG                 (MAC_PPU_REG_BASE + 0x518) /* FTM上报地址低32bit */

#define MAC_PPU_REG_FTM_PROCESS_REG                       (MAC_PPU_REG_BASE + 0x8)   /* FTM采样上报配置寄存器 */
#define MAC_PPU_REG_FTM_BUF_BASE_ADDR_LSB_REG             (MAC_PPU_REG_BASE + 0xC)   /* FTM采样上报片外循环BUF基地址LSB */
#define MAC_PPU_REG_FTM_BUF_BASE_ADDR_MSB_REG             (MAC_PPU_REG_BASE + 0x10)  /* FTM采样上报片外循环BUF基地址MSB */
#define MAC_PPU_REG_FTM_WHITELIST_ADDR_LSB_0_REG          (MAC_PPU_REG_BASE + 0x14)  /* FTM采样白名单低32位 */
#define MAC_PPU_REG_FTM_WHITELIST_ADDR_MSB_0_REG          (MAC_PPU_REG_BASE + 0x94)  /* FTM采样白名单高16位 */

#define MAC_GLB_REG_COMMON_TIMER_MASK_REG                  (MAC_GLB_REG_BASE + 0x428) /* DMAC通用计数器中断屏蔽寄存器 */
#define MAC_TMU_REG_COMMON_TIMER_CTRL_0_REG                (MAC_TMU_REG_BASE + 0xEC)  /* 通用定时器相关参数配置 */
#define MAC_TMU_REG_COMMON_TIMER_VAL_0_REG                 (MAC_TMU_REG_BASE + 0x10C) /* 通用定时器定时值配置 */

#define MAC_GRP_HAC_REG_RPT_MON_GRP_HAC2_REG               (MAC_GRP_HAC_REG_BASE  + 0x448)

/* VAP当前的Timestamp状态值（低32bit），单位us */
#define MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_12_REG      (MAC_TMU_REG_BASE + 0x634)
#define MP12_W_SOFT_INT_SET_AON_REG (MP12_GLBA_CTL_RB_BASE + 0x134)

/* MAC 通用定时器 */
#define MP12_MAC_COMMON_TIMER_OFFSET 0x4
#define MP12_MAC_MAX_COMMON_TIMER 31 /* 通用定时器个数 */
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_OFFSET  18
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK    0x40000
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_OFFSET   17
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK     0x20000
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_NORM_RING_EMPTY_OFFSET    16
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_NORM_RING_EMPTY_MASK      0x10000
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_OFFSET        14
#define MP12_MAC_COMMON_REG_RPT_HOST_INTR_RX_COMPLETE_MASK          0x4000

#define mp12_check_is_mac_common_reg_addr(_addr)    \
    (((_addr) >= MAC_COMMON_REG_BASE) && \
        ((_addr) <= (MAC_GRP_HAC_REG_RPT_MON_GRP_HAC2_REG)))

void mp12_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t irq);
void mp12_host_mac_irq_unmask(hal_host_device_stru *hal_device, uint32_t irq);
void mp12_clear_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t status);
void mp12_clear_host_mac_common_int_status(hal_host_device_stru *hal_device, uint32_t status);
void mp12_get_host_mac_common_int_mask(hal_host_device_stru *hal_device, uint32_t *mask);
void mp12_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *mask);
void mp12_get_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t *status);
void mp12_get_host_mac_common_int_status(hal_host_device_stru *hal_dev, uint32_t *status);
void mp12_set_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t intr_mask);
void mp12_set_host_mac_common_irq_mask(hal_host_device_stru *hal_device, uint32_t intr_mask);
void mp12_clear_host_phy_int_status(hal_host_device_stru *hal_device, hal_intr_rpt_stru *status);
void mp12_set_host_phy_int_mask(hal_host_device_stru *hal_device, hal_intr_state_stru *mask);
void mp12_get_host_phy_int_mask(hal_host_device_stru *hal_device, hal_intr_state_stru *mask);
void mp12_get_host_phy_int_status(hal_host_device_stru *hal_device, hal_intr_state_stru *status);
uint32_t mp12_host_regs_addr_init(hal_host_device_stru *hal_device);
uint32_t mp12_regs_addr_get_offset(uint8_t device_id, uint32_t reg_addr);
int32_t mp12_host_init_common_timer(hal_mac_common_timer *mac_timer);
void mp12_host_set_mac_common_timer(hal_mac_common_timer *mac_common_timer);
void mp12_host_set_mac_common_timer_no_pm_wakeup(hal_mac_common_timer *mac_common_timer);
uint32_t mp12_host_get_tsf_lo(hal_host_device_stru *hal_device, uint8_t hal_vap_id, uint32_t *tsf);
hal_mac_common_timer *mp12_get_host_mac_common_timer_ptr(uint8_t id);
void mp12_host_mac_phy_irq_mask(void);
void mp12_host_intr_regs_init(uint8_t hal_dev_id);
void mp12_host_common_intr_regs_init(void);
#endif
