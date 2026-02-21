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
 */

#include <linux/module.h>
#include <bsp_mprobe.h>
#include "bsp_mperf.h"
#include "msg_id.h"
#include "bsp_msg.h"
#include "bsp_slice.h"
#include "drv_comm.h"
#include "bsp_print.h"
#include "product_config.h"
#include "bsp_diag.h"
#include "mdrv_diag.h"

#undef THIS_MODU
#define THIS_MODU mod_mperf
int mperf_msg_send(void *data, u32 len);
#define mperf_err(fmt, ...) (bsp_err("<%s> " fmt, __FUNCTION__, ##__VA_ARGS__))

int mperf_clk_sync_send(void)
{
    u32 data[5] = {0};
    u64 localtime = trace_clock();
    u64 boottime = ktime_get_boot_fast_ns();
    u32 slice = bsp_get_slice_value();
    u64 dfx_time = dfx_getcurtime();
    data[0] = MPERF_MESSAGE_TYPE_TIME_SYNC;
    data[1] = slice;
    data[2] = dfx_time;
    data[3] = dfx_time >> 32;
    mperf_msg_send(data, sizeof(data));
    mperf_err("time local boot slice:%lld,%lld,%d, %lld", localtime, boottime, slice, dfx_time);
    return 0;
}

EXPORT_SYMBOL(mperf_clk_sync_send);

static msg_chn_t g_mperf_msghdl; 

int mperf_msg_send(void *data, u32 len)
{
    s32 ret;
    struct msg_addr dst;

    dst.core = MSG_CORE_TSP;
    dst.chnid = MSG_CHN_MPERF;

    ret = bsp_msg_lite_sendto(g_mperf_msghdl, &dst, data, len);
    if (ret) {
        mperf_err("msg send fail,ret=0x%x\n", ret);
        return ret;
    }

    return ret;
}

int mperf_message_proc(const struct msg_addr *src, void *buf, u32 len)
{
    if (src == NULL || buf == NULL) {
        return BSP_ERROR;
    }

    if (src->core) {
        mperf_err("unknown msg, src core = 0x%x\n", src->core);
    }

    return BSP_OK;
}

int mperf_msg_task_init(void)
{
    s32 ret;
    struct msgchn_attr lite_attr = { 0 };

    bsp_msgchn_attr_init(&lite_attr);

    lite_attr.chnid = MSG_CHN_MPERF;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    lite_attr.lite_notify = mperf_message_proc;

    ret = bsp_msg_lite_open(&g_mperf_msghdl, &lite_attr);
    if (ret) {
        mperf_err("register msg mid fail, ret=%x\n", ret);
        return ret;
    }

    return ret;
}

static void mperf_mprobe_func_reg(unsigned char *data)
{
    diag_frame_head_s *head = (diag_frame_head_s *)data;

    if (head == NULL) {
        return;
    }
    switch (DIAG_GET_MSG_ID_FROM_CMD_ID(head->cmd_id)) {
        case MPERF_MESSAGE_TYPE_TIME_SYNC:
            (void)mperf_clk_sync_send();
            break;
        default:
            break;
    }
    return;
}

static void mperf_mprobe_callback(unsigned int state)
{
    return;
}

void mperf_mprobe_init(void)
{
    unsigned int cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_OM, DIAG_DRV_MODULE_MPERF, 0); // 0x9FA02380
    bsp_diag_drv_cfg_fn_register(cmd_id, mperf_mprobe_func_reg);
    mdrv_diag_conn_state_notify_fun_reg(mperf_mprobe_callback);
    if (mperf_msg_task_init()) {
        mperf_err("mperf_mprobe_init err.\n");
        return;
    }
    mperf_err("mperf_mprobe_init ok.\n");
    return;
}

int bsp_mperf_init(void)
{
    mperf_mprobe_init();
    return BSP_OK;
}


#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(bsp_mperf_init); /*lint !e528*/
#endif

