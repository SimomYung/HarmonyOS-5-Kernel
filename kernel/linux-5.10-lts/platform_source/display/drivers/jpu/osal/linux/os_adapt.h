/*
 * os_adapt.h
 *
 * This is for linux osal interface.
 *
 * Copyright (c) 2024-2027 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef OS_ADAPT_H
#define OS_ADAPT_H


#include <linux/wait.h>
#include <linux/clk.h>
#include <linux/regulator/driver.h>
#include <linux/platform_device.h>
#include <linux/compat.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <securec.h>
#include <linux/iommu.h>
#include <linux/list.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/dma-buf.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/io.h>
#include <linux/hwspinlock.h>
#include <linux/printk.h>
#include <platform_include/basicplatform/linux/pr_log.h>
#include <asm/io.h>
#include <linux/types.h>
#include <linux/pm_wakeup.h>

#define JPU_IOCTL_MAGIC 'J'
#define JPU_JOB_EXEC _IOW(JPU_IOCTL_MAGIC, 0x21, struct jpu_data_t)

#define JPU_TAG "JPU_LOG"
#define PR_LOG_TAG  JPU_TAG

#define JPEGD_VOTE_BIT_MASK				0x00000040
#define JPEGD_VOTE_OTHER_BIT_MASK		0x00000080

#define SUCCESS					0
#define ERROR					(-1)
#define GET_SEM(sem)			(&(sem))

typedef struct timeval  os_time_t;
typedef struct semaphore *os_sem;
extern uint32_t g_jpu_msg_level;
/*
 * Message printing priorities:
 * LEVEL 0 KERN_EMERG (highest priority)
 * LEVEL 1 KERN_ALERT
 * LEVEL 2 KERN_CRIT
 * LEVEL 3 KERN_ERR
 * LEVEL 4 KERN_WARNING
 * LEVEL 5 KERN_NOTICE
 * LEVEL 6 KERN_INFO
 * LEVEL 7 KERN_DEBUG (Lowest priority)
 */
#define jpu_err(msg, ...) \
	do { if (g_jpu_msg_level > 3) \
		pr_info("%s: "msg, __func__, ## __VA_ARGS__); \
	} while (0)
#define jpu_warning(msg, ...) \
	do { if (g_jpu_msg_level > 4) \
		pr_info("%s: "msg, __func__, ## __VA_ARGS__); \
	} while (0)
#define jpu_info(msg, ...) \
	do { if (g_jpu_msg_level > 6) \
		pr_info("%s: "msg, __func__, ## __VA_ARGS__); \
	} while (0)
#define jpu_debug(msg, ...) \
	do { if (g_jpu_msg_level > 7) \
		pr_info("%s: "msg, __func__, ## __VA_ARGS__); \
	} while (0)

struct jpu_interrupt_t {
	uint32_t reserve;
	uint32_t jpu_dec_done_flag;
	uint32_t fpga_flag;
	wait_queue_head_t jpu_dec_done_wq;
};

int32_t jpu_hwspinlock_init(void);
void os_sem_up(os_sem sem);
void os_sem_down(os_sem sem);
#endif
