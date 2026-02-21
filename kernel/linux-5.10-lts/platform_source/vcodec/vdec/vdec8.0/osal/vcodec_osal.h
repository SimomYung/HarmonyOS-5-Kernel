/*
 * vcodec_osal.h
 *
 * This is for vfmw_osal interface.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
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

#ifndef VCODEC_OSAL_H
#define VCODEC_OSAL_H

#include "vfmw.h"
#include "vcodec_platform_osal.h"

#define OSAL_OK  0
#define OSAL_ERR 1

#define rd_vreg(base, reg, dat) \
	do { \
		(dat) = readl(((uint8_t *)(base) + (reg))); \
	} while (0)

#define wr_vreg(base, reg, dat) \
	do { \
		writel((dat), ((uint8_t *)(base) + (reg))); \
	} while (0)

typedef irq_handler_t vfmw_irq_handler_t;

#define OS_MSLEEP             msleep
#define OS_PRINT              printk

void vcodec_osal_intf_init(void);
void vcodec_osal_intf_exit(void);

void vcodec_osal_mb(void);
void vcodec_osal_udelay(unsigned long usecs);

int32_t vcodec_osal_event_init(uint32_t *handle, int32_t init_val);
void vcodec_osal_event_exit(uint32_t handle);
int32_t vcodec_osal_event_give(uint32_t handle);
int32_t vcodec_osal_event_wait(uint32_t handle, const unsigned int ms_wait_time);

int8_t *vcodec_osal_reg_map(UADDR phy_addr, uint32_t len);              // NO USE
void vcodec_osal_reg_unmap(uint8_t *vir_addr);                     // NO USE

void *vcodec_osal_create_thread(void *task_func, void *param, int8_t *task_name); // NO USE
int32_t vcodec_osal_stop_thread(void *param);                   // NO USE
int32_t vcodec_osal_kthread_should_stop(void);             // NO USE

int32_t vcodec_osal_spin_lock_init(uint32_t *handle);            // NO USE
void vcodec_osal_spin_lock_exit(uint32_t handle);                 // NO USE
int32_t vcodec_osal_spin_lock(uint32_t handle, unsigned long *lock_flag);  // NO USE
int32_t vcodec_osal_spin_unlock(uint32_t handle, unsigned long *lock_flag); // NO USE

int32_t vcodec_osal_sema_init(uint32_t *handle);
void vcodec_osal_sema_exit(uint32_t handle);
int32_t vcodec_osal_sema_down(uint32_t handle);
int32_t vcodec_osal_sema_try(uint32_t handle);                    // NO USE
void vcodec_osal_sema_up(uint32_t handle);

int32_t vcodec_osal_request_irq(uint32_t irq, vfmw_irq_handler_t handler,
	unsigned long flags, const char *name, void *dev);
void vcodec_osal_free_irq(uint32_t irq, void *dev);
void vcodec_osal_enable_irq(uint32_t irq);
void vcodec_osal_disable_irq(uint32_t irq);

uint64_t vcodec_osal_get_time_in_ms(void);                  // NO USE
uint64_t vcodec_osal_get_time_in_us(void);                  // NO USE

int32_t vcodec_osal_proc_create(uint8_t *proc_name, void *read, void *write);
void vcodec_osal_proc_destroy(uint8_t *proc_name);
void vcodec_osal_dump_proc(void *page, int32_t page_count, int32_t *used_bytes,
    int8_t from_shr, const int8_t *format, ...);

int32_t vcodec_work_finish(void *node);
void *vcodec_work_submit(void *ctx, int32_t (*proc)(void *), void *arg);
int32_t vcodec_work_submit_detach(void *ctx, int32_t (*proc)(void *), void *arg);
void vcodec_work_ctx_destroy(void *ctx);
void *vcodec_work_ctx_create(const char *name);
#endif
