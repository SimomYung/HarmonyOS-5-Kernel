/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: devhost net socket operation
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:30:00 2022
 */

#include <asm/uaccess.h>
#include <asm/ioctls.h>
#include <linux/file.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/securec.h>
#include <net/sock.h>
#include <net/wext.h>
#include <net/compat.h>
#include <linux/kmemleak.h>
#include "net_socket_api.h"
#include "net_namespace.h"
#include "net_socket_main.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("net socket ops");
MODULE_VERSION("0.01");

static inline bool is_compat_client(void)
{
	return liblinux_pal_is_compat_client();
}

/* for devhost socket */
static struct device_info *socket_devinfo = NULL;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
#define sock_getname(_socket, _sock, _address, _peer) ({						\
	(_socket)->sock->ops->getname((_sock), (struct sockaddr *)&(_address), (_peer));		\
})
#else
#define sock_getname(_socket, _sock, _address, _peer) ({						\
	int _rc, _len;											\
	_rc = (_socket)->sock->ops->getname((_sock), (struct sockaddr *)&(_address), &(_len), (_peer));	\
	_rc == 0 ? _len : _rc;										\
})
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
#define USER_SOCKPTR(ptr) ((char __user *)(ptr))
#endif

/*
 * 0x100000 is an internal flag in Linux for sendpage.
 * We reuse this bit to mark whether to go fastcopy or not.
 */
#define MSG_FASTCOPY 0x100000
static inline int flags_set_fastcopy(int flags)
{
	return (int)((unsigned int)flags | MSG_FASTCOPY);
}

static inline bool flags_need_fastcopy(int flags)
{
	return (((unsigned int)flags & MSG_FASTCOPY) != 0);
}

static inline int flags_clr_fastcopy(int flags)
{
	return (int)((unsigned int)flags & (~MSG_FASTCOPY));
}

/*
 * 0x80000 is an internal flag in Linux for sendpage.
 */
#define MSG_RECVQUEUE 0x80000
static inline bool flags_init_queue(int flags)
{
	return (((unsigned int)flags & MSG_RECVQUEUE) != 0);
}

static inline void *socket_send_queue(struct devhost_socket *socket)
{
	/*
	* send_queue shared with recv_queue
	*/
	return socket->recv_queue;
}

static int liblinux_net_socket_poll_ex(void *ctx, void *pinfo, unsigned int *revents)
{
	struct file *filp = ((struct devhost_socket *)ctx)->file;

	return devhost_net_fops_poll((void *)filp, pinfo, revents);
}

static int liblinux_net_socket_mmap_ex(void *ctx, struct devhost_vma_info *info)
{
	struct file *filp = ((struct devhost_socket *)ctx)->file;
	return devhost_net_fops_mmap((void *)filp, info);
}

static struct devhost_fops net_socket_fops = {
	.poll = NULL,
};

static struct devhost_fops_ex net_socket_ex_fops = {
	.size = sizeof(struct devhost_fops_ex),
	.poll = liblinux_net_socket_poll_ex,
	.mmap_ex = liblinux_net_socket_mmap_ex,
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
static inline void __iov_iter_init(struct iov_iter *i, unsigned int direction,
				   const struct iovec *iov, unsigned long nr_segs,
				   size_t count)
{
	direction &= READ | WRITE;

	i->type = ITER_IOVEC | direction;
	i->iov = iov;
	i->nr_segs = nr_segs;
	i->iov_offset = 0;
	i->count = count;
}
#else
static inline void __iov_iter_init(struct iov_iter *i, unsigned int direction,
				   const struct iovec *iov, unsigned long nr_segs,
				   size_t count)
{
	iov_iter_init(i, direction, iov, nr_segs, count);
}
#endif

static int move_addr_to_k(const void *uaddr, unsigned int ulen, struct sockaddr_storage *kaddr)
{
	if (ulen > sizeof(struct sockaddr_storage)) {
		return -EINVAL;
	} else if (ulen == 0) {
		return 0;
	}

	if (copy_from_user(kaddr, uaddr, ulen)) {
		return -EFAULT;
	}

	return 0;
}

static int move_addr_to_u(struct sockaddr_storage *kaddr, int klen, void *uaddr, int *ulen)
{
	int length;

	if (get_user(length, ulen)) {
		return -EFAULT;
	}

	if (length < 0) {
		return -EINVAL;
	}

	if (length > klen) {
		length = klen;
	}

	if (length) {
		if (copy_to_user(uaddr, kaddr, length)) {
			return -EFAULT;
		}
	}

	return __put_user(klen, ulen);
}

static int devhost_net_socket_socket(int domain, int type, int protocol, unsigned long ino,
				      void **devhost_socket, void *filp_node)
{
	int rc = 0;
	int flags = 0;
	struct file *newfile = NULL;
	struct devhost_socket *socket = NULL;
	struct net *netns;

	flags = type & ~SOCK_TYPE_MASK;
	if (flags & ~(SOCK_CLOEXEC | SOCK_NONBLOCK)) {
		return -EINVAL;
	}
	type &= SOCK_TYPE_MASK;

	socket = kmalloc(sizeof(struct devhost_socket), GFP_KERNEL | __GFP_ZERO);
	if (socket == NULL) {
		return -ENOMEM;
	}

	rc = devhost_get_netns(&netns);
	if (rc != 0) {
		goto free;
	}

	rc = __sock_create(netns, domain, type, protocol, &socket->sock, 0);
	if (rc < 0) {
		goto out;
	}

	newfile = sock_alloc_file(socket->sock, flags, NULL);
	if (IS_ERR(newfile)) {
		rc = -EINVAL;
		/* ldk: sock_alloc_file already contains sock_release(sock). */
		goto out;
	}

	newfile->liblinux_private = (void *)filp_node;
	socket->file = newfile;

	SOCK_INODE(socket->sock)->i_ino = ino;
	*devhost_socket = socket;
	kmemleak_not_leak((void *)socket);
	if (SOCK_NONBLOCK != O_NONBLOCK && (flags & SOCK_NONBLOCK)) {
		flags = (flags & ~SOCK_NONBLOCK) | O_NONBLOCK;
	}

	socket->file_flags = O_RDWR | (flags & (O_CLOEXEC | O_NONBLOCK));

	put_net(netns);
	return 0;
out:
	put_net(netns);
free:
	kfree(socket);
	return rc;
}

static int devhost_net_socket_close(void *devhost_socket)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (socket->sock != NULL) {
		WRITE_ONCE(socket->file->liblinux_private, NULL);
		__fput_sync(socket->sock->file);
		socket->sock = NULL;
	}
	kfree(socket);

	return 0;
}

static int devhost_net_socket_connect(void *devhost_socket, const void *addr,
				      unsigned addrlen)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct sockaddr_storage kaddr;

	if (socket == NULL) {
		return -EINVAL;
	}

	rc = move_addr_to_k(addr, addrlen, &kaddr);
	if (rc < 0) {
		return rc;
	}

	rc = kernel_connect(socket->sock, (struct sockaddr *)&kaddr, addrlen, socket->file_flags);

	return rc;
}

static int devhost_net_socket_bind(void *devhost_socket, const void *addr,
				   unsigned addrlen)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct sockaddr_storage kaddr_storage;

	if (socket == NULL) {
		return -EINVAL;
	}

	rc = move_addr_to_k(addr, addrlen, &kaddr_storage);
	if (rc < 0) {
		return rc;
	}

	rc = kernel_bind(socket->sock, (struct sockaddr *)&kaddr_storage, addrlen);

	return rc;
}

static int devhost_net_socket_listen(void *devhost_socket, int backlog)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	int somaxconn;
	int kconn = backlog;

	if (socket == NULL) {
		return -EINVAL;
	}

	somaxconn = sock_net(socket->sock->sk)->core.sysctl_somaxconn;
	if ((unsigned int)kconn > somaxconn) {
		kconn = somaxconn;
	}

	rc = kernel_listen(socket->sock, kconn);

	return rc;
}

static int copy_sockname_to_user(struct devhost_socket *new_socket,
				 void *addr,
				 unsigned *addrlen)
{
	int rc = 0;
	int len = 0;
	struct sockaddr_storage address = {0};

	/* peer = 2 : current/former peer ID */
	len = sock_getname(new_socket, new_socket->sock, address, 2);
	if (len <= 0) {
		return -ECONNABORTED;
	}

	rc = move_addr_to_u(&address, len, addr, addrlen);
	if (rc < 0) {
		return rc;
	}

	return 0;
}

static int __sock_accept(struct socket *sock, struct socket **newsock, int flags)
{
    struct sock *sk = sock->sk;
    int err;
 
    err = sock_create_lite(sk->sk_family, sk->sk_type, sk->sk_protocol, newsock);
    if (err < 0) {
        return err;
	}
 
    (*newsock)->ops = sock->ops;
    err = sock->ops->accept(sock, *newsock, flags, true);
    if (err < 0) {
        sock_release(*newsock);
		*newsock = NULL;
        return err;
	}
 
    __module_get((*newsock)->ops->owner);
    return err;
}

static int devhost_net_socket_accept(void *devhost_socket, void *addr, unsigned *addrlen, int flags,
					unsigned long ino, void **new_devhost_socket, void *filp_node)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct file *newfile = NULL;
	struct devhost_socket *new_socket = NULL;

	if (socket == NULL) {
		return -EINVAL;
	}

	new_socket = kmalloc(sizeof(struct devhost_socket), GFP_KERNEL | __GFP_ZERO);
	if (new_socket == NULL) {
		return -ENOMEM;
	}

	rc = __sock_accept(socket->sock, &new_socket->sock, socket->file_flags);
	if (rc < 0) {
		goto out_free;
	}

	newfile = sock_alloc_file(new_socket->sock, flags, NULL);
	if (IS_ERR(newfile)) {
		rc = -EINVAL;
		goto out_free;
	}

	newfile->liblinux_private = (void *)filp_node;
	new_socket->file = newfile;
	new_socket->file_flags = O_RDWR | (flags & O_NONBLOCK);
	SOCK_INODE(new_socket->sock)->i_ino = ino;
	*new_devhost_socket = new_socket;

	if (addr == NULL) {
		return 0;
	}

	rc = copy_sockname_to_user(new_socket, addr, addrlen);
	if (rc != 0) {
		goto out_release;
	}

	return 0;
out_release:
	fput(newfile);
	new_socket->sock = NULL;
out_free:
	kfree(new_socket);
	return rc;
}

static int devhost_net_socket_shutdown(void *devhost_socket, int how)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (socket == NULL) {
		return -EINVAL;
	}

	rc = kernel_sock_shutdown(socket->sock, how);

	return rc;
}

static int ioctl_fionbio(struct devhost_socket *socket, int *arg)
{
	int rc;
	int on;

	rc = get_user(on, arg);
	if (rc) {
		return rc;
	}

	if (on) {
		socket->file_flags |= O_NONBLOCK;
	} else {
		socket->file_flags &= ~O_NONBLOCK;
	}

	return rc;
}

static int normal_socket_ioctl(struct file *filp, int cmd, unsigned long arg)
{
	int rc = -ENOTTY;

	if (filp->f_op->unlocked_ioctl != NULL) {
		rc = filp->f_op->unlocked_ioctl(filp, cmd, arg);
		if (rc == -ENOIOCTLCMD) {
			rc = -ENOTTY;
		}
	}

	return rc;
}

#ifdef CONFIG_COMPAT
static int compat_socket_ioctl(struct file *filp, int cmd, unsigned long arg)
{
	int rc = -ENOIOCTLCMD;

	if (is_compat_client()) {
		if (filp->f_op->compat_ioctl != NULL) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
			set_thread_flag(TIF_32BIT);
#endif
			rc = filp->f_op->compat_ioctl(filp, cmd, arg);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
			clear_thread_flag(TIF_32BIT);
#endif
		}
		if (rc == -ENOIOCTLCMD) {
			rc = normal_socket_ioctl(filp, cmd, arg);
		}
	} else {
		rc = normal_socket_ioctl(filp, cmd, arg);
	}

	return rc;
}
#endif

static int ioctl_socket(struct file *filp, int cmd, unsigned long arg)
{
	int rc = -ENOTTY;

	if (filp->f_op == NULL) {
		return rc;
	}

#ifdef CONFIG_COMPAT
	rc = compat_socket_ioctl(filp, cmd, arg);
#else
	rc = normal_socket_ioctl(filp, cmd, arg);
#endif

	return rc;
}

static int devhost_net_socket_ioctl(void *devhost_socket, int cmd, void *arg_buf)
{
	int rc = -ENOTTY;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (socket == NULL || socket->file == NULL) {
		return rc;
	}

	switch (cmd) {
		case FIONBIO:
			rc = ioctl_fionbio(socket, (int *)arg_buf);
			break;
		default:
			rc = ioctl_socket(socket->file, cmd, (unsigned long)(uintptr_t)arg_buf);
			break;
	}

	return rc;
}

static int socket_dev_ioctl(struct net *netns, int cmd, unsigned long arg, bool is_compat)
{
	int rc = -ENOTTY;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	struct ifreq ifr = {0};
	size_t size = is_compat ? sizeof(struct compat_ifreq) : sizeof(struct ifreq);

	if (copy_from_user(&ifr, arg, size)) {
		return -EFAULT;
	}

	bool need_copyout = false;
	rc = dev_ioctl(netns, cmd, &ifr, &need_copyout);
	if (!rc && need_copyout) {
		if (copy_to_user(arg, &ifr, size)) {
			return -EFAULT;
		}
	}
#else

	rc = dev_ioctl(netns, cmd, arg);
#endif

	return rc;
}

#ifdef CONFIG_COMPAT
static int compat_socket_dev_ioctl(struct net *netns, int cmd, unsigned long arg)
{
	int rc = -ENOIOCTLCMD;

	if (is_compat_client()) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
		set_thread_flag(TIF_32BIT);
#endif
		rc = socket_dev_ioctl(netns, cmd, arg, true);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
		clear_thread_flag(TIF_32BIT);
#endif
	} else {
		rc = socket_dev_ioctl(netns, cmd, arg, false);
	}

	return rc;
}
#endif

static int dev_ioctl_socket(struct net *netns, int cmd, unsigned long arg)
{
	int rc = -ENOTTY;

#ifdef CONFIG_COMPAT
	rc = compat_socket_dev_ioctl(netns, cmd, arg);
#else
	rc = socket_dev_ioctl(netns, cmd, arg, false);
#endif

	return rc;
}

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

static int dev_ioctl_uevent_broadcast(void *buf)
{
	int ret = -ENOSYS;
	struct uevent_msg *msg = NULL;
	struct kobj_uevent_env *env = NULL;

	msg = kzalloc(sizeof(struct uevent_msg), GFP_KERNEL);
	if (!msg) {
		return -ENOMEM;
	}

	if (copy_from_user(msg, buf, sizeof(struct uevent_msg))) {
		kfree(msg);
		return -EFAULT;
	}

	if (msg->len > UEVENT_BUF_LEN) {
		kfree(msg);
		return -EINVAL;
	}

	/* environment buffer */
	env = kzalloc(sizeof(struct kobj_uevent_env), GFP_KERNEL);
	if (!env) {
		kfree(msg);
		return -ENOMEM;
	}

	ret = memcpy_s(env->buf, UEVENT_BUFFER_SIZE, msg->buf, msg->len);
	if (ret != 0) {
		panic("uevent broadcast memcpy panic\n");
	}

	env->buflen = msg->len;

	ret = kobject_uevent_net_broadcast_untagged(env, msg->action, msg->devpath);

	kfree(msg);
	kfree(env);

	return ret;
}

static int devhost_net_socket_dev_ioctl(int cmd, void *arg_buf)
{
	int rc;

	if (cmd == SIOCGIFNAME || cmd == SIOCGIFINDEX) {
		struct net *netns = NULL;
		rc = devhost_get_netns(&netns);
		if (rc == 0) {
			rc = dev_ioctl_socket(netns, cmd, (unsigned long)(uintptr_t)arg_buf);
			put_net(netns);
		}
	} else if (cmd == SIOCUEVENTBROAD) {
		rc = dev_ioctl_uevent_broadcast(arg_buf);
	} else {
		rc = -ENOTTY;
	}

	return rc;
}

static int devhost_net_socket_sendto(void *devhost_socket, const void *buf, size_t len, int flags,
				     const void *daddr, unsigned addrlen)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	int rc;
	struct msghdr msg;
	struct iovec iov;
	struct sockaddr_storage address;
	int msg_flags = flags;

	if (socket == NULL) {
		return -EINVAL;
	}

	rc = import_single_range(WRITE, (void __user *)buf, len, &iov, &msg.msg_iter);
	if (unlikely(rc)) {
		return rc;
	}

	msg.msg_name = NULL;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_namelen = 0;

	if (flags_need_fastcopy(msg_flags)) {
		msg_flags = flags_clr_fastcopy(msg_flags);
		msg.msg_iter.type |= ITER_KVEC;

		if (daddr) {
			msg.msg_name = (struct sockaddr_storage *)daddr;
			msg.msg_namelen = addrlen;
		}
	} else if (daddr) {
		rc = move_addr_to_k(daddr, addrlen, &address);
		if (rc < 0) {
			goto out;
		}
		msg.msg_name = &address;
		msg.msg_namelen = addrlen;
	}

	if (socket->file_flags & O_NONBLOCK) {
		msg_flags |= MSG_DONTWAIT;
	}
	msg.msg_flags = msg_flags;
	rc = sock_sendmsg(socket->sock, &msg);

out:
	return rc;
}

static void net_socket_send_from_sendqueue(struct devhost_socket *socket, size_t len, int flags)
{
	struct recv_queue_meta *meta = RECV_QUEUE_META_ADDR(socket->recv_queue);
	int send_flags = flags_set_fastcopy(flags);
	void *data = socket->recv_queue;
	const void *buf = NULL;

	if ((!(meta->flags & RECV_QUEUE_FLAG_WRITE_BIT)) || len != RECV_QUEUE_IOV_SIZE) {
		return;
	}

	while (meta->count) {
		buf = RECV_QUEUE_IOV_ADDR(data, meta->head);
		(void)devhost_net_socket_sendto(socket, buf, RECV_QUEUE_IOV_SIZE, send_flags, NULL, 0);
		meta_consumer_pop(meta);
	}

	/*
	* tell that next syscall write to sendbuff in kernel first.
	*/
	meta->flags |= RECV_QUEUE_FLAG_KERNEL_DONE_BIT;
}

static void socket_init_queue(struct devhost_socket *socket, void *recv_queue, int flags)
{
	if (socket->recv_queue == NULL) {
		struct recv_queue_meta *meta = RECV_QUEUE_META_ADDR(recv_queue);
		socket->recv_queue = recv_queue;
		meta->flags |= flags;
	}
}

static int devhost_net_socket_send(void *devhost_socket, const void *buf, size_t len, int flags)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (flags_init_queue(flags)) {
		socket_init_queue(socket, (void *)buf, RECV_QUEUE_FLAG_WRITE_BIT);
		return 0;
	}

	if (socket_send_queue(socket) != NULL) {
		net_socket_send_from_sendqueue(socket, len, flags);
	}

	return devhost_net_socket_sendto(devhost_socket, buf, len, flags, NULL, 0);
}

static int devhost_net_socket_recvfrom(void *devhost_socket, void *buf, size_t len, int flags,
				       void *saddr, unsigned *addrlen)
{
	int rc;
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct iovec iov;
	struct msghdr msg;
	struct sockaddr_storage address;

	if (socket == NULL) {
		return -EINVAL;
	}

	rc = import_single_range(READ, buf, len, &iov, &msg.msg_iter);
	if (unlikely(rc)) {
		return rc;
	}

	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_name = saddr ? (struct sockaddr *)&address : NULL;
	msg.msg_namelen = 0;
	msg.msg_iocb = NULL;
	msg.msg_flags = 0;

	if (socket->file_flags & O_NONBLOCK) {
		flags |= MSG_DONTWAIT;
	}

	rc = sock_recvmsg(socket->sock, &msg, flags);
	if (rc < 0) {
		return rc;
	}

	if (saddr) {
		int err = move_addr_to_u(&address, msg.msg_namelen, saddr, addrlen);
		if (err < 0) {
			rc = err;
		}
	}

	return rc;
}

static int devhost_net_socket_recv(void *devhost_socket, void *buf, size_t len, int flags)
{
	return devhost_net_socket_recvfrom(devhost_socket, buf, len, flags, NULL, NULL);
}

static ssize_t import_iovec_bulk(int type, unsigned nr_segs, struct iovec **iovp,
				 const struct iovec __user *uvec, struct iov_iter *i)
{
	struct iovec *iov = *iovp;
	ssize_t total_len = 0;
	unsigned long seg;
	ssize_t ret = -1;

	if (unlikely(nr_segs > UIO_FASTIOV)) {
		iov = kmalloc_array(nr_segs, sizeof(struct iovec), GFP_KERNEL);
		if (iov == NULL) {
			return -ENOMEM;
		}
		if (copy_from_user(iov, uvec, nr_segs * sizeof(struct iovec))) {
			ret = -EFAULT;
			goto out_free;
		}
	}

	for (seg = 0; seg < nr_segs; seg++) {
		ssize_t len = (ssize_t)iov[seg].iov_len;
		if (len < 0) {
			ret = -EINVAL;
			goto out_free;
		}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
		if (!access_ok(iov[seg].iov_base, len)) {
#else
		if (!access_ok(VERIFY_READ, iov[seg].iov_base, len)) {
#endif
			ret = -EFAULT;
			goto out_free;
		}

		if (len > MAX_RW_COUNT - total_len) {
			len = MAX_RW_COUNT - total_len;
			iov[seg].iov_len = len;
		}
		total_len += len;
	}

	__iov_iter_init(i, type, iov, nr_segs, total_len);
	*iovp = iov;

	return total_len;
out_free:
	if (iov != *iovp) {
		kfree(iov);
	}
	return ret;
}

static int copy_msghdr_from_user(struct msghdr *kmsghdr,
				 struct user_msghdr __user *umsghdr,
				 struct sockaddr_storage __user **saddr,
				 struct iovec **iov)
{
	struct user_msghdr msg_tmp = {
		.msg_name = (saddr == NULL) ? kmsghdr->msg_name : NULL,
		.msg_control = kmsghdr->msg_control,
		.msg_controllen = kmsghdr->msg_controllen,
		.msg_iov = *iov,
		.msg_iovlen = UIO_FASTIOV,
	};
	size_t segs_cnt;
	int rc;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	kmsghdr->msg_control_is_user = true;
	if (!access_ok(umsghdr, sizeof(*umsghdr))) {
#else
	if (!access_ok(VERIFY_READ, umsghdr, sizeof(*umsghdr))) {
#endif
		return -EFAULT;
	}

	if ((rc = liblinux_pal_copy_msghdr_from_user(&msg_tmp, umsghdr)) < 0) {
		return rc;
	}

	segs_cnt = msg_tmp.msg_iovlen;
	kmsghdr->msg_namelen = msg_tmp.msg_namelen;
	kmsghdr->msg_control = msg_tmp.msg_control;
	kmsghdr->msg_controllen = msg_tmp.msg_controllen;
	kmsghdr->msg_flags = msg_tmp.msg_flags;

	if (msg_tmp.msg_name == NULL) {
		kmsghdr->msg_name = NULL;
		kmsghdr->msg_namelen = 0;
	}

	if (segs_cnt > UIO_MAXIOV) {
		return -EMSGSIZE;
	}

	kmsghdr->msg_iocb = NULL;

	if (saddr) {
		*saddr = msg_tmp.msg_name;
	}

	return import_iovec_bulk(saddr ? READ : WRITE, segs_cnt, iov, msg_tmp.msg_iov,
				 &kmsghdr->msg_iter);
}

static int get_msg_from_u(struct msghdr *msg_sys, struct user_msghdr *umsg,
			       struct compat_msghdr __user *umsg_compat,
			       struct sockaddr_storage __user **uaddr,
			       struct iovec **iov, int *flags)
{
	int rc;

	/*
	 * The MSG_CMSG_COMPAT flag cannot be set by users. Therefore,
	 * we need to clear the MSG_CMSG_COMPAT flag in flags first.
	 */
	*flags = *flags & ~MSG_CMSG_COMPAT;

#ifdef CONFIG_COMPAT
	if (is_compat_client()) {
		*flags = *flags | MSG_CMSG_COMPAT;
		rc = get_compat_msghdr(msg_sys, umsg_compat, (struct sockaddr **)uaddr, iov);
	} else {
		rc = copy_msghdr_from_user(msg_sys, umsg, uaddr, iov);
	}
#else
	UNUSED(umsg_compat);
	if (flags & MSG_CMSG_COMPAT) {
		return -EINVAL;
	}

	rc = copy_msghdr_from_user(msg_sys, umsg, uaddr, iov);
#endif

	return rc;
}

static int devhost_sendmsg(struct devhost_socket *socket, struct user_msghdr __user * msg,
				struct msghdr *msghdr_sys, int flags)
{
	int rc;
	struct sockaddr_storage address = {0};
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;
	unsigned char ctl[sizeof(struct cmsghdr) + 20] __attribute__ ((aligned(sizeof(__kernel_size_t)))), *ctl_buf = ctl;

	msghdr_sys->msg_name = &address;
	msghdr_sys->msg_control = ctl_buf;
	msghdr_sys->msg_controllen = sizeof(ctl);
	if ((rc = get_msg_from_u(msghdr_sys, msg, (struct compat_msghdr __user *)msg, NULL, &iov, &flags)) < 0) {
		return rc;
	}

	rc = -ENOBUFS;
	if (msghdr_sys->msg_controllen > INT_MAX) {
		goto out_free;
	}

	flags |= (msghdr_sys->msg_flags & MSG_EOR);
	size_t ctl_len = msghdr_sys->msg_controllen;
	if ((MSG_CMSG_COMPAT & flags) && ctl_len) {
#ifdef CONFIG_COMPAT
		rc = cmsghdr_from_user_compat_to_kern(msghdr_sys, socket->sock->sk, ctl, sizeof(ctl));
		if (rc) {
			goto out_free;
		}
#endif
		ctl_buf = msghdr_sys->msg_control;
		ctl_len = msghdr_sys->msg_controllen;
	} else if (ctl_len) {
		if (ctl_len > sizeof(ctl)) {
			if ((ctl_buf = sock_kmalloc(socket->sock->sk, ctl_len, GFP_KERNEL)) == NULL) {
				goto out_free;
			}
			rc = -EFAULT;
			if (copy_from_user(ctl_buf, (void __user __force *)msghdr_sys->msg_control, ctl_len)) {
				goto out_free_ctl;
			}
		}
		msghdr_sys->msg_control = ctl_buf;
	}
	msghdr_sys->msg_flags = flags;

	if (socket->file_flags & O_NONBLOCK) {
		msghdr_sys->msg_flags |= MSG_DONTWAIT;
	}

	rc = sock_sendmsg(socket->sock, msghdr_sys);

out_free_ctl:
	if (ctl_buf != ctl) {
		sock_kfree_s(socket->sock->sk, ctl_buf, msghdr_sys->msg_controllen);
	}
out_free:
	if (iov != iovstack) {
		kfree(iov);
	}
	return rc;
}

static int devhost_net_socket_sendmmsg(void *devhost_socket, const void *msgvec, size_t vlen, int flags)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct mmsghdr __user *entry = (struct mmsghdr __user *)msgvec;
	struct compat_mmsghdr __user *compat_entry = (struct compat_mmsghdr __user *)msgvec;
	struct msghdr msg_sys;
	ssize_t rc;
	int datagrams = 0;
	int flag = flags;
	int len = vlen;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (len > UIO_MAXIOV) {
		len = UIO_MAXIOV;
	}

	flag |= MSG_BATCH;

	while (datagrams < len) {
		if (datagrams == len - 1) {
			flag = flags;
		}

		if (MSG_CMSG_COMPAT & flag) {
			rc = devhost_sendmsg(socket, (struct user_msghdr __user *)compat_entry,
					       &msg_sys, flag);
			if (rc < 0) {
				break;
			}
			rc = __put_user(rc, &compat_entry->msg_len);
			++compat_entry;
		} else {
			rc = devhost_sendmsg(socket, (struct user_msghdr __user *)entry,
					       &msg_sys, flag);
			if (rc < 0) {
				break;
			}
			rc = put_user(rc, &entry->msg_len);
			++entry;
		}

		if (rc)
			break;
		++datagrams;
		if (msg_data_left(&msg_sys))
			break;
		cond_resched();
	}
	if (datagrams != 0) {
		return datagrams;
	}
	return rc;
}

static int devhost_net_socket_sendmsg(void *devhost_socket, const void *msg, int flags)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct msghdr msghdr_sys = {0};

	if (socket == NULL) {
		return -EINVAL;
	}

	return devhost_sendmsg(socket, (struct user_msghdr __user *)msg, &msghdr_sys, flags);
}

#define COMPAT_MSG(msg, member)	((MSG_CMSG_COMPAT & flags) ? &msg##_compat->member : &msg->member)
#define COMPAT_NAMELEN(msg)	COMPAT_MSG(msg, msg_namelen)
#define COMPAT_FLAGS(msg)	COMPAT_MSG(msg, msg_flags)

static int put_msg_to_u(struct msghdr *msg_sys, struct user_msghdr *umsg,
			struct compat_msghdr __user *umsg_compat,
			unsigned long cmsg_ptr,
			struct sockaddr_storage __user *uaddr,
			int __user *uaddr_len, int flags)
{
	int rc;
	rc = __put_user((msg_sys->msg_flags & ~MSG_CMSG_COMPAT), COMPAT_FLAGS(umsg));
	if (rc < 0) {
		return -EFAULT;
	}

	if (flags & MSG_CMSG_COMPAT) {
		rc = __put_user((unsigned long)msg_sys->msg_control - cmsg_ptr,
				  &umsg_compat->msg_controllen);
	} else {
		rc = __put_user((unsigned long)msg_sys->msg_control - cmsg_ptr,
				  &umsg->msg_controllen);
	}
	if (rc < 0) {
		return -EFAULT;
	}

	if (uaddr != NULL) {
		rc = move_addr_to_u(msg_sys->msg_name, msg_sys->msg_namelen, uaddr, uaddr_len);
		if (rc < 0) {
			return -EFAULT;
		}
	}
	return rc;
}

static inline int copy_msghdr_to_user(struct msghdr *msghdr_sys, struct user_msghdr *umsg, unsigned long cmsg_ptr,
					struct sockaddr_storage __user *uaddr, void *ucmsg)
{
	struct user_msghdr smsg = {
		.msg_name = msghdr_sys->msg_name,
		.msg_namelen = msghdr_sys->msg_namelen,
		.msg_control = cmsg_ptr,
		.msg_controllen = (unsigned long)msghdr_sys->msg_control - cmsg_ptr,
		.msg_flags = msghdr_sys->msg_flags,
	};

	return liblinux_pal_copy_msghdr_to_user(umsg, &smsg, uaddr, ucmsg);
}

static int devhost_recvmsg(struct devhost_socket *socket, void *msg,
			    struct msghdr *msghdr_sys, int flags)
{
	struct compat_msghdr __user *umsg_compat = (struct compat_msghdr __user *)msg;
	struct user_msghdr *umsg = (struct user_msghdr *)msg;
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;
	struct sockaddr_storage addr = {0};
	struct sockaddr_storage __user *uaddr = NULL;
	unsigned char msg_control[64] = {0};
	void *ucmsg = NULL;
	ssize_t rc;
	int len;
	unsigned long cmsg_ptr;

	msghdr_sys->msg_name = &addr;
	if ((rc = get_msg_from_u(msghdr_sys, umsg, umsg_compat, &uaddr, &iov, &flags)) < 0) {
		return rc;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	if (msghdr_sys->msg_controllen && msghdr_sys->msg_controllen < sizeof(msg_control) &&
	    (flags & MSG_CMSG_COMPAT) == 0) {
		msghdr_sys->msg_control_is_user = false;
		ucmsg = msghdr_sys->msg_control;
		msghdr_sys->msg_control = (void *)msg_control;
	}
#endif

	cmsg_ptr = (unsigned long)(msghdr_sys->msg_control);
	msghdr_sys->msg_flags = flags & (MSG_CMSG_CLOEXEC | MSG_CMSG_COMPAT);
	msghdr_sys->msg_namelen = 0;

	if (socket->file_flags & O_NONBLOCK) {
		flags |= MSG_DONTWAIT;
	}
	if ((rc = sock_recvmsg(socket->sock, msghdr_sys, flags)) < 0) {
		goto out_free;
	}
	len = rc;

	if ((flags & MSG_CMSG_COMPAT)) {
		rc = put_msg_to_u(msghdr_sys, umsg, umsg_compat, cmsg_ptr, uaddr,
				  COMPAT_NAMELEN(umsg), flags);
	} else {
		rc = copy_msghdr_to_user(msghdr_sys, umsg, cmsg_ptr, uaddr, ucmsg);
	}

	if (rc < 0) {
		goto out_free;
	}
	if (rc == 0) {
		rc = len;
	}

out_free:
	if (iov != iovstack) {
		kfree(iov);
	}
	return rc;
}

static void net_socket_recvmsg_batch(struct devhost_socket *socket, int flags)
{
	int err;
	struct kvec iv;
	void *data = socket->recv_queue;
	char iov[RECV_QUEUE_IOV_SIZE];
	struct msghdr msg_sys = {0};
	unsigned char msg_control[RECV_QUEUE_CONTROL_SIZE] = {0};
	struct recv_queue_meta *meta = RECV_QUEUE_META_ADDR(data);

	if (!(meta->flags & RECV_QUEUE_FLAG_READ_BIT)) {
		return;
	}

	while (meta->count < RECV_QUEUE_BATCH_CNT) {
		msg_sys.msg_controllen = RECV_QUEUE_CONTROL_SIZE;
		msg_sys.msg_control = RECV_QUEUE_CONTROL_ADDR(meta, meta->tail);
		iv.iov_len = RECV_QUEUE_IOV_SIZE;
		iv.iov_base = RECV_QUEUE_IOV_ADDR(data, meta->tail);
		err = kernel_recvmsg(socket->sock, &msg_sys, &iv, 1, iv.iov_len, flags);
		if (err != RECV_QUEUE_IOV_SIZE) {
			break;
		}

		meta_producer_push(meta);
	}

	if (meta->count > 0) {
		/*
		* tell that next syscall recv from recvbuff in kernel first.
		*/
		meta->flags |= RECV_QUEUE_FLAG_KERNEL_DONE_BIT;
	}
}

static int net_socket_recvmsg_to_recvqueue(struct devhost_socket *socket, void *msg, int flags)
{
	int ret;
	struct msghdr msg_sys = {0};

	ret = devhost_recvmsg(socket, msg, &msg_sys, flags);
	if (ret == RECV_QUEUE_IOV_SIZE) {
		net_socket_recvmsg_batch(socket, flags);
	}

	return ret;
}

static int devhost_net_socket_recvmsg(void *devhost_socket, void *msg, int flags)
{
	struct msghdr msg_sys = {0};
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (flags_init_queue(flags)) {
		socket_init_queue(socket, msg, RECV_QUEUE_FLAG_READ_BIT);
		return 0;
	}

	if (socket->recv_queue != NULL) {
		return net_socket_recvmsg_to_recvqueue(socket, msg, flags);
	}

	return devhost_recvmsg(socket, msg, &msg_sys, flags);
}

static struct timespec64 time_add(const struct timespec64 lts,
				const struct timespec64 rts)
{
	struct timespec64 ts;
	long long sec = lts.tv_sec + rts.tv_sec;
	long nsec = lts.tv_nsec + rts.tv_nsec;

	set_normalized_timespec64(&ts, (timeu64_t)sec, nsec);

	if (unlikely(ts.tv_sec < lts.tv_sec || ts.tv_sec < rts.tv_sec)) {
		ts.tv_sec = TIME64_MAX;
		ts.tv_nsec = 0;
	}

	return ts;
}

static bool time_valid(const struct timespec64 *ts)
{
	if (ts->tv_sec < 0) {
		return false;
	}

	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC) {
		return false;
	}
	return true;
}

static int set_time(struct timespec64 *to, time64_t sec, long nsec)
{
	struct timespec64 ts = {.tv_sec = sec, .tv_nsec = nsec};

	if (!time_valid(&ts)) {
		return -EINVAL;
	}

	if (!sec && !nsec) {
		to->tv_sec = to->tv_nsec = 0;
	} else {
		ktime_get_ts64(to);
		*to = time_add(*to, ts);
	}
	return 0;
}

static int check_recvmmsg_data(int *flags, struct timespec64 end_time,
			       struct timespec64 *timeout, struct timespec64 *timeout64, struct msghdr msg_sys)
{
	int rc = 0;

	if (*flags & MSG_WAITFORONE) {
		*flags |= MSG_DONTWAIT;
	}

	if (timeout) {
		ktime_get_ts64(timeout64);
		*timeout = timespec64_sub(end_time, *timeout64);
		if (timeout->tv_sec < 0) {
			timeout->tv_sec = timeout->tv_nsec = 0;
			rc = -EINVAL;
		}

		if (rc == 0 && timeout->tv_nsec == 0 && timeout->tv_sec == 0) {
			rc = -EINVAL;
		}
	}

	if (rc == 0 && msg_sys.msg_flags & MSG_OOB) {
		rc = -EINVAL;
	}
	cond_resched();
	return rc;
}

static int do_sock_recvmmsg(struct devhost_socket *socket, const void *msgvec,
			    size_t vlen, int flags, struct timespec64 *timeout)
{
	struct mmsghdr __user *entry = (struct mmsghdr __user *)msgvec;
	struct msghdr msg_sys;
	int err, datagrams = 0;
	struct timespec64 timeout64, end_time;

	if (timeout && set_time(&end_time, timeout->tv_sec, timeout->tv_nsec)) {
		return -EINVAL;
	}

	if (likely(!(flags & MSG_ERRQUEUE))) {
		err = sock_error(socket->sock->sk);
		if (err) {
			datagrams = err;
			goto out_put;
		}
	}

	while (datagrams < vlen) {
		err = memset_s(&msg_sys, sizeof(msg_sys), 0, sizeof(msg_sys));
		BUG_ON(err != EOK);
		err = devhost_recvmsg(socket, (void *)entry, &msg_sys, flags & ~MSG_WAITFORONE);
		if (err < 0) {
			break;
		}
		err = put_user(err, &entry->msg_len);
		++entry;

		if (err) {
			break;
		}
		++datagrams;

		if (check_recvmmsg_data(&flags, end_time, timeout, &timeout64, msg_sys) != 0) {
			break;
		}
	}

	if (err == 0) {
		goto out_put;
	}

	if (datagrams == 0) {
		datagrams = err;
		goto out_put;
	}

	if (err != -EAGAIN) {
		socket->sock->sk->sk_err = -err;
	}
out_put:
	return datagrams;
}

static struct timespec32 {
	s32	tv_sec;
	s32	tv_nsec;
};

static int get_time(struct timespec64 *ts, void *uts)
{
	struct timespec64 ts64;
	struct timespec32 compat_kts;
	int ret;

	if (is_compat_client()) {
		ret = copy_from_user(&compat_kts, uts, sizeof(compat_kts));
		if (ret) {
			return -EFAULT;
		}

		ts->tv_sec = compat_kts.tv_sec;
		ts->tv_nsec = compat_kts.tv_nsec;
	} else {
		ret = copy_from_user(&ts64, uts, sizeof(ts64));
		if (ret) {
			return -EFAULT;
		}

		ts->tv_sec = ts64.tv_sec;
		ts->tv_nsec = ts64.tv_nsec;
	}

	return 0;
}

static int put_time(const struct timespec64 *ts, void *uts)
{
	int ret = 0;

	if (is_compat_client()) {
		struct timespec64 kts = {
			.tv_sec = ts->tv_sec,
			.tv_nsec = ts->tv_nsec
		};

		ret = copy_to_user(uts, &kts, sizeof(kts));
		if (ret) {
			return -EFAULT;
		}
	} else {
		struct timespec32 compat_kts = {
			.tv_sec = (s32)(ts->tv_sec),
			.tv_nsec = (s32)(ts->tv_nsec)
		};

		ret = copy_to_user(uts, &compat_kts, sizeof(compat_kts));
		if (ret) {
			return -EFAULT;
		}
	}

	return ret;
}

static int devhost_net_socket_recvmmsg(void *devhost_socket, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct timespec64 timeout_sys;
	int datagrams = 0;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (timeout && get_time(&timeout_sys, timeout)) {
		return -EFAULT;
	}

	if (timeout == NULL) {
		return do_sock_recvmmsg(socket, msgvec, vlen, flags, NULL);
	}

	datagrams = do_sock_recvmmsg(socket, msgvec, vlen, flags, &timeout_sys);

	if (datagrams <= 0) {
		return datagrams;
	}

	if (timeout && put_time(&timeout_sys, timeout)) {
		datagrams = -EFAULT;
	}
	return datagrams;
}

static int devhost_net_socket_readv(void *devhost_socket, const void *iovs, int iovcnt)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct msghdr msg = {0};
	ssize_t rc;
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;
	int flags = 0;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (iovcnt > UIO_MAXIOV) {
		return -EMSGSIZE;
	}

	if ((rc = import_iovec(READ, (void __user *)iovs, iovcnt,
			       UIO_FASTIOV, &iov, &msg.msg_iter)) < 0) {
		return rc;
	}

	msg.msg_iocb = NULL;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = 0;

	if (socket->file_flags & O_NONBLOCK) {
		flags |= MSG_DONTWAIT;
	}
	rc = sock_recvmsg(socket->sock, &msg, flags);

	kfree(iov);
	return rc;
}

static int __devhost_net_socket_writev(void *devhost_socket, const void *iovs, int iovcnt)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct msghdr msghdr_sys = {0};
	ssize_t rc;
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (iovcnt > UIO_MAXIOV) {
		return -EMSGSIZE;
	}

	if ((rc = import_iovec(WRITE, (void __user *)iovs, iovcnt,
			       UIO_FASTIOV, &iov, &msghdr_sys.msg_iter)) < 0) {
		return rc;
	}

	msghdr_sys.msg_iocb = NULL;
	msghdr_sys.msg_name = NULL;
	msghdr_sys.msg_namelen = 0;
	msghdr_sys.msg_control = NULL;
	msghdr_sys.msg_controllen = 0;
	msghdr_sys.msg_flags = 0;

	if (socket->file_flags & O_NONBLOCK) {
		msghdr_sys.msg_flags |= MSG_DONTWAIT;
	}
	rc = sock_sendmsg(socket->sock, &msghdr_sys);

	kfree(iov);
	return rc;
}

static int devhost_net_socket_writev(void *devhost_socket, const void *iovs, int iovcnt, int flags)
{
	if (flags_need_fastcopy(flags)) {
		return devhost_net_socket_sendto(devhost_socket, iovs, (size_t)iovcnt, flags, NULL, 0);
	}
	return __devhost_net_socket_writev(devhost_socket, iovs, iovcnt);
}

static int devhost_net_socket_getsockopt(void *devhost_socket, int level, int optname, void *optval,
					 unsigned *optlen)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	int rc;

	if (socket == NULL || socket->sock == NULL || socket->sock->ops == NULL) {
		return -EINVAL;
	}

	if (level == SOL_SOCKET) {
		rc = sock_getsockopt(socket->sock, level, optname, optval, optlen);
	} else if (unlikely(!socket->sock->ops->getsockopt)) {
		rc = -EOPNOTSUPP;
	} else {
		rc = socket->sock->ops->getsockopt(socket->sock, level, optname, optval, optlen);
	}

	return rc;
}

static int devhost_net_socket_setsockopt(void *devhost_socket, int level, int optname, const void *optval,
					 unsigned optlen)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	int rc;

	if (socket == NULL || socket->sock == NULL || socket->sock->ops == NULL) {
		return -EINVAL;
	}

	if (level == SOL_SOCKET) {
		rc = sock_setsockopt(socket->sock, level, optname, USER_SOCKPTR((void *)optval), optlen);
	} else if (unlikely(!socket->sock->ops->setsockopt)) {
		rc = -EOPNOTSUPP;
	} else {
		rc = socket->sock->ops->setsockopt(socket->sock, level, optname, USER_SOCKPTR((void *)optval), optlen);
	}

	return rc;
}

static int devhost_net_socket_getsockname(void *devhost_socket, void *addr, unsigned *addrlen)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct sockaddr_storage address;
	int len, rc;

	if (socket == NULL) {
		return -EINVAL;
	}
	len = sock_getname(socket, socket->sock, address, 0);
	if (len < 0) {
		return len;
	}
	rc = move_addr_to_u(&address, len, addr, addrlen);

	return rc;
}

static int devhost_net_socket_getpeername(void *devhost_socket, void *addr,
					  unsigned *addrlen)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	struct sockaddr_storage address;
	int len, rc;

	if (socket == NULL) {
		return -EINVAL;
	}

	len = sock_getname(socket, socket->sock, address, 1);
	if (len < 0) {
		return len;
	}
	rc = move_addr_to_u(&address, len, addr, addrlen);

	return rc;
}

static int devhost_net_socket_fcntl(void *devhost_socket, int cmd, int arg)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	int rc = 0;
	int setfl_mask = O_APPEND | O_NONBLOCK | O_NDELAY | O_DIRECT | O_NOATIME;

	if (socket == NULL) {
		return -EINVAL;
	}

	if (cmd == F_GETFL) {
		rc = socket->file_flags;
	} else if (cmd == F_SETFL) {
		socket->file_flags = (arg & setfl_mask) | (socket->file_flags & ~setfl_mask);
	} else {
		pr_warn("%s: unhandled cmd=%d, arg=0x%x\n", __func__, cmd, arg);
		rc = -EINVAL;
	}

	return rc;
}

static int devhost_net_socket_fchown(void *devhost_socket, unsigned int uid, unsigned int gid)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;
	kuid_t u = {0};

	if (socket == NULL) {
		return -EINVAL;
	}

	/* we only change sock->sk->sk_uid, do not change inode->i_uid, inode->i_gid */
	if ((uid_t)uid != (uid_t)-1) {
		u.val = (uid_t)uid;
		socket->sock->sk->sk_uid = u;
	}

	return 0;
}

static int devhost_net_socket_fstat(void *devhost_socket, uid_t *uid)
{
	struct devhost_socket *socket = (struct devhost_socket *)devhost_socket;

	if (socket == NULL) {
		return -EINVAL;
	}

	*uid = socket->sock->sk->sk_uid.val;
	return 0;
}

static void *devhost_net_socket_fget(void *devhost_socket)
{
	struct file *f = NULL;

	f = ((struct devhost_socket *)devhost_socket)->file;
	/*
	 * Corresponds to the fput operation.
	 * Therefore, 'override fput' is not required.
	 */
	get_file(f);

	return f;
}

static void devhost_netcall_enter(int pid, int tid, int vpid)
{
	struct task_struct *p = current;
	set_fs(USER_DS);
	p->pid = tid;
	p->tgid = pid;
	task_thread_info(p)->vtgid = vpid;
	clear_tsk_thread_flag(p, TIF_SIGPENDING);
}

#ifdef CONFIG_BPF_SYSCALL
extern int __liblinux_sys_bpf(int cmd, void *uattr, unsigned int size);
#endif
static int devhost_net_socket_bpf(int cmd, void *attr, unsigned size)
{
#ifdef CONFIG_BPF_SYSCALL
	return __liblinux_sys_bpf(cmd, attr, size);
#else
	return -ENOSYS;
#endif
}

static void *devhost_pal_valloc(unsigned int size)
{
	return kvmalloc(size, GFP_KERNEL);
}

static void devhost_pal_vfree(void *ptr)
{
	kvfree(ptr);
}

static struct devhost_net_socket_ops g_net_socket_ops __read_mostly = {
	.close = devhost_net_socket_close,
	.connect = devhost_net_socket_connect,
	.bind = devhost_net_socket_bind,
	.listen = devhost_net_socket_listen,
	.accept = devhost_net_socket_accept,
	.shutdown = devhost_net_socket_shutdown,
	.ioctl = devhost_net_socket_ioctl,
	.fcntl = devhost_net_socket_fcntl,
	.send = devhost_net_socket_send,
	.recv = devhost_net_socket_recv,
	.sendmmsg = devhost_net_socket_sendmmsg,
	.recvmmsg = devhost_net_socket_recvmmsg,
	.sendmsg = devhost_net_socket_sendmsg,
	.recvmsg = devhost_net_socket_recvmsg,
	.sendto = devhost_net_socket_sendto,
	.recvfrom = devhost_net_socket_recvfrom,
	.readv = devhost_net_socket_readv,
	.writev = devhost_net_socket_writev,
	.getsockopt = devhost_net_socket_getsockopt,
	.setsockopt = devhost_net_socket_setsockopt,
	.getsockname = devhost_net_socket_getsockname,
	.getpeername = devhost_net_socket_getpeername,
	.fchown = devhost_net_socket_fchown,
	.fstat = devhost_net_socket_fstat,
	.fget = devhost_net_socket_fget,
	.socket = devhost_net_socket_socket,
};

static struct devhost_net_handle_ops g_net_handle_ops __read_mostly = {
	.netcall_enter = devhost_netcall_enter,
};

static struct devhost_net_ldk_ops g_ldkops __read_mostly = {
	.bpf = devhost_net_socket_bpf,
	.dev_ioctl = devhost_net_socket_dev_ioctl,
	.valloc = devhost_pal_valloc,
	.vfree = devhost_pal_vfree,
};

static int __init devhost_net_socket_init(void)
{
	int rc;

	/* register socket poll ops */
	rc = devhost_net_socket_devinfo_init(&net_socket_fops, &net_socket_ex_fops,
			    &socket_devinfo, &g_net_socket_ops, &g_net_handle_ops,
			    &g_ldkops);
	if (rc < 0) {
		pr_err("socket_devinfo init failed, err=%d\n", rc);
	}

	devhost_net_namespace_init();

	return rc;
}

module_init(devhost_net_socket_init);
