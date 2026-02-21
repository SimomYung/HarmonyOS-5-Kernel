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
#include <linux/types.h>

struct hiusb_port;

void dwc3_cmd_tmo_debug_notify(void)
{
	return;
}
int hiusb_port_set_dp_hpd(struct hiusb_port *port, bool is_in)
{
	return 0;
}
void usb_controller_irq_clr(void)
{
	return;
}

