/*
 * f_fxfer_aoa.c
 *
 * Source file of function driver of aoa
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kref.h>
#include <linux/types.h>
#include <linux/file.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/hid.h>
#include <linux/hiddev.h>
#include <securec.h>

#include <chipset_common/hwusb/hw_usb_fxfer.h>
#include "f_fxfer.h"

#define AOA_STRING_SIZE           256
#define AOA_STRING_MANUFACTURER   0
#define AOA_STRING_MODEL          1
#define AOA_STRING_DESCRIPTION    2
#define AOA_STRING_VERSION        3
#define AOA_STRING_URI            4
#define AOA_STRING_SERIAL         5
#define AOA_STRING_EXTRA          6
#define AOA_STRING_END            7

#define AOA_STRING_EXTRA_DATA     0xA0
#define AOA_IOCTL_EXTRA_DATA      0xC0
#define AOA_IOCTL_IO_CANCEL       0xC1

#define AOA_GET_PROTOCOL          51
#define AOA_SEND_STRING           52
#define AOA_START                 53
#define AOA_REGISTER_HID          54
#define AOA_UNREGISTER_HID        55
#define AOA_SET_HID_REPORT_DESC   56
#define AOA_SEND_HID_EVENT        57
#define AOA_SET_AUDIO_MODE        58

#define AOA_GET_STRING_MANUFACTURER   _IOW('M', 1, char[AOA_STRING_SIZE])
#define AOA_GET_STRING_MODEL          _IOW('M', 2, char[AOA_STRING_SIZE])
#define AOA_GET_STRING_DESCRIPTION    _IOW('M', 3, char[AOA_STRING_SIZE])
#define AOA_GET_STRING_VERSION        _IOW('M', 4, char[AOA_STRING_SIZE])
#define AOA_GET_STRING_URI            _IOW('M', 5, char[AOA_STRING_SIZE])
#define AOA_GET_STRING_SERIAL         _IOW('M', 6, char[AOA_STRING_SIZE])
#define AOA_IS_START_REQUESTED        _IO('M', 7)
#define AOA_GET_AUDIO_MODE            _IO('M', 8)
#define AOA_GET_EXTRA_DATA            _IOW('M', AOA_IOCTL_EXTRA_DATA, char[AOA_STRING_SIZE])
#define AOA_IO_CANCEL                 _IO('M', AOA_IOCTL_IO_CANCEL)

#define HID_STATE_NEW        1    /* bit 0 */
#define HID_STATE_START      2    /* bit 1 */
#define HID_STATE_STOP       4    /* bit 2 */
#define HID_ID_ANY           0xFFFFFFFF

struct aoa_context {
	spinlock_t lock;
	struct delayed_work start_work;
	struct work_struct getprotocol_work;
	struct work_struct sendstring_work;
	struct work_struct send_work;   /* Hicar send string */
	struct work_struct hid_work;    /* Work to fresh aoa_hid_dev */
	struct list_head hid_list;      /* List of aoa_hid_dev */

	struct device *parent;

	int string_index;
	int start_requested;
	int audio_mode;
	char aoa_string[AOA_STRING_END][AOA_STRING_SIZE];
};

struct aoa_hid_dev {
	struct list_head list;
	struct hid_device *dev;
	struct aoa_context *ctx;

	unsigned int state;   /* HID_STATE_XXX */
	unsigned int id;
	unsigned int desc_len;
	unsigned int desc_offset;
	unsigned char *desc;
};

static struct usb_string fxfer_string_defs[] = {
	[0].s = "FXFER_AOA",
	{},
};

static struct usb_gadget_strings fxfer_string_table = {
	.language = 0x0409, /* 0x409 for en-us */
	.strings = fxfer_string_defs,
};

static struct usb_gadget_strings *fxfer_strings[] = {
	&fxfer_string_table,
	NULL,
};

/* Set desc for interface while fxfer_bind */
static void fxfer_aoa_init_function(
	struct fxfer_client_info *client, struct usb_function *func)
{
	pr_info("%s enter\n", __func__);
	func->strings = fxfer_strings;
}

/* Get pointer to aoa_hid_dev in hid_list of aoa_context */
static struct aoa_hid_dev *fxfer_aoa_get_hid(
	struct aoa_context *ctx, unsigned int id, unsigned int state)
{
	static struct aoa_hid_dev *dev = NULL;

	list_for_each_entry(dev, &(ctx->hid_list), list) {
		/* Check hid id */
		if ((id != HID_ID_ANY) && (dev->id != id))
			continue;
		/* Check hid state */
		if (!state || (dev->state & state))
			return dev;
	}

	return NULL;
}

/* Alloc buffer for new aoa_hid_dev */
static struct aoa_hid_dev *fxfer_aoa_create_hid(
	struct aoa_context *ctx, unsigned int id, unsigned int len)
{
	static struct aoa_hid_dev *dev = NULL;

	dev = kzalloc(sizeof(struct aoa_hid_dev), GFP_ATOMIC);
	if (!dev)
		return NULL;

	dev->desc = kzalloc(len, GFP_ATOMIC);
	if (!dev->desc) {
		kfree(dev);
		return NULL;
	}

	dev->ctx = ctx;
	dev->id = id;
	dev->desc_len = len;
	dev->state = HID_STATE_NEW;

	return dev;
}

/* Free buffer of aoa_hid_dev */
static void fxfer_aoa_destroy_hid(struct aoa_hid_dev *dev)
{
	pr_info("%s: delete dev %p, id=%u\n", __func__, dev, dev->id);
	if (dev->dev) {
		hid_destroy_device(dev->dev);
		dev->dev = NULL;
	}

	if (dev->desc)
		kfree(dev->desc);
	kfree(dev);
}

/* Porcess control request of AOA_REGISTER_HID */
static int fxfer_aoa_reg_hid(struct aoa_context *ctx,
	unsigned int id, unsigned int desc_length)
{
	struct aoa_hid_dev *dev = NULL;
	unsigned long flags;

	pr_info("%s: id=%u, length=%u\n", __func__, id, desc_length);
	if (desc_length == 0) {
		pr_err("%s: invalid length\n", __func__);
		return -EINVAL;
	}

	/* Check previous hid-device with same id and stop it first. */
	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, id, (HID_STATE_START | HID_STATE_NEW));
	if (dev) {
		pr_info("%s: stop old device %p\n", __func__, dev);
		dev->state = HID_STATE_STOP;
	}
	spin_unlock_irqrestore(&ctx->lock, flags);

	/* Create new hid-device. */
	dev = fxfer_aoa_create_hid(ctx, id, desc_length);
	if (!dev) {
		pr_err("%s: create hid failed\n", __func__);
		return -ENOMEM;
	}

	spin_lock_irqsave(&ctx->lock, flags);
	list_add(&dev->list, &ctx->hid_list);
	spin_unlock_irqrestore(&ctx->lock, flags);

	pr_info("%s: create new device %p\n", __func__, dev);
	schedule_work(&ctx->hid_work);

	return 0;
}

/* Porcess control request of AOA_UNREGISTER_HID */
static int fxfer_aoa_unreg_hid(struct aoa_context *ctx, unsigned int id)
{
	struct aoa_hid_dev *dev = NULL;
	unsigned long flags;

	/* Stop hid-device with id in cmd. */
	pr_err("%s: id=%u\n", __func__, id);
	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, id, 0);
	if (dev) {
		pr_info("%s: stop old device %p\n", __func__, dev);
		dev->state = HID_STATE_STOP;
	}
	spin_unlock_irqrestore(&ctx->lock, flags);

	schedule_work(&ctx->hid_work);
	return 0;
}

static int fxfer_aoa_hid_parse(struct hid_device *hid)
{
	struct aoa_hid_dev *hdev = hid->driver_data;

	pr_info("%s: enter, dev=%p\n", __func__, hdev);
	if (hdev)
		hid_parse_report(hid, hdev->desc, hdev->desc_len);
	return 0;
}

static int fxfer_aoa_hid_start(struct hid_device *hid)
{
	pr_info("%s: enter\n", __func__);
	return 0;
}

static void fxfer_aoa_hid_stop(struct hid_device *hid)
{
	pr_info("%s: enter\n", __func__);
}

static int fxfer_aoa_hid_open(struct hid_device *hid)
{
	pr_info("%s: enter\n", __func__);
	return 0;
}

static void fxfer_aoa_hid_close(struct hid_device *hid)
{
	pr_info("%s: enter\n", __func__);
}

static int fxfer_aoa_hid_raw_request(
	struct hid_device *hid, unsigned char reportnum,
	__u8 *buf, size_t len, unsigned char rtype, int reqtype)
{
	pr_info("%s: enter\n", __func__);
	return 0;
}

static struct hid_ll_driver fxfer_aoa_hid_ll_driver = {
	.parse = fxfer_aoa_hid_parse,
	.start = fxfer_aoa_hid_start,
	.stop = fxfer_aoa_hid_stop,
	.open = fxfer_aoa_hid_open,
	.close = fxfer_aoa_hid_close,
	.raw_request = fxfer_aoa_hid_raw_request,
};

/* Register hid device in system. */
static int fxfer_aoa_init_hid_device(struct aoa_hid_dev *hdev)
{
	struct hid_device *hid = NULL;
	int ret;

	hid = hid_allocate_device();
	if (IS_ERR(hid)) {
		pr_err("%s: alloc hid failed\n", __func__);
		return -EIO;
	}

	hid->ll_driver = &fxfer_aoa_hid_ll_driver;
	hid->dev.parent = hdev->ctx->parent;

	hid->bus = BUS_USB;
	hid->vendor = HID_ANY_ID;
	hid->product = HID_ANY_ID;
	hid->driver_data = hdev;
	ret = hid_add_device(hid);
	if (ret) {
		pr_err("%s: add hid device failed %d\n", __func__, ret);
		hid_destroy_device(hid);
		return ret;
	}

	hdev->dev = hid;
	return 0;
}

/* Callback function for hid_work, fresh hid-device: reg/unreg hid in system. */
static void fxfer_aoa_hid_refresh(struct aoa_context *ctx)
{
	struct aoa_hid_dev *dev = NULL;
	unsigned long flags;
	bool found = false;

	pr_err("%s: work enter\n", __func__);
	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, HID_ID_ANY, HID_STATE_NEW);
	/* New hid-dev should has full desc. Otherwise we need to wait. */
	if (dev && (dev->desc_offset == dev->desc_len)) {
		found = true;
		spin_unlock_irqrestore(&ctx->lock, flags);
		pr_info("%s: try to start device %p\n", __func__, dev);

		if (fxfer_aoa_init_hid_device(dev)) {
			pr_err("can't add hid device %p\n", dev);
			dev->state = HID_STATE_STOP;
		} else {
			dev->state = HID_STATE_START;
		}
		spin_lock_irqsave(&ctx->lock, flags);
	}
	spin_unlock_irqrestore(&ctx->lock, flags);

	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, HID_ID_ANY, HID_STATE_STOP);
	if (dev) {
		found = true;
		pr_info("%s: found stopped device %p\n", __func__, dev);
		list_del(&dev->list);
	}
	spin_unlock_irqrestore(&ctx->lock, flags);

	if (dev && (dev->state == HID_STATE_STOP)) {
		pr_info("%s: delete stopped device %p\n", __func__, dev);
		fxfer_aoa_destroy_hid(dev);
	}

	/* schedual work agian to process other hid-devices. */
	if (found)
		schedule_work(&ctx->hid_work);
}

/* Remove all hid device reged in system. */
static void fxfer_aoa_remove_all_hid(struct aoa_context *ctx)
{
	static struct aoa_hid_dev *dev = NULL;
	unsigned long flags;
	bool found = false;

	pr_info("%s: enter\n", __func__);

	spin_lock_irqsave(&ctx->lock, flags);
	list_for_each_entry(dev, &(ctx->hid_list), list) {
		found = true;
		dev->state = HID_STATE_STOP;
		pr_info("%s: remove hid %p\n", __func__, dev);
	}
	spin_unlock_irqrestore(&ctx->lock, flags);

	if (found)
		schedule_work(&ctx->hid_work);
}

static void fxfer_aoa_clear(struct fxfer_client_info *client)
{
	if (client && client->private)
		fxfer_aoa_remove_all_hid((struct aoa_context *)client->private);
}

static void fxfer_aoa_complete_stub(struct usb_ep *ep, struct usb_request *req)
{
}

static char *fxfer_aoa_get_string_dst(struct aoa_context *ctx, unsigned int idx)
{
	/* Set idx for hicar(EXTRA_DATA) command */
	if ((idx == AOA_STRING_EXTRA_DATA) || (idx == (AOA_IOCTL_EXTRA_DATA - 1)))
		idx = AOA_STRING_EXTRA;

	if (idx >= AOA_STRING_END) {
		pr_err("%s: invalid idx %u\n", __func__, idx);
		return NULL;
	}

	pr_info("%s: get idx %u\n", __func__, idx);
	return ctx->aoa_string[idx];
}

/* Callback function to process control command AOA_SEND_STRING */
static void fxfer_aoa_complete_set_string(struct usb_ep *ep, struct usb_request *req)
{
	struct aoa_context *ctx = ep->driver_data;
	char *string_dest = NULL;
	int length = req->actual;
	unsigned long flags;

	if (req->status != 0) {
		pr_err("acc_complete_set_string, err %d\n", req->status);
		return;
	}

	string_dest = fxfer_aoa_get_string_dst(ctx, ctx->string_index);
	if (string_dest) {
		if (length >= AOA_STRING_SIZE)
			length = AOA_STRING_SIZE - 1;
		spin_lock_irqsave(&ctx->lock, flags);
		memset_s(string_dest, AOA_STRING_SIZE, 0, AOA_STRING_SIZE);
		if (memcpy_s(string_dest, AOA_STRING_SIZE, req->buf, length)) {
			length = 0;
			string_dest[0] = 0;
		}
		spin_unlock_irqrestore(&ctx->lock, flags);
		pr_info("%s copy %d bytes to string %d\n", __func__, length, ctx->string_index);

		if (ctx->string_index == AOA_STRING_EXTRA_DATA)
			schedule_work(&ctx->send_work);
	} else {
		pr_err("%s unknown string index %d\n", __func__, ctx->string_index);
	}
}

/*
 * Callback function to process control command AOA_SET_HID_REPORT_DESC.
 * Set desc data to hid-device. If all desc data has been set, the device will be
 * reged in system and set to 'START' state.
 */
static void fxfer_aoa_complete_report_desc(
	struct usb_ep *ep, struct usb_request *req)
{
	struct aoa_hid_dev *dev = (struct aoa_hid_dev *)req->context;
	unsigned int length = req->actual;

	if (req->status != 0) {
		pr_err("%s, failed %d\n", __func__, req->status);
		return;
	}

	pr_info("%s: hid=%p, length=%u, offset=%u, total=%u\n",
		__func__, dev, length, dev->desc_offset, dev->desc_len);

	/* Check desc lenght and offset. */
	if (memcpy_s((dev->desc + dev->desc_offset),
		(dev->desc_len - dev->desc_offset), req->buf, length)) {
		pr_err("%s: copy failed\n", __func__);
		return;
	}
	dev->desc_offset += length;
	if (dev->desc_offset == dev->desc_len)
		schedule_work(&dev->ctx->hid_work);
}

static int fxfer_aoa_cmd_report_desc(struct aoa_context * ctx,
	struct usb_composite_dev *cdev, u16 index, u16 value, u16 length)
{
	struct aoa_hid_dev *dev = NULL;
	unsigned long flags;
	int offset;

	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, value, HID_STATE_NEW);
	spin_unlock_irqrestore(&ctx->lock, flags);
	if (!dev) {
		pr_err("%s: set desc, no such new hid\n", __func__);
		return -EINVAL;
	}
	offset = index;
	if (offset != dev->desc_offset || (offset + length) > dev->desc_len) {
		pr_err("%s: invalid desc %d,%d for %d,%d\n", __func__,
			offset, length, dev->desc_offset, dev->desc_len);
		return -EINVAL;
	}
	pr_info("%s: set desc for %u, offset %u, length %u\n",
		__func__, dev->id, offset, length);
	cdev->req->context = dev;
	cdev->req->complete = fxfer_aoa_complete_report_desc;
	return length;
}

/* Callback function to process control command AOA_SEND_HID_EVENT. */
static void fxfer_aoa_complete_hid_event(
	struct usb_ep *ep, struct usb_request *req)
{
	struct aoa_hid_dev *dev = (struct aoa_hid_dev *)req->context;
	int ret;

	if ((req->status != 0) || (!dev->dev)) {
		pr_err("acc_complete_send_hid_event, err %d\n", req->status);
		return;
	}

	ret = hid_report_raw_event(dev->dev, HID_INPUT_REPORT, req->buf, req->actual, 1);
	pr_info("%s: hid=%p, length=%u, ret=%d\n", __func__, dev, req->actual, ret);
}

static int fxfer_aoa_cmd_hid_event(struct aoa_context * ctx,
	struct usb_composite_dev *cdev, u16 value, u16 length)
{
	struct aoa_hid_dev *dev = NULL;
	unsigned long flags;

	spin_lock_irqsave(&ctx->lock, flags);
	dev = fxfer_aoa_get_hid(ctx, value, HID_STATE_START);
	spin_unlock_irqrestore(&ctx->lock, flags);
	if (!dev) {
		pr_err("%s: no such working hid\n", __func__);
		return -EINVAL;
	}
	cdev->req->context = dev;
	cdev->req->complete = fxfer_aoa_complete_hid_event;
	return length;
}

static int fxfer_aoa_do_cmd_out(struct aoa_context * ctx,
	struct usb_composite_dev *cdev, const struct usb_ctrlrequest *ctrl)
{
	int ret = -EOPNOTSUPP;
	u8 request = ctrl->bRequest;
	u16 index = le16_to_cpu(ctrl->wIndex);
	u16 value = le16_to_cpu(ctrl->wValue);
	u16 length = le16_to_cpu(ctrl->wLength);

	switch (request) {
		case AOA_START :
			ctx->start_requested = 1;
			/* Send start work event via uevent in 10ms later. */
			schedule_delayed_work(&ctx->start_work, msecs_to_jiffies(10));
			ret = 0;
			cdev->req->complete = fxfer_aoa_complete_stub;
			break;
		case AOA_SEND_STRING :
			schedule_work(&ctx->sendstring_work);
			ctx->string_index = index;
			cdev->gadget->ep0->driver_data = ctx;
			cdev->req->complete = fxfer_aoa_complete_set_string;
			ret = length;
			break;
		case AOA_SET_AUDIO_MODE :
			if (index == 0 && length == 0) {
				ctx->audio_mode = value;
				cdev->req->complete = fxfer_aoa_complete_stub;
				ret = 0;
			}
			break;

		case AOA_REGISTER_HID :
			cdev->req->complete = fxfer_aoa_complete_stub;
			ret = fxfer_aoa_reg_hid(ctx, value, index);
			break;

		case AOA_UNREGISTER_HID :
			cdev->req->complete = fxfer_aoa_complete_stub;
			ret = fxfer_aoa_unreg_hid(ctx, value);
			break;
		case AOA_SET_HID_REPORT_DESC :
			ret = fxfer_aoa_cmd_report_desc(ctx, cdev, index, value, length);
			break;
		case AOA_SEND_HID_EVENT :
			ret = fxfer_aoa_cmd_hid_event(ctx, cdev, value, length);
			break;
		default :
			break;
	}

	return ret;
}

static int fxfer_aoa_do_cmd_in(struct aoa_context * ctx,
	struct usb_composite_dev *cdev, const struct usb_ctrlrequest *ctrl)
{
	int ret = -EOPNOTSUPP;
	u8 request = ctrl->bRequest;

	if (request == AOA_GET_PROTOCOL) {
		schedule_work(&ctx->getprotocol_work);
		*((u16 *)cdev->req->buf) = 2; /* Version 2 */
		ret = sizeof(u16);
		cdev->req->complete = fxfer_aoa_complete_stub;
		memset_s(ctx->aoa_string, sizeof(ctx->aoa_string), 0, sizeof(ctx->aoa_string));
		ctx->start_requested = 0;
		ctx->audio_mode = 0;
	}

	return ret;
}

static ssize_t fxfer_aoa_read(struct file *fp, char __user *buf,
	size_t count, loff_t *pos)
{
	int ret;
	struct fxfer_client_info *client = (struct fxfer_client_info *)fp->private_data;

	if (!client) {
		pr_info("%s: no client\n", __func__);
		return -ENOENT;
	}

	if (!client->port_online || !client->client_online) {
		pr_info("%s: not ready\n", __func__);
		return -EIO;
	}

	ret = fxfer_sync_read(client, buf, count);
	pr_debug("%s:len=%d, ret=%d\n", __func__, count, ret);

	return ret;
}

static ssize_t fxfer_aoa_write(struct file *fp, const char __user *buf,
	size_t count, loff_t *pos)
{
	int ret;
	size_t sz;
	size_t offset = 0;
	struct fxfer_client_info *client = (struct fxfer_client_info *)fp->private_data;

	if (!client) {
		pr_info("%s: no client\n", __func__);
		return -ENOENT;
	}

	if (!client->port_online || !client->client_online) {
		pr_info("%s: not ready\n", __func__);
		return -EIO;
	}

	while (offset < count) {
		sz = ((count - offset) < client->buf_tx_sz) ? (count - offset) : client->buf_tx_sz;
		ret = fxfer_sync_write(client, buf + offset, sz);
		if (ret < 0)
			return ret;
		offset += sz;
	}
	pr_debug("%s:len=%d, ret=%d\n", __func__, count, ret);

	return count;
}

static long fxfer_aoa_ioctl(struct file *fp, unsigned code, unsigned long value)
{
	struct fxfer_client_info *client = (struct fxfer_client_info *)fp->private_data;
	struct aoa_context *ctx = NULL;
	char *src = NULL;
	int ret;

	pr_info("%s: code=%u\n", __func__, code);

	if (!client || !client->private) {
		pr_err("%s: no client %p\n", __func__, client);
		return -EIO;
	}

	ctx = (struct aoa_context *)client->private;
	switch (code) {
		case AOA_GET_STRING_MANUFACTURER:
		case AOA_GET_STRING_MODEL:
		case AOA_GET_STRING_DESCRIPTION:
		case AOA_GET_STRING_VERSION:
		case AOA_GET_STRING_URI:
		case AOA_GET_STRING_SERIAL:
		case AOA_GET_EXTRA_DATA:
			src = fxfer_aoa_get_string_dst(ctx, (_IOC_NR(code) - 1));
			break;
		case AOA_IS_START_REQUESTED:
			return ctx->start_requested;
		case AOA_GET_AUDIO_MODE:
			return ctx->audio_mode;
		case AOA_IO_CANCEL:
			fxfer_sync_io_cancel(client);
			return 0;
		default:
			break;
	}

	if (!src) {
		pr_err("%s: no src\n", __func__);
		return -EINVAL;
	}

	if (code == AOA_GET_EXTRA_DATA)
		ret = AOA_STRING_SIZE;
	else
		ret = strlen(src) + 1;
	if (copy_to_user((void __user *)value, src, ret))
		ret = -EFAULT;

	pr_info("%s: copy %d bytes data\n", __func__, ret);

	return ret;
}


static int fxfer_aoa_open(struct inode *ip, struct file *fp)
{
	struct fxfer_client_info *client = fxfer_aoa_get_info();

	if (!client) {
		pr_info("%s: no client\n", __func__);
		return -ENOENT;
	}

	pr_info("%s: enter, count=%d\n", __func__, client->open_count);

	client->client_online = 1;
	client->open_count++;
	fp->private_data = client;

	pr_info("%s: finish, count=%d\n", __func__, client->open_count);
	return 0;
}

static int fxfer_aoa_release(struct inode *ip, struct file *fp)
{
	struct fxfer_client_info *client = (struct fxfer_client_info *)fp->private_data;

	if (!client) {
		pr_info("%s: no client\n", __func__);
		return -ENOENT;
	}

	pr_info("%s: enter, count=%d\n", __func__, client->open_count);

	client->open_count--;
	if (client->open_count <= 0) {
		client->client_online = 0;
		/* All client disconnected, notify fxfer function. */
		fxfer_sync_io_cancel(client);
	}
	fp->private_data = NULL;

	pr_info("%s: finish, count=%d\n", __func__, client->open_count);
	return 0;
}

/* file_operations for /dev/usb_accessory */
static const struct file_operations fxfer_aoa_fops = {
	.owner = THIS_MODULE,
	.read = fxfer_aoa_read,
	.write = fxfer_aoa_write,
	.unlocked_ioctl = fxfer_aoa_ioctl,
	.open = fxfer_aoa_open,
	.release = fxfer_aoa_release,
};

static struct miscdevice fxfer_aoa_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "usb_accessory",
	.fops = &fxfer_aoa_fops,
};

static void fxfer_aoa_hid_work(struct work_struct *data)
{
	struct aoa_context *ctx = container_of(data, struct aoa_context, hid_work);
	fxfer_aoa_hid_refresh(ctx);
}

static inline void fxfer_aoa_uevent(char *event)
{
	char *envp[2] = { event, NULL }; /* 2 string for uevent param */

	pr_info("%s: send event %s\n", __func__, event);
	kobject_uevent_env(&fxfer_aoa_device.this_device->kobj, KOBJ_CHANGE, envp);
}

static void fxfer_aoa_getprotocol_work(struct work_struct *data)
{
	fxfer_aoa_uevent("ACCESSORY=GETPROTOCOL");
}

static void fxfer_aoa_send_work(struct work_struct *data)
{
	fxfer_aoa_uevent("ACCESSORY=SEND");
}

static void fxfer_aoa_sendstring_work(struct work_struct *data)
{
	fxfer_aoa_uevent("ACCESSORY=SENDSTRING");
}

static void fxfer_aoa_start_work(struct work_struct *data)
{
	fxfer_aoa_uevent("ACCESSORY=START");
}

static struct fxfer_client_info g_fxfer_aoa = {
	.name = "aoa",
	.flag = XFER_FLAG_IO_SYNC | XFER_DEP_IN | XFER_DEP_OUT,

	.fxfer_init_function = fxfer_aoa_init_function,
	.fxfer_clear = fxfer_aoa_clear,
};

struct fxfer_client_info *fxfer_aoa_get_info(void)
{
	return &g_fxfer_aoa;
}

/* Init aoa_context of client, register misc device /dev/usb_accessory. */
void fxfer_aoa_init(void)
{
	int ret;
	struct aoa_context *ctx = NULL;
	struct fxfer_client_info *client = fxfer_aoa_get_info();

	pr_info("%s enter, flag=0x%x\n", __func__, client->flag);

	if (client->flag & XFER_FLAG_ENABLED) {
		pr_err("%s already inited\n", __func__);
		return;
	}

	client->private = kzalloc(sizeof(struct aoa_context), GFP_KERNEL);
	if (!client->private) {
		pr_err("%s alloc mem failed\n", __func__);
		return;
	}

	ctx = (struct aoa_context *)client->private;
	spin_lock_init(&ctx->lock);
	INIT_LIST_HEAD(&ctx->hid_list);
	INIT_DELAYED_WORK(&ctx->start_work, fxfer_aoa_start_work);
	INIT_WORK(&ctx->send_work, fxfer_aoa_send_work);
	INIT_WORK(&ctx->hid_work, fxfer_aoa_hid_work);
	INIT_WORK(&ctx->getprotocol_work, fxfer_aoa_getprotocol_work);
	INIT_WORK(&ctx->sendstring_work, fxfer_aoa_sendstring_work);

	ret = misc_register(&fxfer_aoa_device);
	if (ret) {
		kfree(client->private);
		client->private = NULL;
		pr_err("%s: reg failed %d\n", __func__, ret);
		return;
	}

	ctx->parent = fxfer_aoa_device.this_device;
	client->flag |= XFER_FLAG_ENABLED;
}

void fxfer_aoa_uninit(void)
{
	struct fxfer_client_info *client = fxfer_aoa_get_info();

	pr_info("%s enter, flag=0x%x\n", __func__, client->flag);

	if (!(client->flag & XFER_FLAG_ENABLED)) {
		pr_err("%s already uninited\n", __func__);
		return;
	}

	client->flag &= ~XFER_FLAG_ENABLED;

	if (client->private) {
		fxfer_aoa_remove_all_hid((struct aoa_context *)client->private);
		msleep(100); /* wait 100ms for hid work to remove all hid */
	}

	misc_deregister(&fxfer_aoa_device);
	pr_info("%s enter, flag=0x%x\n", __func__, client->flag);

	if (client->private) {
		kfree(client->private);
		client->private = NULL;
	}
}

/*
 * Function for processing control request. This function
 * is not called by fxfer driver but gadget driver directly.
 */
int fxfer_aoa_ctrlrequest(struct usb_composite_dev *cdev,
	const struct usb_ctrlrequest *ctrl, bool complete)
{
	struct fxfer_client_info *client = fxfer_aoa_get_info();
	struct aoa_context *ctx = (struct aoa_context *)client->private;
	int ret = -EOPNOTSUPP;

	if (!(client->flag & XFER_FLAG_ENABLED) || !ctx)
		return -ENOTSUPP;

	if (le16_to_cpu(ctrl->wLength) > USB_COMP_EP0_BUFSIZ)
		return -ENOTSUPP;

	if (ctrl->bRequestType == (USB_DIR_OUT | USB_TYPE_VENDOR))
		ret = fxfer_aoa_do_cmd_out(ctx, cdev, ctrl);
	else if (ctrl->bRequestType == (USB_DIR_IN | USB_TYPE_VENDOR))
		ret = fxfer_aoa_do_cmd_in(ctx, cdev, ctrl);

	if (ret >= 0) {
		pr_info("%s: handle req %02x.%02x v%04x i%04x l%u\n", __func__,
			ctrl->bRequestType, ctrl->bRequest, le16_to_cpu(ctrl->wValue),
			le16_to_cpu(ctrl->wIndex), le16_to_cpu(ctrl->wLength));
		if (complete) {
			cdev->req->zero = 0;
			cdev->req->length = ret;
			ret = usb_ep_queue(cdev->gadget->ep0, cdev->req, GFP_ATOMIC);
			if (ret < 0)
				pr_err("%s: setup response queue error\n", __func__);
		}
	}

	return ret;
}
EXPORT_SYMBOL_GPL(fxfer_aoa_ctrlrequest);

/* Function called while usb-disconnect. Clear all hid-devices. */
void fxfer_aoa_disconnect(void)
{
	struct fxfer_client_info *client = fxfer_aoa_get_info();

	pr_info("%s: flag=0x%x\n", __func__, client->flag);
	if ((client->flag & XFER_FLAG_ENABLED) && (client->private)) {
		pr_info("%s: do disconnect\n", __func__);
		fxfer_aoa_remove_all_hid((struct aoa_context *)client->private);
	}
}
EXPORT_SYMBOL_GPL(fxfer_aoa_disconnect);

