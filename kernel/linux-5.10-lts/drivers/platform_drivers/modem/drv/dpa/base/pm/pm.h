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
#ifndef DPA_PM_H
#define DPA_PM_H

#include "bsp_dpa.h"
#include <bsp_version.h>
#include <bsp_print.h>
#include <bsp_dt.h>
#include <bsp_sysctrl.h>
#include <bsp_ipc_fusion.h>

#undef THIS_MODU
#define THIS_MODU mod_dpa

#define DPA_PM_OK 0
#define DPA_PM_ERR (-1)

#define dpa_pm_err(fmt, ...) do { \
    bsp_err("<%s> <%d> " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

#define dpa_pm_fatal(fmt, ...) do { \
    bsp_fatal("<%s> <%d> " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

typedef enum {
    FIRST_INIT,
    DPA_PM_UP,
    DPA_PM_DOWN,
    DPA_PM_RESET,
    DPA_PM_INIT_MAX,
} dpa_pm_init_type_e;

typedef enum {
    DPA_PM_FLOW_UP,
    DPA_PM_FLOW_DOWN,
    DPA_PM_FLOW_MAX,
} dpa_pm_flow_id_e;

typedef enum {
    SLICE_STEP_0,
    SLICE_STEP_1,
    SLICE_STEP_2,
    SLICE_STEP_3,
    SLICE_STEP_4,
    SLICE_STEP_5,
    SLICE_STEP_6,
    SLICE_STEP_7,
    SLICE_STEP_8,
    SLICE_STEP_9,
    SLICE_STEP_MAX,
} slice_step_e;

typedef enum {
    DPA_WAKEUP_SRC_USB,
    DPA_WAKEUP_SRC_GMAC,
    DPA_WAKEUP_SRC_XGMAC,
    DPA_WAKEUP_SRC_SNE,
    DPA_WAKEUP_SRC_IPCMSG,
    DPA_WAKEUP_SRC_MAX,
} wake_src_e;

#define SR_CNT_MAX 10   // 记录最后10次的睡眠请求时间
// 低功耗维测
typedef struct {
    unsigned int wakeup_cnt[DPA_WAKEUP_SRC_MAX];
    unsigned int wakeup_cnt_total;
    unsigned int cur_wake_src;
    unsigned int wakeup_time[SR_CNT_MAX];
    unsigned int sleep_cnt;
    unsigned int sleep_time[SR_CNT_MAX];
    unsigned int sr_flow_slice[DPA_PM_FLOW_MAX][SLICE_STEP_MAX];
} dpa_pm_mntn_s;

typedef struct {
    void (*auto_dfs)(void);
    int (*pm_powerup_subsys)(void *ctrl);
    int (*pm_powerdown_subsys)(void *ctrl);
    int (*pm_powerup_dss)(void *ctrl, unsigned int dss_id);
    int (*pm_powerdown_dss)(void *ctrl, unsigned int dss_id);
    void (*dss_unreset)(unsigned int dss_id);
    void (*enable_wakeup_irq)(void);
    void (*disable_wakeup_irq)(void);
    int (*get_dpa_access_state)(dpa_regs_access_state_e dpa_regs_region);
    void (*set_ddr_addr_info)(unsigned int addr, unsigned int size, int addr_reg, int size_reg);
    void (*rpm_bak_dpa_sysctrl_reg)(void);
    void (*rpm_resume_dpa_sysctrl_reg)(void);
    void (*record_dpa_wakeup_src)(void);
    void (*enable_dpa_wake_acore_irq)(void);
} dpa_pm_cb_ops_s;

typedef struct {
    unsigned int dss_state; /* DSSX的上电状态，每bit对应一个DSS，值0为DOWN，值1为UP */
    version_plat_type_e plat_type;
    version_cses_type_e cses_type;
    version_chip_type_e chip_type;
    dpa_pm_init_type_e init_type;
    struct dpa_reg_info reg_info[REG_BASE_MAX];
    unsigned int scu_init_addr;
    dpa_pm_cb_ops_s cb_ops;
    unsigned int dss_num;
    unsigned int core_mask;
    unsigned int image_itcm_size;
    unsigned int image_dtcm_size;
    void *itcm_bak;
    void *dtcm_bak;
    unsigned int wake_dpa_irq;
    unsigned int wake_acore_irq;
    unsigned int wake_acore_irq_num;
    ipc_handle_t ipc_handle;
    unsigned int div_pmctrl;
    char *dump_base;
    unsigned int dump_size;
} dpa_pm_ctrl_s;


dpa_pm_ctrl_s* dpa_pm_get_ctrl(void);
dpa_pm_mntn_s *dpa_pm_get_mntn(void);
void dpa_pm_record_slice(dpa_pm_flow_id_e flow, slice_step_e step);
int dpa_pm_common_init(dpa_pm_ctrl_s *ctrl, device_node_s *np);
void dpa_pm_cb_ops_register(dpa_pm_ctrl_s *ctrl);
int dpa_pm_startup(dpa_pm_ctrl_s *ctrl, dpa_pm_init_type_e type);
int dpa_pm_powerdown(dpa_pm_ctrl_s *ctrl);
#endif