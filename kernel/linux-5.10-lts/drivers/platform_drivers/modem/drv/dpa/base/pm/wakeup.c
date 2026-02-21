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
#include <bsp_ipcmsg.h>
#include <bsp_ipc_fusion.h>
#include <bsp_print.h>
#include <pm.h>

static ipc_handle_t g_lockup_phandle = NULL;
static ipc_handle_t g_wakeup_phandle = NULL;
static ipc_handle_t g_pm_debug_phandle = NULL;

int dpa_wakeup(unsigned int wakelock)
{
    int ret, cnt = 1000;

    if (wakelock == 0) {
        ret = bsp_ipc_send(g_lockup_phandle);
        if (ret) {
            return ret;
        }

        while (bsp_dpapm_get_dpa_access_state(DPA_REGS_ACCESS_TSP_DSS0) && cnt >= 0) {
            cnt--;
            udelay(10);
        }
        if (cnt < 0) {
            bsp_err("dpa wakeup fail.\n");
            return -1;
        } else {
            bsp_err("dpa wakeup suscess.\n");
            return 0;
        }
    } else {
        ret = bsp_ipc_send(g_wakeup_phandle);
        if (ret) {
            return ret;
        }

        while (bsp_dpapm_get_dpa_access_state(DPA_REGS_ACCESS_DPA_SYSCTRL_CRG) == 0 && cnt >= 0) {
            cnt--;
            udelay(10);
        }
        if (cnt < 0) {
            bsp_err("dpa sleep fail.\n");
            return -1;
        } else {
            bsp_err("dpa sleep suscess.\n");
            return 0;
        }
    }
}

int dpa_pm_wakeup_init(void)
{
    int ret;

    ret = bsp_ipc_open(IPC_DPA_INT_SRC_ACPU_LOCKUP, &g_lockup_phandle);
    if (ret) {
        bsp_err("dpa wakeup open ipc-1 fail.\n");
        return ret;
    }

    ret = bsp_ipc_open(IPC_DPA_INT_SRC_ACPU_WAKEUP, &g_wakeup_phandle);
    if (ret) {
        bsp_err("dpa wakeup open ipc-2 fail.\n");
        return ret;
    }

    ret = bsp_ipc_open(IPC_DPA_INT_SRC_ACPU_PM_DEBUG_REPORT, &g_pm_debug_phandle);
    if (ret) {
        bsp_err("dpa pm open ipc fail.\n");
        return ret;
    }


    return 0;
}

int bsp_dpa_wakeup(unsigned int wakelock)
{
    return dpa_wakeup(wakelock);
}

int dpa_pm_debug_report(void)
{
    return bsp_ipc_send(g_pm_debug_phandle);
}

EXPORT_SYMBOL(bsp_dpa_wakeup);