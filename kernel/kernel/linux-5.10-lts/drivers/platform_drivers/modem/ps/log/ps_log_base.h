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

#ifndef PS_LOG_BASE_H
#define PS_LOG_BASE_H


#ifdef __cplusplus
extern "C" {
#endif

#if (defined(LLT_OS_VER))
#define STATIC
#else
#define STATIC static
#endif

#define PS_LOG_CONSTRUCT_CHR_MSG_ID(pid, msgid) (((msgid) & 0x3FF) | (((pid) & 0x1FF) << 10))

#define PS_LOG_CMD_ID(msgId, msgType, mode, gid) \
    ((((unsigned int)msgId) & 0x7FFFF) \
    | ((((unsigned int)msgType) & 0x1f) << 19) \
    | ((((unsigned int)mode) & 0xF) << 24) \
    | (((unsigned int)gid) << 28 )) \

#define PS_LOG_FILL_SECOND_HEADER(ind, cmdId, msgLen, modemId, levelValue, version) \
do { \
    ((diag_ind_info_s *)(ind))->cmd_id = cmdId; \
    ((diag_ind_info_s *)(ind))->msg_len = msgLen; \
    ((diag_ind_info_s *)(ind))->modem_id = modemId; \
    ((diag_ind_info_s *)(ind))->level = levelValue; \
    ((diag_ind_info_s *)(ind))->msg_ver = version; \
} while (0)

/*
 * 结构体说明：维测消息信令头结构定义
 */
struct ps_log_sig_head {
    unsigned int au_id;
    unsigned short sn;
    unsigned char channel_type;
    unsigned char port_type;
};

#ifdef __cplusplus
}
#endif
#endif
