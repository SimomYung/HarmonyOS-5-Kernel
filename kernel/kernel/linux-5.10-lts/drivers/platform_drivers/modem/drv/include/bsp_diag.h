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
#ifndef __BSP_DIAG_H__
#define __BSP_DIAG_H__

#include <product_config.h>
#include <stdarg.h>
#include <bsp_omp.h>
#include <osl_types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(push)
#pragma pack(4)

#define DIAG_OAM_HEAD_LEN sizeof(diag_oam_head_s)
#define DIAG_MSG_HEAD_LEN sizeof(diag_msg_head_s)
#define DIAG_COMMON_HEAD_LEN (DIAG_OAM_HEAD_LEN + DIAG_MSG_HEAD_LEN)
#define DIAG_TOOL_HEAD_LEN sizeof(diag_tool_head_s)
#define DIAG_DRV_IND_HEAD_LEN (sizeof(diag_drv_ind_head_s))
#define DIAG_PRINT_HEAD_LEN sizeof(diag_print_head_s)

#define DIAG_DRV_HDS_PID (0x8003)

/*
DRV 消息ID描述：
bit31 ~ bit28 : group_id (BSP = 0x9)
bit27 ~ bit24 : mode_id (MODE_COMMON=0xf)
bit23 ~ bit19 : msg_type (print=0x3,trans=0x7,CHR=0x13)
bit18 ~ bit13 : field_id（领域ID，如OM、PM、数传等）
bit12 ~ bit7  : module_id（模块/子领域ID，领域内部定义）
bit6  ~ bit0  : msg_id（模块内部ID，模块内部定义）
*/
#define DIAG_DRV_GEN_CMD_ID(msg_type, field_id, module_id, msg_id) \
    (((unsigned int)DIAG_MSG_TYPE_BSP << 28) | ((unsigned int)(DIAG_MODE_COMM & 0xf) << 24) \
     | ((unsigned int)((msg_type) & 0x1f) << 19) | ((unsigned int)((field_id) & 0x3f) << 13) \
     | ((unsigned int)((module_id) & 0x3f) << 7) | ((unsigned int)((msg_id) & 0x7f)))

#define DIAG_GET_GROUP_FROM_CMD_ID(cmd_id) (((cmd_id) & 0xf0000000) >> 28)
#define DIAG_GET_MODE_FROM_CMD_ID(cmd_id) (((cmd_id) & 0xF000000) >> 24)
#define DIAG_GET_MSG_TYPE_FROM_CMD_ID(cmd_id) (((cmd_id) & 0xF80000) >> 19)
#define DIAG_GET_FIELD_FROM_CMD_ID(cmd_id) (((cmd_id) & 0x7E000) >> 13)
#define DIAG_GET_MODULE_ID_FROM_CMD_ID(cmd_id) ((cmd_id) & 0x1F80)
#define DIAG_GET_MSG_ID_FROM_CMD_ID(cmd_id) ((cmd_id) & 0x7F)

#define DIAG_GET_MODULE_FROM_CMD_ID(cmd_id) ((cmd_id) & 0xFFFFFF80)

#define DIAG_GEN_PRINT_LEVEL(level) (0x8000 >> (level))

/* DRV Field ID */
typedef enum {
    DIAG_DRV_FIELD_START = 0,
    DIAG_DRV_FIELD_OM = 1,
    DIAG_DRV_FIELD_NET = 2,
    DIAG_DRV_FIELD_PM = 3,
    DIAG_DRV_FIELD_BUTT = 64, /* 新增FIELD ID枚举要加在DIAG_DRV_FIELD_BUTT之前，并且不可以其他模块之前插入 */
} diag_drv_field_e;

/* DRV Module ID */
typedef enum {
    DIAG_DRV_MODULE_START = 0,
    DIAG_DRV_MODULE_DIAG = 1,
    DIAG_DRV_MODULE_DS = 2,
    DIAG_DRV_MODULE_SCI = 3,
    DIAG_DRV_MODULE_PRINT = 4,
    DIAG_DRV_MODULE_SYSVIEW = 5,
    DIAG_DRV_MODULE_MEMORY = 6,
    DIAG_DRV_MODULE_MPERF = 7,
    DIAG_DRV_MODULE_MON = 8,
    DIAG_DRV_MODULE_TRACE = 9,
    DIAG_DRV_MODULE_BUTT = 64,
} diag_drv_module_e;

typedef enum {
    DIAG_CHR_REPORT_NO_DIRECT = 0,
    DIAG_CHR_REPORT_DIRECT = 1,
    DIAG_CHR_REPORT_BUTT
} diag_chr_subtype_e;

typedef struct {
    unsigned int sid : 4;
    unsigned int ver_id : 4;
    unsigned int modem_id : 3;
    unsigned int rsv1 : 5;
    unsigned int ssid : 8;
    unsigned int mt : 2;
    unsigned int index : 4;
    unsigned int eof : 1;
    unsigned int ff : 1;
    
    unsigned int trans_id;
    unsigned int timestamp;
    
    unsigned int cmd_id;
    unsigned int msg_len;
    
    unsigned char data[0];
} diag_frame_head_s;

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

    unsigned char data[0];
} diag_oam_head_s;

typedef struct {
    unsigned int cmd_id;
    unsigned int msg_len : 16;
    unsigned int modem_id : 3;
    unsigned int level : 3;
    unsigned int reserved : 2;
    unsigned int msg_ver : 8;

    unsigned char data[0];
} diag_msg_head_s;

typedef struct {
    unsigned int auid;
    unsigned short sn;
    unsigned char rsv;
    unsigned char port_type;
    
    unsigned char data[0];
} diag_tool_head_s;

typedef struct {
    unsigned short mid;
    unsigned short resv;

    unsigned char data[0];
} diag_drv_ind_head_s;

typedef struct {
    unsigned short pid;
    unsigned short level;

    char data[0];
} diag_print_head_s;

typedef struct {
    unsigned short send_pid;
    unsigned char sub_type;
    unsigned char is_need_hids_log : 1;
    unsigned char resv : 7;
    unsigned int timestamp;
    unsigned char data[0];
} diag_chr_data_s;

typedef void (*diag_drv_cfg_fn)(unsigned char *diag_drv_frame);

void bsp_diag_drv_cfg_fn_register(unsigned int module_id, diag_drv_cfg_fn cfg_proc_fn);

int bsp_diag_print_report(unsigned int level, const char *fmt, ...);
#define bsp_diag_drv_log_report(level, fmt, ...)          \
    do {                                                  \
        bsp_diag_print_report(level, fmt, ##__VA_ARGS__); \
    } while (0)

int bsp_diag_trans_report(unsigned int msg_id, unsigned int pid, unsigned char *data, unsigned short length);
int bsp_diag_ind_report(unsigned int ind_type, unsigned int msg_id, diag_drv_ind_head_s *drv_data, unsigned short length);

int bsp_diag_mxt_report(unsigned int cmd_id, unsigned char *data, unsigned int data_len);

// 打桩待实现

// END 打桩待实现

#pragma pack(pop)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
