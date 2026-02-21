/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The api of the Activation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 15 11:43:11 2019
 */

#ifndef ULIBS_INCLUDE_LIBHMACTV_ACTV_H
#define ULIBS_INCLUDE_LIBHMACTV_ACTV_H

#include <hmasm/actv_rpc.h>
#include <libsecure/sec_cred.h>

#include <hmkernel/capability.h>
#include <hmkernel/miscdata.h>
#include <hmkernel/activation/local_area.h>

#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmucap/kobjpool.h>
#include <libhmucap/actv_kobjpool.h>
#include <hmkernel/pac.h>

/*
 * __SIZEOF_POINTER__ is a common predefined macro, which defined
 * the number of bytes of the C standard data type void *.
 *
 * when __SIZEOF_POINTER__ equals to 8, the size of address is 8,
 * and thus the actv need more stack, especially when the lock trace
 * buffer is preserved in the actv stack. So we define different stack size
 * of actv for different __SIZEOF_POINTER__.
 */
#ifndef __hmbuild__
/* Use legacy kconfig option to define morden hmbuild feature macro */
# ifdef CONFIG_SYSMGR_KASAN
#  ifndef __hmfeature_kasan__
#   define __hmfeature_kasan__
#  endif
# endif
#endif

#ifdef __hmfeature_kasan__
/* kasan uses wrapper functions and check functions, need more stack for chain of function calls */
#define ACTV_DEFAULT_STACK_SIZE	(4 * PAGE_SIZE)
#else
#if (__SIZEOF_POINTER__ == 8u)
#define ACTV_DEFAULT_STACK_SIZE	(3 * PAGE_SIZE)
#else
#define ACTV_DEFAULT_STACK_SIZE	(2 * PAGE_SIZE)
#endif
#endif

#define ACTV_BIND_POOL_NUM	2u
#define ACTV_KERN_ARG_NUM	32
#define ACTV_KERN_ARG_BUF_SIZE	(ACTV_KERN_ARG_NUM * sizeof(unsigned long))

typedef void (*rpc_entry_t)(void *attr);
typedef void *(*mem_alloc_func_t)(size_t size);
typedef void (*mem_free_func_t)(void *ptr, size_t size);

struct actv_bind_ret_rref {
	rref_t rref;
};

#define SCHED_MAGIC_VAL 0x1AA1
#define LT_MAGIC_VAL 0x6AA6
#define SECCOMP_MAGIC_VAL 0x7A7A

void hm_actv_local_reset(void);
void hm_actv_unset_local_area(void);
void hm_actv_set_local_area(unsigned long flags);
uint64_t hm_actv_local_get_raising_hmsignals(void);

struct __sched_info_s *hm_actv_local_sched_of(void);

/* fast series with provided local area ptr, no need to read tpidr, better for performance */
static inline void sched_info_set_sched_state(struct __sched_info_s *info, unsigned int state)
{
	info->state = state;
}

static inline bool sched_info_is_inited(struct __sched_info_s *info)
{
	return info->inited == SCHED_MAGIC_VAL;
}

static inline unsigned int sched_info_sched_state_of(struct __sched_info_s *info)
{
	return info->state;
}

static inline unsigned int sched_info_cpu_of(struct __sched_info_s *info)
{
	return info->cpu;
}

static inline bool sched_info_need_resched(struct __sched_info_s *info)
{
	return info->resched != 0;
}

static inline void sched_info_clear_resched(struct __sched_info_s *info)
{
	info->resched = 0;
}

void hm_actv_local_set_sched_inited(void);
unsigned int hm_actv_local_lt_group_of(void);
void hm_actv_local_clear_resched(void);
void hm_actv_local_set_iowait(void);
void hm_actv_local_clr_iowait(void);
void hm_actv_local_set_block_info(unsigned long pc);
void hm_actv_local_clr_block_info(void);
int hm_actv_chain_depth(int *actv_seq);

pid_t __hm_actv_local_thread_vtid_of(void);
pid_t __hm_actv_local_thread_vpid_of(void);
pid_t __hm_actv_local_thread_tid_of(void);
pid_t __hm_actv_local_thread_pid_of(void);
const char *__hm_actv_local_thread_name_of(void);
unsigned long long __hm_actv_local_thread_cref_of(void);
unsigned long long __hm_actv_local_sched_thread_cref_of(void);
__u32 __hm_actv_local_thread_priority_of(void);
__u64 __hm_actv_local_thread_vskey_of(void);
pid_t hm_actv_local_thread_vtid_of(void);
pid_t hm_actv_local_thread_vpid_of(void);
pid_t hm_actv_local_thread_tid_of(void);
pid_t hm_actv_local_thread_pid_of(void);
const char *hm_actv_local_thread_name_of(void);
uint32_t hm_actv_local_thread_cpu_of(void);
unsigned long long hm_actv_local_thread_cref_of(void);
unsigned long long hm_actv_local_sched_thread_cref_of(void);
void hm_actv_local_set_seccomp_info_inited(void);
bool hm_actv_local_is_seccomp_info_inited(void);
bool hm_actv_local_seccomp_need_check_thiscall(void);
uint32_t hm_actv_local_seccomp_scno_of(void);
uint32_t hm_actv_local_seccomp_arch_of(void);
uint64_t hm_actv_local_seccomp_filter_of(void);
uint64_t hm_actv_local_seccomp_cofilter_index_of(void);
volatile struct __seccomp_info_s *hm_actv_local_seccomp_info_of(void);
uint64_t *hm_actv_local_seccomp_args_of(void);
void hm_actv_local_clear_seccomp_info(void);
__u64 hm_actv_local_actv_ready_time_of(void);
__u64 hm_actv_local_actv_block_time_of(void);
__u32 hm_actv_local_actv_ready_cnt_of(void);
__u32 hm_actv_local_actv_block_cnt_of(void);
bool hm_actv_local_actv_is_need_reset(void);
void hm_actv_local_actv_set_need_reset(void);
bool hm_actv_local_is_compat(void);
bool hm_actv_local_is_compat32(void);
bool hm_actv_local_is_ilp32(void);
uint64_t hm_actv_service_info(void);
__u64 hm_actv_local_actv_cred_of(void);
__u64 hm_actv_local_actv_xcred_of(void);
void *hm_actv_local_creds_addr_of(void);
__u32 hm_actv_local_thread_priority_of(void);
__u64 hm_actv_local_thread_vskey_of(void);

__u64 hm_actv_local_saved_uctx_of(void);
void hm_actv_local_set_saved_uctx(__u64 uctx);
__u64 hm_actv_local_flush_cb_of(void);
void hm_actv_local_set_flush_cb(__u64 flush_cb);
__u64 hm_actv_local_flush_stack_of(void);
void hm_actv_local_set_flush_stack(__u64 flush_stack);
void hm_actv_local_set_fs_inited(void);
__u32 hm_actv_local_thread_kcov_idx_state(void);
__u32 __hm_actv_local_thread_kcov_idx_state(void);

struct actv_attr {
	cref_t  actv_pool;
	size_t  tsd_size;
	size_t  actvret_size;
	size_t  stack_size;
	void   *stack;
	void *private_data;
	/*
	 * don't link thread structure into global thread list if needed.
	 * if this flags is set, can't use TLS for dynamic linked elf after dlopen.
	 */
	int    no_link_thread;

	void   *recv_buf;
	size_t  recv_buf_sz;

	void   *action_table;
	size_t  action_table_sz;

	int (*actv_init)(void);
};

struct actv {
	struct kobj_self_mng obj;
	/*
	 * stack buf which is not allocated by hm_actv_created,
	 * should be recycled before destroying actv.
	 */
	void *stack_buf;
	size_t stack_buf_sz;
	/* recv_buf may be recycled before destroy actv */
	void *recv_buf;
	size_t recv_buf_sz;

	/* actv thread structure pointer */
	raw_thread_t thread;
#ifdef CONFIG_ACTV_CACHE_POOL
	struct dlist_node node;
#endif
};

/* internal api which is only used in actv.c and actv_pure.c */
struct arch_actv_local *
__actv_local_of_thread(const struct raw_thread *thread);

void __actv_init_cref(struct raw_thread *thread, cref_t actv_cref);

int hm_actv_create(struct actv *actv, struct actv_attr *attr);
/* internal api which is only used in actv.c and actv_pure.c */
void __actv_destroy_thread(struct raw_thread *thread);
int hm_actv_destroy(struct actv *actv);

/* used for free memm area slot by worker thread */
struct free_slots_work {
	struct raw_work work;
	void *mem_buf;
	void *sstack;
	const struct actv_pure_attr *attr;
};

/*
 * Use pure as suffix after actv (actv_pure, actv_pure_attr) to
 * indicate that the recv buf, stack buf of this actv will not
 * be initialized before it is used by client for the first time.
 * And only some neccessary attributes should be determinted and
 * passed to server for the use when self-initialization.
 *
 * actv_pure will be located at the top of mem_buf,
 * see the layout of mem_buf below.
 */
struct actv_pure {
	/* actv thread structure pointer */
	raw_thread_t thread;
	/* used for free memm area slot by worker thread */
	struct free_slots_work fwork;
};

/*
 * deployment of activation mem buf:
 *
 * High Addr   +------------+ <------------------------------+
 *             | actv_pure  |                      ^         ^
 *             +------------+                      |         |
 *             |  recv buf  |                      |         |
 *             +------------+ <-------------+      |         |
 *             |    tsd     |               ^      |         |
 *             +------------+               |      |         |
 *             |    TLS     |               |      |         |
 * TLS REG --> +------------+               |  Reserved Buf  |
 *             |   pthread  |               |      |         |
 *             +------------+               |      |         |
 *             | raw thread |               |      |   Activation Mem Buf
 *             |  specific  |               |      |         |
 *             +------------+               |      |         |
 *             |   retbuf   | Activation Stack Buf |         |
 *             +------------+               |      |         |
 *             | actv_local |               |      V         |
 *             +------------+ <--------------------+         |
 *             |  128 bytes |               |                |
 * SP REG ---> +------------+               |                |
 *             |    stack   |               |                |
 *             +------------+               |                |
 *             |    guard   |               V                V
 * Low Addr    +------------+ <-------------+----------------+
 *
 * the fields: self_init_hdlr, actv_pool, action_table,
 * action_table_size, actv_init, tsd_size, stack_size,
 * actvret_size, recvbuf_size, mem_buf_size, in actv_pure_attr
 * have the same value for all actvs in the same actv pool.
 *
 * so they should be set before creating actv and can be used
 * when actv is doing self initialization.
 */
struct actv_pure_attr {
	/*
	 * The following four fileds have different value for
	 * different actv in one actvpool.
	 *
	 * The addrs of actv_pure and actv_local are computed
	 * and wrote after alloc mem buf.
	 * The base address of shadow stack is alloced from
	 * mem slot, and the top address of shadow stack is
	 * computed based on the sstack_base.
	 * They are read and saved in called-saved regs at the
	 * beginning of actv_pure related rpc entry.
	 */
	unsigned long actv_pure;	/* arm32: offset = 0; aarch64: offset = 0 */
	unsigned long actv_local;	/* arm32: offset = 4; aarch64: offset = 8 */
	void *sstack_base;		/* arm32: offset = 8(the field cannot be used); aarch64: offset = 16 */
	void *sstack;			/* arm32: offset = 12(the field cannot be used); aarch64: offset = 24 */

	/*
	 * The following fields have the same value for all actv in one actvpool.
	 *
	 * the addr of self_init_hdlr, by which the recv buf, stack
	 * will be self initialized.
	 *
	 * self_init_hdlr must be the first field in actv_pure_attr.
	 * arm32: offset = 16; aarch64: offset = 32
	 */
	long (*self_init_hdlr)(unsigned long, unsigned long);

	/*
	 * global val used for block the actv which needs self initialization
	 * if there has another actv's page fault is handled.
	 *
	 * it must be the third field in actv_pure_attr.
	 * arm32: offset = 20; aarch64: offset = 40
	 */
	int g_futex_val;

	/*
	 * The following four fileds, which is related to assit info that
	 * used for self allocating actv memory, have the same value for
	 * different actv in one actvpool.
	 *
	 * The addrs of assist_actv_pure and assist_actv_local are computed
	 * and wrote after alloc assist membuf.
	 * The assist_sstack is computed based on the assist_sstack_base.
	 * They are read and saved in called-saved regs at the
	 * beginning of actv_pure related rpc entry.
	 */
	unsigned long assist_actv_pure;	/* arm32: offset = 24; aarch64: offset = 48 */
	unsigned long assist_actv_local; /* arm32: offset = 28; aarch64: offset = 56 */
	void *assist_sstack_base;	/* arm32: offset = 32(the field cannot be used); aarch64: offset = 64 */
	void *assist_sstack;		/* arm32: offset = 36(the field cannot be used); aarch64: offset = 72 */

	/*
	 * The following field has the same value for all actv in one actvpool.
	 *
	 * the addr of actv_pure_comb_kobj, by which the tls and actv_cref
	 * of assist membuf will be set.
	 *
	 * arm32: offset = 40; aarch64: offset = 80
	 */
	void (*comb_kobj_hdlr)(unsigned long);

	unsigned long tsd_size;		/* size of thread specific data */
	unsigned long stack_size;	/* size of stack buf */
	unsigned long actvret_size;	/* size of ret buf, which is a part of stack buf */
	unsigned long recvbuf_size;	/* size of recv buf */
	unsigned long membuf_size;	/* size of the whole vitual memory used by this actv */

	int (*actv_init)(void);		/* local init function if need */
	struct raw_thread *link_thread;	/* raw thread used for link the new thread */
	struct raw_workqueue *trig_wq;	/* raw workqueue used for free memm area slot */
	void *actv_kobj_pool;		/* addr of actv_kobjpool, used for alloc pmem for actv */
	unsigned int resv_actv_idpool;	/* used for alloc resv actv id */
	void *action_table;
	unsigned long action_table_size;
	cref_t actv_pool;

	/*
	 * The mem alloc/free functions are used when alloc/free membuf and sstack.
	 * Only special scenes such as the devhost ldk scene use interfaces provided
	 * by themselves to alloc/free memory.
	 */
	mem_alloc_func_t mem_alloc;
	mem_free_func_t mem_free;
};

/*
 * allow protected vspace to specify accessible range,
 * after specifying the accessible range, access to other region will be rejected.
 * *Note* unprotected vspace does not benefit from specify accessible mem.
 */
static inline void hm_actv_set_accessible_mem_range(unsigned long base, unsigned long len)
{
	int err;
	unsigned long ipc_rw_range[2] = {(unsigned long)base, len};
	err = sysfast_miscdata_writev(ipc_rw_range, sizeof(ipc_rw_range), __MISCDATA_ACTV_IPC_RW_BASE, __MISCACCESS_SELF);
	BUG_ON(err != E_HM_OK);
}

static inline void hm_actv_set_accessible_mem_merge_2_range(unsigned long base1, unsigned long len1,
							    unsigned long base2, unsigned long len2)
{
	int err;
	unsigned long base = (base1 < base2) ? base1 : base2;
	unsigned long len = ((base1 + len1) > (base2 + len2)) ? base1 + len1 - base : base2 + len2 - base;
	unsigned long ipc_rw_range[2] = {(unsigned long)base, len};
	err = sysfast_miscdata_writev(ipc_rw_range, sizeof(ipc_rw_range), __MISCDATA_ACTV_IPC_RW_BASE, __MISCACCESS_SELF);
	BUG_ON(err != E_HM_OK);
}

static inline void hm_actv_clear_accessible_mem_range(void)
{
	int err;
	unsigned long ipc_rw_range[2] = {0};
	err = sysfast_miscdata_writev(ipc_rw_range, sizeof(ipc_rw_range), __MISCDATA_ACTV_IPC_RW_BASE, __MISCACCESS_SELF);
	BUG_ON(err != E_HM_OK);
}

int hm_actv_pure_create_ex(struct actv_pure_attr *attr, rpc_entry_t entry);

/* used for creating actv with stack init when been invoked for the first time */
static inline int
hm_actv_pure_create(struct actv_pure_attr *attr)
{
	return hm_actv_pure_create_ex(attr, arch_actv_pure_wait_rpc_entry);
}

/* used for creating actv with stack init before been invoked for the first time */
static inline int
hm_actv_pure_create_pre_init(struct actv_pure_attr *attr)
{
	return hm_actv_pure_create_ex(attr, arch_actv_pure_pre_init_rpc_entry);
}

int hm_actv_pure_destroy(cref_t actv_cref, unsigned long actv_pure,
			 struct actv_pure_attr *attr, bool is_inited);

/* used for actv self initialization */
long actv_pure_self_init_hdlr(unsigned long init_attr, unsigned long rpcinfo);

/* internal api which is only used in actv_pure.c, actv_setup.c and actv_assist.c */
void *actv_pure_mmap_membuf(size_t membuf_size);
void actv_pure_stack_init(struct actv_pure *actv_pure, struct actv_pure_attr *attr,
			  void *recv_buf, unsigned int actv_id, unsigned long sstack_base);
void actv_pure_stack_comb_kobj(unsigned long actv_pure);
void actv_pure_insert_thread_pool(struct actv_pure *actv_pure, struct actv_pure_attr *attr);
unsigned long mem_buf_to_actv_pure(const void *mem_buf, const struct actv_pure_attr *attr);
unsigned long actv_pure_to_actv_local(unsigned long actv_pure, const struct actv_pure_attr *attr);
void *actv_pure_to_recv_buf(unsigned long actv_pure, const struct actv_pure_attr *attr);
void *actv_pure_to_mem_buf(unsigned long actv_pure, const struct actv_pure_attr *attr);

/* used for initing assist mem, including membuf and sstack */
int hm_actv_pure_assist_mem_init(struct actv_pure_attr *attr);

int hm_actv_pool_bind_server(cref_t ap_cref, unsigned long grant_info,
			     const char *buf_pool_name,
			     struct actv_bind_ret_rref *ret_rref);
void hm_actv_pool_bind_clean(const char *buf_pool_name,
			     const struct actv_bind_ret_rref *rec);
void hm_actv_pool_bind_sysmgr_clean(const struct actv_bind_ret_rref *rec);

void *hm_actv_current_actvret(void);
void *hm_actv_current_stack_args(void);
struct arch_actv_local *hm_actv_current_arch_actv_local(void);

int hm_actv_zero_xact_vm(void *dst, unsigned long size);
int hm_actv_read_xact_vm(void *dst, const void *src, unsigned long size);
int hm_actv_write_xact_vm(void *dst, const void *src, unsigned long size);
int hm_actv_copy_in_xact_vm(void *dst, const void *src, unsigned long size);
int hm_actv_read_caller_vm(void *dst, const void *src, unsigned long size);
int hm_actv_write_caller_vm(void *dst, const void *src, unsigned long size);
int hm_actv_strcpy_xact_vm(void *dst, const void *src, unsigned long size);
int hm_actv_strcpy_caller_vm(void *dst, const void *src, unsigned long size);

int hm_actv_pool_read_caller_vm(unsigned long cred,
				void *dst, const void *src, unsigned long size);


int hm_actv_pool_bind_sysmgr(cref_t ap_cref, unsigned long grant_info,
			     struct actv_bind_ret_rref *ret_rref);

int hm_actv_pool_set_cred(cref_t ap_cref, uref_t ap_uref,
			  unsigned int dst_cnode_idx,
			  unsigned long credential);

/* must be called inside a service activation */
__attribute__((returns_nonnull)) void *hm_actvpool_prepare_ret(unsigned long long sender,
			      unsigned long credential,
			      unsigned long size);

__attribute__((returns_nonnull)) void *hm_actvpool_ret(unsigned long long sender, unsigned long credential);

/*
 * for kcall trap and kcall hmsignal, set retval for broken system call
 * when handler decide override retval of the syscall.
 */
void hm_actvpool_set_kcall_retval(unsigned long long sender,
				  unsigned long credential,
				  unsigned long v);

/* prepare the ret buffer for passing the upcall rlambda args */
void *hm_actvpool_prepare_kcall_ret(unsigned long long sender,
				    unsigned long credential,
				    unsigned long size);

struct actvpool_recv {
	void *data;
	unsigned long size;
};

struct actvpool_recv
hm_actvpool_curr_recv_data(unsigned long long sender,
			   unsigned long credential);

#define ACTV_POOL_WATER_TRIGGER		1
#define ACTV_POOL_EXIT_TRIGGER		2

int hm_actv_pool_listen_water(cref_t pool_cref);

int hm_actv_pool_check_water(cref_t pool_cref,
			     struct __actv_pool_water_trigger_info *info);

int hm_actv_pool_set_water(cref_t pool_cref,
			   struct __actv_pool_water *water);

int hm_trigger_wait(unsigned long flags, struct __hmsigmask *sigmask);
extern void actv_threadpool_init(void);
cref_t hm_actv_id2cref(unsigned int actv_id);

int hm_actv_load_stat_ex(struct sysarg_actv_stat *stat,
			 struct __arch_fpsimd_state *fpsimd,
			 cref_t *actv_chain_iter,
			 const cref_t *thread_cref,
			 unsigned long flags);

static inline int hm_actv_load_stat(struct sysarg_actv_stat *stat,
				    cref_t *actv_chain_iter,
				    const cref_t *thread_cref,
				    unsigned long flags)
{
	return hm_actv_load_stat_ex(stat, NULL, actv_chain_iter,
				    thread_cref, flags);
}

struct hm_actv_buf {
	uint64_t	buf;
	uint64_t	size;
};

static inline void hm_actv_buf_init(struct hm_actv_buf *dst,
				    const void *ptr, size_t size)
{
	BUG_ON(ptr == NULL && size != 0);

	dst->buf = (uint64_t)(uintptr_t)ptr;
	dst->size = (uint64_t)size;
};

int hm_actv_read_caller_buf(void *dst, unsigned long dst_size,
			    const struct hm_actv_buf *src);

struct actv_emrg_attr {
	int futex_val;
};

int hm_actv_pool_emrg_init_ex(cref_t actv_pool,
			      struct actv_emrg_attr *attr,
			      rpc_entry_t entry);

static inline int
hm_actv_pool_emrg_init(cref_t actv_pool,
		       struct actv_emrg_attr *attr)
{
	return hm_actv_pool_emrg_init_ex(actv_pool, attr, arch_actv_emrg_entry);
}

int hm_actv_pure_trig_workqueue_init(struct raw_workqueue *trig_wq,
				     const char *name);

int hm_actv_pool_set_schedinfo(cref_t actv_pool,
			       const struct __actvpool_sched_info_s *schedinfo,
			       unsigned int flags);
void hm_actv_fpu_begin(void);
void hm_actv_fpu_end(void);

void hm_actv_current_trigger_detach(void);

#define ACTVPOOL_NAME_LENGTH_MAX	32U
typedef void (*actvpool_bind_func_t)(cref_t ap);
typedef int (*actv_init_func_t)(void);

struct actvpool_private {
	/* actv/emrg attr addr */
	struct actv_pure_attr *p_actv_attr;	/* the addr of actv pure attr */
	struct actv_emrg_attr *p_emrg_attr;	/* the addr of actv emrg attr */

	/* configs for setup actvpool */
	bool use_actv_local_area_k;		/* if use actv local area to sync kernel info when setup actvpool */
	bool path_grant2sysmgr;			/* if grant the path to sysmgr */
	bool path_add_with_mode;		/* if add path with mode */
	bool path_set_grant_info;		/* if use the path_grant_info to replace the default value when add path */
	mode_t path_mode;			/* arg for hm_path_add_with_mode */
	unsigned long path_grant_info;		/* arg for hm_path_add or hm_path_add_with_mode */
	actvpool_bind_func_t bind_func;		/* specified bind func to bind server, if not set,
						 * bind to sysmgr buf ap by default */

	/* configs for setup actvs */
	unsigned int resv_actv_idpool;		/* used for alloc resv actv id */
	actv_init_func_t actv_init;		/* local init function if need */
	mem_alloc_func_t mem_alloc;		/* used for alloc membuf or sstack */
	mem_free_func_t mem_free;		/* used for free membuf or sstack */
	bool use_pre_init_entry;		/* if use pre_init_rpc_entry when create pure actvs */

	/* configs for setup elastic */
	bool main_thread_elastic;		/* if the main thread is elastic thread */
	unsigned int add_water;			/* actv pool add water level */
	unsigned int del_water;			/* actv pool delete water level */
	char elastic_prefix[ACTVPOOL_NAME_LENGTH_MAX];	/* the name prefix for trigger thread and workqueue, such as 'sysmgr' */
	struct __actvpool_sched_info_s *sched_info;  /* sched_info of actvpool such as affinity etc. */
	unsigned int flags;

	/* configs for setup workqueue */
	struct raw_workqueue *share_trig_wq;
	char share_trig_name[ACTVPOOL_NAME_LENGTH_MAX];
};

/*
 * The actvpool configs include two parts: common configs of all actvpools and
 * private configs of each actvpool. The common configs should be set by user
 * when the struct alloced by user, the private configs should be set through
 * the following apis as required.
 */
struct actvpool_configs {
	/* configs for setup actvpool */
	char ap_name[ACTVPOOL_NAME_LENGTH_MAX];	/* if the ap_name[0] == '\0', use default ap name */
	char path_name[ACTVPOOL_NAME_LENGTH_MAX]; /* if the path_name[0] == '\0', could not add or grant path;
						   * if the path_name[0] != '\0', add path to pathpool by default */

	/* configs for setup actvs */
	unsigned int actv_init_num;		/* the num of actvs which is created when init */
	unsigned int actv_max_num;		/* the max allowed num of the actvpool */
	unsigned long tsd_size;			/* size of thread specific data */
	unsigned long stack_size;		/* size of stack buf */
	unsigned long actvret_size;		/* size of ret buf, which is a part of stack buf */
	unsigned long recvbuf_size;		/* size of recv buf */
	void *action_table;
	unsigned long action_table_size;

	/* actvpool private configs */
	struct actvpool_private *p;		/* actvpool private variables, should be set by interfaces */
};

/*
 * The following apis are used for setup and clear actvpool private configs.
 */
/* alloc and init the actvpool private variables */
int hm_apconfig_private_init(struct actvpool_configs *configs);
/* free the mem alloced in init stage */
void hm_apconfig_private_destroy(struct actvpool_configs *configs);
/*
 * DEPRECATED: Now the actv local area is enabled the first time using it.
 *
 * Enable actv local area kernel when new actvpool.
 */
int hm_apconfig_private_enable_alak(const struct actvpool_configs *configs);
/* enable use pre_init rpc entry when create actvs initially */
int hm_apconfig_private_enable_preinitentry(const struct actvpool_configs *configs);
/* enable alloc the memory for passing the upcall rlambda args */
int hm_apconfig_private_enable_kcall_ret(struct actvpool_configs *configs);
/* set emrg actv attr nocow flag when fork */
int hm_apconfig_private_set_emrgnocow(const struct actvpool_configs *configs);
/* set actv init function */
int hm_apconfig_private_set_actvinitfunc(const struct actvpool_configs *configs,
					 actv_init_func_t func);
/*
 * set mem alloc/free function which used for allocing/freeing membuf and sstack,
 * only special scenes such as the devhost ldk scene use interfaces provided by
 * themselves to alloc/free memory.
 */
int hm_apconfig_private_set_memfunc(const struct actvpool_configs *configs,
				    mem_alloc_func_t mem_alloc,
				    mem_free_func_t mem_free);
/* set the resv id for actv pool which is used for computing ids of its actvs. if not set, default value is 0 */
int hm_apconfig_private_set_poolid(const struct actvpool_configs *configs,
				   unsigned int resv_actv_idpool);
/* set actvpool bind function to replace the hm_actv_pool_bind_sysmgr */
int hm_apconfig_private_set_bindfunc(const struct actvpool_configs *configs,
				     actvpool_bind_func_t func);
/*
 * This interface enables the path add function, the following is the meaning of parameters.
 * configs:	the actvpool configs to be set;
 * mode:	if the value is NULL, add path with default mode,
 *		if the value is not NULL, add path with the value which it points to;
 * grant_info:	if the value is NULL, add path with default grant info;
 *		if the value is not NULL, add path with the value which it points to;
 * grant2sysmgr: if grant the actvpool to sysmgr.
 */
int hm_apconfig_private_set_pathparam(const struct actvpool_configs *configs,
				      const mode_t *mode, const unsigned long *grant_info,
				      bool grant2sysmgr);
/*
 * This interface set the configs about elastic, the following is the meaning of parameters.
 * configs:	the actvpool configs to be set;
 * main_thread_elastic: if the main thread as the elastic thread, or create a new thread;
 * add_water: the value trigger elastic add actvs;
 * del_water: the value trigger elastic del actvs;
 * elastic_prefix: the prefix of trigger workqueue and thread.
 */
int hm_apconfig_private_set_elasticparam(const struct actvpool_configs *configs,
					 bool main_thread_elastic,
					 unsigned int add_water,
					 unsigned int del_water,
					 const char *elastic_prefix);

/*
 * This interface set the config of sched_info for actv_pool
 * the following is the meaning of parameters.
 * configs:	the actvpool configs to be set;
 * sched_info: the affinity of actv_pool;
 * flags: flags to setup actv_pool
 */
int hm_apconfig_private_set_schedinfo(struct actvpool_configs *configs,
				      const struct __actvpool_sched_info_s *sched_info,
				      unsigned int flags);

/*
 * This interface set the config about workqueue
 *
 * configs:	the actvpool configs to be set;
 * trig_wq:	to setup trig_wq
 * workqueue_prefix: the prefix of trigger workqueue.
 */
int hm_apconfig_private_set_workqueue(struct actvpool_configs *configs,
				      struct raw_workqueue *trig_wq,
				      const char *workqueue_prefix);

/*
 * The interfaces of setup actvpool do the following work inside based on configs:
 * (1) create a new actvpool;
 * (2) actvpool bind sysmgr (the sysmgr_buf_ap is an exception, it bind the sysmgr_ex_ap);
 * (3) actvpool emrg init;
 * (4) alloc and init actv attr, set it to configs->p_actv_attr;
 * (5) actvpool create actvs initially with the num "actv_init_num";
 * (6) add/grant actvpool path as required.
 */
int hm_setup_actvpool(struct actvpool_configs *configs, cref_t *actvpool);
/*
 * The interfaces of setup actvpool elastic thread do the following work inside based on configs:
 * 1. main thread as trigger thread:
 * (1) trigger workqueue init;
 * (2) set actvpool trigger water;
 * (3) set actvpool listen water;
 * (4) set actvpool thread priority;
 * (5) wait trigger event.
 * 2. create new thread as trigger thread:
 * (1) trigger workqueue init;
 * (2) set actvpool trigger water.
 * (3) create new trigger thread (set listen water -> set thread name -> wait trigger event);
 *
 * The actvpool elastic setup relys on actvpool setup, so the interfaces of setup actvpool and
 * setup elastic should be invoked sequentially. For example:
 *
 *        static struct actvpool_configs ap_configs = {
 *		.actv_init_num = x;
 *		...
 *        };
 *                         |
 *        set other actvpool configs, such as:
 *        hm_actvpool_config_init(&ap_configs, name);
 *        hm_actvpool_config_set_elastic(&ap_configs, true, false, "xxx");
 *                         |
 *        hm_setup_actvpool(&ap_configs);
 *                         |
 *        hm_actvpool_setup_elastic(&ap_configs);
 *
 * Currently, there is no unified interface for reclaiming the resources alloced in hm_setup_actvpool.
 * If setup elastic failed, to avoid resource leakage, the process must be terminated immediately,
 * sysmgr can reclaim the resources.
 */
int hm_actvpool_setup_elastic(const struct actvpool_configs *configs);

void hm_actv_set_pac(cref_t thread_cref);
int hm_actv_pac_update(cref_t actv_cref);
int hm_actv_pool_pac_update(cref_t actv_pool);

#endif
