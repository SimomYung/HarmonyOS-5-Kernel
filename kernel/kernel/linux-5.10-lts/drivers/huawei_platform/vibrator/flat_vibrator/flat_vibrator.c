/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: flat vibrator ctrl driver
 * Author: tianyunhang 30013707
 * Create: 2025/03/04
 */

#include <securec.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

extern volatile int vibrator_shake;

#define GPIO_VIBRATOR_TIMEOUT_MIN 30

/**
 * struct gpio_vibrator_data
 * @dev: device struct
 * @cdev: LED class device for this vibrator
 * @lock: resource lock
 * @vib_gpio: power supply
 */
struct gpio_vibrator_data {
	struct device *dev;
	struct led_classdev cdev;
	int vib_gpio;
};

static void gpio_vibrator_gpio_ctrl(struct led_classdev *cdev, enum led_brightness state)
{
	struct gpio_vibrator_data *vdata = container_of(cdev, struct gpio_vibrator_data, cdev);

	if (vdata == NULL) {
		pr_err("%s: vibrator data is NULL", __func__);
		return;
	}
	/* no blocking, the lock should not be used */
	if (state) {
		vibrator_shake = 1;
		gpio_direction_output(vdata->vib_gpio, 1);
		dev_info(vdata->dev, "flat vibrator gpio open\n");
	} else {
		vibrator_shake = 0;
		gpio_direction_output(vdata->vib_gpio, 0);
		dev_info(vdata->dev, "flat vibrator gpio close\n");
	}
}

static ssize_t gpio_vibrator_min_timeout_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	u32 val = GPIO_VIBRATOR_TIMEOUT_MIN;

	return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%d\n", val);
}

static DEVICE_ATTR(vibrator_min_timeout, 0444, gpio_vibrator_min_timeout_show, NULL);

static struct attribute *gpio_vb_attributes[] = {
	&dev_attr_vibrator_min_timeout.attr, NULL
};

static const struct attribute_group gpio_vb_attr_group = {
	.attrs = gpio_vb_attributes,
};

static const struct of_device_id gpio_vibrator_match[] = {
	{.compatible = "hisilicon,gpio-vibrator"}, {},
};

MODULE_DEVICE_TABLE(of, gpio_vibrator_match);

static int gpio_vibrator_register_led_classdev_gpio(struct gpio_vibrator_data *vdata)
{
	struct led_classdev *cdev = &vdata->cdev;

	cdev->name = "vibrator";
	cdev->flags = LED_CORE_SUSPENDRESUME;
	cdev->brightness_set = gpio_vibrator_gpio_ctrl;
	cdev->default_trigger = "transient";

	return devm_led_classdev_register(vdata->dev, cdev);
}

static int gpio_vibrator_get_gpio_ctrl(struct platform_device *pdev)
{
	struct device_node *dn = NULL;
	struct gpio_vibrator_data *vdata = NULL;
	int ret;

	vdata = devm_kzalloc(&pdev->dev, sizeof(struct gpio_vibrator_data), GFP_KERNEL);
	if (vdata == NULL)
		return -ENOMEM;

	vdata->dev = &pdev->dev;

	dn = vdata->dev->of_node;

	vdata->vib_gpio = of_get_named_gpio(dn, "vibrator_gpio", 0);
	dev_err(vdata->dev, "flat vibrator gpio value %d\n", vdata->vib_gpio);

	if (vdata->vib_gpio < 0) {
		dev_err(vdata->dev, "vibrator gpio is not found\n");
		return -ENODEV;
	}

	/* register led classdev, use "transient" as default trigger */
	ret = gpio_vibrator_register_led_classdev_gpio(vdata);
	if (ret) {
		dev_err(&pdev->dev, "failed to register led classdev\n");
		return ret;
	}

	ret = sysfs_create_group(&vdata->cdev.dev->kobj, &gpio_vb_attr_group);
	if (ret)
		dev_err(&pdev->dev, "unable create vibrator's min_timeout\n");

	platform_set_drvdata(pdev, vdata);

	return ret;
}

static int gpio_vibrator_probe(struct platform_device *pdev)
{
	struct gpio_vibrator_data *vdata = NULL;
	int ret;

	dev_info(&pdev->dev, "flat gpio vibrator probe");

	if (!of_match_node(gpio_vibrator_match, pdev->dev.of_node)) {
		dev_err(&pdev->dev, "dev node is not match. exiting.\n");
		return -ENODEV;
	}

	ret = gpio_vibrator_get_gpio_ctrl(pdev);
	if (!ret) {
		dev_info(&pdev->dev, "gpio vibrator gpio ctrl mode ret=%d\n", ret);
		return ret;
	}

	return 0;
}

static int gpio_vibrator_remove(struct platform_device *pdev)
{
	struct gpio_vibrator_data *vdata = NULL;

	vdata = dev_get_drvdata(&pdev->dev);
	if (vdata == NULL) {
		pr_err("%s:failed to get drvdata\n", __func__);
		return -ENODEV;
	}

	sysfs_remove_group(&vdata->cdev.dev->kobj, &gpio_vb_attr_group);

	return 0;
}

static struct platform_driver gpio_vibrator_driver = {
	.probe = gpio_vibrator_probe,
	.remove = gpio_vibrator_remove,
	.driver = {
		.name = "gpio-vibrator",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(gpio_vibrator_match),
	},
};

static int __init gpio_vibrator_init(void)
{
	return platform_driver_register(&gpio_vibrator_driver);
}

static void __exit gpio_vibrator_exit(void)
{
	platform_driver_unregister(&gpio_vibrator_driver);
}

module_init(gpio_vibrator_init);
module_exit(gpio_vibrator_exit);

MODULE_DESCRIPTION("FLAT vibrator driver");
MODULE_LICENSE("GPL");

