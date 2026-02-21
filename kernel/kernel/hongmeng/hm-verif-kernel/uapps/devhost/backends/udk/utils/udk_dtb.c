/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK dtb interfaces implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:14:11 2019
 */
#include <internal/dtb.h>
#include <libdevhost/devhost.h>

static void *__udk_dtb = NULL;

void udk_set_dtb(void *dtb)
{
	__udk_dtb = dtb;
}

void *udk_get_dtb(void)
{
	return __udk_dtb;
}

int udk_firmware_data(unsigned long long mpool_id, void **buf)
{
	return libdh_firmware_data(mpool_id, buf, NULL);
}

int udk_acquire_bootfdt(void **buf, unsigned long *size)
{
	return libdh_acquire_bootfdt(buf, size, LIBDH_FWTYPE_ALL);
}
