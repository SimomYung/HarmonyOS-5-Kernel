/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Handler for device epoll sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 1 16:00:01 2021
 */
#include <libsysif/devhost/server.h>

#include <devhost/plugin.h>
#include <libstrict/strict.h>

#include "types.h"
#include <devhost/file.h>
#include "devhost_poll.h"

/* poll for fdtable sink */
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_poll, dh_poll_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 unsigned long long, tcb_key, rref_t, fdtable_rref,
			 unsigned int, events, int, idx, int, fd, int, need_add)
{
	int ret;
	unsigned int revents = 0;
	struct devhost_poll_target_info tinfo = {0};
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();
	unsigned int fwd_cnode_idx = sysif_actv_fwd_cnode_idx();

	struct __actvret_drvcall_devhost_fops_poll *pret =
			actvhdlr_drvhandler_devhost_fops_poll_prepare_ret(
			sender, credential);
	BUG_ON_D(pret == NULL);

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((node->filp == NULL) || (devinfo == NULL) ||
	    (devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->poll == NULL)) {
	    return E_HM_NOSYS;
	}

	tinfo.poll_args.tcb_key = tcb_key;
	tinfo.poll_args.fdtable_rref = fdtable_rref;
	tinfo.poll_args.events = events;
	tinfo.poll_args.idx = idx;
	tinfo.poll_args.fd = fd;
	tinfo.cnode_idx = fwd_cnode_idx;
	tinfo.fnode = node;
	ret = devhost_poll_add(node, &tinfo, need_add, &revents);
	if (ret < 0) {
		hm_debug("devhost poll device(%s) failed, err=%s\n",
			  devinfo->devname, hmstrerror(ret));
		return ret;
	}

	pret->revents = revents;
	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_poll_remove, dh_poll_remove_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 int, fd, cref_t, tcb_cref)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();

	UNUSED(sender, credential, fd);

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((node->filp == NULL) || (devinfo == NULL) ||
	    (devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->poll == NULL)) {
		return E_HM_NOSYS;
	}

	devhost_poll_remove(node, tcb_cref);
	return E_HM_OK;
}

/* epoll for fdtable sink */
DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_epoll_ctl, dh_epoll_ctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 const void *, ep_arg)
{
	int ret;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();
	struct epoll_info_arg __ep_arg = {0};

	UNUSED(sender, credential);

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((node->filp == NULL) || (devinfo == NULL) ||
	    (devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->poll == NULL)) {
		return E_HM_NOSYS;
	}

	ret = hm_actv_read_caller_vm(&__ep_arg, ep_arg, sizeof(struct epoll_info_arg));
	if (ret != E_HM_OK) {
		hm_warn("failed to get ep arg, ret=%s\n", hmstrerror(ret));
		return E_HM_INVAL;
	}

	ret = devhost_epoll_ctl(node, &__ep_arg);
	if (ret < 0) {
		return ret;
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_epoll_remove, dh_epoll_remove_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 cref_t, ep_cref)
{
	UNUSED(sender, credential);
	struct filp_node *node = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node != NULL) {
		devhost_epoll_remove(node, ep_cref);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_epoll_recheck, dh_epoll_recheck_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id,
			 unsigned int, events)
{
	int ret;
	unsigned int revents;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();
	struct __actvret_drvcall_devhost_fops_epoll_recheck *pret =
			actvhdlr_drvhandler_devhost_fops_epoll_recheck_prepare_ret(
								sender, credential);
	BUG_ON_D(pret == NULL);

	UNUSED(events);

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((node->filp == NULL) || (devinfo == NULL) ||
	    (devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->poll == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devhost_epoll_recheck(node, &revents);
	if (ret < 0) {
		hm_debug("devhost epoll recheck revents, device(%s) ret=%s\n",
			  devinfo->devname, hmstrerror(ret));
		return ret;
	}

	pret->revents = revents;
	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_sync_ctrlmem_polllist, dh_sync_ctrlmem_polllist_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_id)
{
	int ret;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();

	UNUSED(sender, credential);

	acquire_devhost_filp_node(node, cnode_idx, fastpath, file_id);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	devinfo = node->devinfo;
	if ((node->filp == NULL) || (devinfo == NULL) ||
	    (devinfo->p->ex_fops == NULL) || (devinfo->p->ex_fops->poll == NULL)) {
		return E_HM_NOSYS;
	}

	ret = devhost_epoll_recheck(node, NULL);
	if (ret < 0) {
		hm_debug("devhost epoll sync events, device(%s) ret=%s\n",
			  devinfo->devname, hmstrerror(ret));
		return ret;
	}

	return E_HM_OK;
}

/*
 * epoll sysif module, support device epoll related operations
 *
 * extension point: NULL
 *
 * extension:
 *
 * export symbol:
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(epoll, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_poll, dh_poll_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_poll_remove, dh_poll_remove_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_epoll_ctl, dh_epoll_ctl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_epoll_remove, dh_epoll_remove_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_epoll_recheck, dh_epoll_recheck_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_sync_ctrlmem_polllist, dh_sync_ctrlmem_polllist_ext)
)
