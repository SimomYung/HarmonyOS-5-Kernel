// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/fs_struct.h>
#include <linux/utsname.h>
#include <linux/proc_ns.h>
#include <linux/init_task.h>
#include <linux/kasan.h>
#include <linux/kthread.h>
#include <linux/kmemleak.h>
#include <linux/rtmutex.h>
#include <linux/profile.h>
#include <linux/preempt.h>
#include <linux/module.h>

#include <uapi/linux/sched/types.h>
#include <linux/sched/isolation.h>

#include <trace/hooks/liblinux.h>

#include "signal.h"

static struct kmem_cache *task_struct_cachep;
struct kmem_cache *sighand_cachep;

static inline struct task_struct *alloc_task_struct(void)
{
	/* use GFP_KERNEL, see kernel/fork.c */
	return kmem_cache_alloc(task_struct_cachep, GFP_KERNEL);
}

static inline void free_task_struct(struct task_struct *tsk)
{
	kmem_cache_free(task_struct_cachep, tsk);
}

void __init fork_init(void)
{
	int i;

	task_struct_cachep = kmem_cache_create("task_struct",
			arch_task_struct_size, L1_CACHE_BYTES,
			SLAB_PANIC|SLAB_ACCOUNT, NULL);

	for (i = 0; i < UCOUNT_COUNTS; i++) {
		/* 1024 ns should be enough for liblinux */
		init_user_ns.ucount_max[i] = 1024;
	}
}

extern pid_t __alloc_pid(struct task_struct *p, pid_t assigned_pid);
extern void __free_pid(struct task_struct *p);
extern int liblinux_process_pid(void);
extern void liblinux_raw_thread_init(void);

struct pid_namespace *task_active_pid_ns(struct task_struct *tsk)
{
	return &init_pid_ns;
}
EXPORT_SYMBOL_GPL(task_active_pid_ns);

typedef struct {
	int (*fn)(void *);
	void                  *arg;
	liblinux_pal_thread_handle_t thread_handle;
}__kthread_data;

static void kernel_thread_cleanup(void *data)
{
	struct task_struct *p = (struct task_struct *)data;
	BUG_ON(p == NULL);

	if (p->io_context) {
		exit_io_context(p);
	}

	if (p->flags & PF_KTHREAD) {
		if (unlikely(current->task_works))
			task_work_run();
		put_cred(p->cred);
	}

	put_task_struct(p);
}

static void __liblinux_thread_join(struct thread_info *ti)
{
#ifdef CONFIG_THREAD_INFO_IN_TASK
	struct task_struct *k = container_of(ti, struct task_struct, thread_info);
#else
	struct task_struct *k = ti->task;
#endif
	void *code = NULL;
	void *exit_code = NULL;

	/* get thread exit_code before destroyed by join */
	code = (void *)(unsigned long)k->exit_code;

	/* check free_task() */
	rt_mutex_debug_task_free(k);
	ftrace_graph_exit_task(k);
	if (k->flags & PF_KTHREAD) {
		free_kthread_struct(k);
	}

	trace_ldk_rvh_exit_task(k);

	(void)liblinux_pal_thread_join_ex(&ti->thread_handle, &exit_code);
	BUG_ON(code != exit_code);

	free_task_struct(k);
}

static struct liblinux_pal_mutex_t thread_join_lock = LIBLINUX_MUTEX_INITIALIZER;
static LIST_HEAD(thread_join_task_list);
static DECLARE_WAIT_QUEUE_HEAD(thread_join_wq);
static struct task_struct *thread_join_task = NULL;

static int thread_join_fn(void *_unused)
{
	struct thread_info *ti = NULL;
	struct list_head del_list;

	while (!kthread_should_stop()) {
		liblinux_pal_mutex_lock(&thread_join_lock);
		list_replace_init(&thread_join_task_list, &del_list);
		liblinux_pal_mutex_unlock(&thread_join_lock);

		while (!list_empty(&del_list)) {
			ti = list_first_entry(&del_list, struct thread_info, list);
			list_del(&ti->list);
			__liblinux_thread_join(ti);
		}

		wait_event_interruptible(thread_join_wq, !list_empty(&thread_join_task_list));
	}
	panic("join thread should not be stoppped\n");
	return 0;
}

static void thread_join_add(struct thread_info *ti)
{
	liblinux_pal_mutex_lock(&thread_join_lock);
	list_add(&ti->list, &thread_join_task_list);
	liblinux_pal_mutex_unlock(&thread_join_lock);

	wake_up(&thread_join_wq);
}

static int init_thread_join_task(void)
{
	thread_join_task = kthread_run(thread_join_fn, NULL, "kthread-joind");

	BUG_ON(thread_join_task == NULL);
	return 0;
}
early_initcall(init_thread_join_task);

void __put_task_struct(struct task_struct *t)
{
	lockdep_free_task(t);

	/* skip thread join for non-LDK managed threads */
	if (!is_thread_managed(task_thread_info(t))) {
		return;
	}

	if (current != t) {
		__liblinux_thread_join(task_thread_info(t));
	} else {
		thread_join_add(task_thread_info(t));
	}
}
EXPORT_SYMBOL_GPL(__put_task_struct);

static void kernel_thread_init_thread_info(struct task_struct *p)
{
	WRITE_ONCE(task_thread_info(p)->preempt, liblinux_pal_preempt_init());
	task_thread_info(p)->irq_disabled = 0;
	task_thread_info(p)->thread_handle.tcb_cref = liblinux_pal_thread_actv_cref();
}

static notrace __no_sanitize_address
void __kernel_thread_init_data(struct task_struct *p)
{
	static __thread struct nsproxy tls_nsproxy __aligned(8);
	struct nsproxy *nsproxy = &tls_nsproxy;

	p->stack = (void *)0x0; /* avoid p->stack be used */
#ifdef CONFIG_BPF_SYSCALL
	p->signal->rlim[RLIMIT_MEMLOCK].rlim_max = RLIM_INFINITY;
	p->signal->rlim[RLIMIT_MEMLOCK].rlim_cur = RLIM_INFINITY;
#endif
	/*
	 * in linux, the 2 usage is drop when the thread end with do_exit and
	 * when the kernel schedule from this dead thread to another.
	 * currently we simulate the second put in do_exit
	 */
	refcount_set(&p->usage, 2);
	task_thread_info(p)->preempt_count = 0;
	task_thread_info(p)->umask = 0022;

	kernel_thread_init_thread_info(p);

	if (liblinux_pal_is_thread_ctx()) {
		p->cred = get_cred(&init_cred);
	} else {
		p->flags &= (~(PF_KTHREAD));
		memcpy(nsproxy, &init_nsproxy, sizeof(struct nsproxy));
		p->nsproxy = nsproxy;
	}
}

static notrace __no_sanitize_address
void __kernel_thread_init_data_kthread(struct task_struct *p)
{
	p->stack = (void *)0x0; /* avoid p->stack be used */
#ifdef CONFIG_BPF_SYSCALL
	p->signal->rlim[RLIMIT_MEMLOCK].rlim_max = RLIM_INFINITY;
	p->signal->rlim[RLIMIT_MEMLOCK].rlim_cur = RLIM_INFINITY;
#endif
	/*
	 * in linux, the 2 usage is drop when the thread end with do_exit and
	 * when the kernel schedule from this dead thread to another.
	 * currently we simulate the second put in do_exit
	 */
	refcount_set(&p->usage, 2);
	task_thread_info(p)->preempt_count = 0;
	task_thread_info(p)->umask = 0022;

	task_thread_info(p)->irq_disabled = 0;

	p->cred = get_cred(&init_cred);
}

static notrace __no_sanitize_address
void __kernel_thread_init_data_post(struct task_struct *p)
{
	trace_ldk_rvh_init_task(p);
	__init_sighand(p);
	p->thread_info.tls = raw_get_tls();
	raw_spin_lock_init(&p->pi_lock);
}

static notrace __no_sanitize_address
struct task_struct *kernel_thread_init_data(void)
{
	/* declare task_struct as TLS, all threads have an independent copy */
	static __thread struct task_struct tls_task __aligned(8);
	struct task_struct *p = &tls_task;

	/* init task_struct in TLS */
	memcpy(p, &init_task, sizeof(struct task_struct));
	__kernel_thread_init_data(p);

	if (liblinux_pal_thread_set_my_data((void *)p) != 0) {
		return NULL;
	}

	__kernel_thread_init_data_post(p);

	return p;
}

static notrace __no_sanitize_address
void __kernel_thread_setup(struct task_struct *p)
{
	lockdep_init_task(p);

	/* `p` who copyed from `init_task` and `spin_lock` depends on `current` */
	spin_lock(&init_task.fs->lock);
	init_task.fs->users++;
	spin_unlock(&init_task.fs->lock);
	spin_lock_init(&p->alloc_lock);
	raw_spin_lock_init(&p->pi_lock);

	/*
	 * The `task_struct` is allocated by TLS, which will be released
	 * when thread exit. So it never leak.
	 */
	kmemleak_alloc(p, sizeof(struct task_struct), 0 /* not leak */, GFP_KERNEL);
}

static notrace __no_sanitize_address
void __kernel_thread_setup_kthread(struct task_struct *p)
{
	lockdep_init_task(p);

	/* `p` who copyed from `init_task` and `spin_lock` depends on `current` */
	spin_lock(&init_task.fs->lock);
	init_task.fs->users++;
	spin_unlock(&init_task.fs->lock);
	spin_lock_init(&p->alloc_lock);
	raw_spin_lock_init(&p->pi_lock);
}

static notrace __no_sanitize_address
struct task_struct* kernel_thread_setup(void)
{
	struct task_struct *p = NULL;

	p = kernel_thread_init_data();
	if (p == NULL) {
		return NULL;
	}
	__kernel_thread_setup(p);

	return p;
}

notrace __no_sanitize_address struct task_struct *liblinux_thread_setup_p(void)
{
	struct task_struct *p = kernel_thread_setup();

	if (p == NULL)
		return NULL;

	/*
	 * actv task will not call do_exit,
	 * so refcount should be set to 1.
	 */
	refcount_set(&p->usage, 1);

	return p;
}
EXPORT_SYMBOL(liblinux_thread_setup_p);

/*
 * the callback `ctor->ctx_setup` for `backend_ctx_setup`.
 * the caller of this function should be actv/raw_thread.
 */
notrace __no_sanitize_address int liblinux_thread_setup(void)
{
	/* `current` maybe initialized by `liblinux_preempt_disable` */
	if (unlikely(get_current() != NULL)) {
		return 0;
	}
	if (liblinux_thread_setup_p() == NULL) {
		return -EFAULT;
	}
	return 0;
}
EXPORT_SYMBOL(liblinux_thread_setup);

static void exit_mm(struct task_struct *p)
{
	struct completion *vfork;

	task_lock(p);
	if(p->vfork_done) {
		vfork = p->vfork_done;
		p->vfork_done = NULL;
		complete(vfork);
	}
	task_unlock(p);
}

void do_exit(long code)
{
#ifdef CONFIG_DEBUG_KMEMLEAK
	unsigned long stack_base, stack_size;
	stack_size = liblinux_pal_thread_stack_info(&task_thread_info(current)->thread_handle, &stack_base);
	kmemleak_free(stack_base - stack_size);
#endif
	profile_task_exit(current);
	current->exit_code = code;
	exit_mm(current);
	exit_fs(current);

	__cleanup_sighand(current->sighand);
	__free_pid(current);
	set_special_state(TASK_DEAD);
	/*
	 * simulate linux put_task_struct after task finish
	 * note that the task_struct will go away and caller must
	 * ensure the task_struct is still alive when calling kthread_stop
	 */
	put_task_struct(current);

	liblinux_pal_thread_exit_ex(ERR_PTR(code));

	/* Avoid "noreturn function does return".  */
	for (;;) {
		cpu_relax();    /* For when BUG is null */
	}
}
EXPORT_SYMBOL(do_exit);

void complete_and_exit(struct completion *comp, long code)
{
        if (comp)
                complete(comp);

        do_exit(code);
}
EXPORT_SYMBOL(complete_and_exit);

static void kthread_task_init(struct task_struct *p, liblinux_pal_thread_handle_t *handle)
{
	pid_t nr;

	task_thread_info(p)->thread_handle = *handle;
	p->pid = handle->tid;
	p->tgid = liblinux_process_pid();
	nr = __alloc_pid(p, p->pid);
	if (nr < 0) {
		panic("liblinux: alloc pid failed, ret=%d\n", nr);
	}
	if (system_state == SYSTEM_BOOTING) {
		unsigned long mask = *((unsigned long *)cpumask_bits(housekeeping_cpumask(HK_FLAG_KTHREAD)));
		int ret = liblinux_pal_thread_setaffinity(handle, mask);
		if (ret < 0) {
			pr_warn("set affinity for kthread %d failed\n", p->pid);
		}
	} else {
		/* copy parent cpus_mask */
		cpumask_copy(&p->cpus_mask, &current->cpus_mask);
	}
#ifdef CONFIG_DEBUG_KMEMLEAK
	unsigned long stack_base, stack_size;
	stack_size = liblinux_pal_thread_stack_info(&task_thread_info(p)->thread_handle, &stack_base);
	kmemleak_alloc(stack_base - stack_size, stack_size, 0, GFP_KERNEL);
#endif
	raw_spin_lock_init(&p->pi_lock);
}

static void *kernel_thread_helper(void *arg)
{
	struct task_struct *p = raw_get_current();
	__kthread_data *kt = (__kthread_data *)arg;

	BUG_ON(kt == NULL);
	BUG_ON(p == NULL);

	int  (* _fn)(void *) = kt->fn;
	void *_arg = kt->arg;
	int ret;

	WRITE_ONCE(task_thread_info(p)->preempt, liblinux_pal_preempt_init());
	p->thread_info.tls = raw_get_tls();

	kfree(kt);

	ret = (*_fn)(_arg);

	return ERR_PTR(ret);
}

int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags __attribute__((unused)))
{
	int ret;
	__kthread_data *kt;
	struct task_struct *p;

	kt = (__kthread_data *)kmalloc(sizeof(__kthread_data), GFP_KERNEL);
	if (!kt)
		return -ENOMEM;

	kt->fn  = fn;
	kt->arg = arg;

	p = alloc_task_struct();
	if (!p) {
		kfree(kt);
		return -ENOMEM;
	}
	/* init task_struct */
	memcpy(p, &init_task, sizeof(struct task_struct));
	__kernel_thread_init_data_kthread(p);

	ret = __init_sighand_kthread(p);
	if (ret != 0)
		goto err_sighand;

	__kernel_thread_setup_kthread(p);

	get_task_struct(p);
	ret = liblinux_pal_thread_create_task_init(kernel_thread_helper, (void *)kt,
						   &(kt->thread_handle), p, kthread_task_init);
	if (ret != 0) {
		put_task_struct(p);
		ret = -EAGAIN;
		goto err_thread_create;
	}
	ret = p->pid;
	put_task_struct(p);
	return ret;

err_thread_create:
	__cleanup_sighand(p->sighand);
err_sighand:
	free_task_struct(p);
	kfree(kt);
	return ret;
}

int kernel_thread_init(void)
{
	int ret;
	struct task_struct *p;

	ret = liblinux_pal_thread_init_ex(kernel_thread_cleanup);
	if (ret < 0) {
		panic("kernel thread init failed: %d\n", ret);
	}

	liblinux_raw_thread_init();
	p = kernel_thread_init_data();
	if (p == NULL)
		panic("kernel thread init data failed\n");

	/* PAL thread model need run init in curent contex */
	return 1;
}

void __set_task_comm(struct task_struct *tsk, const char *buf, bool exec)
{
	int ret;

	task_lock(tsk);
	ret = liblinux_pal_thread_setname(&task_thread_info(tsk)->thread_handle, buf);
	if (ret < 0) {
		pr_warn("thread set name failed, ret = %d, name=%s\n", ret, buf);
	}
	strlcpy(tsk->comm, buf, sizeof(tsk->comm));
	task_unlock(tsk);
}

char *__get_task_comm(char *buf, size_t buf_size, struct task_struct *tsk)
{
	task_lock(tsk);
	strncpy(buf, tsk->comm, buf_size);
	task_unlock(tsk);
	return buf;
}
EXPORT_SYMBOL_GPL(__get_task_comm);

int __preempt_disabled[NR_CPUS] = { 0 };
EXPORT_SYMBOL(__preempt_disabled);

void kernel_thread_preempt_enable(void)
{
	liblinux_pal_preempt_resched();
}
EXPORT_SYMBOL(kernel_thread_preempt_enable);

void kernel_thread_preempt_disable(void)
{
	struct preempt_info *pi = current_thread_info()->preempt;
	int cpu;

	do {
		liblinux_pal_preempt_resched();
		barrier();
		if (__test_preempt_synced(pi))
			break;

		__set_preempt_disabled(pi);
		barrier();
		cpu = __get_preempt_cpuid(pi);
		if (__preempt_disabled[cpu] <= 0 && !__test_preempt_busy(pi))
			break;
	} while (true);
}
EXPORT_SYMBOL(kernel_thread_preempt_disable);

void liblinux_preempt_bypass(bool bypass)
{
	struct thread_info *ti = current_thread_info();
	ti->preempt_bypass = bypass;
}
EXPORT_SYMBOL(liblinux_preempt_bypass);

int liblinux_preempt_enable(void)
{
	preempt_enable();
	return raw_smp_processor_id();
}
EXPORT_SYMBOL(liblinux_preempt_enable);

/*
 * for ulibs, this will override `hm_user_preempt_disable`.
 * ulibs will hold lock(calling at `actv_threadpool_insert`) before `attr->actv_init`.
 * it will call `hm_user_futex_wait` (`hm_user_preempt_disable`) to goto blocked when
 * get lock failed.
 */
int liblinux_preempt_disable(void)
{
	if (unlikely(get_current() == NULL)) {
		BUG_ON(liblinux_thread_setup());
	}
	preempt_disable();
	return raw_smp_processor_id();
}
EXPORT_SYMBOL(liblinux_preempt_disable);

static unsigned int kthread_worker_caching_thresh = 10;
module_param(kthread_worker_caching_thresh, uint, S_IRUGO | S_IWUSR);

struct cached_worker {
	struct list_head list;
	struct kthread_worker worker;
};

static DEFINE_SPINLOCK(cached_worker_lock);
static LIST_HEAD(cached_worker_list);
static unsigned int cached_worker_nr = 0;

static void get_cached_worker(void *data, struct kthread_worker **worker,
			 int cpu, unsigned int flags, char *name)
{
	struct cached_worker *c_worker;
	static const struct sched_param param = { .sched_priority = 0 };

	spin_lock(&cached_worker_lock);
	if (!list_empty(&cached_worker_list)) {
		c_worker = list_entry(cached_worker_list.next, struct cached_worker, list);
		list_del_init(&c_worker->list);
		cached_worker_nr -= 1;
		spin_unlock(&cached_worker_lock);

		set_task_comm(c_worker->worker.task, name);
		sched_setscheduler_nocheck(c_worker->worker.task, SCHED_NORMAL, &param);
		if (cpu >= 0)
			set_cpus_allowed_ptr(c_worker->worker.task, cpumask_of(cpu));
		else
			set_cpus_allowed_ptr(c_worker->worker.task,
				housekeeping_cpumask(HK_FLAG_KTHREAD));
		c_worker->worker.flags = flags;
		*worker = &(c_worker->worker);
	} else {
		spin_unlock(&cached_worker_lock);

		c_worker = kzalloc(sizeof(struct cached_worker), GFP_KERNEL);
		if (!c_worker)
			*worker = NULL;
		else
			*worker = &(c_worker->worker);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_get_cached_worker, get_cached_worker);

static void try_cache_worker(void *data, struct kthread_worker *worker)
{
	struct cached_worker *c_worker;
	c_worker = container_of(worker, struct cached_worker, worker);

	spin_lock(&cached_worker_lock);
	if (cached_worker_nr >= kthread_worker_caching_thresh) {
		spin_unlock(&cached_worker_lock);

		kthread_stop(worker->task);
		kfree(c_worker);
	} else {
		list_add_tail(&c_worker->list, &cached_worker_list);
		cached_worker_nr += 1;
		spin_unlock(&cached_worker_lock);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_destroy_worker, try_cache_worker);

#define LIBLINUX_NET_BOOST 3

struct task_boost {
	const char *name;
	int boost;
};

static struct task_boost boost_task_whitelist[] = {
	{"pci_ete_rx", LIBLINUX_NET_BOOST},
};

static void hook_kthread_create_on_node(void* __data, struct task_struct *p)
{
	struct task_boost *booster;

	if (IS_ERR(p)) {
		return;
	}

	for (int i = 0; i < ARRAY_SIZE(boost_task_whitelist); i++) {
		booster = &boost_task_whitelist[i];
		if (strncmp(p->comm, booster->name, strlen(booster->name)) == 0) {
			liblinux_pal_thread_set_task_boost(&task_thread_info(p)->thread_handle,
							   booster->boost);
			break;
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_kthread_create_on_node, hook_kthread_create_on_node);

static void hook_thread_set_freezable(void *args, int *ret)
{
	*ret = liblinux_pal_thread_set_freezable();
}
INIT_VENDOR_HOOK(ldk_rvh_thread_set_freezable, hook_thread_set_freezable);

static void hook_thread_freezing(void *args, bool *ret)
{
	*ret = liblinux_pal_thread_freezing();
}
INIT_VENDOR_HOOK(ldk_rvh_thread_freezing, hook_thread_freezing);

static void hook_thread_try_to_freeze(void *args, int *ret)
{
	*ret = liblinux_pal_thread_try_to_freeze();
}
INIT_VENDOR_HOOK(ldk_rvh_thread_try_to_freeze, hook_thread_try_to_freeze);
