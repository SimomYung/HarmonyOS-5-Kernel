// SPDX-License-Identifier: GPL-2.0
#include <linux/cred.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/profile.h>
#include <linux/sched.h>
#include <linux/init_task.h>
#include <linux/fs_struct.h>
#include <linux/mmap_lock.h>
#include <linux/mmu_notifier.h>
#include <linux/sched/mm.h>
#include <asm/mmu_context.h>
#include <linux/hugetlb.h>
#ifdef CONFIG_KEYS
#include <linux/keyctl.h>
#endif
#ifdef CONFIG_NO_HZ_IDLE
#include <linux/tick.h>
#endif
#include <linux/tty.h>

#include <lnxbase/lnxbase.h>
#include <trace/hooks/liblinux.h>
#include <liblinux/internal.h>
#include <liblinux/pal.h>

#include "libdh_linux.h"

extern int hm_actv_local_thread_tid_of(void);
extern int hm_actv_local_thread_pid_of(void);
extern int hm_actv_local_thread_vpid_of(void);
extern unsigned long long hm_actv_local_thread_vskey_of(void);

static void *__mm_init(struct mm_struct *mm, struct task_struct *p)
{
	mm->mmap = NULL;
	mm->mm_rb = RB_ROOT;
	mm->vmacache_seqnum = 0;
#ifdef CONFIG_RSS_THRESHOLD
	mm->rss_threshold = 0;
#endif
	atomic_set(&mm->mm_users, 1);
	atomic_set(&mm->mm_count, 1);
	seqcount_init(&mm->write_protect_seq);
	mmap_init_lock(mm);
	INIT_LIST_HEAD(&mm->mmlist);
	mm->core_state = NULL;
	mm_pgtables_bytes_init(mm);
	mm->map_count = 0;
	mm->locked_vm = 0;
	atomic_set(&mm->has_pinned, 0);
	atomic64_set(&mm->pinned_vm, 0);
	memset(&mm->rss_stat, 0, sizeof(mm->rss_stat));
	spin_lock_init(&mm->page_table_lock);
	spin_lock_init(&mm->arg_lock);
	mm_init_cpumask(mm);
	RCU_INIT_POINTER(mm->exe_file, NULL);
	mmu_notifier_subscriptions_init(mm);
	init_tlb_flush_pending(mm);
#if defined(CONFIG_TRANSPARENT_HUGEPAGE) && !USE_SPLIT_PMD_PTLOCKS
	mm->pmd_huge_pte = NULL;
#endif
	hugetlb_count_init(mm);
#ifdef CONFIG_TASK_PROTECT_LRU
	mm->protect = 0;
#endif
	mm->flags = 0;
	mm->def_flags = 0;
	init_new_context(p, mm);
	mm->user_ns = &init_user_ns;
}

struct rb_mm_node {
	struct rb_node rb_node;
	unsigned long long vspace_key;
	struct mm_struct mm;
	cpumask_t cpumask_of_mm; /* mast be located after `mm` */
};
static struct rb_root mm_rbroot = RB_ROOT;
static struct liblinux_pal_mutex_t mm_rb_lock = LIBLINUX_MUTEX_INITIALIZER;

struct rb_process_node {
	struct rb_node rb_node;
	unsigned int cnode_idx;
	struct cred cred;
};
static struct rb_root cnode_rbroot = RB_ROOT;
static struct liblinux_pal_mutex_t cnode_rb_lock = LIBLINUX_MUTEX_INITIALIZER;

static inline struct rb_mm_node *get_rb_new_mm(void)
{
	struct rb_mm_node *tmp_node = NULL;

	tmp_node = liblinux_pal_calloc(1, sizeof(struct rb_mm_node));

	return tmp_node;
}

static inline struct rb_process_node *get_rb_new_process(void)
{
	struct rb_process_node *tmp_node = NULL;

	tmp_node = liblinux_pal_calloc(1, sizeof(struct rb_process_node));

	return tmp_node;
}

void liblinux_free_mm(struct mm_struct *mm)
{
	struct rb_mm_node *entry = NULL;

	entry = container_of(mm, struct rb_mm_node, mm);
	liblinux_pal_free(entry);
}

static struct rb_mm_node *rb_simple_mm_get_init(struct rb_root *root, u64 vspace_key,
						struct task_struct *tsk)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct rb_mm_node *entry = NULL;
	struct rb_mm_node *mm_node = NULL;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct rb_mm_node, rb_node);

		if (vspace_key < entry->vspace_key)
			p = &(*p)->rb_left;
		else if (vspace_key > entry->vspace_key)
			p = &(*p)->rb_right;
		else {
			tsk->mm = &entry->mm;
			return entry;
		}
	}

	/* not found */
	mm_node = get_rb_new_mm();
	if (mm_node == NULL) {
		pr_err("current mm init failed !\n");
		return NULL;
	}
	mm_node->vspace_key = vspace_key;
	__mm_init(&mm_node->mm, tsk);
	VSPACE_KEY(&mm_node->mm) = vspace_key;
	tsk->mm = &mm_node->mm;

	rb_link_node(&mm_node->rb_node, parent, p);
	rb_insert_color(&mm_node->rb_node, root);
	return mm_node;
}

void __init_current_mm_info(struct task_struct *p, unsigned long long vspace_key)
{
	struct rb_mm_node *ret = NULL;

	liblinux_pal_mutex_lock(&mm_rb_lock);
	ret = rb_simple_mm_get_init(&mm_rbroot, vspace_key, p);
	liblinux_pal_mutex_unlock(&mm_rb_lock);
}

static struct rb_process_node *rb_simple_process_get_init(struct rb_root *root, u64 cnode_idx,
							  struct task_struct *tsk)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct rb_process_node *entry = NULL;
	struct rb_process_node *cnode_node = NULL;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct rb_process_node, rb_node);

		if (cnode_idx < entry->cnode_idx)
			p = &(*p)->rb_left;
		else if (cnode_idx > entry->cnode_idx)
			p = &(*p)->rb_right;
		else {
			tsk->cred = &entry->cred;
			return entry;
		}
	}

	/* not found */
	cnode_node = get_rb_new_process();
	if (cnode_node == NULL) {
		pr_err("current cred init failed !\n");
		return NULL;
	}
	/* refcnt > 1, never free */
	memcpy(&(cnode_node->cred), &init_cred, sizeof(init_cred));
	cnode_node->cnode_idx = cnode_idx;
	tsk->cred = &cnode_node->cred;

	rb_link_node(&cnode_node->rb_node, parent, p);
	rb_insert_color(&cnode_node->rb_node, root);
	return cnode_node;
}

static inline void __init_current_process_info(struct task_struct *p, unsigned int cnode_idx)
{
	struct rb_process_node *ret = NULL;

	liblinux_pal_mutex_lock(&cnode_rb_lock);
	ret = rb_simple_process_get_init(&cnode_rbroot, cnode_idx, p);
	liblinux_pal_mutex_unlock(&cnode_rb_lock);
}

int __liblinux_mm_release(unsigned long long vspace_key)
{
	struct rb_node **p = &(mm_rbroot.rb_node);
	struct rb_node *parent = NULL;
	struct rb_mm_node *entry = NULL;
	struct rb_mm_node *mm_node = NULL;

	liblinux_pal_mutex_lock(&mm_rb_lock);
	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct rb_mm_node, rb_node);

		if (vspace_key < entry->vspace_key)
			p = &(*p)->rb_left;
		else if (vspace_key > entry->vspace_key)
			p = &(*p)->rb_right;
		else {
			mm_node = entry;
			break;
		}
	}
	if (mm_node == NULL) {
		pr_debug("ldk don't has inited mm for %llx\n", vspace_key);
		liblinux_pal_mutex_unlock(&mm_rb_lock);
		return -ENOENT;
	}
	rb_erase(&mm_node->rb_node, &mm_rbroot);
	liblinux_pal_mutex_unlock(&mm_rb_lock);

	mmap_write_lock(&mm_node->mm);
	VSPACE_KEY(&mm_node->mm) = (unsigned long long)-1;
	mmap_write_unlock(&mm_node->mm);
	mmdrop(&mm_node->mm);
	return 0;
}

#ifdef CONFIG_KEYS
static int __init cred_session_keyring_init(void)
{
	struct key *liblinux_session_keyring = NULL;

	liblinux_session_keyring = (struct key *)keyring_alloc(
					"uid.liblinux", GLOBAL_ROOT_UID, GLOBAL_ROOT_GID,
					&init_cred, (KEY_POS_ALL & ~KEY_POS_SETATTR) | KEY_USR_ALL,
					KEY_ALLOC_UID_KEYRING | KEY_ALLOC_IN_QUOTA, NULL, NULL);
	if (IS_ERR(liblinux_session_keyring)) {
		pr_err("alloc keyring failed: %ld\n", PTR_ERR(liblinux_session_keyring));
		return PTR_ERR(liblinux_session_keyring);
	}
	init_cred.session_keyring = liblinux_session_keyring;
	init_cred.jit_keyring = KEY_REQKEY_DEFL_SESSION_KEYRING;

	(void)get_cred(&init_cred);

	return 0;
}
core_initcall(cred_session_keyring_init);
#endif

void __init liblinux_cred_init(void)
{
}

void liblinux_current_thread_fill(void)
{
	struct task_struct *cur = current;

	cur->pid = hm_actv_local_thread_tid_of();
	current_thread_name_fill(cur->comm, sizeof(cur->comm));
}

void liblinux_get_cred_dac(struct cred **cred)
{
	struct lnxbase_cred_dac cred_dac;
	unsigned int cnode_idx;
	struct task_struct *tsk = current;

	cnode_idx = lnxbase_current_cnode_idx();
	__init_current_process_info(tsk, cnode_idx);

	if (tsk->cred != NULL && tsk->cred != &init_cred)
		*cred = tsk->cred;
	BUG_ON(*cred == NULL);

	lnxbase_get_dac(&cred_dac);
	(*cred)->uid.val = cred_dac.uid;
	(*cred)->gid.val = cred_dac.gid;
	(*cred)->suid.val = cred_dac.suid;
	(*cred)->sgid.val = cred_dac.sgid;
	(*cred)->euid.val = cred_dac.euid;
	(*cred)->egid.val = cred_dac.egid;
	(*cred)->fsuid.val = cred_dac.fsuid;
	(*cred)->fsgid.val = cred_dac.fsgid;

	/* current_umask() use it */
	task_thread_info(tsk)->umask = cred_dac.umask;
}
EXPORT_SYMBOL(liblinux_get_cred_dac);

void liblinux_enter_nodac(void)
{
	struct cred *cred = NULL;
	unsigned long long vspace_key;
	struct task_struct *tsk = current;

	set_fs(USER_DS);

	liblinux_fill_thread_info(&tsk->pid, &tsk->tgid, &task_thread_info(tsk)->vtgid,
					&vspace_key, tsk->comm, sizeof(tsk->comm));
	__init_current_mm_info(tsk, vspace_key);

#ifdef CONFIG_NO_HZ_IDLE
	preempt_disable();
	if (raw_smp_processor_id() != 0 && check_do_timer_none() && tick_nohz_tick_stopped())
		tick_nohz_idle_restart_tick();
	preempt_enable();
#endif

	clear_tsk_thread_flag(tsk, TIF_SIGPENDING);
	__set_current_state(TASK_RUNNING);
}

void liblinux_enter_nomm(void)
{
	struct task_struct *cur = current;
	int rt_prio;
	int nice;

	set_fs(USER_DS);
	cur->pid = hm_actv_local_thread_tid_of();
	cur->tgid = hm_actv_local_thread_pid_of();

	liblinux_pal_thread_get_priority(&rt_prio, &nice);
	if (rt_prio > 0) {
		cur->prio = MAX_RT_PRIO - 1 - rt_prio;
		cur->rt_priority = rt_prio;
		cur->static_prio = NICE_TO_PRIO(0);
		cur->normal_prio = cur->prio;
	} else if (rt_prio == 0) {
		cur->prio = NICE_TO_PRIO(nice);
		cur->rt_priority = 0;
		cur->static_prio = cur->prio;
		cur->normal_prio = cur->prio;
	}

	task_thread_info(cur)->vtgid = hm_actv_local_thread_vpid_of();
	current_thread_name_fill(cur->comm, sizeof(cur->comm));

	cur->mm = NULL;

#ifdef CONFIG_NO_HZ_IDLE
	preempt_disable();
	if (raw_smp_processor_id() != 0 && check_do_timer_none() && tick_nohz_tick_stopped()) {
		tick_nohz_idle_restart_tick();
	}
	preempt_enable();
#endif

	clear_tsk_thread_flag(cur, TIF_SIGPENDING);
	__set_current_state(TASK_RUNNING);
}

void liblinux_enter(int pid, ...)
{
	va_list args;
	unsigned int cnode_idx;
	unsigned int uid;
	unsigned int gid;
	unsigned int suid;
	unsigned int sgid;
	unsigned int euid;
	unsigned int egid;
	unsigned int fsuid;
	unsigned int fsgid;
	int umask;
	struct task_struct *cur = current;
	int rt_prio;
	int nice;

	set_fs(USER_DS);
	va_start(args, pid);
	uid = va_arg(args, unsigned int);
	gid = va_arg(args, unsigned int);
	suid = va_arg(args, unsigned int);
	sgid = va_arg(args, unsigned int);
	euid = va_arg(args, unsigned int);
	egid = va_arg(args, unsigned int);
	fsuid = va_arg(args, unsigned int);
	fsgid = va_arg(args, unsigned int);
	umask = va_arg(args, int);
	cnode_idx = va_arg(args, unsigned int);

	va_end(args);
	pid = hm_actv_local_thread_pid_of();
	cur->pid = hm_actv_local_thread_tid_of();
	cur->tgid = pid;

	liblinux_pal_thread_get_priority(&rt_prio, &nice);
	if (rt_prio > 0) {
		cur->prio = MAX_RT_PRIO - 1 - rt_prio;
		cur->rt_priority = rt_prio;
		cur->static_prio = NICE_TO_PRIO(0);
		cur->normal_prio = cur->prio;
	} else if (rt_prio == 0) {
		cur->prio = NICE_TO_PRIO(nice);
		cur->rt_priority = 0;
		cur->static_prio = cur->prio;
		cur->normal_prio = cur->prio;
	}

	task_thread_info(cur)->vtgid = hm_actv_local_thread_vpid_of();
	current_thread_name_fill(cur->comm, sizeof(cur->comm));

	__init_current_mm_info(cur, hm_actv_local_thread_vskey_of());

	/* current_umask() use it */
	task_thread_info(cur)->umask = umask;

#ifdef CONFIG_NO_HZ_IDLE
	preempt_disable();
	if (raw_smp_processor_id() != 0 && check_do_timer_none() && tick_nohz_tick_stopped())
		tick_nohz_idle_restart_tick();
	preempt_enable();
#endif

	clear_tsk_thread_flag(cur, TIF_SIGPENDING);
	__set_current_state(TASK_RUNNING);
}

void liblinux_exit(void)
{
	struct task_struct *tsk = current;

	/*
	 * just ignore signal
	 * maybe we should send signal to target tsk?
	 */
	clear_tsk_thread_flag(tsk, TIF_SIGPENDING);

	/*
	 * Always run task_work immediately, because the task_struct is reused
	 * by multiple contexts. The real `current` may switch to other task_struct
	 * or may have exited without notify.
	 */
	clear_tsk_thread_flag(tsk, TIF_NOTIFY_RESUME);

	/*
	 * This barrier pairs with task_work_add()->set_notify_resume() after
	 * hlist_add_head(task->task_works);
	 */
	smp_mb__after_atomic();
	if (unlikely(tsk->task_works))
		task_work_run();

	trace_ldk_rvh_syscall_exit(tsk);

	/*
	 * Restore cred and mm to default.
	 * Place it at the end of the function to ensure it will not be used again.
	 */
	tsk->cred = &init_cred;
	tsk->mm = NULL;

	/* Check preempt and irq state, both should be enabled */
	if (!preemptible())
		panic("unexpected preempt state %d irq state %d, caller %pS\n", preempt_count(), irqs_disabled(), current_thread_info()->irq_save_caller);
}

int dh_ctor_handle_fault(void *va, unsigned long long vr_key, unsigned long address, unsigned int flags, void *regs)
{
	struct task_struct *cur = current;

	cur->pid = hm_actv_local_thread_tid_of();
	cur->tgid = hm_actv_local_thread_pid_of();
	cur->thread.fault_vr_key = vr_key;
	return (int)handle_mm_fault(va, address, flags, regs);
}

int dh_ctor_mm_rdlock(void)
{
	int ret = 0;
	struct task_struct *tsk = current;

	__init_current_mm_info(tsk, hm_actv_local_thread_vskey_of());
	if (unlikely(tsk->mm == NULL)) {
		pr_err("mm rdlock get task mm failed!\n");
		ret = -EINVAL;
	} else {
		mmap_read_lock(tsk->mm);
	}

	return ret;
}

void dh_ctor_mm_rdunlock(void)
{
	BUG_ON(unlikely(current->mm == NULL));
	mmap_read_unlock(current->mm);
}

#ifdef CONFIG_PROFILING
void liblinux_profile_task_exit(unsigned long pid, unsigned long uid)
{
	struct cred *cred = current_cred();

	/*
	 * In most registered notifiers of task_exit_notifier, only parameters such as pid and uid
	 * are generally used. However, there is a risk of forging a task. If tcb_cref in a notifier
	 * is used for a PAL interface, a problem may occur. A more graceful way of exiting the process
	 * will be implemented without reusing the task_struct `current`, and the above problem will be solved.
	 */
	cred->uid.val = uid;
	current->pid = pid;
	current->tgid = pid;
	current->group_leader = current;

	profile_task_exit(current);
}
EXPORT_SYMBOL(liblinux_profile_task_exit);
#endif

void liblinux_vhangup(void)
{
	tty_vhangup_self();
}
