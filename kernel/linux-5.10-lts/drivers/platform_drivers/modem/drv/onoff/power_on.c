/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

/*lint --e{528,537,715} */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/freezer.h>
#include <linux/version.h>
#include <asm/system_misc.h>
#include <product_config.h>
#include <mdrv_sysboot.h>
#include <mdrv_chg.h>
#include <mdrv_errno.h>
#include <bsp_dump.h>
#include <bsp_onoff.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_sysctrl.h>
#include "power_exchange.h"
#include "mdrv_chg.h"
#include "onoff_msg.h"
#include "power_off_mbb.h"
#include "power_hal.h"
#include "bsp_modem_boot.h"
#include "power_para.h"
#include "dpart/sysboot_dpart_policy.h"

#define THIS_MODU mod_onoff
#ifdef CONFIG_MODEM_ONOFF_PARA

int modem_boot_is_crash(void);

static int bsp_sysboot_sync(struct notifier_block *notifier, unsigned long val, void *v)
{
    int ret;
    unsigned int ap_state = bsp_sysboot_get_ap_state();
    int modem_is_crash = modem_boot_is_crash();

    if (ap_state != 0 && modem_is_crash == 0) { // 系统启动成功
        bsp_sysboot_clear_dpart_info();
    }

    ret = freeze_processes();
    if (ret) {
        bsp_err("freeze processes failed, ret = 0x%x\n", ret);
    }
    ksys_sync();
    bsp_err("sync fs done\n");
    return NOTIFY_OK;
}

static struct notifier_block g_sysboot_reboot_notifier = {
    .notifier_call = bsp_sysboot_sync,
    .priority = 1,
};

int modem_boot_is_crash(void)
{
    if (mdrv_sysboot_is_modem_ready() == 0) {
        return 1;
    }

    return 0;
}
EXPORT_SYMBOL_GPL(modem_boot_is_crash);

#define POWER_ON_MONITOR_TIMEROUT (60 * 1000000)
#define POWER_SYS_PRIV_RO (S_IRUSR | S_IRGRP)
#define POWER_SYS_PRIV_WR (S_IWUSR | S_IRUGO)

typedef struct {
    drv_start_mode_e mode;
    power_on_start_reason_e reason;
    struct rtc_time time;
} power_on_ctrl_s;

static power_on_ctrl_s g_power_item_info = { 0 }; //lint !e64

/*
 * 功能描述: 记录开机时间.
 */
int power_on_time_get(struct rtc_time *get_time)
{
    struct rtc_time tm;
    struct timespec64 ts;

    if (get_time == NULL) {
        return -EINVAL;
    }

    ktime_get_real_ts64(&ts);

    rtc_time64_to_tm((unsigned long)ts.tv_sec, &tm);

    *get_time = tm;

    bsp_debug("system power on time:%4d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour,
        tm.tm_min, tm.tm_sec);
    return 0;
}

int mdrv_sysboot_get_power_on_time(struct rtc_time *time)
{
    return power_on_time_get(time);
}

/*
 * 功能描述: 获取开机原因
 * 说明: 已经对返回值做了合法性判断
 */
static power_on_start_reason_e power_on_reason_get(void)
{
    power_on_start_reason_e sram_data = power_on_start_reason_get();
    if ((sram_data < POWER_ON_REASON_START) || (sram_data > POWER_ON_REASON_BUTT)) {
        sram_data = POWER_ON_REASON_BUTT;
    }
    return sram_data;
}

/*
 * 功能描述: 获取开机模式(枚举值),也是g_power_status[]的索引
 */
static drv_start_mode_e power_on_mode_get(void)
{
    drv_start_mode_e mode = DRV_START_MODE_NORMAL; // prefer init for invalid to normal
    drv_run_mode_e boot_mode = get_run_mode();
    drv_charging_mode_e pwd_flag = get_pd_charge_flag();

    if (boot_mode == RUN_MODE_RECOVERY) {
        mode = DRV_START_MODE_UPDATE;
    } else if (boot_mode == RUN_MODE_NORMAL) {
        if (pwd_flag == NORMAL_CHARGING_MODE) {
            mode = DRV_START_MODE_NORMAL;
        } else {
            mode = DRV_START_MODE_CHARGING;
        }
    }

    if (mode >= DRV_START_MODE_BUTT) {
        mode = DRV_START_MODE_NORMAL;
    }

    return mode;
}

/*
 * 功能描述: 开机定时器处理，检测C核状态
 */
static int power_on_timeout_isr(int arg)
{
    drv_run_mode_e run_mode = get_run_mode();
    unsigned int ap_state = bsp_sysboot_get_ap_state();

    bsp_err("power_on_timeout_isr, system had been powered one minute ago\n ");

    if (run_mode == RUN_MODE_NORMAL) {
        if (ap_state == 0) {
            bsp_err("ap power on fail!\n ");
            sysboot_notifier_process(AP_BOOT_FAIL);
            system_error(DRV_ERRNO_SYSBOOT_FAIL, 0, 0, NULL, 0);
            return 0;
        }

        if (modem_boot_is_crash()) {
            bsp_err("ccore power on fail!\n ");
            sysboot_notifier_process(MODEM_BOOT_FAIL);
            system_error(DRV_ERRNO_SYSBOOT_FAIL, 0, 0, NULL, 0);
            return 0;
        }
    } else if (run_mode == RUN_MODE_RECOVERY) {
        if (ap_state == 0) {
            bsp_err("ap power on fail!\n ");
            system_error(DRV_ERRNO_SYSBOOT_FAIL, 0, 0, NULL, 0);
            return 0;
        }
        bsp_err("system is recovery mode!\n ");
    }

    bsp_sysboot_clear_dpart_info();
    bsp_sysboot_dpart_async_dump();
    bsp_err("system power on ok!\n ");
    sysboot_notifier_process(MODEM_BOOT_DONE);

    return 0;
}

/*
 * 功能描述: 创建开机timer,用于开机后一段时间检测系统是否正常工作
 */
void power_on_timer_init(void)
{
    timer_handle handle;
    timer_attr_s attr = {"on_timer", POWER_ON_MONITOR_TIMEROUT, TIMER_WAKE, power_on_timeout_isr, 0};
    handle = mdrv_timer_add(&attr);
    if (handle < MDRV_OK) {
        bsp_debug("fail to add on_timer. handle=%d.\n", (s32)handle);
        return;
    }
    return;
}

drv_start_mode_e mdrv_sysboot_get_bootmode(void)
{
    return (drv_start_mode_e)power_on_mode_get();
}

power_on_start_reason_e mdrv_sysboot_get_power_on_reason(void)
{
    return power_on_reason_get();
}

#else
drv_start_mode_e mdrv_sysboot_get_bootmode(void)
{
    return 0;
}

power_on_start_reason_e mdrv_sysboot_get_power_on_reason(void)
{
    return 0;
}

int mdrv_sysboot_get_power_on_time(struct rtc_time *time)
{
    return 0;
}
#endif /* end CONFIG_MODEM_ONOFF_PARA */

EXPORT_SYMBOL_GPL(mdrv_sysboot_get_power_on_reason);
EXPORT_SYMBOL_GPL(mdrv_sysboot_get_bootmode);
EXPORT_SYMBOL_GPL(mdrv_sysboot_get_power_on_time);

/*
 * 功能描述: 用于获取开机模式
 */
int bsp_start_mode_get(void)
{
#ifdef CONFIG_MODEM_ONOFF_PARA
    g_power_item_info.mode = power_on_mode_get();
    return g_power_item_info.mode;
#else
    return DRV_START_MODE_NORMAL;
#endif
}

#ifndef BSP_CONFIG_PHONE_TYPE
sysboot_boot_mode_e bsp_boot_mode_get(void)
{
    u32 i;
    int boot_mode;
    u32 size = 0;
    static sysboot_boot_mode_e logic_mode = SYSBOOT_BOOT_MODE_ERROR;
    struct boot_mode_map *map = boot_mode_map_get(&size);
    struct power_cfg *cfg = power_get_config();
    if (cfg == NULL) {
        bsp_err("power cfg info is null\n");
        return SYSBOOT_BOOT_MODE_ERROR;
    }

    if (logic_mode != SYSBOOT_BOOT_MODE_ERROR) {
        return logic_mode;
    }

    boot_mode = (int)(readl(bsp_sysctrl_addr_get((void*)(uintptr_t)cfg->boot_mode_addr_base)) & cfg->boot_mode_mask);
    for (i = 0; i < size; i++) {
        if (boot_mode == map->phy_mode) {
            logic_mode = map->logic_mode;
            return logic_mode;
        }
        map++;
    }

    bsp_err("phy boot mode is %d\n", boot_mode);
    return SYSBOOT_BOOT_MODE_ERROR;
}
#else
sysboot_boot_mode_e bsp_boot_mode_get(void)
{
    return SYSBOOT_BOOT_MODE_STUB;
}
#endif
EXPORT_SYMBOL_GPL(bsp_boot_mode_get);

sysboot_boot_mode_e mdrv_sysboot_get_boot_mode(void)
{
    return bsp_boot_mode_get();
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_get_boot_mode);

int modem_power_on_init(void)
{
    int ret;
    bsp_debug("modem power on init start\n");
#ifdef CONFIG_MODEM_ONOFF_PARA
    arm_pm_restart = (void *)modem_power_restart;
    pm_power_off = modem_power_off;

    ret = register_reboot_notifier(&g_sysboot_reboot_notifier);
    if (ret) {
        bsp_err("fail to register sysboot notifier, ret = %d\n", ret);
    }

    ret = power_on_shm_init();
    if (ret) {
        return ret;
    }

    /* 清除reboot flag */
    power_on_reboot_flag_set(POWER_OFF_REASON_INVALID);

    /* get power on reason */
    g_power_item_info.reason = power_on_reason_get();

    /* get power time */
    ret = power_on_time_get(&g_power_item_info.time);
    if (ret) {
        bsp_err("fail to get power on time\n");
        return ret;
    }

    /* 创建开机监测timer */
    if (sysboot_get_monitor_flag() == SYSBOOT_MONITOR_ENABLED) {
        power_on_timer_init();
    }
#endif
    ret = onoff_msg_init();
    if (ret == 0) {
        bsp_debug("modem power on init end\n");
    }

    return ret;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
late_initcall(modem_power_on_init);
#endif
