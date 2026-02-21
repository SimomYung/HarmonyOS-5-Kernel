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

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/types.h>
#include <bsp_print.h>
#include "sc_gid_ctrl_hal.h"
#include "sc_gid_ctrl.h"

#define THIS_MODU mod_syscache
#define SC_GID_CTRL_ENTRY_MAX 64

void sc_gid_ctrl_hal_enabel(struct gid_ctrl *gid_ctx, unsigned int idx, unsigned int enable)
{
    writel(enable, gid_ctx->regs_peri + GID_REGION_EN(idx));
    if (gid_ctx->regs_acpu != NULL) {
        writel(enable, gid_ctx->regs_acpu + GID_REGION_EN(idx));
    }
}
EXPORT_SYMBOL(sc_gid_ctrl_hal_enabel);

void sc_gid_ctrl_hal_addr_cfg(struct gid_ctrl *gid_ctx, unsigned int start_addr, unsigned int end_addr, unsigned int cfg, int idx)
{
    writel_relaxed(start_addr, gid_ctx->regs_peri + START_ADDR(idx));
    writel_relaxed(end_addr, gid_ctx->regs_peri + END_ADDR(idx));
    writel_relaxed(cfg, gid_ctx->regs_peri + GID_CFG(idx));
    if (gid_ctx->regs_acpu != NULL) {
        writel_relaxed(start_addr, gid_ctx->regs_acpu + START_ADDR(idx));
        writel_relaxed(end_addr, gid_ctx->regs_acpu + END_ADDR(idx));
        writel_relaxed(cfg, gid_ctx->regs_acpu + GID_CFG(idx));
    }
    sc_gid_ctrl_hal_enabel(gid_ctx, idx, 1);
}
EXPORT_SYMBOL(sc_gid_ctrl_hal_addr_cfg);

void sc_gid_ctrl_dump_reg(struct gid_ctrl *gid_ctx)
{
    int i;
    unsigned int *reg = gid_ctx->dump_base;

    for (i = 0; i < SC_GID_CTRL_ENTRY_MAX; i++) {
        *reg++ = readl(gid_ctx->regs_peri + START_ADDR(i));
        *reg++ = readl(gid_ctx->regs_peri + END_ADDR(i));
        *reg++ = readl(gid_ctx->regs_peri + GID_CFG(i));
        *reg++ = readl(gid_ctx->regs_peri + GID_REGION_EN(i));
    }

    if (gid_ctx->regs_acpu != NULL) {
        for (i = 0; i < SC_GID_CTRL_ENTRY_MAX; i++) {
            *reg++ = readl(gid_ctx->regs_acpu + START_ADDR(i));
            *reg++ = readl(gid_ctx->regs_acpu + END_ADDR(i));
            *reg++ = readl(gid_ctx->regs_acpu + GID_CFG(i));
            *reg++ = readl(gid_ctx->regs_acpu + GID_REGION_EN(i));
        }
    }
}

void sc_gid_ctrl_show(unsigned int idx)
{
    struct gid_ctrl *gid_ctx = sc_get_gid_ctx();

    if (idx >= SC_GID_CTRL_MAX) {
        bsp_err("idx invalid:%u\n", idx);
        return;
    }

    bsp_err("start_addr:0x%llx\n", gid_ctx->cfg_om[idx].start_addr);
    bsp_err("end_addr  :0x%llx\n", gid_ctx->cfg_om[idx].end_addr);
    bsp_err("size      :0x%x\n", gid_ctx->cfg_om[idx].size);
    bsp_err("cfg       :0x%x\n", gid_ctx->cfg_om[idx].cfg);
    bsp_err("enable    :0x%x\n", gid_ctx->cfg_om[idx].enable);

    bsp_err("gid_ctrl_peri\n");
    bsp_err("start_addr:0x%x\n", readl(gid_ctx->regs_peri + START_ADDR(idx)));
    bsp_err("end_addr  :0x%x\n", readl(gid_ctx->regs_peri + END_ADDR(idx)));
    bsp_err("cfg       :0x%x\n", readl(gid_ctx->regs_peri + GID_CFG(idx)));
    bsp_err("en        :0x%x\n", readl(gid_ctx->regs_peri + GID_REGION_EN(idx)));

    if (gid_ctx->gid_only && gid_ctx->regs_acpu != NULL) {
        bsp_err("gid_ctrl_acpu\n");
        bsp_err("start_addr:0x%x\n", readl(gid_ctx->regs_acpu + START_ADDR(idx)));
        bsp_err("end_addr  :0x%x\n", readl(gid_ctx->regs_acpu + END_ADDR(idx)));
        bsp_err("cfg       :0x%x\n", readl(gid_ctx->regs_acpu + GID_CFG(idx)));
        bsp_err("en        :0x%x\n", readl(gid_ctx->regs_acpu + GID_REGION_EN(idx)));
    }
}

EXPORT_SYMBOL(sc_gid_ctrl_show);
