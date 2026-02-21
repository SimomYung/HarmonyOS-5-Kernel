#ifndef UAPI_HMKERNEL_SCHED_MIPS_COMMON_H
#define UAPI_HMKERNEL_SCHED_MIPS_COMMON_H

#define __MIPS_TRACKING_MODULE_NAME "mips_class"
#include <hmkernel/types.h>

#define __MIPS_SET_ENABLE 0UL
#define __MIPS_SET_TASK_ENABLE 1UL
#define __MIPS_SET_CPU_REPORT_POLICY 2UL
#define __MIPS_SET_TASK_REPORT_POLICY 3UL
#define __MIPS_SET_WINDOW_SIZE_PATH 4UL
#define __MIPS_SET_PID_ENABLE 5UL
#define __MIPS_MEM_CMD_ENABLE 6UL
#define __MIPS_MEM_CMD_FREQ_MAP 7UL
#define __MIPS_MEM_CMD_RATIOS 	8UL
#define __MIPS_MEM_CMD_IPC_MIN_SAMPLE_TIME 9UL
#define __MIPS_MEM_CMD_REFILL_EVENT 10UL

struct mips_config {
	unsigned int val;
	unsigned long cmd;
};

#define __MIPS_MEM_DEFAULT_TARGET_RATIO 600
#define __MIPS_MEM_DEFAULT_IPC_MIN_SAMPLE_TIME_MS 4U
#define __MIPS_MEM_DEFAULT_MEM_WINDOW_SIZE_MS	4U

enum dfc_mem_type {
	DFC_MEM_TYPE_DDR,
	DFC_MEM_TYPE_L3C,
	DFC_MEM_TYPE_NUM,
};

struct mips_pmu_data {
	unsigned int inst;
	unsigned int cycle;
	unsigned int refill[DFC_MEM_TYPE_NUM];
};
struct mem_freq_map {
	unsigned int core_freq;
	unsigned int dev_freq;
};

#define __MIPS_MEM_DEFAULT_SIZE 64U
struct __mips_mem_attributes {
	 __bool enable;
	unsigned int cluster_id;
	int type;
	unsigned int refill_event_id;
	unsigned int ratios[__MIPS_MEM_DEFAULT_SIZE];
	int nratios;
	unsigned int ipc_min_sample_time[__MIPS_MEM_DEFAULT_SIZE];
	int nipc_min_sample_time;
	struct mem_freq_map freq_map[__MIPS_MEM_DEFAULT_SIZE];
	int map_size;
	unsigned int nlevel;
};
__bool mips_mem_enable(unsigned int cluster, unsigned int type);
#endif
