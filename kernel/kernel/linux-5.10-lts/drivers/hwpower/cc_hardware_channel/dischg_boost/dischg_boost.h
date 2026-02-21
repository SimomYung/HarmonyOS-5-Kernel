/*
 * discharge_boost.h
 *
 * discharge boost driver
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _DISCHARGE_BOOST_H_
#define _DISCHARGE_BOOST_H_

#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <chipset_common/hwpower/common_module/power_wakeup.h>

struct dischg_boost_config {
	int switch_boost_soc;
	int switch_normal_soc;
	int switch_boost_delay_ms;
	int switch_normal_delay_ms;
	int boost_gpio;
	int normal_gpio;
	int switch_mode;
	int index;
	int switch_boost_vout;
};

struct dischg_boost_context {
	struct dischg_boost_dev *dischg_dev;
	struct dischg_boost_config dischg_cfg;
	struct work_struct switch_work;
	atomic_t prev_scene;
	int enforce_sts;
};

struct dischg_boost_dev {
	struct device *dev;
	struct notifier_block soc_change_nb;
	struct dischg_boost_context *dischg_context;
	struct wakeup_source *wakelock;
	int path_num;
};

#endif /* _DISCHARGE_BOOST_H_ */