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
#include <securec.h>
#include <osl_types.h>
#include <msg_id.h>
#include <mdrv_diag.h>
#include <bsp_slice.h>
#include <bsp_diag.h>
#include <bsp_diag_cmdid.h>
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

unsigned int g_diag_debug_info[DIAG_TOOL_TYPE_BUTT];
unsigned int g_diag_usb_send_start_slice[SCM_DST_CHAN_BUTT] = {0};
diag_dbg_info_s g_diag_dbg = {0};

void diag_reset_debug_info(diag_tool_type_e type)
{
    return;
}

void diag_dbg_api_show(void)
{
    diag_dbg_info_s *dbg = &g_diag_dbg;

    diag_err("api_dbg cnf_rpt_cnt:%d\n", dbg->api_dbg.cnf_rpt_cnt);
    diag_err("api_dbg cnf_rpt_fail_para_err:%d\n", dbg->api_dbg.cnf_rpt_fail_para_err);
    diag_err("api_dbg cnf_rpt_coder_cnt:%d\n", dbg->api_dbg.cnf_rpt_coder_cnt);
    diag_err("api_dbg cnf_rpt_nocoder_cnt:%d\n", dbg->api_dbg.cnf_rpt_nocoder_cnt);
    diag_err("api_dbg ind_rpt_cnt:%d\n", dbg->api_dbg.ind_rpt_cnt);
    diag_err("api_dbg ind_rpt_fail_para_err:%d\n", dbg->api_dbg.ind_rpt_fail_para_err);
    diag_err("api_dbg ind_realtime_rpt_cnt:%d\n", dbg->api_dbg.ind_realtime_rpt_cnt);
    diag_err("api_dbg ind_parse_in_ue_cnt:%d\n", dbg->api_dbg.ind_parse_in_ue_cnt);
    diag_err("api_dbg ind_oam_cnt:%d\n", dbg->api_dbg.ind_oam_cnt);
    diag_err("api_dbg mdrv_print_cnt:%d\n", dbg->api_dbg.mdrv_print_cnt);
    diag_err("api_dbg mdrv_print_fail_disconn:%d\n", dbg->api_dbg.mdrv_print_fail_disconn);
    diag_err("api_dbg mdrv_print_fail_vsnprintf_fail:%d\n", dbg->api_dbg.mdrv_print_fail_vsnprintf_fail);
    diag_err("api_dbg mdrv_print_oam_cnt:%d\n", dbg->api_dbg.mdrv_print_oam_cnt);
    diag_err("api_dbg bsp_ind_rpt_cnt:%d\n", dbg->api_dbg.bsp_ind_rpt_cnt);
    diag_err("api_dbg bsp_ind_rpt_fail_disconn:%d\n", dbg->api_dbg.bsp_ind_rpt_fail_disconn);
    diag_err("api_dbg bsp_ind_rpt_ok_cnt:%d\n", dbg->api_dbg.bsp_ind_rpt_ok_cnt);
    diag_err("api_dbg bsp_trans_cnt:%d\n", dbg->api_dbg.bsp_trans_cnt);
    diag_err("api_dbg bsp_trans_fail_para_err:%d\n", dbg->api_dbg.bsp_trans_fail_para_err);
    diag_err("api_dbg bsp_trans_fail_disconn_switchoff:%d\n", dbg->api_dbg.bsp_trans_fail_disconn_switchoff);
    diag_err("api_dbg bsp_trans_fail_alloc_fail:%d\n", dbg->api_dbg.bsp_trans_fail_alloc_fail);
    diag_err("api_dbg bsp_trans_fail_cpy_data_fail:%d\n", dbg->api_dbg.bsp_trans_fail_cpy_data_fail);
    diag_err("api_dbg bsp_trans_ind_rpt_fail:%d\n", dbg->api_dbg.bsp_trans_ind_rpt_fail);
    diag_err("api_dbg bsp_trans_ind_rpt_succ:%d\n", dbg->api_dbg.bsp_trans_ind_rpt_succ);
    diag_err("api_dbg bsp_print_cnt:%d\n", dbg->api_dbg.bsp_print_cnt);
    diag_err("api_dbg bsp_print_fail_disconn_level:%d\n", dbg->api_dbg.bsp_print_fail_disconn_level);
    diag_err("api_dbg bsp_print_fail_vsnprintf_fail:%d\n", dbg->api_dbg.bsp_print_fail_vsnprintf_fail);
    diag_err("api_dbg bsp_print_ok_cnt:%d\n", dbg->api_dbg.bsp_print_ok_cnt);
}

void diag_dbg_mntn_src_show(void)
{
    diag_dbg_info_s *dbg = &g_diag_dbg;
    int i;

    for (i = 0; i < SCM_SRC_CHAN_BUTT; i++) {
        diag_err("mntn_data scm_src_dbg[%d].scm_cnt:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_cnt);
        diag_err("mntn_data scm_src_dbg[%d].scm_new_oam_hdr_fail:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_new_oam_hdr_fail);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_full_pkt_fail:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_full_pkt_fail);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_full_pkt_succ:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_full_pkt_succ);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_first_seg_fail:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_first_seg_fail);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_first_seg_succ:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_first_seg_succ);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_other_seg_fail:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_other_seg_fail);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_other_seg_succ:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_other_seg_succ);
        diag_err("mntn_data scm_src_dbg[%d].scm_no_space:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_no_space);
        diag_err("mntn_data scm_src_dbg[%d].scm_send_len:%d\n", i, dbg->mntn_data.scm_src_dbg[i].scm_send_len);
    }
}

void diag_dbg_mntn_dst_show(void)
{
    diag_dbg_info_s *dbg = &g_diag_dbg;
    int i;

    for (i = 0; i < SCM_DST_CHAN_BUTT; i++) {
        diag_err("mntn_data scm_dst_dbg[%d].odt_send_len:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_send_len);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_times:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_times);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_len:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_len);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_buff_err_times:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_type_times[ODT_BUFF_ERR]);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_buff_err_len:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_type_len[ODT_BUFF_ERR]);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_ppm_send_err_times:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_type_times[PPM_SEND_FAIL]);
        diag_err("mntn_data scm_dst_dbg[%d].odt_lost_ppm_send_err_len:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_lost_type_len[PPM_SEND_FAIL]);
        diag_err("mntn_data scm_dst_dbg[%d].odt_send_complete_len:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_send_complete_len);
        diag_err("mntn_data scm_dst_dbg[%d].odt_send_to_finish_delta:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_send_to_finish_delta);
        diag_err("mntn_data scm_dst_dbg[%d].odt_buff_over_80_percent:%d\n", i, dbg->mntn_data.scm_dst_dbg[i].odt_buff_over_80_percent);
    }
}

void diag_dbg_mntn_show(void)
{
    diag_dbg_info_s *dbg = &g_diag_dbg;
    int i;

    diag_err("mntn_data msglite_recv_cnt:%d\n", dbg->mntn_data.msglite_recv_cnt);
    diag_err("mntn_data msglite_cnf_nocoder_send:%d\n", dbg->mntn_data.msglite_cnf_nocoder_send);
    diag_err("mntn_data msglite_dispatch_fail:%d\n", dbg->mntn_data.msglite_dispatch_fail);
    diag_err("mntn_data msglite_dispatch_cnt:%d\n", dbg->mntn_data.msglite_dispatch_cnt);
    diag_err("mntn_data msglite_send_cnt:%d\n", dbg->mntn_data.msglite_send_cnt);

    diag_dbg_mntn_src_show();
    diag_dbg_mntn_dst_show();
    for (i = 0; i < USB_PORT_CHAN_BUTT; i++) {
        diag_err("mntn_data usb_dbg[%d].usb_send_expected_len:%d\n", i, dbg->mntn_data.usb_dbg[i].usb_send_expected_len);
        diag_err("mntn_data usb_dbg[%d].usb_send_len:%d\n", i, dbg->mntn_data.usb_dbg[i].usb_send_len);
        diag_err("mntn_data usb_dbg[%d].usb_send_fail_disconn_len:%d\n", i, dbg->mntn_data.usb_dbg[i].usb_send_fail_disconn_len);
        diag_err("mntn_data usb_dbg[%d].usb_send_fail_len:%d\n", i, dbg->mntn_data.usb_dbg[i].usb_send_fail_len);
        diag_err("mntn_data usb_dbg[%d].usb_send_complete:%d\n", i, dbg->mntn_data.usb_dbg[i].usb_send_complete);
    }
    for (i = 0; i < SOCK_PORT_CHAN_BUTT; i++) {
        diag_err("mntn_data sock_dbg[%d].sock_send_len:%d\n", i, dbg->mntn_data.sock_dbg[i].sock_send_len);
    }
    for (i = 0; i < GENL_PORT_CHAN_BUTT; i++) {
        diag_err("mntn_data genl_dbg[%d].genl_send_len:%d\n", i, dbg->mntn_data.genl_dbg[i].genl_send_len);
        diag_err("mntn_data genl_dbg[%d].unicast_fail:%d\n", i, dbg->mntn_data.genl_dbg[i].unicast_fail);
        diag_err("mntn_data genl_dbg[%d].unicast_errno:%d\n", i, dbg->mntn_data.genl_dbg[i].unicast_errno);
        diag_err("mntn_data genl_dbg[%d].memcpy_fail:%d\n", i, dbg->mntn_data.genl_dbg[i].memcpy_fail);
        diag_err("mntn_data genl_dbg[%d].genlmsg_put_fail:%d\n", i, dbg->mntn_data.genl_dbg[i].genlmsg_put_fail);
        diag_err("mntn_data genl_dbg[%d].genlmsg_new_fail:%d\n", i, dbg->mntn_data.genl_dbg[i].genlmsg_new_fail);
        diag_err("mntn_data genl_dbg[%d].net_is_null:%d\n", i, dbg->mntn_data.genl_dbg[i].net_is_null);
    }
}

void diag_dbg_applog_show(void)
{
    diag_err("applog_dbg write entry: 0x%x\n", g_diag_dbg.applog_dbg.write_entry);
    diag_err("applog_dbg switch off: 0x%x\n", g_diag_dbg.applog_dbg.switch_off);
    diag_err("applog_dbg invalid parameter: 0x%d\n", g_diag_dbg.applog_dbg.invalid_para);
    diag_err("applog_dbg copy_head_err: 0x%d\n", g_diag_dbg.applog_dbg.copy_head_err);
    diag_err("applog_dbg len err: 0x%x\n", g_diag_dbg.applog_dbg.len_err);
    diag_err("applog_dbg copy_data_err: 0x%d\n", g_diag_dbg.applog_dbg.copy_data_err);
    diag_err("applog_dbg data report fail: 0x%x\n", g_diag_dbg.applog_dbg.report_fail);
    diag_err("applog_dbg data send ok: 0x%x\n", g_diag_dbg.applog_dbg.send_ok);
}

void diag_dbg_show(void)
{
    diag_dbg_info_s *dbg = &g_diag_dbg;

    diag_dbg_api_show();
    diag_dbg_mntn_show();

    diag_dbg_applog_show();

    diag_err("diag_msp_proc_dbg.genl_dl_cnf_nocoder_cnt:%d\n", dbg->diag_msp_proc_dbg.genl_dl_cnf_nocoder_cnt);
    diag_err("diag_msp_proc_dbg.usb_dl_cnf_coder_cnt:%d\n", dbg->diag_msp_proc_dbg.usb_dl_cnf_coder_cnt);
    diag_err("diag_msp_proc_dbg.sock_dl_cnf_coder_cnt:%d\n", dbg->diag_msp_proc_dbg.sock_dl_cnf_coder_cnt);
    diag_err("diag_msp_proc_dbg.dl_cnf_coder_decode_fail:%d\n", dbg->diag_msp_proc_dbg.dl_cnf_coder_decode_fail);
    diag_err("diag_msp_proc_dbg.dl_cnf_coder_up_task:%d\n", dbg->diag_msp_proc_dbg.dl_cnf_coder_up_task);
    diag_err("diag_msp_proc_dbg.dl_cnf_coder_task_begin:%d\n", dbg->diag_msp_proc_dbg.dl_cnf_coder_task_begin);
    diag_err("diag_msp_proc_dbg.dl_cnf_coder_task_buff_empty:%d\n", dbg->diag_msp_proc_dbg.dl_cnf_coder_task_buff_empty);
    diag_err("diag_msp_proc_dbg.dl_msglite_cnt:%d\n", dbg->diag_msp_proc_dbg.dl_msglite_cnt);
    diag_err("diag_msp_proc_dbg.port_switch_ptr:%d\n", dbg->diag_msp_proc_dbg.port_switch_ptr);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
