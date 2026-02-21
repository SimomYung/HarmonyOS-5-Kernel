/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef __NSTACK_H_INCLUDED__
#define __NSTACK_H_INCLUDED__

#include <net/sock.h>
#include <linux/types.h>

void nstack_event_process(int32_t event, uint8_t *pdata, uint16_t len);
void nstack_init(struct sock *nlfd);
void nstack_deinit(void);

#endif // __NSTACK_H_INCLUDED__