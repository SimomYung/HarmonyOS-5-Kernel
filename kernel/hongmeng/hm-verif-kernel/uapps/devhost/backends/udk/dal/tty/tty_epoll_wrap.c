/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Wrap some func of tty
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 09 11:31:39 2019
 */

#include "tty_wrap.h"

#include <udk/poll.h>
#include <hongmeng/types.h>

#include "tty.h"
#include "tty_chrdev.h"

void tty_poll_notify(tty_poll_wq_t *wq, unsigned long event)
{
	udk_poll_wqueue_wakeup(wq, (unsigned int)event);
}

void tty_poll_wq_init(tty_poll_wq_t *wq)
{
	udk_poll_wqueue_init(wq);
}

int tty_irq_bottom_half_init(struct tty_irq_bottom_half *bottom,
			     int (*func)(void *data), void *data)
{
	if (bottom == NULL || func == NULL) {
		return -EINVAL;
	}
	bottom->func = func;
	bottom->data = data;

	return 0;
}

int tty_irq_bottom_half_schedule(struct tty_irq_bottom_half *bottom)
{
	if (bottom == NULL || bottom->func == NULL) {
		return -EINVAL;
	}

	return bottom->func(bottom->data);
}
