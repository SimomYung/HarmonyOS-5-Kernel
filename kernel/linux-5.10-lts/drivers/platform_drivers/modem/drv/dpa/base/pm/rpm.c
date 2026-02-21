/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/pm_runtime.h>
#include <linux/pm_wakeup.h>
#include <linux/interrupt.h>
#include <linux/of_device.h>
#include <linux/errno.h>
#include <securec.h>
#include "pm.h"
#include "rpm.h"
#include "bsp_dpa.h"
#include <bsp_slice.h>
#include <bsp_diag.h>
#include <bsp_dump.h>
#include <mdrv_errno.h>

struct dpa_rpm_ops *g_rpm_ops[RPM_TYPE_MAX];
spinlock_t g_dpa_rpm_spinlock;

int bsp_dpa_rpm_register(rpm_type_e rpm_type, struct dpa_rpm_ops *dpa_rpm_ops)
{
    if (rpm_type >= RPM_TYPE_MAX) {
        return DPA_PM_ERR;
    }
    if (dpa_rpm_ops == NULL) {
        return DPA_PM_ERR;
    }
    g_rpm_ops[rpm_type] = dpa_rpm_ops;

    return DPA_PM_OK;
}

static int dpa_runtime_probe(struct platform_device *pdev)
{
    dpa_pm_err("dpa_runtime_probe!\n");

    pm_runtime_set_active(&pdev->dev);

    device_init_wakeup(&pdev->dev, true);

    pm_stay_awake(&pdev->dev);

    return 0;
}

static int dpa_runtime_remove(struct platform_device *pdev)
{
    dpa_pm_err("dpa_runtime_remove!\n");
    pm_runtime_disable(&pdev->dev);

    pm_relax(&pdev->dev);

    return 0;
}

int dpa_runtime_suspend(struct device *dev)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int i, time;
    int ret;

    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_0);

    time = bsp_get_slice_value();
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa_runtime_suspend beghin time:0x%x\n", time);

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        dpa_pm_err("dpa_runtime_suspend ctrl null \n");
        return DPA_PM_ERR;
    }

    // step1: DPA L1DTCM备份
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_1);
    memcpy_fromio(ctrl->dtcm_bak, ctrl->reg_info[DPA_DTCM_GLOCAL].virt_addr, ctrl->reg_info[DPA_DTCM_GLOCAL].size);

    // step2: DPA sysctrl备份
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_2);
    if (ctrl->cb_ops.rpm_bak_dpa_sysctrl_reg != NULL) {
        ctrl->cb_ops.rpm_bak_dpa_sysctrl_reg();
    }

    // step3: 使能DPA唤醒源中断（ACPU响应）
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_3);
    if (ctrl->cb_ops.enable_wakeup_irq != NULL) {
        ctrl->cb_ops.enable_wakeup_irq();
    }

    // step4：执行rpm ops回调
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_4);
    for (i = 0; i < RPM_TYPE_MAX; i++) {
        if (g_rpm_ops[i] == NULL || g_rpm_ops[i]->rpm_suspend == NULL) {
            continue;
        }
        // 如果执行失败，注册模块内部做维测计数
        g_rpm_ops[i]->rpm_suspend();
    }

    // step5: 芯片提供的关钟下电流程
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_5);
    ret = dpa_pm_powerdown(ctrl);
    if (ret) {
        dpa_pm_err("dpa power down fail.:ret=%d\n", ret);
        return DPA_PM_ERR;
    }

    // step7: 全系统睡眠投票
    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_6);
    pm_relax(dev);

    time = bsp_get_slice_value();
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa_runtime_suspend end time:0x%x\n", time);

    dpa_pm_record_slice(DPA_PM_FLOW_DOWN, SLICE_STEP_7);

    return 0;
}

int dpa_runtime_force_suspend(void)
{
    int ret;
    struct device *dev = bsp_dpa_get_runtime_dev();

    ret = dpa_runtime_suspend(dev);
    if (ret) {
        dpa_pm_err("dpa_runtime_force_suspend err. \n");
        return ret;
    }

    return ret;
}

int dpa_runtime_resume(struct device *dev)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int i, time;
    int ret;

    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_0);

    time = bsp_get_slice_value();
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa_runtime_resume beghin time:0x%x\n", time);

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return DPA_PM_ERR;
    }

    // step1: 全系统睡眠持票
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_1);
    pm_stay_awake(dev);

    // step2: 芯片提供的上电开钟流程
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_2);
    ret = dpa_pm_startup(ctrl, DPA_PM_UP);
    if (ret) {
        dpa_pm_err("dpa power up fail.:ret=%d\n", ret);
        return DPA_PM_ERR;
    }

    // step3: 执行ops回调
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_3);
    for (i = 0; i < RPM_TYPE_MAX; i++) {
        if (g_rpm_ops[i] == NULL || g_rpm_ops[i]->rpm_resume == NULL) {
            continue;
        }
        // 如果执行失败，注册模块内部做维测计数
        g_rpm_ops[i]->rpm_resume();
    }

    // step4: 恢复sysctrl
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_4);
    if (ctrl->cb_ops.rpm_resume_dpa_sysctrl_reg != NULL) {
        ctrl->cb_ops.rpm_resume_dpa_sysctrl_reg();
    }

    // step5: 恢复L1ITCM、L1DTCM
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_5);
    memcpy_toio(ctrl->reg_info[DPA_ITCM_GLOCAL].virt_addr, ctrl->itcm_bak, ctrl->image_itcm_size);
    memcpy_toio(ctrl->reg_info[DPA_DTCM_GLOCAL].virt_addr, ctrl->dtcm_bak, ctrl->reg_info[DPA_DTCM_GLOCAL].size);

    // step7: tsp核解复位;因为是异步操作，解复位动作只能在镜像加载完成后进行;
    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_6);
    ctrl->cb_ops.dss_unreset(0);

    time = bsp_get_slice_value();
    bsp_diag_drv_log_report(BSP_P_ERR, "dpa_runtime_resume end time:0x%x\n", time);

    dpa_pm_record_slice(DPA_PM_FLOW_UP, SLICE_STEP_7);

    return 0;
}

int dpa_runtime_force_resume(void)
{
    int ret;
    struct device *dev = bsp_dpa_get_runtime_dev();

    ret = dpa_runtime_resume(dev);
    if (ret) {
        dpa_pm_err("dpa_runtime_force_resume err. \n");
        return ret;
    }

    return ret;
}

static const struct dev_pm_ops dpa_runtime_ops = {
    SET_RUNTIME_PM_OPS(dpa_runtime_suspend,
                    dpa_runtime_resume,
                    NULL)
};

static void dpa_runtime_release(struct device * dev)
{
    dpa_pm_err("dpa_runtime_release!\n");
    return;
}

struct platform_device g_dpa_runtime_device = {
    .name       = "dpa_runtime_device",
    .id         = -1,
    .dev        = {
            .release = dpa_runtime_release,
    },
};

static struct platform_driver g_dpa_runtime_driver = {
    .probe      = dpa_runtime_probe,
    .remove     = dpa_runtime_remove,
    .driver     = {
        .owner  = THIS_MODULE,
        .name   = "dpa_runtime_device",
        .pm = &dpa_runtime_ops,
    },
};


static int dpa_runtime_init(void)
{
    dpa_pm_err("dpa_runtime_init\n");
    platform_device_register(&g_dpa_runtime_device);
    platform_driver_register(&g_dpa_runtime_driver);
    spin_lock_init(&g_dpa_rpm_spinlock);

    return 0;
}

static void dpa_runtime_exit(void)
{
    dpa_pm_err("dpa_runtime_exit\n");
    platform_driver_unregister(&g_dpa_runtime_driver);
    platform_device_unregister(&g_dpa_runtime_device);
}

EXPORT_SYMBOL(bsp_dpa_rpm_register);
late_initcall(dpa_runtime_init);
module_exit(dpa_runtime_exit);
MODULE_LICENSE("GPL");

struct device *bsp_dpa_get_runtime_dev(void)
{
    return &g_dpa_runtime_device.dev;
}
