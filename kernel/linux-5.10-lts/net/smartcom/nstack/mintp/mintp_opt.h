/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Author: songqiubin
 *
 * MinTP socket option
 */

#ifndef __MINTP_OPT_H__
#define __MINTP_OPT_H__

#include <net/sock.h>

#define MTP_TOS 1
#define MTP_KEEPIDLE 2
#define MTP_MAX_MSG_SIZE 3
#define MTP_D2D 4
#define MTP_STATS 5
#define MTP_CPU_ID 6
#define MTP_QDISC 7
#define MTP_CONG_CONFIG 8
#define MTP_SENDPAGE_MSG_SIZE 9

int mtp_setsockopt(struct sock *sk, int level, int optname, sockptr_t optval, unsigned int optlen);
int mtp_getsockopt(struct sock *sk, int level, int optname, char __user *optval, int __user *optlen);

#endif /* __MINTP_OPT_H__ */
