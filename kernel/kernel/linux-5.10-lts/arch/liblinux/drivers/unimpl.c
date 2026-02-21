// SPDX-License-Identifier: GPL-2.0-only
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <liblinux/kapi.h>
#include <liblinux/unimpl.h>

/* drivers/cpufreq/cpufreq.c */
LIBLINUX_GPL_EXPORTED_UNIMPL(cpufreq_cpu_put);
LIBLINUX_UNIMPL(cpufreq_suspend);
LIBLINUX_UNIMPL(cpufreq_resume);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(cpufreq_table_index_unsorted, int, 0);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(cpufreq_cpu_get, struct cpufreq_policy *, NULL);
LIBLINUX_EXPORTED_UNIMPL_RET(cpufreq_register_notifier, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(cpufreq_unregister_notifier, int, 0);

/* kernel/linux-5.10/platform_source/basicplatform/drivers/watchdog/wdt_v500.c  */
LIBLINUX_UNIMPL(wdt_fast_reset);
#if defined(CONFIG_DFX_SP805_WATCHDOG) || defined(CONFIG_WATCHDOG_V500)|| defined(CONFIG_MNTN_VWATCHDOG)
LIBLINUX_UNIMPL(wdt_register_notify)
#endif
/* /kernel/linux-5.10/platform_source/see/drivers/prmem/prmem.c */
LIBLINUX_EXPORTED_UNIMPL_RET(__pfree, int, 0);
LIBLINUX_UNIMPL(rdr_archtime_register_print);
LIBLINUX_EXPORTED_UNIMPL_RET(pstrdup, char *, NULL);
LIBLINUX_EXPORTED_UNIMPL(prmem_protect_pool);
LIBLINUX_UNIMPL_RET(prmem_active, bool, false);
LIBLINUX_EXPORTED_UNIMPL_RET(pcalloc, void *, NULL);
/* drivers/huawei_platform/usb/pd/rt1715/tcpci_alert.c */
LIBLINUX_UNIMPL(__popcountdi2);

/* drivers/hwhungtask/hungtask_base.o */
LIBLINUX_EXPORTED_UNIMPL(hungtask_show_state_filter);

/* drivers/tty/sysrq.c */
LIBLINUX_EXPORTED_UNIMPL(handle_sysrq)
LIBLINUX_EXPORTED_UNIMPL_RET(unregister_sysrq_key, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(sysrq_mask, int, 0)

/* drivers/tty/tty_audit.c */
LIBLINUX_UNIMPL(tty_audit_add_data)
LIBLINUX_UNIMPL(tty_audit_tiocsti)
LIBLINUX_UNIMPL_RET(tty_audit_push, int, 0)

/* drivers/devkit/audiokit/headset_debug/headset_debug.c */
LIBLINUX_UNIMPL(headset_debug_init)
LIBLINUX_GPL_EXPORTED_UNIMPL(headset_debug_set_state)
LIBLINUX_GPL_EXPORTED_UNIMPL(headset_debug_uninit)

#ifdef CONFIG_DFX_KERNELDUMP
LIBLINUX_KAPI_DIFF
int add_extra_table(u64 pa, u64 size)
{
	return 0;
}
#endif

#ifdef CONFIG_MEMORY_HOTPLUG_SPARSE
LIBLINUX_KAPI_DIFF
int register_memory_notifier(struct notifier_block *nb)
{
	return 0;
}
#endif
