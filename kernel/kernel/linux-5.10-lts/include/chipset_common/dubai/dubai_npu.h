/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: dubai npu driver
 */
#ifndef DUBAI_NPU_H
#define DUBAI_NPU_H

#include <linux/types.h>

struct dubai_npu_freq_info {
	int64_t freq;
	int64_t run_time;
} __packed;

typedef int (*dubai_npu_enable_fn_t)(bool enable);
typedef int (*dubai_npu_freq_count_fn_t)(void);
typedef int (*dubai_npu_stats_fn_t)(struct dubai_npu_freq_info *data, uint64_t *energy, int num);

struct dubai_npu_stats_ops {
	dubai_npu_enable_fn_t enable;
	dubai_npu_freq_count_fn_t get_num;
	dubai_npu_stats_fn_t get_stats;
};

#endif /* DUBAI_NPU_H */
