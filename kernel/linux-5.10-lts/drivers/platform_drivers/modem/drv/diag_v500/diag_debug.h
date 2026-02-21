/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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
#ifndef __DIAG_DEBUG_H__
#define __DIAG_DEBUG_H__

#include <osl_types.h>
#include <linux/printk.h>
#include <bsp_slice.h>
#include <bsp_print.h>
#include "diag_msp_msg.h"
#include "scm_src.h"
#include "scm_dst.h"
#include "usb_port.h"
#include "sock_port.h"
#include "genl_port.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#undef THIS_MODU
#define THIS_MODU mod_diag

enum {
    DIAG_STATE_INIT_ERR = 1,

    DIAG_INVALID_LOGPARAM_ERR,
    DIAG_PRINTLOG_NOT_OPEN_ERR,
    DIAG_TRANSLOG_NOT_OPEN_ERR,
    DIAG_TRACELOG_NOT_OPEN_ERR,

    DIAG_PACKMSG_OVERFLOW_ERR,

    DIAG_PARSEMSG_DATA_ERR,
    DIAG_PARSEMSG_GET_MATCHED_MSG_ERR,
    DIAG_PARSEMSG_GET_INVALID_MSG_ERR,
    DIAG_PARSEMSG_MERGE_MSGSLICE_ERR,
    DIAG_PARSEMSG_MERGE_MSGLEN_ERR,

    DIAG_MSG_REGISTER_WAKELOCK_ERR,

    DIAG_ENCODE_INVALID_SRCHTYPE_ERR,
    DIAG_ENCODE_GET_SRCBUF_ERR,
    DIAG_ENCODE_OVERFLOW_SRCBUF_ERR,
    DIAG_ENCODE_GET_ODTPDEV_ERR,
    DIAG_ENCODE_DMA_MALLOC_ERR,
    DIAG_ENCODE_GET_DSTCH_ERR,
    DIAG_ENCODE_GET_DSTBUF_ERR,

    DIAG_SRVPORT_NOTCONNECT_ERR,
    DIAG_SRVPORT_ALLOC_CHAN_ERR,
    DIAG_SRVPORT_GET_POINT_ERR,
    DIAG_SRVPORT_PORT_TYPE_ERR,
    DIAG_SRVPORT_SETLOGMODE_ERR,
    DIAG_SRVPORT_NETLINK_DATA_ERR,
    DIAG_SRVPORT_NETLINK_CMD_ERR,
    DIAG_SRVPORT_NLDATA_MALLOC_ERR,
    DIAG_SRVPORT_MEMCPY_ERR,
    DIAG_SRVPORT_NLWALELOCK_INIT_ERR,

    DIAG_TRANSFERTASK_CREATE_ERR,
    DIAG_TRANSFERTASK_MALLOC_ERR,

    DIAG_PPM_CHAN_TYPE_ERR,
    DIAG_PPM_CHAN_STATE_ERR,
};

#define diag_err(fmt, ...)   bsp_err(" <%s> " fmt, __FUNCTION__, ##__VA_ARGS__)
#define diag_crit(fmt, ...)  bsp_err(fmt, ##__VA_ARGS__)
#define diag_debug(fmt, ...)

// 接口维测应满足关系：
// cnf_rpt_cnt = cnf_rpt_fail_para_err + cnf_rpt_nocoder_cnt + cnf_rpt_coder_cnt
// ind_rpt_cnt = ind_rpt_fail_para_err + ind_realtime_rpt_cnt + ind_parse_in_ue_cnt + ind_oam_cnt
// mdrv_print_cnt = mdrv_print_fail_disconn + mdrv_print_fail_vsnprintf_fail + mdrv_print_oam_cnt
// bsp_ind_rpt_cnt = bsp_ind_rpt_fail_disconn + bsp_ind_rpt_ok_cnt
// bsp_trans_cnt = bsp_trans_fail_para_err + bsp_trans_fail_disconn_switchoff + bsp_trans_fail_alloc_fail 
//    + bsp_trans_fail_cpy_data_fail + bsp_trans_ind_rpt_fail + bsp_trans_ind_rpt_succ
// bsp_print_cnt = bsp_print_fail_disconn_level + bsp_print_fail_vsnprintf_fail + bsp_print_ok_cnt
typedef struct {
    // 上行接口计数
    unsigned int cnf_rpt_cnt;
    unsigned int cnf_rpt_fail_para_err;
    unsigned int cnf_rpt_coder_cnt;
    unsigned int cnf_rpt_nocoder_cnt;
    unsigned int ind_rpt_cnt;
    unsigned int ind_rpt_fail_para_err;
    unsigned int ind_realtime_rpt_cnt;
    unsigned int ind_parse_in_ue_cnt;
    unsigned int ind_oam_cnt;
    unsigned int mdrv_print_cnt;
    unsigned int mdrv_print_fail_disconn;
    unsigned int mdrv_print_fail_vsnprintf_fail;
    unsigned int mdrv_print_oam_cnt;
    unsigned int bsp_ind_rpt_cnt;
    unsigned int bsp_ind_rpt_fail_disconn;
    unsigned int bsp_ind_rpt_ok_cnt;
    unsigned int bsp_trans_cnt;
    unsigned int bsp_trans_fail_para_err;
    unsigned int bsp_trans_fail_disconn_switchoff;
    unsigned int bsp_trans_fail_alloc_fail;
    unsigned int bsp_trans_fail_cpy_data_fail;
    unsigned int bsp_trans_ind_rpt_fail;
    unsigned int bsp_trans_ind_rpt_succ;
    unsigned int bsp_print_cnt;
    unsigned int bsp_print_fail_disconn_level;
    unsigned int bsp_print_fail_vsnprintf_fail;
    unsigned int bsp_print_ok_cnt;
} diag_dbg_api_info_s;

typedef struct {
    unsigned int scm_cnt;
    unsigned int scm_new_oam_hdr_fail;
    unsigned int scm_send_full_pkt_fail;
    unsigned int scm_send_full_pkt_succ;
    unsigned int scm_send_first_seg_fail;
    unsigned int scm_send_first_seg_succ;
    unsigned int scm_send_other_seg_fail;
    unsigned int scm_send_other_seg_succ;
    unsigned int scm_no_space;
    unsigned int scm_send_len; // 统计该通道完整包+分片包一共写入odt_src_chn数据量，用于维测buff空满情况
} diag_dbg_scm_src_info_s;

enum {
    ODT_BUFF_ERR,
    PPM_SEND_FAIL,
    ODT_LOST_TYPE_BUTT,
};

typedef struct {
    unsigned int odt_send_len;
    unsigned int odt_lost_times;
    unsigned int odt_lost_len;
    unsigned int odt_lost_type_times[ODT_LOST_TYPE_BUTT];
    unsigned int odt_lost_type_len[ODT_LOST_TYPE_BUTT];
    unsigned int odt_send_complete_len;
    unsigned int odt_send_to_finish_delta;
    unsigned int odt_buff_over_80_percent;
} diag_dbg_scm_dst_info_s;

typedef struct {
    unsigned int usb_send_expected_len;
    unsigned int usb_send_len;
    unsigned int usb_send_fail_disconn_len;
    unsigned int usb_send_fail_len;
    unsigned int usb_send_complete;
} diag_dbg_usb_info_s;

typedef struct {
    unsigned int sock_send_len;
} diag_dbg_sock_info_s;

typedef struct {
    unsigned int genl_send_len;
    unsigned int unicast_fail;
    unsigned int unicast_errno;
    unsigned int memcpy_fail;
    unsigned int genlmsg_put_fail;
    unsigned int genlmsg_new_fail;
    unsigned int net_is_null;
} diag_dbg_genl_info_s;

typedef struct {
    // msglite通道维测计数
    unsigned int msglite_recv_cnt;
    unsigned int msglite_cnf_nocoder_send;
    unsigned int msglite_dispatch_fail;
    unsigned int msglite_dispatch_cnt;
    unsigned int msglite_send_cnt;

    // scm通道维测计数
    diag_dbg_scm_src_info_s scm_src_dbg[SCM_SRC_CHAN_BUTT];
    diag_dbg_scm_dst_info_s scm_dst_dbg[SCM_DST_CHAN_BUTT];

    // usb通道维测计数
    diag_dbg_usb_info_s usb_dbg[USB_PORT_CHAN_BUTT];

    // socket通道维测计数
    diag_dbg_sock_info_s sock_dbg[SOCK_PORT_CHAN_BUTT];

    // genl通道维测计数
    diag_dbg_genl_info_s genl_dbg[GENL_PORT_CHAN_BUTT];
} diag_dbg_mntn_info_s;

#define PORT_SWITCH_LOG_SIZE 16
typedef struct {
    unsigned int org_port;
    unsigned int cmd_port;
    unsigned int after_switch_port;
    unsigned int timestamp;
} port_switch_info_s;

typedef struct {
    // 下行
    unsigned int genl_dl_cnf_nocoder_cnt;
    unsigned int usb_dl_cnf_coder_cnt;
    unsigned int sock_dl_cnf_coder_cnt;
    unsigned int dl_cnf_coder_decode_fail;
    unsigned int dl_cnf_coder_up_task;
    unsigned int dl_cnf_coder_task_begin;
    unsigned int dl_cnf_coder_task_buff_empty;
    unsigned int dl_msglite_cnt;

    // 内部处理流程维测
    unsigned int port_switch_ptr;
    port_switch_info_s port_switch_info[PORT_SWITCH_LOG_SIZE];
} diag_dbg_msp_proc_info_s;

#define CMD_TRACE_SIZE 128
#define CMD_TRACE_PAYLOAD_SIZE 16
typedef enum {
    CMD_TRACE_BUTT,
} cmd_trace_map;

typedef struct {
    unsigned int cmd_id;
    unsigned int timestamp;
    unsigned int trace_map;
    unsigned int payload_valid;
    unsigned int payload_len;
    unsigned char data[0];
} diag_dbg_cmd_trace_s;

typedef struct {
    unsigned int write_entry;
    unsigned int switch_off;
    unsigned int invalid_para;
    unsigned int copy_head_err;
    unsigned int len_err;
    unsigned int copy_data_err;
    unsigned int report_fail;
    unsigned int send_ok;
} diag_dbg_applog_s;

// 本结构体与diag_dbg_info_s首部保持一致，用于mntn周期上报声明指针指向g_diag_dbg首部
typedef struct {
    unsigned int trans_hdr;
    diag_dbg_api_info_s api_dbg;
    diag_dbg_mntn_info_s mntn_data;
    unsigned int delta_time;
} diag_dbg_mntn_update_s;

typedef struct {
    unsigned int trans_hdr;
    diag_dbg_api_info_s api_dbg; // 维测diag对外接口调用与返回情况，周期上报，每次清0
    diag_dbg_mntn_info_s mntn_data; // 维测diag内部处理计数，定位丢包问题，周期上报，每次清0
    unsigned int delta_time;
    diag_dbg_msp_proc_info_s diag_msp_proc_dbg; // 维测diag msp层处理过程，用于定位需要diag执行的流程是否正常
    unsigned int cmd_trace_payload_switch; // 默认0：关闭；1：打开时，每个cmd取数据段（三级头开始）CMD_TRACE_PAYLOAD_SIZE
    diag_dbg_cmd_trace_s diag_dbg_cmd_trace[CMD_TRACE_SIZE];
    diag_dbg_applog_s applog_dbg;
} diag_dbg_info_s;
extern diag_dbg_info_s g_diag_dbg;

static inline void diag_dbg_cmd_trace(unsigned int cmd_id, unsigned int map)
{
    return;
}

static inline void diag_dbg_dst_lost(unsigned int chn, unsigned int type, unsigned int len)
{
    g_diag_dbg.mntn_data.scm_dst_dbg[chn].odt_lost_times++;
    g_diag_dbg.mntn_data.scm_dst_dbg[chn].odt_lost_len += len;
    g_diag_dbg.mntn_data.scm_dst_dbg[chn].odt_lost_type_times[type]++;
    g_diag_dbg.mntn_data.scm_dst_dbg[chn].odt_lost_type_len[type] += len;
}

// 仅用于usb端口发送时间统计
extern unsigned int g_diag_usb_send_start_slice[SCM_DST_CHAN_BUTT];
static inline void diag_dbg_send_usb_start(unsigned int chn)
{
    g_diag_usb_send_start_slice[chn] = bsp_get_slice_value();
}

// 仅用于usb端口发送时间统计,与diag_system_debug_send_usb_start配合使用
static inline void diag_dbg_send_finish(unsigned int chn)
{
    unsigned int curent_slice;
    unsigned int start_slice;
    unsigned int delta;

    curent_slice = bsp_get_slice_value();
    start_slice = g_diag_usb_send_start_slice[chn];

    delta = get_timer_slice_delta(start_slice, curent_slice);
    g_diag_dbg.mntn_data.scm_dst_dbg[chn].odt_send_to_finish_delta += delta;
}

static inline void diag_dbg_port_switch_log(unsigned int org_port, unsigned int cmd_port, unsigned int after_switch_port)
{
    unsigned int ptr = g_diag_dbg.diag_msp_proc_dbg.port_switch_ptr;
    g_diag_dbg.diag_msp_proc_dbg.port_switch_info[ptr].org_port = org_port;
    g_diag_dbg.diag_msp_proc_dbg.port_switch_info[ptr].cmd_port = cmd_port;
    g_diag_dbg.diag_msp_proc_dbg.port_switch_info[ptr].after_switch_port = after_switch_port;
    g_diag_dbg.diag_msp_proc_dbg.port_switch_info[ptr].timestamp = bsp_get_slice_value();
    ptr++;
    g_diag_dbg.diag_msp_proc_dbg.port_switch_ptr = (ptr >= PORT_SWITCH_LOG_SIZE ? 0 : ptr);
}

void diag_reset_debug_info(diag_tool_type_e type);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
