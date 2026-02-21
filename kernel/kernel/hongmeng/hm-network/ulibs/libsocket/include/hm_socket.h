/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for socket
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:20:38 2019
 */

#ifndef __HM_SOCKET_H_
#define __HM_SOCKET_H_

#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <poll.h>
#include <ifaddrs.h>

#include <vfs.h>

#include <hmkernel/capability.h>
#include <libhmsrv_sys/hm_select_common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MSG_SIZE 0xFFFF
#define HMNET_SOCKET_VERSION 2019111401
#define SOCKET_PAIR_NUM 2

#define SIOCUEVENTBROAD 0x89ff

#define UEVENT_BUF_LEN (512)
#define UEVENT_ACTION_LEN (128)
#define UEVENT_DEVPATH_LEN (128)

struct uevent_msg {
	char buf[UEVENT_BUF_LEN];
	char action[UEVENT_ACTION_LEN];
	char devpath[UEVENT_DEVPATH_LEN];
	size_t len;
};

typedef struct fusion_ops {
	int (*fusion_copy_to_user)(void *dst, const void *src, unsigned long n);
	int (*fusion_copy_from_user)(void *dst, const void *src, unsigned long n);

	int (*fusion_register_shrinker)(unsigned long (*shrinker_query)(void *priv),
					unsigned long (*shrinker_shrink)(void *priv, int req_nr),
					void *priv);

	void *(*fusion_valloc)(unsigned int size);
	void  (*fusion_vfree)(void *ptr);
	int (*backend_ctx_setup)(void);
} fusion_ops_t;

int hm_net_dev_ioctl(int request, void *args);

void net_revoke_process(cref_t rref, uint32_t cnode_idx);
int net_notify_fork(cref_t ch, uint32_t src_cidx, uint32_t dst_cidx);
int hm_net_fork_notify(uint32_t src_cidx, uint32_t dst_cidx);


int hm_net_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx, int *recv_fd);
int hm_net_put_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx);

int hm_net_close(int sockfd);

int hm_net_bind(int sockfd, const struct sockaddr *addr, socklen_t len);
int hm_net_listen(int sockfd, int backlog);

int hm_net_fcntl(int sockfd, int cmd, int val);
int hm_net_dup(int oldfd);
int hm_net_dup2(int oldfd, int newfd);

int hm_net_getifaddrs(struct ifaddrs **ifap);

int hm_net_read_fds(uint32_t cnode_idx, int *fdcnt, int *fds, unsigned int max_files);
int hm_net_get_fd_path(int fd, char *path_buf, unsigned int path_buf_len, uint32_t cnode_idx);
int hm_net_query_file_id(int fd, uint32_t app_cnode_idx, unsigned long long *file_idx);

uint32_t hm_net_recheck_epoll_revents(struct __epoll_fetch_info_s *info);

int hm_net_proc_unauthoried_files(int sel_null_fd);

/*
 * APIs for system services to do socket access
 * @cnode: target process cnode
 * @fd: fd number of target process
 * @return: bytes read/write on success, errno on failure
 */
ssize_t hm_net_sys_write(uint32_t cnode, int fd, const void *buf, size_t len, int flags);
ssize_t hm_net_sys_read(uint32_t cnode, int fd, void *buf, size_t len, int flags);

int hm_net_sys_dev_ioctl(int request, void *args);
int hm_net_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg);

struct ux_socket;
int hm_net_sys_socket_attach(struct ux_socket *sock, unsigned int cnode, int fd);
void *hm_net_sys_socket_get(unsigned int cnode, int fd);
int hm_net_sys_socket_put(struct ux_socket *sock);

int hm_net_mmap(int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t len,
		unsigned int prot, unsigned int flags, long long offset, uintptr_t *rvaddr);

/*
 * those symbol mainly be used in sysmgr sysif handler
 * for using sysmgr actvpool
 */
ssize_t net_do_write(unsigned long long sender, unsigned long credential,
			long long socket, const void *buf, size_t len);
ssize_t net_do_writev(unsigned long long sender, unsigned long credential,
			 long long socket, const void *iov, int iovcnt);
ssize_t net_do_read(unsigned long long sender, unsigned long credential,
		       long long socket, void *buf, size_t len);
ssize_t net_do_readv(unsigned long long sender, unsigned long credential,
			long long socket, const void *iov, int iovcnt);
int net_do_close(unsigned long long sender, unsigned long credential,
		    long long ksock, int fd);
int net_do_fstat(unsigned long long sender, unsigned long credential,
		    long long socket, void *buf);
int net_do_fchown(unsigned long long sender, unsigned long credential,
		     long long socket, unsigned int uid, unsigned int gid);
int net_do_ioctl(unsigned long long sender, unsigned long credential,
		    long long socket, int request, unsigned long arg);
int net_do_fcntl(unsigned long long sender, unsigned long credential,
		 long long socket, int cmd, unsigned long val, int fd);
int net_do_dup2(unsigned long long sender, unsigned long credential,
		   long long socket, int newfd, int oldfd);
int net_do_dup3(unsigned long long sender, unsigned long credential,
		   long long socket, int newfd, int flags, int oldfd);
int net_do_dup(unsigned long long sender, unsigned long credential,
		  long long socket, int newfd);

int net_do_create(unsigned long long sender, unsigned long credential,
		     int domain, int type, int protocol);
int net_do_socketpair(unsigned long long sender, unsigned long credential,
			 int domain, int type, int protocol, int *fd);
int net_do_shutdown(unsigned long long sender, unsigned long credential,
		       long long socket, int how);
int net_do_bind(unsigned long long sender, unsigned long credential,
		   long long socket, const void *addr, socklen_t addrlen);
int net_do_listen(unsigned long long sender, unsigned long credential,
		     long long socket, int backlog);
int net_do_accept4(unsigned long long sender, unsigned long credential,
		      long long socket, void *addr, void *addrlen, int flags);
int net_do_connect(unsigned long long sender, unsigned long credential,
		      long long socket, const void *addr, socklen_t addrlen);
ssize_t net_do_send(unsigned long long sender, unsigned long credential,
		       long long socket, const void *buf, size_t len, int flags);
ssize_t net_do_recv(unsigned long long sender, unsigned long credential,
		       long long socket, void *buf, size_t len, int flags);
int net_do_sendmmsg(unsigned long long sender, unsigned long credential,
		       long long socket, const void *msgvec, size_t vlen, int flags);
int net_do_recvmmsg(unsigned long long sender, unsigned long credential,
		       long long socket, const void *msgvec, size_t vlen, int flags,
		       void *timeout);
ssize_t net_do_sendmsg(unsigned long long sender, unsigned long credential,
			  long long socket, const void *msg, int flags);
ssize_t net_do_recvmsg(unsigned long long sender, unsigned long credential,
			  long long socket, void *msg, int flags);
ssize_t net_do_sendto(unsigned long long sender, unsigned long credential,
			 long long socket, const void *buf, size_t len, int flags,
			 const void *addr, socklen_t addrlen);
ssize_t net_do_recvfrom(unsigned long long sender, unsigned long credential,
			   long long socket, void *buf, size_t len, int flags,
			   void *addr, void *addrlen);
int net_do_getsockopt(unsigned long long sender, unsigned long credential,
			 long long socket, int level, int optname,
			 void *optval, socklen_t *optlen);
int net_do_setsockopt(unsigned long long sender, unsigned long credential,
			 long long socket, int level, int optname,
			 const void *optval, socklen_t optlen);
int net_do_getsockname(unsigned long long sender, unsigned long credential,
			  long long socket, void *addr, void *addrlen);
int net_do_getpeername(unsigned long long sender, unsigned long credential,
			  long long socket, void *addr, void *addrlen);
int net_do_revoke(unsigned long long sender, unsigned long credential);

/*
 * Since the header file is public, hm-network and other repos will also
 * include it. The net_do ops series functions are fully implemented on
 * the net side and do not require the stub function. However, when
 * CONFIG_NETFS_FUSION is disalbe, the fs side needs to call the stub
 * function of the net_do series function, so we use the following macro
 * way to isolate the scenario.
 */
#if !(!defined(CONFIG_NETFS_FUSION) && defined(CONFIG_VFS))
/* used in vfs side for direct function call */
int net_do_fork_copy(uint32_t src_cidx, uint32_t dst_cidx);
void net_do_revoke_by_cnode(uint32_t cnode_idx);

int net_do_binderfs_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx, int *recv_fd);
int net_do_binderfs_put_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx);

ssize_t net_do_sys_read(uint32_t cnode, int fd,
		    void *buf, size_t len, int flags);
ssize_t net_do_sys_write(uint32_t cnode, int fd,
		    const void *buf, size_t len, int flags);
int net_do_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg);
/* used to upcall handler call in vfs side */
int net_do_handle_cloexec(uint32_t cnode_idx, int fd, unsigned int rpccnt);
int net_do_handle_common_file_close(uint32_t cnode_idx, int fd, unsigned int rpccnt);
int net_do_handle_rpccnt_put(union __arch_actv_upcall_args_u args);
void net_do_balance_rpccnt(void *common_file, unsigned int rpccnt);

void net_do_eventpoll_polllist_sync(uint32_t cnode, int fd);
int net_do_poll_fds(struct pollfd *user_fds, nfds_t nfds);
int net_do_select(struct actv_upcall_select_s *arg);

struct sec_chk_cred;
void net_do_proc_unauthoried_files(struct sec_chk_cred *cred, struct sysarg_fdtable_all_file *entry,
				   int sel_null_fd);
void net_do_read_fds(unsigned int cnode_idx, unsigned int *fdcnt, int *fds,
		     unsigned int max_files);
int net_do_get_fd_path(int fd, char *path_buf, unsigned int path_buf_len,
		       unsigned int cnode_idx);
#else /* !CONFIG_NETFS_FUSION */
static inline int net_do_fork_copy(uint32_t src_cidx, uint32_t dst_cidx)
{
	return 0;
}
static inline void net_do_revoke_by_cnode(uint32_t cnode_idx)
{
	return;
}

static inline int net_do_binderfs_send_fd(int send_fd, uint32_t send_cidx,
					  uint32_t recv_cidx, int *recv_fd)
{
	return E_HM_OPNOTSUPP;
}

static inline int net_do_binderfs_put_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx)
{
	return E_HM_OPNOTSUPP;
}

static inline ssize_t net_do_sys_read(uint32_t cnode, int fd,
		    void *buf, size_t len, int flags)
{
	return E_HM_OPNOTSUPP;
}
static inline ssize_t net_do_sys_write(uint32_t cnode, int fd,
		    const void *buf, size_t len, int flags)
{
	return E_HM_OPNOTSUPP;
}

static inline int net_do_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg)
{
	return E_HM_OPNOTSUPP;
}

/* used to upcall handler call in vfs side */
static inline int net_do_handle_cloexec(uint32_t cnode_idx, int fd,
					unsigned int rpccnt)
{
	return E_HM_OPNOTSUPP;
}

static inline int net_do_handle_common_file_close(uint32_t cnode_idx, int fd,
						  unsigned int rpccnt)
{
	return E_HM_OPNOTSUPP;
}

static inline int net_do_handle_rpccnt_put(union __arch_actv_upcall_args_u args)
{
	return 0;
}
static inline void net_do_balance_rpccnt(void *common_file, unsigned int rpccnt)
{
	return;
}
static inline void net_do_eventpoll_polllist_sync(uint32_t cnode, int fd)
{
	return;
}
static inline int net_do_poll_fds(struct pollfd *user_fds, nfds_t nfds)
{
	return 0;
}
static inline int net_do_select(struct actv_upcall_select_s *arg)
{
	return 0;
}

struct sec_chk_cred;
static inline void net_do_proc_unauthoried_files(struct sec_chk_cred *cred,
						 struct sysarg_fdtable_all_file *entry, int sel_null_fd)
{
	return;
}

static void net_do_read_fds(unsigned int cnode_idx, unsigned int *fdcnt, int *fds,
			    unsigned int max_files)
{
	UNUSED(cnode_idx, fdcnt, fds, max_files);
	return;
}
static int net_do_get_fd_path(int fd, char *path_buf, unsigned int path_buf_len,
			      unsigned int cnode_idx)
{
	UNUSED(fd, path_buf, path_buf_len, cnode_idx);
	return E_HM_BADF;
}

#endif /* CONFIG_NETFS_FUSION */

#ifdef __cplusplus
}
#endif

#endif
