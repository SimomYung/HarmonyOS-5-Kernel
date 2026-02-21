/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Handler for device file operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 13 16:05:44 2019
 */
#include <libsysif/devhost/server.h>
#include <fs_sysfs.h>
#include <hmkernel/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

#include "udmi.h"
#include "types.h"
#include "devhost.h"
#include "devhost_api.h"
#include "devhost_kobj.h"
#include <devhost/log.h>
#include <devhost/file.h>
#include "utils/actv.h"
#include "utils/utils.h"

static int __devhost_do_open_device(struct device_info *devinfo, struct filp_node *node,
				    rref_t tgt_rref, unsigned int vfs_flags, unsigned int *file_id)
{
	int ret = E_HM_OK;
	uref_t ap_uref;

	if ((devinfo == NULL) || (devinfo->p == NULL)) {
		dh_error("devinfo is invalid\n");
		return E_HM_INVAL;
	}

	/* filp_node holds on the refcnt of node->devinfo */
	if (devinfo->p->ex_fops != NULL && devinfo->p->ex_fops->open != NULL) {
		ret = devinfo->p->ex_fops->open(devinfo, &node->filp, vfs_flags, node->kobj_ctx);
	} else {
		ret = devinfo->p->fops->open(devinfo, &node->filp, vfs_flags);
	}
	if (ret < 0) {
		dh_error("open device failed, devid=%d, device_name=%s, devt=0x%x, ret=%d\n",
			 devinfo->devid, devinfo->devname, devinfo->p->devt, ret);
		ret = posix2hmerrno(-ret);
		return ret;
	}

	if ((ret = devhost_register_file(node, file_id)) < 0) {
		goto err_out;
	}

	/* no need to save ap_uref, it is granted with AUTO_RELEASE flag */
	if (IS_REF_ERR(ap_uref = libdh_ap_grant(tgt_rref))) {
		(void)devhost_unregister_file(node->cnode_idx, *file_id);
		ret = REF_TO_ERR(ap_uref);
		goto err_out;
	}
	return E_HM_OK;

err_out:
	if (devinfo->p->fops->close != NULL) {
		(void)devinfo->p->fops->close(node->filp);
	}
	return ret;
}

static void __dh_pollkobj_release(struct raw_refcnt *ref)
{
	int ret = E_HM_OK;
	struct hmkobj_ctx *kobj_ctx = container_of(ref, struct hmkobj_ctx, refcnt);

	BUG_ON(kobj_ctx == NULL);
	if (kobj_ctx->kobj != NULL) {
		ret = dh_ctrlmem_kobj_destroy(kobj_ctx->kobj);
		if (ret != E_HM_OK) {
			dh_warn("devhost destroy ctrlmem kobj failed, err=%s\n", hmstrerror(ret));
		}
	}

	free(kobj_ctx);
}

static struct hmkobj_ctx *__dh_pollkobj_create(struct device_info *devinfo,
					       unsigned long long *kshm_key,
					       unsigned long *kobj_offset)
{
	struct hmkobj_ctx *kobj_ctx;
	struct dh_ctrlmem_page_grant_info grant_info = { 0 };

	if (!device_flag_test(devinfo->p->flags, DEVICE_SAFEPOLL)) {
		return NULL;
	}

	kobj_ctx = (struct hmkobj_ctx *)calloc(1, sizeof(*kobj_ctx));
	if (kobj_ctx == NULL) {
		return NULL;
	}

	/* `__CTRLMEM_U32_MAX_VAL`: means always synchronize revents to ctrlmem kobj */
	kobj_ctx->kobj = dh_ctrlmem_spawn_polllist(0UL, __CTRLMEM_U32_MAX_VAL, kobj_offset, &grant_info);
	if (kobj_ctx->kobj == NULL) {
		free(kobj_ctx);
		return NULL;
	}

	*kshm_key = grant_info.fsmgr_shm_key;
	dh_debug("devhost dev(%s) create new pollkobj: kshm_key=%#llx, kobj_offset=%#lx\n",
		 devinfo->devname, *kshm_key, *kobj_offset);

	raw_refcnt_init(&kobj_ctx->refcnt, 1, __dh_pollkobj_release);
	return kobj_ctx;
}

int hdr_devhost_fops_open(unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags,
			  struct __actvret_drvcall_devhost_fops_open *pret)
{
	unsigned int cnode_idx = u_cidx;
	struct device_info *devinfo = NULL;
	struct filp_node *node = NULL;
	unsigned int file_id = 0;
	uref_t ap_uref;
	int ret = E_HM_OK;
	struct hmkobj_ctx *kobj_ctx = NULL;

	if (pret == NULL) {
		return E_HM_INVAL;
	}
	if ((ret = devhost_acquire_device(devid, &devinfo)) != 0) {
		return ret;
	}

	if ((vfs_flags & O_NO_OPEN) != 0) { /* O_NO_OPEN is used by transfs mount */
		/* no need to save ap_uref, it is granted with AUTO_RELEASE flag */
		if (IS_REF_ERR(ap_uref = libdh_ap_grant(tgt_rref))) {
			ret = REF_TO_ERR(ap_uref);
		}
		goto out;
	} else {
		/* check if target device support open */
		if ((devinfo->p->fops == NULL) || (devinfo->p->fops->open == NULL)) {
			ret = E_HM_NOSYS;
			goto out;
		}
	}

	kobj_ctx = __dh_pollkobj_create(devinfo, &pret->kshm_key, &pret->kobj_offset);
	if (vfs_flags & O_FASTPATH) {
		ret = hdr_devhost_fops_open_fast(devinfo, vfs_flags & (~O_FASTPATH),
						 &node, tgt_rref, kobj_ctx);
		if (ret == E_HM_OK) {
			pret->filp = ptr_to_u64(node);
			pret->dev_fops_flags = devhost_device_get_fops_flags(devinfo, node->filp);
		}
		goto out;
	}

	if ((node = devhost_alloc_filp_node(devinfo, cnode_idx)) == NULL) {
		ret = E_HM_NOMEM;
		goto out;
	}

	node->kobj_ctx = kobj_ctx;
	if ((ret = __devhost_do_open_device(devinfo, node,
					    tgt_rref, vfs_flags, &file_id)) != 0) {
		devinfo_put(node->devinfo);
		free(node);
		goto out;
	}

	pret->filp = (unsigned long long)file_id;
	pret->dev_fops_flags = devhost_device_get_fops_flags(devinfo, node->filp);
	devhost_put_file(node);

out:
	if (ret != E_HM_OK && kobj_ctx != NULL) {
		devhost_put_kobj(kobj_ctx);
	}
	devinfo_put(devinfo);
	return ret;
}

int hdr_devhost_fops_close(unsigned long long sender, unsigned long credential,
			   unsigned int fastpath, unsigned long long file_id)
{
	unsigned int cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	int ret = E_HM_OK;

	UNUSED(credential);

	if (fastpath) {
		return hdr_devhost_fops_close_fast(file_id,
			(fastpath & HMSRV_IO_CTX_FASTPATH_RPCCNT_MASK) >>
			HMSRV_IO_CTX_FASTPATH_FLAG_SHIFT);
	}

	ret = devhost_unregister_file((uint64_t)cnode_idx, (unsigned int)file_id);
	if (ret < 0) {
		dh_error("file_id is invalid\n");
		return ret;
	}

	return E_HM_OK;
}
