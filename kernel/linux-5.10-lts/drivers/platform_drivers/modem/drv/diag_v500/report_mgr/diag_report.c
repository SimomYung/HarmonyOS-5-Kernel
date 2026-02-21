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
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_malloc.h>
#include <bsp_slice.h>
#include <bsp_print.h>
#include <bsp_diag.h>
#include <mdrv_diag.h>
#include "ppm.h"
#include "scm_src.h"
#include "diag_nve.h"
#include "diag_report.h"
#include "diag_debug.h"
#include "diag_msp_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

diag_report_info_s g_diag_report_info;

static inline void diag_cnf_no_coder_frame_fill(diag_cnf_no_coder_frame_s *cnf_frame, diag_cnf_info_s *diag_msg)
{
    cnf_frame->sid = DIAG_SID_HMI_SRV;
    cnf_frame->ver = DIAG_FRAME_VER_2;
    cnf_frame->ssid = DIAG_FRAME_SSID_APP_CPU;
    cnf_frame->mt = DIAG_FRAME_MT_CNF;
    cnf_frame->timestamp = bsp_get_slice_value();
   
    cnf_frame->cmd_id = diag_msg->cmd_id;

    cnf_frame->msg_len = diag_msg->msg_len;
    cnf_frame->modem_id = diag_msg->modem_id;
    cnf_frame->level = diag_msg->level;
    cnf_frame->reserved = diag_msg->reserved;
    cnf_frame->msg_ver = diag_msg->msg_ver;

    cnf_frame->auid = diag_msg->auid;
    cnf_frame->sn = diag_msg->sn; 
    cnf_frame->rsv = diag_msg->rsv;
    cnf_frame->port_type = diag_msg->port_type;

    if (memcpy_s(&cnf_frame->data, cnf_frame->msg_len - 8, diag_msg->data, diag_msg->msg_len - 8)) {
        diag_err("diag_cnf_no_coder_frame_fill memcpy fail\n");
    }
}

static inline void diag_ind_no_coder_frame_fill(diag_ind_no_coder_frame_s *ind_frame, diag_ind_info_s *diag_msg)
{
    ind_frame->sid = DIAG_SID_HMI_SRV;
    ind_frame->ver = DIAG_FRAME_VER_2;
    ind_frame->ssid = DIAG_FRAME_SSID_APP_CPU;
    ind_frame->mt = DIAG_FRAME_MT_IND;
    ind_frame->timestamp = bsp_get_slice_value();
   
    ind_frame->cmd_id = diag_msg->cmd_id;

    ind_frame->msg_len = diag_msg->msg_len;
    ind_frame->modem_id = diag_msg->modem_id;
    ind_frame->level = diag_msg->level;
    ind_frame->reserved = diag_msg->reserved;
    ind_frame->msg_ver = diag_msg->msg_ver;

    ind_frame->data = diag_msg->data;

    if (memcpy_s(&ind_frame->data, ind_frame->msg_len, diag_msg->data, diag_msg->msg_len)) {
        diag_err("diag_ind_no_coder_frame_fill memcpy fail\n");
    }
}

int diag_report_by_genl(diag_frame_mt_e mt, unsigned char *diag_msg, unsigned int len)
{
    
    diag_cnf_no_coder_frame_s *cnf_frame = NULL;
    diag_ind_no_coder_frame_s *ind_frame = NULL;
    unsigned long lock_flag;

    if (mt == DIAG_FRAME_MT_CNF) {
        cnf_frame = (diag_cnf_no_coder_frame_s *)kzalloc(len + DIAG_MSG_HEAD_LEN, GFP_KERNEL);
        if (cnf_frame == NULL) {
            return DIAG_ERRNO_MALLOC_FAIL;
        }
        diag_cnf_no_coder_frame_fill(cnf_frame, (diag_cnf_info_s *)diag_msg);

        spin_lock_irqsave(&g_diag_report_info.lock[DIAG_CHAN_MSG_CNF], lock_flag);
        cnf_frame->trans_id = g_diag_report_info.trans_id[DIAG_CHAN_MSG_CNF]++;
        spin_unlock_irqrestore(&g_diag_report_info.lock[DIAG_CHAN_MSG_CNF], lock_flag);
        ppm_data_send(PPM_CNF_NOCODER_CHAN, (char *)cnf_frame, NULL, len + DIAG_MSG_HEAD_LEN);
        kfree(cnf_frame);
    } else {
        ind_frame = (diag_ind_no_coder_frame_s *)kzalloc(len + DIAG_MSG_HEAD_LEN, GFP_KERNEL);
        if (ind_frame == NULL) {
            return DIAG_ERRNO_MALLOC_FAIL;
        }
        diag_ind_no_coder_frame_fill(ind_frame, (diag_ind_info_s *)diag_msg);

        spin_lock_irqsave(&g_diag_report_info.lock[DIAG_CHAN_MSG_IND], lock_flag);
        ind_frame->trans_id = g_diag_report_info.trans_id[DIAG_CHAN_MSG_IND]++;
        spin_unlock_irqrestore(&g_diag_report_info.lock[DIAG_CHAN_MSG_IND], lock_flag);
        ppm_data_send(PPM_IND_NOCODER_CHAN, (char *)ind_frame, NULL, len + DIAG_MSG_HEAD_LEN);
        kfree(ind_frame);
    }
    return 0;
}

int mdrv_diag_cnf_report(diag_cnf_info_s *cnf_msg, unsigned int data_len)
{
    int ret;

    g_diag_dbg.api_dbg.cnf_rpt_cnt++;
    if (unlikely((cnf_msg == NULL) || (data_len >= DIAG_FRAME_MAX_LEN) ||
        data_len != (cnf_msg->msg_len + DIAG_CNF_INFO_HEAD_LEN - DIAG_TOOL_HEAD_LEN))) {
        g_diag_dbg.api_dbg.cnf_rpt_fail_para_err++;
        diag_err("cnf msg len error, data_len=0x%x\n", data_len);
        return DIAG_ERRNO_MSG_LEN_ERROR;
    }

    if (cnf_msg->port_type == PPM_CNF_NOCODER_CHAN) {
        g_diag_dbg.api_dbg.cnf_rpt_nocoder_cnt++;
        ret = diag_report_by_genl(DIAG_FRAME_MT_CNF, (unsigned char *)cnf_msg, data_len);
    } else {
        g_diag_dbg.api_dbg.cnf_rpt_coder_cnt++;
        ret = scm_write_data(SCM_CNF_SRC_CHAN, (unsigned char *)cnf_msg, data_len);
    }

    return ret;
}
EXPORT_SYMBOL(mdrv_diag_cnf_report);

int mdrv_diag_ind_report(unsigned int ind_type, diag_ind_info_s *ind_msg, unsigned int data_len)
{
    int ret = DIAG_ERRNO_PARA_ERROR;

    g_diag_dbg.api_dbg.ind_rpt_cnt++;
    if ((unlikely(ind_msg == NULL) || (data_len >= DIAG_FRAME_MAX_LEN) ||
        data_len != (ind_msg->msg_len + DIAG_IND_INFO_HEAD_LEN))) {
        g_diag_dbg.api_dbg.ind_rpt_fail_para_err++;
        diag_err("ind msg len error, data_len=0x%x, ind_msg_len:0x%x\n", data_len, ind_msg->msg_len);
        return DIAG_ERRNO_MSG_LEN_ERROR;
    }

    if ((ind_type & DIAG_REALTIME_REPORT_TYPE) && DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state())) {
        g_diag_dbg.api_dbg.ind_realtime_rpt_cnt++;
       ret = diag_report_by_genl(DIAG_FRAME_MT_IND, (unsigned char *)ind_msg, data_len);
    }

    if ((ind_type & DIAG_PARSE_IN_UE_TYPE) && DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state())) {
        g_diag_dbg.api_dbg.ind_parse_in_ue_cnt++;
        ret = scm_write_data(SCM_NO_CODER_SRC_CHAN, (unsigned char *)ind_msg, data_len);
    }

    if ((ind_type & DIAG_COMMON_OAM_TYPE) && \
            ((DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) || \
            DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) || \
            diag_is_power_on_log_open()))) {
        g_diag_dbg.api_dbg.ind_oam_cnt++;
        ret = scm_write_data(SCM_IND_SRC_CHAN, (unsigned char *)ind_msg, data_len);
    }

    return ret;
}
EXPORT_SYMBOL(mdrv_diag_ind_report);

int mdrv_diag_print_report(diag_ind_info_s *ind_head, const char *fmt, ...)
{
    int len;
    char text[DIAG_PRINT_MAX_LEN + 1] = {'\0'};
    diag_ind_info_s ind_new_head = {0};
    diag_print_head_s *print_ind_head = (diag_print_head_s *)ind_head->data;
    diag_print_head_s *print_head = (diag_print_head_s *)text;
    va_list arg;

    g_diag_dbg.api_dbg.mdrv_print_cnt++;
    if ((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) == 0)) {
        g_diag_dbg.api_dbg.mdrv_print_fail_disconn++;
        return -1;
    }

    ind_new_head.cmd_id = ind_head->cmd_id;
    ind_new_head.msg_len = ind_head->msg_len;
    ind_new_head.modem_id = ind_head->modem_id;
    ind_new_head.level = ind_head->level;
    ind_new_head.reserved = ind_head->reserved;
    ind_new_head.msg_ver = ind_head->msg_ver;

    print_head->pid = print_ind_head->pid;
    print_head->level = DIAG_GEN_PRINT_LEVEL(print_ind_head->level);

    va_start(arg, fmt);
    len = vsnprintf_s(print_head->data, DIAG_PRINT_MAX_LEN, DIAG_PRINT_MAX_LEN - 1, (const char *)fmt, arg);
    if (len <= 0) {
        diag_err("vsnprintf fail, ret:0x%x\n", len);
        va_end(arg);
        g_diag_dbg.api_dbg.mdrv_print_fail_vsnprintf_fail++;
        return DIAG_ERRNOR_VSNPRINTF_ERROR;
    }
    va_end(arg);
    text[DIAG_PRINT_MAX_LEN - 1] = '\0';

    ind_new_head.msg_len = len + DIAG_PRINT_HEAD_LEN;
    ind_new_head.data = (unsigned char *)print_head;
    len = ind_new_head.msg_len + DIAG_IND_INFO_HEAD_LEN;

    g_diag_dbg.api_dbg.mdrv_print_oam_cnt++;
    return mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind_new_head, len);
}
EXPORT_SYMBOL(mdrv_diag_print_report);

int bsp_diag_ind_report(unsigned int ind_type, unsigned int msg_id, diag_drv_ind_head_s *drv_data, unsigned short length)
{
   diag_ind_info_s ind_msg = { 0 };

    g_diag_dbg.api_dbg.bsp_ind_rpt_cnt++;
    if ((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) == 0)) {
        g_diag_dbg.api_dbg.bsp_ind_rpt_fail_disconn++;
        return -1;
    }

    ind_msg.cmd_id = msg_id;
    ind_msg.msg_len =  DIAG_DRV_IND_HEAD_LEN + length;
    ind_msg.level = DIAG_LOG_LEVEL_0;
    ind_msg.reserved = 0;
    ind_msg.msg_ver = 0;
    ind_msg.data = (unsigned char *)drv_data;

    g_diag_dbg.api_dbg.bsp_ind_rpt_ok_cnt++;
    return mdrv_diag_ind_report(ind_type, &ind_msg, ind_msg.msg_len + DIAG_IND_INFO_HEAD_LEN);
}
EXPORT_SYMBOL(bsp_diag_ind_report);

int bsp_diag_mxt_report(unsigned int cmd_id, unsigned char *data, unsigned int data_len)
{
    diag_ind_info_s ind_msg = {0};

    if (data == NULL) {
        diag_err("mprobe report para invalid, len:0x%x\n", data_len);
        return DIAG_ERRNO_PARA_ERROR;
    }

    if (DIAG_GET_MXT_CONN_STATE(mdrv_diag_get_conn_state()) == 0) {
        diag_err("mprobe disconnect\n");
        return DIAG_ERRNO_DISCONNECT;
    }

    ind_msg.cmd_id = cmd_id;
    ind_msg.msg_len = data_len;
    ind_msg.data = data;

    return scm_write_data(SCM_MXT_SRC_CHAN, (unsigned char *)&ind_msg, data_len + DIAG_IND_INFO_HEAD_LEN);
}

int bsp_diag_trans_report(unsigned int msg_id, unsigned int pid, unsigned char *data, unsigned short length)
{
    int ret;
    diag_drv_ind_head_s *trans_head = NULL;
    diag_ind_info_s diag_ind = {0};

    g_diag_dbg.api_dbg.bsp_trans_cnt++;
    if(unlikely(data == 0)) {
        g_diag_dbg.api_dbg.bsp_trans_fail_para_err++;
        return DIAG_ERRNO_PARA_ERROR;
    }

    if (((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) == 0)) || diag_get_cfg_switch(DIAGTOOL_CFG_TRANS_BIT) == DIAG_CFG_CLOSE) {
        g_diag_dbg.api_dbg.bsp_trans_fail_disconn_switchoff++;
        return DIAG_ERRNO_DISCONNECT;
    }

    trans_head = (diag_drv_ind_head_s *)kmalloc(sizeof(diag_drv_ind_head_s) + length, GFP_ATOMIC);
    if (trans_head == NULL) {
        g_diag_dbg.api_dbg.bsp_trans_fail_alloc_fail++;
        return DIAG_ERRNO_MALLOC_FAIL;
    }

    trans_head->mid = pid;
    trans_head->resv = 0;
    if (memcpy_s(trans_head->data, length, data, length)) {
        osl_free(trans_head);
        g_diag_dbg.api_dbg.bsp_trans_fail_cpy_data_fail++;
        return DIAG_ERRNO_MEMCPY_FAIL;
    }

    diag_ind.cmd_id = msg_id;
    diag_ind.msg_len = sizeof(diag_drv_ind_head_s) + length;
    diag_ind.modem_id = DIAG_MODEM_0;
    diag_ind.level = DIAG_LOG_LEVEL_0;
    diag_ind.data = (unsigned char *)trans_head;

    ret = mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &diag_ind, diag_ind.msg_len + DIAG_IND_INFO_HEAD_LEN);
    osl_free(trans_head);
    if (ret) {
        g_diag_dbg.api_dbg.bsp_trans_ind_rpt_fail++;
        return ret;
    }
    g_diag_dbg.api_dbg.bsp_trans_ind_rpt_succ++;

    return ret;
}

int bsp_diag_print_report(unsigned int level, const char *fmt, ...)
{
    int len;
    unsigned int cur_level = diag_get_print_level();
    char text[DIAG_PRINT_MAX_LEN + 1] = {'\0'};
    diag_print_head_s *print_head = (diag_print_head_s *)text;
    diag_ind_info_s ind_head = {0};
    va_list arg;

    g_diag_dbg.api_dbg.bsp_print_cnt++;
    if (((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) == 0)) || cur_level == 0 || level > cur_level) {
        g_diag_dbg.api_dbg.bsp_print_fail_disconn_level++;
        return -1;
    }

    ind_head.cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_PRINT, 0, 0, 0); // 0x9F182200
    ind_head.modem_id = DIAG_MODEM_0;
    ind_head.level = DIAG_LOG_LEVEL_0;
    ind_head.msg_ver = 0;

    print_head->pid = DIAG_DRV_HDS_PID;
    print_head->level = DIAG_GEN_PRINT_LEVEL(level);

    va_start(arg, fmt);
    len = vsnprintf_s(print_head->data, DIAG_PRINT_MAX_LEN - DIAG_PRINT_HEAD_LEN, \
        DIAG_PRINT_MAX_LEN - DIAG_PRINT_HEAD_LEN - 1, (const char *)fmt, arg);
    if (len <= 0) {
        diag_err("vsnprintf fail, ret:0x%x\n", len);
        va_end(arg);
        g_diag_dbg.api_dbg.bsp_print_fail_vsnprintf_fail++;
        return DIAG_ERRNOR_VSNPRINTF_ERROR;
    }
    va_end(arg);
    text[DIAG_PRINT_MAX_LEN - 1] = '\0';

    ind_head.msg_len = len + DIAG_PRINT_HEAD_LEN;
    ind_head.data = (unsigned char *)print_head;
    len = ind_head.msg_len + DIAG_IND_INFO_HEAD_LEN;

    g_diag_dbg.api_dbg.bsp_print_ok_cnt++;
    return mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind_head, len);
}
EXPORT_SYMBOL(bsp_diag_print_report);

static int diag_print_report_hook(unsigned int level, const char *print_buf, va_list arg)
{
    unsigned int len;
    unsigned int cur_level = diag_get_print_level();
    char text[DIAG_PRINT_MAX_LEN + 1] = {'\0'};
    diag_print_head_s *print_head = (diag_print_head_s *)text;
    diag_ind_info_s ind_head = {0};

    if (((DIAG_GET_DIAG_TOOL_CONN_STATE(mdrv_diag_get_conn_state()) == 0) && (DIAG_GET_HMI_SRV_CONN_STATE(mdrv_diag_get_conn_state()) == 0)) || cur_level == 0 || level > cur_level) {
        return DIAG_ERRNO_DISCONNECT;
    }

    if (print_buf == NULL) {
        return DIAG_ERRNO_PARA_ERROR;
    }
    len = strlen(print_buf);
    if (len >= DIAG_PRINT_MAX_LEN - DIAG_PRINT_HEAD_LEN) {
        return DIAG_ERRNO_MSG_LEN_ERROR;
    }

    ind_head.cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_PRINT, 0, 0, 0);
    ind_head.modem_id = DIAG_MODEM_0;
    ind_head.level = DIAG_LOG_LEVEL_0;
    ind_head.msg_ver = 0;

    print_head->pid = DIAG_DRV_HDS_PID;
    print_head->level = DIAG_GEN_PRINT_LEVEL(level);

    if (memcpy_s(print_head->data, len, print_buf, len)) {
        return DIAG_ERRNO_MALLOC_FAIL;
    }
    text[DIAG_PRINT_MAX_LEN - 1] = '\0';

    ind_head.msg_len = len + DIAG_PRINT_HEAD_LEN;
    ind_head.data = (unsigned char *)print_head;
    len = ind_head.msg_len + DIAG_IND_INFO_HEAD_LEN;

    return scm_write_data(SCM_IND_SRC_CHAN, (unsigned char *)&ind_head, len);
}

void diag_report_init(void)
{
    int i;

    for (i = 0; i < DIAG_CHAN_BUTT; i++) {
        spin_lock_init(&g_diag_report_info.lock[i]);
        g_diag_report_info.trans_id[i] = 0;
    }

    bsp_log_string_pull_hook(diag_print_report_hook);

    mdmlog_nve_init();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
