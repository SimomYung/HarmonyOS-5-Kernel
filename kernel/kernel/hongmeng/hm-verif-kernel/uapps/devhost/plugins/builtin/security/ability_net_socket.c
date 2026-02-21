/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The ability check interface of net
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 14:23:19 2022
 */

#include <hongmeng/errno.h>

#include "net_socket.h"
#include "ability.h"

static int sec_chk_ability_net_socket(struct sec_chk_cred_xact *cred_xact)
{
	const struct sec_chk_cred *src_cred = NULL;
	int rc = E_HM_OK;

	/* get cred of xact src */
	src_cred = sec_chk_src_cred(cred_xact);
	if (src_cred == NULL) {
		rc = E_HM_SRCH;
	}

	if (rc >= 0) {
		rc = sec_check_ability(src_cred, mac_cred_of_sec_cred(src_cred),
					ab_devhost_net_socket,
					sec_chk_show_obj_name, NULL);
	}
	return rc;
}

SEC_DEFINE_COMPONENT(net_socket, ability,
	sec_define_chk_xact_method(net_socket_register, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_close, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_connect, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_bind, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_listen, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_accept, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_shutdown, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_ioctl, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_dev_ioctl, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_fcntl, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_send, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_recv, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_sendmmsg, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_recvmmsg, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_sendmsg, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_recvmsg, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_sendto, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_recvfrom, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_readv, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_writev, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_getsockopt, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_setsockopt, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_getsockname, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_getpeername, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_fchown, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_fstat, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_socket, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_alloc_new_ns, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_destroy_ns, sec_chk_ability_net_socket),
	sec_define_chk_xact_method(net_socket_bpf, sec_chk_ability_net_socket)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/net_socket.h>
