// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/init.h>
#include <linux/bug.h>

int panic_on_oops = CONFIG_PANIC_ON_OOPS_VALUE;
#ifdef CONFIG_KASAN
/* panic at once when kasan report */
int panic_on_warn __read_mostly = 1;
#else
int panic_on_warn __read_mostly = 0;
#endif
unsigned long panic_on_taint;
bool panic_on_taint_nousertaint = false;
static unsigned long tainted_mask =
	IS_ENABLED(CONFIG_GCC_PLUGIN_RANDSTRUCT) ? (1 << TAINT_RANDSTRUCT) : 0;
ATOMIC_NOTIFIER_HEAD(panic_notifier_list);

EXPORT_SYMBOL(panic_notifier_list);
/**
 * add_taint: add a taint flag if not already set.
 * @flag: one of the TAINT_* constants.
 * @lockdep_ok: whether lock debugging is still OK.
 *
 * If something bad has gone wrong, you'll want @lockdebug_ok = false, but for
 * some notewortht-but-not-corrupting cases, it can be set to true.
 */
void add_taint(unsigned flag, enum lockdep_ok lockdep_ok)
{
	set_bit(flag, &tainted_mask);
}
EXPORT_SYMBOL(add_taint);

/*
 * TAINT_FORCED_RMMOD could be a per-module flag but the module
 * is being removed anyway.
 */
const struct taint_flag taint_flags[TAINT_FLAGS_COUNT] = {
	[ TAINT_PROPRIETARY_MODULE ]	= { 'P', 'G', true },
	[ TAINT_FORCED_MODULE ]		= { 'F', ' ', true },
	[ TAINT_CPU_OUT_OF_SPEC ]	= { 'S', ' ', false },
	[ TAINT_FORCED_RMMOD ]		= { 'R', ' ', false },
	[ TAINT_MACHINE_CHECK ]		= { 'M', ' ', false },
	[ TAINT_BAD_PAGE ]		= { 'B', ' ', false },
	[ TAINT_USER ]			= { 'U', ' ', false },
	[ TAINT_DIE ]			= { 'D', ' ', false },
	[ TAINT_OVERRIDDEN_ACPI_TABLE ]	= { 'A', ' ', false },
	[ TAINT_WARN ]			= { 'W', ' ', false },
	[ TAINT_CRAP ]			= { 'C', ' ', true },
	[ TAINT_FIRMWARE_WORKAROUND ]	= { 'I', ' ', false },
	[ TAINT_OOT_MODULE ]		= { 'O', ' ', true },
	[ TAINT_UNSIGNED_MODULE ]	= { 'E', ' ', true },
	[ TAINT_SOFTLOCKUP ]		= { 'L', ' ', false },
	[ TAINT_LIVEPATCH ]		= { 'K', ' ', true },
	[ TAINT_AUX ]			= { 'X', ' ', true },
	[ TAINT_RANDSTRUCT ]		= { 'T', ' ', true },
};

/**
 * print_tainted - return a string to represent the kernel taint state.
 *
 * For individual taint flag meanings, see Documentation/admin-guide/sysctl/kernel.rst
 *
 * The string is overwritten by the next call to print_tainted(),
 * but is always NULL terminated.
 */
const char *print_tainted(void)
{
	static char buf[TAINT_FLAGS_COUNT + sizeof("Tainted: ")];

	BUILD_BUG_ON(ARRAY_SIZE(taint_flags) != TAINT_FLAGS_COUNT);

	if (tainted_mask) {
		char *s;
		int i;

		s = buf + sprintf(buf, "Tainted: ");
		for (i = 0; i < TAINT_FLAGS_COUNT; i++) {
			const struct taint_flag *t = &taint_flags[i];
			*s++ = test_bit(i, &tainted_mask) ?
					t->c_true : t->c_false;
		}
		*s = 0;
	} else
		snprintf(buf, sizeof(buf), "Not tainted");

	return buf;
}

int test_taint(unsigned flag)
{
	return test_bit(flag, &tainted_mask);
}
EXPORT_SYMBOL(test_taint);

unsigned long get_taint(void)
{
	return tainted_mask;
}

void panic(const char *fmt, ...)
{
	static char buf[1024];
	va_list args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	pr_emerg("liblinux kernel panic - not syncing: %s\n", buf);
	atomic_notifier_call_chain(&panic_notifier_list, 0, buf);
	asm volatile ("brk 0xdead\n");
	__builtin_trap();
	__builtin_unreachable();
}
EXPORT_SYMBOL(panic);

#ifdef CONFIG_STACKPROTECTOR
__visible void __stack_chk_fail(void)
{
	panic("stack-protector: Kernel stack is corrupted in: %pB",
		__builtin_return_address(0));
}
EXPORT_SYMBOL(__stack_chk_fail);
#endif

#ifndef __WARN_FLAGS
void warn_slowpath_fmt(const char *file, int line, unsigned taint,
		       const char *fmt, ...)
{
	va_list args;
	void *caller = __builtin_return_address(0);

	pr_warn(CUT_HERE);

	if (file)
		pr_warn("WARNING: CPU: %d PID: %d at %s:%d %pS\n",
			raw_smp_processor_id(), current->pid, file, line,
			caller);
	else
		pr_warn("WARNING: CPU: %d PID: %d at %pS\n",
			raw_smp_processor_id(), current->pid, caller);

	if (fmt) {
		va_start(args, fmt);
		vprintk(fmt, args);
		va_end(args);
	}

	dump_stack();
}
EXPORT_SYMBOL(warn_slowpath_fmt);
#else
void __warn_printk(const char *fmt, ...)
{
	va_list args;

	pr_warn(CUT_HERE);

	va_start(args, fmt);
	vprintk(fmt, args);
	va_end(args);
}
EXPORT_SYMBOL(__warn_printk);
#endif
