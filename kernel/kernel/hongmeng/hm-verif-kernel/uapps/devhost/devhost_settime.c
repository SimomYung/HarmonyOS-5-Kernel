/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: devhost settime server support
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 15 15:28:42 2021
 */

#include "devhost.h"

#include <devhost_api.h>
#include <hmkernel/capability.h>
#include <hmkernel/interrupt/kvic.h>
#include <hmkernel/sched/priority.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/trigger.h>
#include <hongmeng/errno.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsync/raw_thread.h>
#include <libstrict/strict.h>

#include <devhost/log.h>

static cref_t settime_server;
static void (*g_time_sync_handler)(void) = NULL;

static void *devhost_settime_handler(void *args)
{
	int err = E_HM_OK;

	UNUSED(args);
	BUG_ON(g_time_sync_handler == NULL);

	err = hm_thread_setname(0, DH_SETTIME_TNAME, NULL);
	if (err < 0) {
		dh_warn("set time thread name failed, err=%s\n", hmstrerror(err));
	}

	backend_ctx_setup();
	while ((err >= 0) || (err == E_HM_HMSIGINTR)) {
		err = hm_trigger_wait(__TRIGGER_WAIT_DONE, NULL);
		if (err >= 0) {
			/* devhost settime core handler func */
			g_time_sync_handler();
		}
	}

	hm_panic("time thread can't auto recover, err = %s\n", hmstrerror(err));
	return NULL;
}

static int devhost_time_sync_sched_attr_init(raw_thread_attr_t *attr)
{
	int err = E_HM_OK;
	unsigned int priority = (unsigned int)hm_thread_sched_getdefaultpriority();
	int policy = hm_thread_sched_getdefaultscheduler();
	struct raw_sched_param param;

	if (policy == SCHED_RR) {
		priority = HM_PRIORITY_DH_HIGH;
	}

	err = raw_thread_attr_init(attr);
	if (err != E_HM_OK) {
		dh_error("settime_server raw_thread_attr_init failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = raw_thread_attr_setinheritsched(attr, RAW_THREAD_EXPLICIT_SCHED);
	if (err != E_HM_OK) {
		dh_error("settime_server raw_thread_attr_setinheritsched failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = raw_thread_attr_setschedpolicy(attr, policy);
	if (err != E_HM_OK) {
		dh_error("settime_server raw_thread_attr_setschedpolicy failed, err=%s\n", hmstrerror(err));
		return err;
	}
	mem_zero_s(param);
	param.priority = priority;
	err = raw_thread_attr_setschedparam(attr, &param);
	if (err != E_HM_OK) {
		dh_error("settime_server raw_thread_attr_setschedparam failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int devhost_time_sync_init(void (*handler)(void))
{
	int err = E_HM_OK;
	raw_thread_t thread;
	raw_thread_attr_t attr;
	/* register time sync handler */
	if (g_time_sync_handler != NULL) {
		dh_error("time sync handler has been registered\n");
		return E_HM_INVAL;
	} else {
		g_time_sync_handler = handler;
	}

	err = devhost_time_sync_sched_attr_init(&attr);
	if (err != E_HM_OK) {
		dh_error("failed to set sched_attr for devhost time\n");
		return err;
	}

	/* setup settime server */
	err = raw_thread_create(&thread, &attr, devhost_settime_handler, NULL);
	if (err != E_HM_OK) {
		dh_error("failed to create settime handler\n");
		return err;
	}
	settime_server = raw_thread_cref_of(thread);

	err = hm_thread_kvic_source_bind(settime_server, (unsigned int)__KVIC_SOURCE_ID_SETTIME, 0);
	if (err < 0) {
		dh_error("settime source bind failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}
