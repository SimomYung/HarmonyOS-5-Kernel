/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes for shmec APIs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 11 17:57:33 2023
 */
#ifndef __PLUGIN_API_SHMEC_H__
#define __PLUGIN_API_SHMEC_H__

#if defined(__KERNEL__) /* linux kernel build env */
#include <linux/types.h>
#else /* generic build env */
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#endif

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

#define SHMEC_SRC    (0x1ULL)
#define SHMEC_DST    (0x2ULL)
#define SHMEC_CHN_CREAT    (0100)

struct shmec_ep;
struct shmec_ep *shmec_create_endpoint(uint64_t flags);
int shmec_delete_endpoint(struct shmec_ep *endpoint);
int shmec_attach(const struct shmec_ep *endpoint, const char *chn, int flag, mode_t mode);
int shmec_detach(const struct shmec_ep *endpoint);
int shmec_send_event(const struct shmec_ep *endpoint, const void *buf, size_t size);
int shmec_recv_event(struct shmec_ep *endpoint, void *buf, size_t size);
int shmec_try_recv_event(struct shmec_ep *endpoint, void *buf, size_t size);

#endif /* __PLUGIN_API_SHMEC_H__ */
