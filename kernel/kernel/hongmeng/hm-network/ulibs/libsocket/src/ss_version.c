/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation for hmnet version
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include "hmnet_version.h"

#define HMNET_VERSION_MAJOR	0
#define HMNET_VERSION_MINOR	1
#define HMNET_VERSION_PATCH	0
#define HMNET_VERSION_BUILD	0

int hm_net_version_get(struct hm_net_version *version)
{
	struct hm_net_version checker;
	int ret;

	if (version == NULL) {
		return E_HM_INVAL;
	}

	checker.major = HMNET_VERSION_MAJOR;
	checker.minor = HMNET_VERSION_MINOR;
	checker.patch = HMNET_VERSION_PATCH;
	checker.build = HMNET_VERSION_BUILD;

	ret = safe_copy(version, &checker, sizeof(checker));
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

/*
 * @return return value 0 denote version compatible
 *         return value 1 denote version incompatible
 *         return value < 0 denote illegal argument
 */
int hm_net_version_check(const struct hm_net_version *expected)
{
	struct hm_net_version checker;
	int ret;

	if (expected == NULL) {
		return E_HM_INVAL;
	}

	ret = safe_copy(&checker, expected, sizeof(checker));
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}

	if (checker.major > HMNET_VERSION_MAJOR) {
		return 1;
	}
	/* According to semantic versioning, only major will affect compatibility */

	return 0;
}
