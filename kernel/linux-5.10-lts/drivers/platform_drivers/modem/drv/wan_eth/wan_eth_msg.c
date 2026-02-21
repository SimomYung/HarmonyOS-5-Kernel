/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <msg_id.h>
#include <bsp_msg.h>
#include "wan_eth_common.h"
#include "wan_eth_msg.h"
#include "bsp_dpa.h"

#define WETH_MSG_SEND_DATA_MAX_SIZE 128

struct weth_ofl_msg_ctx {
    msg_chn_t hd;
    weth_ofl_msg_proc_cb func[WETH_MSG_CMD_ID_MAX];
};

struct weth_ofl_msg_ctx g_weth_ofl_msg_ctx = { 0 };

static int weth_ofl_msg_rcv_proc(const struct msg_addr *src, void *msg, unsigned int len)
{
    int ret = -1;
    struct weth_msg_info *msg_info = (struct weth_msg_info *)msg;

    if (src == NULL) {
        weth_pr_err("para err\n");
        return -1;
    }

    if (src->chnid != MSG_CHN_WETH_OFL) {
        weth_pr_err("msg chnid err\n");
        return -1;
    }

    if (msg == NULL || len < sizeof(struct weth_msg_info)) {
        weth_pr_err("msg is %s len %d\n", (msg == NULL) ? "null" : "ok", len);
        return -1;
    }

    if (msg_info->cmd_id >= WETH_MSG_CMD_ID_MAX) {
        weth_pr_err("cmd_id err %d\n", msg_info->cmd_id);
        return -1;
    }

    weth_pr_dbg("cmd_id %d\n", msg_info->cmd_id);
    if (g_weth_ofl_msg_ctx.func[msg_info->cmd_id] != NULL) {
        ret = g_weth_ofl_msg_ctx.func[msg_info->cmd_id](msg_info->data, msg_info->len);
    }

    return ret;
}

int weth_ofl_msg_init(void)
{
    int ret = 0;
    struct msgchn_attr lite_attr = { 0 };

    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_WETH_OFL;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_DPA);
    lite_attr.lite_notify = weth_ofl_msg_rcv_proc;
    ret = bsp_msg_lite_open(&g_weth_ofl_msg_ctx.hd, &lite_attr);
    if (unlikely(ret)) {
        weth_pr_err("msg open err, ret=%d\n", ret);
        return ret;
    }

    weth_pr_err("msg init ok\n");
    return ret;
}

int weth_ofl_register_msg_rcv_cb(enum weth_msg_cmd_id cmd_id, weth_ofl_msg_proc_cb func)
{
    if (cmd_id >= WETH_MSG_CMD_ID_MAX) {
        weth_pr_err("msg rcv register err %d\n", cmd_id);
    }

    g_weth_ofl_msg_ctx.func[cmd_id] = func;
    return 0;
}

static int weth_ofl_msg_sendto(void *msg, unsigned len)
{
    int ret;
    struct msg_addr dst;

    dst.core = MSG_CORE_DPA;
    dst.chnid = MSG_CHN_WETH_OFL;

    ret = bsp_msg_sendto_dpa(g_weth_ofl_msg_ctx.hd, &dst, msg, len);
    if (ret) {
        weth_pr_err("send msg err, ret = %d \n", ret);
    }
    return ret;
}

int weth_msg_cmd_sendto(enum weth_msg_cmd_id cmd_id, void *data, unsigned int data_len)
{
    int ret;
    unsigned int buf_len;
    struct weth_msg_info *msg = NULL;

    if (data_len > WETH_MSG_SEND_DATA_MAX_SIZE) {
         weth_pr_err("data_len %d err\n", data_len);
         return -1;
    }

    buf_len = sizeof(struct weth_msg_info) + data_len;
    msg = (struct weth_msg_info *)kzalloc(buf_len, GFP_KERNEL);
    if (msg == NULL) {
        weth_pr_err("msg alloc error\n");
        return -1;
    }

    msg->cmd_id = cmd_id;
    msg->len = data_len;
    ret = memcpy_s(msg->data, WETH_MSG_SEND_DATA_MAX_SIZE, data, data_len);
    if (ret) {
        weth_pr_err("buf_len %d cmd_id %d msg len %d ret %d\n", buf_len, msg->cmd_id, msg->len, ret);
        kfree(msg);
        return ret;
    }

    ret = weth_ofl_msg_sendto(msg, buf_len);
    if (ret) {
        weth_pr_err("buf_len %d cmd_id %d msg len %d ret %d\n", buf_len, msg->cmd_id, msg->len, ret);
    }
    kfree(msg);
    return ret;
}

void weth_ofl_msg_exit(void)
{
    int ret;
    ret = bsp_msg_lite_close(g_weth_ofl_msg_ctx.hd);
    if (ret) {
        weth_pr_err("bsp_msg_lite_close err %d", ret);
    }
    return;
}
