/*
 * npu_platform_pm.h
 *
 * about npu hwts plat
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
#ifndef __NPU_PLATFORM_PM_SPEC_H
#define __NPU_PLATFORM_PM_SPEC_H

enum npu_subip {
	NPU_IP_TOP              = 0, /* npu subsys, npu bus */
	NPU_IP_NON_TOP             = 1, /* tzpc */
	NPU_IP_AICORE_GE          = 2, /* ts subsys, hwts init, tcu */
	NPU_IP_AICORE_EFF         = 3, /* aicore subsys, aicore tbu init */
	NPU_IP_TSCPU   = 4, /* ts_subsys_tbu_connect, tscpu startup, doorbell */
	NPU_SUBIP_MAX,
};

#endif
