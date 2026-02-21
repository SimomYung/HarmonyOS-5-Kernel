/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of mem range security checks
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 25 11:27:14 2022
 */
#include <libsec/utility.h>
#include <libsec/mem_range.h>
#include <libsec/mem_range_log.h>
#include <libsec/mac.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>

static void _show_mem_rang_deny(const struct sec_chk_cred *cred, void *arg)
{
	UNUSED(cred);
	const mrange_t *range = (const mrange_t *)arg;
	sec_deny_log("mapping physical memory on [0x%"PRIx64", 0x%"PRIx64")"
		     " as I/O memory failed\n", range->start, range->end);
}

bool sec_check_mem_range_te_policy_failed(const struct sec_chk_cred *cred,
					  uint64_t subj,
					  uint64_t obj,
					  int act,
					  mrange_t *range)
{
	struct sec_log_extra_info info = {
		.func = _show_mem_rang_deny,
		.arg = range,
	};
	return sec_check_te_policy_ex_failed(cred, subj, obj, act, &info);
}

static void _mem_range_fail_log(const struct sec_chk_cred *cred, uint64_t subj, const mrange_t *mrange)
{
	struct sec_deny_log_bundle log;
	mem_zero_s(log);
	log.check_info.subj = subj;
	log.check_info.mrange.start = mrange->start;
	log.check_info.mrange.end = mrange->end;
	log.obj_info = NULL;
	sec_mem_range_chk_fail_log(cred, &log);
}

bool sec_check_mem_range_failed(const struct sec_chk_cred *cred,
				uint64_t subj,
				const mrange_t *mrange)
{
	bool check_passed = true;
	if (!sec_plc_mode_is_disable()) {
		_mem_range_fail_log(cred, subj, mrange);
		if (!sec_plc_mode_is_permissive()) {
			check_passed = false;
		}
	}
	return check_passed;
}
