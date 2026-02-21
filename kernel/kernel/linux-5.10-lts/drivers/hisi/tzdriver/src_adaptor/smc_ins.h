/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: declarations of unified API for tzdriver
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef SMC_INS_H
#define SMC_INS_H

#ifndef CONFIG_LIBLINUX
#define SMCCC_SMC_INST	"smc	#0"
#else
#define SMCCC_SMC_INST "svc	#0x4200"
#endif

#endif