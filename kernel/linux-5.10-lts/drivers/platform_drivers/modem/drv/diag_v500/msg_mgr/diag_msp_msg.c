/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <securec.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/list.h>
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <mdrv_timer.h>
#include <mdrv_diag.h>
#include <bsp_slice.h>
#include <bsp_version.h>
#include <bsp_odt.h>
#include <bsp_reset.h>
#include <bsp_diag.h>
#include "msg_chan.h"
#include "diag_debug.h"
#include "diag_msg_proc.h"
#include "diag_msp_msg.h"
#include "scm_src.h"
#include "scm_dst.h"
#include "ppm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

diag_drv_msg_info_s g_diag_drv_msg_info;
diag_msp_msg_info_s g_diag_msp_msg_info;
diag_msp_msg_proc_s g_diag_msp_proc_table[] = {
    { DIAG_CMD_LOG_TRANS,       diag_trans_cfg_proc },
    { DIAG_CMD_APPLOG_CONFIG,   diag_applog_cfg_proc },
    { DIAG_CMD_LOG_MNTN,        diag_debug_cfg_proc },
    { DIAG_CMD_FLOW_TEST,       diag_flow_cfg_proc },
};

unsigned int mdrv_diag_conn_state_notify_fun_reg(mdrv_diag_conn_state_notify_fun func)
{
    unsigned int i;

    for (i = 0; i < DIAG_NOTIFY_HOOK_MAX; i++) {
        if (g_diag_msp_msg_info.nottify_fn[i] == NULL) {
            g_diag_msp_msg_info.nottify_fn[i] = func;
            return 0;
        }
    }

    diag_err("diag notifier have no space\n");

    return DIAG_ERRNOR_NO_FREE_SPACE;
}

void diag_conn_state_notify(diag_tool_type_e type, unsigned int state)
{
    unsigned int i;

    for (i = 0; i < DIAG_NOTIFY_HOOK_MAX; i++) {
        if (g_diag_msp_msg_info.nottify_fn[i] != NULL) {
            g_diag_msp_msg_info.nottify_fn[i](state);
        }
    }

    return;
}

unsigned int diag_get_print_level(void)
{
    return g_diag_drv_msg_info.print_level;
}

void diag_set_print_level(unsigned int level)
{
    g_diag_drv_msg_info.print_level = level;
}

unsigned int diag_get_cfg_switch(unsigned int cfg_type)
{
    return (g_diag_msp_msg_info.diag_cfg_state[DIAG_TOOL_TYPE_DIAGTOOL] >> cfg_type) & (unsigned int)1;
}

void diag_set_cfg_switch(int tool_type, int switch_bit, unsigned char state)
{
    if (state == DIAG_CFG_OPEN) {
        g_diag_msp_msg_info.diag_cfg_state[tool_type] |= ((unsigned int)1 << switch_bit);
    } else {
        g_diag_msp_msg_info.diag_cfg_state[tool_type] &= ~((unsigned int)1 << switch_bit);
    }

    return;
}

void diag_reset_cfg_switch(diag_tool_type_e tool_type)
{
    g_diag_msp_msg_info.diag_cfg_state[tool_type] = DIAG_CFG_CLOSE;

}

void diag_open_all_switch(void)
{
    unsigned int i;

    for(i = 0; i < DIAG_CFG_BIT_BUTT; i++) {
        diag_set_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL, i, DIAG_CFG_OPEN);
    }

    return;
}

unsigned int diag_is_cycle_on_open(void)
{
    return g_diag_msp_msg_info.cycle_on;
}

unsigned int diag_is_power_on_log_open(void)
{
    return g_diag_msp_msg_info.power_on_log;
}

void diag_show_conn_state(void)
{
    int i;
    for ( i = 0; i < DIAG_TOOL_TYPE_BUTT; i++) {
        diag_err("tool_type:%d, conn_state:%d, auth_state:%d\n", i, g_diag_msp_msg_info.conn_state[i], g_diag_msp_msg_info.auth_state[i]);
    }
}

int diag_msg_auth_check(diag_frame_req_s *data, unsigned int port_type)
{
    int ret;
    unsigned long flag;

    spin_lock_irqsave(&g_diag_msp_msg_info.spinlock, flag);
    if (data->sid == DIAG_SID_DEBUG) {
        ret = g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_MXT];
    } else {
        if (port_type == DIAG_PORT_TYPE_NO_CODER) {
            ret = g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_HMISRV];
        } else {
            ret = g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_DIAGTOOL];
        }
    }
    spin_unlock_irqrestore(&g_diag_msp_msg_info.spinlock, flag);

    return ret;
}

bool diag_msg_is_conn_cmd(unsigned int cmd_id)
{
    switch (cmd_id) {
        case DIAG_CMD_HOST_CONNECT:
        case DIAG_CMD_HOST_DISCONNECT:
        case DIAG_CMD_HMISRV_CONN:
        case DIAG_CMD_HMISRV_DISCONN:
        case DIAG_CMD_HMISRV_AUTH:
        case DIAG_CMD_HMISRV_LOGCAT_CONN:
        case DIAG_CMD_HMISRV_LOGCAT_DISCONN:
            return true;

        default:
            return false;
    }
}

// TODO g_diag_msp_msg_info需要加锁保护
unsigned int mdrv_diag_get_conn_state(void)
{
    unsigned int i;
    unsigned int state = 0;
    unsigned long flag;

    spin_lock_irqsave(&g_diag_msp_msg_info.spinlock, flag);
    for (i = 0; i < DIAG_TOOL_TYPE_BUTT; i++) {
        state |= (g_diag_msp_msg_info.conn_state[i] << i);
    }
    spin_unlock_irqrestore(&g_diag_msp_msg_info.spinlock, flag);

    return state;
}

void diagtool_conn_msg_proc(void)
{
    const bsp_version_info_s *version = bsp_get_version_info();

    diag_crit("received diagtool conn cmd\n");

    diag_reset_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL);

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_DIAGTOOL] = DIAG_CFG_OPEN;

    diag_reset_debug_info(DIAG_TOOL_TYPE_DIAGTOOL);

    diag_conn_state_notify(DIAG_TOOL_TYPE_DIAGTOOL, mdrv_diag_get_conn_state());

    if ((version != NULL) && (version->plat_type == PLAT_EMU)) {
        bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_IND, ODT_IND_MODE_CYCLE);
        diag_open_all_switch();
        diag_crit("EMU diag connect: odt set mode cycle\n");
        return;
    }

    if (diag_is_power_on_log_open() || diag_is_cycle_on_open()) {
        g_diag_msp_msg_info.power_on_log = 0;
    } else {
        scm_dst_chan_reset(ODT_CODER_DST_OM_IND);
    }

    return;
}

void diagtool_rxready_msg_proc(void)
{
    int ret = 0;

    diag_crit("received diagtool rxready cmd\n");
    ret = ppm_logic_port_switch(PPM_DIAG_TOOL_PORT);
    if (ret == 0) {
        diag_crit("enable coder ind dst chnl.\n");
        bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_IND, ODT_IND_MODE_DIRECT);
        bsp_odt_data_send_manager(ODT_CODER_DST_OM_IND, ODT_DEST_DSM_ENABLE);
    } else {
        diag_crit("diagtool port switch failed.\n");
        return;
    }

    if (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state())) {
        diag_report_diag_tool_conn_event(DIAG_TOOL_CONN_PREEMPT_VALUE);
    }

    return;
}


void hmisrv_conn_msg_proc(void)
{
    diag_crit("received hmisrv conn cmd\n");

    diag_reset_cfg_switch(DIAG_TOOL_TYPE_HMISRV);

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_HMISRV] = DIAG_CFG_OPEN;

    diag_reset_debug_info(DIAG_TOOL_TYPE_HMISRV);

    diag_conn_state_notify(DIAG_TOOL_TYPE_HMISRV, mdrv_diag_get_conn_state());

    return;
}

void hmisrv_logcat_conn_msg_proc(void)
{
    int ret = 0;
    diag_port_info_s port_info;
    port_info.port = PPM_HMI_PORT;
    diag_crit("received hmisrv logcat conn cmd\n");

    if (g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_HMISRV] == DIAG_AUTH_TYPE_SUCCESS) {
        ret = diag_coder_port_switch_handler((unsigned char *)&port_info, sizeof(diag_port_info_s));
        if (ret != 0) {
            diag_crit("logcat chnl switch failed.\n");
            return;
        }

        g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_DIAGTOOL] = DIAG_CFG_OPEN;
        g_diag_msp_msg_info.logcat_is_conn = 1;

        diag_crit("enable ind dst chan\n");
        bsp_odt_data_send_manager(ODT_CODER_DST_OM_IND, ODT_DEST_DSM_ENABLE);
    }

    return;
}

void mxt_conn_msg_proc(void)
{
    diag_crit("received mxt conn cmd\n");

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_MXT] = DIAG_CFG_OPEN;

    diag_reset_debug_info(DIAG_TOOL_TYPE_MXT);

    diag_conn_state_notify(DIAG_TOOL_TYPE_MXT, mdrv_diag_get_conn_state());

    return;
}

void diag_send_trans_cfg_to_dpa(unsigned int swt)
{
    int ret;
    msg_frame_s *msg_frame = NULL;
    diag_cfg_switch_s *trans_swt;

    msg_frame = (msg_frame_s *)osl_malloc(sizeof(msg_frame_s) + sizeof(diag_cfg_switch_s));
    if (msg_frame == NULL) {
        diag_err("malloc fail\n");
        return;
    }
    msg_frame->msg_id = DIAG_MSG_MSP_REQ;
    trans_swt = (diag_cfg_switch_s *)(msg_frame->data);
    trans_swt->swt = swt;

    ret = diag_msglite_send_to_dpa((unsigned char *)msg_frame, sizeof(msg_frame_s) + sizeof(diag_cfg_switch_s));
    if (ret) {
        diag_err("send trans swt to dpa fail:0x%x\n", ret);
    }
    osl_free(msg_frame);
}

void diagtool_disconn_msg_proc(void)
{
    const bsp_version_info_s *version = bsp_get_version_info();

    diag_crit("received diagtool disconn cmd\n");

    if (g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_DIAGTOOL] == DIAG_CFG_CLOSE) {
        diag_crit("diagtool already disconn\n");
        return;
    }

    diag_reset_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL);

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_DIAGTOOL] = DIAG_CFG_CLOSE;

    diag_reset_debug_info(DIAG_TOOL_TYPE_DIAGTOOL);

    diag_conn_state_notify(DIAG_TOOL_TYPE_DIAGTOOL, mdrv_diag_get_conn_state());
    diag_send_trans_cfg_to_dpa(0);

    if ((version != NULL) && (version->plat_type == PLAT_EMU)) {
        bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_IND, ODT_IND_MODE_CYCLE);
        diag_crit("EMU diag disconnect: odt set mode cycle\n");
        return;
    } else {
        bsp_odt_data_send_manager(ODT_CODER_DST_OM_IND, ODT_DEST_DSM_DISABLE);
    }

    if (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state())) {
        diag_report_diag_tool_conn_event(DIAG_TOOL_CONN_RELEASE_VALUE);
    }

    if (diag_is_cycle_on_open()) {
        bsp_odt_set_dst_chan_mode(ODT_CODER_DST_OM_IND, ODT_IND_MODE_CYCLE);
    } else {
        scm_clear_diag_tool_src_buff();
    }

    return;
}

void hmisrv_disconn_msg_proc(void)
{
    diag_crit("received hmisrv disconn cmd\n");

    if (g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_HMISRV] == DIAG_CFG_CLOSE) {
        diag_crit("himsrv already disconn\n");
        return;
    }

    diag_reset_cfg_switch(DIAG_TOOL_TYPE_HMISRV);

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_HMISRV] = DIAG_CFG_CLOSE;
    g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_HMISRV] = DIAG_AUTH_TYPE_DEFAULT;

    diag_reset_debug_info(DIAG_TOOL_TYPE_HMISRV);

    bsp_odt_data_send_manager(ODT_CODER_DST_OM_NO_CODER_IND, ODT_DEST_DSM_DISABLE);

    if (g_diag_msp_msg_info.logcat_is_conn) {
        hmisrv_logcat_disconn_msg_proc();
    }

    return;
}

void hmisrv_logcat_disconn_msg_proc(void)
{
    diag_crit("received hmisrv logcat disconn cmd\n");

    g_diag_msp_msg_info.logcat_is_conn = 0;

    diag_reset_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL);

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_DIAGTOOL] = DIAG_CFG_CLOSE;

    diag_reset_debug_info(DIAG_TOOL_TYPE_DIAGTOOL);

    diag_conn_state_notify(DIAG_TOOL_TYPE_DIAGTOOL, mdrv_diag_get_conn_state());
    diag_send_trans_cfg_to_dpa(0);

    bsp_odt_data_send_manager(ODT_CODER_DST_OM_IND, ODT_DEST_DSM_DISABLE);

    scm_clear_diag_tool_src_buff();
    
    return;
}

void mxt_disconn_msg_proc(void)
{
    diag_crit("received mxt disconn cmd\n");

    g_diag_msp_msg_info.conn_state[DIAG_TOOL_TYPE_MXT] = DIAG_CFG_CLOSE;
    g_diag_msp_msg_info.auth_state[DIAG_TOOL_TYPE_MXT] = DIAG_AUTH_TYPE_DEFAULT;

    diag_reset_debug_info(DIAG_TOOL_TYPE_MXT);

    diag_conn_state_notify(DIAG_TOOL_TYPE_MXT, mdrv_diag_get_conn_state());

    return;
}

void diag_disconn_all_tool(void)
{
    diagtool_disconn_msg_proc();
    hmisrv_logcat_disconn_msg_proc();
    mxt_disconn_msg_proc();
}

void diag_auth_msg_proc(diag_tool_type_e tool_type)
{
    if (unlikely(tool_type >= DIAG_TOOL_TYPE_BUTT)) {
        diag_err("tool type error, tool_type=0x%x\n", tool_type);
        return;
    }

    if (tool_type == DIAG_TOOL_TYPE_HMISRV) {
        bsp_odt_data_send_manager(ODT_CODER_DST_OM_NO_CODER_IND, ODT_DEST_DSM_ENABLE);
    }
    g_diag_msp_msg_info.auth_state[tool_type] = DIAG_AUTH_TYPE_SUCCESS;

    diag_err("diag tool: 0x%x auth succ\n", tool_type);
    return;
}

void diag_drv_print_cfg_proc(unsigned char *diag_drv_frame)
{
    diag_frame_req_s *diag_frame = (diag_frame_req_s *)diag_drv_frame;
    diag_cfg_switch_s *print_switch = NULL;

    if (diag_frame->msg_len != sizeof(diag_attr_head_s) + sizeof(diag_cfg_switch_s)) {
        diag_err("invalid print cfg, msglen:%d\n", diag_frame->msg_len);
        return;
    }
    print_switch = (diag_cfg_switch_s *)diag_frame->data;
    g_diag_drv_msg_info.print_level = print_switch->swt;
}

void diag_trans_cfg_proc(diag_frame_req_s *diag_frame)
{
    diag_cfg_switch_s *trans_switch = (diag_cfg_switch_s *)diag_frame->data;
    diag_set_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL, DIAGTOOL_CFG_TRANS_BIT, trans_switch->swt);
    diag_send_trans_cfg_to_dpa(trans_switch->swt);
}

void diag_applog_cfg_proc(diag_frame_req_s *diag_frame)
{
    diag_applog_cfg_s *applog_cfg = (diag_applog_cfg_s *)diag_frame->data;
    diag_set_cfg_switch(DIAG_TOOL_TYPE_DIAGTOOL, DIAGTOOL_CFG_APPLOG_BIT, applog_cfg->swt);
    g_diag_msp_msg_info.applog_level = applog_cfg->level;
    diag_crit("recv applog:0x%x, level:0x%x\n", applog_cfg->swt, applog_cfg->level);
}

#define INVALID_CNFTIMER_HANDLE (-1)
diag_timer_manager_s g_diag_timer_manager[DIAG_TIMER_BUTT];

void diag_debug_cfg_proc(diag_frame_req_s *diag_frame)
{
    diag_debug_cfg_s *debug_cfg = (diag_debug_cfg_s *)(diag_frame->data);
    diag_timer_manager_s *debug_timer = &g_diag_timer_manager[DIAG_TIMER_DEBUG];

    if (unlikely(debug_cfg == NULL)) {
        diag_err("diag debug cfg is null\n");
        return;
    }

    if (debug_timer->tmhandle >= 0) {
        (void)mdrv_timer_delete(debug_timer->tmhandle);
        debug_timer->tmhandle = INVALID_CNFTIMER_HANDLE;
    }

    if (debug_cfg->ap_en == DIAG_CFG_OPEN) {
        /* add single time period ms */
        debug_timer->tmattr.time = debug_cfg->period * 1000;
        debug_timer->tmhandle = mdrv_timer_add(&debug_timer->tmattr);
    }

    return;
}

void diag_flow_cfg_proc(diag_frame_req_s *diag_frame)
{
    diag_flow_cfg_s *flow_cfg = (diag_flow_cfg_s *)(diag_frame->data);
    diag_timer_manager_s *flow_timer = &g_diag_timer_manager[DIAG_TIMER_FLOW];

    if (unlikely(flow_cfg == NULL)) {
        diag_err("diag flow cfg is null\n");
        return;
    }

    if (flow_timer->tmhandle >= 0) {
        (void)mdrv_timer_delete(flow_timer->tmhandle);
        flow_timer->tmhandle = INVALID_CNFTIMER_HANDLE;
    }

    if (flow_cfg->ap_en == DIAG_CFG_OPEN) {
        flow_timer->tmattr.time = flow_cfg->ap_peroid * 1000000;
        flow_timer->tmattr.para = flow_cfg->pack_num;
        flow_timer->tmhandle = mdrv_timer_add(&flow_timer->tmattr);
    }

    return;
}

void diag_drv_show_register_module(void)
{
    diag_drv_cfg_node_s *node = NULL;
    diag_drv_cfg_node_s *tmp = NULL;

    list_for_each_entry_safe(node, tmp, &g_diag_drv_msg_info.cfg_list.list, list) {
        diag_err("module_id:0x%x\n", node->module_id);
    }
}

void diag_drv_msg_proc(unsigned char *diag_drv_frame, unsigned int data_len)
{
    diag_drv_cfg_node_s *node = NULL;
    diag_drv_cfg_node_s *tmp = NULL;
    diag_frame_req_s *diag_frame = (diag_frame_req_s *)diag_drv_frame;

    diag_debug("cmd_id:0x%x\n", diag_frame->cmd_id);

    if (list_empty(&g_diag_drv_msg_info.cfg_list.list)) {
        return;
    }

    list_for_each_entry_safe(node, tmp, &g_diag_drv_msg_info.cfg_list.list, list) {
        if ((node->module_id == DIAG_GET_MODULE_FROM_CMD_ID(diag_frame->cmd_id)) && (node->cfg_fn != NULL)) {
            diag_debug("cmd_id:0x%x find\n", diag_frame->cmd_id);
           node->cfg_fn(diag_drv_frame);
        }
    }

    return;
}

void diag_msp_msg_proc(unsigned char *msp_data, unsigned int data_len)
{
    int i;
    diag_frame_req_s *diag_frame = (diag_frame_req_s *)msp_data;

    if ((diag_frame == NULL) ||
        ((diag_frame->msg_len + DIAG_REQ_MSG_HEAD_LEN - DIAG_TOOL_HEAD_LEN) != data_len)) {
            diag_err("cmd_id:0x%x, msg len error, msg_len=0x%x, data_len:0x%x\n", diag_frame->cmd_id,  diag_frame->msg_len, data_len);
            return;
    }

    if (DIAG_GET_GROUP_FROM_CMD_ID(diag_frame->cmd_id) == DIAG_MSG_TYPE_BSP) {
        diag_drv_msg_proc(msp_data, data_len);
        return;
    }

    for (i = 0; i < sizeof(g_diag_msp_proc_table) / sizeof(g_diag_msp_proc_table[0]); i++) {
        if ((diag_frame->cmd_id == g_diag_msp_proc_table[i].cmd_id) && (g_diag_msp_proc_table[i].msg_proc_cb != NULL)) {
            diag_debug("cmd_id:0x%x\n", diag_frame->cmd_id);
            g_diag_msp_proc_table[i].msg_proc_cb(diag_frame);
            return;
        }
    }

    return;
}

void diag_poweron_log_cfg_proc(diag_log_cfg_s *powon_log_cfg, unsigned int data_len)
{
    if (data_len != sizeof(diag_log_cfg_s)) {
        diag_err("recv invalid poweron cfg, len:%d\n", data_len);
        return;
    }

    g_diag_drv_msg_info.print_level = powon_log_cfg->print_level;
    g_diag_msp_msg_info.cycle_on = powon_log_cfg->cycle;
    g_diag_msp_msg_info.power_on_log = powon_log_cfg->power_on;
    diag_crit("recv poweron:0x%x, cycle_on:0x%x, print_level:0x%x\n", \
        powon_log_cfg->power_on, powon_log_cfg->cycle, powon_log_cfg->print_level);
}

int diag_trigger_disconn(diag_tool_type_e tool_type)
{
    int ret;
    msg_frame_s *disconn_msg;
    diag_conn_info_s *info;

    if (tool_type >= DIAG_TOOL_TYPE_BUTT) {
        diag_err("tool type err\n");
        return DIAG_ERRNO_PARA_ERROR;
    }

    disconn_msg = (msg_frame_s *)osl_malloc(sizeof(msg_frame_s) + sizeof(diag_conn_info_s));
    if (disconn_msg == NULL) {
        diag_err("malloc fail\n");
        return DIAG_ERRNO_MALLOC_FAIL;
    }

    switch (tool_type) {
        case DIAG_TOOL_TYPE_DIAGTOOL:
            disconn_msg->msg_id = DIAG_MSG_DIAGTOOL_DISCONN_REQ;
            break;
        case DIAG_TOOL_TYPE_HMISRV:
            disconn_msg->msg_id = DIAG_MSG_HMISRV_DISCONN_REQ;
            break;
        case DIAG_TOOL_TYPE_MXT:
            disconn_msg->msg_id = DIAG_MSG_MXT_DISCONN_REQ;
            break;
        default:
            diag_err("tool type err\n");
    }

    info = (diag_conn_info_s *)disconn_msg->data;
    info->cmd_id = DIAG_CMD_HOST_DISCONNECT;

    diag_err("tool type:0x%x send disconn to cp\n", tool_type);
    ret = diag_msg_send((unsigned char *)disconn_msg, sizeof(msg_frame_s) + sizeof(diag_conn_info_s));
    if (ret) {
        diag_err("diag send disconn msg to ccore fail, ret=0x%x\n", ret);
    }

    osl_free(disconn_msg);
    return ret;
}

void diag_report_diag_tool_conn_event(unsigned int value)
{
    diag_cnf_info_s cnf_msg = {0};
    unsigned int result = value;

    cnf_msg.cmd_id = DIAG_CMD_DIAG_TOOL_CONN_EVENT;
    cnf_msg.msg_len = sizeof(unsigned int) + 8; //加上三级头长度
    cnf_msg.data = (unsigned char *)&result;

    
    cnf_msg.port_type = PPM_CNF_NOCODER_CHAN;
    (void)mdrv_diag_cnf_report(&cnf_msg, sizeof(diag_cnf_info_s) - sizeof(unsigned char*) + cnf_msg.msg_len - 8);

    return;
}

void diag_report_reset_info(unsigned int value)
{
    diag_cnf_info_s cnf_msg = {0};
    unsigned int result = value;

    cnf_msg.cmd_id = DIAG_CMD_MODEM_WILL_RESET;
    cnf_msg.msg_len = sizeof(unsigned int) + 8; //加上三级头长度
    cnf_msg.data = (unsigned char *)&result;

    if (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state())) {
        cnf_msg.port_type = PPM_CNF_NOCODER_CHAN;
        (void)mdrv_diag_cnf_report(&cnf_msg, sizeof(diag_cnf_info_s) - sizeof(unsigned char*) + cnf_msg.msg_len - 8);
    }

    if (value == DIAG_MODEM_RESET_BEFORE_VALUE) {
        if (DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state())) {
            cnf_msg.port_type = PPM_CNF_CODER_CHAN;
            (void)mdrv_diag_cnf_report(&cnf_msg, sizeof(diag_cnf_info_s) - sizeof(unsigned char*) + cnf_msg.msg_len - 8);
        }
    }

    return;
}

int diag_modem_reset_handler(drv_reset_cb_moment_e param, int userdata)
{
    if (param == MDRV_RESET_CB_BEFORE) {
        diag_crit("diag report before modem reset\n");
        diag_report_reset_info(DIAG_MODEM_RESET_BEFORE_VALUE);
        diag_disconn_all_tool();
    }
    if (param == MDRV_RESET_CB_AFTER) {
        diag_report_reset_info(DIAG_MODEM_RESET_AFTER_VALUE);
    }
    return BSP_OK;
}

void diag_report_mntn(void)
{
    diag_ind_info_s ind_msg = {0};
    static unsigned int last_slice = 0;
    diag_dbg_mntn_update_s *mntn_rpt = (diag_dbg_mntn_update_s*)&g_diag_dbg;
    unsigned int current_slice;
    int ret;

    current_slice = bsp_get_slice_value();
    g_diag_dbg.delta_time = get_timer_slice_delta(last_slice, current_slice);

    ind_msg.cmd_id = 0x9f382001;
    ind_msg.msg_len = sizeof(diag_dbg_info_s);
    ind_msg.modem_id = 0;
    ind_msg.level = DIAG_LOG_LEVEL_0;
    ind_msg.data = (unsigned char *)&g_diag_dbg;

    ret = mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind_msg, DIAG_IND_INFO_HEAD_LEN + ind_msg.msg_len);
    if (ret == 0) {
        (void)memset_s(mntn_rpt, sizeof(diag_dbg_mntn_update_s), 0, sizeof(diag_dbg_mntn_update_s));
        last_slice = current_slice;
    }
    return;
}

int diag_debug_timeout_proc(int data)
{
    diag_timer_manager_s *debug_timer = &g_diag_timer_manager[DIAG_TIMER_DEBUG];
    diag_report_mntn();
    debug_timer->tmhandle = mdrv_timer_add(&debug_timer->tmattr);
    return 0;
}

int diag_flow_test_timeout_proc(int data)
{
    return 0;
}

void diag_msp_timer_init(void)
{
    unsigned char debug_timer_name[] = "diag_debug";
    unsigned char flow_test_timer_name[] = "diag_flow_test";
    int ret;

    diag_timer_manager_s *timer = &g_diag_timer_manager[DIAG_TIMER_DEBUG];
    timer->tmhandle = INVALID_CNFTIMER_HANDLE;
    ret = strcpy_s(timer->tmattr.name, TIMER_NAME_MAX_LEN, debug_timer_name);
    if (ret) {
        diag_err("init diag_debug timer fail: %d\n", ret);
        return;
    }
    timer->tmattr.flag = TIMER_FLAG_DEFAULT;
    timer->tmattr.callback = diag_debug_timeout_proc;

    timer = &g_diag_timer_manager[DIAG_TIMER_FLOW];
    timer->tmhandle = INVALID_CNFTIMER_HANDLE;
    ret = strcpy_s(timer->tmattr.name, TIMER_NAME_MAX_LEN, flow_test_timer_name);
    if (ret) {
        diag_err("init flow_test timer fail: %d\n", ret);
        return;
    }
    timer->tmattr.flag = TIMER_FLAG_DEFAULT;
    timer->tmattr.callback = diag_flow_test_timeout_proc;
}

void bsp_diag_drv_cfg_fn_register(unsigned int module_id, diag_drv_cfg_fn cfg_proc_fn)
{
    diag_drv_cfg_node_s *node = NULL;
    unsigned int state = g_diag_drv_msg_info.cfg_list.init_state;
    unsigned long lock_flag;

    if (state == 0) { /* 自初始化需要优化，当前无法保护并发场景 */
        INIT_LIST_HEAD(&g_diag_drv_msg_info.cfg_list.list);
        spin_lock_init(&g_diag_drv_msg_info.cfg_list.spinlock);
        g_diag_drv_msg_info.cfg_list.init_state = 1;
    }

    node = (diag_drv_cfg_node_s *)osl_malloc(sizeof(diag_drv_cfg_node_s));
    if (node == NULL) {
        diag_err("drv cfg list node malloc fail\n");
        return;
    }

    diag_crit("drv cfg register, module_id:0x%x\n", module_id);
    spin_lock_irqsave(&g_diag_drv_msg_info.cfg_list.spinlock, lock_flag);
    node->module_id = module_id;
    node->cfg_fn = cfg_proc_fn;
    list_add_tail(&node->list, &g_diag_drv_msg_info.cfg_list.list);
    spin_unlock_irqrestore(&g_diag_drv_msg_info.cfg_list.spinlock, lock_flag);
    return;
}

void diag_msp_msg_init(void)
{
    int ret;
    unsigned int state = g_diag_drv_msg_info.cfg_list.init_state;

    if (state == 0) { 
        INIT_LIST_HEAD(&g_diag_drv_msg_info.cfg_list.list);
        spin_lock_init(&g_diag_drv_msg_info.cfg_list.spinlock);
        g_diag_drv_msg_info.cfg_list.init_state = 1;
    }
    spin_lock_init(&g_diag_msp_msg_info.spinlock);

    ret = mdrv_sysboot_register_reset_notify("DIAG", diag_modem_reset_handler, 0, DRV_RESET_CB_PIOR_DIAG);
    if (ret) {
        diag_err("diag reset handle register fail:%d\n", ret);
    }

    diag_msp_timer_init();

    ret = diag_applog_init();
    if (ret) {
        diag_err("diag applog init fail:%d\n", ret);
    }

    bsp_diag_drv_cfg_fn_register(DIAG_CMD_PRINT_CFG, diag_drv_print_cfg_proc);
}


#ifdef DIAG_APPLOG
#define APPLOGCHAR_DEVICE_NAME "applog"
#define APPLOGCHAR_DEVICE_CLASS "applog_class"

#define APPLOG_HEAD_LEN 2
#define APPLOG_MAX_USER_BUFF_LEN 512
#define APPLOG_MAX_BUFF_LEN (APPLOG_MAX_USER_BUFF_LEN + DIAG_IND_INFO_HEAD_LEN)

#define DIAG_APPLOG_CMD_ID 0xEF180000

typedef struct {
    unsigned int init_state;
    struct mutex write_lock;
} diag_applog_ctrl_s;

diag_applog_ctrl_s g_diag_applog_ctrl;
unsigned char *g_applog_sendbuf;

int diag_applog_open(struct inode *inode, struct file *file)
{
    if (g_diag_applog_ctrl.init_state == 0) {
        return -1;
    }
    return 0;
}

int diag_applog_release(struct inode *inode, struct file *file)
{
    return 0;
}

ssize_t diag_applog_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    unsigned int len = 0;
    diag_ind_info_s ind_info = { 0 };

    g_diag_dbg.applog_dbg.write_entry++;
    if (((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && \
        DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state() == 0)) || \
        diag_get_cfg_switch(DIAGTOOL_CFG_APPLOG_BIT) == 0) {
        g_diag_dbg.applog_dbg.switch_off++;
        return 0;
    }

    if ((buf == NULL) || (count <= DIAG_IND_INFO_HEAD_LEN)) {
        g_diag_dbg.applog_dbg.invalid_para++;
        return 0;
    }

    mutex_lock(&g_diag_applog_ctrl.write_lock);
    len = (count > APPLOG_MAX_BUFF_LEN) ? APPLOG_MAX_BUFF_LEN : count;

    if (copy_from_user(&ind_info, buf, DIAG_IND_INFO_HEAD_LEN)) {
        diag_err("copy head from user fail\n");
        g_diag_dbg.applog_dbg.copy_head_err++;
        mutex_unlock(&g_diag_applog_ctrl.write_lock);
        return 0;
    }

    if (ind_info.msg_len != (len - DIAG_IND_INFO_HEAD_LEN)) {
        g_diag_dbg.applog_dbg.len_err++;
        mutex_unlock(&g_diag_applog_ctrl.write_lock);
        return 0;
    }

    if (copy_from_user(g_applog_sendbuf, buf + DIAG_IND_INFO_HEAD_LEN, len - DIAG_IND_INFO_HEAD_LEN)) {
        diag_err("copy data from user fail, buf = %s\n", g_applog_sendbuf);
        g_diag_dbg.applog_dbg.copy_data_err++;
        mutex_unlock(&g_diag_applog_ctrl.write_lock);
        return 0;
    }
    ind_info.data = g_applog_sendbuf;

    ret = mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind_info, len); /* 用scm_write_data可以减少部分冗余检查 */
    mutex_unlock(&g_diag_applog_ctrl.write_lock);
    if (ret) {
        g_diag_dbg.applog_dbg.report_fail++;
        return 0;
    }
    g_diag_dbg.applog_dbg.send_ok++;

    return len;
}

static struct file_operations g_applog_fops = {
    .open = diag_applog_open,
    .owner = THIS_MODULE,
    .write = diag_applog_write,
    .release = diag_applog_release,
};

struct cdev g_applog_cdev;
static unsigned int g_applog_char_major;
static struct class *g_applog_class;

s32 applog_setup_cdev(struct cdev *cdev, struct file_operations *fops)
{
    s32 ret;
    dev_t devno = MKDEV(g_applog_char_major, 0);

    cdev_init(cdev, fops);
    cdev->owner = THIS_MODULE;
    cdev->ops = fops;

    ret = cdev_add(cdev, devno, 1);
    if (ret) {
        diag_err("add applog fail!\n");
        return ret;
    }

    g_applog_class = class_create(THIS_MODULE, APPLOGCHAR_DEVICE_CLASS);
    if (IS_ERR(g_applog_class)) {
        diag_err("class create failed!\n");
        return -1;
    }

    device_create(g_applog_class, NULL, devno, NULL, APPLOGCHAR_DEVICE_NAME);
    return 0;
}

void applog_exit(void)
{
    osl_free(g_applog_sendbuf);
    cdev_del(&(g_applog_cdev));
    class_destroy(g_applog_class);
    unregister_chrdev_region(MKDEV(g_applog_char_major, 0), 1);
}

int diag_applog_init(void)
{
    int ret;
    dev_t dev = 0;

    /* 动态获取设备号 */
    ret = alloc_chrdev_region(&dev, 0, 1, APPLOGCHAR_DEVICE_NAME);
    if (ret) {
        diag_err("applog chrdev failed alloc :%d\n", g_applog_char_major);
        return ret;
    }
    g_applog_char_major = MAJOR(dev);

    /* 创建字符驱动设备 */
    ret = applog_setup_cdev(&g_applog_cdev, &g_applog_fops);
    if (ret) {
        diag_err("setup chrdev failed !\n");
        return ret;
    }

    /* 申请数据buffer */
    g_applog_sendbuf = (unsigned char *)osl_malloc(APPLOG_MAX_USER_BUFF_LEN);
    if (g_applog_sendbuf == NULL) {
        diag_err("kbuf malloc fail!\n");
        return -1;
    }

    memset_s(g_applog_sendbuf, APPLOG_MAX_USER_BUFF_LEN, 0, APPLOG_MAX_USER_BUFF_LEN);

    g_diag_applog_ctrl.init_state = 1;
    diag_crit("applog init ok\n");
    return 0;
}
#else
int diag_applog_init(void)
{
    diag_crit("applog not open\n");
    return 0;
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
