/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 11:22:46 2019
 */

#include <libsysif_net/api.h>
#include <libhmsrv_io/net.h>
#include <libmem/utils.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>

#include <fcntl.h>
#include <hmasm/lsyscall.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>

#include "hm_socket.h"
#include "ss_channel.h"
#include "ux_event.h"

/*
 * hm_net_xx and lsyscall_xx errno is different
 */
int hm_net_fcntl(int sockfd, int cmd, int val)
{
	return lsyscall_socket_fcntl((long)sockfd, (long)cmd, (unsigned long)(long)val, sockfd);
}
