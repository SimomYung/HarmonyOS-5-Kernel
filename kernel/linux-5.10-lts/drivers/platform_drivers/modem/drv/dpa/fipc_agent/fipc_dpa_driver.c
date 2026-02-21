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
#include "fipc_device.h"
#include "fipc_core.h"
#include "fipc_dts.h"
#include "fipc_pmsr.h"
#include "fipc_reset.h"
#include "bsp_dpa.h"

#define FIPC_CLK_MASK 0x0080
#define FIPC_CLK_EN_BASE 0x0000
#define FIPC_CLK_DIS_BASE 0x0004
#define DPA_CRG_BASE 0xF8001000
#define DPA_CRG_SIZE 0x8000

static void *g_fipc_clk_base_addr = NULL;
static void fipc_clock_en(unsigned int clk_mask, void *clk_en_addr)
{
    writel(clk_mask, clk_en_addr);
}

static void fipc_clock_disable(unsigned int clk_mask, void *clk_disable_addr)
{
    writel(clk_mask, clk_disable_addr);
}

void fipc_plat_dpa_suspend(void)
{
    fipc_chn_suspend(FIPC_PMSR_LATE);
    fipc_clock_disable(FIPC_CLK_MASK, (void *)(g_fipc_clk_base_addr + FIPC_CLK_DIS_BASE));
}

void fipc_plat_dpa_resume(void)
{
    fipc_clock_en(FIPC_CLK_MASK, (void *)(g_fipc_clk_base_addr + FIPC_CLK_EN_BASE));
    fipc_chn_resume(FIPC_PMSR_LATE);
}

static struct dpa_rpm_ops g_dpa_fipc_pmsr = {
    .rpm_suspend = fipc_plat_dpa_suspend,
    .rpm_resume = fipc_plat_dpa_resume,
};

// 通道在Acpu的初始化阶段已经完成，此处仅将通道打开
void fipc_plat_dpa_init(void)
{
    u32 idx;
    u32 chidx;
    int ret = 0;
    struct fipc_device *pdev = NULL;
    struct fipc_channel *pchn = NULL;
    g_fipc_clk_base_addr = fipc_ioremap(DPA_CRG_BASE, DPA_CRG_SIZE);

    for (idx = 0; idx < FIPC_DEVICE_NUM_MAX; idx++) {
        pdev = fipc_device_get_fast(idx);
        if (pdev == NULL || pdev->ctrl_level != FIPC_DEV_CONTROL_LEVEL_LATE) {
            fipc_print_trace("acore dpa driver fipc init for dev[0x%x] continue!", idx);
            continue;
        }
        // dpa的eicc需要开钟
        fipc_clock_en(FIPC_CLK_MASK, (void *)(g_fipc_clk_base_addr + FIPC_CLK_EN_BASE));
        for (chidx = 0; chidx < pdev->pipepair_cnt; chidx++) {
            pchn = pdev->channels[chidx];
            if (pchn == NULL) {
                continue;
            }
            if (pchn->pattr == NULL) {
                fipc_print_trace("fipc open delay dev[0x%x] channel[0x%x] failed, attr is NULL!\n", pdev->id,
                    pchn->user_id);
                continue;
            }
            ret |= fipc_channel_open(pdev, pchn, pchn->pattr);
            if (ret) {
                fipc_print_error("fipc_channel_open %d[%d] delay failed @%d\n", pdev->id, pchn->user_id, __LINE__);
            }
            fipc_free(pchn->pattr);
            pchn->pattr = NULL;
            pchn->state = (u32)FIPC_CHN_OPEN;
        }
        pdev->state = FIPC_DEV_STATE_WORKING;
    }
    ret = bsp_dpa_rpm_register(DPA_RPM_EICC, &g_dpa_fipc_pmsr);
    fipc_print_always("acore dpa driver fipc init done ret=%d!", ret);
}

int bsp_dpa_fipc_init(void)
{
    int ret = bsp_dpa_unreset_hook_register(DPA_UNRESET_HOOK_FIPC, (dpa_unreset_hook_ops)&fipc_plat_dpa_init);
    if (ret) {
        mdrv_err("unrst register err %d\n", ret);
        return ret;
    }
    mdrv_err("init ok\n");
    return ret;
}

EXPORT_SYMBOL(fipc_plat_dpa_suspend);
EXPORT_SYMBOL(fipc_plat_dpa_resume);
module_init(bsp_dpa_fipc_init);
