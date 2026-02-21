/*
 * dfa platform header file
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _DFA_PLATFORM_H
#define _DFA_PLATFORM_H

#include "scsi/ufs/ufshcd.h"

int dfa_que_proc_init(struct ufs_hba *hba);
void dfa_em_check(struct ufs_hba *hba);
int dfa_early_reset(struct ufs_hba *hba);
int dfa_power_off(struct ufs_hba *hba);
int dfa_power_on(struct ufs_hba *hba);
int dfa_que_proc_reload(struct ufs_hba *hba);

#endif
