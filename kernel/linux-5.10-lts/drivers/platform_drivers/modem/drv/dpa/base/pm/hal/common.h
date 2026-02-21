/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */



#ifndef DPA_PM_COMMON_H
#define DPA_PM_COMMON_H

#include <bsp_pmctrl.h>

/* ao sctrl  */
#define SC_BISR_STAT0 0x04A0
#define BISR_REPAIR_ACK BIT(6)

#define SC_DPA_MTCMOS_EN 0x0B60
#define DPA_MTCMOS_EN BIT(0)

#define SC_DPA_MTCMOS_DIS 0x0B64
#define DPA_MTCMOS_DIS BIT(0)

#define SC_DPA_MTCMOS_STAT 0x0B6C
#define SC_DPA_MTCMOS_RDY BIT(0)

#define SC_DPA_POFF_CTRL 0x0B54
#define DPA_MEM_CTRL_POFF1_MASK GENMASK(7, 6)
#define DPA_MEM_CTRL_POFF1_SD_MASK BIT(7 + 16)
#define DPA_MEM_CTRL_POFF1_DS_MASK BIT(6 + 16)
#define DPA_MEM_CTRL_POFF1_SD BIT(7)
#define DPA_MEM_CTRL_POFF1_DS BIT(6)

#define DPA_MEM_CTRL_POFF0_MASK GENMASK(5, 4)
#define DPA_MEM_CTRL_POFF0_SD_MASK BIT(5 + 16)
#define DPA_MEM_CTRL_POFF0_DS_MASK BIT(4 + 16)
#define DPA_MEM_CTRL_POFF0_SD BIT(5)
#define DPA_MEM_CTRL_POFF0_DS BIT(4)

#define DPA_RET_MEM_CTRL_POFF0_MASK GENMASK(1, 0)
#define DPA_RET_MEM_CTRL_POFF0_SD_MASK BIT(1 + 16)
#define DPA_RET_MEM_CTRL_POFF0_DS_MASK BIT(0 + 16)
#define DPA_RET_MEM_CTRL_POFF0_SD BIT(1)
#define DPA_RET_MEM_CTRL_POFF0_DS BIT(0)

#define SC_DPA_POFF_STAT 0x0B58
#define DPA_MEM_CTRL_POFF_OUT1_MASK GENMASK(7, 6)
#define DPA_MEM_CTRL_POFF_OUT0_MASK GENMASK(5, 4)

#define DPA_RET_MEM_CTRL_POFF_OUT0_MASK GENMASK(1, 0)
#define DPA_RET_MEM_CTRL_POFF_OUT0_SD BIT(1)
#define DPA_RET_MEM_CTRL_POFF_OUT0_DS BIT(0)

#define SC_AO_NOSEC_ISO_EN 0x0B90
#define DPA_ISO_EN BIT(0)

#define SC_AO_NOSEC_ISO_DIS 0x0B94
#define DPA_ISO_DIS BIT(0)

/* peri crg */
#define PD_CRG_CLKEN2 0x0020
#define DPA_TOP_PD_CLK_ENABLE BIT(2)

#define PD_CRG_CLKDIS2 0x0024
#define DPA_TOP_PD_CLK_DIS BIT(2)

#define PD_CRG_CLKCG_STAT2 0x002C
#define DPA_TOP_PD_CLKCG_STAT BIT(2)

#define PD_CRG_SRSTEN2 0x0080
#define DPA_PD_SRST_EN BIT(6)

#define PD_CRG_SRSTDIS2 0x0084
#define DPA_PD_SRST_DIS BIT(6)

#define PD_CRG_SRSTSTAT2 0x0088
#define DPA_PD_SRST_REQ BIT(6)

#define PD_CRG_CLK_SEL1 0x258
#define DPA_REFCLK_SEL_MASK_SHIFT16 (BIT(8 + 16) | BIT(9 + 16) | BIT(10 + 16) | BIT(11 + 16))
#define DPA_REFCLK_SEL_MASK GENMASK(11, 8)

/* peri sysctrl */
#define SC_PERI_CTRL0 0x0000
#define PDC_PWRDSM_REQ_DPA2SYS_MASK GENMASK(7, 6)
#define PDC_PWRDM_REQ_DPA2SYS_MASK BIT(7 + 16)
#define PDC_PWRDS_REQ_DPA2SYS_MASK BIT(6 + 16)
#define PDC_PWRDM_REQ_DPA2SYS BIT(7)
#define PDC_PWRDS_REQ_DPA2SYS BIT(6)
#define PDC_PWRDSM_REQ_SYS2DPA_MASK GENMASK(5, 4)
#define PDC_PWRDM_REQ_SYS2DPA_MASK BIT(5 + 16)
#define PDC_PWRDS_REQ_SYS2DPA_MASK BIT(4 + 16)
#define PDC_PWRDM_REQ_SYS2DPA BIT(5)
#define PDC_PWRDS_REQ_SYS2DPA BIT(4)

#define SC_PERI_STAT20 0x0150
#define PDC_PWRDSM_ACK_DPA2SYS_MASK GENMASK(7, 6)
#define PDC_PWRDM_ACK_DPA2SYS_MASK BIT(7 + 16)
#define PDC_PWRDS_ACK_DPA2SYS_MASK BIT(6 + 16)
#define PDC_PWRDM_ACK_DPA2SYS BIT(7)
#define PDC_PWRDS_ACK_DPA2SYS BIT(6)
#define PDC_PWRDSM_ACK_SYS2DPA_MASK GENMASK(5, 4)
#define PDC_PWRDM_ACK_SYS2DPA_MASK BIT(5 + 16)
#define PDC_PWRDS_ACK_SYS2DPA_MASK BIT(4 + 16)
#define PDC_PWRDM_ACK_SYS2DPA BIT(5)
#define PDC_PWRDS_ACK_SYS2DPA BIT(4)

// peri fix0 sysctrl
#define SC_INT_MASK_DPA     0x908       // DPA唤醒中断使能控制：0：使能无效，1：使能有效
#define SC_INT_STAT_DPA_RAW 0x960       // DPA唤醒中断屏蔽前状态：0：中断无效，1：中断有效
#define SC_INT_STAT_DPA_MASK 0x964      // DPA唤醒中断屏蔽后状态：0：中断无效，1：中断有效

// peri sysctrl
#define DPA_WAKEUP_INT_PERI_MASK 0x990  // DPA唤醒中断使能控制：0：使能无效，1：使能有效
#define DPA_WAKEUP_INT_PERI_RAW 0x994   // 屏蔽前中断源信号回读
#define DPA_WAKEUP_INT_PERI_MASKED 0x998  // 屏蔽前中断源信号回读

/* dpa crg */
#define SRSTDIS0 0x0104
#define TSP_CORE_SRST_DIS BIT(1)

#define SRSTSTAT0 0x0108
#define DPA_DBG_SRST_REQ BIT(1)

#define CLK_CFG0 0x0200
#define DPA_EXIT_DFS_WAIT_CYCLE_MASK GENMASK(9, 8)
#define DPA_DFS_WAIT_CYCLE_MASK_SHIFT16 (BIT(0 + 16) | BIT(1 + 16) | BIT(2 + 16) | BIT(3 + 16) | BIT(4 + 16) | BIT(5 + 16) | BIT(6 + 16) | BIT(7 + 16))
#define DPA_DFS_WAIT_CYCLE_MASK GENMASK(7, 0)

#define CLK_CFG1 0x0204
#define DPA_DFS_EN_MASK_SHIFT16 BIT(14 + 16)
#define DPA_DFS_EN_MASK GENMASK(14, 14)
#define DPA_DFS_DIV_NUM_MASK_SHIFT16 (BIT(8 + 16) | BIT(9 + 16) | BIT(10 + 16) | BIT(11 + 16) | BIT(12 + 16) | BIT(13 + 16))
#define DPA_DFS_DIV_NUM_MASK GENMASK(13, 8)
#define DPA_CLK_DIVNUM_MASK_SHIFT16 (BIT(0 + 16) | BIT(1 + 16) | BIT(2 + 16) | BIT(3 + 16) | BIT(4 + 16) | BIT(5 + 16))
#define DPA_CLK_DIVNUM_MASK GENMASK(5, 0)

/* dpa sysctrl */
#define SC_CTRL73 0x0124
#define DSS0_T0_INIT_SELFBOOT BIT(0)

#define SC_STAT0 0x0200
#define DSS0_FATAL_ERR_MASK GENMASK(3, 2)
#define DSS0_BUSY_WAIT_MODE_MASK GENMASK(1, 0)
#define DSS0_WAIT_MODE BIT(1)
#define DSS0_BUSY BIT(0)


#define SC_CTRL1 0x0004
#define DSS0_T0_ICU_SBASE_MASK GENMASK(31, 10)

#define SC_CTRL2 0x0008
#define DSS0_T0_INIT_ADDR_MASK GENMASK(31, 0)

#define SC_CTRL3 0x000C
#define DSS0_T0_INIT_SSP_MASK GENMASK(31, 4)

#define SC_CTRL7 0x001C
#define DSS0_T2_INIT_ICU_SBASE_MASK GENMASK(31, 10)

#define SC_CTRL8 0x0020
#define DSS0_T2_INIT_ADDR_MASK GENMASK(31, 0)

#define SC_CTRL9 0x0024
#define DSS0_T2_INIT_SSP_MASK GENMASK(31, 4)

#define SC_CTRL15 0x003C
#define DSS0_T2_ICU_UBASE_MASK GENMASK(31, 10)

#define SC_CTRL13 0x0034
#define DSS0_T0_ICU_UBASE_MASK GENMASK(31, 10)

#define SC_INT_CTRL1 0x904

#define SC_SOFT_RSV_CTRL0 0xA00
#define SC_SOFT_RSV_CTRL1 0xA04
#define SC_SOFT_RSV_CTRL2 0xA08
#define SC_SOFT_RSV_CTRL3 0xA0C
#define SC_SOFT_RSV_CTRL4 0xA10
#define SC_SOFT_RSV_CTRL5 0xA14
#define SC_SOFT_RSV_CTRL6 0xA18
#define SC_SOFT_RSV_CTRL7 0xA1C
#define SC_SOFT_RSV_CTRL8 0xA20
#define SC_SOFT_RSV_CTRL9 0xA24
#define SC_SOFT_RSV_CTRL10 0xA28
#define SC_SOFT_RSV_CTRL11 0xA2C
#define SC_SOFT_RSV_CTRL12 0xA30
#define SC_SOFT_RSV_CTRL13 0xA34
#define SC_SOFT_RSV_CTRL14 0xA38
#define SC_SOFT_RSV_CTRL15 0xA3C

enum dpa_rpm_stash {
    STASH_REG_ADDR = 0,
    STASH_REG_VALUE = 1,
    STASH_REG_BUTTON = 2,
};

#define SC_INT_CTRL0 0x900

// dss0_t0_init_icu_sbase是特权模式的中断基地址，DPA都运行在特权模式;
#define DSS0_THREAD_INIT_ICU_SBASE(thread_id) (0x004 + 0xC * (thread_id))

// dss0_t0_init_icu_ubase是非特权模式的中断基地址，DPA不需要配置
#define DSS0_THREAD_INIT_ICU_UBASE(thread_id) (0x034 + 0x4 * (thread_id))

#define DSS0_THREAD_INIT_ADDR(thread_id) (0x008 + 0xC * (thread_id))

// dss0_t0_init_ssp: acore没办法获取到,上电流程不需要配置
#define DSS0_THREAD_INIT_SSP(thread_id) (0x00C + 0xC * (thread_id))

#define MPSCU_MP_STATUS 0x140020
#define LAST_DSSX_RESET_DONE BIT(24)

#define MPSCU_EX_IF_DIS 0x140060
#define DSS0_CN_INTERFACE_EN BIT(0)
#define DSS0_EX2L1M_INTERFACE_EN BIT(12)

#define MPSCU_CLK_DIS 0x140080
#define DSS0_CLOCK BIT(0)

#define MPSCU_HW_RESET_DSS(n) (0x1400A4 + 0x4 * (n))
#define MPSCU_HW_RESET_DSS_MASK GENMASK(31, 0)
#define DSS_RESET BIT(0)
#define DSS_NORMAL_RESET BIT(1)
#define RESET_ABORT_CNT BIT(24)

#define MPSCU_LCK 0x140500
#define MPSCU_LCK_MASK GENMASK(31, 0)

#define MPSCU_DLP 0x140510
#define MPSCU_DLP_MASK GENMASK(31, 0)

#define DSS0_SCU_THREAD_OFFSET 0x65000

#define DSS0_SCU_INIT_ADDR_THREAD(thread_id) (DSS0_SCU_THREAD_OFFSET + 0x8000 * (thread_id) + 0x100)
#define SCU_INIT_ADDR_THREAD0 0x65100
#define SCU_INIT_ADDR_THREAD2 0x75100
#define INIT_ADDR_MASK GENMASK(31, 0)

#define DSS0_SCU_INIT_SR_THREAD(thread_id) (DSS0_SCU_THREAD_OFFSET + 0x8000 * (thread_id) + 0x120)
#define SCU_INIT_SR_THREAD0 0x65120
#define SCU_INIT_SR_THREAD2 0x75120
#define DI BIT(8)

#define SCU_HW_RESET (0x4D0A0)
#define SCU_HW_RESET_MASK GENMASK(3, 0)
#define CORE_RESET BIT(0)
#define NORMAL_RESET BIT(3)

#define DPA_PM_WAIT_TIME_400US 400
#define DPA_PM_WAIT_TIME_100US 100
#define DPA_PM_WAIT_TIME_30US  30
#define DPA_PM_WAIT_TIME_20US 20
#define DPA_PM_WAIT_TIME_10US 10
#define DPA_PM_WAIT_TIME_5US   5
#define DPA_PM_DELAY_TIME 1

#define DSS_THREAD_MAX_NUM 4

typedef enum dpa_refclk {
    DPA_REF_CLOCK_800M,
    DPA_REF_CLOCK_38P4M,
} dpa_refclk_e;

typedef enum peri_volt {
    PERI_VOLT_0P85V,
    PERI_VOLT_0P75V,
    PERI_VOLT_0P65V,
    PERI_VOLT_BOTTOM
} peri_volt_e;

struct dpa_profile {
    dpa_clk_grade_e clk_grade;
    peri_volt_e volt;
    profile_id_e profile_id;
    unsigned int div;
};

/* tool function, consecutive bits, each bit is set to 1. */
static inline void dpa_pm_bits_enable(void __iomem *ioaddr, unsigned int bitfiled)
{
    unsigned int val = readl(ioaddr);
    val |= bitfiled;
    writel(val, ioaddr);
}

/* tool function, consecutive bits, each bit is set to 1, The mask configuration function is enabled. */
static inline void dpa_pm_bits_enable_mask16(void __iomem *ioaddr, unsigned int bitfiled, unsigned int mask)
{
    unsigned int val = readl(ioaddr);
    val |= bitfiled;
    val |= mask;
    writel(val, ioaddr);
}

/* tool function, consecutive bitsc each bit is set to 0. */
static inline void dpa_pm_bits_disable(void __iomem *ioaddr, unsigned int bitfiled)
{
    unsigned int val = readl(ioaddr);
    val &= ~bitfiled;
    writel(val, ioaddr);
}

/* tool function, consecutive bitsc each bit is set to 0. The mask configuration function is enabled. */
static inline void dpa_pm_bits_disable_mask16(void __iomem *ioaddr, unsigned int bitfiled, unsigned int mask)
{
    unsigned int val = readl(ioaddr);
    val &= ~bitfiled;
    val |= mask;
    writel(val, ioaddr);
}



#endif
