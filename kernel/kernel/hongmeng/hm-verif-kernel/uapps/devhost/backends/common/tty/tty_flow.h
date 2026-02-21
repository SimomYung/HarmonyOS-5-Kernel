/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Define structure and prototypes of flow in tty
 * Author: Huawei OS Kernel Lab
 * Create: July Thu 23 16:47:50 2020
 */
#ifndef TTY_FLOW_H
#define TTY_FLOW_H

#include "tty.h"
#include "tty_wrap.h"

/* the following define are used for tty flow control */
#define FLOW_OUTPUT_STATE_BIT 0x1U /* output state bit */

#define FLOW_OUTPUT_OFF 0x0 /* output state bit is clear */
#define FLOW_OUTPUT_ON  0x1 /* output state bit is set */

void tty_flow_ctl_init(struct tty_flow_ctl *flow_ctl);
void tty_flow_reset(struct tty_flow_ctl *flow_ctl);
bool tty_flow_is_output_off(struct tty_flow_ctl *flow_ctl);
bool tty_flow_is_output_on(struct tty_flow_ctl *flow_ctl);
void tty_flow_set_output_off(struct tty_flow_ctl *flow_ctl);
void tty_flow_set_output_on(struct tty_flow_ctl *flow_ctl);

int tty_flow_schedule(struct tty_flow_ctl *flow_ctl);
void tty_flow_wake_up(struct tty_flow_ctl *flow_ctl);

#endif
