/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kbox parameters
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 9 22:34:50 2019
 */

#ifndef ULIBS_LIBKBOX_PARAM_H
#define ULIBS_LIBKBOX_PARAM_H

#include <hmkernel/const.h>
#include <hmkernel/drivers/kbox.h>

#define KBOX_ALIGN		__KBOX_DEVICE_HEAD_SIZE
#define KBOX_SIZE_MIN		(__U(1) << 15U) /* 32KB */
#define KBOX_SIZE_MAX		(__U(1) << 30U) /* 1GB */
#define KBOX_REGION_COUNT_MAX	64U
#define KBOX_REGION_SIZE_MIN	(__U(1) << 12U) /* 4KB */
#define KBOX_REGION_SIZE_MAX	(__U(1) << 30U) /* 1GB */
#define KBOX_REGION_WRITE_SIZE	(__U(1) << 10U) /* max bytes per write */
#define KBOX_REGION_NAME_SIZE	__KBOX_REGION_NAME_SIZE

#endif
