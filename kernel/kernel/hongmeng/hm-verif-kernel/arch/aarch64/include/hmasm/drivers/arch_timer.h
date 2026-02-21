/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ARM generic timer architecture support
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 08 16:16:22 2020
 */
#ifndef A64_ASM_DRIVERS_ARCH_TIMER_H
#define A64_ASM_DRIVERS_ARCH_TIMER_H

#include <hmasm/barrier.h>
#include <hmasm/processor.h>
#include <hmkernel/compiler.h>

#ifndef __ASSEMBLY__

static inline u64 arch_timer_get_cntpctl(void)
{
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	return read_sysreg("CNTV_CTL_EL0");
#else
	return read_sysreg("CNTP_CTL_EL0");
#endif
}

static inline void arch_timer_set_cntpctl(u64 ctrl)
{
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	write_sysreg("CNTV_CTL_EL0", ctrl);
#else
	write_sysreg("CNTP_CTL_EL0", ctrl);
#endif
}

static inline u64 _arch_timer_read_coarse(void)
{
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	u64 ret_tock = read_sysreg("CNTVCT_EL0");
#else
	u64 ret_tock = read_sysreg("CNTPCT_EL0");
#endif
	return ret_tock;
}

static inline u64 _arch_timer_read(void)
{
	isb();
	u64 ret_tock = _arch_timer_read_coarse();
	return ret_tock;
}

#ifdef CONFIG_TIMER_HARDEN_S4
extern u64 vtimekeep_pcs4_offset(void);
#else
static inline u64 vtimekeep_pcs4_offset(void)
{
	return 0;
}
#endif

static inline u64 arch_timer_read(void)
{
	u64 ret = vtimekeep_pcs4_offset() + _arch_timer_read();
	return ret;
}

static inline u64 arch_timer_read_coarse(void)
{
	u64 ret = vtimekeep_pcs4_offset() + _arch_timer_read_coarse();
	return ret;
}

static inline u64 arch_timer_get_cntpcval(void)
{
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	return read_sysreg("CNTV_CVAL_EL0");
#else
	return read_sysreg("CNTP_CVAL_EL0");
#endif
}

static inline void arch_timer_set_cntpcval(u64 value)
{
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	write_sysreg("CNTV_CVAL_EL0", value);
#else
	write_sysreg("CNTP_CVAL_EL0", value);
#endif
}

static inline u64 arch_timer_get_freq(void)
{
	return read_sysreg("CNTFRQ_EL0");
}

static inline u64 arch_timer_get_cntkctl(void)
{
	return read_sysreg("CNTKCTL_EL1");
}

static inline void arch_timer_set_cntkctl(u64 value)
{
	write_sysreg("CNTKCTL_EL1", value);
}
#endif

#endif
