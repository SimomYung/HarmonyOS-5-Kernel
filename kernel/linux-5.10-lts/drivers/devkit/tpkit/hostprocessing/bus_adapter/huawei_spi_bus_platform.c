/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#include "huawei_spi_bus_platform.h"
#if defined(CONFIG_HUAWEI_TS_KIT_3_0)
#include "../3_0/trace-events-touch.h"
#else
#define trace_touch(x...)
#endif
#include <linux/hwspinlock.h>
#include <hwspinlock_internal.h>
#include <linux/gpio.h>
#include <linux/module.h>

#define GET_HWLOCK_FAIL 0

int thp_bus_lock(struct thp_device *tdev)
{
	int ret;
	unsigned long time;
	unsigned long timeout;
	struct thp_core_data *cd = NULL;
	struct hwspinlock *hwlock = NULL;
	bool flag;

	flag = tdev && tdev->thp_core->multi_spi;
	if (flag)
		cd = thp_get_core_data(tdev->thp_core->panel_id);
	else
		cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	hwlock = cd->hwspin_lock;
	mutex_lock(&cd->spi_mutex);
	if (!cd->use_hwlock)
		return 0;

	timeout = jiffies + msecs_to_jiffies(THP_GET_HARDWARE_TIMEOUT);

	do {
		ret = hwlock->bank->ops->trylock(hwlock);
		if (ret == GET_HWLOCK_FAIL) {
			time = jiffies;
			if (time_after(time, timeout)) {
				thp_log_err("%s:get hardware_mutex timeout\n",
					__func__);
				mutex_unlock(&cd->spi_mutex);
				return -ETIME;
			}
		}
	} while (ret == GET_HWLOCK_FAIL);

	return 0;
}
EXPORT_SYMBOL(thp_bus_lock);

void thp_bus_unlock(struct thp_device *tdev)
{
	struct thp_core_data *cd = NULL;
	struct hwspinlock *hwlock = NULL;
	bool flag;

	flag = tdev && tdev->thp_core->multi_spi;
	if (flag)
		cd = thp_get_core_data(tdev->thp_core->panel_id);
	else
		cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	hwlock = cd->hwspin_lock;
	mutex_unlock(&cd->spi_mutex);
	if (cd->use_hwlock)
		hwlock->bank->ops->unlock(hwlock);
}
EXPORT_SYMBOL(thp_bus_unlock);

int thp_set_spi_max_speed(unsigned int speed, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc;

	cd->sdev->max_speed_hz = speed;

	thp_log_info("%s:set max_speed_hz %d\n", __func__, speed);
	rc = thp_bus_lock(cd->thp_dev);
	if (rc) {
		thp_log_err("%s: get lock failed\n", __func__);
		return rc;
	}
	if (thp_setup_spi(cd)) {
		thp_log_err("%s: set spi speed fail\n", __func__);
		rc = -EIO;
	}
	thp_bus_unlock(cd->thp_dev);
	return rc;
}
EXPORT_SYMBOL(thp_set_spi_max_speed);

int thp_setup_spi(struct thp_core_data *cd)
{
	int rc;

#ifdef CONFIG_HUAWEI_THP_MTK
	cd->mtk_spi_config.cs_setuptime =
		cd->thp_dev->timing_config.spi_sync_cs_low_delay_ns;
#endif
	rc = spi_setup(cd->sdev);
	if (rc) {
		thp_log_err("%s: spi setup fail\n", __func__);
		return rc;
	}

	return 0;
}
EXPORT_SYMBOL(thp_setup_spi);

int thp_spi_sync_alloc_mem(struct thp_core_data *cd)
{
	if (cd->spi_sync_rx_buf || cd->spi_sync_tx_buf) {
		thp_log_debug("%s: has requested memory\n", __func__);
		return 0;
	}

	cd->spi_sync_rx_buf = kzalloc(THP_SYNC_DATA_MAX, GFP_KERNEL);
	if (!cd->spi_sync_rx_buf) {
		thp_log_err("%s:spi_sync_rx_buf request memory fail\n",
			__func__);
		goto exit;
	}
	cd->spi_sync_tx_buf = kzalloc(THP_SYNC_DATA_MAX, GFP_KERNEL);
	if (!cd->spi_sync_tx_buf) {
		thp_log_err("%s:spi_sync_tx_buf request memory fail\n",
			__func__);
		kfree(cd->spi_sync_rx_buf);
		cd->spi_sync_rx_buf = NULL;
		goto exit;
	}
	return 0;

exit:
	return -ENOMEM;
}
EXPORT_SYMBOL(thp_spi_sync_alloc_mem);

int thp_spi_transfer(struct thp_core_data *cd,
	char *tx_buf, char *rx_buf, unsigned int len, unsigned int lock_status)
{
	int rc;
	bool no_need_spi_communication_in_suspend;
	struct spi_message msg;
	struct spi_device *sdev = cd->sdev;
	struct spi_transfer xfer = {
		.tx_buf = tx_buf,
		.rx_buf = rx_buf,
		.len = len,
		.delay_usecs = cd->thp_dev->timing_config.spi_transfer_delay_us,
	};

	no_need_spi_communication_in_suspend = (cd->suspended) && (!cd->need_work_in_suspend) &&
		(!cd->need_spi_communication_in_suspend);
	if (no_need_spi_communication_in_suspend) {
		thp_log_err("%s - suspended\n", __func__);
		return 0;
	}

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);
	if (lock_status == NEED_LOCK) {
		rc = thp_bus_lock(cd->thp_dev);
		if (rc < 0) {
			thp_log_err("%s:get lock failed:%d\n", __func__, rc);
			return rc;
		}
	}
	if (!cd->not_support_cs_control) {
		if (cd->panel_id == MAIN_TOUCH_PANEL)
			thp_spi_cs_set(GPIO_HIGH);
		else if (cd->panel_id == SUB_TOUCH_PANEL)
			thp_spi_cs_set_sub_panel(GPIO_HIGH);
	}
	rc = thp_spi_sync(sdev, &msg);
	if (lock_status == NEED_LOCK)
		thp_bus_unlock(cd->thp_dev);

	return rc;
}
EXPORT_SYMBOL(thp_spi_transfer);

long thp_spi_sync_common(const void __user *data,
	unsigned int lock_status, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_ioctl_spi_sync_data sync_data;
	struct thp_ioctl_spi_sync_data_compat sync_data_compat;
	int rc = 0;
	u8 *tx_buf = NULL;
	u8 *rx_buf = NULL;

	memset(&sync_data, 0, sizeof(sync_data));
	memset(&sync_data_compat, 0, sizeof(sync_data_compat));
	if (!data) {
		thp_log_err("%s: input parameter null\n", __func__);
		return -EINVAL;
	}

#if defined(CONFIG_TEE_TUI)
	if (tui_enable)
		return 0;
#endif

	if (cd->compat_flag == true) {
		if (copy_from_user(&sync_data_compat, data,
			sizeof(sync_data_compat))) {
			thp_log_err("Failed to copy_from_user\n");
			return -EFAULT;
		}
		sync_data.rx = compat_ptr(sync_data_compat.rx);
		sync_data.tx = compat_ptr(sync_data_compat.tx);
		sync_data.size = sync_data_compat.size;
	} else {
		if (copy_from_user(&sync_data, data,
			sizeof(sync_data))) {
			thp_log_err("Failed to copy_from_user\n");
			return -EFAULT;
		}
	}

	if (sync_data.size > THP_SYNC_DATA_MAX) {
		thp_log_err("sync_data.size out of range\n");
		return -EINVAL;
	}

	rc = thp_spi_sync_alloc_mem(cd);
	if (!rc) {
		rx_buf = cd->spi_sync_rx_buf;
		tx_buf = cd->spi_sync_tx_buf;
	} else {
		thp_log_err("%s:buf request memory fail\n", __func__);
		goto exit;
	}

	rc = copy_from_user(tx_buf, sync_data.tx, sync_data.size);
	if (rc) {
		thp_log_err("%s:copy in buff fail\n", __func__);
		goto exit;
	}
	if (lock_status == NEED_LOCK) {
		if (cd->thp_dev->ops->spi_transfer)
			rc = cd->thp_dev->ops->spi_transfer(cd->thp_dev, tx_buf,
				rx_buf, sync_data.size);
		else
			rc = thp_spi_transfer(cd, tx_buf, rx_buf,
				sync_data.size, lock_status);
	} else {
		rc = thp_spi_transfer(cd, tx_buf, rx_buf,
			sync_data.size, lock_status);
	}
	if (rc) {
		thp_log_err("%s: transfer error, ret = %d\n", __func__, rc);
		goto exit;
	}

	try_recovery_fake_panel_status(cd, tx_buf, rx_buf, sync_data.size);

	if (sync_data.rx) {
		rc = copy_to_user(sync_data.rx, rx_buf, sync_data.size);
		if (rc) {
			thp_log_err("%s:copy out buff fail\n", __func__);
			goto exit;
		}
	}

exit:
	return rc;
}
EXPORT_SYMBOL(thp_spi_sync_common);

int thp_set_spi_com_mode(struct thp_core_data *cd, u8 spi_com_mode)
{
	int rc;

	if (!cd) {
		thp_log_err("%s: tdev null\n", __func__);
		return -EINVAL;
	}

	if (spi_com_mode != SPI_DMA_MODE && spi_com_mode != SPI_POLLING_MODE) {
		thp_log_err("%s ->error mode\n", __func__);
		return -EINVAL;
	}
	cd->spi_config.pl022_spi_config.com_mode = spi_com_mode;
	cd->sdev->controller_data = &cd->spi_config.pl022_spi_config;
	rc = thp_setup_spi(cd);
	thp_log_info("%s rc->%d\n", __func__, rc);
	return rc;
}
EXPORT_SYMBOL(thp_set_spi_com_mode);

long thp_ioctl_spi_sync_in_suspend(const void __user *data,
	unsigned int lock_status, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc = 0;

	if ((!cd->need_spi_communication_in_suspend) || (!cd->suspended)) {
		thp_log_info("%s invalid called return\n", __func__);
		return 0;
	}
	thp_log_info("%s spi communication in panel %d suspend\n", __func__, panel_id);
	__pm_wakeup_event(cd->thp_wake_lock, jiffies_to_msecs(HZ));
	rc = thp_spi_sync_common(data, lock_status, panel_id);

	return rc;
}
EXPORT_SYMBOL(thp_ioctl_spi_sync_in_suspend);

long thp_ioctl_spi_sync(const void __user *data,
	unsigned int lock_status, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc = 0;

	thp_log_debug("%s: called\n", __func__);

	if (cd->suspended && (!cd->need_work_in_suspend)) {
		if (spi_sync_count_in_suspend < MAX_IOCTL_COUNT_IN_SUSPEND)
			thp_log_info("%s panel %d suspended return\n", __func__, panel_id);
		spi_sync_count_in_suspend++;
		return cd->spi_in_suspend_return_negative ? -EINVAL : 0;
	}
	rc = thp_spi_sync_common(data, lock_status, panel_id);
	return rc;
}
EXPORT_SYMBOL(thp_ioctl_spi_sync);

long thp_ioctl_set_spi_speed(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	unsigned long max_speed_hz = arg;
	int rc;

	if ((max_speed_hz == cd->sdev->max_speed_hz) || cd->spi_speed_mode)
		return 0;
	rc = thp_set_spi_max_speed(max_speed_hz, panel_id);
	if (rc)
		thp_log_err("%s: failed to set spi speed\n", __func__);
	return rc;
}
EXPORT_SYMBOL(thp_ioctl_set_spi_speed);

void thp_spi_cs_set(u32 control)
{
	int rc = 0;
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);
	struct thp_timing_config *timing = NULL;

	if (!cd || !cd->thp_dev) {
		thp_log_err("%s:no driver data", __func__);
		return;
	}

	timing = &cd->thp_dev->timing_config;
#ifndef CONFIG_HUAWEI_THP_MTK
	if (cd->not_support_cs_control)
		return;
	if (control == SSP_CHIP_SELECT) {
		rc = gpio_direction_output(cd->gpios.cs_gpio, control);
		ndelay(timing->spi_sync_cs_low_delay_ns);
		if (timing->spi_sync_cs_low_delay_us && (!is_fw_update))
			udelay(timing->spi_sync_cs_low_delay_us);
	} else {
		rc = gpio_direction_output(cd->gpios.cs_gpio, control);
		ndelay(timing->spi_sync_cs_hi_delay_ns);
	}
#else
	if (cd->support_pinctrl == 0) {
		thp_log_info("%s: not support pinctrl\n", __func__);
		return;
	}

	if (control == SSP_CHIP_SELECT) {
		pinctrl_select_state(cd->pctrl, cd->mtk_pinctrl.cs_low);
		ndelay(cd->thp_dev->timing_config.spi_sync_cs_low_delay_ns);
	} else {
		pinctrl_select_state(cd->pctrl, cd->mtk_pinctrl.cs_high);
		ndelay(cd->thp_dev->timing_config.spi_sync_cs_hi_delay_ns);
	}
#endif

	if (rc < 0)
		thp_log_err("%s:fail to set gpio cs", __func__);
}
EXPORT_SYMBOL(thp_spi_cs_set);

void thp_spi_cs_set_sub_panel(u32 control)
{
	int rc = 0;
	struct thp_core_data *cd = thp_get_core_data(SUB_TOUCH_PANEL);
	struct thp_timing_config *timing = NULL;

	if (!cd || !cd->thp_dev) {
		thp_log_err("%s:no driver data", __func__);
		return;
	}

	timing = &cd->thp_dev->timing_config;
#ifndef CONFIG_HUAWEI_THP_MTK
	if (cd->not_support_cs_control)
		return;
	if (control == SSP_CHIP_SELECT) {
		rc = gpio_direction_output(cd->gpios.cs_gpio, control);
		ndelay(timing->spi_sync_cs_low_delay_ns);
		if (timing->spi_sync_cs_low_delay_us && (!is_fw_update))
			udelay(timing->spi_sync_cs_low_delay_us);
	} else {
		rc = gpio_direction_output(cd->gpios.cs_gpio, control);
		ndelay(timing->spi_sync_cs_hi_delay_ns);
	}
#else
	if (cd->support_pinctrl == 0) {
		thp_log_info("%s: not support pinctrl\n", __func__);
		return;
	}

	if (control == SSP_CHIP_SELECT) {
		pinctrl_select_state(cd->pctrl, cd->mtk_pinctrl.cs_low);
		ndelay(cd->thp_dev->timing_config.spi_sync_cs_low_delay_ns);
	} else {
		pinctrl_select_state(cd->pctrl, cd->mtk_pinctrl.cs_high);
		ndelay(cd->thp_dev->timing_config.spi_sync_cs_hi_delay_ns);
	}
#endif

	if (rc < 0)
		thp_log_err("%s:fail to set gpio cs", __func__);
}
EXPORT_SYMBOL(thp_spi_cs_set_sub_panel);

int thp_spi_sync(struct spi_device *spi, struct spi_message *message)
{
	int ret;

	thp_log_debug("%s:enter\n", __func__);
	trace_touch(TOUCH_TRACE_SPI, TOUCH_TRACE_FUNC_IN, "thp");
	ret = spi_sync(spi, message);
	trace_touch(TOUCH_TRACE_SPI, TOUCH_TRACE_FUNC_OUT, "thp");
	thp_log_debug("%s:out\n", __func__);
	return ret;
}
EXPORT_SYMBOL(thp_spi_sync);

static const struct of_device_id g_thp_psoc_match_table[] = {
	{ .compatible = "huawei,thp", },
	{ },
};

static const struct of_device_id g_thp_psoc_match_table_sub[] = {
	{ .compatible = "huawei,thp_sub_screen", },
	{ },
};

static const struct spi_device_id g_thp_device_id[] = {
	{ "huawei_thp", 0 },
	{ "huawei_thp_sub_screen", 0 },
	{ }
};
MODULE_DEVICE_TABLE(spi, g_thp_device_id);

struct spi_driver g_thp_spi_driver[2] = {
	{
		.probe = thp_spi_probe,
		.remove = thp_spi_remove,
		.shutdown = thp_spi_shutdown,
		.id_table = g_thp_device_id,
		.driver = {
			.name = "huawei_thp",
			.owner = THIS_MODULE,
			.bus = &spi_bus_type,
			.of_match_table = g_thp_psoc_match_table,
		},
	},
	{
		.probe = thp_spi_probe,
		.remove = thp_spi_remove,
		.id_table = g_thp_device_id,
		.driver = {
			.name = "huawei_thp_sub_screen",
			.owner = THIS_MODULE,
			.bus = &spi_bus_type,
			.of_match_table = g_thp_psoc_match_table_sub,
		},
	}
};

int register_spi_driver(u32 panel_id)
{
	int rc;

	thp_log_info("%s: called\n", __func__);
	rc = spi_register_driver(&g_thp_spi_driver[panel_id]);
	if (rc != 0) {
		thp_log_info("%s: spi register driver fail, rc = %d\n", __func__, rc);
	}
	return rc;
}

int unregister_spi_driver(u32 panel_id)
{
	thp_log_info("%s: called\n", __func__);
	spi_unregister_driver(&g_thp_spi_driver[panel_id]);
	return 0;
}