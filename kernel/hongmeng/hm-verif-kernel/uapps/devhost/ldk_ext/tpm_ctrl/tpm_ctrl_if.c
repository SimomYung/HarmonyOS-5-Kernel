/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Handler for tpm ctrl operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 10 10:05:44 2023
 */

#include <vfs.h>
#include <devhost/device.h>
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <libdevhost/devhost.h>
#include <libdevhost/uaccess.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <hmsysmgr/process/pid.h>

#include "tpm_ctrl.h"
#include "tpm_ctrl_if.h"

#define TPM_DIGEST_MAX_SIZE	32  /* the value should be same with IMA_DIGEST_SIZE */

static const char *tpm_ctrl_name = "tpm_ctrl";

static struct tpm_ctrl_ops *tpm_ctrl_ops = NULL;

static int tpm_ctrl_ioctl_pcr_extend(unsigned long __user arg)
{
	int ret = 0;
	uint8_t digest[TPM_DIGEST_MAX_SIZE] = {0};
	struct tpm_ioctl_args args = {0};

	ret = hm_actv_read_caller_vm(&args, (void __user *)(uintptr_t)arg, sizeof(struct tpm_ioctl_args));
	if (ret != E_HM_OK) {
		hm_warn("failed to read arg, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	if (TPM_DIGEST_MAX_SIZE < args.size) {
		hm_warn("invalid data size=%zu\n", args.size);
		return -EINVAL;
	}

	ret = hm_actv_read_caller_vm(digest, (void __user *)(uintptr_t)args.digest, args.size);
	if (ret != E_HM_OK) {
		hm_warn("failed to read digest, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	return tpm_ctrl_ops->pcr_extend(args.pcr, TPM_DIGEST_MAX_SIZE, digest);
}

static int tpm_ctrl_ioctl_pcr_read(unsigned long __user arg)
{
	int ret = 0;
	struct tpm_ioctl_args args = {0};

	ret = hm_actv_read_caller_vm(&args, (void __user *)(uintptr_t)arg, sizeof(struct tpm_ioctl_args));
	if (ret != E_HM_OK) {
		hm_warn("failed to read arg, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	return tpm_ctrl_ops->pcr_read(args.pcr, args.size, args.digest);
}

static int tpm_ctrl_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	UNUSED(ctx);
	switch (cmd) {
	case TPM_CTRL_PCR_EXTEND:
		ret = tpm_ctrl_ioctl_pcr_extend(arg);
		break;
	case TPM_CTRL_PCR_READ:
		ret = tpm_ctrl_ioctl_pcr_read(arg);
		break;
	default:
		ret = -EINVAL;
	};

	return ret;
}

static struct devhost_fops tpm_ctrl_fops = {
	.ioctl = tpm_ctrl_ioctl,
};

static void tpm_ctrl_devinfo_release(struct device_info *devinfo)
{
	free(devinfo);
}

static int tpm_ctrl_device_info_init(struct device_info **devinfo)
{
	int ret;
	struct device_info *info = NULL;

	info = calloc(1, sizeof(struct device_info));
	if (info == NULL) {
		hm_warn("failed to calloc device info\n");
		return E_HM_NOMEM;
	}

	/* init tpm ctrl device info */
	ret = devhost_device_init(info);
	if (ret != 0) {
		free(info);
		hm_warn("failed to init device info\n");
		return posix2hmerrno(-ret);
	}
	info->devid = -1;
	info->devname = tpm_ctrl_name;
	info->release = tpm_ctrl_devinfo_release;
	devhost_device_capable_devfs(info, 0, 0, NULL, &tpm_ctrl_fops);

	/* register device info to devmgr */
	ret = devhost_register_device(info);
	if (ret != 0) {
		hm_warn("failed to register device info\n");
		devhost_device_destroy(info);
		return posix2hmerrno(-ret);
	}

	*devinfo = info;
	return E_HM_OK;
}

static struct filp_node *tpm_ctrl_filp_node_alloc(struct device_info *info,
						  __u32 cnode_idx,
						  unsigned int *file_id)
{
	int ret;
	struct filp_node *node = NULL;

	node = devhost_alloc_filp_node(info, cnode_idx);
	if (node == NULL) {
		hm_warn("failed to alloc filp node\n");
		return NULL;
	}

	ret = devhost_register_file(node, file_id);
	if (ret < E_HM_OK) {
		hm_warn("failed to register filp node, err=%s\n", hmstrerror(ret));
		devhost_put_file(node);
		return NULL;
	}

	return node;
}

static int tpm_ctrl_register(struct device_info *devinfo)
{
	int ret;
	uref_t dh_ap_uref;
	unsigned int file_id;
	struct filp_node *node = NULL;
	struct server_info *server = NULL;
	unsigned int dev_fops_flags = 0;

	ret = get_fsmgr_si(&server);
	if (ret != E_HM_OK) {
		hm_warn("failed to get fsmgr server: err=%s\n", hmstrerror(ret));
		return ret;
	}

	dh_ap_uref = libdh_ap_export(server->cnode_idx);
	if (IS_REF_ERR(dh_ap_uref)) {
		hm_warn("failed to get devhost cref\n");
		ret = REF_TO_ERR(dh_ap_uref);
		goto out_put_server;
	}

	node = tpm_ctrl_filp_node_alloc(devinfo, server->cnode_idx, &file_id);
	if (node == NULL) {
		hm_warn("failed to alloc filp node\n");
		ret = -EFAULT;
		goto out_revoke_ucap;
	}

	dev_fops_flags = devhost_device_get_fops_flags(devinfo, node->filp);
	ret = actv_fscall(vfs_register_ima_tpm, server, dh_ap_uref, file_id, dev_fops_flags);
	if (ret != E_HM_OK) {
		hm_warn("failed to register ima tpm, err=%s\n", hmstrerror(ret));
		goto out_put_file;
	}

	put_server_info(server);
	return E_HM_OK;
out_put_file:
	devhost_unregister_file(node->cnode_idx, file_id);
out_revoke_ucap:
	(void)hm_ucap_revoke(0, dh_ap_uref);
out_put_server:
	put_server_info(server);
	return ret;
}

PUBLIC_SYMBOL
int devhost_register_tpm_ctrl(struct tpm_ctrl_ops *tpm_ops)
{
	int ret;
	struct device_info *devinfo = NULL;

	if (tpm_ops == NULL) {
		return -EINVAL;
	}

	if (tpm_ctrl_ops != NULL) {
		return -EBUSY;
	}

	tpm_ctrl_ops = tpm_ops;
	ret = tpm_ctrl_device_info_init(&devinfo);
	if (ret != E_HM_OK) {
		hm_warn("failed to init device info, err=%s\n", hmstrerror(ret));
		ret = -EFAULT;
		goto out_clean_global;
	}

	/* register tpm ctrl device info to ima */
	ret = tpm_ctrl_register(devinfo);
	if (ret != E_HM_OK) {
		hm_warn("failed to register tpm ctrl, err=%s\n", hmstrerror(ret));
		ret = -EFAULT;
		goto out_destroy_device;
	}

	return E_HM_OK;
out_destroy_device:
	devhost_device_destroy(devinfo);
out_clean_global:
	tpm_ctrl_ops = NULL;
	return ret;
}
