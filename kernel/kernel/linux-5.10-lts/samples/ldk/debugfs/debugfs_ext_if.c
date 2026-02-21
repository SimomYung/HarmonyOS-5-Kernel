/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Implementation of debugfs_ext interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 23 10:00:00 2024
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
#include "debugfs_ext_if.h"

#ifdef CONFIG_LIBLINUX_DEBUGFS_BINDER
#define DEBUGFS_BINDERFS_CREATE_PROC_NODE 0x1
#define DEBUGFS_BINDERFS_DELETE_PROC_NODE 0x2

struct binder_create_proc_node_args {
	int pid;
	void *dentry;
};

static int binderfs_create_proc_node(unsigned long __user arg)
{
	int ret = E_HM_OK;
	struct binder_create_proc_node_args args = {0};
	int pid;
	void *dentry_ptr;

	ret = hm_actv_read_caller_vm(&args, (void __user *)(uintptr_t)arg, sizeof(struct binder_create_proc_node_args));
	if (ret != E_HM_OK) {
		hm_warn("failed to read arg, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = debugfs_create_binder_proc_node(args.pid, &dentry_ptr);
	if (ret != E_HM_OK) {
		hm_warn("failed to create binder proc node, err=%s\n", hmstrerror(ret));
		return posix2hmerrno(-ret);
	}

	ret = hm_actv_write_caller_vm(args.dentry, &dentry_ptr, sizeof(void *));
	if (ret != E_HM_OK) {
		hm_warn("failed to return dentry, err=%s\n", hmstrerror(ret));
	}

	return ret;
}

static void binderfs_delete_proc_node(unsigned long __user arg)
{
	if (arg != 0U)
		debugfs_delete_binder_proc_node((void *)arg);
}
#endif

static const char *debugfs_ext_name = "debugfs_ext";
static struct debugfs_ext_ops *debugfs_ext_ops = NULL;

static int debugfs_ext_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	int ret = E_HM_OK;

	UNUSED(ctx);
	switch (cmd) {
#ifdef CONFIG_LIBLINUX_DEBUGFS_BINDER
	case DEBUGFS_BINDERFS_CREATE_PROC_NODE:
		ret = binderfs_create_proc_node(arg);
		break;
	case DEBUGFS_BINDERFS_DELETE_PROC_NODE:
		binderfs_delete_proc_node(arg);
		break;
#endif
	default:
		ret = E_HM_INVAL;
	};

	return ret;
}

static struct devhost_fops debugfs_ext_fops = {
	.ioctl = debugfs_ext_ioctl,
};

static void debugfs_ext_devinfo_release(struct device_info *devinfo)
{
	free(devinfo);
}

static int debugfs_ext_device_info_init(struct device_info **devinfo)
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
	info->devname = debugfs_ext_name;
	info->release = debugfs_ext_devinfo_release;
	devhost_device_capable_devfs(info, 0, 0, NULL, &debugfs_ext_fops);

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

static struct filp_node *debugfs_ext_filp_node_alloc(struct device_info *info,
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

static int debugfs_ext_register(struct device_info *devinfo)
{
	int ret;
	uref_t dh_ap_uref;
	unsigned int file_id;
	struct filp_node *node = NULL;
	struct server_info *server = NULL;

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

	node = debugfs_ext_filp_node_alloc(devinfo, server->cnode_idx, &file_id);
	if (node == NULL) {
		hm_warn("failed to alloc filp node\n");
		ret = -EFAULT;
		goto out_revoke_ucap;
	}

	ret = actv_fscall(vfs_register_debugfs_ext, server, dh_ap_uref, file_id);
	if (ret != E_HM_OK) {
		hm_warn("failed to register debugfs_ext, err=%s\n", hmstrerror(ret));
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

int devhost_register_debugfs_ext(void)
{
	int ret;
	struct device_info *devinfo = NULL;

	ret = debugfs_ext_device_info_init(&devinfo);
	if (ret != E_HM_OK) {
		hm_warn("debugfs: failed to init device info, err=%s\n", hmstrerror(ret));
		return -EFAULT;
	}

	ret = debugfs_ext_register(devinfo);
	if (ret != E_HM_OK) {
		hm_warn("debugfs: failed to register dev, err=%s\n", hmstrerror(ret));
		ret = -EFAULT;
		goto out_destroy_device;
	}

	return E_HM_OK;
out_destroy_device:
	devhost_device_destroy(devinfo);
	return ret;
}
