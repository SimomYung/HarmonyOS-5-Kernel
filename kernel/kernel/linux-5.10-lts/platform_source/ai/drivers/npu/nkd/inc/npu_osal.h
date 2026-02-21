/*
 * npu_osal.h
 *
 * about npu operating system abstraction layer
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */
#ifndef __NPU_OSAL_H
#define __NPU_OSAL_H

#include <linux/atomic.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/gfp.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/kfifo.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>
#include <linux/rtc.h>
#include <linux/rwsem.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <securec.h>
#include "npu_log.h"

#ifndef UINT8_MAX
#define UINT8_MAX             0xFF
#endif

#ifndef UINT16_MAX
#define UINT16_MAX            0xFFFF
#endif
#define MAX_UINT16_NUM  0xFFFF

#ifndef UINT32_MAX
#define UINT32_MAX            0xFFFFFFFFU
#endif

#define unused(expr) do { \
		(void)(expr); \
	} while (0)

#ifndef __align_mask
#define __align_mask(x, mask)   (((x) + (mask)) & ~(mask))
#endif
#define align_up(x, a)  __align_mask((x), ((a) - 1))
#ifndef align_down
#define align_down(x, a)  ((x) & ~((a) - 1))
#endif

#define npu_bitmap_get(val, pos) (((u64)(val) >> (unsigned int)(pos)) & 1U)
#define npu_bitmap_set(val, pos) ((val) | (1U << (unsigned int)(pos)))
#define npu_bitmap_clear(val, pos) ((val) & ~(1U << (unsigned int)(pos)))


#define npu_reg_update(addr, value, mask) do { \
	uint32_t reg_value = readl(addr); \
	reg_value = ((reg_value) & (~(mask))) | ((value) & (mask)); \
	writel(reg_value, addr); \
} while (0)

#endif  /* __NPU_OSAL_H */
