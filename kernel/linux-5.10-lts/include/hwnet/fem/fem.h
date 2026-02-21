/*
 * fem.h
 *
 * fem header
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#ifndef _FEM_H_
#define _FEM_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/i2c.h>

enum {
	FEM_DEVID_AU8310 = 0xd,
	FEM_DEVID_6160 = 0xa,
	FEM_DEVID_SGM62110 = 0x4,
};

/* define devid */
#define FEM_IC_GOOD 0
#define FEM_IC_BAD  1

/* register devid */
#define FEM_DEVID_ADDR          0x03
#define FEM_DEVID_MASK          0xf0
#define FEM_DEVID_SHIFT         4

/* register vout_sel */
#define FEM_CONTROL_ADDR     0x01
#define FEM_VOUT1_ADDR       0x04
#define FEM_VOUT2_ADDR       0x05
#define FEM_VOUT_SEL_BASE1   1800
#define FEM_VOUT_SEL_BASE2   2025
#define FEM_VOUT_SEL_STEP    25

#define FEM_VOUT1_SEL_DEFAULT    3450
#define FEM_VOUT2_SEL_DEFAULT    4200

#define FEM_VOUT_SEL_MAX      4975
#define FEM_VOUT_SEL_MIN      2025

struct fem_device_info {
	struct i2c_client *client;
	struct device *dev;
	u32 const_vout1;
	u32 const_vout2;
	u32 init_mode;
	u8 dev_id;
	int gpio_no;
};

#endif /* _FEM_H_ */
