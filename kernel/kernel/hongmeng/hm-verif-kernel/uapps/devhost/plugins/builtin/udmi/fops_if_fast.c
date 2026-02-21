/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Fast Handler for device file operation sysif API
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 27 20:00:00 2023
 */

#include <libsysif/devhost/server.h>

#include "udmi.h"
#include "types.h"
#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>
#include <devhost/file.h>

int hdr_devhost_fops_open_fast(struct device_info *devinfo,
			       unsigned int vfs_flags,
			       struct filp_node **fnode,
			       rref_t tgt_rref,
			       struct hmkobj_ctx *kobj_ctx)
{
	struct filp_node *node = NULL;
	int ret = E_HM_OK;
	uref_t ap_uref;

	node = devhost_alloc_filp_node_fast(devinfo);
	if (node == NULL) {
		ret = E_HM_NOMEM;
		goto err_alloc;
	}

	if (IS_REF_ERR(ap_uref = libdh_ap_grant(tgt_rref))) {
		ret = REF_TO_ERR(ap_uref);
		goto err_out;
	}

	node->kobj_ctx = kobj_ctx;
	/* filp_node holds on the refcnt of node->devinfo */
	if (devinfo->p->ex_fops->open != NULL) {
		ret = devinfo->p->ex_fops->open(devinfo, &node->filp, vfs_flags, kobj_ctx);
	} else {
		ret = devinfo->p->fops->open(devinfo, &node->filp, vfs_flags);
	}
	if (ret < 0) {
		dh_error("open device failed, devid=%d, device_name=%s, devt=0x%x, ret=%d\n",
			 devinfo->devid, devinfo->devname, devinfo->p->devt, ret);
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	node->f_flags = vfs_flags;
	node->f_modes = trans_acc_mode(get_accmode(vfs_flags));

	*fnode = node;
	return E_HM_OK;

err_out:
	devinfo_put(devinfo);
	free(node);
err_alloc:
	return ret;
}

int hdr_devhost_fops_close_fast(unsigned long long filp, unsigned int rpccnt)
{
	uint64_t res = 0ULL;
	struct filp_node *node = u64_to_ptr(filp, struct filp_node);

	if (rpccnt != 0) {
		NOFAIL(raw_compounded_rpccnt_balance(&node->refcnt.refcnt_nr,
						     (uint64_t)rpccnt,
						     &res));
		BUG_ON(res == 0);
	}

	devhost_put_file(node);
	return E_HM_OK;
}
