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

#include <bsp_dt.h>
#include <bsp_fiq.h>
#include <bsp_slice.h>
#include <bsp_sysctrl.h>
#include <bsp_print.h>
#include <osl_bio.h>
#include <osl_types.h>
#include <osl_irq.h>
#include <securec.h>
#include <linux/delay.h>

#undef THIS_MODU
#define THIS_MODU mod_nmi

#define DPA_CORE_NUM 4
#define DPA_DSS_NUM 2
#define DPA_NMI_MAX 8  // MAX interrupt sources per thread
#define DPA_CORE_NUM_PER_REG (32 / DPA_NMI_MAX)
#define DPA_NMI_CHECK_TIME 2000U

#define nmi_err(fmt, ...) (bsp_err(fmt, ##__VA_ARGS__))

struct dpa_nmi_ctrl {
    u32 nmi_init;
    u32 core_mask;
    u32 recv_count[DPA_NMI_MAX];
    u32 send_count[DPA_NMI_MAX];
    void *base_sysctrl;
    u32 nmi_sysctrl[DPA_DSS_NUM][3];  // en, dis, status
};
struct dpa_nmi_ctrl g_dpa_nmi_ctrl;
#define IS_DPA_NMI_SYS_VALID(reg_id, idx) (g_dpa_nmi_ctrl.nmi_sysctrl[reg_id][idx] > 0)

int check_dpa_nmi_status(dpa_nmi_num nmi_id)
{
    u32 reg_id;
    u32 nmi_status[DPA_DSS_NUM] = { 0, 0 };
    u32 res = 0;
    u32 slice = bsp_get_slice_value(); // has triggered nmi a moment ago.
    udelay(DPA_NMI_CHECK_TIME);

    for (reg_id = 0; reg_id < DPA_DSS_NUM; reg_id++) {
        if (IS_DPA_NMI_SYS_VALID(reg_id, 2)) {
            nmi_status[reg_id] = readl((void *)(g_dpa_nmi_ctrl.base_sysctrl + g_dpa_nmi_ctrl.nmi_sysctrl[reg_id][0x2]));
            res += nmi_status[reg_id];
        }
        if (nmi_status[reg_id] != 0) {
            nmi_err("[dpa_nmi]id %d, dss_id: %u, send_cnt[%u], recv_cnt[%u], error, status:0x%x\n", nmi_id, reg_id,
                g_dpa_nmi_ctrl.send_count[nmi_id], g_dpa_nmi_ctrl.recv_count[nmi_id], nmi_status[reg_id]);
        }
    }
    if (res) {
        nmi_err("[dpa_nmi]recv fail, trigger slice: 0x%x\n", slice);
        return BSP_ERROR;
    }
    g_dpa_nmi_ctrl.recv_count[nmi_id]++;
    nmi_err("[dpa_nmi]id%d, send_cnt[%u], recv_cnt[%u], OK, trigger slice: 0x%x\n", nmi_id, g_dpa_nmi_ctrl.send_count[nmi_id],
                g_dpa_nmi_ctrl.recv_count[nmi_id], slice);

    return BSP_OK;
}

int bsp_dpa_nmi_trigger(u32 core_mask, dpa_nmi_num nmi_id)
{
    u32 reg_id = 0;
    u32 reg_core_id = 0;
    u32 core_id = 0;
    u32 tmp = 0;
    u32 trigger_value[DPA_DSS_NUM] = { 0, 0 };

    if (!g_dpa_nmi_ctrl.nmi_init) {
        nmi_err("[dpa_nmi]nmi init fail\n");
        return BSP_ERROR;
    }

    if (nmi_id >= DPA_NMI_MAX) {
        nmi_err("[dpa_nmi]nmi nmi_id %d error\n", nmi_id);
        return BSP_ERROR;
    }

    if ((core_mask & g_dpa_nmi_ctrl.core_mask) != core_mask) {
        nmi_err("[dpa_nmi]nmi core_mask 0x%x error\n", core_mask);
        return BSP_ERROR;
    }
    g_dpa_nmi_ctrl.send_count[nmi_id]++;
    for (core_id = 0; core_id < DPA_CORE_NUM; core_id++) {
        tmp = ((u32)0x1 << (u32)core_id);
        if (tmp & core_mask) {
            reg_core_id = (u32)(core_id % DPA_CORE_NUM_PER_REG);
            reg_id = (u32)(core_id / DPA_CORE_NUM_PER_REG);
            trigger_value[reg_id] |= ((u32)0x1 << (u32)(DPA_NMI_MAX * reg_core_id + nmi_id));
        }
    }
    for (reg_id = 0; reg_id < DPA_DSS_NUM; reg_id++) {
        if (IS_DPA_NMI_SYS_VALID(reg_id, 0) && (trigger_value[reg_id] > 0)) {
            writel(trigger_value[reg_id], (void *)(g_dpa_nmi_ctrl.base_sysctrl + g_dpa_nmi_ctrl.nmi_sysctrl[reg_id][0]));
        }
    }
    return check_dpa_nmi_status(nmi_id);
}

int bsp_send_dpa_nmi(dpa_nmi_num nmi_id)
{
    return bsp_dpa_nmi_trigger(g_dpa_nmi_ctrl.core_mask, nmi_id);
}

int get_dpa_nmi_dts(void)
{
    device_node_s *node = NULL;
    int ret;

    node = bsp_dt_find_compatible_node(NULL, NULL, "dpa,nmi");
    if (node == NULL) {
        nmi_err("[dpa_nmi]can't find nmi node\n");
        return BSP_ERROR;
    }

    ret = bsp_dt_property_read_u32_array(node, "sysctrl_nmi", (u32 *)&g_dpa_nmi_ctrl.nmi_sysctrl,
        sizeof(g_dpa_nmi_ctrl.nmi_sysctrl) / sizeof(u32));
    ret += bsp_dt_property_read_u32(node, "core_mask", &g_dpa_nmi_ctrl.core_mask);
    if (ret) {
        nmi_err("[dpa_nmi]read sysctrl_nmi err\n");
        return BSP_ERROR;
    }
    g_dpa_nmi_ctrl.base_sysctrl = bsp_sysctrl_addr_byindex(SYSCTRL_AP_PERI);
    if (g_dpa_nmi_ctrl.base_sysctrl == NULL) {
        nmi_err("[dpa_nmi]get sysctrl_ap err, sysctrl_flag: %d\n");
        return BSP_ERROR;
    }
    return BSP_OK;
}

int dpa_nmi_init(void)
{
    // global value init
    if (memset_s((void *)&g_dpa_nmi_ctrl, sizeof(struct dpa_nmi_ctrl), 0, sizeof(struct dpa_nmi_ctrl))) {
        nmi_err("[dpa_nmi]g_dpa_nmi_ctrl memset fail\n");
        return BSP_ERROR;
    }

    // read dts config
    if (get_dpa_nmi_dts() != BSP_OK) {
        nmi_err("[dpa_nmi]get nmi dts error\n");
        return BSP_ERROR;
    }

    g_dpa_nmi_ctrl.nmi_init = 1;
    return BSP_OK;
}

EXPORT_SYMBOL(bsp_send_dpa_nmi);

arch_initcall(dpa_nmi_init);

