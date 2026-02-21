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

#include <linux/hashtable.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <mdrv_timer.h>
#include <securec.h>
#include <bsp_diag.h>
#include "hook.h"
#include "debug.h"

#define MONITOR_TIME_OUT 1000000
#define USING_CHR 0
#define USING_CDEV 1
#define NM_MSG_ID 0x9f380520

static int net_monitor_timer_cb(int para)
{
    unsigned char *data = (unsigned char *)net_monitor_get_option_count();
    bsp_diag_trans_report(NM_MSG_ID, DIAG_DRV_HDS_PID, data, sizeof(struct option_count));
    return 0;
}

static void period_report_init(void)
{
    timer_attr_s attr;
    timer_handle handle;

    attr.time = MONITOR_TIME_OUT;
    attr.flag = TIMER_PERIOD;
    attr.callback = net_monitor_timer_cb;
    attr.para = USING_CDEV;
    handle = mdrv_timer_add(&attr);
    if (handle < MDRV_OK) {
        bsp_err("fail to add timer handle=%d.\n", handle);
    }
}

int net_monitor_init(void)
{
    if (net_monitor_socket_opt_init()) {
        return -1;
    }

    period_report_init();

    bsp_err("net_monitor_init done.\n");
    return 0;
}

int net_monitor_test(void) {
    int i = 0;
    struct option_count cnt;

    for (i = 0; i < sizeof(cnt.so_count) / sizeof(cnt.so_count[0]); i++) {
        cnt.so_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.tcp_count) / sizeof(cnt.tcp_count[0]); i++) {
        cnt.tcp_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.udp_count) / sizeof(cnt.udp_count[0]); i++) {
        cnt.udp_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.ip_count) / sizeof(cnt.ip_count[0]); i++) {
        cnt.ip_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.ipmr_count) / sizeof(cnt.ipmr_count[0]); i++) {
        cnt.ipmr_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.ipv6_count) / sizeof(cnt.ipv6_count[0]); i++) {
        cnt.ipv6_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.ipmr6_count) / sizeof(cnt.ipmr6_count[0]); i++) {
        cnt.ipmr6_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.icmpv6_count) / sizeof(cnt.icmpv6_count[0]); i++) {
        cnt.icmpv6_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.ioctl_count) / sizeof(cnt.ioctl_count[0]); i++) {
        cnt.ioctl_count[i] = i;
    }

    for (i = 0; i < sizeof(cnt.flag_count) / sizeof(cnt.flag_count[0]); i++) {
        cnt.flag_count[i] = i;
    }

    bsp_diag_trans_report(NM_MSG_ID, DIAG_DRV_HDS_PID, (unsigned char *)&cnt, sizeof(struct option_count));

    return 0;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
subsys_initcall(net_monitor_init);
#endif