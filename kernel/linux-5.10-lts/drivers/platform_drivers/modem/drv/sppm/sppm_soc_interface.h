/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef __SPPM_SOC_INTERFACE_H__
#define __SPPM_SOC_INTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// ringbuffer config size and shreshold config. default by size/2.
#define SPPM_RB_SIZE_SHIFT_GRANULARITY 6
#define SPPM_RB_SHRESHOLD_SHIFT_GRANULARITY 7

// sppm ctrl reg magic define.
#define SPPM_CTRL_REG_RESET_BIT 8
#define SPPM_CTRL_REG_CRG_RST_BIT 16
#define SPPM_CTRL_REG_RB_SIZE_ALIGN 4
#define SPPM_CTRL_REG_RB_MODE_BIT 28
#define SPPM_CTRL_EN_DELAY_US 300
#define SPPM_CTRL_REG_SIZE 4
#define SPPM_CPU_ID_WRITE_OFFSET 29

#define SPPM_CFG_IDX_DIFF 0x04
#define SPPM_MAX_REGS_EACH_CHAN 14
#define SPPM_MAX_RCHAN_NUM 32
#define SPPM_RCHAN_BUSY_DELAY_US_NUM 10000
#define SPPM_ECAH_MSG_PKG_SIZE 64
#define SPPM_SMP_BASE_FREQ 800000      // 时钟基准800KHz。1.25us
#define SPPM_CHAN_ID_MAGIC 0x0A000000  // 高4 bit设置为1010,配置28位，最低4位由硬件填充
// interrupt flags.
#define SPPM_INT_RING_BUFF_INT (0x1 << 0)
#define SPPM_INT_CPU_PP_OVFW (0x1 << 1)
#define SPPM_INT_DIR_RD_OVFW (0x1 << 2)
#define SPPM_INT_RING_BUFF_OVFW (0x1 << 3)
#define SPPM_INT_WRESP_ERR (0x1 << 4)
#define SPPM_INT_RRESP_ERR (0x1 << 5)
#define SPPM_INT_REG_NUM_ERR (0x1 << 6)
#define SPPM_INT_CPU_WR_REG_ERR (0x1 << 7)
#define SPPM_INT_ALL 0xff

#define SOC_MODEM_PERI_CRG_CLKEN 0x000
#define SOC_MODEM_PERI_CRG_CLKDIS 0x004

#define SOC_MODEM_PERI_SRST_EN 0x100
#define SOC_MODEM_PERI_SRST_DIS 0x104

#define SPPM_ADDR_BASE_OFFSET 0x000

/* 寄存器说明：SPPM 读通道寄存器配置空间，每个通道支持14个寄存器
   每个通道配置内存偏移 14*4=0x38 */
#define SPPM_CHAN_ADDR_BASE (SPPM_ADDR_BASE_OFFSET + 0x000)

/* 寄存器说明：SPPM 通用寄存器配置区域
 */
#define SPPM_CFG_BASE_OFFSET 0x800

#define SPPM_CFG_SMP_START_EN_BASE (SPPM_CFG_BASE_OFFSET + 0x000)
#define SPPM_CFG_DIRECT_CH_SF_BASE (SPPM_CFG_BASE_OFFSET + 0x008)
#define SPPM_CFG_CPU_CH_ID_BASE (SPPM_CFG_BASE_OFFSET + 0x00C)

#define SPPM_CFG_DIRECT_SMP_INTVAL_BASE (SPPM_CFG_BASE_OFFSET + 0x10)

#define SPPM_CFG_DIRECT_CH_ID_BASE (SPPM_CFG_BASE_OFFSET + 0x90)

#define SPPM_CFG_DIRECT_CH_NUM_BASE (SPPM_CFG_BASE_OFFSET + 0x110)

#define SPPM_CFG_RING_BUF_ADDR_BASE (SPPM_CFG_BASE_OFFSET + 0x190)  // 高32位地址，需要16字节对齐
#define SPPM_CFG_RING_BUF_CFG_BASE (SPPM_CFG_BASE_OFFSET + 0x194)   // 高28位地址，每帧数据64字节
#define SPPM_CFG_RING_BUF_INT_TH_BASE (SPPM_CFG_BASE_OFFSET + 0x198)
#define SPPM_CFG_RING_BUF_RPTR_BASE (SPPM_CFG_BASE_OFFSET + 0x19C)
#define SPPM_CFG_RING_BUF_WPTR_BASE (SPPM_CFG_BASE_OFFSET + 0x1A0)
#define SPPM_CFG_SPPM_INT_ENA_BASE (SPPM_CFG_BASE_OFFSET + 0x1A4)
#define SPPM_CFG_SPPM_INT_CLR_BASE (SPPM_CFG_BASE_OFFSET + 0x1A8)
#define SPPM_CFG_HAC_QSP_USER_BASE (SPPM_CFG_BASE_OFFSET + 0x1B0)
#define SPPM_CFG_HAC_QSP_WCH_MAXLEN_BASE (SPPM_CFG_BASE_OFFSET + 0x1B4)
#define SPPM_CFG_HAC_QSP_WR_OST_EN_BASE (SPPM_CFG_BASE_OFFSET + 0x1B8)
#define SPPM_CFG_HAC_QSP_WR_OST_LMT_BASE (SPPM_CFG_BASE_OFFSET + 0x1BC)
#define SPPM_CFG_HAC_QSP_RD_OST_EN_BASE (SPPM_CFG_BASE_OFFSET + 0x1C0)
#define SPPM_CFG_HAC_QSP_RD_OST_LMT_BASE (SPPM_CFG_BASE_OFFSET + 0x1C4)
#define SPPM_CFG_CPU_WR_EN_BASE (SPPM_CFG_BASE_OFFSET + 0x1C8)
#define SPPM_CFG_CPU_WR_MST_ID_BASE (SPPM_CFG_BASE_OFFSET + 0x1CC)

#define SPPM_CFG_DIRECT_RD_DDR_HIGH_ADDR_BASE (SPPM_CFG_BASE_OFFSET + 0x1d0)

#define SPPM_CFG_SPPM_RAW_INT_STA_BASE (SPPM_CFG_BASE_OFFSET + 0x3A0)
#define SPPM_CFG_SMP_START_TS_BASE (SPPM_CFG_BASE_OFFSET + 0x3AC)
#define SPPM_CFG_CPU_OVFW_TS_BASE (SPPM_CFG_BASE_OFFSET + 0x3B0)
#define SPPM_CFG_DIRECT_OVFW_TS_BASE (SPPM_CFG_BASE_OFFSET + 0x3B4)
#define SPPM_CFG_SPPM_MASK_INT_STA_BASE (SPPM_CFG_BASE_OFFSET + 0x3C0)
#define SPPM_CFG_DIRECT_CH_BUSY_BASE (SPPM_CFG_BASE_OFFSET + 0x3C8)
#define SPPM_CFG_HAC_QSP_DEBUG_BASE (SPPM_CFG_BASE_OFFSET + 0x3D0)
#define SPPM_CFG_SPPM_ALL_IDLE_BASE (SPPM_CFG_BASE_OFFSET + 0x3D4)

#define SPPM_CFG_CLK_GATE_DLY_CNT_BASE (SPPM_CFG_BASE_OFFSET + 0x250)
#define SPPM_CFG_DIRECT_CH_EN_BASE (SPPM_CFG_BASE_OFFSET + 0x254)
#define SPPM_CFG_DIRECT_CH_DIS_BASE (SPPM_CFG_BASE_OFFSET + 0x258)
#define SPPM_CFG_DIRECT_CH_STATE_BASE (SPPM_CFG_BASE_OFFSET + 0x25C)

/* 寄存器说明：SPPM CPU直写缓存寄存器区域
 */
#define SPPM_CPU_WR_CFG_BASE 0xC00
#define SPPM_CPU_WR_TSP_DATA_L_BASE (SPPM_CPU_WR_CFG_BASE + 0x000)
#define SPPM_CPU_WR_TSP_DATA_H_BASE (SPPM_CPU_WR_CFG_BASE + 0x004)
#define SPPM_CPU_WR_ACPU_DATA_L_BASE (SPPM_CPU_WR_CFG_BASE + 0x008)
#define SPPM_CPU_WR_ACPU_DATA_H_BASE (SPPM_CPU_WR_CFG_BASE + 0x00C)
#define SPPM_CPU_WR_CM3_DATA_L_BASE (SPPM_CPU_WR_CFG_BASE + 0x010)
#define SPPM_CPU_WR_CM3_DATA_H_BASE (SPPM_CPU_WR_CFG_BASE + 0x014)
#define SPPM_CPU_WR_DPA_DATA_L_BASE (SPPM_CPU_WR_CFG_BASE + 0x018)
#define SPPM_CPU_WR_DPA_DATA_H_BASE (SPPM_CPU_WR_CFG_BASE + 0x01C)
#define SPPM_CPU_WR_PSA_DATA_L_BASE (SPPM_CPU_WR_CFG_BASE + 0x020)
#define SPPM_CPU_WR_PSA_DATA_H_BASE (SPPM_CPU_WR_CFG_BASE + 0x024)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of sppm_soc_interface.h */
