/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include <osl_module.h>
#include <osl_malloc.h>
#include <msg_id.h>
#include <bsp_msg.h>
#include <bsp_print.h>
#include "avs_base.h"

#undef THIS_MODU
#define THIS_MODU mod_avs

static msg_chn_t g_avs_msghdl;
avs_msg_cb_s g_avs_msg_cb = { 0 };

int avs_msg_notify_cb(const struct msg_addr *src, void *msg, unsigned int len)
{
    if (msg == NULL || src == NULL || len < sizeof(avs_msg_s)) {
        avs_err("avs check msg err\n");
        return -1;
    }

    if (src->core != MSG_CORE_TSP || src->chnid != MSG_CHN_AVS) {
        avs_err("avs src err\n");
        return -1;
    }

    avs_msg_s *avs_msg = (avs_msg_s *)msg;
    if (avs_msg->avs_msg_type >= AVS_MSG_TYPE_MAX) {
        avs_err("avs msg type err %d\n", avs_msg->avs_msg_type);
        return -1;
    }
    unsigned data_len = avs_msg->data_len;
    char *data = &avs_msg->data[0];
    if (g_avs_msg_cb.msg_cb[avs_msg->avs_msg_type] != NULL) {
        g_avs_msg_cb.msg_cb[avs_msg->avs_msg_type](data, data_len);
    }

    return 0;
}

int avs_val_notify(int avs_msg_type, char *data, unsigned int len)
{
    int ret;
    struct msg_addr dst;
    dst.core = MSG_CORE_TSP;

    dst.chnid = MSG_CHN_AVS;
    unsigned int tmp_size = sizeof(avs_msg_s) + len;
    avs_msg_s *avs_msg = (avs_msg_s *)osl_malloc(tmp_size);
    if (avs_msg == NULL) {
        avs_err("malloc fail\n");
        return -1;
    }
    avs_msg->avs_msg_type = avs_msg_type;
    avs_msg->data_len = len;
    ret = memcpy_s(&avs_msg->data[0], len, data, len);
    if (ret != EOK) {
        avs_err("memcpy fail\n");
        osl_free(avs_msg);
        return -1;
    }
    ret = bsp_msg_lite_sendto(g_avs_msghdl, &dst, (void *)avs_msg, tmp_size);
    if (ret != 0) {
        avs_err("msg send fail\n");
        osl_free(avs_msg);
        return -1;
    }
    osl_free(avs_msg);
    return 0;
}

void avs_msg_cb_register(avs_msg_type_e msg_type, avs_msg_cb ops)
{
    if (msg_type >= AVS_MSG_TYPE_MAX) {
        avs_err("avs msg reg err\n");
        return;
    }
    g_avs_msg_cb.msg_cb[msg_type] = ops;
}

int avs_msg_init(void)
{
    int ret = 0;
    struct msgchn_attr lite_attr = { 0 };
    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_AVS;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    lite_attr.lite_notify = avs_msg_notify_cb;
    ret = bsp_msg_lite_open(&g_avs_msghdl, &lite_attr);
    if (ret) {
        avs_err("avs msg open err,ret=%d\n", ret);
        return ret;
    }
    return ret;
}