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
#include <linux/interrupt.h>
#include <linux/sizes.h>
#include <bsp_syscache.h>
#include <bsp_print.h>
#include <bsp_dt.h>
#include "sc.h"

#define THIS_MODU mod_syscache
#define SC_POLL_TIMEOUT_US 1000000
#define SC_CMO_SIZE 2048
#define SC_WAY_USED 0xFFFFFFFF
struct sc g_sc_ctx;
EXPORT_SYMBOL(g_sc_ctx);

#define SC_DEBUG_BUGON BIT(0)
#define SC_DEBUG_ERR BIT(1)
#define SC_BUG_ON()                                                  \
    do {                                                             \
        if (g_sc_ctx.dbg & SC_DEBUG_BUGON) {                         \
            BUG_ON(1);                                               \
        }                                                            \
    } while (0)
#define SC_BUG_ERR(fmt, ...)                                         \
    do {                                                             \
        if (g_sc_ctx.dbg & SC_DEBUG_ERR) {                           \
            bsp_err(fmt, ##__VA_ARGS__);                             \
        }                                                            \
    } while (0)
void sc_set_dbg(unsigned int bit)
{
    g_sc_ctx.dbg |= BIT(bit);
    bsp_err("sc dbg:%u\n", g_sc_ctx.dbg);
}

static int sc_build_cmo_cmd(struct sc_cmo_cfg *cfg, sc_cmo_cmd *cmo_cmd)
{
    int ret = 0;

    cmo_cmd->param.by_way.opt = cfg->ops & CMO_OPERATE_MASK;
    cmo_cmd->param.by_way.cmdt = cfg->cmo_type;
    switch (cfg->cmo_type) {
        case CMO_BY_WAY:
            cmo_cmd->param.by_way.way_bitmap = cfg->way_bitmap & CMO_BITMAP_MASK;
            break;
        case CMO_BY_GID:
            cmo_cmd->param.by_gid.gid_bitmap = cfg->gid_bitmap & CMO_BITMAP_MASK;
            break;
        case CMO_BY_64PA:
            if (!IS_ALIGNED(cfg->phy_addr, SZ_64)) {
                bsp_err("phy_addr is not align 64\n");
                ret = -EINVAL;
            }
            cmo_cmd->param.by_64pa.phy_addr = cfg->phy_addr >> CMO_64B_PA_SHIFT;
            break;
        case CMO_BY_128PA:
            if (!IS_ALIGNED(cfg->phy_addr, SZ_128)) {
                bsp_err("phy_addr is not align 128\n");
                ret = -EINVAL;
            }
            cmo_cmd->param.by_128pa.phy_addr = cfg->phy_addr >> CMO_128B_PA_SHIFT;
            break;
        case CMO_BY_2KPA:
            if (!IS_ALIGNED(cfg->phy_addr, SZ_2K) || !IS_ALIGNED(cfg->phy_addr + cfg->size, SZ_2K)
                || cfg->phy_addr >= cfg->phy_addr + cfg->size) {
                ret = -EINVAL;
            }
            cmo_cmd->param.by_2kpa.start_addr = (cfg->phy_addr >> CMO_2K_PA_SHIFT);
            cmo_cmd->param.by_2kpa.end_addr = ((cfg->phy_addr + cfg->size) >> CMO_2K_PA_SHIFT) + 1;
            break;
        case CMO_BY_WAY_AND_GID:
            cmo_cmd->param.by_way_and_gid.gid_bitmap = cfg->gid_bitmap & CMO_BITMAP_MASK;
            cmo_cmd->param.by_way_and_gid.way_bitmap = cfg->way_bitmap & CMO_BITMAP_MASK;
            break;
        default:
            bsp_err("invalid type %d\n", cfg->cmo_type);
            ret = -EINVAL;
            break;
    }
    return ret;
}

int sc_ops_cache(sc_sync_type sync_type, sc_ops_type ops, sc_cmo_type cmo_type,
    unsigned int gid_bitmap, unsigned int way_bitmap, unsigned long long phy_addr, int size)
{
    int ret;
    sc_cmo_cmd cmo_cmd = {0};
    struct sc_cmo_cfg cfg;

    if ((ops != SC_INV && ops != SC_CLEAN && ops != SC_CLEAN_INV) || cmo_type > CMO_BY_WAY_AND_GID) {
        bsp_err("%d %d invalid para\n", ops, cmo_type);
        return -EINVAL;
    }

    cfg.sync_type = sync_type;
    cfg.ops = ops;
    cfg.cmo_type = cmo_type;
    cfg.gid_bitmap = gid_bitmap;
    cfg.way_bitmap = way_bitmap;
    cfg.phy_addr = phy_addr;
    cfg.size = size;

    ret = sc_build_cmo_cmd(&cfg, &cmo_cmd);
    if (ret) {
        return ret;
    }
    sc_hal_cfg_cmo(&g_sc_ctx, cmo_cmd.value);

    return ret;
}
EXPORT_SYMBOL(sc_ops_cache);

int sc_cmo_sync_cmd_by_event(struct sc *sc_ctx)
{
    unsigned int status;
    int cpu_id;
    ktime_t timeout;
    unsigned long flags;
    sc_cmo_cmd snyc;
    int ret = 0;

    preempt_disable();
    local_irq_save(flags);

    cpu_id = smp_processor_id();
    snyc.value = 0;
    snyc.param.sync.opt = 1;
    snyc.param.sync.cmdt = 1;
    snyc.param.sync.rslt = cpu_id;
    snyc.param.sync.seq = 1;
    sc_hal_cfg_cmo(sc_ctx, snyc.value);

    timeout = ktime_add_us(ktime_get(), SC_POLL_TIMEOUT_US);
    while (!(sc_hal_get_cmo_stat(sc_ctx, cpu_id) & CMO_FINISH_MASK)) {
        if (ktime_compare(ktime_get(), timeout) > 0) {
            SC_BUG_ERR("cmo sync time out\n");
            sc_ctx->timeout++;
            ret = -1;
            break;
        }
        wfe();
    }

    sc_hal_clear_cmo_intr(sc_ctx, cpu_id, CMO_EVENT_INT_CLR_FLAG);
    status = sc_hal_get_cmo_stat(sc_ctx, cpu_id);
    if (status & CMO_FINISH_MASK) {
        SC_BUG_ERR("cmo intr clear fail:%d\n", status);
        sc_ctx->cmo_clr_fail++;
        ret = -1;
    }

    local_irq_restore(flags);
    preempt_enable();

    return ret;
}
EXPORT_SYMBOL(sc_cmo_sync_cmd_by_event);

void bsp_sc_invalid_cache(unsigned long long phy_addr, unsigned int size)
{
    unsigned long flags = 0;
    int ret;
    unsigned long long addr;
    unsigned long long start_addr;
    unsigned long long end_addr;
    struct sc *sc_ctx = &g_sc_ctx;

    start_addr = ALIGN_DOWN(phy_addr, SC_CMO_SIZE);
    end_addr = ALIGN(phy_addr + size, SC_CMO_SIZE);

    // 只有dra内存刷cache 按照2048长度刷提升效率
    spin_lock_irqsave(&sc_ctx->lock, flags);
    for (addr = start_addr; addr < end_addr; addr += SC_CMO_SIZE) {
        ret = sc_ops_cache(SC_CMO_EVENT, SC_CLEAN_INV, CMO_BY_2KPA, 0, 0, addr, SC_CMO_SIZE);
    }
    if (ret) {
        sc_ctx->cmo_cache_err++;
        spin_unlock_irqrestore(&sc_ctx->lock, flags);
        return;
    }
    sc_cmo_sync_cmd_by_event(sc_ctx);
    spin_unlock_irqrestore(&sc_ctx->lock, flags);
}

void sc_cmo_by_gid(unsigned int gid)
{
    int ret;

    ret = sc_ops_cache(SC_CMO_EVENT, SC_CLEAN_INV, CMO_BY_GID, BIT(gid), 0, 0, 0);
    if (!ret) {
        sc_cmo_sync_cmd_by_event(&g_sc_ctx);
    }
}

irqreturn_t sc_irq_thread(int irq, void *dev_id)
{
    struct sc *sc_ctx = (struct sc *)dev_id;
    cmo_irq_stat irq_stat;
    unsigned int i, j;

    if (sc_ctx->cmo_err_irq) {
        irq_stat.val = sc_ctx->cmo_err_irq;
        bsp_err("cmo err irq:%u\n", irq_stat.val);
        SC_BUG_ON();
        sc_ctx->cmo_cmd_acess_err += irq_stat.bits.cmo_cmd_acess_err;
        sc_ctx->cmo_overflow_err += irq_stat.bits.cmo_overflow_err;
        sc_ctx->cmo_pa_err += irq_stat.bits.cmo_pa_err;
        sc_ctx->cmo_byway_err += irq_stat.bits.cmo_byway_err;
        sc_ctx->cmo_bygid_err += irq_stat.bits.cmo_bygid_err;
    }

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        if (sc_ctx->dfx_err_irq[i]) {
            irq_stat.val = sc_ctx->dfx_err_irq[i];
            bsp_err("slice:%d dfx err irq:0x%x ecc err:0x%x\n", i, irq_stat.val, sc_ctx->ecc_err[i]);
            for (j = 0; j < SC_DFX_INT_BIT_CNT; j++) {
                sc_ctx->dfx_cnt[i].cnt[j] += !!(irq_stat.val & BIT(j));
            }
        }
    }

    sc_hal_irq_enable_dfx(sc_ctx);
    return IRQ_HANDLED;
}

irqreturn_t sc_irq_handler(int irq, void *dev_id)
{
    struct sc *sc_ctx = (struct sc *)dev_id;
    unsigned int i;

    sc_hal_irq_disable_dfx(sc_ctx);

    sc_ctx->cmo_err_irq = sc_hal_get_dfx_cmo_irq(sc_ctx);
    if (sc_ctx->cmo_err_irq) {
        sc_hal_clr_dfx_cmo_irq(sc_ctx);
    }
    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        sc_ctx->dfx_err_irq[i] = sc_hal_get_dfx_irq(sc_ctx, i);
        if (sc_ctx->dfx_err_irq[i]) {
            sc_hal_clr_dfx_irq(sc_ctx, i);
        }
        sc_ctx->ecc_err[i] = sc_hal_get_ecc_err(sc_ctx, i);
    }

    return IRQ_WAKE_THREAD;
}

int sc_irq_init(struct sc *sc_ctx)
{
    if (devm_request_threaded_irq(sc_ctx->dev, sc_ctx->irq_dfx, sc_irq_handler, sc_irq_thread, sc_ctx->irq_flag, "sc_dfx", sc_ctx)) {
        bsp_err("irq=%u request fail\n", sc_ctx->irq_dfx);
        return -1;
    }

    sc_hal_irq_enable_dfx(sc_ctx);
    return 0;
}

int sc_dt_config(struct sc *sc_ctx)
{
    device_node_s *node = NULL;
    int ret;

    node = bsp_dt_find_compatible_node(NULL, NULL, "peri_syscache");
    if (node == NULL) {
        bsp_err("no peri_syscache\n");
        return -1;
    }

    sc_ctx->regs = bsp_dt_iomap(node, 0);
    if (sc_ctx->regs == NULL) {
        bsp_err("no gid ctrl reg\n");
        return -1;
    }

    sc_ctx->irq_fc = bsp_dt_irq_parse_and_map(node, 0);
    if (sc_ctx->irq_fc < 0) {
        bsp_err("bsp_dt_irq_parse_and_map failed\n");
        return -1;
    }

    sc_ctx->irq_dfx = bsp_dt_irq_parse_and_map(node, 1);
    if (sc_ctx->irq_dfx < 0) {
        bsp_err("bsp_dt_irq_parse_and_map failed\n");
        return -1;
    }

    ret = bsp_dt_property_read_u32(node, "sc-slice-cnt", &sc_ctx->slice_cnt);
    if (ret || sc_ctx->slice_cnt > SC_INT_CNT || sc_ctx->slice_cnt == 0) {
        bsp_err("no gid ctrl slice_cnt:%u\n", sc_ctx->slice_cnt);
        return -1;
    }

    ret = bsp_dt_property_read_u32(node, "sc-gid-quota-ringbuf", &sc_ctx->gid_quota_ringbuf);
    if (ret) {
        bsp_err("no gid_quota_ringbuf\n");
        return -1;
    }

    sc_ctx->dfx_cnt = (struct sc_dfx_err *)kzalloc(sizeof(struct sc_dfx_err) * sc_ctx->slice_cnt, GFP_KERNEL);
    if (sc_ctx->dfx_cnt == NULL) {
        bsp_err("sc_dfx_err alloc fail\n");
        return -1;
    }

    return 0;
}

int sc_init(struct platform_device *pdev)
{
    struct sc *sc_ctx = &g_sc_ctx;

    bsp_err("sc init start\n");
    spin_lock_init(&sc_ctx->lock);
    sc_ctx->dev = &pdev->dev;

    if (sc_dt_config(sc_ctx)) {
        return -1;
    }

    sc_ctx->irq_flag = 0;
    sc_ctx->dbg = 0;
    if (sc_irq_init(sc_ctx)) {
        return -1;
    }

    return 0;
}

void sc_show_om(void)
{
    struct sc *sc_ctx = &g_sc_ctx;
    unsigned int i, j;

    for (i = 0; i < sc_ctx->slice_cnt; i++) {
        for (j = 0; j < SC_DFX_INT_BIT_CNT; j++) {
            bsp_err("dfx slice[%d] bit[%2d]      :%u\n", i, j, sc_ctx->dfx_cnt[i].cnt[j]);
        }
    }

    bsp_err("cmo_cmd_acess_err :%u\n", sc_ctx->cmo_cmd_acess_err);
    bsp_err("cmo_overflow_err  :%u\n", sc_ctx->cmo_overflow_err);
    bsp_err("cmo_pa_err        :%u\n", sc_ctx->cmo_pa_err);
    bsp_err("cmo_byway_err     :%u\n", sc_ctx->cmo_byway_err);
    bsp_err("cmo_bygid_err     :%u\n", sc_ctx->cmo_bygid_err);
    bsp_err("cmo_clr_fail      :%u\n", sc_ctx->cmo_clr_fail);
    bsp_err("timeout           :%u\n", sc_ctx->timeout);
}

void sc_set_bypass(unsigned int gid, unsigned int bypass)
{
    struct sc *sc_ctx = &g_sc_ctx;

    if (gid >= SC_GID_MAX) {
        bsp_err("gid err\n");
        return;
    }
    sc_hal_set_bypass(sc_ctx, gid, !!bypass);
    sc_cmo_by_gid(gid);
}

unsigned int sc_show_quota(unsigned int gid)
{
    if (gid >= SC_GID_MAX) {
        bsp_err("gid err\n");
        return 0;
    }
    return sc_hal_get_cache_size(&g_sc_ctx, gid);
}
EXPORT_SYMBOL(sc_set_dbg);
EXPORT_SYMBOL(sc_cmo_by_gid);
EXPORT_SYMBOL(sc_show_om);
EXPORT_SYMBOL(sc_set_bypass);
EXPORT_SYMBOL(sc_show_quota);

void sc_dump_hook(void)
{
    int i;

    for (i = 0; i < SC_GID_MAX; i++) {
        sc_cmo_by_gid(i);
    }

    sc_dump_reg_hook(&g_sc_ctx);
}

// stub
struct page *lb_alloc_pages(u32 pgid, gfp_t gfp_mask, u32 order)
{
    return NULL;
}

int lb_free_pages(u32 pgid, struct page *pages, u32 order)
{
    return 0;
}

void lb_cmo_by_gid(u32 pgid, u32 ops) {}
