/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Default override for devhost fops sysif
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 20 15:03:27 2020
 */
#include <libsysif/devhost/server.h>

#include <libstrict/strict.h>

/*
 * The 'override' directory is used in the following scene:
 * Some services need to use libdh to have some of devhost's capabilities,
 * so they only implement the part of devhost sysif that they need.
 * During these services, there may be access to the unimplemented part of
 * devhost sysif. To avoid the E_HM_NOSYS error returned by default affecting
 * the service logic, wrapper sysif can be implemented here to override
 * the E_HM_NOSYS error.
 */
DEFINE_ACTVHDLR_ALS(drvhandler_devhost_sysfs_open,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned int, flags, unsigned int, mode, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_open *pret =
		actvhdlr_drvhandler_devhost_sysfs_open_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);

	UNUSED(sender, credential, flags, mode, ctx);
	pret->fd = (size_t)tagid;
	return E_HM_OK;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_sysfs_close,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, uintptr_t, ctx)
{
	UNUSED(sender, credential, tagid, fd, ctx);
	return E_HM_OK;
}
