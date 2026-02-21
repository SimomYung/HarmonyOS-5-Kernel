// SPDX-License-Identifier: GPL-2.0-only
#include <linux/compiler.h>
#include <linux/cpuhotplug.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/memblock.h>
#include <linux/mman.h>
#include <linux/of_clk.h>
#include <linux/of_fdt.h>
#include <linux/slab.h>
#include <linux/start_kernel.h>
#include <linux/user_namespace.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h>
#include <asm/kasan.h>
#include <asm/setup.h>
#include <asm/sections.h>
#include <asm/stackprotector.h>
#include <asm/set_memory.h>
#include <trace/hooks/liblinux.h>
#include <securec.h>

extern int kernel_thread_init(void);
extern void __setup_nr_cpu(void);
extern void __init arch_page_alloc_init(void);
extern void __init chrdev_init(void);

void __weak liblinux_cred_init(void) {}
void __weak liblinux_load_extra_modules(void) {}

void *__fdt_pointer __initdata;

static
void hook_kernel_init_done(void *args, enum system_states state)
{
	struct completion *done = (struct completion *)args;
	complete(done);
	do_exit(0);
}

void __ref liblinux_do_initcalls(void)
{
	int ret;
	DECLARE_COMPLETION_ONSTACK(done);

	liblinux_cred_init();

	ret = register_trace_ldk_rvh_kernel_init_done(hook_kernel_init_done, &done);
	if (ret < 0) {
		panic("liblinux: register kernel_init done hook failed, ret=%d\n",
		      ret);
	}

	/* Initialize smp_call_function facilities before creating kernel_init thread. */
	liblinux_smp_init();

	/* rest_init assume preempt disabled */
	preempt_disable();
	rest_init();
	preempt_enable();

	wait_for_completion(&done);
}

void __init liblinux_vendor_hook_init(void)
{
	inithook_t *fn = NULL;

	for (fn = __vendor_hook_initcall_start; fn < __vendor_hook_initcall_end; fn++) {
		(*fn)();
	}
}

#ifdef __clang__
#define __no_stack_protector	__attribute__((no_stack_protector))
#else
#define __no_stack_protector	__attribute__((optimize("-fno-stack-protector")))
#endif

#ifdef CONFIG_LIBLINUX_C_ASYNC_PROBE
# define LIBLINUX_CMDLINE_APPEND_SIZE	1024
/*
 * For mixed chips from different manufacturers should in a same group
 * to avoid resource (e.g. gpio pins) racing.
 */
# define LIBLINUX_APPENDED_CMDLINE									\
	" driver_async_probe=smartpakit_i2c:,"								\
	"hisi_nfc,pn544,thn31," /* NFC */								\
	"aw32280:cps2021:cps2023:scharger_v700_drv:hi6526_charger:bq25970:rt9466:bq2560x_charger:"	\
	"hl7139:ltc7820:nu2205:rt9759:sc8510:sc8545:sc8546:sc8562:sc8571:sm5450:sy6513," /* charger */	\
	"wireless_cps4029:wireless_cps4067:wireless_cps4057:wireless_cps7181:wireless_cps8601:"		\
	"wireless_idtp9415:wireless_mt5735:wireless_mt5727:wireless_mt5785:wireless_mt5806:"		\
	"wireless_nu1619:wireless_stwlc33:wireless_stwlc68:wireless_stwlc88," /* wireless charger */	\
	"awinic_haptic:haptic_hv:cs_haptic:sih_haptic_688X" /* vibrator */
#else
# define LIBLINUX_CMDLINE_APPEND_SIZE	128
# define LIBLINUX_APPENDED_CMDLINE	" "
#endif

void __ref __no_stack_protector
liblinux_kernel_init(const char *cmdline)
{
	asm volatile (			\
		"1: nop\n"		\
		"2:\n"			\
		_ASM_EXTABLE(1b, 2b));

	/* thread model may run init in seperated context */
	if (kernel_thread_init() == 0) {
		return;
	}

	/* clear init thread preempt_count before do-init */
	init_idle_preempt_count(current, 0);
	__setup_nr_cpu();

	/* Set up initial canary early, so that start_kernel() can return */
	boot_init_stack_canary();

	if (cmdline != NULL) {
		int rc;
		char append[LIBLINUX_CMDLINE_APPEND_SIZE] = LIBLINUX_APPENDED_CMDLINE;
		size_t len = strnlen(cmdline, COMMAND_LINE_SIZE);

#ifdef CONFIG_LIBLINUX_NO_HZ
		size_t alen = strnlen(append, LIBLINUX_CMDLINE_APPEND_SIZE);
		rc = snprintf_s(append + alen, sizeof(append) - alen, sizeof(append) - alen - 1,
				" nohz_full=1-%u", cpumask_weight(cpu_possible_mask) - 1);
		if (rc < 0)
			panic("liblinux: snprintf_s failed\n");
#endif

		if (len + sizeof(append) > COMMAND_LINE_SIZE)
			panic("liblinux: cmdline is too long: %zu\n", len);
		rc = memcpy_s(boot_command_line, COMMAND_LINE_SIZE, cmdline, len);
		if (rc)
			panic("liblinux: memcpy_s failed\n");
		rc = memcpy_s(boot_command_line + len, COMMAND_LINE_SIZE - len, append, sizeof(append));
		if (rc)
			panic("liblinux: memcpy_s failed\n");
	}

	/* start_kernel assume IRQ disabled */
	local_irq_disable();
	start_kernel();
	local_irq_enable();
	preempt_count_set(0);
}

/*
 * last step of `start_kernel()`, the real rest_init was moved to
 * liblinux_do_initcalls()
 */
void __init arch_call_rest_init(void)
{
	/*
	 * set pwd/root of init_task to make sure all kthreads
	 * live in the same rootfs.
	 */
	set_fs_pwd(init_task.fs, &current->fs->pwd);
	set_fs_root(init_task.fs, &current->fs->root);
}

void __init __no_sanitize_address setup_arch(char **cmdline_p)
{
	parse_early_param();

	/* early page alloc init */
	arch_page_alloc_init();

	*cmdline_p = boot_command_line;

	if (!__fdt_pointer || !early_init_dt_scan(__fdt_pointer)) {
		pr_crit("\nError: invalid device tree blob\n");
		while (true)
			cpu_relax();
	}

	/* of reserved mem*/
	early_init_fdt_scan_reserved_mem();

	unflatten_device_tree();
}

extern int liblinux_kasan_populate_shadow(const void *addr, size_t len);
extern int liblinux_populate_bss(const void *start, const void *stop);

void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
#ifdef CONFIG_KASAN
	liblinux_kasan_populate_shadow(kasan_mem_to_shadow((void *)(unsigned long)base),
				       size >> KASAN_SHADOW_SCALE_SHIFT);
#endif
	liblinux_populate_bss(__bss_start, __bss_stop);
}

void set_task_stack_end_magic(struct task_struct *tsk)
{
	unsigned long *stackend;

	stackend = end_of_stack(tsk);
	*stackend = STACK_END_MAGIC;    /* for overflow detection */
}

void free_initmem(void)
{
	/* skip free initmem */
}

/* dummy inits for start_kernel() */
void __init anon_vma_init(void) {}
void __init init_IRQ(void) {}
void __init nsfs_init(void) {}
void __init pagecache_init(void) {}
void __init pid_idr_init(void) {}

static void sighand_ctor(void *data)
{
	struct sighand_struct *sighand = data;

	spin_lock_init(&sighand->siglock);
	init_waitqueue_head(&sighand->signalfd_wqh);
}

void __init proc_caches_init(void)
{
	fs_cachep = kmem_cache_create("fs_cache",
			sizeof(struct fs_struct), 0,
			SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_ACCOUNT,
			NULL);
	sighand_cachep = kmem_cache_create("sighand_cache",
			sizeof(struct sighand_struct), 0,
			SLAB_HWCACHE_ALIGN|SLAB_PANIC|SLAB_TYPESAFE_BY_RCU|
			SLAB_ACCOUNT, sighand_ctor);
}

int  __init rand_initialize(void) { return 0; }
void __init sched_clock_init(void) {}
void __init sched_init(void)
{
	wait_bit_init();
}
void __init setup_log_buf(int early) {}
void __init signals_init(void) {}
void __init smp_prepare_boot_cpu(void) {}

void __init smp_prepare_cpus(unsigned int max_cpus)
{
	unsigned int this_cpu = smp_processor_id();

	store_cpu_topology(this_cpu);
}

__u32 __weak arch_timer_get_rate(void)
{
	return 0;
}
void __init time_init(void)
{
	u32 arch_timer_rate;

	of_clk_init(NULL);

	arch_timer_rate = arch_timer_get_rate();
	if (arch_timer_rate) {
		/* Calibrate the delay loop directly */
		lpj_fine = arch_timer_rate / HZ;
	}
}
void __init trap_init(void) {}
/* dummy init for kernel_init_freeable() */
void __init dtd_init_rodata(void) {}
void __init sched_init_smp(void) {}
void mark_rodata_ro(void)
{
	unsigned long size = __end_ro_after_init - __start_ro_after_init;

	liblinux_pal_set_pageattr(__start_ro_after_init, size >> PAGE_SHIFT, PROT_READ, PROT_WRITE | PROT_EXEC);
	pr_info("Write protected read-only-after-init data: %luk\n", size >> 10);
}
void __usermodehelper_set_disable_depth(enum umh_disable_depth depth)
{
	if (depth == UMH_ENABLED) {
		liblinux_load_extra_modules();
	}
}
void __weak arch_cpu_idle_prepare(void) {}
void cpu_startup_entry(enum cpuhp_state state)
{
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	/* for linux, this cpu will enter idle, but we just exit */
}
void hck_dfx_kdump_register(void) {}

/* fs init namespace */
void __override prepare_namespace(void) {}

u64 __cpu_logical_map[NR_CPUS] = { [0 ... NR_CPUS-1] = INVALID_HWID };

u64 cpu_logical_map(int cpu)
{
	return __cpu_logical_map[cpu];
}

bool arch_match_cpu_phys_id(int cpu, u64 phys_id)
{
	return phys_id == cpu_logical_map(cpu);
}

/* fs/proc/root.c */
void __init proc_self_init(void)
{
	/* ldk don't has `self/net` dir. create it directly */
	proc_mkdir("net", NULL);
}
void __init proc_thread_self_init(void) {}
/* fs/proc/base.c */
void __init set_proc_pid_nlink(void) {}

#ifdef CONFIG_MEMORY_HOTPLUG_SPARSE
/* driver/base/memory.c */
void __init memory_dev_init(void) {}
#endif
