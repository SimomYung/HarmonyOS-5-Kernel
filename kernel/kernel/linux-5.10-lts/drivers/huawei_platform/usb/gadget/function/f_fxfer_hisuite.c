/*
 * f_fxfer_hisuite.c
 *
 * Source file of common function driver of fxfer hisuite
 *
 * Copyright (C) 2025 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/kfifo.h>
#include <linux/module.h>
#include <linux/poll.h>
#include <linux/eventfd.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <securec.h>
#include "configfs.h"
#include "f_fxfer.h"
#include "f_fxfer_hisuite.h"

#define MAX_EP_DEV 2
#define MAKE_CMD_WORD(t, c) ((((unsigned short)(t)) << 8) | ((unsigned short)(c)))

#define HUNGUP_EVENT_NOMEM     1 /* Bit0 */
#define HUNGUP_EVENT_NOFUNC    2 /* Bit1 */

struct fxfer_hisuite_memory {
	struct list_head list;
	uint64_t kmem_addr;
	uint64_t umem_start;
	uint32_t size;
	struct fxfer_io_req io_req;
	uint8_t index;
};

struct fxfer_hisuite_context {
	struct fxfer_client_info *client;

	spinlock_t mem_lock;
	spinlock_t fifo_lock;
	struct list_head mem_list;
	struct kfifo io_fifo_result;
	wait_queue_head_t io_wait_que;
	bool mem_valid;
	bool io_cancel;
	uint32_t hungup_event;
	uint32_t tx_max;
	uint32_t rx_max;
	struct delayed_work notify_work;
	struct fxfer_notify notify;

	struct class *dev_class;
	struct cdev *cdev_ctl;
	struct cdev *cdev_io;
	int io_open_cnt;
	dev_t devno_region;
};

static struct usb_string fxfer_hisuite_string_defs[] = {
	[0].s = "HISUITE",
	{},
};

static struct usb_gadget_strings fxfer_hisuite_string_table = {
	.language = 0x0409, /* 0x409 for en-us */
	.strings = fxfer_hisuite_string_defs,
};

static struct usb_gadget_strings *fxfer_hisuite_strings[] = {
	&fxfer_hisuite_string_table,
	NULL,
};

static struct fxfer_client_info *g_hisuite_client;
static bool g_hisuite_use_ctl;

static ssize_t fxfer_hisuite_io_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	int status = 0;
	unsigned int copied = 0;
	unsigned long flags = 0;
	struct fxfer_hisuite_context *ctx = NULL;

	if (!file || !file->private_data)
		return -EINVAL;

	ctx = (struct fxfer_hisuite_context *)file->private_data;
	spin_lock_irqsave(&ctx->fifo_lock, flags);
	if (kfifo_is_empty(&ctx->io_fifo_result)) {
		spin_unlock_irqrestore(&ctx->fifo_lock, flags);
	    return 0;
	}
	status = kfifo_to_user(&ctx->io_fifo_result, buf, count, &copied);
	spin_unlock_irqrestore(&ctx->fifo_lock, flags);
	client_info(ctx->client, "%s: status=%d, copied=%u\n", __func__, status, copied);
	status = (status == 0) ? copied : -EIO;

    return status;
}

static ssize_t fxfer_hisuite_io_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	return count;
}

static unsigned int fxfer_hisuite_io_poll(struct file *file, struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	struct fxfer_hisuite_context *ctx = NULL;

	if (!file || !file->private_data) {
		pr_err("%s: invalid arg\n", __func__);
		return -EINVAL;
	}

	ctx = (struct fxfer_hisuite_context *)file->private_data;
	client_info(ctx->client, "%s: enter, evt=%x\n", __func__, ctx->hungup_event);

	if (!kfifo_is_empty(&ctx->io_fifo_result)) {
		mask |= POLLIN;
		return mask;
	}

	poll_wait(file, &ctx->io_wait_que, wait);
	if (!kfifo_is_empty(&ctx->io_fifo_result))
		mask |= POLLIN;

	if (ctx->hungup_event) {
		if (ctx->hungup_event & HUNGUP_EVENT_NOMEM)
			mask |= POLLIN;
		if (ctx->hungup_event & HUNGUP_EVENT_NOFUNC)
			mask |= POLLPRI;
		ctx->hungup_event = 0;
	}

	client_info(ctx->client, "%s: mask=0x%x\n", __func__, mask);

	return mask;
}

static int fxfer_hisuite_io_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct fxfer_hisuite_context *ctx = NULL;
	struct fxfer_hisuite_memory *mem = NULL;
	unsigned long flags = 0;
	void *virt_mem = NULL;
	int ret;

	if (!file || !vma || !file->private_data)
		return -EINVAL;

	ctx = (struct fxfer_hisuite_context *)file->private_data;
	pr_info("%s: addr=0x%lx, size=%u\n", __func__, vma->vm_start, (vma->vm_end - vma->vm_start));

	virt_mem = kzalloc((vma->vm_end - vma->vm_start), GFP_KERNEL);
	if (!virt_mem) {
		pr_info("%s alloc memory failed\n", __func__);
		return -ENOMEM;
	}

	mem = kzalloc(sizeof(struct fxfer_hisuite_memory), GFP_KERNEL);
	if (!mem) {
		kfree(virt_mem);
		pr_info("%s alloc memory failed\n", __func__);
		return -ENOMEM;
	}

	ret = remap_pfn_range(vma, vma->vm_start, (virt_to_phys(virt_mem) >> PAGE_SHIFT),
			(vma->vm_end - vma->vm_start), vma->vm_page_prot);
	if (ret) {
		pr_info("%s remap_pfn_range failed with error code %d\n", __func__, ret);
		kfree(mem);
		kfree(virt_mem);
		return -ENOMEM;
	}

	mem->kmem_addr = (uint64_t)virt_mem;
	mem->umem_start = vma->vm_start;
	mem->size = (vma->vm_end - vma->vm_start);

	ctx->mem_valid = true;
	spin_lock_irqsave(&ctx->mem_lock, flags);
	list_add_tail(&mem->list, &ctx->mem_list);
	spin_unlock_irqrestore(&ctx->mem_lock, flags);

	return 0;
}

static void fxfer_hisuite_notify_work(struct work_struct *work)
{
	struct fxfer_hisuite_context *ctx = NULL;

	ctx = container_of(work, struct fxfer_hisuite_context, notify_work.work);
	pr_info("%s: notify type=%u, len=%u.", __func__, ctx->notify.type, ctx->notify.length);
	(void)fxfer_send_notify(ctx->client, (const char *)(&ctx->notify), sizeof(ctx->notify));
}

static void fxfer_hisuite_notify_state(struct fxfer_hisuite_context *ctx, uint32_t state)
{
	pr_info("%s: notify %u", __func__, state);
	(void)memset_s(&ctx->notify, sizeof(ctx->notify), 0, sizeof(ctx->notify));
	ctx->notify.type = NOTIFY_TYPE_STATE;
	ctx->notify.length = 1;
	ctx->notify.data[0] = state;
	mod_delayed_work(system_wq, &ctx->notify_work, msecs_to_jiffies(1));
}

static void fxfer_hisuite_clear_req(struct fxfer_hisuite_context *ctx)
{
	struct list_head *list_req = &ctx->mem_list;
	unsigned long flags = 0;
	struct fxfer_hisuite_memory *mem = NULL;

	pr_info("%s: enter, mem_valid=%d\n", __func__, ctx->mem_valid);

	spin_lock_irqsave(&ctx->mem_lock, flags);
	if (!ctx->mem_valid) {
		while (!list_empty(list_req)) {
			mem = list_first_entry(list_req, struct fxfer_hisuite_memory, list);
			list_del(&mem->list);
			pr_info("%s: mem=%u, data=%d\n", __func__, mem->index, NOT_NULL(mem->io_req.port_data));
			if (mem->io_req.port_data) {
				fxfer_bulk_io_freereq(ctx->client, &mem->io_req);
				mem->io_req.port_data = NULL;
			}
			kfree(mem->kmem_addr);
			kfree(mem);
		}
	} else {
		list_for_each_entry(mem, &ctx->mem_list, list) {
			if (mem && mem->io_req.port_data) {
				pr_info("%s: mem=%u\n", __func__, mem->index);
				fxfer_bulk_io_freereq(ctx->client, &mem->io_req);
				mem->io_req.port_data = NULL;
			}
		}
	}
	spin_unlock_irqrestore(&ctx->mem_lock, flags);
}

static void fxfer_hisuite_clear_resources(
	struct fxfer_hisuite_context *ctx, bool cancel_io, bool free_mem)
{
	unsigned long flags = 0;

	pr_info("%s: cancel_io=%d, free_mem=%d, mem_valid=%d\n", __func__, cancel_io, free_mem, ctx->mem_valid);

	if (free_mem) {
		ctx->mem_valid = false;
		ctx->tx_max = 0;
		ctx->rx_max = 0;
	}

	if (cancel_io) {
		ctx->io_cancel = true;
		fxfer_bulk_io_disable(ctx->client);
	}

	if (ctx->mem_valid) {
		if (ctx->client->client_online)
			wake_up_all(&ctx->io_wait_que);
	} else {
		spin_lock_irqsave(&ctx->fifo_lock, flags);
		kfifo_reset(&ctx->io_fifo_result);
		spin_unlock_irqrestore(&ctx->fifo_lock, flags);
	}
	fxfer_hisuite_clear_req(ctx);

	if (cancel_io) {
		ctx->io_cancel = false;
		fxfer_bulk_io_enable(ctx->client);
	}

	if (free_mem && waitqueue_active(&ctx->io_wait_que)) {
		ctx->hungup_event |= HUNGUP_EVENT_NOMEM;
		wake_up_all(&ctx->io_wait_que);
		pr_info("%s: to notify client offline\n", __func__);
	}
}

static int fxfer_hisuite_set_meminfo(struct fxfer_hisuite_context *ctx, void __user *info_addr)
{
	int ret = -ENOMEM;
	unsigned long flags = 0;
	struct fxfer_hisuite_memory *mem = NULL;
	struct ioctl_mem_info info = {0};

	if (!ctx || !info_addr) {
		pr_err("%s: null addr\n", __func__);
		return -EINVAL;
	}

	ret = copy_from_user(&info, info_addr, sizeof(struct ioctl_mem_info));
	if (ret) {
		pr_err("%s: copy from user failed\n", __func__);
		return -EFAULT;
	}

	pr_info("%s: addr=0x%lx, index=%u, is tx=%u\n",
		__func__, info.mem_addr, info.index, info.tx);

	if (!VALID_NUMBER(info.index)) {
		pr_err("%s: invalid index %u\n", __func__, info.index);
		return -EINVAL;
	}

	ret = -ENOMEM;
	spin_lock_irqsave(&ctx->mem_lock, flags);
	list_for_each_entry(mem, &ctx->mem_list, list) {
		if (mem && (mem->umem_start == info.mem_addr)) {
			if (mem->index) {
				pr_err("%s: already set index %u\n", __func__, mem->index);
				ret = -EBUSY;
				break;
			}
			mem->io_req.tx = (info.tx == DIRECT_TX);
			pr_info("%s: found mem %u, is_tx=%d\n", __func__, mem->index, mem->io_req.tx);
			mem->index = info.index;
			mem->io_req.client_data = (char *)ctx->client;
			ret = 0;
			if (mem->io_req.tx)
				ctx->tx_max = MAX_VAL(ctx->tx_max, mem->size);
			else
				ctx->rx_max = MAX_VAL(ctx->rx_max, mem->size);
			break;
		}
	}
	spin_unlock_irqrestore(&ctx->mem_lock, flags);

	return ret;
}

static struct fxfer_io_req *fxfer_hisuite_fill_ioreq(struct fxfer_hisuite_context *ctx, uint32_t io_flag)
{
	unsigned long flags = 0;
	struct fxfer_hisuite_memory *mem = NULL;
	struct fxfer_io_req *io_req = NULL;

	if (!VALID_NUMBER(GET_NUMBER(io_flag))) {
		pr_err("%s: invalid index %u\n", __func__, GET_NUMBER(io_flag));
		return NULL;
	}

	spin_lock_irqsave(&ctx->mem_lock, flags);
	list_for_each_entry(mem, &ctx->mem_list, list) {
		if (GET_NUMBER(io_flag) == mem->index) {
			client_info(ctx->client, "%s: found mem %u, is_tx=%d\n", __func__, mem->index, mem->io_req.tx);

			if ((GET_LENGTH(io_flag) > mem->size) || (GET_LENGTH(io_flag) == 0)) {
				pr_err("%s: mem length %u not enougth\n", __func__, mem->size);
				break;
			}

			if (mem->io_req.tx != (GET_DIRECT(io_flag) == DIRECT_TX)) {
				pr_err("%s: wrong direct %u\n", __func__, GET_DIRECT(io_flag));
				break;
			}

			mem->io_req.buffer = (unsigned char *)mem->kmem_addr;
			mem->io_req.len = GET_LENGTH(io_flag);
			mem->io_req.offset = 0;
			io_req = &mem->io_req;
			list_del(&mem->list);
			break;
		}
	}
	spin_unlock_irqrestore(&ctx->mem_lock, flags);

	return io_req;
}

static void fxfer_hisuite_return_ioreq(struct fxfer_hisuite_context *ctx, struct fxfer_io_req *io_req_ptr)
{
	unsigned long flags = 0;
	struct fxfer_hisuite_memory *mem = container_of(io_req_ptr, struct fxfer_hisuite_memory, io_req);

	spin_lock_irqsave(&ctx->mem_lock, flags);
	list_add_tail(&mem->list, &ctx->mem_list);
	spin_unlock_irqrestore(&ctx->mem_lock, flags);
}

static int fxfer_hisuite_send_bulk_io(struct fxfer_hisuite_context *ctx, uint32_t io_flag)
{
	int ret;
	struct fxfer_io_req *io_req = NULL;

	client_info(ctx->client, "%s: dump io_flag=%u,%u,%u,%u\n", __func__,
		GET_NUMBER(io_flag), GET_RESULT(io_flag), GET_DIRECT(io_flag), GET_LENGTH(io_flag));

	io_req = fxfer_hisuite_fill_ioreq(ctx, io_flag);
	if (!io_req) {
		pr_err("%s: no io req\n", __func__);
		return -ENOMEM;
	}

	if (!io_req->port_data && (ctx->client->flag & XFER_FLAG_PRE_REQ)) {
		ret = fxfer_bulk_io_allocreq(ctx->client, io_req);
		if (ret) {
			io_req->port_data = NULL;
			pr_err("%s: send io ret %d\n", __func__, ret);
			fxfer_hisuite_return_ioreq(ctx, io_req);
			return -EIO;
		}
	}

	ret = fxfer_bulk_io_request(ctx->client, io_req);
	if (ret) {
		pr_err("%s: send io ret %d\n", __func__, ret);
		fxfer_hisuite_return_ioreq(ctx, io_req);
		return -EIO;
	}

	return 0;
}

static int fxfer_hisuite_submit_ioreq(struct fxfer_hisuite_context *ctx, void __user *info_addr)
{
	int ret = -ENOMEM;
	uint32_t io_flag = 0;

	if (!info_addr || !ctx->client->port_online) {
		pr_err("%s: addr=%d, or no link\n", __func__, NOT_NULL(info_addr));
		return (info_addr) ? -EINVAL : -ENOLINK;
	}

	ret = copy_from_user(&io_flag, info_addr, sizeof(uint32_t));
	if (ret) {
		pr_err("%s: copy from user failed\n", __func__);
		return -EFAULT;
	}

	ret = fxfer_hisuite_send_bulk_io(ctx, io_flag);
	if (ret)
		return -EIO;

	return 0;
}

static int fxfer_hisuite_submit_multi_ioreq(struct fxfer_hisuite_context *ctx, void __user *info_addr)
{
	uint32_t io_flags[FXFER_IO_MAX_REQ];
	int ret;
	int index;

	if (!info_addr || !ctx->client->port_online) {
		pr_err("%s: addr=%d, or no link\n", __func__, NOT_NULL(info_addr));
		return (info_addr) ? -EINVAL : -ENOLINK;
	}

	ret = copy_from_user(io_flags, info_addr, sizeof(io_flags));
	if (ret) {
		pr_err("%s: copy from user failed\n", __func__);
		return -EFAULT;
	}

	for (index = 0; index < FXFER_IO_MAX_REQ; index++) {
		if (GET_LENGTH(io_flags[index]) == 0) {
			SET_RESULT(io_flags[index], FXFER_E_INVAL);
			continue;
		}
		ret = fxfer_hisuite_send_bulk_io(ctx, io_flags[index]);
		if (ret)
			SET_RESULT(io_flags[index], FXFER_E_IO);
		else
			SET_RESULT(io_flags[index], FXFER_E_OK);
	}

	ret = copy_to_user(info_addr, io_flags, sizeof(io_flags));
	if (ret) {
		pr_err("%s: copy from user failed\n", __func__);
		return -EFAULT;
	}

	return 0;
}

static int fxfer_hisuite_send_notify(struct fxfer_hisuite_context *ctx, void __user *info_addr)
{
	int ret = -ENOMEM;
	unsigned long flags = 0;
	struct ioctl_hisuite_notify notify = {0};
	errno_t rc;

	if (!ctx || !info_addr) {
		pr_err("%s: null addr\n", __func__);
		return -EINVAL;
	}

	ret = copy_from_user(&notify, info_addr, sizeof(notify));
	if (ret) {
		pr_err("%s: copy from user failed\n", __func__);
		return -EFAULT;
	}

	pr_info("%s: type=%u, length=%u\n", __func__, notify.type, notify.length);
	(void)memset_s(&ctx->notify, sizeof(ctx->notify), 0, sizeof(ctx->notify));
	ctx->notify.type = notify.type;
	ctx->notify.length = notify.length;
	rc = memcpy_s(ctx->notify.data, sizeof(ctx->notify.data), notify.data, sizeof(notify.data));
	if (rc != EOK) {
		pr_err("%s: copy notify data failed, ret=%d\n", __func__, (int)rc);
		return -EFAULT;
	}
	ret = fxfer_send_notify(ctx->client, (const char *)(&ctx->notify), sizeof(ctx->notify));

	return ret;
}

static long fxfer_hisuite_io_ioctl(struct file *file, unsigned code, unsigned long value)
{
	struct fxfer_hisuite_context *ctx = NULL;
	long ret = 0;

	if (!file || !file->private_data)
		return -EINVAL;

	ctx = (struct fxfer_hisuite_context *)file->private_data;
	client_info(ctx->client, "%s: param=%u-%u-%lu\n", __func__, code, _IOC_NR(code), value);

	switch (code) {
		case IOCTL_HISUITE_SET_MEM_INFO :
			ret = (long)fxfer_hisuite_set_meminfo(ctx, (void __user *)value);
			break;
		case IOCTL_HISUITE_SUBMIT_IOREQ :
			ret = (long)fxfer_hisuite_submit_ioreq(ctx, (void __user *)value);
			break;
		case IOCTL_HISUITE_SUBMIT_MULTI_IOREQ :
			ret = (long)fxfer_hisuite_submit_multi_ioreq(ctx, (void __user *)value);
			break;
		case IOCTL_HISUITE_FREE_MEM :
			pr_info("%s: free mem\n", __func__);
			fxfer_hisuite_clear_resources(ctx, true, true);
			fxfer_hisuite_notify_state(ctx, NOTIFY_STATE_RESET);
			break;
		case IOCTL_HISUITE_IO_CANCEL :
			pr_info("%s: io cancel\n", __func__);
			fxfer_hisuite_clear_resources(ctx, true, false);
			fxfer_hisuite_notify_state(ctx, NOTIFY_STATE_RESET);
			break;
		case IOCTL_HISUITE_SEND_NOTIFY :
			ret = (long)fxfer_hisuite_send_notify(ctx, (void __user *)value);
			break;
		default :
			ret = -ENOTSUPP;
			break;
	}

	return ret;
}

static int fxfer_hisuite_io_open(struct inode *inode, struct file *file)
{
	struct fxfer_hisuite_context *ctx = NULL;

	if (!g_hisuite_client || !g_hisuite_client->private || !file) {
		pr_err("%s: invalid param\n", __func__);
		return -ENODEV;
	}

	file->private_data = g_hisuite_client->private;
	ctx = (struct fxfer_hisuite_context *)file->private_data;
	ctx->io_open_cnt++;
	ctx->client->client_online = 1;
	pr_info("%s: enter, cnt=%d\n", __func__, ctx->io_open_cnt);
	fxfer_hisuite_notify_state(ctx, NOTIFY_STATE_CONNECT);

	return 0;
}

static int fxfer_hisuite_io_release(struct inode *inode, struct file *file)
{
	struct fxfer_hisuite_context *ctx = NULL;

	if (!file || !file->private_data) {
		pr_err("%s: invalid param\n", __func__);
		return -EINVAL;
	}

	ctx = (struct fxfer_hisuite_context *)file->private_data;
	ctx->io_open_cnt--;
	pr_info("%s: enter, cnt=%d\n", __func__, ctx->io_open_cnt);

	if (ctx->io_open_cnt <= 0) {
		ctx->client->client_online = 0;
		fxfer_hisuite_clear_resources(ctx, true, true);
		fxfer_hisuite_notify_state(ctx, NOTIFY_STATE_DISCONNECT);
	}

	return 0;
}

static const struct file_operations hisuite_io_operations = {
    .owner   = THIS_MODULE,
    .llseek =    no_llseek,    
    .mmap = fxfer_hisuite_io_mmap,
    .read = fxfer_hisuite_io_read,
    .write = fxfer_hisuite_io_write,
    .poll = fxfer_hisuite_io_poll,
    .open = fxfer_hisuite_io_open,
    .release = fxfer_hisuite_io_release,
    .unlocked_ioctl = fxfer_hisuite_io_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = fxfer_hisuite_io_ioctl,
#endif
};

static int fxfer_hisuite_create_io_dev(struct fxfer_hisuite_context *ctx)
{
	int ret;
	struct device *io_device = NULL;

	pr_info("%s: enter\n", __func__);
	if (ctx->cdev_io) {
		pr_info("%s: already\n", __func__);
		return 0;
	}

	ctx->cdev_io = cdev_alloc();
	if (!ctx->cdev_io) {
		pr_info("%s: alloc control dev failed\n", __func__);
		return -ENODEV;
	}

	ctx->cdev_io->ops = &hisuite_io_operations;
	if (g_hisuite_use_ctl)
		ret = cdev_add(ctx->cdev_io, MKDEV(MAJOR(ctx->devno_region), 1), 1);
	else
		ret = cdev_add(ctx->cdev_io, MKDEV(MAJOR(ctx->devno_region), 0), 1);
	if (ret) {
		cdev_del(ctx->cdev_io);
		ctx->cdev_io = NULL;
		pr_info("%s: add control dev ret %d\n", __func__, ret);
		return -ENODEV;
	}

	io_device = device_create(ctx->dev_class, NULL, ctx->cdev_io->dev, NULL, "fxfer.hisuite.io");
	if (IS_ERR(io_device)) {
		cdev_del(ctx->cdev_io);
		ctx->cdev_io = NULL;
		pr_info("%s: device_create ret %d\n", __func__, PTR_ERR(io_device));
		return -ENODEV;
	}

	return 0;
}

static int fxfer_hisuite_ctl_open(struct inode *inode, struct file *file)
{
	struct cdev *cdev = inode->i_cdev;

	pr_info("%s: enter, dev=0x%x\n", __func__, cdev->dev);

	if (!g_hisuite_client || !g_hisuite_client->private)
		return -ENODEV;

	file->private_data = g_hisuite_client->private;

	return 0;
}

static int fxfer_hisuite_ctl_release(struct inode *inode, struct file *file)
{
	struct cdev *cdev = inode->i_cdev;

	pr_info("%s: enter 0x%x\n", __func__, cdev->dev);

	return 0;
}

static ssize_t fxfer_hisuite_ctl_write(struct file *file,
	const char __user *buf, size_t len, loff_t *ptr)
{
	return len;
}

static const struct file_operations hisuite_ctl_operations = {
    .owner = THIS_MODULE,
    .llseek = no_llseek,
    .open = fxfer_hisuite_ctl_open,
    .release = fxfer_hisuite_ctl_release,
    .write = fxfer_hisuite_ctl_write,
};

static char *hisuite_devnode(struct device *dev, umode_t *mode)
{
	return kasprintf(GFP_KERNEL, "fxfer_hisuite/%s", dev_name(dev));
}

static struct class fxfer_hisuite_class = {
	.name = "fxfer_hisuite",
	.devnode = hisuite_devnode,
};

static int fxfer_hisuite_dev_init(struct fxfer_client_info *client, struct fxfer_hisuite_context *ctx)
{
	int ret;
	struct device *ctl_device = NULL;

	ret = class_register(&fxfer_hisuite_class);
	if (ret) {
		pr_info("%s: class_register ret %d\n", __func__, ret);
		return -ENODEV;
	}
	ctx->dev_class = &fxfer_hisuite_class;

	ret = alloc_chrdev_region(&ctx->devno_region, 0, MAX_EP_DEV, "fxfer.hisuite.dev");
	if (ret < 0) {
		pr_info("%s: alloc_chrdev_region ret %d\n", __func__, ret);
		goto failed;
	}

	if (!g_hisuite_use_ctl)
		goto create_io;

	ctx->cdev_ctl = cdev_alloc();
	if (!ctx->cdev_ctl) {
		pr_info("%s: alloc control dev ret %d\n", __func__, ret);
		goto failed;
	}

	ctx->cdev_ctl->ops = &hisuite_ctl_operations;
	ret = cdev_add(ctx->cdev_ctl, MKDEV(MAJOR(ctx->devno_region), 0), 1);
	if (ret) {
		pr_info("%s: add control dev ret %d\n", __func__, ret);
		goto failed;
	}

	ctl_device = device_create(ctx->dev_class, NULL, ctx->cdev_ctl->dev, NULL, "fxfer.hisuite.ctl");
	if (IS_ERR(ctl_device)) {
		pr_info("%s: device_create ret %d\n", __func__, PTR_ERR(ctl_device));
		goto failed;
	}

create_io:
	ret = fxfer_hisuite_create_io_dev(ctx);
	if (ret)
		goto failed;

	return 0;

failed:
	if (ctx->cdev_ctl) {
		cdev_del(ctx->cdev_ctl);
		ctx->cdev_ctl = NULL;
	}

	if (ctx->devno_region) {
		unregister_chrdev_region(&ctx->devno_region, MAX_EP_DEV);
		ctx->devno_region = 0;
	}

	class_unregister(&fxfer_hisuite_class);
	ctx->dev_class = NULL;

	return -ENODEV;
}

static int fxfer_hisuite_init(struct fxfer_client_info *client)
{
	struct fxfer_hisuite_context *ctx = NULL;
	unsigned int i;
	int ret;

	if (client->flag & XFER_FLAG_ENABLED) {
		pr_err("%s already inited\n", __func__);
		return 0;
	}

	pr_info("%s enter\n", __func__);
	ctx = kzalloc(sizeof(struct fxfer_hisuite_context), GFP_KERNEL);
	if (!ctx) {
		pr_err("%s: alloc ctx failed\n", __func__);
		return -ENOMEM;
	}

	INIT_DELAYED_WORK(&ctx->notify_work, fxfer_hisuite_notify_work);
	spin_lock_init(&ctx->mem_lock);
	spin_lock_init(&ctx->fifo_lock);
	INIT_LIST_HEAD(&ctx->mem_list);
	init_waitqueue_head(&ctx->io_wait_que);
	ret = kfifo_alloc(&ctx->io_fifo_result, 50, GFP_KERNEL); /* Max to 50 requests */
	if (ret) {
		pr_err("%s: alloc fifo failed\n", __func__);
		kfree(ctx);
		return -ENOMEM;
	}

	ret = fxfer_hisuite_dev_init(client, ctx);
	if (ret) {
		pr_err("%s: allo dev failed\n", __func__);
		kfree(ctx);
		kfifo_free(&ctx->io_fifo_result);
		return -ENOMEM;
	}

	ctx->client = client;
	client->private = ctx;
	client->flag |= XFER_FLAG_ENABLED;

	return 0;
}

static int fxfer_hisuite_get_state(struct fxfer_hisuite_context *ctx, unsigned char *buf, unsigned int len)
{
	errno_t ret;
	struct fxfer_state state = { HISUITE_VERSION };

	if (!buf || !len)
		return -EINVAL;

	state.client_connect = (uint8_t)ctx->client->client_online;
	if (ctx->client->client_online) {
		/* Swap rx/tx direct */
		state.tx_max = ctx->rx_max;
		state.rx_max = ctx->tx_max;
	}

	ret = memcpy_s(buf, len, &state, MIN_VAL(len, sizeof(struct fxfer_state)));
	if (ret != EOK) {
		pr_err("%s: copy state ret %d\n", __func__, ret);
		return -ENOMEM;
	}

	return (int)MIN_VAL(len, sizeof(struct fxfer_state));
}

static int fxfer_hisuite_setup(struct fxfer_client_info *client,
	struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
	int value = -EOPNOTSUPP;
	struct fxfer_hisuite_context *ctx = NULL;

	if (!client || !client->private || !f || !ctrl) {
		pr_err("%s: invalid param\n", __func__);
		return -EINVAL;
	}

	ctx = (struct fxfer_hisuite_context *)client->private;

	switch (MAKE_CMD_WORD(ctrl->bRequestType, ctrl->bRequest)) {
		case MAKE_CMD_WORD((USB_DIR_IN | USB_TYPE_VENDOR | USB_DT_INTERFACE), CMD_GET_STATE) :
			value = fxfer_hisuite_get_state(ctx, f->config->cdev->req->buf, le16_to_cpu(ctrl->wLength));
			break;
		default :
			break;
	}

	if (value >= 0) {
		pr_info("%s:ret=%d, req=0x%x, 0x%x, 0x%x, 0x%x, %u\n", __func__, value,
			ctrl->bRequestType, ctrl->bRequest, le16_to_cpu(ctrl->wIndex),
			le16_to_cpu(ctrl->wValue), le16_to_cpu(ctrl->wLength));
	}

	return value;
}

/* Set desc for interface while fxfer_bind */
static void fxfer_hisuite_init_function(
	struct fxfer_client_info *client, struct usb_function *func)
{
	pr_info("%s enter\n", __func__);
	func->strings = fxfer_hisuite_strings;
}

static void fxfer_hisuite_clear(struct fxfer_client_info *client)
{
	struct fxfer_hisuite_context *ctx = NULL;

	if (!client || !client->private)
		return;

	pr_info("%s enter\n", __func__);
	ctx = (struct fxfer_hisuite_context *)client->private;
	fxfer_hisuite_clear_resources(ctx, false, false);
}

static void fxfer_hisuite_destroy(struct fxfer_client_info *client)
{
	struct fxfer_hisuite_context *ctx = NULL;

	if (!client || !client->private)
		return;

	pr_info("%s enter\n", __func__);
	ctx = (struct fxfer_hisuite_context *)client->private;

	fxfer_hisuite_clear(client);
	g_hisuite_client = NULL;

	if (waitqueue_active(&ctx->io_wait_que)) {
		pr_info("%s: to notify client destroyed\n", __func__);
		ctx->hungup_event |= HUNGUP_EVENT_NOFUNC;
		wake_up_all(&ctx->io_wait_que);
	}
}

static void fxfer_hisuite_connect(struct fxfer_client_info *client)
{
	struct fxfer_hisuite_context *ctx = NULL;

	if (!client || !client->private)
		return;

	pr_info("%s enter\n", __func__);
	ctx = (struct fxfer_hisuite_context *)client->private;
}

static void fxfer_hisuite_io_finish(
	struct fxfer_client_info *client, struct fxfer_io_req *req, int result)
{
	struct fxfer_hisuite_context *ctx = NULL;
	struct fxfer_hisuite_memory *mem = NULL;
	uint32_t io_flag = 0;
	unsigned long flags = 0;
	int ret;

	if (!client || !client->private || !req)
		return;

	ctx = (struct fxfer_hisuite_context *)client->private;
	mem = container_of(req, struct fxfer_hisuite_memory, io_req);
	client_info(client, "%s enter, ret=%d, index=%u, is_tx=%d\n", __func__, result, mem->index, req->tx);

	SET_NUMBER(io_flag, mem->index);
	if (ctx->io_cancel) {
		SET_RESULT(io_flag, FXFER_E_CANCEL);
	} else if (result < 0) {
		SET_RESULT(io_flag, FXFER_E_IO);
	} else {
		SET_RESULT(io_flag, FXFER_E_OK);
		SET_LENGTH(io_flag, (uint32_t)result);
	}
	if (req->tx)
		SET_DIRECT(io_flag, DIRECT_TX);

	if (!ctx->mem_valid) {
		pr_info("%s: free invalid mem %u\n", __func__, mem->index);
		fxfer_bulk_io_freereq(ctx->client, &mem->io_req);
		kfree(mem->kmem_addr);
		kfree(mem);
		return;
	}

	fxfer_hisuite_return_ioreq(ctx, req);
	if ((!ctx->client->client_online) || (!ctx->client->port_online) || ctx->io_cancel)
		fxfer_hisuite_clear_req(ctx);

	spin_lock_irqsave(&ctx->fifo_lock, flags);
	ret = kfifo_in(&ctx->io_fifo_result, &io_flag, sizeof(uint32_t));
	spin_unlock_irqrestore(&ctx->fifo_lock, flags);
	if (ret != sizeof(uint32_t)) {
		pr_err("%s: kfifo_in failed ret %d\n", __func__, ret);
	} else {
		wake_up_all(&ctx->io_wait_que);
	}
}

static struct fxfer_client_info g_fxfer_hisuite = {
	.name = "hisuite",
	.flag = XFER_FLAG_IO_BULK | XFER_DEP_IN | XFER_DEP_OUT | XFER_DEP_INTR | XFER_FLAG_OS_DESC,
	.buf_rx_sz = 128, /* 128 bytes read buffer  */
	.buf_tx_sz = 128, /* 128 bytes write buffer  */
	.fxfer_init_function = fxfer_hisuite_init_function,
	.fxfer_ctrl_req = fxfer_hisuite_setup,
	.fxfer_clear = fxfer_hisuite_clear,
	.fxfer_destroy = fxfer_hisuite_destroy,
	.fxfer_connect = fxfer_hisuite_connect,
	.fxfer_io_finish = fxfer_hisuite_io_finish,
};

struct fxfer_client_info *fxfer_hisuite_get_info(void)
{
	int ret;
	struct fxfer_hisuite_context *ctx = NULL;

	ret = fxfer_hisuite_init(&g_fxfer_hisuite);
	if (ret) {
		pr_err("%s: init failed %d\n", __func__, ret);
		return NULL;
	}

	g_hisuite_client = &g_fxfer_hisuite;
	if (g_fxfer_hisuite.private) {
		ctx = (struct fxfer_hisuite_context *)g_fxfer_hisuite.private;
		ctx->hungup_event = 0;
	}

	return &g_fxfer_hisuite;
}

