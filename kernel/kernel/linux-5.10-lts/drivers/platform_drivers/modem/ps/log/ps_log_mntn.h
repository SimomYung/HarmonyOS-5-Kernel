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

#ifndef PS_LOG_MNTN_H
#define PS_LOG_MNTN_H

#include <linux/kernel.h>
#include "mdrv_msg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PS_PID_LOGP 454

#define PS_LOG_LAYER_RPT_FAIL_MAX_NUM 100
#define PS_LOG_TRACE_RPT_FAIL_MAX_NUM 100
#define PS_LOG_REPLY_RPT_FAIL_MAX_NUM 40
#define PS_LOG_RPT_FAIL_RECORD_TIME_STAMP_LEN (5 * 32768) /* 5秒 */

#define PS_LOG_ACORE_HOOK_LAYER_ID_BEGIN  0x1000 /* A核PS维测平台上报层间消息ID开始 */

#define PS_LOG_ACORE_TRANS_ID_BEGIN  29900 /* A核PS维测平台trans消息ID定义开始 */
#define PS_LOG_ACORE_TRANS_ID_END    29999 /* A核PS维测平台trans消息ID定义结束 */

#define PS_LOG_PRINT_E(fmt, ...) pr_err("[PS_LOG]: [ERR] <%s> "fmt"\n", __func__, ##__VA_ARGS__)

enum ps_log_trans_id {
    ID_PS_LOG_ACORE_LAYER_RPT_FAIL_RECORD             = PS_LOG_ACORE_TRANS_ID_BEGIN + 1, /* 层间消息失败记录 */
    ID_PS_LOG_ACORE_TRACE_RPT_FAIL_RECORD             = PS_LOG_ACORE_TRANS_ID_BEGIN + 2, /* trace上报失败勾包 */
    ID_PS_LOG_ACORE_REPLY_RPT_FAIL_RECORD             = PS_LOG_ACORE_TRANS_ID_BEGIN + 3, /* reply 上报失败勾包 */

    /* A核最后一个trans勾包 */
    ID_PS_LOG_ACORE_HOOK_TRANS_BUTT = PS_LOG_ACORE_TRANS_ID_END,
};

/*
 * 结构说明  : 上报失败类型
 */
enum ps_log_rpt_fail_type {
    PS_LOG_RPT_FAIL_RECORD_TYPE_LAYER = 0, /* 层间上报失败 */
    PS_LOG_RPT_FAIL_RECORD_TYPE_TRACE = 1, /* TRACE上报失败 */
    PS_LOG_RPT_FAIL_RECORD_TYPE_REPLY = 2, /* 回复上报失败 */

    PS_LOG_RPT_FAIL_RECORD_BUTT
};

/*
 * 功能说明: 层间消息记录信息
 */
struct ps_log_layer_rpt_fail_record {
    unsigned short snd_pid;
    unsigned short rcv_pid;
    unsigned short msg_id;
    short ret;
};

struct ps_log_layer_rpt_fail_info {
    unsigned int num; /* 当前统计的上报条数 */
    unsigned int timeStamp;
    struct ps_log_layer_rpt_fail_record record[PS_LOG_LAYER_RPT_FAIL_MAX_NUM];
};

/*
 * 功能说明: trace消息记录信息
 */
struct ps_log_rpt_fail_record {
    unsigned short snd_pid;
    unsigned short msg_id;
    unsigned short msg_type;
    short ret;
};

struct ps_log_trace_rpt_fail_info {
    unsigned int num; /* 当前统计的上报条数 */
    unsigned int timeStamp;
    struct ps_log_rpt_fail_record record[PS_LOG_TRACE_RPT_FAIL_MAX_NUM];
};

struct ps_log_reply_rpt_fail_info {
    unsigned short num; /* 当前统计的上报条数 */
    unsigned int timeStamp;
    struct ps_log_rpt_fail_record record[PS_LOG_REPLY_RPT_FAIL_MAX_NUM];
};

/*
 * 描述: 调用DRV接口上报维测失败勾包
 */
struct ps_log_hook_rpt_fail_record_info {
    MDRV_MSG_HEAD
    unsigned int msgId;
    unsigned char data[0];
};

void ps_log_record_layer_rpt_fail(unsigned short snd_pid, unsigned short rcv_pid, unsigned short msg_id, short ret);
void ps_log_record_trace_rpt_fail(unsigned short snd_pid, unsigned short msg_id, unsigned short msg_type, short ret);
void ps_log_record_reply_rpt_fail(unsigned short snd_pid, unsigned short msg_id, unsigned short msg_type, short ret);
void ps_log_print_rpt_fail_record(enum ps_log_rpt_fail_type type);
void ps_log_proc_layer_rpt_fail_record(void);
void ps_log_proc_trace_rpt_fail_record(void);
void ps_log_proc_reply_rpt_fail_record(void);

#ifdef __cplusplus
}
#endif
#endif
