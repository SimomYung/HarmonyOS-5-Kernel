/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implement of hiprost
 * Create: 2025-03-27
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/of_gpio.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <securec.h>
#include <cam_log.h>

enum hiprost_config_type {
    HIPROST_CONFIG_POWER_ON = 0,
    HIPROST_CONFIG_POWER_OFF,
};

struct hiprost_cfg {
    int cfgtype;
};
#define HIPROST_IOC_CFG _IOWR('H', 0, struct hiprost_cfg)


static DEFINE_MUTEX(shared_lock); // protect concurrent enable/disable power ref
static LIST_HEAD(shared_list);

struct power_setting;
typedef int (*power_config_t)(struct power_setting *, bool on);
typedef int (*power_enable_t)(struct power_setting *);
typedef void (*power_disable_t)(struct power_setting *);

struct power_setting {
	const char *name;
	uint32_t val_on;
	uint32_t val_off;
	uint32_t delay_on;
	uint32_t delay_off;
	void *private;

    power_enable_t enable;
    power_disable_t disable;
};

struct power_setting_array {
	struct mutex power_lock;
    uint32_t power_ref;
	unsigned int nsettings;
    struct power_setting* settings;
};

#define HIPROST_FILE_MAX_LEN 20
typedef struct _hiprost_device {
	struct miscdevice dev;
	atomic_t ref_count;
    struct power_setting_array pst_array;
} hiprost_device;

static int hiprost_open(struct inode *inode, struct file *file) {
	int ref_count;
	struct miscdevice *mdev = NULL;
	hiprost_device *hiprost_dev = NULL;

	(void)inode;
	if (!file)
		return -EFAULT;

	mdev = file->private_data;
	hiprost_dev = container_of(mdev, hiprost_device, dev);

	ref_count = atomic_inc_return(&hiprost_dev->ref_count);
	cam_info("%s: %s device opened, ref:%d",
			 __func__, mdev->name, ref_count);
	return 0;
}

static long hiprost_config(hiprost_device* hiprost_dev, struct hiprost_cfg* data);
static int hiprost_release(struct inode *inode, struct file *file) {
	int ref_count;
	hiprost_device *hiprost_dev = NULL;
	struct miscdevice *mdev = NULL;
	struct hiprost_cfg cfg = {0};

	(void)inode;
	if (!file)
		return 0;
	hiprost_dev = file->private_data;
	if (!hiprost_dev)
		return 0;
	mdev = &hiprost_dev->dev;
	ref_count = atomic_dec_return(&hiprost_dev->ref_count);
	if(ref_count == 0) {
		cfg.cfgtype = HIPROST_CONFIG_POWER_OFF;
		hiprost_config(hiprost_dev, &cfg);
	}

	cam_info("%s: %s device closed, ref:%d",
			__func__, mdev->name, ref_count);
	return 0;
}

static int hiprost_do_power_setting(struct power_setting *beg,
				   struct power_setting *end, bool on)
{
	uint32_t delay;
	int step = beg < end ? 1 : -1;
	struct power_setting *pset = beg;

	while (pset != end) {
		if (on) {
			cam_debug("hiprost power-seq-on: %s val:%u", pset->name,
				  pset->val_on);
			if (pset->enable(pset) != 0)
				goto power_up_fail;
			delay = pset->delay_on;
		} else {
			cam_debug("hiprost power-seq-off: %s val:%u", pset->name,
				  pset->val_off);
			pset->disable(pset);
			delay = pset->delay_off;
		}
        udelay(delay);
		pset += step;
	}
	return 0;

power_up_fail:
	cam_err("hiprost power-seq-on: %s val: %u failed", pset->name, pset->val_on);
	hiprost_do_power_setting(pset - step, beg - step, false);
	return -EINVAL;
}

static long hiprost_enable_power_setting(struct power_setting_array* pst)
{
	long ret = 0;
	struct power_setting* beg = pst->settings;
	struct power_setting* end = pst->settings + pst->nsettings;
	mutex_lock(&pst->power_lock);
	if (pst->power_ref == 0) {
		ret = hiprost_do_power_setting(beg, end, true);
		if (ret)
			goto err_out;
	}
	++pst->power_ref;
	cam_info("hiprost power_up: power-seq-on, ref: %u", pst->power_ref);
err_out:
	if (ret)
		cam_err("hiprost power_up: power-seq-on failed: %d", ret);
	mutex_unlock(&pst->power_lock);
	return ret;
}

static void hiprost_disable_power_setting(struct power_setting_array *pst)
{
	struct power_setting* beg = pst->settings + pst->nsettings - 1;
	struct power_setting* end = pst->settings - 1;

	mutex_lock(&pst->power_lock);
	if (pst->power_ref == 1) {
		hiprost_do_power_setting(beg, end, false);
	}
	if (pst->power_ref > 0)
		--pst->power_ref;
	cam_info("hiprost power_down: power-seq-off, ref:%u", pst->power_ref);
	mutex_unlock(&pst->power_lock);
}

static long hiprost_config(hiprost_device* hiprost_dev, struct hiprost_cfg* data)
{
	long ret = 0;
	cam_info("%s: cfgtype: %d", __func__, data->cfgtype);
	switch (data->cfgtype) {
		case HIPROST_CONFIG_POWER_ON:
			ret = hiprost_enable_power_setting(&hiprost_dev->pst_array);
			break;
		case HIPROST_CONFIG_POWER_OFF:
			hiprost_disable_power_setting(&hiprost_dev->pst_array);
			break;
		default:
			break;
	}
	return ret;
}

static long hiprost_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	long ret = 0;
	struct miscdevice *mdev = NULL;
	hiprost_device *hiprost_dev = NULL;
	struct hiprost_cfg data;
	if (!filp) {
		cam_err("%s: filp is NULL", __func__);
		return -EFAULT;
	}

	if (_IOC_SIZE(cmd) > sizeof(data)) {
		cam_err("%s: cmd size too large", __func__);
		return -EINVAL;
	}

	if (arg == 0) {
		cam_err("%s, arg for hiprost_cfg ioctl is NULL\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(&data, (void __user *)arg, _IOC_SIZE(cmd)) != 0) {
		cam_err("%s: copy in arg failed", __func__);
		return -EFAULT;
	}

	mdev = filp->private_data;
	hiprost_dev = container_of(mdev, hiprost_device, dev);

	cam_info("%s: cmd: %u", __func__, cmd);
	switch (cmd) {
		case HIPROST_IOC_CFG:
			ret = hiprost_config(hiprost_dev, &data);
			break;
		default:
			break;
	}

	if (copy_to_user((void __user *)arg, &data, _IOC_SIZE(cmd)) != 0) {
		cam_err("%s: copy back arg failed", __func__);
		return -EFAULT;
	}
	return ret;
}

static const struct file_operations hiprost_fops = {
	.owner = THIS_MODULE,
	.open = hiprost_open,
	.release = hiprost_release,
	.unlocked_ioctl = hiprost_ioctl,
#if CONFIG_COMPAT
	.compat_ioctl = hiprost_ioctl,
#endif
};

enum devm_get_type {
	DEVM_GET_GPIO,
};

struct power_type_ops {
	const char *prefix;
	enum devm_get_type type;
	power_enable_t enable;
	power_disable_t disable;
};

struct shared_data {
	struct list_head list;
	uint32_t enable_count;
    const char* name;
	power_config_t custom_config;

    union {
        uint32_t gpio_num;
    };
};

static struct shared_data *get_shared_data(power_config_t config, const char* name)
{
	struct shared_data *data;

	list_for_each_entry(data, &shared_list, list) {
		if (data->custom_config != config)
			continue;
		if (strcmp(data->name, name) == 0)
			return data;
	}

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;

	data->name = name;
	data->custom_config = config;
	list_add_tail(&data->list, &shared_list);
	return data;
}

static int hiprost_power_enable(struct power_setting* pst)
{
	int ret = 0;
	struct shared_data *data = pst->private;

	mutex_lock(&shared_lock);
	if (++data->enable_count == 1)
		ret = data->custom_config(pst, true);

    if (ret)
        --data->enable_count;

	cam_debug("hiprost power-seq-on: %s enable count: %u ret: %d", pst->name,
					  data->enable_count, ret);
	mutex_unlock(&shared_lock);
	return ret;
}

static void hiprost_power_disable(struct power_setting* pst)
{
	struct shared_data *data = pst->private;

	mutex_lock(&shared_lock);
	if (data->enable_count == 1)
		data->custom_config(pst, false); // false: power off
	if (data->enable_count > 0)
		--data->enable_count;
	cam_debug("hiprost power-seq-off: %s enable count: %u", pst->name,
		  data->enable_count);
	mutex_unlock(&shared_lock);
}

static struct power_type_ops *find_generic_ops(const char *name)
{
    static struct power_type_ops generic_typeops[] = {
        {"gpio-", DEVM_GET_GPIO, hiprost_power_enable, hiprost_power_disable },
    };
	uint32_t i;
	struct power_type_ops *ops;

	for (i = 0; i < ARRAY_SIZE(generic_typeops); ++i) {
		ops = &generic_typeops[i];
		if (strncmp(ops->prefix, name, strlen(ops->prefix)) == 0)
			return ops;
	}
	return NULL;
}


#define POWER_SEQ_KEY "power-seq"
#define POWER_SEQ_VAL_ON_KEY "power-seq-val-on"
#define POWER_SEQ_VAL_OFF_KEY "power-seq-val-off"
#define POWER_SEQ_DELAY_ON_KEY "power-seq-delay-on"
#define POWER_SEQ_DELAY_OFF_KEY "power-seq-delay-off"
#define GPIO_CTRL_TYPES_KEY "gpio-ctrl-types"

static int hiprost_gpio_input_output(uint32_t gpio, int *val, const char *name,
				    bool input)
{
	int ret;

	if (!name)
		name = "unknown";

	ret = gpio_request(gpio, name);
	if (ret != 0) {
		cam_err("%s: fail to request gpio:%u for %s", __func__, gpio, name);
		return ret;
	}

	if (input) {
		ret = gpio_direction_input(gpio);
		*val = gpio_get_value(gpio);
	} else {
		ret = gpio_direction_output(gpio, *val);
	}

	if (ret != 0)
		cam_err("%s: fail to set gpio:%u as %s for %s", __func__, gpio,
			(input ? "input" : "output"), name);

	gpio_free(gpio);
	return ret;
}

static int hiprost_gpio_output(uint32_t gpio, int val, const char *name)
{
	return hiprost_gpio_input_output(gpio, &val, name, false);
}

static int hiprost_gpio_config(struct power_setting *pst, bool on)
{
	struct shared_data *data = pst->private;
	uint32_t output = on ? pst->val_on : pst->val_off;
	return hiprost_gpio_output(data->gpio_num, (int)output, pst->name);
}

static int hiprost_get_named_gpio(struct device *dev, const char *name)
{
	int idx;
    int cnt;
	int ret;
	const char *gpio_name = NULL;
    struct gpio_desc* gd = devm_gpiod_get(dev, name, GPIOD_ASIS);
    if (!IS_ERR_OR_NULL(gd)) {
        ret = desc_to_gpio(gd);
        devm_gpiod_put(dev, gd);
        return ret;
    }

	cnt = of_property_count_strings(dev->of_node, GPIO_CTRL_TYPES_KEY);
	for (idx = 0; idx < cnt; ++idx) {
		ret = of_property_read_string_index(
			dev->of_node, GPIO_CTRL_TYPES_KEY, idx, &gpio_name);
		if (ret < 0)
			return ret;
		if (strcmp(gpio_name, name) == 0)
			return of_get_gpio(dev->of_node, idx);
    }
    return -ENODATA;
}

static struct shared_data *devm_gpio_get(struct device *dev, const char *name)
{
    struct shared_data* data;
    int gpio = hiprost_get_named_gpio(dev, name);
	if (gpio <= 0) {
		dev_err(dev, "%pOF get gpio:%s failed", dev->of_node, name);
		return NULL;
	}
    dev_info(dev, "gpio %s num: %d", name, gpio);
	data = get_shared_data(hiprost_gpio_config, name);
	if (data)
		data->gpio_num = gpio;
    return data;
}

static bool parse_power_ops(struct device *dev, struct power_setting* pst)
{
	const char* suffix_name;
	struct power_type_ops *ops = find_generic_ops(pst->name);
	if (!ops) {
		dev_err(dev, "power setting seq-type: %s not matched", pst->name);
		return false;
	}
	pst->enable = ops->enable;
	pst->disable = ops->disable;
	suffix_name = pst->name + strlen(ops->prefix);
	switch (ops->type) {
		case DEVM_GET_GPIO:
			pst->private = devm_gpio_get(dev, suffix_name);
			break;
		default:
			break;
	}
	if (IS_ERR_OR_NULL(pst->private)) {
		dev_err(dev, "%pOF fail to get %s", dev->of_node, pst->name);
		return false;
	}
	return true;
}

static bool parse_power_setting(struct device *dev, struct power_setting* pst, uint idx)
{
	if (of_property_read_string_index(dev->of_node, POWER_SEQ_KEY, idx,
					  &pst->name)) {
		dev_err(dev, "%pOF get index:%u of %s failed", dev->of_node,
			idx, POWER_SEQ_KEY);
		return false;
	}

	if (of_property_read_u32_index(dev->of_node, POWER_SEQ_VAL_ON_KEY, idx,
				       &pst->val_on)) {
		dev_err(dev, "%pOF get index:%u of %s failed", dev->of_node,
			idx, POWER_SEQ_VAL_ON_KEY);
		return false;
	}
    (void)of_property_read_u32_index(dev->of_node, POWER_SEQ_VAL_OFF_KEY, idx, &pst->val_off);
    (void)of_property_read_u32_index(dev->of_node, POWER_SEQ_DELAY_ON_KEY, idx, &pst->delay_on);
    (void)of_property_read_u32_index(dev->of_node, POWER_SEQ_DELAY_OFF_KEY, idx, &pst->delay_off);
    dev_info(dev, "power setting %s valon: %u valoff: %u delayon: %u delayoff: %u",
            pst->name, pst->val_on, pst->val_off, pst->delay_on, pst->delay_off);

    return parse_power_ops(dev, pst);
}

static bool parse_power_setting_array(struct device *dev, struct power_setting_array* pst_array)
{
    int nsettings;
    nsettings = of_property_count_strings(dev->of_node, POWER_SEQ_KEY);
    if (nsettings <= 0) {
		dev_err(dev, "%pOF has no prop: %s", dev->of_node, POWER_SEQ_KEY);
        return false;
    }
    mutex_init(&pst_array->power_lock);
    pst_array->power_ref = 0;
    pst_array->nsettings = nsettings;
    pst_array->settings = devm_kzalloc(dev, nsettings * sizeof(struct power_setting), GFP_KERNEL);
	if (!pst_array->settings)
		return false;

	for (uint i = 0; i < pst_array->nsettings; ++i) {
        if (!parse_power_setting(dev, &pst_array->settings[i], i)) {
            return false;
        }
    }

    return true;
}

static int hiprost_platform_probe(struct platform_device *pdev)
{
    hiprost_device *hiprost_dev = NULL;
    int ret;

	if (pdev == NULL) {
		cam_err("%s:%d pdev is null.\n", __func__, __LINE__);
		return -EINVAL;
	}
	cam_info("%s enter %pOF", __func__, pdev->dev.of_node);

    hiprost_dev = devm_kzalloc(&pdev->dev, sizeof(hiprost_device), GFP_KERNEL);
    if (!hiprost_dev) {
		cam_err("%s:%d kzalloc failed\n", __func__, __LINE__);
		return -ENOMEM;
    }

	atomic_set(&hiprost_dev->ref_count, 0);
    if (!parse_power_setting_array(&pdev->dev, &hiprost_dev->pst_array)) {
		cam_err("%s:%d parse power setting failed\n", __func__, __LINE__);
		return -EINVAL;
    }

    hiprost_dev->dev = (struct miscdevice) {
        .minor = MISC_DYNAMIC_MINOR,
        .name = "hiprost",
        .fops = &hiprost_fops,
    };

    if ((ret=misc_register(&hiprost_dev->dev)) != 0) {
		cam_err("%s: failed to register misc device", __func__);
        return ret;
    }

	cam_info("%s: create %s success", __func__, hiprost_dev->dev.name);
	platform_set_drvdata(pdev, hiprost_dev);

	return 0;
}

static int hiprost_platform_remove(struct platform_device *pdev)
{
	cam_info("%s enter", __func__);
	hiprost_device *hiprost_dev = platform_get_drvdata(pdev);
	if (IS_ERR_OR_NULL(hiprost_dev)) {
		cam_err("%s: hiprost_dev is not inited", __func__);
		return -EINVAL;
	}
    misc_deregister(&hiprost_dev->dev);
	return 0;
}

static const struct of_device_id hiprost_dt_match[] = {
	{
		.compatible = "vendor,hiprost",
	},
	{},
};

MODULE_DEVICE_TABLE(of, hiprost_dt_match);

static struct platform_driver hiprost_platform_driver = {
	.driver = {
		.name           = "hiprost",
		.owner          = THIS_MODULE,
		.of_match_table = hiprost_dt_match,
	},

	.probe = hiprost_platform_probe,
	.remove = hiprost_platform_remove,
};

static int __init hiprost_init_module(void)
{
	cam_info("enter %s", __func__);
	return platform_driver_register(&hiprost_platform_driver);
}

static void __exit hiprost_exit_module(void)
{
	platform_driver_unregister(&hiprost_platform_driver);
}
//lint -restore

module_init(hiprost_init_module);
module_exit(hiprost_exit_module);
MODULE_DESCRIPTION("hiprost");
MODULE_LICENSE("GPL v2");

