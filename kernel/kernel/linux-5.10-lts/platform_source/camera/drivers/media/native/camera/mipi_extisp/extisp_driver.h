#ifndef EXTISP_DRIVER_H
#define EXTISP_DRIVER_H

#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/of_device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-v4l2.h>
#include <media/videobuf2-dma-contig.h>
#include <securec.h>

#include "power_utils.h"

#define SENSOR_NUM_MAX 6
#define SENSOR_NAME_LEN_MAX 30
#define EXTISP_NUM_MAX 2
#define DEVICE_NAME_SIZE 32

#define HWSENSOR_IOCTL_EXTISP_CFG _IOWR('V', BASE_VIDIOC_PRIVATE + 4, extisp_cfg_data_t)

typedef enum extisp_config_type
{
	EXTISP_CONFIG_POWER,
	EXTISP_CONFIG_RW_DES_REG,
	EXTISP_CONFIG_GET_SENSOR_INFO,
	EXTISP_CONFIG_RELEASE,
	EXTISP_CONFIG_GET_ISP_INDEX,
	EXTISP_CONFIG_MAX_INDEX
} extisp_config_type_t;

typedef enum power_cfg_type {
    CFG_TYPE_ISP,
    CFG_TYPE_SENSOR,
    CFG_TYPE_MAX
} power_cfg_type_t;

typedef enum isp_idx {
	ISP_IDX_YHW = 0,
	ISP_IDX_XXW = 1,
	ISP_IDX_MAX = 2,
} isp_idx_t;

typedef enum i2c_rw_ops {
	I2C_RW_OPS_WRITE = 0,
	I2C_RW_OPS_READ = 1,
} i2c_rw_ops_t;

typedef struct extisp_power_cfg {
    power_cfg_type_t cfg_type;
    int powerup_or_down;
    int parameter;  // for sensor index.
} extisp_power_cfg_t;

typedef struct reg_rw_data {
    uint32_t reg_addr;
    uint8_t *data;
    int len;
} reg_rw_data_t;

typedef struct extisp_reg_rw_cfg {
    int reg_read_or_write;
    reg_rw_data_t reg_data;
    struct i2c_rdwr_ioctl_data rdwr;
} extisp_reg_rw_cfg_t;

typedef struct sensor_info {
    int id;
    int position;
    char sensor_name[SENSOR_NAME_LEN_MAX];
} sensor_info_t;

typedef struct extisp_sensor_info_list {
	int num;
	sensor_info_t info[SENSOR_NUM_MAX];
} extisp_sensor_info_list_t;

typedef struct extisp_cfg_data {
    int cfgtype;
    int index;

    union {
		char name[DEVICE_NAME_SIZE];
        extisp_power_cfg_t power_cfg;
        extisp_reg_rw_cfg_t reg_rw_cfg;
        extisp_sensor_info_list_t sensorinfo_list;
    } cfg;
} extisp_cfg_data_t;

typedef struct sensor_data_ctrl {
    struct device *dev;
    int sensor_idx;
    struct power_setting_group power_group;
    uint32_t power_cnt;
    sensor_info_t sensorinfo;
} sensor_data_ctrl_t;

typedef struct exitisp_reg_ops exitisp_reg_ops_t;

typedef struct extisp_data_ctrl {
    int isp_idx;
    int isp_index_gpiono;
    struct v4l2_subdev subdev;
    int cam_dev_num;
    const char *extisp_name;
    struct platform_device *extisp_pdev;
    struct power_setting_group power_group;
    uint32_t power_cnt;
    int sensor_num;
    struct platform_device *sensor_pdev[SENSOR_NUM_MAX];
    struct i2c_client *i2c_c;
	unsigned int i2c_slave_id;
    exitisp_reg_ops_t *extisp_reg_ops;
    struct mutex lock;
    struct wakeup_source *power_ws;
    struct dsm_client *dsm_c;
} extisp_data_ctrl_t;

typedef struct exitisp_reg_ops {
    int (*match_id)(extisp_data_ctrl_t *);
    int (*rw_des_reg)(struct i2c_client *, extisp_reg_rw_cfg_t *);
} exitisp_reg_ops_t;

#endif
