/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Stringify
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 17 15:59:12 2024
 */
#ifndef HMKERNEL_STRINGIFY_H
#define HMKERNEL_STRINGIFY_H

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#endif
