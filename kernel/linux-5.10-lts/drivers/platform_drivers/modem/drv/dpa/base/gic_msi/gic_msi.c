/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include "gic_msi.h"

struct gic_msi_info g_gic_msi_info = {0};

int bsp_gic_msi_sendto_dpa(enum gic_msi_id id, void *msg, unsigned int len)
{
    unsigned int i;
    void *base_addr = g_gic_msi_info.base_addr;

    if (id >= GIC_MSI_MAX || len != (g_gic_msi_info.msg_record[id].size * 4) || g_gic_msi_info.msg_record[id].uid_sync == 0) {
        bsp_err("gic msi para err\n");
        return -1;
    }

    if (len != 0 && msg == NULL) {
        bsp_err("gic msi msg is NULL\n");
        return -1;
    }

    //写msg
    for (i = 0; i < len; i++) {
        writel(*(unsigned int *)msg, (void *)(uintptr_t)(base_addr + g_gic_msi_info.msg_record[id].start_addr_offset + i * 4));
        msg += sizeof(unsigned int);
    }

    //触发中断
    writel(id, base_addr);

    return 0;
}

static int gic_msi_msg_rcv_proc(const struct msg_addr *src, void *msg, unsigned int len)
{
    unsigned int uid;

    if (len != sizeof(struct gic_msi_msg_info) ) {
        bsp_err("gic msi msg open err, ret=%d\n", len);
        return -1;
    }

    uid = ((struct gic_msi_msg_info_recv *)(uintptr_t)msg)->uid;
    g_gic_msi_info.msg_record[uid].uid_sync = 1;
    g_gic_msi_info.msg_record[uid].start_addr_offset = ((struct gic_msi_msg_info_recv *)(uintptr_t)msg)->start_addr_offset;
    g_gic_msi_info.msg_record[uid].size = ((struct gic_msi_msg_info_recv *)(uintptr_t)msg)->size;

    return 0;
}

void show_gic_msi_msg_info(void)
{
    unsigned int i;

     for (i = 0; i < GIC_MSI_MAX; i++) {
        bsp_err("uid_sync%x\n", g_gic_msi_info.msg_record[i].uid_sync);
        bsp_err("start addr%x\n", g_gic_msi_info.msg_record[i].start_addr_offset);
        bsp_err("size%u\n", g_gic_msi_info.msg_record[i].size);
    }
}
EXPORT_SYMBOL(show_gic_msi_msg_info);

int gic_msi_msg_init(void)
{
    int ret = 0;
    struct msgchn_attr lite_attr = { 0 };

    g_gic_msi_info.base_addr = ioremap(GIC_MSI_BASE_ADDR_PHY, DPA_GIC_MSI_SIZE);

    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_GIC_MSI;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_DPA);
    lite_attr.lite_notify = gic_msi_msg_rcv_proc;
    ret = bsp_msg_lite_open(&g_gic_msi_info.msg_chn_hdl, &lite_attr);
    if (unlikely(ret)) {
        bsp_err("gic msi msg open err, ret=%d\n", ret);
        return ret;
    }

    bsp_err("gic msi init ok\n");

    return ret;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(gic_msi_msg_init);
#endif