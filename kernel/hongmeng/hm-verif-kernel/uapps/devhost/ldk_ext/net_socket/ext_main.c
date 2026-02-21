/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Net socket extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 11:20:49 2022
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <devhost/plugin.h>
#include <devhost/poll.h>
#include <devhost/memory.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <libdevhost/devhost.h>
#include <hmnet/hmnet_ldk.h>
#include <hmnet/hm_socket.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

#ifdef CONFIG_NET_NS
#include <vfs.h>
#include <sched.h>
#include <stdarg.h>
#include <malloc.h>
#include <lib/utils.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/fs/api.h>
#include <libsysif/utils_common.h>
#include <libsysif/sysmgr/api.h>
#include <libsysif/utils.h>
#include <libhmsrv_sys/hm_path.h>
#include <libsec/cred.h>
#endif

#include "net_socket_api.h"
#include "net_socket_if.h"
#include "net_socket.h"

static struct socket_device_info socket_info;
#ifdef CONFIG_NET_NS
static struct devhost_net_namespace_ops *net_namespace_ops = NULL;
#endif

static void devhost_socket_release_filp_node(struct raw_compounded_auto_refcnt *ref)
{
	int ret = E_HM_OK;
	struct filp_node *node = NULL;
	struct socket_device_info *sock_dev = NULL;

	node = refcnt_to_filp_node(ref);
	BUG_ON(node == NULL || node->devinfo == NULL);

	/* cleanup poll resources needs to be executed before fops->close */
	devhost_poll_close(node);
	devhost_epoll_close2(node);

	sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);
	if (sock_dev->sock_ops != NULL && node->filp != NULL) {
		ret = sock_dev->sock_ops->close(node->filp);
		if (ret < 0) {
			hm_error("failed to close socket, err = %d\n", ret);
		}
	}

	if (node->hmkobj != NULL) {
		ret = hm_ctrlmem_kobjpool_destroy(node->hmkobj);
		if (ret < 0) {
			hm_warn("destroy hmkobj err=%s\n", hmstrerror(ret));
		}
		node->hmkobj = NULL;
	}

	devinfo_put(node->devinfo);

	free(node);
}

struct filp_node *alloc_socket_filp_node(__u32 cnode_idx, unsigned long long *kshm_key_out,
					 unsigned long *kobj_offset_out)
{
	struct filp_node *node = NULL;
	struct device_info *devinfo = &(socket_info.socket_devinfo);

	node = (struct filp_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}

	mem_zero_s(*node);

	node->hmkobj = hm_ctrlmem_kobjpool_spawn_polllist(ptr_to_ulong(node), 0, NULL);
	if (node->hmkobj == NULL) {
		hm_error("socket spawn poll kobj failed\n");
		free(node);
		return NULL;
	}

	*kshm_key_out = ptr_to_ulong(node->hmkobj);
	*kobj_offset_out = 0;

	raw_mutex_init(&node->ctx_lock);
	node->private_ctx = NULL;
	/* this refcnt will decrease in devhost_socket_release_filp_node */
	node->devinfo = devinfo_get(devinfo);
	node->cnode_idx = cnode_idx;
	raw_compounded_auto_refcnt_init(&node->refcnt, 1U, devhost_socket_release_filp_node);
	filp_poll_list_init(node);

	return node;
}

int (*ldk_bpf)(int cmd, void *attr, unsigned int size);

int call_bpf(int cmd, void *attr, unsigned int size)
{
	return ldk_bpf(cmd, attr, size);
}

static int (*ldk_dev_ioctl)(int cmd, void *arg_buf);

int call_dev_ioctl(int cmd, void *arg_buf)
{
	int ret = -ENOSYS;

	if (ldk_dev_ioctl != NULL) {
		ret = ldk_dev_ioctl(cmd, arg_buf);
	}

	return ret;
}

#ifdef CONFIG_NET_FUSION

static int devhost_fusion_register_shrinker(unsigned long (*shrinker_query)(void *priv),
					    unsigned long (*shrinker_shrink)(void *priv, int req_nr),
					    void *priv)
{
	struct liblinux_page_allocator *common_allocator = liblinux_pal_page_allocator_get("common");
	struct liblinux_pal_sub_shrinker shrinker;

	shrinker.query = shrinker_query;
	shrinker.shrink = shrinker_shrink;
	shrinker.priv = priv;

	liblinux_pal_sub_shrinker_register(common_allocator, shrinker);

	return 0;
}

static int __ldk_ctx_setup(void)
{
	return backend_ctx_setup();
}

static fusion_ops_t fs_ops = {
	.fusion_copy_from_user = devhost_pal_copy_from_user,
	.fusion_copy_to_user = devhost_pal_copy_to_user,

	.fusion_register_shrinker = devhost_fusion_register_shrinker,
	.backend_ctx_setup = __ldk_ctx_setup,
};
#endif

PUBLIC_SYMBOL
int devhost_net_socket_devinfo_init(struct devhost_fops *fops,
				    struct devhost_fops_ex *ex_fops,
				    struct device_info **devinfo,
				    struct devhost_net_socket_ops *ops,
				    struct devhost_net_handle_ops *handle,
				    struct devhost_net_ldk_ops *ldkops)
{
	int ret;

	BUG_ON(fops == NULL);
	BUG_ON(devinfo == NULL);

	ret = devhost_device_init(&(socket_info.socket_devinfo));
	if (ret < 0) {
		/* devhost_device_init() returns posix errno */
		hm_warn("failed to init socket_devinfo, ret=%d, %s\n", ret, hmstrerror(ret));
		return ret;
	}

	socket_info.socket_devinfo.devname = "devhost_net_socket";
	devhost_device_capable_devfs(&(socket_info.socket_devinfo), 0, 0, NULL, fops);
	devhost_device_capable_devfs_ex(&(socket_info.socket_devinfo), ex_fops);
	*devinfo = &(socket_info.socket_devinfo);

	if (socket_info.sock_ops != NULL) {
		hm_warn("Multiple registration of LDK socket ops\n");
		return -EEXIST;
	}

	socket_info.sock_ops = ops;
	socket_info.handle_ops = handle;
	ldk_bpf = ldkops->bpf;
	ldk_dev_ioctl = ldkops->dev_ioctl;
#ifdef CONFIG_NET_FUSION
	fs_ops.fusion_valloc = ldkops->valloc;
	fs_ops.fusion_vfree = ldkops->vfree;
#endif

	return 0;
}
EXPORT_SYMBOL(devhost_net_socket_devinfo_init);

static void *devhost_socket_fget(int fd, void (*fget)(void*))
{
	struct filp_node *node = NULL;
	struct socket_device_info *sock_dev = NULL;
	unsigned long long file_idx;
	int ret;
	uint32_t net_cnode_idx = sysif_actv_src_cnode_idx();
	uint32_t app_cnode_idx = sysif_actv_fwd_cnode_idx();

	UNUSED(fget);
	/* param fd is hm fd, use it to query file_id from netd */
	ret = hm_net_query_file_id(fd, app_cnode_idx, &file_idx);
	if (ret != E_HM_OK) {
		return NULL;
	}

	acquire_devhost_filp_node(node, net_cnode_idx, 0, (unsigned int)file_idx);
	if (node == NULL) {
		return NULL;
	}

	sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);
	if (sock_dev->sock_ops == NULL) {
		return NULL;
	}
	return sock_dev->sock_ops->fget(node->filp);
}

#ifdef CONFIG_NET_NS
PUBLIC_SYMBOL
void devhost_net_namespace_register(struct devhost_net_namespace_ops *ops)
{
	net_namespace_ops = ops;
	return;
}
EXPORT_SYMBOL(devhost_net_namespace_register);

PUBLIC_SYMBOL
struct devhost_net_namespace_ops *devhost_get_net_namespace_ops(void)
{
	return net_namespace_ops;
}
EXPORT_SYMBOL(devhost_get_net_namespace_ops);

PUBLIC_SYMBOL
int devhost_get_nsid_by_fd(int fd, unsigned int *nsid)
{
	int err = E_HM_OK;
	static rref_t procfs_rref = 0;
	struct __actvret_fscall_vfs_query_info ret = {0};
	uint32_t cnode_idx = 0UL;

	err = hm_path_acquire("fs-procfs-actv", &procfs_rref);
	if (err != E_HM_OK) {
		return err;
	}

	err = actvxactcapcall_fscall_vfs_query_info(true, true, procfs_rref, fd, cnode_idx, 0, &ret);
	if (err == E_HM_OK) {
		*nsid = (unsigned int)(unsigned long)(ret.filep & 0xffffffffU);
	}

	if (hm_path_release("fs-procfs-actv", procfs_rref) != E_HM_OK) {
		hm_warn("release unused rref failed\n");
	}

	return err;
}
EXPORT_SYMBOL(devhost_get_nsid_by_fd);

PUBLIC_SYMBOL
int devhost_get_net_nsid(unsigned int *nsid)
{
	int err;
	struct sec_chk_cred cred = SEC_CHK_CRED_RESET;

	err = sec_chk_cred_init_requester_simple(&cred);
	if (err == E_HM_OK) {
		*nsid = netns_id_of_sec_cred(&cred);
	}

	return err;
}
EXPORT_SYMBOL(devhost_get_net_nsid);

PUBLIC_SYMBOL
int devhost_get_nsid_by_pid(int pid, int flags, unsigned int *nsid)
{
	int err = E_HM_OK;
	*nsid = ROOT_NET_NSID;
	struct __actvret_hmcall_ns_get_nsid_by_pid ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_ns_get_nsid_by_pid(pid, flags, &ret);
	if (err >= 0) {
		*nsid = ret.nsid;
	}

	return err;
}
EXPORT_SYMBOL(devhost_get_nsid_by_pid);
#else
PUBLIC_SYMBOL
void devhost_net_namespace_register(struct devhost_net_namespace_ops *ops)
{
	UNUSED(ops);
	return;
}
EXPORT_SYMBOL(devhost_net_namespace_register);

PUBLIC_SYMBOL
int devhost_get_nsid_by_fd(int fd, unsigned int *nsid)
{
	UNUSED(fd);
	*nsid = ROOT_NET_NSID;
	return 0;
}
EXPORT_SYMBOL(devhost_get_nsid_by_fd);

PUBLIC_SYMBOL
int devhost_get_net_nsid(unsigned int *nsid)
{
	*nsid = ROOT_NET_NSID;
	return 0;
}
EXPORT_SYMBOL(devhost_get_net_nsid);

PUBLIC_SYMBOL
int devhost_get_nsid_by_pid(int pid, int flags, unsigned int *nsid)
{
	UNUSED(pid, flags);
	*nsid = ROOT_NET_NSID;
	return 0;
}
EXPORT_SYMBOL(devhost_get_nsid_by_pid);
#endif

int dh_extension_init(struct lnxbase_ctx *ctx);

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo net_socket_si = __LIBLINUX_SYMINFO_INITIALIZER;

	ret = lnxbase_load_symbols(ctx, "net_socket_ext", &net_socket_si);
	if (ret < 0) {
		hm_error("load net_socket extension symbols, %d\n", ret);
		return ret;
	}

#ifdef CONFIG_NET_FUSION
	ret = hmnetd_fusion_entry(0, NULL, &fs_ops);
	if (ret < 0) {
		hm_error("start hmnetd in fusion mode failed, %d\n", ret);
		return -hmerrno2posix(ret);
	}
#endif
	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("init net socket plugin failed, %d\n", ret);
		return -hmerrno2posix(ret);
	}

	ret = lnxbase_register_fget(NET_FD, devhost_socket_fget);
	if (ret < 0) {
		hm_error("register devhost net fget failed, err=%d\n", ret);
		return  -hmerrno2posix(ret);
	}

	return 0;
}
