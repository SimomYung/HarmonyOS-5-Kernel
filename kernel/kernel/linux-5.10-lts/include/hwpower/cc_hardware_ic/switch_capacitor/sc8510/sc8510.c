// SPDX-License-Identifier: GPL-2.0
/*
 * sc8510.c
 *
 * sc8510 driver
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#include "sc8510.h"
#include <chipset_common/hwpower/common_module/power_i2c.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/common_module/power_printk.h>

#define HWLOG_TAG sc8510
HWLOG_REGIST();

static int sc8510_read_byte(struct sc8510_dev_info *di, u8 reg, u8 *data)
{
	if (!di || !data) {
		hwlog_err("read_byte: di is null\n");
		return -ENODEV;
	}

	return power_i2c_u8_read_byte(di->client, reg, data);
}

static int sc8510_write_byte_mask(struct sc8510_dev_info *di, u8 reg, u8 mask, u8 shift, u8 value)
{
	if (!di) {
		hwlog_err("write_byte_mask: di is null\n");
		return -ENODEV;
	}

	return power_i2c_u8_write_byte_mask(di->client, reg, value, mask, shift);
}

static int sc8510_fix_freq_mode_enable(struct sc8510_dev_info *di, bool enable)
{
	int ret;

	if (di->device_id == SC8510_DEVICE_ID) {
		ret = sc8510_write_byte_mask(di, SC8510_FSW_CTRL_REG,
			SC8510_FIX_FREQ_EN_MASK, SC8510_FIX_FREQ_EN_SHIFT, enable);
	} else if (di->device_id == SGM41603A_DEVICE_ID) {
		ret = sc8510_write_byte_mask(di, SGM41603A_SCC_CFG1_REG,
			SGM41603A_FIX_FREQ_EN_MASK, SGM41603A_FIX_FREQ_EN_SHIFT, enable);
	} else {
		hwlog_err("unknown device\n");
		return -EINVAL;
	}
	hwlog_info("[fix_freq_mode_enable] %s, ret=%d\n", enable ? "enable" : "disable", ret);
	return ret;
}

static void sc8510_reg_init(struct sc8510_dev_info *di)
{
	int ret;

	if (!di)
		return;
	if (di->device_id == SC8510_DEVICE_ID) {
		if (di->fwd_ocp_max_en) {
			ret = sc8510_write_byte_mask(di, SC8510_OCP_REG,
				SC8510_FWD_OCP_MASK, SC8510_FWD_OCP_SHIFT, SC8510_FWD_OCP_MAX);
			if (ret)
				hwlog_err("set fwd ocp fail\n");
		}
		if (di->audio_mode_en) {
			ret = sc8510_write_byte_mask(di, SC8510_AUDIO_MODE_REG,
				SC8510_AUDIO_MODE_MASK, SC8510_AUDIO_MODE_SHIFT, SC8510_AUDIO_EN);
			if (ret)
				hwlog_err("set audio enable fail\n");
		}
	} else if (di->device_id == SGM41603A_DEVICE_ID) {
		ret = sc8510_write_byte_mask(di, SGM41603A_V2X_OVP_REG, SGM41603A_V2X_OVP_MASK,
			SGM41603A_V2X_OVP_SHIFT, SGM41603A_V2X_OVP_10500MV);
		ret += sc8510_write_byte_mask(di, SGM41603A_MISC2_REG, SGM41603A_BATFET_DET_DIS_MASK,
			SGM41603A_BATFET_DET_DIS_SHIFT, SGM41603A_BATFET_DET_DIS);
		if (ret)
			hwlog_err("reg init fail ret:%d\n", ret);
	} else {
		hwlog_err("unknown device\n");
	}
}

static int sc8510_notifier_call(struct notifier_block *nb, unsigned long event, void *data)
{
	struct sc8510_dev_info *di = container_of(nb, struct sc8510_dev_info, event_nb);

	if (!di)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_USB_DISCONNECT:
		sc8510_fix_freq_mode_enable(di, false);
		break;
	case POWER_NE_USB_CONNECT:
		sc8510_fix_freq_mode_enable(di, true);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int sc8510_device_check(struct sc8510_dev_info *di)
{
	int ret;
	int i;
	int retry_times = 3; /* retry 3 times */

	for (i = 0; i < retry_times; i++) {
		ret = sc8510_read_byte(di, SC8510_DEVICE_ID_REG, &di->device_id);
		if (!ret) {
			hwlog_info("[device_check] device_id=0x%x\n", di->device_id);
			return 0;
		}

		power_usleep(DT_USLEEP_10MS);
	}

	hwlog_err("device_check: device_id=0x%x, ret=%d\n", di->device_id, ret);
	return ret;
}

static void sc8510_parse_dts(struct device_node *np, struct sc8510_dev_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"audio_mode_en", &di->audio_mode_en, 0);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"fwd_ocp_max_en", &di->fwd_ocp_max_en, 0);
}

static int sc8510_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct sc8510_dev_info *di = NULL;
	struct device_node *np = NULL;

	if (!client || !client->dev.of_node || !id)
		return -ENODEV;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &client->dev;
	np = client->dev.of_node;
	di->client = client;

	ret = sc8510_device_check(di);
	if (ret)
		goto dev_check_fail;

	sc8510_parse_dts(np, di);

	di->event_nb.notifier_call = sc8510_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_CONNECT, &di->event_nb);
	if (ret)
		goto notifier_fail;

	i2c_set_clientdata(client, di);

	sc8510_reg_init(di);

	hwlog_info("probe ok\n");
	return 0;

notifier_fail:
dev_check_fail:
	i2c_set_clientdata(client, NULL);
	devm_kfree(&client->dev, di);
	return ret;
}

static int sc8510_remove(struct i2c_client *client)
{
	struct sc8510_dev_info *di = i2c_get_clientdata(client);

	if (!di)
		return -ENODEV;

	(void)power_event_bnc_unregister(POWER_BNT_CONNECT, &di->event_nb);
	i2c_set_clientdata(client, NULL);

	return 0;
}

MODULE_DEVICE_TABLE(i2c, charge_pump_sc8510);
static const struct of_device_id sc8510_of_match[] = {
	{
		.compatible = "sc8510",
		.data = NULL,
	},
	{
		.compatible = "sgm41603a",
		.data = NULL,
	},
	{},
};

static const struct i2c_device_id sc8510_i2c_id[] = {
	{ "sc8510", 0 },
	{ "sgm41603a", 0 },
	{},
};

static struct i2c_driver sc8510_driver = {
	.probe = sc8510_probe,
	.remove = sc8510_remove,
	.id_table = sc8510_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "sc8510",
		.of_match_table = of_match_ptr(sc8510_of_match),
	},
};

static int __init sc8510_init(void)
{
	return i2c_add_driver(&sc8510_driver);
}

static void __exit sc8510_exit(void)
{
	i2c_del_driver(&sc8510_driver);
}

module_init(sc8510_init);
module_exit(sc8510_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("sc8510 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
