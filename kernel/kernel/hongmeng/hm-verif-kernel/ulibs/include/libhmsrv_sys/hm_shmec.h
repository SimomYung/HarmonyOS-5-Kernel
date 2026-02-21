/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Interface for shmec management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 7 19:49:50 2022
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SHMEC_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SHMEC_H

#include <stdint.h>
#include <stddef.h>
#include <sys/mman.h>
#include <hmkernel/capability.h>
#include <hmkernel/drivers/ec.h>
#include <vsync/queue/rbuffer.h>
#include <libhmsrv_sys/hm_ecmgr.h>

struct shmec_ep;

struct shmec_ep *hm_shmec_create_endpoint(uint64_t flags);
int hm_shmec_delete_endpoint(struct shmec_ep *endpoint);
int hm_shmec_attach(const struct shmec_ep *endpoint, const char *chn, int flag, mode_t mode);
int hm_shmec_detach(const struct shmec_ep *endpoint);
int hm_shmec_send_event(const struct shmec_ep *endpoint, const void *buf, size_t size);

/* Data is received in blocking mode until the futex is woken up. */
int hm_shmec_recv_event(struct shmec_ep *endpoint, void *buf, size_t size);

/* Skip the futex wait and try to read the data in the rbuffer. */
int hm_shmec_try_recv_event(struct shmec_ep *endpoint, void *buf, size_t size);
#endif /* ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SHMEC_H */
