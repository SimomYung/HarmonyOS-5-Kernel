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
#include <linux/pm_runtime.h>
#include <linux/of_device.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_dump.h>
#include <bsp_om_enum.h>
#include <pm.h>
#include "dump.h"
#include "debugfs.h"

#define DPA_PM_DUMP_SIZE 400
static void dpa_pm_dump_cb(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    dpa_pm_mntn_s *mntn = NULL;
    dpa_debugfs_ctrl_s *dbgfs_ctrl = NULL;
    int ret;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        dpa_pm_fatal("ctrl is null. \n");
        return;
    }

    mntn = dpa_pm_get_mntn();
    if (mntn == NULL) {
        dpa_pm_fatal("mntn is null. \n");
        return;
    }

    ret = memcpy_s(ctrl->dump_base, ctrl->dump_size, mntn, sizeof(dpa_pm_mntn_s));
    if (ret) {
        dpa_pm_err("dpa pm dump fail.\n");
        return;
    }

    dbgfs_ctrl = dpa_dfs_get_ctrl();
    if (dbgfs_ctrl == NULL) {
        dpa_pm_fatal("dbgfs_ctrl is null. \n");
        return;
    }
    ret = memcpy_s(ctrl->dump_base + sizeof(dpa_pm_mntn_s), ctrl->dump_size - sizeof(dpa_pm_mntn_s), dbgfs_ctrl, sizeof(dpa_debugfs_ctrl_s));
    if (ret) {
        dpa_pm_err("dpa dbgfs dump fail.\n");
        return;
    }
}

int dpa_pm_dump_init(void)
{
    int ret;
    dpa_pm_ctrl_s *ctrl;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        dpa_pm_fatal("ctrl is null. \n");
        return -1;
    }

    ctrl->dump_size = ctrl->dump_size ? ctrl->dump_size : DPA_PM_DUMP_SIZE;

    ctrl->dump_base = (char *)bsp_dump_register_field(DUMP_MODEMAP_DPA_PM, "dpa-pm", ctrl->dump_size, 0);
    if (unlikely(ctrl->dump_base == NULL)) {
        dpa_pm_fatal("dpa pm dump register failed\n");
        return -1;
    }

    (void)memset_s(ctrl->dump_base, ctrl->dump_size, 0, ctrl->dump_size);

    ret = bsp_dump_register_hook("dpa-pm", dpa_pm_dump_cb);
    if (ret == BSP_ERROR) {
        dpa_pm_fatal("dpa-pm dump register fail\n");
        return -1;
    }

    return 0;
}