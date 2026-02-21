/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_DUBAI_PROXY_CPUSTAT_H__
#define __LINUX_DUBAI_PROXY_CPUSTAT_H__

int dubai_cpustat_request(void *stat, int *count, size_t size, int type);
int dubai_proc_cmdline_request(uid_t uid, pid_t pid, char *cmdline, size_t size, char *comm_buf, size_t comm_size);
int dubai_pids_of_uid_request(void *info, size_t uid_info_size,
			      void *pid_set, size_t pid_set_size);
int dubai_decomposed_procs_update(void *proc_info, int count);
int dubai_binder_stats_request(void *stat_info, int *count, int max_count);

int dubai_cpu_total_inst(uint64_t *info, uint32_t cpu_nr);

#endif /* __LINUX_DUBAI_PROXY_CPUSTAT_H__ */
