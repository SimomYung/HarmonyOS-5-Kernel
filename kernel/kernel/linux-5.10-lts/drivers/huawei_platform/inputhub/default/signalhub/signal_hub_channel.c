

#include "signal_hub_channel.h"
#include "signal_hub_config.h"

#include <linux/err.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/delay.h>

#include "contexthub_boot.h"
#include "contexthub_ext_log.h"
#include "contexthub_recovery.h"
#include "sensor_info.h"
#include <securec.h>

#include <platform_include/smart/linux/base/ap/protocol.h>

static struct mutex signalhub_mipi_mutex;
static DECLARE_WAIT_QUEUE_HEAD(signalhub_waitq);
static int is_new_data_available = 0;
static uint32_t g_mipi_read_data[MAX_MIPI_REPORT_DATA_LEN] = {0};
static bool g_is_dts_inited = false;

int signal_hub_send_to_sensorhub(int cmd, const void *buf, uint32_t length)
{
	int ret = 0;
	struct write_info pkg_ap;

	memset_s(&pkg_ap, sizeof(pkg_ap), 0, sizeof(pkg_ap));
	pkg_ap.tag = TAG_SIGNALHUB;
	pkg_ap.cmd = cmd;
	pkg_ap.wr_buf = buf;
	pkg_ap.wr_len = length;
	ret = write_customize_cmd(&pkg_ap, NULL, true);
	if (ret)
		hwlog_err("%s fail,ret=%d\n", __func__, ret);
	return ret;
}

static ssize_t signal_hub_read(struct file *file, char __user *buf,
	size_t count, loff_t *pos)
{
	hwlog_info("signalhub: enter %s\n", __func__);
	return 0;
}

static ssize_t signal_hub_write(struct file *file, const char __user *data,
	size_t len, loff_t *ppos)
{
	hwlog_info("signalhub: enter %s\n", __func__);
	return len;
}

static int signal_hub_open(struct inode *inode, struct file *file)
{
	hwlog_info("signalhub %s: enter\n", __func__);
	return 0;
}

static int signal_hub_set_status_param(const void __user *argp)
{
	int ret = 0;
	sighub_status_msg msg;

	hwlog_info("signalhub %s: ioctl type config\n", __func__);
	if (!argp) {
		hwlog_err("signalhub %s argp NULL Pointer!\n", __func__);
		return -EINVAL;
	}
	memset_s(&msg, sizeof(sighub_status_msg), 0, sizeof(sighub_status_msg));

  	if (copy_from_user(&msg, argp, sizeof(sighub_status_msg))) {
		hwlog_warn("signalhub %s copy_from_user failed\n", __func__);
		return -EFAULT;
	}

	if (msg.size > SIGHUB_STATUS_MSG_MAX_LEN) {
		hwlog_err("signalhub %s data buff:%u over max\n", __func__, msg.size);
		return -EFAULT;
	}

	hwlog_info("signalhub %s data size:%u, cmd:%u\n", __func__, msg.size, msg.cmd);
	ret = signal_hub_send_to_sensorhub(CMD_CMN_CONFIG_REQ, &msg, sizeof(sighub_status_msg));
	return ret;
}

static int signal_hub_set_config_param(const void __user *argp)
{
	int ret = 0;
	sighub_config_msg msg;

	hwlog_info("signalhub %s: ioctl type config\n", __func__);
	if (!argp) {
		hwlog_err("signalhub %s argp NULL Pointer!\n", __func__);
		return -EINVAL;
	}
	memset_s(&msg, sizeof(sighub_config_msg), 0, sizeof(sighub_config_msg));

  	if (copy_from_user(&msg, argp, sizeof(sighub_config_msg))) {
		hwlog_warn("signalhub %s copy_from_user failed\n", __func__);
		return -EFAULT;
	}

	if (msg.size > SIGHUB_CONFIG_MSG_MAX_LEN) {
		hwlog_err("signalhub %s data buff:%u over max\n", __func__, msg.size);
		return -EFAULT;
	}

	hwlog_info("signalhub %s data size:%u, cmd:%u\n", __func__, msg.size, msg.cmd);
	ret = signal_hub_send_to_sensorhub(CMD_CMN_CONFIG_REQ, &msg, sizeof(sighub_config_msg));
	return ret;
}

static int process_signalhub_data_from_mcu(const struct pkt_header *head)
{
	hwlog_info("signalhub:get data from mcu\n", __func__);
	pkt_signalhub_report_data *report = NULL;
	if (!head)
		return -1;

	report = (pkt_signalhub_report_data *)head;
	hwlog_info("signalhub:%s, data size %d\n", __func__, report->hd.length);
	if (report->hd.length <= 0 || report->hd.length > SIGHUB_MAX_MCU_DATA_SIZE) {
		hwlog_err("input length %u invalid, expected at least: %u, max: %u\n", report->hd.length,
			sizeof(pkt_signalhub_report_data), SIGHUB_MAX_MCU_DATA_SIZE);
		return -EINVAL;
	}
	signalhub_factory_report_data *factory_report = (signalhub_factory_report_data *)report->value;
	hwlog_info("mipi data = %u,%u\n", factory_report->report_data[0], factory_report->report_data[1]);

	switch (factory_report->cmd) {
		case SIGNALHUB_AP_REPORT_MIPI_READ_CMD:
			hwlog_info("handle SIGNALHUB_AP_REPORT_MIPI_READ_CMD\n");
			mutex_lock(&signalhub_mipi_mutex);
			g_mipi_read_data[0] = factory_report->report_data[0];
			g_mipi_read_data[1] = factory_report->report_data[1];
			is_new_data_available = 1;
			mutex_unlock(&signalhub_mipi_mutex);
			break;
		default:
			hwlog_info("handle unknow massage %u\n", factory_report->cmd);
			break;
	}
	hwlog_info("signalhub:%s, get data from mcu success\n", __func__);
	return 0;
}

static int signal_hub_open_sensorhub(void)
{
	int ret;
	struct write_info pkg_ap = {0};

	pkg_ap.tag = TAG_SIGNALHUB;
	pkg_ap.cmd = CMD_CMN_OPEN_REQ;

	ret = write_customize_cmd(&pkg_ap, NULL, true);
	hwlog_info(" %s: send open_req fail, ret = %d\n", __func__, ret);
	return ret;
}

static int signal_hub_close_sensorhub(void)
{
	int ret;
	struct write_info pkg_ap = {0};

	pkg_ap.tag = TAG_SIGNALHUB;
	pkg_ap.cmd = CMD_CMN_CLOSE_REQ;

	ret = write_customize_cmd(&pkg_ap, NULL, true);
	hwlog_info(" %s: send close_req fail, ret = %d\n", __func__, ret);
	return ret;
}

static int signal_hub_send_mipi_init_para(void __user *argp)
{
	int ret = 0;
	sighub_send_msg_data *msg;

	hwlog_info("%s: enter\n", __func__);
	if (!argp) {
		hwlog_err("signalhub %s argp NULL Pointer!\n", __func__);
		return -EINVAL;
	}
	//为sighub_send_msg_data的data成员分配空间,准备加载mipi_init_para
	msg = (sighub_send_msg_data *)kmalloc(sizeof(sighub_send_msg_data) + sizeof(mipi_init_para), GFP_KERNEL);
	if (!msg) {
		hwlog_err("Failed to allocate buffer\n");
		return -EINVAL;
	}

	ret = memset_s(msg,
		sizeof(sighub_send_msg_data) + sizeof(mipi_init_para),
		0,
		sizeof(sighub_send_msg_data) + sizeof(mipi_init_para));
	if (ret != EOK) {
		hwlog_err("%s memset_s failed, ret = %d\n", __func__, ret);
		kfree(msg);
		return ret;
	}

	//强制转换的类型为unsigned char * 保证数据按最小进行分割,存入msg的data
	if (copy_from_user((unsigned char *)msg->data, (unsigned char *)argp, sizeof(mipi_init_para))) {
		hwlog_err("signalhub %s copy_from_user failed\n", __func__);
		kfree(msg);
		return -EFAULT;
	}

	mipi_init_para *mipi_para = (mipi_init_para *)msg->data;
	if (mipi_para == NULL) {
		hwlog_err("mipi_unit is null\n");
		kfree(msg);
		return -EFAULT;
	}

	hwlog_info("mipiPinId= %d,mipiDivFreq= %d", mipi_para->mipiPinId, mipi_para->mipiDivFreq);

	msg->size = sizeof(mipi_init_para);
	msg->cmd = SIGNAL_HUB_FACTORY_INIT_CMD;
	hwlog_info("signalhub %s data size:%u, cmd:%u\n", __func__, msg->size, msg->cmd);
	ret = signal_hub_send_to_sensorhub(CMD_CMN_CONFIG_REQ, msg, sizeof(sighub_send_msg_data) + sizeof(mipi_init_para));

	kfree(msg);
	return ret;
}

static int signal_hub_wait_for_mipi_read(void)
{
	unsigned long remain_time = 0;
	remain_time = wait_event_timeout(signalhub_waitq, is_new_data_available != 0, msecs_to_jiffies(MIPI_READ_TIMEOUT));
	mutex_lock(&signalhub_mipi_mutex);
	is_new_data_available = 0;
	mutex_unlock(&signalhub_mipi_mutex);
	if (remain_time <= 0) {
		hwlog_err("wait for read data timeout\n");
		return -EFAULT;
	}
	return 0;
}

static int signal_hub_read_mipi_data(void __user *argp, mipi_drv_unit *mipi_unit)
{
	//等待sensor回调
	if (signal_hub_wait_for_mipi_read() != 0) {
		hwlog_err("wait for read data fail\n");
		return -EFAULT;
	}

	uint64_t result = 0;
	// 将两个 uint32 的内容放入 uint64 中
	result = ((uint64_t)g_mipi_read_data[1] << 32) | (uint64_t)g_mipi_read_data[0]; // 32:data1高位

	if ((result & ((uint64_t)1 << 52)) != 0) { // 52:reg_read标志位
		//reg_read
		reg_read tmp;
		if (memcpy_s(&tmp,sizeof(tmp), &result, sizeof(result)) != 0) {
			hwlog_err("reg_read memcpy_s fail\n");
			return -EFAULT;
		}
		hwlog_info("reg_read,data: %u\n",tmp.data0);
		mipi_unit->data.bitByte0 = tmp.data0;
	} else if ((result & ((uint64_t)1 << 50)) != 0) { // 50:reg_read_long标志位
		//extend_reg_read_long
		extend_reg_read_long tmp;
		if (memcpy_s(&tmp,sizeof(tmp), &result, sizeof(result)) != 0) {
			hwlog_err("reg_read memcpy_s fail\n");
			return -EFAULT;
		}
		hwlog_info("extend_reg_read_long,data: %u,%u,%u\n",tmp.data0,tmp.data1,tmp.data2);
		mipi_unit->data.bitByte0 = tmp.data0;
		mipi_unit->data.bitByte1 = tmp.data1;
		mipi_unit->data.bitByte2 = tmp.data2;
	} else {
		//extend_reg_read
		extend_reg_read tmp;
		if (memcpy_s(&tmp,sizeof(tmp), &result, sizeof(result)) != 0) {
			hwlog_err("reg_read memcpy_s fail\n");
			return -EFAULT;
		}
		hwlog_info("extend_reg_read,data: %u,%u,%u,%u\n",tmp.data0,tmp.data1,tmp.data2,tmp.data3);
		mipi_unit->data.bitByte0 = tmp.data0;
		mipi_unit->data.bitByte1 = tmp.data1;
		mipi_unit->data.bitByte2 = tmp.data2;
		mipi_unit->data.bitByte3 = tmp.data3;
	}

	if (copy_to_user(argp, mipi_unit, sizeof(mipi_drv_unit))) {
		hwlog_err("copy_to_user fail\n");
		return -EINVAL;
	}
	mutex_lock(&signalhub_mipi_mutex);
	g_mipi_read_data[0] = 0;
	g_mipi_read_data[1] = 0;
	mutex_unlock(&signalhub_mipi_mutex);
	return 0;
}


static int signal_hub_send_mipi_data(void __user *argp)
{
	int ret = 0;
	sighub_send_msg_data *msg;

	hwlog_info("%s: enter\n", __func__);
	if (!argp) {
		hwlog_err("signalhub %s argp NULL Pointer!\n", __func__);
		return -EINVAL;
	}
	//为sighub_send_msg_data的data成员分配空间,准备加载mipi_drv_unit
	msg = (sighub_send_msg_data *)kmalloc(sizeof(sighub_send_msg_data) + sizeof(mipi_drv_unit), GFP_KERNEL);
	if (!msg) {
		hwlog_err("Failed to allocate buffer\n");
		return -EINVAL;
	}

	ret = memset_s(msg,
		sizeof(sighub_send_msg_data) + sizeof(mipi_drv_unit),
		0,
		sizeof(sighub_send_msg_data) + sizeof(mipi_drv_unit));
	if (ret != EOK) {
		hwlog_err("%s memset_s failed, ret = %d\n", __func__, ret);
		kfree(msg);
		return ret;
	}

	//强制转换的类型为unsigned char * 保证数据按最小进行分割,存入msg的data
	if (copy_from_user((unsigned char *)msg->data, (unsigned char *)argp, sizeof(mipi_drv_unit))) {
		hwlog_err("signalhub %s copy_from_user failed\n", __func__);
		kfree(msg);
		return -EFAULT;
	}

	mipi_drv_unit *mipi_unit = (mipi_drv_unit *)msg->data;
	if (mipi_unit == NULL) {
		hwlog_err("mipi_unit is null\n");
		kfree(msg);
		return -EFAULT;
	}

	hwlog_info("bitRwFlag= %d,bitByteCnt= %d,bitMipiPort= %d,bitSlaveId= %d,bitRegAddr= %d,bitRateCtrl= %d,"
				"bitMipiCtrl= %d,rsv= %d,bitByte=%d,%d,%d,%d",
		mipi_unit->cmd.bitRwFlag, mipi_unit->cmd.bitByteCnt, mipi_unit->cmd.bitMipiPort, mipi_unit->cmd.bitSlaveId,
		mipi_unit->cmd.bitRegAddr, mipi_unit->cmd.bitRateCtrl, mipi_unit->cmd.bitMipiCtrl, mipi_unit->cmd.rsv,
		mipi_unit->data.bitByte0, mipi_unit->data.bitByte1, mipi_unit->data.bitByte2, mipi_unit->data.bitByte3);

	msg->size = sizeof(mipi_drv_unit);
	msg->cmd = SIGNAL_HUB_FACTORY_CMD;
	hwlog_info("signalhub %s data size:%u, cmd:%u\n", __func__, msg->size, msg->cmd);
	ret = signal_hub_send_to_sensorhub(CMD_CMN_CONFIG_REQ, msg, sizeof(sighub_send_msg_data) + sizeof(mipi_drv_unit));

	//读mipi标识位,需要等待返回值
	if (mipi_unit->cmd.bitRwFlag == 0) {
		ret = signal_hub_read_mipi_data(argp, mipi_unit);
	}

	kfree(msg);
	return ret;
}

static long signal_hub_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int32_t ret = -1;
	void __user *argp = (void __user *)(uintptr_t)arg;
	hwlog_info("signal_hub_: enter %s, cmd: %u\n", __func__, cmd);
	switch (cmd) {
		case SIGHUB_IOCTL_STATUS_CHANGE_CMD:
			ret = signal_hub_set_status_param(argp);
			break;
		case SIGHUB_IOCTL_CONFIG_CHANGE_CMD:
			ret = signal_hub_set_config_param(argp);
			break;
		case SIGHUB_IOCTL_OPEN_CMD:
			hwlog_info("signalhub:%s open cmd : %u\n", __func__, cmd);
			if (g_is_dts_inited == false) {
				init_config_from_dts();
				g_is_dts_inited = true;
			}
			ret = signal_hub_open_sensorhub();
			break;
		case SIGHUB_IOCTL_CLOSE_CMD:
			hwlog_info("signalhub:%s close cmd : %u\n", __func__, cmd);
			ret = signal_hub_close_sensorhub();
			break;
		case SGHB_IOCTL_SGHB_MIPI_INIT_CMD:
			hwlog_info("signalhub:%s MIPI init cmd : %u\n", __func__, cmd);
			ret = signal_hub_send_mipi_init_para(argp);
			break;
		case SGHB_IOCTL_SGHB_MIPI_RW_CMD:
			hwlog_info("signalhub:%s MIPI R/W cmd : %u\n", __func__, cmd);
			ret = signal_hub_send_mipi_data(argp);
			break;
		default:
			hwlog_err("signalhub:%s unknown cmd : %u\n", __func__, cmd);
			return -ENOTTY;
	}

	return ret;
}


/* file_operations to signal_hub */
static const struct file_operations signal_hub_fops = {
	.owner		     = THIS_MODULE,
	.read		      = signal_hub_read,
	.write		     = signal_hub_write,
	.unlocked_ioctl    = signal_hub_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl      = signal_hub_ioctl,
#endif
	.open		      = signal_hub_open,
};

/* miscdevice to signal_hub */
static struct miscdevice signal_hub_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "signal_hub",
	.fops = &signal_hub_fops,
};

/*
* Description:	apply kernel buffer, register signal_hub_miscdev
* Return:		result of function, 0 successed, else false
*/
static int __init signal_hub_init(void)
{
	int ret;

	if (is_sensorhub_disabled())
		return -1;

	ret = misc_register(&signal_hub_miscdev);
	if (ret != 0) {
		hwlog_err("%s cannot register miscdev err=%d\n", __func__, ret);
		return ret;
	}

	/* register mcu data callback */
	register_mcu_event_notifier(TAG_SIGNALHUB, CMD_DATA_REQ, process_signalhub_data_from_mcu);
	hwlog_info("%s signal hub regist ok\n", __func__);

	/* initialize waitqueue for mipi read */
	init_waitqueue_head(&signalhub_waitq);
	mutex_init(&signalhub_mipi_mutex);

	return ret;
}


/* release kernel buffer, deregister signal_hub_miscdev */
static void __exit signal_hub_exit(void)
{
	misc_deregister(&signal_hub_miscdev);
	hwlog_info("signal hub exit %s\n", __func__);
}

late_initcall_sync(signal_hub_init);
module_exit(signal_hub_exit);


MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SignalHub driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");