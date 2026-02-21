#include <linux/types.h>
#include <linux/printk.h>
#include <linux/capability.h>
#include <linux/sched.h>
#include <liblinux/kallsyms.h>

static inline int kallsyms_for_perf(void)
{
#ifdef CONFIG_PERF_EVENTS
	extern int sysctl_perf_event_paranoid;
	if (sysctl_perf_event_paranoid <= 1)
		return 1;
#endif
	return 0;
}

/**
 * This function can only substitute kallsyms_show_value when
 * the cred passed to kallsyms_show_value is current's cred.
 * liblinux_current_show_syms_value will check CAP_SYSLOG
 * using current's cred directly.
 */
bool liblinux_current_show_syms_value(void)
{
	switch (kptr_restrict) {
	case 0:
		if (kallsyms_for_perf())
			return true;
		fallthrough;
	case 1:
		if (has_capability_noaudit(current, CAP_SYSLOG))
			return true;
		fallthrough;
	default:
		return false;
	}
}
