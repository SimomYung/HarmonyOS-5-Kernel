/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: UDK shrinker sysif
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 26 10:03:27 2024
 */
#include <libdevhost/fusion/dev.h>
#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>

#include <udk/shrinker.h>
#include <udk/log.h>

#include "fusion.h"

int libdh_shrinker_query(rref_t rref, int flag)
{
	int ret = 0;

	if (rref != UDK_FUSION_MAGIC_RREF) {
		return E_HM_NODEV;
	}

	ret = udk_shrinker_query(flag);

	return ret;
}

int libdh_shrinker_shrink(rref_t rref, int max_nr, int *ret_nr)
{
	int ret = 0;

	if (rref != UDK_FUSION_MAGIC_RREF) {
		return E_HM_NODEV;
	}

	ret = udk_shrinker_shrink(max_nr, ret_nr);

	return ret;
}
