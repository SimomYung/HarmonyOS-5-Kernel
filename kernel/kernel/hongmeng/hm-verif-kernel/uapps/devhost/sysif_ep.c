/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of all sysif extension points
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 23 09:11:36 2020
 */
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmactv/actv.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>

#include <devhost/plugin.h>

#include "devhost.h"
#include "utils/actv.h"

static int sysif_check_selfcall(void)
{
	enum sec_cred_selfcall_type type = sysif_actv_current_callinfo()->selfcall_type;
	return (type == __SEC_CRED_ACTV_SELFCALL) ? E_HM_OK : E_HM_ACCES;
}

static bool sysif_ep_assign_selfcall(struct sysif_extension_point *ep)
{
	struct libdh_extension *ext = ep->ext_inst->ext;
	return (ext != NULL) && ((ext->ext_flag & DH_EXT_FLAG_SELFCALL) != 0);
}

/* allow all if `security` is disabled */
int __attribute((weak))
devhost_sec_chk_dispatch(int callno, unsigned long long sender, unsigned long credential)
{
	UNUSED(callno, sender, credential);
	return E_HM_OK;
}

int sysif_precond_checker_entry(struct sysif_extension_point *ep,
				int callno,
				unsigned long long sender,
				unsigned long cred)
{
	int ret = E_HM_OK;

	/* skip security check when selfcall */
	if (sysif_ep_assign_selfcall(ep)) {
		ret = sysif_check_selfcall();
	} else {
		ret = devhost_sec_chk_dispatch(callno, sender, cred);
	}

	return ret;
}

#define DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(method, ...)	\
	DECLARE_SYSIF_EP_ALS_HDLR(drvhandler_devhost, method, devhost, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_HDLR, drvhandler_devhost, method, devhost, int, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_NOSEC_HDLR, drvhandler_devhost, method, devhost, int, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOMM_HDLR(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_NOMM_HDLR, drvhandler_devhost, method, devhost, int, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR_LONG(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_HDLR_LONG, drvhandler_devhost, method, devhost, long, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR_LONG(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_NOSEC_HDLR_LONG, drvhandler_devhost, method, devhost, long, ##__VA_ARGS__)

#define DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(method, ...)	\
	DECLARE_SYSIF_EP_CUSTOM_HDLR(DEFINE_SYSCALL_NO_ENTER_HDLR, drvhandler_devhost, method, devhost, int, ##__VA_ARGS__)

/* netlink sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_netlink_recv,
				      unsigned long long, sender, unsigned long, credential,
				      int, protocol, unsigned int, src_portid,
				      unsigned long, msg_buff, unsigned int, msg_len);

/* procfs sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(procfs_open,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, ubuf);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(procfs_close,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, ubuf);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(procfs_read,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
				      struct hm_actv_buf, ubuf, uintptr_t, ctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(procfs_write,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
				      struct hm_actv_buf, ubuf, uintptr_t, ctx);

/* sysfs sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sysfs_open,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned int, flags,
				      unsigned int, mode,  uintptr_t, ctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sysfs_close,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned long long, fd,
				      uintptr_t, ctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sysfs_read,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
				      struct hm_actv_buf, ubuf, uintptr_t, ctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sysfs_write,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
				      struct hm_actv_buf, ubuf, uintptr_t, ctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sysfs_poll,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tagid, uintptr_t, ctx, bool, pollable);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_sysfs_read,
				      unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_sysfs_write,
				      unsigned long long, sender, unsigned long, credential);

/* clock sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(clock_settime,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long, tp, unsigned long long, file_id)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(clock_gettime,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long, tp, unsigned long long, file_id)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(clock_getres,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long, tp, unsigned long long, file_id)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(clock_adjtime,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long, tx, unsigned long long, file_id)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(clock_poweroff_alarm,
				      unsigned long long, sender, unsigned long, credential,
				      long, tm)

/* rpc sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(rpc_export,
				  unsigned long long, sender, unsigned long, credential,
				  int, devid,
				  unsigned int, cnode_idx)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(rpc_invoke,
				      unsigned long long, sender, unsigned long, credential,
				      int, devid, int, cmd,
				      unsigned long, req_ubuf, unsigned long, req_size,
				      unsigned long, resp_ubuf, unsigned long, resp_size)

/* net sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(net_prepare,
				  unsigned long long, sender, unsigned long, credential)

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(net_init,
				  unsigned long long, sender, unsigned long, credential,
				  int, ifindex, rref_t, tgt_rref)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_send,
				      unsigned long long, sender, unsigned long, credential,
				      int, ifindex, unsigned int, offset,
				      unsigned int, size, unsigned int, payload_len,
				      unsigned long, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_ioctl,
				      unsigned long long, sender, unsigned long, credential,
				      int, ifindex, int, request, unsigned long, args)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_status,
				      unsigned long long, sender, unsigned long, credential,
				      int, ifindex)

/* net socket sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(net_socket_register,
				  unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_close,
				  unsigned long long, sender, unsigned long, credential,
				  int, sockfd)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_connect,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const struct sockaddr *, addr,
				  unsigned, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_bind,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const struct sockaddr *, addr,
				  unsigned, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_listen,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, backlog)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_accept,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, struct sockaddr *, addr, unsigned *, addrlen,
				  int, flags, unsigned long, ino)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_shutdown,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, how)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_ioctl,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, cmd, void *, arg_buf)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_dev_ioctl,
				  unsigned long long, sender, unsigned long, credential,
				  int, cmd, void *, arg_buf)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_fcntl,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, cmd, int, arg)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_send,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const void *, buf, size_t, len,
				  int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_recv,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, void *, buf, size_t, len,
				  int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_sendmmsg,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const void *, msgvec,
				  size_t, vlen, int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_sendmsg,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const struct msghdr *, msg,
				  int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_recvmsg,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, struct msghdr *, msg,
				  int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_recvmmsg,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const void *, msgvec,
				  size_t, vlen, int, flags, void *, timeout)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_sendto,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const void *, buf, size_t, len,
				  int, flags, const struct sockaddr *, daddr,
				  unsigned, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_recvfrom,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, void *, buf, size_t, len,
				  int, flags, struct sockaddr *, saddr,
				  unsigned *, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_readv,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const struct iovec *, iov,
				  int, iovcnt)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_writev,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, const struct iovec *, iov,
				  int, iovcnt, int, flags)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_getsockopt,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, level, int, optname,
				  void *, optval, unsigned *, optlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_setsockopt,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, int, level, int, optname,
				  const void *, optval, unsigned , optlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_getsockname,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, struct sockaddr *, addr,
				  unsigned *, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_getpeername,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, struct sockaddr *, addr,
				  unsigned *, addrlen)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_fchown,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, unsigned, uid, unsigned, gid)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(net_socket_fstat,
				  unsigned long long, sender, unsigned long, credential,
				  long, sockfd, uid_t *, uid)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_socket,
				  unsigned long long, sender, unsigned long, credential,
				  int, domain, int, type, int, protocol, unsigned long, ino)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_bpf,
				  unsigned long long, sender, unsigned long, credential,
				  int, cmd, void*, attr, unsigned, size)

#ifdef CONFIG_FILEGUARD
/* fileguard sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fileguard_message_notify,
				  unsigned long long, sender, unsigned long, credential,
			 	  void *, message, size_t, len);
#endif

/* kstate sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(kstate_binderinfo_notify,
				  unsigned long long, sender, unsigned long, credential,
				  int, calling_pid, int, called_pid);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(kstate_killinfo_notify,
				  unsigned long long, sender, unsigned long, credential,
				  int, pid, int, seg);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(kstate_freeze_binder_info_notify,
				  unsigned long long, sender, unsigned long, credential,
				  struct kstate_binder_info *, info);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(kstate_request_anco_thaw,
				  unsigned long long, sender, unsigned long, credential,
				  char *, reason, unsigned int, size);

/* iaware notify sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(iaware_notify_thread_msg,
				  unsigned long long, sender, unsigned long, credential,
				  struct evtcb_thread_msg_s, msg)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_alloc_new_ns,
				  unsigned long long, sender, unsigned long, credential,
				  int, nsid)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(net_socket_destroy_ns,
				  unsigned long long, sender, unsigned long, credential,
				  int, nsid)
/* udmi sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(core_load_driver,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, name,
				      struct hm_actv_buf, args,
				      unsigned long long, mpool_id,
				      unsigned long, size);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(core_unload_driver,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, drvname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(core_create_device,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, devname,
				      struct hm_actv_buf, dev_compat,
				      int, parent_devid,
				      unsigned int, resource_count);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(core_bind_device,
				      unsigned long long, sender, unsigned long, credential,
				      int, devid,
				      struct hm_actv_buf, drvname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOMM_HDLR(shrinker_query,
					   unsigned long long, sender, unsigned long, credential,
					   int, type);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOMM_HDLR(shrinker_shrink,
					   unsigned long long, sender, unsigned long, credential,
					   int, max_nr);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_open,
				      unsigned long long, sender, unsigned long, credential, unsigned int, u_cidx,
				      int, devid, rref_t, tgt_rref, unsigned int, vfs_flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_close,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath, unsigned long long, file_id);

/* fops sysif */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_read,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long, ubuf,
				      unsigned long long, file_id,
				      unsigned long long, offset,
				      unsigned long, count,
				      unsigned int, flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_write,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long, ubuf,
				      unsigned long long, file_id,
				      unsigned long long, offset,
				      unsigned long, count,
				      unsigned int, flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_read_iter,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long long, file_id, unsigned long long, offset,
				      const void *, vec, int, nr_segs, size_t, count,
				      unsigned int, flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_write_iter,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long long, file_id, unsigned long long, offset,
				      const void *, vec, int, nr_segs, size_t, count,
				      unsigned int, flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_llseek,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      int, whence, unsigned long long, file_id, long long, offset);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR(fops_ioctl,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      int, request,
				      unsigned long long, file_id,
				      unsigned long, arg);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_fcntl,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      int, request,
				      unsigned long long, file_id,
				      unsigned long, arg);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_flush,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long long, file_id);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_mmap,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long, addr,
				      unsigned long long, file_id,
				      unsigned long, length,
				      unsigned long, prot,
				      unsigned int, flags,
				      unsigned long, offset,
				      bool, iomap_prepare_ahead);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_munmap,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, key, unsigned long, start, unsigned long, len,
				      int, vs_is_releasing);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_do_page_fault,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, vma_key, unsigned long long, vr_key, 
				  unsigned long long, addr, unsigned int, flags,
				  unsigned long long, offset);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_mm_release,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned long long, vspace_key);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(fops_file_get,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath,
				      unsigned long long, file_id);

/* epoll sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_poll,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id,
				  unsigned long long, tcb_key, rref_t, fdtable_rref,
				  unsigned int, events, int, idx, int, fd, int, need_add);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_poll_remove,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id,
				  int, fd, cref_t, tcb_cref);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_epoll_ctl,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id,
				  const void *, ep_arg);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_epoll_remove,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id,
				  cref_t, ep_cref);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_epoll_recheck,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id,
				  unsigned int, events);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(fops_sync_ctrlmem_polllist,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, fastpath, unsigned long long, file_id);

/* crypto sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(core_entropy_read,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, num);

/* pm sysif */
DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(pm_action,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, action);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(pm_dubai_hwlog,
				  unsigned long long, sender, unsigned long, credential,
				  struct pm_dubai_ctrl_log *, log_info);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(pm_prepare,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, type, struct reboot_cmd, cmd);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(pm_power_off,
				      unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(pm_reboot,
				      unsigned long long, sender, unsigned long, credential,
				      struct reboot_cmd, cmd);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(pm_cpuhp_action,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, cpu, unsigned int, action);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(pm_wl_stat,
				  unsigned long long, sender, unsigned long, credential,
				  struct wl_stat_queue_info, queue_info, size_t, cnt);

DECLARE_DEVHOST_SYSIF_EP_ALS_HDLR(pm_thermal_action,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, action, struct thermal_action_arg, arg);

/* transfs sysif about mount */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_mounted,
				      unsigned long long, sender, unsigned long, credential,
				      struct hm_actv_buf, fs_name,
				      struct hm_actv_buf, dev_name,
				      struct hm_actv_buf, mount_opts);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_unmounted,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, int, flags, unsigned long long, file_id);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_drop_caches,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, mode, int, max_nr);

/* transfs sysif about dentry */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_mkdir,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_index, mode_t, mode,
				      struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_rmdir,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_index,
				      unsigned int, d_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_create,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_inedx, unsigned int, pd_index, uint32_t, mode,
				      struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_unlink,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_inedx,
				      unsigned int, d_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_mknod,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_index,
				      unsigned int, mode, unsigned long long, devno,
				      struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_link,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, old_d_index,
				      unsigned int, new_pd_index, struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_symlink,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_index,
				      struct hm_actv_buf, actv_dname, struct hm_actv_buf, actv_path);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_statfs,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, d_index, void *, buf);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_rename,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, old_pd_index,
				      unsigned int, old_d_index, unsigned int, new_pd_index,
				      unsigned int, new_d_index, struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_lookup,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, pd_index,
				      struct hm_actv_buf, actv_dname, void *, meta);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_setattr,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, d_index,
				      const void *, attr, uint64_t, size);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_readlink,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, d_index, char *, buf,
				      size_t, bufsize);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_getattr,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, d_index, void *, meta);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_listxattr,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, d_index,
				      char *, buffer, size_t, size);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_getxattr,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, d_index,
				      int, prefix, struct hm_actv_buf, name,
				      char *, value, size_t, size);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_setxattr,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, d_index,
				      int, prefix, struct hm_actv_buf, name,
				      const char *, value, size_t, size, int, flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_tmpfile,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, pd_index,
				      uint32_t, mode, struct hm_actv_buf, actv_dname);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_kill_dentry,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, d_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_node_put,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, v_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_update_opts,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, char *, ubuf, size_t, buflen);

/* transfs sysif about node */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_sync,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index);

/* transfs sysif about file */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_write,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      uint64_t, pos, const void *, src, size_t, len);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_read,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      uint64_t, pos, const void *, src, size_t, len);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_write_iter,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      uint64_t, pos, const void *, iov, int, nr_segs,
				      size_t, count);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_fsync,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_readdir,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, f_index, uint64_t, pos,
				      void *, buf, size_t, size);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_open,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, d_index, unsigned int, f_modes,
				      unsigned int, f_flags);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_fallocate,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, sb_index, unsigned int, f_index, unsigned int, mode,
				      uint64_t, pos, uint64_t, len);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_close,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_ioctl,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      unsigned int, cmd, unsigned long, arg);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_direct_IO,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      uint64_t, offset, void *, buf,
				      size_t, len, unsigned int, type);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(transfs_iommap,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, s_index, unsigned int, f_index,
				      unsigned long, addr, unsigned long long, vspace_rref,
				      unsigned long, length, unsigned long, prot, unsigned int, flags,
				      unsigned long, offset);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NO_ENTER_HDLR(process_revoke,
				  unsigned long long, sender, unsigned long, credential,
				  unsigned int, cnode_idx, pid_t, pid, uid_t, uid);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sec_mgmt,
		    unsigned long long, sender, unsigned long, credential,
		    unsigned int, module, unsigned int, command);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sec_test_hkip_seharmony_prot_policydb,
		    unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sec_test_hkip_roafter_prot,
		    unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sec_sync_fastpath_open,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long long, filp, unsigned int, task_sid,
				      unsigned int, state);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(sec_sync_fastpath_dev,
				      unsigned long long, sender, unsigned long, credential,
				      int, devid, unsigned int, task_sid, unsigned int, state);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR_LONG(devfs_read,
					   unsigned long long, sender, unsigned long, credential,
					   long long, file, void *, buf, size_t, count);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR_LONG(devfs_write,
					   unsigned long long, sender, unsigned long, credential,
					   long long, file, const void *, buf, size_t, count);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR(devfs_ioctl,
				      unsigned long long, sender, unsigned long, credential,
				      long long, file, unsigned int, cmd, unsigned long, arg);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_NOSEC_HDLR_LONG(devfs_readv,
					   unsigned long long, sender, unsigned long, credential,
					   long long, file, const void *, _vec, int, vlen);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR_LONG(devfs_writev,
					   unsigned long long, sender, unsigned long, credential,
					   long long, file, const void *, _vec, int, vlen);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR_LONG(devfs_lseek,
					   unsigned long long, sender, unsigned long, credential,
					   long long, file, long, offset, int, whence);

#if defined(__arm__) || !defined(__sysif_server_devhost_no_compat32_handlers__)
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(devfs__llseek,
				      unsigned long long, sender, unsigned long, credential,
				      long long, file, unsigned long, offset_high,
				      unsigned long, offset_low, long long *, res, int, whence);
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(devfs_fcntl64,
				      unsigned long long, sender, unsigned long, credential,
				      long long, file, int, cmd, unsigned long, arg, int, fd);
#endif

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(devfs_fcntl,
				      unsigned long long, sender, unsigned long, credential,
				      long long, file, int, cmd, unsigned long, arg, int, fd);

/* tracefs */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(trace_enable_disable_ldk_event,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tp_index,
				      bool, enable)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(trace_ldk_event_format,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tp_index,
				      char *, format)

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(trace_print_ldk_event,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, tp_index,
				      void *, entry,
				      size_t, entry_size,
				      char *, event_str)

/* file_info */
DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(file_info_query_dmabuf,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath, unsigned long long, file_idx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(file_info_query_fence,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath, unsigned long long, file_idx,
				      unsigned int, offset);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(file_info_query_ion,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned int, fastpath, unsigned long long, file_idx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(file_info_query_meminfo,
				      unsigned long long, sender, unsigned long, credential);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(file_info_lowmem_debug,
				      unsigned long long, sender, unsigned long, credential,
				      int, oom_score_adj, unsigned long long, mem_total,
					  bool, is_emergency);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(probe_handle_brk,
				  unsigned long long, sender, unsigned long, credential,
				  struct arch_uctx *, uctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(probe_handle_ss_brk,
				  unsigned long long, sender, unsigned long, credential,
				  struct arch_uctx *, uctx);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(tests_hkip_protection,
				      unsigned long long, sender, unsigned long, credential,
				      unsigned long, test_id);

DECLARE_DEVHOST_SYSIF_EP_SYSCALL_HDLR(posix_vhangup,
				      unsigned long long, sender, unsigned long, credential);
