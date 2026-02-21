/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 29 10:37:35 2019
 */
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/sync.h>
#include <udk/log.h>
#include <udk/irq.h>
#include <udk/securec.h>

#include <helper.h>

#include "udktest__device.h"

static int save_local_flags(void)
{
	unsigned long flags;

	udk_local_irq_disable();
	flags = udk_local_save_flags();
	if (flags != 1) {
		return -1;
	}

	udk_local_irq_enable();
	flags = udk_local_save_flags();
	if (flags != 0) {
		return -1;
	}

	flags = udk_local_irq_save();
	if (flags != 0) {
		return -1;
	}

	flags = udk_local_irq_save();
	if (flags != 1) {
		return -1;
	}

	flags = 0;
	udk_local_irq_restore(flags);
	flags = udk_local_save_flags();
	if (flags != 0) {
		return -1;
	}
	return 0;
}

int udktest__device_local(void)
{
	PROLOGUE;

	TCALL("udktest__device_local_flags", save_local_flags);

	return ret;
}
