/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TC_LINUX_DEF_H
#define TC_LINUX_DEF_H

#include <linux/freezer.h>
#include <linux/file.h>
#include <linux/path.h>
#include <linux/version.h>
#if (KERNEL_VERSION(5, 4, 0) <= LINUX_VERSION_CODE)
#include <linux/mman.h>
#else
#include <asm/mman.h>
#endif
#include <linux/signal.h>
#include <linux/sizes.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/cred.h>
#if (KERNEL_VERSION(4, 14, 0) <= LINUX_VERSION_CODE)
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#endif
#include <linux/cpu.h>
#include <linux/regulator/consumer.h>
#include <linux/spi/spi.h>
#include <linux/clk-provider.h>
#include <linux/suspend.h>
#include <linux/of_device.h>
#include <linux/of_reserved_mem.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <asm/ioctls.h>
#include <linux/bitfield.h>
#include <linux/uuid.h>

/* define end */

#endif