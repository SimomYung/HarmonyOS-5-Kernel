/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK sysfs if
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 10:03:27 2023
 */

#include <libstrict/strict.h>
#include <libsysif/devhost/server.h>

#include <libdevhost/sysfs.h>
#include "fusion.h"
#include "fops_core.h"

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_sysfs_read,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
		    struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_read *pret =
		actvhdlr_drvhandler_devhost_sysfs_read_prepare_ret(sender, credential);
	size_t rsize = 0;
	int ret = libdh_sysfs_read(tagid, pos, &ubuf, ctx, &rsize);

	BUG_ON(pret == NULL);
	if ((!udk_fusion_is_selfcall()) && (!udk_fusion_cnode_is_devmgr())) {
		return E_HM_PERM;
	}
	UNUSED(fd);
	pret->rsize = (unsigned long long)rsize;
	return ret;
}

DEFINE_ACTVHDLR_ALS(drvhandler_devhost_sysfs_write,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
		    struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_write *pret =
		actvhdlr_drvhandler_devhost_sysfs_write_prepare_ret(sender, credential);
	size_t wsize = 0;
	int ret = libdh_sysfs_write(tagid, pos, &ubuf, ctx, &wsize);

	BUG_ON(pret == NULL);
	if ((!udk_fusion_is_selfcall()) && (!udk_fusion_cnode_is_devmgr())) {
		return E_HM_PERM;
	}
	UNUSED(fd);
	pret->wsize = (unsigned long long)wsize;
	return ret;
}
