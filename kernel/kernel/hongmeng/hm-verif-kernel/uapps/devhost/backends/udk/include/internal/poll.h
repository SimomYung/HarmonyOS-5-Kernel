/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of poll
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 11 18:52:44 UTC 2023
 */

#ifndef __UDK_INTERNAL_POLL_H__
#define __UDK_INTERNAL_POLL_H__
#include <udk/poll.h>

int udk_poll_wevent_init(void *poll_info, struct udk_poll_wevent **out_event);
void udk_poll_wevent_deinit(void *poll_info, struct udk_poll_wevent **event);

#endif /* __UDK_INTERNAL_POLL_H__ */
