// SPDX-License-Identifier: GPL-2.0-only

#define pr_fmt(fmt) "liblinux: " fmt

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/cpuhotplug.h>
#include <linux/irq.h>
#include <linux/percpu-rwsem.h>
#include <linux/tick.h>
#include <linux/topology.h>
#include <linux/init.h>
#include <asm/smp.h>
#include <asm/cputype.h>
#include <liblinux/pal.h>

static int cpu_nr;

static void init_cpu_topo(unsigned int *topo, unsigned int size)
{
	(void)liblinux_pal_cpu_topology(topo, size);
}

#define CPU_TOPO_LEVEL 3
/*
 * see drivers/base/arch_topology.c
 * in liblinux, llc_id is not init, SCHED_SMT_USER_HIDE is not related
 */
void update_siblings_masks(unsigned int cpuid)
{
	struct cpu_topology *cpu_topo, *cpuid_topo = &cpu_topology[cpuid];
	int cpu;

	/* update core and thread sibling masks */
	for_each_online_cpu(cpu) {
		cpu_topo = &cpu_topology[cpu];

		if (cpuid_topo->package_id != cpu_topo->package_id)
			continue;

		cpumask_set_cpu(cpuid, &cpu_topo->core_sibling);
		cpumask_set_cpu(cpu, &cpuid_topo->core_sibling);

		if (cpuid_topo->core_id != cpu_topo->core_id)
			continue;

		cpumask_set_cpu(cpuid, &cpu_topo->thread_sibling);
		cpumask_set_cpu(cpu, &cpuid_topo->thread_sibling);
	}
}

void store_cpu_topology(unsigned int cpuid)
{
	update_siblings_masks(cpuid);
}

static void set_cpu_topo(unsigned int cpu, unsigned int *topo)
{
	struct cpu_topology *cpuid_topo = &cpu_topology[cpu];

	/* topology layout:
	 *      thread_id   core_id   package_id
	 * cpu     x           x          x
	 */
	cpuid_topo->thread_id = topo[0];
	cpuid_topo->core_id = topo[1];
	cpuid_topo->package_id = topo[2];
	pr_info("cpu topology: cpu=%u,  topo=%u, %u, %u\n",
			cpu, topo[0], topo[1], topo[2]);

	/* same as `clear_cpu_topology()` */
	cpuid_topo->llc_id = -1;
	cpumask_clear(&cpuid_topo->llc_sibling);
	cpumask_set_cpu(cpu, &cpuid_topo->llc_sibling);
	cpumask_clear(&cpuid_topo->core_sibling);
	cpumask_set_cpu(cpu, &cpuid_topo->core_sibling);
	cpumask_clear(&cpuid_topo->thread_sibling);
	cpumask_set_cpu(cpu, &cpuid_topo->thread_sibling);
}

void __setup_nr_cpu(void)
{
	static unsigned long online_mask;
	unsigned long cpu_map[NR_CPUS];
	unsigned int topo[NR_CPUS][CPU_TOPO_LEVEL];
	int i;

	cpu_nr = liblinux_pal_cpu_config(&online_mask, cpu_map, UL(NR_CPUS),
					    1 /* always enable preempt */);
	if (cpu_nr < 0) {
		panic("get cpu config failed, ret=%d\n", cpu_nr);
	}

	init_cpu_possible(cpu_none_mask);
	init_cpu_online(cpu_none_mask);
	init_cpu_topo(&topo[0][0], sizeof(topo));
	for (i = 0; i < cpu_nr; i++) {
		set_cpu_possible(i, true);
		set_cpu_present(i, (online_mask & (1 << i)) == (1 << i));
		set_cpu_logical_map(i, cpu_map[i] & MPIDR_HWID_BITMASK);
		set_cpu_topo(i, topo[i]);
	}
}

static DEFINE_PER_CPU(struct cpu, cpu_devices);
static int __init topology_init(void)
{
	int i;
	struct cpu *cpu = NULL;

	for_each_possible_cpu(i) {
		cpu = &per_cpu(cpu_devices, i);
		cpu->hotpluggable = true;
		if (register_cpu(cpu, i) != 0)
			panic("Failed to register CPU device");
	}
	return 0;
}
subsys_initcall(topology_init)

static DECLARE_COMPLETION(cpu_running);

static void __secondary_start_kernel(void *data)
{
	int cpu = smp_processor_id();

	/*
	 * Enable GIC and timers.
	 */
	notify_cpu_starting(cpu);
	store_cpu_topology(cpu);

	pr_info("CPU%u: Booted secondary processor 0x%010lx\n",
			cpu, (unsigned long)-1);

	set_cpu_online(cpu, true);
	complete(&cpu_running);
}

/*
 * simulate sd, store in static variable because info is fixed
 */
static struct __call_single_data sd_cpu_up = {
	.func = __secondary_start_kernel,
	.info = NULL,
};

extern void cpuhp_online_idle_by_cpu(unsigned int cpu);
int __cpu_up(unsigned int cpu, struct task_struct *idle)
{
	/*
	* Registered ipi and sets the sched-level before
	* invoke_on_cpu_single_async calling immediately later.
	*/
	register_user_call_func_ipi(cpu);
	liblinux_pal_process_set_slv(cpu, 0);

	invoke_on_cpu_single_async(cpu, &sd_cpu_up);

	wait_for_completion_timeout(&cpu_running,
				    msecs_to_jiffies(5000));
	if (cpu_online(cpu)) {
		cpuhp_online_idle_by_cpu(cpu);
		return 0;
	}

	panic("CPU%u: failed to come online\n", cpu);
	return -EIO;
}

static void __idle_cpu_die(void *data)
{
	local_irq_disable();
	tick_nohz_idle_stop_tick();
	cpuhp_report_idle_dead();
	cpu_report_death();
	rcu_note_context_switch(false);
	local_irq_enable();
}

extern void lnxbase_invoke_on_cpu(unsigned int cpu,
				   void (*callback)(void *), void *data);
int __cpu_disable(void)
{
	int cpu = smp_processor_id();

	/*
	 * Take this CPU offline.  Once we clear this, we can't return,
	 * and we must not schedule until we're ready to give up the cpu.
	 */
	set_cpu_online(cpu, false);

	lnxbase_invoke_on_cpu(cpu, __idle_cpu_die, NULL);

	/*
	* unregister here by considering that smp_call_function should
	* not be called after __cpu_disable.
	*/
	liblinux_unregister_ipi(IPI_USER_CALL_FUNC, cpu);
	irq_migrate_all_off_this_cpu();
	return 0;
}

void __cpu_die(unsigned int cpu)
{
	if (!cpu_wait_death(cpu, 5)) {
		pr_crit("CPU%u: cpu didn't die\n", cpu);
		return;
	}
	pr_notice("CPU%u: shutdown\n", cpu);
}

#ifdef CONFIG_LIBLINUX_NO_HZ
static atomic_t idle_nr;
static bool cpu0_tick_stopped;

int liblinux_nohz_idle_enter(void)
{
	int ret = 0;
	int this_cpu;

	local_irq_disable();
	rcu_note_context_switch(false);
	local_irq_enable();

	this_cpu = get_cpu();
	if (!this_cpu) {
		int curr;

		do {
			curr = atomic_read(&idle_nr);
			if (curr + 1 != cpu_nr) {
				ret = -EAGAIN;
				break;
			}
		} while (atomic_cmpxchg(&idle_nr, curr, curr + 1) != curr);

		if (!ret) {
			tick_nohz_idle_enter();
			raw_local_irq_disable();
			tick_nohz_idle_stop_tick();
			cpu0_tick_stopped = tick_nohz_tick_stopped();
			if (!cpu0_tick_stopped)
				atomic_dec(&idle_nr);
			rcu_idle_enter();
			raw_local_irq_enable();
		}
	} else {
		atomic_inc(&idle_nr);
		raw_local_irq_disable();
		rcu_idle_enter();
		raw_local_irq_enable();
	}
	put_cpu();

	return ret;
}
EXPORT_SYMBOL_GPL(liblinux_nohz_idle_enter);

static void empty_fn(void *info)
{
	/* Empty function to wake up CPU 0. */
}

void liblinux_nohz_idle_exit(void)
{
	int this_cpu = get_cpu();

	raw_local_irq_disable();
	rcu_idle_exit();
	raw_local_irq_enable();

	if (!this_cpu) {
		if (cpu0_tick_stopped)
			atomic_dec(&idle_nr);
		tick_nohz_idle_exit();
	} else if (atomic_fetch_dec(&idle_nr) == cpu_nr) {
		smp_call_function_single(0, empty_fn, NULL, 0);
	}

	put_cpu();
}
EXPORT_SYMBOL_GPL(liblinux_nohz_idle_exit);
#endif

unsigned int cpufreq_quick_get(unsigned int cpuid)
{
    return liblinux_pal_cpufreq_quick_get(cpuid);
}
EXPORT_SYMBOL_GPL(cpufreq_quick_get);

unsigned int cpufreq_quick_get_max(unsigned int cpuid)
{
    return liblinux_pal_cpufreq_quick_get_max(cpuid);
}
EXPORT_SYMBOL_GPL(cpufreq_quick_get_max);

unsigned int cpufreq_get_hw_max_freq(unsigned int cpuid)
{
    return liblinux_pal_cpufreq_quick_get_hw_max(cpuid);
}
EXPORT_SYMBOL_GPL(cpufreq_get_hw_max_freq);

unsigned long long get_cpu_idle_time(unsigned int cpuid, unsigned long long *wall, int io_busy)
{
    return liblinux_pal_get_cpu_idle_time(cpuid, wall, io_busy);
}
EXPORT_SYMBOL_GPL(get_cpu_idle_time);
