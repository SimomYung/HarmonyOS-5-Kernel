/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
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
#include <bsp_syscache.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include "sc_gid_ctrl.h"
#include "sc_hal.h"
#include "sc.h"

#define THIS_MODU mod_syscache
#define SC_FLOW_START_MASK   0x4000319
#define SC_FLOW_END_MASK     0x318
#define SC_FLOW_GID_SHIFT    10
#define SC_FLOW_GID_MASK     (0xffff << FLOW_GID_SHIFT)
#define SC_FLOW_ENABLE_MASK  BIT(26)
#define SC_CMO_IRQ_CLEAR     0x1f000
#define SC_DUMP_MEM_OFFSET 0x800

void sc_hal_set_bypass(struct sc *sc_ctx, int gid, unsigned int bypass)
{
    unsigned int val;

    val = readl(sc_ctx->regs + GLB_ROUTE_PLC_S);
    if (bypass) {
        writel(val | BIT(gid), sc_ctx->regs + GLB_ROUTE_PLC_S);
    } else {
        writel(val & (~BIT(gid)), sc_ctx->regs + GLB_ROUTE_PLC_S);
    }
}
EXPORT_SYMBOL(sc_hal_set_bypass);

unsigned int sc_hal_get_bypass(struct sc *sc_ctx, int slice)
{
    return readl((void *)(sc_ctx->regs + SC_GID_STATE(slice)));
}
EXPORT_SYMBOL(sc_hal_get_bypass);

unsigned int sc_hal_get_cache_size(struct sc *sc_ctx, unsigned int gid)
{
    unsigned int i;
    unsigned int size = 0;

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        size += readl(sc_ctx->regs + SLC_GID_COUNT(i, gid));
    }

    return size;
}
EXPORT_SYMBOL(sc_hal_get_cache_size);

void sc_hal_cfg_cmo(struct sc *sc_ctx, unsigned long long value)
{
    writeq(value, (sc_ctx->regs + CMO_CMD_L));
}

void sc_hal_clear_cmo_intr(struct sc *sc_ctx, unsigned int idx, unsigned int value)
{
    writel(value, (sc_ctx->regs + CFG_CMO_CLEAR(idx)));
}

unsigned int sc_hal_get_cmo_stat(struct sc *sc_ctx, unsigned int idx)
{
    return readl(sc_ctx->regs + CFG_CMO_STATUS(idx));
}

void sc_hal_start_flow(struct sc *sc_ctx, unsigned int gid, int bypass)
{
    unsigned int reg;
    unsigned int i;

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        reg = readl(sc_ctx->regs + GLB_FLOW_STAT(i));
        if (reg & SC_FLOW_ENABLE_MASK) {
            continue;
        }

        reg = SC_FLOW_START_MASK | BIT(SC_FLOW_GID_SHIFT + gid);
        writel(reg, (sc_ctx->regs + GLB_FLOW_STAT(i)));
    }
}

void sc_hal_end_flow(struct sc *sc_ctx, unsigned int id)
{
    unsigned int i;

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        writel(SC_FLOW_END_MASK, sc_ctx->regs + GLB_FLOW_STAT(i));
    }
}

unsigned long long sc_hal_get_flow(struct sc *sc_ctx, unsigned int dir)
{
    unsigned long long flow_cnt = 0;
    unsigned int i;

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        if (dir) {
            flow_cnt += readl(sc_ctx->regs + GLB_FLOW_STATS_IN(i));
        } else {
            flow_cnt += readl(sc_ctx->regs + GLB_FLOW_STATS_OUT(i));
        }
    }

    return flow_cnt;
}
EXPORT_SYMBOL(sc_hal_get_flow);

void sc_hal_irq_enable_fc(struct sc *sc_ctx)
{
    writel(SC_FC_IRQ_MASK, sc_ctx->regs + CMO_CFG_INT_EN);
}

void sc_hal_irq_enable_dfx(struct sc *sc_ctx)
{
    unsigned int i;

    writel(SC_FC_IRQ_MASK, sc_ctx->regs + CMO_CFG_INT_EN);
    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        writel(SC_DFX_IRQ_MASK, sc_ctx->regs + SLC_INT_EN(i));
    }
}

void sc_hal_irq_disable_fc(struct sc *sc_ctx)
{
    writel(0, sc_ctx->regs + CMO_CFG_INT_EN);
}

void sc_hal_irq_disable_dfx(struct sc *sc_ctx)
{
    unsigned int i;

    writel(0, sc_ctx->regs + CMO_CFG_INT_EN);
    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        writel(0, sc_ctx->regs + SLC_INT_EN(i));
    }
}

unsigned int sc_hal_get_dfx_cmo_irq(struct sc *sc_ctx)
{
    return readl(sc_ctx->regs + CMO_CFG_INT_INI);
}
EXPORT_SYMBOL(sc_hal_get_dfx_cmo_irq);

void sc_hal_clr_dfx_cmo_irq(struct sc *sc_ctx)
{
    writel(SC_CMO_IRQ_CLEAR, sc_ctx->regs + CMO_CFG_INT_CLR);
}

unsigned int sc_hal_get_dfx_irq(struct sc *sc_ctx, unsigned int slice)
{
    return readl(sc_ctx->regs + SLC_INT_INI(slice));
}

void sc_hal_clr_dfx_irq(struct sc *sc_ctx, unsigned int slice)
{
    writel(SC_CMO_IRQ_CLEAR, sc_ctx->regs + SLC_INT_CLEAR(slice));
}

unsigned int sc_hal_get_ecc_err(struct sc *sc_ctx, unsigned int slice)
{
    return readl(sc_ctx->regs + SLC_DFX_ECCERR(slice));
}

struct sc_reg_dump {
    unsigned int start;
    unsigned int end;
};

struct sc_reg_dump g_sc_reg_dump[] = {
        {SLC_DFX_MODULE(0), SLC_DFX_MEM1(0)}, //0x74
        {SLC_DFX_MODULE(1), SLC_DFX_MEM1(1)}, //0x74
        {SLC_INT_INI(0), SLC_INT_INI(0)}, //0x4
        {SLC_INT_INI(1), SLC_INT_INI(1)}, //0x4
        {SLC_DFX_ECCERR(0), SLC_DFX_ECCERR(0)}, //0x4
        {SLC_DFX_ECCERR(1), SLC_DFX_ECCERR(1)}, //0x4
        {CMO_CFG_INT_INI, CMO_CFG_INT_INI}, //0x4
    };

void sc_dump_reg_hook(struct sc *sc_ctx)
{
    unsigned int i, j;
    struct gid_ctrl *gid_ctrl = sc_get_gid_ctx();
    unsigned int *reg = (unsigned int *)((char *)gid_ctrl->dump_base + SC_DUMP_MEM_OFFSET); //2K offset
    unsigned int cnt = sizeof(g_sc_reg_dump) / sizeof(struct sc_reg_dump);

    for (i = 0; i < cnt; i++) {
        for (j = g_sc_reg_dump[i].start; j <= g_sc_reg_dump[i].end; j += sizeof(unsigned int)) {
            *reg = readl(sc_ctx->regs + j);
            reg++;
        }
    }
}
