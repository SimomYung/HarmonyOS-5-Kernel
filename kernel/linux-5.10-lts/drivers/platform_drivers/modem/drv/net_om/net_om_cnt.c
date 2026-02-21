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

#include "net_om.h"

#undef THIS_MODU
#define THIS_MODU mod_netom
#define NET_OM_TIMEOUT 200

unsigned int g_a_hidslog_freq = 200; /* ms unit */
struct net_trans_s g_net_trans;
#define WAN_SET_TRANS_TIMEOUT() (jiffies + msecs_to_jiffies(g_a_hidslog_freq))

void net_report_init(void);

void bsp_set_netlog_freq(unsigned int timeout)
{
    g_a_hidslog_freq = timeout;
}

int bsp_net_report_register(struct net_om_info *info)
{
    net_report_init();
    if (info == NULL || info->data == NULL || info->size == 0 || info->func_cb == NULL) {
        return -EINVAL;
    }
    list_add_tail(&info->list, &g_net_trans.list);
    return 0;
}

void trans_report_cycle(struct timer_list *t)
{
    struct net_trans_s *net_trans = from_timer(net_trans, t, trans_timer);
    struct net_om_info *info = NULL;
    struct net_om_info *tmp = NULL;
    int ret;

    list_for_each_entry_safe(info, tmp, &net_trans->list, list) {
        if (info->func_cb != NULL) {
            info->func_cb();

            ret = bsp_diag_trans_report(info->ul_msg_id, DIAG_DRV_HDS_PID, info->data, info->size);
            if (ret != BSP_OK) {
                net_trans->transreport_error++;
            } else {
                net_trans->get_dbg_info_error++;
            }
        }
    }

    mod_timer(&net_trans->trans_timer, WAN_SET_TRANS_TIMEOUT());
}

void net_report_init(void)
{
    /* init list */
    struct net_trans_s *net_trans = &g_net_trans;

    if (net_trans->init != 1) {
        INIT_LIST_HEAD(&net_trans->list);
        net_trans->init = 1;
    }
}

static void net_om_report_disable(void)
{
    struct net_trans_s *net_trans = &g_net_trans;

    mutex_lock(&net_trans->timer_mutex);
    if (net_trans->report_onoff != 0) {
        (void)del_timer_sync(&net_trans->trans_timer);
        net_trans->report_onoff = 0;
    }

    mutex_unlock(&net_trans->timer_mutex);
    bsp_err("net_om report close\n");
    return;
}

static void net_om_report_enable(unsigned int hidslog_freq)
{
    struct net_trans_s *net_trans = &g_net_trans;

    mutex_lock(&net_trans->timer_mutex);
    if (net_trans->report_onoff == 1 ) {
        if (hidslog_freq != 0) {
            g_a_hidslog_freq = hidslog_freq;
        }
        mutex_unlock(&net_trans->timer_mutex);
        return;
    }

    g_a_hidslog_freq = NET_OM_TIMEOUT;
    timer_setup(&net_trans->trans_timer, trans_report_cycle, 0);
    add_timer(&net_trans->trans_timer);

    net_trans->report_onoff = 1;
    mutex_unlock(&net_trans->timer_mutex);
    bsp_err("net_om report open\n");
    return;
}

static void net_report_switch_cfg(unsigned char *diag_drv_frame)
{
    int ret;
    diag_frame_req_s *diag_frame = (diag_frame_req_s *)diag_drv_frame;
    net_om_diag_cmd_s *net_om_diag_cmd = (net_om_diag_cmd_s *)diag_frame->data;

    if (net_om_diag_cmd->net_om_switch_acore == 0) {
        net_om_report_disable();
    } else {
        net_om_report_enable(net_om_diag_cmd->report_log_period_acore);
    }

    ret = net_om_send_cfg_to_dpa(net_om_diag_cmd);
    if (ret != 0) {
        bsp_err("net om send to dpa fail ret: %d", ret);
    }

    return;
}

static void netom_diag_stat_proc(unsigned int state)
{
    net_om_diag_cmd_s net_om_diag_cmd;

    net_om_diag_cmd.report_log_period_dpa = 0;
    if (DIAG_GET_DIAG_TOOL_CONN_STATE(state) == DIAG_STATE_DISCONN) {
        net_om_report_disable();
        net_om_diag_cmd.net_om_switch_dpa = 0;
        net_om_send_cfg_to_dpa(&net_om_diag_cmd);
    }

    return;
}

void netom_report_switch_init(void)
{
    int ret;
    unsigned int cmd_id;

    net_report_init();
    mutex_init(&g_net_trans.timer_mutex);
    cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_NET, DIAG_DRV_MODULE_PRINT, 0); // 0x9FA04200;
    bsp_diag_drv_cfg_fn_register(cmd_id, net_report_switch_cfg);

    ret = (int)mdrv_diag_conn_state_notify_fun_reg(netom_diag_stat_proc);
    if (ret) {
        bsp_err("netom diag_conn_state_notify_fun_reg err, ret = %d\n", ret);
        return;
    }

    ret = net_om_msg_init();
    if (ret != 0) {
        bsp_err("net om msg init ok\n");
        return;
    }
    bsp_err("net om init ok\n");
    return;
}

EXPORT_SYMBOL(netom_report_switch_init);
EXPORT_SYMBOL(g_a_hidslog_freq);
EXPORT_SYMBOL(bsp_set_netlog_freq);
EXPORT_SYMBOL(bsp_net_report_register);
