/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of actv
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 15 11:43:11 2019
 */
#include <libhmactv/actv.h>

#include <hmkernel/trigger.h>

#include <sys/mman.h>
#include <lib/utils.h>

#include <libsysif/sysmgr/api.h>
#include <libsysif/utils_server.h>

#include <hmasm/actv_rpc.h>
#include <libmem/utils.h>
#include <libmem/flags.h>

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/macro.h>
#include <hongmeng/syscall.h>

#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libhmactv/actv_id.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmsync/raw_mutex.h>
#include <sys/prctl.h>
/* for raw thread implement */
#include "../libhmsync/raw_thread_impl.h"

#include "actv_threadpool.h"

#define MAX_ASD_SIZE RAW_THREAD_KEYS_MAX

struct actv_init_args {
	struct raw_thread *thread;
	int (*actv_init)(void);
};

static int
actv_hdlr_init(struct arch_actv_local *actv_local)
{
	struct actv_init_args *args = ulong_to_ptr(actv_local->rpcinfo, struct actv_init_args);
	struct raw_thread *thread = args->thread;
	int err = 0;

	raw_set_actv_tls(THREAD_TO_TLS(thread));

#ifdef CONFIG_NEED_SHADOW_STACK
	set_shadow_stack(CMPT_THREAD_REG(thread, sstack));
#endif

	if (args->actv_init != NULL) {
		err = args->actv_init();
	}

	/* Set default local area to support credential trasnfering */
	hm_actv_set_local_area(__ACTV_LOCAL_AREA_FLAG_CRED);

	return err;
}

struct arch_actv_local *
__actv_local_of_thread(const struct raw_thread *thread)
{
	unsigned long reserve_size = 0;
	unsigned long aligned_thread = ALIGN_DOWN(ptr_to_ulong(thread), RAW_THREAD_STACK_ALIGN);

	reserve_size += ALIGN_UP(sizeof(struct arch_actv_local), RAW_THREAD_STACK_ALIGN);
	reserve_size += ALIGN_UP(CMPT_THREAD_REG(thread, THREAD_STACK_INFO_SIZE), RAW_THREAD_STACK_ALIGN);

	return ulong_to_ptr(aligned_thread - reserve_size, struct arch_actv_local);
}

void __actv_init_cref(struct raw_thread *thread, cref_t actv_cref)
{
	thread->cref.actv_cref = actv_cref;
}

static struct arch_actv_local *
init_actv_stack(const struct actv_init_args *args,
		void *recv_buf,
		unsigned long recv_buf_sz,
		void **action_table,
		size_t action_table_sz)
{
	struct arch_actv_local *actv_local = __actv_local_of_thread(args->thread);

	actv_local->rply_buf	     = NULL;
	actv_local->recv_buf	     = recv_buf;
	actv_local->rply_recv_buf_sz = __RPC_BUFSZ_ENCODE(recv_buf_sz, 0u);
	actv_local->init	     = actv_hdlr_init;
	actv_local->rpcinfo	     = ptr_to_ulong(args);

	actv_local->action_table	= action_table;
	actv_local->action_table_sz	= action_table_sz;

	BUG_ON(recv_buf_sz > __RPC_BUFSZ_BUFSZ_MAXLEN);
	return actv_local;
}

static bool
check_parameter(const struct actv *actv, struct actv_attr *attr)
{
	if ((actv == NULL) || (attr == NULL) || attr->stack != NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return false;
	}

	if (attr->recv_buf_sz > __RPC_BUFSZ_BUFSZ_MAXLEN) {
		__hmattr_sideff_resolved("no relative side effect");
		return false;
	}

	/* XXX: tsd is used for himalloc, sysmgr don't need. */
	if (hmsrv_sysmgr_cnode_idx() != hm_ucap_self_cnode_idx()) {
		attr->tsd_size = MAX_ASD_SIZE;
	}

	if (attr->stack_size == 0U) {
		attr->stack_size = ACTV_DEFAULT_STACK_SIZE;
	}

	return true;
}

static void link_raw_thread(const struct actv_attr *attr, struct raw_thread *new)
{
	/* Link raw thread to global thread list */
	if (attr->no_link_thread != 0) {
		CMPT_THREAD_REG(new, next) = RAW_THREAD_TO_PTHREAD(new);
		CMPT_THREAD_REG(new, prev) = RAW_THREAD_TO_PTHREAD(new);
	} else {
		struct raw_thread *self = __raw_thread_self();
		raw_thread_link_to_global(new, self);
	}
}

#ifdef CONFIG_NEED_SHADOW_STACK
static void sstack_munmap(char *addr)
{
	int err;

	if (addr == NULL) {
		return;
	}

	err = hm_mem_munmap(addr, SHADOW_STACK_TOTAL_SIZE);
	if (err != E_HM_OK) {
		hm_warn("hm_mem_munmap failed, err=%s\n", hmstrerror(err));
	}
}

static int sstack_init(struct raw_thread *thread)
{
	CMPT_THREAD_REG(thread, sstack_base) = raw_mmap_shadow_stack();
	if (CMPT_THREAD_REG(thread, sstack_base) == NULL) {
		return E_HM_NOMEM;
	}
	CMPT_THREAD_REG(thread, sstack) = (void *)shadow_stack_start_addr(
						(uintptr_t)CMPT_THREAD_REG(thread, sstack_base));
	return E_HM_OK;
}
#endif

#ifdef CONFIG_PAC
static cref_t g_pac_cref;
static struct raw_mutex g_pac_lock = RAW_MUTEX_INITIALIZER;
static void pac_init(void)
{
	cref_t tcb_cref = 0;
	int rc;
	raw_mutex_lock(&g_pac_lock);
	if (g_pac_cref == 0) {
		rc = sysfast_thread_cref(&tcb_cref);
		if (rc == E_HM_OK) {
			hm_actv_set_pac(tcb_cref);
		} else {
			hm_warn("failed to get thread tcb: %s\n", hmstrerror(rc));
		}
	}
	raw_mutex_unlock(&g_pac_lock);
}

void hm_actv_set_pac(cref_t thread_cref)
{
	g_pac_cref = thread_cref;
}

int hm_actv_pac_update(cref_t actv_cref)
{
	int rc;

	if (g_pac_cref == 0) {
		pac_init();
	}
	rc = syscap_ActivationPacCtrl(actv_cref, __PAC_CTRL_COPY, g_pac_cref);
	if (rc == 0) {
		rc = syscap_ActivationPacCtrl(actv_cref, __PAC_CTRL_UPDATE, PR_PAC_APIBKEY);
	}

	return rc;
}

int hm_actv_pool_pac_update(cref_t actv_pool)
{
	int rc;

	if (g_pac_cref == 0) {
		pac_init();
	}
	rc = syscap_ActivationPoolPacCtrl(actv_pool, __PAC_CTRL_COPY, g_pac_cref);
	if (rc == 0) {
		rc = syscap_ActivationPoolPacCtrl(actv_pool, __PAC_CTRL_UPDATE, PR_PAC_APIBKEY);
	}

	return rc;
}
#else
void hm_actv_set_pac(cref_t thread_cref)
{
	UNUSED(thread_cref);
}
int hm_actv_pac_update(cref_t actv_cref)
{
	UNUSED(actv_cref);
	return 0;
}
int hm_actv_pool_pac_update(cref_t actv_pool)
{
	UNUSED(actv_pool);
	return 0;
}
#endif /* CONFIG_PAC */

static void __actv_thread_setup(struct raw_thread *thread,
				const struct actv_attr *attr,
				char *tsd, char *addr,
				unsigned int actv_id)
{
	if (attr->stack == NULL) {
		CMPT_THREAD_REG(thread, THREAD_MAP_BASE) = (void *)addr;
		CMPT_THREAD_REG(thread, map_size) = attr->stack_size + (size_t)PAGE_SIZE;
	}

	CMPT_THREAD_REG(thread, tid)              = (int)actv_id;
	CMPT_THREAD_REG(thread, THREAD_TSD)       = (void **)(void *)tsd;
	CMPT_THREAD_REG(thread, locale)           = __libc_global_locale(plibc);
	CMPT_THREAD_REG(thread, robust_list.head) = CMPT_THREAD_REF(thread, robust_list.head);

	raw_thread_set_private(thread, attr->private_data);
}

struct actv_info_s {
	struct actv_attr *attr;
	cref_t actv_cref;
	unsigned int actv_id;
};

static int stack_init(struct actv_info_s *actv_info, char *addr,
		      size_t map_size, struct raw_thread **thread)
{
	int err;
	struct arch_actv_local *actv_local = NULL;
	struct actv_attr *attr = actv_info->attr;

	/* copy tls from elf tls section to stack */
	char *tsd = addr + map_size - (attr->tsd_size * sizeof(void *));
	struct raw_thread *new = raw_tls_copy((void *)(tsd - __raw_thread_tls_size()));

	if (attr->actvret_size == 0U) {
		attr->actvret_size = sizeof(union __actvret_hmcall);
	}
	/* use stack_size as actvret size */
	CMPT_THREAD_REG(new, THREAD_STACK_INFO_SIZE) = attr->actvret_size;

	/* Preserve the base of stack */
	new->stack_base = ptr_to_ulong(__actv_local_of_thread(new)) - ACTV_KERN_ARG_BUF_SIZE;
	/* Skip the guard page */
	new->stack_size = new->stack_base - (ptr_to_ulong(addr) + PAGE_SIZE);

#ifdef CONFIG_NEED_SHADOW_STACK
	err = sstack_init(new);
	if (err != E_HM_OK) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_NOMEM;
	}
#endif

	struct actv_init_args args = {
		.thread = new,
		.actv_init = attr->actv_init,
	};

	/* init actv real stack */
	actv_local = init_actv_stack(&args, attr->recv_buf, attr->recv_buf_sz,
				     attr->action_table, attr->action_table_sz);

	/* setup actv thread, and use actv id as 'thread' tid */
	__actv_thread_setup(new, attr, tsd, addr, actv_info->actv_id);

	raw_thread_lockdep_depth_init(new);
	raw_thread_rdlock_lockowner_init(new);

	__actv_init_cref(new, actv_info->actv_cref);
	err = actv_threadpool_insert(new);
	if (err != 0) {
#ifdef CONFIG_NEED_SHADOW_STACK
		sstack_munmap(CMPT_THREAD_REG(new, sstack_base));
#endif
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_OBJEXIST;
	}

	err = syscap_ActivationInit(actv_info->actv_cref,
				    ulong_to_ptr(ptr_to_ulong(arch_actv_rpc_entry), void),
				    (uintptr_t)actv_local);
	if (err < E_HM_OK) {
		hm_warn("Activation init failed, err=%s\n", hmstrerror(err));
#ifdef CONFIG_NEED_SHADOW_STACK
		sstack_munmap(CMPT_THREAD_REG(new, sstack_base));
#endif
		actv_threadpool_remove(new);
		__hmattr_sideff_resolved("no relative side effect");
		return err;
	}

	link_raw_thread(attr, new);
	*thread = new;

	return E_HM_OK;
}

static void stack_munmap(char *addr, size_t size)
{
	int err;
	err = hm_mem_munmap(addr, size + (size_t)PAGE_SIZE);
	if (err < E_HM_OK) {
		hm_warn("hm_mem_munmap failed, err=%s\n", hmstrerror(err));
	}
}

static char *stack_mmap(size_t stack_size)
{
	/* excluding the guard page, map other stack pages to R/W permission */
	char *addr = hm_mem_mmap_ex(NULL, stack_size,
			       __U(PROT_READ) | __U(PROT_WRITE),
#ifdef CONFIG_FORK_COW
			       __U(MAP_PRIVATE) | __U(MAP_ANON) | MFLAG_NOCOW | MFLAG_MAP_ACTV | MFLAG_MAP_ACTV_CACHE,
#else
			       __U(MAP_PRIVATE) | __U(MAP_ANON) | MFLAG_MAP_ACTV | MFLAG_MAP_ACTV_CACHE,
#endif
			       -1, 0, PAGE_SIZE);
	if (addr == MAP_FAILED) {
		hm_warn("map stack failed\n");
		__hmattr_sideff_resolved("no relative side effect");
		return addr;
	}
	return addr - PAGE_SIZE;
}

static char *stack_prepare(const struct actv_attr *attr, size_t *map_size)
{
	char *addr = MAP_FAILED;

	/* alloc stack for activation, stack_size+guard_size */
	*map_size = attr->stack_size + (size_t)PAGE_SIZE;
	addr = stack_mmap(attr->stack_size);

	return addr;
}

static void __actv_fill(struct actv *actv, struct actv_attr *attr,
			struct raw_thread *thread, cref_t actv_cref)
{
	actv->thread      = thread;
	actv->obj.cref   = actv_cref;
	actv->recv_buf    = attr->recv_buf;
	actv->recv_buf_sz = attr->recv_buf_sz;
}

static int new_actv(cref_t actv_pool_cref, struct kobj_self_mng *obj)
{
	int err;
	err = hm_ucap_new_actv(actv_pool_cref, obj);
	if (err != E_HM_OK) {
		__hmattr_sideff_resolved("nothing to rollback, no relative side effect");
		return err;
	}
#ifdef CONFIG_PAC
	err = hm_actv_pac_update(obj->cref);
	if (err != E_HM_OK) {
		(void)hm_ucap_delete_actv(obj);
		__hmattr_sideff_resolved("activation has been deleted, no relative side effect");
		return err;
	}
#endif
	return err;
}

/*
 *
 *  Stack of activation & raw thread:
 *
 *       Activation Stack                 raw thread stack
 *
 *  High  +------------+                   +------------+
 *        |   tsd(0)   |  <- (empty)       |    tsd     |
 *        +------------+                   +------------+
 *        |    TLS     |                   |    TLS     |
 *        +------------+  <--- TLS reg --> +------------+
 *        |   pthread  |                   |   pthread  |
 *        +------------+                   +------------+
 *        | raw thread |                   | raw thread |
 *        |  specific  |                   |  specific  |
 *        +------------+                   +------------+
 *        |   retbuf   |                   |   stack    |
 *        +------------+                   +------------+
 *        | actv_local |                   |   guard    |
 *        +------------+                   +------------+
 *        |  128 bytes |
 *        +------------+  <----SP
 *        |    stack   |
 *        +------------+
 *        |    guard   |
 *   Low  +------------+
 *
 *   NOTE:
 *   1. Asd: (activation specific data), it is dynamic ALS, when supports
 *      pthread_key_create(); must reserve memory for it.
 *
 *   2. ALS: (activation local storage), it is like TLS data, when supports
 *      __thread; must reserve memory for it.
 *
 *   3. Put a `struct pthread` in stack to compatible with pthread/raw thread,
 *      then in user space, activation is a thread actually.
 */
int hm_actv_create(struct actv *actv, struct actv_attr *attr)
{
	int err;
	size_t map_size;
	char *addr = NULL;
	unsigned int actv_id;
	struct raw_thread *thread = NULL;
	struct actv_info_s actv_info;

	if (!check_parameter(actv, attr)) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	mem_zero_p(actv, sizeof(struct actv), sizeof(struct actv));
#ifdef CONFIG_ACTV_CACHE_POOL
	dlist_init(&actv->node);
#endif

	actv_id = actv_id_alloc();
	if (actv_id == (unsigned int) -1) {
		__hmattr_sideff_resolved("actv_id alloc failed, no relative side effect");
		return E_HM_NOMEM;
	}

	addr = stack_prepare(attr, &map_size);
	if (addr == MAP_FAILED) {
		actv_id_free(actv_id);
		__hmattr_sideff_resolved("actv_id has freed, no relative side effect");
		return E_HM_NOMEM;
	}

	/* New actv capability in kernel */
	err = new_actv(attr->actv_pool, &actv->obj);
	if (err != E_HM_OK) {
		if (attr->stack == NULL) {
			stack_munmap(addr, attr->stack_size);
		}
		actv_id_free(actv_id);
		__hmattr_sideff_resolved("actv_id has freed, no relative side effect");
		return err;
	}
	actv_info.attr      = attr;
	actv_info.actv_cref = actv->obj.cref;
	actv_info.actv_id   = actv_id;
	err = stack_init(&actv_info, addr, map_size, &thread);
	if (err < E_HM_OK) {
		(void)hm_ucap_delete_actv(&actv->obj);
		if (attr->stack == NULL) {
			stack_munmap(addr, attr->stack_size);
		}
		actv_id_free(actv_id);
		__hmattr_sideff_resolved("actv_id has freed, no relative side effect");
		return err;
	}

	__actv_fill(actv, attr, thread, actv->obj.cref);

	return E_HM_OK;
}

void __actv_destroy_thread(struct raw_thread *thread)
{
	/* unlink actv thread from global thread list */
	raw_thread_remove_from_global(thread);
	/* run tsd destructors */
	__raw_thread_tsd_run_dtors(thread);
}

/* caller should guarantee actv is not in use */
static int __actv_destroy(struct actv *actv, struct raw_thread *thread)
{
	int err;

	err = hm_ucap_delete_actv(&actv->obj);
	if (err < E_HM_OK) {
		hm_warn("delete actv failed, err=%s\n", hmstrerror(err));
	}

	/* remove from threadpool and free actv id */
	if (CMPT_THREAD_REG(thread, tid) != 0) {
		actv_threadpool_remove(thread);
		actv_id_free((unsigned int)(CMPT_THREAD_REG(thread, tid)));
	}

	__actv_destroy_thread(thread);

	return err;
}

int hm_actv_destroy(struct actv *actv)
{
	int err;
	int unmap_ret = 0;
	struct raw_thread *thread = NULL;

	if (actv == NULL || actv->thread == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	thread = actv->thread;

	err = __actv_destroy(actv, thread);
	if (err < E_HM_OK) {
		hm_warn("actv destroy failed, err=%s\n", hmstrerror(err));
	}

#ifdef CONFIG_NEED_SHADOW_STACK
	sstack_munmap(CMPT_THREAD_REG(thread, sstack_base));
#endif

	if (CMPT_THREAD_REG(thread, THREAD_MAP_BASE) != NULL) {
		unmap_ret = hm_mem_munmap(CMPT_THREAD_REG(thread, THREAD_MAP_BASE), CMPT_THREAD_REG(thread, map_size));
		if (unmap_ret != E_HM_OK) {
			hm_warn("unmap actv stack failed, err=%s\n", hmstrerror(unmap_ret));
		}
	}

	return (err != E_HM_OK) ? err : unmap_ret;
}

/*
 * void ret value types, because even if it fails, it can't do anything.
 */
void hm_actv_pool_bind_clean(const char *buf_pool_name,
			     const struct actv_bind_ret_rref *rec)
{
	int err = E_HM_OK;

	if ((buf_pool_name == NULL) || (rec == NULL)) {
		__hmattr_sideff_resolved("no relative side effect");
		return;
	}

	if (!IS_REF_ERR(rec->rref)) {
		err = hm_path_release(buf_pool_name, rec->rref);
		if (err < 0) {
			hm_warn("failed to path release, err=%s\n",
				 hmstrerror(err));
		}
	}
}

void hm_actv_pool_bind_sysmgr_clean(const struct actv_bind_ret_rref *rec)
{
	hm_actv_pool_bind_clean("sysmgr_actvpool_buf", rec);
}

static int actv_acquire_actvpool(const char *buf_pool_name,
				 struct actv_bind_ret_rref *rec)
{
	int err = E_HM_OK;

	/* acquire and bind two kinds of server actvpool to let this server use
	 * the server's actvpool */
	err = hm_path_acquire(buf_pool_name, &(rec->rref));
	if (err < 0) {
		hm_warn("failed to acquire path buf_pool_name=%s, err=%s\n",
			buf_pool_name, hmstrerror(err));
		hm_actv_pool_bind_clean(buf_pool_name, rec);
	}

	return err;
}

static int actv_bind_actvpool(const struct actv_bind_ret_rref *bind_rref, cref_t pool_ap)
{
	int err;

	err = syscap_ActivationPoolBind(bind_rref->rref,
					pool_ap,
					(unsigned int)__CAP_ACTV_BIND_FASTCALL_G1_NOBUF,
					__ACTV_POOL_BIND_FLAG_POOL);
	if (err != E_HM_OK) {
		hm_warn("failed to ActivationPoolBind NOBUF, err=%s\n", hmstrerror(err));
	}

	return err;
}

int hm_actv_pool_bind_server(cref_t ap_cref, unsigned long grant_info,
			     const char *buf_pool_name,
			     struct actv_bind_ret_rref *ret_rref)
{
	int err;

	UNUSED(grant_info);
	if ((buf_pool_name == NULL) || (ret_rref == NULL)) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	err = actv_acquire_actvpool(buf_pool_name, ret_rref);
	if (err < 0) {
		/* here do not print error message because has been printed in function above */
		__hmattr_sideff_resolved("no relative side effect");
		return err;
	}

	err = actv_bind_actvpool(ret_rref, ap_cref);
	if (err < 0) {
		/* here do not print error message because has been printed in function above */
		hm_actv_pool_bind_clean(buf_pool_name, ret_rref);
		__hmattr_sideff_resolved("bind has cleaned, no relative side effect");
		return err;
	}
	return E_HM_OK;
}

int hm_actv_pool_bind_sysmgr(cref_t ap_cref, unsigned long grant_info,
			     struct actv_bind_ret_rref *ret_rref)
{
	return hm_actv_pool_bind_server(ap_cref,
					grant_info,
					"sysmgr_actvpool_buf",
					ret_rref);
}

int hm_actv_pool_set_cred(cref_t ap_cref, uref_t ap_uref,
			  unsigned int dst_cnode_idx,
			  unsigned long credential)
{
	return syscap_ActivationPoolSetCredential(ap_cref, ap_uref,
						  dst_cnode_idx, credential);
}

struct arch_actv_local *hm_actv_current_arch_actv_local(void)
{
	struct raw_thread *self = __raw_thread_self();
	return __actv_local_of_thread(self);
}

bool hm_actv_local_is_compat(void)
{
#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	return (__RPC_INFO_DECODE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_FAKE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_FAKE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_ILP32_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_ILP32_BIT((unsigned long long)local->rpcinfo)) != 0;
#else
	return false;
#endif
}

bool hm_actv_local_is_compat32(void)
{
#ifdef CONFIG_COMPAT
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	return (__RPC_INFO_DECODE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_FAKE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_FAKE_COMPAT_BIT((unsigned long long)local->rpcinfo)) != 0;
#else
	return false;
#endif
}

bool hm_actv_local_is_ilp32(void)
{
#ifdef CONFIG_ILP32
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	return (__RPC_INFO_DECODE_ILP32_BIT((unsigned long long)local->rpcinfo) ||
		__RPC_INFO_DECODE_XACT_ILP32_BIT((unsigned long long)local->rpcinfo)) != 0;
#else
	return false;
#endif
}

void *hm_actv_current_actvret(void)
{
	struct raw_thread *thread = __raw_thread_self();
	unsigned long aligned_thread = ALIGN_DOWN(ptr_to_ulong(thread), RAW_THREAD_STACK_ALIGN);

	return ulong_to_ptr(aligned_thread - CMPT_THREAD_REG(thread, THREAD_STACK_INFO_SIZE), void);
}

void *hm_actv_current_stack_args(void)
{
	unsigned long reserved_size = 0UL;
	struct raw_thread *thread = __raw_thread_self();
	unsigned long aligned_thread = ALIGN_DOWN(ptr_to_ulong(thread), RAW_THREAD_STACK_ALIGN);

	reserved_size += ALIGN_UP(sizeof(struct arch_actv_local), RAW_THREAD_STACK_ALIGN);
	reserved_size += ALIGN_UP(CMPT_THREAD_REG(thread, THREAD_STACK_INFO_SIZE), RAW_THREAD_STACK_ALIGN);
	reserved_size += ALIGN_UP(sizeof(unsigned long) * __RPC_INFO_MAX_STK_BUF_NR_UL, RAW_THREAD_STACK_ALIGN);

	return ulong_to_ptr(aligned_thread - reserved_size, void);
}

static int actvpool_set_rply(void *data, unsigned long datasz)
{
	struct arch_actv_local *actv_local = hm_actv_current_arch_actv_local();
	unsigned long bufsz = 0UL;
	unsigned long rply_recv_buf_sz = 0UL;
	int err = 0;

	if (datasz > __RPC_BUFSZ_DATASZ_MAXLEN) {
		err = E_HM_INVAL;
	}

	if (err == 0) {
		bufsz = __RPC_BUFSZ_DECODE_BUFSZ(actv_local->rply_recv_buf_sz);
		rply_recv_buf_sz = __RPC_BUFSZ_ENCODE(bufsz, datasz);
		actv_local->rply_buf		= data;
		actv_local->rply_recv_buf_sz	= rply_recv_buf_sz;
	}
	return err;
}

static void *prepare_ret_remotecall(unsigned long size)
{
	int err;
	void *actvret = hm_actv_current_actvret();
	NOFAIL(memset_s(actvret, size, 0, size));

	err = actvpool_set_rply(actvret, size);
	BUG_ON(err < 0);
	return actvret;
}

static void *prepare_ret_selfcall(unsigned long long sender)
{
	struct sysif_actv_selfcall *pselfcall = (struct sysif_actv_selfcall *)(uintptr_t)(sender);

	return pselfcall->buf;
}

__attribute__((returns_nonnull))
void *hm_actvpool_prepare_ret(unsigned long long sender,
			      unsigned long credential,
			      unsigned long size)
{
	void *pret = NULL;

	if (!sysif_actv_is_selfcall(credential)) {
		pret = prepare_ret_remotecall(size);
	} else {
		pret = prepare_ret_selfcall(sender);
	}
	return pret;
}

__attribute__((returns_nonnull))
void *hm_actvpool_ret(unsigned long long sender, unsigned long credential)
{
	void *pret = NULL;

	if (!sysif_actv_is_selfcall(credential)) {
		pret = hm_actv_current_actvret();
	} else {
		struct sysif_actv_selfcall *pselfcall = (struct sysif_actv_selfcall *)(uintptr_t)(sender);
		pret = pselfcall->buf;
	}
	return pret;
}

void hm_actvpool_set_kcall_retval(unsigned long long sender,
				  unsigned long credential,
				  unsigned long v)
{
	UNUSED(sender);

	if (!sysif_actv_is_selfcall(credential)) {
		struct arch_actv_local *actv_local = hm_actv_current_arch_actv_local();
		unsigned long bufsz = 0UL;
		unsigned long rply_recv_buf_sz = 0UL;

		bufsz = __RPC_BUFSZ_DECODE_BUFSZ(actv_local->rply_recv_buf_sz);

		rply_recv_buf_sz		= __RPC_BUFSZ_ENCODE(bufsz, 0UL);
		actv_local->kcall_retval	= v;
		actv_local->rply_recv_buf_sz	= rply_recv_buf_sz;
	}
}

void *hm_actvpool_prepare_kcall_ret(unsigned long long sender,
				    unsigned long credential,
				    unsigned long size)
{
	UNUSED(sender);
	/*
	 * The credential received by the upcall handler should
	 * not be __ACTV_CRED_SELFCALL.
	 */
	BUG_ON(sysif_actv_is_selfcall(credential));

	void *pret = NULL;

	if (size <= __ACTV_UPCALL_MAX_RET_ARG_SZ) {
		pret = prepare_ret_remotecall(size);
	}

	return pret;
}

static struct actvpool_recv
curr_recv_data_selfcall(unsigned long long sender)
{
	struct actvpool_recv ret;
	struct sysif_actv_selfcall *pselfcall = u64_to_ptr(sender, void);

	ret.data = pselfcall->data;
	ret.size = pselfcall->data_sz;
	return ret;
}

static struct actvpool_recv
curr_recv_data_remotecall(void)
{
	struct actvpool_recv ret;
	struct arch_actv_local *actv_local = hm_actv_current_arch_actv_local();

	ret.data = actv_local->recv_buf;
	ret.size = __RPC_INFO_DECODE_MEM_BUF_LEN(actv_local->rpcinfo);
	return ret;
}

struct actvpool_recv
hm_actvpool_curr_recv_data(unsigned long long sender,
			   unsigned long credential)
{
	struct actvpool_recv ret;

	if (!sysif_actv_is_selfcall(credential)) {
		ret = curr_recv_data_remotecall();
	} else {
		ret = curr_recv_data_selfcall(sender);
	}
	return ret;
}

int hm_actv_pool_listen_water(cref_t pool_cref)
{
	if (IS_REF_ERR(pool_cref)) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	return syscap_ActivationPoolListenWater(pool_cref, (int)ACTV_POOL_WATER_TRIGGER);
}

int hm_actv_pool_check_water(cref_t pool_cref,
			     struct __actv_pool_water_trigger_info *info)
{
	if ((IS_REF_ERR(pool_cref)) || (info == NULL)) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	return syscap_ActivationPoolCheckWater(pool_cref, info);
}

int
hm_actv_pool_set_water(cref_t pool_cref,
		       struct __actv_pool_water *water)
{
	if (IS_REF_ERR(pool_cref) || (water == NULL)) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	return syscap_ActivationPoolSetWater(pool_cref, water, 0UL);
}

int hm_trigger_wait(unsigned long flags, struct __hmsigmask *sigmask)
{
	return sysfast_trigger_wait(flags, sigmask);
}

cref_t
hm_actv_id2cref(unsigned int actv_id)
{
	RAW_RWLOCK_RD_GUARD(_, actv_threadpool_lock());
	struct raw_thread *thread = actv_threadpool_search_nolock(actv_id);
	if (thread == NULL) {
		return ERR_TO_REF(E_HM_NOOBJ);
	}
	return thread->cref.actv_cref;
}

int hm_actv_load_stat_ex(struct sysarg_actv_stat *stat,
			 struct __arch_fpsimd_state *fpsimd,
			 cref_t *actv_chain_iter,
			 const cref_t *thread_cref,
			 unsigned long flags)
{
	return sysfast_actv_load_stat(stat, fpsimd, actv_chain_iter,
				      thread_cref, flags);
}

/*
 * Copy arg from caller to callee.
 *
 * Parameters:
 *  - dst		receive buf
 *  - dst_size		receive max buf length
 *  - src		caller source data addr and length
 *
 * Return E_HM_OK for success, hmerrno for errors.
 */
int hm_actv_read_caller_buf(void *dst, unsigned long dst_size,
			    const struct hm_actv_buf *src)
{
	int ret;

	if ((dst == NULL) || (src == NULL)) {
		return E_HM_INVAL;
	}

	if (src->size == 0U) {
		return E_HM_OK;
	}

	if ((ulong_to_ptr((unsigned long)src->buf, const void) == NULL) || (dst_size < src->size)) {
		return E_HM_INVAL;
	}

	ret = hm_actv_read_caller_vm(dst, ulong_to_ptr((unsigned long)src->buf, const void), (unsigned long)src->size);
	return (ret >= 0) ? 0 : ret;
}

int hm_actv_pool_emrg_init_ex(cref_t actv_pool,
			      struct actv_emrg_attr *attr,
			      rpc_entry_t entry)
{
	if (attr == NULL) {
		return E_HM_INVAL;
	}
	/*
	 * The futex_val must be touched since kernel will check
	 * whether it has physical memory.
	 */
	attr->futex_val = -1;
	return syscap_ActivationPoolEmrgInit(actv_pool,
					     ulong_to_ptr(ptr_to_ulong(entry), void),
					     ptr_to_ulong(attr), &attr->futex_val);
}

int hm_actv_pool_read_caller_vm(unsigned long cred,
				void *dst, const void *src, unsigned long size)
{
	int err;
	if (sysif_actv_is_selfcall(cred)) {
		NOFAIL(memcpy_s(dst, size, src, size));
		err = E_HM_OK;
	} else {
		err = hm_actv_read_caller_vm((void *)dst, src, size);
	}
	return err;
}

int hm_actv_chain_depth(int *actv_seq)
{
	struct sysarg_actv_stat stat;
	cref_t thread_cref = 0ULL;
	unsigned long flags = 0UL;
	cref_t actv_cref = 0ULL;
	int actv_dep = 0;
	int rc;

	if (actv_seq == NULL) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(stat);
		thread_cref = hm_thread_self_cref();
		while ((rc = hm_actv_load_stat(&stat, &actv_cref, &thread_cref, flags)) >= 0) {
			if (actv_cref == 0ULL) {
				break;
			}
			actv_dep++;
			rc = E_HM_OK;
		}
	}
	if (rc == E_HM_OK) {
		*actv_seq = actv_dep;
	}
	return rc;
}

int hm_actv_pool_set_schedinfo(cref_t actv_pool,
			       const struct __actvpool_sched_info_s *sched_info,
			       unsigned int flags)
{
	return syscap_ActivationPoolSetSchedInfo(actv_pool, sched_info, flags);
}

#ifndef CONFIG_SUPPORT_SPIN_FPU
void hm_actv_fpu_begin(void)
{
	int err;
	err = sysfast_actv_fpu_begin();
	BUG_ON(err != E_HM_OK, "err = %s\n", hmstrerror(err));
}

void hm_actv_fpu_end(void)
{
	int err;
	err = sysfast_actv_fpu_end();
	BUG_ON(err != E_HM_OK, "err = %s\n", hmstrerror(err));
}
#else
void hm_actv_fpu_begin(void) { }
void hm_actv_fpu_end(void) { }
#endif

void hm_actv_current_trigger_detach(void)
{
	int err;
	unsigned int tid = (unsigned int)raw_thread_tid_of(raw_thread_self());

	if (tid >= actv_id_min()) {
		do {
			err = syscap_ActivationTriggerDetachOne(0);
		} while (err == E_HM_OK);

		BUG_ON_D(err != E_HM_TRIGGER_NO_CURR, "err=%s\n", hmstrerror(err));
	}
}
