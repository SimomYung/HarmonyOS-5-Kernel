// SPDX-License-Identifier: GPL-2.0-only
#include <linux/err.h>
#include <liblinux/unimpl.h>
#include <linux/err.h>

/* kernel/exec.c */
int suid_dumpable = 0;
LIBLINUX_UNIMPL(set_dumpable);

/* kernel/fork.c */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(get_task_mm, void *, NULL);
LIBLINUX_GPL_EXPORTED_UNIMPL(mmput);
LIBLINUX_UNIMPL_RET(ksys_unshare, int, -EINVAL);

/* kernel/pid.c */
LIBLINUX_UNIMPL_RET(find_ge_pid, void *, NULL);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(find_get_pid, void *, NULL);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(pid_vnr, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(pid_task, void *, NULL);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(get_pid_task, void *, NULL);

/* kernel/printk/printk_safe.c */
LIBLINUX_UNIMPL(printk_nmi_direct_enter);
LIBLINUX_UNIMPL(printk_nmi_direct_exit);
LIBLINUX_UNIMPL(__printk_safe_enter);
LIBLINUX_UNIMPL(__printk_safe_exit);

/* kernel/cgroup/cgroup.c */
LIBLINUX_UNIMPL_RET(css_next_child, void *, NULL)

/* kernel/sched/core.c */
LIBLINUX_UNIMPL_RET(cpus_share_cache, bool, false);
LIBLINUX_UNIMPL(dump_cpu_task)
/* hack: assume task always inactive */
LIBLINUX_UNIMPL_RET(wait_task_inactive, unsigned long, 1)
LIBLINUX_UNIMPL_RET(nr_context_switches, unsigned long long, 0)
LIBLINUX_UNIMPL_RET(sched_cpu_starting, int, 0)
LIBLINUX_UNIMPL_RET(sched_tick_offload_init, int, 0)
LIBLINUX_UNIMPL(sched_ttwu_pending)
LIBLINUX_UNIMPL_RET(task_sched_runtime, unsigned long long, 0);
LIBLINUX_UNIMPL(wake_up_if_idle)
LIBLINUX_UNIMPL_RET(idle_task, void *, NULL)
LIBLINUX_UNIMPL_RET(task_curr, int, 1)
LIBLINUX_UNIMPL_RET(try_invoke_on_locked_down_task, bool, false)
LIBLINUX_UNIMPL(show_state_filter)

/* kernel/sched/cputime.c */
LIBLINUX_UNIMPL(vtime_user_enter)
LIBLINUX_UNIMPL(vtime_user_exit)

/* kernel/signal.c */
LIBLINUX_EXPORTED_UNIMPL(kernel_sigaction)
LIBLINUX_UNIMPL(ignore_signals)
LIBLINUX_EXPORTED_UNIMPL_RET(send_sig, int, -ENOSYS)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(kill_pid_usb_asyncio, int, 0)
LIBLINUX_EXPORTED_UNIMPL_RET(kill_pgrp, int, 0)
LIBLINUX_UNIMPL_RET(__group_send_sig_info, int, 0)
LIBLINUX_UNIMPL_RET(group_send_sig_info, int, 0)
LIBLINUX_UNIMPL(flush_signal_handlers)
LIBLINUX_UNIMPL(flush_sigqueue)

/* kernel/umh.c */
LIBLINUX_EXPORTED_UNIMPL_RET(call_usermodehelper_setup, void *, NULL)
LIBLINUX_EXPORTED_UNIMPL_RET(call_usermodehelper_exec, int, -EINVAL)
LIBLINUX_EXPORTED_UNIMPL_RET(call_usermodehelper, int, -ENOMEM)

/* security/{}.c */
LIBLINUX_UNIMPL_RET(cap_capable, int, 0)
LIBLINUX_UNIMPL_RET(cap_convert_nscap, int, 0)
LIBLINUX_UNIMPL_RET(cap_settime, int, -EPERM)
LIBLINUX_UNIMPL_RET(cap_inode_getsecurity, int, 0)
LIBLINUX_UNIMPL_RET(cap_inode_killpriv, int, -EPERM)
LIBLINUX_UNIMPL_RET(cap_inode_need_killpriv, int, 0)
LIBLINUX_UNIMPL_RET(cap_inode_removexattr, int, 0)
LIBLINUX_UNIMPL_RET(cap_inode_setxattr, int, 0)
LIBLINUX_UNIMPL_RET(cap_task_setioprio, int, 0)

/* kernel/capability.c */
LIBLINUX_EXPORTED_UNIMPL_RET(file_ns_capable, bool, true)
LIBLINUX_UNIMPL_RET(has_ns_capability, bool, true)
LIBLINUX_UNIMPL_RET(has_ns_capability_noaudit, bool, true)
LIBLINUX_EXPORTED_UNIMPL_RET(ns_capable_setid, bool, true)

/* kernel/time/posix-cpu-timers.c */
LIBLINUX_UNIMPL(run_posix_cpu_timers)

/* kernel/exit.c */
LIBLINUX_UNIMPL_RET(is_current_pgrp_orphaned, int, 0)
LIBLINUX_UNIMPL(__wake_up_parent)

/* drivers/rphone/rprocess/rasprocess.c */
LIBLINUX_EXPORTED_UNIMPL_RET(kill_pid, int, 0)
LIBLINUX_EXPORTED_UNIMPL(mm_trace_rss_stat)

/* mm/zsmalloc.c */
LIBLINUX_EXPORTED_UNIMPL(zpool_register_driver)
LIBLINUX_EXPORTED_UNIMPL_RET(zpool_unregister_driver, int, 0)

LIBLINUX_UNIMPL_RET(find_bug, void *, NULL);

LIBLINUX_UNIMPL(register_kernel_break_hook);

/* kernel/printk/printk.c */
LIBLINUX_EXPORTED_UNIMPL(console_start);
LIBLINUX_EXPORTED_UNIMPL(console_stop);
LIBLINUX_EXPORTED_UNIMPL_RET(unregister_console, int, 0);
bool console_suspend_enabled = true;
#ifdef CONFIG_DFX_BB
unsigned long *prb_buf = NULL;
u32 prb_buf_size = 0;
unsigned long *dfx_ex_prb_info_buf = NULL;
u32 dfx_ex_prb_info_buf_size = 0;
unsigned long *dfx_ex_prb_desc_buf = NULL;
u32 dfx_ex_prb_desc_buf_size= 0;
char *dfx_ex_log_buf = NULL;
u32 dfx_ex_log_buf_len = 0;
u32 *dfx_ex_log_first_idx = NULL;
u32 *dfx_ex_log_next_idx = NULL;
#endif /* CONFIG_DFX_BB */

/* kernel/power/energy_model.c */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(em_dev_register_perf_domain, int, 0)

/* kernel/power/suspend.c */
LIBLINUX_GPL_EXPORTED_UNIMPL(suspend_set_ops)

/* lib/is_single_threaded.c */
LIBLINUX_UNIMPL_RET(current_is_single_threaded, bool, false)

/* Walkaround cdc bug */
LIBLINUX_UNIMPL_RET(save_notify_log_info, int, 0)

/* kernel/profile.c */
LIBLINUX_UNIMPL(force_schedstat_enabled)
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(profile_pc, unsigned long, 0)

/* kernel/reboot.c */
LIBLINUX_UNIMPL(ctrl_alt_del)

/* kernel/power/snapshot.c */
LIBLINUX_UNIMPL_RET(swsusp_page_is_forbidden, int, 0)
LIBLINUX_UNIMPL(swsusp_set_page_free)
LIBLINUX_UNIMPL(swsusp_unset_page_free)

/* kernel/freezer.c */
unsigned int sysctl_user_handle_freezing = 0;
atomic_t system_freezing_cnt = ATOMIC_INIT(0);
EXPORT_SYMBOL(system_freezing_cnt);
LIBLINUX_EXPORTED_UNIMPL_RET(freezing_slow_path, bool, false)
LIBLINUX_EXPORTED_UNIMPL_RET(__refrigerator, bool, false)

/* kernel/events/callchain.c */
LIBLINUX_UNIMPL_RET(get_callchain_buffers, int, -ENOSYS)
LIBLINUX_UNIMPL_RET(perf_event_max_stack_handler, int, -ENOSYS)
LIBLINUX_UNIMPL(put_callchain_buffers)
LIBLINUX_UNIMPL(put_callchain_entry)
struct perf_callchain_entry;
LIBLINUX_UNIMPL_RET(get_callchain_entry, struct perf_callchain_entry *, NULL)
LIBLINUX_UNIMPL_RET(get_perf_callchain, struct perf_callchain_entry *, NULL)

/* kernel/events/core.c */
int sysctl_perf_cpu_time_max_percent __read_mostly = 0;
int sysctl_perf_event_max_contexts_per_stack = 0;
int sysctl_perf_event_max_stack = 0;
int sysctl_perf_event_mlock __read_mostly = 0;
int sysctl_perf_event_paranoid __read_mostly = 0;
int sysctl_perf_event_sample_rate __read_mostly	= 0;

struct pt_regs {};
struct pt_regs __perf_regs[4];
LIBLINUX_UNIMPL(perf_event_bpf_event)
LIBLINUX_UNIMPL_RET(perf_event_exit_cpu, int, 0)
struct file;
LIBLINUX_UNIMPL_RET(perf_event_get, struct file *, ERR_PTR(-EBADF))
LIBLINUX_UNIMPL(perf_event_init);
LIBLINUX_UNIMPL_RET(perf_event_init_cpu, int, 0)
LIBLINUX_UNIMPL(perf_event_ksymbol)
LIBLINUX_UNIMPL_RET(perf_event_output, int, -ENOSYS)
LIBLINUX_UNIMPL_RET(perf_event_read_local, int, -ENOSYS)
struct perf_event;
LIBLINUX_UNIMPL_RET(perf_get_event, const struct perf_event *, ERR_PTR(-EINVAL))
LIBLINUX_UNIMPL_RET(perf_proc_update_handler, int, -ENOSYS)
LIBLINUX_UNIMPL_RET(perf_swevent_get_recursion_context, int, -ENOSYS)
LIBLINUX_GPL_EXPORTED_UNIMPL(perf_trace_run_bpf_submit)
LIBLINUX_UNIMPL_RET(perf_cpu_time_max_percent_handler, int, -ENOSYS)
LIBLINUX_UNIMPL_RET(perf_pmu_register, int, 0)
