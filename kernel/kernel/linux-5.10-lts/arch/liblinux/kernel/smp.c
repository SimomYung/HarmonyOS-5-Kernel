// SPDX-License-Identifier: GPL-2.0-only

#define pr_fmt(fmt) "liblinux: smp: " fmt

#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/irqdomain.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#ifdef CONFIG_VENDOR_RPMB
#include <linux/rpmb/rpmb.h>
#endif
#ifdef CONFIG_DFX_FIQ
#include <linux/dfx_fiq.h>
#endif
#include <liblinux/pal.h>

#include <trace/hooks/liblinux.h>

/*
 * not supported here
 */
int setup_profiling_timer(unsigned int multiplier)
{
	return -EINVAL;
}

#define CALL_SYNC		1
#define CALL_ASYNC		2
#define CALL_ASYNC_NOALLOC	3
#define SMPCALL_DONE		0xdead

struct smp_call_handler {
	struct llist_head list;
	int cpu;
};

struct smp_sync_call_stat {
	unsigned long req_cnt;
	unsigned long res_cnt;
};
static struct smp_sync_call_stat g_smpcall_cnt[NR_CPUS];
#define smpcall_stat_syncreq_inc(cpu)	(g_smpcall_cnt[cpu].req_cnt++)
#define smpcall_stat_syncres_inc(cpu)	(g_smpcall_cnt[cpu].res_cnt++)
static inline void smpcall_stat_dump(int cpu)
{
	pr_info("cpu%d smpcall syncstat: req %lu, res: %lu\n", cpu,
		g_smpcall_cnt[cpu].req_cnt, g_smpcall_cnt[cpu].res_cnt);
}

static int show_smpcall_stat(struct seq_file *m, void *v)
{
	int i = 0;
	for (; i < NR_CPUS; ++i) {
		seq_printf(m, "cpu%d smpcall syncstat: req %lu, res: %lu\n", i,
			 g_smpcall_cnt[i].req_cnt, g_smpcall_cnt[i].res_cnt);
	}
	return 0;
}

static int smpcall_stat_open(struct inode *inode, struct file *file)
{
	return single_open(file, show_smpcall_stat, NULL);
}

static const struct file_operations smpcall_stat_fops = {
	.open		= smpcall_stat_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

DEFINE_PER_CPU_SHARED_ALIGNED(struct smp_call_handler, g_hdlr);

#define CSD_LOCK_TIMEOUT (5ULL * NSEC_PER_SEC)
static void wait_csd(int cpu, struct __call_single_data *csd)
{
	struct smp_call_handler *hdlr = per_cpu_ptr(&g_hdlr, cpu);
	int err;
	while (smp_load_acquire(&csd->flags) == CALL_SYNC) {
		err = liblinux_pal_futex_timedwait_ex((int *)(&csd->flags), CALL_SYNC, CSD_LOCK_TIMEOUT, 0, NULL);
		if (err == -ETIMEDOUT) {
			pr_warn("smpcall long time to wait: cpu %d, csd flags %u, llist empty %d\n", cpu, csd->flags, (int)llist_empty(&hdlr->list));
			smpcall_stat_dump(cpu);
#ifdef CONFIG_CSD_LOCK_WAIT_DEBUG
			if (smp_load_acquire(&csd->flags) == CALL_SYNC)
				liblinux_pal_smp_call_cpu(IPI_USER_CALL_FUNC, cpu);
#endif
		}
	}
}

void arch_send_call_function_single_ipi(int cpu)
{
	liblinux_pal_smp_call_cpu(IPI_USER_CALL_FUNC, cpu);
}

static void handle_smp_call(struct smp_call_handler *hdlr);

#define CS_UNLOCK	0
#define CS_LOCK		1
static int g_cslock;

void liblinux_flush_smp_call_function(void)
{
	struct smp_call_handler *hdlr;
	unsigned long flags;

	local_irq_save(flags);

	hdlr = this_cpu_ptr(&g_hdlr);
	handle_smp_call(hdlr);

	local_irq_restore(flags);
}

static void call_sync_lock(void)
{
	unsigned long flags;
	struct smp_call_handler *hdlr;

	lockdep_assert_preemption_disabled();

	if (likely(xchg(&g_cslock, CS_LOCK) == CS_UNLOCK))
		return;

	if (pr_warn_once("%s: serializing sync smp call\n", __func__))
		dump_stack();

	hdlr = this_cpu_ptr(&g_hdlr);
	local_irq_save(flags);
	for (;;) {
		/*
		 * The CPU holding g_cslock is waiting for smp call function to
		 * finish. Handle smp call on this CPU because we have disabled
		 * preemption and blocked execution of smpcall thread. This can
		 * avoid ABBA-deadlock and allow smp call to proceed.
		 */
		handle_smp_call(hdlr);

		if (xchg(&g_cslock, CS_LOCK) == CS_UNLOCK)
			break;
	}
	local_irq_restore(flags);
}

static void call_sync_unlock(void)
{
	smp_store_release(&g_cslock, CS_UNLOCK);
}

static inline int __call_cpu(int cpu, struct __call_single_data *csd, bool force)
{
	struct smp_call_handler *hdlr = per_cpu_ptr(&g_hdlr, cpu);
	int ret = 0;

	/*
	 * If a CPU is taken down after get mask_ofl_ipi
	 * in sync_rcu_exp_select_node_cpus, the smp_call_function_single
	 * will be invoked on the offline CPU.
	 * In this case, the call event may remain on the llist.
	 * When the CPU is taken on, the sgi interrupt needs to be
	 * forcibly triggered to process events on the llist.
	*/
	if (llist_add(&csd->llist, &hdlr->list) || force) {
		ret = liblinux_pal_smp_call_cpu(IPI_USER_CALL_FUNC, cpu);
	}
	return ret;
}

static int call_cpu(int cpu, struct __call_single_data *csd)
{
	return __call_cpu(cpu, csd, false);
}

static int call_cpu_force(int cpu, struct __call_single_data *csd)
{
	return __call_cpu(cpu, csd, true);
}

static void __smp_call_function_many(const struct cpumask *cpu_mask, smp_call_func_t func, void *info, smp_cond_func_t cond_func)
{
	int cpu;
	struct __call_single_data __percpu *data;

	data = alloc_percpu_gfp(struct __call_single_data, GFP_ATOMIC);
	if (!data) {
		pr_warn_once("%s: no memory\n", __func__);
		return;
	}

	call_sync_lock();
	for_each_cpu(cpu, cpu_mask) {
		struct __call_single_data *csd = per_cpu_ptr(data, cpu);

		if (cond_func && !cond_func(cpu, info)) {
			csd->flags = 0;
			continue;
		}
		csd->flags = CALL_SYNC;
		csd->func = func;
		csd->info = info;
		smpcall_stat_syncreq_inc(cpu);
		call_cpu(cpu, csd);
	}

	for_each_cpu(cpu, cpu_mask) {
		struct __call_single_data *csd = per_cpu_ptr(data, cpu);

		wait_csd(cpu, csd);
	}
	call_sync_unlock();

	free_percpu(data);
}

static int call_cpu_async(int cpu, smp_call_func_t func, void *info)
{
	struct __call_single_data *csd = kmalloc(sizeof(*csd), GFP_ATOMIC);

	if (!csd) {
		pr_warn_once("%s: no memory\n", __func__);
		return -ENOMEM;
	}

	csd->flags = CALL_ASYNC;
	csd->func = func;
	csd->info = info;
	return call_cpu(cpu, csd);
}

static void __smp_call_function_many_async(const struct cpumask *cpu_mask, smp_call_func_t func, void *info, smp_cond_func_t cond_func)
{
	int cpu;

	for_each_cpu(cpu, cpu_mask) {
		if (cond_func && !cond_func(cpu, info))
			continue;
		call_cpu_async(cpu, func, info);
	}
}

static void
smp_call_function_many_cond(const struct cpumask *mask,
				 smp_call_func_t func, void *info,
				 bool wait, smp_cond_func_t cond_func)
{
	int this_cpu = smp_processor_id();
	struct cpumask cpu_mask = {0};

	cpumask_and(&cpu_mask, mask, cpu_online_mask);
	__cpumask_clear_cpu(this_cpu, &cpu_mask);

	if (wait)
		__smp_call_function_many(&cpu_mask, func, info, cond_func);
	else
		__smp_call_function_many_async(&cpu_mask, func, info, cond_func);
}


int __override smp_call_function_single(int cpu, smp_call_func_t func, void *info, int wait)
{
	int ret = 0;
	int this_cpu;

	this_cpu = get_cpu();
	if (this_cpu == cpu) {
		unsigned long flags;
		/*
		 * We can unlock early even for the synchronous on-stack case,
		 * since we're doing this from the same CPU..
		 */
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
		put_cpu();
		return 0;
	}

	if ((unsigned)cpu >= nr_cpu_ids || !cpu_online(cpu)) {
		put_cpu();
		return -ENXIO;
	}

	if (wait) {
		struct __call_single_data csd = {
			.func = func,
			.info = info,
			.flags = CALL_SYNC,
		};
		call_sync_lock();
		smpcall_stat_syncreq_inc(cpu);
		ret = call_cpu(cpu, &csd);
		wait_csd(cpu, &csd);
		call_sync_unlock();
	} else {
		ret = call_cpu_async(cpu, func, info);
	}

	put_cpu();
	return ret;
}

int __override smp_call_function_single_async(int cpu, struct __call_single_data *csd)
{
	int ret = 0;
	preempt_disable();
	csd->flags = CALL_ASYNC_NOALLOC;
	ret = call_cpu(cpu, csd);
	preempt_enable();
	return ret;
}

int invoke_on_cpu_single_async(int cpu, struct __call_single_data *csd)
{
	preempt_disable();
	csd->flags = CALL_ASYNC_NOALLOC;
	call_cpu_force(cpu, csd);
	preempt_enable();
	return 0;
}

void __override smp_call_function_many(const struct cpumask *mask,
			    smp_call_func_t func, void *info, bool wait)
{
	smp_call_function_many_cond(mask, func, info, wait, NULL);
}

void __override on_each_cpu_mask(const struct cpumask *mask, smp_call_func_t func,
			void *info, bool wait)
{
	int cpu = get_cpu();

	smp_call_function_many(mask, func, info, wait);
	if (cpumask_test_cpu(cpu, mask)) {
		unsigned long flags;
		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
	put_cpu();
}

void __override on_each_cpu_cond_mask(smp_cond_func_t cond_func, smp_call_func_t func,
			   void *info, bool wait, const struct cpumask *mask)
{
	int cpu = get_cpu();

	smp_call_function_many_cond(mask, func, info, wait, cond_func);
	if (cpumask_test_cpu(cpu, mask) && cond_func(cpu, info)) {
		unsigned long flags;

		local_irq_save(flags);
		func(info);
		local_irq_restore(flags);
	}
	put_cpu();
}

static void handle_call_data(struct __call_single_data *csd, int cpu)
{
	csd->func(csd->info);

	if (csd->flags == CALL_SYNC) {
		smpcall_stat_syncres_inc(cpu);
		smp_store_release(&csd->flags, SMPCALL_DONE);
		liblinux_pal_futex_wake((int *)(&csd->flags));
	} else if (csd->flags == CALL_ASYNC) {
		kfree(csd);
	} else if (csd->flags == CALL_ASYNC_NOALLOC) {
		/* nothing to do */
	} else {
		panic("invalid csd data: %u\n", csd->flags);
	}
}

static void handle_smp_call(struct smp_call_handler *hdlr)
{
	struct llist_node *node;
	struct __call_single_data *csd;

	node = llist_del_all(&hdlr->list);
	node = llist_reverse_order(node);
	while (node != NULL) {
		csd = llist_entry(node, struct __call_single_data, llist);
		node = llist_next(node);
		handle_call_data(csd, hdlr->cpu);
	}
}

static void do_handle_smp_call_ipi(void)
{
	struct smp_call_handler *hdlr = this_cpu_ptr(&g_hdlr);
	int cpu_online_flag, cpu_active_flag;

	for (;;) {
		if (llist_empty(&hdlr->list))
			break;

		cpu_online_flag = cpu_online(hdlr->cpu);
		cpu_active_flag = cpu_active(hdlr->cpu);
		local_irq_disable();
		if (cpu_online_flag)
			irq_enter();
		handle_smp_call(hdlr);
#ifdef CONFIG_LIBLINUX_NO_HZ
		/* emulate tick to accelerate RCU */
		if (cpu_active_flag && hdlr->cpu)
			rcu_sched_clock_irq(1);
#endif
		if (cpu_online_flag)
			irq_exit();

		/* for expedited RCU grace period */
		rcu_note_context_switch(false);
		local_irq_enable();
	}
}

void __init liblinux_smp_init(void)
{
	int cpu;

	for_each_cpu(cpu, cpu_possible_mask) {
		int pid;
		struct smp_call_handler *hdlr = per_cpu_ptr(&g_hdlr, cpu);

		init_llist_head(&hdlr->list);
		hdlr->cpu = cpu;
	}
}

static int __ipi_base = 0;
static int __nr_ipi = 0;

static void do_handle_IPI(int ipinr)
{
	switch (ipinr) {
	#ifdef CONFIG_VENDOR_RPMB
		case IPI_SECURE_RPMB:
			local_irq_disable();
			irq_enter();
			vendor_rpmb_active();
			irq_exit();
			local_irq_enable();
			break;
	#endif

	#ifdef CONFIG_DFX_FIQ
		case IPI_MNTN_INFORM:
			local_irq_disable();
			irq_enter();
			dfx_mntn_inform();
			irq_exit();
			local_irq_enable();
			break;
	#endif
		case IPI_USER_CALL_FUNC:
			do_handle_smp_call_ipi();
			break;
	default:
		pr_crit("CPU%u: Unknown IPI message 0x%x\n", raw_smp_processor_id(), ipinr);
		break;
	}
}

static int ipi_handler(int irq, void *data)
{
	do_handle_IPI(irq);
	return IRQ_HANDLED;
}

static int __enable_ipi(unsigned int cpu)
{
	int i;

	BUG_ON(raw_smp_processor_id() != cpu);
	for (i = 0; i < __nr_ipi; i++) {
		liblinux_register_ipi_handler(__ipi_base + i, ipi_handler, cpu);
	}

	return 0;
}

static int __disable_ipi(unsigned int cpu)
{
	int i;

	BUG_ON(raw_smp_processor_id() != cpu);
	for (i = 0; i < __nr_ipi; i++) {
		liblinux_unregister_ipi(__ipi_base + i, cpu);
	}

	return 0;
}

void register_user_call_func_ipi(unsigned int cpu)
{
	liblinux_register_ipi_handler(IPI_USER_CALL_FUNC, ipi_handler, cpu);
}

/* register from ipi_base to ipi_base + nr_ipi,
 * caution: this is different from linux */
void set_smp_ipi_range(int ipi_base, int nr_ipi)
{
	__ipi_base = ipi_base;
	__nr_ipi = nr_ipi;

	cpuhp_setup_state(CPUHP_AP_ONLINE_DYN, "ipi:register",
			  __enable_ipi, __disable_ipi);
}

EXPORT_SYMBOL(set_smp_ipi_range);

static __init int ipi_init(void)
{
	set_smp_ipi_range(IPI_SECURE_RPMB, IPI_MNTN_INFORM - IPI_SECURE_RPMB + 1);
	debugfs_create_file("ldk_smpcall_stat", S_IRUSR, NULL, NULL, &smpcall_stat_fops);

	return 0;
}
device_initcall(ipi_init);

static int __init ipi_user_call_fun_init(void)
{
	/*
	* Only for the boot cpu.
	* The secondary cpus do this in __cpu_up.
	*/
	register_user_call_func_ipi(smp_processor_id());

	return 0;
}
early_initcall(ipi_user_call_fun_init);
