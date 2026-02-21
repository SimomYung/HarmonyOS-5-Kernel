/*
 * load_iomcu_ca.h
 *
 * head of load_iomcu_ca.c
 *
 * Copyright (c) 2022-2023 Huawei Technologies Co., Ltd.
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

#ifndef __LOAD_IOMCU_CA_H__
#define __LOAD_IOMCU_CA_H__

#define LOAD_IOMCU_OK               0
#define LOAD_IOMCU_ERROR            1

int load_and_verify_iomcu_image(void);

#endif /* __LOAD_IOMCU_CA_H__ */

