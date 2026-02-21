/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: lnxbase time related helper
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 28 10:44:55 2021
 */
#include <lnxbase/lnxbase.h>

#include <sys/auxv.h>
#include <hmkernel/mm/udata.h>
#include <hmkernel/vtimekeep.h>
#include <hongmeng/syscall.h>
#ifdef __aarch64__
#include <hmasm/barrier.h> /* for isb() */
#include <hmasm/pmu.h> /* for read_sysreg() */
#endif

#include <devhost/log.h>

static const struct kshare_udata_s *ks_udata = NULL;

static __u64 __sysfast_tock_read(void)
{
	__u64 tock;

#ifdef __aarch64__
	/* prefer cntpct_el0 for aarch64 */
	isb();
#ifdef CONFIG_TIMER_HARDEN_S4
	tock = read_sysreg("cntpct_el0") + ks_udata->pcs4offset;
#else
	tock = read_sysreg("cntpct_el0");
#endif
#else
	/* fall back to sysfast tock */
	if (sysfast_tock_read(VTIMEKEEP_CLOCK_MONOTONIC, (__u64 *)&tock) < 0) {
		tock = ks_udata->tock_last;
	}
#endif
	return tock;
}

static struct lnxbase_clocksource default_cs = {
	.read = __sysfast_tock_read,
	.mult = 0U,
	.shift = 0U,
};

const struct lnxbase_clocksource *lnxbase_default_clocksource(void)
{
	if (ks_udata == NULL) {
		unsigned long vdso_base = getauxval(AT_SYSINFO_EHDR);
		/* query kshare udata and init default clocksource */
		if (vdso_base != 0) {
			ks_udata = (struct kshare_udata_s *)(vdso_base - UDATA_PMEM_SIZE);
			default_cs.mult = ks_udata->mul;
			default_cs.shift = ks_udata->mov;
		} else {
			dh_error("lnxbase: get auxv AT_SYSINFO_EHDR failed\n");
			return NULL;
		}
	}

	return &default_cs;
}
