/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Wrap some func of tty
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 09 11:31:39 2019
 */

#include "tty_wrap.h"
#include "tty.h"
#include "tty_chrdev.h"
#include "linux_device.h"

void tty_poll_notify(tty_poll_wq_t *wq, unsigned long event)
{
	wake_up_interruptible_poll(wq, event);
}
EXPORT_SYMBOL(tty_poll_notify);

void tty_poll_wq_init(tty_poll_wq_t *wq)
{
	init_waitqueue_head(wq);
}
EXPORT_SYMBOL(tty_poll_wq_init);

static void native_tasklet_handler(unsigned long data)
{
	struct tty_irq_bottom_half *bottom = NULL;

	bottom = (struct tty_irq_bottom_half *)(uintptr_t)data;
	if (bottom == NULL || bottom->func == NULL) {
		return;
	}
	(void)bottom->func(bottom->data);
}

int tty_irq_bottom_half_init(struct tty_irq_bottom_half *bottom,
			     int (*func)(void *data), void *data)
{
	if (bottom == NULL || func == NULL) {
		return -EINVAL;
	}
	bottom->func = func;
	bottom->data = data;
	tasklet_init(&bottom->tasklet, native_tasklet_handler,
		     (unsigned long)(uintptr_t)bottom);

	return 0;
}
EXPORT_SYMBOL(tty_irq_bottom_half_init);

int tty_irq_bottom_half_schedule(struct tty_irq_bottom_half *bottom)
{
	if (bottom == NULL) {
		return -EINVAL;
	}
	tasklet_schedule(&bottom->tasklet);

	return 0;
}
EXPORT_SYMBOL(tty_irq_bottom_half_schedule);

int tty_sem_waiters(struct tty_sem_t *sem)
{
	if (sem == NULL) {
		return -EINVAL;
	}

	return (int)(sem->waiters);
}
EXPORT_SYMBOL(tty_sem_waiters);
