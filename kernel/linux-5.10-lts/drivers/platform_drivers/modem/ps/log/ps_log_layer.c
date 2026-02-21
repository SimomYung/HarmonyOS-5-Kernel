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
#include "ps_log_ue_interface.h"
#include "mdrv_diag.h"
#include "mdrv_msg.h"
#include "ps_log_base.h"
#include "ps_log_mntn.h"

#define PS_LOG_GET_MSG_ID(msg) (((struct ps_log_msg_head *)(msg))->msgId) /* 获取层间消息的msgid */
#define PS_LOG_LAYER_MSG_THIRD_HEAD_LENGTH 4 /* 层间消息三级头长度（sndpid + rcvpid 占4 BYTE） */
#define PS_LOG_MSG_LENGTH_MIN (sizeof(struct ps_log_msg_head)) /* msg头 + msgid的长度 */

struct ps_log_msg_head {
    MDRV_MSG_HEAD
    unsigned int msg_id;
};

/*
 * @brief 层间消息上报接口
 * @par 描述: 供协议栈组件上报层间维测消息
 * @attention 注意事项:
 * 1. data长度不应超过宏PS_LOG_LAYER_MSG_LENGTH_MAX 定义的长度，单位Byte，超过长度消息丢弃
 * @param[in] layer_head: 层间消息上报需要涉及的额外信息
 * @param[in] data: 上报的数据内容指针
 * @param[in] length: data长度
 */
void ps_log_rpt_layer(const struct ps_log_layer_head *layer_head, const void *data, unsigned short length)
{
    diag_ind_info_s ind = {0};
    unsigned int msg_id;
    unsigned int snd_pid;
    unsigned int rcv_pid;
    int ret;
    struct ps_log_msg_head *head;

    /* 参数检查 */
    if ((data == NULL) || (length < PS_LOG_MSG_LENGTH_MIN)) {
        PS_LOG_PRINT_E("data = %p, length = %d", data, length);
        return;
    }

    head = (struct ps_log_msg_head *)data;
    snd_pid = head->src_id;
    rcv_pid = head->dst_id;

    if ((length > PS_LOG_LAYER_MSG_LENGTH_MAX) || (layer_head == NULL)) {
        PS_LOG_PRINT_E("length = %d, layer_head = %p, snd_pid = %d, rcv_pid = %d", length, layer_head, snd_pid, rcv_pid);
        return;
    }

    msg_id = ((struct ps_log_msg_head *)data)->msg_id;
    PS_LOG_FILL_SECOND_HEADER(&ind, PS_LOG_CMD_ID(msg_id, PS_LOG_TYPE_LAYER, layer_head->mode, layer_head->gid),
        length, layer_head->modem_id, layer_head->level, layer_head->version);
    ind.data = (unsigned char *)data;

    /* drv接口中最后一个参数是表示头和数据内容的长度，不是第二个指针地址的长度 */
    ret = mdrv_diag_ind_report(DIAG_COMMON_OAM_TYPE, &ind, length + DIAG_IND_INFO_HEAD_LEN);
    if (ret != 0) {
        ps_log_record_layer_rpt_fail(snd_pid, rcv_pid, msg_id, ret);
    }
    ps_log_proc_layer_rpt_fail_record();
}