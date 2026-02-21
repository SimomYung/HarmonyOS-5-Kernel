/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include "dra.h"
#include "dra_hal.h"
#include "dra_def.h"

void dra_set_debug_level(unsigned int bit)
{
    g_dra_ctx->dbg |= BIT(bit);
    dra_err("dbg level:%u\n", g_dra_ctx->dbg);
}

void dra_clear_debug_level(unsigned int bit)
{
    g_dra_ctx->dbg &= (~BIT(bit));
    dra_err("dbg level:%u\n", g_dra_ctx->dbg);
}

void dra_list_measure(unsigned int level)
{
    struct dra_map *map = &g_dra_ctx->opipes[level].map;
    struct dra_opipe *opipes = &g_dra_ctx->opipes[level];
    struct list_head *pos = NULL;
    unsigned int cnt = 0;
    unsigned long flags;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->free_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("free_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->busy_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("busy_list cnt %u\n", cnt);
    cnt = 0;
    spin_lock_irqsave(&opipes->lock, flags);
    list_for_each(pos, &map->reclaim_list)
    {
        cnt++;
    }
    spin_unlock_irqrestore(&opipes->lock, flags);
    bsp_err("reclaim_list cnt %u\n", cnt);
}

void dra_show_opipe_status(unsigned int level)
{
    struct dra *dra = g_dra_ctx;
    unsigned int i = level;

    if (!dra_opipe_is_enable(dra, level)) {
        bsp_err("level:%u is disable\n", level);
        return;
    }

    bsp_err("map_invalid_level %u\n", dra->status.map_invalid_level);
    bsp_err("unmap_invalid_level %u\n", dra->status.unmap_invalid_level);
    bsp_err("opipe%u:skb_alloc_success  %u\n", i, dra->opipe_status[level].alloc_success);
    bsp_err("opipe%u:skb_own_free  %u\n", i, dra->opipe_status[level].skb_own_free);
    bsp_err("opipe%u:map_own_success  %u\n", i, dra->opipe_status[level].map_own_success);
    bsp_err("opipe%u:map_own_fail  %u\n", i, dra->opipe_status[level].map_own_fail);
    bsp_err("opipe%u:free_list_empty  %u\n", i, dra->opipe_status[level].free_list_empty);
    bsp_err("opipe%u:skb_alloc_fail  %u\n", i, dra->opipe_status[level].alloc_fail);
    bsp_err("opipe%u:skb_build_fail  %u\n", i, dra->opipe_status[level].skb_build_fail);
    bsp_err("opipe%u:unmap_own  %u\n", i, dra->opipe_status[level].unmap_own);
    bsp_err("opipe%u:unmap_reuse  %u\n", i, dra->opipe_status[level].unmap_reuse);
    bsp_err("opipe%u:map_tcp_ip_ok  %u\n", i, dra->opipe_status[level].map_tcp_ip_ok);
    bsp_err("opipe%u:map_tcp_ip_fail  %u\n", i, dra->opipe_status[level].map_tcp_ip_fail);
    bsp_err("opipe%u:unmap_tcp_ip  %u\n", i, dra->opipe_status[level].unmap_tcp_ip);
    bsp_err("opipe%u:unmap_test_tcp_ip  %u\n", i, dra->opipe_status[level].unmap_test_tcp_ip);
    bsp_err("opipe%u:delay_free_cnt %u\n", i, dra->opipes[i].delay_free_cnt);
    bsp_err("opipe%u:reclaim_cnt %u\n", i, dra->opipes[i].reclaim_cnt);
    bsp_err("opipe%u:reclaim_depth %u\n", i, dra->opipes[i].reclaim_depth);
    bsp_err("opipe%u:rls_full_cnt %u\n", i, dra->opipes[i].rls_full_cnt);
    bsp_err("opipe%u:timer_free_cnt %u\n", i, dra->opipes[i].timer_free_cnt);
    bsp_err("opipe%u:free_to_kernel %u\n", i, dra->opipe_status[level].free_to_kernel);
    bsp_err("opipe%u:free_page %u\n", i, dra->opipe_status[level].free_page);
    bsp_err("opipe%u:put_page %u\n", i, dra->opipe_status[level].put_page);
    bsp_err("opipe%u:skb_tcp_ip_free %u\n", i, dra->opipe_status[level].skb_tcp_ip_free);
    bsp_err("opipe%u:busy_depth %u\n", i, dra->opipes[i].busy_depth);
    bsp_err("opipe%u:max_alloc_cnt %u\n", i, dra->opipes[i].max_alloc_cnt);
    bsp_err("opipe%u:min_alloc_cnt %u\n", i, dra->opipes[i].min_alloc_cnt);
    bsp_err("opipe%u:free_success %u\n", i, dra->opipe_status[level].free);
    bsp_err("refill_succ: %u\n", dra->status.refill_succ);
    bsp_err("sche_cnt: %u\n", dra->status.sche_cnt);
    bsp_err("reclaim_succ: %u\n", dra->status.reclaim_succ);
    bsp_err("tab size: %u\n", dra->opipes[level].map.tab_max_size);
    bsp_err("page_unreuse: %u\n", dra->opipes[level].page_unreuse);
    bsp_err("kfifo_empty: %u\n", dra->opipes[level].kfifo_empty);
    bsp_err("kfifo_in: %u\n", dra->reuse_fifo.kfifo.in);
    bsp_err("kfifo_out: %u\n", dra->reuse_fifo.kfifo.out);
    bsp_err("refill_from_recycle: %u\n", dra->opipes[level].refill_from_recycle);
    bsp_err("refill_from_kfifo: %u\n", dra->opipes[level].refill_from_kfifo);
    bsp_err("refill_from_alloc: %u\n", dra->opipes[level].refill_from_alloc);

    if (dra->opipes[level].use_tab) {
        dra_list_measure(level);
    }
}

unsigned int bsp_dra_get_cur_mem(void)
{
    unsigned int total_size = 0;
    struct dra *dra = g_dra_ctx;
    /* dra分为2k，4k，9k三种不同的块大小 */
    unsigned int blocks[] = {0x800, 0x1000, 0x2400};
    unsigned int level;
    /* dra分为2k，4k，9k三种不同的块大小 */
    for (level = 1; level <= 0x3; level++) {
        if (!dra_opipe_is_enable(dra, level)) {
            continue;
        }
        total_size += blocks[level - 1] * dra->opipes[level].busy_depth;
    }
    return total_size;
}

void dra_show_ipipe_status(void)
{
    struct dra *dra = g_dra_ctx;
    unsigned int i;
    struct dra_ipipe *ipipe = NULL;

    if (dra == NULL) {
        return;
    }

    for (i = 0; i < dra->max_cpu_ipipes; i++) {
        ipipe = dra->cpu_ipipes + i;
        bsp_err("ipipe%u ipipe_empty %u\n", ipipe->idx, ipipe->debug.ipipe_empty);
        bsp_err("ipipe%u alloc_success %u\n", ipipe->idx, ipipe->debug.alloc);
    }
}

void dra_mdmreset_status(void)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("timer cnt:%d\n", dra->timer_cnt);
    bsp_err("resetting:%d\n", dra->event);
    bsp_err("deinit_cnt:%d\n", dra->deinit_cnt);
    bsp_err("reinit_cnt:%d\n", dra->reinit_cnt);
}

void dra_get_rls_cnt(unsigned int idx)
{
    struct dra *dra = g_dra_ctx;
    struct dra_recycle_poll *recycle = &dra->recycle[idx];
    unsigned int rls_cnt = 0;

    recycle->fifo.write = dra_get_rls(dra, idx);
    if (recycle->fifo.write != recycle->fifo.read) {
        rls_cnt = dra_get_busy_num(recycle->fifo.write, recycle->fifo.read, recycle->fifo.depth);
    }

    bsp_err("rls cnt:%d\n", rls_cnt);
}

int dra_hal_show_rls(unsigned int ch)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("DRA_RLS_CH_BASE_ADDR_L:%#x\n", readl(dra->regs + DRA_RLS_CH_BASE_ADDR_L(ch)));
    bsp_err("DRA_RLS_CH_BASE_ADDR_H:%#x\n", readl(dra->regs + DRA_RLS_CH_BASE_ADDR_H(ch)));
    bsp_err("DRA_RLS_CH_DEPTH:%#x\n", readl(dra->regs + DRA_RLS_CH_DEPTH(ch)));
    bsp_err("DRA_RLS_CH_WPTR:%#x\n", readl(dra->regs + DRA_RLS_CH_WPTR(ch)));
    bsp_err("DRA_RLS_CH_RPTR:%#x\n", readl(dra->regs + DRA_RLS_CH_RPTR(ch)));
    bsp_err("DRA_RLS_CH_PTR_ADDR_L:%#x\n", readl(dra->regs + DRA_RLS_CH_PTR_ADDR_L(ch)));
    bsp_err("DRA_RLS_CH_PTR_ADDR_H:%#x\n", readl(dra->regs + DRA_RLS_CH_PTR_ADDR_H(ch)));
    bsp_err("DRA_RLS_CH_THRH:%#x\n", readl(dra->regs + DRA_RLS_CH_THRH(ch)));
    bsp_err("DRA_RLS_CH_CTRL:%#x\n", readl(dra->regs + DRA_RLS_CH_CTRL(ch)));
    bsp_err("DRA_RLS_CH_TIME_OUT_CFG:%#x\n", readl(dra->regs + DRA_RLS_CH_TIME_OUT_CFG(ch)));
    return 0;
}

int dra_hal_show_ddr(unsigned int ch)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("DRA_DDR_BUF_BASE_ADDR_L:%#x\n", readl(dra->regs + DRA_DDR_BUF_BASE_ADDR_L(ch)));
    bsp_err("DRA_DDR_BUF_BASE_ADDR_H:%#x\n", readl(dra->regs + DRA_DDR_BUF_BASE_ADDR_H(ch)));
    bsp_err("DRA_DDR_BUF_DEPTH:%#x\n", readl(dra->regs + DRA_DDR_BUF_DEPTH(ch)));
    bsp_err("DRA_DDR_BUF_WPTR:%#x\n", readl(dra->regs + DRA_DDR_BUF_WPTR(ch)));
    bsp_err("DRA_DDR_BUF_RPTR:%#x\n", readl(dra->regs + DRA_DDR_BUF_RPTR(ch)));
    bsp_err("DRA_DDR_BUF_DOWN_THRH:%#x\n", readl(dra->regs + DRA_DDR_BUF_DOWN_THRH(ch)));
    bsp_err("DRA_DDR_BUF_WPTR_INTRL:%#x\n", readl(dra->regs + DRA_DDR_BUF_WPTR_INTRL(ch)));
    bsp_err("DRA_DDR_BUF_RPTR_INTRL:%#x\n", readl(dra->regs + DRA_DDR_BUF_RPTR_INTRL(ch)));
    return 0;
}

int dra_hal_show_allc(unsigned int ch)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("DRA_ALLOC_CH_BASE_ADDR_L:%#x\n", readl(dra->regs + DRA_ALLOC_CH_BASE_ADDR_L(ch)));
    bsp_err("DRA_ALLOC_CH_BASE_ADDR_H:%#x\n", readl(dra->regs + DRA_ALLOC_CH_BASE_ADDR_H(ch)));
    bsp_err("DRA_ALLOC_CH_DEPTH:%#x\n", readl(dra->regs + DRA_ALLOC_CH_DEPTH(ch)));
    bsp_err("DRA_ALLOC_CH_WPTR:%#x\n", readl(dra->regs + DRA_ALLOC_CH_WPTR(ch)));
    bsp_err("DRA_ALLOC_CH_RPTR:%#x\n", readl(dra->regs + DRA_ALLOC_CH_RPTR(ch)));
    bsp_err("DRA_ALLOC_CH_PTR_ADDR_L:%#x\n", readl(dra->regs + DRA_ALLOC_CH_PTR_ADDR_L(ch)));
    bsp_err("DRA_ALLOC_CH_PTR_ADDR_H:%#x\n", readl(dra->regs + DRA_ALLOC_CH_PTR_ADDR_H(ch)));
    bsp_err("DRA_ALLOC_CH_CTRL:%#x\n", readl(dra->regs + DRA_ALLOC_CH_CTRL(ch)));
    bsp_err("DRA_ALLOC_CH_EN:%#x\n", readl(dra->regs + DRA_ALLOC_CH_EN(ch)));

    return 0;
}

int dra_hal_show_rec(unsigned int ch)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("DRA_RECYCLE_POOL_BASE_ADDR_L:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_BASE_ADDR_L(ch)));
    bsp_err("DRA_RECYCLE_POOL_BASE_ADDR_H:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_BASE_ADDR_H(ch)));
    bsp_err("DRA_RECYCLE_POOL_DEPTH:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_DEPTH(ch)));
    bsp_err("DRA_RECYCLE_POOL_WPTR:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_WPTR(ch)));
    bsp_err("DRA_RECYCLE_POOL_RPTR:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_RPTR(ch)));
    bsp_err("DRA_RECYCLE_POOL_UP_THRH:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_UP_THRH(ch)));
    bsp_err("DRA_RECYCLE_POOL_WPTR_INTRL:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_WPTR_INTRL(ch)));

    return 0;
}

int dra_hal_show_intr(unsigned int ch)
{
    struct dra *dra = g_dra_ctx;
    bsp_err("DRA_CORE0_BIG_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE0_BIG_POOL_INT_MASK(0)));
    bsp_err("DRA_CORE0_BIG_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE0_BIG_POOL_INT_MASK(1)));
    bsp_err("DRA_CORE0_RECYCLE_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE0_RECYCLE_POOL_INT_MASK));
    bsp_err("DRA_CORE1_BIG_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE1_BIG_POOL_INT_MASK(0)));
    bsp_err("DRA_CORE1_BIG_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE1_BIG_POOL_INT_MASK(1)));
    bsp_err("DRA_CORE1_RECYCLE_POOL_INT_MASK:%#x\n", readl(dra->regs + DRA_CORE1_RECYCLE_POOL_INT_MASK));
    bsp_err("DRA_BIG_POOL_INT_RAW:%#x\n", readl(dra->regs + DRA_BIG_POOL_INT_RAW(0)));
    bsp_err("DRA_BIG_POOL_INT_RAW:%#x\n", readl(dra->regs + DRA_BIG_POOL_INT_RAW(1)));
    bsp_err("DRA_RECYCLE_POOL_INT_RAW:%#x\n", readl(dra->regs + DRA_RECYCLE_POOL_INT_RAW));

    return 0;
}

void dra_show_buf(unsigned long long buf)
{
    dra_buf_t dra_buf;
    struct page *page = NULL;

    dra_buf.addr = buf;
    page = phys_to_page(dra_buf.bits.phyaddr);
    bsp_err("page:0x%llx\n", (unsigned long long)(uintptr_t)page);
    bsp_err("compound:%d\n", PageCompound(page));
    bsp_err("refcount:0x%x\n", page_ref_count(page));
}

void dra_help(void)
{
    bsp_err("dra_show_opipe_status \n");
    bsp_err("dra_show_ipipe_status \n");
    bsp_err("dra_show_push_cnt \n");
    bsp_err("dra_get_rls_cnt\n");
}
