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

#include <asm/memory.h>
#include <asm/irq_regs.h>
#include <asm/irqflags.h>
#include <asm/string.h>
#include <bsp_sysctrl.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_dt.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_platform.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/platform_device.h>
#include <linux/vmalloc.h>
#include <linux/gfp.h>
#include <linux/dma-direction.h>
#include <linux/dma-mapping.h>
#include <linux/dma-map-ops.h>
#include "sppm_reg_base.h"
#include "sppm_soc_interface.h"
#include "mdrv_sppm.h"
#include "sppm_ipc_protocol.h"
#include "sppm_device.h"

#undef THIS_MODU
#define THIS_MODU mod_sppm

static sppm_dev_info *g_sppm_info = NULL;

#if defined(__aarch64__)
#else
#error you should define a ARM platform for sppm driver.
#endif

void sppm_debug_info_show(void)
{
    sppm_dev_info *dev_info = g_sppm_info;

    sppm_info("shreshold_cnt:%d\n", dev_info->om_info.shreshold_cnt);
    sppm_info("cpu_pp_ovfw:%d\n", dev_info->om_info.cpu_pp_ovfw);
    sppm_info("regs_ovfl:%d\n", dev_info->om_info.regs_ovfl);
    sppm_info("ring_buf_ovfw:%d\n", dev_info->om_info.ring_buf_ovfw);
    sppm_info("rresp_err:%d\n", dev_info->om_info.rresp_err);
    sppm_info("wresp_err:%d\n", dev_info->om_info.wresp_err);
    sppm_info("regs_numerr:%d\n", dev_info->om_info.regs_numerr);
    sppm_info("cpu_wr_reg_err:%d\n", dev_info->om_info.cpu_wr_reg_err);

    sppm_info("cur wptr:%d\n", sppm_get_rb_rptr());
    sppm_info("cur rptr:%d\n", sppm_get_rb_wptr());
    sppm_info("cur int raw state:0x%x\n", sppm_get_raw_int_status());
    sppm_info("cur int masked state:0x%x\n", sppm_get_int_status_masked());
    sppm_info("cur rchan enable state:0x%x\n", sppm_get_rchan_en_state());
    sppm_info("start at 32k slice of:0x%x\n", sppm_get_rchan_en_state());
    sppm_info("direct rchan over flow at 32k slice of:0x%x\n", sppm_get_32k_slice());
    sppm_info("cpu write over flow at 32k slice of:0x%x\n", sppm_get_32k_ov_slice());
    sppm_info("rchan busy state:0x%x\n", sppm_get_rchan_busy_state());
    sppm_info("sppm busy state:0x%x\n", sppm_get_all_idle_state());
    return;
}

// 中断下半部根据注册的回调函数处理。
int sppm_shreshold_cb_reg(sppm_shreshold_cb pcb)
{
    sppm_dev_info *dev_info = g_sppm_info;
    if (pcb != NULL) {
        dev_info->callback = pcb;
    }
    return SPPM_OK;
}

// 中断下半部处理函数，水线中断需要处理。
static void sppm_bottom_half_work(struct work_struct *work)
{
    sppm_dev_info *dev_info = g_sppm_info;
    if (dev_info->callback == NULL) {
        sppm_clear_int_status(SPPM_INT_RING_BUFF_INT);
    } else {
        // 更新读指针,写清寄存器，打开水线中断
        dev_info->callback();
    }

    return;
}

// bitmask0
static void sppm_rb_threshold_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.shreshold_cnt++;
    schedule_delayed_work(&dev_info->sppm_task, 0);
    return;
}

// bitmask1
static void sppm_cpu_pp_ovfl_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.cpu_pp_ovfw++;
    sppm_err("<%s> sppm ringbuff overflowed ++.\n", __func__);
    return;
}

// bitmask2
static void sppm_rchan_regs_ovfl_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.regs_ovfl++;
    sppm_err("<%s> sppm rchan read overflow err++.\n", __func__);
    return;
}

// bitamsk3
static void sppm_rb_ovfl_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.ring_buf_ovfw++;
    sppm_err("<%s> sppm ringbuff overflowed err++.\n", __func__);
    return;
}

// bitmask4
static void sppm_wresp_err_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.wresp_err++;
    sppm_err("<%s> sppm_wresp_err_handler err++.\n", __func__);
    return;
}

// bitmask5
static void sppm_rresp_err_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.rresp_err++;
    sppm_err("<%s>sppm_rresp_err_handler err++.\n", __func__);
    return;
}

// bitmask6
static void sppm_rchan_regs_numerr_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.regs_numerr++;
    bsp_sppm_stop_sampling();
    sppm_err("<%s> sppm_rchan_regs_numerr_handler err ++.\n", __func__);
    return;
}

// bitmask7
static void sppm_cpu_wr_reg_err_handler(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    dev_info->om_info.cpu_wr_reg_err++;
    sppm_err("<%s> sppm_cpu_wr_reg_err_handler err++.\n", __func__);
    return;
}

static irqreturn_t sppm_interrupt_handler(int irq, void *data)
{
    unsigned int val;
    sppm_dev_info *dev_info = (sppm_dev_info *)data;

    spin_lock(&dev_info->synclock);
    val = sppm_get_int_status_masked();
    if (val & SPPM_INT_RING_BUFF_INT) {
        sppm_set_int_disable(SPPM_INT_RING_BUFF_INT);
        sppm_clear_int_status(SPPM_INT_RING_BUFF_INT);
        sppm_rb_threshold_handler();
        goto out;
    }

    if (val & SPPM_INT_CPU_PP_OVFW) {
        sppm_clear_int_status(SPPM_INT_CPU_PP_OVFW);
        sppm_cpu_pp_ovfl_handler();
    }

    if (val & SPPM_INT_DIR_RD_OVFW) {
        sppm_set_int_disable(SPPM_INT_DIR_RD_OVFW);
        sppm_clear_int_status(SPPM_INT_DIR_RD_OVFW);
        sppm_rchan_regs_ovfl_handler();
    }

    if (val & SPPM_INT_RING_BUFF_OVFW) {
        sppm_set_int_disable(SPPM_INT_RING_BUFF_OVFW);
        sppm_clear_int_status(SPPM_INT_RING_BUFF_OVFW);
        sppm_rb_ovfl_handler();
    }

    if (val & SPPM_INT_WRESP_ERR) {
        sppm_set_int_disable(SPPM_INT_WRESP_ERR);
        sppm_clear_int_status(SPPM_INT_WRESP_ERR);
        sppm_wresp_err_handler();
    }

    if (val & SPPM_INT_RRESP_ERR) {
        sppm_set_int_disable(SPPM_INT_RRESP_ERR);
        sppm_clear_int_status(SPPM_INT_RRESP_ERR);
        sppm_rresp_err_handler();
    }

    if (val & SPPM_INT_REG_NUM_ERR) {
        sppm_clear_int_status(SPPM_INT_REG_NUM_ERR);
        sppm_rchan_regs_numerr_handler();
    }

    if (val & SPPM_INT_CPU_WR_REG_ERR) {
        sppm_clear_int_status(SPPM_INT_CPU_WR_REG_ERR);
        sppm_cpu_wr_reg_err_handler();
    }

out:
    spin_unlock(&dev_info->synclock);
    return IRQ_HANDLED;
}

int sppm_pwr_enable(void)
{
    (void)sppm_crg_clk_en();
    return SPPM_OK;
}

int sppm_pwr_disable(void)
{
    (void)sppm_crg_clk_dis();
    return SPPM_OK;
}

static int sppm_dst_ringbuff_alloc(sppm_dev_info *dev_info)
{
    void *virt_addr = NULL;
    dma_addr_t phy_addr;

    virt_addr = dma_alloc_coherent(dev_info->dev, dev_info->rb_size, &phy_addr, GFP_KERNEL);
    sppm_info("<%s>  ringbuff with size %d\n", __func__, dev_info->rb_size);
    if (virt_addr == NULL) {
        sppm_err("<%s> cannot allocate ring buffer\n", __func__);
        return SPPM_ERROR;
    }
    dev_info->virt_addr = (uintptr_t)virt_addr;
    dev_info->rb_phy_addr = (uintptr_t)phy_addr;
    (void)memset_s((void *)dev_info->virt_addr, dev_info->rb_size, 0x00, dev_info->rb_size);
    return SPPM_OK;
}

static int sppm_dst_ringbuff_release(sppm_dev_info *dev_info)
{
    if (dev_info->virt_addr) {
        dma_free_coherent(
            dev_info->dev, dev_info->rb_size, (void *)dev_info->virt_addr, (dma_addr_t)dev_info->rb_phy_addr);
    }
    dev_info->virt_addr = 0x00UL;
    dev_info->rb_phy_addr = 0x00UL;
    return SPPM_OK;
}

static int sppm_of_get_attribute(const device_node_s *np, sppm_dev_info *dev_info)
{
    int ret;
    const char *platform = NULL;

    ret = bsp_dt_property_read_u32(np, "version_id", &dev_info->version_id);
    if (ret < 0) {
        sppm_err("<%s> cannot get sppm version_id\n", __func__);
        return -ENOENT;
    }

    ret = bsp_dt_property_read_u32(np, "rchan_nums", &dev_info->rchan_num);
    if (ret < 0) {
        sppm_err("<%s> cannot get sppm rchan nums\n", __func__);
        return -ENOENT;
    }

    ret = bsp_dt_property_read_u32(np, "rchan_start_idx", &dev_info->rchan_start_idx);
    if (ret < 0) {
        sppm_err("<%s> cannot get sppm rchan start idx\n", __func__);
        return -ENOENT;
    }

    ret = bsp_dt_property_read_string(np, (const char *)"platform", &platform);
    if (ret < 0 || platform == NULL) {
        sppm_err("fail to read platform name\n");
        return -ENOENT;
    }

    if (strcpy_s(dev_info->platform_name, SPPM_SRC_NAME_LEN, platform)) {
        sppm_err("fail to cpy platform name\n");
        return -ENOENT;
    }
    return SPPM_OK;
}

static int sppm_devinfo_init(sppm_dev_info *dev_info, struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    int ret;
    device_node_s *np = NULL;

    np = bsp_dt_find_compatible_node(NULL, NULL, "sppm-driver");
    if (np == NULL) {
        sppm_err("sppm_driver dt find err.\n");
        return -1;
    }

    dev_info->dev = dev;
    dev_info->regs = bsp_dt_iomap(np, 0);
    if (dev_info->regs == NULL) {
        sppm_err("iomap failed.\n");
        return -1;
    }

    dev_info->irq = bsp_dt_irq_parse_and_map(np, 0);
    if (dev_info->irq < 0) {
        sppm_err("bsp_dt_irq_parse_and_map failed.\n");
        return -1;
    }

    ret = request_irq(dev_info->irq, sppm_interrupt_handler, NULL, "sppm-irq", dev_info);
    if (ret < 0) {
        sppm_err("<%s> requset irq error\n", __func__);
        return -ENODEV;
    }
    ret = sppm_of_get_attribute(np, dev_info);
    if (ret < 0) {
        sppm_err("<%s> get dts fail\n", __func__);
        return -ENOENT;
    }

    g_sppm_info = dev_info;

    return SPPM_OK;
}

static int sppm_base_addr_init(sppm_dev_info *dev_info)
{
    void __iomem *pericrg_base;

    // SYSCTRL_MODEM_PERICRG 接口待SYSCTRL组件提供
    pericrg_base = ioremap((phys_addr_t)(uintptr_t)0xE6800000, 0x1000);
    if (pericrg_base == NULL) {
        sppm_err("<%s> pericrg_base ioremap fail\n", __func__);
        return -ENOMEM;
    }
    sppm_base_addr_set((void *)dev_info->regs, (void *)pericrg_base);
    return SPPM_OK;
}

static void sppm_dma_config_init(struct device *sppm_dev)
{
    if (sppm_dev == NULL) {
        sppm_err("<%s> perfstat_dev is NULL! \n", __func__);
        return;
    }
    of_dma_configure(sppm_dev, NULL, true);
    dma_set_mask_and_coherent(sppm_dev, DMA_BIT_MASK(BITS_PER_LONG));
    return;
}

// sppm_probe for sppm controller1.
static int sppm_ctrl_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    sppm_dev_info *dev_info = NULL;
    int ret;

    sppm_info("sppm start.\n");
    sppm_dma_config_init(dev);

    dev_info = devm_kzalloc(dev, sizeof(sppm_dev_info), GFP_KERNEL);
    if (dev_info == NULL) {
        sppm_err("<%s> cannot alloc sppm desc\n", __func__);
        return -ENOMEM;
    }

    ret = sppm_devinfo_init(dev_info, pdev);
    if (ret != SPPM_OK) {
        devm_kfree(dev, dev_info);
        return ret;
    }
    ret = sppm_base_addr_init(dev_info);
    if (ret != SPPM_OK) {
        sppm_err("<%s> sppm_base_addr_init fail\n", __func__);
        devm_kfree(dev, dev_info);
        return ret;
    }

    INIT_DELAYED_WORK(&dev_info->sppm_task, sppm_bottom_half_work);
    spin_lock_init(&dev_info->synclock);
    dev_info->status = 0x01;
    sppm_reset_en();
    sppm_reset_dis();
    sppm_info("sppm end.\n");
    return SPPM_OK;
}

sppm_dev_info *sppm_get_dev_info(void)
{
    return g_sppm_info;
}

/*******************************************************************************
 * Function:		sppm_probe
 * ********************************************************************************/
static int sppm_probe(struct platform_device *pdev)
{
    static int probe_cnt = 0;

    if (probe_cnt == 0) {
        sppm_ctrl_probe(pdev);
        probe_cnt++;
    }
    return 0;
}

static int sppm_remove(struct platform_device *pdev)
{
    return 0;
}

int sppm_dev_info_show(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    sppm_info("sppm devinfo detail:\n");
    sppm_info("version id %d\n", dev_info->version_id);
    sppm_info("platform name %s\n", dev_info->platform_name);
    sppm_info("rchan start idx AP core %d\n", dev_info->rchan_start_idx);
    sppm_info("rchan at AP core %d\n", dev_info->rchan_num);
    sppm_info("sample type %d(threshold 0, circular 1)\n", dev_info->sample_type);
    sppm_info("irq config at dts %d\n", dev_info->irq);
    sppm_info("rinbuffer size %d\n", dev_info->rb_size);

    return SPPM_OK;
}

int sppm_ui_mode_config(sppm_ipc_buf_config *pconfig)
{
    sppm_dev_info *dev_info = g_sppm_info;
    if ((dev_info->rb_size != pconfig->buffer_size) && (dev_info->virt_addr != 0x00UL)) {
        sppm_dst_ringbuff_release(dev_info);
    }

    dev_info->rb_size = pconfig->buffer_size;
    if (dev_info->virt_addr == 0x00UL) {
        if (sppm_dst_ringbuff_alloc(dev_info) != SPPM_OK) {
            return SPPM_ERROR;
        }
    }

    dev_info->sample_type = pconfig->sample_type;
    dev_info->dest_chan = pconfig->dest_chan;
    return SPPM_OK;
}

// 配置水线，打开水线中断，配置ringbuff
int sppm_device_config(void)
{
    sppm_dev_info *dev_info = g_sppm_info;
    if (dev_info->rb_phy_addr == 0x00UL) {
        return SPPM_ERROR;
    }
    sppm_set_rb_wptr(0);
    sppm_set_rb_rptr(0);
    sppm_set_ringbuff_mode(dev_info->sample_type);
    sppm_set_ringbuff_base_addr(dev_info->rb_phy_addr);
    sppm_set_ringbuff_size((dev_info->rb_size) >> SPPM_RB_SIZE_SHIFT_GRANULARITY);
    sppm_set_ringbuff_int_threshold((dev_info->rb_size) >> SPPM_RB_SHRESHOLD_SHIFT_GRANULARITY);  // 水线设置为一半
    sppm_clear_int_status(SPPM_INT_ALL);
    return SPPM_OK;
}

static struct platform_device g_sppm_device = {
    .name = "HiSPPM",
};

static struct platform_driver g_sppm_driver = {
    .driver = {.name = "HiSPPM", .owner = THIS_MODULE},
    .probe = sppm_probe,
    .remove = sppm_remove,
};

int sppm_init(void)
{
    int ret = SPPM_OK;
    ret = platform_device_register(&g_sppm_device);
    if (ret) {
        sppm_err("sppm dev register fail.\n");
        return ret;
    }
    ret = platform_driver_register(&g_sppm_driver);
    if (ret) {
        sppm_err("<%s> platform_driver_register failed %d\n", __func__, ret);
    }

    return ret;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(sppm_init);
#endif
