/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of config
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 27 14:20:01 2019
 */
#include <stdio.h>
#include <stdlib.h>

#include <libmem/utils.h>
#include <libsysif_net/api.h>

#include "hmnet_config.h"
#include "ss_shmem.h"
#include "ss_channel.h"

int hm_net_show_stats(const char *protocol)
{
	struct ss_shmem_info shm = {0};
	struct sysif_protocol ss_protocol = {0};
	rref_t rref;
	int ret;

	mem_zero_s(ss_protocol);

	if (protocol != NULL) {
		ret = safe_copy(ss_protocol.protocol, protocol, sizeof(ss_protocol.protocol));
		if (ret < 0) {
			hm_error("safe_copy hm_net_cfg protocol failed, ret=%d\n", ret);
			return E_HM_POSIX_FAULT;
		}
	}

	ret = ss_shmem_alloc(&shm);
	if (ret < 0) {
		hm_error("alloc shmem failed, ret=%d\n", ret);
		return E_HM_NOMEM;
	}

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		ss_shmem_free(&shm);
		return ret;
	}

	ret = actvcapcall_netcall_cfg_show_stats(rref,
						 ss_protocol, shm.server_id);
	ss_shmem_free(&shm);
	hm_net_put_rref(&rref);

	return ret;
}

#define HMNET_LOG_SETTING_MASK		0x3fffffff
#define HMNET_LOG_SETTING_LABEL		0x40000000
#define HMNET_LOG_SETTING_AF_LABEL	0x80000000
int hm_net_cfg_log(const char *module, const char *arg)
{
	rref_t rref;
	int ret;
	uint32_t setting;

	if (module == NULL || arg == NULL) {
		return E_HM_INVAL;
	}

	if (strncmp(module, "mask", 4) == 0) {
		setting = (uint32_t)strtol(arg, NULL, 16);
		setting &= HMNET_LOG_SETTING_MASK;
	} else if (strncmp(module, "tcpip", 5) == 0) {
		setting = (uint32_t)strtol(arg, NULL, 0);
		setting &= HMNET_LOG_SETTING_MASK;
		setting |= HMNET_LOG_SETTING_AF_LABEL;
	} else if (strncmp(module, "netd", 4) == 0) {
		setting = (uint32_t)strtol(arg, NULL, 0);
		setting &= HMNET_LOG_SETTING_MASK;
		setting |= HMNET_LOG_SETTING_LABEL;
	} else {
		return E_HM_INVAL;
	}

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_cfg_loglevel(rref, setting);

	hm_net_put_rref(&rref);

	return ret;
}
