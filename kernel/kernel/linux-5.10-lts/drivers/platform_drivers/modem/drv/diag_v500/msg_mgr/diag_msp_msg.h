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
#ifndef __DIAG_MSP_MSG_H__
#define __DIAG_MSP_MSG_H__

#include <osl_types.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <osl_spinlock.h>
#include <mdrv_diag.h>
#include <mdrv_timer.h>
#include <bsp_diag.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define DIAGTOOL_CONN_MSG_ID 0x1
#define HMISRV_CONN_MSG_ID 0x2
#define MXT_CONN_MSG_ID 0x3

#define DIAGTOOL_DISCONN_MSG_ID 0x1
#define HMISRV_DISCONN_MSG_ID 0x2
#define MXT_DISCONN_MSG_ID 0x3

// TODO #define DIAG_GET_TOOL_TYPE(diag_frame) ((diag_conn_info_s *)diag_frame->sid)
#define DIAG_GET_TOOL_TYPE(diag_frame) (0)

#define DIAG_GET_DIAG_TOOL_AUTH_STATE(state) ((state) & 0x1)
#define DIAG_GET_HMI_SRV_AUTH_STATE(state) (((state) & 0x2) >> 1)
#define DIAG_GET_MXT_AUTH_STATE(state) (((state) & 0x4) >> 2)

#define DIAG_NOTIFY_HOOK_MAX 64

#define DIAG_CMD_PRINT_CFG          0x9f002200

#define DIAG_TOOL_CONN_PREEMPT_VALUE 1
#define DIAG_TOOL_CONN_RELEASE_VALUE 2

#define DIAG_MODEM_RESET_BEFORE_VALUE 1
#define DIAG_MODEM_RESET_AFTER_VALUE 2

typedef enum {
    DIAG_AUTH_TYPE_DEFAULT = 0,
    DIAG_AUTH_TYPE_AUTHING = 1,
    DIAG_AUTH_TYPE_SUCCESS = 2,
    DIAG_AUTH_TYPE_BUTT
} diag_auth_type_e;

typedef enum {
    DIAGTOOL_CFG_INIT_BIT = 0,
    DIAGTOOL_CFG_TRANS_BIT,
    DIAGTOOL_CFG_APPLOG_BIT,
    DIAGTOOL_CFG_MNTN_BIT,
    DIAGTOOL_CFG_FLOW_TEST_BIT,
    DIAG_CFG_BIT_BUTT
} diag_cfg_bit_e;

typedef enum {
    DIAG_CFG_CLOSE = 0,
    DIAG_CFG_OPEN,
    DIAG_CFG_STATE_BUTT
} diag_cfg_state_e;

typedef enum {
    DIAG_TOOL_TYPE_DIAGTOOL,
    DIAG_TOOL_TYPE_HMISRV,
    DIAG_TOOL_TYPE_MXT,
    DIAG_TOOL_TYPE_BUTT
} diag_tool_type_e;

typedef enum {
    DIAG_TIMER_DEBUG,
    DIAG_TIMER_FLOW,
    DIAG_TIMER_BUTT
} diag_timer_e;

typedef enum {
    TOOL_DISCONN,
    TOOL_LINK,
    TOOL_CONN,
} tool_stat_e;

typedef enum {
    HMI_DISCONN,
    HMI_LINK,
    HMI_CONN,
} hmi_stat_e;

typedef enum {
    LOGCAT_DISCONN,
    LOGCAT_CONN,
} logcat_stat_e;

typedef enum {
    MXT_DISCONN,
    MXT_CONN,
} mxt_stat_e;

typedef struct {
    unsigned int result;
} diag_cfg_result_s;

typedef struct {
    unsigned int swt;
} diag_cfg_switch_s;

typedef struct {
    timer_attr_s tmattr;
    timer_handle tmhandle;
} diag_timer_manager_s;

typedef struct {
    unsigned short ap_en;
    unsigned short period;
    unsigned int rsv; // HIDS开关向前兼容，此处与HIDS结构保持一致
} diag_debug_cfg_s;

typedef struct {
    unsigned int ap_peroid;
    unsigned int tsp_peroid;
    unsigned int ap_en;
    unsigned int tsp_en;
    unsigned int pack_num;
} diag_flow_cfg_s;

typedef struct {
    unsigned char power_on;
    unsigned char cycle;
    unsigned char print_level;
} diag_log_cfg_s;

typedef struct {
    unsigned int swt;
    unsigned int level;
} diag_applog_cfg_s;

typedef struct {
    struct list_head list;
    unsigned int module_id;
    diag_drv_cfg_fn cfg_fn;
} diag_drv_cfg_node_s;

typedef struct {
    struct list_head list;
    spinlock_t spinlock;
    unsigned int init_state;
} diag_drv_cfg_list;

typedef struct {
    unsigned int print_level;
    diag_drv_cfg_list cfg_list;
} diag_drv_msg_info_s;

typedef struct {
    spinlock_t spinlock;
    tool_stat_e tool_stat;
    hmi_stat_e hmi_stat;
    logcat_stat_e logcat_stat;
    mxt_stat_e mxt_stat;
    unsigned int diag_cfg_state[DIAG_TOOL_TYPE_BUTT];
    unsigned int power_on_log;
    unsigned int cycle_on;
    unsigned int applog_level;
    diag_debug_cfg_s debug_cfg;
    mdrv_diag_conn_state_notify_fun nottify_fn[DIAG_NOTIFY_HOOK_MAX];
} diag_msp_msg_info_s;

typedef void (*diag_msp_msg_fn)(diag_frame_req_s *diag_frame);
typedef struct {
    unsigned int cmd_id;
    diag_msp_msg_fn msg_proc_cb;
} diag_msp_msg_proc_s;

bool diag_is_auth_succ(diag_frame_req_s *data, unsigned int port_type);
bool diag_msg_is_conn_cmd(unsigned int cmd_id);
unsigned int diag_get_cfg_switch(unsigned int cfg_type);
unsigned int diag_get_print_level(void);
int diag_trigger_disconn(diag_tool_type_e tool);
void diag_msp_msg_init(void);
int diag_applog_init(void);

void diagtool_conn_msg_proc(void);
void diagtool_disconn_msg_proc(void);
void diagtool_rxready_msg_proc(void);
void hmisrv_conn_msg_proc(void);
void hmisrv_logcat_conn_msg_proc(void);
void hmisrv_logcat_disconn_msg_proc(void);
void hmisrv_disconn_msg_proc(void);
void mxt_conn_msg_proc(void);
void mxt_disconn_msg_proc(void);
void diag_poweron_log_cfg_proc(diag_log_cfg_s *powon_log_cfg, unsigned int data_len);
void diag_msp_msg_proc(unsigned char *msp_data, unsigned int data_len);

void diag_print_cfg_proc(diag_frame_req_s *diag_frame);
void diag_trans_cfg_proc(diag_frame_req_s *diag_frame);
void diag_applog_cfg_proc(diag_frame_req_s *diag_frame);
void diag_debug_cfg_proc(diag_frame_req_s *diag_frame);
void diag_flow_cfg_proc(diag_frame_req_s *diag_frame);

void diag_report_diag_tool_conn_event(unsigned int value);
unsigned int diag_is_power_on_log_open(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif   /* __DIAG_MSG_H__ */
