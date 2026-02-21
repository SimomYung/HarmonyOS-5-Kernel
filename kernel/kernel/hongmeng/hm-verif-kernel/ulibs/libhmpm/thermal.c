/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The ulibs for thermal
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 04 19:27:45 2023
 */

#include <libhmpm/thermal.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/pwrmgr/api.h>

#include "pm_sysif.h"

struct thermal_stat_config {
	rref_t rref;
	struct thermal_zone_type type;
};

static int set_thermal_type(struct thermal_zone_type *tz_type, const char *type)
{
	int err = E_HM_OK;
	size_t len;

	len = strnlen(type, THERMAL_ZONE_TYPE_MAX);
	if (len >= THERMAL_ZONE_TYPE_MAX || len == 0) {
		hm_warn("invalid thermal type len\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(tz_type->string, sizeof(tz_type->string),
				 type, len));
	}

	return err;
}

static int thermal_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct thermal_stat_config *config = (struct thermal_stat_config *)data;

	return actvcapcall_pmcall_thermmgr_stat(config->rref, config->type, *attr);
}

int hm_thermal_stat(const char *type, struct bunch *bunch)
{
	int err = E_HM_OK;
	rref_t rref = 0;
	struct thermal_zone_type tz_type;
	struct thermal_stat_config config;
	struct bunch_ipc_reader_ctx ctx;

	if (bunch == NULL || type == NULL) {
		hm_error("invalid type or bunch\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = set_thermal_type(&tz_type, type);
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(config);
		config.type = tz_type;
		config.rref = rref;
		ctx.ipc = thermal_stat_ipc;
		ctx.private_data = ptr_to_void(&config);
		ctx.target_info = rref;
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_error("bunch ipc request failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}
