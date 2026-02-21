/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kbox UDK interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 25 17:18:36 2019
 */
#include <udk/kbox.h>

#include <devhost/kbox.h>

int udk_kbox_create(const char *name, size_t size)
{
	return devhost_kbox_create(name, size);
}

int udk_kbox_open(const char *name)
{
	return devhost_kbox_open(name);
}

ssize_t udk_kbox_write(int rid, const void *buf, size_t size)
{
	return devhost_kbox_write(rid, buf, size);
}

void udk_kbox_close(int rid)
{
	devhost_kbox_close(rid);
}
