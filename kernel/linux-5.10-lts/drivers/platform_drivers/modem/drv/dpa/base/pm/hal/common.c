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

#include <linux/slab.h>
#include <bsp_dump.h>
#include <bsp_slice.h>
#include <bsp_dpa.h>
#include <bsp_pmctrl.h>
#include <bsp_diag.h>
#include "common.h"
#include "pm.h"


dpa_poweron_hook_ops g_poweron_hook_ops[DPA_POWERON_HOOK_TYPE_MAX];

int bsp_dpa_powerup_hook_register(powerup_type_e type, dpa_poweron_hook_ops ops)
{
    if (type >= DPA_POWERON_HOOK_TYPE_MAX) {
        return DPA_PM_ERR;
    }
    if (ops == NULL) {
        return DPA_PM_ERR;
    }
    g_poweron_hook_ops[type] = ops;

    return DPA_PM_OK;
}

int bsp_dpa_powerup_hook_unregister(powerup_type_e type)
{
    if (type >= DPA_POWERON_HOOK_TYPE_MAX) {
        return DPA_PM_ERR;
    }

    g_poweron_hook_ops[type] = NULL;

    return DPA_PM_OK;
}

static inline void dpa_pm_cfg_refclk_sel(dpa_pm_ctrl_s *ctrl, dpa_refclk_e refclk)
{
    unsigned int cfg;

    // power up, dpa ref clock: 800M; poer down, dpa refclk: 38.4M
    cfg = (refclk == DPA_REF_CLOCK_800M) ? 0xF000200 : 0xF000100;
    writel(cfg, ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_CLK_SEL1);
}

int dpa_pm_powerdown_dss(void *ptr, unsigned int dss_id)
{
    unsigned int value, mask;
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)ptr;

    // step1-dss wait idle
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + SC_STAT0, value, (value & DSS0_BUSY_WAIT_MODE_MASK) == DSS0_WAIT_MODE, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_100US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_STAT0, DSS0_WAIT_MODE, value);
        return -ETIMEDOUT;
    }

    // step2-unlock mpscu
    writel(0x42524B4E, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_LCK);

    // step3-module clk disable
    value = readl(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    mask = DSS0_CN_INTERFACE_EN | DSS0_EX2L1M_INTERFACE_EN;
    value |= mask;
    writel(value, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS, value, (value & mask) == mask, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_5US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", MPSCU_EX_IF_DIS, mask, value);
        return -ETIMEDOUT;
    }

    dpa_pm_bits_enable(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS, DSS0_CLOCK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS, value, (value & DSS0_CLOCK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_5US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", MPSCU_CLK_DIS, (unsigned int)DSS0_CLOCK, value);
        return -ETIMEDOUT;
    }

    // step4-memory config: retention memory, dss_l1tcm_enter_lp
    value = readl(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL);
    value |= 1 | DPA_RET_MEM_CTRL_POFF0_SD_MASK | DPA_RET_MEM_CTRL_POFF0_DS_MASK;
    writel(value, ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL);
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_STAT, value, (value & DPA_RET_MEM_CTRL_POFF_OUT0_MASK) == 1, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_10US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_POFF_STAT, 1, value);
        return -ETIMEDOUT;
    }
    udelay(1);

    return DPA_PM_OK;
}

int dpa_pm_powerdown_subsys(void *ptr)
{
    unsigned int value;
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)ptr;

    // step2-SYSQIC 握手, DPA2SYS POWER IDLE REQ
    dpa_pm_bits_enable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDM_REQ_DPA2SYS, PDC_PWRDM_REQ_DPA2SYS_MASK);
    dpa_pm_bits_enable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDS_REQ_DPA2SYS, PDC_PWRDS_REQ_DPA2SYS_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_STAT20, value,
        (value & PDC_PWRDSM_ACK_DPA2SYS_MASK) == PDC_PWRDSM_ACK_DPA2SYS_MASK, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_20US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_PERI_STAT20, (unsigned int)PDC_PWRDSM_ACK_DPA2SYS_MASK, value);
        return -ETIMEDOUT;
    }

    dpa_pm_bits_enable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDM_REQ_SYS2DPA, PDC_PWRDM_REQ_SYS2DPA_MASK);
    dpa_pm_bits_enable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDS_REQ_SYS2DPA, PDC_PWRDS_REQ_SYS2DPA_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_STAT20, value,
        (value & PDC_PWRDSM_ACK_SYS2DPA_MASK) == PDC_PWRDSM_ACK_SYS2DPA_MASK, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_20US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_PERI_STAT20, (unsigned int)PDC_PWRDSM_ACK_SYS2DPA_MASK, value);
        return -ETIMEDOUT;
    }
    // step3_0-dpa ref clk, 38.4M
    dpa_pm_cfg_refclk_sel(ctrl, DPA_REF_CLOCK_38P4M);

    // step3-module clk disable
    dpa_pm_bits_enable(ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_CLKDIS2, DPA_TOP_PD_CLK_DIS);

    // step4_0-配置non retention memory进入低功耗模式
    dpa_pm_bits_enable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF0_MASK, DPA_MEM_CTRL_POFF0_SD_MASK | DPA_MEM_CTRL_POFF0_DS_MASK);
    dpa_pm_bits_enable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF1_MASK, DPA_MEM_CTRL_POFF1_SD_MASK | DPA_MEM_CTRL_POFF1_DS_MASK);


    // step4-module iso enable
    dpa_pm_bits_enable(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_AO_NOSEC_ISO_EN, DPA_ISO_EN);

    // step5-module reset
    writel(DPA_PD_SRST_EN, ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_SRSTEN2);

    // step6-module power down
    dpa_pm_bits_enable(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_MTCMOS_DIS, DPA_MTCMOS_DIS);

    return DPA_PM_OK;
}

void dpa_dss_unreset(unsigned int dss_id)
{
    unsigned int reg_val;
    dpa_pm_ctrl_s *ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        dpa_pm_fatal("ctrl is null \n");
        return;
    }

    reg_val = readl(ctrl->reg_info[DPA_TSP_BASE].virt_addr + SCU_HW_RESET);
    reg_val |= CORE_RESET | NORMAL_RESET;
    writel(reg_val, ctrl->reg_info[DPA_TSP_BASE].virt_addr + SCU_HW_RESET);
}

static inline void dpa_pm_config_scu_init_addr(dpa_pm_ctrl_s *ctrl)
{
    unsigned int thread_id;

    for (thread_id = 0; thread_id < DSS_THREAD_MAX_NUM; thread_id++) {
        if ((ctrl->core_mask & BIT(thread_id)) == 0) {
            continue;
        }

        writel(ctrl->scu_init_addr, ctrl->reg_info[DPA_TSP_BASE].virt_addr + DSS0_SCU_INIT_ADDR_THREAD(thread_id));
    }
}

// 只需要配置dss0_t0_init_icu_sbase、dss0_t0_init_addr即可
static void dpa_pm_config_tsp_init_reg(dpa_pm_ctrl_s *ctrl)
{
    unsigned int thread_id;

    for (thread_id = 0; thread_id < DSS_THREAD_MAX_NUM; thread_id++) {
        if ((ctrl->core_mask & BIT(thread_id)) == 0) {
            continue;
        }
        // dssX tX init icu sbase
        writel(ctrl->scu_init_addr, ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + DSS0_THREAD_INIT_ICU_SBASE(thread_id));
        // dssX tX init addr
        writel(ctrl->scu_init_addr, ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + DSS0_THREAD_INIT_ADDR(thread_id));
    }
}

static int dpa_pm_powerup_dss(void *ptr, unsigned int dss_id)
{
    unsigned int reg_val, value;
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)ptr;

    // 手册上见【dpa_ram_poff_cfg】
    dpa_pm_config_tsp_init_reg(ctrl);

    // STEP0-disable dss external interface;eg:dss0:bit0&bit12;dss1:bit1&bit13;
    reg_val = readl(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    reg_val |= DSS0_CN_INTERFACE_EN | DSS0_EX2L1M_INTERFACE_EN;
    writel(reg_val, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);

    // STEP1- mpscu_hw_reset
    writel(DSS_RESET | DSS_NORMAL_RESET | RESET_ABORT_CNT, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_HW_RESET_DSS(dss_id));
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_MP_STATUS, value, (value & LAST_DSSX_RESET_DONE), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_10US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", MPSCU_MP_STATUS, (unsigned int)LAST_DSSX_RESET_DONE, value);
        return -ETIMEDOUT;
    }

    // step2_0-memory config: retention memory, dss_l1tcm_exit_lp
    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_RET_MEM_CTRL_POFF0_SD, DPA_RET_MEM_CTRL_POFF0_SD_MASK);
    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_RET_MEM_CTRL_POFF0_DS, DPA_RET_MEM_CTRL_POFF0_DS_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_STAT, value, !(value & DPA_RET_MEM_CTRL_POFF_OUT0_MASK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_10US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_POFF_STAT, !DPA_RET_MEM_CTRL_POFF_OUT0_MASK, value);
        return -ETIMEDOUT;
    }
    udelay(1);
    // STEP2-module clk enable;
    dpa_pm_bits_disable(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS, DSS0_CLOCK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS, value, !(value & DSS0_CLOCK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_5US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", MPSCU_CLK_DIS, !DSS0_CLOCK, value);
        return -ETIMEDOUT;
    }

    reg_val = readl(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    reg_val &= ~(DSS0_CN_INTERFACE_EN | DSS0_EX2L1M_INTERFACE_EN);
    writel(reg_val, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS, value, !(value & (DSS0_CN_INTERFACE_EN | DSS0_EX2L1M_INTERFACE_EN)), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_5US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", MPSCU_EX_IF_DIS, 0, value);
        return -ETIMEDOUT;
    }

    // STEP3-init addr config
    dpa_pm_config_scu_init_addr(ctrl);

    return DPA_PM_OK;
}

static int dpa_pm_powerup_tsp(dpa_pm_ctrl_s *ctrl)
{
    unsigned int value;

    // step9-slave boot config
    dpa_pm_bits_disable(ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + SC_CTRL73, DSS0_T0_INIT_SELFBOOT);

    // step10-tsp mp unrst
    dpa_pm_bits_enable(ctrl->reg_info[DPA_CRG_BASE].virt_addr + SRSTDIS0, TSP_CORE_SRST_DIS);

    // step11-dss clk disable
    dpa_pm_bits_enable(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS, DSS0_CLOCK);

    // step12-DLP enable
    writel(0x1F, ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_DLP);
    if (readl_poll_timeout_atomic(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_DLP, value, (value == 0x1F), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_5US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_MTCMOS_STAT, (unsigned int)SC_DPA_MTCMOS_RDY, value);
        return -ETIMEDOUT;
    }

    return DPA_PM_OK;
}

void dpa_pm_cfg_clk_grade(dpa_pm_ctrl_s *ctrl, peri_volt_trend_e peri_volt_trend, dpa_clk_grade_e clk_grade)
{
    int ret;
    unsigned int value = 0;

    struct dpa_profile profile[DPA_CLK_GRADE_BOTTOM] = {
        { .clk_grade = DPA_CLK_GRADE_200M, .volt = PERI_VOLT_0P65V, .profile_id = PROFILE_ID_0, .div = 3 },
        { .clk_grade = DPA_CLK_GRADE_400M, .volt = PERI_VOLT_0P65V, .profile_id = PROFILE_ID_0, .div = 1 },
        { .clk_grade = DPA_CLK_GRADE_800M, .volt = PERI_VOLT_0P85V, .profile_id = PROFILE_ID_2, .div = 0 },
    };

    // 根据回片要求，回片按高频高压起，投电压到0.85v；回片后在恢复为常压，投电压到0.65v；
    if (peri_volt_trend == PERI_VOLT_UP) {
        ret = bsp_pmctrl_set_profile_sync(ctrl->div_pmctrl, profile[clk_grade].profile_id);
        if (ret != 0) {
            dpa_pm_fatal("dpa pm set peri volt fail, ret = 0x%x \n", ret);
        }
    }

    // dpa ref clock:800M,divnum:1, dpa tsp work at 800MHZ frequency
    value = profile[clk_grade].div | DPA_CLK_DIVNUM_MASK_SHIFT16;
    writel(value, ctrl->reg_info[DPA_CRG_BASE].virt_addr + CLK_CFG1);


    if (peri_volt_trend == PERI_VOLT_DOWN) {
        ret = bsp_pmctrl_set_profile_sync(ctrl->div_pmctrl, profile[clk_grade].profile_id);
        if (ret != 0) {
            bsp_err("dpa set peri volt fail, ret = 0x%x \n", ret);
        }
    }
}

static int dpa_pm_powerup_sub(dpa_pm_ctrl_s *ctrl)
{
    unsigned int value;

    // step1-module power on
    dpa_pm_bits_enable(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_MTCMOS_EN, DPA_MTCMOS_EN);
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_MTCMOS_STAT, value, value & SC_DPA_MTCMOS_RDY, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_30US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_MTCMOS_STAT, (unsigned int)SC_DPA_MTCMOS_RDY, value);
        return -ETIMEDOUT;
    }

    // step2-module iso disable
    dpa_pm_bits_enable(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_AO_NOSEC_ISO_DIS, DPA_ISO_DIS);

    // step3-memory rempair
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_BISR_STAT0, value, value & BISR_REPAIR_ACK, DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_400US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_BISR_STAT0, (unsigned int)BISR_REPAIR_ACK, value);
        return -ETIMEDOUT;
    }

    // step4-memory config:non retention memory
    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF0_SD, DPA_MEM_CTRL_POFF0_SD_MASK);
    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF0_DS, DPA_MEM_CTRL_POFF0_DS_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_STAT, value, !(value & DPA_MEM_CTRL_POFF_OUT0_MASK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_10US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_POFF_STAT, !DPA_MEM_CTRL_POFF_OUT0_MASK, value);
        return -ETIMEDOUT;
    }

    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF1_SD, DPA_MEM_CTRL_POFF1_SD_MASK);
    dpa_pm_bits_disable_mask16(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_CTRL, DPA_MEM_CTRL_POFF1_DS, DPA_MEM_CTRL_POFF1_DS_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[AO_SYSCTRL_BASE].virt_addr + SC_DPA_POFF_STAT, value, !(value & DPA_MEM_CTRL_POFF_OUT1_MASK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_10US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_DPA_POFF_STAT, !DPA_MEM_CTRL_POFF_OUT1_MASK, value);
        return -ETIMEDOUT;
    }
    udelay(1);

    // step4_1-module clk enable
    dpa_pm_bits_enable(ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_CLKEN2, DPA_TOP_PD_CLK_ENABLE);

    // step5-module unrst
    writel(DPA_PD_SRST_DIS, ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_SRSTDIS2);


    // step6-SYSQIC POWER IDLE DISABLE
    dpa_pm_bits_disable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDM_REQ_DPA2SYS, PDC_PWRDM_REQ_DPA2SYS_MASK);
    dpa_pm_bits_disable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDS_REQ_DPA2SYS, PDC_PWRDS_REQ_DPA2SYS_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_STAT20, value, !(value & PDC_PWRDSM_ACK_DPA2SYS_MASK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_20US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_PERI_STAT20, !PDC_PWRDSM_ACK_DPA2SYS_MASK, value);
        return -ETIMEDOUT;
    }

    dpa_pm_bits_disable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDM_REQ_SYS2DPA, PDC_PWRDM_REQ_SYS2DPA_MASK);
    dpa_pm_bits_disable_mask16(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_CTRL0, PDC_PWRDS_REQ_SYS2DPA, PDC_PWRDS_REQ_SYS2DPA_MASK);
    if (readl_poll_timeout_atomic(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_STAT20, value, !(value & PDC_PWRDSM_ACK_SYS2DPA_MASK), DPA_PM_DELAY_TIME, DPA_PM_WAIT_TIME_20US)) {
        dpa_pm_fatal("time out, addr = 0x%x, expecte_val = 0x%x, reg_val = 0x%x!\n", SC_PERI_STAT20, !PDC_PWRDSM_ACK_SYS2DPA_MASK, value);
        return -ETIMEDOUT;
    }

    // step8-configure clk_sel/clkdiv according to profile
    dpa_pm_cfg_clk_grade(ctrl, PERI_VOLT_DOWN, DPA_CLK_GRADE_400M);

    // step9_0-configure clk_sel/clkdiv according to profile
    dpa_pm_cfg_refclk_sel(ctrl, DPA_REF_CLOCK_800M);

    return DPA_PM_OK;
}


static int dpa_pm_powerup_subsys(void *ptr)
{
    int ret, i;
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)ptr;

    ret = dpa_pm_powerup_sub(ctrl);
    if (ret) {
        dpa_pm_fatal("dpa_pm_powerup_sub fail \n");
        return ret;
    }

    ret = dpa_pm_powerup_tsp(ctrl);
    if (ret) {
        dpa_pm_fatal("dpa_pm_powerup_tsp fail \n");
        return ret;
    }

    for (i = 0; i < DPA_POWERON_HOOK_TYPE_MAX; i++) {
        if (g_poweron_hook_ops[i] == NULL) {
            continue;
        }
        // 如果执行失败，注册模块内部做维测计数
        g_poweron_hook_ops[i]();
    }

    return DPA_PM_OK;
}

static void dpa_auto_dfs(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int value;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    /* 配置自动降频分频参数；要是你配的时候看不到是哪档电压  就统一把dfs_divnum配成7  这样最低50M */
    value = 0x700 | DPA_DFS_DIV_NUM_MASK_SHIFT16;
    writel(value, ctrl->reg_info[DPA_CRG_BASE].virt_addr + CLK_CFG1);

    /* 配置自动降频等待时间, 进入自动降频的等待时间，等待高频时钟的N*16个时钟周期，默认256 */
    value = 0xFF | DPA_DFS_WAIT_CYCLE_MASK_SHIFT16;
    writel(value, ctrl->reg_info[DPA_CRG_BASE].virt_addr + CLK_CFG0);

    /* 配置自动降频使能 */
    dpa_pm_bits_enable_mask16(ctrl->reg_info[DPA_CRG_BASE].virt_addr + CLK_CFG1, DPA_DFS_EN_MASK, DPA_DFS_EN_MASK_SHIFT16);
    return;
}

void dpa_enable_wakeup_irq(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int value;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    /* bit0:usb bit1:gmac bit2:xgmac */
    value = readl(ctrl->reg_info[PERI_FIX0_SYSCTRL_BASE].virt_addr + SC_INT_MASK_DPA);
    value |= 7;
    writel(value, ctrl->reg_info[PERI_FIX0_SYSCTRL_BASE].virt_addr + SC_INT_MASK_DPA);

    /* bit0:sne bit1:ipcmsg_int[5] */
    value = readl(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + DPA_WAKEUP_INT_PERI_MASK);
    value |= 3;
    writel(value, ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + DPA_WAKEUP_INT_PERI_MASK);

    return;
}

void dpa_disable_wakeup_irq(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int value;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    /* bit0:usb bit1:gmac bit2:xgmac */
    value = readl(ctrl->reg_info[PERI_FIX0_SYSCTRL_BASE].virt_addr + SC_INT_MASK_DPA);
    value &= ~(7);
    writel(value, ctrl->reg_info[PERI_FIX0_SYSCTRL_BASE].virt_addr + SC_INT_MASK_DPA);

    /* bit0:sne bit1:ipcmsg_int[5] */
    value = readl(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + DPA_WAKEUP_INT_PERI_MASK);
    value &= ~(3);
    writel(value, ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + DPA_WAKEUP_INT_PERI_MASK);

    return;
}

int get_dpa_access_state(dpa_regs_access_state_e dpa_regs_region)
{
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)bsp_dpa_pm_get_ctrl();
    unsigned value;
    if (ctrl == NULL) {
        return BSP_ERROR;
    }

    // 1-module clk enable status;DPA PD总时钟状态,0：时钟关闭;1：时钟开启
    value = readl_relaxed(ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_CLKCG_STAT2);
    if ((value & DPA_TOP_PD_CLKCG_STAT) == 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "PD_CRG_CLKCG_STAT2 = 0x%x, value = 0x%x \n", PD_CRG_CLKCG_STAT2, value);
        return BSP_ERROR;
    }

    // 2-module unrst status; dpa子系统软复位请求状态控制,0：复位请求无效；1：复位请求有效
    value = readl_relaxed(ctrl->reg_info[PERI_CRG_BASE].virt_addr + PD_CRG_SRSTSTAT2);
    if ((value & DPA_PD_SRST_REQ) != 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "PD_CRG_SRSTSTAT2 = 0x%x, value = 0x%x \n", PD_CRG_SRSTSTAT2, value);
        return BSP_ERROR;
    }

    // 3-SYSQIC POWER IDLE DISABLE status
    value = readl_relaxed(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + SC_PERI_STAT20);
    if ((value & (PDC_PWRDSM_ACK_SYS2DPA_MASK | PDC_PWRDSM_ACK_DPA2SYS_MASK)) != 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "SC_PERI_STAT20 = 0x%x, value = 0x%x \n", SC_PERI_STAT20, value);
        return BSP_ERROR;
    }

    if (dpa_regs_region == DPA_REGS_ACCESS_DPA_SYSCTRL_CRG) {
        return BSP_OK;
    }

    // 4-tsp mp unrst status; dpa_mp软复位请求状态。0：复位请求无效；1：复位请求有效
    value = readl_relaxed(ctrl->reg_info[DPA_CRG_BASE].virt_addr + SRSTSTAT0);
    if ((value & DPA_DBG_SRST_REQ) != 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "SRSTSTAT0 = 0x%x, value = 0x%x \n", SRSTSTAT0, value);
        return BSP_ERROR;
    }

    if (dpa_regs_region == DPA_REGS_ACCESS_TSP_MPSS) {
        return BSP_OK;
    }

    // 5-module clk enable;
    // 5.1-tsp dss clock status, dss bit == 0 is enable
    value = readl_relaxed(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_CLK_DIS);
    if ((value & DSS0_CLOCK) != 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "MPSCU_CLK_DIS = 0x%x, value = 0x%x \n", MPSCU_CLK_DIS, value);
        return BSP_ERROR;
    }
    // 5.2-tsp dss ex interface status, dss bit == 0 && dss+12 bit == 0 is enable
    value = readl_relaxed(ctrl->reg_info[DPA_TSP_BASE].virt_addr + MPSCU_EX_IF_DIS);
    if ((value & (DSS0_CN_INTERFACE_EN | DSS0_EX2L1M_INTERFACE_EN)) != 0) {
        bsp_diag_drv_log_report(BSP_P_ERR, "MPSCU_EX_IF_DIS = 0x%x, value = 0x%x \n", MPSCU_EX_IF_DIS, value);
        return BSP_ERROR;
    }

    if (dpa_regs_region == DPA_REGS_ACCESS_TSP_DSS0) {
        return BSP_OK;
    }

    return BSP_ERROR;
}

void set_ddr_addr_info(unsigned int addr, unsigned int size, int addr_reg, int size_reg)
{
    dpa_pm_ctrl_s *ctrl = NULL;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    writel(addr, ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + addr_reg);
    writel(size, ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + size_reg);
}

unsigned int g_dpa_sysctrl_reg_bak[][STASH_REG_BUTTON] = {
    /* STASH_REG_ADDR  STASH_REG_VALUE */
    { SC_INT_CTRL0, 0 },
    { SC_SOFT_RSV_CTRL0, 0 },
    { SC_SOFT_RSV_CTRL1, 0 },
    { SC_SOFT_RSV_CTRL2, 0 },
    { SC_SOFT_RSV_CTRL6, 0 },
    { SC_SOFT_RSV_CTRL7, 0 },
};

void rpm_bak_dpa_sysctrl_reg(void)
{
    unsigned int i;
    dpa_pm_ctrl_s *ctrl = NULL;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    for (i = 0; i < sizeof(g_dpa_sysctrl_reg_bak) / sizeof(g_dpa_sysctrl_reg_bak[0]); i++) {
        g_dpa_sysctrl_reg_bak[i][STASH_REG_VALUE] = readl(ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + g_dpa_sysctrl_reg_bak[i][STASH_REG_ADDR]);
    }
}

void rpm_resume_dpa_sysctrl_reg(void)
{
    unsigned int i;
    dpa_pm_ctrl_s *ctrl = NULL;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    for (i = 0; i < sizeof(g_dpa_sysctrl_reg_bak) / sizeof(g_dpa_sysctrl_reg_bak[0]); i++) {
        writel(g_dpa_sysctrl_reg_bak[i][STASH_REG_VALUE], ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + g_dpa_sysctrl_reg_bak[i][STASH_REG_ADDR]);
    }

}

void record_dpa_wakeup_src(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    dpa_pm_mntn_s *mntn = NULL;
    unsigned int status, i;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    mntn = dpa_pm_get_mntn();
    if (mntn == NULL) {
        return;
    }

    // 记录唤醒源
    /* bit0:usb bit1:gmac bit2:xgmac */
    status = readl(ctrl->reg_info[PERI_FIX0_SYSCTRL_BASE].virt_addr + SC_INT_STAT_DPA_MASK);
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa wakeup src peri fix status = 0x%x \n", status);
    for (i = 0; i < 3; i++) {
        if(status & BIT(i)) {
            mntn->wakeup_cnt[DPA_WAKEUP_SRC_USB + i]++;
            mntn->cur_wake_src = DPA_WAKEUP_SRC_USB + i;
        }
    }

    /* bit0:sne bit1:ipcmsg_int[5] */
    status = readl(ctrl->reg_info[PERI_SYSCTRL_BASE].virt_addr + DPA_WAKEUP_INT_PERI_MASKED);
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa wakeup src peri status = 0x%x \n", status);
    for (i = 0; i < 2; i++) {
        if(status & BIT(i)) {
            mntn->wakeup_cnt[DPA_WAKEUP_SRC_SNE + i]++;
            mntn->cur_wake_src = DPA_WAKEUP_SRC_SNE + i;
        }
    }
}
// DPA唤醒ACore中断使能，当前不存在DPA唤醒ACore场景
void enable_dpa_wake_acore_irq(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    /* bit0:dpa_eicc_acpu_intr_ipipe, bit1:dpa_eicc_acpu_intr_opipe, bit2:dpa_wdt0_int, bit3:dpa_wdt1_int, bit4:dpa_bus_err */
    dpa_pm_bits_enable(ctrl->reg_info[DPA_SYSCTRL_BASE].virt_addr + SC_INT_CTRL1, GENMASK(4, 0));
}

void dpa_pm_cb_ops_register(dpa_pm_ctrl_s *ctrl)
{
    dpa_pm_cb_ops_s *cb_ops = &ctrl->cb_ops;

    cb_ops->auto_dfs = dpa_auto_dfs;
    cb_ops->pm_powerup_subsys = dpa_pm_powerup_subsys;
    cb_ops->pm_powerdown_subsys = dpa_pm_powerdown_subsys;
    cb_ops->pm_powerup_dss = dpa_pm_powerup_dss;
    cb_ops->pm_powerdown_dss = dpa_pm_powerdown_dss;
    cb_ops->dss_unreset = dpa_dss_unreset;
    cb_ops->enable_wakeup_irq = dpa_enable_wakeup_irq;
    cb_ops->disable_wakeup_irq = dpa_disable_wakeup_irq;
    cb_ops->get_dpa_access_state = get_dpa_access_state;
    cb_ops->set_ddr_addr_info = set_ddr_addr_info;
    cb_ops->rpm_bak_dpa_sysctrl_reg = rpm_bak_dpa_sysctrl_reg;
    cb_ops->rpm_resume_dpa_sysctrl_reg = rpm_resume_dpa_sysctrl_reg;
    cb_ops->record_dpa_wakeup_src = record_dpa_wakeup_src;
    cb_ops->enable_dpa_wake_acore_irq = enable_dpa_wake_acore_irq;
}

void bsp_dpa_pm_cfg_clk_grade(peri_volt_trend_e peri_volt_trend, dpa_clk_grade_e clk_grade)
{
    dpa_pm_ctrl_s *ctrl = NULL;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    dpa_pm_cfg_clk_grade(ctrl, peri_volt_trend, clk_grade);
}