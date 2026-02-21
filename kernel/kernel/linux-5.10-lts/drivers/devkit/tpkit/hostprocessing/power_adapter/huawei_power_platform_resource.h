/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#ifndef __POWER_PLATFORM_RESOURCE__
#define __POWER_PLATFORM_RESOURCE__
#include <linux/regulator/consumer.h>
#include <linux/gpio.h>
#include <linux/hwspinlock.h>
#include <hwspinlock_internal.h>
#include <linux/mutex.h>
#include <linux/delay.h>

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
#include <huawei_platform/sensor/hw_comm_pmic.h>
#endif

/**
 * @power_supply_mode: 0:Independent power supply.
 *					 1:Common power supply.
 *					 If use common power supply,
 *					 ic enter sleep mode when suspend.
 * @power_sequence : IC power-on sequence.
 *					 0: use goodix sequence.
 */
struct power_config {
	u8 power_supply_mode;
	u8 power_sequence;
	bool common_power_flag;
};

enum thp_power_type {
	THP_POWER_UNUSED = 0,
	THP_POWER_LDO = 1,
	THP_POWER_GPIO = 2,
	THP_POWER_PMIC = 3,
	THP_POWER_INVALID_TYPE,
};

struct thp_pmic_power_supply {
	int ldo_num;
	int value;
	int pmic_num;
};

struct thp_power_supply {
	int use_count;
	int type;
	int gpio;
	int ldo_value;
	struct thp_pmic_power_supply pmic_power;
	struct regulator *regulator;
};

enum thp_power_id {
	THP_IOVDD = 0,
	THP_VCC = 1,
	THP_IOVDD_S = 2,
	THP_VCC_S = 3,
	THP_POWER_ID_MAX,
};

#define is_invaild_power_id(x) ((x) >= THP_POWER_ID_MAX)

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
static struct hw_comm_pmic_cfg_t tp_pmic_ldo_set;
#endif

static char *thp_power_name[THP_POWER_ID_MAX] = {
	"thp-iovdd",
	"thp-vcc",
	"thp-iovdd-s",
	"thp-vcc-s",
};

struct thp_poweron_timing {
	u8 first_pull_up;
	u8 second_pull_up;
	u8 third_pull_up; /* maybe used of common power */
	u8 forth_pull_up; /* maybe used of common power */
	u8 delay_after_first_pull_up;
	u8 delay_after_second_pull_up;
	u8 delay_after_third_pull_up; /* maybe used of common power */
	u8 delay_after_forth_pull_up; /* maybe used of common power */
	u8 delay_before_rst_pull_up;
	u8 need_control_cs_before_poweron;
	u8 need_control_cs_after_poweron;
};

struct thp_poweroff_timing {
	u8 first_pull_down;
	u8 second_pull_down;
	u8 third_pull_down; /* maybe used of common power */
	u8 forth_pull_down; /* maybe used of common power */
	u8 delay_after_first_pull_down;
	u8 delay_after_second_pull_down;
	u8 delay_after_third_pull_down; /* maybe used of common power */
	u8 delay_after_forth_pull_down; /* maybe used of common power */
};

int thp_power_init(u32 panel_id);
int thp_power_release(u32 panel_id);
int thp_power_module_register(void);
void thp_power_module_exit(void);
void thp_power_lock(void);
void thp_power_unlock(void);
int thp_power_supply_get(enum thp_power_id power_id, u32 panel_id);
int thp_power_supply_put(enum thp_power_id power_id, u32 panel_id);
int thp_power_supply_ctrl(enum thp_power_id power_id,
	int status, unsigned int delay_ms, u32 panel_id);
#endif
