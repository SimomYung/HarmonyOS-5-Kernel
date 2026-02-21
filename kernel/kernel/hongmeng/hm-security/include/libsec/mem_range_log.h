/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Provides mem range logging function(s)
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 25 11:18:53 2022
 */
#ifndef ULIBS_LIBSEC_MEM_RANGE_LOG_H
#define ULIBS_LIBSEC_MEM_RANGE_LOG_H

#include <libpolicy_api.h>
#include <policy_def.h>

#include "cred.h"
#include "log_cache.h"
#include "module.h"

extern struct sec_mod sec_mod_mem_range;
void sec_mem_range_chk_fail_log(const struct sec_chk_cred *cred, const struct sec_deny_log_bundle *log);

#endif
