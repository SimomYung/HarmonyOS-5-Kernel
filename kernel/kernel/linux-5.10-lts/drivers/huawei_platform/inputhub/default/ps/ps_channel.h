/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: ps channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __PS_CHANNEL_H__
#define __PS_CHANNEL_H__

#define TCS3718_XTALK_THRE_H             700
#define TCS3718_XTALK_THRE_L             300
#define SIP3623_XTALK_THRE_H             160
#define SIP3623_XTALK_THRE_L             0

int send_ps_calibrate_data_to_mcu(void);
void reset_ps_calibrate_data(void);
int tp_event_to_ultrasonic(unsigned char event);

#endif
