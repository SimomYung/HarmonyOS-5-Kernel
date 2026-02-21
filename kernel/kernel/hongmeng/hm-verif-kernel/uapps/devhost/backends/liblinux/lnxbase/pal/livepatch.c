/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: liblinux PAL for kernel livepatching
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 17 10:00:00 2022
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <hongmeng/syscall.h>
#include <hmkernel/capability.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_thread.h>

#include <devhost/log.h>

static int check_actv_stack(klp_stacktrace_t trace_callback, void *args)
{
	cref_t cref;
	int err;
	int ret = 0;
	unsigned long fp, pc, tls;
	struct arch_uctx uctx;

	hm_ucap_for_each_cap(0, hmobj_Activation, cref, err) {
		mem_zero_s(uctx);
		ret = syscap_ActivationLoadActvUctx(cref, &uctx, NULL);
		if (ret == E_HM_ACTIVATION_WORKING || ret == E_HM_ACTIVATION_INACTIVE) {
			fp = __REG_FP(&(uctx.regs));
			pc = __REG_PC(&(uctx.regs));
			tls = __REG_TLS(&(uctx.regs));
			ret = trace_callback(fp, pc, tls, args);
			if (ret != 0) {
				return ret;
			}
		} else if (ret == E_HM_ACTIVATION_STATE) {
			/* skipping unused actvs */
			ret = 0;
			continue;
		} else {
			/* break on error conditions */
			ret = -hmerrno2posix(ret);
			break;
		}
	}
	return ret;
}

static int check_thread_stack(klp_stacktrace_t trace_callback, void *args)
{
	int err;
	int ret = 0;
	cref_t iter, cref;
	unsigned long fp, pc, tls;
	struct sysarg_actv_stat stat;

	hm_ucap_for_each_cap(0, hmobj_Thread, cref, err) {
		iter = 0ULL;
		mem_zero_s(stat);
		while (hm_actv_load_stat(&stat, &iter, &cref, __ACTV_LOAD_STAT_UCTX) == E_HM_OK) {
			if (iter != 0ULL) {
				/* only examine base actvs */
				continue;
			}
			fp = __REG_FP(&(stat.uctx.regs));
			pc = __REG_PC(&(stat.uctx.regs));
			tls = __REG_TLS(&(stat.uctx.regs));
			ret = trace_callback(fp, pc, tls, args);
			if (ret != 0) {
				return ret;
			}
			break;
		}
	}
	return ret;
}

/*
 * This works like for_each_process_thread in Linux, for thread iteration in devhost.
 * For each thread(native thread and actv thread), it calls trace_callback with args.
 */
int liblinux_pal_klp_for_each_thread(klp_stacktrace_t trace_callback, void *args)
{
	int ret;

	ret = check_actv_stack(trace_callback, args);
	if (ret == 0) {
		ret = check_thread_stack(trace_callback, args);
	}
	return ret;
}

