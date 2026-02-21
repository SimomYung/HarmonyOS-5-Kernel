/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
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
#ifndef _HEATING_CALIBRATION_PHONE_H_
#define _HEATING_CALIBRATION_PHONE_H_

#include <product_config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/file.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <asm/atomic.h>

/* geekbench heating parameter */
#define MAX_QUEUE_LEN 10
#define MAX_WORKER_NUMS 32
#define GK_HEATING_DEVICE_NAME "heating_dev"
#define GK_HEATING_DEVICE_CLASS "heating_class"
#define GK_HEATING_APP_NAME "heating_proxy"
#define GK_HEATING_FIRST_MINOR 0
#define GK_HEATING_DEVICES_NUMBER 1
#define GK_HEATING_MAX_CMD_SIZE 512

struct params_trans {
    unsigned int stop_heating;
    unsigned int cpu_mask;
    unsigned int worker_nums;
};

struct geekbench_params_node {
    struct params_trans para;
    struct list_head para_node;
};

struct geekbench_heating_ctrl {
    struct cdev cdev;
    dev_t dev_no;
    struct class *heating_class;
    struct device *device;
    wait_queue_head_t r_wait;
    wait_queue_head_t w_wait;
    struct list_head para_queue;
    spinlock_t queue_lock;
    int queue_size;
};

struct core_heating_kernel {
    volatile u32 thread_flag;
    u32 core_id;
    struct task_struct *thremal_thread;
};

struct thermal_info_kernel {
    u32 core_num_max;
    u32 config_cores;
    u32 cpu_mask;
};

enum ap_heating_mode {
    AP_HEATING_KERNEL,
    AP_HEATING_USER,
};

#endif /* _HEATING_CALIBRATION_PHONE_H_  */