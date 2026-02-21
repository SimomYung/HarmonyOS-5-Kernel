/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The templates of security
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 16:46:22 2020
 */

#include "security.h"

#include <policy_def.h>
#include <libpolicy_api.h>

#include <devhost_api.h>
#include <devhost/log.h>
#include <sys/mman.h>
#include <libsec/dac.h>
#include <libsec/server.h>
#include <libsec/ability.h>
#include <libsec/posix_cap.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#ifdef CONFIG_SEHARMONY
#include <libhmseharmony/seharmony.h>
#endif

#include "ability.h"
#include "posix_cap.h"
#ifdef CONFIG_SEHARMONY
#include "seharmony.h"
#include "overlay.h"
#endif

static struct sec_mod *devhost_sec_mod[] = {
	&sec_mod_ability,
	&sec_mod_dac,
#ifdef CONFIG_POSIX_CAPS
	&sec_mod_posix_cap,
#endif
#ifdef CONFIG_SEHARMONY
	&sec_mod_seharmony,
#endif
};

static struct sec_comp *devhost_sec_comp[] = {
	&sec_comp_ability_common,
	&sec_comp_ability_net,
	&sec_comp_ability_net_socket,
	&sec_comp_ability_iaware_notify,
	&sec_comp_ability_kstate,
	&sec_comp_ability_fileguard,
	&sec_comp_ability_rpc,
	&sec_comp_ability_fops,
	&sec_comp_ability_pm,
	&sec_comp_ability_clock,
#ifdef CONFIG_POSIX_CAPS
	&sec_comp_posix_cap_clock,
#endif
#ifdef CONFIG_DEVHOST_ENTROPY_COLLECT
	&sec_comp_ability_entropy_dh,
#endif
#ifdef CONFIG_SEHARMONY
	&sec_comp_seharmony_devhost,
	&sec_comp_overlay_devhost_devhost,
#endif
};

int devhost_security_init(void)
{
	void *addr = NULL;
	int rc;
	size_t sz = 0UL;
	addr = policy_mmap(POLICY_SHM, &sz);
	if (addr == MAP_FAILED) {
		hm_error("mmap policy for devhost failed\n");
		return E_HM_INVAL;
	}
	rc = policy_init(addr, sz);
	if (rc != 0) {
		hm_error("devhost policy init failed\n");
		goto init_failed;
	}
	rc = sec_server_init(devhost_sec_mod, ARRAY_SIZE(devhost_sec_mod),
			     devhost_sec_comp, ARRAY_SIZE(devhost_sec_comp), false);
	if (rc != 0) {
		hm_error("devhost server init failed\n");
		goto init_failed;
	}
	return rc;
init_failed:
	policy_munmap(addr, sz);
	return rc;
}

static int dh_ability_map[(int)DH_ABILITY_NR] = {
	[DH_ABILITY_TTY] = ab_tty,
	[DH_ABILITY_SERIAL_TOGGLE] = ab_serial_toggle,
};

static int dh_capability_map[(int)DH_CAP_NR] = {
	[DH_CAP_ADMIN] = POSIX_CAP_SYS_ADMIN,
	[DH_CAP_TTY_CONFIG] = POSIX_CAP_SYS_TTY_CONFIG,
};

static int __sec_chk_has_ability(const struct sec_chk_cred *cred, int ability)
{
	return sec_check_ability(cred, mac_cred_of_sec_cred(cred), ability, sec_chk_show_obj_name, NULL);
}

static int sec_chk_has_ability(int ability)
{
	int ret;
	struct sec_chk_cred cred = {0};
	ret = sec_chk_cred_init_requester(&cred);
	if (ret != E_HM_OK) {
		hm_error("cred init requester failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = __sec_chk_has_ability(&cred, ability);
	return ret;
}

#ifdef CONFIG_POSIX_CAPS
static int __sec_chk_has_capability(const struct sec_chk_cred *cred, unsigned int capability)
{
	struct sec_cred_caps *posix_cap;

	posix_cap = caps_cred_of_sec_cred(cred);
	return sec_do_capable(cred, posix_cap->eff_cap,
				   capability) ? E_HM_OK : E_HM_PERM;
}

static int sec_chk_has_capability(unsigned int capability)
{
	int ret;
	struct sec_chk_cred cred = {0};
	ret = sec_chk_cred_init_requester(&cred);
	if (ret != E_HM_OK) {
		hm_error("cred init requester failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = __sec_chk_has_capability(&cred, capability);
	return ret;
}
#else
static int sec_chk_has_capability(unsigned int capability)
{
	(void)capability;
	return E_HM_OK;
}
#endif

static int sec_chk_has_ab_and_cap(int ability, unsigned int capability)
{
	int ret;
	struct sec_chk_cred cred = {0};
	ret = sec_chk_cred_init_requester(&cred);
	if (ret != E_HM_OK) {
		hm_error("cred init requester failed, ret=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = __sec_chk_has_ability(&cred, ability);
#ifdef CONFIG_POSIX_CAPS
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = __sec_chk_has_capability(&cred, capability);
	return ret;
#else
	(void)capability;
	return ret;
#endif
}

PUBLIC_SYMBOL
int devhost_caller_has_ability(int ability)
{
	int hm_ability;
	if ((ability < 0) || (ability >= (int)DH_ABILITY_NR)) {
		dh_warn("ability: %d is invalid\n", ability);
		return -EINVAL;
	}
	hm_ability = dh_ability_map[ability];
	if (hm_ability < 0) {
		dh_warn("ability: %d is removed\n", ability);
		return -EPERM;
	}
	return -hmerrno2posix(sec_chk_has_ability(hm_ability));
}

PUBLIC_SYMBOL
int devhost_caller_has_capability(int cap)
{
	int posix_cap;
	if ((cap < 0) || (cap >= (int)DH_CAP_NR)) {
		dh_warn("capability: %d is invalid\n", cap);
		return -EINVAL;
	}
	posix_cap = dh_capability_map[cap];
	if (posix_cap < 0) {
		dh_warn("capability: %d is removed\n", cap);
		return -EPERM;
	}
	return -hmerrno2posix(sec_chk_has_capability((unsigned int)posix_cap));
}

PUBLIC_SYMBOL
int devhost_caller_has_ab_and_cap(int ability, int cap)
{
	int hm_ability;
	int posix_cap;

	if ((ability < 0) || (ability >= (int)DH_ABILITY_NR) ||
	    (cap < 0) || (cap >= (int)DH_CAP_NR)) {
		dh_warn("invalid ability: %d or capability: %d\n", ability, cap);
		return -EINVAL;
	}

	hm_ability = dh_ability_map[ability];
	posix_cap = dh_capability_map[cap];
	if (hm_ability < 0 || posix_cap < 0) {
		dh_warn("removed ability: %d or capability: %d\n", ability, cap);
		return -EPERM;
	}
	return -hmerrno2posix(sec_chk_has_ab_and_cap(hm_ability, (unsigned int)posix_cap));
}
