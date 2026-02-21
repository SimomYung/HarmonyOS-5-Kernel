/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Futex function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 02 16:32:46 2019
 */
#include <sys/time.h>
#include <stdarg.h>
#include <errno.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libhmactv/actv_id.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>
#include <hmkernel/rt_futex.h>
#include <libhmtrace_ng/hmtrace.h>
#include <libhmelf/vsymbol.h>

struct wait_args_s {
	volatile int *uaddr;
	int val;
	const struct timespec *ts;
	unsigned long bitset;
	unsigned int flags;
};

static int __futex_wait_ex(volatile int *uaddr, int val,
			   unsigned long long *ptimeout, struct __hmsigmask *mask,
			   unsigned int flags, unsigned long bitset,
			   unsigned long bitset2, unsigned long data,
			   struct __block_param_s *block_param);

#ifdef CONFIG_USER_FUTEX

int __private_futex_wait(volatile int *uaddr, int val,
			 unsigned long long *ptimeout, struct __hmsigmask *mask,
			 unsigned int flags, unsigned long bitset,
			 struct __block_param_s *block_param);

int __private_futex_wake(volatile int *uaddr, unsigned int num,
							unsigned int flags,
							unsigned long bitset, unsigned long bitset2);

int __private_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
							unsigned int deq_num, unsigned int req_num,
							unsigned int flags);

int __private_futex_wake_op(volatile int *uaddr1, volatile int *uaddr2,
		     unsigned int nr_wake1, unsigned int nr_wake2,
		     int need_wake2, unsigned int flags);

#endif /* CONFIG_USER_FUTEX */

static inline unsigned int futex_type_of_flags(unsigned int flags)
{
	unsigned int type = flags & FUTEX_TYPE_MASK;
	BUG_ON(type != FUTEX_TYPE_SHARED &&  type != FUTEX_TYPE_PRIVATE);
	return type;
}

static int __futex_wait(struct wait_args_s *wait_args,
			struct __hmsigmask *mask)
{
	struct thread_futex_wait_args args = {0};
	__u64 timeout = (__u64)-1;
	int ret;

	/* flags only contains shared vs private info */
	unsigned int type_flags = futex_type_of_flags(wait_args->flags);

	ret = timespec_to_values(wait_args->ts, &timeout);
	if (ret != E_HM_OK) {
		hm_debug("time transfre failed\n");
		__hmattr_sideff_resolved("no relative side effect");
		return ret;
	}

	if (type_flags == FUTEX_TYPE_SHARED) {
		args.uaddr   = (__u64)(uintptr_t)(wait_args->uaddr);
		args.val     = (__s32)wait_args->val;
		args.bitset  = (__u64)wait_args->bitset;
		args.timeout = (__u64)timeout;
		args.sigmask = (__u64)(uintptr_t)(mask);
		ret = actvcall_hmcall_thread_futex_wait(args);
		if (ret == E_HM_FUTEXTYPE) {
			wait_args->flags ^= FUTEX_TYPE_MASK; /* this will both flip the shared flag and the private flag bit */
			type_flags = FUTEX_TYPE_PRIVATE;
		}
	}

	if (type_flags == FUTEX_TYPE_PRIVATE) {
#ifdef CONFIG_USER_FUTEX
		ret = __private_futex_wait(wait_args->uaddr, wait_args->val,
					   &timeout, mask, wait_args->flags, wait_args->bitset, NULL);
#else
		ret = sysfast_futex_wait(wait_args->uaddr, wait_args->val,
					 &timeout, mask, wait_args->flags, wait_args->bitset);
#endif /* CONFIG_USER_FUTEX */
	}

	/*
	 * The cancel_handler of SIGCANCEL sent by pthread_cancel may
	 * change the return code of sysfast_futex_wait to -ECANCELED.
	 * Here becomes a cancellation point after signal unblocked.
	 */
	if (unlikely(ret == -ECANCELED)) {
		ret = E_HM_CANCELED;
	}
	return (ret == E_HM_HMSIGINTR) ? E_HM_INTR : ret;
}

int hm_futex_wait(volatile int *uaddr, int val,
		  const struct timespec *ts, unsigned int flags)
{
	struct __hmsigmask hmsig_mask = {(__u64)-1};
	struct wait_args_s args;

	args.uaddr  = uaddr;
	args.val    = val;
	args.ts     = ts;
	args.flags  = flags;
	args.bitset = FUTEX_BITSET_MATCH_ANY;

	return __futex_wait(&args, &hmsig_mask);
}

int hm_futex_wait_bitset(volatile int *uaddr, int val,
			 const struct timespec *ts, unsigned int flags, unsigned long bitset)
{
	struct __hmsigmask hmsig_mask = {(__u64)-1};
	struct wait_args_s args;

	args.uaddr  = uaddr;
	args.val    = val;
	args.ts     = ts;
	args.flags  = flags;
	args.bitset = bitset;

	return __futex_wait(&args, &hmsig_mask);
}

int hm_futex_timedwait_ex(volatile int *uaddr, int val, unsigned long long *timeout,
			  struct __hmsigmask *mask, unsigned int flags,
			  unsigned long bitset, unsigned long bitset2, unsigned long data,
			  struct __block_param_s *block_param)
{
	int ret = E_HM_OK;
	struct thread_futex_wait_args args = {0};

	unsigned int type_flags = futex_type_of_flags(flags);
	if (type_flags == FUTEX_TYPE_SHARED) {
		args.uaddr   = (__u64)(uintptr_t)uaddr;
		args.val     = (__s32)val;
		args.timeout = (timeout != NULL) ? (__u64)(*timeout) : (__u64)-1;
		args.sigmask = (__u64)(uintptr_t)(mask);
		args.bitset  = (__u64)FUTEX_BITSET_MATCH_ANY;
		ret = actvcall_hmcall_thread_futex_wait(args);
		if (ret == E_HM_FUTEXTYPE) {
			flags ^= FUTEX_TYPE_MASK; /* this will both flip the shared flag and the private flag bit */
			type_flags = FUTEX_TYPE_PRIVATE;
		}
	}

	if (type_flags == FUTEX_TYPE_PRIVATE) {
#ifdef CONFIG_USER_FUTEX
		if (bitset2 & FUTEX_BITSET2_PIP_MASK) {
			ret = __futex_wait_ex(uaddr, val, timeout, mask, flags, bitset, bitset2, data, block_param);
		} else {
			ret = __private_futex_wait(uaddr, val, timeout, mask, flags, bitset, block_param);
		}
#else
		ret = __futex_wait_ex(uaddr, val, timeout, mask, flags, bitset, bitset2, data, block_param);
#endif /* CONFIG_USER_FUTEX */
	}

	return ret;
}

int hm_futex_timedwait(volatile int *uaddr, int val, unsigned long long *timeout,
		       struct __hmsigmask *mask, unsigned int flags)
{
	return hm_futex_timedwait_ex(uaddr, val, timeout, mask, flags, FUTEX_BITSET_MATCH_ANY, 0x00UL, 0x00UL, NULL);
}

int hm_futex_wait_interruptible(volatile int *uaddr, int val,
				const struct timespec *ts, unsigned int flags)
{
	struct wait_args_s args;

	args.uaddr  = uaddr;
	args.val    = val;
	args.ts     = ts;
	args.flags  = flags;
	args.bitset = FUTEX_BITSET_MATCH_ANY;

	return __futex_wait(&args, NULL);
}

int hm_futex_wake(volatile int *uaddr, unsigned int num, unsigned int flags)
{
	return hm_futex_wake_bitset(uaddr, num, flags, FUTEX_BITSET_MATCH_ANY);
}

int hm_futex_wake_ex(volatile int *uaddr, unsigned int num, unsigned int flags,
		     unsigned long bitset, unsigned long bitset2)
{
	return hm_futex_wake_bitset_ex(uaddr, num, flags, bitset, bitset2);
}

int hm_futex_wake_bitset_ex(volatile int *uaddr, unsigned int num, unsigned int flags,
					unsigned long bitset, unsigned long bitset2)
{
	struct thread_futex_wake_args args = {0};
	int ret = E_HM_OK;

	if (bitset == 0UL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	unsigned int type_flags = futex_type_of_flags(flags);
	if (type_flags == FUTEX_TYPE_SHARED) {
		args.uaddr  = (__u64)(uintptr_t)uaddr;
		args.num    = (__u32)num;
		args.bitset = (__u64)bitset;
		args.bitset2 = (__u64)bitset2;
		ret = actvcall_hmcall_thread_futex_wake(args);
		if (ret == E_HM_FUTEXTYPE) {
			/* flags contains more than just shared and private info */
			flags ^= FUTEX_TYPE_MASK; /* this will both flip the shared flag and the private flag bit */
			type_flags = FUTEX_TYPE_PRIVATE;
		}
	}

	if (type_flags == FUTEX_TYPE_PRIVATE) {
#ifdef CONFIG_USER_FUTEX
		if (bitset2 & FUTEX_BITSET2_PIP_MASK) {
			ret = sysfast_futex_wake_ex(uaddr, num, flags, bitset, bitset2);
		} else {
			ret = __private_futex_wake(uaddr, num, flags, bitset, bitset2);
		}
#else
		ret = sysfast_futex_wake_ex(uaddr, num, flags, bitset, bitset2);
#endif /* CONFIG_USER_FUTEX */
	}

	return ret;
}

int hm_futex_wake_bitset(volatile int *uaddr, unsigned int num, unsigned int flags, unsigned long bitset)
{
	return hm_futex_wake_bitset_ex(uaddr, num, flags, bitset, 0x00UL);
}

int hm_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
		     unsigned int deq_num, unsigned int req_num, unsigned int flags)
{
	struct thread_futex_requeue_args args = {0};
	int ret = E_HM_OK;

	/* user_flags only contains shared vs private info */
	unsigned int type_flags = futex_type_of_flags(flags);
	if (type_flags == FUTEX_TYPE_SHARED) {
		args.src_uaddr = (__u64)(uintptr_t)src_ptr;
		args.dst_uaddr = (__u64)(uintptr_t)dst_ptr;
		args.deq_num   = (__u32)deq_num;
		args.req_num   = (__u32)req_num;
		ret = actvcall_hmcall_thread_futex_requeue(args);
		if (ret == E_HM_FUTEXTYPE) {
			/* flags contains more than just shared and private info */
			flags ^= FUTEX_TYPE_MASK; /* this will both flip the shared flag and the private flag bit */
			type_flags = FUTEX_TYPE_PRIVATE;
		}
	}

	if (type_flags == FUTEX_TYPE_PRIVATE) {
#ifdef CONFIG_USER_FUTEX
		ret = __private_futex_requeue(src_ptr, dst_ptr, deq_num, req_num, flags);
#else
		ret = sysfast_futex_requeue(src_ptr, dst_ptr, deq_num, req_num);
#endif
	}

	return ret;
}

#ifdef CONFIG_RT_FUTEX

static int hm_rtfutex_shared(volatile int *uaddr,
			  __u64 timeout, __u32 owner_tid)
{
	struct thread_futex_wait_args args = {0};

	args.uaddr	= (__u64)(uintptr_t)uaddr;
	args.val	= (int)owner_tid;
	args.flags.is_rtfutex = 1;
	args.timeout	= (__u64)timeout;
	return actvcall_hmcall_thread_futex_wait(args);
}

int hm_futex_wait_pi(volatile int *uaddr, unsigned int flags,
		     const struct timespec *ts)
{
	__u64 timeout = (__u64)(-1);
	__u32 owner_tid;
	__u32 caller_tid = (__u32)raw_thread_tid_of(raw_thread_self());
	cref_t owner_cref;
	unsigned int retry = 0;
	int val;
	int ret;

	ret = timespec_to_values(ts, &timeout);
	if (ret != E_HM_OK) {
		hm_warn("time transfer failed\n");
		__hmattr_sideff_resolved("no relative side effect");
		return ret;
	}

	do {
		val = raw_atomic_int_cas((raw_atomic_int_t *)(uintptr_t)uaddr, 0, (int)caller_tid);
		if (val == 0) {
			ret = E_HM_OK;
			break;
		}
		owner_tid = (__u32)val & __U(RT_FUTEX_ID_MASK);

		if ((flags&FUTEX_TYPE_MASK) == FUTEX_TYPE_SHARED) {
			ret = hm_rtfutex_shared(uaddr, timeout, owner_tid);
		} else {
			if (owner_tid < actv_id_min()) {
				owner_cref = hm_thread_tid2cref((int)owner_tid);
				flags &= (unsigned int)~RT_FUTEX_OWNER_IS_ACTV;
			} else {
				owner_cref = hm_actv_id2cref(owner_tid);
				flags |= (unsigned int)RT_FUTEX_OWNER_IS_ACTV;
			}
			if (unlikely(IS_REF_ERR(owner_cref))) {
				/* maybe the owner has already exited or been destroyed,
				* just try again */
				ret = E_HM_NOOBJ;
			} else {
				ret = sysfast_rt_futex_wait(uaddr, (unsigned long)flags,
								&timeout, &owner_cref,
								(unsigned long)owner_tid,
								(unsigned long)caller_tid);
			}
		}
		if (ret == E_HM_NOOBJ) {
			retry++;
			ret = E_HM_AGAIN;
			if (retry > 1024U) {
				hm_warn("bad cref, owner_tid=%u uaddr %d\n", owner_tid, *uaddr);
				ret = E_HM_INVAL;
				break;
			}
		}
	} while (ret == E_HM_AGAIN);

	return ret;
}

int hm_futex_wake_pi(volatile const int *uaddr, unsigned int flags)
{
	struct thread_futex_wake_args args = {0};
	unsigned int type = (flags&FUTEX_TYPE_MASK);

	if (type == FUTEX_TYPE_SHARED) {
		args.uaddr = (__u64)(uintptr_t)uaddr;
		args.flags.is_rtfutex = 1;
		return actvcall_hmcall_thread_futex_wake(args);
	} else {
		return sysfast_rt_futex_wake(uaddr, (unsigned long)flags);
	}
}

#endif

int hm_set_robust_list(const void *list_head, unsigned long size)
{
	int ret;

	ret = (int)lsyscall_thread_set_robust_list(ptr_to_ulong(list_head), size);
	if (ret != E_HM_OK) {
		hm_debug("actvcall set robust list failed, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

static int __futex_wait_ex(volatile int *uaddr, int val,
			   unsigned long long *ptimeout, struct __hmsigmask *mask,
			   unsigned int flags, unsigned long bitset,
			   unsigned long bitset2, unsigned long data,
			   struct __block_param_s *block_param)
{
	int ret;

	/* In the lock scenario, the value of ptimeout is NULL */
	if ((ptimeout == NULL) && (block_param != NULL)) {
		ret = sysfast_lock_wait(uaddr, val, mask, flags, bitset, bitset2, data, block_param);
	} else {
		ret = sysfast_futex_wait_ex(uaddr, val, ptimeout, mask, flags, bitset, bitset2, data);
	}
	return ret;
}

#ifdef CONFIG_TRACE_BLOCK_INFO
void hm_set_block_param(struct __block_param_s *block_param, unsigned long pc)
{
	if (hm_trace_is_tracing_block_enabled()) {
		struct vsymbol_info info = {0};

		int err = hm_elf_lookup_vsymbol_by_addr(pc, &info);
		if (err == E_HM_OK) {
			block_param->fname = info.fname;
			block_param->sname = info.sname;
			block_param->fname_size = strnlen(info.fname, SYM_MOD_NAME_SIZE - 1U);
			block_param->sname_size = strnlen(info.sname, SYM_FUNC_NAME_SIZE - 1U);
			block_param->offset = info.offset;
			block_param->size = info.size;
		} else {
			hm_debug("hm_elf_lookup_vsymbol_by_addr, err=%s\n", hmstrerror(err));
		}
	}
}

void hm_set_block_info(struct __block_info_s *block_info, unsigned long pc)
{
	if (hm_trace_is_tracing_block_enabled()) {
		struct vsymbol_info info = {0};

		int err = hm_elf_lookup_vsymbol_by_addr(pc, &info);
		if (err == E_HM_OK) {
			NOFAIL(strncpy_s(block_info->fname, SYM_MOD_NAME_SIZE, info.fname, SYM_MOD_NAME_SIZE - 1U));
			NOFAIL(strncpy_s(block_info->sname, SYM_FUNC_NAME_SIZE, info.sname, SYM_FUNC_NAME_SIZE - 1U));
			block_info->offset = info.offset;
			block_info->size = info.size;
		} else {
			hm_debug("hm_elf_lookup_vsymbol_by_addr, err=%s\n", hmstrerror(err));
		}
	}
}
#else
void hm_set_block_param(struct __block_param_s *block_param, unsigned long pc)
{
	UNUSED(block_param, pc);
}
void hm_set_block_info(struct __block_info_s *block_info, unsigned long pc)
{
	UNUSED(block_info, pc);
}
#endif /* CONFIG_TRACE_BLOCK_INFO */

#ifdef CONFIG_USER_FUTEX

int __private_futex_wait(volatile int *uaddr, int val,
			 unsigned long long *ptimeout, struct __hmsigmask *mask,
			 unsigned int flags, unsigned long bitset,
			 struct __block_param_s *block_param)
{
	int ret;
	if (hm_user_futex_allowed(flags)) {
		ret = hm_user_futex_wait(uaddr, val, bitset, flags, mask, ptimeout);
	} else {
		ret = __futex_wait_ex(uaddr, val, ptimeout, mask,
				      flags & ~__FUTEX_FLAG_USER_MASK, bitset, 0x00UL, 0x00UL, block_param);
	}
	return ret;
}

int __private_futex_wake(volatile int *uaddr, unsigned int num,
							unsigned int flags,
							 unsigned long bitset, unsigned long bitset2)
{
	int ret;
	if (hm_user_futex_allowed(flags)) {
		ret = hm_user_futex_wake_ex(uaddr, num, bitset, bitset2, flags);
	} else {
		ret = sysfast_futex_wake_ex(uaddr, num, flags & ~__FUTEX_FLAG_USER_MASK, bitset, bitset2);
	}
	return ret;
}

int __private_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
							unsigned int deq_num, unsigned int req_num,
							unsigned int flags)
{
	int ret;
	if (hm_user_futex_allowed(flags)) {
		ret = hm_user_futex_requeue(src_ptr, dst_ptr, deq_num, req_num);
	} else {
		ret = sysfast_futex_requeue(src_ptr, dst_ptr, deq_num, req_num);
	}
	return ret;
}

int hm_thread_block(volatile int *uaddr, int val, unsigned long long *ptimeout, struct __hmsigmask *mask,
		    unsigned int flags, const struct __block_param_s *block_param)
{
	int rc = E_HM_OK;

	if (uaddr == NULL) {
		rc = E_HM_FAULT;
	}

	if (ptr_to_ulong(uaddr) % sizeof(int) != 0UL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = sysfast_block_thread((int*)uaddr, mask, flags, ptimeout, NULL, val, block_param);
	}

	return rc;
}

int hm_thread_unblock(unsigned long long tcb_cref, volatile int *uaddr, unsigned long flags)
{
	struct __ufutex_unblock_target_s wake_iov = {.waiter_cref = tcb_cref};
	int rc = E_HM_OK;

	rc = sysfast_unblock_threads(&wake_iov, 1, (int*)uaddr, flags);

	return rc;
}

#endif /* CONFIG_USER_FUTEX */
