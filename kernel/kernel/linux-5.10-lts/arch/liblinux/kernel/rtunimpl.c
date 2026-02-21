// SPDX-License-Identifier: GPL-2.0-only
#include <linux/compiler.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <liblinux/unimpl.h>

/*
 * This can be empty if ldk net stack is enabled.
 * To support netlink, realize the func if ldk net stack is disabled.
 * */
int liblinux_pal_netlink_init(struct netlink_pal_handler *hdr)
{
	return 0;
}

LIBLINUX_UNIMPL(drop_slab)

/* arch */
LIBLINUX_UNIMPL(show_regs)
LIBLINUX_UNIMPL(__show_regs)

/* arch/xxx/kernel/smp.c */
LIBLINUX_UNIMPL(arch_send_call_function_ipi_mask)
#ifndef CONFIG_LIBLINUX_ASYNC_SYSFS
LIBLINUX_UNIMPL(smp_cpus_done)
#endif

/* fs related */
LIBLINUX_UNIMPL_RET(get_cmdline, int, 0)

LIBLINUX_UNIMPL(wakeup_flusher_threads)

/* kernel/fork.c */
LIBLINUX_UNIMPL_RET(nr_processes, int, 0);

/* depends by kernel/kthread.c */
LIBLINUX_UNIMPL(sync_mm_rss)

/* kernel/smp.c */
LIBLINUX_UNIMPL(smp_send_stop)

/* drivers/cpufreq/cpufreq.c */
struct em_perf_domain;
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(em_cpu_get, struct em_perf_domain *, NULL)

/* reboot */
LIBLINUX_GPL_EXPORTED_UNIMPL(orderly_poweroff)
enum reboot_mode {
	REBOOT_UNDEFINED = -1,
	REBOOT_COLD = 0,
	REBOOT_WARM,
	REBOOT_HARD,
	REBOOT_SOFT,
	REBOOT_GPIO,
};

enum reboot_mode        reboot_mode;

/* drivers/char/random.c */
LIBLINUX_GPL_EXPORTED_UNIMPL(add_disk_randomness)
LIBLINUX_EXPORTED_UNIMPL_RET(add_random_ready_callback, int, -EALREADY)
LIBLINUX_EXPORTED_UNIMPL_RET(wait_for_random_bytes, int, 0)
LIBLINUX_UNIMPL(rand_initialize_disk)
LIBLINUX_GPL_EXPORTED_UNIMPL(add_input_randomness);

/* drivers/cpuidle/% */
LIBLINUX_UNIMPL(cpuidle_pause)
LIBLINUX_UNIMPL(cpuidle_resume)

LIBLINUX_EXPORTED_UNIMPL_RET(register_sysrq_key, int, 0)


/* of */
LIBLINUX_UNIMPL_RET(of_setup_earlycon, int, 0)

/* kernel/power/% */
LIBLINUX_UNIMPL(__pm_pr_dbg)
LIBLINUX_UNIMPL_RET(pm_print_times_enabled, bool, false)
LIBLINUX_EXPORTED_UNIMPL(pm_vt_switch_required)
LIBLINUX_EXPORTED_UNIMPL(pm_vt_switch_unregister)
struct device;
LIBLINUX_EXPORTED_UNIMPL_RET(dev_pm_domain_attach_by_name, struct device *, NULL)

/* ipa thermal */
#ifndef CONFIG_LDK_THERMAL
LIBLINUX_UNIMPL(update_pid_value)
LIBLINUX_GPL_EXPORTED_UNIMPL(update_actor_weights)
LIBLINUX_GPL_EXPORTED_UNIMPL(restore_actor_weights)
LIBLINUX_UNIMPL(ipa_freq_limit_reset)
LIBLINUX_UNIMPL(ipa_parse_pid_parameter)
LIBLINUX_EXPORTED_UNIMPL_RET(ipa_freq_limit, int, 0)
LIBLINUX_UNIMPL_RET(boost_show, ssize_t, 0)
LIBLINUX_UNIMPL_RET(boost_store, ssize_t, 0)
LIBLINUX_UNIMPL_RET(boost_timeout_show, ssize_t, 0)
LIBLINUX_UNIMPL_RET(boost_timeout_store, ssize_t, 0)
LIBLINUX_UNIMPL_RET(cur_power_show, ssize_t, 0)
LIBLINUX_UNIMPL_RET(cur_enable_show, ssize_t, 0)
LIBLINUX_UNIMPL_RET(cur_enable_store, ssize_t, 0)
LIBLINUX_UNIMPL(get_cur_power)
LIBLINUX_UNIMPL(set_tz_type)

LIBLINUX_UNIMPL_RET(power_actor_set_powers, int, 0)
LIBLINUX_UNIMPL_RET(of_parse_ipa_sensor_index_table, int, 0)
#endif
LIBLINUX_UNIMPL_RET(thermal_of_get_cdev_type, bool, false)

/* kernel/printk/ */
LIBLINUX_UNIMPL(kmsg_dump)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(kmsg_dump_register, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(kmsg_dump_unregister, int, 0)
LIBLINUX_UNIMPL_RET(kmsg_dump_reason_str, char *, NULL)
LIBLINUX_GPL_EXPORTED_UNIMPL(kmsg_dump_rewind)

#ifdef CONFIG_DFX_MEMCHECK
LIBLINUX_UNIMPL_RET(smaps_get_pss, u64, 0)
LIBLINUX_UNIMPL_RET(arch_vma_name, const char *, NULL)
#endif

unsigned long lnxbase_shrink_mem(int size) { return 0; }

/* kernel/sched/core.c */
LIBLINUX_UNIMPL(rt_mutex_setprio)

/* kernel/time/posix-timers.c */
LIBLINUX_UNIMPL_RET(common_timer_del, int, 0)

/* kernel/pid.c */
LIBLINUX_EXPORTED_UNIMPL_RET(find_task_by_vpid, void *, NULL)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(find_vpid, void *, NULL)
LIBLINUX_EXPORTED_UNIMPL_RET(__task_pid_nr_ns, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(get_task_pid, void *, NULL)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(pid_nr_ns, int, 0)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(find_pid_ns, void *, NULL)

/* for basic platform drivers */
LIBLINUX_UNIMPL_RET(lpcpu_get_idle_cpumask, unsigned int, 0);
/* platform_source/cee/drivers/drg/drg.c */
LIBLINUX_EXPORTED_UNIMPL(drg_devfreq_register)
LIBLINUX_EXPORTED_UNIMPL(drg_devfreq_unregister)

/* kernel/ksysfs.c */
int file_caps_enabled = 0;
#ifdef CONFIG_DPU_FB_V600
/* don't support `free_reserved_page` */
extern uint32_t g_logo_buffer_size;
extern uint32_t g_logo_buffer_base;
void dpufb_free_logo_buffer(void *);
__override void dpufb_free_logo_buffer(void *fb)
{
	g_logo_buffer_size = 0;
	g_logo_buffer_base = 0;
}
#endif

/* watchdog */
struct rdr_arctimer_s {
	u32 cntv_ctl_el0;
	u32 cntv_tval_el0;
	u32 cntp_ctl_el0;
	u32 cntp_tval_el0;
	u32 cntfrq_el0;

	u64 cntv_cval_el0;
	u64 cntp_cval_el0;
	u64 cntvct_el0;
	u64 cntpct_el0;
};

struct rdr_arctimer_s g_rdr_arctimer_record;
void __iomem *wdt_base;
LIBLINUX_UNIMPL(dfx_wdt_dump)
LIBLINUX_UNIMPL_RET(watchdog_hiwdt_hardlockup_happen, bool, false)
LIBLINUX_UNIMPL_RET(watchdog_othercpu_hardlockup_happen, bool, false)
LIBLINUX_UNIMPL_RET(watchdog_softlockup_happen, bool, false)
LIBLINUX_UNIMPL(watchdog_lockup_panic_config)
LIBLINUX_UNIMPL(rdr_arctimer_register_read)

/* platform_source/basicplatform/kernel/hisi_gcov/hisi_kernel_gcov.c */
#ifdef CONFIG_GCOV_KERNEL
LIBLINUX_UNIMPL(gcov_get_gcda)
#endif

/* drivers/platform_drivers/tzdriver */
LIBLINUX_UNIMPL_RET(get_mm_exe_file, void *, NULL);
LIBLINUX_EXPORTED_UNIMPL_RET(get_user_pages_remote, long, -ENOSYS);

LIBLINUX_UNIMPL_RET(vma_is_stack_for_current, int, 0);
LIBLINUX_UNIMPL_RET(__vm_enough_memory, int, 0);

/* drivers/hisi/modem depends */
LIBLINUX_EXPORTED_UNIMPL_RET(register_restart_handler, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(unregister_restart_handler, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL(core_ctl_set_boost);

/* kernel/sched/idle.c */
LIBLINUX_UNIMPL(cpu_idle_poll_ctrl)

/* platform_source/cee/kernel/sched/ */
/* introduced by CONFIG_CPU_ISOLATION_OPT */
LIBLINUX_UNIMPL_RET(sched_isolate_cpu, int, 0)
LIBLINUX_UNIMPL_RET(sched_unisolate_cpu, int, 0)

/* platform_source/see/drivers/prmem/prmem.c */
LIBLINUX_EXPORTED_UNIMPL_RET(pmalloc_aligned, void *, NULL);
LIBLINUX_UNIMPL(prmem_init);
LIBLINUX_EXPORTED_UNIMPL_RET(prmem_protect_addr, int, -ENOSYS);

/* mm/memory.c */
LIBLINUX_EXPORTED_UNIMPL_RET(follow_pfn, int, -ENOSYS);

/* mm/vmalloc.c */
LIBLINUX_EXPORTED_UNIMPL_RET(vm_map_ram, void *, NULL);
LIBLINUX_EXPORTED_UNIMPL(vm_unmap_ram);

/* mm/gup.c */
LIBLINUX_EXPORTED_UNIMPL_RET(pin_user_pages_locked, long, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL(unpin_user_pages);

LIBLINUX_GPL_EXPORTED_UNIMPL_RET(system_32bit_el0_cpumask, void *, NULL);

/* for dubai */
LIBLINUX_UNIMPL(dubai_proc_cputime_init);
LIBLINUX_UNIMPL(dubai_proc_cputime_exit);

LIBLINUX_EXPORTED_UNIMPL_RET(hiview_send_hisysevent, int, 0);
