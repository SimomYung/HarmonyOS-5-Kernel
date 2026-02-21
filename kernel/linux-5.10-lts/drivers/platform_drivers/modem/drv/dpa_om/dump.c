/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2023. All rights reserved.
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
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <osl_bio.h>
#include <bsp_print.h>
#include <bsp_dump.h>
#include <bsp_fiq.h>
#include <bsp_ipc_fusion.h>
#include <bsp_dpa.h>
#include <bsp_wdt.h>
#include <bsp_slice.h>
#include <bsp_fipc.h>
#include <mdrv_errno.h>
#include "ocd.h"

#undef THIS_MODU
#define THIS_MODU mod_dump
#define DPA_NUM 2
#define DPA_32K 32768
#define DPA_DELAY 100
#define SC_SOFT_RSV_CTRL2 0xA08
#define DPA_SAVE_BITS 3
#define DPA_SAVE_DONE 3
u32 g_dpa_dump_exc;
spinlock_t g_dpa_dump_spinlock;
static void dump_dap_save_tcm_backup(const char *path)
{
    struct dpa_reg_info dtcm_info, itcm_info;
    int ret = -1;
    dtcm_info.virt_addr = NULL;
    itcm_info.virt_addr = NULL;
    bsp_dpa_tcm_bak_info_get(&dtcm_info, &itcm_info);
    if (dtcm_info.virt_addr != NULL && dtcm_info.size != 0) {
        ret = bsp_dump_log_save(path, "dpa_dtcm.bin", (void *)(uintptr_t)dtcm_info.virt_addr,
            (void *)(uintptr_t)dtcm_info.phy_addr, dtcm_info.size);
    }
    if (ret) {
        bsp_err("save dpa tcm backup err, %d\n", ret);
    }
}
static void dump_dpa_save_tcm(const char *path, const char *file_name, reg_ctrl_e id, struct dpa_reg_info *tcm_info)
{
    int ret;
    tcm_info->virt_addr = NULL;
    tcm_info->size = 0;
    bsp_dpa_reg_info_get(id, tcm_info);
    if (tcm_info->virt_addr == NULL || tcm_info->size == 0) {
        bsp_err("no dpa dtcm addr\n");
        return;
    }
    ret = bsp_dump_log_save(path, file_name, (void *)(uintptr_t)tcm_info->virt_addr,
        (void *)(uintptr_t)tcm_info->phy_addr, tcm_info->size);
    if (ret) {
            bsp_err("save dpa tcm err %d, id = %d\n", ret, id);
    } else {
        bsp_err("save dpa tcm done, id = %d\n", id);
    }
    return;
}
static void dump_dpa_save(const char *path)
{
    unsigned int reg_value, start_slice, cur_slice;
    struct dpa_reg_info info = { 0 };
    const dump_exc_info_s *exc_info = bsp_dump_get_current_exc_info();
    /* 当前策略，单独复位，不存dpa日志,待低功耗调测完成后再加上保存dpa日志 */
    if (g_dpa_dump_exc == 0 && (exc_info != NULL && exc_info->exc_cfg != NULL && exc_info->exc_cfg->reset_subsys_mask != DUMP_SUBSYS_MASK_AP)) {
        return;
    }
    // 保存ltrace
    ocd_ltrace_save_all();
    // 判断dpa是否上电
    if (bsp_dpapm_get_dpa_access_state(DPA_REGS_ACCESS_TSP_DSS0)) {
        bsp_err("dpa sleep, save backup to fs\n");
        dump_dap_save_tcm_backup(path);
        return;
    }
    // 在mdm单独复位中，dpa可能睡眠，需要持锁让dpa不睡。
    // 等待超时或者标记位生成
    start_slice = bsp_get_slice_value();
    cur_slice = start_slice;
    info.virt_addr = 0;
    bsp_dpa_reg_info_get(DPA_SYSCTRL_BASE, &info);
    if (info.virt_addr == NULL || info.size == 0) {
        bsp_err("dpa sysctrl null\n");
    } else {
        reg_value = 0;
        while (get_timer_slice_delta(start_slice, cur_slice) < DPA_32K) {
            reg_value = readl((void *)((uintptr_t)info.virt_addr + SC_SOFT_RSV_CTRL2));
            if ((reg_value & DPA_SAVE_BITS) == DPA_SAVE_DONE) {
                break;
            }
            udelay(DPA_DELAY);
            cur_slice = bsp_get_slice_value();
        }
        bsp_err("dpa sysctrl status: 0x%x, %s\n", reg_value, (reg_value & DPA_SAVE_BITS) == DPA_SAVE_DONE ? "ok" : "timeout");
    }
    dump_dpa_save_tcm(path, "dpa_dtcm.bin", DPA_DTCM_GLOCAL, &info);
    return;
}
static void dpa_dtcm_backup(void)
{
    struct dpa_reg_info dtcm_backup, itcm_info;
    struct dpa_reg_info tcm_info;
    u32 dtcm_image_size;
    int ret = -1;
    dtcm_backup.virt_addr = NULL;
    itcm_info.virt_addr = NULL;
    tcm_info.virt_addr = NULL;

    bsp_dpa_reg_info_get(DPA_DTCM_GLOCAL, &tcm_info);
    if (tcm_info.virt_addr == NULL || tcm_info.size == 0) {
        bsp_err("dpa dtcm addr err\n");
        return;
    }
    bsp_dpa_tcm_bak_info_get(&dtcm_backup, &itcm_info);
    // 在ipc和nmi的dump流程中，dpa不会睡眠，因此，此处不需要持dpa的锁。但是在mdm单独复位中，dpa可能睡眠，需要持锁让dpa不睡。
    if (dtcm_backup.virt_addr != NULL && dtcm_backup.size != 0) {
        dtcm_image_size = tcm_info.size < dtcm_backup.size ? tcm_info.size : dtcm_backup.size;
        ret = memcpy_s((void *)(uintptr_t)dtcm_backup.virt_addr, dtcm_backup.size,
            (void *)(uintptr_t)tcm_info.virt_addr, dtcm_image_size);
    }
    if (ret) {
        bsp_err("backup dpa dtcm err, %d\n", ret);
    } else {
        bsp_err("backup dpa dtcm ok\n");
    }
}
static void dump_dpa_work(void)
{
    // 停wdt
    bsp_wdt_dpa_disable();
    // 停ltrace
    ocd_ltrace_stop_all();
    /* 当前策略，dpa低功耗时，不保存dpa数据 */
    if (bsp_dpapm_get_dpa_access_state(DPA_REGS_ACCESS_TSP_DSS0)) {
        bsp_err("dpa sleep, no need backup ddr\n");
        return;
    }
    // FIXME: 备份dtcm到ddr
    dpa_dtcm_backup();
    // fipc backup
    bsp_dpa_fipc_dump();
}
static void dump_dpa_notify(void)
{
    // 判断dpa是否已经异常
    unsigned long flags = 0;
    const dump_exc_info_s *exc_info = bsp_dump_get_current_exc_info();
    spin_lock_irqsave(&g_dpa_dump_spinlock, flags);
    /* dpa先异常，不需要通知DPA复位 */
    /* 当前策略，单独复位，不通知DPA复位 */
    if (g_dpa_dump_exc || (exc_info != NULL && exc_info->exc_cfg != NULL && exc_info->exc_cfg->reset_subsys_mask != DUMP_SUBSYS_MASK_AP)) {
        spin_unlock_irqrestore(&g_dpa_dump_spinlock, flags);
        return;
    }
    g_dpa_dump_exc = 1;
    spin_unlock_irqrestore(&g_dpa_dump_spinlock, flags);
    bsp_err("notify dpa\n");
    // 通知dpa复位
    bsp_send_dpa_nmi(DPA_NMI_DUMP);
    dump_dpa_work();
}
void dpa_dump_ipc_handler(void *arg)
{
    unsigned long flags = 0;
    spin_lock_irqsave(&g_dpa_dump_spinlock, flags);
    if (g_dpa_dump_exc) {
        spin_unlock_irqrestore(&g_dpa_dump_spinlock, flags);
        return;
    }
    g_dpa_dump_exc = 1;
    spin_unlock_irqrestore(&g_dpa_dump_spinlock, flags);
    bsp_err("dpa exc\n");
    dump_dpa_work();
    // 调用system_error整机复位
    system_error(DRV_ERRNO_DPA_EXC, 0, 0, NULL, 0);
}
s32 dpa_dump_ipc_init(void)
{
    int ret;
    ipc_handle_t phandle = NULL;

    ret = bsp_ipc_open(IPC_ACPU_INT_SRC_DPA_DUMP, &phandle);
    if (ret != BSP_OK) {
        bsp_err("fail to open ipc\n");
        return BSP_ERROR;
    }
    ret = bsp_ipc_connect(phandle, (ipc_cbk)dpa_dump_ipc_handler, NULL);
    if (ret != BSP_OK) {
        bsp_err("bsp_ipc_connect\n");
        return BSP_ERROR;
    }
    return BSP_OK;
}
int bsp_dpa_dump_init(void)
{
    s32 ret;
    spin_lock_init(&g_dpa_dump_spinlock);
    ret = dpa_dump_ipc_init();
    if (ret != BSP_OK) {
        bsp_err("register dpa ipc fail\n");
    }
    ret = bsp_dump_register_hook("dpa_dump", dump_dpa_notify);
    if (ret == BSP_ERROR) {
        bsp_err("cannot notify dpa\n");
        return ret;
    }
    return bsp_dump_register_log_notifier(DUMP_SAVE_DPA_DTCM, dump_dpa_save);
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(bsp_dpa_dump_init);
#endif
