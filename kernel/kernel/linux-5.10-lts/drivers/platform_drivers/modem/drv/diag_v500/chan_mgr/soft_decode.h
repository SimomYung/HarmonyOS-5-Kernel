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

#ifndef _SOFT_DECODE_H_
#define _SOFT_DECODE_H_

#include <osl_types.h>
#include <osl_spinlock.h>
#include <osl_sem.h>
#include "decode_algorithm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define RING_BUFFER_SIZE (36 * 1024) /* ring_buffer数据接收buffer大小 */
#define HDLC_BUFFER_SIZE (16 * 1024) /* 软解码接收数据包大小 */
#define SOFT_DECODE_TASK_PRIORITY 76
#define SOFT_DECODE_TASK_STACK_SIZE 8192

typedef struct {
    int write_ptr;
    int read_ptr;
    int buf_size;
    char reserved[0x4];
    char *buf;
} om_ring_s;

typedef struct {
    char *buffer;
    char *rb_buffer;
    unsigned int size;
    unsigned int rb_size;
} rw_buffer_s;

typedef struct {
    osl_sem_id sem_id;
    om_ring_s *ring_buffer;
} soft_decode_rcv_ctrl_s;

typedef struct {
    unsigned int data_len; /* 接收或发送数据长度 */
    unsigned int num;      /* 接收或发送数据次数 */
} scm_mantennace_info_s;

typedef struct {
    unsigned int sem_create_err;
    unsigned int sem_give_err;
    unsigned int ringbuff_create_err;
    unsigned int task_id_err;
    unsigned int buff_no_enough;
    unsigned int rbuff_flush;
    unsigned int rbuff_put_err;
} scm_soft_decode_rb_info_s;

typedef struct {
    scm_soft_decode_rb_info_s rb_info;
    scm_mantennace_info_s put_info;
    scm_mantennace_info_s get_info;
    scm_mantennace_info_s hdlc_decap_data;
    unsigned int frame_decap_data;
    unsigned int hdlc_init_err;
    unsigned int data_type_err;
} soft_decode_debug_info_s;

typedef void (*soft_decode_dst_cb)(unsigned int port_type, const unsigned char *data, unsigned int data_len);
typedef struct {
    soft_decode_rcv_ctrl_s rcv_ctrl;        /* soft_decode数据接收任务控制结构 */
    soft_decode_debug_info_s debug_info;      /* soft_decode维测结构 */
    spinlock_t spinlock;                    /* 自旋锁，用来作OM数据接收的临界资源保护 */
    om_hdlc_s hdlc_entity;                  /* HDLC控制结构 */
    soft_decode_dst_cb dst_data_proc_cb;
} soft_decode_info_s;


void diag_coder_proc_register(soft_decode_dst_cb func);

int diag_coder_chan_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
