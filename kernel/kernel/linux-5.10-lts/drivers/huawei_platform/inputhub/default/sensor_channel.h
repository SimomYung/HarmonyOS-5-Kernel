/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sensor channel header file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#ifndef __SENSOR_CHANNEL_H__
#define __SENSOR_CHANNEL_H__

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#include <huawei_platform/inputhub/sensorhub.h>
#include <securec.h>

#include "acc_sysfs.h"
#include "airpress_sysfs.h"
#include "als_sysfs.h"
#include "cap_prox_sysfs.h"
#include "contexthub_boot.h"
#include "contexthub_recovery.h"
#include "contexthub_route.h"
#include "gyro_sysfs.h"
#include "mag_sysfs.h"
#include "ps_sysfs.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_info.h"
#include "sensor_sysfs.h"
#include "therm_sysfs.h"
#include "tof_detect.h"
#include "tof_channel.h"
#include "tof_sysfs.h"
#include "contexthub_logbuff.h"
#include "contexthub_debug.h"
#include "contexthub_pm.h"

#define SET_DEBUG_MODEL 1
#define SET_NORMAL_MODEL    5
#define SET_DUMP_ACC    10
#define SET_DUMP_ACC1   20
#define SET_DUMP    50
#define DEBUG    4
#define NORMAL    3

enum {
    ANCO_RUNNING = 0,
    ANCO_PRE_FREEZE,
    ANCO_PAUSED,
    ANCO_EXIT,
    ANCO_FAIL,
};

void send_sensor_calib_data(void);
void send_compass_background_calibration_flag(struct ioctl_para *para, int tag);
bool switch_sensor(int tag, bool enable, int host);
void send_debugs_command(int tag, int32_t command);
int send_sensor_batch_flush_cmd(unsigned int cmd, struct ioctl_para *para, int tag, int host);
void enable_sensors_when_recovery_iom3(void);
void config_sensors_when_recovery_iom3(void);
uint8_t get_hal_sensor_type_to_tag(int shb_type);

#endif
