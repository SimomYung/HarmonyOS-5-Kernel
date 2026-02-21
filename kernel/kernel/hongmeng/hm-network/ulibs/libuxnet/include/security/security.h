/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The security interface for network
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 14:57:23 2019
 */

#ifndef __HMNETD_SECURITY_SECURITY_H__
#define __HMNETD_SECURITY_SECURITY_H__

#include <sys/socket.h>

int hmnetd_security_init(void);
int check_port_policy(const struct sockaddr_storage *sockaddr);

#endif
