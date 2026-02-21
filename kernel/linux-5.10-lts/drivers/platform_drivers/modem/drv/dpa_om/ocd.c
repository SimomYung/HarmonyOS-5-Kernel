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
#include "ocd.h"
#include <bsp_dpa.h>

#undef THIS_MODU
#define THIS_MODU "dpa_ocd"

struct ocd_ltrace_info_s *g_ltrace_info[DPA_OCD_NUMS];
struct ocd_ltrace_cfg_s g_ltrace_cfg;

void ltrace_thread_disable_hw(unsigned int traceid)
{
    unsigned int val, is_wrapped, wrptr;
    writel(g_ltrace_cfg.disable_cfg, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
    // Read OCD_LTRACE_CTL and save the value of WRAP field.
    val = readl((void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
    is_wrapped = (val & OCD_LTRACE_CTL_WRAP) ? 1 : 0;
    g_ltrace_info[traceid]->is_wrapped = is_wrapped;
    // Read the value of LTRACE write pointer OCD_LTRACE_WRPTR.
    wrptr = readl((void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_WRPTR)) / LTRACE_DATA_WIDTH;
    g_ltrace_info[traceid]->wrptr = wrptr;
}

static char* ltrace_save_data_v300(u32 wrptr, u32 maxbuf, u32 traceid, char *bufp)
{
    errno_t mret;
    u32 i, read_data, ptr_val, is_wrapped;
    is_wrapped = g_ltrace_info[traceid]->is_wrapped;
    ptr_val = 0xFFFFFFFF;
    if (is_wrapped == 0) {
        maxbuf = wrptr;
        // set OCD_LTRACE_PTR=0
        writel(0, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_WRPTR));
    }
    wrptr = wrptr * LTRACE_DATA_WIDTH;

    // read OCD_LTRACE_RDATA register for saved_pointer/4 number of times
    // or until the address of the pointer is equal to saved_pointer.
    for (i = 0; (i < maxbuf) && (ptr_val != wrptr); i++) {
        if (bufp >= (g_ltrace_info[traceid]->buf + g_ltrace_info[traceid]->size +
            sizeof(struct trace_data_head_info))) {
            mdrv_err("read tmc > buf i= 0x%x, ptr_val = 0x%x\n", i, ptr_val);
            break;
        }
        read_data = readl((void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_RDATA(0)));
        mret = memcpy_s(bufp, LTRACE_DATA_WIDTH, &read_data, LTRACE_DATA_WIDTH);
        if (mret) {
            mdrv_err("memcpy fail i= 0x%x, ptr_val = 0x%x\n", i, ptr_val);
            break;
        }
        bufp += LTRACE_DATA_WIDTH;
        ptr_val = readl((void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_WRPTR));
    }
    return bufp;
}
static char *ltrace_save_data(char *bufp, u32 traceid)
{
    unsigned int maxbuf = g_ltrace_cfg.buf_len / LTRACE_DATA_WIDTH;
    unsigned int wrptr = g_ltrace_info[traceid]->wrptr;
    char *buf_addr = bufp;
    if (wrptr > maxbuf) {
        mdrv_err("wrptr = 0x%x err\n", wrptr);
        return buf_addr;
    }
    switch (g_ltrace_cfg.version) {
        case OCD_VERSION_LTRACE_V300:
            buf_addr = ltrace_save_data_v300(wrptr, maxbuf, traceid, bufp);
            break;
        default:
            break;
    }
    return buf_addr;
}
unsigned int ltrace_get_status(unsigned int traceid)
{
    unsigned int val;

    if (traceid >= DPA_OCD_NUMS || g_ltrace_info[traceid] == NULL) {
        mdrv_err("ltrace %d is not err.\n", traceid);
        return LTRACE_DISABLED;
    }
    if (g_ltrace_info[traceid]->process == DPA_OCD_SUSPEND_PROCESS) {
        return LTRACE_DISABLED;
    }
    // Check that LTRACE is disabled in OCD_LTRACE_CTL (ENABLED = 0, TRI_EN = 0, STATUS = 0, MODE = 0 (Circular),
    // FILTER0/1 = 0, CNT = 0).
    val = readl((void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
    if (val & (OCD_LTRACE_CTL_ENABLED | OCD_LTRACE_CTL_TRI_EN | OCD_LTRACE_CTL_STATUS | OCD_LTRACE_CTL_MODE |
        OCD_LTRACE_CTL_CNT)) {
        return LTRACE_ENABLED;
    } else {
        return LTRACE_DISABLED;
    }
}
void ltrace_thread_disable(unsigned int traceid)
{
    unsigned long flags = 0;
    struct trace_data_head_info *head_info = (struct trace_data_head_info *)g_ltrace_info[traceid]->buf;
    if (head_info->magic == OCD_HOTPLUG_MAGICNUM) {
        return;
    }
    if (g_ltrace_info[traceid]->process == DPA_OCD_SUSPEND_PROCESS) {
        spin_lock_irqsave(&g_ltrace_info[traceid]->spinlock, flags);
        head_info->magic = OCD_HOTPLUG_MAGICNUM;
        writel(g_ltrace_cfg.disable_cfg, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
        spin_unlock_irqrestore(&g_ltrace_info[traceid]->spinlock, flags);
    } else {
        ltrace_thread_disable_hw(traceid);
    }
}

void ltrace_thread_enable(unsigned int traceid)
{
    unsigned long flags = 0;
    struct trace_data_head_info *head_info = NULL;
    head_info = (struct trace_data_head_info *)g_ltrace_info[traceid]->buf;

    if (ltrace_get_status(traceid) == LTRACE_ENABLED) {
        spin_lock_irqsave(&g_ltrace_info[traceid]->spinlock, flags);
        writel(g_ltrace_cfg.disable_cfg, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
        spin_unlock_irqrestore(&g_ltrace_info[traceid]->spinlock, flags);
    }

    // Write 0 to OCD_LTRACE_WRPTR to set it to the beginning of the trace buffer.
    writel(0, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_WRPTR));
    writel(g_ltrace_cfg.enable_cfg, (void *)((uintptr_t)g_ltrace_info[traceid]->base + OCD_LTRACE_CTL));
    mb();
    head_info->magic = 0;
    return;
}
static int ltrace_get_basic_cfg(struct device_node *dev_node, struct ocd_ltrace_info_s *drvdata)
{
    u32 cpu_index = 0;
    u32 thread_index = 0;
    u32 phy_index = 0;
    if (bsp_dt_property_read_u32_array(dev_node, "cpu_index", &cpu_index, 1) != MDRV_OK || cpu_index >= DPA_CORE_NUMS) {
        mdrv_err("no cpu_index found %x\n", cpu_index);
        return MDRV_ERROR;
    }
    drvdata->cpu = cpu_index;

    if (bsp_dt_property_read_u32_array(dev_node, "thread_index", &thread_index, 1) != MDRV_OK || thread_index >= DPA_OCD_NUMS) {
        mdrv_err("no thread_index found %x\n", thread_index);
        return MDRV_ERROR;
    }
    drvdata->thread = thread_index;

    if (bsp_dt_property_read_u32_array(dev_node, "phy_index", &phy_index, 1) != MDRV_OK || phy_index >= DPA_THREAD_MAX) {
        mdrv_err("no phy_index found %x\n", phy_index);
        return MDRV_ERROR;
    }
    drvdata->base = OCD_BASEADDR(g_ltrace_cfg.tsp_base, cpu_index, phy_index);
    return 0;
}

/* register dump mem */
char *ocd_register_buffer(unsigned int traceid, unsigned int len, u32 version)
{
    char name[DPA_OCD_NUMS][FIELD_NAMELEN_MAX] = { "dpa_trace0", "dpa_trace1" };

    if (traceid >= DPA_OCD_NUMS) {
        mdrv_err("invalid traceid.\n");
        return NULL;
    }
    return (char *)mdrv_om_register_field(DUMP_KERNEL_DPA_LTRACE0 + traceid, name[traceid], len, version);
}
void ltrace_probe(struct device_node *dev_node)
{
    struct ocd_ltrace_info_s *drvdata = NULL;
    u32 traceid;
    mdrv_err("ltrace_probe enter\n");
    drvdata = (struct ocd_ltrace_info_s *)(uintptr_t)osl_malloc(sizeof(struct ocd_ltrace_info_s));
    if (drvdata == NULL) {
        mdrv_err("malloc drvdata failed\n");
        return;
    }

    (void)memset_s(drvdata, sizeof(struct ocd_ltrace_info_s), 0, sizeof(struct ocd_ltrace_info_s));
    if (ltrace_get_basic_cfg(dev_node, drvdata)) {
        osl_free(drvdata);
        return;
    }
    traceid = drvdata->thread;

    drvdata->buf = ocd_register_buffer(traceid, GET_TRACE_DUMP_LEN(g_ltrace_cfg.buf_len), g_ltrace_cfg.version);
    if (drvdata->buf == NULL) {
        mdrv_err("get trace buff failed.\n");
        osl_free(drvdata);
        return;
    }

    drvdata->size = g_ltrace_cfg.buf_len;
    spin_lock_init(&drvdata->spinlock);

    if (traceid < DPA_OCD_NUMS) {
        g_ltrace_info[traceid] = drvdata;
    } else {
        mdrv_err("no thread index found!\n");
        osl_free(drvdata);
        return;
    }

    return;
}

void ocd_ltrace_suspend(void)
{
    unsigned int j;
    for (j = 0; j < DPA_OCD_NUMS; j++) {
        if (g_ltrace_info[j]) {
            g_ltrace_info[j]->process = DPA_OCD_SUSPEND_PROCESS;
            ltrace_thread_disable(j);
        }
    }
}
void ocd_clk_enable(void)
{
    u32 val;
    void *reg_addr = NULL;
    if (g_ltrace_cfg.tsp_base == NULL) {
        return;
    }
    reg_addr = g_ltrace_cfg.tsp_base + OCD_DSS0_SCU_REG_SHARE_BASE + SCU_CLK_DIS;
    val = readl(reg_addr);
    val &= (~OCD_CLK_BIT);
    writel(val, reg_addr);
}
void ocd_ltrace_resume(void)
{
    u32 j;
    ocd_clk_enable();
    for (j = 0; j < DPA_OCD_NUMS; j++) {
        if (g_ltrace_info[j]) {
            g_ltrace_info[j]->process = DPA_OCD_NORMAL_PROCESS;
            ltrace_thread_enable(j);
        }
    }
}
EXPORT_SYMBOL_GPL(ocd_ltrace_resume);
static void ocd_ltrace_enable_all(void)
{
    ocd_ltrace_resume();
}

void ocd_ltrace_stop_all(void)
{
    u32 j;
    for (j = 0; j < DPA_OCD_NUMS; j++) {
        if (g_ltrace_info[j]) {
            if ( g_ltrace_info[j]->process == DPA_OCD_SUSPEND_PROCESS) {
                continue;
            }
            ltrace_thread_disable(j);
        }
    }
}
EXPORT_SYMBOL_GPL(ocd_ltrace_stop_all);
static void ltrace_dump_data(unsigned int traceid)
{
    char *bufp = NULL;
    struct trace_data_head_info *head_info = NULL;
    head_info = (struct trace_data_head_info *)g_ltrace_info[traceid]->buf;

    if (head_info == NULL || head_info->magic == OCD_MAGIC_NUM || head_info->magic == OCD_HOTPLUG_MAGICNUM) {
        return;
    }
    /* SR or hotplug process */
    if (g_ltrace_info[traceid]->process == DPA_OCD_SUSPEND_PROCESS) {
        head_info->magic = OCD_HOTPLUG_MAGICNUM;
        return;
    }
    head_info->magic = OCD_MAGIC_NUM;
    head_info->length = 0;
    if (ltrace_get_status(traceid) == LTRACE_ENABLED) {
        ltrace_thread_disable_hw(traceid);
    }
    bufp = ltrace_save_data(g_ltrace_info[traceid]->buf + sizeof(struct trace_data_head_info), traceid);
    head_info->length = (u32)(bufp - g_ltrace_info[traceid]->buf) - sizeof(struct trace_data_head_info);
    return;
}
void ocd_ltrace_save_all(void)
{
    u32 j;
    for (j = 0; j < DPA_OCD_NUMS; j++) {
        if (g_ltrace_info[j]) {
            if ( g_ltrace_info[j]->process == DPA_OCD_SUSPEND_PROCESS) {
                continue;
            }
            ltrace_dump_data(j);
        }
    }
}
EXPORT_SYMBOL_GPL(ocd_ltrace_save_all);
int ocd_ltrace_init(void)
{
    struct device_node *ocd_node = NULL;
    struct device_node *child = NULL;
    struct dpa_reg_info info = { 0 };

    ocd_node = bsp_dt_find_compatible_node(NULL, NULL, "ocd,ltrace,dpa");
    if (ocd_node == NULL) {
        return 0;
    }

    if ( bsp_dt_property_read_u32_array(ocd_node, "ltrace_disable_cfg", &g_ltrace_cfg.disable_cfg, 1)) {
        mdrv_err("no ocd_disable_ctl\n");
        g_ltrace_cfg.disable_cfg = OCD_LTRACE_CTL_MAN_DIS;
    }
    if (bsp_dt_property_read_u32_array(ocd_node, "ltrace_version", &g_ltrace_cfg.version, 1)) {
        mdrv_err("no ltrace_version\n");
        g_ltrace_cfg.version = OCD_VERSION_LTRACE;
    }
    if (bsp_dt_property_read_u32_array(ocd_node, "ltrace_buf_len", &g_ltrace_cfg.buf_len, 1)) {
        mdrv_err("no ltrace_buf_len\n");
        g_ltrace_cfg.buf_len = LTRACE_MAX_BUF;
    }
    bsp_dpa_reg_info_get(DPA_TSP_BASE, &info);
    if (info.virt_addr == NULL) {
        mdrv_err("get info err\n");
        return MDRV_ERROR;
    }
    g_ltrace_cfg.tsp_base = info.virt_addr;
    g_ltrace_cfg.enable_cfg = OCD_LTRACE_CTL_INT | OCD_LTRACE_CTL_SBR | OCD_LTRACE_CTL_COF | OCD_LTRACE_CTL_MAN_EN;
    bsp_dt_for_each_child_of_node(ocd_node, child)
    {
        (void)ltrace_probe(child);
    }
    ocd_ltrace_enable_all();
    return MDRV_OK;
}
struct dpa_rpm_ops g_dpa_ocd_ops = {
    .rpm_suspend = ocd_ltrace_suspend,
    .rpm_resume = ocd_ltrace_resume,
};
void dpa_ltrace_init(void)
{
    int ret;
    if (bsp_get_version_info() == NULL) {
        return ;
    }
    if (bsp_get_version_info()->plat_type == PLAT_ESL || bsp_get_version_info()->plat_type == PLAT_HYBRID ||
        bsp_get_version_info()->cses_type == TYPE_ESL_EMU) {
        mdrv_err("not supported in ESL!\n");
        return ;
    }
    ocd_ltrace_init();
    ret = bsp_dpa_rpm_register(DPA_RPM_FTRACE, &g_dpa_ocd_ops);
    if (ret) {
        mdrv_err("rpm register err\n");
    } else {
        mdrv_err("ltrace ok\n");
    }
}
int bsp_dpa_ocd_init(void)
{
    int ret = bsp_dpa_unreset_hook_register(DPA_UNRESET_HOOK_OCD, (dpa_unreset_hook_ops)&dpa_ltrace_init);
    if (ret) {
        mdrv_err("unrst register err %d\n", ret);
        return ret;
    }
    mdrv_err("init ok\n");
    return ret;
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(bsp_dpa_ocd_init);
#endif