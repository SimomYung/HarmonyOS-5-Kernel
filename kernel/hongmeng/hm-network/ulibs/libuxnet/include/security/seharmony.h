/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The security template of seharmony
 * Author: Huawei OS Kernel Lab
 * Create: Thurs Mar 09 17:55:34 2023
 */

#ifndef __HMNETD_SECURITY_SEHARMONY_H__
#define __HMNETD_SECURITY_SEHARMONY_H__

#include <libsec/chk.h>

#include <ux_socket.h>
#include <libsec/component.h>

#ifdef CONFIG_SEHARMONY
extern struct sec_comp sec_comp_seharmony_net;

int sec_chk_sel_socket_perms(const struct sec_chk_cred *cred,
			     ux_socket_t *sock);
#endif

#endif
