#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

#include <asm-generic/gpio.h>

#include <linux/atomic.h>

#include <platform_include/camera/native/pvled_cfg.h>

#include <linux/platform_device.h>
#include <platform_include/cee/linux/ulsr.h> 

#define LED_COUNT 1

struct led_dev {
	dev_t devid;
	int major;
	int minor;
	struct cdev cdev;
	struct class *class;
	struct device *device;
	struct device_node *node;
	atomic_t status;
	int led_gpio;
	bool inited;
};

static struct led_dev camera_pv_led;


static int led_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int led_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int32_t value =0 ;
	pr_info("PVLED ioctl used.\n");

	switch (cmd) {
	case KERNEL_PVLED_ON:
		gpio_set_value(camera_pv_led.led_gpio, 1);
		atomic_set(&(camera_pv_led.status), 1);
		pr_info("PVLED ioctl on.\n");
		break;
	case KERNEL_PVLED_OFF:
		gpio_set_value(camera_pv_led.led_gpio, 0);
		atomic_set(&(camera_pv_led.status), 0);
		pr_info("PVLED ioctl off.\n");
		break;
	default:
		pr_err("PVLED ioctl not found.\n");
		return -ENOTTY;
	}
	return 0;
}

static int led_resume(struct device *dev)
{
	if (camera_pv_led.inited && is_ulsr()) {
		gpio_direction_output(camera_pv_led.led_gpio, 0);
	}

	return 0;
}

static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,
	.unlocked_ioctl = led_ioctl,
};

static int led_probe(struct platform_device *pdev)
{
	int ret = 0;
	camera_pv_led.inited = false;

	pr_info("PVLED INIT.\n");

	atomic_set(&(camera_pv_led.status), 0);

	camera_pv_led.major = 0;
	ret = alloc_chrdev_region(&camera_pv_led.devid, 0, LED_COUNT, "pvled");
	camera_pv_led.major = MAJOR(camera_pv_led.devid);
	camera_pv_led.minor = MINOR(camera_pv_led.devid);

	if (ret < 0) {
		goto failed_devid;
	}

	camera_pv_led.cdev.owner = THIS_MODULE;
	cdev_init(&camera_pv_led.cdev, &led_fops);
	ret = cdev_add(&camera_pv_led.cdev, camera_pv_led.devid, LED_COUNT);
	if (ret < 0) {
		goto failed_cdev;
	}

	camera_pv_led.class = class_create(THIS_MODULE, "pvledfs");
	if (IS_ERR_OR_NULL(camera_pv_led.class)) {
		ret = PTR_ERR(camera_pv_led.class);
		goto failed_class;
	}

	camera_pv_led.device = device_create(camera_pv_led.class, NULL, camera_pv_led.devid, NULL, "pvled");
	if (IS_ERR_OR_NULL(camera_pv_led.device)) {
		ret = PTR_ERR(camera_pv_led.device);
		goto failed_device;
	}

	camera_pv_led.node = of_find_node_by_path("/huawei_pvled");
	if (camera_pv_led.node == NULL) {
		ret = -EINVAL;
		pr_err("PVLED NODE NOT FOUND.\n");
		goto failed_findnode;
	}

	camera_pv_led.led_gpio = of_get_named_gpio(camera_pv_led.node, "pvled-gpios", 0);
	if (!gpio_is_valid(camera_pv_led.led_gpio)) {
		pr_err("can't find led gpio\n");
		ret = -EINVAL;
		goto failed_findnode;
	}

	ret = gpio_request(camera_pv_led.led_gpio, "pvled");
	if (ret) {
		pr_err("Failed to request gpio \r\n");
		ret = -EINVAL;
		goto failed_findnode;
	}


	ret = gpio_direction_output(camera_pv_led.led_gpio, 0);
	if (ret < 0) {
		goto failed_setoutput;
	}

	camera_pv_led.inited = true;
	pr_info("PVLED INIT DONE.\n");

	return 0;

failed_setoutput:
	gpio_free(camera_pv_led.led_gpio);
failed_findnode:
	device_destroy(camera_pv_led.class, camera_pv_led.devid);
failed_device:
	class_destroy(camera_pv_led.class);
failed_class:
	cdev_del(&camera_pv_led.cdev);
failed_cdev:
	unregister_chrdev_region(camera_pv_led.devid, LED_COUNT);
failed_devid:
	return ret;
}

static int led_remove(struct platform_device *pdev)
{
	if (camera_pv_led.inited) {
		cdev_del(&camera_pv_led.cdev);
		unregister_chrdev_region(camera_pv_led.devid, LED_COUNT);
		gpio_free(camera_pv_led.led_gpio);
		device_destroy(camera_pv_led.class, camera_pv_led.devid);
	}
	return 0;
}

static const struct of_device_id led_dt_match[] = {
	{
		.compatible = "huawei,pvled",
	},
	{},
};

const struct dev_pm_ops led_dev_pm_ops = {
	.resume = led_resume,
};

static struct platform_driver led_driver = {
	.probe = led_probe,
	.driver = {
		.name = "pvled",
		.owner = THIS_MODULE,
		.of_match_table = led_dt_match,
		.pm = &led_dev_pm_ops,
	},
	.remove = led_remove,
};

static int __init led_init(void)
{
	return platform_driver_register(&led_driver);
}

static void __exit led_exit(void)
{
	platform_driver_unregister(&led_driver);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZYB");