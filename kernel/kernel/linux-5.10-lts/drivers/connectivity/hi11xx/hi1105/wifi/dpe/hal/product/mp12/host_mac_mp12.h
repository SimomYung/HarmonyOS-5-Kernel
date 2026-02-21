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
#define DFX_REG_BANK_CFG_INTR_MASK_HOST_REG (DFX_REG_BANK_BASE + 0x20)         /* PHY给host中断MASK寄存器 */
#define DFX_REG_BANK_CFG_INTR_CLR_REG (DFX_REG_BANK_BASE + 0x28)               /* PHY中断清除寄存器 */
#define DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG (DFX_REG_BANK_BASE + 0x2C) /* PHY给host中断MASK寄存器2 */
#define DFX_REG_BANK_CFG_INTR_CLR2_REG (DFX_REG_BANK_BASE + 0x34) /* PHY中断清除寄存器2 */
#define DFX_REG_BANK_PHY_INTR_RPT_REG (DFX_REG_BANK_BASE + 0x14C)              /* PHY中断状态上报寄存器 */
#define DFX_REG_BANK_PHY_INTR_RPT2_REG (DFX_REG_BANK_BASE + 0x150) /* PHY中断状态上报寄存器2 */

#define CALI_TEST_REG_BANK_0_FIFO_FORCE_EN_REG (CALI_TEST_REG_BANK_0_BASE + 0x28) /* FIFO使能对应的寄存器配置地址 */
#define CALI_TEST_REG_BANK_0_FTM_CFG_REG (CALI_TEST_REG_BANK_0_BASE + 0x2D8)   /* FTM配置寄存器 */
#define RX_CHN_EST_REG_BANK_CHN_EST_COM_REG (RX_CHN_EST_REG_BANK_BASE + 0x264) /* 信道估计通用寄存器 */
#define RF_CTRL_REG_BANK_0_CFG_ONLINE_CALI_USE_RF_REG_EN_REG (RF_CTRL_REG_BANK_0_BASE + 0x30)

#define PHY_GLB_REG_BANK_PHY_BW_MODE_STA_REG           (PHY_GLB_REG_BANK_BASE + 0x4) /* 频宽模式配置寄存器 */

/* MAC 寄存器 */
#define MAC_GLB_REG_HOST_INTR_MASK_REG                       (MAC_GLB_REG_BASE + 0x284) /* Host中断屏蔽寄存器 */
#define MAC_GLB_REG_DMAC_INTR_MASK_REG                       (MAC_GLB_REG_BASE + 0x288) /* DMAC中断屏蔽寄存器 */
#define MAC_GLB_REG_COMMON_TIMER_MASK_REG                    (MAC_GLB_REG_BASE + 0x298) /* DMAC通用计数器中断屏蔽寄存器 */
#define MAC_GLB_REG_HOST_INTR_CLR_REG                        (MAC_GLB_REG_BASE + 0x2E8) /* Host中断清除寄存器 */
#define MAC_GLB_REG_HOST_INTR_STATUS_REG                     (MAC_GLB_REG_BASE + 0x364) /* 上报到Host的中断状态寄存器 */


#define MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG             (MAC_COMMON_REG_BASE + 0x34)  /* MAC COMMON Host中断屏蔽寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG              (MAC_COMMON_REG_BASE + 0x13C)  /* MAC COMMON Host中断清除寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG           (MAC_COMMON_REG_BASE + 0x218) /* 上报到Host的中断状态寄存器 */

/* rx ring reset ctrl,每个bit对应1个ring */
#define MP12_RING_RESET_NORM_DATA_FREE_RING_MASK    BIT0
#define MP12_RING_RESET_SMALL_DATA_FREE_RING_MASK   BIT1
#define MP12_RING_RESET_NORM_MGMT_FREE_RING_MASK    BIT2
#define MP12_RING_RESET_SMALL_MGMT_FREE_RING_MASK   BIT3
#define MP12_RING_RESET_LARGE_MGMT_FREE_RING_MASK   BIT4
#define MP12_RING_RESET_PPDU_FREE_RING_MASK         BIT5
#define MP12_RING_RESET_PPDU_HOST_FREE_RING_MASK    BIT6
#define MP12_RING_RESET_DATA_COMP_RING_MASK         BIT7
#define MP12_RING_RESET_MGMT_COMP_RING_MASK         BIT8
#define MP12_RING_RESET_PPDU_COMP_RING_MASK         BIT9

#define MAC_LUT_REG_ML0_TX_INFO_BUF_WPTR_REG           (MAC_LUT_REG_BASE + 0x100) /* BA INFO BUFFER的写指针上报 */

/* 接收普通数据帧Free Ring基地址配置寄存器LSB */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG        (MAC_COMMON_REG_BASE + 0x40)
/* 接收普通数据帧Free Ring基地址配置寄存器MSB */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG        (MAC_COMMON_REG_BASE + 0x44)
/* 接收小包数据帧Free Ring基地址配置寄存器_LSB */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG       (MAC_COMMON_REG_BASE + 0x48)
/* 接收小包数据帧Free Ring基地址配置寄存器_MSB */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG       (MAC_COMMON_REG_BASE + 0x4C)
/* RX PPDU HOST描述符Free Ring基地址LSB */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_LSB_REG   (MAC_COMMON_REG_BASE + 0x60)
/* RX PPDU HOST描述符Free Ring基地址MSB */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_MSB_REG   (MAC_COMMON_REG_BASE + 0x64)
/* 接收普通数据帧Free Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_SIZE_REG            (MAC_COMMON_REG_BASE + 0x68)
/* 接收小包数据帧Free Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_SIZE_REG           (MAC_COMMON_REG_BASE + 0x6C)
/* RX PPDU HOST描述符Free Ring的大小 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_SIZE_REG       (MAC_COMMON_REG_BASE + 0x80)
/* 接收普通数据帧Free Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_WPTR_REG            (MAC_COMMON_REG_BASE + 0x84)
/* 接收小包数据帧Free Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_WPTR_REG           (MAC_COMMON_REG_BASE + 0x88)
/* RX PPDU HOST描述符Free Ring的写指针 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_WPTR_REG       (MAC_COMMON_REG_BASE + 0x9C)
/* 接收数据帧Complete Ring基地址配置寄存器LSB */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_LSB_REG              (MAC_COMMON_REG_BASE + 0xA0)
/* 接收数据帧Complete Ring基地址配置寄存器MSB */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_ADDR_MSB_REG              (MAC_COMMON_REG_BASE + 0xA4)
/* 接收数据帧Complete Ring的大小配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_SIZE_REG                  (MAC_COMMON_REG_BASE + 0xB0)
/* 接收数据帧Complete Ring的读指针配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_RPTR_REG                  (MAC_COMMON_REG_BASE + 0xBC)
/* 接收普通数据帧Free Ring的读指针配置寄存器 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG        (MAC_COMMON_REG_BASE + 0x108)
/* 接收小包数据帧的Free Ring的读指针 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG       (MAC_COMMON_REG_BASE + 0x10C)
/* RX PPDU HOST描述符的Free Ring的读指针 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_CFG_REG   (MAC_COMMON_REG_BASE + 0x120)
/* 接收数据帧Complete Ring的写指针配置寄存器 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_CFG_REG              (MAC_COMMON_REG_BASE + 0x124)
/* 接收ring复位配置寄存器 */
#define MAC_COMMON_REG_RX_RING_RESET_CTRL_REG                     (MAC_COMMON_REG_BASE + 0x148)
/* RING指针上报host侧地址 */
#define MAC_COMMON_REG_CFG_MAC_RING_PTR_RPT_HOST_ADDR_LSB_REG     (MAC_COMMON_REG_BASE + 0x19C)
/* RING指针上报host侧地址 */
#define MAC_COMMON_REG_CFG_MAC_RING_PTR_RPT_HOST_ADDR_MSB_REG     (MAC_COMMON_REG_BASE + 0x1A0)
/* RX_RING指针上报模式配置寄存器 */
#define MAC_COMMON_REG_CFG_RING_PTR_RPT_MODE_REG                  (MAC_COMMON_REG_BASE + 0x1B4)
/* 接收普通数据帧Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_NORM_DATA_FREE_RING_RPTR_REG            (MAC_COMMON_REG_BASE + 0x2A8)
/* 接收小包数据帧Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_SMALL_DATA_FREE_RING_RPTR_REG           (MAC_COMMON_REG_BASE + 0x2AC)
/* 接收数据帧Complete Ring的写指针上报 */
#define MAC_COMMON_REG_RX_DATA_CMP_RING_WPTR_REG                  (MAC_COMMON_REG_BASE + 0x2B0)
/* FREE RING上报信息 */
#define MAC_COMMON_REG_RPT_HOST_FREE_RING_STATUS_REG              (MAC_COMMON_REG_BASE + 0x2B4)
/* RX PPDU HOST描述符Free Ring的读指针上报 */
#define MAC_COMMON_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_REG       (MAC_COMMON_REG_BASE + 0x2B8)
/* PPDU HOST FREE RING上报信息 */
#define MAC_COMMON_REG_RPT_PPDU_HOST_FREE_RING_STATUS_REG         (MAC_COMMON_REG_BASE + 0x2BC)

#define MAC_PPU_REG_CFG_ML0_CSI_PROCESS_REG               (MAC_PPU_REG_BASE + 0x4) /* CSI采样上报配置寄存器 */
#define MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_LSB_REG     (MAC_PPU_REG_BASE + 0x8) /* CSI采样上报片外循环BUF基地址LSB */
#define MAC_PPU_REG_CFG_ML0_CSI_BUF_BASE_ADDR_MSB_REG     (MAC_PPU_REG_BASE + 0xC) /* CSI采样上报片外循环BUF基地址MSB */
#define MAC_PPU_REG_CFG_ML0_CSI_BUF_SIZE_REG              (MAC_PPU_REG_BASE + 0x10) /* CSI采样上报片外循环BUF大小 */
#define MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_LSB_0_REG  (MAC_PPU_REG_BASE + 0x14) /* CSI采样白名单低32位 */
#define MAC_PPU_REG_CFG_ML0_CSI_WHITELIST_ADDR_MSB_0_REG  (MAC_PPU_REG_BASE + 0x94) /* CSI采样白名单高16位 */
#define MAC_PPU_REG_LOCATION_INFO_MASK_REG                (MAC_PPU_REG_BASE + 0x134) /* 定位信息上报 */
#define MAC_PPU_REG_CSI_INFO_ADDR_MSB_REG                 (MAC_PPU_REG_BASE + 0x138) /* CSI上报地址高32bit */
#define MAC_PPU_REG_CSI_INFO_ADDR_LSB_REG                 (MAC_PPU_REG_BASE + 0x13C) /* CSI上报地址低32bit */

#define MAC_RPU_REG_RX_FRAMEFILT_REG                      (MAC_RPU_REG_BASE + 0x30)   /* RX过滤控制寄存器 */
#define MAC_RPU_REG_RX_FRAMEFILT2_REG                     (MAC_RPU_REG_BASE + 0x34)   /* RX过滤控制寄存器2 */
#define MAC_RPU_REG_RX_CTRL_REG                           (MAC_RPU_REG_BASE + 0x3C)   /* 接收控制寄存器 */
#define MAC_RPU_REG_RX_DATA_BUFF_LEN_REG                  (MAC_RPU_REG_BASE + 0x68) /* RX数据帧Buffer大小配置寄存器 */

#define MAC_TMU_REG_COMMON_TIMER_CTRL_0_REG                (MAC_TMU_REG_BASE + 0x14C)  /* 通用定时器相关参数配置 */
#define MAC_TMU_REG_COMMON_TIMER_VAL_0_REG                 (MAC_TMU_REG_BASE + 0x16C) /* 通用定时器定时值配置 */
/* VAP当前的Timestamp状态值（低32bit），单位us */
#define MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_4_REG       (MAC_TMU_REG_BASE + 0x614)

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

#define	MAC_COMMON_REG_CFG_CMP_RING_INTR_MODE_LEN	                3
#define	MAC_COMMON_REG_CFG_CMP_RING_INTR_MODE_OFFSET	            10
#define	MAC_COMMON_REG_CFG_FREE_CMP_RING_RPT_MODE_LEN	            10
#define	MAC_COMMON_REG_CFG_FREE_CMP_RING_RPT_MODE_OFFSET	        0

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
void mp12_clear_host_phy_int_status(hal_host_device_stru *hal_device, hal_intr_state_stru *status);
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
uint8_t mp12_hw_vap_id_to_hal_vap_id(uint8_t hw_vap_id);
#endif
