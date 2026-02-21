/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Headfiles and defines inherited from udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 11:51:01 2019
 */

#ifndef TTY_UDK_WRAP_H
#define TTY_UDK_WRAP_H

#include <udk/mm.h>
#include <udk/api.h>
#include <udk/log.h>
#include <udk/irq.h>
#include <udk/sync.h>
#include <udk/poll.h>
#include <udk/kvic.h>
#include <udk/timer.h>
#include <udk/iolib.h>
#include <udk/power.h>
#include <udk/errno.h>
#include <udk/atomic.h>
#include <udk/module.h>
#include <udk/securec.h>
#include <udk/security.h>
#include <hmasm/page.h>
#include <hmasm/processor.h>

#include <udk/lib/idr.h>
#include <udk/lib/dlist.h>
#include <udk/lib/string.h>
#include <libtty/serial.h>
#include <libstrict/strict.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_thread_cond.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <devhost/device.h>

struct tty_struct;
struct tty_chrdev;

#define tty_io_read32(c)		udk_read32(c)
#define tty_io_write32(v, c)		udk_write32(c, v)

#define tty_sem_t                       udk_sem
#define tty_sem_init(sem, value)        udk_sem_init(sem, value)
#define tty_sem_post(sem)		udk_sem_post(sem)
#define tty_sem_wait(sem)		udk_sem_wait(sem)

#define tty_cond_t			udk_thread_cond
#define tty_cond_attr			udk_thread_cond_attr
#define tty_cond_init(cond, attr)	udk_thread_cond_init(cond, attr)
#define tty_cond_wait(cond, mutex)	udk_thread_cond_wait(cond, mutex)
#define tty_cond_broadcast(cond)	udk_thread_cond_broadcast(cond)

#define tty_mutex_t			udk_mutex
#define tty_mutex_init(lock)		udk_mutex_init(lock)
#define tty_mutex_lock(lock)		udk_mutex_lock(lock)
#define tty_mutex_unlock(lock)		udk_mutex_unlock(lock)

#define tty_list_node			dlist_node
#define tty_list_init(...)		dlist_init(__VA_ARGS__)
#define tty_list_delete(...)		dlist_delete(__VA_ARGS__)
#define tty_list_insert_tail(...)	dlist_insert_tail(__VA_ARGS__)
#define tty_list_for_each_entry(...)	dlist_for_each_entry(__VA_ARGS__)
#define TTY_LIST_HEAD			DLIST_HEAD

#define tty_atomic_t raw_atomic_int_t
#define tty_atomic_set(ptr, val) raw_atomic_int_set(ptr, val)
#define tty_atomic_read(ptr) raw_atomic_int_read(ptr)
#define tty_compare_exchange(ptr, old_v, new_v) \
	raw_atomic_int_cas(ptr, old_v, new_v)
#define tty_atomic_inc(ptr) raw_atomic_int_inc(ptr)
#define tty_atomic_dec(ptr) raw_atomic_int_dec(ptr)

/*
 * Since idr_for_each has not implemented in ulibs yet,
 * we use idr_get_next to go through the idr
 */
#define idr_search_forward(idr, item, id)                                      \
	for((item) = idr_get_next((idr), &(id));                               \
	    (item) != NULL;                                                    \
	    ((id)++), (item) = idr_get_next((idr), &(id)))

#define tty_info(fmt, ...)		udk_info(fmt, ##__VA_ARGS__)
#define tty_warn(fmt, ...)		udk_warn(fmt, ##__VA_ARGS__)
#define tty_debug(fmt, ...)		udk_debug(fmt, ##__VA_ARGS__)
#define tty_err(fmt, ...)		udk_error(fmt, ##__VA_ARGS__)

#define tty_modules_init(initfn) udk_module_init(initfn)
#define tty_modules_exit(exitfn) udk_module_exit(exitfn)

/* udk do not need to export symbol to other ko */
#define tty_export_symbol(...)

#define TTY_IRQ_NONE (-1)
#define TTY_IRQ_HANDLED 0
#define tty_irqreturn_t int
#define tty_free_irq(...) udk_free_irq(__VA_ARGS__)
#define tty_request_irq(...) udk_request_irq(__VA_ARGS__)

#define tty_kill_process_group(...) udk_kill_process_group(__VA_ARGS__)
#define tty_kill_process(...) udk_kill_process(__VA_ARGS__)
#define tty_kvic_prepare_irq(...) udk_kvic_prepare_irq(__VA_ARGS__)
#define tty_kvic_create_mapping(...) udk_kvic_create_mapping(__VA_ARGS__)
#define tty_shutdown_hm(...) udk_shutdown(__VA_ARGS__)
#define tty_snapshot(...) udk_snapshot(__VA_ARGS__)
#define tty_mysid(...) udk_mysid(__VA_ARGS__)
#define tty_getfgpgrp(...) udk_getfgpgrp(__VA_ARGS__)
#define tty_setfgpgrp(...) udk_setfgpgrp(__VA_ARGS__)
#define tty_current_pid(...) udk_current_pid(__VA_ARGS__)
#define tty_current_pgid(...) udk_current_pgid(__VA_ARGS__)
#define tty_getsid(...) udk_getsid(__VA_ARGS__)
#define tty_getpgid(...) udk_getpgid(__VA_ARGS__)
#define tty_kconsole_print(...) udk_kconsole_print(__VA_ARGS__)
#define tty_kconsole_enable(...) udk_kconsole_enable(__VA_ARGS__)
#define tty_kconsole_disable(...) udk_kconsole_disable(__VA_ARGS__)
#define tty_session_set_ttyinfo(...) udk_session_set_ttyinfo(__VA_ARGS__)

/* Security check APIs */
#define tty_caller_has_ability(...) udk_caller_has_ability(__VA_ARGS__)
#define tty_caller_has_capability(...) udk_caller_has_capability(__VA_ARGS__)
#define tty_caller_has_ab_and_cap(...) udk_caller_has_ab_and_cap(__VA_ARGS__)
#define TTY_ABILITY udk_ability_of_tty()
#define TTY_ABILITY_SERIAL_TOGGLE udk_ability_of_serial_toggle()
#define TTY_CAP_ADMIN udk_capability_of_admin()
#define TTY_CAP_CONFIG udk_capability_of_tty_config()

typedef struct udk_poll_wqueue tty_poll_wq_t;
void tty_poll_notify(tty_poll_wq_t *wq, unsigned long event);
void tty_poll_wq_init(tty_poll_wq_t *wq);

/* Tempapory define __user as empty */
#ifndef __user
#define __user
#endif

/*
 * These two macros(copy_from_user/copy_to_user) are already defined in the product code,
 * so add a check before defining them here.
 */
#ifndef copy_from_user
#define copy_from_user(...) udk_copy_from_user(__VA_ARGS__)
#endif

#ifndef copy_to_user
#define copy_to_user(...) udk_copy_to_user(__VA_ARGS__)
#endif

static inline void *tty_mem_zalloc(size_t size)
{
	void *addr = NULL;

	addr = udk_malloc(size);
	if (addr == NULL) {
		return NULL;
	}

	mem_zero_b(addr, size);
	return addr;
}

static inline void tty_mem_free(void *block)
{
	udk_free(block);
}

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define MS_PRE_SEC	1000UL
#define NS_PRE_MS	1000000UL
static inline void tty_msleep(unsigned int ms)
{
	struct timespec ts;
	ts.tv_sec = (time_t)(ms / MS_PRE_SEC);
	ts.tv_nsec = (long)((ms % MS_PRE_SEC) * NS_PRE_MS);
	(void)udk_nanosleep(&ts, NULL);
}

struct tty_irq_bottom_half {
	int (*func)(void *data);
	void *data;
};

int tty_irq_bottom_half_init(struct tty_irq_bottom_half *bottom,
			     int (*func)(void *data), void *data);
int tty_irq_bottom_half_schedule(struct tty_irq_bottom_half *bottom);

static inline int tty_sem_waiters(struct tty_sem_t *sem)
{
	if (sem == NULL) {
		return -EINVAL;
	}

	return (int)vatomic32_read(&sem->waiters);
}

#endif
