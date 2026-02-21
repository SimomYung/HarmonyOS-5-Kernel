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
#ifndef __DIAG_REPORT_H__
#define __DIAG_REPORT_H__

#include <securec.h>
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_malloc.h>
#include <mdrv_diag.h>
#include <bsp_slice.h>
#include "msg_chan.h"
#include "diag_msp_msg.h"
#include "diag_msg_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define DIAG_FRAME_MAX_LEN ((64 * 1024) - (15 * sizeof(diag_frame_cnf_s)))
#define DIAG_CNF_NO_CODER_FRAME_HEAD_LEN (sizeof(diag_cnf_no_coder_frame_s) - sizeof(unsigned char *))
#define DIAG_IND_NO_CODER_FRAME_HEAD_LEN (sizeof(diag_ind_no_coder_frame_s) - sizeof(unsigned char *))
#define DIAG_CNF_CODER_FRAME_HEAD_LEN (sizeof(diag_cnf_odt_frame_s) - sizeof(unsigned char *))
#define DIAG_IND_CODER_FRAME_HEAD_LEN (sizeof(diag_ind_odt_frame_s) - sizeof(unsigned char *))

typedef enum {
    DIAG_CHAN_MSG_CNF,
    DIAG_CHAN_MSG_IND,
    DIAG_CHAN_NO_CODER,
    DIAG_CHAN_CODER_CNF,
    DIAG_CHAN_CODER_IND,
    DIAG_CHAN_BUTT 
} diag_chan_e; /* 这里需要注意对齐的问题，如果增加枚举值，需要同步修改diag_report_info_s的rsv字段 */

typedef struct {
    spinlock_t lock[DIAG_CHAN_BUTT];
    unsigned char trans_id[DIAG_CHAN_BUTT];
    unsigned char rsv1; /* 这里需要注意对齐的问题，如果修改diag_chan_e， 需要同步修改该结构体的rsv字段 */
    unsigned char rsv2;
} diag_report_info_s;

typedef struct {
    unsigned int odt_magic;
    unsigned int odt_len;

    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;
	
	unsigned int cmd_id;
	
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

	unsigned int auid;
	unsigned short sn;
	unsigned char rsv;
	unsigned char port_type;

	unsigned char *data;
} diag_cnf_odt_frame_s;

typedef struct {
    unsigned int auid;
	unsigned short sn;
	unsigned char rsv;
	unsigned char port_type;
} diag_attr_head_s;

typedef struct {
    unsigned int odt_magic;
    unsigned int odt_len;

    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;
	
	unsigned int cmd_id;
	
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

	unsigned char *data;
} diag_ind_odt_frame_s;

typedef struct {
    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;
	
	unsigned int cmd_id;
	
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

	unsigned int auid;
	unsigned short sn;
	unsigned char rsv;
	unsigned char port_type;

    unsigned char *data;
} diag_cnf_no_coder_frame_s;

typedef struct {
    unsigned int sid : 4;
    unsigned int ver : 4;
    unsigned int ssid : 8;
    unsigned int trans_id : 8;
    unsigned int mt : 2;
    unsigned int fragment : 4;
    unsigned int eof_flag : 1;
    unsigned int ff_flag : 1;
    unsigned int timestamp;
	
	unsigned int cmd_id;
	
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

    unsigned char *data;
} diag_ind_no_coder_frame_s;

void diag_report_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
