// SPDX-License-Identifier: GPL-2.0
/*
 * interval control thermal governor
 *
 * Copyright (C) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/thermal.h>
#include <trace/events/thermal.h>

#include "thermal_core.h"

static int get_tz_interval(struct thermal_zone_device *tz, int trip)
{
	int interval;

	if (!tz->ops->get_interval || tz->ops->get_interval(tz, trip, &interval) != 0)
		interval = 0;
	return interval;
}

static void thermal_zone_trip_update(struct thermal_zone_device *tz, int trip)
{
	struct thermal_instance *instance;
	unsigned long new_target, old_target;

	new_target = (unsigned long)get_tz_interval(tz, trip);
	mutex_lock(&tz->lock);

	list_for_each_entry(instance, &tz->thermal_instances, tz_node) {
		if (instance->trip != trip)
			continue;

		old_target = instance->target;
		instance->target = new_target;
		dev_dbg(&instance->cdev->device, "old_target=%lu, target=%lu\n",
			old_target, instance->target);

		if (instance->initialized && old_target == instance->target)
			continue;

		instance->initialized = true;
		mutex_lock(&instance->cdev->lock);
		instance->cdev->updated = false; /* cdev needs update */
		mutex_unlock(&instance->cdev->lock);
	}

	mutex_unlock(&tz->lock);
}

/**
 * interval_ctrl_throttle - throttles devices associated with the given zone
 * @tz: thermal_zone_device
 * @trip: trip point index
 *
 * Throttling Logic: This uses the temperature of the thermal zone to throttle.
 * target of all the cooling devices is associated with the zone temperature internal
 */
static int interval_ctrl_throttle(struct thermal_zone_device *tz, int trip)
{
	struct thermal_instance *instance;

	thermal_zone_trip_update(tz, trip);

	mutex_lock(&tz->lock);

	list_for_each_entry(instance, &tz->thermal_instances, tz_node)
		thermal_cdev_update(instance->cdev);

	mutex_unlock(&tz->lock);

	return 0;
}

static struct thermal_governor thermal_gov_interval_ctrl = {
	.name		= "interval_ctrl",
	.throttle	= interval_ctrl_throttle,
};
THERMAL_GOVERNOR_DECLARE(thermal_gov_interval_ctrl);
