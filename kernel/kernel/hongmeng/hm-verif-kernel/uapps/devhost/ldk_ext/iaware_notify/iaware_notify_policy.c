/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: policy for iaware notify
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 23 16:31:59 2023
 */

#include "iaware_notify_policy.h"
#include <hmkernel/errno.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include "iaware_notify.h"
#include "iaware_notify_api.h"
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <devhost/log.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_thread.h>

#define PID_NS_NUM 2
#define INVALID_PID (-1)

const char* mali_thread_names[] = {MALI_THREAD_NAME, MALI_UTIL_NAME, MALI_MEM_PURGE_NAME,
				   GPU_WK_CLIENT_NAME, GPU_WK_SRV_NAME, GPU_MEM_POOL_NAME};

struct evtmgr_thread_name_wl_s name_whitelist = {
	.count = DEFAULT_THREAD_NAME_NO,
	.threads = {BINDER_THREAD_NAME, MALI_THREAD_NAME, MALI_UTIL_NAME,
		    MALI_MEM_PURGE_NAME, GPU_WK_CLIENT_NAME,
		    GPU_WK_SRV_NAME, GPU_MEM_POOL_NAME},
	.event_bitmask = (1 << EVTMGR_ETYPE_THREAD_CLONE) |
			 (1 << EVTMGR_ETYPE_THREAD_SET_NAME),
};

struct evtmgr_thread_group_wl_s group_whitelist = {
	.count = DEFAULT_THREAD_GROUP_NO,
	.groups = {VIP_GROUP},
	.event_bitmask = (1 << EVTMGR_ETYPE_THREAD_CLONE),
};

static int get_level1_tid(int ktid)
{
	int tids[PID_NS_NUM];
	int res_tid = INVALID_PID;
	int err;

	for (int i = 0; i < PID_NS_NUM; i++) {
		tids[i] = INVALID_PID;
	}

	err = hm_thread_nstid_of_root(ktid, tids, PID_NS_NUM);
	if (err != E_HM_OK) {
		hm_error("dh_iaware_notify: %s get tid:%d failed\n", __func__, ktid);
	} else {
		for (int j = 0; j < PID_NS_NUM; j++) {
			if (tids[j] != INVALID_PID) {
				res_tid = tids[j];
			}
		}
	}
	hm_info("dh_iaware_notify: %s get tid:%d/%d\n", __func__, ktid, res_tid);
	return res_tid;
}

static int send_to_cpu_netlink(int sock_no, int pid, int tid)
{
	struct cpu_netlink_event_s event;
	int tmp_id;
	event.sock_no = sock_no;
	event.len = CPU_NETLINK_DATA_LEN;
	event.data[0] = tid;
	event.data[1] = pid;

	for (int i = 0; i < event.len; i++) {
		tmp_id = get_level1_tid(event.data[i]);
		if (tmp_id != INVALID_PID) {
			event.data[i] = tmp_id;
		}
	}

	hm_debug("dh_iaware_notify: send_to_cpu_netlink,sock_no=%d, pid=%d, tid=%d\n",
		 sock_no, event.data[1], event.data[0]);

	return iaware_notify_ops->send_to_cpu_netlink(&event);
}

static bool check_vip_status(int group)
{
	return group == VIP_GROUP;
}

static bool check_name_whitelist(const char *name)
{
	for (int i = DEFAULT_THREAD_NAME_NO; i < name_whitelist.count; i++) {
		if (strstr(name, name_whitelist.threads[i])) {
			return true;
		}
	}
	return false;
}

static bool is_mali_thread(const char* comm)
{
	int length = sizeof(mali_thread_names) / sizeof(mali_thread_names[0]);
	for (int i = 0; i < length; i++) {
		if (strstr(comm, mali_thread_names[i]) != NULL)
			return true;
	}
	return false;
}

static int handle_fork_event(struct evtcb_thread_msg_s *msg)
{
	int pid;
	int tid;
	const char *name = NULL;
	int ret = E_HM_OK;
	int group;

	pid = msg->pid;
	tid = msg->tid;
	name = msg->name;
	group = msg->group;

	if (group < 0) {
		return E_HM_INVAL;
	}

	hm_debug("dh_iaware_notify: handle fork event,pid=%d,tid=%d,name=%s,group=%d\n",
		 pid, tid, name, group);
	if (check_vip_status(group)) {
		ret = send_to_cpu_netlink(PROC_FORK, pid, tid);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	if (check_name_whitelist(name)) {
		ret = send_to_cpu_netlink(PROC_AUX_COMM_FORK, pid, tid);
	}

	return ret;
}

static int handle_set_name_event(struct evtcb_thread_msg_s *msg)
{
	int sock_no = 0;
	int pid;
	int tid;
	const char *new_name = NULL;
	const char *name = NULL;
	int ret = E_HM_OK;

	pid = msg->pid;
	tid = msg->tid;
	name = msg->name;
	new_name = msg->new_name;

	if (new_name == NULL) {
		return E_HM_INVAL;
	}

	hm_debug("dh_iaware_notify: handle set name event,pid=%d,tid=%d,name=%s,new_name=%s\n",
		  pid, tid, name, new_name);
	if (strstr(new_name, BINDER_THREAD_NAME)) {
		sock_no = PROC_COMM;
	} else if (is_mali_thread(new_name)) {
		sock_no = PROC_MALI_COMM;
	} else if (check_name_whitelist(new_name)) {
		sock_no = PROC_AUX_COMM;
	} else if (check_name_whitelist(name)) {
		sock_no = PROC_AUX_COMM_REMOVE;
	} else {
		return E_HM_OK;
	}

	if (sock_no != 0) {
		ret = send_to_cpu_netlink(sock_no, pid, tid);
	}

	return ret;
}

int thread_event_handler(struct evtcb_thread_msg_s *msg)
{
	int event_type;
	int pid;
	int tid;
	const char *name = NULL;
	int ret = E_HM_OK;

	if (msg == NULL) {
		return E_HM_INVAL;
	}

	pid = msg->pid;
	tid = msg->tid;
	name = msg->name;
	event_type = msg->event_type;

	if (pid < -1 || tid < 0 || name == NULL) {
		return E_HM_INVAL;
	}

	if (event_type == EVTMGR_ETYPE_THREAD_CLONE) {
		ret = handle_fork_event(msg);
	} else if (event_type == EVTMGR_ETYPE_THREAD_SET_NAME) {
		ret = handle_set_name_event(msg);
	} else {
		hm_debug("dh_iaware_notify: revieve invailed type event");
		return E_HM_INVAL;
	}

	return ret;
}

int set_default_whitelist(void)
{
	int ret;
	uref_t dh_iaware_notify_uref;

	dh_iaware_notify_uref = dh_iaware_notify_uref_of();

	ret = hm_event_rule_ctrl(dh_iaware_notify_uref, EVTMGR_ERULE_SET_THREAD_GROUP_WL,
				 (void*)&group_whitelist, sizeof(group_whitelist));
	if (ret != E_HM_OK) {
		hm_error("iaware_notify set default name policy to eventmgr failed,ret=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = hm_event_rule_ctrl(dh_iaware_notify_uref, EVTMGR_ERULE_SET_THREAD_NAME_WL,
				 (void*)&name_whitelist, sizeof(name_whitelist));
	if (ret != E_HM_OK) {
		hm_error("iaware_notify set default group policy to eventmgr failed,ret=%s\n", hmstrerror(ret));
	}

	return ret;
}

int set_thread_name_whitelist(char *buffer)
{
	int ret = E_HM_OK;
	char *tok = NULL;
	uref_t dh_iaware_notify_uref;

	if (buffer == NULL) {
		return E_HM_INVAL;
	} else {
		tok = strsep(&buffer, ":");
		while (buffer != NULL) {
			if (name_whitelist.count >= EVTMGR_THREAD_WL_MAX) {
				hm_error("Supports up to %d aux key thread\n", EVTMGR_THREAD_WL_MAX);
				return E_HM_INVAL;
			}
			tok = strsep(&buffer, ",");
			if (tok && *tok != '\0') {
				NOFAIL(strncpy_s(name_whitelist.threads[name_whitelist.count],
						 HMCAP_THREAD_NAME_SIZE,
						 tok, HMCAP_THREAD_NAME_SIZE - 1));
				name_whitelist.count++;
			}
		}
	}

	dh_iaware_notify_uref = dh_iaware_notify_uref_of();
	ret = hm_event_rule_ctrl(dh_iaware_notify_uref, EVTMGR_ERULE_SET_THREAD_NAME_WL,
				 (void*)&name_whitelist, sizeof(name_whitelist));

	return ret;
}
