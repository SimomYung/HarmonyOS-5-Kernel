/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: stub
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/device.h>
#include <linux/types.h>
#include <linux/err.h>

static struct class *g_hiusb_client_class;
static struct class *g_hiusb_port_class;

void *hiusb_get_client_class(void)
{
	return g_hiusb_client_class;
}

void *hiusb_get_port_class(void)
{
	return g_hiusb_port_class;
}

int hiusb_client_init(struct module *owner)
{
	struct class *client_class = NULL;

	client_class = class_create(owner, "hiusb_client");
	if (!IS_ERR(client_class))
		g_hiusb_client_class = client_class;

	return PTR_ERR_OR_ZERO(client_class);
}

void hiusb_client_exit(void)
{
	if (g_hiusb_client_class)
		class_destroy(g_hiusb_client_class);
	g_hiusb_client_class = NULL;
}

int hiusb_port_init(struct module *owner)
{
	struct class *port_class = NULL;

	port_class = class_create(owner, "hiusb_port");
	if (!IS_ERR(port_class))
		g_hiusb_port_class = port_class;

	return PTR_ERR_OR_ZERO(port_class);
}

void hiusb_port_exit(void)
{
	if (g_hiusb_port_class) {
		class_destroy(g_hiusb_port_class);
		g_hiusb_port_class = NULL;
	}
}

static int __init hiusb_driver_init(void)
{
	int ret;

	pr_info("+\n");

	ret = hiusb_port_init(THIS_MODULE);
	if (ret)
		return ret;

	ret = hiusb_client_init(THIS_MODULE);
	if (ret) {
		hiusb_port_exit();
		return ret;
	}
	pr_info("-\n");
	return 0;
}

early_initcall(hiusb_driver_init);
