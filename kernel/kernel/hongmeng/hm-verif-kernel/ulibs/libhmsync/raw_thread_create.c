/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Create/Terminate a raw_thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019
 */
#ifdef __HOST_LLT__
#include <limits.h>
#else
/* for malloc_reclaim_thread_memory */
#include <malloc_hook.h>
#endif
#include <string.h>
#include <sys/mman.h>
/* for CLONE_XXX flags */
#include <sched.h>

#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#endif

#include <hmkernel/compiler.h>
#include <libsysif/sysmgr/api.h>
#include <lib/utils.h>
#include <hmkernel/const.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libmem/utils.h>
#include <hongmeng/syscall.h>
#include <hmkernel/rq.h>
#include <hmkernel/futex.h>
#include <hmkernel/sched/priority.h>
#include <libhmactv/actv.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/capability.h>
#include <hmkernel/attributes/hmattributes.h>

#include <libhmsync/atomic.h>
#include <libhmsync/thread.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>

#ifndef __HOST_UT__
#include <clone.h>
#endif

#include "raw_tls.h"
#include "raw_thread_impl.h"
#include "raw_thread_unmapself.h"

#define USER_STACK_SPACE_SIZE (2 * 1024UL)

static const size_t __raw_thread_tsd_size = sizeof(void *) * __UL(RAW_THREAD_KEYS_MAX);

struct __libc *plibc = NULL;
/*
 * we supposed native app will use raw thread only. Don't need to consider
 * creating pthread and raw thread concurrently. So here use a mutex in raw thread should be find.
 */
static struct raw_mutex thread_mutex = RAW_MUTEX_INITIALIZER;

volatile int raw_tl_lock = 0;

struct raw_mutex *raw_thread_lock(void)
{
	return &thread_mutex;
}

/*
 * This interface is used to protect the shared stack address when a
 * detach-raw_thread exits. The lock is released after the raw_thread
 * exits. Therefore, the unlock interface is not provided.
 */
static void raw_thread_list_lock(void)
{
	struct raw_thread *self = __raw_thread_self();
	int tid = CMPT_THREAD_REG(self, tid);
	int val = raw_tl_lock;

	if (val != tid) {
		while ((val = raw_atomic_int_cas(ptr_to_type(&raw_tl_lock,
						 raw_atomic_int_t *), 0, tid)) != 0) {
			(void)lsyscall_thread_futex(ptr_to_type(&raw_tl_lock, int *),
						   __FUTEX_OP_WAIT, val, 0, NULL, 0);
		}
	}
}

#define STACK_ALIAN  (ULONG_MAX - 15UL)

#ifdef CONFIG_NEED_SHADOW_STACK
#define RAW_THREAD_TMP_SSTACK_SIZE	64
void *raw_mmap_shadow_stack(void)
{
	int err;
	bool unmapped = false;
	void *sstack_base = NULL;
	void *sstack = NULL;
	uint32_t flags = __U(MAP_PRIVATE) | __U(MAP_ANON);
#ifdef CONFIG_SFI
	flags |= MFLAG_MAP_SAFE;
#endif
	sstack_base = hm_mem_mmap(0, SHADOW_STACK_TOTAL_SIZE, PROT_NONE, flags, -1, 0);
	/* MAP_FAILED is (void *)-1 */
	if (sstack_base == MAP_FAILED) {
		sstack_base = NULL;
	}

	if (sstack_base != NULL) {
		sstack = hm_mem_mmap_fixed((void *)((uintptr_t)sstack_base + SHADOW_STACK_GUARD_SIZE),
					   SHADOW_STACK_SIZE, __U(PROT_READ) | __U(PROT_WRITE),
					   flags, -1, 0, &unmapped, NULL);
		if (sstack == MAP_FAILED) {
			hm_warn("re-map sstack failed\n");
			err = hm_mem_munmap(sstack_base, unmapped ? SHADOW_STACK_GUARD_SIZE : SHADOW_STACK_TOTAL_SIZE);
			if (err != E_HM_OK) {
				hm_warn("hm_mem_munmap failed in alloc sstack failed, err=%s\n",
					hmstrerror(err));
			}
			sstack_base = NULL;
		}
	}

	return sstack_base;
}

static void raw_munmap_shadow_stack(struct raw_thread *td)
{
	void *sstack_base = CMPT_THREAD_REG(td, sstack_base);
	int err;

	if (sstack_base != NULL) {
		err = hm_mem_munmap(sstack_base, SHADOW_STACK_TOTAL_SIZE);
		if (err != E_HM_OK) {
			hm_warn("hm_mem_munmap shadow stack failed in raw thread join, err=%s\n",
				hmstrerror(err));
		}
		CMPT_THREAD_REG(td, sstack_base) = NULL;
		CMPT_THREAD_REG(td, sstack) = NULL;
	}
}

static void init_sstack(struct raw_thread *td)
{
	uintptr_t tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE];
	void *sstack_base = NULL;

	/* When sysmgr uses the raw thread interface, mmap is a selfcall.
	 * It needs to use the sstack of the current thread. */
	tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE - 1] = 0;
	set_shadow_stack(&tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE - 1]);
	sstack_base = raw_mmap_shadow_stack();
	BUG_ON(sstack_base == NULL);

	CMPT_THREAD_REG(td, sstack_base) = sstack_base;
	CMPT_THREAD_REG(td, sstack) = (void *)shadow_stack_start_addr((uintptr_t)sstack_base);
	set_shadow_stack(CMPT_THREAD_REG(td, sstack));
	return;
}
#endif

static void *raw_thread_entry(void *args)
{
	void *exitcode = NULL;
	struct raw_thread *self = (struct raw_thread *)args;

	if (self->cref.tcb_cref == 0ULL) {
		self->cref.tcb_cref = hm_thread_self_cref();
	}

#ifdef CONFIG_NEED_SHADOW_STACK
	init_sstack(self);
#endif

	exitcode = (self->start)(self->start_arg);
	raw_thread_exit(exitcode);
	return NULL;
}

#ifdef PAC_FWCFI_SYSSERVICE
static __ro_after_init unsigned long g_pac_raw_thread_entry_typeid;

__PAC_INIT_SECTION
void raw_thread_entry_modifier_init(void)
{
	g_pac_raw_thread_entry_typeid = (unsigned long)__builtin_get_modifier_bytype(&raw_thread_entry);
}
#else
void raw_thread_entry_modifier_init(void) {}
#endif

static int do_raw_thread_create(const raw_thread_attr_t *raw_attr,
				struct raw_thread *new)
{
	int err;
	uint64_t clone_flags;
	unsigned long ctid;
	volatile unsigned long entry = ptr_to_ulong(&raw_thread_entry);

	BUG_ON(raw_attr == NULL || new == NULL);
	clone_flags = raw_attr->flags |
		      THREAD_CONFIG_FLAGS_MASK_IS_SUSPEND |
		      THREAD_CONFIG_FLAGS_MASK_CALLER_VSPACE |
		      THREAD_CONFIG_FLAGS_MASK_SERVICES_CREATE |
		      CLONE_CHILD_CLEARTID | CLONE_THREAD |
		      CLONE_VM | CLONE_SIGHAND | CLONE_SETTLS;

	/*
	 * If raw_thread_terminate interface is called to kill a thread,
	 * the target thread will not enter raw_thread_exit function. If
	 * the joinable thread is also blocked at the raw_tl_lock address,
	 * the thread that invokes the raw_thread_join interface cannot
	 * be woken up. Therefore, for joinable thread, retain the original
	 * tid address block mode. After raw_thread_terminate is deleted,
	 * change it to the raw_tl_lock address.
	 */
	if (CMPT_THREAD_REG(new, DETACH_STAT) == DT_DETACHED) {
		ctid = ptr_to_ulong(&raw_tl_lock);
	} else {
		ctid = ptr_to_ulong(CMPT_THREAD_REF(new, tid));
	}

	do {
		/*
		 * To avoid the trouble of big/little endian and arch,
		 * rpcinfo is defined as 64bits.
		 */
		unsigned long long rpcinfo;
		rpcinfo = __RPC_INFO_ENCODE(__sysmgr_method_proc_hm_clone, 0,
					    ARGS_IN_STACK);
		err = ____clone(rpcinfo, 0UL, (unsigned long long)clone_flags,
				ptr_to_ulong(CMPT_THREAD_REG(new, THREAD_STACK)), 0UL,
				ptr_to_ulong(THREAD_TO_TLS(new)), ctid, entry,
				ptr_to_ulong(new),
#ifdef PAC_FWCFI_SYSSERVICE
				g_pac_raw_thread_entry_typeid);
#else
				0);
#endif
	} while (err == E_HM_ACTIVATION_RPC_RETRY);

	if (err < 0) {
		hm_warn("____clone failed, err=%s\n", hmstrerror(err));
	}
	if (err >= 0) {
		if (new->cref.tcb_cref == 0) {
			new->cref.tcb_cref = hm_thread_tid2cref(err);
		}
		CMPT_THREAD_REG(new, tid) = err;
		err = E_HM_OK;
	}

	return err;
}

static void *alloc_tsd(void **map, void **stack, size_t size, size_t guard)
{
	void *tsd = NULL;

	if (guard != 0U) {
		*map = hm_mem_mmap_ex(NULL, size - guard,
				   __U(PROT_READ) | __U(PROT_WRITE),
				   __U(MAP_PRIVATE) | __U(MAP_ANON) |
				   __U(MFLAG_NOCOW) | __U(MFLAG_MAP_STACK) |
				   __U(MFLAG_MAP_ACTV),
				   -1, 0, guard);
		if (*map == MAP_FAILED) {
			hm_warn("re-map stack failed\n");
			__hmattr_sideff_resolved("no relative side effect");
			return NULL;
		}
		*map = (char *)(*map) - guard;
	} else {
		*map = hm_mem_mmap(NULL, size,
				   __U(PROT_READ) | __U(PROT_WRITE),
				   __U(MAP_PRIVATE) | __U(MAP_ANON) | __U(MFLAG_NOCOW) |
				   __U(MFLAG_MAP_STACK) | __U(MFLAG_MAP_ACTV),
				   -1, 0);
		if (*map == MAP_FAILED) {
			__hmattr_sideff_resolved("no relative side effect");
			return NULL;
		}
	}

	tsd = (void *)((char *)*map + size - __raw_thread_tsd_size);
	if (*stack == NULL) {
		*stack = (char *)tsd - __raw_thread_tls_size();
	}
	return tsd;
}

/* Link raw thread to global thread list */
void raw_thread_link_to_global(struct raw_thread *new, struct raw_thread *prev_thread)
{
	BUG_ON(prev_thread == NULL || new == NULL);
	raw_mutex_lock(&thread_mutex);
	__libc_thread_minus_1(plibc)++;
	CMPT_THREAD_REG(new, next) = CMPT_THREAD_REG(prev_thread, next);
	CMPT_THREAD_REG(new, prev) = RAW_THREAD_TO_PTHREAD(prev_thread);
	CMPT_THREAD_REG(new, next->prev) = RAW_THREAD_TO_PTHREAD(new);
	CMPT_THREAD_REG(new, prev->next) = RAW_THREAD_TO_PTHREAD(new);
	raw_mutex_unlock(&thread_mutex);
}

/* Remove raw thread from global thread list */
void raw_thread_remove_from_global(struct raw_thread *thread)
{
	BUG_ON(thread == NULL);
	raw_mutex_lock(&thread_mutex);
	/* check if the thread is still in global thread list */
	if ((CMPT_THREAD_REG(thread, prev) != RAW_THREAD_TO_PTHREAD(thread)) ||
	    (CMPT_THREAD_REG(thread, next) != RAW_THREAD_TO_PTHREAD(thread))) {
		__libc_thread_minus_1(plibc)--;
		CMPT_THREAD_REG(thread, next->prev) = CMPT_THREAD_REG(thread, prev);
		CMPT_THREAD_REG(thread, prev->next) = CMPT_THREAD_REG(thread, next);
		CMPT_THREAD_REG(thread, prev) = RAW_THREAD_TO_PTHREAD(thread);
		CMPT_THREAD_REG(thread, next) = RAW_THREAD_TO_PTHREAD(thread);
	}
	raw_mutex_unlock(&thread_mutex);
#ifndef __HOST_LLT__
	/* When a thread exits, if the memory management module supports the memory
	 * thread cache function, resources in the memory cache pool of the current
	 * thread need to be released. This interface is reserved for the memory
	 * management module. */
	malloc_reclaim_thread_memory(CMPT_THREAD_REG(thread, THREAD_MALLOC_TCACHE));
#endif
}

struct raw_thread_config {
	size_t guard;
	size_t map_size;
	void *tsd;
	void *map;
	void *stack;
};

static int alloc_thread_memory(const raw_thread_attr_t *attr, size_t tls_size, struct raw_thread_config *conf)
{
	int rc = E_HM_OK;

	conf->guard = (size_t)ALIGN_UP(attr->guardsize, PAGE_SIZE);
	conf->map_size = conf->guard + (size_t)ALIGN_UP((attr->stacksize + __raw_thread_tsd_size + tls_size),
						PAGE_SIZE);
	if (conf->tsd == NULL) {
		conf->tsd = alloc_tsd(&conf->map, &conf->stack, conf->map_size, conf->guard);
		if (conf->tsd == NULL) {
			rc = E_HM_NOMEM;
		}
	}
	if (rc == E_HM_OK) {
		BUG_ON(conf->stack == NULL);
	}
	return rc;
}

static int init_thread_stack_and_tls(struct raw_thread **thread,
				     raw_thread_attr_t *attr,
				     void *(*func)(void *args), void *args,
				     const raw_thread_attr_t *raw_attr)
{
	/*
	 * raw_thread memory for stack and tls layout:
	 *
	 * higher addr   ---------
	 *              |   tsd   |
	 *     /|\       ---------
	 *      |       |   tls   |
	 *      |        ---------  <-- TLS reg pointer (TLS_reg)
	 *      |       | pthread |
	 *      |        ---------  <-- pthread_self(); `TLS_reg - sizeof(struct pthread)`
	 *      |       |  raw    |
	 *      |       | thread  |
	 *      |       | specific|
	 *      |        ---------  <-- Stack pointer(SP) & raw_thread_self(); => `TLS_reg - sizeof(struct raw_thread)`
	 *      |       |         |
	 *      |       |  stack  |
	 *      |       |         |
	 *      |        ---------
	 *              |  guard  |
	 * lower addr    ---------
	 */
	int rc = E_HM_OK;
	struct raw_thread *td = NULL;
	size_t tls_size = __raw_thread_tls_size();
	struct raw_thread_config conf = {0};

	if (raw_attr != NULL) {
		*attr = *raw_attr;
	} else {
		attr->stacksize = DEFAULT_RAW_THREAD_STACKSIZE;
		attr->guardsize = DEFAULT_RAW_THREAD_GUARDSIZE;
	}

	/*
	 * To adapt new lockowner, we don't support caller specify stack. */
	if (attr->stackaddr != 0UL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = alloc_thread_memory(attr, tls_size, &conf);
	}

	if (rc == E_HM_OK) {
		/* get the address of thread descriptor */
		td = raw_tls_copy((void *)((char *)conf.tsd - tls_size));
		CMPT_THREAD_REG(td, THREAD_TSD) = (void **)conf.tsd;
		/* stack should aligned to 16 bytes in arm64 */
		CMPT_THREAD_REG(td, THREAD_STACK)     = ulong_to_ptr(ALIGN_DOWN((uintptr_t)conf.stack, 16UL), void);
		td->cref.tcb_cref                     = 0ULL;
		td->start                             = func;
		td->start_arg                         = args;
		td->stack_base                        = ALIGN_DOWN((uintptr_t)conf.stack, 16UL);
		td->stack_size                        = td->stack_base - (ptr_to_ulong(conf.map) + conf.guard);
		CMPT_THREAD_REG(td, THREAD_STACK_INFO_SIZE) = attr->stacksize;
		CMPT_THREAD_REG(td, THREAD_MAP_BASE)  = conf.map;
		CMPT_THREAD_REG(td, map_size)         = conf.map_size;
		if ((attr->mode & RAW_THREAD_DETACH_MODE) == RAW_THREAD_DETACH_MODE) {
			CMPT_THREAD_REG(td, DETACH_STAT) = DT_DETACHED;
		} else {
			CMPT_THREAD_REG(td, DETACH_STAT) = DT_JOINABLE;
		}
		CMPT_THREAD_REG(td, locale)           = __libc_global_locale(plibc);
		CMPT_THREAD_REG(td, robust_list.head) = CMPT_THREAD_REF(td, robust_list.head);
		raw_thread_lockdep_depth_init(td);
		raw_thread_rdlock_lockowner_init(td);
		*thread = td;
	}
	return rc;
}

static void destroy_stack_and_tls(const struct raw_thread *thread)
{
	int err;

	BUG_ON(thread == NULL);

	if (CMPT_THREAD_REG(thread, THREAD_MAP_BASE) != NULL) {
		err = hm_mem_munmap(CMPT_THREAD_REG(thread, THREAD_MAP_BASE),
				    CMPT_THREAD_REG(thread, map_size));
		if (err != E_HM_OK) {
			hm_warn("unmap memory failed and may result in memory leak, err=%s\n",
				hmstrerror(err));
		}
	}
}

static void delete_thread(struct raw_thread *thread)
{
	int err;

	BUG_ON(thread == NULL);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	err = raw_thread_terminate(thread);
#pragma GCC diagnostic pop
	if (err != E_HM_OK) {
		hm_warn("raw_thread_terminate failed, err=%s\n", hmstrerror(err));
	}

	/*
	 * do not futex_wait tid_address, because raw_thread_terminate may fail,
	 * and the target thread never clear tid_address.
	 */
	raw_thread_remove_from_global(thread);
	destroy_stack_and_tls(thread);
}

int raw_thread_create(raw_thread_t *thread,
		      const raw_thread_attr_t *raw_attr,
		      void *(*func)(void *args),
		      void *args)
{
	int ret = E_HM_OK;
	struct raw_thread *new = NULL;
	raw_thread_attr_t attr = {0};
	struct raw_thread *self = __raw_thread_self();
	struct sched_param param_user;

	if (func == NULL || thread == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = init_thread_stack_and_tls(&new, &attr, func, args, raw_attr);
		if (ret != E_HM_OK) {
			hm_warn("init stack and tls memory for new thread failed, err=%s\n",
				hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		ret = do_raw_thread_create(&attr, new);
		if (ret != E_HM_OK) {
			hm_warn("do_raw_thread_create failed, err=%s\n", hmstrerror(ret));
			destroy_stack_and_tls(new);
		}
	}

	if (ret == E_HM_OK) {
		raw_thread_link_to_global(new, self);
		*thread = new;

		if (raw_attr != NULL && raw_attr->inherit == RAW_THREAD_EXPLICIT_SCHED) {
			param_user.sched_priority = (int)raw_attr->prio;
			ret = hm_thread_sched_setscheduler(raw_thread_tid_of(new),
							   (int)raw_attr->policy, &param_user);
			if (ret != E_HM_OK) {
				hm_warn("new child set sched attr failed, err=%s\n", hmstrerror(ret));
				delete_thread(new);
				*thread = NULL;
			}
		}
	}
	/*
	 * NOTE: The initialization of raw_thread meta data must be finished before resume it,
	 * becasue the new thread may use its meta data once start to run.
	 * */
	if (ret == E_HM_OK) {
		if (!thread_config_flags_is_suspend(attr.flags)) {
			ret = hm_thread_resume(new->cref.tcb_cref);
			if (ret != E_HM_OK) {
				hm_warn("failed to resume new thread, err=%s\n", hmstrerror(ret));
				delete_thread(new);
				*thread = NULL;
			}
		}
	}
	return ret;
}

/*
 * This interface sends HMSIGNAL_KILL signal to kill target thread. The target
 * thread may be forcibly ended regardless of which instruction the thread is
 * running, which may cause unknown risks. Do not use this interface in the
 * future.
 */
int __deprecated raw_thread_terminate(const struct raw_thread *thread)
{
	int ret = E_HM_OK;

	if (thread == NULL || thread == raw_thread_self()) {
		ret = E_HM_INVAL;
	} else {
		ret = actvcall_hmcall_thread_kill(CMPT_THREAD_REG(thread, tid));
	}
	return ret;
}

void __noreturn raw_thread_exit(void *exitcode)
{
	int status;
	unsigned int exit_retry = 0;
	struct raw_thread *self = __raw_thread_self();

	CMPT_THREAD_REG(self, THREAD_EXITCODE) = exitcode;

	raw_thread_remove_from_global(self);
	/* run tsd destructors */
	__raw_thread_tsd_run_dtors(NULL);

#ifdef CONFIG_NEED_SHADOW_STACK
	/* When sysmgr uses the raw thread interface, mmap is a selfcall.
	 * It needs to use the sstack of the current thread. */
	uintptr_t tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE];
	tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE - 1] = 0;
	set_shadow_stack(&tmp_sstack[RAW_THREAD_TMP_SSTACK_SIZE - 1]);
	/* free shadow stack */
	raw_munmap_shadow_stack(self);
#endif

	status = raw_atomic_int_cas(ptr_to_type(CMPT_THREAD_REF(self, DETACH_STAT),
				    raw_atomic_int_t *), DT_JOINABLE, DT_EXITING);
	hm_actv_unset_local_area();
	if (status == DT_DETACHED && CMPT_THREAD_REG(self, THREAD_MAP_BASE) != NULL) {
		raw_thread_list_lock();
		raw_thread_unmapself(CMPT_THREAD_REG(self, THREAD_MAP_BASE),
				     CMPT_THREAD_REG(self, map_size));
	}

	while (1) {
		hm_thread_exit(0);
		exit_retry++;
		if (exit_retry > 5u) {
			hm_warn_ratelimited("now has retry exit for more than 5 times, retry times=%u\n", exit_retry);
		}
	}
}

int raw_thread_join(raw_thread_t thread, void **exitcode)
{
	struct raw_thread *target = (struct raw_thread *)thread;
	int tid;
	int err = E_HM_INVAL;

	if (target != NULL && (CMPT_THREAD_REG(target, DETACH_STAT) <= DT_JOINABLE)) {
		while ((tid = CMPT_THREAD_REG(target, tid)) != 0) {
			(void)lsyscall_thread_futex(CMPT_THREAD_REF(target, tid),
						    __FUTEX_OP_WAIT, tid, 0, NULL, 0);
		}
		if (exitcode != NULL) {
			*exitcode = CMPT_THREAD_REG(target, THREAD_EXITCODE);
		}
		raw_thread_remove_from_global(target);

#ifdef CONFIG_NEED_SHADOW_STACK
		raw_munmap_shadow_stack(target);
#endif

		if (CMPT_THREAD_REG(target, THREAD_MAP_BASE) != NULL) {
			err = hm_mem_munmap(CMPT_THREAD_REG(target, THREAD_MAP_BASE),
					    CMPT_THREAD_REG(target, map_size));
			if (err != E_HM_OK) {
				hm_warn("hm_mem_munmap map_base failed in raw thread join, err=%s\n",
					hmstrerror(err));
			}
		}
	}

	return err;
}

raw_thread_t raw_thread_self(void)
{
	return (raw_thread_t)__raw_thread_self();
}

cref_t raw_thread_cref_of(raw_thread_t thread)
{
	struct raw_thread *self = (struct raw_thread *)thread;

	return (self == NULL) ? 0ULL : self->cref.tcb_cref;
}

pid_t raw_thread_tid_of(raw_thread_t thread)
{
	int ret = E_HM_OK;
	struct raw_thread *self = (struct raw_thread *)thread;

	if (self == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = CMPT_THREAD_REG(self, tid);
	}
	return ret;
}

unsigned long raw_thread_stack_size_of(raw_thread_t thread)
{
	struct raw_thread *self = (struct raw_thread *)thread;

	return (self == NULL) ? 0UL : CMPT_THREAD_REG(self, THREAD_STACK_INFO_SIZE);
}

unsigned long raw_thread_stack_base_of(raw_thread_t thread)
{
	struct raw_thread *self = (struct raw_thread *)thread;

	return (self == NULL) ? 0UL : self->stack_base;
}

/* for actv raw thread */
unsigned long raw_thread_actv_stack_size_of(raw_thread_t thread)
{
	struct raw_thread *self = (struct raw_thread *)thread;

	return (self == NULL) ? 0UL : self->stack_size;
}

size_t raw_thread_tls_size(void)
{
	return __raw_thread_tls_size();
}

void *raw_thread_private_of(struct raw_thread *thread)
{
	return __raw_thread_private_of(thread);
}

void raw_thread_set_private(struct raw_thread *thread, void *priv)
{
	__raw_thread_set_private(thread, priv);
}
