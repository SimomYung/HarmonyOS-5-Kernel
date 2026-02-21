

#include "sensor_channel.h"
#include <huawei_platform/power/hw_astate_dispatch.h>

#if defined(CONFIG_HUAWEI_KSTATE)
int pre_anco_state;
int freeze_sensors[TAG_SENSOR_END];
#ifdef CONFIG_SUPPORT_EXTEND_TAGS
int freeze_sensors_extend[EXTEND_TAGS_NUM];
#endif
#endif

static ssize_t shb_read(struct file *file, char __user *buf, size_t count,
	loff_t *pos)
{
	return inputhub_route_read(ROUTE_SHB_PORT_LAKE, buf, count);
}

static ssize_t shb_write(struct file *file, const char __user *data,
	size_t len, loff_t *ppos)
{
	hwlog_info("%s need to do\n", __func__);
	return len;
}

#if defined(CONFIG_HUAWEI_KSTATE)
static void enable_freeze_sensors(int tag, int tab)
{
	if (tag < TAG_SENSOR_END)
		freeze_sensors[tag] = tab;
#ifdef CONFIG_SUPPORT_EXTEND_TAGS
	if (tag >= TAG_EXTEND_BEGIN && tag < TAG_EXTEND_END)
		freeze_sensors_extend[tag - TAG_EXTEND_BEGIN] = tab;
#endif
}
#endif

static int set_delay_lake_handler(int tag, interval_param_t *delay_param)
{
	int ret;
	uint8_t app_config[16] = { 0 }; /* 16 :  default app config length */

	if (switch_sensor(tag, true, HOST_LAKE)) {
		if (tag == TAG_STEP_COUNTER)
			ret = inputhub_sensor_enable_stepcounter(true,
				TYPE_STANDARD, HOST_LAKE);
		else
			ret = inputhub_sensor_enable(tag, true, HOST_LAKE);
		if (ret) {
			hwlog_err("failed to enable sensor tag %d\n",
				tag);
			return -1;
		}
#if defined(CONFIG_HUAWEI_KSTATE)
		enable_freeze_sensors(tag, 1);
#endif
	}
	if (tag == TAG_STEP_COUNTER) {
		inputhub_sensor_setdelay(tag, delay_param, HOST_LAKE);
		app_config[0] = 1;
		app_config[1] = TYPE_STANDARD;
		return send_app_config_cmd(TAG_STEP_COUNTER,
			app_config, true);
	} else {
		return inputhub_sensor_setdelay(tag, delay_param, HOST_LAKE);
	}
}

static int sensor_flush_lake_handler(unsigned int cmd, int tag, struct ioctl_para *para)
{
	int ret;
	uint8_t app_config[16] = { 0 }; /* 16 :  default app config length */

	if (tag == TAG_STEP_COUNTER) {
		ret = send_sensor_batch_flush_cmd(cmd, para, tag, HOST_LAKE);
		if (ret == 0) {
			/* success */
			app_config[0] = 1;
			app_config[1] = TYPE_STANDARD;
			return send_app_config_cmd(TAG_STEP_COUNTER,
				app_config, true);
		} else {
			/* fail */
			hwlog_err("send_sensor_cmd failed %d in %s %d\n",
				cmd, __func__, __LINE__);
			return ret;
		}
	} else {
		return send_sensor_batch_flush_cmd(cmd, para, tag, HOST_LAKE);
	}
}

static int sensor_batch_enable_check(int tag)
{
#ifdef CONFIG_SYS_PM_ULSR
	if (tag == TAG_ALS) {
		hwlog_err("in ulsr recovery enable sensor tag %d return success\n", tag);
		return 0;
	}
#endif
	hwlog_err("failed to enable sensor tag %d\n", tag);
	return -1;
}

static int disable_sensor_lake(int tag)
{
	switch_sensor(tag, false, HOST_LAKE);
#if defined(CONFIG_HUAWEI_KSTATE)
	enable_freeze_sensors(tag, 0);
#endif
	if (tag == TAG_STEP_COUNTER)
		return inputhub_sensor_enable_stepcounter(false,
			TYPE_STANDARD, HOST_LAKE);
	else
		return inputhub_sensor_enable(tag, false, HOST_LAKE);
}

static int send_sensor_cmd(unsigned int cmd, unsigned long arg)
{
	uint8_t app_config[16] = { 0 }; /* 16 :  default app config length */
	void __user *argp = (void __user *)(uintptr_t)arg;
	int tag;
	int ret;
	struct ioctl_para para;
	interval_param_t delay_param;

	if (get_flag_for_sensor_test())
		return 0;

	if (copy_from_user(&para, argp, sizeof(para)))
		return -EFAULT;

	delay_param.period = para.delay_ms;
	delay_param.batch_count = 1;
	delay_param.mode = AUTO_MODE;
	delay_param.reserved[0] = TYPE_STANDARD; /* for step counter only */

	if (!(para.shbtype >= SENSORHUB_TYPE_BEGIN &&
		para.shbtype < SENSORHUB_TYPE_END)) {
		hwlog_err("error shbtype %d in %s\n", para.shbtype, __func__);
		return -EINVAL;
	}

	tag = get_hal_sensor_type_to_tag(para.shbtype);
	switch (cmd) {
	case SHB_IOCTL_APP_ENABLE_SENSOR:
		send_compass_background_calibration_flag(&para, tag);
		break;
	case SHB_IOCTL_APP_DISABLE_SENSOR:
		return disable_sensor_lake(tag);
	case SHB_IOCTL_APP_DELAY_SENSOR:
		return set_delay_lake_handler(tag, &delay_param);
	case SHB_IOCTL_APP_SENSOR_BATCH:
		if (switch_sensor(tag, true, HOST_LAKE) &&
			inputhub_sensor_enable(tag, true, HOST_LAKE)) {
#if defined(CONFIG_HUAWEI_KSTATE)
			enable_freeze_sensors(tag, 1);
#endif
			return sensor_batch_enable_check(tag);
		}
		send_debugs_command(tag, para.period_ms);
		/* fall-through */
	case SHB_IOCTL_APP_SENSOR_FLUSH:
		return sensor_flush_lake_handler(cmd, tag, &para);
	default:
		hwlog_err("unknown shb_hal_cmd %d in %s\n", cmd, __func__);
		return -EINVAL;
	}
	return 0;
}

static long shb_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	int sensor_mcu_mode;

	/*
	 * MAGNETIC_FIELD_UNCALIBRATED~GEOMAGNETIC_ROTATION_VECTOR
	 */
	switch (cmd) {
	case SHB_IOCTL_APP_ENABLE_SENSOR:
	case SHB_IOCTL_APP_DISABLE_SENSOR:
	case SHB_IOCTL_APP_DELAY_SENSOR:
		break; /* fall through */
	case SHB_IOCTL_APP_GET_SENSOR_MCU_MODE:
		sensor_mcu_mode = get_sensor_mcu_mode();
		hwlog_info("is_sensor_mcu_mode [%d]\n", sensor_mcu_mode);
		if (copy_to_user(argp, &sensor_mcu_mode,
			sizeof(sensor_mcu_mode)))
			return -EFAULT;
		return 0;
	case SHB_IOCTL_APP_SENSOR_BATCH:
		hwlog_info("shb_ioctl cmd : batch flush SHB_IOCTL_APP_SENSOR_BATCH\n");
		break;
	case SHB_IOCTL_APP_SENSOR_FLUSH:
		hwlog_info("shb_ioctl cmd : batch flush SHB_IOCTL_APP_SENSOR_FLUSH\n");
		break;
	default:
		hwlog_err("shb_ioctl unknown cmd : %d\n", cmd);
		return -ENOTTY;
	}
	send_sensor_calib_data();
	return send_sensor_cmd(cmd, arg);
}

/* shb_open: do nothing now */
static int shb_open(struct inode *inode, struct file *file)
{
	hwlog_info("%s ok\n", __func__);
#ifndef CONFIG_SPI_NOR_AGENT
	send_sensor_calib_data();
#endif
	return 0;
}

/* shb_release: do nothing now */
static int shb_release(struct inode *inode, struct file *file)
{
	hwlog_info("%s ok\n", __func__);
	return 0;
}

static int sensor_recovery_notifier(struct notifier_block *nb,
	unsigned long foo, void *bar)
{
	hwlog_info("%s %lu +\n", __func__, foo);
	switch (foo) {
	case IOM3_RECOVERY_IDLE:
	case IOM3_RECOVERY_START:
	case IOM3_RECOVERY_MINISYS:
	case IOM3_RECOVERY_3RD_DOING:
	case IOM3_RECOVERY_FAILED:
		break;
	case IOM3_RECOVERY_DOING:
		reset_calibrate_data();
		enable_sensors_when_recovery_iom3();
		config_sensors_when_recovery_iom3();
		break;
	default:
		hwlog_err("%s -unknow state %ld\n", __func__, foo);
		break;
	}
	hwlog_info("%s -\n", __func__);
	return 0;
}

static struct notifier_block sensor_recovery_notify = {
	.notifier_call = sensor_recovery_notifier,
	.priority = -1,
};

static const struct file_operations shb_fops = {
	.owner          = THIS_MODULE,
	.llseek         =   no_llseek,
	.read           =   shb_read,
	.write          =   shb_write,
	.unlocked_ioctl = shb_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl   = shb_ioctl,
#endif
	.open           =   shb_open,
	.release        =   shb_release,
};

#if defined(CONFIG_HUAWEI_KSTATE)
static void restart_freezed_sensors()
{
	uint8_t app_config[16] = { 0 }; /* 16 :  default app config length */
	uint8_t tag;
	interval_param_t delay_param;

	delay_param.batch_count = 1;
	delay_param.mode = AUTO_MODE;
	delay_param.reserved[0] = TYPE_STANDARD; /* for step counter only */
	for (tag = TAG_SENSOR_BEGIN; tag < TAG_SENSOR_END; tag++) {
		if (freeze_sensors[tag]) {
			switch_sensor(tag, true, HOST_LAKE);
			if (tag == TAG_STEP_COUNTER)
				inputhub_sensor_enable_stepcounter(true, TYPE_STANDARD, HOST_LAKE);
			else
				inputhub_sensor_enable(tag, true, HOST_LAKE);

			delay_param.period = sensor_status.delays[tag].delay_lake;
			if (tag == TAG_STEP_COUNTER) {
				inputhub_sensor_setdelay(tag, &delay_param, HOST_LAKE);
				app_config[0] = 1;
				app_config[1] = TYPE_STANDARD;
				send_app_config_cmd(TAG_STEP_COUNTER,
					app_config, true);
			} else {
				inputhub_sensor_setdelay(tag, &delay_param, HOST_LAKE);
			}
		}
	}
#ifdef CONFIG_SUPPORT_EXTEND_TAGS
	for (tag = TAG_EXTEND_BEGIN; tag < TAG_EXTEND_END; tag++) {
		if (freeze_sensors_extend[tag]) {
			switch_sensor(tag, true, HOST_LAKE);
			inputhub_sensor_enable(tag, true, HOST_LAKE);
			delay_param.period = sensor_status.delays_extend[tag].delay_lake;
			inputhub_sensor_setdelay(tag, &delay_param, HOST_DEFAULT);
		}
	}
#endif
}

static void pause_freeze_sensors()
{
	uint8_t app_config[16] = { 0 }; /* 16 :  default app config length */
	uint8_t tag;

	for (tag = TAG_SENSOR_BEGIN; tag < TAG_SENSOR_END; tag++) {
		if (freeze_sensors[tag]) {
			switch_sensor(tag, false, HOST_LAKE);
			if (tag == TAG_STEP_COUNTER)
				inputhub_sensor_enable_stepcounter(false, TYPE_STANDARD, HOST_LAKE);
			else
				inputhub_sensor_enable(tag, false, HOST_LAKE);
		}
	}
#ifdef CONFIG_SUPPORT_EXTEND_TAGS
	for (tag = TAG_EXTEND_BEGIN; tag < TAG_EXTEND_END; tag++) {
		if (freeze_sensors_extend[tag]) {
			switch_sensor(tag, false, HOST_LAKE);
			inputhub_sensor_enable(tag, false, HOST_LAKE);
		}
	}
#endif
}

static void anco_status_cb(int status)
{
	hwlog_info("anco_status_cb status %d", status);
	switch (status) {
	case ANCO_RUNNING:
		if (pre_anco_state == ANCO_PAUSED || pre_anco_state == ANCO_EXIT)
			restart_freezed_sensors();
		pre_anco_state = ANCO_RUNNING;
		break;
	case ANCO_PRE_FREEZE:
		pre_anco_state = ANCO_PRE_FREEZE;
		break;
	case ANCO_PAUSED:
	case ANCO_EXIT:
		pause_freeze_sensors();
		pre_anco_state = status;
		break;
	case ANCO_FAIL:
		pre_anco_state = ANCO_FAIL;
		break;
	default:
		hwlog_err("unknown status %d", status);
		break;
	}
}

static struct anco_status_subscriber anco_status_cb_subscriber = {
	.name = "sensor_lake",
	.hook = anco_status_cb,
};
#endif

static struct miscdevice senorhub_miscdev = {
	.minor =    MISC_DYNAMIC_MINOR,
	.name =     "sensorhub_lake",
	.fops =     &shb_fops,
};

/*******************************************************************************************
Function:       sensorhub_init
Description:    申请kernel缓冲区并初始化读写指针，注册msic设备，hash表初始化仅在原节点处理,
				注册anco状态处理函数
Data Accessed:  无
Data Updated:   无
Input:          无
Output:         无
Return:         成功或者失败信息
*******************************************************************************************/
static int __init sensorhub_init(void)
{
	int ret;

	if (is_sensorhub_disabled())
		return -1;
	ret = inputhub_route_open(ROUTE_SHB_PORT_LAKE);
	if (ret != 0) {
		hwlog_err("cannot open inputhub route err=%d\n", ret);
		return ret;
	}
	ret = misc_register(&senorhub_miscdev);
	if (ret != 0) {
		inputhub_route_close(ROUTE_SHB_PORT_LAKE);
		hwlog_err("cannot register miscdev err=%d\n", ret);
		return ret;
	}
	register_iom3_recovery_notifier(REC_USR_SENSOR, &sensor_recovery_notify);
#if defined(CONFIG_HUAWEI_KSTATE)
	subscribe_anco_status(&anco_status_cb_subscriber);
#endif
	hwlog_info("%s ok\n", __func__);
	return 0;
}

/*******************************************************************************************
Function:       sensorhub_exit
Description:    解注册anco状态处理函数，释放kernel缓冲区并初始化读写指针，注销msic设备
Data Accessed:  无
Data Updated:   无
Input:          无
Output:         无
Return:         成功或者失败信息
*******************************************************************************************/
static void __exit sensorhub_exit(void)
{
#if defined(CONFIG_HUAWEI_KSTATE)
	unsubscribe_anco_status(&anco_status_cb_subscriber);
#endif
	inputhub_route_close(ROUTE_SHB_PORT_LAKE);
	misc_deregister(&senorhub_miscdev);
	hwlog_info("exit %s\n", __func__);
}

late_initcall_sync(sensorhub_init);
module_exit(sensorhub_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SensorHub lake driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");