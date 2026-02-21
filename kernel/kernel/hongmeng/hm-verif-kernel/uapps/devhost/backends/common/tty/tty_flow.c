/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Function of data flow in tty
 * Author: Huawei OS Kernel Lab
 * Create: July Thu 23 16:47:50 2020
 */
#include "tty_flow.h"

void tty_flow_ctl_init(struct tty_flow_ctl *flow_ctl)
{
	struct tty_cond_attr cond_attr;

	if (flow_ctl == NULL) {
		tty_warn("tty: flow_ctl is NULL\n");
		return;
	}
	flow_ctl->state = FLOW_OUTPUT_ON;
	tty_mutex_init(&flow_ctl->mutex);
	cond_attr.attr = COND_INTERRUPT_FLAG;
	(void)tty_cond_init(&flow_ctl->cond, &cond_attr);
}

void tty_flow_reset(struct tty_flow_ctl *flow_ctl)
{
	BUG_ON(flow_ctl == NULL);
	flow_ctl->state = FLOW_OUTPUT_ON;
}

static inline bool __flow_state_query(struct tty_flow_ctl *flow_ctl,
				      unsigned int mask, unsigned int state)
{
	bool ret = false;

	(void)tty_mutex_lock(&flow_ctl->mutex);
	ret = ((flow_ctl->state & mask) == state);
	(void)tty_mutex_unlock(&flow_ctl->mutex);

	return ret;
}

/* query whether the tty flow output state flag is off */
bool tty_flow_is_output_off(struct tty_flow_ctl *flow_ctl)
{
	BUG_ON(flow_ctl == NULL);
	return __flow_state_query(flow_ctl, FLOW_OUTPUT_STATE_BIT, FLOW_OUTPUT_OFF);
}

/* query whether the tty flow output state flag is on */
bool tty_flow_is_output_on(struct tty_flow_ctl *flow_ctl)
{
	BUG_ON(flow_ctl == NULL);
	return __flow_state_query(flow_ctl, FLOW_OUTPUT_STATE_BIT, FLOW_OUTPUT_ON);
}

static inline void __flow_state_update(struct tty_flow_ctl *flow_ctl,
				      unsigned int mask, bool set_off)
{
	(void)tty_mutex_lock(&flow_ctl->mutex);
	if (set_off) {
		flow_ctl->state &= (~mask);
	} else {
		flow_ctl->state |= mask;
	}
	(void)tty_mutex_unlock(&flow_ctl->mutex);
}

/* set tty flow output state flag off */
void tty_flow_set_output_off(struct tty_flow_ctl *flow_ctl)
{
	BUG_ON(flow_ctl == NULL);
	__flow_state_update(flow_ctl, FLOW_OUTPUT_STATE_BIT, true);
}

/* set tty flow output state flag on */
void tty_flow_set_output_on(struct tty_flow_ctl *flow_ctl)
{
	BUG_ON(flow_ctl == NULL);
	__flow_state_update(flow_ctl, FLOW_OUTPUT_STATE_BIT, false);
}

int tty_flow_schedule(struct tty_flow_ctl *flow_ctl)
{
	int ret = 0;
	if (flow_ctl == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&flow_ctl->mutex);
	while (((flow_ctl->state & FLOW_OUTPUT_STATE_BIT) == FLOW_OUTPUT_OFF) && ret == 0) {
		ret = tty_cond_wait(&flow_ctl->cond, &flow_ctl->mutex);
	}
	(void)tty_mutex_unlock(&flow_ctl->mutex);
	return ret;
}

void tty_flow_wake_up(struct tty_flow_ctl *flow_ctl)
{
	if (flow_ctl == NULL) {
		tty_warn("tty: tty of flow wake up writer is NULL\n");
		return;
	}
	(void)tty_mutex_lock(&flow_ctl->mutex);
	(void)tty_cond_broadcast(&flow_ctl->cond);
	(void)tty_mutex_unlock(&flow_ctl->mutex);
}
