/*
 * hw_kcollect.c
 *
 * This file use to collect kernel state and report it
 *
 * Copyright (c) 2014-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <huawei_platform/power/hw_kcollect.h>
#include <securec.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/suspend.h>
#include <linux/time.h>
#include <huawei_platform/power/hw_kstate.h>
#include <huawei_platform/power/hw_astate_dispatch.h>
#include <linux/string.h>

#define MAX_WAKE_ANCO_RUN_REASON_LENGTH 30
#define MAX_ANCO_THAW_REASON_LENGTH 32
#define ANCO_PRE_FREEZE = 1
#define ANCO_PAUSED 2
#ifdef CONFIG_HW_BINDER_RECORD
#define TRANSAC_ASYNC 0x01
#define TF_ASYNC_WAKEUP_LATER 0x100000
#define ASYNC_WAKEUP_LATER (TRANSAC_ASYNC | TF_ASYNC_WAKEUP_LATER)
#endif /* CONFIG_HW_BINDER_RECORD */

struct kcollect_info {
	s32 mask;
	u32 tv_sec; /* seconds */
	u32 tv_usec; /* microseconds */
	u32 len;
	char buffer[KCOLLECT_BUFFER_SIZE];
};

static int switch_mask = 0;
static int killed_pid = -1;
static int g_anco_status = 0;

/*
 * kstate call back
 * @return 0 for success
 */
static int kcollect_cb(channel_id src, packet_tag tag, const char *data, size_t len)
{
	int mask = 0;

	if (IS_ERR_OR_NULL(data) || (len != sizeof(mask))) {
		pr_err("hw_kcollect %s: invalid data or len:%d\n", __func__, (int)len);
		return -1;
	}

	if (memcpy_s(&mask, sizeof(mask), data, len) != EOK) {
		pr_err("hw_kcollect %s: failed to memcpy_s\n", __func__);
		return -1;
	}

	switch_mask |= mask;

	pr_info("hw_kcollect %s: src=%d tag=%d len=%d pg_switch_mask=%d\n", __func__, src, tag, (int)len, switch_mask);

	return 0;
}

static struct kstate_opt kcollect_opt = {
	.name = "kcollect",
	.tag = PACKET_TAG_KCOLLECT,
	.dst = CHANNEL_ID_KCOLLECT,
	.hook = kcollect_cb,
};

/*
 * packet and send data to userspace by kstate
 * @param mask: message mask
 * @return 0 for success
 */
static int report(int mask, va_list args, const char *fmt)
{
	int ret = -1;
	struct kcollect_info info;
	int length;
	size_t info_len;

	if (memset_s(&info, sizeof(info), 0, sizeof(info)) != EOK) {
		pr_err("hw_kcollect %s: failed to memset_s\n", __func__);
		return -1;
	}

	length = vscnprintf(info.buffer, KCOLLECT_BUFFER_SIZE - 1, fmt, args);
	if (length > 0) {
		info.mask = mask;
		info.len = length + 1;
		info.tv_sec = 0;
		info.tv_usec = 0;
		info_len = sizeof(info) - KCOLLECT_BUFFER_SIZE + length + 1;
		char *checkLevel = "[ANCO]";
		int level = 0;
		if (strstr(info.buffer, checkLevel)) {
			level = 1;
		}

		ret = kstate(CHANNEL_ID_KCOLLECT, PACKET_TAG_KCOLLECT, (char*)&info, info_len, level);
		if (ret < 0) {
			pr_err("hw_kcollect %s: kstate error\n", __func__);
			ret = -1;
		}
	} else {
		pr_err("hw_kcollect %s: vscnprintf length:%d\n", __func__, length);
	}

	pr_debug("hw_kcollect %s: length=%d mask=%d\n", __func__, length, mask);

	return ret;
}

static int get_level1_pid(int kpid)
{
	struct pid_namespace *ns_0 = NULL;
	struct pid *pid_0 = NULL;

	if (kpid == 0)
		return kpid;

	if (current == NULL || current->thread_pid == NULL || current->thread_pid->level == 0)
		return kpid;

	ns_0 = current->thread_pid->numbers[0].ns;
	if (ns_0 == NULL)
		return kpid;

	pid_0 = find_pid_ns(kpid, ns_0);
	if (pid_0 == NULL)
		return kpid;

	return pid_0->numbers[pid_0->level].nr;
}

static bool check_mask(int mask)
{
	return mask & switch_mask;
}

static int hwancoinfo(int type, char *reason, int timeout)
{
	int ret = -1;
	if (!check_mask(KCOLLECT_ANCORUNNING_MASK)) {
		pr_err("hw_kcollect %s: check_mask error, switch_mask: %d\n", __func__,
			switch_mask);
		return ret;
	}

	return kcollect(KCOLLECT_ANCORUNNING_MASK, "[TYPE %d ][REASON %s ][TIMEOUT %d ]", type, reason, timeout);
}

int hwbinderinfo(int callingpid, int calledpid)
{
	int called_pid;
	if (!check_mask(KCOLLECT_BINDERCALL_MASK)) {
		pr_err("hw_kcollect %s: check_mask error, switch_mask: %d\n", __func__,
			switch_mask);
		return -1;
	}

	called_pid = get_level1_pid(calledpid);
	return kcollect(KCOLLECT_BINDERCALL_MASK, "[TPID %d CALLED][PID %d]", called_pid, get_level1_pid(callingpid));
}

int hw_freeze_binder_info(struct kstate_binder_info *info)
{
	if (info == NULL) {
		pr_err("invalid kstate_binder_info pointer\n");
		return -EINVAL;
	}

	if (!check_mask(KCOLLECT_BINDERCALL_MASK)) {
		pr_err("hw_kcollect %s: check_mask error for freeze binder info, switch_mask: %d\n",
			__func__, switch_mask);
		return -1;
	}

	int calling_pid = info->caller_pid;
	int called_pid = get_level1_pid(info->callee_pid);
	unsigned int is_freeze = info->is_freeze;
	unsigned int type = info->type;
	unsigned int flags = info->flags;
	unsigned int level = info->callee_level;

#ifdef CONFIG_HW_BINDER_RECORD
	if (!is_freeze) {
		pr_debug("hw_kcollect %s: pid=%d not freeze.\n", __func__, called_pid);
		return 0;
	}

	if ((flags & ASYNC_WAKEUP_LATER) == ASYNC_WAKEUP_LATER) {
		pr_debug("hw_kcollect %s: calling_pid=%d, called_pid=%d wakeup later.\n", __func__, calling_pid, called_pid);
		return 0;
	}
	return kcollect(KCOLLECT_BINDERCALL_MASK, "[TPID %d CALLED][PID %d][%u][%u][%u][%u]", called_pid,
		get_level1_pid(calling_pid), is_freeze, type, flags, level);
#else
	return kcollect(KCOLLECT_BINDERCALL_MASK, "[TPID %d CALLED][PID %d][%u][%u]", called_pid,
		get_level1_pid(calling_pid), is_freeze, type);
#endif  // CONFIG_HW_BINDER_RECORD
}

void dispatch_binder_call(char *info)
{
	if (info == NULL) {
		pr_err("%s error.\n", __func__);
		return;
	}
	int callingPid = -1;
	int calledPid = -1;
	if (sscanf_s(info, "%d_%d", &callingPid, &calledPid) != 2) {
		pr_err("unable to parse input:%s\n", info);
		return;
	}

	pr_info("dispatch binder call, ancoCallingPid: %d, ancoCalledPid: %d\n", callingPid, calledPid);
	kcollect(KCOLLECT_BINDERCALL_MASK, "[TPID %d CALLED][PID %d][ANCO]", calledPid, callingPid);
}

int hwkillinfo(int pid, int seg)
{
	int ret = -1;
	if (!check_mask(KCOLLECT_FREEZER_MASK))
		return ret;

	if (killed_pid != pid) {
		ret = kcollect(KCOLLECT_FREEZER_MASK, "[PID %d KILLED][SIG %d]", get_level1_pid(pid), seg);
		killed_pid = pid;
	}

	return ret;
}

int hw_request_anco_thaw(char *reason, unsigned int size)
{
	char dest[MAX_ANCO_THAW_REASON_LENGTH];
	if (memcpy_s(dest, sizeof(dest), reason, size) != EOK) {
		pr_err("copy reason fail.");
		return -1;
	}
	pr_info("request anco running reason: %s\n", dest);
	return request_anco_running(reason);
}


int request_anco_running(char *reason)
{
	return hwancoinfo(REQUEST_ANCO_RUNNING, reason, -1);
}

int keep_anco_running(char *reason, int timeout)
{
	return hwancoinfo(KEEP_ANCO_RUNNING, reason, timeout);
}

int cancel_anco_running(char *reason)
{
	return hwancoinfo(CANCEL_ANCO_RUNNING, reason, -1);
}

int hw_packet_cb(int uid, int pid)
{
	if (uid == 0 && pid == -1)
		return -1;

	if (uid != 0 && pid == -1 && (g_anco_status == 1 || g_anco_status == 2)) {
		int ret = -1;
		char reason[MAX_WAKE_ANCO_RUN_REASON_LENGTH] = "netpacket_data_come";
		ret = request_anco_running(reason);
		pr_info("netpacket from uid=%d, request_anco_running ret=%d\n", uid, ret);
		return -1;
	}

	if (!check_mask(KCOLLECT_NETPACKET_MASK))
		return -1;

	return kcollect(KCOLLECT_NETPACKET_MASK, "[PID %d NET][UID %d]", get_level1_pid(pid), uid);
}

/*
 * collect the data and report system
 * @param mask: message mask
 * @return 0 for success
 */
int kcollect(int mask, const char *fmt, ...)
{
	va_list args;
	int ret = -1;

	if (mask & switch_mask) {
		va_start(args, fmt);
		ret = report(mask, args, fmt);
		va_end(args);
	} else {
		pr_info("hw_kcollect %s: mask %d, switch_mask %d\n",
			__func__, mask, switch_mask);
	}

	return ret;
}


static void anco_status_cb(int status)
{
	g_anco_status = status;
	pr_info("hw_kcollect ancostatus = %d\n", g_anco_status);
}

static struct anco_status_subscriber anco_status_cb_subscriber = {
	.name = "kcollect",
	.hook = anco_status_cb,
};

static int __init kcollect_init(void)
{
	subscribe_anco_status(&anco_status_cb_subscriber);
	return kstate_register_hook(&kcollect_opt);
}

static void __exit kcollect_exit(void)
{
	kstate_unregister_hook(&kcollect_opt);
	unsubscribe_anco_status(&anco_status_cb_subscriber);
}

late_initcall(kcollect_init);
module_exit(kcollect_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("collect kernel state and report");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
