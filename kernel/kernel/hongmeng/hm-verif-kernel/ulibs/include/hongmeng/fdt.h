/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The purpose of this header file is to control the inclusion of
 * libhmfdt.h based on the macro CONFIG_SUPPORT_HMFDT
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 31 10:53:34 2023
 */

#ifndef ULIBS_HONGMENG_FDT_H
#define ULIBS_HONGMENG_FDT_H

#ifndef CONFIG_SUPPORT_HMFDT
#include <libfdt.h>
#else
#include <libhmfdt.h>
#endif /* !CONFIG_SUPPORT_HMFDT */

#endif
