// SPDX-License-Identifier: GPL-2.0
/*
 * cps8601.c
 *
 * cps8601 driver
 *
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
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

#ifdef CONFIG_SYS_PM_ULSR
#include <platform_include/cee/linux/ulsr.h>
#endif /* CONFIG_SYS_PM_ULSR */
#include "cps8601.h"
#include "cps8601_mtp.h"
#include "cps8602_mtp.h"
#include "cps8601_mtp_extra.h"

#define HWLOG_TAG wireless_cps8601
HWLOG_REGIST();

static int cps8601_i2c_read(struct cps8601_dev_info *di, u8 *cmd, int cmd_len, u8 *dat, int dat_len)
{
	int i;

	for (i = 0; i < WLTRX_IC_I2C_RETRY_CNT; i++) {
		if (!power_i2c_read_block(di->client, cmd, cmd_len, dat, dat_len))
			return 0;
		power_usleep(DT_USLEEP_10MS); /* delay for i2c retry */
	}

	return -EIO;
}

static int cps8601_i2c_write(struct cps8601_dev_info *di, u8 *cmd, int cmd_len)
{
	int i;

	for (i = 0; i < WLTRX_IC_I2C_RETRY_CNT; i++) {
		if (!power_i2c_write_block(di->client, cmd, cmd_len))
			return 0;
		power_usleep(DT_USLEEP_10MS); /* delay for i2c retry */
	}

	return -EIO;
}

int cps8601_read_block(struct cps8601_dev_info *di, u16 reg, u8 *data, u8 len)
{
	u8 cmd[CPS8601_ADDR_LEN];

	if (!di || !data) {
		hwlog_err("read_block: para null\n");
		return -EINVAL;
	}

	cmd[0] = reg >> POWER_BITS_PER_BYTE;
	cmd[1] = reg & POWER_MASK_BYTE;

	return cps8601_i2c_read(di, cmd, CPS8601_ADDR_LEN, data, len);
}

int cps8601_write_block(struct cps8601_dev_info *di, u16 reg, u8 *data, u8 len)
{
	int ret;
	u8 *cmd = NULL;

	if (!di || !data) {
		hwlog_err("write_block: para null\n");
		return -EINVAL;
	}
	cmd = kzalloc(sizeof(u8) * (CPS8601_ADDR_LEN + len), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	cmd[0] = reg >> POWER_BITS_PER_BYTE;
	cmd[1] = reg & POWER_MASK_BYTE;
	memcpy(&cmd[CPS8601_ADDR_LEN], data, len);

	ret = cps8601_i2c_write(di, cmd, CPS8601_ADDR_LEN + len);
	kfree(cmd);
	return ret;
}

int cps8601_hw_read_block(struct cps8601_dev_info *di, u32 addr, u8 *data, u8 len)
{
	u8 cmd[CPS8601_HW_ADDR_LEN];

	if (!di || !data) {
		hwlog_err("hw_read_block: para null\n");
		return -EINVAL;
	}

	/* 1dword=4bytes, 1byte=8bits 0xff: byte mask */
	cmd[0] = (u8)((addr >> 24) & 0xff);
	cmd[1] = (u8)((addr >> 16) & 0xff);
	cmd[2] = (u8)((addr >> 8) & 0xff);
	cmd[3] = (u8)((addr >> 0) & 0xff);

	return cps8601_i2c_read(di, cmd, CPS8601_HW_ADDR_LEN, data, len);
}

int cps8601_hw_write_block(struct cps8601_dev_info *di, u32 addr, u8 *data, u8 data_len)
{
	int ret;
	u8 *cmd = NULL;

	if (!di || !data) {
		hwlog_err("hw_write_block: para null\n");
		return -EINVAL;
	}

	cmd = kzalloc(sizeof(u8) * (CPS8601_HW_ADDR_LEN + data_len), GFP_KERNEL);
	if (!cmd)
		return -ENOMEM;

	/* 1dword=4bytes, 1byte=8bits 0xff: byte mask */
	cmd[0] = (u8)((addr >> 24) & 0xff);
	cmd[1] = (u8)((addr >> 16) & 0xff);
	cmd[2] = (u8)((addr >> 8) & 0xff);
	cmd[3] = (u8)((addr >> 0) & 0xff);
	memcpy(&cmd[CPS8601_HW_ADDR_LEN], data, data_len);

	ret = cps8601_i2c_write(di, cmd, CPS8601_HW_ADDR_LEN + data_len);
	kfree(cmd);
	return ret;
}

int cps8601_read_byte(struct cps8601_dev_info *di, u16 reg, u8 *data)
{
	return cps8601_read_block(di, reg, data, POWER_BYTE_LEN);
}

int cps8601_read_word(struct cps8601_dev_info *di, u16 reg, u16 *data)
{
	u8 buff[POWER_WORD_LEN] = { 0 };

	if (!data || cps8601_read_block(di, reg, buff, POWER_WORD_LEN))
		return -EINVAL;

	*data = buff[0] | (buff[1] << POWER_BITS_PER_BYTE);
	return 0;
}

int cps8601_write_byte(struct cps8601_dev_info *di, u16 reg, u8 data)
{
	return cps8601_write_block(di, reg, &data, POWER_BYTE_LEN);
}

int cps8601_write_word(struct cps8601_dev_info *di, u16 reg, u16 data)
{
	u8 buff[POWER_WORD_LEN];

	buff[0] = data & POWER_MASK_BYTE;
	buff[1] = data >> POWER_BITS_PER_BYTE;

	return cps8601_write_block(di, reg, buff, POWER_WORD_LEN);
}

int cps8601_write_word_mask(struct cps8601_dev_info *di, u16 reg, u16 mask, u16 shift, u16 data)
{
	int ret;
	u16 val = 0;

	ret = cps8601_read_word(di, reg, &val);
	if (ret)
		return ret;

	val &= ~mask;
	val |= ((data << shift) & mask);

	return cps8601_write_word(di, reg, val);
}

int cps8601_hw_read_dword(struct cps8601_dev_info *di, u32 addr, u32 *data)
{
	u8 buff[POWER_DWORD_LEN] = { 0 };

	if (!data || cps8601_hw_read_block(di, addr, buff, POWER_DWORD_LEN))
		return -EINVAL;

	*data = buff[0] | (buff[1] << 8) | (buff[2] << 16) | (buff[3] << 24);
	return 0;
}

int cps8601_hw_write_dword(struct cps8601_dev_info *di, u32 addr, u32 data)
{
	return cps8601_hw_write_block(di, addr, (u8 *)&data, POWER_DWORD_LEN);
}

int cps8601_get_chip_id(struct cps8601_dev_info *di, u16 *chip_id)
{
	int ret;

	ret = cps8601_read_word(di, CPS8601_CHIP_ID_ADDR, chip_id);
	hwlog_info("[get_chip_id] chipid=0x%x, ret=%d\n", *chip_id, ret);
	if (ret)
		return ret;

	if (di->client->addr == CPS8601_I2C_SLAVE_ADDR)
		*chip_id = CPS8601_CHIP_ID;
	else if (di->client->addr == CPS8602_I2C_SLAVE_ADDR)
		*chip_id = CPS8602_CHIP_ID;
	return 0;
}

static int cps8601_get_mtp_version(struct cps8601_dev_info *di, u16 *mtp_ver)
{
	return cps8601_read_word(di, CPS8601_MTP_VER_ADDR, mtp_ver);
}

int cps8601_get_chip_info(struct cps8601_dev_info *di, struct cps8601_chip_info *info)
{
	int ret;

	if (!info || !di)
		return -EINVAL;

	ret = cps8601_get_chip_id(di, &info->chip_id);
	ret += cps8601_get_mtp_version(di, &info->mtp_ver);
	if (ret) {
		hwlog_err("get_chip_info: failed\n");
		return ret;
	}

	return 0;
}

int cps8601_get_chip_info_str(struct cps8601_dev_info *di, char *info_str, int len)
{
	int ret;
	struct cps8601_chip_info chip_info = { 0 };

	if (!info_str || (len != CPS8601_CHIP_INFO_STR_LEN))
		return -EINVAL;

	ret = cps8601_get_chip_info(di, &chip_info);
	if (ret)
		return ret;

	memset(info_str, 0, CPS8601_CHIP_INFO_STR_LEN);

	return snprintf(info_str, len, "chip_id:0x%04x mtp_ver:0x%04x",
		chip_info.chip_id, chip_info.mtp_ver);
}

int cps8601_get_mode(struct cps8601_dev_info *di, u8 *mode)
{
	return cps8601_read_byte(di, CPS8601_OP_MODE_ADDR, mode);
}

void cps8601_enable_irq(struct cps8601_dev_info *di)
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

void cps8601_disable_irq_nosync(struct cps8601_dev_info *di)
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

void cps8601_chip_enable(bool enable, void *dev_data)
{
}

static void cps8601_irq_work(struct work_struct *work)
{
	int ret;
	u8 mode = 0;
	struct cps8601_dev_info *di = NULL;

	if (!work)
		return;

	di = container_of(work, struct cps8601_dev_info, irq_work);
	if (!di)
		goto exit;

	/* get System Operating Mode */
	ret = cps8601_get_mode(di, &mode);
	if (!ret)
		hwlog_info("[irq_work] mode=0x%x\n", mode);

	/* handler irq */
	if ((mode == CPS8601_OP_MODE_TX) || (mode == CPS8601_OP_MODE_BP))
		cps8601_tx_mode_irq_handler(di);
	else
		hwlog_info("[irq_work] mode=0x%x\n", mode);

exit:
	if (di) {
		cps8601_enable_irq(di);
		power_wakeup_unlock(di->wakelock, false);
	}
}

static irqreturn_t cps8601_interrupt(int irq, void *_di)
{
	struct cps8601_dev_info *di = _di;

	if (!di)
		return IRQ_HANDLED;

	power_wakeup_lock(di->wakelock, false);
	hwlog_info("[interrupt] ++\n");
	if (di->irq_active) {
		disable_irq_nosync(di->irq_int);
		di->irq_active = false;
		queue_work(di->system_wq, &di->irq_work);
	} else {
		hwlog_info("[interrupt] irq is not enable\n");
		power_wakeup_unlock(di->wakelock, false);
	}
	hwlog_info("[interrupt] --\n");

	return IRQ_HANDLED;
}

static int cps8601_dev_check(struct cps8601_dev_info *di)
{
	int ret;
	u16 chip_id = 0;

	wlps_control(di->ic_type, WLPS_TX_PWR_SW, true);
	power_usleep(DT_USLEEP_10MS); /* delay for power on */
	ret = cps8601_get_chip_id(di, &chip_id);
	if (ret) {
		hwlog_err("dev_check: failed\n");
		wlps_control(di->ic_type, WLPS_TX_PWR_SW, false);
		return ret;
	}
	wlps_control(di->ic_type, WLPS_TX_PWR_SW, false);

	di->chip_id = chip_id;
	hwlog_info("[dev_check] chip_id=0x%04x\n", chip_id);
	if ((chip_id != CPS8601_CHIP_ID) && (chip_id != CPS8602_CHIP_ID)) {
		hwlog_err("dev_check: rx_chip not match\n");
		return -EINVAL;
	}

	return 0;
}

struct device_node *cps8601_dts_dev_node(void *dev_data)
{
	struct cps8601_dev_info *di = dev_data;

	if (!di || !di->dev)
		return NULL;

	return di->dev->of_node;
}

static int cps8601_gpio_init(struct cps8601_dev_info *di, struct device_node *np)
{
	if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
		return 0;

	if (power_gpio_config_output(np, "gpio_rx_online", "gpio_rx_online",
		&di->gpio_rx_online, 0))
		return -EINVAL;

	if (power_gpio_config_output(np, "gpio_lowpower", "gpio_lowpower",
		&di->gpio_lowpower, 1)) {
		gpio_free(di->gpio_rx_online);
		return -EINVAL;
	}

	return 0;
}

static int cps8601_irq_init(struct cps8601_dev_info *di, struct device_node *np)
{
	if (power_gpio_config_interrupt(np, "gpio_int", "cps8601_int",
		&di->gpio_int, &di->irq_int))
		return -EINVAL;

	if (request_irq(di->irq_int, cps8601_interrupt, IRQF_TRIGGER_FALLING, "cps8601_irq", di)) {
		hwlog_err("irq_init: request cps8601_irq failed\n");
		gpio_free(di->gpio_int);
		return -EINVAL;
	}

	enable_irq_wake(di->irq_int);
	di->irq_active = true;
	INIT_WORK(&di->irq_work, cps8601_irq_work);

	return 0;
}

static void cps8601_register_pwr_dev_info(struct cps8601_dev_info *di)
{
	struct power_devices_info_data *pwr_dev_info = NULL;

	pwr_dev_info = power_devices_info_register();
	if (pwr_dev_info) {
		pwr_dev_info->dev_name = di->dev->driver->name;
		pwr_dev_info->dev_id = di->chip_id;
		pwr_dev_info->ver_id = 0;
	}
}

static void cps8601_ops_unregister(struct wltrx_ic_ops *ops)
{
	if (!ops)
		return;

	if (ops->tx_ops)
		kfree(ops->tx_ops);
	if (ops->fw_ops)
		kfree(ops->fw_ops);
	if (ops->qi_ops)
		kfree(ops->qi_ops);
	kfree(ops);
}

static int cps8601_event_notifier_call(struct notifier_block *nb, unsigned long event, void *data)
{
	struct cps8601_dev_info *di = container_of(nb, struct cps8601_dev_info, event_nb);

	if (!di)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_WLTX_AUX_KB_HALL_AWAY_FROM:
	case POWER_NE_WLTX_AUX_PEN_HALL_AWAY_FROM:
		di->fod_gap = CPS8601_CLEAR_FOD_GAP;
		break;
	default:
		return NOTIFY_OK;
	}

	return NOTIFY_OK;
}

static void cps8601_free_dev_resources(struct cps8601_dev_info *di)
{
	power_event_bnc_unregister(POWER_BNT_WLTX_AUX, &di->event_nb);
	power_wakeup_source_unregister(di->wakelock);
	mutex_destroy(&di->mutex_irq);
	destroy_workqueue(di->system_wq);
	gpio_free(di->gpio_int);
	free_irq(di->irq_int, di);
	if (di->default_psy_type == CPS8601_DEFAULT_LOWPOWER) {
		gpio_free(di->gpio_rx_online);
		gpio_free(di->gpio_lowpower);
	}
}

#ifdef CONFIG_HIBERNATION
static void cps8601_s4_restore_work(struct work_struct *work)
{
	struct cps8601_dev_info *di = NULL;

	if (!work)
		return;
	di = container_of(work, struct cps8601_dev_info, s4_work);
	if (!di)
		return;

	gpio_set_value(di->gpio_rx_online, false);
	wlps_control(di->ic_type, WLPS_TX_PWR_SW, true);
	power_usleep(DT_USLEEP_20MS);
	cps8601_tx_lowpower_enable(true, di);
	hwlog_info("[s4_restore_work] succ\n");
}
#endif /* CONFIG_HIBERNATION */

static int cps8601_request_dev_resources(struct cps8601_dev_info *di, struct device_node *np)
{
	int ret;
	u32 pinctrl_len = (di->default_psy_type == CPS8601_DEFAULT_LOWPOWER) ?
		CPS8601_Q_PINCTRL_LEN : CPS8601_PINCTRL_LEN;

	ret = cps8601_parse_dts(np, di);
	if (ret)
		goto parse_dts_fail;

	ret = power_pinctrl_config(di->dev, "pinctrl-names", pinctrl_len);
	if (ret)
		goto pinctrl_config_fail;

	ret = cps8601_gpio_init(di, np);
	if (ret)
		goto gpio_init_fail;

	ret = cps8601_irq_init(di, np);
	if (ret)
		goto irq_init_fail;

	di->system_wq = alloc_workqueue("cps8601_highpri_unbound_wq",
		WQ_HIGHPRI | WQ_UNBOUND, WQ_UNBOUND_MAX_ACTIVE);
	if (!di->system_wq) {
		ret = -ENOMEM;
		goto alloc_wq_fail;
	}

	di->wakelock = power_wakeup_source_register(di->dev, "cps8601_wakelock");
	mutex_init(&di->mutex_irq);

	di->event_nb.notifier_call = cps8601_event_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_WLTX_AUX, &di->event_nb);
	if (ret)
		goto register_notify_fail;

#ifdef CONFIG_HIBERNATION
	INIT_WORK(&di->s4_work, cps8601_s4_restore_work);
#endif /* CONFIG_HIBERNATION */

	return 0;

register_notify_fail:
	power_wakeup_source_unregister(di->wakelock);
	mutex_destroy(&di->mutex_irq);
	destroy_workqueue(di->system_wq);
alloc_wq_fail:
	gpio_free(di->gpio_int);
	free_irq(di->irq_int, di);
irq_init_fail:
	if (di->default_psy_type == CPS8601_DEFAULT_LOWPOWER) {
		gpio_free(di->gpio_rx_online);
		gpio_free(di->gpio_lowpower);
	}
gpio_init_fail:
pinctrl_config_fail:
parse_dts_fail:
	return ret;
}

static int cps8601_ops_register(struct wltrx_ic_ops *ops, struct cps8601_dev_info *di)
{
	int ret;

	ret = cps8601_fw_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register fw_ops failed\n");
		return ret;
	}

	ret = cps8601_tx_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register tx_ops failed\n");
		return ret;
	}

	ret = cps8601_qi_ops_register(ops, di);
	if (ret) {
		hwlog_err("ops_register: register qi_ops failed\n");
		return ret;
	}
	di->g_val.qi_hdl = hwqi_get_handle();

	return 0;
}

static void cps8601_fw_mtp_check(struct cps8601_dev_info *di)
{
	if (power_cmdline_is_powerdown_charging_mode()) {
		di->g_val.mtp_chk_complete = true;
		return;
	}

	INIT_DELAYED_WORK(&di->mtp_check_work, cps8601_fw_mtp_check_work);
	schedule_delayed_work(&di->mtp_check_work, msecs_to_jiffies(WIRELESS_FW_WORK_DELAYED_TIME));
}

static int cps8601_get_firmware_info(struct cps8601_dev_info *di)
{
	if (!di)
		return -EINVAL;

	switch (di->chip_id) {
	case CPS8601_CHIP_ID:
		di->firm_info.btl_size = CPS8601_FW_BTL_SIZE;
		di->firm_info.btl_ver = CPS8601_BTL_VER;
		di->firm_info.btl = g_cps8601_bootloader;
		if (di->use_extra_firmwire) {
			di->firm_info.mtp_size = CPS8601_FW_MTP_SIZE_EXTRA;
			di->firm_info.mtp_ver = CPS8601_MTP_VER_EXTRA;
			di->firm_info.mtp_crc = CPS8601_MTP_CRC_EXTRA;
			di->firm_info.mtp = g_cps8601_mtp_extra;
		} else {
			di->firm_info.mtp_size = CPS8601_FW_MTP_SIZE;
			di->firm_info.mtp_ver = CPS8601_MTP_VER;
			di->firm_info.mtp_crc = CPS8601_MTP_CRC;
			di->firm_info.mtp = g_cps8601_mtp;
		}
		break;
	case CPS8602_CHIP_ID:
		di->firm_info.btl_size = CPS8602_FW_BTL_SIZE;
		di->firm_info.btl_ver = CPS8602_BTL_VER;
		di->firm_info.mtp_size = CPS8602_FW_MTP_SIZE;
		di->firm_info.mtp_ver = CPS8602_MTP_VER;
		di->firm_info.mtp_crc = CPS8602_MTP_CRC;
		di->firm_info.mtp = g_cps8602_mtp;
		di->firm_info.btl = g_cps8602_bootloader;
		break;
	default:
		hwlog_err("%s: wrong chip id\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int cps8601_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct cps8601_dev_info *di = NULL;
	struct device_node *np = NULL;
	struct wltrx_ic_ops *ops = NULL;

	if (!client || !client->dev.of_node)
		return -ENODEV;

	if (wltx_ic_is_ops_registered(id->driver_data))
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

	ret = cps8601_dev_check(di);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto dev_ck_fail;
	}

	ret = cps8601_request_dev_resources(di, np);
	if (ret)
		goto req_dev_res_fail;

	ret = cps8601_ops_register(ops, di);
	if (ret)
		goto ops_regist_fail;

	ret = cps8601_get_firmware_info(di);
	if (ret)
		goto get_firminfo_fail;

	cps8601_fw_mtp_check(di);
	cps8601_register_pwr_dev_info(di);

	hwlog_info("[ic_type:%u]wireless_chip probe ok\n", di->ic_type);
	return 0;

get_firminfo_fail:
ops_regist_fail:
	cps8601_free_dev_resources(di);
req_dev_res_fail:
dev_ck_fail:
	cps8601_ops_unregister(ops);
	devm_kfree(&client->dev, di);
	return ret;
}

static int cps8601_remove(struct i2c_client *client)
{
	struct cps8601_dev_info *l_dev = i2c_get_clientdata(client);

	if (!l_dev)
		return -ENODEV;

	power_event_bnc_unregister(POWER_BNT_WLTX_AUX, &l_dev->event_nb);
	if (l_dev->default_psy_type == CPS8601_DEFAULT_LOWPOWER) {
		gpio_free(l_dev->gpio_rx_online);
		gpio_free(l_dev->gpio_lowpower);
	}
	gpio_free(l_dev->gpio_int);
	free_irq(l_dev->irq_int, l_dev);
	mutex_destroy(&l_dev->mutex_irq);
	power_wakeup_source_unregister(l_dev->wakelock);
	cancel_delayed_work(&l_dev->mtp_check_work);
	devm_kfree(&client->dev, l_dev);

	return 0;
}

#ifdef CONFIG_PM
static int cps8601_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct cps8601_dev_info *di = NULL;

	if (!client)
		return 0;

	di = i2c_get_clientdata(client);
	if (!di)
		return 0;

	hwlog_info("[suspend]\n");
#ifdef CONFIG_SYS_PM_ULSR
	if (is_ulsr()) {
		cps8601_disable_irq_nosync(di);
		if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
			return 0;
		wlps_control(di->ic_type, WLPS_TX_PWR_SW, false);

		return 0;
	}
#endif /* CONFIG_SYS_PM_ULSR */

	if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
		return 0;

	gpio_set_value(di->gpio_lowpower, CPS8601_Q_LONG_PERIOD);

	return 0;
}

static int cps8601_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct cps8601_dev_info *di = NULL;

	if (!client)
		return 0;

	di = i2c_get_clientdata(client);
	if (!di)
		return 0;

	hwlog_info("[resume]\n");
#ifdef CONFIG_SYS_PM_ULSR
	if (is_ulsr()) {
		irq_set_irq_type(di->irq_int, IRQF_TRIGGER_FALLING);
		cps8601_enable_irq(di);
		if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
			return 0;
		gpio_direction_output(di->gpio_lowpower, CPS8601_Q_SHORT_PERIOD);
		gpio_direction_output(di->gpio_rx_online, false);
		wlps_control(di->ic_type, WLPS_TX_PWR_SW, true);
		cps8601_tx_lowpower_enable(true, di);

		return 0;
	}
#endif /* CONFIG_SYS_PM_ULSR */

	if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
		return 0;

	gpio_set_value(di->gpio_lowpower, CPS8601_Q_SHORT_PERIOD);

	return 0;
}

#ifdef CONFIG_HIBERNATION
static int cps8601_freeze(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct cps8601_dev_info *di = NULL;

	if (!client)
		return 0;

	di = i2c_get_clientdata(client);
	if (!di)
		return 0;

	wlps_control(di->ic_type, WLPS_TX_PWR_SW, false);
	hwlog_info("[freeze]\n");
	return 0;
}

static int cps8601_restore(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct cps8601_dev_info *di = NULL;

	if (!client)
		return 0;

	di = i2c_get_clientdata(client);
	if (!di)
		return 0;

	hwlog_info("[restore]\n");
	if (di->default_psy_type != CPS8601_DEFAULT_LOWPOWER)
		return 0;

	schedule_work(&di->s4_work);
	return 0;
}
#endif /* CONFIG_HIBERNATION */

static const struct dev_pm_ops cps8601_pm_ops = {
	.suspend = cps8601_suspend,
	.resume = cps8601_resume,
#ifdef CONFIG_HIBERNATION
	.freeze = cps8601_freeze,
	.restore = cps8601_restore,
#endif /* CONFIG_HIBERNATION */
};

#define CPS8601_PM_OPS (&cps8601_pm_ops)
#else
#define CPS8601_PM_OPS (NULL)
#endif /* CONFIG_PM */

static void cps8601_shutdown(struct i2c_client *client)
{
	hwlog_info("[shutdown]\n");
}

MODULE_DEVICE_TABLE(i2c, wireless_cps8601);
static const struct i2c_device_id cps8601_i2c_id[] = {
	{ "cps8601", WLTRX_IC_MAIN },
	{ "cps8601_aux", WLTRX_IC_AUX },
	{ "cps8602_aux", WLTRX_IC_AUX },
	{}
};

static struct i2c_driver cps8601_driver = {
	.probe = cps8601_probe,
	.shutdown = cps8601_shutdown,
	.remove = cps8601_remove,
	.id_table = cps8601_i2c_id,
	.driver = {
		.owner = THIS_MODULE,
		.name = "wireless_cps8601",
		.pm = CPS8601_PM_OPS,
	},
};

static int __init cps8601_init(void)
{
	return i2c_add_driver(&cps8601_driver);
}

static void __exit cps8601_exit(void)
{
	i2c_del_driver(&cps8601_driver);
}

device_initcall(cps8601_init);
module_exit(cps8601_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("cps8601 module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
