/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Futex function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 07 18:05:09 2023
 */

#include <libalgo/rbtree.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <libhmactv/actv.h>
#include <libhmactv/actv_id.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libpreempt/preempt.h>
#include <libstrict/strict.h>
#include <libsysif/sysmgr/api.h>

#include <hongmeng/panic.h>
#include <hongmeng/syscall.h>

#include <vsync/spinlock.h>
#include <vsync/atomic.h>

#ifdef CONFIG_CACHELINE_SIZE_L1
#define UFUTEX_ALIGNED_SIZE CONFIG_CACHELINE_SIZE_L1
#else
#define UFUTEX_ALIGNED_SIZE 64
#endif

#ifdef CONFIG_USER_FUTEX

#define USER_FUTEX_HASH_TABLE_SIZE 83
struct ufutex_spinlock_s {
	struct spinlock_s slock;
};

struct user_futex_s {
	struct rbt_tree rbt_root;
	struct dlist_node dlist_head;
	struct ufutex_spinlock_s slock;
};

struct ufutex_hash_table_s {
	struct user_futex_s buckets[USER_FUTEX_HASH_TABLE_SIZE];
};

static bool user_futex_enabled = false;
static struct ufutex_hash_table_s *ufutex_hash_table;

struct waiter_info_s {
	struct rbt_node tree_node;
	struct dlist_node list_node;
	volatile int *uaddr;
	unsigned int priority;
	int val;
	cref_t cref;
	unsigned long bitset;
	bool should_block;
#ifdef CONFIG_USER_FUTEX_DEBUGGING
	bool on_tree;
#endif
	cref_t waker_cref;
	/*
	 * generation counter.
	 * monolitically incremented when the waiter
	 * is added or removed from the tree
	 */
	unsigned int gencnt;
} __aligned(UFUTEX_ALIGNED_SIZE);

#ifdef CONFIG_USER_FUTEX_DEBUGGING
#define ufutex_bugon(cond, uaddr, fmt, ...)		\
	if (unlikely((cond))) {				\
		hm_panic(fmt, __VA_ARGS__);		\
	}

#define ufutex_wait_status(waiter)	((unsigned int)waiter->on_tree << 1 | (unsigned int)waiter->should_block)
#else
#define ufutex_bugon(cond, uaddr, fmt, ...)
#define ufutex_wait_status(waiter)	((unsigned int)waiter->should_block)
#endif

/* per thread storage */
static __thread struct waiter_info_s waiter_self;

static inline void hm_ufutex_rbtree_init(struct user_futex_s *tree)
{
	dlist_init(&tree->dlist_head);
	spinlock_init(&tree->slock.slock);
	tree->rbt_root.root = NULL;
}

void hm_user_futex_init(bool replace_futex)
{
	BUG_ON(ufutex_hash_table != NULL, "User futex hash table already allocated!\n");
	ufutex_hash_table = (struct ufutex_hash_table_s *)malloc(sizeof(struct ufutex_hash_table_s));

	unsigned int i;
	for(i = 0; i < USER_FUTEX_HASH_TABLE_SIZE; i++) {
		hm_ufutex_rbtree_init(&ufutex_hash_table->buckets[i]);
	}
	/*
	 * If replace_futex is true, all the original hm futexes in this
	 * process are replaced by user futex in hm_futex.c
	 */
	user_futex_enabled = replace_futex;
}

static inline unsigned long ufutex_hash(const int *uaddr)
{
	/* We will be at least four byte aligned, so lets lose the last few bits */
	unsigned long hash = (ptr_to_ulong(uaddr) >> 2) % USER_FUTEX_HASH_TABLE_SIZE;
	return hash;
}

static struct user_futex_s* ufutex_tree(volatile int *uaddr)
{
	return &ufutex_hash_table->buckets[ufutex_hash((int *) uaddr)];
}

bool hm_user_futex_allowed(unsigned int flags)
{
	return ((user_futex_enabled) && ((flags & __FUTEX_FLAG_USER_NOT) == 0));
}

static inline void __ufutex_spinlock_acquire(spinlock_t *l)
{
	if (!spinlock_tryacquire(l)) {
		spinlock_acquire(l);
	}
}

static int spinlock_acquire_preempt(struct ufutex_spinlock_s *slock)
{
	int cpu = -1;
	/* in sysmgr, the cpu returned by hm_user_preempt_disale is always valid */
	cpu = hm_user_preempt_disable();
	__ufutex_spinlock_acquire(&slock->slock);

	return cpu;
}

static void spinlock_release_preempt(struct ufutex_spinlock_s *slock, int cpu)
{
	spinlock_release(&slock->slock);
	if (cpu >= 0) {
		hm_user_preempt_enable();
	}
}

static void init_waiter_info(struct waiter_info_s * waiter, volatile int *uaddr, int val, unsigned long bitset)
{
	ufutex_bugon((rbt_parent(&waiter->tree_node) != NULL) ||
			(ufutex_tree(waiter->uaddr)->rbt_root.root == &waiter->tree_node),
		waiter->uaddr,
		"waiter is still on tree? %llx,%u,%u\n",
		waiter->cref,
		(waiter->uaddr == uaddr),
		ufutex_wait_status(waiter));

	waiter->uaddr = uaddr;
	waiter->val = val;

	waiter->bitset = bitset;
	waiter->waker_cref = 0;

#ifdef CONFIG_USER_FUTEX_DEBUGGING
	waiter->on_tree = false;
#endif
}

static struct waiter_info_s *sync_waiter_self_ex(void)
{
	waiter_self.cref = hm_actv_local_sched_thread_cref_of();
	waiter_self.priority = hm_actv_local_thread_priority_of();
	return &waiter_self;
}

static int double_acquire_preempt(struct ufutex_spinlock_s *l1, struct ufutex_spinlock_s *l2)
{
	int cpu = -1;
	cpu = hm_user_preempt_disable();

	if (ptr_to_ulong(l1) == ptr_to_ulong(l2)) {
		__ufutex_spinlock_acquire(&l1->slock);
	} else if (ptr_to_ulong(l1) > ptr_to_ulong(l2)) {
		__ufutex_spinlock_acquire(&l1->slock);
		__ufutex_spinlock_acquire(&l2->slock);
	} else {
		__ufutex_spinlock_acquire(&l2->slock);
		__ufutex_spinlock_acquire(&l1->slock);
	}
	return cpu;
}

static void double_release_preempt(struct ufutex_spinlock_s *l1, struct ufutex_spinlock_s *l2, int cpu)
{
	if (ptr_to_ulong(l1) == ptr_to_ulong(l2)) {
		spinlock_release(&l1->slock);
	} else if (ptr_to_ulong(l1) > ptr_to_ulong(l2)) {
		spinlock_release(&l2->slock);
		spinlock_release(&l1->slock);
	} else {
		spinlock_release(&l1->slock);
		spinlock_release(&l2->slock);
	}
	if (cpu >= 0) {
		hm_user_preempt_enable();
	}
}

static void waiter_insert(struct waiter_info_s *waiter)
{
	struct user_futex_s *tree = ufutex_tree(waiter->uaddr);
	struct rbt_node *p = tree->rbt_root.root;
	struct rbt_node *parent = NULL;
	struct waiter_info_s *curr;
	bool is_rchild = false;
	while (p != NULL) {
		curr = rbt_entry(p, struct waiter_info_s, tree_node);
		ufutex_bugon(curr == waiter,
				waiter->uaddr,
				"-----error-----already in tree, %llx, %u, %u\n",
				waiter->cref,
				ufutex_wait_status(waiter),
				(unsigned int)(rbt_parent(&curr->tree_node) == parent));

		parent = p;

		if (curr->uaddr == waiter->uaddr) {
			/*
			* multiple waiters on same uaddr.
			* high priority waiter goes left, lower or same priority goes to right.
			*/
			if (curr->priority < waiter->priority) {
				is_rchild = false;
				p = p->rbt_left;
			} else {
				is_rchild = true;
				p = p->rbt_right;
			}
		} else if (curr->uaddr > waiter->uaddr) {
			is_rchild = false;
			p = p->rbt_left;
		} else {
			is_rchild = true;
			p = p->rbt_right;
		}
	}

	rbt_insert(&tree->rbt_root, &waiter->tree_node, parent, is_rchild);

	BUG_ON(&waiter->tree_node == waiter->tree_node.rbt_left);

	if (parent == NULL) {
		dlist_insert_head(&tree->dlist_head, &waiter->list_node);
	} else {
		struct waiter_info_s *parent_waiter = rbt_entry(parent, struct waiter_info_s, tree_node);
		if (is_rchild) {
			dlist_insert(&parent_waiter->list_node, &waiter->list_node);
		} else {
			dlist_insert_tail(&parent_waiter->list_node, &waiter->list_node);
		}
	}

#ifdef CONFIG_USER_FUTEX_DEBUGGING
	waiter->on_tree = true;
#endif
}

static struct waiter_info_s *waiter_next(const struct user_futex_s *futex_tree, struct waiter_info_s *curr)
{
	struct waiter_info_s *next = NULL;
	struct dlist_node *node = dlist_get_next(&curr->list_node);
	if (node != NULL && node != &(futex_tree->dlist_head)) {
		next = dlist_entry(node, struct waiter_info_s, list_node);
	}
	return next;
}

static struct waiter_info_s *
get_first_waiter(struct rbt_tree *troot, int *uaddr)
{
	struct waiter_info_s *curr;
	struct rbt_node *p = troot->root;
	struct waiter_info_s *ret_entry = NULL;

	while (p != NULL) {
		curr = rbt_entry(p, struct waiter_info_s, tree_node);
		if (curr->uaddr == uaddr) {
			ret_entry = curr;
			p = p->rbt_left;
		} else if (curr->uaddr < uaddr) {
			p = p->rbt_right;
		} else if (ret_entry == NULL) {
			p = p->rbt_left;
		} else {
			break;
		}
	}

	return ret_entry;
}

static inline bool is_waiter_on_tree(struct waiter_info_s *remove)
{
	struct user_futex_s *tree = ufutex_tree(remove->uaddr);
	bool ret = ((rbt_parent(&remove->tree_node) != NULL) ||
				(tree->rbt_root.root == &remove->tree_node));

	return ret;
}

static void remove_waiter(struct waiter_info_s *remove)
{
#ifdef CONFIG_USER_FUTEX_DEBUGGING
	ufutex_bugon(__rbt_is_colorless(&remove->tree_node) ||
			!(__rbt_is_red(&remove->tree_node) || __rbt_is_black(&remove->tree_node)),
		remove->uaddr,
		"rbt_remove failure:%llx - %d,%d,%d,%d,%d\n",
		remove->cref,
		remove->on_tree,
		__rbt_is_colorless(&remove->tree_node),
		__rbt_is_red(&remove->tree_node),
		__rbt_is_black(&remove->tree_node),
		(rbt_parent(&remove->tree_node) == NULL));
#endif

	struct user_futex_s *tree = ufutex_tree(remove->uaddr);
	rbt_remove(&tree->rbt_root, (struct rbt_node *)&remove->tree_node);

#ifdef CONFIG_USER_FUTEX_DEBUGGING
	remove->on_tree = false;
#endif

	dlist_delete(&remove->list_node);
}

static inline bool uaddr_should_wait(volatile int *uaddr, int val, unsigned int flags)
{
	/*
	 * uaddr is checked differently depending on whether or not it's a bitlock futex usage
	 * if it's a bitlock, val is a mask of the lock bit (and the wait bit),
	 * and both need to be set for a valid wait condition.
	 */
	bool ret;
	if ((flags & __FUTEX_FLAG_BITLOCK) != 0U) {
		unsigned int uval_masked = ((unsigned int)*uaddr & (unsigned int)val);
		ret = (uval_masked == (unsigned int)val);
	} else {
		ret = (*uaddr == val);
	}
	return ret;
}

static int unblock_chk_return(int rc, unsigned int flags, struct waiter_info_s *waiter, bool *reblock)
{
	int ret = rc;
	BUG_ON((rc ==  E_HM_AGAIN), "sysfast_block_thread returns E_HM_AGAIN\n");

	*reblock = false;

	if (rc == E_HM_OVERFLOW) {
		ret = E_HM_AGAIN;
		/* get the new priority */
		waiter_self.priority = hm_actv_local_thread_priority_of();
		/* do not touch gen_cnt */
		if (uaddr_should_wait(waiter->uaddr, waiter->val, flags)) {
			*reblock = true;
		}
	}

	if (rc != E_HM_OK) {
		struct user_futex_s *tree = ufutex_tree(waiter->uaddr);
		int cpu = spinlock_acquire_preempt(&tree->slock);
		if (is_waiter_on_tree(waiter)) {
			remove_waiter(waiter);

			if (*reblock) {
				waiter_insert(waiter);
			}
		}
		spinlock_release_preempt(&tree->slock, cpu);
	}

	if (rc == E_HM_BUSY) {
		ret = E_HM_OK;
	}

	return ret;
}

int hm_user_futex_wait(volatile int *uaddr, int val,
					unsigned long bitset, unsigned int flags,
					struct __hmsigmask *mask, unsigned long long *ptimeout)
{
	if (uaddr == NULL) {
		return E_HM_FAULT;
	}

	if (ptr_to_ulong(uaddr) % sizeof(int) != 0UL) {
		return E_HM_INVAL;
	}

	int *block_uaddr = NULL;
	unsigned int gen_cnt = 0;
	struct __block_param_s block_param = {0};

	if (!uaddr_should_wait(uaddr, val, flags)) {
		return E_HM_AGAIN;
	}

	init_waiter_info(sync_waiter_self_ex(), uaddr, val, bitset);

	int rc = E_HM_OK;
	struct user_futex_s *tree = ufutex_tree(uaddr);

	waiter_self.should_block = false;
	int cpu = spinlock_acquire_preempt(&tree->slock);
	if (uaddr_should_wait(uaddr, val, flags)) {
		waiter_insert(&waiter_self);
		/* get the snapshot */
		gen_cnt = ++waiter_self.gencnt;
		waiter_self.should_block = true;
		block_uaddr = (int*) waiter_self.uaddr;
	} else {
		rc = E_HM_AGAIN;
	}
	spinlock_release_preempt(&tree->slock, cpu);

	while (waiter_self.should_block) {
		hm_set_block_param(&block_param, ptr_to_ulong(__builtin_return_address(0U)));
		rc = sysfast_block_thread(block_uaddr, mask, flags, ptimeout, &waiter_self.gencnt, gen_cnt, &block_param);
		rc = unblock_chk_return(rc, flags, &waiter_self, &waiter_self.should_block);
	}

	return rc;
}

static unsigned int get_waiters_and_req(volatile int *src_ptr, volatile int *dst_ptr, unsigned int deq_num,
					unsigned int req_num, struct __ufutex_unblock_target_s wake_iov[],
					unsigned int iov_size)
{
	unsigned int i = 0U;

	struct waiter_info_s *next;
	struct waiter_info_s *curr;
	unsigned int total_num = req_num + deq_num;
	unsigned int actual_deq_num = 0;

	curr = get_first_waiter(&ufutex_tree(src_ptr)->rbt_root, (int*)src_ptr);
	if (curr != NULL) {
		while (i < total_num) {
			next = waiter_next(ufutex_tree(curr->uaddr), curr);
			if (i < deq_num) {
				wake_iov[i].gencnt_expected = ++curr->gencnt;
				wake_iov[i].gencnt_addr = ptr_to_u64(&curr->gencnt);
				wake_iov[i++].waiter_cref = (__u64)curr->cref;
				actual_deq_num = i;
				remove_waiter(curr);
				BUG_ON(i > iov_size);
			} else {
				/* We are doing requeue - do NOT change gencnt */
				remove_waiter(curr);
				curr->uaddr = dst_ptr;
				waiter_insert(curr);
				i++;
			}
			curr = next;
			if ((curr == NULL) || (curr->uaddr != src_ptr)) {
				break;
			}
		}
	}

	return actual_deq_num;
}

static unsigned long futex_get_wake_bitset(unsigned long waiter_bitset, unsigned long bitset1, unsigned long bitset2)
{
	unsigned long wake_bitset;

	/* If bitset is matched, means block_at->futex's bitset is same with bitset which transfered by userspace */
	if (bitset2 != 0UL && ((waiter_bitset & bitset2 & __FUTEX_BITSET2_RWLOCK_RW_MASK) != 0UL)) {
		wake_bitset = bitset2;
	} else {
		wake_bitset = bitset1;
	}
	return wake_bitset;
}

struct get_waiters_args_s {
	struct __ufutex_unblock_target_s wake_iov[__UFUTEX_MAX_UNBLOCK_TARGET];
	cref_t waker_cref;

	unsigned int waiters_left;
	unsigned int num_added;

	struct waiter_info_s *next_waiter;

	bool is_first_check;
};

static void do_enqueue_waiters(struct waiter_info_s * waiter, unsigned int idx, struct get_waiters_args_s *args)
{
	/* check if we can queue waiter in wake_iov, or we need to pend it */
	if (idx < __UFUTEX_MAX_UNBLOCK_TARGET) {
		/*
		 * We only remove waiters whose waker_cref is 0.
		 * If a waiter's waker_cref is not 0, it needs to
		 * be removed by the waker thread who has the matching cref.
		 */
		args->wake_iov[idx].gencnt_expected = ++waiter->gencnt;
		args->wake_iov[idx].gencnt_addr = ptr_to_u64(&waiter->gencnt);
		args->wake_iov[idx].waiter_cref = (__u64)waiter->cref;
		args->num_added = (idx + 1);
		remove_waiter(waiter);
	} else {
		/*
		 * There are more waiters than __UFUTEX_MAX_UNBLOCK_TARGET
		 * Mark the waiter with the waker_cref
		 */
		waiter->waker_cref = args->waker_cref;
		++(args->waiters_left);
	}
}

static unsigned int get_waiters_to_wake_ex(volatile int *uaddr, unsigned int num,
				unsigned long bitset, unsigned long bitset2,
				struct get_waiters_args_s *args)
{
	unsigned int i = 0U;

	struct waiter_info_s *next;
	struct waiter_info_s *curr;
	unsigned long l_bitset = 0;
	struct user_futex_s *tree = ufutex_tree(uaddr);

	BUG_ON(args == NULL);
	args->waiters_left = 0;
	args->num_added = 0;

	curr = get_first_waiter(&tree->rbt_root, (int*)uaddr);
	if (curr != NULL) {
		l_bitset = futex_get_wake_bitset(curr->bitset, bitset, bitset2);
	}

	while (i < num && curr != NULL && curr->uaddr == uaddr) {
		/* check if bitset is match */
		if ((curr->bitset & l_bitset) != 0U) {
			next = waiter_next(tree, curr);
			if ((args->is_first_check && (curr->waker_cref == 0)) ||
				(!args->is_first_check && (curr->waker_cref == args->waker_cref))) {
				do_enqueue_waiters(curr, i++, args);
			}
			curr = next;
		} else {
			curr = waiter_next(tree, curr);
		}
	}

	args->next_waiter = ((curr != NULL) && (curr->uaddr == uaddr)) ? curr : NULL;
	return args->num_added;
}

static void clear_bitlock_wait_bit(volatile int *uaddr, unsigned int lock_bitmask, unsigned int wait_bitmask)
{
	/* if we haven't woken up anyone and if lock is not busy, we clear the wait bit */
	unsigned int new_val;
	unsigned int expt_val;
	unsigned int old = (unsigned int)vatomic32_read_rlx((vatomic32_t *)(intptr_t)uaddr);
	do {
		if ((old & lock_bitmask) != 0) {
			/* lock is busy, let the holder to deal with it. */
			break;
		} else if ((old & wait_bitmask) == 0) {
			/* wait bit is cleared by someone else, nothing for me to do */
			break;
		}
		new_val = expt_val = old;
		new_val &= ~(wait_bitmask);
		old = (unsigned int)vatomic32_cmpxchg((vatomic32_t *)(intptr_t)uaddr, expt_val, new_val);
	} while (old != expt_val);
}

int hm_user_futex_wake_ex(volatile int *uaddr, unsigned int num,
				unsigned long bitset, unsigned long bitset2,
				unsigned int flags)
{
	if (uaddr == NULL) {
		return E_HM_FAULT;
	}

	if (ptr_to_ulong(uaddr) % sizeof(int) != 0UL) {
		return E_HM_INVAL;
	}

	int rc = E_HM_OK;
	int ret_num = 0;
	unsigned int num_to_wake = num;
	struct user_futex_s *tree = ufutex_tree(uaddr);

	struct get_waiters_args_s args;
	args.waker_cref = hm_actv_local_thread_cref_of();
	args.waiters_left = num;
	args.num_added = 0;
	args.next_waiter = NULL;
	args.is_first_check = true;

	bool is_bitlock = ((flags & __FUTEX_FLAG_BITLOCK) != 0U);

	unsigned long true_bitset1 = is_bitlock ? FUTEX_BITSET_MATCH_ANY : bitset;
	unsigned long true_bitset2 = is_bitlock ? 0 : bitset2;

	while ((num_to_wake > 0) && (args.waiters_left > 0)) {
		int cpu = spinlock_acquire_preempt(&tree->slock);
		unsigned int num_added = get_waiters_to_wake_ex(uaddr, num_to_wake, true_bitset1, true_bitset2,  &args);
		if (is_bitlock && (args.next_waiter == NULL)) {
			clear_bitlock_wait_bit(uaddr, (unsigned int)bitset, (unsigned int)bitset2);
		}
		spinlock_release_preempt(&tree->slock, cpu);

		if (num_added == 0) {
			break;
		}

		rc = sysfast_unblock_threads(args.wake_iov, num_added, (int*)uaddr, flags);
		BUG_ON((rc < 0), "user_futex:unblock_thread error:%d (%s)\n", rc, hmstrerror(rc));

		ret_num += rc;

		num_to_wake -= num_added;
		args.is_first_check = false;
	}
	return (rc >= 0) ? ret_num : rc;
}

static int __futex_requeue_addr_check(volatile const int *src_uaddr, volatile const int *dst_uaddr)
{
	int err = E_HM_OK;
	if (src_uaddr == dst_uaddr) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (ptr_to_ulong(src_uaddr) % sizeof(int) != 0UL ||
			ptr_to_ulong(dst_uaddr) % sizeof(int) != 0UL) {
			err = E_HM_INVAL;
		}
	}
	return err;
}

int hm_user_futex_requeue(volatile int *src_ptr, volatile int *dst_ptr,
		      unsigned int deq_num, unsigned int req_num)
{
	struct __ufutex_unblock_target_s wake_iov[__UFUTEX_MAX_UNBLOCK_TARGET] = {0};
	int rc = E_HM_OK;
	BUG_ON(deq_num > __UFUTEX_MAX_UNBLOCK_TARGET);
	unsigned int num_added = 0;
	struct user_futex_s *src_tree = ufutex_tree(src_ptr);
	struct user_futex_s *dst_tree = ufutex_tree(dst_ptr);

	rc = __futex_requeue_addr_check(src_ptr, dst_ptr);
	if (rc == E_HM_OK) {
		int cpu = double_acquire_preempt(&src_tree->slock, &dst_tree->slock);
		num_added = get_waiters_and_req(src_ptr, dst_ptr, deq_num, req_num, wake_iov, __UFUTEX_MAX_UNBLOCK_TARGET);
		double_release_preempt(&src_tree->slock, &dst_tree->slock, cpu);

		if (num_added > 0) {
			rc = sysfast_unblock_threads(wake_iov, num_added, (int*)src_ptr, 0);
			BUG_ON((rc < 0), "user_futex:unblock_thread error:%d,%u,%u,%u\n", rc, num_added, deq_num, req_num);
		}
	}

	return rc;
}

#define UFUTEX_DUMP_ITER_LIMIT __UFUTEX_MAX_UNBLOCK_TARGET

void hm_user_futex_dump(volatile int *uaddr)
{
#ifdef CONFIG_USER_FUTEX_DEBUGGING
	if (ufutex_hash_table == NULL) {
		hm_error("user-futex tree not allocated\n");
		return;
	}

	int i = 0;
	struct user_futex_s *tree = ufutex_tree(uaddr);
	int cpu = spinlock_acquire_preempt(&tree->slock);

	struct waiter_info_s *curr = rbt_entry(tree->rbt_root.root, struct waiter_info_s, tree_node);
	while ((curr != NULL) && (i < UFUTEX_DUMP_ITER_LIMIT)) {
		i++;
		hm_error("ufutex_waiter_info: %llx, prio: %d, val: %d, bitset: %lx, on_addr: %u, status: %x, %u\n",
			curr->cref,
			curr->priority,
			curr->val,
			curr->bitset,
			(unsigned int)(curr->uaddr == uaddr),
			ufutex_wait_status(curr),
			(unsigned int)(__rbt_is_red(&curr->tree_node) || __rbt_is_black(&curr->tree_node)));
		curr = waiter_next(tree, curr);
	}
	spinlock_release_preempt(&tree->slock, cpu);

	if (i == 0) {
		hm_error("has no waiters on tree\n");
	} else if (i >= UFUTEX_DUMP_ITER_LIMIT) {
		hm_error("ufutex_dump iteration limit reached. Possible loop found\n");
	}
#else
	UNUSED(uaddr);
	hm_error("Not supported\n");
#endif
}

#else /* CONFIG_USER_FUTEX */

void hm_user_futex_init(bool replace_futex) {(void)replace_futex;}
void hm_user_futex_dump(volatile int *uaddr) {UNUSED(uaddr);}
#endif
