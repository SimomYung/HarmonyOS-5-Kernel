/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK security check
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 07 06:28:45 2021
 */
#include <udk/log.h>
#include <udk/types.h>
#include <udk/security.h>
#include <devhost/security.h>

#include <libsec/posix_cap.h>

#define DEFINE_UDK_ABILITY(ability, ability_s)			\
	int udk_ability_of_##ability(void)		\
	{						\
		return (int)DH_ABILITY_##ability_s;		\
	}

#define DEFINE_UDK_CAPABILITY(cap, cap_s)			\
	int udk_capability_of_##cap(void)		\
	{						\
		return (int)DH_CAP_##cap_s;			\
	}

DEFINE_UDK_ABILITY(tty, TTY);
DEFINE_UDK_ABILITY(serial_toggle, SERIAL_TOGGLE);

DEFINE_UDK_CAPABILITY(admin, ADMIN);
DEFINE_UDK_CAPABILITY(tty_config, TTY_CONFIG);

int udk_caller_has_ability(int ability)
{
	return devhost_caller_has_ability(ability);
}

int udk_caller_has_capability(int cap)
{
	return devhost_caller_has_capability(cap);
}

int udk_caller_has_ab_and_cap(int ability, int cap)
{
	return devhost_caller_has_ab_and_cap(ability, cap);
}

bool udk_capable(int cap)
{
	int ret;
	struct sec_chk_cred cred;

	mem_zero_s(cred);
	ret = sec_chk_cred_init_requester(&cred);
	if (ret != 0) {
		udk_warn("udk cap %d failed get cred\n", cap);
		return false;
	}

	bool res = sec_capable(&cred, (unsigned int)cap);
	return res;
}
