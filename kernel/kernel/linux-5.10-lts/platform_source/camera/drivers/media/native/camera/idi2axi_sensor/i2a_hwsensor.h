/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi sensor interface
 *
 * This file is released under the GPLv2.
 */

#ifndef I2A_HWSENSOR_H
#define I2A_HWSENSOR_H

#include <linux/platform_device.h>

enum i2a_hwsensor_state_kind_t {
	I2A_HWSENSOR_POWER_DOWN,
	I2A_HWSENSOR_POWER_UP,
};

typedef struct _tag_i2a_hwsensor_intf i2a_hwsensor_intf_t;

/*
 * @brief the sensor interface.
 */
typedef struct _tag_i2a_hwsensor_vtbl {
	/* sensor function table */
	char const *(*get_name)(i2a_hwsensor_intf_t *);
	int (*match_id)(i2a_hwsensor_intf_t *, void *);
	int (*config)(i2a_hwsensor_intf_t *, void *);
	int (*power_up)(i2a_hwsensor_intf_t *);
	int (*power_down)(i2a_hwsensor_intf_t *);
	int (*stream_on)(i2a_hwsensor_intf_t *, void *);
	int (*stream_off)(i2a_hwsensor_intf_t *, void *);
	int (*check_format)(i2a_hwsensor_intf_t *, void *);
	int (*get_uds_info)(i2a_hwsensor_intf_t *, void *);
} i2a_hwsensor_vtbl_t;

struct _tag_i2a_hwsensor_intf {
	i2a_hwsensor_vtbl_t *vtbl;
};

static inline char const *i2a_hwsensor_intf_get_name(i2a_hwsensor_intf_t *hsi)
{
	return hsi->vtbl->get_name(hsi);
}

int i2a_hwsensor_register(struct platform_device *pdev, i2a_hwsensor_intf_t *intf);
void i2a_hwsensor_unregister(struct platform_device *pdev);

#endif