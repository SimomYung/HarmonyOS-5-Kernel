/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network sockopt
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_UX_SOCKOPT_H
#define __LIBUXNET_UX_SOCKOPT_H

#include "ux_socket.h"

#ifndef CONFIG_VISP
#define UX_MAX_SOCKOPT_LEN	256
#else
#define UX_MAX_SOCKOPT_LEN	65536
#endif

/*
 * In the unix send process, the data to be sent consists of ux_unix_buf_t and the data
 * of user sent. Therefore, the value of sndbuf must be greater than the size of
 * ux_unix_buf_t. Otherwise, the size of the data to be sent is incorrectly calculated.
 */
#define UX_MAX_SOCK_STRUCT_SIZE 2048
#define UX_PKT_MIN_TRUESIZE (2048 + UX_MAX_SOCK_STRUCT_SIZE)
#define UX_SOCK_MIN_SNDBUF	(UX_MAX_SOCK_STRUCT_SIZE * 2)
#define UX_SOCK_MIN_RCVBUF	UX_MAX_SOCK_STRUCT_SIZE
#define UX_SOCK_MAX_SNDBUF	(UX_SOCK_MIN_SNDBUF * 256)
#define UX_SOCK_MAX_RCVBUF	(UX_SOCK_MIN_RCVBUF * 256)

#define UX_SOCK_DEFAULT_SNDBUF	(UX_SOCK_MIN_SNDBUF * 128)
#define UX_SOCK_DEFAULT_RCVBUF	(UX_SOCK_MIN_RCVBUF * 128)

#define SOCK_BUF_FIXUP		2

/* To avoid repetition, the number of cookies created by the vfs process is an odd number,
 * and the number of cookies created by the devhost process is an even number.
 */
#define HMNETD_SOCKET_COOKIE_STEP 2
#ifdef CONFIG_NETFS_FUSION
#define HMNETD_SOCKET_MIN_COOKIE_NUMBER 1
#define HMNETD_SOCKET_MAX_COOKIE_NUMBER ULLONG_MAX
#else
#define HMNETD_SOCKET_MIN_COOKIE_NUMBER 2
#define HMNETD_SOCKET_MAX_COOKIE_NUMBER (ULLONG_MAX - 1)
#endif

int ux_common_getsockopt(ux_socket_t *sock, int level, int optname,
			 void *optval, socklen_t *optlen);

int ux_common_setsockopt(ux_socket_t *sock, int level, int optname,
			 const void *optval, socklen_t optlen);

unsigned long long generate_hmnetd_cookie(unsigned long long cookie);
#endif
