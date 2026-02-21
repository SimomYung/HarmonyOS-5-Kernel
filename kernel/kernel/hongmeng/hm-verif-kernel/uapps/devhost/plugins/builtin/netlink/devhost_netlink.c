/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Core logic of devhost netlink
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 23 17:45:16 2020
 */
#include <dlfcn.h>
#include <hmnet/hm_netlink.h>
#include <hmnet/hm_genetlink.h>
#include <devhost/plugin.h>
#include <devhost/netlink.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <hmkernel/capability.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>

#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>

static dh_netlink_rcv_if_t dh_netlink_process = NULL;

PUBLIC_SYMBOL
int devhost_netlink_register_entr(dh_netlink_rcv_if_t func)
{
	if (dh_netlink_process == NULL) {
		dh_netlink_process = func;
		return 0;
	}
	return -EEXIST;
}

PUBLIC_SYMBOL
int devhost_netlink_multicast(const void *msg,
			      unsigned int msg_size, int protocol,
			      unsigned int dst_group)
{
	int ret = E_HM_OK;
	ret = hm_netlink_broadcast(protocol, 0, dst_group, msg, (size_t)msg_size);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

PUBLIC_SYMBOL
int devhost_netlink_unicast(const void *msg,
			    unsigned int msg_size, int protocol,
			    unsigned int dst_portid)
{
	int ret = E_HM_OK;
	ret = hm_netlink_unicast(protocol, 0, dst_portid, msg, msg_size);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

PUBLIC_SYMBOL
int devhost_netlink_register_genl(const char *fam_name, const void *fam_info, unsigned long info_len,
				  unsigned int *fam_id, unsigned int *mcgrp_offset)
{
	int ret = E_HM_OK;
	unsigned short netd_new_id = 0;
	unsigned int netd_new_grp_off = 0;
	cref_t dh_ap_cref;
	dh_ap_cref = libdh_get_dh_ap();
	if (IS_REF_ERR(dh_ap_cref)) {
		return -EINVAL;
	}
	ret = hm_netlink_register_generic(fam_info, (unsigned int)info_len, dh_ap_cref,
					  &netd_new_grp_off, &netd_new_id);
	if (ret < 0) {
		hm_error("register genl %s failed %d\n", fam_name, ret);
		return -hmerrno2posix(ret);
	}
	*fam_id = (unsigned int)netd_new_id;
	*mcgrp_offset = netd_new_grp_off;
	return 0;
}

PUBLIC_SYMBOL
int devhost_netlink_unregister_genl(unsigned int fam_id)
{
	int ret = E_HM_OK;

	ret = hm_netlink_unregister_generic((unsigned short)fam_id);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

typedef int (*nl_reg_unit_t)(unsigned long long ap_ref, unsigned int protocol, unsigned int nr_grp);
typedef int (*nl_unreg_unit_t)(unsigned int protocol);
static nl_reg_unit_t g_nl_reg_user;
static nl_unreg_unit_t g_nl_unreg_user;

static int __netlink_register_user(unsigned long long ap_ref,
				   unsigned int protocol, unsigned int nr_grp)
{
	if (g_nl_reg_user != NULL) {
		return g_nl_reg_user(ap_ref, protocol, nr_grp);
	}
	return E_HM_NOSYS;
}

static int __netlink_unregister_user(unsigned int protocol)
{
	if (g_nl_unreg_user != NULL) {
		return g_nl_unreg_user(protocol);
	}
	return E_HM_NOSYS;
}

PUBLIC_SYMBOL
int devhost_netlink_register(unsigned int protocol, unsigned int nr_grp)
{
	cref_t dh_ap_cref;
	int ret = E_HM_OK;

	dh_ap_cref = libdh_get_dh_ap();
	if (IS_REF_ERR(dh_ap_cref)) {
		return -EINVAL;
	}

	ret = __netlink_register_user(dh_ap_cref, protocol, nr_grp);
	if (ret < 0) {
		hm_error("register custom netlink %u failed, %d\n", protocol, ret);
		return -hmerrno2posix(ret);
	}

	return 0;
}

PUBLIC_SYMBOL
int devhost_netlink_unregister(unsigned int protocol)
{
	int ret = E_HM_OK;

	ret = __netlink_unregister_user(protocol);
	if (ret < 0) {
		hm_error("unregister custom netlink %u failed, %d\n", protocol, ret);
		return -hmerrno2posix(ret);
	}

	return 0;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_netlink_recv, netlink_recv_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, protocol, unsigned int, src_portid,
			 unsigned long, msg_buff, unsigned int, msg_len)
{
	UNUSED(sender, credential);
	const void *nl_msg = ulong_to_ptr(msg_buff, void);
	size_t nl_msg_sz = msg_len;
	int err = E_HM_OK;
	struct netlink_rcv_msg args = {
		.msg = nl_msg,
		.msg_size = nl_msg_sz,
		.protocol = protocol,
		.portid = src_portid,
	};

	if ((nl_msg == NULL) || (nl_msg_sz == 0)) {
		return E_HM_INVAL;
	}
	if (dh_netlink_process == NULL) {
		return E_HM_OPNOTSUPP;
	}
	err = dh_netlink_process(&args);
	if (err < 0) {
		return err;
	}

	return E_HM_OK;
}

static int dh_netlink_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	UNUSED(plugin);
	g_nl_unreg_user = (nl_unreg_unit_t)(uintptr_t)dlsym(RTLD_DEFAULT, "hm_netlink_unregister_user");
	g_nl_reg_user = (nl_reg_unit_t)(uintptr_t)dlsym(RTLD_DEFAULT, "hm_netlink_register_user");
	return E_HM_OK;
}

/*
 * define devhost builtin netlink plugin. This is netlink intermediate layer
 * between frontend(uapp) and backend(liblinux). It deliver netlink messages
 * which come from hmnetd to the backend.
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_net_netlink_recv`
 *
 * export symbol:
 *  - `devhost_netlink_register_entr`
 *  - `devhost_netlink_multicast`
 *  - `devhost_netlink_unicast`
 *  - `devhost_netlink_register_genl`
 *  - `devhost_netlink_unregister_genl`
 *  - `devhost_netlink_register`
 *  - `devhost_netlink_unregister`
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(netlink, dh_netlink_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_netlink_recv, netlink_recv_ext)
);
