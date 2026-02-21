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
#include <pm.h>


static void dpa_pm_wakeup_src_show(unsigned int cur_wake_src)
{
    switch (cur_wake_src) {
        case DPA_WAKEUP_SRC_USB:
            dpa_pm_err("cur_wake_src is usb \n");
            break;
        case DPA_WAKEUP_SRC_GMAC:
            dpa_pm_err("cur_wake_src is gmac \n");
            break;
        case DPA_WAKEUP_SRC_XGMAC:
            dpa_pm_err("cur_wake_src is xgmac \n");
            break;
        case DPA_WAKEUP_SRC_SNE:
            dpa_pm_err("cur_wake_src is sne \n");
            break;
        case DPA_WAKEUP_SRC_IPCMSG:
            dpa_pm_err("cur_wake_src is ipcmsg \n");
            break;
        default:
            break;
    }
}

void dpa_pm_mntn_show(void)
{
    dpa_pm_ctrl_s *ctrl = NULL;
    dpa_pm_mntn_s *mntn = NULL;
    unsigned int i;

    ctrl = dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    mntn = dpa_pm_get_mntn();
    if (mntn == NULL) {
        return;
    }

    dpa_pm_err("dpa dss_status: %u \n", ctrl->dss_state);

    dpa_pm_wakeup_src_show(mntn->cur_wake_src);

    for (i = 0; i < DPA_WAKEUP_SRC_MAX; i++) {
        switch (i) {
            case DPA_WAKEUP_SRC_USB:
                dpa_pm_err("usb wakeup cnt: %u \n", mntn->wakeup_cnt[i]);
                break;
            case DPA_WAKEUP_SRC_GMAC:
                dpa_pm_err("gmac wakeup cnt: %u \n", mntn->wakeup_cnt[i]);
                break;
            case DPA_WAKEUP_SRC_XGMAC:
                dpa_pm_err("xgmac wakeup cnt: %u \n", mntn->wakeup_cnt[i]);
                break;
            case DPA_WAKEUP_SRC_SNE:
                dpa_pm_err("sne wakeup cnt: %u \n", mntn->wakeup_cnt[i]);
                break;
            case DPA_WAKEUP_SRC_IPCMSG:
                dpa_pm_err("ipcmsg wakeup cnt: %u \n", mntn->wakeup_cnt[i]);
                break;
            default:
                break;
        }
    }

    dpa_pm_err("dpa sleep cnt: %u \n", mntn->sleep_cnt);
    for (i = 0; i < SR_CNT_MAX; i++) {
        dpa_pm_err("dpa last %u sleep time: %u \n", i, mntn->sleep_time[i]);
    }

    dpa_pm_err("dpa wakeup total cnt: %u \n", mntn->wakeup_cnt_total);
    for (i = 0; i < SR_CNT_MAX; i++) {
        dpa_pm_err("dpa last %u wakeup time: %u \n", i, mntn->wakeup_time[i]);
    }
}


void dpa_pm_help(void)
{
    dpa_pm_err("01. dpa_pm_mntn_show \n");
}