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
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include "securec.h"
#include "mdrv_diag.h"
#include "ps_log_ue_interface.h"
#include "ps_log_base.h"
#include "ps_log_mntn.h"

/* 由于三级头由组件填写，则组件回复消息的最小长度是sndpid(VOS_UINT16) + XXX(VOS_UINT16) */
#define PS_LOG_CNF_LENGTH_MIN 4
#define PS_LOG_CNF_SIG_HEADER 8 /* 信令头长度 */

#define PS_LOG_FILL_CNF_SECOND_HEAD(cnf, cmdid, msglen, modemid, sighead, version) \
do { \
    ((diag_cnf_info_s *)(cnf))->cmd_id = cmdid; \
    ((diag_cnf_info_s *)(cnf))->msg_len = msglen; \
    ((diag_cnf_info_s *)(cnf))->modem_id = modemid; \
    ((diag_cnf_info_s *)(cnf))->auid = ((struct ps_log_sig_head *)(sighead))->au_id; \
    ((diag_cnf_info_s *)(cnf))->sn = ((struct ps_log_sig_head *)(sighead))->sn; \
    ((diag_cnf_info_s *)(cnf))->rsv = ((struct ps_log_sig_head *)(sighead))->channel_type; \
    ((diag_cnf_info_s *)(cnf))->port_type = ((struct ps_log_sig_head *)(sighead))->port_type; \
    ((diag_cnf_info_s *)(cnf))->msg_ver = version; \
} while (0)

/**
 * @brief 消息回复上报接口
 * @par 描述: 回复工具或者应用下发的消息
 * @attention 注意事项:
 * 1. 只能用于回复一条消息，平台不做拆分；
 * 2. data数据长度不应超过宏PS_LOG_CNF_LENGTH_MAX 定义的长度，单位Byte，超过长度消息丢弃
 * 3. 由于A核没有CHR，所以这里没有实现CHR的回复，如果后续增加CHR回复，需要调整函数实现
 * @param[in] cnf_head: 回复cnf的头信息，参见struct ps_log_cnf_head
 * @param[in] reply_para: 工具下发的信令头和cmdId，由工具下发时填写，维测平台调用组件的回调函数时传入，组件回复时直接使用
 * @param[in] data: 上报的数据内容指针
 * @param[in] length: 消息长度
 */
void ps_log_reply(const struct ps_log_cnf_head *cnf_head, const struct ps_log_reply_para *reply_para, const void *data, unsigned short length)
{
    diag_cnf_info_s cnf = {0};
    int ret;
    unsigned int msg_id;

    if (cnf_head == NULL) {
        PS_LOG_PRINT_E("cnf_head is null");
        return;
    }

    msg_id = reply_para->cmd_id & 0x7FFFF;
    if ((length > PS_LOG_CNF_LENGTH_MAX)) {
        PS_LOG_PRINT_E("length = %d, sndpid = %d, msgid = %d", length, cnf_head->snd_pid, msg_id);
        return;
    }

    PS_LOG_FILL_CNF_SECOND_HEAD(&cnf, reply_para->cmd_id, length + PS_LOG_CNF_SIG_HEADER, cnf_head->modem_id, &reply_para->sig_head,
        cnf_head->version);
    if ((data != NULL) && (length != 0)) {
        cnf.data = (unsigned char *)data;
    }
    /* drv接口中最后一个参数是表示头和数据内容的长度，不是第一个指针地址的长度 */
    ret = mdrv_diag_cnf_report(&cnf, DIAG_CNF_INFO_HEAD_LEN + length);
    if (ret != 0) {
        ps_log_record_reply_rpt_fail(cnf_head->snd_pid, msg_id, (reply_para->cmd_id) >> 19 & 0x1F , ret);
    }
    ps_log_proc_reply_rpt_fail_record();
}
