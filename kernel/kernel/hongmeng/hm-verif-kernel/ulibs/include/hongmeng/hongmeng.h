/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hongmeng
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 12:57:57 2018
 */
#ifndef ULIBS_HONGMENG_HONGMENG_H
#define ULIBS_HONGMENG_HONGMENG_H

#include <stdbool.h>

void hm_set_fork_not_allowed(void);

bool hm_fetch_fork_allowd_flag(void);

#endif
