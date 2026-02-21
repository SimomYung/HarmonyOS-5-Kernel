/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
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
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <product_config.h>
#include <securec.h>
#include <bsp_msg.h>
#include <msg_id.h>
#include <mdrv_bmi_vsim.h>
#include "vsim_driver.h"

#define THIS_MODU mod_vsim
#ifdef CONFIG_MODEM_VSIM
static event_cb_func g_vsim_auth_request_cb = NULL;
static msg_chn_t g_vsim_msghdl;

int vsim_request_proc(const struct msg_addr *src, void *buf, u32 len)
{
    int ret = 0;
    struct msg_addr dst;
    struct vsim_data_s msg = {0};
    event_cb_func vsim_request_cb = g_vsim_auth_request_cb;
    dst.core = MSG_CORE_TSP;
    dst.chnid = MSG_CHN_VSIM;

    vsim_print_error("vsim request begin\n");
    if (src == NULL || buf == NULL || len != sizeof(struct vsim_data_s)) {
        vsim_print_error("msg paras invalid\n");
        return VSIM_ERROR_ARGS;
    }

    if (src->core != MSG_CORE_TSP || ((struct vsim_data_s *)buf)->event_id != VSIM_AUTH_EVENT) {
        vsim_print_error("vsim proc invalid\n");
        return VSIM_ERROR_ARGS;
    }

    if (vsim_request_cb == NULL) {
        vsim_print_error("event_cb is NULL, request no handler\n");
        ret = VSIM_OK;
    } else {
        ret = vsim_request_cb();
        if (ret) {
            vsim_print_error("event_cb is error, ret = 0x%x\n", ret);
            ret = VSIM_ERROR;
        }
    }

    msg.ret = ret;
    msg.event_id = VSIM_AUTH_EVENT;

    ret = bsp_msg_lite_sendto(g_vsim_msghdl, &dst, &msg, sizeof(struct vsim_data_s));
    if (ret) {
        vsim_print_error("send msg err %d.\n", ret);
        return VSIM_ERROR;
    }

    vsim_print_error("vsim request end\n");
    return VSIM_OK;
}

int mdrv_vsim_auth_request_register(event_cb_func vsim_request_cb)
{
    if (vsim_request_cb == NULL) {
        vsim_print_error("event_cb is NULL\n");
        return VSIM_ERROR_ARGS;
    }

    if (g_vsim_auth_request_cb != NULL) {
        vsim_print_error("event_cb has registed\n");
        return VSIM_ERROR_ARGS;
    }

    g_vsim_auth_request_cb = vsim_request_cb;
    return VSIM_OK;
}

int __init vsim_agent_init(void)
{
    int ret;

    struct msgchn_attr lite_attr = { 0 };
    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_VSIM;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    lite_attr.lite_notify = vsim_request_proc;
    ret = bsp_msg_lite_open(&g_vsim_msghdl, &lite_attr);
    if (ret) {
        vsim_print_error("msg open err %d\n", ret);
        return VSIM_ERROR_ICC_FAIL;
    }

    vsim_print_error("vsim agent init end.\n");
    return VSIM_OK;
}

module_init(vsim_agent_init);

#else

int mdrv_vsim_auth_request_register(event_cb_func vsim_request_cb)
{
    vsim_print_error("vsim send auth request is stub.\n");
    return VSIM_NOT_SUPPORT;
}
#endif

EXPORT_SYMBOL(mdrv_vsim_auth_request_register);
