/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The api of the actv pure
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 09 16:11:18 2020
 */
#include <libhmactv/actv.h>

#include <hmkernel/trigger.h>
#include <hmkernel/capability.h>
#include <hmkernel/sched/sched.h>

#include <sys/mman.h>
#include <lib/utils.h>

#include <hmasm/actv_rpc.h>
#include <libmem/utils.h>
#include <libmem/flags.h>

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/macro.h>
#include <hongmeng/syscall.h>

#include <libhmlog/hmlog.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libhmactv/actv_id.h>
#include <libsched/priority.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsync/raw_workqueue.h>
/* for raw thread implement */
#include "actv_threadpool.h"
#include "../libhmsync/raw_tls.h"
#include "../libhmsync/raw_thread_impl.h"
#include "actv_threadpool.h"

#define MAX_ASD_SIZE (unsigned long)RAW_THREAD_KEYS_MAX

#define ACTV_PURE_TRIG_WORKQUEUE_THREAD 1u
#define ACTV_PURE_TRIG_WORKQUEUE_NR_QUEUES 1u
#define ACTV_PURE_TRIG_WORK_PRIORITY  0u

unsigned long
actv_pure_to_actv_local(unsigned long actv_pure, const struct actv_pure_attr *attr)
{
	unsigned long resv_sz = 0UL;
	unsigned char *tsd = NULL;
	raw_thread_t thread = NULL;
	unsigned long aligned_thread;

	BUG_ON(attr == NULL);

	/* size for recv buf */
	resv_sz += ALIGN_UP(attr->recvbuf_size, RAW_THREAD_STACK_ALIGN);

	/* size for tsd */
	resv_sz += ALIGN_UP(attr->tsd_size * sizeof(void *), RAW_THREAD_STACK_ALIGN);

	/* size for tls and raw_thread */
	tsd = ulong_to_ptr((actv_pure - resv_sz), unsigned char);
	thread = raw_tls_to_thread(tsd - __raw_thread_tls_size());
	aligned_thread = ALIGN_DOWN(ptr_to_ulong(thread), RAW_THREAD_STACK_ALIGN);
	resv_sz += (ptr_to_ulong(tsd) - aligned_thread);

	/* size for ret buf */
	resv_sz += ALIGN_UP(attr->actvret_size, RAW_THREAD_STACK_ALIGN);

	/* size for arch_actv_local */
	resv_sz += ALIGN_UP(sizeof(struct arch_actv_local), RAW_THREAD_STACK_ALIGN);

	return (actv_pure - resv_sz);
}

void *
actv_pure_to_recv_buf(unsigned long actv_pure, const struct actv_pure_attr *attr)
{
	unsigned long resv_recv_buf_sz;

	BUG_ON(attr == NULL);
	/* size for recv buf */
	resv_recv_buf_sz = ALIGN_UP(attr->recvbuf_size, RAW_THREAD_STACK_ALIGN);
	return ulong_to_ptr((actv_pure - resv_recv_buf_sz), void);
}

void *
actv_pure_to_mem_buf(unsigned long actv_pure, const struct actv_pure_attr *attr)
{
	BUG_ON(attr == NULL);
	return ulong_to_ptr((ALIGN_UP(actv_pure, PAGE_SIZE) - attr->membuf_size), void);
}

unsigned long
mem_buf_to_actv_pure(const void *mem_buf, const struct actv_pure_attr *attr)
{
	unsigned long actv_pure;
	unsigned long resv_actv_pure_sz;

	BUG_ON(attr == NULL);
	resv_actv_pure_sz = ALIGN_UP(sizeof(struct actv_pure), RAW_THREAD_STACK_ALIGN);
	actv_pure = ptr_to_ulong(mem_buf) + attr->membuf_size - resv_actv_pure_sz;
	return actv_pure;
}

static bool actv_pure_check_parameters(struct actv_pure_attr *attr)
{
	unsigned long need_buf_size;

	if (attr == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return false;
	}

	/* recvbuf_size should be aligned to RAW_THREAD_STACK_ALIGN */
	if ((attr->recvbuf_size > __RPC_BUFSZ_BUFSZ_MAXLEN) ||
	    (!IS_ALIGNED(attr->recvbuf_size, RAW_THREAD_STACK_ALIGN))) {
		__hmattr_sideff_resolved("no relative side effect");
		return false;
	}

	need_buf_size = ALIGN_UP(sizeof(struct actv_pure), RAW_THREAD_STACK_ALIGN) +
			attr->recvbuf_size + attr->stack_size + (size_t)PAGE_SIZE;
	/* membuf_size should be aligned to PAGE_SIZE */
	if ((attr->membuf_size < need_buf_size) ||
	    (!IS_ALIGNED(attr->membuf_size, PAGE_SIZE))) {
		__hmattr_sideff_resolved("no relative side effect");
		return false;
	}

	/* XXX: tsd is used for himalloc, sysmgr don't need . */
	if (hmsrv_sysmgr_cnode_idx() != hm_ucap_self_cnode_idx()) {
		attr->tsd_size = MAX_ASD_SIZE;
	}

	return true;
}

static int __actv_pure_create(struct actv_pure_attr *attr, rpc_entry_t entry)
{
	int err;
	cref_t actv_cref;

	/*
	 * (1) Create actv kobj.
	 * (2) Do not set actv kobj res here, it will be set after alloc membuf.
	 */
	actv_cref = hm_ucap_new_actv_pure(attr->actv_pool,
					  attr->actv_kobj_pool,
					  NULL);
	if (IS_REF_ERR(actv_cref)) {
		__hmattr_sideff_resolved("no relative side effect");
		return REF_TO_ERR(actv_cref);
	}

#ifdef CONFIG_PAC
	err = hm_actv_pac_update(actv_cref);
	if (err != E_HM_OK) {
		hm_error("Activation pac copy failed: %s\n", hmstrerror(err));
		(void)hm_ucap_delete_actv_pure(actv_cref, attr->actv_kobj_pool);
		return err;
	}
#endif

	/* init the actv to move it to ready state */
	err = syscap_ActivationInit(actv_cref, ulong_to_ptr(ptr_to_ulong(entry), void),
				    ptr_to_ulong(attr));
	if (err == E_HM_ACTIVATION_RPC_RETRY) {
		/*
		 * When entry is arch_actv_pure_pre_init_rpc_entry,
		 * ActivationInit will finish the self init and return
		 * E_HM_ACTIVATION_RPC_RETRY. This actv is transfered to
		 * READY state and can provide service.
		 */
		err = E_HM_OK;
	}
	if (err != E_HM_OK) {
		hm_error("Activation init failed: %s\n", hmstrerror(err));
		(void)hm_ucap_delete_actv_pure(actv_cref, attr->actv_kobj_pool);
	}

	return err;
}

static void __actv_pure_attr_fill(struct actv_pure_attr *attr, const void *mem_buf, void *sstack)
{
	unsigned long actv_pure;
	unsigned long actv_local;

	/* compute actv_pure */
	actv_pure = mem_buf_to_actv_pure(mem_buf, attr);
	/* compute actv_local */
	actv_local = actv_pure_to_actv_local(actv_pure, attr);

	attr->actv_pure = actv_pure;
	attr->actv_local = actv_local;

#ifdef CONFIG_NEED_SHADOW_STACK
	attr->sstack_base = sstack;
	attr->sstack = (void *)shadow_stack_start_addr((uintptr_t)attr->sstack_base);
#else
	UNUSED(sstack);
#endif
}

static void __actv_pure_attr_clear(struct actv_pure_attr *attr)
{
	attr->actv_pure = 0UL;
	attr->actv_local = 0UL;
#ifdef CONFIG_NEED_SHADOW_STACK
	attr->sstack_base = NULL;
	attr->sstack = NULL;
#endif
}

/*
 *  deployment of activation mem buf:
 *
 *  High Addr   +------------+ <------------------------------+
 *              | actv_pure  |                      ^         ^
 *              +------------+                      |         |
 *              |  recv buf  |                      |         |
 *              +------------+ <-------------+      |         |
 *              |    tsd     |               ^      |         |
 *              +------------+               |      |         |
 *              |    TLS     |               |      |         |
 *  TLS REG --> +------------+               |  Reserved Buf  |
 *              |   pthread  |               |      |         |
 *              +------------+               |      |         |
 *              | raw thread |               |      |   Activation Mem Buf
 *              |  specific  |               |      |         |
 *              +------------+               |      |         |
 *              |   retbuf   | Activation Stack Buf |         |
 *              +------------+               |      |         |
 *              | actv_local |               |      V         |
 *              +------------+ <--------------------+         |
 *              |  128 bytes |               |                |
 *  SP REG ---> +------------+               |                |
 *              |    stack   |               |                |
 *              +------------+               |                |
 *              |    guard   |               V                V
 *  Low Addr    +------------+ <-------------+----------------+
 *
 * This api only create actv kobj and pass parameters to kernel.
 * The actv stack allocation and initialization will be done with the help of
 * assist stack when it is pushed by client for the first time.
 */
int hm_actv_pure_create_ex(struct actv_pure_attr *attr, rpc_entry_t entry)
{
	int err = E_HM_OK;

	if (!actv_pure_check_parameters(attr)) {
		hm_error("actv pure attr is invalid\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		/* create and init actv kobj */
		err = __actv_pure_create(attr, entry);
	}

	return err;
}

static void actv_pure_free_membuf(void *membuf, const struct actv_pure_attr *attr)
{
	int err;

	err = hm_mem_munmap(membuf, attr->membuf_size);
	if (err < 0) {
		hm_warn("unmap membuf failed, err=%s\n", hmstrerror(err));
	}
}

#ifdef CONFIG_NEED_SHADOW_STACK
static void actv_pure_free_sstack(void *sstack, const struct actv_pure_attr *attr)
{
	int err;

	if (attr->mem_free != NULL) {
		attr->mem_free(sstack, SHADOW_STACK_TOTAL_SIZE);
	} else {
		err = hm_mem_munmap(sstack, SHADOW_STACK_TOTAL_SIZE);
		if (err < 0) {
			hm_warn("unmap sstack failed, err=%s\n", hmstrerror(err));
		}
	}
}
#endif

static int __handle_actv_pure_mem_free(struct raw_work *work)
{
	const struct actv_pure_attr *attr = NULL;
	struct free_slots_work *fwork =
		container_of(work, struct free_slots_work, work);

	BUG_ON(fwork == NULL);
	attr = fwork->attr;
	if (attr != NULL) {
#ifdef CONFIG_NEED_SHADOW_STACK
		/*
		 * free the sstack before free mem buf,
		 * since the sstack is stored in mem buf.
		 */
		actv_pure_free_sstack(fwork->sstack, attr);
#endif
		/* work destroy should before free mem buf */
		raw_work_destroy(work);
		actv_pure_free_membuf(fwork->mem_buf, attr);
	} else {
		hm_warn("attr in fwork is NULL\n");
	}
	return E_HM_OK;
}

static void
__add_free_slots_work(const struct actv_pure_attr *attr,
		      unsigned long actv_pure, void *sstack)
{
	int err;
	struct free_slots_work *fwork = NULL;
	void *mem_buf = actv_pure_to_mem_buf(actv_pure, attr);
	struct actv_pure *p_actv_pure = ulong_to_ptr(actv_pure, struct actv_pure);

	BUG_ON(p_actv_pure == NULL);
	fwork = &p_actv_pure->fwork;
	BUG_ON(fwork == NULL);

	fwork->attr = attr;
	fwork->mem_buf = mem_buf;
	fwork->sstack = sstack;
	err = raw_work_init(&fwork->work, __handle_actv_pure_mem_free,
			    ACTV_PURE_TRIG_WORK_PRIORITY);
	BUG_ON(err != E_HM_OK, "raw_work_init failed, err=%s\n", hmstrerror(err));

	err = raw_workqueue_add_work(attr->trig_wq, &fwork->work);
	BUG_ON(err != E_HM_OK, "raw_work_init failed, err=%s\n", hmstrerror(err));
}

static void
__actv_pure_mem_free_inited(unsigned long actv_pure, void *sstack,
			    const struct actv_pure_attr *attr)
{
	if (attr->trig_wq != NULL) {
		/* use trigger worker thread to free memm area slots */
		__add_free_slots_work(attr, actv_pure, sstack);
	} else {
		void *mem_buf = actv_pure_to_mem_buf(actv_pure, attr);
		/* free the mem buf */
		actv_pure_free_membuf(mem_buf, attr);
#ifdef CONFIG_NEED_SHADOW_STACK
		/* free the sstack */
		actv_pure_free_sstack(sstack, attr);
#else
		UNUSED(sstack);
#endif
	}
}

static int __actv_pure_thread_destroy(unsigned long actv_pure)
{
	int err = E_HM_OK;
	struct raw_thread *thread = NULL;
	struct actv_pure *actv = ulong_to_ptr(actv_pure, struct actv_pure);

	if (actv == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}
	thread = actv->thread;

	/* remove from threadpool and free actv id */
	if (CMPT_THREAD_REG(thread, tid) != 0) {
		actv_threadpool_remove(thread);
		err = actv_pure_id_free((unsigned int)CMPT_THREAD_REG(thread, tid));
		BUG_ON(err < 0, "actv pure id %u exceed range\n", (unsigned int)CMPT_THREAD_REG(thread, tid));
	}

	__actv_destroy_thread(thread);
	return err;
}

/* caller should guarantee the actv is not in use */
int hm_actv_pure_destroy(cref_t actv_cref, unsigned long actv_pure,
			 struct actv_pure_attr *attr, bool is_inited)
{
	int err;
	void *sstack = NULL;

	if (attr == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

#ifdef CONFIG_NEED_SHADOW_STACK
	sstack = hm_ucap_actv_pure_sstack_of(actv_cref, attr->actv_kobj_pool);
#endif

	if (is_inited) {
		err = __actv_pure_thread_destroy(actv_pure);
		if (err < E_HM_OK) {
			hm_warn("actv pure destroy failed: err=%s\n", hmstrerror(err));
		}
	}

	err = hm_ucap_delete_actv_pure(actv_cref, attr->actv_kobj_pool);
	if (err < E_HM_OK) {
		hm_error("delete actv failed, err=%s\n", hmstrerror(err));
	}

	if (is_inited) {
		/*
		 * If actv pure is self inited, memory should be recycled while
		 * free memm area slot. The fwork in struct actv_pure can be used
		 * for raw_workqueue_add_work and thus the memory recycling will
		 * be done by the worker thread.
		 */
		__actv_pure_mem_free_inited(actv_pure, sstack, attr);
	} else {
		/*
		 * If actv pure is not self inited, no memory should be recycled,
		 * so do nothing here.
		 */
	}

	return err;
}

void
actv_pure_stack_init(struct actv_pure *actv_pure,
		     struct actv_pure_attr *attr,
		     void *recv_buf, unsigned int actv_id,
		     unsigned long sstack_base)
{
	char *tsd = NULL;
	char *stk_top = recv_buf;
	unsigned long resv_tsd_sz;
	struct raw_thread *new = NULL;
	struct arch_actv_local *actv_local = NULL;

	BUG_ON(actv_pure == NULL, "actv_pure is unexpected NULL\n");
	BUG_ON(attr == NULL, "attr is unexpected NULL\n");
	BUG_ON(recv_buf == NULL, "recv_buf is unexpected NULL\n");

	/* 3.1) copy tls from elf tls section to stack */
	resv_tsd_sz = ALIGN_UP(attr->tsd_size * sizeof(void *), RAW_THREAD_STACK_ALIGN);
	tsd = ulong_to_ptr((ptr_to_ulong(stk_top) - resv_tsd_sz), char);
	new = raw_tls_copy((void *)(tsd - __raw_thread_tls_size()));

	/* 3.2) use actvret_size as thread stack_size */
	CMPT_THREAD_REG(new, THREAD_STACK_INFO_SIZE) = attr->actvret_size;

	/* Preserve the base of stack */
	new->stack_base = ptr_to_ulong(__actv_local_of_thread(new)) - ACTV_KERN_ARG_BUF_SIZE;
	/* Skip the guard page */
	new->stack_size = new->stack_base - (ptr_to_ulong(actv_pure_to_mem_buf(ptr_to_ulong(actv_pure), attr)) + PAGE_SIZE);

#ifdef CONFIG_NEED_SHADOW_STACK
	/* init shadow stack */
	BUG_ON((void *)sstack_base == NULL, "sstack_base is unexpected NULL\n");
	CMPT_THREAD_REG(new, sstack_base) = (void *)sstack_base;
	CMPT_THREAD_REG(new, sstack) = (void *)shadow_stack_start_addr((uintptr_t)CMPT_THREAD_REG(new, sstack_base));
#else
	(void)sstack_base;
#endif

	/* 3.3) init actv local */
	actv_local = __actv_local_of_thread(new);
	actv_local->rply_buf		= NULL;
	actv_local->recv_buf		= recv_buf;
	actv_local->rply_recv_buf_sz	= __RPC_BUFSZ_ENCODE(attr->recvbuf_size, 0u);
	actv_local->action_table	= attr->action_table;
	actv_local->action_table_sz	= attr->action_table_size;

	/* 3.4) setup actv thread, and use actv id as 'thread' tid */
	CMPT_THREAD_REG(new, tid)              = (int)actv_id;
	CMPT_THREAD_REG(new, THREAD_TSD)       = (void **)(void *)tsd;
	CMPT_THREAD_REG(new, locale)           = __libc_global_locale(plibc);
	CMPT_THREAD_REG(new, robust_list.head) = CMPT_THREAD_REF(new, robust_list.head);
	raw_thread_lockdep_depth_init(new);

	actv_pure->thread = new;
}

void actv_pure_stack_comb_kobj(unsigned long actv_pure)
{
	int err;
	cref_t actv_cref = ERR_TO_REF(E_HM_INVAL);
	struct actv_pure *p_actv_pure = ulong_to_ptr(actv_pure, struct actv_pure);

	BUG_ON(p_actv_pure == NULL, "actv_pure is unexpected NULL\n");

	/* set tls */
	raw_set_actv_tls(THREAD_TO_TLS(p_actv_pure->thread));
	hm_actv_local_reset();

	/* set actv_cref */
	err = sysfast_actv_cref(&actv_cref);
	BUG_ON(err < 0);
	err = (int)IS_REF_ERR(actv_cref);
	BUG_ON(err != 0);
	__actv_init_cref(p_actv_pure->thread, actv_cref);
}

void actv_pure_insert_thread_pool(struct actv_pure *actv_pure, struct actv_pure_attr *attr)
{
	int err;

	BUG_ON(actv_pure == NULL, "actv_pure is unexpected NULL\n");
	BUG_ON(attr == NULL, "attr is unexpected NULL\n");

	/*
	 * Note: actv_threadpool_insert need hold lock,
	 * so it must be called after set tls and actv id.
	 */
	err = actv_threadpool_insert(actv_pure->thread);
	BUG_ON(err != 0);

	raw_thread_link_to_global(actv_pure->thread, attr->link_thread);
}

static void *actv_pure_alloc_membuf(const struct actv_pure_attr *attr)
{
	void *membuf = MAP_FAILED;

	if (attr->mem_alloc != NULL) {
		hm_error("Don't support user specify mem_alloc hook\n");
	} else {
		membuf = actv_pure_mmap_membuf(attr->membuf_size);
		if (membuf == NULL) {
			hm_error("alloc actv membuf by mmap failed\n");
		}
	}

	return (membuf == MAP_FAILED) ? NULL : membuf;
}

#ifdef CONFIG_NEED_SHADOW_STACK
static void *actv_pure_alloc_sstack(struct actv_pure_attr *attr)
{
	void *sstack = NULL;

	if (attr->mem_alloc != NULL) {
		sstack = attr->mem_alloc(SHADOW_STACK_TOTAL_SIZE);
		if (sstack == NULL || sstack == MAP_FAILED) {
			hm_error("alloc actv sstack by memfunc failed\n");
		}
	} else {
		sstack = raw_mmap_shadow_stack();
		if (sstack == NULL) {
			hm_error("alloc actv sstack by mmap failed\n");
		}
	}

	return (sstack == MAP_FAILED) ? NULL : sstack;
}
#endif

static int actv_pure_self_alloc(struct actv_pure_attr *attr)
{
	int err = E_HM_OK;
	void *membuf = NULL;
	void *sstack = NULL;

	/* alloc actv membuf */
	membuf = actv_pure_alloc_membuf(attr);
	if (membuf == NULL) {
		err = E_HM_NOMEM;
	}

#ifdef CONFIG_NEED_SHADOW_STACK
	/* alloc actv sstack */
	if (err == E_HM_OK) {
		sstack = actv_pure_alloc_sstack(attr);
		if (sstack == NULL) {
			actv_pure_free_membuf(membuf, attr);
			err = E_HM_NOMEM;
		}
	}
#endif

	/* set the values of actv_pure/actv_local/sstack_base/sstack in attr */
	if (err == E_HM_OK) {
		__actv_pure_attr_fill(attr, membuf, sstack);
	}

	return err;
}

static void actv_pure_self_free(struct actv_pure_attr *attr)
{
	void *membuf = NULL;

	BUG_ON(attr == NULL, "actv pure attr should not be NULL\n");

	/* free actv membuf */
	membuf = actv_pure_to_mem_buf(attr->actv_pure, attr);
	if (membuf != NULL) {
		actv_pure_free_membuf(membuf, attr);
	}
#ifdef CONFIG_NEED_SHADOW_STACK
	if (attr->sstack_base != NULL) {
		actv_pure_free_sstack(attr->sstack_base, attr);
	}
#endif
	/* clear the values of actv_pure/actv_local/sstack_base/sstack in attr */
	__actv_pure_attr_clear(attr);
}

/* set membuf and sstack into kobj slot */
static void actv_pure_set_kobj_res(struct actv_pure_attr *attr)
{
	int err;
	cref_t actv_cref;
	/* fetch actv_cref */
	err = sysfast_actv_cref(&actv_cref);
	BUG_ON(err < 0);
	/* set membuf into kobj slot, used for membuf destroy */
	actv_kobj_res_set(attr->actv_kobj_pool, actv_cref, ulong_to_ptr(attr->actv_pure, void));
#ifdef CONFIG_NEED_SHADOW_STACK
	/* set sstack into kobj slot, used for sstack destroy */
	actv_kobj_sstack_set(attr->actv_kobj_pool, actv_cref, ulong_to_ptr(attr->sstack_base, void));
#endif
}

static int actv_pure_set_retval(unsigned long rpcinfo, int err)
{
	BUG_ON(err >= E_HM_OK);

	int retval = err;
	unsigned int callno = (unsigned int)__RPC_INFO_DECODE_CALLNO(rpcinfo);
	/*
	 * The rpcinfo of __ACTV_KERNMIN could be confused with that of
	 * syscap_ActivationInit, ignore it here.
	 */
	if ((callno > __ACTV_KERNMIN) && (callno < __ACTV_KERNMAX)) {
		hm_error_ratelimited("actv pure self init failed, callno=%u err=%s\n", callno, hmstrerror(err));
		/*
		 * The callee actv for upcall failed to do self-initialization, return
		 * E_HM_ACTIVATION_RPC_RETRY to restart the upcall if needed.
		 */
		retval = E_HM_ACTIVATION_RPC_RETRY;
	}

	return retval;
}

long actv_pure_self_init_hdlr(unsigned long init_attr, unsigned long rpcinfo)
{
	int err;
	unsigned int actv_id;
	void *recv_buf = NULL;
	void *mem_buf = NULL;
	void *touch_buf = NULL;
	unsigned long actv_pure;
	struct actv_pure_attr *attr = ulong_to_ptr(init_attr, struct actv_pure_attr);

	BUG_ON(attr == NULL, "actv pure attr should not be NULL\n");

	/*
	 * If the uninited actv is used to process vsfault, return E_HM_NOOBJ directly,
	 * the vsfault will be processed in kernel.
	 * Otherwise, deadlock might occur while alloc membuf, as the sysmgr vspace lock
	 * might be held when trigger vsfault.
	 */
	if ((unsigned int)__RPC_INFO_DECODE_CALLNO(rpcinfo) == __ACTV_UPCALL_VSFAULT) {
		hm_info("the uninited actv processes vsfault, return directly\n");
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_NOOBJ;
	}

	/* 0) alloc membuf and sstack */
	err = actv_pure_self_alloc(attr);
	if (err < 0) {
		hm_error_ratelimited("self alloc mem failed, err=%s\n", hmstrerror(err));
		hm_actv_unset_local_area();
		__hmattr_sideff_resolved("no relative side effect");
		return actv_pure_set_retval(rpcinfo, err);
	}
	actv_pure = attr->actv_pure;

	/* 1) alloc actv id */
	err = actv_pure_id_alloc(attr->resv_actv_idpool, &actv_id);
	if (err < 0) {
		hm_error_ratelimited("alloc actv pure id failed, err=%s\n", hmstrerror(err));
		actv_pure_self_free(attr);
		hm_actv_unset_local_area();
		__hmattr_sideff_resolved("no relative side effect");
		return actv_pure_set_retval(rpcinfo, err);
	}

	/* 2) touch buf */
	mem_buf = actv_pure_to_mem_buf(actv_pure, attr);
	touch_buf = ulong_to_ptr((ptr_to_ulong(mem_buf) + (unsigned long)PAGE_SIZE), void);
	err = touch_buffer(touch_buf, attr->membuf_size - (size_t)PAGE_SIZE);
	if (err < 0) {
		hm_error_ratelimited("touch recv buffer failed: %s\n", hmstrerror(err));
		int ret = actv_pure_id_free(actv_id);
		BUG_ON(ret < 0, "actv pure id %u exceed range\n", (unsigned int)actv_id);
		actv_pure_self_free(attr);
		hm_actv_unset_local_area();
		__hmattr_sideff_resolved("no relative side effect");
		return actv_pure_set_retval(rpcinfo, err);
	}

	/* 3) init stack buf: recv_buf is the top of stack buf */
	recv_buf = actv_pure_to_recv_buf(actv_pure, attr);
	actv_pure_stack_init(ulong_to_ptr(actv_pure, struct actv_pure),
			     attr, recv_buf, actv_id, ptr_to_ulong(attr->sstack_base));
	actv_pure_stack_comb_kobj(actv_pure);
	actv_pure_insert_thread_pool(ulong_to_ptr(actv_pure, struct actv_pure), attr);

	/* 4) call actv_init if need */
	if (attr->actv_init != NULL) {
		err = attr->actv_init();
		/* actv_init should not fail */
		BUG_ON(err < 0, "attr->actv_init failed, err = %s\n", hmstrerror(err));
	}

	/*
	 * 5) The guard has already been allocated inside actv_pure_self_alloc.
	 *    Record membuf and sstack in actv kobj slot and return RETRY errno when finish self init.
	 */
	actv_pure_set_kobj_res(attr);

	/* 6) Set default local area to support credential trasnfering */
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_CRED);

	return E_HM_ACTIVATION_RPC_RETRY;
}

int hm_actv_pure_trig_workqueue_init(struct raw_workqueue *trig_wq,
				     const char *name)
{
	int err;
	int priority = hm_thread_sched_getdefaultpriority();
	int policy = hm_thread_sched_getdefaultscheduler();
	struct raw_workqueue_attr attr;

	if (policy == SCHED_RR) {
		priority = HM_PRIORITY_ELASTIC;
	}

	if (trig_wq == NULL) {
		hm_error("trig wq is NULL\n");
		return E_HM_INVAL;
	}
	raw_workqueue_attr_init(&attr, ACTV_PURE_TRIG_WORKQUEUE_THREAD,
				ACTV_PURE_TRIG_WORKQUEUE_NR_QUEUES);
	NOFAIL(raw_workqueue_attr_set_inherit(&attr, RAW_THREAD_EXPLICIT_SCHED));
	NOFAIL(raw_workqueue_attr_set_policy(&attr, policy));
	NOFAIL(raw_workqueue_attr_set_prio(&attr, priority));
	NOFAIL(raw_workqueue_attr_set_name(&attr, name));
	err = raw_workqueue_init(trig_wq, &attr);
	if (err != E_HM_OK) {
		hm_error("raw work queue init failed: %s\n", hmstrerror(err));
	}
	return err;
}
