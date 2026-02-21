/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:37:35 2019
 */

#include "udktest__device.h"
#include <udk/device.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <udk/securec.h>

#include <helper.h>
#include <udktests.h>

int udktest__device(void)
{
	int ret = 0, err = 0;

	ret = udktest__device_alloc();
	if (ret) {
		err += 1;
	}
	ret = udktest__device_add();
	if (ret) {
		err += 1;
	}
	ret = udktest__device_resprop();
	if (ret) {
		err += 1;
	}

	/* FIXME: local irq is not compatible with actvhdlr,
	 *        temporary ignore test result. */
	(void)udktest__device_local();

	return (err == 0) ? 0 : -1;
}
