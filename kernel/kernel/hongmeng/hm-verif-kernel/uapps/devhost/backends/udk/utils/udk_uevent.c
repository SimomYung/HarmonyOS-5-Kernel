/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: udk uevent
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 04 15:22:41 2023
 */

#include <stdarg.h>

#include <udk/mm.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/uevent.h>
#include <hmnet/hm_socket.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

struct udk_uevent_env {
	struct uevent_msg umsg;
};

struct udk_uevent_env *udk_uevent_env_alloc(void)
{
	struct udk_uevent_env *env = udk_malloc(sizeof(struct udk_uevent_env));

	if (env == NULL) {
		return NULL;
	}
	mem_zero_s(*env);

	return env;
}

void udk_uevent_env_free(struct udk_uevent_env *env)
{
	udk_free(env);
}

int udk_uevent_env_buf_read(struct udk_uevent_env *env,
			    void *dst, size_t pos, size_t size, size_t *rsize)
{
	int ret;
	size_t len;
	struct uevent_msg *umsg = NULL;

	if ((env == NULL) || (dst == NULL) || (rsize == NULL)) {
		return -EINVAL;
	}

	umsg = &(env->umsg);
	len = (umsg->len < size ? umsg->len : size);
	if (pos >= len) {
		*rsize = 0;
		return 0;
	}

	ret = udk_copy_to_user(dst, umsg->buf + pos, len - pos);
	if (ret < 0) {
		udk_warn("udk copy to user len = %zu failed, %s\n", len, strerror(-ret));
		return ret;
	}
	*rsize = len - pos;

	return (int)len;
}

int udk_uevent_env_add(struct udk_uevent_env *env, int type, const char *format, ...)
{
	int len = -1;
	va_list vargs;
	size_t remain = 0;
	struct uevent_msg *umsg = NULL;

	if ((env == NULL) || (format == NULL) || (type >= UDK_UEVENT_ENV_TYPE_MAX)) {
		return -EINVAL;
	}

	umsg = &(env->umsg);
	va_start(vargs, format);
	switch (type) {
	case UDK_UEVENT_ENV_ACTION:
		remain = sizeof(umsg->action);
		len = vsnprintf_s(umsg->action, remain, remain - 1, format, vargs);
		break;
	case UDK_UEVENT_ENV_DEVPATH:
		remain = sizeof(umsg->devpath);
		len = vsnprintf_s(umsg->devpath, remain, remain - 1, format, vargs);
		break;
	case UDK_UEVENT_ENV_BUF:
		remain = sizeof(umsg->buf) - umsg->len;
		len = vsnprintf_s(&(umsg->buf[umsg->len]), remain, remain - 1, format, vargs);
		if (len < (int)remain) {
			umsg->len += (size_t)len;
		}
		break;
	default:
		udk_warn("unsupport udk uevent type: %d\n", type);
		break;
	}
	va_end(vargs);

	if (len >= (int)remain) {
		udk_warn("uevent add overflowed, type = %d, remain = %zu\n", type, remain);
		return -ENOMEM;
	}

	return 0;
}

int udk_uevent_net_broadcast(struct udk_uevent_env *env)
{
	if (env == NULL) {
		return -EINVAL;
	}

	return -hmerrno2posix(hm_net_dev_ioctl(SIOCUEVENTBROAD, (void *)(&(env->umsg))));
}
