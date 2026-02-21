// SPDX-License-Identifier: GPL-2.0
/*
 * sc8886.c
 *
 * sc8886 boost driver
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

#include "sc8886.h"
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_pinctrl.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_i2c.h>
#include <chipset_common/hwpower/reverse_charge/reverse_charge_pd.h>

#define HWLOG_TAG sc8886
HWLOG_REGIST();

static int sc8886_write_byte(struct sc8886_device_info *di, u8 reg, u8 value)
{
	if (!di || (di->chip_already_init == 0)) {
		hwlog_err("chip not init\n");
		return -ENODEV;
	}

	return power_i2c_u8_write_byte(di->client, reg, value);
}

static int sc8886_read_byte(struct sc8886_device_info *di, u8 reg, u8 *value)
{
	if (!di || (di->chip_already_init == 0)) {
		hwlog_err("chip not init\n");
		return -ENODEV;
	}

	return power_i2c_u8_read_byte(di->client, reg, value);
}

static int sc8886_write_mask(struct sc8886_device_info *di, u8 reg, u8 mask, u8 shift, u8 value)
{
	int ret;
	u8 val = 0;

	ret = sc8886_read_byte(di, reg, &val);
	if (ret < 0)
		return ret;

	val &= ~mask;
	val |= ((value << shift) & mask);

	return sc8886_write_byte(di, reg, val);
}

static int sc8886_set_vcg_on(void *dev_data, int enable)
{
	struct sc8886_device_info *di = dev_data;

	if (!di)
		return -ENODEV;

	return 0;
}

static int sc8886_set_idle_mode(void *dev_data, int mode)
{
	struct sc8886_device_info *di = dev_data;

	if (!di)
		return -ENODEV;

	return 0;
}

static int sc8886_set_vbus_mv(void *dev_data, int vbus)
{
	int ret;
	u32 set_vbus;
	u8 vbus_reg_high;
	u8 vbus_reg_low;
	u8 flag[2] = {0}; /* 2: two regs */
	struct sc8886_device_info *di = dev_data;

	if (!di || (vbus < SC8666_OTG_VOL_RANGE_LOW) ||
		(vbus > SC8666_OTG_VOL_RANGE_HIGH))
		return -EINVAL;

	set_vbus = (u32)vbus / SC8886_OTG_VOLT_RESOLUTION;
	vbus_reg_high = ((set_vbus >> SC8886_OTG_VOLT_SET_LOW_BITS) &
		SC8886_OTG_VOLT_SET_HIGH_MASK);
	vbus_reg_low = ((set_vbus << SC8886_OTG_VOLT_SET_LOW_SHIFT) &
		SC8886_OTG_VOLT_SET_LOW_MASK);

	ret = sc8886_write_byte(di, SC8886_OTG_VOLT_SET_LOW, vbus_reg_low);
	ret += sc8886_write_byte(di, SC8886_OTG_VOLT_SET_HIGH, vbus_reg_high);
	if (ret)
		return -ENODEV;

	(void)sc8886_read_byte(di, SC8886_OTG_VOLT_SET_HIGH, &flag[0]);
	(void)sc8886_read_byte(di, SC8886_OTG_VOLT_SET_LOW, &flag[1]);
	hwlog_info("%s: reg[0x%x]=0x%x reg[0x%x]=0x%x\n", __func__,
		SC8886_OTG_VOLT_SET_HIGH, flag[0], SC8886_OTG_VOLT_SET_LOW, flag[1]);

	return 0;
}

static int sc8886_set_ibus_ma(void *dev_data, int ibus)
{
	int ret;
	u8 ibus_reg_high = 0;
	u8 flag = 0;
	u32 set_ibus;
	struct sc8886_device_info *di = dev_data;

	if (!di || (ibus <= 0))
		return -EINVAL;

	set_ibus = SC8886_REVERSE_IBUS_LIMIT / SC8886_OTG_CUR_RESOLUTION;
	ibus_reg_high = (set_ibus & SC8886_OTG_CUR_SET_HIGH_MASK);
	ret = sc8886_write_byte(di, SC8886_OTG_CUR_SET_HIGH, ibus_reg_high);
	if (ret)
		return -ENODEV;

	(void)sc8886_read_byte(di, SC8886_OTG_CUR_SET_HIGH, &flag);
	hwlog_info("%s: reg[0x%x]=0x%x\n", __func__, SC8886_OTG_CUR_SET_HIGH, flag);

	return 0;
}

static int sc8886_ic_enable(void *dev_data, int enable)
{
	int ret;
	struct sc8886_device_info *di = dev_data;

	if (!di)
		return -ENODEV;

	ret = gpio_direction_output(di->boost_en, enable);
	ret += sc8886_write_mask(di, SC8886_CHARGE_OP3_REG1, SC8666_ENABLE_OTG_MASK,
		SC8886_ENABLE_OTG_SHIFT, enable);
	ret += sc8886_write_mask(di, SC8886_CHARGE_OP1_REG1, SC8886_PTM_PINSEL_MASK,
		SC8886_PTM_PINSEL_SHIFT, enable);
	(void)power_msleep(DT_MSLEEP_10MS, 0, NULL);

	if (enable == 0) {
		ret += sc8886_write_mask(di, SC8886_CHARGE_OP1_REG1, SC8886_PTM_PINSEL_MASK,
			SC8886_PTM_PINSEL_SHIFT, 1);
		power_usleep(DT_USLEEP_10MS);
		ret += sc8886_write_mask(di, SC8886_CHARGE_OP1_REG1, SC8886_PTM_PINSEL_MASK,
			SC8886_PTM_PINSEL_SHIFT, 0);
	}

	return ret;
}

static int sc8886_enable_reverse_9v_pd_charge(bool enable, void *dev_data)
{
	struct sc8886_device_info *di = dev_data;
	int ret;

	if (!di)
		return -EPERM;

	if (enable) {
		ret = sc8886_set_vbus_mv(di, SC8886_REVERSE_9V);
		ret += sc8886_set_ibus_ma(di, SC8886_REVERSE_3A);
	} else {
		ret = sc8886_set_vbus_mv(di, SC8886_REVERSE_5V);
		ret += sc8886_set_ibus_ma(di, SC8886_REVERSE_3A);
	}

	return ret;
}

static struct rchg_pd_ic_ops g_sc8886_rc_pd_ops = {
	.dev_name = "sc8886",
	.enable_reverse_charge = sc8886_enable_reverse_9v_pd_charge,
};

/* print the register head in charging process */
static int sc8886_register_head(char *buffer, int size, void *dev_data)
{
	int ret;
	struct sc8886_device_info *di = dev_data;

	if (!buffer || !di)
		return -EPERM;

	ret = snprintf_s(buffer, size, size - 1, "Chip     ChrS0  ChrS1  ");
	if (ret < 0)
		return ret;

	return 0;
}

/* print the register value in charging process */
static int sc8886_dump_reg(char *buffer, int size, void *dev_data)
{
	int ret;
	struct sc8886_device_info *di = dev_data;
	struct sc8886_log_data log_data;
	log_data.chrg_stat0 = 0;
	log_data.chrg_stat1 = 0;

	if (!buffer || !di)
		return -ENODEV;

	ret = sc8886_read_byte(di, SC8886_CHARGE_STATUS_REG0, &log_data.chrg_stat0);
	ret += sc8886_read_byte(di, SC8886_CHARGE_STATUS_REG1, &log_data.chrg_stat1);
	if (ret)
		hwlog_err("log_data read err\n");

	ret = snprintf_s(buffer, size, size - 1, "%-9s0x%-5x0x%-5x",
		di->name, log_data.chrg_stat0, log_data.chrg_stat1);
	if (ret < 0)
		return ret;

	return 0;
}

static struct boost_ops g_sc8886_boost_ops = {
	.chip_name = "sc8886",
	.set_vcg_on = sc8886_set_vcg_on,
	.set_vbus = sc8886_set_vbus_mv,
	.set_ibus = sc8886_set_ibus_ma,
	.ic_enable = sc8886_ic_enable,
	.set_idle_mode = sc8886_set_idle_mode,
};

static struct power_log_ops g_sc8886_log_ops = {
	.dev_name = "sc8886",
	.dump_log_head = sc8886_register_head,
	.dump_log_content = sc8886_dump_reg,
};

static void sc8886_ops_register(struct sc8886_device_info *di)
{
	int ret;

	g_sc8886_rc_pd_ops.dev_data = (void *)di;
	g_sc8886_boost_ops.dev_data = (void *)di;
	g_sc8886_log_ops.dev_data = (void *)di;

	ret = boost_ops_register(&g_sc8886_boost_ops);
	ret += power_log_ops_register(&g_sc8886_log_ops);
	ret += rchg_pd_ops_register(&g_sc8886_rc_pd_ops);
	if (ret)
		hwlog_err("sc8886 ops register fail\n");
}

static int sc8886_parse_dts(struct device_node *np,
	struct sc8886_device_info *di)
{
	int ret;

	ret = power_gpio_config_output(np, "boost_en", "sc8886_boost_en",
		&di->boost_en, 0);
	if (ret)
		hwlog_err("%s: config gpio fail\n", __func__);

	return ret;
}

static int sc8886_check_chip_id(struct sc8886_device_info *di)
{
	int ret;

	if (di->chip_id == SC8886_DEVICE_ID) {
		ret = snprintf_s(di->name, CHIP_DEV_NAME_LEN, CHIP_DEV_NAME_LEN - 1, "sc8886");
	} else if (di->chip_id == BQ25713_DEVICE_ID) {
		ret = snprintf_s(di->name, CHIP_DEV_NAME_LEN, CHIP_DEV_NAME_LEN - 1, "bq25713");
	} else {
		hwlog_err("%s: unknown device\n", __func__);
		return -EINVAL;
	}
	if (ret < 0)
		hwlog_err("%s: snprintf device name fail", __func__);

	return 0;
}

static int sc8886_device_check(struct sc8886_device_info *di)
{
	int ret;
	int i;
	int retry_times = 3; /* retry 3 times */

	for (i = 0; i < retry_times; i++) {
		ret = sc8886_read_byte(di, SC8886_DEVICE_ID_REG, &di->chip_id);
		if (!ret) {
			hwlog_info("[device_check] chip_id=0x%x\n", di->chip_id);
			return sc8886_check_chip_id(di);
		}
		power_usleep(DT_USLEEP_10MS);
	}

	hwlog_err("%s: read device id error, ret=%d\n", __func__, ret);
	return ret;
}

static void sc8886_reg_init(struct sc8886_device_info *di)
{
	int ret;

	ret = sc8886_write_mask(di, SC8886_CHARGE_OP3_REG0, SC8886_OTG_RANGE_LOW_MASK,
			SC8886_OTG_RANGE_LOW_SHIFT, 1);
	ret += sc8886_write_mask(di, SC8886_CHARGE_OP3_REG0, SC8886_OTG_MODE_MASK,
		SC8886_OTG_MODE_SHIFT, 1);
	if (ret)
		hwlog_err("set OTG_MODE or OTG_RANGE_LOW fail ret:%d\n", ret);
}

static int sc8886_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct sc8886_device_info *di = NULL;
	struct device_node *np = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &client->dev;
	np = di->dev->of_node;
	di->client = client;
	di->chip_already_init = 1;

	ret = sc8886_device_check(di);
	if (ret)
		goto fail;

	ret = sc8886_parse_dts(np, di);
	if (ret)
		goto fail;
	sc8886_reg_init(di);
	sc8886_ops_register(di);

	i2c_set_clientdata(client, di);
	hwlog_info("%s success\n", __func__);
	return 0;

fail:
	devm_kfree(&client->dev, di);
	di = NULL;
	return ret;
}

static int sc8886_remove(struct i2c_client *client)
{
	struct sc8886_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	devm_kfree(&client->dev, di);
	di = NULL;
	return 0;
}

static void sc8886_shutdown(struct i2c_client *client)
{
	struct sc8886_device_info *di = i2c_get_clientdata(client);

	if (!di)
		return;

	sc8886_set_idle_mode(di, SC8886_IDLE_STATE);
}

#ifdef CONFIG_PM
static int sc8886_i2c_suspend(struct device *dev)
{
	return 0;
}

static int sc8886_i2c_resume(struct device *dev)
{
	return 0;
}

static const struct dev_pm_ops sc8886_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(sc8886_i2c_suspend, sc8886_i2c_resume)
};
#define SC8886_PM_OPS (&sc8886_pm_ops)
#else
#define SC8886_PM_OPS (NULL)
#endif /* CONFIG_PM */

MODULE_DEVICE_TABLE(i2c, sc8886);
static const struct of_device_id sc8886_of_match[] = {
	{
		.compatible = "sc8886",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id sc8886_i2c_id[] = {
	{ "sc8886", 0 },
	{},
};

static struct i2c_driver sc8886_driver = {
	.probe = sc8886_probe,
	.remove = sc8886_remove,
	.shutdown = sc8886_shutdown,
	.id_table = sc8886_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "sc8886",
		.of_match_table = of_match_ptr(sc8886_of_match),
		.pm = SC8886_PM_OPS,
	},
};

static int __init sc8886_init(void)
{
	return i2c_add_driver(&sc8886_driver);
}

static void __exit sc8886_exit(void)
{
	i2c_del_driver(&sc8886_driver);
}

module_init(sc8886_init);
module_exit(sc8886_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("sc8886 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");