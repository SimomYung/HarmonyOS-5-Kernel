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
#include <linux/firmware.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/io.h>
#include <product_config.h>
#include <mdrv_memory_layout.h>
#include <bsp_version.h>
#include <securec.h>
#include "pm.h"
#include "rpm.h"
#include "load.h"
#include "bsp_dpa.h"
#include "common.h"

#define FIRMWARE_NAME "dpa_fw.bin"

#define CMD_INIT_NAME "dpa"

struct device g_dpa_dev;
unsigned int g_load_flag = 0;

dpa_unreset_hook_ops g_unreset_hook_ops[DPA_UNRESET_HOOK_TYPE_MAX];

int bsp_dpa_unreset_hook_register(unreset_type_e type, dpa_unreset_hook_ops ops)
{
    if (type >= DPA_UNRESET_HOOK_TYPE_MAX) {
        return DPA_PM_ERR;
    }
    if (ops == NULL) {
        return DPA_PM_ERR;
    }
    g_unreset_hook_ops[type] = ops;

    return DPA_PM_OK;
}

static DEFINE_MUTEX(dpa_fw_mutex);

static int dpa_load_fw_para_check(struct image_file_head *image_head)
{
    unsigned int i;

    if (image_head->file_num < 2) {
        dpa_pm_fatal("file_num < 2, please check.\n");
        return -1;
    }

    for (i = 0; i < 2; i++) { // 只校验itcm和dtcm，test code不校验
        if (image_head->file_info[i].offset == 0 ||  image_head->file_info[i].file_size == 0) {
            dpa_pm_fatal("file_info[%u] offset or size null, please check.\n", i);
            return -1;
        }
    }

    return 0;
}

static int dpa_load_addr_map(void **dpa_it_virt_addr, unsigned int *dpa_it_size)
{
    unsigned int dpa_it_phy_addr;

    dpa_it_phy_addr = mdrv_mem_region_get("dpa_it_ddr", dpa_it_size);
    if (dpa_it_phy_addr != 0) {
        // ioremap_wc 映射是mem属性，访问不要求4B对齐
        *dpa_it_virt_addr = (void *)ioremap_wc((phys_addr_t)dpa_it_phy_addr, (size_t)(*dpa_it_size));
        if (*dpa_it_virt_addr == NULL) {
            dpa_pm_fatal("ioremap dpa it addr fail.\n");
        }
    } else {
        dpa_pm_fatal("get dpa it addr err.\n");
    }

    return 0;
}

void dpa_load_fw(dpa_pm_ctrl_s *ctrl, char *firmware, int size)
{
    int ret;
    void *dst_addr = NULL;
    void *dpa_it_virt_addr = NULL;
    unsigned int dst_size, dpa_it_size, i;
    struct image_file_head *image_head = (struct image_file_head *)firmware;

    ret = dpa_load_fw_para_check(image_head);
    if (ret) {
        dpa_pm_fatal("dpa_load_fw para check fail.\n");
        return;
    }

    ret = dpa_load_addr_map(&dpa_it_virt_addr, &dpa_it_size);
    if (ret) {
        dpa_pm_fatal("dpa_load addr map fail.\n");
        return;
    }

    for (i = 0; i < image_head->file_num; i++) {
        if (i == 0) {
            dst_addr = ctrl->reg_info[DPA_ITCM_GLOCAL].virt_addr;
            dst_size = ctrl->reg_info[DPA_ITCM_GLOCAL].size;
            ctrl->image_itcm_size = image_head->file_info[i].file_size;
            // itcm bakeup
            ret = memcpy_s(ctrl->itcm_bak, ctrl->reg_info[DPA_ITCM_GLOCAL].size, firmware + image_head->file_info[i].offset, image_head->file_info[i].file_size);
            if (ret) {
                dpa_pm_err("itcm back up fail.\n");
                return;
            }
        } else if (i == 1) {
            dst_addr = ctrl->reg_info[DPA_DTCM_GLOCAL].virt_addr;
            dst_size = ctrl->reg_info[DPA_DTCM_GLOCAL].size;
            ctrl->image_dtcm_size = image_head->file_info[i].file_size;
        } else {
            if (dpa_it_virt_addr == NULL) {
                dpa_pm_fatal("get dpa it addr err.\n");
                break;
            }
            dst_addr = dpa_it_virt_addr;
            dst_size = dpa_it_size;
        }

        if (i == 0 || i == 1) {
            memcpy_toio(dst_addr, firmware + image_head->file_info[i].offset, image_head->file_info[i].file_size);
        } else {
            ret = memcpy_s(dst_addr, dst_size, firmware + image_head->file_info[i].offset, image_head->file_info[i].file_size);
            if (ret) {
                dpa_pm_fatal("load image %u segment err.\n", i);
            }
        }
    }

    return;
}

void dpa_ddr_config(dpa_pm_ctrl_s *ctrl)
{
    unsigned int dpa_ddr_addr, dpa_ddr_size;

    // dt ddr
    dpa_ddr_addr = mdrv_mem_region_get("dpa_it_ddr", &dpa_ddr_size);
    if (ctrl->cb_ops.set_ddr_addr_info != NULL) {
        ctrl->cb_ops.set_ddr_addr_info(dpa_ddr_addr, dpa_ddr_size, SC_SOFT_RSV_CTRL0, SC_SOFT_RSV_CTRL1);
    }

    // sc ddr
    dpa_ddr_addr = mdrv_mem_region_get("dpa_ddr", &dpa_ddr_size);
    if (ctrl->cb_ops.set_ddr_addr_info != NULL) {
        ctrl->cb_ops.set_ddr_addr_info(dpa_ddr_addr, dpa_ddr_size, SC_SOFT_RSV_CTRL6, SC_SOFT_RSV_CTRL7);
    }
}

void trigger_async_request_cb(const struct firmware *fw, void *context)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    unsigned int i;

    if (fw == NULL) {
        mutex_unlock(&dpa_fw_mutex);
        dpa_pm_fatal("firmware is null, failed to async load firmware: '%s' \n", (char *)context);
        return;
    }

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        dpa_pm_fatal("ctrl is null. \n");
        return;
    }

    dpa_load_fw(ctrl, (char *)fw->data, fw->size);
    dpa_pm_err("'%s' loaded size: %u\n", (char *)context, (unsigned int)fw->size);

    // 释放固件资源
    release_firmware(fw);
    mutex_unlock(&dpa_fw_mutex);

    // DPA管理的DDR配置
    dpa_ddr_config(ctrl);

    for (i = 0; i < DPA_UNRESET_HOOK_TYPE_MAX; i++) {
        if (g_unreset_hook_ops[i] == NULL) {
            continue;
        }
        // 如果执行失败，注册模块内部做维测计数
        g_unreset_hook_ops[i]();
    }

    // tsp核解复位;因为是异步操作，解复位动作只能在镜像加载完成后进行;
    ctrl->cb_ops.dss_unreset(0);
    return;
}

int dpa_pm_load_firmware(void)
{
    int ret;
    char *name = "dpa_fw.bin";

    mutex_lock(&dpa_fw_mutex);

    // 加载固件
    ret = request_firmware_nowait(THIS_MODULE, 1, FIRMWARE_NAME, &g_dpa_dev, GFP_KERNEL, name, trigger_async_request_cb);
    if (ret) {
        dpa_pm_fatal("async load of '%s' failed: %d\n", name, ret);
        return ret;
    }

    return DPA_PM_OK;
}

static ssize_t dpa_load_show(struct device *pdev, struct device_attribute *attr, char *buf)
{
    return g_load_flag;
}

static ssize_t dpa_load_store(struct device *pdev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret;

    if ((kstrtouint(buf, 0, &g_load_flag) < 0)) {
        dpa_pm_fatal("dpa_load_store read load flag err. \n");
        return count;
    }

    if (g_load_flag <= 0) {
        dpa_pm_fatal("dpa_load_store load flag = %u. \n", g_load_flag);
        return count;
    } else {
        ret = dpa_pm_load_firmware();
        if (ret) {
            dpa_pm_fatal("dpa_pm_load_firmware fail.\n");
            return count;
        }
    }

    return count;
}

static DEVICE_ATTR(dpa_load, S_IWUSR, dpa_load_show, dpa_load_store);

int dpa_load_init(void)
{
    int ret;

    g_dpa_dev.init_name = CMD_INIT_NAME;
    device_initialize(&g_dpa_dev);
    ret = device_add(&g_dpa_dev);
    if (ret) {
        dpa_pm_err("device_add failed.   %d\n", ret);
        return -EIO;
    }

    ret = device_create_file(&g_dpa_dev, &dev_attr_dpa_load);
    if (ret) {
        dpa_pm_err("pfa device_create_file \n");
        return -EIO;
    }

    return DPA_PM_OK;
}

void dpa_load_exit(void)
{
    device_remove_file(&g_dpa_dev, &dev_attr_dpa_load);
    device_del(&g_dpa_dev);

    return;
}

int bsp_dpa_load_init(void)
{
    return dpa_load_init();
}

void bsp_dpa_load_exit(void)
{
    dpa_load_exit();
    return;
}