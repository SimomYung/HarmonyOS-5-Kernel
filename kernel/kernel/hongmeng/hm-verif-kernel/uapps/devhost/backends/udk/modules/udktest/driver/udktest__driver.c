/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:37:35 2019
 */

#include "udktest__driver.h"
#include <udk/device.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <udk/securec.h>

#include <helper.h>
#include <udktests.h>

int udktest__driver(void)
{
	int ret = 0, err = 0;

	ret = udktest__driver_register();
	if (ret) {
		err += 1;
	}

	return (err == 0) ? 0 : -1;
}
