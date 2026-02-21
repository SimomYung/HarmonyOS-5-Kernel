/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_thread attribute
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */

#include <hmkernel/sched/sched.h>
#include <hmkernel/sched/priority.h>
#include <hmkernel/const.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_thread.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>

#include "raw_thread_impl.h"

int raw_thread_attr_init(raw_thread_attr_t *attr)
{
	int err = E_HM_OK;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(*attr);
		attr->stacksize = DEFAULT_RAW_THREAD_STACKSIZE;
		attr->guardsize = DEFAULT_RAW_THREAD_GUARDSIZE;
	}

	return err;
}

int raw_thread_attr_destroy(const raw_thread_attr_t *attr)
{
	UNUSED(attr);
	return 0;
}

int raw_thread_attr_setstack(raw_thread_attr_t *attr, const void *stack, size_t size)
{
	int err = E_HM_OK;

	if (attr == NULL || stack == NULL ||
	    size < (size_t)PAGE_SIZE || size > SIZE_MAX / 4U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		attr->stackaddr = ptr_to_size_t(stack) + size;
		attr->stacksize = size;
	}
	return err;
}

int raw_thread_attr_setstacksize(raw_thread_attr_t *attr, size_t size)
{
	int err = E_HM_OK;

	if (attr == NULL || size < (size_t)PAGE_SIZE || size > SIZE_MAX / 4U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		attr->stackaddr = 0;
		attr->stacksize = size;
	}
	return err;
}

static int max_priority_of(unsigned int policy)
{
	int ret = 0;

	switch (policy) {
	case SCHED_FIFO:
		ret = HM_PRIORITY_SCHED_RT_MAX;
		break;
	case SCHED_RR:
		ret = HM_PRIORITY_SCHED_RT_MAX;
		break;
	case SCHED_OTHER:
	case SCHED_BATCH:
	case SCHED_IDLE:
		ret = 0;
		break;
	case SCHED_EDF:
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static int min_priority_of(unsigned int policy)
{
	int ret = 0;

	switch (policy) {
	case SCHED_FIFO:
		/* fall through */
	case SCHED_RR:
		ret = HM_PRIORITY_SCHED_RT_MIN;
		break;
	case SCHED_OTHER:
	case SCHED_BATCH:
	case SCHED_IDLE:
		ret = 0;
		break;
	case SCHED_EDF:
		/* fall through */
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static int check_priority(const raw_thread_attr_t *attr, int prio)
{
	int err = E_HM_INVAL;
	unsigned int policy = attr->policy;
	int prio_max = max_priority_of(policy);
	int prio_min = min_priority_of(policy);
	if (prio_max >= 0 && prio_min >= 0 && prio <= prio_max && prio >= prio_min) {
		err = E_HM_OK;
	}

	return err;
}

int raw_thread_attr_setschedparam(raw_thread_attr_t *attr, const struct raw_sched_param *param)
{
	int ret = 0;

	if (attr == NULL || param == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = check_priority(attr, (int)param->priority);
	}
	if (ret == E_HM_OK) {
		attr->prio = param->priority;
	}

	return ret;
}

int raw_thread_attr_getschedparam(const raw_thread_attr_t *attr, struct raw_sched_param *param)
{
	int err = E_HM_OK;
	if (attr == NULL || param == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		param->priority = attr->prio;
	}
	return err;
}

int raw_thread_attr_setguardsize(raw_thread_attr_t *attr, size_t size)
{
	int err = E_HM_OK;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (size > SIZE_MAX / 8U) {
			err = E_HM_INVAL;
		} else {
			attr->guardsize = size;
		}
	}

	return err;
}

int raw_thread_attr_getinheritsched(const raw_thread_attr_t *attr, int *inherit)
{
	int err = E_HM_OK;

	if (attr == NULL || inherit == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*inherit = (int)attr->inherit;
	}
	return err;
}

int raw_thread_attr_setinheritsched(raw_thread_attr_t *attr, int inherit)
{
	int err = E_HM_OK;

	if (attr == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (inherit != RAW_THREAD_INHERIT_SCHED &&
		    inherit != RAW_THREAD_EXPLICIT_SCHED) {
			err = E_HM_INVAL;
		} else {
			attr->inherit = (unsigned int)inherit;
		}
	}

	return err;
}

int raw_thread_attr_getschedpolicy(const raw_thread_attr_t *attr, int *policy)
{
	int err = E_HM_OK;

	if (attr == NULL || policy == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*policy = (int)attr->policy;
	}
	return err;
}

int raw_thread_attr_setschedpolicy(raw_thread_attr_t *attr, int policy)
{
	int err = E_HM_OK;

	if (attr == NULL || policy < SCHED_OTHER || policy >= SCHED_NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		attr->policy = (unsigned int)policy;
	}
	return err;
}

int raw_thread_attr_setflags(raw_thread_attr_t *attr, __u64 flags)
{
	int ret = E_HM_OK;

	if (attr == NULL) {
		ret = E_HM_INVAL;
	} else {
		attr->flags = flags;
	}
	return ret;
}

int raw_thread_attr_getmode(const raw_thread_attr_t *attr, unsigned int *mode)
{
	int err = E_HM_OK;

	if (attr == NULL || mode == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*mode = attr->mode;
	}
	return err;
}

int raw_thread_attr_setmode(raw_thread_attr_t *attr, unsigned int mode)
{
	int ret = E_HM_OK;

	if (attr == NULL) {
		ret = E_HM_INVAL;
	} else {
		attr->mode = mode;
	}
	return ret;
}
