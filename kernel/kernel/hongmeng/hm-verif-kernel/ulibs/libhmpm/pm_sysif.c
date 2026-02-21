/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2023. All rights reserved.
 * Description: Common sysif wrapper for power management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 12:46:29 2019
 */
#include "pm_sysif.h"

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libsysif/pwrmgr/api.h>
#include <libsysif/freqmgr/api.h>

enum pm_sysif_index {
	PM_SYSIF_PWRMGR,
	PM_SYSIF_FREQMGR,
	PM_SYSIF_MAX,
};

struct pm_sysif {
	bool inited;
	rref_t rref;
	struct raw_mutex mutex;
};

static struct pm_sysif pm_sysif_array[PM_SYSIF_MAX] = {
	[PM_SYSIF_PWRMGR] = {
		.inited = false,
		.rref = 0ULL,
		.mutex = RAW_MUTEX_INITIALIZER,
	},
	[PM_SYSIF_FREQMGR] = {
		.inited = false,
		.rref = 0ULL,
		.mutex = RAW_MUTEX_INITIALIZER,
	},
};

static const char *index_to_path(enum pm_sysif_index index)
{
	const char *path = NULL;

	switch (index) {
	case PM_SYSIF_PWRMGR:
		path = PWRMGR_PATH;
		break;
	case PM_SYSIF_FREQMGR:
		path = FREQMGR_PATH;
		break;
	case PM_SYSIF_MAX:
		/* fall through */
	default:
		break;
	}

	return path;
}

static rref_t sysif_get_rref(enum pm_sysif_index index)
{
	int err = E_HM_OK;
	rref_t rref;
	const char *path = NULL;
	struct pm_sysif *sysif = &pm_sysif_array[index];

	raw_mutex_lock(&sysif->mutex);
	if (sysif->inited) {
		rref = sysif->rref;
	} else {
		path = index_to_path(index);
		if (path == NULL) {
			err = E_HM_INVAL;
		}
		if (err == E_HM_OK) {
			err = hm_path_acquire(path, &rref);
			if (err == E_HM_NOENT && index == PM_SYSIF_FREQMGR) {
				err = E_HM_NOSYS;
			} else if (err != E_HM_OK) {
				hm_error("path '%s' acquire failed: %s\n",
					 path, hmstrerror(err));
			}
		}
		if (err == E_HM_OK) {
			sysif->inited = true;
			sysif->rref = rref;
		} else {
			rref = ERR_TO_REF(err);
		}
	}
	raw_mutex_unlock(&sysif->mutex);

	return rref;
}

rref_t pm_sysif_get_rref(void)
{
	return sysif_get_rref(PM_SYSIF_PWRMGR);
}

rref_t fm_sysif_get_rref(void)
{
	return sysif_get_rref(PM_SYSIF_FREQMGR);
}
