/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * mcu.c
 *
 * mcu driver
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
#include "mcu_hc32l110.h"
#include "mcu_hc32l110_i2c.h"
#include "mcu_hc32l110_fw.h"
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_firmware.h>
#include <chipset_common/hwpower/common_module/power_i2c.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <linux/timer.h>
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_nv.h>
#ifdef CONFIG_NVE_AP_KERNEL
#include <linux/mtd/nve_ap_kernel_interface.h>
#endif
#include <linux/string.h>
#include <linux/device.h>

#define hwlog_info(fmt, args...) pr_info("[mcu_hc32l110]" fmt, ##args)
#define hwlog_err(fmt, args...) pr_err("[mcu_hc32l110]" fmt, ##args)

#define MCU_CMD_RTCTIME                       0x03
#define MCU_CMD_SOC_STATUS                    0x0A

#define MCU_FRAME_RTC_LEN                     7
#define MCU_CUR_YEAR                          2000
#define MCU_BASE_YEAR                         1900
#define PROC_VER                              "proc_mcu"
#define PROC_MCU_LOCK                         "mcu_lock_status"
#define MCU_VERSION_PROCLEN                   15
#define MCU_FLASH_LOCK_BUFLEN                 16
#define MCU_HD_ADDR                           0x57
#define MCU_ST_ADDR                           0x56
#define MCU_CMD_STATUS_S0                     0x00
#define MCU_CMD_STATUS_S5                     0x01
#define MCU_CMD_STATUS_S4                     0x02
#define SHUTDOWN_RETRIEVE_TIME_0              0
#define SHUTDOWN_RETRIEVE_TIME_12             12
#define SHUTDOWN_RETRIEVE_TIME_48             48
#define MCU_SHUTDOWN_RETRIEVE_TIME_12_VAL     0x03
#define MCU_SHUTDOWN_RETRIEVE_TIME_48_VAL     0x04
#define BUF_MAX_LEN                           16
#define BASE_DEC                              10
#define MCU_MAX_RST_RETRY_TIME                2
#define MCU_NV_NUMBER                         464
#define MCU_DEV_ID_CHECK_TIMES                3

extern struct rtc_wkalrm poweroff_rtc_alarm;
static int g_sd_time;
static int g_major;
static struct class *g_cls;
static int g_getVersion = 0;
static int g_getI2cPath = 0;
static struct mcu_hc32l110_device_info *g_dev_info;
struct sd_retrieve_map {
	int sd_retrieve_time;
	int mcu_cmd_val;
};

static struct sd_retrieve_map sd_retrieve_table[] = {
	{SHUTDOWN_RETRIEVE_TIME_0, MCU_CMD_STATUS_S5},
	{SHUTDOWN_RETRIEVE_TIME_12, MCU_SHUTDOWN_RETRIEVE_TIME_12_VAL},
	{SHUTDOWN_RETRIEVE_TIME_48, MCU_SHUTDOWN_RETRIEVE_TIME_48_VAL}
};

static ssize_t sd_retrieve_node_show(struct device* dev, struct device_attribute* attr, char *buf)
{
	hwlog_info("shutdown_retrieve node show: saved val = %d\n", g_sd_time);
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_sd_time);
}

static ssize_t sd_retrieve_node_store(struct device* dev, struct device_attribute* attr, const char *buf, size_t count)
{
	long val;
	if (kstrtol(buf, BASE_DEC, &val) < 0) {
		hwlog_err("sd_retrieve_node val invalid\n");
		return -EINVAL;
	}
	if (val < INT_MIN || val > INT_MAX) {
		hwlog_err("sd_retrieve_node val out of int range\n");
		return -EINVAL;
	}

	g_sd_time = (int)val;
	hwlog_info("%s sd_time set %d\n", __func__, g_sd_time);
	if (g_sd_time < SHUTDOWN_RETRIEVE_TIME_0 || g_sd_time > SHUTDOWN_RETRIEVE_TIME_48) {
		return -EINVAL;
	}
	return count;
}

static DEVICE_ATTR(shutdown_time_node, S_IWUSR| S_IRUSR| S_IRGRP| S_IWGRP,
	sd_retrieve_node_show, sd_retrieve_node_store);

struct file_operations mcu_driver_ops = {
	.owner = THIS_MODULE,
};

static int sd_retrieve_node_init(void)
{
	struct device *dev;
	g_major = register_chrdev(0, "mcu_driver", &mcu_driver_ops);
	g_cls = class_create(THIS_MODULE, "hw_peripheral");
	dev = device_create(g_cls, 0, MKDEV(g_major, 0), NULL, "hw_mcu");
	if (sysfs_create_file(&(dev->kobj), &dev_attr_shutdown_time_node.attr)) {
		return -EINVAL;
	}
	return 0;
}

static int sd_retrieve_node_exit(void)
{
	device_destroy(g_cls, MKDEV(g_major, 0));
	class_destroy(g_cls);
	unregister_chrdev(g_major, "mcu_driver");
	return 0;
}

void mcu_hc32l110_hard_reset(struct mcu_hc32l110_device_info *di, int flag)
{
	hwlog_info("hard reset flag [%d], BOOT-1, APP-2\n", flag);

	(void)gpio_direction_output(di->gpio_boot0, (flag == HARD_RESET_FLAG_BOOT) ? (1) : (0));
	power_msleep(DT_MSLEEP_10MS, 0, NULL);

	(void)gpio_direction_output(di->gpio_reset, 1);
	power_msleep(DT_MSLEEP_10MS, 0, NULL);
	(void)gpio_direction_output(di->gpio_reset, 0);
}

static int mcu_hc32l110_decimal_to_hex(int decimal)
{
	return ((decimal / 10) << 4) | (decimal % 10);
}

static void mcu_hc32l110_rtc_time_dec2hex(struct rtc_time *tm)
{
	tm->tm_sec = mcu_hc32l110_decimal_to_hex(tm->tm_sec);
	tm->tm_min = mcu_hc32l110_decimal_to_hex(tm->tm_min);
	tm->tm_hour = mcu_hc32l110_decimal_to_hex(tm->tm_hour);
	tm->tm_wday = mcu_hc32l110_decimal_to_hex(tm->tm_wday);
	tm->tm_mday = mcu_hc32l110_decimal_to_hex(tm->tm_mday);
	tm->tm_mon = mcu_hc32l110_decimal_to_hex(tm->tm_mon + 1);
	tm->tm_year = mcu_hc32l110_decimal_to_hex(tm->tm_year + MCU_BASE_YEAR - MCU_CUR_YEAR);
}

static int mcu_reboot_notify_callback(struct notifier_block *this, unsigned long code, void *unused)
{
	struct rtc_time tm;
	struct mcu_hc32l110_device_info *di = container_of(this, struct mcu_hc32l110_device_info, event_nb);
	if (!di) {
		return NOTIFY_BAD;
	}

	if (!rtc_read_time(di->rtc_dev, &tm)) {
		mcu_hc32l110_rtc_time_dec2hex(&tm);
		mcu_hc32l110_write_rtc_time(di, &tm);
	}

	if (di->is_usb_connected || code != SYS_POWER_OFF || poweroff_rtc_alarm.enabled) {
		return NOTIFY_BAD;
	}

	if (di->enter_std_proc) {
		mcu_hc32l110_write_byte(di, MCU_CMD_SOC_STATUS, MCU_CMD_STATUS_S4);
	} else { // S5 mode
		int val = MCU_CMD_STATUS_S5;
		for (int i = 0; i < sizeof(sd_retrieve_table) / sizeof(sd_retrieve_table[0]); ++i) {
			if (g_sd_time == sd_retrieve_table[i].sd_retrieve_time) {
				val = sd_retrieve_table[i].mcu_cmd_val;
			}
		}
		mcu_hc32l110_write_byte(di, MCU_CMD_SOC_STATUS, val);
	}

	return NOTIFY_DONE;
}

static int mcu_hc32l110_parse_dts(struct device_node *np, struct mcu_hc32l110_device_info *di)
{
	if (of_property_read_bool(np, "hard_reset_none")) {
		di->hard_reset_none = true;
	}
	if (power_gpio_config_output(np, "gpio_reset", "hc32l110_gpio_reset", &di->gpio_reset, 0)) {
		goto reset_config_fail;
	}
	if (power_gpio_config_output(np, "gpio_boot0", "hc32l110_gpio_boot0", &di->gpio_boot0, 0)) {
		goto enable_config_fail;
	}
	return 0;

enable_config_fail:
	gpio_free(di->gpio_reset);
reset_config_fail:
	return -EINVAL;
}

static int has_wakealarm(struct device *dev, const void *data)
{
	struct rtc_device *candidate = to_rtc_device(dev);
	hwlog_info("RTCX: devname is %s!\n", dev_name(dev));
	if (!candidate || !candidate->ops) {
		hwlog_err("RTCX: get rtc failed");
		return 0;
	}
	if (!candidate->ops->read_alarm) {
		hwlog_info("RTCX: read_alarm not support!\n");
		return 0;
	}
	if (!device_may_wakeup(candidate->dev.parent)) {
		hwlog_info("RTCX: can_wakeup not support!\n");
		return 0;
	}
	hwlog_info("RTCX: has_wakealarm!\n");
	return 1;
}

static struct rtc_device *mcu_open_rtc_device(void)
{
	struct rtc_device *rtc = NULL;
	struct device *dev;

	dev = class_find_device(rtc_class, NULL, NULL, has_wakealarm);
	if (dev) {
		rtc = rtc_class_open(dev_name(dev));
		put_device(dev);
		hwlog_info("RTCX: dev ok(name:%s)!\n", dev_name(dev));
	}
	if (!rtc) {
		hwlog_info("RTCX: rtc_class_open failed!\n");
		return NULL;
	} else {
		hwlog_info("RTCX: rtc_class_open success!\n");
		return rtc;
	}
}

static int hc32l110_usb_event_notifier_call(struct notifier_block *nb, unsigned long event, void *data)
{
	hwlog_info("mcu receive usb event\n");
	struct mcu_hc32l110_device_info *di = container_of(nb, struct mcu_hc32l110_device_info, usb_event_nb);

	if (!di) {
		hwlog_err("mcu usb nb call err!");
		return NOTIFY_BAD;
	}

	switch (event) {
		case POWER_NE_USB_DISCONNECT:
			di->is_usb_connected = false;
			hwlog_info("mcu usb disconnected\n");
			break;
		case POWER_NE_USB_CONNECT:
			di->is_usb_connected = true;
			hwlog_info("mcu usb connected\n");
			break;
		default:
			break;
	}

	return NOTIFY_OK;
}

static int mcu_hc32l110_hex_to_decimal(u8 hex)
{
	return (hex >> 4) * 10 + (hex & 0x0F);
}

static void mcu_hc32l110_rtc_time_hex2dec(struct rtc_time *tm)
{
	if (!tm) {
		hwlog_err("rtc_time err\n");
		return;
	}

	tm->tm_sec = mcu_hc32l110_hex_to_decimal(tm->tm_sec);
	tm->tm_min = mcu_hc32l110_hex_to_decimal(tm->tm_min);
	tm->tm_hour = mcu_hc32l110_hex_to_decimal(tm->tm_hour);
	tm->tm_wday = mcu_hc32l110_hex_to_decimal(tm->tm_wday);
	tm->tm_mday = mcu_hc32l110_hex_to_decimal(tm->tm_mday);
	tm->tm_mon = mcu_hc32l110_hex_to_decimal(tm->tm_mon) - 1;
	tm->tm_year = MCU_CUR_YEAR + mcu_hc32l110_hex_to_decimal(tm->tm_year) - MCU_BASE_YEAR;
}

static void mcu_hc32l110_rtc_hctosys(struct rtc_device *rtc)
{
	int err;
	struct rtc_time tm;
	struct timespec64 tv64 = {
		.tv_nsec = NSEC_PER_SEC >> 1,
	};
	hwlog_info("start to hctosys sync mcu time \n");
	err = rtc_read_time(rtc, &tm);
	if (err) {
		hwlog_err("hctosys: unable to read the hardware clock, ret = %d\n", err);
		return;
	}

	hwlog_info("after: soc time: %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", tm.tm_sec, tm.tm_min,
		tm.tm_hour, tm.tm_wday, tm.tm_mday, tm.tm_mon, tm.tm_year);

	tv64.tv_sec = rtc_tm_to_time64(&tm);

#if BITS_PER_LONG == 32
	if (tv64.tv_sec > INT_MAX) {
		return;
	}
#endif

	err = do_settimeofday64(&tv64);
	if (err) {
		hwlog_err("do_settimeofday64 err, ret = %d\n", err);
	}
	hwlog_info("end to hctosys sync mcu time \n");
	return;
}

static int mcu_hc32l110_sync_time_to_soc(struct mcu_hc32l110_device_info *di)
{
	struct rtc_time tm;
	struct rtc_time soc_tm;

	if (!di) {
		return -EINVAL;
	}

	di->rtc_dev = mcu_open_rtc_device();
	if (di->rtc_dev == NULL) {
		hwlog_err("mcu open rtc dev err.\n");
		return -EINVAL;
	}

	if (mcu_hc32l110_read_rtc_time(di, &tm)) {
		hwlog_err("mcu read rtc time err.\n");
		return -EFAULT;
	}

	if (rtc_read_time(di->rtc_dev, &soc_tm)) {
		hwlog_err("soc read rtc time err.\n");
	}

	mcu_hc32l110_rtc_time_hex2dec(&tm);

	hwlog_info("time info from mcu: %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", tm.tm_sec, tm.tm_min,
		tm.tm_hour, tm.tm_wday, tm.tm_mday, tm.tm_mon, tm.tm_year);
	hwlog_info("time info form soc: %02x, %02x, %02x, %02x, %02x, %02x, %02x\n", soc_tm.tm_sec,
		soc_tm.tm_min, soc_tm.tm_hour, soc_tm.tm_wday, soc_tm.tm_mday, soc_tm.tm_mon, soc_tm.tm_year);

	if (rtc_set_time(di->rtc_dev, &tm)) {
		hwlog_err("mcu set rtc time err.\n");
		return -EFAULT;
	}

	mcu_hc32l110_rtc_hctosys(di->rtc_dev);
	return 0;
}

static void mcu_hc32l110_update_std_status(char* str1, char c)
{
	if (str1 == NULL) {
		hwlog_err("%s update std status fail \n", __func__);
		return;
	}

	const char* feature = "std_status=";
	char* pos = strstr(str1, feature);

	if (pos == NULL) {
		(void)strcat_s(str1, NVE_NV_DATA_SIZE, feature);
		int len = strlen(str1);
		if (len + 1 >= NVE_NV_DATA_SIZE) {
			hwlog_err("%s str1 overflow after appending\n", __func__);
			return;
		}
		str1[len] = c;
		str1[len + 1] = '\0';
	} else {
		pos += strlen(feature);
		*pos = c;
	}
}

static int mcu_hc32l110_write_std_status_to_nv(char std_status)
{
	hwlog_info("start to write std status : %c to nv\n", std_status);
	struct opt_nve_info_user nv_info;
 
	(void)memset_s(&nv_info, sizeof(nv_info), 0, sizeof(nv_info));
 
	nv_info.nv_operation = NV_READ;
	nv_info.nv_number = MCU_NV_NUMBER;      // NV Id for S4
	nv_info.valid_size = NVE_NV_DATA_SIZE;
	if (nve_direct_access_interface(&nv_info)) {
		hwlog_err("read nv data fail\n");
		return -EINVAL;
	}

	mcu_hc32l110_update_std_status(&nv_info.nv_data[0], std_status);
	nv_info.nv_operation = NV_WRITE;
	if (nve_direct_access_interface(&nv_info)) {
		hwlog_err("write std status to nv fail\n");
		return -EINVAL;
	}
	return 0;
}

static int mcu_hc32l110_hibernate_callback(struct notifier_block *nb, unsigned long event, void *ptr)
{
	struct mcu_hc32l110_device_info *di = container_of(nb, struct mcu_hc32l110_device_info, std_event_nb);
	if (!di) {
		hwlog_err("mcu hibernate nb callback error!");
		return NOTIFY_BAD;
	}

	switch (event) {
	case PM_HIBERNATION_PREPARE:
		(void)mcu_hc32l110_write_std_status_to_nv('4');
		di->enter_std_proc = true;
		break;
	case PM_POST_HIBERNATION:
	case PM_RESTORE_PREPARE:
	case PM_POST_RESTORE:
		(void)mcu_hc32l110_write_std_status_to_nv('0');
		(void)mcu_hc32l110_write_byte(di, MCU_CMD_SOC_STATUS, 0);
		di->enter_std_proc = false;
		(void)mcu_hc32l110_sync_time_to_soc(di);
		break;
	default:
		break;
	}

	hwlog_info("mcu receive hibernate event %d", event);
	return NOTIFY_OK;
}

static int mcu_hc32l110_register_callback(struct mcu_hc32l110_device_info *di)
{
	di->event_nb.notifier_call = mcu_reboot_notify_callback;
	if (register_reboot_notifier(&di->event_nb)) {
		hwlog_err("register reboot notify err.\n");
		return -EINVAL;
	}

	di->usb_event_nb.notifier_call = hc32l110_usb_event_notifier_call;
	if (power_event_bnc_register(POWER_BNT_CONNECT, &di->usb_event_nb)) {
		hwlog_err("register usb event err.\n");
		goto mcu_free_reboot_nb;
	}

	di->std_event_nb.notifier_call = mcu_hc32l110_hibernate_callback;
	if (register_pm_notifier(&di->std_event_nb)) {
		hwlog_err("register std event err.\n");
		goto mcu_free_usb_nb;
	}

	return 0;

mcu_free_usb_nb:
	power_event_bnc_unregister(POWER_BNT_CONNECT, &di->usb_event_nb);
mcu_free_reboot_nb:
	unregister_reboot_notifier(&di->event_nb);
	return -EINVAL;
}

// return:   0: locked 1:unlocked
static int mcu_check_lock_status(struct mcu_hc32l110_device_info *di)
{
	int ret;
	u8 mcu_data_block[MCU_HC32L110_DATA_INFO_LEN];
	ret = mcu_hc32l110_read_block(di, MCU_HC32L110_FW_DEV_ID_REG,
		mcu_data_block, MCU_HC32L110_DATA_INFO_LEN);
	if (ret) {
		hwlog_err("read mcu status fail, check i2c or mcu status!\n");
		return ret;
	}

	hwlog_info("mcu flash status=0x%x, locked %s\n", mcu_data_block[MCU_HC32L110_DATA_FLASH_STATUS],
		mcu_data_block[MCU_HC32L110_DATA_FLASH_STATUS] == MCU_HC32L110_FLSAH_LOCKED ? "succ" : "fail");
	di->flash_status = mcu_data_block[MCU_HC32L110_DATA_FLASH_STATUS];
	return mcu_data_block[MCU_HC32L110_DATA_FLASH_STATUS] != MCU_HC32L110_FLSAH_LOCKED;
}

static ssize_t mcu_ver_read(struct file *filp, char __user *ubuf,
	size_t cnt, loff_t *ppos)
{
		(void) filp;
		int len = 0;
		char buf[MCU_VERSION_PROCLEN];

		if (g_getI2cPath == MCU_HD_ADDR) {
			len = sprintf_s(buf, MCU_VERSION_PROCLEN, "HD %x\n", g_getVersion);
		} else if (g_getI2cPath == MCU_ST_ADDR) {
			len = sprintf_s(buf, MCU_VERSION_PROCLEN, "ST %x\n", g_getVersion);
		}

		if (len == 0) {
			hwlog_info("mcu_ver_read failed!\n");
			return 0;
		}
		return simple_read_from_buffer(ubuf, cnt, ppos, buf, len);
}

static ssize_t mcu_flash_lock(struct file *filp, char __user *ubuf,
	size_t cnt, loff_t *ppos)
{
	(void)filp;
	char buf[MCU_FLASH_LOCK_BUFLEN];
	int len = 0;
	int locked_time = 0;

	hwlog_info("mcu_flash_lock print!\n");
	while (mcu_check_lock_status(g_dev_info) && locked_time < MCU_HC32L110_FLASH_LOCK_RET_TIME) {
		hwlog_info("mcu flash unlocked, try to lock : %d\n", locked_time + 1);
		locked_time++;
		mcu_hc32l110_write_byte(g_dev_info, MCU_HC32L110_FLSAH_LOCK_REG, MCU_HC32L110_FLSAH_LOCK_CMD);
		power_msleep(DT_MSLEEP_200MS, 0, NULL); // wait locked

		if (!mcu_check_lock_status(g_dev_info)) { // locked, reinit mcu
			hwlog_info("mcu flash locked succes, flash reg val=0x%x!\n", g_dev_info->flash_status);
			mcu_hc32l110_hard_reset(g_dev_info, HARD_RESET_FLAG_APP);
			power_msleep(DT_MSLEEP_100MS, 0, NULL);
			mcu_hc32l110_hard_reset(g_dev_info, HARD_RESET_FLAG_APP);
			power_msleep(DT_MSLEEP_500MS, 0, NULL); // wait mcu init
			len = sprintf_s(buf, MCU_FLASH_LOCK_BUFLEN, "1\n");
			return simple_read_from_buffer(ubuf, cnt, ppos, buf, len);
		}

		if (locked_time == MCU_HC32L110_FLASH_LOCK_RET_TIME) { // return failed;
			len = sprintf_s(buf, MCU_FLASH_LOCK_BUFLEN, "0\n");
			return simple_read_from_buffer(ubuf, cnt, ppos, buf, len);
		}
	}

	len = sprintf_s(buf, MCU_FLASH_LOCK_BUFLEN, "1\n");
	return simple_read_from_buffer(ubuf, cnt, ppos, buf, len);
}

static struct proc_ops mcu_ver_info_fops = {
	.proc_read = mcu_ver_read,
};

static struct proc_ops mcu_flash_lock_fops = {
	.proc_read = mcu_flash_lock,
};

static int mcu_proc_init(void)
{
	struct proc_dir_entry *p_ver = NULL;
	struct proc_dir_entry *p_lock = NULL;
	p_ver = proc_create(PROC_VER, 0444, NULL, &mcu_ver_info_fops);
	if (p_ver == NULL) {
		hwlog_err("mcu_ver_info_proc create failed!\n");
		return -EINVAL;
	}

	p_lock = proc_create(PROC_MCU_LOCK, 0444, NULL, &mcu_flash_lock_fops);
	if (p_lock == NULL) {
		hwlog_err("mcu_lock_proc create failed!\n");
		goto out_proc_lock;
	}

	hwlog_info("mcu_proc_init succ!\n");
	return 0;

out_proc_lock:
	proc_remove(p_ver);
	return -EINVAL;
}

static void mcu_fail_free_rtc(struct mcu_hc32l110_device_info *di)
{
	unregister_reboot_notifier(&di->event_nb);
	power_event_bnc_unregister(POWER_BNT_CONNECT, &di->usb_event_nb);
	unregister_pm_notifier(&di->std_event_nb);
	if (di->rtc_dev != NULL) {
		rtc_class_close(di->rtc_dev);
	}
}

static void mcu_fail_free_gpio(struct mcu_hc32l110_device_info *di)
{
	gpio_free(di->gpio_boot0);
	gpio_free(di->gpio_reset);
}

static void mcu_hc32l110_device_info_init(struct mcu_hc32l110_device_info *di, struct i2c_client *client)
{
	g_dev_info = di;
	di->dev = &client->dev;
	di->client = client;
	i2c_set_clientdata(client, di);
	di->is_low_power_mode = false;
	di->chip_already_init = 1;
	g_sd_time = 0;
	g_getI2cPath = client->addr;
}

static void mcu_hc32l110_check_and_resume(struct mcu_hc32l110_device_info *di)
{
	int i;
	if (di->hard_reset_none) {
		hwlog_info("hard reset is not supported,can not resume!");
		return;
	}

	if (!mcu_hc32l110_fw_get_dev_id(di)) {
		hwlog_info("get mcu dev id success\n");
		return;
	}

	for (i = 0; i < MCU_DEV_ID_CHECK_TIMES; i++) {
		hwlog_err("get mcu dev id error, [START RST MCU], retry times:%d\n", i + 1);
		mcu_hc32l110_hard_reset(di, HARD_RESET_FLAG_APP);
		power_msleep(DT_MSLEEP_100MS, 0, NULL);
		if (!mcu_hc32l110_fw_get_dev_id(di)) {
			hwlog_info("get mcu dev id success, retry times:%d\n", i + 1);
			break;
		}
	}

	if (i == MCU_DEV_ID_CHECK_TIMES) {
		hwlog_err("resume mcu fail\n");
	}
}

static int mcu_hc32l110_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	hwlog_info("in mcu probe of hw_tablet!\n");
	struct mcu_hc32l110_device_info *di;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		hwlog_err("i2c_check_functionality error!\n");
		return -EINVAL;
	}

	di = devm_kzalloc(&client->dev, sizeof(*di), GFP_KERNEL);
	if (!di) {
		hwlog_err("mcu malloc err\n");
		return -ENOMEM;
	}

	mcu_hc32l110_device_info_init(di, client);

	if (mcu_hc32l110_parse_dts(di->dev->of_node, di)) {
		goto fail_free_mem;
	}

	if (mcu_proc_init()) {
		goto fail_free_gpio;
	}

	if (mcu_hc32l110_register_callback(di)) {
		goto fail_free_gpio;
	}

	if (mcu_hc32l110_sync_time_to_soc(di) == (-EINVAL)) {
		goto fail_free_rtc;
	}

	mcu_hc32l110_check_and_resume(di);

	(void)mcu_hc32l110_update_check(di);

	hwlog_info("mcu probe done!\n");
	return 0;

fail_free_rtc:
	mcu_fail_free_rtc(di);
fail_free_gpio:
	mcu_fail_free_gpio(di);
fail_free_mem:
	devm_kfree(&client->dev, di);
	return -EPERM;
}

static int mcu_hc32l110_remove(struct i2c_client *client)
{
	struct mcu_hc32l110_device_info *di = i2c_get_clientdata(client);
	if (!di) {
		return -ENODEV;
	}

	(void)power_event_bnc_unregister(POWER_BNT_CONNECT, &di->usb_event_nb);
	(void)unregister_reboot_notifier(&di->event_nb);
	(void)unregister_pm_notifier(&di->std_event_nb);
	(void)rtc_class_close(di->rtc_dev);
	gpio_free(di->gpio_boot0);
	gpio_free(di->gpio_reset);
	devm_kfree(&client->dev, di);
	return 0;
}

void set_mcu_fw_ver(int ver)
{
	g_getVersion = ver;
	hwlog_info("set fw_ver for AT:%d\n", g_getVersion);
}

static const struct i2c_device_id mcu_hc32l110_idtable[] = {
	{"huawei,mcu-hc32", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, mcu_hc32l110_idtable);

static const struct of_device_id mcu_hc32l110_of_match[] = {
	{.compatible = "huawei,mcu-hc32", .data = NULL},
	{},
};

static struct i2c_driver mcu_hc32l110_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "hw_mcu_hc32l110",
		.of_match_table = mcu_hc32l110_of_match,
	},
	.probe = mcu_hc32l110_probe,
	.remove	= mcu_hc32l110_remove,
	.id_table = mcu_hc32l110_idtable,
};

static int __init mcu_driver_init(void)
{
	int ret;
	hwlog_info("mcu driver init!\n");
	ret = sd_retrieve_node_init();
	if (ret) {
		hwlog_info("sd retrieve node init failed!\n");
		return -EINVAL;
	}

	hwlog_info("sd retrieve node init successfully!\n");
	return i2c_add_driver(&mcu_hc32l110_driver);
}

static void __exit mcu_driver_exit(void)
{
	hwlog_info("mcu driver exit!\n");
	(void)sd_retrieve_node_exit();
	hwlog_info("sd retrieve node exit successfully!\n");
	return i2c_del_driver(&mcu_hc32l110_driver);
}

device_initcall_sync(mcu_driver_init);
module_exit(mcu_driver_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("mcu hc32l110 driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
