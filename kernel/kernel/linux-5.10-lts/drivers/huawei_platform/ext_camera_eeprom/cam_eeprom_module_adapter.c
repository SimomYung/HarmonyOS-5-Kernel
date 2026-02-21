// SPDX-License-Identifier: GPL-2.0
/* 
 * camera_eeprom_module_adapter.c
 *
 * camera_eeprom_module_adapter driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "cam_eeprom_i2c.h"
#include "cam_eeprom_module_adapter.h"
#include <linux/delay.h>
#include <huawei_platform/log/hw_log.h>
#include <securec.h>

#define ARRAY_SIZE(arr)    (sizeof(arr) / sizeof((arr)[0]))
#define HWLOG_TAG cam_eeprom_module_adapter
HWLOG_REGIST();

static struct cam_eeprom_module_device_info *g_di = NULL;

extern int register_camerafs_attr(struct device_attribute *attr);

static int camera_eeprom_write_block(struct cam_eeprom_module_device_info *di,
	u16 reg, u8 *buf, size_t buf_len)
{
	int rc = 0;
	int remain_length = buf_len;
	u16 addr = reg;
	u16 offset = 0;

	if (!di || !buf) {
		hwlog_err("%s di or buf is null\n", __func__);
		return -EINVAL;
	}

	while (remain_length >= PAGE_SIZE) {
		rc = i2c_u16_write_page(di->client, addr, buf + offset, PAGE_SIZE);
		if (rc != 0) {
			hwlog_err("write block fail\n");
			return rc;
		}
		offset += PAGE_SIZE;
		addr += PAGE_SIZE;
		remain_length -= PAGE_SIZE;
		mdelay(WRITE_INTERVAL_MS);
	}

	while (remain_length > 0) {
		rc = i2c_u16_write_byte(di->client, addr, buf[offset]);
		if (rc != 0) {
			hwlog_err("write byte fail\n");
			return rc;
		}
		offset++;
		addr++;
		remain_length--;
		mdelay(WRITE_INTERVAL_MS);
	}

	return rc;
}

static int camera_eeprom_read_block(struct cam_eeprom_module_device_info *di,
	u16 reg, u8 *buf, u16 buf_len)
{
	int rc = 0;

	if (!di || !buf) {
		hwlog_err("%s di or buf is null\n", __func__);
		return -EINVAL;
	}
	rc = i2c_u16_read_block(di->client, reg, buf, buf_len);
	if (rc != 0) {
		hwlog_err("eeprom read block failed\n");
		return -EINVAL;
	}
	mdelay(WRITE_INTERVAL_MS);

	return rc;
}

static ssize_t otp_read_addr_store(struct device *dev,
        struct device_attribute *attr, const char *buf,
        size_t count)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&di->lock);
	rc = memcpy_s(&reg, sizeof(reg), buf, count);
	if (rc != 0) {
		hwlog_err("memcpy_s failed\n");
		return -EINVAL;
	}
	if (reg > di->eeprom_capacity) {
		hwlog_err("red reg error\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	di->otp_read_addr = di->base_addr + reg;
	mutex_unlock(&di->lock);

	return count;
}

static ssize_t otp_write_addr_store(struct device *dev,
        struct device_attribute *attr, const char *buf,
        size_t count)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&di->lock);
	rc = memcpy_s(&reg, sizeof(reg), buf, count);
	if (rc != 0) {
		hwlog_err("memcpy_s failed\n");
		return -EINVAL;
	}
	
	if (reg > di->eeprom_capacity) {
		hwlog_err("write reg error\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	di->otp_write_addr = di->base_addr + reg;
	mutex_unlock(&di->lock);

	return count;
}

static ssize_t otp_enable_show(struct device *dev,
        struct device_attribute *attr, char *buf)
{
	int enable = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	if (di->otp_enable)
		enable = 1;

	return scnprintf(buf, PAGE_SIZE, "%d\n", enable);
}

static ssize_t otp_enable_store(struct device *dev,
        struct device_attribute *attr, const char *buf,
        size_t count)
{
	int rc = 0;
	char *token = NULL;
	char *next_token = NULL;
	u32 val = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di)
		goto input_error;

	char check_str[MAX_OTP_ENABLE_SIZE + 1] = { 0 };
	rc = memcpy_s(check_str, MAX_OTP_ENABLE_SIZE, buf, count);
	if (rc != 0) {
		hwlog_err("%s memcpy fail\n", __func__);
		return -EINVAL;
	}
	token = strtok_s(check_str, ":", &next_token);
	if (!token)
		goto input_error;
	if (strcmp(token, "AtcmdServer") != 0)
		goto input_error;

	token = strtok_s(NULL, ":", &next_token);
	if (!token)
		goto input_error;
	if (sscanf_s(token, "%u\n", &val) != 1)
		goto input_error;
	if (di->cam_index != val)
		goto input_error;

	token = strtok_s(NULL, ":", &next_token);
	if (!token)
		goto input_error;
	if (sscanf_s(token, "%u\n", &val) != 1)
		goto input_error;
	if (val == 1)
		di->otp_enable = true;
	else
		di->otp_enable = false;

	return count;
input_error:
	hwlog_err("[%s] input error\n", __func__);
	return -EINVAL;
}

static ssize_t otp_header_show(struct device *dev,
        struct device_attribute *attr, char *buf)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	reg = di->otp_read_addr;
	mutex_lock(&di->lock);
	rc = camera_eeprom_read_block(di, reg, buf, MAX_HEADER_SIZE);
	if (rc != 0) {
		hwlog_err("read header failed\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	mutex_unlock(&di->lock);

	return MAX_HEADER_SIZE;
}

static ssize_t otp_header_store(struct device *dev,
        struct device_attribute *attr, const char *buf,
        size_t count)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	if (!di->otp_enable) {
		hwlog_info("%s otp not enable\n", __func__);
		return -EINVAL;
	}
	reg = di->otp_write_addr;

	mutex_lock(&di->lock);
	rc = camera_eeprom_write_block(di, reg, (u8 *)buf, count);
	if (rc != 0) {
		hwlog_err("write header failed\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	mutex_unlock(&di->lock);

	return count;
}

static ssize_t otp_data_show(struct device *dev,
        struct device_attribute *attr, char *buf)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	reg = di->otp_read_addr;
	mutex_lock(&di->lock);
	rc = camera_eeprom_read_block(di, reg, buf, MAX_SEGMENT_SIZE);
	if (rc != 0) {
		hwlog_err("read segment failed\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	mutex_unlock(&di->lock);

	return MAX_SEGMENT_SIZE;
}

static ssize_t otp_data_store(struct device *dev,
        struct device_attribute *attr, const char *buf,
        size_t count)
{
	int rc = 0;
	u16 reg = 0;
	struct cam_eeprom_module_device_info *di = g_di;

	if (!dev || !attr || !buf ||!di) {
		hwlog_err("[%s] input NULL\n", __func__);
		return -EINVAL;
	}

	if (!di->otp_enable) {
		hwlog_info("%s otp not enable\n", __func__);
		return -EINVAL;
	}

	reg = di->otp_write_addr;
	mutex_lock(&di->lock);
	rc = camera_eeprom_write_block(di, reg, (u8 *)buf, count);
	if (rc != 0) {
		hwlog_err("write segment failed\n");
		mutex_unlock(&di->lock);
		return -EINVAL;
	}
	mutex_unlock(&di->lock);

	return count;
}

static DEVICE_ATTR(otp_header, 0664, otp_header_show, otp_header_store);
static DEVICE_ATTR(otp_data, 0664, otp_data_show, otp_data_store);
static DEVICE_ATTR(otp_enable, 0664, otp_enable_show, otp_enable_store);
static DEVICE_ATTR(otp_read_addr, 0220, NULL, otp_read_addr_store);
static DEVICE_ATTR(otp_write_addr, 0220, NULL, otp_write_addr_store);

static struct device_attribute *cam_eeprom_attributes[] = {
	&dev_attr_otp_header,
	&dev_attr_otp_data,
	&dev_attr_otp_enable,
	&dev_attr_otp_read_addr,
	&dev_attr_otp_write_addr,
	NULL,
};

static int cam_eeprom_module_parse_dts(struct device_node *np,
	struct cam_eeprom_module_device_info *di)
{
	if (of_property_read_u32(np, "cam_index", &di->cam_index)) {
		hwlog_err("cam_index not exist!\n");
		return -EINVAL;
	}

	if (of_property_read_u32(np, "capacity", &di->eeprom_capacity)) {
		hwlog_err("eeprom_capacity not exist!\n");
		return -EINVAL;
	}

	if (of_property_read_u16(np, "base_addr", &di->base_addr)) {
		hwlog_err("base addr not exist use default 0!\n");
		di->base_addr = 0;
	}
	return 0;
}

static int cam_eeprom_module_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct cam_eeprom_module_device_info *di = NULL;
	struct device_node *np = NULL;
	int rc = 0;
	int i = 0;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;

	if (cam_eeprom_module_parse_dts(np, di))
		goto fail_free_mem;

	mutex_init(&di->lock);
	for (i = 0; i < ARRAY_SIZE(cam_eeprom_attributes); i++) {
		if (!cam_eeprom_attributes[i])
			break;
		rc = register_camerafs_attr(cam_eeprom_attributes[i]);
		if (rc)
			goto destory_lock;
	}
	dev_set_drvdata(&client->dev, di);
	g_di = di;

	return rc;

destory_lock:
	mutex_destroy(&di->lock);
fail_free_mem:
	devm_kfree(&client->dev, di);
	return -EPERM;
}

static int cam_eeprom_module_remove(struct i2c_client *client)
{
	struct cam_eeprom_module_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	mutex_destroy(&di->lock);
	devm_kfree(&client->dev, di);

	return 0;
}

MODULE_DEVICE_TABLE(i2c, cam_eeprom_module);
static const struct of_device_id cam_eeprom_module_of_match[] = {
	{
		.compatible = "cam_eeprom_module",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id cam_cam_eeprom_module_i2c_id[] = {
	{ "cam_eeprom_module", 0 }, {}
};

static struct i2c_driver cam_eeprom_module_driver = {
	.probe = cam_eeprom_module_probe,
	.remove = cam_eeprom_module_remove,
	.id_table = cam_cam_eeprom_module_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "cam_eeprom_module",
		.of_match_table = of_match_ptr(cam_eeprom_module_of_match),
	},
};

static __init int cam_eeprom_module_init(void)
{
	return i2c_add_driver(&cam_eeprom_module_driver);
}

static __exit void cam_eeprom_module_exit(void)
{
	i2c_del_driver(&cam_eeprom_module_driver);
}

late_initcall(cam_eeprom_module_init);
module_exit(cam_eeprom_module_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("cam_eeprom_module driver");
MODULE_AUTHOR("huawei Technologies Co., Ltd.");
