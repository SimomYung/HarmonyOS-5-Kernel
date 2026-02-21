/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2027. All rights reserved.
 *
 * jpu_osal.h
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

#ifndef JPU_OSAL_H
#define JPU_OSAL_H

#include "jpu.h"

int32_t os_request_irq(uint32_t isrno, irq_handler_t handler, struct jpu_data_type *jpu_device);
void os_enable_irq(uint32_t isrno);
void os_disable_irq(uint32_t isrno);
void os_free_irq(uint32_t isrno);
int32_t jpu_hardlock_get(void);
void jpu_hardlock_put(void);
void os_wait_ms(uint32_t millisec);
long os_timestamp_diff(const os_time_t *lasttime, const os_time_t *curtime);
void os_get_timestamp(os_time_t *tick);
int32_t jpu_wait_interrupt(struct jpu_interrupt_t *jpu_interrupt);
void jpu_wake_up_interrupt(struct jpu_interrupt_t *jpu_interrupt);
uint32_t os_copy_from_user(void *dst, unsigned long dst_size,
							const void __user *src, unsigned long src_size);
#endif
