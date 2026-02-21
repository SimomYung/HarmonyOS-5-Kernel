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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/kernel.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include <bsp_pfa.h>
#include <bsp_dpa.h>
#include <bsp_diag.h>
#include "debugfs.h"

dpa_debugfs_ctrl_s g_dpa_debugfs_ctrl = { 0 };

void dpa_dfs_show(void)
{
    dpa_debugfs_ctrl_s *ctrl = &g_dpa_debugfs_ctrl;

    bsp_err("DPA DVFS Settings \n");
    bsp_err("dfs_enable %u \n", ctrl->dfs_enable);
    bsp_err("dfs_time_interval : %d \n", ctrl->dfs_time_interval);

    bsp_err("Debug infos \n");
    bsp_err("cur_dfs_grade:%d \n", ctrl->cur_dfs_grade);
    bsp_err("ctrl->last_dfs_grade:%d \n", ctrl->last_dfs_grade);

    bsp_err("pfa_td_count:%u \n", ctrl->pfa_td_count);
    bsp_err("pfa_td_count_last:%u \n", ctrl->pfa_td_count_last);
    bsp_err("pfa_rd_count:%u \n", ctrl->pfa_rd_count);
    bsp_err("pfa_rd_count_last:%u \n", ctrl->pfa_rd_count_last);
    bsp_err("pfa_total_byte:%u \n", ctrl->pfa_total_byte);

    bsp_err("cur_byte:%u \n", ctrl->cur_byte);
    bsp_err("gmac_byte:%u \n", ctrl->gmac_byte);
    bsp_err("xgmac_byte:%u \n", ctrl->xgmac_byte);
    bsp_err("usb_byte:%u \n", ctrl->usb_byte);
    bsp_err("pfa_byte:%u \n", ctrl->pfa_byte);
    bsp_err("dfs_cnt:%d \n", ctrl->dfs_cnt);
    bsp_err("dfs_timer_cnt:%d \n", ctrl->dfs_timer_cnt);
    bsp_err("last_dfs_timestamp:%d \n", ctrl->last_dfs_timestamp);
}

dpa_debugfs_ops g_debugfs_ops[DPA_DEBUGFS_TYPE_MAX];
int bsp_dpa_debugfs_register(dpa_debugfs_type_e type, dpa_debugfs_ops ops)
{
    if (type >= DPA_DEBUGFS_TYPE_MAX || ops == NULL) {
        return -1;
    }

    g_debugfs_ops[type] = ops;

    return 0;
}

inline dpa_debugfs_ctrl_s* dpa_dfs_get_ctrl(void)
{
    return &g_dpa_debugfs_ctrl;
}

int dpa_dfs_enable(unsigned int enable)
{
    dpa_debugfs_ctrl_s *ctrl = &g_dpa_debugfs_ctrl;

    ctrl->dfs_enable = enable;

    return enable;
}

void dpa_set_clk_grade(dpa_debugfs_ctrl_s *ctrl, unsigned int cur_dfs_grade, unsigned int last_dfs_grade)
{
    peri_volt_trend_e volt_trend = PERI_VOLT_NO_CHANGE;
    int dpa_is_access;

    // 配置dpa频率及电压
    if (cur_dfs_grade > last_dfs_grade) {// 需要800M频率时，需要把电压升到0.85v
        if (cur_dfs_grade == DPA_CLK_GRADE_800M) {
            volt_trend = PERI_VOLT_UP;
        }
    } else if (cur_dfs_grade < last_dfs_grade){
        if (last_dfs_grade == DPA_CLK_GRADE_800M) {
            volt_trend = PERI_VOLT_DOWN;   // 降压降频
        }
    }

    dpa_is_access = bsp_dpapm_get_dpa_access_state(DPA_REGS_ACCESS_TSP_DSS0);
    if (dpa_is_access) {
        return;
    }

    bsp_dpa_pm_cfg_clk_grade(volt_trend, cur_dfs_grade);

    ctrl->last_dfs_timestamp = bsp_get_slice_value();
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa_set_clk_grade time:0x%x, cur_dfs_grade=%u\n", ctrl->last_dfs_timestamp, cur_dfs_grade);
}

#define DPA_PKT_LEN 1500
void dpa_get_pfa_total_byte(dpa_debugfs_ctrl_s *ctrl)
{
    unsigned long long td_bytes = 0;
    unsigned long long rd_bytes = 0;

    bsp_pfa_get_dpa_cnt(&ctrl->pfa_td_count, &ctrl->pfa_rd_count);

    if (ctrl->pfa_td_count >= ctrl->pfa_td_count_last) {
        td_bytes = (ctrl->pfa_td_count - ctrl->pfa_td_count_last) * DPA_PKT_LEN;
    } else {
        td_bytes = (0xffffffff - ctrl->pfa_td_count_last + ctrl->pfa_td_count) * DPA_PKT_LEN;
    }

    if (ctrl->pfa_rd_count >= ctrl->pfa_rd_count_last) {
        rd_bytes = (ctrl->pfa_rd_count - ctrl->pfa_rd_count_last) * DPA_PKT_LEN;
    } else {
        rd_bytes = (0xffffffff - ctrl->pfa_rd_count_last + ctrl->pfa_rd_count) * DPA_PKT_LEN;
    }

    if (td_bytes > rd_bytes) {
        ctrl->pfa_total_byte = td_bytes;
    } else {
        ctrl->pfa_total_byte = rd_bytes;
    }

    ctrl->pfa_td_count_last = ctrl->pfa_td_count;
    ctrl->pfa_rd_count_last = ctrl->pfa_rd_count;
}

#define DPA_GRADE_DIVIDING_LINE (4 * 1024 * 1024)
unsigned int dpa_byte_to_grade(dpa_debugfs_ctrl_s *ctrl, unsigned int byte)
{
    if (byte >= DPA_GRADE_DIVIDING_LINE) {
        return DPA_CLK_GRADE_800M;
    } else {
        return DPA_CLK_GRADE_400M;
    }
}

void dpa_dfs_ctrl_work(struct work_struct *work)
{
    dpa_debugfs_ctrl_s *ctrl = &g_dpa_debugfs_ctrl;
    unsigned int type;

    ctrl->dfs_timer_cnt++;

    if (!ctrl->dfs_enable) {
        return;
    }

    for (type = 0; type < DPA_DEBUGFS_TYPE_MAX; type++) {
        if (g_debugfs_ops[type] == NULL) {
            continue;
        }
        switch (type) {
            case DPA_DEBUGFS_GMAC:
                g_debugfs_ops[type](ctrl->dfs_time_interval, &ctrl->gmac_byte);
                break;
            case DPA_DEBUGFS_XGMAC:
                g_debugfs_ops[type](ctrl->dfs_time_interval, &ctrl->xgmac_byte);
                break;
            case DPA_DEBUGFS_USB:
                g_debugfs_ops[type](ctrl->dfs_time_interval, &ctrl->usb_byte);
                break;
            case DPA_DEBUGFS_PFA:
                g_debugfs_ops[type](ctrl->dfs_time_interval, &ctrl->pfa_byte);
                break;
            default:
                break;
        }
    }

    dpa_get_pfa_total_byte(ctrl);
    ctrl->pfa_byte = (ctrl->pfa_total_byte / 1024) * 8 * (1000 / ctrl->dfs_time_interval); /* kbps, 1024,8,1000 unit trans */
    ctrl->cur_byte = max3(ctrl->gmac_byte, ctrl->xgmac_byte, ctrl->pfa_byte);
    ctrl->cur_dfs_grade = dpa_byte_to_grade(ctrl, ctrl->cur_byte);

    if (ctrl->cur_dfs_grade != ctrl->last_dfs_grade) {
        ctrl->dfs_cnt++;
        dpa_set_clk_grade(ctrl, ctrl->cur_dfs_grade, ctrl->last_dfs_grade);  // dpa调频调压
        ctrl->last_dfs_grade = ctrl->cur_dfs_grade;
    }

    queue_delayed_work(ctrl->dfs_workqueue, &ctrl->dfs_work, msecs_to_jiffies(ctrl->dfs_time_interval));
}

int dpa_dfs_delay_work_init(dpa_debugfs_ctrl_s *ctrl)
{
    ctrl->dfs_workqueue = create_singlethread_workqueue("dpa_dfs_work");
    if (ctrl->dfs_workqueue == NULL) {
        bsp_err("create dfs work fail\n");
        return -1;
    }
    INIT_DELAYED_WORK(&ctrl->dfs_work, dpa_dfs_ctrl_work);
    queue_delayed_work(ctrl->dfs_workqueue, &ctrl->dfs_work, ctrl->dfs_time_interval);

    return 0;
}

void dpa_dfs_init(void)
{
    dpa_debugfs_ctrl_s *ctrl = &g_dpa_debugfs_ctrl;

    if (!ctrl->dfs_timer_init_flag) {
        dpa_dfs_delay_work_init(ctrl);
        ctrl->dfs_timer_init_flag = 1;
    }

    ctrl->cur_dfs_grade = DPA_CLK_GRADE_400M;
    ctrl->last_dfs_grade = DPA_CLK_GRADE_400M;
    ctrl->dfs_time_interval = 300;
    ctrl->dfs_timer_cnt = 0;
    ctrl->dfs_cnt = 0;
    ctrl->pfa_td_count = 0;
    ctrl->pfa_td_count_last = 0;
    ctrl->pfa_rd_count = 0;
    ctrl->pfa_rd_count_last = 0;
    ctrl->cur_byte = 0;
    ctrl->pfa_byte = 0;
    ctrl->gmac_byte = 0;
    ctrl->xgmac_byte = 0;
    ctrl->pfa_total_byte = 0;

    return;
}

static int dpa_debugfs_init(void)
{
    int ret;
    dpa_debugfs_ctrl_s *ctrl = &g_dpa_debugfs_ctrl;

    bsp_err("dpa_debugfs_init\n");

    ret = bsp_dpa_unreset_hook_register(DPA_UNRESET_HOOK_DFS, dpa_dfs_init);
    if (ret) {
        bsp_err("dpa_debugfs_init unrst register fail. \n");
        return ret;
    }

    ctrl->dfs_enable = 1;

    return 0;
}

module_init(dpa_debugfs_init);