/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Provide mem range check functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 25 11:18:53 2022
 */
#ifndef ULIBS_LIBSEC_MEM_RANGE_H
#define ULIBS_LIBSEC_MEM_RANGE_H

#include <libpolicy_api.h>
#include <policy_def.h>
#include "cred.h"

bool sec_check_mem_range_failed(const struct sec_chk_cred *cred,
				uint64_t subj,
				const mrange_t *mrange);
bool sec_check_mem_range_te_policy_failed(const struct sec_chk_cred *cred,
					  uint64_t subj,
					  uint64_t obj,
					  int act,
					  mrange_t *range);

#endif
