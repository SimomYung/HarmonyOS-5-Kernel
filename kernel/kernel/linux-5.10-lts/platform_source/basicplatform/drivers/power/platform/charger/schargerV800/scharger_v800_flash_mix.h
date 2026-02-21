/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * scharger_v800_flash_mix.h
 *
 * SchargerV800 flash header.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SCHARGER_V800_FLASH_MIX_H__
#define __SCHARGER_V800_FLASH_MIX_H__
#include "../../../../../../camera/drivers/media/native/camera/sensor/flash/hw_flash.h"
enum scharger_v800_power_mode {
	POWER_STANDBY_MODE = 0,
	PASS_THROUGH_MODE,
	BOOST_MODE,
	EXTERNAL_BOOST_MODE,
};

enum scharger_v800_current_conf {
	CURRENT_TORCH_LEVEL_MMI_BACK = 0,
	CURRENT_TORCH_LEVEL_MMI_FRONT = 1,
	CURRENT_TORCH_LEVEL_RT_BACK = 2,
	CURRENT_TORCH_LEVEL_RT_FRONT = 3,
	CURRENT_MIX_MAX = 4,
};

/* ir flash RT self test param */
enum scharger_v800_selftest_param {
	IR_SELFTEST_ON_TIME_INDEX = 0,
	IR_SELFTEST_OFF_TIME_INDEX = 1,
	IR_SELFTEST_CURRENT_INDEX = 2,
	IR_SELFTEST_TIMES_INDEX = 3,
	IR_SELFTEST_MAX_INDEX = 4,
};

/* flash mode for entire IC which depends on */
/* current flash mode of single front and back */
enum scharger_v800_entire_ic_mode_t {
	ENTIRE_IC_STANDBY_MODE = 0,
	ENTIRE_IC_FLASH_MODE = 1,
	ENTIRE_IC_TORCH_MODE = 2,
};

/* flash mode for single front or back */
enum scharger_v800_single_mode_t {
	SINGLE_STANDBY_MODE = 0x00,
	SINGLE_FLASH_MODE = 0x01,
	SINGLE_TORCH_MODE = 0x02,
};

struct scharger_v800_private_data_t {
	struct device *dev;
	struct regmap *regmap;
	unsigned int scharger_version;
	unsigned int torch_current;
	unsigned int flash_type;
	enum scharger_v800_power_mode power_mode;
	unsigned int front_cur;
	unsigned int back_cur;
	unsigned int id_list[HWFLASH_POSITION_MAX];
	unsigned int id_list_count;
	enum scharger_v800_single_mode_t front_mode;
	enum scharger_v800_single_mode_t back_mode;
	enum scharger_v800_entire_ic_mode_t entire_ic_mode;
	unsigned int need_wakelock;
	unsigned int led_type;
	unsigned int selftest_param[IR_SELFTEST_MAX_INDEX];
	/* flash electric current config */
	unsigned int ecurrent[CURRENT_MIX_MAX];
	bool use_mode_lock;
	int use_external_boost;
	int flash_irq;
	struct work_struct flash_irq_work;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	struct wakeup_source scharger_v800_flash_wakelock;
#else
	struct wakeup_source *scharger_v800_flash_wakelock;
#endif
};

#ifdef CONFIG_USE_CAMERA3_ARCH
enum scharger_v800_entire_ic_mode_t scharger_v800_flash_state(void);
int scharger_v800_flash_set_led_en(unsigned int reg_val);
#else
static inline enum scharger_v800_entire_ic_mode_t scharger_v800_flash_state(void)
{
	return ENTIRE_IC_STANDBY_MODE;
}
static inline int scharger_v800_flash_set_led_en(unsigned int reg_val)
{
	return 0;
}
#endif
int scharger_v800_flash_irq_init(struct scharger_v800_private_data_t *pdata);
void scharger_v800_flash_irq_work(struct work_struct *work);
#endif