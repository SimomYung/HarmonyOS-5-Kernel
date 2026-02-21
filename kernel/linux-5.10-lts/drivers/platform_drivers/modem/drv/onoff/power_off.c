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

/*lint --e{537,559,715} */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <linux/reboot.h>
#include <product_config.h>
#include <osl_bio.h>
#include <osl_list.h>
#include <osl_types.h>
#include <osl_spinlock.h>
#include <mdrv_chg.h>
#include <mdrv_sysboot.h>
#include "power_exchange.h"
#include <bsp_reset.h>
#include <bsp_onoff.h>
#include <bsp_sysctrl.h>
#include "mdrv_chg.h"
#include "bsp_dump.h"
#include "bsp_llt.h"
#include "onoff_msg.h"
#include "power_off_mbb.h"
#include <bsp_print.h>

#define THIS_MODU mod_onoff
#ifdef CONFIG_MODEM_ONOFF_PARA
#include "power_hal.h"
#include <bsp_pmu.h>

struct shutdown_judge_table_info {
    shutdown_class_s *table;
    u32 table_array_size;
};

/* add callback list for reboot */
static LIST_HEAD(g_list_callback_for_reboot);
static DEFINE_MUTEX(g_list_mutexlock_for_reboot);

/* add callback list for power off */
static LIST_HEAD(g_list_callback_for_power_off);
static DEFINE_MUTEX(g_list_mutexlock_for_power_off);

static DEFINE_MUTEX(g_modem_power_off_mutex);
static DEFINE_MUTEX(g_modem_shutdown_judge_table_mutex);

static struct shutdown_judge_table_info g_cust_shutdown_judge_table_info;
power_off_ctrl_s g_power_off_ctrl_table = { DRV_SHUTDOWN_BUTT, { 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
/*
 * The table that defines the shutdown behavior
 * 终端定义的shutdown行为对照表
 */
static shutdown_class_s g_shutdown_judge_table[] = {
    /* 1.shutdown_reason 2.charger state 3. power off charging state; 4.shutdown action style 5.reboot reason */
    {{ DRV_SHUTDOWN_POWER_KEY, 0, 0 }, { DO_POWER_OFF, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_POWER_KEY, 1, 0 }, { DO_REBOOT, POWER_OFF_REASON_BY_CHARGE }},
    {{ DRV_SHUTDOWN_POWER_KEY, 1, 1 }, { DO_REBOOT, POWER_OFF_REASON_NORMAL }},

    {{ DRV_SHUTDOWN_BATTERY_ERROR, 0, 0 }, { DO_POWER_OFF, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_BATTERY_ERROR, 1, 0 }, { DO_REBOOT, POWER_OFF_REASON_BAD_BATTERY }},
    {{ DRV_SHUTDOWN_BATTERY_ERROR, 1, 1 }, { DO_REBOOT, POWER_OFF_REASON_BAD_BATTERY }},

    {{ DRV_SHUTDOWN_LOW_BATTERY, 0, 0 }, { DO_POWER_OFF, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_LOW_BATTERY, 1, 0 }, { DO_REBOOT, POWER_OFF_REASON_LOW_BATTERY }},
    {{ DRV_SHUTDOWN_LOW_BATTERY, 1, 1 }, { DO_REBOOT, POWER_OFF_REASON_LOW_BATTERY }},

    {{ DRV_SHUTDOWN_TEMPERATURE_PROTECT, 0, 0 }, { DO_POWER_OFF, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_TEMPERATURE_PROTECT, 1, 0 }, { DO_REBOOT, POWER_OFF_REASON_OVER_TEMP }},
    {{ DRV_SHUTDOWN_TEMPERATURE_PROTECT, 1, 1 }, { DO_NOTHING, POWER_OFF_REASON_INVALID }},

    {{ DRV_SHUTDOWN_CHARGE_REMOVE, 1, 0 }, { DO_NOTHING, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_CHARGE_REMOVE, 1, 0 }, { DO_NOTHING, POWER_OFF_REASON_INVALID }},
    {{ DRV_SHUTDOWN_CHARGE_REMOVE, 1, 1 }, { DO_POWER_OFF, POWER_OFF_REASON_INVALID }},
};

static unsigned long g_power_is_handled;
static int g_power_reboot_core = POWER_CORE_APP;

void bsp_power_set_reboot_core(power_reboot_core_e reboot_core)
{
    g_power_reboot_core = reboot_core;
}

static inline power_reboot_core_e power_get_reboot_core(void)
{
    return g_power_reboot_core;
}

/*
 * function: power_off_down_operation
 * Description: the Final power off action interface
 * ShutdownReason : shutdown reason.
 * Note: The device operation interface, the chip platform unique
 */
static void power_off_down_operation(void)
{
    void *sysctrl_virt_base_addr = bsp_sysctrl_addr_byindex(SYSCTRL_AO);
    struct power_cfg *cfg = power_get_config();

    bsp_err("system power down!\n");

    if (cfg == NULL) {
        bsp_err("power cfg info is null\n");
        goto dead_loop;
    }

    if (sysctrl_virt_base_addr == NULL) {
        bsp_err("sysctrl virt address error!\n");
        goto dead_loop;
    }

    if (is_in_llt()) {
        bsp_err("power down for llt test !\n");
        return;
    }

    osl_reg_set_bit(sysctrl_virt_base_addr + cfg->pwroff_addr_offset, cfg->pwroff_bits_offset,
        cfg->pwroff_bits_offset, cfg->pwroff_cmd_value);

dead_loop:
    mdelay(3000U);
    bsp_err("fail to power off after 3s\n");
}
/*
 * function: power_off_reboot_operation
 * Description: pmu reset
 * Note: The device operation interface, the chip platform unique
 */
static void power_off_reboot_operation(void)
{
    void *sysctrl_virt_base_addr = bsp_sysctrl_addr_byindex(SYSCTRL_AO);
    struct power_cfg *cfg = power_get_config();
    bsp_err("system reboot!\n");

    if (cfg == NULL) {
        bsp_err("power cfg info is null\n");
        goto dead_loop;
    }

    if (sysctrl_virt_base_addr == NULL) {
        bsp_err("sysctrl virt address error!\n");
        goto dead_loop;
    }

    if (is_in_llt()) {
        bsp_err("reboot for llt test !\n");
        return;
    }
    osl_reg_set_bit(sysctrl_virt_base_addr + cfg->reboot_addr_offset, cfg->reboot_bits_offset,
        cfg->reboot_bits_offset, cfg->reboot_cmd_value);

dead_loop:
    mdelay(3000U);
    bsp_err("fail to reboot after 3s\n");
}

/*
 * Function:  is_charger_in
 * Description: get charger(USB/DC) state
 * 0: not in place 1: in place
 * Note: In terminal baseline, comprehensive consideration of the inward and outward charger state
 */
static char is_charger_in(void)
{
    return bsp_pmu_usb_state_get();
}
/*
 * Function:  bsp_power_off_callback_register
 * Description: some processing before power_off.
 * hook: processing function.
 * Note: 1. for MBB moudles who want to do sth before power off
 * 2. hook function must be no parameter and no return
 */
void bsp_power_off_callback_register(void (*hook)(shutdown_result_action_e))
{
    /* lint --e{*} */
    bsp_power_off_callback_s *callback = (bsp_power_off_callback_s *)kzalloc(sizeof(bsp_power_off_callback_s), GFP_KERNEL);
    if (callback == NULL) {
        bsp_err("fail to malloc callback struct for power off\r\n");
        return;
    }

    callback->fn = hook;

    /* prevant re-entry of power off callback register */
    mutex_lock(&g_list_mutexlock_for_power_off);
    list_add_tail(&callback->node, &g_list_callback_for_power_off);
    mutex_unlock(&g_list_mutexlock_for_power_off);
}
EXPORT_SYMBOL_GPL(bsp_power_off_callback_register);
/*
 * Function:  bsp_reboot_callback_register
 * Description: some processing before reboot.
 * hook: processing function.
 * Note: 1. for MBB moudles who want to do sth before reboot
 * 2. hook function must be no parameter and no return
 */
void bsp_reboot_callback_register(void (*hook)(void))
{
    /*lint --e{*} */
    bsp_onoff_callback_s *callback = (bsp_onoff_callback_s *)kzalloc(sizeof(bsp_onoff_callback_s), GFP_KERNEL);
    if (callback == NULL) {
        bsp_err("fail to malloc callback struct for reboot\r\n");
        return;
    }

    callback->fn = hook;

    mutex_lock(&g_list_mutexlock_for_reboot);
    list_add_tail(&callback->node, &g_list_callback_for_reboot);
    mutex_unlock(&g_list_mutexlock_for_reboot);
}
/*
 * function: power_off_hook_callback
 * Description: deal the process before power off
 */
static void power_off_hook_callback(unsigned int power_off_style)
{
    /*lint --e{*} */
    struct list_head *pos = NULL;
    struct list_head *next = NULL;
    bsp_power_off_callback_s *hook = NULL;

    if (test_and_set_bit(0, &g_power_is_handled)) {
        bsp_debug("power off hook callback bypass\n");
        return;
    }
    bsp_debug("power off hook callback start\n");

    mutex_lock(&g_list_mutexlock_for_power_off);
    list_for_each_safe(pos, next, &g_list_callback_for_power_off)
    {
        hook = list_entry(pos, bsp_power_off_callback_s, node);
        if ((hook != NULL) && (hook->fn != NULL)) {
            hook->fn(power_off_style);
        }
    }
    mutex_unlock(&g_list_mutexlock_for_power_off);
}
/*
 * function: reboot_hook_callback
 * Description: deal the process before reboot
 */
static void reboot_hook_callback(void)
{
    /*lint --e{*} */
    struct list_head *pos = NULL;
    struct list_head *next = NULL;
    bsp_onoff_callback_s *hook = NULL;

    if (test_and_set_bit(1, &g_power_is_handled)) {
        bsp_debug("reboot hook callback bypass\n");
        return;
    }
    bsp_debug("reboot hook callback start\n");

    mutex_lock(&g_list_mutexlock_for_reboot);
    list_for_each_safe(pos, next, &g_list_callback_for_reboot)
    {
        hook = list_entry(pos, bsp_onoff_callback_s, node);
        if ((hook != NULL) && (hook->fn != NULL)) {
            hook->fn();
        }
    }
    mutex_unlock(&g_list_mutexlock_for_reboot);
}

/*
 * function: power_off_operation
 * Description: the entray of power off behavior
 */
void power_off_operation(unsigned int power_off_style)
{
    /* shutdown_result_action */
    unsigned long flags = 0;

    local_irq_save(flags);
    /* deal the process before power off, include soft power_off operation */
    power_off_hook_callback(power_off_style);
    if (power_off_style != DO_SOFT_POWER_OFF) {
        power_off_down_operation();
    }
    local_irq_restore(flags);
}
EXPORT_SYMBOL_GPL(power_off_operation);

/*
 * function: power_off_set_flag_and_reboot
 * Description: the entry of reboot behavior
 */
static void power_off_set_flag_and_reboot(power_off_reboot_flag_e en_reboot_flag)
{
    unsigned long flags = 0;

    /* lint --e{*} */
    reboot_hook_callback();
    if (power_get_reboot_core() == POWER_CORE_LPM) {
        /*send reboot cmd to lpm*/
        bsp_err("send reboot cmd to lpm\n");
        onoff_msg_send_to_lpm(E_POWER_POWER_REBOOT, DRV_SHUTDOWN_UPDATE);
        return;
    }
    local_irq_save(flags);

    power_on_reboot_flag_set(en_reboot_flag);
    power_off_reboot_operation();

    local_irq_restore(flags);
}

/*
 * function: get_shutdown_judge_table
 * Description: Get the address and size of shutdown table
 */
static void *get_shutdown_judge_table(u32 *array_member_num)
{
    void *table = NULL;
    u32 array_size;

    mutex_lock(&g_modem_shutdown_judge_table_mutex);
    table = (void *)g_cust_shutdown_judge_table_info.table;
    array_size = g_cust_shutdown_judge_table_info.table_array_size;
    mutex_unlock(&g_modem_shutdown_judge_table_mutex);

    if (table == NULL) {
        table = (void *)g_shutdown_judge_table;
        array_size = sizeof(g_shutdown_judge_table);
    }

    *array_member_num = array_size;
    return table;
}

/*
 * function: shutdown_get_judge_result
 * Description: translate shutdown reason to reboot reason
 * ShutdownReason : shutdown reason.
 * Note: If the charger is in place, you must restart instead of shutting it down.So first translate the
 * shutdown reason to the reboot reasons
 */
static void *shutdown_get_judge_result(const void *input_factor, unsigned int input_factor_size,
    unsigned int shutdown_class_size)
{
    unsigned int index;
    unsigned int table_num = 0; /* table member size */
    void *judge_table = NULL;
    u32 array_size = 0;

    if (input_factor == NULL || input_factor_size == 0 || shutdown_class_size == 0) {
        return NULL;
    }

    judge_table = get_shutdown_judge_table(&array_size);
    if (judge_table == NULL || array_size == 0) {
        return NULL;
    } else {
        /* get judgement array size */
        table_num = array_size / shutdown_class_size;
    }

    /* find the matched array member */
    for (index = 0; index < table_num; index++) {
        if (memcmp(judge_table, input_factor, input_factor_size) == 0) {
            break;
        }
        judge_table += shutdown_class_size; /* next member */
    }

    if (index >= table_num) {
        return NULL;
    } else {
        return judge_table;
    }
}

int mdrv_sysboot_register_shutdown_judge_table(shutdown_class_s *table, unsigned int table_array_size)
{
    if (table == NULL || table_array_size == 0) {
        bsp_err("table or table_size(%u) is err\n", table_array_size);
        return -EINVAL;
    }

    mutex_lock(&g_modem_shutdown_judge_table_mutex);
    if (g_cust_shutdown_judge_table_info.table != NULL || g_cust_shutdown_judge_table_info.table_array_size != 0) {
        bsp_err("shutdown judge table had registered\n");
        mutex_unlock(&g_modem_shutdown_judge_table_mutex);
        return -EINVAL;
    }
    g_cust_shutdown_judge_table_info.table = table;
    g_cust_shutdown_judge_table_info.table_array_size = table_array_size;
    mutex_unlock(&g_modem_shutdown_judge_table_mutex);
    return 0;
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_register_shutdown_judge_table);

/*
 * func: power_off_excecute
 * Description: the real action of shutdown
 * note: drv_shut_down->power_off_excecute
 * 1. Adding, deleting, and modifying the input and output factors of shutdown_class only affect this
 * function
 */
void power_off_excecute(drv_shutdown_reason_e en_shutdown_reason)
{
    input_condition_s input_factor = { 0 }; /* input factor */
    shutdown_class_s *ret_result = NULL;

    if (en_shutdown_reason >= DRV_SHUTDOWN_BUTT) {
        bsp_err("enShutdownReason is %d, invalid\r\n", en_shutdown_reason);
        return;
    }

    /* get shutdown judgement input factor, which can be extended here */
    input_factor.shutdown_reason = en_shutdown_reason;
    input_factor.the_charger_state = is_charger_in();
    input_factor.is_power_off_charging = (DRV_START_MODE_CHARGING == bsp_start_mode_get());

    ret_result = (shutdown_class_s *)shutdown_get_judge_result(&input_factor, sizeof(input_condition_s),
        sizeof(shutdown_class_s));
    if (ret_result != NULL) {
        bsp_err("reason is %lu, charger_state is %lu, power_off_charging is %lu\r\n", input_factor.shutdown_reason,
            input_factor.the_charger_state, input_factor.is_power_off_charging);

        switch (ret_result->out.result) {
            case DO_REBOOT:
                power_off_set_flag_and_reboot(ret_result->out.reboot_reason);
                break;
            case DO_NOTHING:
                /* do nothing */
                break;
            case DO_POWER_OFF:
                power_off_operation(DO_POWER_OFF);
                break;
            case DO_SOFT_POWER_OFF:
                power_off_operation(DO_SOFT_POWER_OFF);
                break;
            default:
                /* do nothing */
                break;
        }
    } else {
        bsp_err("JUDGE table NOT FOUND\n");
    }
}

EXPORT_SYMBOL_GPL(power_off_excecute);

/*
 * Function:  power_off_timeout_isr
 * Description: timer used by drv_shut_down
 */
static int power_off_timeout_isr(int arg)
{
    if (!is_in_llt()) {
        mutex_lock(&g_modem_power_off_mutex);
        kernel_power_off();
        mutex_unlock(&g_modem_power_off_mutex);
    }
    return 0;
}

void modem_power_restart(enum reboot_mode mode, const char *cmd __attribute__((unused)))
{
    power_off_set_flag_and_reboot(POWER_OFF_REASON_NORMAL);
}
EXPORT_SYMBOL_GPL(modem_power_restart);

void modem_power_off(void)
{
    power_off_excecute(g_power_off_ctrl_table.reason);
}
EXPORT_SYMBOL_GPL(modem_power_off);

#endif /* end CONFIG_MODEM_ONOFF_PARA */

/*
 * Function:  drv_shut_down
 * Description: start the power off process right now or a few second later, and no reboot flow.
 * eReason : shutdown reason.
 * delay_in_ms: timing shutdown time in ms
 * Note: the ONLY entry of normal shutdown
 */
int drv_shut_down(drv_shutdown_reason_e en_reason, unsigned int delay_in_ms)
{
#ifdef CONFIG_MODEM_ONOFF_PARA
    timer_handle handle;
    unsigned int delay_in_us = delay_in_ms * 1000; /* 乘1000换算成微秒时间 */
    if (en_reason >= DRV_SHUTDOWN_BUTT) {
        bsp_err("enShutdownReason is %d, invalid\r\n", en_reason);
        return -1;
    }

    g_power_off_ctrl_table.reason = en_reason;

    if (delay_in_ms > 0) {
        /* prevant re-entry of soft-timer */
        timer_attr_s attr = {"off_timer", 0, TIMER_WAKE, power_off_timeout_isr, 0};
        attr.time = (delay_in_us > POWER_OFF_MONITOR_TIMEROUT) ? POWER_OFF_MONITOR_TIMEROUT : delay_in_us;
        handle = mdrv_timer_add(&attr);
        if (handle < MDRV_OK) {
            bsp_err("fail to add off_timer. handle=%d. \r\n", (int)handle);
            return -1;
        }
    } else {
        if (!is_in_llt()) {
            mutex_lock(&g_modem_power_off_mutex);
            kernel_power_off();
            mutex_unlock(&g_modem_power_off_mutex);
        }
    }
#endif
    return 0;
}
EXPORT_SYMBOL_GPL(drv_shut_down);

/*
 * Function:  bsp_drv_power_reboot
 * Description: same as drv_power_off, the public API
 * Note: the ONLY entry of Normal restart
 */
void bsp_drv_power_reboot(void)
{
#ifdef CONFIG_MODEM_ONOFF_PARA
#ifdef CONFIG_EXTRA_ONOFF
    if (!is_in_llt()) {
        u32 ret;
        u32 status = DRV_ERROR_USER_RESET;

        bsp_err("bsp_drv_power_reboot is called, modem reset...\n");
        ret = (u32)bsp_icc_send(ICC_CPU_APLUSB, ((ICC_CHN_APLUSB_IFC << 16U) | APLUSB_IFC_FUNC_SYSBOOT), (u8 *)&status,
            (u32)sizeof(u32));
        if (ret != (u32)sizeof(u32)) {
            bsp_err("send icc fail.\n");
        } else {
            bsp_err("send icc success.\n");
        }
    }
#else
    if (!is_in_llt()) {
        kernel_restart(NULL);
    }
#endif
#else
    if (!is_in_llt()) {
        bsp_err("bsp_drv_power_reboot is called, modem reset...\n");
        system_error(DRV_ERROR_USER_RESET, 0, 0, NULL, 0);
    }
#endif
}

void mdrv_sysboot_restart(void)
{
    bsp_drv_power_reboot();
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_restart);

/*
 * Function:  bsp_drv_power_reboot_direct
 */
void bsp_drv_power_reboot_direct(void)
{
#ifdef CONFIG_MODEM_ONOFF_PARA
    power_off_reboot_operation();
#else
    if (!is_in_llt()) {
        system_error(DRV_ERROR_USER_RESET, 0, 0, NULL, 0);
    }
#endif
}
EXPORT_SYMBOL_GPL(bsp_drv_power_reboot_direct);

void mdrv_sysboot_power_off_cb_register(void (*hook)(shutdown_result_action_e))
{
#ifdef CONFIG_MODEM_ONOFF_PARA
    bsp_power_off_callback_register(hook);
#endif
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_power_off_cb_register);

void mdrv_sysboot_reboot_cb_register(void (*hook)(void))
{
#ifdef CONFIG_MODEM_ONOFF_PARA
    bsp_reboot_callback_register(hook);
#endif
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_reboot_cb_register);

void mdrv_sysboot_shutdown(void)
{
    (void)drv_shut_down(DRV_SHUTDOWN_TEMPERATURE_PROTECT, 0);
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_shutdown);

int mdrv_sysboot_shut_down(drv_shutdown_reason_e en_reason, unsigned int delay_in_ms)
{
    return drv_shut_down(en_reason, delay_in_ms);
}
EXPORT_SYMBOL_GPL(mdrv_sysboot_shut_down);
