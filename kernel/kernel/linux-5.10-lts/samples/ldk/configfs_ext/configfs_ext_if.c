// SPDX-License-Identifier: GPL-2.0
#include <vfs.h>
#include <devhost/device.h>
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <libdevhost/devhost.h>
#include <libdevhost/uaccess.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <hmsysmgr/process/pid.h>

#include "configfs_ext_if.h"

#define CONFIGFS_HMDFS_GET_BID 0x1
#define CONFIGFS_SHAREFS_GET_BID 0x4

#define HMDFS_BNAME_MAX_SIZE 256

static const char *configfs_ext_name = "configfs_ext";

static struct configfs_ext_ops *configfs_ext_ops = NULL;

/* sharefs share the same args format with hmdfs */
struct hmdfs_get_bid_ioctl_args {
	char *bname;
	size_t size;
};

static int configfs_ext_ioctl_hmdfs_get_bid(unsigned long __user arg)
{
	int ret = 0;
	struct hmdfs_get_bid_ioctl_args args = {0};
	char bname[HMDFS_BNAME_MAX_SIZE] = {0};
	int bid = 0;

	ret = hm_actv_read_caller_vm(&args, (void __user *)(uintptr_t)arg, sizeof(struct hmdfs_get_bid_ioctl_args));
	if (ret != E_HM_OK) {
		hm_warn("failed to read arg, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	if (args.size > HMDFS_BNAME_MAX_SIZE) {
		hm_warn("args size is too long\n");
		return -EINVAL;
	}

	ret = hm_actv_read_caller_vm(bname, (void __user *)(uintptr_t)args.bname, args.size);
	if (ret != E_HM_OK) {
		hm_warn("failed to read args.bname, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	bid = get_bid(bname);

	return bid;
}

static int configfs_ext_ioctl_sharefs_get_bid(unsigned long __user arg)
{
	int ret = 0;
	struct hmdfs_get_bid_ioctl_args args = {0};
	char bname[HMDFS_BNAME_MAX_SIZE] = {0};
	int bid = 0;

	ret = hm_actv_read_caller_vm(&args, (void __user *)(uintptr_t)arg, sizeof(struct hmdfs_get_bid_ioctl_args));
	if (ret != E_HM_OK) {
		hm_warn("failed to read arg, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	if (args.size > HMDFS_BNAME_MAX_SIZE) {
		hm_warn("args size is too long\n");
		return -EINVAL;
	}

	ret = hm_actv_read_caller_vm(bname, (void __user *)(uintptr_t)args.bname, args.size);
	if (ret != E_HM_OK) {
		hm_warn("failed to read args.bname, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	bid = get_bid_config(bname);

	return bid;
}

static int configfs_ext_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	UNUSED(ctx);
	switch (cmd) {
	case CONFIGFS_HMDFS_GET_BID:
		ret = configfs_ext_ioctl_hmdfs_get_bid(arg);
		break;
	case CONFIGFS_SHAREFS_GET_BID:
		ret = configfs_ext_ioctl_sharefs_get_bid(arg);
		break;
	default:
		ret = -EINVAL;
	};

	return ret;
}

static struct devhost_fops configfs_ext_fops = {
	.ioctl = configfs_ext_ioctl,
};

static void configfs_ext_devinfo_release(struct device_info *devinfo)
{
	free(devinfo);
}

static int configfs_ext_device_info_init(struct device_info **devinfo)
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
	info->devname = configfs_ext_name;
	info->release = configfs_ext_devinfo_release;
	devhost_device_capable_devfs(info, 0, 0, NULL, &configfs_ext_fops);

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

static struct filp_node *configfs_ext_filp_node_alloc(struct device_info *info,
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

static int configfs_ext_register(struct device_info *devinfo)
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

	node = configfs_ext_filp_node_alloc(devinfo, server->cnode_idx, &file_id);
	if (node == NULL) {
		hm_warn("failed to alloc filp node\n");
		ret = -EFAULT;
		goto out_revoke_ucap;
	}

	dev_fops_flags = devhost_device_get_fops_flags(devinfo, node->filp);
	ret = actv_fscall(vfs_register_configfs_ext, server, dh_ap_uref, file_id, dev_fops_flags);
	if (ret != E_HM_OK) {
		hm_warn("failed to register configfs_ext, err=%s\n", hmstrerror(ret));
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

int devhost_register_configfs_ext(void)
{
	int ret;
	struct device_info *devinfo = NULL;

	ret = configfs_ext_device_info_init(&devinfo);
	if (ret != E_HM_OK) {
		hm_warn("failed to init device info, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	/* register tpm ctrl device info to ima */
	ret = configfs_ext_register(devinfo);
	if (ret != E_HM_OK) {
		hm_warn("failed to register tpm ctrl, err=%s\n", hmstrerror(ret));
		ret = -EFAULT;
		goto out_destroy_device;
	}

	return E_HM_OK;
out_destroy_device:
	devhost_device_destroy(devinfo);
	return ret;
}
