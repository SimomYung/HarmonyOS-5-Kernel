/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implement of camera sync
 * Create: 2023-12-19
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef __CAM_SYNC_H__
#define __CAM_SYNC_H__
#include <linux/types.h>
#define CAM_SYNC_ACQUIRE_ONLINE_LOCK _IO('S', 0)
#define CAM_SYNC_RELEASE_ONLINE_LOCK _IO('S', 1)
#define CAM_SYNC_ACQUIRE_HISP_POWER_LOCK _IO('S', 2)
#define CAM_SYNC_RELEASE_HISP_POWER_LOCK _IO('S', 3)

#define MAX_PAYLOAD 256
enum cam_sync_msg_type {
    PEER_ACK,
    PEER_DIED,
    PEER_PRIVATE_MSG_BEGIN = 0x1000,
};

struct cam_sync_msg {
    union {
        struct {
            enum cam_sync_msg_type type;
            unsigned int message_id;
            __u8 body[0];
        };
        __u8 payload[MAX_PAYLOAD];
    };
};

#define CAM_SYNC_SEND_PEER_RPMSG _IOW('S', 4, struct cam_sync_msg)
#define CAM_SYNC_RECV_PEER_RPMSG _IOR('S', 5, struct cam_sync_msg)
#define CAM_SYNC_ANCO_STATUS _IOR('S', 6, __u32)

#endif
