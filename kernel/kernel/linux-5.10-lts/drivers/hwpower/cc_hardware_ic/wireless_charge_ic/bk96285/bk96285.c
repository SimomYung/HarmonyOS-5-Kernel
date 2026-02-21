// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285.c
 *
 * bk96285 driver
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

#include "bk96285.h"

#define HWLOG_TAG wireless_bk96285
HWLOG_REGIST();

#define BK96285_PINCTRL_LEN		2

bool bk96285_is_pwr_good(struct bk96285_dev_info *di)
{
	int gpio_val;
	int gpio_mode;

	if (!di)
		return false;

	if (!di->g_val.mtp_chk_complete)
		return true;

	gpio_val = gpio_get_value(di->gpio_pwr_good);
	if (gpio_val == BK96285_GPIO_PWR_GOOD_VAL)
		return true;

	/* 0 for output, 1 for input */
	gpio_mode = gpiod_get_direction(gpio_to_desc(di->gpio_pwr_good));
	if (!gpio_mode)
		hwlog_info("[pwr fali] gpio=%d, mode=%d\n",
			di->gpio_pwr_good, gpio_mode);

	return false;
}

static int bk96285_i2c_read(struct bk96285_dev_info *di, u8 *cmd, int cmd_len,
	u8 *buf, int buf_len)
{
	int i;

	for (i = 0; i < WLTRX_IC_I2C_RETRY_CNT; i++) {
		if (!bk96285_is_pwr_good(di))
			return -EIO;
		if (!power_i2c_read_block(di->client, cmd, cmd_len, buf, buf_len))
			return 0;
		power_usleep(DT_USLEEP_10MS);
	}

	return -EIO;
}

static int bk96285_i2c_write(struct bk96285_dev_info *di, u8 *buf, int buf_len)
{
	int i;

	for (i = 0; i < WLTRX_IC_I2C_RETRY_CNT; i++) {
		if (!bk96285_is_pwr_good(di))
			return -EIO;
		if (!power_i2c_write_block(di->client, buf, buf_len))
			return 0;
		power_usleep(DT_USLEEP_10MS);
	}

	return -EIO;
}

int bk96285_read_block(struct bk96285_dev_info *di, u16 reg, u8 *data, u8 len)
{
	u8 cmd[BK96285_ADDR_LEN];

	if (!di || !data) {
		hwlog_err("read_block: para null\n");
		return -EINVAL;
	}

	cmd[0] = reg >> POWER_BITS_PER_BYTE;
	cmd[1] = reg & POWER_MASK_BYTE;

	return bk96285_i2c_read(di, cmd, BK96285_ADDR_LEN, data, len);
}

int bk96285_write_block(struct bk96285_dev_info *di, u16 reg, u8 *data, u8 data_len)
{
	int ret;
	u8 *cmd = NULL;

	if (!di || !data) {
		hwlog_err("write_block: para null\n");
		return -EINVAL;
	}

	cmd = kzalloc(sizeof(u8) * (BK96285_ADDR_LEN + data_len), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	cmd[0] = reg >> POWER_BITS_PER_BYTE;
	cmd[1] = reg & POWER_MASK_BYTE;
	memcpy_s(&cmd[BK96285_ADDR_LEN], data_len, data, data_len);

	ret = bk96285_i2c_write(di, cmd, BK96285_ADDR_LEN + data_len);

	kfree(cmd);
	return ret;
}

int bk96285_read_register(struct bk96285_dev_info *di, u16 reg, u8 *pdata, u8 len)
{
	u16 align_addr = reg & BK96285_ALIGN_ADDR_MASK;
	u16 align_offs = reg % BK96285_ALIGN_ADDR_LEN;
	u8 ext = (reg + len) % BK96285_ALIGN_ADDR_LEN;
	u8 length = len;
	u8 *pbuf = NULL;
	int ret;

	length += align_offs;
	if (ext != 0)
		length += BK96285_ALIGN_ADDR_LEN - ext;

	pbuf = kzalloc(length, GFP_KERNEL);
	if (!pbuf)
		return -ENOMEM;

	ret = bk96285_read_block(di, align_addr, pbuf, length);
	if (ret)
		hwlog_err("Read block failed, reg=0x%04X len=%d ret=%d", reg, length, ret);
	else
		(void)memcpy_s(pdata, len, pbuf + align_offs, len);
	kfree((void *)pbuf);
	return ret;
}

int bk96285_write_register(struct bk96285_dev_info *di, u16 reg, u8 *pdata, u8 len)
{
	u16 align_addr = reg & BK96285_ALIGN_ADDR_MASK;
	u16 align_offs = reg % BK96285_ALIGN_ADDR_LEN;
	u8 ext = (reg + len) % BK96285_ALIGN_ADDR_LEN;
	u8 *pbuf = kzalloc(len + BK96285_READ_EXT_LEN, GFP_KERNEL);
	u8 length = len;
	int ret;

	if (!pbuf)
		return -EINVAL;

	if (align_offs != 0) {
		ret = bk96285_read_register(di, align_addr, pbuf, align_offs);
		if (ret) {
			hwlog_err("write_register: Block read failed: reg=0x%04X len=%d ret=%d",
				align_addr, align_offs, ret);
			kfree((void *)pbuf);
			return ret;
		}
		(void)memcpy_s(pbuf + align_offs, length, pdata, length);
		length += align_offs;
	} else {
		ret = memcpy_s(pbuf, len + BK96285_READ_EXT_LEN, pdata, length);
		if (ret) {
			hwlog_err("memcpy_s fail in bk96285_write_register\n");
			kfree((void *)pbuf);
			return ret;
		}
	}

	if (ext != 0) {
		ret = bk96285_read_register(di, align_addr + length, pbuf + length,
			BK96285_ALIGN_ADDR_LEN - ext);
		if (ret) {
			hwlog_err("write_register: Block read failed: reg=0x%04X len=%d ret=%d",
				align_addr + length, 4 - ext, ret);
			kfree((void *)pbuf);
			return ret;
		}
		length += (BK96285_ALIGN_ADDR_LEN - ext);
	}

	ret = bk96285_write_block(di, align_addr, pbuf, length);
	if (ret)
		hwlog_err("write_register: Write block failed: 0X%04X, ret=0x%08x", align_addr, ret);

	kfree((void *)pbuf);
	return ret;
}

int bk96285_read_byte(struct bk96285_dev_info *di, u16 reg, u8 *data)
{
	return bk96285_read_register(di, reg, data, POWER_BYTE_LEN);
}

int bk96285_read_word(struct bk96285_dev_info *di, u16 reg, u16 *data)
{
	u8 buff[POWER_WORD_LEN] = { 0 };

	if (!data || bk96285_read_register(di, reg, buff, POWER_WORD_LEN))
		return -EIO;

	*data = buff[0] | (buff[1] << POWER_BITS_PER_BYTE);
	return 0;
}

int bk96285_read_dword(struct bk96285_dev_info *di, u16 reg, u32 *data)
{
	u8 buff[POWER_DWORD_LEN] = { 0 };

	if (!data || bk96285_read_block(di, reg, buff, POWER_DWORD_LEN))
		return -EIO;

	/* 1dword=4bytes, 1byte=8bit */
	*data = buff[0] | (buff[1] << 8) | (buff[2] << 16) | (buff[3] << 24);
	return 0;
}

int bk96285_write_byte(struct bk96285_dev_info *di, u16 reg, u8 data)
{
	return bk96285_write_register(di, reg, &data, POWER_BYTE_LEN);
}

int bk96285_write_word(struct bk96285_dev_info *di, u16 reg, u16 data)
{
	return bk96285_write_register(di, reg, (u8 *)&data, POWER_WORD_LEN);
}

int bk96285_write_dword(struct bk96285_dev_info *di, u16 reg, u32 data)
{
	return bk96285_write_block(di, reg, (u8 *)&data, POWER_DWORD_LEN);
}

int bk96285_read_byte_mask(struct bk96285_dev_info *di, u16 reg, u8 mask, u8 shift, u8 *data)
{
	int ret;
	u8 val = 0;

	ret = bk96285_read_byte(di, reg, &val);
	if (ret)
		return ret;

	val &= mask;
	val >>= shift;
	*data = val;

	return 0;
}

int bk96285_write_byte_mask(struct bk96285_dev_info *di, u16 reg, u8 mask, u8 shift, u8 data)
{
	int ret;
	u8 val = 0;

	ret = bk96285_read_byte(di, reg, &val);
	if (ret)
		return ret;

	val &= ~mask;
	val |= ((data << shift) & mask);

	return bk96285_write_byte(di, reg, val);
}

int bk96285_write_word_mask(struct bk96285_dev_info *di, u16 reg, u16 mask, u16 shift, u16 data)
{
	int ret;
	u16 val = 0;

	ret = bk96285_read_word(di, reg, &val);
	if (ret)
		return ret;

	val &= ~mask;
	val |= ((data << shift) & mask);

	return bk96285_write_word(di, reg, val);
}

int bk96285_read_dword_mask(struct bk96285_dev_info *di, u16 reg, u32 mask, u32 shift, u32 *data)
{
	int ret;
	u32 val = 0;

	ret = bk96285_read_dword(di, reg, &val);
	if (ret)
		return ret;

	val &= mask;
	val >>= shift;
	*data = val;

	return 0;
}

int bk96285_write_dword_mask(struct bk96285_dev_info *di, u16 reg, u32 mask, u32 shift, u32 data)
{
	int ret;
	u32 val = 0;

	ret = bk96285_read_dword(di, reg, &val);
	if (ret)
		return ret;

	val &= ~mask;
	val |= ((data << shift) & mask);

	return bk96285_write_dword(di, reg, val);
}

int bk96285_get_mode(struct bk96285_dev_info *di, u32 *mode)
{
	if (!di || !mode)
		return -ENODEV;

	return bk96285_read_dword(di, BK96285_SYS_MODE_ADDR, mode);
}

void bk96285_chip_enable(bool enable, void *dev_data)
{
	int gpio_val;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	gpio_set_value(di->gpio_en,
		enable ? di->gpio_en_valid_val : !di->gpio_en_valid_val);
	gpio_val = gpio_get_value(di->gpio_en);
	hwlog_info("[chip_enable] gpio %s now\n", gpio_val ? "high" : "low");
}

bool bk96285_is_chip_enable(void *dev_data)
{
	int gpio_val;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return false;

	gpio_val = gpio_get_value(di->gpio_en);
	return gpio_val == di->gpio_en_valid_val;
}

static int bk96285_get_chip_id(struct bk96285_dev_info *di, u32 *chip_id)
{
	return bk96285_read_dword(di, BK96285_CHIP_ID_ADDR, chip_id);
}

int bk96285_fw_get_mtp_ver(struct bk96285_dev_info *di, u16 *mtp_ver)
{
	return bk96285_read_word(di, BK96285_MTP_VER_ADDR, mtp_ver);
}

int bk96285_get_chip_info(struct bk96285_dev_info *di, struct bk96285_chip_info *info)
{
	int ret;

	if (!info)
		return -EINVAL;

	ret = bk96285_get_chip_id(di, &info->chip_id);
	ret += bk96285_fw_get_mtp_ver(di, &info->mtp_ver);
	if (ret) {
		hwlog_err("get_chip_info: failed\n");
		return ret;
	}

	return 0;
}

int bk96285_set_product_type(struct bk96285_dev_info *di, u32 product_type)
{
	return bk96285_write_dword(di, BK96285_PRODUCT_TYPE_ADDR, product_type);
}

void bk96285_enable_irq(struct bk96285_dev_info *di)
{
	if (!di)
		return;

	mutex_lock(&di->mutex_irq);
	if (!di->irq_active) {
		hwlog_info("[enable_irq] ++\n");
		di->irq_active = true;
		enable_irq(di->irq_int);
	}
	hwlog_info("[enable_irq] --\n");
	mutex_unlock(&di->mutex_irq);
}

void bk96285_disable_irq_nosync(struct bk96285_dev_info *di)
{
	if (!di)
		return;

	mutex_lock(&di->mutex_irq);
	if (di->irq_active) {
		hwlog_info("[disable_irq_nosync] ++\n");
		disable_irq_nosync(di->irq_int);
		di->irq_active = false;
	}
	hwlog_info("[disable_irq_nosync] --\n");
	mutex_unlock(&di->mutex_irq);
}

void bk96285_enable_irq_wake(struct bk96285_dev_info *di)
{
	if (!di)
		return;

	if (!di->irq_awake) {
		hwlog_info("[enable_irq_wake] ++\n");
		(void)enable_irq_wake(di->irq_int);
		di->irq_awake = true;
	}
	hwlog_info("[enable_irq_wake] --\n");
}

void bk96285_disable_irq_wake(struct bk96285_dev_info *di)
{
	if (!di)
		return;

	if (di->irq_awake) {
		hwlog_info("[disable_irq_wake] ++\n");
		(void)disable_irq_wake(di->irq_int);
		di->irq_awake = false;
	}
	hwlog_info("[disable_irq_wake] --\n");
}
static void bk96285_register_pwr_dev_info(struct bk96285_dev_info *di)
{
	struct power_devices_info_data *pwr_dev_info = NULL;

	pwr_dev_info = power_devices_info_register();
	if (pwr_dev_info) {
		pwr_dev_info->dev_name = di->dev->driver->name;
		pwr_dev_info->dev_id = di->chip_id;
		pwr_dev_info->ver_id = 0;
	}
}

static void bk96285_ops_unregister(struct wltrx_ic_ops *ops)
{
	if (!ops)
		return;

	kfree(ops->fw_ops);
	kfree(ops->rx_ops);
	kfree(ops->tx_ops);
	kfree(ops->qi_ops);
	kfree(ops);
}

static int bk96285_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di)
{
	int ret;

	ret = bk96285_fw_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register fw_ops failed\n");
		return ret;
	}
	ret = bk96285_rx_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register rx_ops failed\n");
		return ret;
	}
	ret = bk96285_tx_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register tx_ops failed\n");
		return ret;
	}
	ret = bk96285_qi_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register qi_ops failed\n");
		return ret;
	}
	di->g_val.qi_hdl = hwqi_get_handle();
	ret = bk96285_hw_test_ops_register(di);
	if (ret) {
		hwlog_err("ops_register: register hw_test_ops failed\n");
		return ret;
	}
	return 0;
}

struct device_node *bk96285_dts_dev_node(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di || !di->dev)
		return NULL;

	return di->dev->of_node;
}

static int bk96285_gpio_init(struct bk96285_dev_info *di, struct device_node *np)
{
	if (power_gpio_config_output(np, "gpio_en", "bk96285_en",
		&di->gpio_en, di->gpio_en_valid_val))
		goto gpio_en_fail;

	if (power_gpio_config_output(np, "gpio_sleep_en", "bk96285_sleep_en",
		&di->gpio_sleep_en, WLTRX_IC_SLEEP_DISABLE))
		goto gpio_sleep_en_fail;

	if (power_gpio_config_input(np, "gpio_pwr_good", "bk96285_pwr_good",
		&di->gpio_pwr_good))
		goto gpio_pwr_good_fail;

	return 0;

gpio_pwr_good_fail:
	gpio_free(di->gpio_sleep_en);
gpio_sleep_en_fail:
	gpio_free(di->gpio_en);
gpio_en_fail:
	return -EINVAL;
}

static void bk96285_free_dev_resources(struct bk96285_dev_info *di)
{
	power_wakeup_source_unregister(di->wakelock);
	mutex_destroy(&di->mutex_irq);
	gpio_free(di->gpio_int);
	free_irq(di->irq_int, di);
	gpio_free(di->gpio_en);
	gpio_free(di->gpio_sleep_en);
	gpio_free(di->gpio_pwr_good);
}

static void bk96285_irq_work(struct work_struct *work)
{
	int ret;
	int gpio_val;
	u32 mode = 0;
	struct bk96285_dev_info *di = container_of(work, struct bk96285_dev_info, irq_work);

	if (!di)
		goto exit;

	gpio_val = gpio_get_value(di->gpio_en);
	if (gpio_val != di->gpio_en_valid_val) {
		hwlog_err("[irq_work] gpio %s\n", gpio_val ? "high" : "low");
		goto exit;
	}
	if (di->need_ignore_irq) {
		hwlog_info("[irq_work] ignore irq\n");
		goto exit;
	}
	ret = bk96285_get_mode(di, &mode);
	if (!ret)
		hwlog_info("[irq_work] mode=0x%x\n", mode);
	else
		bk96285_rx_abnormal_irq_handler(di);

	if (mode & BK96285_SYS_MODE_RX)
		bk96285_rx_mode_irq_handler(di);
	else if ((mode & BK96285_SYS_MODE_TX) || (mode & BK96285_SYS_MODE_BP))
		bk96285_tx_mode_irq_handler(di);
exit:
	if (di && !di->g_val.irq_abnormal)
		bk96285_enable_irq(di);

	power_wakeup_unlock(di->wakelock, false);
}

static irqreturn_t bk96285_interrupt(int irq, void *_di)
{
	struct bk96285_dev_info *di = _di;

	if (!di) {
		hwlog_err("interrupt: di null\n");
		return IRQ_HANDLED;
	}

	power_wakeup_lock(di->wakelock, false);
	hwlog_info("[interrupt] ++\n");
	if (di->irq_active) {
		disable_irq_nosync(di->irq_int);
		di->irq_active = false;
		schedule_work(&di->irq_work);
	} else {
		hwlog_info("[interrupt] irq is not enable,do nothing\n");
		power_wakeup_unlock(di->wakelock, false);
	}
	hwlog_info("[interrupt] --\n");
	return IRQ_HANDLED;
}

static int bk96285_irq_init(struct bk96285_dev_info *di,
	struct device_node *np)
{
	INIT_WORK(&di->irq_work, bk96285_irq_work);

	if (power_gpio_config_interrupt(np, "gpio_int", "bk96285_int",
		&di->gpio_int, &di->irq_int))
		return -EINVAL;

	if (request_irq(di->irq_int, bk96285_interrupt,
		IRQF_TRIGGER_FALLING, "bk96285_irq", di)) {
		hwlog_err("irq_init: request irq failed\n");
		gpio_free(di->gpio_int);
		return -EINVAL;
	}

	bk96285_enable_irq_wake(di);
	di->irq_active = true;

	return 0;
}
static int bk96285_request_dev_resources(struct bk96285_dev_info *di, struct device_node *np)
{
	int ret;

	ret = bk96285_parse_dts(np, di);
	if (ret)
		goto parse_dts_fail;
	ret = power_pinctrl_config(di->dev, "pinctrl-names", BK96285_PINCTRL_LEN);
	if (ret)
		return ret;
	ret = bk96285_gpio_init(di, np);
	if (ret)
		goto gpio_init_fail;
	ret = bk96285_irq_init(di, np);
	if (ret)
		goto irq_init_fail;
	di->wakelock = power_wakeup_source_register(di->dev, "bk96285_wakelock");
	mutex_init(&di->mutex_irq);
	return 0;

irq_init_fail:
	gpio_free(di->gpio_en);
	gpio_free(di->gpio_sleep_en);
	gpio_free(di->gpio_pwr_good);
gpio_init_fail:
parse_dts_fail:
	return ret;
}

static int bk96285_dev_check(struct bk96285_dev_info *di)
{
	int ret;
	u32 chip_id = 0;

	wlps_control(di->ic_type, WLPS_RX_EXT_PWR, true);
	power_usleep(DT_USLEEP_10MS);
	ret = bk96285_get_chip_id(di, &chip_id);
	if (ret) {
		hwlog_err("dev_check: failed, ret = %d\n", ret);
		wlps_control(di->ic_type, WLPS_RX_EXT_PWR, false);
		return ret;
	}
	wlps_control(di->ic_type, WLPS_RX_EXT_PWR, false);

	di->chip_id = chip_id;
	hwlog_info("[dev_check] chip_id=0x%x\n", chip_id);
	return 0;
}

static int bk96285_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int ret;
	struct bk96285_dev_info *di = NULL;
	struct device_node *np = NULL;
	struct wltrx_ic_ops *ops = NULL;

	hwlog_info("[probe] begin\n");
	if (!client || !id || !client->dev.of_node)
		return -ENODEV;

	if (wlrx_ic_is_ops_registered(id->driver_data) ||
		wltx_ic_is_ops_registered(id->driver_data))
		return 0;

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	ops = kzalloc(sizeof(*ops), GFP_KERNEL);
	if (!ops) {
		devm_kfree(&client->dev, di);
		return -ENOMEM;
	}

	di->dev = &client->dev;
	np = client->dev.of_node;
	di->client = client;
	di->ic_type = id->driver_data;
	i2c_set_clientdata(client, di);

	ret = bk96285_dev_check(di);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto dev_ck_fail;
	}

	ret = bk96285_request_dev_resources(di, np);
	if (ret)
		goto req_dev_res_fail;

	ret = bk96285_ops_register(ops, di);
	if (ret)
		goto ops_regist_fail;

	wlic_iout_init(di->ic_type, np, NULL);
	bk96285_fw_mtp_check(di);
	bk96285_register_pwr_dev_info(di);
	hwlog_info("[probe] ok\n");
	return 0;

ops_regist_fail:
	hwlog_err("ops_regist: fail\n");
	bk96285_free_dev_resources(di);
req_dev_res_fail:
	hwlog_err("request: fail\n");
dev_ck_fail:
	bk96285_ops_unregister(ops);
	devm_kfree(&client->dev, di);
	return ret;
}

static void bk96285_shutdown(struct i2c_client *client)
{
	struct bk96285_dev_info *di = i2c_get_clientdata(client);

	if (!di)
		return;
	bk96285_rx_shutdown_handler(di);
}

static const struct of_device_id bk96285_of_match[] = {
	{
		.compatible = "bk,bk96285",
	},
	{},
};
MODULE_DEVICE_TABLE(of, bk96285_of_match);

static const struct i2c_device_id bk96285_i2c_id[] = {
	{ "bk96285", WLTRX_IC_MAIN }, {}
};
MODULE_DEVICE_TABLE(i2c, bk96285_i2c_id);

static struct i2c_driver bk96285_driver = {
	.probe = bk96285_probe,
	.shutdown = bk96285_shutdown,
	.id_table = bk96285_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "wireless_bk96285",
		.of_match_table = of_match_ptr(bk96285_of_match),
	},
};

static int __init bk96285_init(void)
{
	return i2c_add_driver(&bk96285_driver);
}

static void __exit bk96285_exit(void)
{
	i2c_del_driver(&bk96285_driver);
}

device_initcall(bk96285_init);
module_exit(bk96285_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("bk96285 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
