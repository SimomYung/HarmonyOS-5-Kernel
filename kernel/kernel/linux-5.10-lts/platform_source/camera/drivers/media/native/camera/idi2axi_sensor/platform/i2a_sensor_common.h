

#ifndef _I2A_SENSOR_COMMOM_H_
#define _I2A_SENSOR_COMMOM_H_

#include "i2a_hwsensor.h"

struct i2a_i2c_client {
	struct i2c_client *i2c_c;
	unsigned int i2a_sensor_slave_id;
	unsigned int i2a_sensor_camera_type;
};

struct i2a_sensor_resolution {
	uint32_t width;
	uint32_t height;
};

typedef struct _tag_i2a_hwsensor_board_info {
	const char *name; /* sensor name */
	struct device *dev;

	uint32_t sensor_index;
	struct i2a_i2c_client iclient;
	struct platform_device *i2a_sensor_des_pdev;
	uint32_t resolution_cnt;
	struct i2a_sensor_resolution *resolution;
} i2a_hwsensor_board_info_t;

typedef struct _i2a_sensor_t {
	i2a_hwsensor_intf_t intf;
	i2a_hwsensor_board_info_t *board_info;
	enum i2a_hwsensor_state_kind_t state;
	struct device *dev;
	struct mutex state_lock;
} i2a_sensor_t;

#endif