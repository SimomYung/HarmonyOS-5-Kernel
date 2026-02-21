/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: function definition for base hash operation
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef AUTH_PATH_IMPL_H
#define AUTH_PATH_IMPL_H

#include <linux/types.h>

#ifndef SELINUX_CA_HIDL_LABEL
#define SELINUX_CA_HIDL_LABEL ""
#endif

#ifndef SELINUX_TEECD_LABEL
#define SELINUX_TEECD_LABEL ""
#endif

#ifndef SELINUX_CONTAINER_TEECD_LABEL
#define SELINUX_CONTAINER_TEECD_LABEL ""
#endif

#ifndef SELINUX_TEE_TELEPORT_LABEL
#define SELINUX_TEE_TELEPORT_LABEL ""
#endif

#ifndef SELINUX_TEE_AGENTD_LABEL
#define SELINUX_TEE_AGENTD_LABEL ""
#endif

#ifndef CA_HIDL_PATH_UID_AUTH_CTX
#define CA_HIDL_PATH_UID_AUTH_CTX ""
#endif

#ifndef TEECD_PATH_UID_AUTH_CTX
#define TEECD_PATH_UID_AUTH_CTX ""
#endif

#ifndef CONTAINER_TEECD_PATH_UID_AUTH_CTX
#define CONTAINER_TEECD_PATH_UID_AUTH_CTX ""
#endif

#ifndef CADAEMON_PATH_UID_AUTH_CTX
#define CADAEMON_PATH_UID_AUTH_CTX ""
#endif

#if ((defined CONFIG_CLIENT_AUTH) || (defined CONFIG_MDC_HAL_AUTH))
int check_hidl_auth(void);
#endif

#define CHECK_ACCESS_SUCC      0
#define CHECK_ACCESS_FAIL      0xffff
#define CHECK_PATH_HASH_FAIL   0xff01
#define CHECK_SECLABEL_FAIL    0xff02
#define CHECK_CODE_HASH_FAIL   0xff03
#define ENTER_BYPASS_CHANNEL   0xff04

#define MAX_PATH_SIZE          512
#define MAX_SCTX_LEN           128

#ifdef CONFIG_TEECD_AUTH
int check_teecd_auth(void);
int check_container_teecd_auth(void);
#else
static inline int check_teecd_auth(void) { return 0; }
static inline int check_container_teecd_auth(void) { return 0; }
#endif

#if defined(CONFIG_CLIENT_HIDL) || defined(CONFIG_MDC_HAL_AUTH) || defined(CONFIG_CADAEMON_AUTH)
bool is_hidl_or_cadaemon(void);
#endif

#ifdef CONFIG_CADAEMON_AUTH
int check_cadaemon_auth(void);
#endif

#ifdef CONFIG_TEE_TELEPORT_AUTH
int check_tee_teleport_auth(void);
#endif

#ifdef CONFIG_TEE_AGENTD_AUTH
int check_tee_agentd_auth(void);
#endif

#if defined(CONFIG_SELINUX_AUTH_ENBALE) && defined(CONFIG_SECURITY_SELINUX)
int check_proc_selinux_access(const char *s_ctx);
#else
static inline int check_proc_selinux_access(const char *s_ctx)
{
    (void)s_ctx;
    return 0;
}
#endif

#endif
