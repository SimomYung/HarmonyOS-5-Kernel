/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for policycap
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <hongmeng/compiler.h>
#include <lib/utils.h>
#include <libhmseharmony/policycap.h>

#include "policycap.h"

static const char *policycap_names[] = {
	"network_peer_controls",
	"open_perms",
	"extended_socket_class",
	"always_check_network",
	"cgroup_seclabel",
	"nnp_nosuid_transition",
	"genfs_seclabel_symlinks",
	"ioctl_skip_cloexec"
};

raw_static_assert(HMSEL_POLICYDB_CAPABILITY_MAX == ARRAY_SIZE(policycap_names), capnum_valid);

const char *hmsel_policycap_name(unsigned int idx)
{
	if (idx < ARRAY_SIZE(policycap_names)) {
		return policycap_names[idx];
	} else {
		return "unknown policycap";
	}
}
unsigned int hmsel_policycap_num(void)
{
	return ARRAY_SIZE(policycap_names);
}
