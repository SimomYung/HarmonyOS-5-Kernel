/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: motion channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __MOTION_CHANNEL_H__
#define __MOTION_CHANNEL_H__

#define IS_CAMERA_USE_SHAKE_FLAG 0
#define IS_FRAMEWORK_USE_SHAKE_FLAG 1

void enable_motions_when_recovery_iom3(void);
void disable_motions_when_sysreboot(void);
int send_motion_shake_cmd(int cmd, int arg, int flag);
int get_shake_state_signs(void);

#endif
