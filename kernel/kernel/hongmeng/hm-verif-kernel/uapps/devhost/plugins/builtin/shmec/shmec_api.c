/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Exported shmec APIs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 11 17:55:33 2023
 */

#include "shmec.h"
#include <hongmeng/errno.h>
#include <devhost/plugin.h>
#include <libhmsrv_sys/hm_shmec.h>

PUBLIC_SYMBOL
struct shmec_ep *shmec_create_endpoint(uint64_t flags)
{
	return hm_shmec_create_endpoint(flags);
}

PUBLIC_SYMBOL
int shmec_delete_endpoint(struct shmec_ep *endpoint)
{
	int err = hm_shmec_delete_endpoint(endpoint);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

PUBLIC_SYMBOL
int shmec_attach(const struct shmec_ep *endpoint, const char *chn, int flag, mode_t mode)
{
	int err = hm_shmec_attach(endpoint, chn, flag, mode);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

PUBLIC_SYMBOL
int shmec_detach(const struct shmec_ep *endpoint)
{
	int err = hm_shmec_detach(endpoint);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

PUBLIC_SYMBOL
int shmec_send_event(const struct shmec_ep *endpoint, const void *buf, size_t size)
{
	int err = hm_shmec_send_event(endpoint, buf, size);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

PUBLIC_SYMBOL
int shmec_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	int err = hm_shmec_recv_event(endpoint, buf, size);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

PUBLIC_SYMBOL
int shmec_try_recv_event(struct shmec_ep *endpoint, void *buf, size_t size)
{
	int err = hm_shmec_try_recv_event(endpoint, buf, size);
	return (err < 0) ? -hmerrno2posix(err) : err;
}

/* there is no extension */
DEFINE_BUILTIN_PLUGIN(shmec, NULL);
