/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network security functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 16:01:12 2021
 */

#include <stddef.h>

#include "security/ux_security.h"

static ux_sec_rsvport_checker rsvport_checker;
static ux_sec_rsvport_range_adder rsvport_range_adder;

void ux_sec_rsvport_init(ux_sec_rsvport_checker checker,
			 ux_sec_rsvport_range_adder adder)
{
	rsvport_checker = checker;
	rsvport_range_adder = adder;
}

bool ux_sec_rsvport_check(uint16_t port)
{
	if (rsvport_checker == NULL) {
		return false; /* not reserved */
	}

	return rsvport_checker(port);
}

void ux_sec_rsvport_add_range(uint16_t start, uint16_t end)
{
	if (rsvport_range_adder == NULL) {
		return;
	}

	rsvport_range_adder(start, end);
}
