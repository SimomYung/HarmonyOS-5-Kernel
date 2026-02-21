/*
 * Linux kernel modules for MT380X FlightSense TOF sensor
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/time.h>
#include <linux/of_gpio.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_device.h>
#include <linux/time.h>
#include <platform_include/camera/native/laser_cfg.h>

#ifdef CONFIG_HUAWEI_HW_DEV_DCT
#include <huawei_platform/devdetect/hw_dev_dec.h>
#endif

#include "mt380x.h"
#include "mt380x_platform.h"
#include "mt380x_firmware.h"
#include "mt380x_api.h"
#include "mt380x_module.h"

#define HWLASER_CALIBRATION_CROSSTALK 1
#define HWLASER_CALIBRATION_OFFSET 2

#define MT380X_DRV_NAME "mt380x"
static int crosstalk_mark;
static int offset_mark;

struct mt380x_api_fn_t {
	int32_t (*power_on)(mt380x_dev dev);
	int32_t (*power_off)(mt380x_dev dev);
	int32_t (*set_period)(mt380x_dev dev, uint32_t period);
	int32_t (*single_measure)(mt380x_dev dev);
	int32_t (*begin_continuous_measure)(mt380x_dev dev);
	int32_t (*end_continuous_measure)(mt380x_dev dev);
	int32_t (*get_measure_data)(mt380x_dev dev);
	int32_t (*get_interrupt_state)(mt380x_dev dev, uint8_t *stat);
	int32_t (*chip_init)(mt380x_dev dev);
	int32_t (*begin_crosstalk_calibration)(mt380x_dev dev);
	int32_t (*begin_offset_calibration)(mt380x_dev dev, uint32_t param);
	int32_t (*get_crosstalk_params)(mt380x_dev dev);
	int32_t (*config_crosstalk_params)(mt380x_dev dev);
	int32_t (*config_offset_params)(mt380x_dev dev);
	int32_t (*get_online_state)(mt380x_dev dev);
};

static struct mt380x_api_fn_t mt380x_api_func_tbl = {
	.power_on = mt380x_power_on,
	.power_off = mt380x_power_off,
	.set_period = mt380x_set_period,
	.single_measure = mt380x_single_measure,
	.begin_continuous_measure = mt380x_begin_continuous_measure,
	.end_continuous_measure = mt380x_end_continuous_measure,
	.get_measure_data = mt380x_get_measure_datas,
	.get_interrupt_state = mt380x_get_and_clear_interrupt_state,
	.chip_init = mt380x_init,
	.begin_crosstalk_calibration = mt380x_begin_crosstalk_calibration,
	.begin_offset_calibration = mt380x_begin_offset_calibration,
	.get_crosstalk_params = mt380x_get_crosstalk_params,
	.config_crosstalk_params = mt380x_config_crosstalk_params,
	.config_offset_params = mt380x_config_offset_params,
	.get_online_state = mt380x_read_chip_version,
};

struct mt380x_api_fn_t *mt380x_func_tbl;

static void mt380x_setup_api_functions(void)
{
	mt380x_func_tbl->power_on = mt380x_power_on;
	mt380x_func_tbl->power_off = mt380x_power_off;
	mt380x_func_tbl->set_period = mt380x_set_period;
	mt380x_func_tbl->single_measure = mt380x_single_measure;
	mt380x_func_tbl->begin_continuous_measure = mt380x_begin_continuous_measure;
	mt380x_func_tbl->end_continuous_measure = mt380x_end_continuous_measure;
	mt380x_func_tbl->get_measure_data = mt380x_get_measure_datas;
	mt380x_func_tbl->get_interrupt_state = mt380x_get_and_clear_interrupt_state;
	mt380x_func_tbl->chip_init = mt380x_init;
	mt380x_func_tbl->begin_crosstalk_calibration = mt380x_begin_crosstalk_calibration;
	mt380x_func_tbl->begin_offset_calibration = mt380x_begin_offset_calibration;
	mt380x_func_tbl->get_crosstalk_params = mt380x_get_crosstalk_params;
	mt380x_func_tbl->config_crosstalk_params = mt380x_config_crosstalk_params;
	mt380x_func_tbl->config_offset_params = mt380x_config_offset_params;
	mt380x_func_tbl->get_online_state = mt380x_read_chip_version;
}

static void mt380x_enable_irq(struct mt380x_data *data)
{
	if (!data) {
		mt380x_errmsg("data is null\n");
		return;
	}

	if (data->intr_state == MT380X_INTR_DISABLED) {
		data->intr_state = MT380X_INTR_ENABLED;
		enable_irq(data->irq);
		mt380x_infomsg("enable irq succ\n");
	}
}

static void mt380x_disable_irq(struct mt380x_data *data)
{
	if (!data) {
		mt380x_errmsg("data is null\n");
		return;
	}

	if (data->intr_state == MT380X_INTR_ENABLED) {
		data->intr_state = MT380X_INTR_DISABLED;
		disable_irq(data->irq);
		mt380x_infomsg("disable irq succ\n");
	}
}

static ssize_t mt380x_enable_chip_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (data)
		return scnprintf(buf, PAGE_SIZE, "%u\n", data->enable_chip);

	return -EINVAL;
}

static ssize_t mt380x_enable_chip_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	unsigned int val = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val != 0 && val != 1) {
		mt380x_errmsg("enable store invalid value=%u\n", val);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val == 1) {
		if (data->enable_chip == 0) {
			data->enable_chip = 1;
			mt380x_enable_irq(data);
			status = mt380x_func_tbl->power_on(data);
			if (status != MT380X_ERROR_NONE) {
				data->enable_chip = 0;
				mt380x_disable_irq(data);
			}
		} else {
			mt380x_infomsg("already enabled\n");
		}
	} else {
		if (data->enable_chip == 1) {
			data->enable_chip = 0;
			mt380x_disable_irq(data);
			data->fwdl_status = 0;
			status = mt380x_func_tbl->power_off(data);
			if (status != MT380X_ERROR_NONE)
				mt380x_disable_irq(data);
		} else {
			mt380x_infomsg("already disabled\n");
		}
	}
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(enable_chip, 0664, mt380x_enable_chip_show, mt380x_enable_chip_store);

static ssize_t mt380x_debug_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (data)
		return snprintf(buf, PAGE_SIZE, "%u\n", data->debug_enable);

	return -EINVAL;
}

static ssize_t mt380x_debug_enable_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	unsigned int on = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &on) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (on != 0 && on != 1) {
		mt380x_errmsg("set debug=%u\n", on);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	data->debug_enable = on;
	mutex_unlock(&data->work_mutex);
	return count;
}

static DEVICE_ATTR(debug_enable, 0664, mt380x_debug_enable_show, mt380x_debug_enable_store);

static ssize_t mt380x_init_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	unsigned int val = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (!val) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}

	status = mt380x_func_tbl->chip_init(data);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("%d, CHIP INIT FAILED\n", __LINE__);
		data->fwdl_status = 0;
		mutex_unlock(&data->work_mutex);
		return -EIO;
	}
	data->fwdl_status = 1;
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(init_chip, 0220, NULL, mt380x_init_store);

static ssize_t mt380x_period_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (data)
		return scnprintf(buf, PAGE_SIZE, "%u\n", data->period);

	return -EINVAL;
}

static ssize_t mt380x_period_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	uint32_t val = 0;
	mt380x_error status = MT380X_ERROR_NONE;
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val < 1 || val > 100) {
		mt380x_errmsg("val is invalid data, val:%u\n", val);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	data->period = val;
	status = mt380x_func_tbl->set_period(data, data->period);
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(period, 0664, mt380x_period_show, mt380x_period_store);

static ssize_t mt380x_capture_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	unsigned int val = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val != 0 && val != 1) {
		mt380x_errmsg("capture store invalid value=%u\n", val);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val == 1)
		status = mt380x_func_tbl->begin_continuous_measure(data);
	else
		status = mt380x_func_tbl->end_continuous_measure(data);
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(capture, 0220, NULL, mt380x_capture_store);

static ssize_t mt380x_crosstalk_calib_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (data)
		return scnprintf(buf, PAGE_SIZE, "%d.%u\n",
			data->crosstalk_data.crosstalk_position,
			data->crosstalk_data.crosstalk_ratio);

	return -EINVAL;
}

static ssize_t mt380x_crosstalk_calib_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	unsigned int val = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val != 1) {
		mt380x_errmsg("crosstalk calibration store invalid value=%u\n", val);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	crosstalk_mark = 1;
	status = mt380x_func_tbl->begin_crosstalk_calibration(data);
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("%d, PERFORM CROSSTALK CALIB FAILED\n", __LINE__);
	mdelay(480);
	crosstalk_mark = 0;
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(crosstalk_calib, 0664, mt380x_crosstalk_calib_show,
	mt380x_crosstalk_calib_store);

static ssize_t mt380x_offset_calib_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct mt380x_data *data = dev_get_drvdata(dev);

	if (data)
		return scnprintf(buf, PAGE_SIZE, "%d\n", data->offset_data.offset_cal);

	return -EINVAL;
}

static ssize_t mt380x_offset_calib_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	unsigned int val = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%u\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val != 1) {
		mt380x_errmsg("offset calibration store invalid value=%u\n", val);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	offset_mark = 1;
	status = mt380x_func_tbl->begin_offset_calibration(data, 0);
	offset_mark = 0;
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(offset_calib, 0664, mt380x_offset_calib_show, mt380x_offset_calib_store);

static ssize_t mt380x_crosstalk_config_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	mt380x_error status = MT380X_ERROR_NONE;
	int32_t val_position = 0;
	uint32_t val_ratio = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%d %u", &val_position, &val_ratio) != 2) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val_position < -128 || val_position > 127) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	data->crosstalk_position_config = (int8_t)val_position;
	if (val_ratio > 255) {
		mt380x_errmsg("val_ratio over 255:%u\n", val_ratio);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}

	data->crosstalk_ratio_config = (uint8_t)val_ratio;
	status = mt380x_func_tbl->config_crosstalk_params(data);
	mutex_unlock(&data->work_mutex);
	return status ? status : count;
}

static DEVICE_ATTR(crosstalk_config, 0220, NULL, mt380x_crosstalk_config_store);

static ssize_t mt380x_offset_config_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct mt380x_data *data = dev_get_drvdata(dev);
	int val = 0;
	int rc = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}

	mutex_lock(&data->work_mutex);
	if (sscanf_s(buf, "%d\n", &val) != 1) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	if (val < -128 || val > 127) {
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	data->offset_config = (int8_t)val;
	rc = mt380x_func_tbl->config_offset_params(data);
	if (rc) {
		mt380x_errmsg("config offset calibration data fail %d", rc);
		mutex_unlock(&data->work_mutex);
		return rc;
	}
	mutex_unlock(&data->work_mutex);
	return count;
}

static DEVICE_ATTR(offset_config, 0220, NULL, mt380x_offset_config_store);

static ssize_t mt380x_crosstalk_data_read(struct file *filp,
	struct kobject *kobj, struct bin_attribute *attr,
	char *buf, loff_t off, size_t count)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct mt380x_data *data = dev_get_drvdata(dev);
	int rc = 0;
	int8_t crosstalk_position_config_temp = 0;
	uint16_t crosstalk_ratio_config_temp = 0;
	(void)off;
	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}
	void *src = (void *)&(data->crosstalk_data);

	mutex_lock(&data->work_mutex);
	if (!data->enable_chip) {
		mt380x_errmsg("can't set calib data while disable sensor\n");
		mutex_unlock(&data->work_mutex);
		return -EBUSY;
	}

	if (count > sizeof(struct mt380x_crosstalk_calib_data)) {
		mt380x_errmsg("count over:%u", count);
		mutex_unlock(&data->work_mutex);
		return -EFAULT;
	}

	if (memcpy_s(buf, count, src, count) != EOK) {
		mt380x_errmsg("memcpy_s failed");
		mutex_unlock(&data->work_mutex);
		return -EFAULT;
	}
	crosstalk_position_config_temp = data->crosstalk_position_config;
	crosstalk_ratio_config_temp = data->crosstalk_ratio_config;
	data->crosstalk_position_config = data->crosstalk_data.crosstalk_position;
	data->crosstalk_ratio_config = data->crosstalk_data.crosstalk_ratio;
	rc = mt380x_func_tbl->config_crosstalk_params(data);
	if (rc) {
		mt380x_errmsg("config crosstalk calibration data fail %d", rc);
		data->crosstalk_position_config = crosstalk_position_config_temp;
		data->crosstalk_ratio_config = crosstalk_ratio_config_temp;
		mutex_unlock(&data->work_mutex);
		return rc;
	}
	mutex_unlock(&data->work_mutex);

	return count;
}

static ssize_t mt380x_crosstalk_data_write(struct file *filp,
	struct kobject *kobj, struct bin_attribute *attr,
	char *buf, loff_t off, size_t count)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct mt380x_data *data = dev_get_drvdata(dev);
	struct mt380x_crosstalk_calib_data *crosstalk_data =
		(struct mt380x_crosstalk_calib_data *)buf;
	int rc = 0;
	(void)off;
	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}
	mutex_lock(&data->work_mutex);

	if (!data->enable_chip) {
		rc = -EBUSY;
		mt380x_errmsg("can't set calib data while disable sensor\n");
		goto error;
	}

	if (count != sizeof(struct mt380x_crosstalk_calib_data))
		goto invalid;

	if (data->debug_enable) {
		mt380x_infomsg("crosstalk crosstalk_position: %d\n",
			crosstalk_data->crosstalk_position);
		mt380x_infomsg("crosstalk crosstalk_ratio: %u\n", crosstalk_data->crosstalk_ratio);
	}
	if (crosstalk_data->crosstalk_position < -128 ||
		crosstalk_data->crosstalk_position > 127 ||
		crosstalk_data->crosstalk_ratio < 1 ||
		crosstalk_data->crosstalk_ratio > 255) {
		mt380x_errmsg("crosstalk_data is invalued data");
		goto error;
	}
	data->crosstalk_position_config = crosstalk_data->crosstalk_position;
	data->crosstalk_ratio_config = crosstalk_data->crosstalk_ratio;
	rc = mt380x_func_tbl->config_crosstalk_params(data);
	if (rc) {
		mt380x_errmsg("config crosstalk calibration data fail %d", rc);
		goto error;
	}
	mutex_unlock(&data->work_mutex);

	return count;

invalid:
	mt380x_errmsg("invalid data size:%u, sizeof:%u",
		count, sizeof(struct mt380x_crosstalk_calib_data));
	rc = -EINVAL;

error:
	mutex_unlock(&data->work_mutex);

	return rc;
}

static ssize_t mt380x_offset_data_read(struct file *filp,
	struct kobject *kobj, struct bin_attribute *attr,
	char *buf, loff_t off, size_t count)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct mt380x_data *data = dev_get_drvdata(dev);
	(void)off;
	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}
	void *src = (void *)&(data->offset_data);

	mutex_lock(&data->work_mutex);
	if (!data->enable_chip) {
		mt380x_errmsg("can't set calib data while disable sensor\n");
		mutex_unlock(&data->work_mutex);
		return -EBUSY;
	}

	if (count > sizeof(struct mt380x_offset_calib_data)) {
		mt380x_errmsg("count overstep:%u", count);
		mutex_unlock(&data->work_mutex);
		return -EBUSY;
	}

	if (memcpy_s(buf, count, src, count) != EOK) {
		mt380x_errmsg("memcpy_s failed");
		mutex_unlock(&data->work_mutex);
		return -EFAULT;
	}
	data->offset_config = data->offset_data.offset_cal;
	mutex_unlock(&data->work_mutex);

	return count;
}

static ssize_t mt380x_offset_data_write(struct file *filp,
	struct kobject *kobj, struct bin_attribute *attr,
	char *buf, loff_t off, size_t count)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct mt380x_data *data = dev_get_drvdata(dev);
	struct mt380x_offset_calib_data *offset_data = NULL;
	int16_t offset_cal_temp = 0;

	if (!data) {
		mt380x_errmsg("data is null\n");
		return -EINVAL;
	}
	offset_data = kzalloc(sizeof(struct mt380x_offset_calib_data), GFP_KERNEL);
	if (!offset_data) {
		mt380x_errmsg("offset_data is null\n");
		return -EINVAL;
	}
	if (memcpy_s(offset_data, sizeof(struct mt380x_offset_calib_data),
		(struct mt380x_offset_calib_data *)buf,
		sizeof(struct mt380x_offset_calib_data)) != EOK) {
		mt380x_errmsg("memcpy_s failed");
		kfree(offset_data);
		return -EFAULT;
	}
	int rc = 0;
	(void)off;

	mutex_lock(&data->work_mutex);
	if (!data->enable_chip) {
		rc = -EBUSY;
		mt380x_errmsg("can't set calib data while disable sensor\n");
		goto error;
	}

	if (count != sizeof(struct mt380x_offset_calib_data))
		goto invalid;

	if (data->debug_enable)
		mt380x_infomsg("offset config: %d\n", offset_data->offset_cal);
	offset_cal_temp = data->offset_config;
	data->offset_config = offset_data->offset_cal;

	rc = mt380x_func_tbl->config_offset_params(data);
	if (rc) {
		mt380x_errmsg("config offset fail %d", rc);
		data->offset_config = offset_cal_temp;
		goto error;
	}
	mutex_unlock(&data->work_mutex);
	kfree(offset_data);
	return count;

invalid:
	mt380x_errmsg("invalid syntax");
	rc = -EINVAL;

error:
	mutex_unlock(&data->work_mutex);
	kfree(offset_data);
	return rc;
}

static struct attribute *mt380x_attributes[] = {
	&dev_attr_enable_chip.attr,
	&dev_attr_debug_enable.attr,
	&dev_attr_init_chip.attr,
	&dev_attr_period.attr,
	&dev_attr_capture.attr,
	&dev_attr_crosstalk_calib.attr,
	&dev_attr_offset_calib.attr,
	&dev_attr_crosstalk_config.attr,
	&dev_attr_offset_config.attr,
	NULL,
};

static const struct attribute_group mt380x_attr_group = {
	.name = NULL,
	.attrs = mt380x_attributes,
};

static struct bin_attribute mt380x_crosstalk_data_attr = {
	.attr = {
		.name = "crosstalk_calib_data",
		.mode = 0664,
	},
	.size = sizeof(struct mt380x_crosstalk_calib_data),
	.read = mt380x_crosstalk_data_read,
	.write = mt380x_crosstalk_data_write,
};

static struct bin_attribute mt380x_offset_data_attr = {
	.attr = {
		.name = "offset_calib_data",
		.mode = 0664,
	},
	.size = sizeof(struct mt380x_offset_calib_data),
	.read = mt380x_offset_data_read,
	.write = mt380x_offset_data_write,
};

static irqreturn_t mt380x_irq_handler(int vec, void *info)
{
	struct mt380x_data *data = (struct mt380x_data *)info;
	mt380x_error status = MT380X_ERROR_NONE;

	if (!data || !data->fwdl_status) {
		mt380x_errmsg("%d : data and data->fwdl_status is null\n", __LINE__);
		return IRQ_HANDLED;
	}

	if (data->irq == vec) {
		if (crosstalk_mark) {
			status = mt380x_func_tbl->get_crosstalk_params(data);
			if (status != MT380X_ERROR_NONE)
				mt380x_errmsg("%d : status = %d\n", __LINE__, status);
		}

		if (!crosstalk_mark && !offset_mark) {
			status = mt380x_func_tbl->get_measure_data(data);
			if (status != MT380X_ERROR_NONE) {
				mt380x_errmsg("%d : status = %d\n", __LINE__, status);
				return IRQ_HANDLED;
			}
		}
	}
	return IRQ_HANDLED;
}

static int mt380x_status(struct mt380x_data *data, hwlaser_status_t *laser_status)
{
	int rc = -EINVAL;

	if (!data || !laser_status)
		return rc;

	laser_status->status = MT380X_ERROR_NONE;

	return MT380X_ERROR_NONE;
}

static int mt380x_perform_cal(struct mt380x_data *data, hwlaser_ioctl_perform_calibration_t *cal)
{
	int rc = 0;

	mutex_lock(&data->work_mutex);
	switch (cal->calibration_type) {
	case HWLASER_CALIBRATION_CROSSTALK:
		crosstalk_mark = 1;
		mt380x_enable_irq(data);
		rc = mt380x_func_tbl->begin_crosstalk_calibration(data);
		if (rc != MT380X_ERROR_NONE) {
			mt380x_errmsg("%d, PERFORM CROSSTALK CALIB FAILED\n", __LINE__);
			crosstalk_mark = 0;
			mt380x_disable_irq(data);
			break;
		}
		mdelay(480);
		crosstalk_mark = 0;
		mt380x_disable_irq(data);
		data->crosstalk_position_config = data->crosstalk_data.crosstalk_position;
		data->crosstalk_ratio_config = data->crosstalk_data.crosstalk_ratio;
		break;
	case HWLASER_CALIBRATION_OFFSET:
		offset_mark = 1;
		rc = mt380x_func_tbl->begin_offset_calibration(data, cal->param1);
		if (rc != MT380X_ERROR_NONE) {
			mt380x_errmsg("%d, PERFORM OFFSET CALIB FAILED\n", __LINE__);
			offset_mark = 0;
			break;
		}
		offset_mark = 0;
		data->offset_config = data->offset_data.offset_cal;
		break;
	default:
		mt380x_errmsg("mt380x perform cal %d, fail %d",
			cal->calibration_type, rc);
	}
	mutex_unlock(&data->work_mutex);

	return rc;
}

static int mt380x_laser_get_set_cal_data(struct mt380x_data *data,
	hwlaser_calibration_data_t *cali_data_local)
{
	int rc = 0;

	mutex_lock(&data->work_mutex);
	if (cali_data_local->is_read) {
		cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_cal =
			data->crosstalk_data.crosstalk_position;
		cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_peak =
			data->crosstalk_data.crosstalk_ratio;
		cali_data_local->u.dataL5.mt380x_offset_calib_data.offset_cal =
			data->offset_data.offset_cal;
	} else {
		data->crosstalk_position_config =
			cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_cal;
		data->crosstalk_ratio_config =
			cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_peak;
		if (data->debug_enable) {
			mt380x_infomsg("config xtalk, position=%d,ratio=%u\n",
				data->crosstalk_position_config, data->crosstalk_ratio_config);
		}
		rc = mt380x_func_tbl->config_crosstalk_params(data);
		if (rc != MT380X_ERROR_NONE) {
			mt380x_errmsg("%d, CONFIG CROSSTALK PARAMETER FAILED\n", __LINE__);
			mutex_unlock(&data->work_mutex);
			return -EINVAL;
		}
		data->offset_config = cali_data_local->u.dataL5.mt380x_offset_calib_data.offset_cal;
		rc = mt380x_func_tbl->config_offset_params(data);
		if (rc != MT380X_ERROR_NONE) {
			mt380x_errmsg("%d, PERFORM OFFSET CONFIG FAILED\n", __LINE__);
			mutex_unlock(&data->work_mutex);
			return -EINVAL;
		}
		if (data->debug_enable)
			mt380x_infomsg("offset config = %d\n", data->offset_config);
	}
	mutex_unlock(&data->work_mutex);
	return rc;
}

static int mt380x_laser_set_freq(struct mt380x_data *data, void *arg)
{
	int rc = 0;
	if (!((uint32_t *)arg)) {
		mt380x_errmsg("arg is null\n");
		return -EFAULT;
	}
	mutex_lock(&data->work_mutex);
	if (memcpy_s(&(data->period), sizeof(uint32_t),
		(uint32_t *)arg, sizeof(uint32_t)) != EOK) {
		mt380x_errmsg("%d, GET PERIOD DATA FAILED\n", __LINE__);
		mutex_unlock(&data->work_mutex);
		return -EFAULT;
	}
	if (data->debug_enable)
		mt380x_infomsg("period setting: %d\n", data->period);
	if (data->period < 1 || data->period > 100) {
		mt380x_errmsg("data->period is invalid data, data->period:%u\n", data->period);
		mutex_unlock(&data->work_mutex);
		return -EINVAL;
	}
	mt380x_func_tbl->set_period(data, data->period);
	mutex_unlock(&data->work_mutex);
	return rc;
}

void mt380x_set_params(struct mt380x_data *data, hwlaser_calibration_data_t *cali_data_local)
{
	mutex_lock(&data->work_mutex);
	data->crosstalk_position_config =
		cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_cal;
	data->crosstalk_ratio_config = cali_data_local->u.dataL5.mt380x_xtalk_calib_data.xtalk_peak;
	data->offset_config = cali_data_local->u.dataL5.mt380x_offset_calib_data.offset_cal;
	if (data->debug_enable) {
		mt380x_infomsg("Param crosstalk_position_config: %d\n",
			data->crosstalk_position_config);
		mt380x_infomsg("Param crosstalk_ratio_config: %u\n", data->crosstalk_ratio_config);
		mt380x_infomsg("Param offset_config: %d\n", data->offset_config);
	}
	mutex_unlock(&data->work_mutex);
}

void mt380x_get_laser_info(struct mt380x_data *data, hwlaser_info_t *pinfo)
{
	mutex_lock(&data->work_mutex);
	if (strncpy_s(pinfo->name, HWLASER_NAME_SIZE - 1, "mt380x", HWLASER_NAME_SIZE - 1))
		mt380x_errmsg("strncpy_s failed %d", __LINE__);
	if (strncpy_s(pinfo->product_name, HWLASER_NAME_SIZE - 1, "LASER", HWLASER_NAME_SIZE - 1))
		mt380x_errmsg("strncpy_s failed %d", __LINE__);
	pinfo->version = HWLASER_MXS_I0_VERSION;
	pinfo->ap_pos = HWLASER_POS_AP;
	mutex_unlock(&data->work_mutex);
}

long mt380x_ioctl(void *hw_data, unsigned int cmd, void *arg)
{
	long rc = 0;
	struct mt380x_data *data = (struct mt380x_data *)hw_data;
	hwlaser_calibration_data_t *cali_data_local = NULL;
	hwlaser_status_t *status = NULL;
	int32_t online_state;

	if (!data) {
		mt380x_errmsg("laser data is null\n");
		return -EFAULT;
	}

	switch (cmd) {
	case HWLASER_IOCTL_POWERON:
		mt380x_infomsg("HWLASER_IOCTL_POWERON\n");
		mutex_lock(&data->work_mutex);
		rc = mt380x_func_tbl->power_on(data);
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_INIT:
		mt380x_infomsg("HWLASER_IOCTL_INIT\n");
		mutex_lock(&data->work_mutex);
		rc = mt380x_func_tbl->chip_init(data);
		if (rc == MT380X_ERROR_NONE)
			data->fwdl_status = 1;
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_SET_FREQ:
		mt380x_infomsg("HWLASER_IOCTL_SET_FREQ\n");
		rc = mt380x_laser_set_freq(data, arg);
		break;
	case HWLASER_IOCTL_PERFORM_CALIBRATION:
		mt380x_infomsg("HWLASER_IOCTL_PERFORM_CALIBRATION\n");
		hwlaser_ioctl_perform_calibration_t *cal_perform = NULL;

		cal_perform = (hwlaser_ioctl_perform_calibration_t *)arg;
		if (!cal_perform) {
			mt380x_errmsg("cal_perform is null\n");
			return -EFAULT;
		}
		rc = mt380x_perform_cal(data, cal_perform);
		break;
	case HWLASER_IOCTL_PARAMETER:
		mt380x_infomsg("HWLASER_IOCTL_PARAMETER\n");
		cali_data_local = (hwlaser_calibration_data_t *)arg;
		if (!cali_data_local) {
			mt380x_errmsg("cali_data_local is null\n");
			return -EFAULT;
		}
		mt380x_set_params(data, cali_data_local);
		break;
	case HWLASER_IOCTL_CALIBRATION_DATA:
		mt380x_infomsg("HWLASER_IOCTL_CALIBRATION_DATA\n");
		cali_data_local = (hwlaser_calibration_data_t *)arg;
		if (!cali_data_local) {
			mt380x_errmsg("cali_data_local is null\n");
			return -EFAULT;
		}
		rc = mt380x_laser_get_set_cal_data(data, cali_data_local);
		break;
	case HWLASER_IOCTL_START:
		mt380x_infomsg("HWLASER_IOCTL_START\n");
		mutex_lock(&data->work_mutex);
		mt380x_enable_irq(data);
		rc = mt380x_func_tbl->begin_continuous_measure(data);
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_MZ_DATA:
		mt380x_infomsg("HWLASER_IOCTL_GET_DATA\n");
		mutex_lock(&data->work_mutex);
		hwlaser_RangingData_t *p_ranging_data = NULL;

		p_ranging_data = (hwlaser_RangingData_t *)arg;
		if (!p_ranging_data) {
			mt380x_errmsg("p_ranging_data is null\n");
			return -EFAULT;
		}
		if (memcpy_s(&(p_ranging_data->m_data), sizeof(struct mt380x_measurement_data),
			&(data->range_data), sizeof(struct mt380x_measurement_data)) != EOK) {
			mt380x_errmsg("laser memcpy failed %d", __LINE__);
			mutex_unlock(&data->work_mutex);
			return -EFAULT;
		}
		mt380x_infomsg("milimeter(%d),status(%u)",
			p_ranging_data->m_data.milimeter,
			p_ranging_data->m_data.status);
		mt380x_infomsg("milimeter(%d), status(%u)",
			data->range_data.obj_range,
			data->range_data.range_status);
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_GET_INFO:
		mt380x_infomsg("HWLASER_IOCTL_GET_INFO\n");
		hwlaser_info_t *pinfo = NULL;

		pinfo = (hwlaser_info_t *)arg;
		if (!pinfo) {
			mt380x_errmsg("pinfo is null\n");
			return -EFAULT;
		}
		mt380x_get_laser_info(data, pinfo);
		break;
	case HWLASER_IOCTL_MATCHID: // device online=0, offline = 1
		mt380x_infomsg("HWLASER_IOCTL_MATCHID\n");
		mutex_lock(&data->work_mutex);
		online_state = mt380x_func_tbl->get_online_state(data);
		mt380x_infomsg("%d, online_state:%d\n", __LINE__, online_state);
		status = (hwlaser_status_t *)arg;
		if (!status) {
			mt380x_errmsg("status is null\n");
			return -EFAULT;
		}
		rc = mt380x_status(data, status);
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_STOP:
		mt380x_infomsg("HWLASER_IOCTL_STOP\n");
		mutex_lock(&data->work_mutex);
		mt380x_disable_irq(data);
		rc = mt380x_func_tbl->end_continuous_measure(data);
		mutex_unlock(&data->work_mutex);
		break;
	case HWLASER_IOCTL_POWEROFF:
		mt380x_infomsg("HWLASER_IOCTL_POWEROFF\n");
		mutex_lock(&data->work_mutex);
		mt380x_disable_irq(data);
		data->fwdl_status = 0;
		rc = mt380x_func_tbl->power_off(data);
		mutex_unlock(&data->work_mutex);
		break;
	default:
		rc = -EFAULT;
	}

	return rc;
}
EXPORT_SYMBOL(mt380x_ioctl);

static int mt380x_parse_dt(struct device_node *np, struct mt380x_data *data)
{
	enum of_gpio_flags flags;

	if (!data || !np)
		return -EINVAL;

	data->irq_gpio = of_get_named_gpio_flags(np, "mt380x,irq-gpio", 0, &flags);
	if (data->irq_gpio < 0) {
		mt380x_errmsg("get irq gpio: %d error\n", data->irq_gpio);
		return -ENODEV;
	}
	mt380x_dbgmsg("INT GPIO: %d\n", data->irq_gpio);

	data->xshut_gpio = of_get_named_gpio_flags(np, "mt380x,xshut-gpio", 0, &flags);
	if (data->xshut_gpio < 0) {
		mt380x_errmsg("get xshut gpio: %d error\n", data->xshut_gpio);
		return -ENODEV;
	}
	mt380x_dbgmsg("XSHUT GPIO: %d\n", data->xshut_gpio);

	return 0;
}

static int mt380x_setup(struct mt380x_data *data)
{
	int rc = 0;
	int irq = 0;
	uint8_t buf;

	if (!data)
		return -EINVAL;

	if (!gpio_is_valid(data->irq_gpio) || !gpio_is_valid(data->xshut_gpio))
		return -ENODEV;

	gpio_request(data->xshut_gpio, "mt380x xshut gpio");
	gpio_request(data->irq_gpio, "mt380x irq gpio");
	gpio_direction_input(data->irq_gpio);
	irq = gpio_to_irq(data->irq_gpio);
	if (irq < 0) {
		mt380x_errmsg("fail to map GPIO: %d to INT: %d\n", data->irq_gpio, irq);
		rc = -EINVAL;
		goto exit_free_gpio;
	} else {
		mt380x_dbgmsg("request irq: %d\n", irq);
		rc = request_threaded_irq(irq, NULL, mt380x_irq_handler,
			IRQF_TRIGGER_FALLING | IRQF_ONESHOT, "mt380x_interrupt", (void *)data);
		if (rc) {
			mt380x_errmsg("%d, Could not allocate MT380X_INT ! result:%d\n",
				__LINE__, rc);
			goto exit_free_gpio;
		}
	}
	data->irq = irq;
	data->intr_state = MT380X_INTR_DISABLED;
	disable_irq(data->irq);
	data->fwdl_status = 0;
	mt380x_func_tbl = &mt380x_api_func_tbl;
	mt380x_setup_api_functions();
	mt380x_func_tbl->power_on(data);
	mt380x_read_byte(data, MT380X_REG_DEV_ADDR, &buf);
	mt380x_func_tbl->power_off(data);
	if (buf != MT380X_CHIP_ADDR) {
		mt380x_errmsg("MT380x I2C Transfer Failed\n");
		rc = -EFAULT;
		goto exit_free_irq;
	}
	mt380x_infomsg("MT380x I2C Transfer Successfully\n");

	data->input_dev = input_allocate_device();
	if (!data->input_dev) {
		mt380x_errmsg("Error allocating input_dev.\n");
		goto input_dev_alloc_err;
	}
	data->input_dev->name = "mt380x";
	data->input_dev->id.bustype = BUS_I2C;
	input_set_drvdata(data->input_dev, data);
	set_bit(EV_ABS, data->input_dev->evbit);
	input_set_abs_params(data->input_dev, ABS_HAT0Y, 0, 0xffffffff, 0, 0);
	input_set_abs_params(data->input_dev, ABS_HAT1X, 0, 0xffffffff, 0, 0);
	input_set_abs_params(data->input_dev, ABS_HAT1Y, 0, 0xffffffff, 0, 0);
	input_set_abs_params(data->input_dev, ABS_BRAKE, 0, 0xffff, 0, 0);
	input_set_abs_params(data->input_dev, ABS_TILT_X, 0, 0xffff, 0, 0);
	input_set_abs_params(data->input_dev, ABS_WHEEL, 0, 0xffff, 0, 0);
	rc = input_register_device(data->input_dev);
	if (rc) {
		mt380x_errmsg("Error registering input_dev\n");
		goto input_reg_err;
	}
	rc = sysfs_create_group(&data->input_dev->dev.kobj, &mt380x_attr_group);
	if (rc) {
		mt380x_errmsg("Error creating sysfs attribute group\n");
		goto sysfs_create_group_err;
	}
	rc = sysfs_create_bin_file(&data->input_dev->dev.kobj, &mt380x_crosstalk_data_attr);
	if (rc) {
		rc = -ENOMEM;
		mt380x_errmsg("%d error:%d\n", __LINE__, rc);
		goto sysfs_create_bin_err1;
	}
	rc = sysfs_create_bin_file(&data->input_dev->dev.kobj, &mt380x_offset_data_attr);
	if (rc) {
		rc = -ENOMEM;
		mt380x_errmsg("%d error:%d\n", __LINE__, rc);
		goto sysfs_create_bin_err2;
	}

	data->period = 30;
	data->crosstalk_position_config = 0;
	data->crosstalk_ratio_config = 0;
	data->offset_config = 0;
	data->debug_enable = 1;
	data->range_data.range_status = 255;
#ifdef CONFIG_HUAWEI_HW_DEV_DCT
	set_hw_dev_flag(DEV_I2C_LASER);
#endif
	return 0;

sysfs_create_bin_err2:
	sysfs_remove_bin_file(&data->input_dev->dev.kobj,
		&mt380x_crosstalk_data_attr);
sysfs_create_bin_err1:
	sysfs_remove_group(&data->input_dev->dev.kobj,
		&mt380x_attr_group);
sysfs_create_group_err:
	input_unregister_device(data->input_dev);
input_reg_err:
	input_free_device(data->input_dev);
input_dev_alloc_err:
exit_free_irq:
	free_irq(irq, data);
exit_free_gpio:
	gpio_free(data->xshut_gpio);
	gpio_free(data->irq_gpio);
	return rc;
}

int mt380x_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct mt380x_data *mt380x_data = NULL;
	struct device *dev = &client->dev;
	struct device_node *node = NULL;
	hw_laser_ctrl_t *ctrl = NULL;
	int ret = 0;

	mt380x_infomsg("mt380x Enter\n");
	mt380x_data = kzalloc(sizeof(struct mt380x_data), GFP_KERNEL);
	if (!mt380x_data) {
		mt380x_errmsg("mt380x devm_kzalloc error\n");
		return -ENOMEM;
	}
	if (!dev->of_node) {
		kfree(mt380x_data);
		mt380x_errmsg("mt380x dev->of_node error\n");
		return -EINVAL;
	}

	mt380x_data->dev_name = dev_name(&client->dev);
	mt380x_data->client = client;
	mt380x_data->dev = dev;
	ctrl = (hw_laser_ctrl_t *)id->driver_data;
	ctrl->data = (void *)mt380x_data;
	node = dev->of_node;
	i2c_set_clientdata(client, mt380x_data);
	mutex_init(&mt380x_data->work_mutex);
	ret = mt380x_parse_dt(node, mt380x_data);
	if (ret) {
		mt380x_errmsg("MT380x Parse DT Failed\n");
		goto exit_error;
	}
	ret = mt380x_setup(mt380x_data);
	if (ret) {
		mt380x_errmsg("MT380x Setup Failed\n");
		goto exit_error;
	}
	return 0;

exit_error:
	mutex_destroy(&mt380x_data->work_mutex);
	i2c_set_clientdata(client, NULL);
	kfree(mt380x_data);
	return ret;
}
EXPORT_SYMBOL(mt380x_probe);

int mt380x_remove(struct i2c_client *client)
{
	struct mt380x_data *data = i2c_get_clientdata(client);

	if (!data)
		return -EINVAL;

	if (!IS_ERR(data->miscdev.this_device) &&
		data->miscdev.this_device) {
		mt380x_dbgmsg("mt380x to unregister misc dev\n");
		misc_deregister(&data->miscdev);
	}
	if (data->input_dev) {
		mt380x_dbgmsg("mt380x to unregister sysfs dev\n");
		sysfs_remove_group(&data->input_dev->dev.kobj, &mt380x_attr_group);
		sysfs_remove_bin_file(&data->input_dev->dev.kobj,
			&mt380x_crosstalk_data_attr);
		sysfs_remove_bin_file(&data->input_dev->dev.kobj,
			&mt380x_offset_data_attr);
		mt380x_dbgmsg("to unregister input dev\n");
		input_unregister_device(data->input_dev);
	}
	if (data->xshut_gpio) {
		gpio_direction_output(data->xshut_gpio, 0);
		gpio_free(data->xshut_gpio);
	}
	if (data->irq_gpio) {
		free_irq(data->irq, data);
		gpio_free(data->irq_gpio);
	}
	misc_deregister(&data->miscdev);
	i2c_set_clientdata(client, NULL);
	mutex_destroy(&data->work_mutex);
	kfree(data);
	return 0;
}
EXPORT_SYMBOL(mt380x_remove);
