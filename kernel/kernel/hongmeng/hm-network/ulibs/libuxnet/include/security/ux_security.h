/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network security functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 15:50:32 2021
 */
#ifndef _UX_SECURITY_H_
#define _UX_SECURITY_H_

#include <stdbool.h>
#include <stdint.h>

/* tcpip stack need to register implementation */
typedef bool (*ux_sec_rsvport_checker)(uint16_t);
typedef void (*ux_sec_rsvport_range_adder)(uint16_t, uint16_t);
void ux_sec_rsvport_init(ux_sec_rsvport_checker checker,
			 ux_sec_rsvport_range_adder adder);

/* public interface for reserved port security user */
bool ux_sec_rsvport_check(uint16_t port);
void ux_sec_rsvport_add_range(uint16_t start, uint16_t end);

#endif
