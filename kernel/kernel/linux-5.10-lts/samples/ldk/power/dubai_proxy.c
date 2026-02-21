// SPDX-License-Identifier: GPL-2.0
#include <linux/netlink.h>
#include <net/sock.h>
#include <securec.h>

#include <chipset_common/dubai/dubai_ioctl.h>

#include "dubai_cpu_stats.h"
#include "utils/dubai_utils.h"
#include "utils/buffered_log_sender.h"

#include "dubai_proxy_cpustat.h"

/*
 * This number should not repeat numbers in
 * kernel/oh-ldk/include/uapi/linux/netlink.h.
 */
#define NETLINK_DUBAI	35
#define USER_PORT	100
#define NAME_LEN		48
/* active process count */
#define MAX_STAT_ENTRY_COUNT	2524
/* dead process count */
#define MAX_DEAD_ENTRY_COUNT	1000

#define PID_INVALID	0

#define PREFIX_LEN	32
#define DECOMPOSE_COUNT_MAX	10

#define IOC_PROC_CPUTIME_ENABLE DUBAI_CPU_DIR_IOC(W, 1, bool)
#define IOC_PROC_CPUTIME_REQUEST DUBAI_CPU_DIR_IOC(W, 2, long long)
#define IOC_TASK_CPUPOWER_SUPPORT DUBAI_CPU_DIR_IOC(R, 3, bool)
#define IOC_PROC_DECOMPOSE_SET DUBAI_CPU_DIR_IOC(W, 4, struct dev_transmit_t)
#define IOC_CPU_CONFIGS_GET DUBAI_CPU_DIR_IOC(R, 5, struct dubai_cpu_config_info *)
#define IOC_CPU_INST_GET DUBAI_CPU_DIR_IOC(R, 6, struct liblinux_cpu_inst *)

#define IOC_PROC_CMDLINE_GET DUBAI_UTILS_DIR_IOC(WR, 1, struct proc_cmdline)
#define IOC_RSS_GET DUBAI_UTILS_DIR_IOC(R, 2, unsigned long long)
#define IOC_PIDS_GET DUBAI_UTILS_DIR_IOC(WR, 3, struct uid_name_to_pid)

enum {
	MAGIC_PROC_CPUTIME = 0,
};

enum {
	PROCESS_STATE_DEAD = 0,
	PROCESS_STATE_ACTIVE,
};

static struct netlink_kernel_cfg dubai_nlcfg = {
	.input  = NULL,
};

/*
 * These data structure should be consistent with those
 * in sysmgr and uapps/tppmgr.
 */
struct dubai_stat_entry {
	uid_t uid;
	pid_t pid;
	unsigned long long time;
	unsigned long long power;
	unsigned char cmdline;
	char name[NAME_LEN];
} __attribute__((packed));

struct dubai_transmit_data {
	long long timestamp;
	int type;
	int count;
	struct dubai_stat_entry entries[0];
} __attribute__((packed));

struct dubai_stat_info {
	int length;
	int magic;
	struct dubai_transmit_data transmit[0];
} __attribute__((packed));

struct decomposed_proc_info {
	uid_t uid;
	char comm[TASK_COMM_LEN];
	char prefix[PREFIX_LEN];
} __attribute__((packed));

struct uid_info {
	uid_t uid;
	char name[CMDLINE_LEN];
} __attribute__((packed));

struct uid_pid_set {
	pid_t pids[MAX_PID_NUM];
	int pid_num;
} __attribute__((packed));

struct liblinux_cpu_inst {
	uint64_t inst;
	uint32_t cpu;
} __attribute__((__packed__));

/* active process size */
#define DUBAI_STAT_INFO_SIZE	(sizeof(struct dubai_stat_info) \
		+ sizeof(struct dubai_transmit_data) \
		+ MAX_STAT_ENTRY_COUNT * sizeof(struct dubai_stat_entry))
#define DUBAI_STAT_ENTRIES_SIZE	(MAX_STAT_ENTRY_COUNT * sizeof(struct dubai_stat_entry))

/* dead process size */
#define DUBAI_DEAD_INFO_SIZE	(sizeof(struct dubai_stat_info) \
		+ sizeof(struct dubai_transmit_data) \
		+ MAX_DEAD_ENTRY_COUNT * sizeof(struct dubai_stat_entry))
#define DUBAI_DEAD_ENTRIES_SIZE	(MAX_DEAD_ENTRY_COUNT * sizeof(struct dubai_stat_entry))

/* active process buff */
static char g_stat_info[DUBAI_STAT_INFO_SIZE];
/* dead process buff */
static char g_dead_process_stat_info[DUBAI_DEAD_INFO_SIZE];
static struct decompose_info_data {
	int length;
	struct decomposed_proc_info info[DECOMPOSE_COUNT_MAX];
} g_decompose_info_data;

static struct liblinux_cpu_inst g_cpu_inst_data[NR_CPUS];
static uint64_t g_cpu_inst_arr[NR_CPUS];

static inline size_t dubai_stat_info_size(int count)
{
	return sizeof(struct dubai_stat_info) +
	       sizeof(struct dubai_transmit_data) +
	       count * sizeof(struct dubai_stat_entry);
}

static void dubai_stat_info_init(struct dubai_stat_info *stat, long long timestamp, int type)
{
	struct dubai_transmit_data *transmit = NULL;

	stat->length = 0;
	stat->magic = MAGIC_PROC_CPUTIME;
	transmit = stat->transmit;
	transmit->timestamp = timestamp;
	transmit->type = type;
	transmit->count = 0;
}

static int dubai_send_buffered_log(struct dubai_stat_info *stat)
{
	int rc;
	int count = 0;
	unsigned int len = 0U;

	count = stat->transmit[0].count;
	len = dubai_stat_info_size(count);
	stat->length = (int)(len - sizeof(struct dubai_stat_info));
	rc = send_buffered_log((const struct buffered_log_entry *)stat);

	return rc;
}

static int dubai_get_dead_proc_cputime(u64 timestamp)
{
	int rc;
	struct dubai_stat_info *dead_process_stat = (struct dubai_stat_info *)g_dead_process_stat_info;

	dubai_stat_info_init(dead_process_stat, timestamp, PROCESS_STATE_DEAD);
	rc = dubai_cpustat_request((void *)(&dead_process_stat->transmit->entries[0]),
				    &dead_process_stat->transmit->count,
				    DUBAI_DEAD_ENTRIES_SIZE,
				    PROCESS_STATE_DEAD);
	if (rc == 0) {
		rc = dubai_send_buffered_log(dead_process_stat);
	}
	return rc;
}

static int dubai_get_active_proc_cputime(u64 timestamp)
{
	int rc;
	struct dubai_stat_info *stat = (struct dubai_stat_info *)g_stat_info;

	dubai_stat_info_init(stat, timestamp, PROCESS_STATE_ACTIVE);
	rc = dubai_cpustat_request((void *)(&stat->transmit->entries[0]),
				   &stat->transmit->count,
				   DUBAI_STAT_ENTRIES_SIZE,
				   PROCESS_STATE_ACTIVE);
	if (rc == 0) {
		rc = dubai_send_buffered_log(stat);
	}
	return rc;
}

static int dubai_get_proc_cputime(void __user *argp)
{
	int rc = 0;
	u64 timestamp;

	if (copy_from_user(&timestamp, argp, sizeof(u64))) {
		return -EFAULT;
	}
	rc = dubai_get_dead_proc_cputime(timestamp);
	if (rc < 0) {
		return rc;
	}
	rc = dubai_get_active_proc_cputime(timestamp);

	return rc;
}

static int dubai_get_task_cpupower_enable(void __user *argp)
{
	int cpu_power_support = 1;

	if(copy_to_user(argp, &cpu_power_support, sizeof(cpu_power_support))) {
		return -EFAULT;
	}

	return 0;
}

static int dubai_update_decomposed_procs(void __user *argp)
{
	int ret, size, count, length, remain;

	ret = copy_from_user(&length, argp, sizeof(int));
	if (ret != 0) {
		return -EFAULT;
	}
	count = length / (int)(sizeof(struct decomposed_proc_info));
	remain = length % (int)(sizeof(struct decomposed_proc_info));
	if ((count <= 0) || (count > DECOMPOSE_COUNT_MAX) || (remain != 0)) {
		dubai_err("Invalid length, length: %d, count: %d, remain: %d", length, count, remain);
		return -EINVAL;
	}
	/* total size is the actual decomposed_proc_info array size plus the size number of it */
	size = length + sizeof(struct dev_transmit_t);
	ret = memset_s(&g_decompose_info_data, sizeof(struct decomposed_proc_info),
		       0, sizeof(struct decomposed_proc_info));
	if (ret > 0) {
		return -EFAULT;
	}
	ret = copy_from_user(&g_decompose_info_data, argp, size);
	if (ret != 0) {
		return -EFAULT;
	}
	ret = dubai_decomposed_procs_update((void *)g_decompose_info_data.info, count);
	if (ret < 0) {
		dubai_err("Failed to set process decompose list");
	}

	return ret;
}

int dubai_obtain_binder_stats_info(void *stat_info, int *count, int max_count)
{
	return dubai_binder_stats_request(stat_info, count, max_count);
}

int liblinux_pal_cpu_inst_init(void)
{
	uint32_t i;
	for (i = 0; i < NR_CPUS; i++) {
		g_cpu_inst_data[i].inst = 0;
		g_cpu_inst_data[i].cpu = i;
	}
	return 0;
}

void liblinux_pal_cpu_inst_exit(void)
{
	int err;
	err = memset_s(g_cpu_inst_data, sizeof(g_cpu_inst_data), 0, sizeof(g_cpu_inst_data));
	if (err != 0) {
		dubai_err("Failed to cpu inst data memset");
	}
	err = memset_s(g_cpu_inst_arr, sizeof(g_cpu_inst_arr), 0, sizeof(g_cpu_inst_arr));
	if (err != 0) {
		dubai_err("Failed to cpu inst arr memset");
	}
}

static int dubai_get_cpu_total_inst(void __user *argp)
{
	int err, i;

	err = memset_s(g_cpu_inst_arr, sizeof(g_cpu_inst_arr), 0, sizeof(g_cpu_inst_arr));
	if (err != 0) {
		return err;
	}
	err = dubai_cpu_total_inst(g_cpu_inst_arr, NR_CPUS);
	if (err != 0) {
		return err;
	}
	for (i = 0; i < NR_CPUS; i++) {
		g_cpu_inst_data[i].inst = g_cpu_inst_arr[i];
	}
	if (copy_to_user(argp, g_cpu_inst_data,
			 sizeof(struct liblinux_cpu_inst) * num_present_cpus())) {
		return -EFAULT;
	}
	return 0;
}

long dubai_ioctl_cpu(unsigned int cmd, void __user *argp)
{
	int rc = 0;

	switch (cmd) {
	case IOC_PROC_CPUTIME_ENABLE:
		break;
	case IOC_PROC_CPUTIME_REQUEST:
		rc = dubai_get_proc_cputime(argp);
		break;
	case IOC_TASK_CPUPOWER_SUPPORT:
		rc = dubai_get_task_cpupower_enable(argp);
		break;
	case IOC_PROC_DECOMPOSE_SET:
		rc = dubai_update_decomposed_procs(argp);
		break;
	case IOC_CPU_INST_GET:
		rc = dubai_get_cpu_total_inst(argp);
		break;
	case IOC_CPU_CONFIGS_GET:
		break;
	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

static int dubai_get_rss(void __user *argp)
{
	return -ENOSYS;
}

static int dubai_get_pids_by_uid_name(void __user *argp)
{
	int ret = 0;
	int i;
	struct uid_name_to_pid uid_name_data;
	struct uid_info info;
	struct uid_pid_set pid_set;

	ret = copy_from_user(&uid_name_data, argp, sizeof(struct uid_name_to_pid));
	if (ret != 0) {
		return -EFAULT;
	}
	uid_name_data.name[CMDLINE_LEN - 1] = '\0';
	if (strlen(uid_name_data.name) == 0) {
		return -EINVAL;
	}
	info.uid = uid_name_data.uid;
	ret = strcpy_s(info.name, CMDLINE_LEN, uid_name_data.name);
	if (ret != 0) {
		return ret;
	}
	ret = dubai_pids_of_uid_request((void *)&info, sizeof(struct uid_info),
					(void *)&pid_set, sizeof(struct uid_pid_set));
	if (ret < 0) {
		dubai_err("Failed to get pids by uid and name");
		return ret;
	}
	for (i = 0; i < pid_set.pid_num; i++) {
		uid_name_data.pid[i] = pid_set.pids[i];
	}
	ret = copy_to_user(argp, &uid_name_data, sizeof(struct uid_name_to_pid));
	if (ret != 0) {
		return -EFAULT;
	}

	return ret;
}

static int dubai_get_proc_cmdline(void __user *argp)
{
	int ret = 0;
	struct proc_cmdline cmdline;
	char comm_buf[TASK_COMM_LEN] = {0};

	if (copy_from_user(&cmdline, argp, sizeof(struct proc_cmdline))) {
		return -EFAULT;
	}
	if (cmdline.pid == PID_INVALID) {
		return -EINVAL;
	}
	cmdline.comm[TASK_COMM_LEN - 1] = '\0';
	cmdline.cmdline[CMDLINE_LEN - 1] = '\0';
	ret = dubai_proc_cmdline_request(cmdline.uid, cmdline.pid, cmdline.cmdline, CMDLINE_LEN, comm_buf, TASK_COMM_LEN);
	if (ret == 0) {
		if (strlen(cmdline.comm) > 0) {
			if (strncmp(cmdline.comm, comm_buf, TASK_COMM_LEN - 1) != 0 &&
			    strstr(cmdline.cmdline, cmdline.comm) == NULL) {
				return -EFAULT;
			}
		}
	} else {
		return -EINVAL;
	}
	if (copy_to_user(argp, &cmdline, sizeof(struct proc_cmdline))) {
		return -EFAULT;
	}

	return 0;
}

long __override dubai_ioctl_utils(unsigned int cmd, void __user *argp)
{
	int rc = 0;

	switch (cmd) {
	case IOC_PROC_CMDLINE_GET:
		rc = dubai_get_proc_cmdline(argp);
		break;
	case IOC_RSS_GET:
		rc = dubai_get_rss(argp);
		break;
	case IOC_PIDS_GET:
		rc = dubai_get_pids_by_uid_name(argp);
		break;
	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

static int __init dubai_proxy_init(void)
{
	return liblinux_pal_pm_class_init();
}
module_init(dubai_proxy_init);

static void __exit dubai_proxy_exit(void)
{
	liblinux_pal_pm_class_exit();
}
module_exit(dubai_proxy_exit);

