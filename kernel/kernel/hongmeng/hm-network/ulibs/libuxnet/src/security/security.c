/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The security for network
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 14:51:31 2019
 */
#include <libpolicy_api.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_security.h>

#include "ux_netlog.h"
#include "security/ux_security.h"
#include "security/security.h"
#include "security/ability.h"
#include "security/seharmony.h"
#include "security/posix_cap.h"
#ifdef CONFIG_SEHARMONY
#include <libhmseharmony/seharmony.h>
#endif

#define SIXTEEN_BIT_MAX 0xffff

static struct sec_mod *mod[] = {
	&sec_mod_ability,
#ifdef CONFIG_POSIX_CAPS
	&sec_mod_posix_cap,
#endif
#ifdef CONFIG_SEHARMONY
	&sec_mod_seharmony,
#endif
};

static struct sec_comp *comp[] = {
	&sec_comp_ability_reserved_port_range,
	&sec_comp_ability_raw_socket,
	&sec_comp_ability_net_admin,
	&sec_comp_ability_net_bind,
	&sec_comp_ability_net_dev,
	&sec_comp_ability_net_fs,
	&sec_comp_ability_as_devhost,
	&sec_comp_ability_as_netd,
#ifdef CONFIG_POSIX_CAPS
	&sec_comp_posix_cap_net,
#endif
#ifdef CONFIG_VIRTIO_VSOCK
	&sec_comp_ability_vsock,
#endif
	&sec_comp_ability_net_stack_init,
#ifdef CONFIG_SEHARMONY
	&sec_comp_seharmony_net,
#endif
};

static int setup_all_reserved_ports(void)
{
	struct sec_plc_port_range_iterator it = {0};
	struct sec_plc_port_range_unit range = {0};
	int ret;

	ret = sec_plc_port_range_get_reserved(&it);
	if (ret != 0) {
		net_error(HMNET_CORE, "failed to get all reserved ports, ret=%d\n", ret);
		return E_HM_INVAL;
	}

	for (; sec_plc_iter_port_range_get(&it, &range) == E_HM_OK;
		sec_plc_iter_port_range_next(&it)) {
		if (range.start > range.end || range.start > SIXTEEN_BIT_MAX || range.end > SIXTEEN_BIT_MAX) {
			net_error(HMNET_CORE, "out of range\n");
			return E_HM_INVAL;
		}
		ux_sec_rsvport_add_range((uint16_t)(range.start), (uint16_t)(range.end));
	}

	return E_HM_OK;
}

int hmnetd_security_init(void)
{
	void *addr = NULL;
	int rc;
	size_t sz = 0;
	int err;

	addr = policy_mmap(POLICY_SHM, &sz);
	if (addr == MAP_FAILED) {
		net_error(HMNET_CORE, "mmap policy failed.\n");
		return E_HM_INVAL;
	}

	rc = policy_init(addr, sz);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "fs policy init failed.\n");
		goto init_failed;
	}

#if defined(CONFIG_SEHARMONY) && !defined(CONFIG_NET_FUSION)
	rc = hm_sec_shconf_init();
	if (rc != E_HM_OK) {
		hm_error("failed to init shconf, err=%s\n", hmstrerror(rc));
		goto init_failed;
	}
#endif

	rc = sec_server_init(
		mod, ARRAY_SIZE(mod), comp, ARRAY_SIZE(comp), false);
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "security server init failed\n");
		goto init_failed;
	}

	rc = setup_all_reserved_ports();
	if (rc != E_HM_OK) {
		net_error(HMNET_CORE, "get reserved ports failed\n");
		goto setup_failed;
	}

	return E_HM_OK;

setup_failed:
	sec_server_fini(mod, ARRAY_SIZE(mod), comp, ARRAY_SIZE(comp));
init_failed:
	err = hm_mem_munmap(addr, sz);
	if (err < 0) {
		net_error(HMNET_CORE, "security: hm_mem_unmap failed: %s\n", hmstrerror(err));
	}
	return E_HM_INVAL;
}
