/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implement of camera sync
 * Create: 2023-12-19
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <cam_log.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <cam_sync.h>
#include <huawei_platform/power/hw_astate_dispatch.h>
#include <securec.h>

static atomic_t g_anco_status = ATOMIC_INIT(0);
static void anco_status_cb(int status)
{
	atomic_set(&g_anco_status, status);
	cam_info("camera ancostatus = %d\n", status);
}

static struct anco_status_subscriber anco_status_cb_subscriber = {
	.name = "camera",
	.hook = anco_status_cb,
};

struct pid_mutex {
	const char* name;
	atomic_t locked_pid;
	wait_queue_head_t waitq;
};

static void init_pid_mutex(struct pid_mutex* mutex, const char* name) {
	if (!mutex)
		return;
	mutex->name = name;
	init_waitqueue_head(&mutex->waitq);
	atomic_set(&mutex->locked_pid, 0);
}

void hisp_update_hisp_uid(bool force);
static int pid_mutex_lock(struct pid_mutex* mutex) {
	if (!mutex)
		return -EBADF;

	pid_t me = current->tgid;
	pid_t cur = 0;

	while (true) {
		cur = atomic_cmpxchg(&mutex->locked_pid, 0, me);
		if (cur == 0 || cur == me)
			break;

		cam_warn("%d acquire %s failed, waiting", me, mutex->name);
		if (wait_event_interruptible(mutex->waitq, !atomic_read(&mutex->locked_pid))) {
			cam_warn("%d interrupted, exit", me);
			return -EAGAIN;
		}
	}

	cam_info("%d acquire %s", me, mutex->name);
	return 0;
}

static void pid_mutex_unlock(pid_t me, struct pid_mutex* mutex) {
	if (!mutex)
		return;

	if (atomic_cmpxchg(&mutex->locked_pid, me, 0) == me) {
		cam_info("%d release %s", me, mutex->name);
		wake_up(&mutex->waitq);
	}
}

#define MAX_MSGS 128
struct msg_queue
{
	struct cam_sync_msg msgs[MAX_MSGS];
	uint16_t pending_msg;
	uint16_t front;
	uint16_t rear;
	wait_queue_head_t waitq;
};

static void init_msg_queue(struct msg_queue* que)
{
	if (!que)
		return;

	que->pending_msg = 0;
	que->front = 0;
	que->rear = MAX_MSGS - 1;
	init_waitqueue_head(&que->waitq);
}

static int enqueue_msg(struct msg_queue* que, void* data, uint32_t len)
{
	struct cam_sync_msg* msg = NULL;
	unsigned long flags;

	if (!que || !data || len > sizeof(struct cam_sync_msg)) {
		cam_err("%s: providing invalid msg pool or msg data, len(%u)", __func__, que, data, len);
		return -EINVAL;
	}

	spin_lock_irqsave(&que->waitq.lock, flags);
	if (que->pending_msg == MAX_MSGS) {
		spin_unlock_irqrestore(&que->waitq.lock, flags);
		cam_warn("%s msg queue is full", __func__);
		return -ENOMEM;
	}

	que->rear = (que->rear + 1) % MAX_MSGS;
	msg = &que->msgs[que->rear];

	if (get_fs() == KERNEL_DS) {
		if (memcpy_s(msg, sizeof(struct cam_sync_msg), data, len) != EOK) {
			cam_err("%s: memcpy_s failed, dest:%zu, len:%u", __func__, sizeof(struct cam_sync_msg), len);
			goto copy_fail;
		}
	} else {
		if (copy_from_user(msg, data, len) != EOK) {
			cam_err("%s: copy_from_user failed, dest:%zu, len:%u", __func__,
					sizeof(struct cam_sync_msg), len);
			goto copy_fail;
		}
	}
	++que->pending_msg;
	spin_unlock_irqrestore(&que->waitq.lock, flags);
	cam_debug("%s: enqueue msg type = %d", __func__, msg->type);
	wake_up(&que->waitq);
	return 0;

copy_fail:
	que->rear = (que->rear - 1 + MAX_MSGS) % MAX_MSGS;
	spin_unlock_irqrestore(&que->waitq.lock, flags);
	return -EFAULT;
}

static int dequeue_msg(struct msg_queue* que, void* data, uint32_t len)
{
	struct cam_sync_msg msg = {};
	unsigned long flags;
	if (!que || !data) {
		cam_err("%s: providing invalid msg pool or msg data", __func__);
		return -EINVAL;
	}

	if (wait_event_interruptible(que->waitq, que->pending_msg != 0)) {
		cam_warn("%s interrupted, exit", __func__);
		return -EAGAIN;
	}

	spin_lock_irqsave(&que->waitq.lock, flags);
	if (que->pending_msg == 0) {
		cam_warn("%s no available msgs", __func__);
		spin_unlock_irqrestore(&que->waitq.lock, flags);
		return -EAGAIN;
	}
	msg = que->msgs[que->front];
	que->front = (que->front + 1) % MAX_MSGS;
	--que->pending_msg;
	spin_unlock_irqrestore(&que->waitq.lock, flags);

	if (len != sizeof(struct cam_sync_msg))
		cam_debug("%s: send msg size:%u, recv size:%u",
			__func__, sizeof(struct cam_sync_msg), len);

	len = min_t(u32, len, sizeof(struct cam_sync_msg));
	if (copy_to_user(data, &msg, len)) {
		cam_err("%s: copy to user failed, dest:%zu, len:%u",
			__func__, sizeof(struct cam_sync_msg), len);
		return -EFAULT;
	}

	return len; // if copy successful, return bytes copied
}

#define MAX_CLIENTS 2
struct client_info {
	pid_t pid;
	struct file* filp;
	struct msg_queue queue;
};
struct cam_sync_device {
	struct miscdevice dev;
	struct pid_mutex online_lock;
	struct pid_mutex hisp_power_lock;

	struct mutex lock;
	struct client_info clients[MAX_CLIENTS];
};

static void init_client_info(struct client_info *info, pid_t pid, struct file *filp)
{
	if (!info)
		return;
	info->pid = pid;
	info->filp = filp;
	init_msg_queue(&info->queue);
}

static void destroy_client_info(struct client_info *info)
{
	info->pid = 0;
	info->filp = NULL;
}

static long send_msg_to_other(struct cam_sync_device* dev, pid_t me, unsigned long arg, size_t len)
{
	long i;
	for (i = 0; i < MAX_CLIENTS; ++i)
		if (dev->clients[i].pid != me && dev->clients[i].pid != 0)
			return enqueue_msg(&dev->clients[i].queue, arg, len);
	cam_warn("%s: not found other", __func__);
	return -ENOENT;
}

static long recv_msg_from_other(struct cam_sync_device* dev, pid_t me, void* arg, size_t len)
{
	long i;
	for (i = 0; i < MAX_CLIENTS; ++i)
		if (dev->clients[i].pid == me)
			return dequeue_msg(&dev->clients[i].queue, arg, len);
	cam_warn("%s: not found self", __func__, me);
	return -EAGAIN;
}

static long get_anco_status(struct cam_sync_device* dev, pid_t me, void* arg, size_t len)
{
	long rc = 0;
	uint32_t status = atomic_read(&g_anco_status);
	uint32_t online_lock_pid = atomic_read(&dev->online_lock.locked_pid);
	bool is_online_locked = (online_lock_pid != 0) && (online_lock_pid != me);
	status |= (is_online_locked << 16);

	cam_info("%s: status = 0x%x\n", __func__, status);
	if (copy_to_user(arg, &status, min_t(size_t, len, sizeof(status))))
		rc = -EFAULT;
	return rc;
}

static long cam_sync_ioctl(struct file *filp, unsigned int cmd,
		unsigned long arg)
{
	cam_info("enter %s cmd: 0x%x", __func__, cmd);
	struct miscdevice* mdev = NULL;
	struct cam_sync_device *idev = NULL;
	size_t len = _IOC_SIZE(cmd);
	long rc = 0;
	if (!filp)
		return -EFAULT;
	mdev = filp->private_data;
	if (!mdev)
		return -EFAULT;
	idev = container_of(mdev, struct cam_sync_device, dev);
	if (IS_ERR_OR_NULL(idev))
		return -EFAULT;

	switch (cmd) {
	case CAM_SYNC_ACQUIRE_ONLINE_LOCK:
		rc = pid_mutex_lock(&idev->online_lock);
		if (rc == 0)
			hisp_update_hisp_uid(true);
		break;
	case CAM_SYNC_RELEASE_ONLINE_LOCK:
		pid_mutex_unlock(current->tgid, &idev->online_lock);
		break;
	case CAM_SYNC_ACQUIRE_HISP_POWER_LOCK:
		rc = pid_mutex_lock(&idev->hisp_power_lock);
		break;
	case CAM_SYNC_RELEASE_HISP_POWER_LOCK:
		pid_mutex_unlock(current->tgid, &idev->hisp_power_lock);
		break;
	case CAM_SYNC_SEND_PEER_RPMSG:
		mutex_lock(&idev->lock);
		rc = send_msg_to_other(idev, current->tgid, arg, len);
		mutex_unlock(&idev->lock);
		break;
	case CAM_SYNC_RECV_PEER_RPMSG:
		// recv might wait, no need lock
		rc = recv_msg_from_other(idev, current->tgid, arg, len);
		break;
    case CAM_SYNC_ANCO_STATUS:
		rc = get_anco_status(idev, current->tgid, arg, len);
		break;
	default:
		cam_err("invalid IOCTL CMD(0x%x)", cmd);
		rc = -EINVAL;
		break;
	}

	return rc;
}

static int cam_sync_open(struct inode *inode, struct file *file) {
	cam_info("enter %s", __func__);
	struct miscdevice* mdev = NULL;
	struct cam_sync_device *idev = NULL;
	long i;
	int rc;

	if (!file)
		return -EFAULT;
	mdev = file->private_data;
	if (!mdev)
		return -EFAULT;
	idev = container_of(mdev, struct cam_sync_device, dev);
	if (IS_ERR_OR_NULL(idev))
		return -EFAULT;

	if (!try_module_get(THIS_MODULE)) {
		cam_warn("%s this module is removed", __func__);
		return -ENODEV;
	}
	rc = -EBUSY;
	mutex_lock(&idev->lock);
	for (i = 0; i < MAX_CLIENTS; ++i)
		if (idev->clients[i].pid == current->tgid) {
			cam_warn("%s: %d already opened", __func__, current->tgid);
			goto out;
		}

	for (i = 0; i < MAX_CLIENTS; ++i)
		if (idev->clients[i].pid == 0) {
			init_client_info(&idev->clients[i], current->tgid, file);
			rc = 0;
			cam_info("%s: %d succ opened", __func__, current->tgid);
			goto out;
		}

out:
	mutex_unlock(&idev->lock);
	if (rc)
		module_put(THIS_MODULE);
	return rc;
}

static int cam_sync_release(struct inode *inode, struct file *file) {
	cam_info("enter %s", __func__);
	mm_segment_t old_fs;
	struct cam_sync_device *idev = NULL;
	pid_t me = 0;
	long i;
	(void)inode;
	if (!file)
		return -EFAULT;
	idev = file->private_data;
	if (IS_ERR_OR_NULL(idev))
		return -EFAULT;

    old_fs = get_fs();
    set_fs(KERNEL_DS);
	mutex_lock(&idev->lock);
	for (i = 0; i < MAX_CLIENTS; ++i)
		if (idev->clients[i].filp == file) {
			me = idev->clients[i].pid;
			destroy_client_info(&idev->clients[i]);
			cam_info("%s: %d closed", __func__, me);
			break;
		}
	struct cam_sync_msg peer_died = {
		.type = PEER_DIED,
		.message_id = 0,
	};
	send_msg_to_other(idev, me, &peer_died, sizeof(peer_died));
	mutex_unlock(&idev->lock);
	set_fs(old_fs);

	pid_mutex_unlock(me, &idev->online_lock);
	pid_mutex_unlock(me, &idev->hisp_power_lock);
	module_put(THIS_MODULE);
	return 0;
}

static const struct file_operations cam_sync_fops = {
	.owner = THIS_MODULE,
	.open = cam_sync_open,
	.release = cam_sync_release,
	.unlocked_ioctl = cam_sync_ioctl,
#if CONFIG_COMPAT
	.compat_ioctl = cam_sync_ioctl,
#endif
};

static int32_t cam_sync_probe(struct platform_device *pdev)
{
	struct cam_sync_device *idev = NULL;
	int ret = 0;
	if (IS_ERR_OR_NULL(pdev)) {
		cam_err("%s: null pdev", __func__);
		return -ENODEV;
	}

	idev = kzalloc(sizeof(*idev), GFP_KERNEL);
	if (!idev)
		return -ENOMEM;

	mutex_init(&idev->lock);
	init_pid_mutex(&idev->online_lock, "online_lock");
	init_pid_mutex(&idev->hisp_power_lock, "hisp_power_lock");
	idev->dev.minor = MISC_DYNAMIC_MINOR;
	idev->dev.name = "cam_sync";
	idev->dev.fops = &cam_sync_fops;

	ret = misc_register(&idev->dev);
	if (ret != 0) {
		cam_err("%s: failed to register misc device, ret: %d", __func__, ret);
		kfree(idev);
		return ret;
	}
	platform_set_drvdata(pdev, idev);
	subscribe_anco_status(&anco_status_cb_subscriber);
	cam_info("%s: create %s success", __func__, idev->dev.name);

	return ret;
}

static int32_t cam_sync_remove(struct platform_device *pdev)
{
	struct cam_sync_device *idev = platform_get_drvdata(pdev);
	if (IS_ERR_OR_NULL(idev)) {
		cam_err("%s: cam_sync is not inited", __func__);
		return -EINVAL;
	}
	mutex_destroy(&idev->lock);
	misc_deregister(&idev->dev);
	kfree(idev);
	unsubscribe_anco_status(&anco_status_cb_subscriber);

	return 0;
}

static const struct of_device_id cam_sync_dt_match[] = {
	{ .compatible = "vendor,cam_sync", },
	{},
};
MODULE_DEVICE_TABLE(of, cam_sync_dt_match);

static struct platform_driver cam_sync_platform_driver = {
	.driver = {
		.name = "vendor,cam_sync",
		.owner = THIS_MODULE,
		.of_match_table = cam_sync_dt_match,
	},
	.probe = cam_sync_probe,
	.remove = cam_sync_remove,
};

static int __init cam_sync_init_module(void)
{
	cam_info("enter %s", __func__);
	return platform_driver_register(&cam_sync_platform_driver);
}

static void __exit cam_sync_exit_module(void)
{
	platform_driver_unregister(&cam_sync_platform_driver);
}

module_init(cam_sync_init_module);
module_exit(cam_sync_exit_module);
MODULE_DESCRIPTION("cam_sync");
MODULE_LICENSE("GPL v2");
