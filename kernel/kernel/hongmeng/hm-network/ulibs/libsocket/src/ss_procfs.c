/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of procfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 31 19:53:08 CST 2023
 */

#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>

#include "ss_channel.h"
#include "hmnet_procfs.h"

int hmnet_procfs_create_ldk_entry(unsigned int nsid, const char* path,
				struct hmnet_procfs_ldk_entry_ops *ops,
				void *ctx, unsigned int mode)
{
	rref_t rref;
	int ret = 0;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		hm_error("netcall procfs create failed to get rref ret = %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = actvcapcall_netcall_procfs_create_ldk_entry(rref, nsid, path, ops, ctx, mode);
	if (ret < 0 && ret != E_HM_EXIST) {
		hm_error("netcall procfs create ldk entry failed ret = %s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}

int hmnet_procfs_delete_ldk_entry_specific(const char *pathname, unsigned int nsid)
{
	rref_t rref;
	int ret = 0;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	ret = actvcapcall_netcall_procfs_delete_ldk_entry_specific(rref, pathname, nsid);

	return -hmerrno2posix(ret);
}

int hmnet_procfs_create_ldk_entries(void* parent, unsigned int size,
			struct hmnet_procfs_ldk_entry_ops *ops)
{
	rref_t rref;
	int ret = 0;

	ret = hm_net_ap_rref_noput(&rref);
	if (ret < 0) {
		hm_error("netcall procfs create failed to get rref ret = %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = actvcapcall_netcall_procfs_create_ldk_entries(rref, parent, size, ops);
	if (ret < 0 && ret != E_HM_EXIST) {
		hm_error("netcall procfs create ldk entry failed ret = %s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}
