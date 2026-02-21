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

#include <osl_bio.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include <linux/delay.h>
#include "sppm_soc_interface.h"
#include "mdrv_sppm.h"
#include "sppm_reg_base.h"

#undef THIS_MODU
#define THIS_MODU mod_sppm

static void __iomem *g_sppm_base;
static void __iomem *g_pericrg_base;

// sppm addr after ioremap.
void sppm_base_addr_set(void *sppm_base, void *pericrg_base)
{
    g_sppm_base = (void __iomem *)sppm_base;
    g_pericrg_base = (void __iomem *)pericrg_base;
    return;
}

// sppm crg reset en
int sppm_reset_en(void)
{
    SPPM_WRITEL(g_pericrg_base, 0x01UL << SPPM_CTRL_REG_RESET_BIT, SOC_MODEM_PERI_SRST_EN);
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// sppm crg reset disable
int sppm_reset_dis(void)
{
    SPPM_WRITEL(g_pericrg_base, 0x01UL << SPPM_CTRL_REG_RESET_BIT, SOC_MODEM_PERI_SRST_DIS);
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// sppm crg clk en
int sppm_crg_clk_en(void)
{
    unsigned int value;
    value = 0x01UL << SPPM_CTRL_REG_CRG_RST_BIT;
    SPPM_WRITEL(g_pericrg_base, value, SOC_MODEM_PERI_CRG_CLKEN);
    udelay(SPPM_CTRL_EN_DELAY_US);  // 硬件约束解复位后32us,hybrid运行速度是EMU的5倍
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// sppm crg clk disable
int sppm_crg_clk_dis(void)
{
    unsigned int value;
    value = 0x01UL << SPPM_CTRL_REG_CRG_RST_BIT;
    SPPM_WRITEL(g_pericrg_base, value, SOC_MODEM_PERI_CRG_CLKDIS);
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// SMP_START_EN SPPM使能
int sppm_start_en(void)
{
    SPPM_WRITEL(g_sppm_base, 0x01UL, SPPM_CFG_SMP_START_EN_BASE);
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// SPPM去使能
int sppm_start_dis(void)
{
    SPPM_WRITEL(g_sppm_base, 0x00UL, SPPM_CFG_SMP_START_EN_BASE);
    sppm_info("<%s>\n", __func__);
    return SPPM_OK;
}

// DIRECT_CH_EN
int sppm_rchan_enable(unsigned int chan_idx)
{
    unsigned int value;
    value = (0x01UL << chan_idx);
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_DIRECT_CH_EN_BASE);
    return SPPM_OK;
}

int sppm_rchan_disable(unsigned int chan_idx)
{
    unsigned int value;
    unsigned int idle_state;
    unsigned int delay_cnt = SPPM_RCHAN_BUSY_DELAY_US_NUM;
    value = (0x01UL << chan_idx);
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_DIRECT_CH_DIS_BASE);
    idle_state = sppm_get_rchan_busy_state();
    while ((idle_state & (0x01 << chan_idx)) != 0) {
        udelay(0x01);
        delay_cnt--;
        if (delay_cnt == 0) {
            return SPPM_ERROR;
        }
    }
    return SPPM_OK;
}

// 读通道使能状态获取
unsigned int sppm_get_rchan_en_state(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_DIRECT_CH_STATE_BASE);
    return value;
}

// DIRECT_CH_SF can be configed only once after reset.
int sppm_rchan_set_secunsec(unsigned int secunsec)
{
    SPPM_WRITEL(g_sppm_base, secunsec, SPPM_CFG_DIRECT_CH_SF_BASE);
    return SPPM_OK;
}

// DIRECT_CH_SF can be configed only once after reset.
int sppm_rchan_get_secunsec(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_DIRECT_CH_SF_BASE);
    return value;
}

// CPU_CH_ID cpu write direct chan id.
int sppm_set_wchan_chanid(unsigned int wchan_id)
{
    unsigned int chan_id_tmp = wchan_id;
    chan_id_tmp |= SPPM_CHAN_ID_MAGIC;
    SPPM_WRITEL(g_sppm_base, chan_id_tmp, SPPM_CFG_CPU_CH_ID_BASE);
    return SPPM_OK;
}

int sppm_write_cpu(unsigned int cpudata, unsigned int ts)
{
    SPPM_WRITEL(g_sppm_base, cpudata, SPPM_CPU_WR_ACPU_DATA_L_BASE);
    SPPM_WRITEL(g_sppm_base, ts, SPPM_CPU_WR_ACPU_DATA_H_BASE);
    return SPPM_OK;
}

// DIRECT_SMP_INTVAL 读通道周期配置寄存器
int sppm_set_rchan_period(unsigned int chan_id, unsigned int prd_cnt)
{
    SPPM_WRITEL(g_sppm_base, prd_cnt, SPPM_CFG_DIRECT_SMP_INTVAL_BASE + chan_id * SPPM_CFG_IDX_DIFF);
    return SPPM_OK;
}

// DIRECT_CH_ID 直采通道id配置寄存器
int sppm_set_rchan_chanid(unsigned int chan_id, unsigned int chanid_val)
{
    unsigned int chan_id_tmp = chanid_val;
    chan_id_tmp |= SPPM_CHAN_ID_MAGIC;
    SPPM_WRITEL(g_sppm_base, chan_id_tmp, SPPM_CFG_DIRECT_CH_ID_BASE + chan_id * SPPM_CFG_IDX_DIFF);
    return SPPM_OK;
}

// DIRECT_CH_REG_NUM 直采通道寄存器个数配置寄存器
int sppm_set_rchan_reg_nums(unsigned int chan_id, unsigned int nums)
{
    SPPM_WRITEL(g_sppm_base, nums, SPPM_CFG_DIRECT_CH_NUM_BASE + chan_id * SPPM_CFG_IDX_DIFF);
    return SPPM_OK;
}

// SPPM_CHAN_ADDR_BASE 设置本通道的寄存器
int sppm_set_rchan_reg_addr(unsigned int chan_id, unsigned int reg_idx, unsigned int reg_val)
{
    SPPM_WRITEL(g_sppm_base,
        reg_val,
        SPPM_CHAN_ADDR_BASE + chan_id * SPPM_MAX_REGS_EACH_CHAN * SPPM_CFG_IDX_DIFF + reg_idx * SPPM_CFG_IDX_DIFF);
    return SPPM_OK;
}

// RING_BUF_ADDR 外部ring buf初始地址，16字节对齐，只需要配置高32位
int sppm_set_ringbuff_base_addr(unsigned int msb_addr)
{
    SPPM_WRITEL(g_sppm_base, msb_addr >> SPPM_CTRL_REG_RB_SIZE_ALIGN, SPPM_CFG_RING_BUF_ADDR_BASE);
    return SPPM_OK;
}

// RING_BUF_CFG 外部ring buf配置寄存器。低6位不填，默认64字节对齐
int sppm_set_ringbuff_size(unsigned int msb_size)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_RING_BUF_CFG_BASE);
    value &= 0xFC000000;
    value |= (msb_size);
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_RING_BUF_CFG_BASE);
    return SPPM_OK;
}

// RING_BUF_CFG 循环模式/中断模式配置 bit28, 0:normal 1:循环
int sppm_set_ringbuff_mode(unsigned int mode)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_RING_BUF_CFG_BASE);
    if (mode == 0x01) {
        value |= (mode << SPPM_CTRL_REG_RB_MODE_BIT);
    } else {
        value &= ~(0x01 << SPPM_CTRL_REG_RB_MODE_BIT);
    }

    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_RING_BUF_CFG_BASE);
    return SPPM_OK;
}

// RING_BUF_INT_TH 外部ring buf上报阈值
int sppm_set_ringbuff_int_threshold(unsigned int thrd_size)
{
    SPPM_WRITEL(g_sppm_base, thrd_size, SPPM_CFG_RING_BUF_INT_TH_BASE);
    return SPPM_OK;
}

// RING_BUF_RPTR 输出ringbuffer 读指针
unsigned int sppm_get_rb_rptr(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_RING_BUF_RPTR_BASE);
    return value;
}

// RING_BUF_WPTR 输出ringbuffer 写指针
unsigned int sppm_get_rb_wptr(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_RING_BUF_WPTR_BASE);
    return value;
}

// RING_BUF_RPTR 设置ringbuffer 读指针
unsigned int sppm_set_rb_rptr(unsigned int value)
{
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_RING_BUF_RPTR_BASE);
    return SPPM_OK;
}

// RING_BUF_WPTR 设置ringbuffer 写指针
unsigned int sppm_set_rb_wptr(unsigned int value)
{
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_RING_BUF_WPTR_BASE);
    return SPPM_OK;
}

// CPU_WR_EN CPU直写模式通道使能信号
int sppm_wchan_enable(void)
{
    SPPM_WRITEL(g_sppm_base, 0x01, SPPM_CFG_CPU_WR_EN_BASE);
    return SPPM_OK;
}

// CPU_WR_EN CPU直写模式通道去使能信号
int sppm_wchan_disable(void)
{
    SPPM_WRITEL(g_sppm_base, 0x00, SPPM_CFG_CPU_WR_EN_BASE);
    return SPPM_OK;
}

// CPU_WR_MST_ID 5个CPU直写通道mst id配置寄存器，支持一次配置完成
int sppm_set_wchan_mst_id(unsigned int msg_id)
{
    SPPM_WRITEL(g_sppm_base, msg_id, SPPM_CFG_CPU_WR_MST_ID_BASE);
    return SPPM_OK;
}

// DIRECT_RD_DDR_HIGH_ADDR 直接采样模式DDR空间高4bit地址配置寄存器
int sppm_set_rchan_hibit_addr(unsigned int chan_id, unsigned int msb_addr)
{
    SPPM_WRITEL(g_sppm_base, msb_addr, SPPM_CFG_DIRECT_RD_DDR_HIGH_ADDR_BASE + chan_id * SPPM_CFG_IDX_DIFF);
    return SPPM_OK;
}

// SMP_START_TS 采样开始时刻32K时戳上报
unsigned int sppm_get_32k_slice(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SMP_START_TS_BASE);
    return value;
}

// CPU_OVFW_TS CPU直写模式溢出时刻32K时戳上报
unsigned int sppm_get_32k_ov_slice(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_CPU_OVFW_TS_BASE);
    return value;
}

// SPPM_RAW_INT_STA SPPM屏蔽中断使能控制
unsigned int sppm_set_int_enable(unsigned int bitmask)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SPPM_INT_ENA_BASE);
    value |= bitmask;
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_SPPM_INT_ENA_BASE);
    return SPPM_OK;
}

// SPPM_RAW_INT_STA SPPM中断寄存器去使能控制
unsigned int sppm_set_int_disable(unsigned int bitmask)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SPPM_INT_ENA_BASE);
    value &= ~(bitmask);
    SPPM_WRITEL(g_sppm_base, value, SPPM_CFG_SPPM_INT_ENA_BASE);
    return SPPM_OK;
}

// SPPM_RAW_INT_STA SPPM原始中断状态寄存器
unsigned int sppm_get_raw_int_status(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SPPM_RAW_INT_STA_BASE);
    return value;
}

// SPPM_INT_CLR SPPM中断清除寄存器
unsigned int sppm_clear_int_status(unsigned int clr_data)
{
    SPPM_WRITEL(g_sppm_base, clr_data, SPPM_CFG_SPPM_INT_CLR_BASE);
    return SPPM_OK;
}

// SPPM_MASK_INT_STA SPPM屏蔽后中断状态寄存器
unsigned int sppm_get_int_status_masked(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SPPM_MASK_INT_STA_BASE);
    return value;
}

// DIRECT_CH_BUSY 直接采样模式各通道忙状态上报
unsigned int sppm_get_rchan_busy_state(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_DIRECT_CH_BUSY_BASE);
    return value;
}

// SPPM_ALL_IDLE sppm模块IDLE状态寄存器
unsigned int sppm_get_all_idle_state(void)
{
    unsigned int value;
    value = SPPM_READL(g_sppm_base, SPPM_CFG_SPPM_ALL_IDLE_BASE);
    return value;
}
