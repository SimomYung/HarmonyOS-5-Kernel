/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Headfiles and defines inherited from udk
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 25 18:42:56 2019
 */

#ifndef TTY_LIBLINUX_WRAP_H
#define TTY_LIBLINUX_WRAP_H

#include <linux/io.h>
#include <linux/idr.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/serial.h>
#include <linux/signal.h>
#include <linux/atomic.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#include <asm/ioctls.h>
#include <uapi/asm-generic/termios.h>
#include <uapi/asm-generic/termbits.h>
#include <linux/securec.h>
#include <devhost/pm.h>
#include <devhost/backend.h>
#include <devhost/security.h>
#include <devhost/interrupt.h>

struct tty_struct;
struct tty_chrdev;

#define UNUSED(x) ((void)(x))

#define tty_sem_t                       liblinux_pal_sem_t
#define tty_sem_init(sem, value)	liblinux_pal_sem_init(sem, value)
#define tty_sem_post(sem)		liblinux_pal_sem_post(sem)
#define tty_sem_wait(sem)		liblinux_pal_sem_wait(sem)

/*
 * Because COND_INTERRUPT_FLAG is hongmeng specific properties, liblinux does
 * not have this concept. To implement TTY compatible with both udk and
 * liblinux, the same definition is implemented here as in ulibs, as a
 * temporary solution.
 */
#define COND_INTERRUPT_FLAG		0x40000000
#define tty_cond_t			liblinux_pal_cond_t
#define tty_cond_attr			liblinux_pal_cond_attr_t
#define tty_cond_init(cond, attr)	liblinux_pal_cond_init(cond, attr)
#define tty_cond_wait(cond, mutex)	liblinux_pal_cond_wait(cond, mutex)
#define tty_cond_broadcast(cond)	liblinux_pal_cond_broadcast(cond)

#define tty_mutex_t		liblinux_pal_mutex_t
#define tty_mutex_init(lock)	liblinux_pal_mutex_init(lock)
#define tty_mutex_lock(lock)	liblinux_pal_mutex_lock(lock)
#define tty_mutex_unlock(lock)	liblinux_pal_mutex_unlock(lock)

#define tty_list_node			list_head
#define tty_list_init			INIT_LIST_HEAD
#define tty_list_delete(...)		list_del(__VA_ARGS__)
#define tty_list_insert_tail(n, h)	list_add_tail(h, n)
#define tty_list_for_each_entry(pos, head, type, member)                       \
	list_for_each_entry(pos, head, member)
#define TTY_LIST_HEAD			LIST_HEAD

#define tty_atomic_t vatomic32_t
#define tty_atomic_set(ptr, val) atomic_set(ptr, val)
#define tty_atomic_read(ptr) vatomic32_read(ptr)
#define tty_atomic_inc(ptr) vatomic32_inc_get(ptr)
#define tty_atomic_dec(ptr) vatomic32_dec_get(ptr)

#define tty_compare_exchange(ptr, old_v, new_v) \
	vatomic32_cmpxchg(ptr, old_v, new_v)

/* Fitting liblinux idr to udk */
#define idr_get_next(idp, nextid) idr_get_next(idp, (unsigned int *)(nextid))
#define idr_remove(idp, id) idr_remove(idp, (int)(id))
#define idr_alloc(idr, ptr) idr_alloc(idr, ptr, 0, INT_MAX, GFP_KERNEL)
/* To fit udk, we use idr_get_next to implemnt idr_for_each_forward */
#define idr_search_forward(idr, item, id)                                      \
	for((item) = idr_get_next((idr), &(id));                               \
	    (item) != NULL;                                                    \
	    ((id)++), (item) = idr_get_next((idr), &(id)))

#define tty_info(fmt, ...)	pr_info("[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define tty_warn(fmt, ...)	pr_warn("[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
/* in liblinux, pr_debug will not be printed */
#define tty_debug(fmt, ...)	pr_debug("[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define tty_err(fmt, ...)	pr_err("[%s:%d] " fmt, __func__, __LINE__, ##__VA_ARGS__)

#define tty_kill_process_group(...) devhost_kill_process_group(__VA_ARGS__)
#define tty_kill_process(...) devhost_kill_process(__VA_ARGS__)
#define tty_kvic_prepare_irq(...) devhost_kvic_prepare_irq(__VA_ARGS__)
#define tty_kvic_create_mapping(...) devhost_kvic_create_mapping(__VA_ARGS__)
#define tty_shutdown_hm(...) devhost_shutdown(__VA_ARGS__)
#define tty_snapshot(...) devhost_snapshot(__VA_ARGS__)
#define tty_mysid(...) devhost_mysid(__VA_ARGS__)
#define tty_getfgpgrp(...) devhost_getfgpgrp(__VA_ARGS__)
#define tty_setfgpgrp(...) devhost_setfgpgrp(__VA_ARGS__)
#define tty_current_pid(...) devhost_current_pid(__VA_ARGS__)
#define tty_current_pgid(...) devhost_current_pgid(__VA_ARGS__)
#define tty_getsid(...) devhost_getsid(__VA_ARGS__)
#define tty_getpgid(...) devhost_getpgid(__VA_ARGS__)
#define tty_kconsole_print(...) devhost_kconsole_print(__VA_ARGS__)
#define tty_kconsole_enable(...) devhost_kconsole_enable(__VA_ARGS__)
#define tty_kconsole_disable(...) devhost_kconsole_disable(__VA_ARGS__)
#define tty_session_set_ttyinfo(...) devhost_session_set_ttyinfo(__VA_ARGS__)

/* Security check APIs */
#define tty_caller_has_ability(...) devhost_caller_has_ability(__VA_ARGS__)
#define tty_caller_has_capability(...) devhost_caller_has_capability(__VA_ARGS__)
#define tty_caller_has_ab_and_cap(...) devhost_caller_has_ab_and_cap(__VA_ARGS__)
#define TTY_ABILITY ((int)DH_ABILITY_TTY)
#define TTY_ABILITY_SERIAL_TOGGLE ((int)DH_ABILITY_SERIAL_TOGGLE)
#define TTY_CAP_ADMIN ((int)DH_CAP_ADMIN)
#define TTY_CAP_CONFIG ((int)DH_CAP_TTY_CONFIG)

#define tty_export_symbol(...) EXPORT_SYMBOL(__VA_ARGS__)

#define tty_io_read32(c) readl(c)
#define tty_io_write32(v, c) writel(v, c)
/* define ptr_add for liblinux */
#define ptr_add(p, o) ((p) + (o))

#define TTY_IRQ_NONE IRQ_NONE
#define TTY_IRQ_HANDLED IRQ_HANDLED
#define tty_irqreturn_t irqreturn_t
#define tty_request_irq(...) request_irq(__VA_ARGS__)
/*
 * Since the interrupt request by tty backend is not shared irq,
 * just set the argument dev_id as NULL when free_irq
 */
#define tty_free_irq(...) free_irq(__VA_ARGS__, NULL)

extern char *strerror(int e);
/*
 * Since liblinux will not be able to include libhwsecure.h,
 * we copy define of mem_zero here
 */
static inline void __bzero(void *ptr, size_t len)
{
	int ___ret;

	___ret = memset_s(ptr, len, '\0', len);
	BUG_ON(___ret != 0);
}

#define mem_zero_s(data)	__bzero(&(data), sizeof(data))
#define mem_zero_a(array)	__bzero((void*)(&(array[0])), sizeof(array))
#define mem_zero_b(mem, len)	__bzero((void *)(mem), (len))

static inline void *tty_mem_zalloc(size_t size)
{
	if (unlikely(size == 0)) {
		return NULL;
	}
	return kzalloc(size, GFP_KERNEL);
}

static inline void tty_mem_free(const void *block)
{
	kfree(block);
}

#define tty_poll_wq_t wait_queue_head_t
void tty_poll_notify(tty_poll_wq_t *wq, unsigned long event);
void tty_poll_wq_init(tty_poll_wq_t *wq);

#define tty_msleep(ms) msleep(ms)

struct tty_irq_bottom_half {
	struct tasklet_struct tasklet;
	int (*func)(void *data);
	void *data;
};

int tty_irq_bottom_half_init(struct tty_irq_bottom_half *bottom,
			     int (*func)(void *data), void *data);
int tty_irq_bottom_half_schedule(struct tty_irq_bottom_half *bottom);

int tty_sem_waiters(struct tty_sem_t *sem);

#endif
