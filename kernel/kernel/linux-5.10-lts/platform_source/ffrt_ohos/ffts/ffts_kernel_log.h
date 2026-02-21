/*
 * ffts_kernel_log.h
 *
 * ffts log head file
 *
 * Copyright (c) 2023-2030 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _FFTS_KERNEL_LOG_H_
#define _FFTS_KERNEL_LOG_H_
#include <linux/printk.h>
#define drv_printk(level, module, fmt, ...) \
	printk(level " [%s] [%s %d] " fmt, module, \
		__func__, __LINE__, ## __VA_ARGS__)

#define MODULE_DEVDRV   "FFTS_DRV"
#define ffts_drv_err(fmt, ...)  \
	drv_printk(KERN_ERR, MODULE_DEVDRV"] [E",  fmt, ## __VA_ARGS__)
#define ffts_drv_warn(fmt, ...) \
	drv_printk(KERN_WARNING, MODULE_DEVDRV"] [W",  fmt, ## __VA_ARGS__)
#define ffts_drv_info(fmt, ...)
#define ffts_drv_debug(fmt, ...)

#define cond_return_error(COND, ERRCODE, ...) \
			cond_return_((COND), ERRCODE, ##__VA_ARGS__)

// Do NOT use this Marco directly
#define cond_return_(COND, ERRCODE, ...) \
			if ((COND)) { \
				ffts_drv_err(__VA_ARGS__); \
				return ERRCODE; \
			}

#endif /* _DRV_LOG_H_ */
