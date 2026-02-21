/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
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

#include <product_config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/of.h>
#include <bsp_dt.h>
#include <bsp_slice.h>
#include <mdrv_public.h>
#include "heating_calibration.h"
#include <msg_id.h>
#include <bsp_msg.h>
#include <securec.h>

static msg_chn_t g_fd;

static int heating_calibration_msg_proc(const struct msg_addr *src_info, void *buf, u32 len)
{
    struct hc_msg_info *hc_info = NULL;

    if (len != sizeof(struct hc_msg_info)) {
        heating_calibration_print("heating_calibration msg len error, len = %d.\n", len);
        return HEATING_CALIBRATION_ERROR;
    }
    hc_info = (struct hc_msg_info *)buf;

    heating_calibration_print("heating_calibration msg proc running.\n");

    if ((hc_info->msg_id) == ID_AP_HEATING_BEGIN) {
        hc_start_ap_heating_proc(hc_info->cpu_mask, hc_info->worker_nums);
    } else if ((hc_info->msg_id) == ID_AP_HEATING_END) {
        hc_stop_ap_heating_proc();
    } else if ((hc_info->msg_id) == ID_RESISTOR_HEATING) {
        hc_resistor_heating_control(hc_info->duty_cycle);
    } else {
        heating_calibration_print("cp action unkown\n");
    }
    return HEATING_CALIBRATION_OK;
}

int heating_calibration_msg_init(void)
{
    struct msgchn_attr lite_attr = { 0 };
    bsp_msgchn_attr_init(&lite_attr);
    lite_attr.chnid = MSG_CHN_WARMUP;
    lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP) | MSG_CORE_MASK(MSG_CORE_APP);
    lite_attr.lite_notify = heating_calibration_msg_proc;
    if (bsp_msg_lite_open(&g_fd, &lite_attr) == 0) {
        heating_calibration_print("heating_calibration_msg_init %d succcess\n", MSG_CHN_WARMUP);
    } else {
        heating_calibration_print("heating_calibration_msg_init %d fail\n", MSG_CHN_WARMUP);
        return -1;
    }
    return 0;
}

int heating_cal_probe(struct platform_device *pdev)
{
    s32 ret;

    ret = heating_calibration_methods_init(pdev);
    if (ret < 0) {
        heating_calibration_print("heating_calibration_methods_init errNo = 0x%x\n", ret);
        return HEATING_CALIBRATION_ERROR;
    }

    ret = heating_calibration_msg_init();
    if (ret < 0) {
        heating_calibration_print("heating_calibration_msg_init errNo = 0x%x\n", ret);
        return HEATING_CALIBRATION_ERROR;
    }

    heating_calibration_print("bsp_heating_calibration_init ok\n");
    return HEATING_CALIBRATION_OK;
}

static const struct of_device_id g_heating_cal_of_match[] = {
    {
        .compatible = "heating_cal",
    },
    {},
};
MODULE_DEVICE_TABLE(of, g_heating_cal_of_match);

static struct platform_device g_heating_cal_platform_device = {
    .name = "Modem heating_cal",
    .id = -1,
};

static struct platform_driver g_heating_cal_driver = {
    .probe = heating_cal_probe,
    .remove = NULL,
    .driver        = {
        .owner    = THIS_MODULE,
        .name    = "Modem heating_cal",
        .of_match_table = of_match_ptr(g_heating_cal_of_match),
        .probe_type = PROBE_FORCE_SYNCHRONOUS,
    },
};

int heating_calibration_init(void)
{
    int ret = -1;
    device_node_s *np = bsp_dt_find_compatible_node(NULL, NULL, "heating_cal");
    if (np == NULL) {
        heating_calibration_print("can not find dts compatiblenode\n");
        return ret;
    }

    /* 如果status为false，表示业务场景需要自己创建设备 */
    if (bsp_dt_device_is_available(np) == false) {
        ret = platform_device_register(&g_heating_cal_platform_device);
        if (ret != 0) {
            heating_calibration_print("platform_device_register not succ\n");
            return ret;
        }
    }

    ret = platform_driver_register(&g_heating_cal_driver);
    if (ret != 0) {
        heating_calibration_print("platform_driver_register not succ\n");
    }
    return ret;
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
late_initcall(heating_calibration_init);
#endif
