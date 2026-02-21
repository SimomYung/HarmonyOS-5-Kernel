/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Handler for shrinker sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 30 17:58:29 2023
 */

#include <libsysif/devhost/server.h>

#include <devhost/plugin.h>
#include <devhost/shrinker.h>
#include <libstrict/strict.h>

#include "types.h"
#include "devhost_shrinker.h"

#define max(x, y) ({				\
	typeof(x) ____x = (x);			\
	typeof(y) ____y = (y);			\
	(____x > ____y) ? ____x : ____y;	\
})

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_shrinker_query, dh_shrinker_query_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, type)
{
	UNUSED(sender, credential);
	return devhost_shrinker_query(type);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_shrinker_shrink, dh_shrinker_shrink_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, max_nr)
{
	struct __actvret_drvcall_devhost_shrinker_shrink *pret =
		actvhdlr_drvhandler_devhost_shrinker_shrink_prepare_ret(sender,
								   credential);
	BUG_ON_D(pret == NULL);
	int ret = E_HM_OK;
	ret = devhost_shrinker_shrink(max_nr);
	if (ret >= 0) {
		pret->cnt = ret;
		pret->scanned = max(max_nr, ret); /* scanned = max(upperbound, actual shrinked) */
		return E_HM_OK;
	}
	return ret;
}

/*
 * shrinker sysif module, support devhost shrinker related operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_shrinker_query`
 *  - target sysif-ep: `drvhandler_devhost_shrinker_shrink`
 *
 * export symbol:
 *  - func: `devhost_shrinker_register`
 *  - func: `devhost_shrinker_do_shrink`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(shrinker, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_shrinker_query, dh_shrinker_query_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_shrinker_shrink, dh_shrinker_shrink_ext)
)
