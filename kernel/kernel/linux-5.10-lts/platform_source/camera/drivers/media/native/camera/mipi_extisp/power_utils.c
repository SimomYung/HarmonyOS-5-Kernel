#include <linux/types.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/gpio/consumer.h>
#include "power_utils.h"

#define utils_err(fmt, args...) \
do { \
	pr_err("[power_utils err] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define utils_info(fmt, args...) \
do { \
	pr_info("[power_utils info] %s: %d " fmt, __func__, __LINE__, ## args); \
} while (0)

#define DELAY_THRESHOLD  2
#define MICROSECONDS_IN_ONE_MILLISECOND 1000
#define ADDITIONAL_DELAY 1000

static DEFINE_MUTEX(gpio_lock); // protect gpios list and enable ref
static LIST_HEAD(gpios);

struct power_setting {
	const char *name;
	uint32_t    val_on;
	uint32_t    val_off;
	uint32_t    delay;

	void       *private;
	int (*enable)(struct power_setting *set);
	int (*disable)(struct power_setting *set);
};

struct power_gpio_data {
	struct list_head entry;
	struct kref kref;
	uint32_t gpio;
	struct gpio_desc *desc;
	uint32_t enable_count;
};

static int power_get_seq_data(struct device *dev, int idx, struct power_setting *data)
{
    if (!dev || !data)
        return -EINVAL;

    if (of_property_read_string_index(dev->of_node,
        "power-seq-type", idx, &data->name)) {
        dev_err(dev, "OF index:%d seq type failed", idx);
        return -ENODATA;
    }
    if (of_property_read_u32_index(dev->of_node,
        "power-seq-val-on", idx, &data->val_on)) {
        dev_err(dev, "OF index:%d seq val on failed", idx);
        return -ENODATA;
    }
    if (of_property_read_u32_index(dev->of_node,
        "power-seq-val-off", idx, &data->val_off)) {
        dev_err(dev, "OF index:%d seq val off failed", idx);
        return -ENODATA;
    }
    if (of_property_read_u32_index(dev->of_node,
        "power-seq-delay", idx, &data->delay)) {
        dev_err(dev, "OF index:%d seq delay failed", idx);
        return -ENODATA;
    }
	return 0;
}

/* POWER GPIO SECTION */
static int power_gpio_enable(struct power_setting *setting)
{
	int ret = 0;
	struct power_gpio_data *data = NULL;

	if (!setting || !setting->private)
		return -EINVAL;
	data = setting->private;

    utils_info("name:%s, gpio:%d, val_on:%d, delay:%d, enable_count:%d .",
        setting->name, data->gpio, setting->val_on, setting->delay, data->enable_count);

	mutex_lock(&gpio_lock);
	if(!data){
		ret = -EINVAL;
		goto err_out;
	}
	if (data->enable_count == 0) {
		ret = gpio_request(data->gpio, setting->name);
		if (ret)
			goto err_out;

		ret = gpiod_direction_output(data->desc, setting->val_on);
		if (ret) {
			gpiod_put(data->desc);
			goto err_out;
		}
	}
	++data->enable_count;
err_out:
	mutex_unlock(&gpio_lock);
	return ret;
}

static int power_gpio_disable(struct power_setting *setting)
{
	int ret = 0;
	struct power_gpio_data *data = NULL;

	if (!setting || !setting->private)
		return -EINVAL;
	data = setting->private;

    utils_info("name:%s, gpio:%d, val_off:%d, delay:%d, enable_count:%d .",
        setting->name, data->gpio, setting->val_on, setting->delay, data->enable_count);

	mutex_lock(&gpio_lock);
	if (data->enable_count == 0) {
		utils_info("power_gpio_disable count 0");
		mutex_unlock(&gpio_lock);
		return 0;
	}

	--data->enable_count;
	if (data->enable_count == 0) {
		ret = gpiod_direction_output(data->desc, setting->val_off);
		utils_info("power off seq gpio:%d 2 ret:%d", data->gpio, ret);
		gpio_free(data->gpio);
	}
	mutex_unlock(&gpio_lock);
	return ret;
}

static void release_power_gpio_data(struct kref *ref)
{
	struct power_gpio_data *data = container_of(ref, struct power_gpio_data, kref);

	if(!data){
		utils_err("Invalid power_gpio_data pointer\n");
        	return;
	}
	mutex_lock(&gpio_lock);
	list_del(&data->entry);
	if (data->enable_count != 0) {
		data->enable_count = 0;
		gpio_free(data->gpio);
	}
	mutex_unlock(&gpio_lock);
	kfree(data);
}

static struct power_gpio_data *__gpio_data_get(struct gpio_desc *desc)
{
	struct power_gpio_data *data = NULL;

	list_for_each_entry(data, &gpios, entry) {
		if (data->desc == desc) {
			kref_get(&data->kref);
			return data;
		}
	}
	return NULL;
}

static struct power_gpio_data *__gpio_data_create(struct gpio_desc *desc)
{
	struct power_gpio_data *data = NULL;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;

	kref_init(&data->kref);
	data->desc = desc;
	data->gpio = desc_to_gpio(desc);
	list_add(&data->entry, &gpios);
	return data;
}

static struct power_gpio_data *gpio_data_get_or_create(struct device *dev, const char *name)
{
	struct gpio_desc *desc = NULL;
	struct power_gpio_data *data = NULL;

	mutex_lock(&gpio_lock);
	desc = gpiod_get(dev, name, 0);
	if (IS_ERR(desc)) {
		mutex_unlock(&gpio_lock);
		dev_err(dev, "OF get gpio:%s failed", name);
		return NULL;
	}

	data = __gpio_data_get(desc);
	if (!data)
		data = __gpio_data_create(desc);
	gpiod_put(desc);
	mutex_unlock(&gpio_lock);

	return data;
}

static void devm_gpio_release(struct device *dev, void *res)
{
	struct power_gpio_data *data = *(struct power_gpio_data **)res;

	kref_put(&data->kref, release_power_gpio_data);
}

static struct power_gpio_data *devm_gpio_get(struct device *dev, const char *name)
{
	struct power_gpio_data **ptr, *data = NULL;

	ptr = devres_alloc(devm_gpio_release, sizeof(**ptr), GFP_KERNEL);
	if (!ptr)
		return NULL;

	data = gpio_data_get_or_create(dev, name);
	if (data) {
		*ptr = data;
		devres_add(dev, ptr);
	} else {
		dev_err(dev, "OF get gpio:%s failed", name);
		devres_free(ptr);
	}

	return data;
}

/* POWER CLOCK SECTION */
static int power_clk_enable(struct power_setting *setting)
{
	int ret = 0;
	struct clk *clk = NULL;

	if (!setting || !setting->private)
		return -EINVAL;

	clk = setting->private;
	if (setting->val_on) { // incase some clk is fixed freq
		ret = clk_set_rate(clk, setting->val_on);
		if (ret)
			return ret;
	}
	return clk_prepare_enable(clk);
}

static int power_clk_disable(struct power_setting *setting)
{
	struct clk *clk = NULL;

	if (!setting || !setting->private)
		return -EINVAL;

	clk = setting->private;
	clk_disable_unprepare(clk);
	return 0;
}

/* POWER REGULAOTR SECTION */
static int power_regulator_enable(struct power_setting *setting)
{
	int ret = 0;
	struct regulator *supply = NULL;

	if (!setting || !setting->private)
		return -EINVAL;

	supply = setting->private;

	utils_info("name:%s, val_on:%d, delay:%d .",
        setting->name, setting->val_on, setting->delay);

	if (setting->val_on) {
		ret = regulator_set_voltage(supply, setting->val_on, setting->val_on);
		if (ret)
			return ret;
	}
	return regulator_enable(supply);
}

static int power_regulator_disable(struct power_setting *setting)
{
	struct regulator *supply = NULL;

	if (!setting || !setting->private)
		return -EINVAL;

	supply = setting->private;
	regulator_disable(supply);
	return 0;
}

enum devm_get_type {
	DEVM_GET_CLOCK,
	DEVM_GET_REGULATOR,
	DEVM_GET_GPIO,
};
struct power_type_ops {
	const char *prefix;
	enum devm_get_type type;
	int (*enable)(struct power_setting *);
	int (*disable)(struct power_setting *);
};

static struct power_type_ops typeops[] = {
	{ "gpio-",  DEVM_GET_GPIO,        power_gpio_enable,      power_gpio_disable },
	{ "clock-", DEVM_GET_CLOCK,       power_clk_enable,       power_clk_disable },
	{ "power-", DEVM_GET_REGULATOR,   power_regulator_enable, power_regulator_disable },
};

static struct power_type_ops *find_devget_ops(const char *name)
{
	uint32_t i = 0;
	struct power_type_ops *ops = NULL;

	for (i = 0; i < ARRAY_SIZE(typeops); ++i) {
		ops = &typeops[i];
		if (strncmp(ops->prefix, name, strlen(ops->prefix)) == 0)
			return ops;
	}
	return NULL;
}

static int parse_single_power_setting(struct device *dev, struct power_setting *setting)
{
	const char *suffix_name;
	struct power_type_ops *ops = find_devget_ops(setting->name);

	if (!ops) {
		dev_err(dev, "power setting seq-type: %s not matched",
			setting->name);
		return -EINVAL;
	}

	setting->enable = ops->enable;
	setting->disable = ops->disable;
	suffix_name = setting->name + strlen(ops->prefix);
	utils_info("suffix_name:%s .", suffix_name);
	switch (ops->type) {
	case DEVM_GET_CLOCK:
		setting->private = devm_clk_get(dev, suffix_name);
		break;
	case DEVM_GET_REGULATOR:
		setting->private = devm_regulator_get(dev, suffix_name);
		break;
	case DEVM_GET_GPIO:
		setting->private = devm_gpio_get(dev, suffix_name);
		break;
	default:
		setting->private = NULL;
		return -EFAULT;
	}
	if (IS_ERR_OR_NULL(setting->private)) {
		dev_err(dev, "OF fail to get %s", setting->name);
		return -EFAULT;
	}
	if(!setting->private){
		dev_err(dev, "setting->private is NULL");
        	return -EFAULT;
	}
	return 0;
}

int get_power_setting_utils(struct device *dev, struct power_setting_group *grp)
{
	int i, ret = 0;

	if (!dev || !grp)
		return -EINVAL;

	grp->nsettings = of_property_count_strings(dev->of_node, "power-seq-type");
	if (grp->nsettings <= 0) {
		dev_warn(dev, "OF no power-seq-type claimed");
		return 0;
	}

	grp->settings = devm_kzalloc(dev, grp->nsettings * sizeof(*grp->settings), GFP_KERNEL);
	if (!grp->settings)
		return -ENOMEM;

	for (i = 0; i < grp->nsettings; ++i) {
		ret = power_get_seq_data(dev, i, &grp->settings[i]);
		if (ret)
			return ret;

		ret = parse_single_power_setting(dev, &grp->settings[i]);
		if (ret)
			return ret;
	}

	return 0;
}

int put_power_setting_utils(struct device *dev, struct power_setting_group *grp)
{
	int i, ret = 0;

	if (!dev || !grp)
		return -EINVAL;

	dev_info(dev, "put_power_setting_utils enter");

    devm_kfree(dev, grp->settings);

	return 0;
}

void do_isp_rst_after_release(struct power_setting_group *grp)
{
	int i, ret = 0;
	struct power_setting *set = NULL;
	const char *reset_name = "gpio-reset";

	if (!grp)
		return;
	if (grp->nsettings <= 0) // no power setting, as success
		return;
	if (!grp->settings)
		return;

	for (i = grp->nsettings - 1; i >= 0; --i){
		set = &grp->settings[i];
		if (strncmp(set->name, reset_name, strlen(set->name)) == 0) {
			ret = set->disable(set);
			if (ret) {
				utils_err("power %s disable failed", set->name);
				return;
			}
			utils_info("name:%s set->delay: %d ret:%d", set->name, set->delay, ret);
			if (set->delay > DELAY_THRESHOLD)
				usleep_range(set->delay * MICROSECONDS_IN_ONE_MILLISECOND, set->delay * MICROSECONDS_IN_ONE_MILLISECOND + ADDITIONAL_DELAY);
			else if (set->delay != 0)
				mdelay(set->delay);
			break;
		}
	}
	utils_info("end.");
}

int enable_power_setting_utils(struct power_setting_group *grp)
{
	int i, ret = 0;
	struct power_setting *set = NULL;
	utils_info("enter.");

	if (!grp)
		return -EINVAL;
	if (grp->nsettings <= 0) { // no power setting, as success
		utils_err("no power settings");
		return 0;
	}
	if (!grp->settings)
		return -EINVAL;

	for (i = 0; i < grp->nsettings; ++i) {
		set = &grp->settings[i];
		ret = set->enable(set);
		if (ret) {
			utils_err("power %s enable failed", set->name);
			goto err_out;
		}
		utils_info("name:%s set->delay: %d ret:%d", set->name, set->delay, ret);
		if (set->delay > DELAY_THRESHOLD)
			usleep_range(set->delay * MICROSECONDS_IN_ONE_MILLISECOND, set->delay * MICROSECONDS_IN_ONE_MILLISECOND + ADDITIONAL_DELAY);
		else if (set->delay != 0)
			mdelay(set->delay);
	}
	return 0;

err_out:
	while (--i >= 0)
		grp->settings[i].disable(&grp->settings[i]);
	return ret;
}

void disable_power_setting_utils(struct power_setting_group *grp)
{
	int i, ret = 0;
	struct power_setting *set = NULL;

	if (!grp)
		return;
	if (grp->nsettings <= 0) // no power setting, as success
		return;
	if (!grp->settings)
		return;

	for (i = grp->nsettings - 1; i >= 0; --i){
		set = &grp->settings[i];
		ret = set->disable(set);
		if (ret) {
			utils_err("power %s disable failed", set->name);
			return;
		}
		utils_info("name:%s set->delay: %d ret:%d", set->name, set->delay, ret);
		if (set->delay > DELAY_THRESHOLD)
			usleep_range(set->delay * MICROSECONDS_IN_ONE_MILLISECOND, set->delay * MICROSECONDS_IN_ONE_MILLISECOND + ADDITIONAL_DELAY);
		else if (set->delay != 0)
			mdelay(set->delay);
	}
	utils_info("end.");
}

