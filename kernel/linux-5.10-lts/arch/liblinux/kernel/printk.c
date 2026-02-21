// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/sched/debug.h>
#include <liblinux/kapi.h>

#define LIBLINUX_LOGLEVEL_DEFAULT CONFIG_LIBLINUX_LOGLEVEL_DEFAULT

int printk_delay_msec __read_mostly;
DEFINE_RATELIMIT_STATE(printk_ratelimit_state, 5 * HZ, 10);

int console_printk[4] = {
	CONSOLE_LOGLEVEL_DEFAULT,	/* console_loglevel */
	MESSAGE_LOGLEVEL_DEFAULT,	/* default_message_loglevel */
	CONSOLE_LOGLEVEL_MIN,		/* minimum_console_loglevel */
	CONSOLE_LOGLEVEL_DEFAULT,	/* default_console_loglevel */
};

extern int printf(const char *fmt, ...);

int printk(const char *fmt, ...)
{
	int ret;
	va_list args;

	va_start(args, fmt);
	ret = vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, fmt, args);
	va_end(args);
	return ret;
}
EXPORT_SYMBOL(printk);

int printk_deferred(const char *fmt, ...)
{
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = vprintk_emit(0, LOGLEVEL_SCHED, NULL, fmt, args);
	va_end(args);
	return ret;
}
EXPORT_SYMBOL(printk_deferred);

int vprintk_emit(int facility, int level,
		 const struct dev_printk_info *dev_info,
		 const char *fmt, va_list args)
{
	char textbuf[1024];
	const char *text = textbuf;
	int text_len;
	int kern_level;

	if (level == LOGLEVEL_SCHED)
		level = LOGLEVEL_DEFAULT;

	text_len = vscnprintf(textbuf, sizeof(textbuf), fmt, args);
	if (text_len < 0 || text_len >= sizeof(textbuf)) {
		return text_len;
	}

	/* strip kernel syslog prefix and extract log level or control flags */
	if (facility == 0) {
		while ((kern_level = printk_get_level(text)) != 0) {
			switch (kern_level) {
			case '0' ... '7':
				if (level == LOGLEVEL_DEFAULT)
					level = kern_level - '0';
				break;
			case 'c':
				/*
				 * KERN_CONT use last log-lvl; The value of `LIBLINUX_LOGLEVEL_XXX`
				 * should differ from `LOGLEVEL_DEFAULT` & `LOGLEVEL_SCHED` of linux
				 */
				level = LIBLINUX_LOGLEVEL_CONT;
			}

			text_len -= 2;
			text += 2;
		}
	}

	if (level >= LIBLINUX_LOGLEVEL_DEFAULT)
		return 0;

	if (level == LOGLEVEL_DEFAULT) /* KERN_DEFAULT */
		level = LIBLINUX_LOGLEVEL_INFO;

	liblinux_pal_log_store(level, text, text_len + 1);

	return text_len;
}
EXPORT_SYMBOL(vprintk_emit);

int vprintk(const char *fmt, va_list args)
{
	return vprintk_emit(0, LOGLEVEL_DEFAULT, NULL, fmt, args);
}
EXPORT_SYMBOL(vprintk);

int __printk_ratelimit(const char *func)
{
	return 1;
}
EXPORT_SYMBOL(__printk_ratelimit);

/* for tty */
struct console *console_drivers = NULL;
EXPORT_SYMBOL_GPL(console_drivers);

void console_lock(void) {}
void console_unlock(void) {}
EXPORT_SYMBOL(console_lock);
EXPORT_SYMBOL(console_unlock);
int console_trylock(void)
{
	return 0;
}
EXPORT_SYMBOL(console_trylock);
int is_console_locked(void)
{
	return 0;
}
EXPORT_SYMBOL(is_console_locked);
atomic_t ignore_console_lock_warning __read_mostly = ATOMIC_INIT(1);
EXPORT_SYMBOL(ignore_console_lock_warning);
void __sched console_conditional_schedule(void)
{
	return;
}
EXPORT_SYMBOL(console_conditional_schedule);

/*
 * Return the console tty driver structure and its associated index
 */
struct tty_driver *console_device(int *index)
{
	struct console *c;
	struct tty_driver *driver = NULL;

	console_lock();
	for_each_console(c) {
		if (!c->device)
			continue;
		driver = c->device(c, index);
		if (driver)
			break;
	}
	console_unlock();
	return driver;
}

int console_set_on_cmdline;
EXPORT_SYMBOL(console_set_on_cmdline);
int add_preferred_console(char *name, int idx, char *options)
{
	return 0;
}

/*
 * Initialize the console device. This is called *early*, so
 * we can't necessarily depend on lots of kernel help here.
 * Just do some early initializations, and do the complex setup
 * later.
 */
void __init console_init(void)
{
	int ret;
	initcall_t call;
	initcall_entry_t *ce;

	/* Setup the default TTY line discipline. */
	n_tty_init();

	/*
	 * set up the console device so that later boot sequences can
	 * inform about problems etc..
	 */
	ce = __con_initcall_start;
	while (ce < __con_initcall_end) {
		call = initcall_from_entry(ce);
		ret = call();
		if(ret < 0)
			pr_err("%pS failed, ret = %d\n", call, ret);
		ce++;
	}
}

int oops_in_progress = 0;
EXPORT_SYMBOL(oops_in_progress);

bool kmsg_dump_get_buffer(struct kmsg_dumper *dumper, bool syslog,
			  char *buf, size_t size, size_t *len)
{
	return liblinux_get_hm_log_buffer(buf, (unsigned int)size, len);
}
EXPORT_SYMBOL(kmsg_dump_get_buffer);

LIBLINUX_KAPI_DIFF
void register_console(struct console *newcon)
{
	(void)newcon;
}
EXPORT_SYMBOL(register_console);
