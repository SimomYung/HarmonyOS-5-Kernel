/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: kbox support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 9 17:36:50 2020
 */
#include <devhost/kbox.h>

#include <libkbox/kbox.h>

int devhost_kbox_create(const char *name, size_t size)
{
	return kbox_create(name, size);
}

int devhost_kbox_open(const char *name)
{
	return kbox_open(name);
}

ssize_t devhost_kbox_write(int rid, const void *buf, size_t size)
{
	return kbox_write(rid, buf, size);
}

void devhost_kbox_close(int rid)
{
	kbox_close(rid);
}
