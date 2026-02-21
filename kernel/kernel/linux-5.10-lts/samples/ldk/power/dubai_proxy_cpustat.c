// SPDX-License-Identifier: GPL-2.0
#include <errno.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <liblinux/pal.h>

int dubai_cpustat_request(void *stat, int *count, size_t size, int type)
{
	return hm_dubai_cpustat_request(stat, count, size, type);
}

int dubai_proc_cmdline_request(uid_t uid, pid_t pid, char *cmdline, size_t size, char *comm_buf, size_t comm_size)
{
	return hm_dubai_proc_cmdline_request(uid, pid, cmdline, size, comm_buf, comm_size);
}

int dubai_pids_of_uid_request(void *info, size_t uid_info_size,
			      void *pid_set, size_t pid_set_size)
{
	struct uid_info *hm_uid_info = (struct uid_info *)info;
	struct uid_pid_set *hm_pid_set = (struct uid_pid_set *)pid_set;

	if (info == NULL || sizeof(struct uid_info) != uid_info_size ||
	    pid_set == NULL || sizeof(struct uid_pid_set) != pid_set_size) {
		return -EINVAL;
	}

	return hm_dubai_pids_of_uid_request(hm_uid_info, uid_info_size, hm_pid_set, pid_set_size);
}

int dubai_decomposed_procs_update(void *proc_info, int count)
{
	struct decomposed_proc_info *hm_proc_info = (struct decomposed_proc_info *)proc_info;

	if (proc_info == NULL) {
		return -EINVAL;
	}

	return hm_dubai_decomposed_procs_update(proc_info, count);
}

int dubai_binder_stats_request(void *stat_info, int *count, int max_count)
{
	return hm_dubai_binder_stats_request(stat_info, count, max_count);
}

int dubai_cpu_total_inst(uint64_t *info, uint32_t cpu_nr)
{
	return liblinux_pal_get_cpu_total_inst(info, cpu_nr);
}
