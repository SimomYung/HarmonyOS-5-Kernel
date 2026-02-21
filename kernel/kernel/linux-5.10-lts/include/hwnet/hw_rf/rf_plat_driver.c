/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to start the driver peripheral.
 */

#include <securec.h>
#include <log/hw_log.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/kthread.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/poll.h>
#include <linux/reboot.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/version.h>

#include "rf_plat_driver.h"
#include "vcom/vcom.h"
#include "satellite/rf_satellite_ctrl.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG HW_RF_PLAT_DEV
HWLOG_REGIST();

#define DEVICE_REG_TRY_MAX_COUNT 100
#define DEVICE_REG_SLEEP_INTERVAL 1000

static struct platform_device *g_rf_device = NULL;

static struct ps_core_s *g_rf_core_s = NULL;
static struct ps_core_s *get_rf_core_data(void)
{
	return g_rf_core_s;
}

int32_t rf_init_gpio(int32_t gpio_id, char *gpio_name, int32_t status, int32_t gpio_direction)
{
	int32_t rc;

	hwlog_info("%s: enter, gpio name: %s, id: %d", __func__, gpio_name, gpio_id);

	if (!gpio_is_valid(gpio_id)) {
		hwlog_err("%s: gpio %s not configured in dts\n", __func__, gpio_name);
		return 0;
	}

	rc = gpio_request(gpio_id, gpio_name);
	if (rc) {
		hwlog_err("%s: gpio %d failed %d\n",
			__func__, gpio_id, rc);
		return rc;
	}

	rc = gpio_get_value(gpio_id);
	hwlog_info("%s: get gpio %d value %d",
		__func__, gpio_id, rc);

	if (gpio_direction == GPIO_DIRECTION_INPUT) {
		rc = gpio_direction_input(gpio_id);
		if (rc) {
			hwlog_err("%s: input gpio %d input high err %d",
				__func__, gpio_id, rc);
			return rc;
		}
	} else {
		rc = gpio_direction_output(gpio_id, status);
		if (rc) {
			hwlog_err("%s: output gpio %d output high err %d",
				__func__, gpio_id, rc);
			return rc;
		}
	}

	rc = gpio_get_value(gpio_id);
	hwlog_info("%s: get value gpio %d value %d",
		__func__, gpio_id, rc);

	return 0;
}

int32_t rf_fac_ioctl_proc(unsigned int cmd, int data)
{
	int32_t ret = -1;
	struct ps_core_s *cd = NULL;

	hwlog_err("%s: enter", __func__);
	cd = get_rf_core_data();
	if (cd == NULL)
		hwlog_err("%s: core data is null", __func__);

	switch (cmd) {
	case RSMC_START_CMD_FAC:
		ret = rsmc_on_proc(cd);
		break;
	case RSMC_STOP_CMD_FAC:
		ret = rsmc_off_proc(cd);
		break;
	case TTS_START_CMD_FAC:
		ret = tts_on_proc(cd);
		break;
	case TTS_STOP_CMD_FAC:
		ret = tts_off_proc(cd);
		break;
	case GET_HKADC_VALUE_FAC:
		ret = get_hkadc_value(cmd, data);
		break;
	case GET_RF_BOARD_ID:
		ret = get_rf_boardid(cd);
		break;
	case FXZ_START_CMD_FAC:
		ret = tts_on_proc(cd);
		break;
	case FXZ_STOP_CMD_FAC:
		ret = tts_off_proc(cd);
		break;
	default:
		hwlog_info("%s invalid cmd type:%s\n", __func__, _IOC_TYPE(cmd));
		break;
	}

	return ret;
}

int32_t rf_release_ioctl_proc(unsigned int cmd, int data)
{
	int32_t ret = -1;
	struct ps_core_s *cd = NULL;

	hwlog_err("%s: enter", __func__);
	cd = get_rf_core_data();
	if (cd == NULL)
		hwlog_err("%s: core data is null", __func__);

	switch (cmd) {
	case RSMC_START_CMD_RELEASE:
		ret = rsmc_on_proc(cd);
		break;
	case RSMC_STOP_CMD_RELEASE:
		ret = rsmc_off_proc(cd);
		break;
	case TTS_START_CMD_RELEASE:
		ret = tts_on_proc(cd);
		break;
	case TTS_STOP_CMD_RELEASE:
		ret = tts_off_proc(cd);
		break;
	case FXZ_START_CMD_RELEASE:
		ret = tts_on_proc(cd);
		break;
	case FXZ_STOP_CMD_RELEASE:
		ret = tts_off_proc(cd);
		break;
	default:
		hwlog_info("%s invalid cmd type:%s\n", __func__, _IOC_TYPE(cmd));
		break;
	}

	return ret;
}

static int32_t rf_parse_dts_config(struct device_node *dev_node,
	struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);

	if ((dev_node == NULL) || (cd == NULL)) {
		hwlog_info("%s: input null\n", __func__);
		return -1;
	}

	rf_sateliete_parse_dts_config(dev_node,	cd);
	cd->dev_node = dev_node;

	return 0;
}

static int32_t rf_ps_probe(struct platform_device *pdev)
{
	struct ps_core_s *ps_plat_d = NULL;
	int32_t err;

	hwlog_info("%s, enter\n", __func__);
	g_rf_device = pdev;
	ps_plat_d = kzalloc(sizeof(struct ps_core_s), GFP_KERNEL);
	if (ps_plat_d == NULL) {
		hwlog_info("%s, no mem to allocate\n", __func__);
		return -1;
	}
	err = rf_parse_dts_config(pdev->dev.of_node, ps_plat_d);
	if (err != 0) {
		hwlog_info("%s, rf_parse_dts_config failed\n", __func__);
		goto err_core_init;
	}
	ps_plat_d->dev = &pdev->dev;
	dev_set_drvdata(&pdev->dev, ps_plat_d);

	hwlog_info("%s, parse dts done\n", __func__);

	/* get reference of pdev */
	ps_plat_d->pm_pdev = pdev;

	g_rf_core_s = ps_plat_d;

	hwlog_info("%s is success!\n", __func__);
	return 0;

err_core_init:
	kfree(ps_plat_d);

	return -1;
}

static int32_t rf_ps_remove(struct platform_device *pdev)
{
	struct ps_core_s *ps_plat_d = NULL;

	hwlog_info("%s, device removed\n", __func__);
	ps_plat_d = dev_get_drvdata(&pdev->dev);
	if (ps_plat_d == NULL) {
		hwlog_info("%s, ps_plat_d is null\n", __func__);
		return -1;
	}
	return 0;
}

static int32_t hw_rf_driver_init(void)
{
	int32_t ret;
	struct ps_core_s *cd = NULL;

	hwlog_err("%s: enter", __func__);

	cd = get_rf_core_data();
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	ret = rf_satelite_config_init(cd);
	if (ret != 0)
		return ret;

	return 0;
}

static struct of_device_id g_rf_match_table[] = {
	{
		.compatible = DTS_RF_COMP_NAME,
		.data = NULL,
	},
	{},
};

static struct platform_driver g_rf_platform_driver = {
	.probe = rf_ps_probe,
	.remove = rf_ps_remove,
	.driver = {
		.name = "huawei_rf",
		.owner = THIS_MODULE,
		.of_match_table = g_rf_match_table,
	},
};

int32_t hw_rf_dev_init(void)
{
	int32_t ret;
	struct ps_core_s *cd = NULL;
	int32_t count = 0;

	hwlog_info("%s, enter\n", __func__);
	ret = platform_driver_register(&g_rf_platform_driver);
	if (ret) {
		hwlog_info("%s, Unable to register platform rf driver.\n", __func__);
		return -1;
	}
	cd = get_rf_core_data();
	while (cd == NULL && count < DEVICE_REG_TRY_MAX_COUNT) {
		msleep(DEVICE_REG_SLEEP_INTERVAL);
		count++;
		cd = get_rf_core_data();
	}
	hwlog_info("%s, count:%d.\n", __func__, count);

	ret = hw_rf_driver_init();
	if (ret) {
		hwlog_info("%s, driver init error.\n", __func__);
		return -1;
	}
	hwlog_info("%s done, ret:%d \n", __func__, ret);

	return ret;
}

void hw_rf_dev_exit(void)
{
	platform_driver_unregister(&g_rf_platform_driver);
}

MODULE_DESCRIPTION("Public serial Driver for multiple rf device");
MODULE_LICENSE("GPL");