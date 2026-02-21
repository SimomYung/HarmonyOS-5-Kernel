/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: dubai npu stats
 */
#ifndef DUBAI_NPU_STATS_H
#define DUBAI_NPU_STATS_H

#include <chipset_common/dubai/dubai_npu.h>

#define DUBAI_NPU_FREQ_MAX_SIZE		20

struct dubai_npu_stats {
	int32_t num;
	uint64_t energy;
	struct dubai_npu_freq_info npu_info[DUBAI_NPU_FREQ_MAX_SIZE];
} __packed;

#ifdef CONFIG_POWER_DUBAI_TASK_NPU_POWER
long dubai_ioctl_npu(unsigned int cmd, void __user *argp);
int dubai_npu_register_ops(struct dubai_npu_stats_ops *op);
int dubai_npu_unregister_ops(void);
void dubai_npu_stats_init(void);
void dubai_npu_stats_exit(void);
#else
static inline long dubai_ioctl_npu(unsigned int cmd, void __user *argp)
{
	return 0;
}
static inline int dubai_npu_register_ops(struct dubai_npu_stats_ops *op)
{
	return 0;
}
static inline int dubai_npu_unregister_ops(void)
{
	return 0;
}
static inline void dubai_npu_stats_init(void)
{
}
static inline void dubai_npu_stats_exit(void)
{
}
#endif /* CONFIG_POWER_DUBAI_TASK_NPU_POWER */

#endif /* DUBAI_NPU_STATS_H */
