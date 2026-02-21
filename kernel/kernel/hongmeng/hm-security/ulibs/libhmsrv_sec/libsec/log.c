/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Log information denied by security framework
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 23 22:48:00 2020
 */
#include <libsec/log.h>
#include <libsec/utility.h>
#include <hmkernel/const.h>
#include <hmsysmgr/dumpstack.h>
#include <libsysif/utils.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

#ifdef CONFIG_SEC_DENY_LOG
#define LOG_BUF_SIZE 512U
#endif

void __attribute__((weak)) sec_chk_show_obj_name(const struct sec_chk_cred *cred,
						 void *obj)
{
	int ret;
	pid_t pid;
	char *exe = NULL;

	ret = sec_log_read_pid_node(cred, &pid, "exe", &exe);
	if (ret == E_HM_OK) {
		if (obj == NULL) {
			sec_deny_log("operation denied. (process=0x%x, pid=%d, exe=%s)\n",
				     cred->cnode_idx, pid, exe);
		} else {
			sec_deny_log("operation denied. (process=0x%x, pid=%d, exe=%s, obj=%s)\n",
				     cred->cnode_idx, pid, exe,
				     (const char *)obj);
		}
		free(exe);
	} else {
		hm_error("failed to get 'exe' information. (process=0x%x, pid=%d, ret=%s)\n",
			 cred->cnode_idx, pid, hmstrerror(ret));
	}
}

#ifdef CONFIG_SEC_DENY_LOG
static void __attribute__((format(printf, 3, 0)))
sec_log_vprintf(const struct sec_deny_log_info *info, pid_t pid,
		const char *fmt, va_list args)
{
	char *exe_tmp = NULL;
	char format_buf[LOG_BUF_SIZE] = {0};
	int err;

	(void)sec_log_read_pid_exe(pid, "exe", &exe_tmp);
	err = sprintf_s(format_buf, LOG_BUF_SIZE-1, "[pid=%d, exe=%s]:%s",
			pid, (exe_tmp==NULL) ? "unknow" : exe_tmp, fmt);
	if(err < 0) {
		__hmlog_printf(info->level, NULL, info->func, info->line,
			       "[pid=%d, exe=%s]: format is too long(MAX_LEN is %u)\n",
			       pid, (exe_tmp==NULL) ? "unknow" : exe_tmp,
			       LOG_BUF_SIZE);
	} else {
		__hmlog_vprintf(info->level, info->func, info->line,
				format_buf, args);
	}
	if (exe_tmp != NULL) {
		free(exe_tmp);
	}
}

static int sec_log_basic_init(struct sec_cred_basic *basic,
			      const struct sec_deny_log_info *info)
{
	int ret = E_HM_OK;

	if (info->basic != NULL) {
		*basic = *info->basic;
		return ret;
	}
	if (info->cred == NULL) {
		struct sec_chk_cred chk = {0};
		ret = sec_chk_cred_init_requester(&chk);
		if (ret == E_HM_OK) {
			*basic = *basic_cred_of_sec_cred(&chk);
		}
	} else {
		*basic = *basic_cred_of_sec_cred(info->cred);
	}
	return ret;
}
#endif

void sec_dump_stack(void)
{
#ifdef CONFIG_SEC_DENY_LOG
	unsigned int flags = 0U;
	dumpstack_set_flag(flags, DUMPSTACK_FLAGS_STACK_BACKTRACE);
	(void)actvcall_hmcall_dump_sec_deny(flags);
#endif
}

void __attribute__((format(printf, 2, 3)))
sec_log_debug(const struct sec_deny_log_info *info, const char * fmt, ...)
{
#ifdef CONFIG_SEC_DENY_LOG
	unsigned int mode = (unsigned int)hm_sec_debug_get_mode();
	if (mode == HM_SEC_SHCONF_SEC_DEBUG_DISABLE) {
		return;
	}
	struct sec_cred_basic basic = {0};
	if (sec_log_basic_init(&basic, info) != E_HM_OK) {
		return;
	}
	if (info->is_log_enabled || sec_log_is_enabled(mode, basic.flags)) {
		va_list args;
		va_start(args, fmt);
		sec_log_vprintf(info, basic.pid, fmt, args);
		va_end(args);
	}
	if (info->is_trace_disabled == false &&
	    sec_log_trace_is_enabled(mode, basic.flags)) {
		sysif_set_sec_dump_hook(sec_dump_stack);
	}
#else
	UNUSED(info, fmt);
#endif
}
