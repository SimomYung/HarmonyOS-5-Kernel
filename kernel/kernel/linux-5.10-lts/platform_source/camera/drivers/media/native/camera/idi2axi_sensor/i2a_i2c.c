/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi i2c helper, support local/remote i2c device read/write
 *
 * This file is released under the GPLv2.
 */
#include "i2a_i2c.h"
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/module.h>
#include <linux/kconfig.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/byteorder/generic.h>
#include <linux/string.h>
#include <securec.h>
#include "i2a_debug.h"

#define I2C_LEN_1BYTE 1
#define I2C_LEN_2BYTE 2
#define I2C_LEN_3BYTE 3
#define I2C_LEN_4BYTE 4
#define SLAVE_REG_SIZE 2
#define STACK_MSG_SIZE 128
#define MAX_READ_BYTES  (1024 * 1024) // 1M
#define MAX_WRITE_BYTES (1024 * 1024) // 1M

union i2c_data {
	uint8_t u8;
	__be16  u16;
	__be32  u32;
};

int cci_read_bytes(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *out_data, uint32_t len)
{
	int ret;
	uint8_t reg_addr[SLAVE_REG_SIZE];
	struct i2c_msg msg[2];

	reg_addr[0] = (uint8_t)(reg & 0xFF);

	msg[0].addr = iclient->i2c_c->addr;
	msg[0].flags = 0;
	msg[0].buf = reg_addr;
	msg[0].len = 1;

	msg[1].addr = iclient->i2c_c->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].buf = out_data;
	msg[1].len = len;

	ret = i2c_transfer(iclient->i2c_c->adapter, msg, 2); // for i2c msg
	if (ret != 2)
		pr_err("fail to tx i2x msg");

	pr_info("I2C Read: slave:%#hx, reg:%#x len:%hhx val:0x%x", msg[0].addr,
		reg, len, *out_data);
	return len;
}

int cci_write_bytes(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *in_data, uint32_t len)
{
	int ret = 0;
	uint8_t stack_msg[STACK_MSG_SIZE];
	uint8_t *msgdata;
	struct i2c_msg msg; /* for i2c msg */

	if (!iclient || !iclient->i2c_c)
		return -EINVAL;

	msgdata = stack_msg;

	msgdata[0] = (uint8_t)(reg & 0xFF);
	ret = memcpy_s(&msgdata[1], len, in_data, len);
	if (ret != 0) {
		pr_err("%s memcpy_s fail", __func__);
		return -EINVAL;
	}

	msg.addr = iclient->i2c_c->addr;
	msg.flags = 0;
	msg.buf = msgdata;
	msg.len = len + 1; // 1 is the 8 bit reg

	ret = i2c_transfer(iclient->i2c_c->adapter, &msg, 1);
	if (ret == 1)
		ret = 0;
	else
		pr_err("fail to tx i2x msg");

	pr_info("I2C Write: slave:%#hx, reg:%#x len:%hhx val:%d ret:%d", msg.addr,
		reg, len, *in_data, ret);

	return len;
}

/**
 * cci_read_integer - treat out_data as a u8/u16/u32 according to len 1/2/4
 *		      and convert slave endianess to cpu endianess

 * Returns negative errno, or 0 for success
 */
int cci_read_integer(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *out_data, uint32_t len)
{
	int ret;
	union i2c_data data;

	if (!out_data || (len != I2C_LEN_1BYTE && len != I2C_LEN_2BYTE && len != I2C_LEN_4BYTE))
		pr_err("cci_read_integer fail %p, %d, %d", out_data, *out_data, len);

	data.u32 = 0;
	ret = cci_read_bytes(iclient, reg, &data.u8, len);
	if (ret != len) {
		pr_err("cci_read_bytes fail %d %d", ret, len);
		return ret;
	}

	switch (len) {
	case I2C_LEN_4BYTE:
		*(uint32_t*)out_data = be32_to_cpu(data.u32);
		break;
	case I2C_LEN_2BYTE:
		*(uint16_t*)out_data = be16_to_cpup(data.u16);
		break;
	case I2C_LEN_1BYTE:
		*out_data = data.u8;
		break;
	default:
		pr_err("%s not support length", __func__);
		*out_data = data.u8;
		break;
	}
	return 0;
}

/**
 * cci_write_integer - treat in_data as a u8/u16/u32 according to len 1/2/4
 *		       and convert cpu endianess to slave endianess

 * Returns negative errno, or 0 for success
 */
int cci_write_integer(struct i2a_i2c_client *iclient, uint16_t reg, uint32_t in_data, uint32_t len)
{
	int ret;
	union i2c_data data;

	if (len != I2C_LEN_1BYTE && len != I2C_LEN_2BYTE && len != I2C_LEN_4BYTE)
		pr_err("cci_write_integer 1 %d", len);

	switch (len) {
	case I2C_LEN_4BYTE:
		data.u32 = cpu_to_be32(in_data);
		break;
	case I2C_LEN_2BYTE:
		data.u16 = cpu_to_be16(in_data);
		break;
	case I2C_LEN_1BYTE:
		data.u8 = in_data;
		break;
	default:
		pr_err("%s not support length", __func__);
		break;
	}
	ret = cci_write_bytes(iclient, reg, &data.u8, len);
	if (ret != len)
		return ret;

	return 0;
}

static int idi_des_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct i2a_des_i2c_data *data;

	if (!client || !id)
		return -ENOENT;

	pr_info("probing i2c client: %s\n", client->name);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("%s i2c_check_functionality failed\n", __func__);
		return -EINVAL;
	}

	data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->dev = &client->dev;
	mutex_init(&data->mtx);
	i2c_set_clientdata(client, data);
	return 0;
}

static int idi_des_i2c_remove(struct i2c_client *client)
{
	pr_info("removing i2c client: %s", client->name);
	return 0;
}

static const struct i2c_device_id idi_i2c_id[] = {
	/* name, driver_data */
	{"i2a_serdes_i2c", },
	{"i2a_sensor_i2c", },
	{}
};

static const struct of_device_id of_idi_i2c_match[] = {
	{.compatible = "hisilicon,i2a_serdes_i2c"},
	{.compatible = "hisilicon,i2a_sensor_i2c"},
	{}
};
MODULE_DEVICE_TABLE(of, of_idi_i2c_match);

static struct i2c_driver i2a_des_i2c_driver = {
	.probe    = idi_des_i2c_probe,
	.remove   = idi_des_i2c_remove,
	.id_table = idi_i2c_id,
	.driver = {
		.name           = "i2a_i2c",
		.of_match_table = of_idi_i2c_match,
	},
};


#if IS_MODULE(CONFIG_CAM_IDI2AXI)
int i2a_des_i2c_init(void)
{
	return i2c_add_driver(&i2a_des_i2c_driver);
}

void i2a_des_i2c_exit(void)
{
	i2c_del_driver(&i2a_des_i2c_driver);
}
#else
module_i2c_driver(i2a_des_i2c_driver);
MODULE_DESCRIPTION("i2a i2c driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Hisilicon");
#endif