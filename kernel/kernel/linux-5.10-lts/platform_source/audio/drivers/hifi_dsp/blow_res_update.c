/*
 * blow_res_update.c
 *
 * blow detection result notify lpc
 *
 * Copyright (c) 2024-2025 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 
#include "blow_res_update.h"
 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/poll.h>
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif
#include "securec.h"
#include "audio_log.h"
#include "audio_common_msg.h"
 
#define BLOW_RES_NOTIFY_MSG  _IO('N', 0x1)
#define LIMIT_BLOW_ALGO_CNT 10  // DO NOT CHANGE


enum blow_res_update_state {
	BLOW_RES_UPDATE_OFF,
	BLOW_RES_UPDATE_READY,
	BLOW_RES_UPDATE_NUM
};

struct blow_res_data {
	unsigned short sound_intensity;  // 0 ~ 12; 0: no blow detected
	unsigned short sound_direction; // 0: no blow; 1: near bottom mic; 2: middle; 3: near top mic
	unsigned short face_x_coord_min;  // 0 ~ 480; (0,0,0,0) - invalid
	unsigned short face_x_coord_max;  // 0 ~ 480;
	unsigned short face_y_coord_min;  // 0 ~ 640;
	unsigned short face_y_coord_max;  // 0 ~ 640;
	unsigned int time_stamp_face_msg;  // time stamp when face positon recorded
	unsigned int time_stamp_blow_res;  // time stamp of blow detection result
};

struct blow_res_node_info {
	char *name;
	dev_t devt;
	struct cdev blow_res_cdev;
	struct class *class;
	struct blow_res_data *p_blow_res_data;
	unsigned int flag;
	wait_queue_head_t wait;
};

struct blow_res_data g_blow_res_data;

struct blow_res_node_info g_blow_res_node = {
		.name = "blow_res_update_wallpaper",
		.p_blow_res_data = &g_blow_res_data,
		.flag = BLOW_RES_UPDATE_OFF,
};

struct mutex blow_res_ioctl_mutex;

static unsigned short g_ls_sound_intensity[LIMIT_BLOW_ALGO_CNT + 1];
static unsigned short g_ls_sound_direction[LIMIT_BLOW_ALGO_CNT + 1];
static unsigned int g_ls_timestamp[LIMIT_BLOW_ALGO_CNT + 1];

static ssize_t blow_res_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	size_t len;
	struct blow_res_node_info *node = NULL;
 
	if (!filp)
		return -ENOENT;
 
	if (!buf)
		return -EINVAL;
 
	if (!pos)
		return -EFAULT;
 
	node = (struct blow_res_node_info *)filp->private_data;
	if (!node) {
		AUDIO_LOGE("node is null");
		return -EIO;
	}
 
	if (wait_event_interruptible(node->wait, (node->flag == BLOW_RES_UPDATE_READY))) { //lint !e578
		AUDIO_LOGW("blow result state not change");
		return -ERESTARTSYS;
	}
 
	node->flag = BLOW_RES_UPDATE_OFF;
	len = sizeof(struct blow_res_data);
	AUDIO_LOGI("blow res pos %lld, count %zu, len %zu", *pos, count, len);
	if (count < len) {
		AUDIO_LOGE("blow res usr count need larger, count %zu", count);
		return -EFAULT;
	}
	if (copy_to_user(buf, node->p_blow_res_data, len)) {
		AUDIO_LOGE("blow res copy to user fail");
		return -EFAULT;
	}
 
	return (ssize_t)len;
}

static int blow_res_release(struct inode *inode, struct file *filp)
{
	if (!inode)
		return -EIO;
 
	if (!filp)
		return -ENOENT;
 
	if (MINOR(g_blow_res_node.devt) == iminor(inode)) {
		filp->private_data = NULL;
		g_blow_res_node.flag = BLOW_RES_UPDATE_OFF;
		AUDIO_LOGI("blow res release succ. devt %d", g_blow_res_node.devt);
		return 0;
	}
 
	AUDIO_LOGE("blow res no dev to release");
 
	return -EBUSY;
}

static int blow_res_open(struct inode *inode, struct file *filp)
{
	if (!inode)
		return -EIO;
 
	if (!filp)
		return -ENOENT;
 
	if (MINOR(g_blow_res_node.devt) == iminor(inode)) {
		g_blow_res_node.flag = BLOW_RES_UPDATE_OFF;
		filp->private_data = &g_blow_res_node;
		AUDIO_LOGI("blow res open succ devt %d", g_blow_res_node.devt);
		return 0;
	}
 
	AUDIO_LOGE("blow res no device can be opened");
 
	return -EBUSY;
}

static unsigned int blow_res_poll(struct file *filp, poll_table *wait)
{
	struct blow_res_node_info *node = NULL;
	unsigned int mask = 0;

	if (!filp)
		return -ENOENT;

	if (!wait)
		return -EINVAL;

	node = (struct blow_res_node_info *)filp->private_data;
	if (!node) {
		AUDIO_LOGE("node is null");
		return -EIO;
	}

	poll_wait(filp, &node->wait, wait);

	if (node->flag == BLOW_RES_UPDATE_READY) {
		mask = POLLIN | POLLRDNORM;
	}

	return mask;
}

static void blow_res_update_notify(void)
{
	g_blow_res_node.flag = BLOW_RES_UPDATE_READY;
	wake_up_interruptible(&g_blow_res_node.wait);
}

static long blow_res_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
	long ret = 0;

	mutex_lock(&blow_res_ioctl_mutex);
	switch (cmd) {
	case BLOW_RES_NOTIFY_MSG:
		blow_res_update_notify();
		break;
	default:
		ret = -EINVAL;
		break;
	}
	mutex_unlock(&blow_res_ioctl_mutex);

	return ret;
}

static long blow_res_ioctl32(struct file *fd, unsigned int cmd, unsigned long arg)
{
	void *user_ptr = compat_ptr(arg);

	return blow_res_ioctl(fd, cmd, (uintptr_t)user_ptr);
}

static const struct file_operations blow_res_fops = {
	.owner = THIS_MODULE,
	.open = blow_res_open,
	.read = blow_res_read,
	.release = blow_res_release,
	.poll = blow_res_poll,
    .unlocked_ioctl = blow_res_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = blow_res_ioctl32,
#endif
};

static int blow_res_update_init_node(dev_t devt)
{
	int ret;
	struct blow_res_node_info *node = &g_blow_res_node;
 
	AUDIO_LOGI("init %s", node->name);
 
	init_waitqueue_head(&node->wait);
	node->devt = MKDEV(MAJOR(devt), 0);
	cdev_init(&node->blow_res_cdev, &blow_res_fops);
	ret = cdev_add(&node->blow_res_cdev, node->devt, 1);
	if (ret != 0) {
		AUDIO_LOGE("cdev add fail %d", ret);
		return -EFAULT;
	}
 
	node->class = class_create(THIS_MODULE, node->name);
	if (!node->class) {
		AUDIO_LOGE("class creat fail");
		goto class_create_error;
	}
 
	if (IS_ERR(device_create(node->class, NULL, node->devt, &node, "%s", node->name))) {
		AUDIO_LOGE("device create fail");
		goto device_create_error;
	}
 
	return 0;

device_create_error:
	class_destroy(node->class);
	node->class = NULL;
class_create_error:
	cdev_del(&node->blow_res_cdev);

	return -EPERM;
}

static void blow_res_update_exit_node(void)
{
	struct blow_res_node_info *node = &g_blow_res_node;
 
	device_destroy(node->class, node->devt);
	class_destroy(node->class);
	node->class = NULL;
	cdev_del(&node->blow_res_cdev);
}

int blow_res_update_init(void)
{
    int ret;
	dev_t devt;
 
	ret = alloc_chrdev_region(&devt, 0, 1, "blow_res_update");
	if (ret != 0) {
		AUDIO_LOGE("alloc chrdev region fail");
		return -EFAULT;
	}
 
    ret = blow_res_update_init_node(devt);
    if (ret != 0) {
		unregister_chrdev_region(MKDEV(MAJOR(devt), 0), 1);
		AUDIO_LOGE("init dev node fail. ret %d", ret);
        return -EPERM;
    }

	mutex_init(&blow_res_ioctl_mutex);
 
    AUDIO_LOGI("blow res update init succeed");
    return 0;
}

void blow_res_update_deinit(void)
{
	dev_t devt = g_blow_res_node.devt;
 
	blow_res_update_exit_node();

	mutex_destroy(&blow_res_ioctl_mutex);
 
	unregister_chrdev_region(MKDEV(MAJOR(devt), 0), 1);

	AUDIO_LOGI("blow res update deinit succeed");
}

static void blow_res_print(struct blow_res_data *blow_res) 
{
	static int log_cnt = 0;

	g_ls_sound_intensity[log_cnt] = blow_res->sound_intensity;
	g_ls_sound_direction[log_cnt] = blow_res->sound_direction;
	g_ls_timestamp[log_cnt] = blow_res->time_stamp_blow_res;

	if (log_cnt == LIMIT_BLOW_ALGO_CNT - 1) {
		AUDIO_LOGI("last %u blow sound intensity: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u", 
			LIMIT_BLOW_ALGO_CNT, g_ls_sound_intensity[0], g_ls_sound_intensity[1], g_ls_sound_intensity[2],
			g_ls_sound_intensity[3], g_ls_sound_intensity[4], g_ls_sound_intensity[5], g_ls_sound_intensity[6],
			g_ls_sound_intensity[7], g_ls_sound_intensity[8], g_ls_sound_intensity[9]);
		AUDIO_LOGI("last %u blow sound direction: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u", 
			LIMIT_BLOW_ALGO_CNT, g_ls_sound_direction[0], g_ls_sound_direction[1], g_ls_sound_direction[2],
			g_ls_sound_direction[3], g_ls_sound_direction[4], g_ls_sound_direction[5], g_ls_sound_direction[6],
			g_ls_sound_direction[7], g_ls_sound_direction[8], g_ls_sound_direction[9]);
		AUDIO_LOGI("last %u blow time stamp: %u, %u, %u, %u, %u, %u, %u, %u, %u, %u", 
			LIMIT_BLOW_ALGO_CNT, g_ls_timestamp[0], g_ls_timestamp[1], g_ls_timestamp[2],
			g_ls_timestamp[3], g_ls_timestamp[4], g_ls_timestamp[5], g_ls_timestamp[6],
			g_ls_timestamp[7], g_ls_timestamp[8], g_ls_timestamp[9]);
		log_cnt = 0;
	} else {		
		log_cnt++;
	}
}

void rcv_dsp_blow_cognition_res_msg(enum socdsp_om_work_id work_id, const uint8_t *data, uint32_t len)
{
	struct blow_cognition_res_msg *msg = NULL;
	int ret;

	if (!data || len != sizeof(*msg)) {
		AUDIO_LOGE("data is null or size is invaled, size: %u", len);
		return;
	}

	msg = (struct blow_cognition_res_msg *)data;

	ret = memcpy_s(&g_blow_res_data, sizeof(struct blow_res_data), msg, sizeof(struct blow_cognition_res_msg));
	if (ret != 0) {
		AUDIO_LOGE("copy blow result memory fail ret %d", ret);
	} else {
		blow_res_print(&g_blow_res_data);
		blow_res_update_notify();
	}
}