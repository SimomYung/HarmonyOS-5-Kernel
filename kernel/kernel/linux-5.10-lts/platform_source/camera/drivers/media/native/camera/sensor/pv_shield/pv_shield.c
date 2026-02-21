// SPDX-License-Identifier: GPL-2.0
/* pv_shield.c
 *
 * camera_eeprom_module_adapter driver
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <asm-generic/gpio.h>
#include <linux/atomic.h>
#include <platform_include/camera/native/pvshield_cfg.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>

#define SHIELD_COUNT 1
#define DELAY_TIME   200

struct shield_dev {
	dev_t devid;
	int major;
	int minor;
	struct cdev cdev;
	struct class *class;
	struct device *device;
	struct device_node *node;
	atomic_t status;
	int shield_outa_gpio;
	int shield_outb_gpio;
	bool inited;
};

static struct shield_dev camera_pv_shield;
static struct delayed_work gpio_delayed_work;

static void gpio_work_handler(struct work_struct *work)
{
	if (!gpio_is_valid(camera_pv_shield.shield_outb_gpio)) {
		pr_err("outb gpio is illegal\n");
		return;
	}

	gpio_set_value(camera_pv_shield.shield_outb_gpio, 0);
	pr_info("gpio work done\n");
}

static int shield_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int shield_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long shield_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	pr_info("PVSHIELD ioctl used.\n");

	switch (cmd) {
	case KERNEL_PVSHIELD_ON:
		gpio_set_value(camera_pv_shield.shield_outb_gpio, 0);
		gpio_set_value(camera_pv_shield.shield_outa_gpio, 1);
		mdelay(DELAY_TIME);
		gpio_set_value(camera_pv_shield.shield_outa_gpio, 0);
		atomic_set(&(camera_pv_shield.status), 1);
		pr_info("PVSHIELD ioctl on.\n");
		break;
	case KERNEL_PVSHIELD_OFF:
		gpio_set_value(camera_pv_shield.shield_outa_gpio, 0);
		gpio_set_value(camera_pv_shield.shield_outb_gpio, 1);
		mdelay(DELAY_TIME);
		gpio_set_value(camera_pv_shield.shield_outb_gpio, 0);
		atomic_set(&(camera_pv_shield.status), 0);
		pr_info("PVSHIELD ioctl off.\n");
		break;
	default:
		pr_err("PVSHIELD ioctl not found.\n");
		return -ENOTTY;
	}
	return 0;
}

static int shield_gpio_init(struct shield_dev *pv_shield)
{
	int ret = 0;

	pv_shield->node = of_find_node_by_path("/huawei_pvshield");
	if (pv_shield->node == NULL) {
		pr_err("PVSHIELD NODE NOT FOUND.\n");
		return -EINVAL;
	}

	pv_shield->shield_outa_gpio = of_get_named_gpio(pv_shield->node, "pvshield-gpios", 0);
	if (!gpio_is_valid(pv_shield->shield_outa_gpio)) {
		pr_err("can't find shield outa gpio\n");
		return -EINVAL;
	}

	ret = gpio_request(pv_shield->shield_outa_gpio, "pvshield_outa");
	if (ret) {
		pr_err("Failed to request shield_outb_gpio\n");
		return -EINVAL;
	}

	pv_shield->shield_outb_gpio = of_get_named_gpio(pv_shield->node, "pvshield-gpios", 1);
	if (!gpio_is_valid(camera_pv_shield.shield_outb_gpio)) {
		pr_err("can't find shield outb gpio\n");
		goto free_gpio_outa;
	}

	ret = gpio_request(pv_shield->shield_outb_gpio, "pvshield_outb");
	if (ret) {
		pr_err("Failed to request shield_outb_gpio\n");
		goto free_gpio_outa;
	}

	ret = gpio_direction_output(pv_shield->shield_outa_gpio, 0);
	if (ret < 0) {
		pr_err("Failed to set shield_outa_gpio\n");
		goto free_gpio_outb;
	}

	ret = gpio_direction_output(pv_shield->shield_outb_gpio, 1);
	if (ret < 0) {
		pr_err("Failed to set shield_outb_gpio\n");
		goto free_gpio_outb;
	}

	INIT_DELAYED_WORK(&gpio_delayed_work, gpio_work_handler);
	schedule_delayed_work(&gpio_delayed_work, msecs_to_jiffies(DELAY_TIME));
	return 0;

free_gpio_outb:
	gpio_free(pv_shield->shield_outb_gpio);
free_gpio_outa:
	gpio_free(pv_shield->shield_outa_gpio);
	return ret;
}

static int shield_resume(struct device *dev)
{
	return 0;
}

static const struct file_operations shield_fops = {
	.owner = THIS_MODULE,
	.open = shield_open,
	.release = shield_release,
	.unlocked_ioctl = shield_ioctl,
};

static int shield_probe(struct platform_device *pdev)
{
	int ret = 0;
	camera_pv_shield.inited = false;

	pr_info("PVSHIELD INIT.\n");
	atomic_set(&(camera_pv_shield.status), 0);

	camera_pv_shield.major = 0;
	ret = alloc_chrdev_region(&camera_pv_shield.devid, 0, SHIELD_COUNT, "pvshield");
	camera_pv_shield.major = MAJOR(camera_pv_shield.devid);
	camera_pv_shield.minor = MINOR(camera_pv_shield.devid);

	if (ret < 0)
		goto fail_devid;

	camera_pv_shield.cdev.owner = THIS_MODULE;
	cdev_init(&camera_pv_shield.cdev, &shield_fops);
	ret = cdev_add(&camera_pv_shield.cdev, camera_pv_shield.devid, SHIELD_COUNT);
	if (ret < 0)
		goto fail_cdev;

	camera_pv_shield.class = class_create(THIS_MODULE, "pvshieldfs");
	if (IS_ERR_OR_NULL(camera_pv_shield.class)) {
		ret = PTR_ERR(camera_pv_shield.class);
		goto fail_class;
	}

	camera_pv_shield.device = device_create(camera_pv_shield.class, NULL, camera_pv_shield.devid, NULL, "pvshield");
	if (IS_ERR_OR_NULL(camera_pv_shield.device)) {
		ret = PTR_ERR(camera_pv_shield.device);
		goto fail_device;
	}

	ret = shield_gpio_init(&camera_pv_shield);
	if (ret < 0)
		goto fail_findnode;

	camera_pv_shield.inited = true;
	pr_info("PVSHIELD INIT DONE.\n");

	return 0;

fail_findnode:
	device_destroy(camera_pv_shield.class, camera_pv_shield.devid);
fail_device:
	class_destroy(camera_pv_shield.class);
fail_class:
	cdev_del(&camera_pv_shield.cdev);
fail_cdev:
	unregister_chrdev_region(camera_pv_shield.devid, SHIELD_COUNT);
fail_devid:
	return ret;
}

static int shield_remove(struct platform_device *pdev)
{
	if (camera_pv_shield.inited) {
		cdev_del(&camera_pv_shield.cdev);
		unregister_chrdev_region(camera_pv_shield.devid, SHIELD_COUNT);
		cancel_delayed_work(&gpio_delayed_work);
		gpio_free(camera_pv_shield.shield_outa_gpio);
		gpio_free(camera_pv_shield.shield_outb_gpio);
		device_destroy(camera_pv_shield.class, camera_pv_shield.devid);
	}
	return 0;
}

static const struct of_device_id shield_dt_match[] = {
	{
		.compatible = "huawei,pvshield",
	},
	{},
};

const struct dev_pm_ops shield_dev_pm_ops = {
	.resume = shield_resume,
};

static struct platform_driver shield_driver = {
	.probe = shield_probe,
	.driver = {
		.name = "pvshield",
		.owner = THIS_MODULE,
		.of_match_table = shield_dt_match,
		.pm = &shield_dev_pm_ops,
	},
	.remove = shield_remove,
};

static int __init shield_init(void)
{
	return platform_driver_register(&shield_driver);
}

static void __exit shield_exit(void)
{
	platform_driver_unregister(&shield_driver);
}

module_init(shield_init);
module_exit(shield_exit);

MODULE_LICENSE("GPLV2");
MODULE_DESCRIPTION("pv_shield driver");
MODULE_AUTHOR("huawei Technologies Co., Ltd.");
