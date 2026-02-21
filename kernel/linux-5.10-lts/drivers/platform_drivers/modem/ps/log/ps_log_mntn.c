/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include "mdrv_timer.h"
#include "securec.h"
#include "ps_log_mntn.h"
#include "ps_log_ue_interface.h"
#include "ps_log_base.h"
#include "mdrv_diag.h"

struct ps_log_layer_rpt_fail_info g_layer_rpt_fail_record;
struct ps_log_trace_rpt_fail_info g_trace_rpt_fail_record;
struct ps_log_reply_rpt_fail_info g_reply_rpt_fail_record;

/*
 * 功能描述: mntn内部用于上报tans消息
 * 说明：参数由调用者保证
 */
static void ps_log_rpt_trans(enum ps_log_trans_id msg_id, void *data, unsigned int length)
{
    diag_ind_info_s ind = {0};
    PS_LOG_FILL_SECOND_HEADER(&ind, PS_LOG_CMD_ID(msg_id, PS_LOG_TYPE_STRUCT, PS_LOG_MODE_COMM, PS_LOG_GID_PS),
        length, PS_LOG_MODEM_INVALID, PS_LOG_OPEN_LEVEL_3, 0);
    ind.data = (unsigned char *)data;

    /* drv接口中最后一个参数是表示头和数据内容的长度，不是第三个指针地址的长度 */
    mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind, length + DIAG_IND_INFO_HEAD_LEN);
}

/*
 * 功能描述: 上报层间消息失败记录
 */
static void ps_log_rpt_layer_fail_record(void)
{
    PS_LOG_TRANS_STRUCT(ps_log_layer_rpt_fail_info);
    PS_LOG_TRANS_STRUCT_NAME(ps_log_layer_rpt_fail_info) info = { 0 };
    info.head.snd_pid = PS_PID_LOGP;
    info.trans = g_layer_rpt_fail_record;
    ps_log_rpt_trans(ID_PS_LOG_ACORE_LAYER_RPT_FAIL_RECORD, &info, sizeof(info));
}

/*
 * 功能描述: 上报层间消息失败记录
 */
static void ps_log_rpt_trace_fail_record(void)
{
    PS_LOG_TRANS_STRUCT(ps_log_trace_rpt_fail_info);
    PS_LOG_TRANS_STRUCT_NAME(ps_log_trace_rpt_fail_info) info = { 0 };
    info.head.snd_pid = PS_PID_LOGP;
    info.trans = g_trace_rpt_fail_record;
    ps_log_rpt_trans(ID_PS_LOG_ACORE_TRACE_RPT_FAIL_RECORD, &info, sizeof(info));
}

/*
 * 功能描述: 上报层间消息失败记录
 */
static void ps_log_rpt_reply_fail_record(void)
{
    PS_LOG_TRANS_STRUCT(ps_log_reply_rpt_fail_info);
    PS_LOG_TRANS_STRUCT_NAME(ps_log_reply_rpt_fail_info) info = { 0 };
    info.head.snd_pid = PS_PID_LOGP;
    info.trans = g_reply_rpt_fail_record;
    ps_log_rpt_trans(ID_PS_LOG_ACORE_REPLY_RPT_FAIL_RECORD, &info, sizeof(info));
}

bool ps_log_is_need_rpt_fail_record(unsigned int oldTimestamp, unsigned int currTimeStamp)
{
    unsigned int timeLen = (currTimeStamp >= oldTimestamp) ? (currTimeStamp - oldTimestamp) : (0xFFFFFFFF - oldTimestamp + currTimeStamp);
    if (timeLen >= PS_LOG_RPT_FAIL_RECORD_TIME_STAMP_LEN) {
        return true;
    }

    return false;
}

/*
 * 功能描述: 层间上报失败处理
 */
void ps_log_record_layer_rpt_fail(unsigned short snd_pid, unsigned short rcv_pid, unsigned short msg_id, short ret)
{
    struct ps_log_layer_rpt_fail_record *record = NULL;
    unsigned int idx = g_layer_rpt_fail_record.num % PS_LOG_LAYER_RPT_FAIL_MAX_NUM;
    g_layer_rpt_fail_record.num++;

    record = &g_layer_rpt_fail_record.record[idx];
    record->snd_pid = snd_pid;
    record->rcv_pid = rcv_pid;
    record->ret = ret;
    record->msg_id = msg_id;
}

/*
 * 功能描述: 在shell口打印层间上报失败处理记录
 */
static void ps_log_print_layer_rpt_fail_record(void)
{
    unsigned int i;
    struct ps_log_layer_rpt_fail_record *record = NULL;
    PS_LOG_PRINT_E("Layer Rpt Fail Record: total num = %d\n", g_layer_rpt_fail_record.num);
    for (i = 0; i < PS_LOG_LAYER_RPT_FAIL_MAX_NUM; i++) {
        record = &g_layer_rpt_fail_record.record[i];
        PS_LOG_PRINT_E("sndPid = %d, rcvPid = %d, msgId = %d, ret = %#hx \n", record->snd_pid, record->rcv_pid, record->msg_id, record->ret);
    }
}

/*
 * 功能描述: trace上报失败处理
 */
void ps_log_record_trace_rpt_fail(unsigned short snd_pid, unsigned short msg_id, unsigned short msg_type, short ret)
{
    struct ps_log_rpt_fail_record *record = NULL;
    unsigned int idx = g_trace_rpt_fail_record.num % PS_LOG_TRACE_RPT_FAIL_MAX_NUM;
    g_trace_rpt_fail_record.num++;

    record = &g_trace_rpt_fail_record.record[idx];
    record->snd_pid = snd_pid;
    record->msg_id = msg_id;
    record->ret = ret;
    record->msg_type = msg_type;
}

/*
 * 功能描述: 在shell口打印trace上报失败处理记录
 */
static void ps_log_print_trace_rpt_fail_record(void)
{
    unsigned int i;
    struct ps_log_rpt_fail_record *record = NULL;
    PS_LOG_PRINT_E("Trace Rpt Fail Record: total num = %d\n", g_trace_rpt_fail_record.num);
    for (i = 0; i < PS_LOG_TRACE_RPT_FAIL_MAX_NUM; i++) {
        record = &g_trace_rpt_fail_record.record[i];
        PS_LOG_PRINT_E("sndPid = %d, msgId = %d, msgType = %d, ret = %#hx \n", record->snd_pid, record->msg_id, record->msg_type, record->ret);
    }
}

/*
 * 功能描述: reply上报失败处理
 */
void ps_log_record_reply_rpt_fail(unsigned short snd_pid, unsigned short msg_id, unsigned short msg_type, short ret)
{
    struct ps_log_rpt_fail_record *record = NULL;
    unsigned int idx = g_reply_rpt_fail_record.num % PS_LOG_REPLY_RPT_FAIL_MAX_NUM;
    g_reply_rpt_fail_record.num++;

    record = &g_reply_rpt_fail_record.record[idx];
    record->snd_pid = snd_pid;
    record->msg_id = msg_id;
    record->ret = ret;
    record->msg_type = msg_type;
}

/*
 * 功能描述: 在shell口打印reply上报失败处理记录
 */
static void ps_log_print_reply_rpt_fail_record(void)
{
    unsigned int i;
    struct ps_log_rpt_fail_record *record = NULL;
    PS_LOG_PRINT_E("Reply Rpt Fail Record: total num = %d\n", g_reply_rpt_fail_record.num);
    for (i = 0; i < PS_LOG_REPLY_RPT_FAIL_MAX_NUM; i++) {
        record = &g_reply_rpt_fail_record.record[i];
        PS_LOG_PRINT_E("sndPid = %d, msgId = %d, msgType = %d, ret = %#hx \n", record->snd_pid, record->msg_id, record->msg_type, record->ret);
    }
}

void ps_log_print_rpt_fail_record(enum ps_log_rpt_fail_type type)
{
    switch (type) {
        case PS_LOG_RPT_FAIL_RECORD_TYPE_LAYER:
            ps_log_print_layer_rpt_fail_record();
            return;

        case PS_LOG_RPT_FAIL_RECORD_TYPE_TRACE:
            ps_log_print_trace_rpt_fail_record();
            return;

        case PS_LOG_RPT_FAIL_RECORD_TYPE_REPLY:
            ps_log_print_reply_rpt_fail_record();
            return;

        default:
            PS_LOG_PRINT_E("Input para err: \n");
            return;
    }
}

void ps_log_proc_layer_rpt_fail_record(void)
{
    unsigned int oldTimeStamp = g_layer_rpt_fail_record.timeStamp;
    unsigned int curTimeStamp = mdrv_timer_get_normal_timestamp();

    if (!ps_log_is_need_rpt_fail_record(oldTimeStamp, curTimeStamp)) {
        return;
    }
    ps_log_rpt_layer_fail_record();
    g_layer_rpt_fail_record.timeStamp = curTimeStamp;
}

void ps_log_proc_trace_rpt_fail_record(void)
{
    unsigned int oldTimeStamp = g_trace_rpt_fail_record.timeStamp;
    unsigned int curTimeStamp = mdrv_timer_get_normal_timestamp();

    if (!ps_log_is_need_rpt_fail_record(oldTimeStamp, curTimeStamp)) {
        return;
    }
    ps_log_rpt_trace_fail_record();
    g_trace_rpt_fail_record.timeStamp = curTimeStamp;
}

void ps_log_proc_reply_rpt_fail_record(void)
{
    unsigned int oldTimeStamp = g_reply_rpt_fail_record.timeStamp;
    unsigned int curTimeStamp = mdrv_timer_get_normal_timestamp();

    if (!ps_log_is_need_rpt_fail_record(oldTimeStamp, curTimeStamp)) {
        return;
    }
    ps_log_rpt_reply_fail_record();
    g_reply_rpt_fail_record.timeStamp = curTimeStamp;
}