/*
 * hufs_hibern8.h
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

#ifndef HUFS_HIBERN8_H
#define HUFS_HIBERN8_H
#include "scsi/ufs/ufshcd.h"

/* Auto Hibern8 Timer, 30ms */
#define UFS_AHIT_AUTOH8_TIMER_30ms 0x0C1E

enum auto_hibern8_limited_operation {
	LIMITED_TIME_STOP = 0,
	LIMITED_TIME_30MS_START,
};

int hufs_hibern8_config(struct scsi_device *sdev);
#ifdef CONFIG_CGROUP_IOLIMIT_HIBERN8
extern void ufshcd_iolimit_reset_h8(void);
extern void ufshcd_iolimit_wakeup_and_update_h8(void);
#endif
#endif
