/*
 * suspend_hm.c
 *
 * suspend on hm kernel
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
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

#include "sr_mntn.h"
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <liblinux/pal.h>

#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <lpm_kernel_map.h>
#else
#include<soc_sctrl_interface.h>
#include<soc_acpu_baseaddr_interface.h>
#endif

#include <linux/clk.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>

#if defined(CONFIG_SYS_PM_ULSR)
#include <pm_common.h>
#endif
#if defined (CONFIG_PLATDRV_SUPPORT_FFA)
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#endif

#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) "lowpm-host: " fmt

static void __iomem *g_ap_suspend_flag_addr = NULL;
#define REG_ADDR_AP_SUSPEND_FLAG SOC_SCTRL_SCBAKDATA8_ADDR(SOC_ACPU_SCTRL_BASE_ADDR)
#define REG_SIZE 4
#define AP_SUSPEND_BIT 16
#define GUEST_SUSPEND_BIT 31

#ifdef CONFIG_SYS_PM_ULSR
static void __iomem *g_is_ulsr_addr = NULL;
#define SOC_SCTRL_IS_ULSR_ADDR SOC_SCTRL_SC_LP_FLAG_ADDR(SOC_ACPU_SCTRL_BASE_ADDR)
bool is_ulsr(void)
{
	return !!(readl(g_is_ulsr_addr) & SC_LP_FLAG_ULSR_MASK);
}
#endif

#if defined (CONFIG_SR_TRACE_POINT_SEC)
static void trace_point(unsigned int point)
{
	int ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = (unsigned long)FID_BL31_SET_SR_POSITION;
	args.data1 = (unsigned long)point;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("ffa send msg fail: %d", ret);
}
#elif defined(CONFIG_SR_TRACE_POINT)
static void __iomem *g_ap_sr_trace_reg_addr = NULL;
#define SOC_SCTRL_SR_TRACE_ADDR SR_TRACE_REG(SOC_ACPU_SCTRL_BASE_ADDR)
static void trace_point(unsigned int point)
{
	writel(point, g_ap_sr_trace_reg_addr);
}
#else
static inline void trace_point(unsigned int point)
{
	(void)point;
}
#endif

static void set_ap_suspend_flag(void)
{
	unsigned int val;
#ifdef CONFIG_PRODUCT_CDC_ACE
	val = readl(g_ap_suspend_flag_addr) | (unsigned int)BIT(GUEST_SUSPEND_BIT);
#else
	val = readl(g_ap_suspend_flag_addr) | (unsigned int)BIT(AP_SUSPEND_BIT);
#endif
	writel(val, g_ap_suspend_flag_addr);
}

static void clear_ap_suspend_flag(void)
{
	unsigned int val;
#ifdef CONFIG_PRODUCT_CDC_ACE
	val = readl(g_ap_suspend_flag_addr) & (~(unsigned int)BIT(GUEST_SUSPEND_BIT));
#else
	val = readl(g_ap_suspend_flag_addr) & (~(unsigned int)BIT(AP_SUSPEND_BIT));
#endif
	writel(val, g_ap_suspend_flag_addr);
}

static int pm_suspend_core_suspend(void)
{
	pr_info("%s ++", __func__);
#if defined(CONFIG_SYS_PM_ULSR)
	if (is_ulsr())
		pr_info("%s ++ulsr", __func__);
#endif
	lowpm_sr_mntn_suspend();

	/* host : clock & regulator suspend check */
	get_ip_regulator_state();
	pmclk_monitor_enable();

	set_ap_suspend_flag();
	trace_point(1500);
	pr_info("%s --", __func__);
	return 0;
}

static void pm_suspend_core_resume(void)
{
	pr_info("%s ++", __func__);
	clear_ap_suspend_flag();

	if (liblinux_is_suspend_success()) {
		lowpm_sr_mntn_resume();
	}

	trace_point(7500);
	pr_info("%s --", __func__);
	return;
}

static struct liblinux_suspend_core_ops g_suspend_syscore_ops = {
	.suspend = pm_suspend_core_suspend,
	.resume = pm_suspend_core_resume,
};

static bool check_guest_suspended(void)
{
	return (readl(g_ap_suspend_flag_addr) & BIT(GUEST_SUSPEND_BIT)) == BIT(GUEST_SUSPEND_BIT);
}

static int pm_suspend_begin(suspend_state_t state)
{
	pr_info("%s host will suspend %d", __func__, state);

	if (!check_guest_suspended()) {
		pr_crit("%s guest is not suspend, refuse host suspend", __func__);
		return -EBUSY;
	}
	return 0;
}

static const struct platform_suspend_ops g_suspend_ops = {
	.begin = pm_suspend_begin,
};

static __init int pm_init(void)
{
	g_ap_suspend_flag_addr = ioremap(REG_ADDR_AP_SUSPEND_FLAG, REG_SIZE);
	if (g_ap_suspend_flag_addr == NULL) {
		pr_err("%s map AP suspend flag reg failed", __func__);
		return -ENODATA;
	}

#ifdef CONFIG_SR_TRACE_POINT
#ifndef CONFIG_SR_TRACE_POINT_SEC
	g_ap_sr_trace_reg_addr = ioremap(SOC_SCTRL_SR_TRACE_ADDR, REG_SIZE);
	if (g_ap_sr_trace_reg_addr == NULL) {
		pr_err("%s map AP sr trace reg failed", __func__);
		return -ENODATA;
	}
#endif
#endif

#ifdef CONFIG_SYS_PM_ULSR
	g_is_ulsr_addr = ioremap(SOC_SCTRL_IS_ULSR_ADDR, REG_SIZE);
	if (g_is_ulsr_addr == NULL) {
		pr_err("%s map AP is ulsr reg failed", __func__);
		return -ENODATA;
	}
#endif

	liblinux_suspend_core_ops_register(&g_suspend_syscore_ops);
	suspend_set_ops(&g_suspend_ops);
	pr_info("%s success", __func__);
	return 0;
}

arch_initcall(pm_init);
