/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: serverside control point implementation of seccomp
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 19 9:18:09 2023
 */
#include <libhmseccomp/seccomp.h>

#include <hmkernel/extensions/lsyscall.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libhmsync/raw_mutex.h>
#include <libhmaudit/audit.h>
#include <libhmaudit/audit_log.h>
#include <hongmeng/syscall.h>

bool __ro_after_init g_seccomp_fast_access_mode = false;
#ifdef CONFIG_SECCOMP
#include <libhmcbpf/cbpf_seccomp.h>
#include <libhmcbpf/cbpf.h>

static inline bool seccomp_is_linux_sys_nr_defined(int linux_syscall_number)
{
	return linux_syscall_number >= 0;
}

static inline bool seccomp_is_valid_syscall_map(int linux_syscall_number,
						int native_syscall_number)
{
	bool ret = false;
	UNUSED(native_syscall_number);

	/* linux syscall number is undefined */
	if (seccomp_is_linux_sys_nr_defined(linux_syscall_number)) {
		ret = true;
	}

	return ret;
}

static void audit_seccomp(unsigned long syscallnu, long signr, unsigned int action, struct hm_seccomp_data_s *sc_data)
{
	int in_compat_syscall = 0;
#ifdef CONFIG_COMPAT
	if(sc_data->arch == AUDIT_ARCH_ARM) {
		in_compat_syscall = 1;
	}
#endif
#ifdef CONFIG_AUDIT
	(void)audit_log_send(HM_AUDIT_SECCOMP, TYPE_AUDIT_COMMON | TYPE_AUDIT_FORMAT,
		     "sig=%ld arch=0x%x syscall=%ld compat=%d ip=0x%lx code=0x%x",
		     signr, sc_data->arch, syscallnu, in_compat_syscall, 0, action);
#else
	hm_info("sig=%ld arch=0x%x syscall=%lu compat=%d ip=%d code=0x%x",
		signr, sc_data->arch, syscallnu, in_compat_syscall, 0, action);
#endif
}

/* For use with hm_seccomp_actions_logged */
#define HM_SECCOMP_LOG_KILL_PROCESS		(1)
#define HM_SECCOMP_LOG_KILL_THREAD		(1 << 1)
#define HM_SECCOMP_LOG_TRAP			(1 << 2)
#define HM_SECCOMP_LOG_ERRNO			(1 << 3)
#define HM_SECCOMP_LOG_TRACE			(1 << 4)
#define HM_SECCOMP_LOG_LOG			(1 << 5)
#define HM_SECCOMP_LOG_ALLOW			(1 << 6)
#define HM_SECCOMP_LOG_USER_NOTIF		(1 << 7)

static const unsigned int hm_seccomp_actions_logged = HM_SECCOMP_LOG_KILL_PROCESS |
						      HM_SECCOMP_LOG_KILL_THREAD |
						      HM_SECCOMP_LOG_ERRNO |
							  HM_SECCOMP_ACTION_TRACE |
						      HM_SECCOMP_LOG_TRAP |
						      HM_SECCOMP_LOG_LOG;

static void seccomp_log(unsigned long lsyscall_nu, long signr, unsigned int action, struct hm_seccomp_data_s *sc_data,
			bool request)
{
	bool log = false;

	switch (action) {
	case HM_SECCOMP_ACTION_ALLOW:
		break;
	case HM_SECCOMP_ACTION_TRAP:
		log = request && (hm_seccomp_actions_logged & HM_SECCOMP_LOG_TRAP);
		break;
	case HM_SECCOMP_ACTION_ERRNO:
		log = request && (hm_seccomp_actions_logged & HM_SECCOMP_LOG_ERRNO);
		break;
	case HM_SECCOMP_ACTION_TRACE:
		log = request && (hm_seccomp_actions_logged & HM_SECCOMP_LOG_TRACE);
		break;
	case HM_SECCOMP_ACTION_NOTIF:
		log = request && (hm_seccomp_actions_logged & HM_SECCOMP_LOG_USER_NOTIF);
		break;
	case HM_SECCOMP_ACTION_LOG:
		log = hm_seccomp_actions_logged & HM_SECCOMP_LOG_LOG;
		break;
	case HM_SECCOMP_ACTION_KILL_THREAD:
		log = hm_seccomp_actions_logged & HM_SECCOMP_LOG_KILL_THREAD;
		break;
	case HM_SECCOMP_ACTION_KILL_PROCESS:
	default:
		log = hm_seccomp_actions_logged & HM_SECCOMP_LOG_KILL_PROCESS;
	}

	if (!log) {
		return;
	}
	audit_seccomp(lsyscall_nu, signr, action, sc_data);
}

static int seccomp_run_cbpf_filter(struct seccomp_filter_sysmgr *curr_filter, struct hm_seccomp_data_s *sc_data,
			    unsigned int *ret_action, unsigned short *ret_data, bool is_fast_path)
{
	struct seccomp_filter_sysmgr *curr_filter_sys = curr_filter;
	struct seccomp_filter_s *curr_filter_u = curr_filter_sys->sc_filter;
	unsigned int ret = HM_SECCOMP_ACTION_ALLOW;
	unsigned int ret_cur = HM_SECCOMP_ACTION_ALLOW;
	struct cbpf_prog_s *cbpf_prog = NULL;

	while (curr_filter_u != NULL) {
		if (is_fast_path) {
			cbpf_prog = curr_filter_sys->prog;
		} else {
			cbpf_prog = sec_filter_idx_to_ptr(curr_filter_u->prog_idx);
		}
		if (cbpf_prog->cbpf_func != NULL && is_fast_path) {
			ret_cur = cbpf_prog->cbpf_func((void *)sc_data,
						       (struct cbpf_sock_filter_s *)cbpf_prog->filter,
						       sizeof(struct hm_seccomp_data_s));
		} else {
			ret_cur = cbpf_exec((void *)sc_data,
					    (struct cbpf_sock_filter_s *)cbpf_prog->filter,
					    sizeof(struct hm_seccomp_data_s));
		}
		if (HM_SECCOMP_ACTION_ONLY(ret_cur) < HM_SECCOMP_ACTION_ONLY(ret)) {
			ret = ret_cur;
		}
		if (is_fast_path) {
			curr_filter_sys = curr_filter_sys->prev;
			curr_filter_u = curr_filter_sys == NULL ? NULL : curr_filter_sys->sc_filter;
		} else {
			curr_filter_u = sec_filter_idx_to_ptr(curr_filter_u->prev_idx);
		}
	}

	*ret_action = ret & HM_SECCOMP_ACTION_FULL;
	*ret_data = ret & HM_SECCOMP_ACTION_DATA;

	return E_HM_OK;
}

static int seccomp_handle_trace_action(unsigned short data, bool is_fast_path)
{
	/* call actvcall to enable ptrace event */
	if (hm_pdebug_seccomp_event(ulong_to_ptr(data, void)) != E_HM_OK) {
		return (int)(HM_MAX_ERRNO | HM_SECCOMP_ERRNO_TAG);
	}

	/* get latest filter data after possible changes caused by event */
	struct hm_seccomp_data_s sc_data = {0};
	int ret = hm_seccomp_refresh_data((void *)&sc_data, sizeof(sc_data));
	if (ret != E_HM_OK) {
		hm_error("seccomp refresh data form kernel failed, error: %s\n", hmstrerror(ret));
		return (int)(HM_MAX_ERRNO | HM_SECCOMP_ERRNO_TAG);
	}

	if (!seccomp_is_valid_syscall_map((int)sc_data.linux_syscall_number, (int)sc_data.native_syscall_number)) {
		return (int)(HM_MAX_ERRNO | HM_SECCOMP_ERRNO_TAG);
	}

	/* recompute seccomp rule after event */
	unsigned int ret_action = 0;
	unsigned short ret_data = 0;
	struct seccomp_filter_sysmgr *curr_filter = (struct seccomp_filter_sysmgr *)seccomp_current_filter_of_caller();
	ret = seccomp_run_cbpf_filter(curr_filter, &sc_data, &ret_action, &ret_data, is_fast_path);
	BUG_ON(ret != E_HM_OK);
	/* handle ret actions of seccomp_bpf */
	struct seccomp_action_flags seccomp_action_flags = {
		curr_filter->sc_filter->log,
		true,
		is_fast_path
	};
	ret = seccomp_handle_actions(ret_action, ret_data, &sc_data, seccomp_action_flags);
	return ret;
}

int seccomp_handle_actions(unsigned int action, unsigned short data, struct hm_seccomp_data_s *sc_data,
					struct seccomp_action_flags seccomp_action_flags)
{
	int ret = E_HM_OK;
	switch (action) {
	case HM_SECCOMP_ACTION_ALLOW:
		break;
	case HM_SECCOMP_ACTION_ERRNO:
		if (data > HM_MAX_ERRNO) {
			data = HM_MAX_ERRNO;
		}
		ret = (int)(data | HM_SECCOMP_ERRNO_TAG);
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), 0, action, sc_data, seccomp_action_flags.request);
		break;
	case HM_SECCOMP_ACTION_LOG:
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), 0, action, sc_data, true);
		break;
	case HM_SECCOMP_ACTION_TRACE:
		if (seccomp_action_flags.recheck_after_trace) {
			break;
		}
		ret = seccomp_handle_trace_action(data, seccomp_action_flags.is_fast_path);
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), 0, action, sc_data, true);
		break;
	case HM_SECCOMP_ACTION_NOTIF:
		ret = (int) E_HM_NOSYS;
		break;
	case HM_SECCOMP_ACTION_KILL_PROCESS:
		ret = (int)__ACTV_WFERET_SECCOMP_KILL_PROCESS;
		hm_thread_set_kill_signal(seccomp_current_tid_of());
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), SIGSYS, action, sc_data, true);
		break;
	case HM_SECCOMP_ACTION_TRAP:
		ret = (int)__ACTV_WFERET_SECCOMP_TRAP | data;
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), 0, action, sc_data, true);
		break;
	case HM_SECCOMP_ACTION_KILL_THREAD:
	default:
		ret = (int)__ACTV_WFERET_SECCOMP_KILL;
		hm_thread_set_kill_signal(seccomp_current_tid_of());
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), SIGSYS, action, sc_data, true);
		break;
	}

	return ret;
}

static int do_secure_computing_filtering(struct seccomp_filter_sysmgr *curr_filter, bool is_fast_path)
{
	int ret = E_HM_OK;
	struct hm_seccomp_data_s *sc_data;
	unsigned int ret_action = 0;
	unsigned short ret_data = 0;

	BUG_ON(curr_filter == NULL);
	sc_data = (struct hm_seccomp_data_s *)seccomp_current_info_of();
	BUG_ON(sc_data == NULL);
	if (!seccomp_is_valid_syscall_map((int)sc_data->linux_syscall_number, (int)sc_data->native_syscall_number)) {
		hm_error("invalid syscall map, linux syscall nr: %u, native syscall nr: %u\n",
			sc_data->linux_syscall_number, sc_data->native_syscall_number);
		ret = E_HM_INVAL;
	}
	/* run seccomp-bpf filter on seccomp_data */
	if (ret == E_HM_OK) {
		ret = seccomp_run_cbpf_filter(curr_filter, sc_data, &ret_action, &ret_data, is_fast_path);
		BUG_ON(ret != E_HM_OK);
		/* handle ret actions of seccomp_bpf */
		struct seccomp_action_flags seccomp_action_flags = {
			curr_filter->sc_filter->log,
			false,
			is_fast_path
		};
		ret = seccomp_handle_actions(ret_action, ret_data, sc_data, seccomp_action_flags);
	}

	return ret;
}

static int seccomp_strict_mode(void)
{
	unsigned int i;
	int ret = E_HM_INVAL;
	int this_scno = seccomp_current_linux_scno_of();
	BUG_ON(this_scno < 0);

	for (i = 0; i < (sizeof(strict_allowed_syscalls) / sizeof(int)); i++) {
		if (this_scno == strict_allowed_syscalls[i]) {
			ret = E_HM_OK;
			break;
		}
	}

	if (ret != E_HM_OK) {
		seccomp_log((unsigned long)seccomp_current_linux_scno_of(), SIGKILL,
			    HM_SECCOMP_ACTION_KILL_THREAD, seccomp_current_info_of(), true);
		hm_thread_set_kill_signal(seccomp_current_tid_of());
		ret = (int)__ACTV_WFERET_SECCOMP_KILL_STRICT;
	}

	return ret;
}

/* a secure computing fastpath for fusionmgr, it exploits the fact that
 * vfs service and sysmgr are in the same address space to access the
 * seccomp filter directly, instead of performing an actv rpc call.
 */
static int hm_secure_computing_with_filter(struct seccomp_filter_sysmgr *curr_filter, bool is_fast_path)
{
	int ret = E_HM_OK;
	if (curr_filter != NULL && hm_actv_local_seccomp_need_check_thiscall()) {
		if ((unsigned int)curr_filter->sc_filter->state == HM_SECCOMP_STATE_COMPAT_STRICT) {
			ret = seccomp_strict_mode();
			if (ret != E_HM_OK) {
				hm_error("strict mode does not allow this syscall, err=0x%x\n", ret);
			}
		} else {
			ret = do_secure_computing_filtering(curr_filter, is_fast_path);
		}
	}
	return ret;
}

int hm_secure_computing_fusionmgr(void)
{
	struct seccomp_filter_sysmgr *curr_filter = (struct seccomp_filter_sysmgr *)seccomp_current_filter_of_caller();
	if (curr_filter != NULL) {
		return hm_secure_computing_with_filter(curr_filter, true);
	}
	return E_HM_OK;
}

int hm_secure_computing_copath(void)
{
	int ret = E_HM_OK;
	struct seccomp_filter_sysmgr curr_filter = { 0 };
	cofilter_idx idx = seccomp_current_cofilter_index_of_caller();
	if (idx != 0) {
		ret = sec_cofilter_init(false);
		if (ret == E_HM_OK) {
			curr_filter.sc_filter = sec_filter_idx_to_ptr(idx);
			if (curr_filter.sc_filter != NULL) {
				ret = hm_secure_computing_with_filter(&curr_filter, false);
			}
		}
	}
	return ret;
}

#ifdef CONFIG_SECCOMP_CACHE_DEBUG
static int proc_pid_seccomp_cache_arch(char *dest, size_t lens, const char *name,
					const void *bitmap)
{
	unsigned int nr;
	int ret;
	char temp[32]; // 32 is sufficient for storing syscall information once.

	for (nr = 0; nr < MAX_LINUX_SYSCALL_NUMBER; nr++) {
		bool cached = raw_bitmap_test_bit(bitmap, (unsigned int)MAX_LINUX_SYSCALL_NUMBER, nr);
		const char *status = cached ? "ALLOW" : "FILTER";
		ret = sprintf_s(temp, sizeof(temp), "%s %u %s\n", name, nr, status);
		if (ret < 0) {
			hm_error("copy seccomp proc info failed!\n");
			return E_HM_INVAL;
		}
		if (strlen(dest) + strlen(temp) < lens) {
			ret = strcpy_s(dest + strlen(dest), lens - strlen(dest), temp);
		}
		if (ret != 0) {
			hm_error("copy seccomp proc info failed!\n");
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

int proc_seccomp_cache_info(char *dest, size_t lens, unsigned long long *allow_syscalls_native,
			     unsigned long long *allow_syscalls_compat)
{
	int ret;
	if (dest == NULL || lens == 0) {
		return E_HM_INVAL;
	}

	ret = proc_pid_seccomp_cache_arch(dest, lens, HM_SECCOMP_ARCH_NATIVE_NAME,
					  allow_syscalls_native);
#ifdef CONFIG_COMPAT
	if (ret == E_HM_OK) {
		ret = proc_pid_seccomp_cache_arch(dest, lens, HM_SECCOMP_ARCH_COMPAT_NAME,
						  allow_syscalls_compat);
	}
#else
	UNUSED(allow_syscalls_compat);
#endif
	return ret;
}
#else
int proc_seccomp_cache_info(char *dest, size_t lens, unsigned long long *allow_syscalls_native,
			    unsigned long long *allow_syscalls_compat)
{
	UNUSED(dest, lens, allow_syscalls_native);
	return E_HM_NOSYS;
}
#endif
#else
int hm_secure_computing_fusionmgr(void)
{
	return E_HM_OK;
}

int hm_secure_computing_copath(void)
{
	return E_HM_OK;
}

int seccomp_handle_actions(unsigned int action, unsigned short data, struct hm_seccomp_data_s *sc_data,
				struct seccomp_action_flags seccomp_action_flags)
{
	UNUSED(action, data, sc_data, seccomp_action_flags);
	return E_HM_OK;
}
#endif /* CONFIG_SECCOMP */

