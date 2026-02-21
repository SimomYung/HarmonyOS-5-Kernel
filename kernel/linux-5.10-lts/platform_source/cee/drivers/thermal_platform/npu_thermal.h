/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved. */
#ifndef __NPU_THERMAL_H_
#define __NPU_THERMAL_H_

#include <thermal_core.h>
struct nputh_data {
	int temp;
	enum thermal_trip_type type;
	char cdev_name[THERMAL_NAME_LENGTH];

	int num_intervals;
	int *intervals;

	int passive_delay;
	int polling_delay;
#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	bool dyn_step_wise_enabled;
	int ori_trip_temp;
	int dyn_trips_num;
	int dyn_ref_temp_num;
	int *dyn_ref_temp;
	int **dyn_intervals;
#endif /* CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL */
};

struct npu_thermal_zone {
	struct thermal_zone_device *thermal_dev;
	struct delayed_work thermal_work;
	struct nputh_data *param;
	unsigned int cur_index;
};

int npu_thermal_creat_attrs(struct thermal_zone_device *tz);
void npu_thermal_destroy_attrs(struct thermal_zone_device *tz);

#endif __NPU_THERMAL_H_
