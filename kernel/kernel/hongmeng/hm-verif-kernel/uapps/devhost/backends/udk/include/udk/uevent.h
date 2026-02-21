/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: This is the header of udk uevent
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 05 10:12:53 2023
 */
#ifndef __UDK_UEVENT_H__
#define __UDK_UEVENT_H__

#include <udk/types.h>

#define UEVENT_ACTION_SIZE	(64)

#define UEVENT_READ_FORMAT "MAJOR=%u\nMINOR=%u\nDEVNAME=%s\n"
#define UEVENT_WRITE_FORMAT "ACTION=%s%cDEVPATH=%s/%s%cSUBSYSTEM=%s%cMAJOR=%u%cMINOR=%u%cDEVNAME=%s%c"

enum {
	UDK_UEVENT_ENV_ACTION,
	UDK_UEVENT_ENV_DEVPATH,
	UDK_UEVENT_ENV_BUF,
	UDK_UEVENT_ENV_TYPE_MAX
};
struct udk_uevent_env;
struct udk_uevent_env *udk_uevent_env_alloc(void);
void udk_uevent_env_free(struct udk_uevent_env *env);
int udk_uevent_env_buf_read(struct udk_uevent_env *env,
			    void *dst, size_t pos, size_t size, size_t *rsize);
int udk_uevent_env_add(struct udk_uevent_env *env, int type, const char *format, ...);
int udk_uevent_net_broadcast(struct udk_uevent_env *env);

#endif /* __UDK_UEVENT_H__ */
