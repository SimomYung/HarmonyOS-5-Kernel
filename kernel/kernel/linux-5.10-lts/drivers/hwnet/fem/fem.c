/*
 * fem.c
 *
 * fem driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "fem.h"
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_i2c.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_debug.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_devices_info.h>
#include <chipset_common/hwpower/common_module/power_printk.h>

#define HWLOG_TAG fem
HWLOG_REGIST();

static int fem_read_byte(struct fem_device_info *di, u8 reg, u8 *val)
{
	int ret;
	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}
	ret = power_i2c_u8_read_byte(di->client, reg, val);
	hwlog_info("%s: ret %d\n", __func__, ret);
	return ret;
}

static void fem_write_byte(struct fem_device_info *di, u8 reg, u8 value)
{
	int ret;
	if (!di) {
		hwlog_err("chip not init\n");
		return;
	}
	ret = power_i2c_u8_write_byte(di->client, reg, value);
	hwlog_info("%s: fem_write_byte ret %d\n", __func__, ret);
}

static void fem_set_vout(struct fem_device_info *di, u8 addr, unsigned int vset)
{
	u8 vset_reg;
	if (!di) {
		hwlog_err("chip not init\n");
		return;
	}
	if (vset < FEM_VOUT_SEL_MIN || vset > FEM_VOUT_SEL_MAX) {
		hwlog_err("%s: vset %u is out of range \n", __func__, vset);
		return;
	}
	hwlog_info("%s: devid 0x%x\n", __func__, di->dev_id);
	if (di->dev_id == FEM_DEVID_AU8310 || di->dev_id == FEM_DEVID_SGM62110) {
		vset_reg = (vset - FEM_VOUT_SEL_BASE1) / FEM_VOUT_SEL_STEP; 
	} else if (di->dev_id == FEM_DEVID_6160) {
		vset_reg = (vset - FEM_VOUT_SEL_BASE2) / FEM_VOUT_SEL_STEP; 
	} else {
		hwlog_err("devid not adapt\n");
	}
	fem_write_byte(di, addr, vset_reg);
}

static int fem_get_vout(struct fem_device_info *di, u8 addr, unsigned int *vol)
{
	u8 vset_reg;
	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	if (fem_read_byte(di, addr, &vset_reg))
		return -EIO;

	hwlog_info("%s: devid 0x%x\n", __func__, di->dev_id);
	if (di->dev_id == FEM_DEVID_AU8310 || di->dev_id == FEM_DEVID_SGM62110) {
		*vol = FEM_VOUT_SEL_BASE1 + vset_reg * FEM_VOUT_SEL_STEP;
	} else if (di->dev_id == FEM_DEVID_6160) {
		*vol = FEM_VOUT_SEL_BASE2 + vset_reg * FEM_VOUT_SEL_STEP;
	} else {
		hwlog_err("devid not adapt\n");
		return -EIO;
	}

	return 0;
}

static int fem_get_control(struct fem_device_info *di, unsigned int *vol)
{
	return fem_get_vout(di, FEM_CONTROL_ADDR, vol);
}

static void fem_set_control(struct fem_device_info *di, unsigned int vset)
{
	fem_set_vout(di, FEM_CONTROL_ADDR, vset);
}

static int fem_get_vout1(struct fem_device_info *di, unsigned int *vol)
{
	return fem_get_vout(di, FEM_VOUT1_ADDR, vol);
}

static void fem_set_vout1(struct fem_device_info *di, unsigned int vset)
{
	fem_set_vout(di, FEM_VOUT1_ADDR, vset);
}

static int fem_get_vout2(struct fem_device_info *di, unsigned int *vol)
{
	return fem_get_vout(di, FEM_VOUT2_ADDR, vol);
}

static void fem_set_vout2(struct fem_device_info *di, unsigned int vset)
{
	fem_set_vout(di, FEM_VOUT2_ADDR, vset);
}

static void fem_init_vout(struct fem_device_info *di)
{
	unsigned int reg_val;
	if (!di) {
		hwlog_err("chip not init\n");
		return;
	}

	// set default voltage
	hwlog_info("init const_vout is %d %d\n", di->const_vout1, di->const_vout2);
	fem_set_vout1(di, di->const_vout1);
	fem_set_vout2(di, di->const_vout2);

	// read status
	(void)fem_get_control(di, &reg_val);
	hwlog_info("fem_init_vout control reg_val %d\n", reg_val);
	(void)fem_get_vout1(di, &reg_val);
	hwlog_info("fem_init_vout FEM_VOUT1_ADDR reg_val %d\n", reg_val);
	(void)fem_get_vout2(di, &reg_val);
	hwlog_info("fem_init_vout FEM_VOUT2_ADDR reg_val %d\n", reg_val);
}

static int fem_get_device_id(struct fem_device_info *di)
{
	int ret;
	u8 reg_val;
	if (!di) {
		hwlog_err("chip not init\n");
		return -EIO;
	}

	ret = fem_read_byte(di, FEM_DEVID_ADDR, &reg_val);
	if (ret) {
		hwlog_err("%s: chek chip fail not adapt\n", __func__);
		return FEM_IC_BAD;
	}
	di->dev_id = (reg_val & FEM_DEVID_MASK) >> FEM_DEVID_SHIFT;
	hwlog_info("%s: device_id is 0x%x reg_val 0x%x\n", __func__, reg_val, di->dev_id);
	switch (di->dev_id) {
	case FEM_DEVID_AU8310:
	case FEM_DEVID_6160:
	case FEM_DEVID_SGM62110:
		hwlog_info("fem is good\n");
		return FEM_IC_GOOD;
	default:
		hwlog_err("no chip match, return ic bad\n");
		return FEM_IC_BAD;
	}
}

static void fem_parse_dts(struct device_node *np,
	struct fem_device_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		 "const_vout1", &di->const_vout1, FEM_VOUT1_SEL_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"const_vout2", &di->const_vout2, FEM_VOUT2_SEL_DEFAULT);
	(void)power_gpio_config_output(np,
		"gpio_enable", "gpio_enable", &di->gpio_no, 1);
}

#ifdef CONFIG_HUAWEI_POWER_DEBUG
static ssize_t fem_dbg_vout1_show(void *dev_data, char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}
	if (fem_get_vout1(di, &vout))
		return scnprintf(buf, size, "fail to get vout\n");

	return scnprintf(buf, size, "fem vout=%u\n", vout);
}

static ssize_t fem_dbg_vout1_store(void *dev_data, const char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}

	if (sscanf_s(buf, "%u", &vout) != 1) {
		hwlog_err("%s sscanf_s failed\n", __func__);
		return -EINVAL;
	}

	fem_set_vout1(di, vout);
	return size;
}

static ssize_t fem_dbg_vout2_show(void *dev_data, char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}
	if (fem_get_vout2(di, &vout))
		return scnprintf(buf, size, "fail to get vout\n");

	return scnprintf(buf, size, "fem vout=%u\n", vout);
}

static ssize_t fem_dbg_vout2_store(void *dev_data, const char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}

	if (sscanf_s(buf, "%u", &vout) != 1) {
		hwlog_err("%s sscanf_s failed\n", __func__);
		return -EINVAL;
	}

	fem_set_vout2(di, vout);
	return size;
}

static ssize_t fem_dbg_control_show(void *dev_data, char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}
	if (fem_get_control(di, &vout))
		return scnprintf(buf, size, "fail to get vout\n");

	return scnprintf(buf, size, "fem vout=%u\n", vout);
}

static ssize_t fem_dbg_control_store(void *dev_data, const char *buf, size_t size)
{
	struct fem_device_info *di = dev_data;
	unsigned int vout = 0;

	if (!buf || !di) {
		hwlog_err("buf or di is null\n");
		return -EINVAL;
	}

	if (sscanf_s(buf, "%u", &vout) != 1) {
		hwlog_err("%s sscanf_s failed\n", __func__);
		return -EINVAL;
	}

	fem_set_control(di, vout);
	return size;
}
#endif /* CONFIG_HUAWEI_POWER_DEBUG */

static void fem_register_power_debug(struct fem_device_info *di)
{
#ifdef CONFIG_HUAWEI_POWER_DEBUG
	power_dbg_ops_register("fem", "vout1", di,
		fem_dbg_vout1_show, fem_dbg_vout1_store);
	power_dbg_ops_register("fem", "vout2", di,
		fem_dbg_vout2_show, fem_dbg_vout2_store);
	power_dbg_ops_register("fem", "control", di,
		fem_dbg_control_show, fem_dbg_control_store);
#endif
}

static void fem_shutdown(struct i2c_client *client)
{
	struct fem_device_info *di = i2c_get_clientdata(client);
	if (!di)
		return;

	hwlog_err("%s: not adapt\n", __func__);
}

static int fem_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct device_node *np = NULL;
	struct fem_device_info *di = NULL;
	if (!client || !client->dev.of_node || !id) {
		hwlog_err("%s client error\n", __func__);
		return -ENODEV;
	}

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		hwlog_err("[%s,%d]: need I2C_FUNC_I2C\n", __func__, __LINE__);
		return -ENODEV;
	}

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di) {
		hwlog_err("%s: di error\n", __func__);
		return -ENOMEM;
	}
	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;

	fem_parse_dts(np, di);
	ret = fem_get_device_id(di);
	if (ret != FEM_IC_GOOD) {
		goto get_devid_fail;
	}

	// register debug
	fem_register_power_debug(di);
	fem_init_vout(di);
	i2c_set_clientdata(client, di);
	return 0;

get_devid_fail:
	devm_kfree(&client->dev, di);
	return -EINVAL;
}

static int fem_remove(struct i2c_client *client)
{
	struct fem_device_info *di = i2c_get_clientdata(client);
	if (!di)
		return -ENODEV;

	i2c_set_clientdata(client, NULL);
	return 0;
}

MODULE_DEVICE_TABLE(i2c, bst_fem);
static const struct of_device_id fem_of_match[] = {
	{
		.compatible = "huawei,bst_fem",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id fem_i2c_id[] = {
	{ "bst_fem", 0 },
	{},
};

static struct i2c_driver fem_driver = {
	.probe = fem_probe,
	.remove = fem_remove,
	.id_table = fem_i2c_id,
	.shutdown = fem_shutdown,
	.driver = {
	.owner = THIS_MODULE,
		.name = "bst_fem",
		.of_match_table = of_match_ptr(fem_of_match),
	},
};

static int __init fem_init(void)
{
	return i2c_add_driver(&fem_driver);
}

static void __exit fem_exit(void)
{
	i2c_del_driver(&fem_driver);
}

fs_initcall_sync(fem_init);
module_exit(fem_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fem module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
