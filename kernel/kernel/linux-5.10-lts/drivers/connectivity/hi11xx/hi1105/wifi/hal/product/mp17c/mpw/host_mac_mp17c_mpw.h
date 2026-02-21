/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明   : HOST MAC OPS接口
 * 作者       : wifi
 * 创建日期   : 2012年10月18日
 */

#ifndef HOST_MAC_MP17C_MPW_H
#define HOST_MAC_MP17C_MPW_H

#include "frw_ext_if.h"
#include "hal_common.h"
#include "host_hal_device.h"
#include "oal_ext_if.h"

/* MP17C MAC寄存器基地址 */
#define MAC_COMMON_REG_BASE  (0x40020000)
#define MAC_LUT_REG_BASE     (0x40021000)
#define MAC_PPU_REG_BASE     (0x40022000)
#define MAC_GRP_HAC_REG_BASE (0x40023000)
#define MAC_GLB_REG_BASE     (0x40030000)
#define MAC_CTRL_REG_BASE    (0x40031000)
#define MAC_TPU_REG_BASE     (0x40032000)
#define MAC_RPU_REG_BASE     (0x40033000)
#define MAC_TMU_REG_BASE     (0x40034000)

/* MP17C PHY寄存器基地址 */
#define DFX_REG_BANK_BASE           (0x40074000)
#define RX_CHN_EST_REG_BANK_BASE    (0x40082800)
#define RF_CTRL_REG_BANK_0_BASE     (0x40074800)
#define CALI_TEST_REG_BANK_0_BASE   (0x40071000)
#define PHY_GLB_REG_BANK_BASE       (0x40070800)
#define MP17C_GLB_CTL_RB_BASE_BASE  (0x40000000)


/* master: 0x4002_0000~0x4002_ffff slave: 0x4003_0000~0x4003_ffff */
#define MP17C_MAC_BANK_REG_OFFSET 0x10000
/* master: 0x4006_0000~0x          slave: 0x400a_0000~0x */
#define MP17C_PHY_BANK_REG_OFFSET 0x40000

/* PHY中断状态上报寄存器 */
#define DFX_REG_BANK_PHY_INTR_RPT_REG                   (DFX_REG_BANK_BASE + 0x144)
/* PHY中断状态上报寄存器2 */
#define DFX_REG_BANK_PHY_INTR_RPT2_REG                  (DFX_REG_BANK_BASE + 0x148)
/* PHY给host中断MASK寄存器 */
#define DFX_REG_BANK_CFG_INTR_MASK_HOST_REG             (DFX_REG_BANK_BASE + 0x20)
/* PHY给host中断MASK寄存器2 */
#define DFX_REG_BANK_CFG_INTR_MASK_HOST2_REG            (DFX_REG_BANK_BASE + 0x2C)
/* PHY中断清除寄存器 */
#define DFX_REG_BANK_CFG_INTR_CLR_REG                   (DFX_REG_BANK_BASE + 0x28)
/* PHY中断清除寄存器2 */
#define DFX_REG_BANK_CFG_INTR_CLR2_REG                  (DFX_REG_BANK_BASE + 0x34)
/* FTM配置寄存器 */
#define CALI_TEST_REG_BANK_0_FTM_CFG_REG                (CALI_TEST_REG_BANK_0_BASE + 0x37C)
/* 信道估计通用寄存器 */
#define RX_CHN_EST_REG_BANK_CHN_EST_COM_REG             (RX_CHN_EST_REG_BANK_BASE + 0x248)
#define RF_CTRL_REG_BANK_0_CFG_ONLINE_CALI_USE_RF_REG_EN_REG   (RF_CTRL_REG_BANK_0_BASE + 0x24)
#define CALI_TEST_REG_BANK_0_FIFO_FORCE_EN_REG                 (CALI_TEST_REG_BANK_0_BASE + 0x28)
/* 频宽模式配置寄存器 */
#define PHY_GLB_REG_BANK_PHY_BW_MODE_STA_REG           (PHY_GLB_REG_BANK_BASE + 0x8)

/* MAC 寄存器 */
#define MAC_GLB_REG_HOST_INTR_STATUS_REG                     (MAC_GLB_REG_BASE + 0x504) /* 上报到Host的中断状态寄存器 */
#define MAC_GLB_REG_HOST_INTR_CLR_REG                        (MAC_GLB_REG_BASE + 0x400) /* Host中断清除寄存器 */
#define MAC_GLB_REG_HOST_INTR_MASK_REG                       (MAC_GLB_REG_BASE + 0x404) /* Host中断屏蔽寄存器 */

#define MAC_COMMON_REG_HOST_COMMON_INTR_STATUS_REG           (MAC_COMMON_REG_BASE + 0x218) /* 上报到Host的中断状态寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_CLR_REG              (MAC_COMMON_REG_BASE + 0xB0)  /* MAC COMMON Host中断清除寄存器 */
#define MAC_COMMON_REG_HOST_COMMON_INTR_MASK_REG             (MAC_COMMON_REG_BASE + 0xB4)  /* MAC COMMON Host中断屏蔽寄存器 */

/* rx ring reset ctrl,每个bit对应1个ring */
#define MP17C_MPW_RING_RESET_PPDU_FREE_RING_MASK         0x1
#define MP17C_MPW_RING_RESET_NORM_MGMT_FREE_RING_MASK    0x2
#define MP17C_MPW_RING_RESET_SMALL_MGMT_FREE_RING_MASK   0x4
#define MP17C_MPW_RING_RESET_LARGE_MGMT_FREE_RING_MASK   0x8
#define MP17C_MPW_RING_RESET_NORM_DATA_FREE_RING_MASK    0x10
#define MP17C_MPW_RING_RESET_SMALL_DATA_FREE_RING_MASK   0x20
#define MP17C_MPW_RING_RESET_PPDU_COMP_RING_MASK         0x40
#define MP17C_MPW_RING_RESET_MGMT_COMP_RING_MASK         0x80
#define MP17C_MPW_RING_RESET_DATA_COMP_RING_MASK         0x100
#define MP17C_MPW_RING_RESET_PPDU_HOST_FREE_RING_MASK    0x200

#define MAC_RPU_REG_RX_RING_RESET_CTRL_REG               (MAC_RPU_REG_BASE + 0x58) /* 接收ring复位配置寄存器 */

#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_RPTR_REG      (MAC_RPU_REG_BASE + 0xC00) /* 接收普通数据帧Free Ring的读指针上报 */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_RPTR_REG     (MAC_RPU_REG_BASE + 0xC04) /* 接收小包数据帧Free Ring的读指针上报 */
#define MAC_RPU_REG_RX_DATA_CMP_RING_WPTR_REG            (MAC_RPU_REG_BASE + 0xC08) /* 接收数据帧Complete Ring的写指针上报 */
#define MAC_RPU_REG_RPT_HOST_FREE_RING_STATUS_REG        (MAC_RPU_REG_BASE + 0xC0C) /* FREE RING上报信息 */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_REG (MAC_RPU_REG_BASE + 0xC10) /* RX PPDU HOST描述符Free Ring读指针上报 */
#define MAC_RPU_REG_RPT_PPDU_HOST_FREE_RING_STATUS_REG   (MAC_RPU_REG_BASE + 0xC14) /* PPDU HOST FREE RING上报信息 */

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

#define MAC_RPU_REG_RX_DATA_BUFF_LEN_REG                 (MAC_RPU_REG_BASE + 0x5C)  /* RX数据帧Buffer大小配置寄存器 */
#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_ADDR_LSB_REG  (MAC_RPU_REG_BASE + 0x64)  /* 接收普通数据帧Free Ring基地址配置寄存器LSB */
#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_ADDR_MSB_REG  (MAC_RPU_REG_BASE + 0x68)  /* 接收普通数据帧Free Ring基地址配置寄存器MSB */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_ADDR_LSB_REG (MAC_RPU_REG_BASE + 0x6C)  /* 接收小包数据帧Free Ring基地址配置寄存器_LSB */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_ADDR_MSB_REG (MAC_RPU_REG_BASE + 0x70)  /* 接收小包数据帧Free Ring基地址配置寄存器_MSB */
#define MAC_RPU_REG_RX_DATA_CMP_RING_ADDR_LSB_REG        (MAC_RPU_REG_BASE + 0xE0)  /* 接收数据帧Complete Ring基地址配置寄存器LSB */
#define MAC_RPU_REG_RX_DATA_CMP_RING_ADDR_MSB_REG        (MAC_RPU_REG_BASE + 0xE4)  /* 接收数据帧Complete Ring基地址配置寄存器MSB */
#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_SIZE_REG      (MAC_RPU_REG_BASE + 0x8C)  /* 接收普通数据帧Free Ring的大小配置寄存器 */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_SIZE_REG     (MAC_RPU_REG_BASE + 0x90)  /* 接收小包数据帧Free Ring的大小配置寄存器 */
#define MAC_RPU_REG_RX_DATA_CMP_RING_SIZE_REG            (MAC_RPU_REG_BASE + 0xF0)  /* 接收数据帧Complete Ring的大小配置寄存器 */
#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_WPTR_REG      (MAC_RPU_REG_BASE + 0xA8)  /* 接收普通数据帧Free Ring的写指针配置寄存器 */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_WPTR_REG     (MAC_RPU_REG_BASE + 0xAC)  /* 接收小包数据帧Free Ring的写指针配置寄存器 */
#define MAC_RPU_REG_RX_DATA_CMP_RING_RPTR_REG            (MAC_RPU_REG_BASE + 0x108) /* 接收数据帧Complete Ring的读指针配置寄存器 */

/* RX PPDU HOST描述符Free Ring基地址LSB */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_LSB_REG  (MAC_RPU_REG_BASE + 0x84)
/* RX PPDU HOST描述符Free Ring基地址MSB */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_ADDR_MSB_REG  (MAC_RPU_REG_BASE + 0x88)
/* RX PPDU HOST描述符Free Ring的大小 */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_SIZE_REG      (MAC_RPU_REG_BASE + 0xA4)
/* RX PPDU HOST描述符Free Ring的写指针 */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_WPTR_REG      (MAC_RPU_REG_BASE + 0xC0)
/* RX PPDU HOST描述符的Free Ring的读指针 */
#define MAC_RPU_REG_RX_PPDU_HOST_DESC_FREE_RING_RPTR_CFG_REG  (MAC_RPU_REG_BASE + 0xDC)

/* 接收数据帧Complete Ring的写指针配置寄存器 */
#define MAC_RPU_REG_RX_DATA_CMP_RING_WPTR_CFG_REG            (MAC_RPU_REG_BASE + 0xFC)
#define MAC_RPU_REG_RX_NORM_DATA_FREE_RING_RPTR_CFG_REG      (MAC_RPU_REG_BASE + 0xC4)  /* 接收普通数据帧Free Ring的读指针配置寄存器 */
#define MAC_RPU_REG_RX_SMALL_DATA_FREE_RING_RPTR_CFG_REG     (MAC_RPU_REG_BASE + 0xC8)  /* 接收小包数据帧的Free Ring的读指针 */
#define MAC_RPU_REG_RX_FRAMEFILT_REG                         (MAC_RPU_REG_BASE + 0x8)   /* RX过滤控制寄存器 */
#define MAC_RPU_REG_RX_FRAMEFILT2_REG                        (MAC_RPU_REG_BASE + 0xC)   /* RX过滤控制寄存器2 */

#define MAC_GLB_REG_DMAC_INTR_MASK_REG                    (MAC_GLB_REG_BASE + 0x40C) /* DMAC中断屏蔽寄存器 */
#define MAC_RPU_REG_RX_CTRL_REG                           (MAC_RPU_REG_BASE + 0x4)   /* 接收控制寄存器 */

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

/* VAP当前的Timestamp状态值（低32bit），单位us */
#define MAC_TMU_REG_VAP_TSFTIMER_RDVALL_STATUS_12_REG      (MAC_TMU_REG_BASE + 0x634)
#define MP17C_W_SOFT_INT_SET_AON_REG (MP17C_GLB_CTL_RB_BASE_BASE + 0x134)

/* MAC 通用定时器 */
#define MP17C_MAC_COMMON_TIMER_OFFSET 0x4
#define MP17C_MAC_MAX_COMMON_TIMER 31 /* 通用定时器个数 */
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_OFFSET  5
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_DATA_RING_OVERRUN_MASK    0x20
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_OFFSET   4
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_SMALL_RING_EMPTY_MASK     0x10
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_NORM_RING_EMPTY_OFFSET    3
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_NORM_RING_EMPTY_MASK      0x8
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_RX_COMPLETE_OFFSET        2
#define MP17C_MAC_GLB_REG_RPT_HOST_INTR_RX_COMPLETE_MASK          0x4

void mp17c_mpw_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t irq);
void mp17c_mpw_host_mac_irq_unmask(hal_host_device_stru *hal_device, uint32_t irq);
void mp17c_mpw_clear_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t status);
void mp17c_mpw_clear_host_mac_common_int_status(hal_host_device_stru *hal_dev, uint32_t status);
void mp17c_mpw_get_host_mac_common_int_mask(hal_host_device_stru *hal_device, uint32_t *mask);
void mp17c_mpw_get_host_mac_int_mask(hal_host_device_stru *hal_device, uint32_t *mask);
void mp17c_mpw_get_host_mac_int_status(hal_host_device_stru *hal_device, uint32_t *status);
void mp17c_mpw_get_host_mac_common_int_status(hal_host_device_stru *hal_device, uint32_t *status);
void mp17c_mpw_set_host_mac_irq_mask(hal_host_device_stru *hal_device, uint32_t intr_mask);
void mp17c_mpw_clear_host_phy_int_status(hal_host_device_stru *hal_device, uint32_t status);
void mp17c_mpw_set_host_phy_int_mask(hal_host_device_stru *hal_device, uint32_t mask);
void mp17c_mpw_get_host_phy_int_mask(hal_host_device_stru *hal_device, uint32_t *mask);
void mp17c_mpw_get_host_phy_int_status(hal_host_device_stru *hal_device, uint32_t *status);
uint32_t mp17c_mpw_host_regs_addr_init(hal_host_device_stru *hal_device);
uint32_t mp17c_mpw_regs_addr_get_offset(uint8_t device_id, uint32_t reg_addr);
int32_t mp17c_mpw_host_init_common_timer(hal_mac_common_timer *mac_timer);
void mp17c_mpw_host_set_mac_common_timer(hal_mac_common_timer *mac_common_timer);
void mp17c_mpw_host_set_mac_common_timer_no_pm_wakeup(hal_mac_common_timer *mac_common_timer);
uint32_t mp17c_mpw_host_get_tsf_lo(hal_host_device_stru *hal_device, uint8_t hal_vap_id, uint32_t *tsf);
hal_mac_common_timer *mp17c_mpw_get_host_mac_common_timer_ptr(uint8_t id);
void mp17c_mpw_host_mac_phy_irq_mask(void);
void mp17c_mpw_host_intr_regs_init(uint8_t hal_dev_id);
void mp17c_mpw_host_common_intr_regs_init(void);
#endif
