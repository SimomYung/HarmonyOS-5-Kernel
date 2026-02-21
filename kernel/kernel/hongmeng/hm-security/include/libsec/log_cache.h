/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Log bundle information for security framework
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 29 19:20:59 2022
 */
#ifndef ULIBS_LIBSEC_LOG_CACHE_H
#define ULIBS_LIBSEC_LOG_CACHE_H
#include <stdarg.h>
#include <stdint.h>
#include <libsecplc/policy_fmt.h>
#include <policy_def.h>

#include "cred.h"

/* It aggregates all kinds of security checks (MAC, ability, mem_range, and port range) */
struct sec_check_info {
	uint64_t subj;
	union {
		struct {
			enum action_def action;
			uint64_t obj;
		};
		enum ability_def ability;
		mrange_t mrange;
		sec_plc_port_range_t port_range;
	};
};

struct sec_deny_log_bundle {
	struct sec_check_info check_info;
	struct sec_log_extra_info *obj_info;
};

#endif
