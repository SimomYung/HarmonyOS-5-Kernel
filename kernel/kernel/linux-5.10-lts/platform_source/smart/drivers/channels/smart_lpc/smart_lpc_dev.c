/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2021. All rights reserved.
 * Description: aios dev drvier to communicate with sensorhub lpc app
 * Create: 2024-11-30
 */
 
#include "smart_lpc_dev.h"
 
#include <asm/io.h>
#include <securec.h>
#include <platform_include/smart/linux/base/ap/protocol.h>
 
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/debugfs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/syscalls.h>
#include <linux/completion.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/of_address.h>
#include <linux/device.h>
#include <linux/poll.h>
#include "inputhub_api/inputhub_api.h"
#include <platform_include/smart/linux/iomcu_status.h>
#include "shmem/shmem.h"

#define LPC_DEV_COMPAT_STR "hisilicon,lpc-dev"

#define lpc_log_info(msg...) pr_info("[I/LPC]" msg)
#define lpc_log_err(msg...) pr_err("[E/LPC]" msg)
#define lpc_log_warn(msg...) pr_warn("[W/LPC]" msg)

#ifdef __LLT_UT__
#define STATIC
#else
#define STATIC static
#endif

struct lpc_read_data_t {
	unsigned int recv_len;
	void *p_recv;
};

typedef int (*lpc_ioctl_ops_f)(struct file *file, unsigned int cmd, unsigned long arg);
typedef int (*lpc_resp_ops_f)(struct pkt_subcmd_resp *p_resp);

struct lpc_ioctl_ops {
	unsigned int cmd;
	lpc_ioctl_ops_f ops;
};

struct lpc_resp_ops {
	unsigned int cmd;
	unsigned int resp_len;
	lpc_resp_ops_f ops;
};

STATIC int resp_session_upload(struct pkt_subcmd_resp *p_resp);

STATIC struct lpc_ioctl_ops g_lpc_ioctl_ops[] = {
	{LPC_IOCTL_LPC_OPEN, NULL},
	{LPC_IOCTL_LPC_CLOSE, NULL},
};

STATIC struct lpc_resp_ops g_lpc_resp_ops[] = {
	{SUB_CMD_LPC_SESSION_UPLOAD, 0, resp_session_upload},
};

enum lpc_read_state {
	LPC_READ_UPDATE_OFF,
	LPC_READ_UPDATE_READY,
};

struct lpc_priv_t {
	struct device *self;            /* self device. */
	struct completion lpc_wait;
	struct mutex read_mutex;        /* Used to protect ops on read */
	struct mutex lpc_mutex;         /* Used to protect ops on ref_cnt/sh_recover_flg */
	struct mutex ioctl_mutex;       /* Used to protect ops on ioctl */
	struct kfifo read_kfifo;
	wait_queue_head_t read_wait;         /* for read */
	unsigned int read_flag;
	int ref_cnt;
	int sh_recover_flag;
	int send_ipc_to_shb;
};

#define LPC_READ_CACHE_COUNT          5
#define LPC_RESET_NOTIFY              0xFFFF
#define LPC_DEV_MAX_UPLOAD_LEN        0x1000

static struct lpc_priv_t g_lpc_priv = { 0 };

STATIC inline unsigned int lpc_dev_get_ioctl_ops_num(void)
{
	return (sizeof(g_lpc_ioctl_ops) / sizeof(struct lpc_ioctl_ops));
}

STATIC inline unsigned int lpc_dev_get_resp_ops_num(void)
{
	return (sizeof(g_lpc_resp_ops) / sizeof(struct lpc_resp_ops));
}

STATIC void lpc_dev_wait_init(struct completion *p_wait)
{
	if (p_wait == NULL) {
		lpc_log_err("lpc dev wait init: wait NULL\n");
		return;
	}

	init_completion(p_wait);
}

STATIC int lpc_dev_wait_completion(struct completion *p_wait, unsigned int tm_out)
{
	if (p_wait == NULL) {
		lpc_log_err("lpc dev wait completion: wait NULL\n");
		return -EFAULT;
	}

	lpc_log_info("lpc dev wait completion: waitting\n");
	if (tm_out != 0) {
		if (wait_for_completion_interruptible_timeout(
			p_wait, msecs_to_jiffies(tm_out)) == 0) {
			lpc_log_warn("lpc dev wait completion: wait timeout\n");
			return -ETIMEOUT;
		}
	} else {
		if (wait_for_completion_interruptible(p_wait)) {
			lpc_log_warn("lpc dev wait completion: wait interrupted.\n");
			return -EFAULT;
		}
	}

	return 0;
}

STATIC void lpc_dev_complete(struct completion *p_wait)
{
	if (p_wait == NULL) {
		lpc_log_err("lpc dev complete: wait NULL\n");
		return;
	}

	complete(p_wait);
}

STATIC long lpc_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = -ENOTTY;
	unsigned int i;
	unsigned int cmd_num = lpc_dev_get_ioctl_ops_num();

	lpc_log_info("%s cmd: [0x%x]\n", __func__, cmd);
	if (file == NULL) {
		lpc_log_err("%s: filp is null\n", __func__);
		return -EINVAL;
	}
	mutex_lock(&g_lpc_priv.ioctl_mutex);

	if (g_lpc_priv.sh_recover_flag == 1) {
		mutex_unlock(&g_lpc_priv.ioctl_mutex);
		lpc_log_info("%s sensorhub in recover mode\n", __func__);
		return -EBUSY;
	}

	reinit_completion(&g_lpc_priv.lpc_wait);

	lpc_log_info("%s reinit completion\n", __func__);

	for (i = 0; i < cmd_num; i++) {
		if (cmd == g_lpc_ioctl_ops[i].cmd)
			break;
	}

	if (i == cmd_num) {
		mutex_unlock(&g_lpc_priv.ioctl_mutex);
		lpc_log_err("%s unknown cmd: [0x%x]\n", __func__, cmd);
		return -ENOTTY;
	}

	if (g_lpc_ioctl_ops[i].ops != NULL)
		ret = g_lpc_ioctl_ops[i].ops(file, cmd, arg);

	mutex_unlock(&g_lpc_priv.ioctl_mutex);

	if (ret != 0)
		lpc_log_err("%s err %x\n", __func__, ret);

	return ret;
}

STATIC int resp_session_upload(struct pkt_subcmd_resp *p_resp)
{
	struct lpc_upload_t *p_upload = NULL;
	struct lpc_read_data_t read_data = {0};
	int ret = 0;

	mutex_lock(&g_lpc_priv.read_mutex);

	if (kfifo_avail(&g_lpc_priv.read_kfifo) < sizeof(struct lpc_read_data_t)) {
		lpc_log_err("%s read_kfifo is full, drop upload data.\n", __func__);
		ret = -EFAULT;
		goto ret_err;
	}

	p_upload = (struct lpc_upload_t *)(p_resp + 1);

	if (p_upload->notify_len > LPC_DEV_MAX_UPLOAD_LEN) {
		lpc_log_err("%s upload length err 0x%x.\n", __func__, p_upload->notify_len);
		ret = -EFAULT;
		goto ret_err;
	}

	read_data.recv_len = (u32)sizeof(struct lpc_upload_t) + p_upload->notify_len;
	read_data.p_recv = kzalloc(read_data.recv_len, GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		lpc_log_err("Failed to alloc memory to save upload resp...\n");
		ret = -EFAULT;
		goto ret_err;
	}

	ret = memcpy_s(read_data.p_recv, read_data.recv_len, p_upload, read_data.recv_len);
	if (ret != 0) {
		lpc_log_err("%s memcpy_s failed...\n", __func__);
		ret = -EFAULT;
		goto ret_err;
	}

	if (kfifo_in(&g_lpc_priv.read_kfifo, (unsigned char *)&read_data,
		(u32)sizeof(struct lpc_read_data_t)) == 0) {
		lpc_log_err("%s: kfifo_in failed\n", __func__);
		ret = -EFAULT;
		goto ret_err;
	}

	g_lpc_priv.read_flag = LPC_READ_UPDATE_READY;
	mutex_unlock(&g_lpc_priv.read_mutex);
	wake_up_interruptible(&g_lpc_priv.read_wait);
	lpc_log_info("Session Upload Received, [0x%x].\n", p_upload->session_id);

	return 0;
ret_err:
	if (read_data.p_recv != NULL) {
		kfree(read_data.p_recv);
		read_data.p_recv = NULL;
	}

	mutex_unlock(&g_lpc_priv.read_mutex);

	return ret;
}

STATIC int lpc_get_resp(const struct pkt_header *head)
{
	struct pkt_subcmd_resp *p_resp = NULL;
	int ret = 0;
	unsigned int i;
	unsigned int cmd_num = lpc_dev_get_resp_ops_num();

	p_resp = (struct pkt_subcmd_resp *)(head);

	if (p_resp == NULL) {
		lpc_log_err("%s: p_resp is null\n", __func__);
		return -EFAULT;
	}

	if (p_resp->hd.tag != TAG_LPC) {
		lpc_log_err("%s: invalid tag [0x%x]\n", __func__, p_resp->hd.tag);
		return -EFAULT;
	}

	lpc_log_info("%s: cmd[%d], length[%d], tag[%d], sub_cmd[%d]\n",
			   __func__, p_resp->hd.cmd, p_resp->hd.length, p_resp->hd.tag, p_resp->subcmd);

	for (i = 0; i < cmd_num; i++) {
		if (p_resp->subcmd == g_lpc_resp_ops[i].cmd)
			break;
	}

	if (i == cmd_num) {
		lpc_log_warn("unhandled cmd: tag[%d], sub_cmd[%d]\n", p_resp->hd.tag, p_resp->subcmd);
		return -EFAULT;
	}

	if (g_lpc_resp_ops[i].resp_len != 0) {
		if ((g_lpc_resp_ops[i].resp_len + 8) != head->length) {
			lpc_log_err("%s: invalid payload length: tag[%d], sub_cmd[%d], length[0x%x]\n",
					  __func__, p_resp->hd.tag, p_resp->subcmd, head->length);
			return -EFAULT;
		}
	}

	if (g_lpc_resp_ops[i].ops != NULL)
		ret = g_lpc_resp_ops[i].ops(p_resp);

	return ret;
}

STATIC void lpc_sensorhub_reset_handler(void)
{
	struct lpc_read_data_t read_data = {0};

	lpc_log_info("enter [%s]\n", __func__);
	if (g_lpc_priv.ref_cnt == 0) {
		lpc_log_err("%s: device not open\n", __func__);
		return;
	}

	mutex_lock(&g_lpc_priv.read_mutex);
	
	if (kfifo_avail(&g_lpc_priv.read_kfifo) <  (u32)sizeof(struct lpc_read_data_t)) {
		lpc_log_err("%s read_kfifo is full, drop upload data.\n", __func__);
		goto ERR;
	}

	read_data.recv_len = (unsigned int)sizeof(u32);
	read_data.p_recv = kzalloc(sizeof(u32), GFP_ATOMIC);
	if (read_data.p_recv == NULL) {
		lpc_log_err("Failed to alloc memory for sensorhub reset message...\n");
		goto ERR;
	}

	*(u32 *)read_data.p_recv = LPC_RESET_NOTIFY;

	if (kfifo_in(&g_lpc_priv.read_kfifo, (unsigned char *)&read_data,
		(u32)sizeof(struct lpc_read_data_t)) == 0) {
		lpc_log_err("%s: kfifo_in failed\n", __func__);
		goto ERR;
	}
	g_lpc_priv.read_flag = LPC_READ_UPDATE_READY;
	mutex_unlock(&g_lpc_priv.read_mutex);
	wake_up_interruptible(&g_lpc_priv.read_wait);
	g_lpc_priv.sh_recover_flag = 1;
	if (g_lpc_priv.send_ipc_to_shb != 0)
		g_lpc_priv.send_ipc_to_shb = 0;

	lpc_dev_complete(&g_lpc_priv.lpc_wait);
	return;

ERR:
	if (read_data.p_recv != NULL) {
		kfree(read_data.p_recv);
		read_data.p_recv = NULL;
	}

	mutex_unlock(&g_lpc_priv.read_mutex);
}

STATIC int lpc_sensorhub_reset_notifier(struct notifier_block *nb, unsigned long action, void *data)
{
	switch (action) {
	case IOM3_RECOVERY_IDLE:
		lpc_sensorhub_reset_handler();
		break;
	default:
		break;
	}

	return 0;
}

static struct notifier_block lpc_reboot_notify = {
	.notifier_call = lpc_sensorhub_reset_notifier,
	.priority = -1,
};

STATIC ssize_t lpc_dev_read(struct file *file,
				  char __user *buf, size_t count, loff_t *pos)
{
	struct lpc_read_data_t read_data = {0};
	u32 error = 0;
	u32 length;

	lpc_log_info("[%s]\n", __func__);
	if (buf == NULL || count == 0)
		goto ERR;

	if (wait_event_interruptible(g_lpc_priv.read_wait, (g_lpc_priv.read_flag == LPC_READ_UPDATE_READY))) {
		lpc_log_err("%s: LPC_READ_STATE not change.\n", __func__);
		goto ERR;
	}

	mutex_lock(&g_lpc_priv.read_mutex);
	g_lpc_priv.read_flag = LPC_READ_UPDATE_OFF;
	if (kfifo_len(&g_lpc_priv.read_kfifo) < sizeof(struct lpc_read_data_t)) {
		lpc_log_err("%s: read data failed.\n", __func__);
		mutex_unlock(&g_lpc_priv.read_mutex);
		goto ERR;
	}

	if (kfifo_out(&g_lpc_priv.read_kfifo, (unsigned char *)&read_data,
		(u32)sizeof(struct lpc_read_data_t)) == 0
		|| !read_data.p_recv) {
		lpc_log_err("%s: kfifo out failed.\n", __func__);
		mutex_unlock(&g_lpc_priv.read_mutex);
		goto ERR;
	}

	if (count < read_data.recv_len) {
		length = (u32)count;
		lpc_log_err("%s user buffer is too small\n", __func__);
	} else {
		length = read_data.recv_len;
	}

	lpc_log_info("[%s] copy len[0x%x], count[0x%x]\n", __func__, read_data.recv_len, (u32)count);

	error = length;
	/* copy to user */
	if (copy_to_user(buf, read_data.p_recv, length)) {
		lpc_log_err("%s failed to copy to user\n", __func__);
		error = 0;
	}

	mutex_unlock(&g_lpc_priv.read_mutex);

ERR:
	if (read_data.p_recv != NULL) {
		kfree(read_data.p_recv);
		read_data.p_recv = NULL;
		read_data.recv_len = 0;
	}

	return error;
}

STATIC ssize_t lpc_dev_write(struct file *file, const char __user *data,
				   size_t len, loff_t *ppos)
{
	lpc_log_info("%s need to do...\n", __func__);
	return (ssize_t)len;
}

STATIC int lpc_dev_open(struct inode *inode, struct file *file)
{
	int ret = 0;

	lpc_log_info("enter %s\n", __func__);
	if (file == NULL) {
		lpc_log_err("%s: file is null\n", __func__);
		return -EFAULT;
	}
	mutex_lock(&g_lpc_priv.lpc_mutex);
	if (g_lpc_priv.ref_cnt != 0) {
		lpc_log_warn("%s duplicate open.\n", __func__);
		mutex_unlock(&g_lpc_priv.lpc_mutex);
		return -EFAULT;
	}

	if (g_lpc_priv.ref_cnt == 0)
		ret = send_cmd_from_kernel(TAG_LPC, CMD_CMN_OPEN_REQ, 0, NULL, (size_t)0);

	file->private_data = &g_lpc_priv;

	if (ret == 0)
		g_lpc_priv.ref_cnt++;

	g_lpc_priv.sh_recover_flag = 0;
	g_lpc_priv.send_ipc_to_shb = 0;

	mutex_unlock(&g_lpc_priv.lpc_mutex);
	return ret;
}

STATIC int lpc_dev_release(struct inode *inode, struct file *filep)
{
	struct read_info rd;

	lpc_log_info("enter %s\n", __func__);

	mutex_lock(&g_lpc_priv.lpc_mutex);
	if (g_lpc_priv.ref_cnt == 0) {
		lpc_log_err("%s: ref cnt is 0.\n", __func__);
		mutex_unlock(&g_lpc_priv.lpc_mutex);
		return -EFAULT;
	}

	g_lpc_priv.ref_cnt--;

	if (g_lpc_priv.ref_cnt == 0) {
		(void)memset_s((void *)&rd, sizeof(struct read_info), 0, sizeof(struct read_info));

		send_cmd_from_kernel_response(TAG_LPC, CMD_CMN_CLOSE_REQ, 0, NULL, (size_t)0, &rd);
		lpc_log_info("%s: got close resp\n", __func__);
	}

	mutex_unlock(&g_lpc_priv.lpc_mutex);
	return 0;
}

STATIC unsigned int lpc_dev_poll(struct file *filp, poll_table *wait)
{
	unsigned int mask = 0;
	if (filp == NULL) {
		lpc_log_err("%s: file is null\n", __func__);
		return mask;
	}
	poll_wait(filp, &g_lpc_priv.read_wait, wait);
	if (g_lpc_priv.read_flag == LPC_READ_UPDATE_READY)
		mask = POLLIN | POLLRDNORM;
	return mask;
}

static const struct file_operations lpc_dev_fops = {
	.owner             = THIS_MODULE,
	.llseek            = no_llseek,
	.unlocked_ioctl    = lpc_dev_ioctl,
	.open              = lpc_dev_open,
	.release           = lpc_dev_release,
	.read              = lpc_dev_read,
	.write             = lpc_dev_write,
	.poll              = lpc_dev_poll,
};

static struct miscdevice lpc_miscdev = {
	.minor =    MISC_DYNAMIC_MINOR,
	.name =     "smart_lpc_dev",
	.fops =     &lpc_dev_fops,
};

STATIC int __init lpc_dev_init(struct platform_device *pdev)
{
	int ret = 0;

	if (is_sensorhub_disabled()) {
		lpc_log_err("sensorhub disabled....\n");
		return -EFAULT;
	}

	ret = misc_register(&lpc_miscdev);
	if (ret != 0) {
		lpc_log_err("%s cannot register miscdev err=%d\n", __func__, ret);
		return ret;
	}
	init_waitqueue_head(&g_lpc_priv.read_wait);
	mutex_init(&g_lpc_priv.read_mutex);
	mutex_init(&g_lpc_priv.lpc_mutex);
	mutex_init(&g_lpc_priv.ioctl_mutex);

	lpc_dev_wait_init(&g_lpc_priv.lpc_wait);
	ret = register_mcu_event_notifier(TAG_LPC, CMD_CMN_CONFIG_RESP, lpc_get_resp);
	if (ret != 0) {
		lpc_log_err("[%s]: register CMD_CMN_CONFIG_RESP notifier failed. [%d]\n", __func__, ret);
		goto ERR1;
	}

	ret = register_iom3_recovery_notifier(REC_USR_LPC, &lpc_reboot_notify);
	if (ret < 0) {
		lpc_log_err("[%s]register_iom3_recovery_notifier fail\n", __func__);
		goto ERR3;
	}

	if (kfifo_alloc(&g_lpc_priv.read_kfifo,
			sizeof(struct lpc_read_data_t) * LPC_READ_CACHE_COUNT, GFP_KERNEL)) {
		lpc_log_err("%s kfifo alloc failed.\n", __func__);
		ret = -ENOMEM;
		goto ERR4;
	}
	g_lpc_priv.ref_cnt = 0;
	g_lpc_priv.self = &(pdev->dev);
	g_lpc_priv.send_ipc_to_shb = 0;
	g_lpc_priv.read_flag = LPC_READ_UPDATE_OFF;

	return 0;
ERR4:
	/* unregister_iom3_recovery_notifier */
ERR3:
ERR2:
	unregister_mcu_event_notifier(TAG_LPC, CMD_CMN_CONFIG_RESP, lpc_get_resp);
ERR1:
	misc_deregister(&lpc_miscdev);

	lpc_log_err("%s : init failed....\n", __func__);

	return ret;
}

STATIC void __exit lpc_dev_exit(void)
{
	lpc_log_info("%s : enter....\n", __func__);

	kfifo_free(&g_lpc_priv.read_kfifo);

	unregister_mcu_event_notifier(TAG_LPC, CMD_CMN_CONFIG_RESP, lpc_get_resp);

	misc_deregister(&lpc_miscdev);
}

/* probe() function for platform driver */
static int lpc_probe(struct platform_device *pdev)
{
	if (pdev == NULL) {
		lpc_log_err("lpc probe: pdev is NULL\n");
		return -EFAULT;
	}

	lpc_log_info("lpc probe...\n");

	return lpc_dev_init(pdev);
}

/* remove() function for platform driver */
static int __exit lpc_remove(struct platform_device *pdev)
{
	lpc_log_info("lpc remove...\n");

	lpc_dev_exit();

	return 0;
}

static const struct of_device_id lpc_match_table[] = {
	{ .compatible = LPC_DEV_COMPAT_STR, },
	{},
};

static struct platform_driver lpc_platdev = {
	.driver = {
		.name = "smart_lpc_dev",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(lpc_match_table),
	},
	.probe  = lpc_probe,
	.remove = lpc_remove,
};

static int __init lpc_main_init(void)
{
	return platform_driver_register(&lpc_platdev);
}

static void __exit lpc_main_exit(void)
{
	platform_driver_unregister(&lpc_platdev);
}

late_initcall_sync(lpc_main_init);
module_exit(lpc_main_exit);