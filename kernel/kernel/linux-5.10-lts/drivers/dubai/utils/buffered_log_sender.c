/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
 * Description: dubai buffered log sender
 */

#include "buffered_log_sender.h"

#include <linux/net.h>
#ifdef CONFIG_LIBLINUX
#include <linux/socket.h>
#endif /* CONFIG_LIBLINUX */
#include <linux/un.h>
#include <linux/vmalloc.h>
#include <securec.h>

#include "dubai_utils.h"

#include <securec.h>
#define DUBAID_SOCKET_NAME			"DUBAI_BUFFERED_LOG"
#define BUFFERED_LOG_TYPE			"dubai_driver"
#define MAX_DATA_TYPE_LENGTH		32
// limit log under 50MB
#define MAX_LOG_LENGTH				52428800

static DEFINE_MUTEX(sock_lock);

#ifndef CONFIG_LIBLINUX
static struct socket *dubaid_sock = NULL;
#endif /* CONFIG_LIBLINUX */

static int do_send_buffered_log(const struct buffered_log_entry *entry);
static int send_buffered_log_init(const struct buffered_log_entry *entry);
int (*send_buffered_log)(const struct buffered_log_entry *entry) = send_buffered_log_init;

#ifdef CONFIG_LIBLINUX
struct pal_unix_socket {
	int fd;
};
static struct pal_unix_socket dubaid_sock_s = {-1};
static struct pal_unix_socket *dubaid_sock = NULL;
extern int liblinux_pal_socket_create(int family, int type, int protocol);
extern int liblinux_pal_socket_connect(int sockfd, const struct sockaddr *addr, int addrlen);
extern ssize_t liblinux_pal_socket_sendmsg(int sockfd, const struct user_msghdr *msg, int flags);
extern int liblinux_pal_socket_close(int sockfd);

static int sock_create_pal_unix(int family, int type, int protocol, struct pal_unix_socket **sock)
{
	int fd;

	fd = liblinux_pal_socket_create(family, type, protocol);
	if (fd < 0) {
		pr_err("liblinux_pal_socket_create error\n");
		return -1;
	}
	dubaid_sock_s.fd = fd;
	*sock = &dubaid_sock_s;

	return 0;
}

static int sock_connect_pal_unix(struct pal_unix_socket *sock, const struct sockaddr *addr, int addrlen)
{
	int ret;

	if (sock == NULL || sock->fd < 0) {
		return -1;
	}
	ret = liblinux_pal_socket_connect(sock->fd, addr, addrlen);
	if (ret < 0) {
		pr_err("liblinux_pal_socket_connect error\n");
		return -1;
	}

	return 0;
}

static int sock_release_pal_unix(struct pal_unix_socket *sock)
{
	int ret;

	if (sock == NULL || sock->fd < 0) {
		return -1;
	}
	ret = liblinux_pal_socket_close(sock->fd);
	if (ret < 0) {
		pr_err("liblinux_pal_socket_close error\n");
		return -1;
	}
	sock->fd = -1;

	return 0;
}

static int sock_write_to_daemon(const struct buffered_log_entry *entry)
{
	struct user_msghdr msg;
	struct iovec iov[3];

	if (dubaid_sock == NULL || entry == NULL) {
		dubai_err("Invalid parameter");
		return -EINVAL;
	}
	if (unlikely(!dubai_is_valid_length(entry->length))) {
		dubai_err("Invalid log entry length: %d, magic: %d", entry->length, entry->magic);
		return -EINVAL;
	}
	iov[0].iov_base = (void *) &(entry->length);
	iov[0].iov_len = sizeof(entry->length);
	iov[1].iov_base = (void *) &(entry->magic);
	iov[1].iov_len = sizeof(entry->magic);
	iov[2].iov_base = (void *) entry->data;
	iov[2].iov_len = (size_t) entry->length;
	memset_s(&msg, sizeof(msg), 0, sizeof(msg));
	msg.msg_iov = iov;
	msg.msg_iovlen = 3;
	return liblinux_pal_socket_sendmsg(dubaid_sock->fd, &msg, MSG_DONTWAIT | MSG_NOSIGNAL);
}
#else
static int write_to_daemon(const struct buffered_log_entry *entry)
{
	size_t payload_size = 0;
	struct msghdr msg;
	struct kvec vec[3];

	if (dubaid_sock == NULL || entry == NULL) {
		dubai_err("Invalid parameter");
		return -EINVAL;
	}
	if (unlikely(!dubai_is_valid_length(entry->length))) {
		dubai_err("Invalid log entry length: %d, magic: %d", entry->length, entry->magic);
		return -EINVAL;
	}

	vec[0].iov_base = (void *) &(entry->length);
	vec[0].iov_len = sizeof(entry->length);
	payload_size += vec[0].iov_len;
	vec[1].iov_base = (void *) &(entry->magic);
	vec[1].iov_len = sizeof(entry->magic);
	payload_size += vec[1].iov_len;
	vec[2].iov_base = (void *) entry->data;
	vec[2].iov_len = (size_t) entry->length;
	payload_size += vec[2].iov_len;
	memset_s(&msg, sizeof(msg), 0, sizeof(msg));

	return kernel_sendmsg(dubaid_sock, &msg, vec, 3, payload_size);
}
#endif /* CONFIG_LIBLINUX */

// Send data type to dubaid socket server to authenticate this socket
static int authenticate_data_type(int magic)
{
	char buf[MAX_DATA_TYPE_LENGTH] = {0};
	struct buffered_log_entry *entry = NULL;

	entry = (struct buffered_log_entry *) buf;
	entry->length = strlen(BUFFERED_LOG_TYPE);
	entry->magic = magic;
	memcpy_s(entry->data, entry->length, BUFFERED_LOG_TYPE, entry->length);

#ifdef CONFIG_LIBLINUX
	return sock_write_to_daemon(entry);
#else
	return write_to_daemon(entry);
#endif /* CONFIG_LIBLINUX */
}

static int make_sockaddr_un(const char *name, struct sockaddr_un *un, int *alen)
{
	size_t name_len;

	memset_s(un, sizeof(*un), 0, sizeof(*un));
	name_len = strlen(name);
	if ((name_len + 1) > sizeof(un->sun_path)) {
		dubai_err("Invalid name length");
		return -EINVAL;
	}
	// The path in this case is not supposed to be '\0'-terminated
	un->sun_path[0] = 0;
	memcpy_s(un->sun_path + 1, name_len, name, name_len);
	un->sun_family = AF_LOCAL;
	*alen = name_len + offsetof(struct sockaddr_un, sun_path) + 1;

	return 0;
}

/*
 * Try to connect dubaid local socket server and authenticate data type
 * Connect success if both operations were done
 */
static int connect_dubaid_server(int magic)
{
	int ret;
	int alen = 0;
	struct sockaddr_un un;

	if (dubaid_sock != NULL)
		return 0;
#ifdef CONFIG_LIBLINUX
	ret = sock_create_pal_unix(AF_LOCAL, SOCK_STREAM, 0, &dubaid_sock);
#else
	ret = sock_create(AF_LOCAL, SOCK_STREAM, 0, &dubaid_sock);
#endif /* CONFIG_LIBLINUX */
	if (ret < 0 || dubaid_sock == NULL) {
		dubai_err("Failed to create socket");
		goto error;
	}
	ret = make_sockaddr_un(DUBAID_SOCKET_NAME, &un, &alen);
	if (ret < 0) {
		dubai_err("Failed to make socket address");
		goto error;
	}
#ifdef CONFIG_LIBLINUX
	ret = sock_connect_pal_unix(dubaid_sock, (struct sockaddr *)&un, alen);
#else
	ret = kernel_connect(dubaid_sock, (struct sockaddr *)&un, alen, O_CLOEXEC);
#endif /* CONFIG_LIBLINUX */
	if (ret < 0) {
		dubai_err("Failed to connect socket");
		goto error;
	}
	ret = authenticate_data_type(magic);
	if (ret < 0) {
		dubai_err("Failed to authenticate data type");
		goto error;
	}
	dubai_info("Succeed to connect dubai local socket server");

	return 0;

error:
	if (dubaid_sock != NULL) {
#ifdef CONFIG_LIBLINUX
		sock_release_pal_unix(dubaid_sock);
#else
		sock_release(dubaid_sock);
#endif /* CONFIG_LIBLINUX */
		dubaid_sock = NULL;
	}

	return -ECONNREFUSED;
}

// Initilize socket to connect dubaid local socket server
static int send_buffered_log_init(const struct buffered_log_entry *entry)
{
	int ret;

	if (entry == NULL) {
		dubai_err("Invalid parameter");
		return -EINVAL;
	}

	mutex_lock(&sock_lock);
	if (send_buffered_log == send_buffered_log_init) {
		// initialize dubaid socket for the first time
		ret = connect_dubaid_server(entry->magic);
		if (ret < 0) {
			// Retry to connect to dubaid local socket server
			ret = connect_dubaid_server(entry->magic);
			if (ret < 0) {
				mutex_unlock(&sock_lock);
				return -ECONNREFUSED;
			}
		}
		send_buffered_log = do_send_buffered_log;
	}
	mutex_unlock(&sock_lock);

	return do_send_buffered_log(entry);
}

static int do_send_buffered_log(const struct buffered_log_entry *entry)
{
	int ret;

	if (entry == NULL) {
		dubai_err("Invalid parameter");
		return -EINVAL;
	}

	mutex_lock(&sock_lock);
#ifdef CONFIG_LIBLINUX
	ret = sock_write_to_daemon(entry);
#else
	ret = write_to_daemon(entry);
#endif /* CONFIG_LIBLINUX */
	if (ret < 0) {
		if (dubaid_sock != NULL) {
#ifdef CONFIG_LIBLINUX
			sock_release_pal_unix(dubaid_sock);
#else
			sock_release(dubaid_sock);
#endif /* CONFIG_LIBLINUX */
			dubaid_sock = NULL;
		} else {
			dubai_err("Unexpectedly! dubaid_sock is null!");
		}
		dubai_err("Failed to write to daemon: %d", ret);
		send_buffered_log = send_buffered_log_init;
	}
	mutex_unlock(&sock_lock);

	return ret;
}

// Warnning: Using vmalloc to allocate memory, so this fuction might sleep
struct buffered_log_entry *create_buffered_log_entry(long long size, int magic)
{
	unsigned long entry_size;
	struct buffered_log_entry *entry = NULL;

	might_sleep();

	if (!dubai_is_valid_length(size)) {
		dubai_err("Invalid log size: %lld, magic: %d", size, magic);
		return NULL;
	}

	entry_size = size + sizeof(struct buffered_log_entry);
	entry = vmalloc(entry_size);
	if (entry == NULL) {
		dubai_err("Failed to allocate memory");
		return NULL;
	}
	memset_s(entry, entry_size, 0, entry_size);
	entry->length = size;
	entry->magic = magic;

	return entry;
}

void free_buffered_log_entry(struct buffered_log_entry *entry)
{
	if (entry)
		vfree(entry);
}

void buffered_log_release(void)
{
	mutex_lock(&sock_lock);
	if (dubaid_sock != NULL)
#ifdef CONFIG_LIBLINUX
		sock_release_pal_unix(dubaid_sock);
#else
		sock_release(dubaid_sock);
#endif /* CONFIG_LIBLINUX */
	mutex_unlock(&sock_lock);
}

int dubai_is_valid_length(long long len)
{
	return (((len) >= 0) && ((len) <= MAX_LOG_LENGTH));
}