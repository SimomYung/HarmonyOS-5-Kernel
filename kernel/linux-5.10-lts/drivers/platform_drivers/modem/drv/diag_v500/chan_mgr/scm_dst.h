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
#ifndef __SCM_DST_H__
#define __SCM_DST_H__

#include <linux/of_platform.h>
#include <osl_sem.h>
#include <osl_types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 上溢水线 = 最大包长+1K的裕量 */
#define DIAG_CNF_DST_OVERFLOW_THRESHOLD (5 * 1024)

typedef enum {
    SCM_CNF_DST_CHAN,
    SCM_IND_DST_CHAN,
    SCM_NO_CODER_DST_CHAN,
    SCM_DEBUGGING_DST_CHAN,
    SCM_DST_CHAN_BUTT
} scm_dst_chan_e;

typedef enum {
    SCM_TEE_CMD_CLOSE_MXT_SRC_CHAN,
    SCM_TEEOS_CMD_BUTT
} scm_tee_srv_e;

typedef void (*dst_read_cb)(unsigned char *virt_addr, unsigned char *phy_addr, unsigned int len);
typedef struct {
    unsigned int init_state;
    unsigned int chan_id;
    unsigned int ppm_chan;
    unsigned int buff_len;
    unsigned int threshold;
    unsigned int timeout_mode;
    unsigned char *virt_buff;
    unsigned char *phy_buff;
    dst_read_cb func; /* 编码目的通道数据处理回调函数 */
} scm_dst_cfg_s;

int scm_dst_chan_init(void);
int scm_rls_dst_buffer(unsigned int dst_chan_id, unsigned int data_len);
int scm_dst_data_send_handler(unsigned int dst_chan_id, unsigned int len);
void scm_dst_chan_reset(unsigned int dst_chan_id);

int scm_dst_nocoder_cb(unsigned int chan_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif   /* __SRVCHAN_H__ */
