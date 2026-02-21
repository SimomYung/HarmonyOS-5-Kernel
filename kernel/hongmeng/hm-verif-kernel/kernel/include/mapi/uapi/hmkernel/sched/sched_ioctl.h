#ifndef HISI_SCHED_IOCTL_H
#define HISI_SCHED_IOCTL_H
#include <hmkernel/types.h>
#include <hmkernel/sched_module.h>

#define HISI_SCHED_CLASS_NAME "hisi_sched"

#define TCB_NAME_LEN	16

#define __SCHED_SET_HISI_PERF_HINT 0x1U

#define __SCHED_GET_THERMAL_INTE_EN 0x10U
#define __SCHED_SET_THERMAL_INTE_EN 0x11U
#define __SCHED_SET_FREQ_LIMIT 0x12U
#define __SCHED_SET_THERMAL_INTE_WHITELIST 0x13U
#define __SCHED_GET_THERMAL_INTE_WHITELIST 0x14U

#define THERMAL_WHITELIST_MAX_BUF_SIZE 2048U
 
struct thermal_intelligent_whitelist {
	unsigned int uid;
	char tcb_name[TCB_NAME_LEN];
	bool enable;
};

#endif
