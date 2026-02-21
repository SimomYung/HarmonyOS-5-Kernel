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
#include <linux/pwm.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/file.h>
#include <linux/sched.h>
#include <asm/atomic.h>
#include <linux/of.h>
#include <bsp_slice.h>
#include <osl_sem.h>
#include <mdrv_public.h>
#include "heating_cal_phone.h"
#include "heating_calibration.h"
#include <securec.h>

struct geekbench_heating_ctrl g_geekbench_heating = {0};
static struct device *g_hc_dev = NULL;
static struct pwm_device *g_pwm_dev = NULL;
#define PWM_PERIOD_HC_PHONE 100000
int g_is_pwm_init = 0;

/* math calc parameter */
#define APW_MCP_DIV0 2
#define APW_MCP_DIV1 5
#define APW_MCP_MULTI0 2
#define APW_MCP_MULTI1 4
#define APW_MCP_MULTI2 7

#define CORE_HEATING_MAX_NUM 12

/* action flag */
#define SYSTEM_HEATING_BEGIN 1
#define SYSTEM_HEATING_END 0

#define MAX_CYCLES 100000

static osl_sem_id g_thermal_sem_kernel;
static struct work_struct g_kernel_mode_thermal_up, g_kernel_thermal_down;
struct core_heating_kernel g_cores_array_phone[CORE_HEATING_MAX_NUM] = {0};
struct thermal_info_kernel g_thermal_info_kernel = { 0 };
static volatile u64 g_calc_result = 0;
enum ap_heating_mode g_heating_mode = 0;

void do_kernel_mode_heating_algorithm(struct core_heating_kernel *current_core)
{
    unsigned int math_calc = 0x10;
    unsigned int calc_result = 0;
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
    heating_calibration_print("thermal thread binded for core %d is stopped!\n", current_core->core_id);
}

int hc_ap_kernel_mode_kthread(void *core_array)
{
    struct core_heating_kernel *current_core = (struct core_heating_kernel *)core_array;
    if (current_core == NULL) {
        return HEATING_CALIBRATION_ERROR;
    }
    heating_calibration_print("start thermal thread success! Bind for core %d.\n", current_core->core_id);
    do_kernel_mode_heating_algorithm(current_core);
    return HEATING_CALIBRATION_OK;
}

void thermal_core_up_kernel_work(struct work_struct *unuse_work)
{
    int i;
    osl_sem_down(&g_thermal_sem_kernel);
    for (i = 0; i < g_thermal_info_kernel.core_num_max; i++) {
        if (g_thermal_info_kernel.cpu_mask & (1UL << i)) {
            g_cores_array_phone[i].core_id = i;
            g_cores_array_phone[i].thread_flag = (u32)SYSTEM_HEATING_BEGIN;
            g_cores_array_phone[i].thremal_thread = kthread_create(hc_ap_kernel_mode_kthread, &g_cores_array_phone[i], "thermal_kthread");
            kthread_bind(g_cores_array_phone[i].thremal_thread, i);
            wake_up_process(g_cores_array_phone[i].thremal_thread);
        }
    }
    osl_sem_up(&g_thermal_sem_kernel);
}

static void thermal_core_down_kernel_work(struct work_struct *unuse_work)
{
    u32 i;
    osl_sem_down(&g_thermal_sem_kernel);
    for (i = 0; i < g_thermal_info_kernel.core_num_max; i++) {
        if (g_thermal_info_kernel.cpu_mask & (1UL << i)) {
            g_cores_array_phone[i].thread_flag = (u32)SYSTEM_HEATING_END;
        }
    }
    osl_sem_up(&g_thermal_sem_kernel);
}

static unsigned int hc_calculate_corenum_phone(unsigned int cpu_mask)
{
    unsigned int count = 0;
    while (cpu_mask) {
        count += cpu_mask & 1;
        cpu_mask >>= 1;
    }
    return count;
}

struct geekbench_heating_ctrl *get_geekbench_heating_ctrl(void)
{
    return &g_geekbench_heating;
}

static int hc_geekbench_heating_open(struct inode *inode, struct file *file)
{
    UNUSED(inode);
    UNUSED(file);
    if (strcmp(current->comm, GK_HEATING_APP_NAME)) {
        heating_calibration_print("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }
    return 0;
}

static ssize_t hc_geekbench_heating_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct geekbench_heating_ctrl *ctrl = get_geekbench_heating_ctrl();
    struct geekbench_params_node *node = NULL;

    UNUSED(file);
    UNUSED(ppos);
    if (strcmp(current->comm, GK_HEATING_APP_NAME)) {
        heating_calibration_print("<%s> wrong task!!!current = %s\n", __FUNCTION__, current->comm);
        return -ENODEV;
    }

    /* wait for a request to complete */
    wait_event(ctrl->r_wait, (ctrl->queue_size > 0));

    node = list_first_entry(&(ctrl->para_queue), struct geekbench_params_node, para_node); /* 获得头节点 */
    if (copy_to_user(buf, &(node->para), sizeof(struct params_trans))) {
        kfree(node);
        return -EFAULT;
    }
    spin_lock(&ctrl->queue_lock);
    list_del(ctrl->para_queue.next); 
    ctrl->queue_size--;
    spin_unlock(&ctrl->queue_lock);
    wake_up(&ctrl->w_wait);

    kfree(node);

    return sizeof(struct params_trans);
}

static const struct file_operations g_gk_heating_dev_fops = {
    .open = hc_geekbench_heating_open,
    .owner = THIS_MODULE,
    .read = hc_geekbench_heating_read,
};

static void enqueue_cmd_queue(struct geekbench_params_node *node)
{
    struct geekbench_heating_ctrl *ctrl = get_geekbench_heating_ctrl();

    /* 如队列满，阻塞，等待用户态读走后，继续执行 */
    wait_event(ctrl->w_wait, (ctrl->queue_size < MAX_QUEUE_LEN));

    spin_lock(&ctrl->queue_lock);
    list_add_tail(&(node->para_node), &(ctrl->para_queue));
    ctrl->queue_size++;
    spin_unlock(&ctrl->queue_lock);
    wake_up(&ctrl->r_wait);
    return;
}

void hc_ap_kernel_mode_heating(unsigned int cpu_mask)
{
    osl_sem_down(&g_thermal_sem_kernel);
    g_thermal_info_kernel.config_cores = hc_calculate_corenum_phone(cpu_mask);
    g_thermal_info_kernel.cpu_mask = cpu_mask;
    osl_sem_up(&g_thermal_sem_kernel);
    schedule_work(&g_kernel_mode_thermal_up);
}

void hc_start_ap_heating_proc(unsigned int cpu_mask, unsigned int worker_nums)
{
    struct geekbench_params_node *node = NULL;

    if (cpu_mask == 0 || worker_nums == 0 ) {
        heating_calibration_print("[heating_cal] cpu_mask and worker_nums can not be zero!, cpu_mask=%d, worker_nums=%d\n", cpu_mask, worker_nums);
        return;
    }
    if (worker_nums > MAX_WORKER_NUMS) {
        heating_calibration_print("[heating_cal] worker_nums can not bigger than 32!, worker_nums=%d\n", worker_nums);
        return;
    }

    /* If bit 31 is 1, the kernel-mode heating is used. */
    if (cpu_mask & 0x80000000) {
        g_heating_mode = AP_HEATING_KERNEL;
        hc_ap_kernel_mode_heating((cpu_mask & 0xFFF));
    } else {
        g_heating_mode = AP_HEATING_USER;
        node = (struct geekbench_params_node *)kzalloc(sizeof(struct geekbench_params_node), GFP_KERNEL);
        node->para.cpu_mask = cpu_mask;
        node->para.worker_nums = worker_nums;
        node->para.stop_heating = 0;
        INIT_LIST_HEAD(&node->para_node);
        enqueue_cmd_queue(node);
    }
}

void hc_stop_ap_heating_proc(void)
{
    struct geekbench_params_node *node = NULL;

    if (g_heating_mode == AP_HEATING_KERNEL) {
        osl_sem_down(&g_thermal_sem_kernel);
        g_thermal_info_kernel.config_cores = 0;
        osl_sem_up(&g_thermal_sem_kernel);
        schedule_work(&g_kernel_thermal_down);
    } else {
        node = (struct geekbench_params_node *)kzalloc(sizeof(struct geekbench_params_node), GFP_KERNEL);
        node->para.cpu_mask = 0;
        node->para.worker_nums = 0;
        node->para.stop_heating = 1;
        INIT_LIST_HEAD(&node->para_node);
        enqueue_cmd_queue(node);
    }
}

void hc_resistor_heating_control(int duty_cycle)
{
    struct pwm_state state;
    struct device_node *np = NULL;
    int err;

    if (duty_cycle < 0 || duty_cycle > 100) {
        heating_calibration_print("hc_resistor_heating_control duty_cycle = %d is invalid.\n", duty_cycle);
        return;
    }

    if (g_is_pwm_init == 0) {
        np = of_find_compatible_node(NULL, NULL, "hisilicon,blpwm_test");
        if (np == NULL) {
            heating_calibration_print("can not find dts compatible node!\n");
            return;
        }

        g_pwm_dev = devm_of_pwm_get(g_hc_dev, np, NULL);
        if (g_pwm_dev == NULL) {
            heating_calibration_print("%s, get pwm failed\n", __func__);
        }
        g_is_pwm_init = 1;
    }

    state.period = PWM_PERIOD_HC_PHONE;
    state.duty_cycle = PWM_PERIOD_HC_PHONE * duty_cycle / 100;
    state.polarity = PWM_POLARITY_NORMAL;
    state.enabled = (duty_cycle == 0) ? false : true;

    err = pwm_apply_state(g_pwm_dev, &state);
    if (err) {
        heating_calibration_print("failed to apply initial PWM state: %d", err);
        return;
    }

    heating_calibration_print("hc_resistor_heating_control duty_cycle = %d success.\n", duty_cycle);
    return;
}

static int heating_cal_geekbench_dev_setup(void)
{
    int ret;
    const struct file_operations *fops = NULL;
    struct geekbench_heating_ctrl *ctrl = get_geekbench_heating_ctrl();

    fops = &g_gk_heating_dev_fops;

    /* 初始化等待队列头 */
    init_waitqueue_head(&ctrl->r_wait);
    init_waitqueue_head(&ctrl->w_wait);
    INIT_LIST_HEAD(&(ctrl->para_queue));
    spin_lock_init(&ctrl->queue_lock);

    ret = alloc_chrdev_region(&ctrl->dev_no, GK_HEATING_FIRST_MINOR, GK_HEATING_DEVICES_NUMBER, GK_HEATING_DEVICE_NAME);
    if (ret) {
        heating_calibration_print("alloc_chrdev_region failed\n");
        return ret;
    }

    cdev_init(&ctrl->cdev, fops);

    ret = cdev_add(&ctrl->cdev, ctrl->dev_no, GK_HEATING_DEVICES_NUMBER);
    if (ret) {
        heating_calibration_print("add applog fail!\n");
        unregister_chrdev_region(ctrl->dev_no, GK_HEATING_DEVICES_NUMBER);
        return ret;
    }

    ctrl->heating_class = class_create(THIS_MODULE, GK_HEATING_DEVICE_CLASS);
    if (IS_ERR(ctrl->heating_class)) {
        heating_calibration_print("class create failed!\n");
        cdev_del(&ctrl->cdev);
        unregister_chrdev_region(ctrl->dev_no, GK_HEATING_DEVICES_NUMBER);
        return HEATING_CALIBRATION_ERROR;
    }

    ctrl->device = device_create(ctrl->heating_class, NULL, ctrl->dev_no, NULL, GK_HEATING_DEVICE_NAME);
    if (IS_ERR(ctrl->device)) {
        heating_calibration_print("<%s> device_create failed, err = %ld.\n", __FUNCTION__, PTR_ERR(ctrl->device));
        class_destroy(ctrl->heating_class);
        cdev_del(&ctrl->cdev);
        unregister_chrdev_region(ctrl->dev_no, GK_HEATING_DEVICES_NUMBER);
        return HEATING_CALIBRATION_ERROR;
    }

    return HEATING_CALIBRATION_OK;
}

int heating_calibration_methods_init(struct platform_device *pdev)
{
    int ret;

    if (pdev == NULL) {
        heating_calibration_print("platform_device is NULL!\n");
        return HEATING_CALIBRATION_ERROR;
    }
    g_hc_dev = &pdev->dev;
    ret = heating_cal_geekbench_dev_setup();
    if (ret < 0) {
        heating_calibration_print("heating_cal_geekbench_dev_setup errNo = 0x%x\n", ret);
        return HEATING_CALIBRATION_ERROR;
    }

    osl_sem_init(1, &g_thermal_sem_kernel);
    INIT_WORK(&g_kernel_mode_thermal_up, thermal_core_up_kernel_work);
    INIT_WORK(&g_kernel_thermal_down, thermal_core_down_kernel_work);
    g_thermal_info_kernel.core_num_max = num_possible_cpus();

    return HEATING_CALIBRATION_OK;
}
