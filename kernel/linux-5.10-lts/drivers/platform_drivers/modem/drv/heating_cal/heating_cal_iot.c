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
#include <linux/pinctrl/consumer.h>
#ifdef CONFIG_PWM_IOT
#include <linux/pwm.h>
#endif
#include <bsp_slice.h>
#include <bsp_dt.h>
#include <osl_sem.h>
#include <mdrv_public.h>
#include "heating_cal_iot.h"
#include "heating_calibration.h"
#include <securec.h>

#ifdef CONFIG_PINCTRL
struct heating_cal_pinctrl_cfg {
    struct device *dev;
    struct pinctrl *pinctrl;
    struct pinctrl_state *pins_default;
    struct pinctrl_state *pins_idle;
};
struct heating_cal_pinctrl_cfg g_healting_cal_pinctrl = { 0 };
#endif

static osl_sem_id g_thermal_sem;
static struct work_struct g_thermal_up, g_thermal_down;
struct core_heating g_cores_array[CORE_HEATING_MAX_NUM] = {};
struct current_core_heating_info g_thermal_info = { 0 };
static volatile u64 g_calc_result = 0;
int g_pwm_num = -1;

void test_pid_affinity(struct core_heating *current_core)
{
    struct cpumask cpu_mask = {};
    pid_t curr_pid;

    cpumask_clear(&cpu_mask);
    curr_pid = task_pid_nr_ns(current, NULL);
    if (sched_getaffinity(curr_pid, &cpu_mask) < 0) {
        heating_calibration_print("sched_getaffinity err\n");
        return;
    }
    current_core->cpu_mask = cpu_mask;

    if (cpumask_test_cpu(current_core->core_id, &cpu_mask)) {
        heating_calibration_print("pid: %d is runnig in core : %d cpumask_weight:%d\n", curr_pid, current_core->core_id,
            cpumask_weight(&cpu_mask));
    }
    return;
}

void do_something_thermal_core(struct core_heating *current_core)
{
    unsigned int math_calc = 0x10;
    int i;
    do {
        for (i = 0; i < MAX_CYCLES; i++) {
            math_calc += (math_calc / APW_MCP_DIV0);
            math_calc -= (APW_MCP_MULTI0 * i);
            math_calc += (math_calc * APW_MCP_MULTI2);
            math_calc += (APW_MCP_MULTI1 * i);
            math_calc += (math_calc / APW_MCP_DIV1);
            math_calc -= (APW_MCP_MULTI0 * i);
        }
        g_calc_result += math_calc;
    } while (current_core->thread_flag != SYSTEM_HEATING_END);
}

int heating_core_kthread(void *core_array)
{
    struct core_heating *current_core = (struct core_heating *)core_array;
    if (current_core == NULL) {
        return HEATING_CALIBRATION_ERROR;
    }
    test_pid_affinity(current_core);
    do_something_thermal_core(current_core);
    current_core->core_id = 0;
    cpumask_clear(&current_core->cpu_mask);
    return HEATING_CALIBRATION_OK;
}

void thermal_core_up(struct work_struct *unuse_work)
{
    u32 valid_core_num;
    struct task_struct *task = NULL;
    u32 core_num;
    int i;
    pid_t curr_pid;
    struct cpumask cpu_mask = {};

    osl_sem_down(&g_thermal_sem);

    if (g_thermal_info.config_cores <= g_thermal_info.thermal_core) {
        osl_sem_up(&g_thermal_sem);
        return;
    }
    core_num = g_thermal_info.config_cores - g_thermal_info.thermal_core;

    valid_core_num = min(core_num, g_thermal_info.core_num_max);

    for (i = g_thermal_info.core_num_max; i > (g_thermal_info.core_num_max - valid_core_num); i--) {
        g_cores_array[i].core_id = i - 1;
        g_cores_array[i].thread_flag = (u32)SYSTEM_HEATING_BEGIN;

        task = (struct task_struct *)kthread_run(heating_core_kthread, &g_cores_array[i], "thermal_kthread");
        if (IS_ERR((void *)task)) {
            core_num = g_thermal_info.core_num_max - i;
            heating_calibration_print("kthread_run fail\n");
            goto end;
        }
        curr_pid = task_pid_nr_ns(task, NULL);

        cpumask_clear(&cpu_mask);
        cpumask_set_cpu(g_cores_array[i].core_id, &cpu_mask);

        if (sched_setaffinity(curr_pid, &cpu_mask) < 0) {
            heating_calibration_print("sched_setaffinity err\n");
        } else {
            heating_calibration_print("new pid: %d is runnig in core : %d \n", curr_pid, g_cores_array[i].core_id);
        }
    }
    core_num = valid_core_num;
end:
    g_thermal_info.thermal_core += core_num;
    g_thermal_info.core_num_max -= core_num;
    osl_sem_up(&g_thermal_sem);
    heating_calibration_print("increase %d cpus,thermal_cpus: %d,max_cpus: %d\n", core_num, g_thermal_info.thermal_core,
        num_possible_cpus());
}

void thermal_core_down(struct work_struct *unuse_work)
{
    u32 i;
    osl_sem_down(&g_thermal_sem);
    g_thermal_info.core_num_max = num_possible_cpus();
    for (i = g_thermal_info.core_num_max; i > (g_thermal_info.core_num_max - g_thermal_info.thermal_core); i--) {
        g_cores_array[i].thread_flag = (u32)SYSTEM_HEATING_END;
    }
    heating_calibration_print(" %d core thermal down\n", g_thermal_info.thermal_core);
    g_thermal_info.thermal_core = 0;
    osl_sem_up(&g_thermal_sem);
}

static unsigned int hc_calculate_corenum(unsigned int cpu_mask)
{
    unsigned int count = 0;
    while (cpu_mask) {
        count += cpu_mask & 1;
        cpu_mask >>= 1;
    }
    return count;
}

void hc_start_ap_heating_proc(unsigned int cpu_mask, unsigned int worker_nums)
{
    osl_sem_down(&g_thermal_sem);
    g_thermal_info.config_cores = hc_calculate_corenum(cpu_mask);
    osl_sem_up(&g_thermal_sem);
    schedule_work(&g_thermal_up);
}

void hc_stop_ap_heating_proc(void)
{
    osl_sem_down(&g_thermal_sem);
    g_thermal_info.config_cores = 0;
    osl_sem_up(&g_thermal_sem);
    schedule_work(&g_thermal_down);
}

int heating_cal_pwm_gpio_select(void)
{
#ifdef CONFIG_PINCTRL
    int ret;
    struct heating_cal_pinctrl_cfg *cfg = &g_healting_cal_pinctrl;

    cfg->pinctrl = devm_pinctrl_get(cfg->dev);
    if (IS_ERR_OR_NULL(cfg->pinctrl)) {
        heating_calibration_print("%s: could not get pinctrl\n", __func__);
        cfg->pinctrl = NULL;
        return HEATING_CALIBRATION_ERROR;
    }

    cfg->pins_default = pinctrl_lookup_state(cfg->pinctrl, "heating_cal_default");
    if (IS_ERR_OR_NULL(cfg->pins_default)) {
        heating_calibration_print("%s: could not get default pinctrl\n", __func__);
        goto pins_cfg_error;
    }

    ret = pinctrl_select_state(cfg->pinctrl, cfg->pins_default);
    if (ret) {
        heating_calibration_print("select heating_cal default pinctrl fail, ret %d !\n", ret);
        goto pins_cfg_error;
    }

    return 0;

pins_cfg_error:
    devm_pinctrl_put(cfg->pinctrl);
    return HEATING_CALIBRATION_ERROR;
#endif
    return 0;
}

void heating_cal_pwm_gpio_release(void)
{
#ifdef CONFIG_PINCTRL
    int ret;
    struct heating_cal_pinctrl_cfg *cfg = &g_healting_cal_pinctrl;

    ret = pinctrl_select_state(cfg->pinctrl, cfg->pins_idle);
    if (ret) {
        heating_calibration_print("select heating_cal idle pinctrl fail, ret %d !\n", ret);
    }

    devm_pinctrl_put(cfg->pinctrl);
#endif
}

#ifdef CONFIG_PWM_IOT
struct pwm_device *g_heating_dev;
#define PWM_PERIOD_HC 120000
int g_is_pwm_init = 0;
int g_is_pwm_pinctrl_selected = 0;

void hc_pwm_pin_release(void)
{
    if (g_is_pwm_pinctrl_selected != 0) {
        heating_cal_pwm_gpio_release();
        heating_calibration_print("pwm_pinctrl released\n");
    }
    g_is_pwm_pinctrl_selected = 0;
    heating_calibration_print("hc_pwm_pin_release succ\n");
}

int hc_pwm_pin_select(void)
{
    int ret;

    if (g_is_pwm_pinctrl_selected == 0) {
        ret = heating_cal_pwm_gpio_select();
        if (ret != 0) {
            heating_calibration_print("pwm select no succ\n");
            return HEATING_CALIBRATION_ERROR;
        }
        g_is_pwm_pinctrl_selected = 1;
        heating_calibration_print("pwm_pinctrl selected\n");
    }

    return 0;
}

int pwm_duty_cycle_config(int duty_cycle)
{
    int tmp, ret;

    tmp = PWM_PERIOD_HC * duty_cycle / 100;

    /* 配置占空比duty_cycle */
    ret = pwm_config(g_heating_dev, tmp, PWM_PERIOD_HC);
    if (ret != 0) {
        heating_calibration_print("hc_resistor_heating_control pwm_config error!!\n");
        return HEATING_CALIBRATION_ERROR;
    }

    return HEATING_CALIBRATION_OK;
}

void hc_resistor_heating_control(int duty_cycle)
{
    int ret;

    if (g_is_pwm_init == 0) {
        g_heating_dev = pwm_request(g_pwm_num, "iot,heating_cal_app");
        if (IS_ERR(g_heating_dev)) {
            heating_calibration_print("[init]%s: failed to add heating driver, Maybe your product not support resistor heating.\n", __func__);
            return;
        }
        g_is_pwm_init = 1;
    }

    heating_calibration_print("hc_resistor_heating_control duty_cycle = %d.\n", duty_cycle);
    if (duty_cycle < 0 || duty_cycle > 100) {
        heating_calibration_print("hc_resistor_heating_control duty_cycle = %d is invalid.\n", duty_cycle);
        return;
    }

    if (duty_cycle == 0) {
        heating_calibration_print("hc_resistor_heating_control pwm duty_cycle = 0 . \n");
        pwm_duty_cycle_config(duty_cycle);
        return;
    }

    ret = hc_pwm_pin_select();
    if (ret != 0) {
        return;
    }

    ret = pwm_duty_cycle_config(duty_cycle);
    if (ret != 0) {
        heating_calibration_print("hc_resistor_heating_control pwm_duty_cycle_config error!!\n");
        return;
    }

    ret = pwm_enable(g_heating_dev);
    if (ret < 0) {
        heating_calibration_print("hc_resistor_heating_control pwm_enable error!!\n");
        return;
    }
    heating_calibration_print("hc_resistor_heating_control duty_cycle = %d success.\n", duty_cycle);
}
#else
void hc_resistor_heating_control(int duty_cycle)
{
    heating_calibration_print("[heating_cal] pwm not support, duty_cycle = %d.\n", duty_cycle);
}
#endif

int heating_calibration_methods_init(struct platform_device *pdev)
{
    int ret;
    device_node_s *np = NULL;
    osl_sem_init(1, &g_thermal_sem);
    INIT_WORK(&g_thermal_up, thermal_core_up);
    INIT_WORK(&g_thermal_down, thermal_core_down);
    g_thermal_info.core_num_max = num_possible_cpus();

    np = bsp_dt_find_compatible_node(NULL, NULL, "heating_cal");
    if (np == NULL) {
        heating_calibration_print("can not find dts compatible node.\n");
        return HEATING_CALIBRATION_ERROR;
    }

    ret = bsp_dt_property_read_u32(np, "pwm_num", &g_pwm_num);
    if (ret) {
        heating_calibration_print("can not find dts pwm_num property.\n");
        return HEATING_CALIBRATION_ERROR;
    }

#ifdef CONFIG_PINCTRL
    g_healting_cal_pinctrl.dev = &pdev->dev;
#endif
    return HEATING_CALIBRATION_OK;
}