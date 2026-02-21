/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: devhost posix syscall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 9 19:30:00 2024
 */
#include <libsysif/devhost/server.h>

#include <devhost/plugin.h>
#include <devhost/security.h>
#include <libhmactv/actv.h>
#include <libdevhost/devhost.h>

#include "devhost.h"

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_posix_vhangup, dh_posix_vhangup_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);
	if (devhost_caller_has_capability(DH_CAP_TTY_CONFIG) < 0) {
		return E_HM_PERM;
	}

	devhost_backend_vhangup();
	return E_HM_OK;
}

DEFINE_BUILTIN_PLUGIN(posix_syscall, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_posix_vhangup, dh_posix_vhangup_ext)
);
