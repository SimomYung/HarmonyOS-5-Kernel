/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The security interface of reserved port range
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 18 13:17:19 2020
 */

#include <hongmeng/errno.h>
#include "security/net_bind_service.h"
#include "security/net_admin.h"
#include "security/raw_socket.h"
#include "security/posix_cap.h"

#include <generated/hmsd/hook/net/net_admin.h>
#include <generated/hmsd/hook/net/net_bind_service.h>
#include <generated/hmsd/hook/net/raw_socket.h>
#include <generated/hmsd/hook/net/seharmony.h>

#ifdef CONFIG_POSIX_CAPS
SEC_DEFINE_POSIX_CAP_DEFAULT(net_bind_service, NO_ARGS(), POSIX_CAP_NET_BIND_SERVICE)

SEC_DEFINE_POSIX_CAP_DEFAULT(net_admin, NO_ARGS(), POSIX_CAP_NET_ADMIN)

SEC_DEFINE_POSIX_CAP_DEFAULT(raw_socket, NO_ARGS(), POSIX_CAP_NET_RAW)

SEC_DEFINE_POSIX_CAP_DEFAULT(socket_fchown, ARGS(struct ux_socket *, sock), POSIX_CAP_CHOWN)

SEC_DEFINE_COMPONENT(net, posix_cap,
		     sec_define_posix_cap_method(net_bind_service),
		     sec_define_posix_cap_method(net_admin),
		     sec_define_posix_cap_method(raw_socket),
		     sec_define_posix_cap_method(socket_fchown)
		    );
#define ENABLE_SEC_MOD_CAPABILITY
#include <generated/hmsd/module/net/net_admin.h>
#include <generated/hmsd/module/net/net_bind_service.h>
#include <generated/hmsd/module/net/raw_socket.h>
#include <generated/hmsd/module/net/seharmony.h>
#endif
