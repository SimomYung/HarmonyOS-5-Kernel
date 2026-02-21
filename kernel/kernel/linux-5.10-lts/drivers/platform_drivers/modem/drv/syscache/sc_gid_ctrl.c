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
#include <securec.h>
#include <bsp_syscache.h>
#include <bsp_print.h>
#include <bsp_dt.h>
#include <bsp_version.h>
#include <bsp_dump.h>
#include "sc_gid_ctrl_hal.h"
#include "sc_gid_ctrl.h"
#include "sc.h"

#define THIS_MODU mod_syscache

#define SC_GID_CTRL_USED 0xffffffaa
#define SC_GID_CTRL_UNUSED 0
#define SC_U64_BITLEN 64
#define SC_ALIGN 4096
#define SC_DUMP_SIZE 0xA00

unsigned int sc_rw_policy[SC_RW_POLICY_MAX] = {
    0x8, 0x9, 0xa, 0xb, 0x8, 0x9, 0xa, 0xb
};
struct gid_ctrl g_gid_ctrl;
EXPORT_SYMBOL(g_gid_ctrl);
extern struct page *lb_alloc_pages(u32 pgid, gfp_t gfp_mask, u32 order);
extern int lb_free_pages(u32 pgid, struct page *pages, u32 order);
extern void lb_cmo_by_gid(u32 pgid, u32 ops);

struct gid_ctrl *sc_get_gid_ctx(void)
{
    return &g_gid_ctrl;
}

int bsp_sc_get_sc_mode(struct sc_mode *mode, int rpolicy, int wpolicy, unsigned int type)
{
    gid_cfg cfg = g_gid_ctrl.common_cfg;
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;

    UNUSED(type);
    if (mode == NULL || rpolicy >= SC_RW_POLICY_MAX || wpolicy >= SC_RW_POLICY_MAX) {
        bsp_err("para err:%d-%d\n", rpolicy, wpolicy);
        return -1;
    }

    if (gid_ctx->gid_only && gid_ctx->regs_acpu != NULL) {
        cfg.bits.gid = SC_GID_DRA_MEM;
        cfg.bits.arhint = sc_rw_policy[rpolicy];
        cfg.bits.awhint = sc_rw_policy[wpolicy];
    } else {
        cfg.bits.gid = 0;
    }

    mode->sc_idx = g_gid_ctrl.sc_mode_idx;
    mode->cfg = cfg.val;

    return 0;
}

int sc_gid_ctrl_cfg(struct gid_ctrl *gid_ctx, unsigned long long phy_addr, unsigned int size)
{
    unsigned long flag;
    gid_cfg cfg = gid_ctx->common_cfg;
    unsigned int start_addr;
    unsigned int end_addr;
    int i;

    start_addr = phy_addr >> GID_CTRL_ADDR_MASK_BIT;
    end_addr = ((phy_addr + size) >> GID_CTRL_ADDR_MASK_BIT) - 1;

    if (gid_ctx->gid_only && gid_ctx->regs_acpu != NULL) {
        cfg.bits.gid = SC_GID_RINGBUF;
    } else {
        cfg.bits.gid = 0;
    }

    spin_lock_irqsave(&gid_ctx->lock, flag);
    for (i = 0; i < SC_GID_CTRL_MAX; i++) {
        if (gid_ctx->cfg_om[i].enable == 0) {
            break;
        }
    }

    if (i == SC_GID_CTRL_MAX) {
        bsp_err("no idle gid ctrl entry\n");
        spin_unlock_irqrestore(&gid_ctx->lock, flag);
        return -1;
    }
    bsp_err("gid ctrl addr:0x%llx-0x%x\n", phy_addr, size);

    gid_ctx->cfg_om[i].start_addr = phy_addr;
    gid_ctx->cfg_om[i].end_addr = phy_addr + size;
    gid_ctx->cfg_om[i].size = size;
    gid_ctx->cfg_om[i].cfg = cfg.val;
    gid_ctx->cfg_om[i].enable = SC_GID_CTRL_USED;
    sc_gid_ctrl_hal_addr_cfg(gid_ctx, start_addr, end_addr, cfg.val, i);
    spin_unlock_irqrestore(&gid_ctx->lock, flag);

    return 0;
}

int bsp_sc_region_config(unsigned long long phy_addr, unsigned int size)
{
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;

    if (gid_ctx->init == 0 || phy_addr == 0 || !IS_ALIGNED(phy_addr, SC_ALIGN)
        || size == 0 || !IS_ALIGNED(size, SC_ALIGN)) {
        bsp_err("init:%u\n", gid_ctx->init);
        return -1;
    }

    return sc_gid_ctrl_cfg(gid_ctx, phy_addr, size);
}

char *bsp_sc_alloc_ringbuf(struct device *dev, unsigned int size, unsigned long long *phy_addr, gfp_t gfp)
{
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;
    char *base = NULL;
    int ret;

    if (gid_ctx->init == 0 || phy_addr == NULL) {
        bsp_err("sc no init:%u\n", gid_ctx->init);
        return NULL;
    }

    size = ALIGN(size, SC_GID_CTRL_ALIGN);
    if (gid_ctx->gid_only) {
        base = (char *)dma_alloc_coherent(dev, size, phy_addr, gfp);
    } else {
        base = (char *)kmalloc(size, gfp);
        *phy_addr = virt_to_phys((char *)(uintptr_t)base);
    }
    if (base == NULL) {
        bsp_err("alloc ringbuf fail\n");
        return NULL;
    }

    ret = sc_gid_ctrl_cfg(gid_ctx, *phy_addr, size);
    if (ret) {
        goto ERR;
    }
    return base;
ERR:
    if (gid_ctx->gid_only) {
        dma_free_coherent(dev, size, base, *phy_addr);
    } else {
        kfree(base);
    }

    return NULL;
}

void bsp_sc_free_ringbuf(struct device *dev, size_t size, void *cpu_addr, dma_addr_t dma_handle)
{
    unsigned long flag;
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;
    unsigned int start_addr;
    int i;

    if (dev == NULL || size == 0 || cpu_addr == NULL || dma_handle == 0) {
        bsp_err("param err\n");
        return;
    }

    start_addr = dma_handle;

    spin_lock_irqsave(&gid_ctx->lock, flag);
    for (i = 0; i < SC_GID_CTRL_MAX; i++) {
        if (gid_ctx->cfg_om[i].start_addr == start_addr) {
            break;
        }
    }

    if (i == SC_GID_CTRL_MAX) {
        bsp_err("no find gid ctrl entry\n");
        spin_unlock_irqrestore(&gid_ctx->lock, flag);
        return;
    }

    gid_ctx->cfg_om[i].enable = SC_GID_CTRL_UNUSED;
    sc_gid_ctrl_hal_enabel(gid_ctx, i, 0);
    bsp_sc_invalid_cache(dma_handle, gid_ctx->cfg_om[i].size);
    if (gid_ctx->gid_only) {
        dma_free_coherent(dev, gid_ctx->cfg_om[i].size, cpu_addr, dma_handle);
    } else {
        kfree(cpu_addr);
    }
    spin_unlock_irqrestore(&gid_ctx->lock, flag);
    return;
}

dma_addr_t bsp_sc_dma_map_single(struct device *dev, void *addr, size_t size, enum dma_data_direction dir)
{
    if (g_gid_ctrl.gid_only) {
        return dma_map_single(dev, addr, size, dir);
    } else {
        return virt_to_phys(addr);
    }
}

void bsp_sc_dma_unmap_single(struct device *dev, dma_addr_t addr, size_t size, enum dma_data_direction dir)
{
    if (g_gid_ctrl.gid_only) {
        if (bsp_get_version_info()->plat_type != PLAT_ESL) {
            bsp_sc_invalid_cache(addr, size);
        }
        dma_unmap_single(dev, addr, size, dir);
    }
}

unsigned long bsp_sc_alloc_page(unsigned int pgid, gfp_t gfp_mask)
{
    struct page *page = NULL;

    if (g_gid_ctrl.gid_only) {
        return __get_free_page(gfp_mask);
    } else {
        page = lb_alloc_pages(pgid, gfp_mask, 0);
        return (page == NULL) ? 0 : (unsigned long)(uintptr_t)page_to_virt(page);
    }
}

void bsp_sc_free_page(unsigned int pgid, unsigned long addr)
{
    if (g_gid_ctrl.gid_only) {
        // 刷cache
        bsp_sc_invalid_cache(addr, PAGE_SIZE);
        free_page(addr);
    } else {
        lb_free_pages(pgid, phys_to_page(addr), 0);
    }
}

EXPORT_SYMBOL(bsp_sc_get_sc_mode);
EXPORT_SYMBOL(bsp_sc_alloc_ringbuf);
EXPORT_SYMBOL(bsp_sc_free_ringbuf);
EXPORT_SYMBOL(bsp_sc_dma_unmap_single);
EXPORT_SYMBOL(bsp_sc_dma_map_single);
EXPORT_SYMBOL(bsp_sc_alloc_page);
EXPORT_SYMBOL(bsp_sc_free_page);
EXPORT_SYMBOL(bsp_sc_region_config);

void gid_ctrl_dump_phone_cmo(void)
{
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;
    int i;

    for (i = 0; i < SC_GID_PHONE_USED_CNT; i++) {
        if (gid_ctx->sc_gid_cmo[i] != 0) {
            lb_cmo_by_gid(gid_ctx->sc_gid_cmo[i], 2); // 0, INV 1,CLN 2,CI
            bsp_err("dump gid cmo:%d\n", gid_ctx->sc_gid_cmo[i]);
        }
    }
}

void gid_ctrl_dump_hook(void)
{
    sc_gid_ctrl_dump_reg(&g_gid_ctrl);
    gid_ctrl_dump_phone_cmo();
    sc_dump_hook();
}
EXPORT_SYMBOL(gid_ctrl_dump_hook);

static void gid_ctrl_dump_init(struct gid_ctrl *gid_ctx)
{
    int ret;

    gid_ctx->dump_base = (unsigned int *)bsp_dump_register_field(DUMP_MODEMAP_SYSCACHE, "SYSCACHE", SC_DUMP_SIZE, 0);
    if (gid_ctx->dump_base == NULL) {
        bsp_err("dump mem alloc fail\n");
        return;
    }
    ret = (int)memset_s(gid_ctx->dump_base, SC_DUMP_SIZE, 0, SC_DUMP_SIZE);
    if (ret) {
        bsp_err("dump memset_s fail\n");
    }

    ret = bsp_dump_register_hook("gid_ctrl", gid_ctrl_dump_hook);
    if (ret == BSP_ERROR) {
        bsp_err("register dump fail\n");
    }

    ret = bsp_dump_register_async_callback(DUMP_CP_CALLBACK, 0, gid_ctrl_dump_hook);
    if (ret == BSP_ERROR) {
        bsp_err("register async dump fail\n");
    }
}

int gid_ctrl_acpu_dt_config(struct gid_ctrl *gid_ctx)
{
    device_node_s *node = NULL;

    node = bsp_dt_find_compatible_node(NULL, NULL, "gid_ctrl_acpu");
    if (node == NULL) {
        bsp_err("no support gid_ctrl_acpu\n");
        return 0;
    }

    gid_ctx->regs_acpu = bsp_dt_iomap(node, 0);
    if (gid_ctx->regs_acpu == NULL) {
        bsp_err("no gid ctrl acpu reg\n");
        return -1;
    }

    return 0;
}

int gid_ctrl_dt_config(struct gid_ctrl *gid_ctx)
{
    device_node_s *node = NULL;
    unsigned int cfg_arr[5];
    int ret;

    node = bsp_dt_find_compatible_node(NULL, NULL, "gid_ctrl_peri");
    if (node == NULL) {
        bsp_err("no gid_ctrl_peri\n");
        return -1;
    }

    gid_ctx->regs_peri = bsp_dt_iomap(node, 0);
    if (gid_ctx->regs_peri == NULL) {
        bsp_err("no gid ctrl peri reg\n");
        return -1;
    }

    ret = bsp_dt_property_read_u32(node, "gid-only", &gid_ctx->gid_only);
    if (ret) {
        bsp_err("no gid ctrl gid_only\n");
        return -1;
    }

    ret = bsp_dt_property_read_u32(node, "gid-ctrl-sc-mode-idx", &gid_ctx->sc_mode_idx);
    if (ret) {
        bsp_err("no gid ctrl sc_mdoe\n");
        return -1;
    }

    ret = bsp_dt_property_read_u32_array(node, "sc_gid_phone_used", gid_ctx->sc_gid_cmo, SC_GID_PHONE_USED_CNT);
    if (ret) {
        bsp_err("no support sc_gid_phone_used\n");
    }

    ret = bsp_dt_property_read_u32(node, "gid-ctrl-max-entry-num", &gid_ctx->entry_max_num);
    if (ret || gid_ctx->entry_max_num > SC_GID_CTRL_MAX) {
        bsp_err("no gid ctrl entry_max_num:%u\n", gid_ctx->entry_max_num);
        return -1;
    }

    ret = bsp_dt_property_read_u32_array(node, "sc-mode-cfg", cfg_arr, 5);
    if (ret) {
        bsp_err("no gid ctrl policy\n");
        return -1;
    }

    gid_ctx->common_cfg.bits.arhint = cfg_arr[0];
    gid_ctx->common_cfg.bits.awhint = cfg_arr[1];
    gid_ctx->common_cfg.bits.arcache = cfg_arr[2];
    gid_ctx->common_cfg.bits.awcache = cfg_arr[3];
    gid_ctx->common_cfg.bits.domain = cfg_arr[4];
    gid_ctx->common_cfg.bits.override_en = 0x3f;

    return gid_ctrl_acpu_dt_config(gid_ctx);
}

static int gid_ctrl_probe(struct platform_device *pdev)
{
    struct gid_ctrl *gid_ctx = &g_gid_ctrl;

    bsp_err("[init] start\n");
    // sc初始化在前，调用sc初始化接口
    if (sc_init(pdev)) {
        bsp_err("sc init fail\n");
        return -1;
    }

    spin_lock_init(&gid_ctx->lock);
    if (gid_ctrl_dt_config(gid_ctx)) {
        return -1;
    }

    gid_ctrl_dump_init(gid_ctx);
    gid_ctx->init = 1;
    bsp_err("[init] succ\n");

    return 0;
}

static int gid_ctrl_remove(struct platform_device *pdev)
{
    return 0;
}

static struct platform_device g_gid_ctrl_device = {
    .name = "gid_ctrl",
    .id = -1,
    .num_resources = 0,
};

static struct platform_driver g_gid_ctrl_pltfm_driver = {
    .probe = gid_ctrl_probe,
    .remove = gid_ctrl_remove,
    .driver = {
        .name = "gid_ctrl",
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

int gid_ctrl_init(void)
{
    bsp_err("gid_ctrl_init\n");
    if (platform_device_register(&g_gid_ctrl_device)) {
        bsp_err("dev register fail.\n");
        return -1;
    }

    if (platform_driver_register(&g_gid_ctrl_pltfm_driver)) {
        bsp_err("driver register fail.\n");
        return -1;
    }

    return 0;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
core_initcall(gid_ctrl_init);
#endif

void gid_ctrl_exit(void)
{
    /*
     * don't need kfree platform_data in exit process,
     * platform_driver_unregister will do it.
     */
    platform_driver_unregister(&g_gid_ctrl_pltfm_driver);
    return;
}
MODULE_DESCRIPTION("Specific gid_ctrl Driver");
MODULE_LICENSE("GPL v2");
