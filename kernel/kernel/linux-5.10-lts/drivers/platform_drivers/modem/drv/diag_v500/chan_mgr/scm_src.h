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
#ifndef __SCM_SRC_H__
#define __SCM_SRC_H__

#include <linux/of_platform.h>
#include <osl_sem.h>
#include <osl_types.h>
#include "diag_report.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SCM_ALIGN_8_BYTES(len) (((len) + 7) & (~7))
#define DIAG_ODT_HEAD_SIZE sizeof(scm_odt_head_s)

typedef enum {
    SCM_CHANNEL_UNINIT = 0, /* 未初始化 */
    SCM_CHANNEL_INIT_SUCC,  /* 初始化成功 */
    SCM_CHANNEL_MEM_FAIL,   /* 初始化申请内存错误 */
    SCM_CHANNEL_CFG_FAIL,   /* 初始化通道配置 */
    SCM_CHANNEL_START_FAIL, /* 通道开启错误 */
    SCM_CHANNEL_INIT_BUTT
} scm_chan_init_state_e;

typedef enum {
    SCM_CNF_SRC_CHAN,
    SCM_IND_SRC_CHAN,
    SCM_NO_CODER_SRC_CHAN,
    SCM_MXT_SRC_CHAN,
    SCM_SRC_CHAN_BUTT
} scm_src_chan_e;

typedef char* (*get_data_func)(char *msg_hdr);
typedef struct {
    unsigned int init_state;    /* 通道初始化状态，初始化后自动修改 */
    unsigned int chan_id;       /* 编码源通道ID，固定配置 */
    unsigned int dst_chan_id;   /* 编码目的通道ID */
    unsigned int data_type_en;  /* 数据来源类型 */
    unsigned int data_type;     /* 数据来源类型 */
    unsigned int chan_mode;     /* 通道类型 */
    unsigned int chan_prio;     /* 通道优先级 */
    unsigned int bypass_en;     /* 通道非编码是否使能 */
    unsigned int buff_len;      /* 编码源通道数据空间大小 */
    unsigned char *virt_buff;   /* 编码源通道数据空间内存指针 */
    unsigned char *phy_buff;
    spinlock_t src_lock;
    unsigned char *oam_msg_hdr;

    unsigned int msg_mt;
    unsigned int msg_hdr_len;
    unsigned int trans_id;
    get_data_func get_data;
    unsigned int scm_src_chan_id; // diag_scm通道编号，详见scm_src_chan_e，用于反查当前参数属于diag定义的第几个通道
} scm_src_cfg_s;

typedef struct {
    unsigned int odt_magic; // 硬件识别magic
    unsigned int odt_len; // 指示出了odt_位域的数据长度

    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;
} oam_hdr_s;

typedef struct {
    unsigned int odt_magic; // 硬件识别magic
    unsigned int odt_len; // 指示出了odt_位域的数据长度
} scm_odt_head_s;

typedef struct {
    unsigned char *head;
    unsigned int head_size;
    unsigned char *data;
    unsigned int data_size;
} scm_frame_s;

int scm_init(void);

int scm_write_data(scm_src_chan_e chan, unsigned char *msg, unsigned int data_len);

void scm_clear_diag_tool_src_buff(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif   /* __SCM_SRC_H__ */
