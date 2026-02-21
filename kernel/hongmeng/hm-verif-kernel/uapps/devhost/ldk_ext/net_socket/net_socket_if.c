/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Handler for LDK stack socket operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:47:36 2022
 */
#include <libsysif/devhost/server.h>

#include <devhost/backend.h>
#include <hmnet/hmnet_ldk.h>
#include <libcrt/hmcrt.h>
#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <hmnet/hmnet_ldk.h>
#include <libdevhost/devhost.h>

#include "net_socket.h"
#include "net_socket_if.h"
#include "net_socket_api.h"
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <devhost/log.h>
#include <devhost/security.h>
#include <devhost/workqueue.h>

static int drvhdlr_devhost_net_socket_socket(unsigned int cnode_idx, int domain, int type,
	int protocol, unsigned long ino, void (*pre_hook)(struct devhost_net_handle_ops *),
	net_socket_ret_t *pret);

/*
 * Add the necessary operations as required
 * for performance consideration.
 */
static inline
void net_enter_lite(struct devhost_net_handle_ops *handle)
{
	if (handle != NULL) {
		handle->netcall_enter(hm_actv_local_thread_pid_of(), hm_actv_local_thread_tid_of(),
					hm_actv_local_thread_vpid_of());
	}
}

static inline
void net_enter_nop(struct devhost_net_handle_ops *handle)
{
	UNUSED(handle);
}

/* diff filp acquire way in fusion/no-fusion mode */
#ifdef CONFIG_NET_FUSION
#define acquire_net_socket_filp_node(cnode_idx, sockfd)	({ \
	do {				\
		UNUSED(cnode_idx);		\
		node = ulong_to_ptr((unsigned long)sockfd, struct filp_node);	\
	} while(0);					\
})
#else
#define acquire_net_socket_filp_node(cnode_idx, sockfd)		\
	acquire_devhost_filp_node(node, (cnode_idx), 0, (unsigned int)(sockfd))
#endif

#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(x, func, handle, ...)				\
static inline int drvhdlr_devhost_net_socket_##func(unsigned long long sender, \
						unsigned long credential,	\
						long sockfd,		\
						__scopedptr_map(x, __scopedptr_def, __VA_ARGS__))	\
{ \
	UNUSED(sender, credential);	\
	int ret = 0;						\
	__u32 cnode_idx = sysif_actv_src_cnode_idx();   \
	struct socket_device_info *sock_dev = NULL;	\
	struct filp_node *node = NULL;			\
					\
	acquire_net_socket_filp_node(cnode_idx, sockfd);	\
	do {							\
		if (node == NULL) {	\
			hm_error("devhost filp node is null\n");	\
			ret = E_HM_INVAL;	\
			break;			\
		}  \
		sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);	\
		if (sock_dev == NULL || sock_dev->handle_ops == NULL || sock_dev->sock_ops == NULL) { \
			hm_error("sock_dev or sock_ops is invalid\n");	\
			ret = E_HM_INVAL;	\
			break;		\
		}  \
		handle(sock_dev->handle_ops); \
		ret = sock_dev->sock_ops->func(node->filp, __scopedptr_map(x, __scopedptr_call, __VA_ARGS__));	\
		if (ret == -ERESTARTSYS) {	\
			ret = E_HM_RESTARTSYS;	\
			break;	\
		} else if (ret < 0) {	\
			ret = posix2hmerrno(-ret);	\
			break;						\
		}								\
	} while (0);						\
							\
	return ret;			\
}

#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL1(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(1, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL2(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(2, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL3(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(3, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL4(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(4, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL5(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(5, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL6(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(6, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL7(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(7, func, handle, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALL8(func, handle, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(8, func, handle, __VA_ARGS__)

/* no extra enter ops, use default enter */
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_NOP_CALLx(x, func, ...) \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(x, func, net_enter_nop, __VA_ARGS__)
/* specific lite enter ops */
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALLx(x, func, ...) \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_CALLx(x, func, net_enter_lite, __VA_ARGS__)

#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_NOP_CALL(func, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_NOP_CALLx(__HDLR_ARG_NR(__VA_ARGS__), func, __VA_ARGS__)
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(func, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALLx(__HDLR_ARG_NR(__VA_ARGS__), func, __VA_ARGS__)

DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(connect, const struct sockaddr *, addr, socklen_t, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(bind, const struct sockaddr *, addr, socklen_t, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(listen, int, backlog)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(ioctl, int, cmd, void *, arg_buf)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(fcntl, int, cmd, int, arg)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(send, const void *, buf, size_t, len, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(recv, void *, buf, size_t, len, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(sendmsg, const struct msghdr *, msg, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(recvmsg, struct msghdr *, msg, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(sendmmsg, const void *, msgvec, size_t, vlen, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(recvmmsg, const void *, msgvec, size_t, vlen, int, flags, void *, timeout)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(sendto, const void *, buf, size_t, len, int, flags,
					const struct sockaddr *, daddr, socklen_t, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(recvfrom, void *, buf, size_t, len, int, flags,
					struct sockaddr *, saddr, socklen_t *, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(readv, const struct iovec *, iov, int, iovcnt)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(writev, const struct iovec *, iov, int, iovcnt, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(getsockopt, int, level, int, optname, void *, optval,
					socklen_t *, optlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(setsockopt, int, level, int, optname, const void *, optval,
					socklen_t, optlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(getsockname, struct sockaddr *, addr, socklen_t *, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(getpeername, struct sockaddr *, addr, socklen_t *, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(fchown, unsigned int, uid, unsigned int, gid)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(fstat, uid_t *, uid)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_LITE_CALL(accept, struct sockaddr *, addr, socklen_t *, addrlen, int, flags,
						unsigned long, ino, void **, devhost_socket, void *, filp_node)

/* NOTE: transfer selfcall into function call in fusion mode */
#ifdef CONFIG_NET_FUSION
#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALLx(x, func, ...)				\
int devhost_net_socket_directcall_##func(long sockfd,		\
						__scopedptr_map(x, __scopedptr_def, __VA_ARGS__))	\
{	\
	int __ret;	\
	\
	__ret = drvhdlr_devhost_net_socket_##func(0, 0, sockfd, __scopedptr_map(x, __scopedptr_call, __VA_ARGS__));	\
	backend_syscall_exit();	\
	\
	return __ret;	\
}

#define DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(func, ...)  \
	DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALLx(__HDLR_ARG_NR(__VA_ARGS__), func, __VA_ARGS__)

DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(send, const void *, buf, size_t, len, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(recv, void *, buf, size_t, len, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(sendto, const void *, buf, size_t, len, int, flags,
					const struct sockaddr *, daddr, socklen_t, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(recvfrom, void *, buf, size_t, len, int, flags,
					      struct sockaddr *, saddr, socklen_t *, addrlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(sendmsg, const struct msghdr *, msg, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(recvmsg, struct msghdr *, msg, int, flags)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(getsockopt, int, level, int, optname, void *, optval, socklen_t *, optlen)
DEFINE_DRVHDLR_DEVHOST_NET_SOCKET_DIRECTCALL(setsockopt, int, level, int, optname, const void *, optval,
					     socklen_t, optlen)

int devhost_net_socket_directcall_close(unsigned int cnode_idx, int sockfd)
{
	int ret;
	struct socket_device_info *sock_dev = NULL;
	struct filp_node *node = NULL;

	acquire_devhost_filp_node(node, cnode_idx, 0, (unsigned int)sockfd);
	if (node == NULL) {
		hm_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);
	if (sock_dev == NULL || sock_dev->handle_ops == NULL) {
		hm_error("sock_dev or sock_ops is invalid\n");
		return E_HM_INVAL;
	}

	net_enter_lite(sock_dev->handle_ops);

	ret = devhost_unregister_file((uint64_t)cnode_idx, (unsigned int)sockfd);

	backend_syscall_exit();

	return ret;
}

int devhost_net_socket_directcall_socket(unsigned int cnode_idx, int domain, int type, int protocol,
					 unsigned long ino, net_socket_ret_t *socket_directcall_ret)
{
	int ret;

	ret = drvhdlr_devhost_net_socket_socket(cnode_idx, domain, type, protocol, ino, net_enter_lite, socket_directcall_ret);
	backend_syscall_exit();

	return ret;
}

#endif /* CONFIG_NET_FUSION */

static int ldk_reg_table[] = {
#ifdef CONFIG_DEVHOST_NET_QIPCRTR
	AF_QIPCRTR,
#endif
#ifdef CONFIG_DEVHOST_NET_INET
	AF_INET,
#endif
#ifdef CONFIG_DEVHOST_NET_NETLINK
	AF_NETLINK,
#endif
#ifdef CONFIG_DEVHOST_NET_PACKET
	AF_PACKET,
#endif
#ifdef CONFIG_DEVHOST_NET_INET6
	AF_INET6,
#endif
#ifdef CONFIG_DEVHOST_NET_PPPOX
	AF_PPPOX,
#endif
#ifdef CONFIG_DEVHOST_NET_XDP
	AF_XDP,
#endif
#ifdef CONFIG_DEVHOST_NET_KEY
	AF_KEY,
#endif
#ifdef CONFIG_DEVHOST_NET_ALG
	AF_ALG,
#endif
};

static int devhost_net_socket_register(void)
{
	cref_t dh_ap_cref;
	int ret = E_HM_OK;

	dh_ap_cref = libdh_get_dh_ap();

	for (unsigned int i = 0; i < ARRAY_SIZE(ldk_reg_table); i++) {
		ret = hm_net_ldk_domain_register(ldk_reg_table[i], dh_ap_cref);
		if (ret != E_HM_OK) {
			hm_warn("devhost failed to register LDK domain = %d, ret = %s\n",
				ldk_reg_table[i], hmstrerror(ret));
		} else {
			hm_info("devhost register LDK domain = %d success\n", ldk_reg_table[i]);
		}
	}
	return E_HM_OK;
}

struct raw_workqueue *devhost_net_get_actvpool_wq(void)
{
	return devhost_get_actvpool_wq();
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_register, dh_net_socket_register_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	UNUSED(sender, credential);

	return devhost_net_socket_register();
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_close, dh_net_socket_close_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, sockfd)
{
	UNUSED(sender, credential);
	__u32 cnode_idx = sysif_actv_src_cnode_idx();
	struct filp_node *node = NULL;
	int ret;

	acquire_devhost_filp_node(node, cnode_idx, 0, (unsigned int)sockfd);
	if (node == NULL) {
		hm_error("file_id is invalid\n");
		return E_HM_INVAL;
	}

	ret = devhost_unregister_file((uint64_t)cnode_idx, (unsigned int)sockfd);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_connect, dh_net_socket_connect_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const struct sockaddr *, addr,
			 socklen_t, addrlen)
{
	return drvhdlr_devhost_net_socket_connect(sender, credential, sockfd, addr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_bind, dh_net_socket_bind_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const struct sockaddr *, addr,
			 socklen_t, addrlen)
{
	return drvhdlr_devhost_net_socket_bind(sender, credential, sockfd, addr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_listen, dh_net_socket_listen_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, backlog)
{
	return drvhdlr_devhost_net_socket_listen(sender, credential, sockfd, backlog);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_accept, dh_net_socket_accept_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, struct sockaddr *, addr, socklen_t *, addrlen,
			 int, flags, unsigned long, ino)
{
	struct __actvret_drvcall_devhost_net_socket_accept *pret =
		actvhdlr_drvhandler_devhost_net_socket_accept_prepare_ret(sender, credential);
	int ret;
	struct filp_node *node = NULL;
	struct filp_node *new_node = NULL;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
	void *devhost_socket = NULL;
	unsigned int file_id = -1;
	__u32 cnode_idx = sysif_actv_src_cnode_idx();

	BUG_ON(pret == NULL);
	acquire_net_socket_filp_node(cnode_idx, sockfd);
	if (node == NULL) {
		hm_error("devhost filp node is invalid\n");
		return E_HM_INVAL;
	}

	new_node = alloc_socket_filp_node(cnode_idx, &kshm_key, &kobj_offset);
	if (new_node == NULL) {
		return E_HM_NOMEM;
	}

	ret = drvhdlr_devhost_net_socket_accept(sender, credential, sockfd, addr,
						addrlen, flags, ino, &devhost_socket, new_node);
	if (ret < 0) {
        devhost_put_file(new_node);
		return ret;
	}

	new_node->filp = devhost_socket;

	ret = devhost_register_file(new_node, &file_id);
	if (ret < 0) {
		devhost_put_file(new_node);
		hm_error("failed to register file_node, err = %d\n", ret);
		return ret;
	} else {
		pret->filp = (unsigned long long)new_node;
		pret->kshm_key = kshm_key;
		pret->kobj_offset = kobj_offset;
	}

	devhost_put_file(new_node);

	return file_id;
}

static inline bool __has_listeners(struct filp_node *node)
{
	struct dlist_node *head = wait_head_of_filp(node);
	bool has_sleeper = false;

	if (head != NULL) {
		filp_poll_list_lock(node);
		has_sleeper = !dlist_empty(head);
		filp_poll_list_unlock(node);
	}

	return has_sleeper;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_shutdown, dh_net_socket_shutdown_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, how)
{
	UNUSED(sender, credential);
	int ret = 0;
	int count = 0;
	__u32 cnode_idx = sysif_actv_src_cnode_idx();
	struct socket_device_info *sock_dev = NULL;
	struct filp_node *node = NULL;

	acquire_net_socket_filp_node(cnode_idx, sockfd);

	if (node == NULL) {
		hm_error("devhost filp node is null\n");
		ret = E_HM_INVAL;
	} else {
		sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);
		if (sock_dev == NULL || sock_dev->handle_ops == NULL || sock_dev->sock_ops == NULL) {
			hm_error("sock_dev or sock_ops is invalid\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		net_enter_lite(sock_dev->handle_ops);
		ret = sock_dev->sock_ops->shutdown(node->filp, how);
		if (ret == -ERESTARTSYS) {
			ret = E_HM_RESTARTSYS;
		} else if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}

		/**
		 * shutdown operation will wakeup sleepers, but when a close operations
		 * is directly after shutdown, this may cause select recheck failed for the 
		 * fd is already closed, we add some gap after shutdown operatin to ensure
		 * select can be successfully wakeup;
		 *
		 * try 10 time is enough.
		 */
		while (__has_listeners(node) && (count < 10)) {
			usleep(20);
			++count;
		}
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_ioctl, dh_net_socket_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, cmd, void *, arg_buf)
{
	return drvhdlr_devhost_net_socket_ioctl(sender, credential, sockfd, cmd, arg_buf);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_dev_ioctl, dh_net_socket_dev_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, cmd, void *, arg_buf)
{
	UNUSED(sender, credential);
	int ret;

	ret = call_dev_ioctl(cmd, arg_buf);
	if (ret == -ERESTARTSYS) {
		ret = E_HM_RESTARTSYS;
	} else if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_fcntl, dh_net_socket_fcntl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, cmd, int, arg)
{
	return drvhdlr_devhost_net_socket_fcntl(sender, credential, sockfd, cmd, arg);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_send, dh_net_socket_send_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const void *, buf, size_t, len, int, flags)
{
	return drvhdlr_devhost_net_socket_send(sender, credential, sockfd, buf, len, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_recv, dh_net_socket_recv_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, void *, buf, size_t, len, int, flags)
{
	return drvhdlr_devhost_net_socket_recv(sender, credential, sockfd, buf, len, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_sendmmsg, dh_net_socket_sendmmsg_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const void *, msgvec, size_t, vlen, int, flags)
{
	return drvhdlr_devhost_net_socket_sendmmsg(sender, credential, sockfd, msgvec, vlen, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_recvmmsg, dh_net_socket_recvmmsg_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const void *, msgvec, size_t, vlen, int, flags, void *, timeout)
{
	return drvhdlr_devhost_net_socket_recvmmsg(sender, credential, sockfd, msgvec, vlen, flags, timeout);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_sendmsg, dh_net_socket_sendmsg_ext,
			 unsigned long long, sender, unsigned long, credential,
			long, sockfd, const struct msghdr *, msg, int, flags)
{
	return drvhdlr_devhost_net_socket_sendmsg(sender, credential, sockfd, msg, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_recvmsg, dh_net_socket_recvmsg_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, struct msghdr *, msg, int, flags)
{
	return drvhdlr_devhost_net_socket_recvmsg(sender, credential, sockfd, msg, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_sendto, dh_net_socket_sendto_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const void *, buf, size_t, len, int, flags,
			 const struct sockaddr *, daddr, socklen_t, addrlen)
{
	return drvhdlr_devhost_net_socket_sendto(sender, credential,
						 sockfd, buf, len, flags, daddr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_recvfrom, dh_net_socket_recvfrom_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, void *, buf, size_t, len, int, flags,
			 struct sockaddr *, saddr, socklen_t *, addrlen)
{
	return drvhdlr_devhost_net_socket_recvfrom(sender, credential,
						   sockfd, buf, len, flags, saddr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_readv, dh_net_socket_readv_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const struct iovec *, iov, int, iovcnt)
{
	return drvhdlr_devhost_net_socket_readv(sender, credential, sockfd, iov, iovcnt);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_writev, dh_net_socket_writev_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, const struct iovec *, iov, int, iovcnt, int, flags)
{
	return drvhdlr_devhost_net_socket_writev(sender, credential, sockfd, iov, iovcnt, flags);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_getsockopt, dh_net_socket_getsockopt_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, level, int, optname, void *, optval,
			 socklen_t *, optlen)
{
	return drvhdlr_devhost_net_socket_getsockopt(sender, credential, sockfd,
						     level, optname, optval, optlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_setsockopt, dh_net_socket_setsockopt_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, int, level, int, optname, const void *, optval,
			 socklen_t, optlen)
{
	return drvhdlr_devhost_net_socket_setsockopt(sender, credential, sockfd,
						     level, optname, optval, optlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_getsockname, dh_net_socket_getsockname_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, struct sockaddr *, addr,
			 socklen_t *, addrlen)
{
	return drvhdlr_devhost_net_socket_getsockname(sender, credential, sockfd, addr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_getpeername, dh_net_socket_getpeername_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, struct sockaddr *, addr,
			 socklen_t *, addrlen)
{
	return drvhdlr_devhost_net_socket_getpeername(sender, credential, sockfd, addr, addrlen);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_fchown, dh_net_socket_fchown_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, unsigned int, uid, unsigned int, gid)
{
	return drvhdlr_devhost_net_socket_fchown(sender, credential, sockfd, uid, gid);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_fstat, dh_net_socket_fstat_ext,
			 unsigned long long, sender, unsigned long, credential,
			 long, sockfd, uid_t *, uid)
{
	return drvhdlr_devhost_net_socket_fstat(sender, credential, sockfd, uid);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_bpf, dh_net_socket_bpf_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, cmd, void *, attr, unsigned int, size)
{
	UNUSED(sender, credential);
	int ret = E_HM_OK;

	ret = call_bpf(cmd, attr, size);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}
	return ret;
}

static int drvhdlr_devhost_net_socket_socket(unsigned int cnode_idx, int domain, int type,
					     int protocol, unsigned long ino,
					     void (*pre_hook)(struct devhost_net_handle_ops *),
					     net_socket_ret_t *pret)
{
	struct filp_node *node = NULL;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
	unsigned int file_id = -1;
	void *devhost_socket = NULL;
	int ret = E_HM_OK;

	node = alloc_socket_filp_node(cnode_idx, &kshm_key, &kobj_offset);
	if (node == NULL) {
		return E_HM_NOMEM;
	}

	struct socket_device_info *sock_dev = NULL;
	sock_dev = container_of(node->devinfo, struct socket_device_info, socket_devinfo);
	if (sock_dev->sock_ops == NULL || sock_dev->handle_ops == NULL) {
		devhost_put_file(node);
		return E_HM_INVAL;
	}

	pre_hook(sock_dev->handle_ops);

	ret = sock_dev->sock_ops->socket(domain, type, protocol, ino, &devhost_socket, node);
	if (ret == -ERESTARTSYS) {
		devhost_put_file(node);
		return E_HM_RESTARTSYS;
	} else if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	if (ret < 0) {
		devhost_put_file(node);
		return ret;
	}

	node->filp = devhost_socket;

	ret = devhost_register_file(node, &file_id);
	if (ret < 0) {
		hm_error("failed to register file_node, err = %d\n", ret);
		devhost_put_file(node);
		return ret;
	}
	pret->filp = (unsigned long long)(uintptr_t)node;
	pret->kshm_key = kshm_key;
	pret->kobj_offset = kobj_offset;
	devhost_put_file(node);

	return file_id;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_socket, dh_net_socket_socket_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, domain, int, type, int, protocol, unsigned long , ino)
{
	unsigned int cnode_idx = sysif_actv_src_cnode_idx();
	struct __actvret_drvcall_devhost_net_socket_socket *pret =
		actvhdlr_drvhandler_devhost_net_socket_socket_prepare_ret(sender, credential);
	BUG_ON(pret == NULL);

	return drvhdlr_devhost_net_socket_socket(cnode_idx, domain, type, protocol, ino, net_enter_nop, pret);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_alloc_new_ns, dh_net_socket_alloc_new_ns_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, nsid)
{
#ifdef CONFIG_NET_NS
	UNUSED(sender, credential);
	struct devhost_net_namespace_ops *net_namespace_ops = devhost_get_net_namespace_ops();
	int ret;

	if (net_namespace_ops == NULL || net_namespace_ops->alloc_new_ns == NULL) {
		return E_HM_INVAL;
	}

	ret = net_namespace_ops->alloc_new_ns(nsid);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
#else
	UNUSED(sender, credential, nsid);
	return 0;
#endif
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_net_socket_destroy_ns, dh_net_socket_destroy_ns_ext,
			 unsigned long long, sender, unsigned long, credential,
			 int, nsid)
{
#ifdef CONFIG_NET_NS
	UNUSED(sender, credential);
	struct devhost_net_namespace_ops *net_namespace_ops = devhost_get_net_namespace_ops();
	int ret;

	if (net_namespace_ops == NULL || net_namespace_ops->destroy_ns == NULL) {
		return E_HM_INVAL;
	}

	ret = net_namespace_ops->destroy_ns(nsid);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
	}

	return ret;
#else
	UNUSED(sender, credential, nsid);
	return 0;
#endif
}

/* load net_socket module */
static int net_socket_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("net_socket.ko");
	if (ret < 0) {
		hm_error("lnxbase: load net_socket.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, net_socket_load_module, NULL);

/*
 * net sysif module, support network devices operations
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_net_socket_register`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_close`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_connect`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_bind`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_listen`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_accept`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_shutdown`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_ioctl`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_dev_ioctl`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_fcntl`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_send`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_recv`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_sendmmsg`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_recvmmsg`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_sendmsg`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_recvmsg`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_sendto`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_recvfrom`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_readv`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_writev`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_getsockopt`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_setsockopt`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_getsockname`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_getpeername`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_fchown`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_fstat`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_socket`
 *  - target sysif-ep: `drvhandler_devhost_net_socket_bpf`
 *
 * dependence: NULL
 */

#ifdef CONFIG_NET_FUSION
/* assign selfcall flag to skip the security check in fusion mode */
#define SOCKET_EP_FLAG	DH_EXT_FLAG_SELFCALL
#else
#define SOCKET_EP_FLAG	DH_EXT_FLAG_NONE
#endif

#define IMPORT_SYSIF_SOCKET(ep_name, ext_name) \
		IMPORT_SYSIF_EXTENSION_CUSTOM(ep_name, ext_name, SOCKET_EP_FLAG)

DEFINE_DSO_PLUGIN(net_socket, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_net_socket_register, dh_net_socket_register_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_close, dh_net_socket_close_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_connect, dh_net_socket_connect_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_bind, dh_net_socket_bind_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_listen, dh_net_socket_listen_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_accept, dh_net_socket_accept_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_shutdown, dh_net_socket_shutdown_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_ioctl, dh_net_socket_ioctl_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_dev_ioctl, dh_net_socket_dev_ioctl_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_fcntl, dh_net_socket_fcntl_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_send, dh_net_socket_send_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_recv, dh_net_socket_recv_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_sendmmsg, dh_net_socket_sendmmsg_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_recvmmsg, dh_net_socket_recvmmsg_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_sendmsg, dh_net_socket_sendmsg_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_recvmsg, dh_net_socket_recvmsg_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_sendto, dh_net_socket_sendto_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_recvfrom, dh_net_socket_recvfrom_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_readv, dh_net_socket_readv_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_writev, dh_net_socket_writev_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_getsockopt, dh_net_socket_getsockopt_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_setsockopt, dh_net_socket_setsockopt_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_getsockname, dh_net_socket_getsockname_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_getpeername, dh_net_socket_getpeername_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_fchown, dh_net_socket_fchown_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_fstat, dh_net_socket_fstat_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_socket, dh_net_socket_socket_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_alloc_new_ns, dh_net_socket_alloc_new_ns_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_destroy_ns, dh_net_socket_destroy_ns_ext),
	IMPORT_SYSIF_SOCKET(drvhandler_devhost_net_socket_bpf, dh_net_socket_bpf_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
