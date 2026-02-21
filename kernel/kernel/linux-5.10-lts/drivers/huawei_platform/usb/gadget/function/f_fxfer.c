/*
 * f_fxfer.c
 *
 * Source file of common function driver of fxfer
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/file.h>
#include <linux/device.h>
#include <linux/workqueue.h>
#include <linux/usb.h>
#include <linux/usb_usual.h>
#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>
#include <linux/configfs.h>

#include <securec.h>

#include "configfs.h"
#include "f_fxfer.h"

#define DBG_TX   1
#define DBG_RX   2
#define FXFER_DBG(port, dbg) \
	(((port)->opts) && ((port)->opts->dbg_info & (dbg)))

#define FXFER_COMP_ID_LEN   16
#define FEFER_NUMBER_ENDPOINTS   2
#define FEFER_NOTIFY_NUMBER_ENDPOINTS   3
#define MAX_PKT_LEN_HS   512
#define MAX_PKT_LEN_SS   1024

#define INTR_BUFFER_SIZE        32
#define FXFER_INTR_INTERVAL     6
#define XFER_SS_MAX_BURST       15

#define CMD_TYPE_DBG            0
#define CMD_TYPE_TX_SZ          1
#define CMD_TYPE_RX_SZ          2
#define CMD_TYPE_CLIENT_DBG     3
#define CMD_TYPE_CLASS          4
#define CMD_TYPE_SUBCLASS       5
#define CMD_TYPE_PROTOCAL       6
#define CMD_TYPE_PRE_REQ        7
#define CMD_TYPE_NUM            8

#define CMD_BUF_SZ_LIMIT        (1024 * 1024)
#define XFER_MAX_BUFFER_SIZE    16384
#define XFER_BULK_BUFFER_NUM    2

static const char *fxfer_cmd_list[] = {
	"dbg=",        /* CMD_TYPE_DBG */
	"txsize=",     /* CMD_TYPE_TX_SZ */
	"rxsize=",     /* CMD_TYPE_RX_SZ */
	"clientdbg=",  /* CMD_TYPE_CLIENT_DBG */
	"class=",      /* CMD_TYPE_CLASS */
	"subclass=",   /* CMD_TYPE_SUBCLASS */
	"proto=",      /* CMD_TYPE_PROTOCAL */
	"prereq=",     /* CMD_TYPE_PRE_REQ */
};

/*
 * Global common interface description.
 */
static struct usb_interface_descriptor fxfer_interface_desc = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = 0,
	.bNumEndpoints = FEFER_NUMBER_ENDPOINTS,
	.bInterfaceClass = USB_CLASS_VENDOR_SPEC,
	.bInterfaceSubClass = USB_SUBCLASS_VENDOR_SPEC,
	.bInterfaceProtocol = 0,
};

static struct usb_endpoint_descriptor fxfer_ss_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __cpu_to_le16(MAX_PKT_LEN_SS),
};

static struct usb_ss_ep_comp_descriptor fxfer_ss_in_comp_desc = {
	.bLength = sizeof(fxfer_ss_in_comp_desc),
	.bDescriptorType = USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_endpoint_descriptor fxfer_ss_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __cpu_to_le16(MAX_PKT_LEN_SS),
};

static struct usb_ss_ep_comp_descriptor fxfer_ss_out_comp_desc = {
	.bLength = sizeof(fxfer_ss_out_comp_desc),
	.bDescriptorType = USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_endpoint_descriptor fxfer_ss_intr_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize = __cpu_to_le16(INTR_BUFFER_SIZE),
};

static struct usb_ss_ep_comp_descriptor fxfer_ss_intr_comp_desc = {
	.bLength = sizeof(fxfer_ss_intr_comp_desc),
	.bDescriptorType = USB_DT_SS_ENDPOINT_COMP,
};

static struct usb_endpoint_descriptor fxfer_highspeed_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __cpu_to_le16(MAX_PKT_LEN_HS),
};

static struct usb_endpoint_descriptor fxfer_highspeed_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __cpu_to_le16(MAX_PKT_LEN_HS),
};

static struct usb_endpoint_descriptor fxfer_highspeed_intr_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize = __cpu_to_le16(INTR_BUFFER_SIZE),
	.bInterval = FXFER_INTR_INTERVAL,
};

static struct usb_endpoint_descriptor fxfer_fullspeed_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fxfer_fullspeed_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
};

static struct usb_endpoint_descriptor fxfer_fullspeed_intr_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize = __cpu_to_le16(INTR_BUFFER_SIZE),
	.bInterval = FXFER_INTR_INTERVAL,
};

static struct usb_descriptor_header *fs_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_fullspeed_in_desc,
	(struct usb_descriptor_header *)&fxfer_fullspeed_out_desc,
	NULL,
};

static struct usb_descriptor_header *hs_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_highspeed_in_desc,
	(struct usb_descriptor_header *)&fxfer_highspeed_out_desc,
	NULL,
};

static struct usb_descriptor_header *ss_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_ss_in_desc,
	(struct usb_descriptor_header *)&fxfer_ss_in_comp_desc,
	(struct usb_descriptor_header *)&fxfer_ss_out_desc,
	(struct usb_descriptor_header *)&fxfer_ss_out_comp_desc,
	NULL,
};

static struct usb_descriptor_header *fs_notify_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_fullspeed_in_desc,
	(struct usb_descriptor_header *)&fxfer_fullspeed_out_desc,
	(struct usb_descriptor_header *)&fxfer_fullspeed_intr_desc,
	NULL,
};

static struct usb_descriptor_header *hs_notify_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_highspeed_in_desc,
	(struct usb_descriptor_header *)&fxfer_highspeed_out_desc,
	(struct usb_descriptor_header *)&fxfer_highspeed_intr_desc,
	NULL,
};

static struct usb_descriptor_header *ss_notify_fxfer_descs[] = {
	(struct usb_descriptor_header *)&fxfer_interface_desc,
	(struct usb_descriptor_header *)&fxfer_ss_in_desc,
	(struct usb_descriptor_header *)&fxfer_ss_in_comp_desc,
	(struct usb_descriptor_header *)&fxfer_ss_out_desc,
	(struct usb_descriptor_header *)&fxfer_ss_out_comp_desc,
	(struct usb_descriptor_header *)&fxfer_ss_intr_desc,
	(struct usb_descriptor_header *)&fxfer_ss_intr_comp_desc,
	NULL,
};

struct f_fxfer_port {
	struct usb_function func;
	struct usb_ep *ep_in;
	struct usb_ep *ep_out;
	struct usb_ep *ep_intr;
	struct usb_composite_dev *cdev;
	struct f_fxfer_opts *opts;
	bool suspended;
	spinlock_t port_lock;

	struct fxfer_client_info *client;

	/* Resouces for sync IO */
	struct work_struct stop_work;
	wait_queue_head_t read_wq;
	wait_queue_head_t write_wq;
	struct usb_request *sync_req_in;
	struct usb_request *sync_req_out;
	bool tx_busy;
	bool rx_busy;

	/* Resouces for notification */
	struct usb_request *req_intr;
	bool notify_busy;
};

struct f_fxfer_opts {
	struct usb_function_instance func_inst;
	struct f_fxfer_port *port;
	const char *name;
	char fxfer_ext_compat_id[FXFER_COMP_ID_LEN];
	struct usb_os_desc fxfer_os_desc;
	struct fxfer_client_info *client;
	unsigned int dbg_info;
	unsigned char cls;
	unsigned char subcls;
	unsigned char prot;
};

static inline int fxfer_usb_ep_enable(struct usb_ep *ep)
{
	return (ep) ? usb_ep_enable(ep) : 0;
}

static inline int fxfer_usb_ep_disable(struct usb_ep *ep)
{
	return (ep) ? usb_ep_disable(ep) : 0;
}

static inline int fxfer_usb_ep_set_halt(struct usb_ep *ep)
{
	return (ep) ? usb_ep_set_halt(ep) : 0;
}

static inline int fxfer_usb_ep_clear_halt(struct usb_ep *ep)
{
	return (ep) ? usb_ep_clear_halt(ep) : 0;
}

static inline int fxfer_config_ep_by_speed(struct usb_gadget *g,
	struct usb_function *f, struct usb_ep *ep)
{
	return (ep) ? config_ep_by_speed(g, f, ep) : 0;
}

static inline struct f_fxfer_opts *to_fxfer_opts(struct config_item *item)
{
	return container_of(to_config_group(item),
		struct f_fxfer_opts, func_inst.group);
}

static inline struct f_fxfer_port *func_to_fxfer(struct usb_function *f)
{
	return container_of(f, struct f_fxfer_port, func);
}

/* Allocate interface id and desc string id */
static int fxfer_allocate_interface_id(struct f_fxfer_port *port,
	struct usb_configuration *c, struct usb_function *f)
{
	int id;
	int strid;

	/* allocate interface id for function */
	id = usb_interface_id(c, f);
	if (id < 0) {
		pr_info("%s alloc intf failed\n", __func__);
		return id;
	}
	pr_info("%s alloc intf id %d\n", __func__, id);
	fxfer_interface_desc.bInterfaceNumber = id;

	/* Currently only support interface desc string, it should be in first. */
	if (f->strings && f->strings[0] && f->strings[0]->strings) {
		strid = usb_string_id(c->cdev);
		if (strid > 0) {
			pr_info("%s alloc intf string id %d\n", __func__, strid);
			fxfer_interface_desc.iInterface = strid;
			f->strings[0]->strings->id = strid;
		}
	}

	return 0;
}

/*
 * Set XFER_FLAG_OS_DESC to flag of client to allocate os desc.
 * Add such following cmds in script file to create a WINUSB interface:
 * write /config/usb_gadget/g1/functions/fxfer.XXX/os_desc/interface.FXFER/compatible_id "WINUSB"
 * mkdir /config/usb_gadget/g1/functions/fxfer.XXX/os_desc/interface.FXFER/DeviceInterfaceGUID
 * write /config/usb_gadget/g1/functions/fxfer.XXX/os_desc/interface.FXFER/DeviceInterfaceGUID/type "1"
 * write /config/usb_gadget/g1/functions/fxfer.XXX/os_desc/interface.FXFER/DeviceInterfaceGUID/data "{GUID}"
 * GUID format is like 'c262fd51-8d0a-48c8-a5a6-6cd7b5dda3c8'
 */
static int fxfer_fill_os_desc_table(struct usb_configuration *c,
	struct usb_function *f, struct f_fxfer_port *port)
{
	struct usb_composite_dev *cdev = c->cdev;
	struct f_fxfer_opts *ops = NULL;

	pr_info("%s enter\n", __func__);

	if (!cdev->use_os_string || !(port->client->flag & XFER_FLAG_OS_DESC)) {
		pr_info("%s:flag 0x%x no os desc\n", __func__, port->client->flag);
		return 0;
	}

	ops = container_of(f->fi, struct f_fxfer_opts, func_inst);

	/* use_os_string: value of '/config/usb_gadget/g1/os_desc/use' */
	if (cdev->use_os_string) {
		f->os_desc_table = kzalloc(sizeof(*f->os_desc_table), GFP_KERNEL);
		if (f->os_desc_table == NULL) {
			pr_err("%s: alloc mem failed\n", __func__);
			return -ENOMEM;
		}
		f->os_desc_n = 1;
		f->os_desc_table[0].os_desc = &ops->fxfer_os_desc;
		f->os_desc_table[0].if_id = fxfer_interface_desc.bInterfaceNumber;
	}

	return 0;
}

/* create endpoint */
static struct usb_ep *fxfer_create_ep(struct f_fxfer_port *port,
	struct usb_endpoint_descriptor *dep)
{
	struct usb_ep *ep = NULL;
	struct usb_composite_dev *cdev = port->cdev;

	if (dep) {
		ep = usb_ep_autoconfig(cdev->gadget, dep);
		if (ep == NULL) {
			pr_info("usb_ep_autoconfig for ep failed\n");
			return NULL;
		}
		pr_info("usb_ep_autoconfig for ep got %s\n", ep->name);
	}

	return ep;
}

/* Allocate a request buffer for endpoint */
static struct usb_request *fxfer_create_req(struct usb_ep *ep, int sz)
{
	struct usb_request *req = NULL;

	if (!ep || (sz <= 0))
		return NULL;

	req = usb_ep_alloc_request(ep, GFP_KERNEL);
	if (req) {
		req->buf = kmalloc(sz, GFP_KERNEL);
		if (!req->buf) {
			pr_err("%s: alloc req buf for %s failed\n", __func__, ep->name);
			usb_ep_free_request(ep, req);
			return NULL;
		}
		memset_s(req->buf, sz, 0, sz);
		pr_info("%s: alloc req size %d of %s\n", __func__, sz, ep->name);
	} else {
		pr_err("%s: alloc req for %s failed\n", __func__, ep->name);
	}

	return req;
}

/* Release request buffer for endpoint */
static void fxfer_delete_req(struct usb_request *req, struct usb_ep *ep)
{
	if (req && ep) {
		pr_info("%s: free req of %s\n", __func__, ep->name);
		if (req->buf) {
			kfree(req->buf);
			req->buf = NULL;
		}
		usb_ep_free_request(ep, req);
	}
}

/* Callback function of sync tx of ep-in transfer */
static void fxfer_sync_complete_in(struct usb_ep *ep, struct usb_request *req)
{
	struct f_fxfer_port *port = NULL;

	if (!ep || !req || !ep->driver_data) {
		pr_err("%s: invalid %d, %d, %d\n", __func__,
			NOT_NULL(ep), NOT_NULL(req), NOT_NULL(ep ? ep->driver_data : 0));
		return;
	}

	port = (struct f_fxfer_port *)ep->driver_data;
	/* IO finish, clear busy flag. */
	if (port->tx_busy) {
		port->tx_busy = false;
		wake_up(&port->write_wq);
	}

	if (FXFER_DBG(port, DBG_TX))
		pr_info("%s: ep=%s, status=%d, sz=%d\n",
			__func__, ep->name, req->status, req->actual);
}

/* Callback function of sync rx of ep-out transfer */
static void fxfer_sync_complete_out(struct usb_ep *ep, struct usb_request *req)
{
	struct f_fxfer_port *port = NULL;

	if (!ep || !req || !ep->driver_data) {
		pr_err("%s: invalid %d, %d, %d\n",
			__func__, NOT_NULL(ep), NOT_NULL(req), NOT_NULL(ep ? ep->driver_data : 0));
		return;
	}

	port = (struct f_fxfer_port *)ep->driver_data;
	/* IO finish, clear busy flag. */
	if (port->rx_busy) {
		port->rx_busy = false;
		wake_up(&port->read_wq);
	}

	if (FXFER_DBG(port, DBG_RX))
		pr_info("%s: ep=%s, status=%d, sz=%d\n",
			__func__, ep->name, req->status, req->actual);
}

/* Callback function of sync tx of ep-intr transfer */
static void fxfer_sync_complete_intr(struct usb_ep *ep, struct usb_request *req)
{
	struct f_fxfer_port *port = NULL;

	if (!ep || !req || !ep->driver_data) {
		pr_err("%s: invalid %d, %d, %d\n", __func__,
			NOT_NULL(ep), NOT_NULL(req), NOT_NULL(ep ? ep->driver_data : 0));
		return;
	}

	port = (struct f_fxfer_port *)ep->driver_data;
	port->notify_busy = false;

	pr_info("%s: ep=%s, status=%d, sz=%d\n",
		__func__, ep->name, req->status, req->actual);
}

static void fxfer_sync_fill_usb_req(struct f_fxfer_port *port, int sz, bool tx)
{
	if (tx) {
		if (FXFER_DBG(port, DBG_TX))
			pr_info("%s: try to write %d bytes, ep=%d, req=%d\n",
				__func__, sz,  NOT_NULL(port->ep_in), NOT_NULL(port->sync_req_in));
		port->sync_req_in->length = sz;
		port->sync_req_in->zero = ((sz % port->ep_in->maxpacket) == 0);
		port->sync_req_in->complete = fxfer_sync_complete_in;
		port->ep_in->driver_data = port;
	} else {
		if (FXFER_DBG(port, DBG_RX))
			pr_info("%s: try to read %d bytes, ep=%d, req=%d\n",
				__func__, sz, NOT_NULL(port->ep_out), NOT_NULL(port->sync_req_out));
		port->sync_req_out->length = sz;
		port->sync_req_out->complete = fxfer_sync_complete_out;
		port->ep_out->driver_data = port;
	}
}

static void fxfer_stop_sync_io_work(struct work_struct *data)
{
	struct f_fxfer_port *port = container_of(data, struct f_fxfer_port, stop_work);

	pr_info("%s: enter\n", __func__);

	if (!port->client || !port->client->port_online) {
		pr_info("%s: client is not online\n", __func__);
		return;
	}

	if (port->tx_busy){
		pr_info("%s: stop sync write\n", __func__);
		(void)usb_ep_dequeue(port->ep_in, port->sync_req_in);
	}

	if (port->rx_busy) {
		pr_info("%s: stop sync read\n", __func__);
		(void)usb_ep_dequeue(port->ep_out, port->sync_req_out);
	}
}

/* Interface for sync write */
int fxfer_sync_write(struct fxfer_client_info *client, const char *buf, int sz)
{
	struct f_fxfer_port *port = NULL;
	int ret;

	if (!client || !buf || (sz <= 0) || (sz > client->buf_tx_sz)) {
		pr_err("%s: invalid arg %d\n", __func__, sz);
		return -EINVAL;
	}

	port = (struct f_fxfer_port *)client->context;
	if (!port) {
		pr_err("%s: no port\n", __func__);
		return -EINVAL;
	}

	if (FXFER_DBG(port, DBG_TX))
		pr_info("%s: client=%s, sz=%d\n", __func__, client->name, sz);

	if (port->suspended || port->tx_busy || !port->sync_req_in) {
		if (FXFER_DBG(port, DBG_TX))
			pr_info("%s: dump state=%d, %d, 0x%x.\n", __func__,
				port->suspended, port->tx_busy, port->sync_req_in);
		return -EIO;
	}

	if (copy_from_user(port->sync_req_in->buf, buf, sz)) {
		pr_err("%s: copy failed\n", __func__);
		return -EFAULT;
	}

	fxfer_sync_fill_usb_req(port, sz, true);

	/* Send usb request to usb driver and set busy flag. */
	port->tx_busy = true;
	ret = usb_ep_queue(port->ep_in, port->sync_req_in, GFP_KERNEL);
	if (ret < 0) {
		pr_err("%s: usb_ep_queue failed %d\n", __func__, ret);
		port->tx_busy = false;
		return -EIO;
	}

	/*
	 * Wait for IO complete. Flag 'port_online' means usb
	 * connection and flag 'client_online' means client opened.
	 */
	ret = wait_event_interruptible(port->write_wq, (!port->tx_busy));
	if (!client->port_online) {
		port->tx_busy = false;
		pr_info("%s: port offline\n", __func__);
		return -ESHUTDOWN;
	}

	if ((ret < 0) || (port->tx_busy)) {
		port->tx_busy = false;
		pr_err("%s: wait_event_interruptible ret %d\n", __func__, ret);
		if (port->ep_in && port->sync_req_in) {
			pr_err("%s: cancel IO\n", __func__);
			(void)usb_ep_dequeue(port->ep_in, port->sync_req_in);
		}
		return ((ret < 0) ? ret : -EIO);
	}

	port->tx_busy = false;

	if (!client->client_online) {
		pr_info("%s: client offline\n", __func__);
		return -ESHUTDOWN;
	}

	if (port->sync_req_in->status != 0) {
		if (FXFER_DBG(port, DBG_TX))
			pr_err("%s: status=%d\n", __func__, port->sync_req_in->status);
		return -EIO;
	}

	if (FXFER_DBG(port, DBG_TX))
		pr_info("%s: writen %d bytes\n", __func__, port->sync_req_in->actual);
	return port->sync_req_in->actual;
}

/* Interface for sync read */
int fxfer_sync_read(struct fxfer_client_info *client, char *buf, int sz)
{
	struct f_fxfer_port *port = NULL;
	int ret;

	if (!client || !buf || (sz <= 0) || (sz > client->buf_rx_sz)) {
		pr_err("%s: invalid arg %d\n", __func__, sz);
		return -EINVAL;
	}

	port = (struct f_fxfer_port *)client->context;
	if (!port) {
		pr_err("%s: no port\n", __func__);
		return -EINVAL;
	}

	if (FXFER_DBG(port, DBG_RX))
		pr_info("%s: client=%s, sz=%d\n", __func__, client->name, sz);

	if (port->suspended || port->rx_busy || !port->sync_req_out) {
		if (FXFER_DBG(port, DBG_RX))
			pr_err("%s: dump state=%d, %d, %d\n", __func__,
				port->suspended, port->rx_busy, port->sync_req_out);
		return -EIO;
	}

	fxfer_sync_fill_usb_req(port, sz, false);

	/* Send usb request to usb driver and set busy flag. */
	port->rx_busy = true;
	ret = usb_ep_queue(port->ep_out, port->sync_req_out, GFP_KERNEL);
	if (ret < 0) {
		pr_err("%s: usb_ep_queue failed %d\n", __func__, ret);
		port->rx_busy = false;
		return -EIO;
	}

	/*
	 * Wait for IO complete. Flag 'port_online' means usb
	 * connection and flag 'client_online' means client opened.
	 */
	ret = wait_event_interruptible(port->read_wq, (!port->rx_busy));
	if (!client->port_online) {
		port->rx_busy = false;
		pr_info("%s: port offline\n", __func__);
		return -ESHUTDOWN;
	}

	if ((ret < 0) || (port->rx_busy)) {
		port->rx_busy = false;
		pr_err("%s: wait_event_interruptible ret %d\n", __func__, ret);
		if (port->ep_out && port->sync_req_out) {
			pr_err("%s: cancel IO\n", __func__);
			(void)usb_ep_dequeue(port->ep_out, port->sync_req_out);
		}
		return ((ret < 0) ? ret : -EIO);
	}

	port->rx_busy = false;

	if (!client->client_online) {
		pr_info("%s: client offline\n", __func__);
		return -ESHUTDOWN;
	}

	if (port->sync_req_out->status != 0) {
		if (FXFER_DBG(port, DBG_RX))
			pr_err("%s: status=%d\n", __func__, port->sync_req_out->status);
		return -EIO;
	}

	ret = (port->sync_req_out->actual < sz) ? port->sync_req_out->actual : sz;
	if (copy_to_user((void *)buf, (const void *)port->sync_req_out->buf, ret)) {
		pr_err("%s: copy_to_user failed\n", __func__);
		return -EFAULT;
	}

	if (FXFER_DBG(port, DBG_RX))
		pr_info("%s: read %d bytes\n", __func__, port->sync_req_out->actual);
	return port->sync_req_out->actual;
}

/* Stop sync io transfer */
void fxfer_sync_io_cancel(struct fxfer_client_info *client)
{
	struct f_fxfer_port *port = NULL;

	if (!client || !client->context || !client->port_online) {
		pr_err("%s: port %s not online\n", __func__, (client ? client->name : "NA"));
		return;
	}

	port = (struct f_fxfer_port *)client->context;
	pr_info("%s:name=%s\n", __func__, client->name);

	if (port->tx_busy || port->rx_busy) {
		pr_info("%s: schedule_work to stop sync io\n", __func__);
		schedule_work(&port->stop_work);
	}
}

static void fxfer_bulk_io_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct f_fxfer_port *port = NULL;
	struct fxfer_io_req *io_req = NULL;
	struct usb_request *client_req = NULL;

	if (!ep || !req || !ep->driver_data || !req->context) {
		pr_err("%s: invalid %d, %d\n", __func__, NOT_NULL(ep), NOT_NULL(req));
		return;
	}

	port = (struct f_fxfer_port *)ep->driver_data;
	io_req = (struct fxfer_io_req *)req->context;
	client_req = (struct usb_request *)io_req->port_data;

	if (!port->client) {
		usb_ep_free_request(ep, req);
		return;
	}

	if (port->client->fxfer_io_finish) {
		client_info(port->client, "%s: status=%d, actual=%d\n", __func__, req->status, req->actual);
		port->client->fxfer_io_finish(port->client, io_req, ((!req->status) ? req->actual : -EIO));
	}

	if (client_req != req) {
		client_info(port->client, "%s: free req\n", __func__);
		usb_ep_free_request(ep, req);
	}
}

int fxfer_bulk_io_request(struct fxfer_client_info *client, struct fxfer_io_req *io_req)
{
	struct usb_ep *ep = NULL;
	struct usb_request *req = NULL;
	struct f_fxfer_port *port = NULL;
	int ret;

	if (!client || !io_req) {
		pr_err("%s: invalid param\n", __func__);
		return -EINVAL;
	}

	if (!client->fxfer_io_finish || !client->context || !client->port_online) {
		pr_err("%s: invalid client\n", __func__);
		return -EINVAL;
	}

	if (!io_req->buffer || !io_req->len) {
		pr_err("%s: invalid req=%d, buffer=%d, len=%u\n", __func__,
			NOT_NULL(io_req->port_data), NOT_NULL(io_req->buffer), io_req->len);
		return -EINVAL;
	}

	client_info(client, "%s: dump io: addr=0x%lx, len=%u, offset=%u, tx=%d\n",
		__func__, (uint64_t)io_req->buffer, io_req->len, io_req->offset, io_req->tx);

	port = (struct f_fxfer_port *)client->context;
	ep = io_req->tx ? port->ep_in : port->ep_out;
	if (io_req->port_data) {
		req = (struct usb_request *)io_req->port_data;
	} else {
		client_info(client, "%s: need alloc req\n", __func__);
		req = usb_ep_alloc_request(ep, GFP_ATOMIC);
		if (!req) {
			pr_err("%s: alloc req failed\n", __func__);
			return -ENOMEM;
		}
	}

	req->length = io_req->len;
	req->buf = io_req->buffer + io_req->offset;
	req->complete = fxfer_bulk_io_complete;
	if (io_req->tx)
		req->zero = ((req->length % port->ep_in->maxpacket) == 0);
	req->context = io_req;
	ep->driver_data = port;

	ret = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (ret < 0) {
		pr_err("%s: ret %d\n", __func__, ret);
		if (!io_req->port_data)
			usb_ep_free_request(ep, req);
	}

	return ret;
}

int fxfer_bulk_io_allocreq(struct fxfer_client_info *client, struct fxfer_io_req *io_req)
{
	struct f_fxfer_port *port = NULL;
	struct usb_ep *ep = NULL;
	struct usb_request *req = NULL;

	if (!client || !io_req || !client->context) {
		pr_err("%s: invalid arg %d %d\n", __func__, NOT_NULL(client), NOT_NULL(io_req));
		return -EINVAL;
	}

	if (!client->port_online || !client->client_online) {
		pr_info("%s: port not online: %d-%d\n", __func__, client->port_online, client->client_online);
		return -EIO;
	}

	if (io_req->port_data) {
		pr_info("%s: already alloced\n", __func__);
		return 0;
	}

	port = (struct f_fxfer_port *)client->context;
	ep = io_req->tx ? port->ep_in : port->ep_out;
	req = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (!req) {
		pr_err("%s: alloc ep req failed\n", __func__);
		return -EIO;
	}
	io_req->port_data = (char *)req;

	if (io_req->tx) {
		client->buf_bulk_tx_num++;
		pr_info("%s: tx num=%d\n", __func__, client->buf_bulk_tx_num);
	} else {
		client->buf_bulk_rx_num++;
		pr_info("%s: rx num=%d\n", __func__, client->buf_bulk_rx_num);
	}
	return 0;
}

int fxfer_bulk_io_freereq(struct fxfer_client_info *client, struct fxfer_io_req *io_req)
{
	struct f_fxfer_port *port = NULL;
	struct usb_ep *ep = NULL;
	struct usb_request *req = NULL;

	if (!client || !io_req || !client->context)
		return -EINVAL;

	port = (struct f_fxfer_port *)client->context;
	ep = io_req->tx ? port->ep_in : port->ep_out;
	req = (struct usb_request *)io_req->port_data;
	io_req->port_data = NULL;

	if (req) {
		usb_ep_free_request(ep, req);
		if (io_req->tx) {
			client->buf_bulk_tx_num--;
			pr_info("%s: tx num=%d\n", __func__, client->buf_bulk_tx_num);
		} else {
			client->buf_bulk_rx_num--;
			pr_info("%s: rx num=%d\n", __func__, client->buf_bulk_rx_num);
		}
	}

	return 0;
}

void fxfer_bulk_io_disable(struct fxfer_client_info *client)
{
	int ret;
	struct f_fxfer_port *port = NULL;

	if (!client || !client->context || !client->port_online) {
		pr_err("%s: invalid client %d\n", __func__, NOT_NULL(client));
		return;
	}

	port = (struct f_fxfer_port *)client->context;
	fxfer_usb_ep_disable(port->ep_out);
	fxfer_usb_ep_disable(port->ep_in);
}

static int fxfer_set_ep_enable(struct f_fxfer_port *port, struct usb_ep *ep)
{
	int ret;
	struct usb_function *f = NULL;

	if (!port || !ep || !port->client || !port->client->port_online) {
		pr_info("%s:invalid arg\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: ep=%s\n", __func__, ep->name);
	f = &port->func;

	if (!port->client || !port->client->port_online) {
		pr_info("%s: client=%d\n", __func__, NOT_NULL(port->client));
		return 0;
	}

	if (!ep->desc) {
		pr_info("%s: try to config\n", __func__);
		ret = fxfer_config_ep_by_speed(f->config->cdev->gadget, f, ep);
		if (ret) {
			pr_err("%s: config failed\n", __func__);
			return ret;
		}
	}

	ret = fxfer_usb_ep_enable(ep);
	if (ret) {
		pr_err("%s: enable failed\n", __func__);
		return ret;
	}

	return 0;
}

void fxfer_bulk_io_enable(struct fxfer_client_info *client)
{
	int ret;
	struct f_fxfer_port *port = NULL;

	if (!client || !client->context || !client->port_online) {
		pr_err("%s: invalid client %d\n", __func__, NOT_NULL(client));
		return;
	}

	pr_info("%s: enter\n", __func__);
	port = (struct f_fxfer_port *)client->context;
	fxfer_set_ep_enable(port, port->ep_out);
	fxfer_set_ep_enable(port, port->ep_in);
}

int fxfer_send_notify(struct fxfer_client_info *client, const char *buf, int sz)
{
	struct f_fxfer_port *port = NULL;
	int ret;

	if (!client || !buf || (sz <= 0) || (sz > INTR_BUFFER_SIZE)) {
		pr_err("%s: invalid arg %d\n", __func__, sz);
		return -EINVAL;
	}

	port = (struct f_fxfer_port *)client->context;
	if (!port || !client->port_online) {
		pr_err("%s: no port or offline\n", __func__);
		return -EINVAL;
	}

	pr_info("%s: client=%s, sz=%d, busy=%d\n", __func__, client->name, sz, (int)port->notify_busy);

	if (port->suspended || !port->req_intr) {
		pr_info("%s: dump state=%d, %d.\n", __func__, port->suspended, NOT_NULL(port->req_intr));
		return -EIO;
	}

	if (port->notify_busy) {
		(void)usb_ep_dequeue(port->ep_intr, port->req_intr);
		ret = 10; /* try 10 times */
		while (port->notify_busy && (ret-- > 0))
			msleep(10); /* wait 10ms to cancel last notify */

		if (port->notify_busy) {
			pr_err("%s: still busy after cancel last notify.\n", __func__);
			return -EBUSY;
		}
	}

	if (memcpy_s(port->req_intr->buf, INTR_BUFFER_SIZE, buf, sz)) {
		pr_err("%s: copy failed\n", __func__);
		return -EFAULT;
	}

	port->req_intr->length = sz;
	port->req_intr->complete = fxfer_sync_complete_intr;
	port->ep_intr->driver_data = port;

	/* Send usb request to usb driver and set busy flag. */
	port->notify_busy = true;
	ret = usb_ep_queue(port->ep_intr, port->req_intr, GFP_KERNEL);
	if (ret < 0) {
		pr_err("%s: usb_ep_queue failed %d\n", __func__, ret);
		port->notify_busy = false;
		return -EIO;
	}

	return 0;
}

/* Create endpoints for function */
static int fxfer_create_endpoints(struct f_fxfer_port *port,
	struct usb_endpoint_descriptor *in_desc,
	struct usb_endpoint_descriptor *out_desc,
	struct usb_endpoint_descriptor *intr_desc)
{
	pr_info("%s: in=%d, out=%d, intr=%d, tx=%u, rx=%u\n",
		__func__, NOT_NULL(in_desc), NOT_NULL(out_desc), NOT_NULL(intr_desc),
		port->client->buf_tx_sz, port->client->buf_rx_sz);

	port->ep_in = fxfer_create_ep(port, in_desc);
	port->ep_out = fxfer_create_ep(port, out_desc);
	port->ep_intr = fxfer_create_ep(port, intr_desc);

	if (port->client->flag & XFER_FLAG_IO_SYNC) {
		if (port->ep_in)
			port->sync_req_in = fxfer_create_req(port->ep_in, port->client->buf_tx_sz);
		if (port->ep_out)
			port->sync_req_out = fxfer_create_req(port->ep_out, port->client->buf_rx_sz);
	}

	port->req_intr = fxfer_create_req(port->ep_intr, INTR_BUFFER_SIZE);

	return 0;
}

/* Delete all usb request. */
static void fxfer_del_all_req(struct f_fxfer_port *port)
{
	fxfer_delete_req(port->sync_req_in, port->ep_in);
	port->sync_req_in = NULL;
	fxfer_delete_req(port->sync_req_out, port->ep_out);
	port->sync_req_out = NULL;
	fxfer_delete_req(port->req_intr, port->ep_intr);
	port->req_intr = NULL;
}

/* Set address for all endpoints desc */
static void fxfer_set_dep_addr(unsigned int addr_in,
	unsigned int addr_out, unsigned int addr_intr)
{
	pr_info("%s, addr=%x,%x,%x\n", __func__, addr_in, addr_out, addr_intr);

	fxfer_fullspeed_in_desc.bEndpointAddress = addr_in;
	fxfer_highspeed_in_desc.bEndpointAddress = addr_in;
	fxfer_ss_in_desc.bEndpointAddress = addr_in;

	fxfer_fullspeed_out_desc.bEndpointAddress = addr_out;
	fxfer_highspeed_out_desc.bEndpointAddress = addr_out;
	fxfer_ss_out_desc.bEndpointAddress = addr_out;

	fxfer_fullspeed_intr_desc.bEndpointAddress = addr_intr;
	fxfer_highspeed_intr_desc.bEndpointAddress = addr_intr;
	fxfer_ss_intr_desc.bEndpointAddress = addr_intr;
}

static int fxfer_set_func_desc(struct f_fxfer_port *port, struct usb_function *f)
{
	int ret;

	/* Allocate endpoints for interface. */
	ret = fxfer_create_endpoints(port,
		(port->client->flag & XFER_DEP_IN) ? &fxfer_fullspeed_in_desc : NULL,
		(port->client->flag & XFER_DEP_OUT) ? &fxfer_fullspeed_out_desc : NULL,
		(port->client->flag & XFER_DEP_INTR) ? &fxfer_fullspeed_intr_desc : NULL);
	if (ret) {
		pr_err("%s: fxfer_create_endpoints failed %d\n", __func__, ret);
		return ret;
	}

	fxfer_set_dep_addr(((port->client->flag & XFER_DEP_IN) ? fxfer_fullspeed_in_desc.bEndpointAddress : 0),
		((port->client->flag & XFER_DEP_OUT) ? fxfer_fullspeed_out_desc.bEndpointAddress : 0),
		((port->client->flag & XFER_DEP_INTR) ? fxfer_fullspeed_intr_desc.bEndpointAddress : 0));

	fxfer_interface_desc.bInterfaceClass = (port->opts->cls) ? (port->opts->cls) : USB_CLASS_VENDOR_SPEC;
	fxfer_interface_desc.bInterfaceSubClass = (port->opts->subcls) ? (port->opts->subcls) : USB_SUBCLASS_VENDOR_SPEC;
	fxfer_interface_desc.bInterfaceProtocol = (port->opts->prot) ? (port->opts->prot) : 0;

	if (port->client->flag & XFER_FLAG_IO_BULK) {
		fxfer_ss_in_comp_desc.bMaxBurst = XFER_SS_MAX_BURST;
		fxfer_ss_out_comp_desc.bMaxBurst = XFER_SS_MAX_BURST;
	} else {
		fxfer_ss_in_comp_desc.bMaxBurst = 0;
		fxfer_ss_out_comp_desc.bMaxBurst = 0;
	}

	/* Copy global desc to current funtion. */
	if (port->client->flag & XFER_DEP_INTR) {
		fxfer_interface_desc.bNumEndpoints = FEFER_NOTIFY_NUMBER_ENDPOINTS;
		ret = usb_assign_descriptors(f, fs_notify_fxfer_descs, hs_notify_fxfer_descs,
			ss_notify_fxfer_descs, ss_notify_fxfer_descs);
	} else {
		fxfer_interface_desc.bNumEndpoints = FEFER_NUMBER_ENDPOINTS;
		ret = usb_assign_descriptors(f, fs_fxfer_descs, hs_fxfer_descs,
			ss_fxfer_descs, ss_fxfer_descs);
	}

	return ret;
}

static int fxfer_bind(struct usb_configuration *c, struct usb_function *f)
{
	int ret;
	struct usb_composite_dev *cdev = c->cdev;
	struct f_fxfer_port *port = func_to_fxfer(f);

	pr_info("%s: %s\n", __func__, port->client->name);

	port->cdev = cdev;

	ret = fxfer_allocate_interface_id(port, c, f);
	if (ret)
		return ret;

	ret = fxfer_fill_os_desc_table(c, f, port);
	if (ret) {
		pr_err("%s fill desc err\n", __func__);
		return ret;
	}

	ret = fxfer_set_func_desc(port, f);
	if (ret) {
		pr_err("%s set desc err %d\n", __func__, ret);
		goto fail;
	}

	pr_info("%s: %s speed %s\n", __func__,
		gadget_is_superspeed_plus(c->cdev->gadget) ? "super-plus" :
		gadget_is_superspeed(c->cdev->gadget) ? "super" :
		(gadget_is_dualspeed(c->cdev->gadget) ? "dual" : "full"), f->name);

	return 0;

fail:
	if (f->os_desc_table) {
		kfree(f->os_desc_table);
		f->os_desc_table = NULL;
	}
	fxfer_del_all_req(port);
	return -EIO;
}

static int fxfer_config_enable_ep(struct usb_function *f, struct usb_ep *ep)
{
	int ret;

	if (!f || !ep)
		return 0;

	ret = fxfer_config_ep_by_speed(f->config->cdev->gadget, f, ep);
	if (ret) {
		pr_err("%s: config %s failed\n", __func__, ep->name);
		return ret;
	}

	ret = fxfer_usb_ep_enable(ep);
	if (ret) {
		pr_err("%s: enable %s failed\n", __func__, ep->name);
		return ret;
	}

	return 0;
}

/*
 * Set-config will trigger set alt function. It means that PC host has
 * already recognize this USB device. Now we should enable all ep
 * and begin to do IO transfer. Set current port to online state.
 */
static int fxfer_set_alt(struct usb_function *f, unsigned intf, unsigned alt)
{
	struct f_fxfer_port *port = func_to_fxfer(f);
	int ret;

	pr_info("%s enter, inft=%u, alt=%u", __func__, intf, alt);

	ret = fxfer_config_enable_ep(f, port->ep_in);
	if (ret)
		return ret;

	ret = fxfer_config_enable_ep(f, port->ep_out);
	if (ret) {
		fxfer_usb_ep_disable(port->ep_in);
		return ret;
	}

	ret = fxfer_config_enable_ep(f, port->ep_intr);
	if (ret) {
		fxfer_usb_ep_disable(port->ep_out);
		fxfer_usb_ep_disable(port->ep_in);
		return ret;
	}

	port->client->port_online = 1;
	port->suspended = false;

	if (port->client->fxfer_connect)
		port->client->fxfer_connect(port->client);

	return 0;
}

/*
 * Process control req from PC host. Common fxfer funciton
 * do not support any control req, send it to client driver.
 * If client dirver process this req, we will send result to
 * PC host via ep0.
 */
static int fxfer_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
	struct f_fxfer_port *port = func_to_fxfer(f);
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_request *req = cdev->req;
	int value = -EOPNOTSUPP;

	if (port->client && port->client->fxfer_ctrl_req)
		value = port->client->fxfer_ctrl_req(port->client, f, ctrl);

	if (value >= 0) {
		req->zero = 0;
		req->length = value;
		value = usb_ep_queue(cdev->gadget->ep0, req, GFP_ATOMIC);
		if (value < 0)
			pr_err("%s: usb_ep_queue=%d\n", __func__, value);
	}

	return value;
}

/* Function disabled while disconnecting. */
static void fxfer_disable(struct usb_function *f)
{
	struct f_fxfer_port *port = func_to_fxfer(f);

	pr_info("%s enter", __func__);
	fxfer_usb_ep_disable(port->ep_in);
	fxfer_usb_ep_disable(port->ep_out);
	fxfer_usb_ep_disable(port->ep_intr);
}

/* Unbind function. */
static void fxfer_unbind(struct usb_configuration *c, struct usb_function *f)
{
	struct f_fxfer_port *port = func_to_fxfer(f);

	pr_info("%s enter", __func__);

	if (port->client) {
		if (port->client->fxfer_clear)
			port->client->fxfer_clear(port->client);
		port->client->port_online = 0;
	}

	/*
	 * Trigger waiting event of sync IO. It will stop the
	 * transfer work and return fail event to client.
	 */
	wake_up(&port->read_wq);
	wake_up(&port->write_wq);

	fxfer_del_all_req(port);

	if (f->os_desc_table)
		kfree(f->os_desc_table);
	f->os_desc_table = NULL;
	f->os_desc_n = 0;

	usb_free_all_descriptors(f);

	pr_info("%s finish", __func__);
}

/* Free resource of function. */
static void fxfer_free_func(struct usb_function *f)
{
	struct f_fxfer_port *port = func_to_fxfer(f);
	struct f_fxfer_opts *opts = port->opts;
	pr_info("%s enter", __func__);
	if (port->client)
		port->client->context = NULL;
	kfree(port);
	opts->port = NULL;
}

static void fxfer_resume(struct usb_function *f)
{
	struct f_fxfer_port *port = func_to_fxfer(f);
	unsigned long flags;

	pr_info("%s enter", __func__);

	spin_lock_irqsave(&port->port_lock, flags);
	port->suspended = false;
	spin_unlock_irqrestore(&port->port_lock, flags);

}

static void fxfer_suspend(struct usb_function *f)
{
	struct f_fxfer_port *port = func_to_fxfer(f);
	unsigned long flags;

	pr_info("%s enter", __func__);

	spin_lock_irqsave(&port->port_lock, flags);
	port->suspended = true;
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static int fxfer_parse_store_cmd(const char *cmd, unsigned int *pval)
{
	int idx;
	long val = 0;
 
	for (idx = 0; idx < (sizeof(fxfer_cmd_list) / sizeof(const char *)); idx++) {
		if (!strncmp(cmd, fxfer_cmd_list[idx], strlen(fxfer_cmd_list[idx]))) {
			/* Get integer with radix base 10. */
			if (kstrtol(cmd + strlen(fxfer_cmd_list[idx]), 10, &val) < 0) {
				pr_err("%s: invalid param\n", __func__);
				return -EINVAL;
			}
			*pval = (unsigned int)val;
			return idx;
		}
	}
 
	return -EINVAL;
}
 
static void fxfer_set_buf_size(struct f_fxfer_opts *opts, unsigned int val, int tx)
{
	if (!opts || !opts->client || opts->client->port_online) {
		pr_err("%s, busy in set size\n", __func__);
		return;
	}
 
	if ((val == 0) || (val > CMD_BUF_SZ_LIMIT)) {
		pr_err("%s, sz=%u, limit=%u\n", __func__, val, CMD_BUF_SZ_LIMIT);
		return;
	}
 
	if (tx)
		opts->client->buf_tx_sz = val;
	else
		opts->client->buf_rx_sz = val;
	pr_info("%s: %s buf size is %u\n", __func__, ((tx) ? "tx" : "rx"), val);
}

/* Set parameters for an instance. */
static ssize_t f_fxfer_console_store(struct config_item *item, const char *page, size_t count)
{
	struct f_fxfer_opts *opts = to_fxfer_opts(item);
	int idx;
	unsigned int val = 0;
 
	idx = fxfer_parse_store_cmd(page, &val);
	pr_info("%s: idx=%d, val=%u\n", __func__, idx, val);

	switch (idx) {
		case CMD_TYPE_DBG :
			opts->dbg_info = val;
			pr_info("%s, dbg_level=0x%x\n", __func__, opts->dbg_info);
			break;
		case CMD_TYPE_TX_SZ :
		case CMD_TYPE_RX_SZ :
			fxfer_set_buf_size(opts, val, (idx == CMD_TYPE_TX_SZ));
			break;
		case CMD_TYPE_CLIENT_DBG :
			if (opts->client) {
				opts->client->debug = (val > 0);
				pr_info("%s: client debug=%d\n", __func__, (int)opts->client->debug);
			}
			break;
		case CMD_TYPE_PRE_REQ :
			if (opts->client) {
				if (val > 0)
					opts->client->flag |= XFER_FLAG_PRE_REQ;
				else
					opts->client->flag &= ~((unsigned int)XFER_FLAG_PRE_REQ);
			}
			break;
		case CMD_TYPE_CLASS :
			opts->cls = (unsigned char)(val & 0xFF);
			break;
		case CMD_TYPE_SUBCLASS :
			opts->subcls = (unsigned char)(val & 0xFF);
			break;
		case CMD_TYPE_PROTOCAL :
			opts->prot = (unsigned char)(val & 0xFF);
			break;
		default:
			pr_info("%s, invalid cmd %s\n", __func__, page);
			return -EINVAL;
	}

	return count;
}

static ssize_t f_fxfer_console_show(struct config_item *item, char *page)
{
	struct f_fxfer_opts *opts = to_fxfer_opts(item);
	size_t len = 0;

	if (!opts->client)
		return -ENODEV;
 
	len += scnprintf((page + len), (PAGE_SIZE - len),
		"dbg=%u, class=0x%x, subclass=0x%x, prot=0x%x\n",
		opts->dbg_info, opts->cls, opts->subcls, opts->prot);

	len += scnprintf((page + len), (PAGE_SIZE - len),
		"flag=0x%x, txsz=%u, rxsz=%u, bulk_txnum=%u, bulk_rxnum=%u\n",
		opts->client->flag, opts->client->buf_tx_sz, opts->client->buf_rx_sz,
		opts->client->buf_bulk_tx_num, opts->client->buf_bulk_rx_num);

	return len;
}

CONFIGFS_ATTR(f_fxfer_, console);

static struct configfs_attribute *f_fxfer_attrs[] = {
	&f_fxfer_attr_console,
	NULL,
};

static void f_fxfer_attr_release(struct config_item *item)
{
	struct f_fxfer_opts *opts = to_fxfer_opts(item);

	pr_info("%s enter", __func__);
	usb_put_function_instance(&opts->func_inst);
}


static struct configfs_item_operations f_fxfer_item_ops = {
	.release = f_fxfer_attr_release,
};

static const struct config_item_type f_fxfer_func_type = {
	.ct_item_ops = &f_fxfer_item_ops,
	.ct_attrs = f_fxfer_attrs,
	.ct_owner = THIS_MODULE,
};

static void fxfer_init_port(struct f_fxfer_port *port)
{
	init_waitqueue_head(&port->read_wq);
	init_waitqueue_head(&port->write_wq);
	spin_lock_init(&port->port_lock);
	INIT_WORK(&port->stop_work, fxfer_stop_sync_io_work);

	port->func.name = "fxfer";
	port->func.bind = fxfer_bind;
	port->func.set_alt = fxfer_set_alt;
	port->func.setup = fxfer_setup;
	port->func.disable = fxfer_disable;
	port->func.unbind = fxfer_unbind;
	port->func.free_func = fxfer_free_func;
	port->func.resume = fxfer_resume;
	port->func.suspend = fxfer_suspend;

	/*
	 * Let client do their own function init. Client could recover any
	 * ballback function, or set string desc for interface.
	 */
	if (port->client->fxfer_init_function)
		port->client->fxfer_init_function(port->client, &port->func);
}

/*
 * Symlink function to /config/usb_gadget/g1/configs/b.1,
 * add interface of current funciton to usb-config list.
 */
static struct usb_function *fxfer_alloc_func(struct usb_function_instance *fi)
{
	struct f_fxfer_opts *opts = container_of(fi, struct f_fxfer_opts, func_inst);
	struct fxfer_client_info *client = opts->client;

	pr_info("%s enter, name=%s\n", __func__, opts->name);

	if (!client) {
		pr_err("%s, no client\n", __func__);
		return NULL;
	}

	opts->port = kzalloc(sizeof(struct f_fxfer_port), GFP_KERNEL);
	if (!opts->port) {
		pr_err("%s alloc failed", __func__);
		return ERR_PTR(-ENOMEM);
	}

	client->context = opts->port;
	opts->port->client = client;
	opts->port->opts = opts;

	fxfer_init_port(opts->port);

	return &opts->port->func;
}

/* In this fucntion, function instance will try to get the client and init it. */
static int fxfer_set_inst_name(struct usb_function_instance *fi, const char *name)
{
	int name_len;
	struct f_fxfer_opts *opts = NULL;
	struct usb_os_desc *descs[1] = {NULL};
	char *names[1] = {NULL};
	char *ptr = NULL;

	pr_info("%s %s", __func__, name);

	name_len = (int)strlen(name) + 1;
	if (name_len > 40)  /* No more than 40 bytes */
		return -ENAMETOOLONG;

	ptr = kstrndup(name, name_len, GFP_KERNEL);
	if (ptr == NULL)
		return -ENOMEM;

	opts = container_of(fi, struct f_fxfer_opts, func_inst);
	opts->name = ptr;

	if (!strcmp(opts->name, "aoa")) {
		fxfer_aoa_init();
		opts->client = fxfer_aoa_get_info();
	} else if (!strcmp(opts->name, "hisuite")) {
		opts->client = fxfer_hisuite_get_info();
	}

	if (opts->client) {
		if (!opts->client->buf_rx_sz || (opts->client->buf_rx_sz > XFER_MAX_BUFFER_SIZE))
			opts->client->buf_rx_sz = XFER_MAX_BUFFER_SIZE;

		if (!opts->client->buf_tx_sz || (opts->client->buf_tx_sz > XFER_MAX_BUFFER_SIZE))
			opts->client->buf_tx_sz = XFER_MAX_BUFFER_SIZE;
	} else {
		kfree(opts->name);
		opts->name = NULL;
		pr_err("%s: no client", __func__);
		return -ENXIO;
	}

	if (opts->client && (opts->client->flag & XFER_FLAG_OS_DESC)) {
		descs[0] = &opts->fxfer_os_desc;
		names[0] = ptr;
		usb_os_desc_prepare_interf_dir(&opts->func_inst.group, 1, descs, names, THIS_MODULE);
	}

	return 0;
}

static void fxfer_free_instance(struct usb_function_instance *fi)
{
	struct f_fxfer_opts *opts = NULL;

	pr_info("%s enter", __func__);

	opts = container_of(fi, struct f_fxfer_opts, func_inst);

	if (opts->client) {
		if (opts->client->fxfer_destroy)
			opts->client->fxfer_destroy(opts->client);
		opts->client->port_online = false;
		opts->client->context = NULL;
	}

	if (opts->name) {
		kfree(opts->name);
		opts->name = NULL;
	}
	kfree(opts);
}

/*
 * Allocate new fucntion instance via command like
 * 'mkdir /config/usb_gadget/g1/functions/fxfer.aoa'.
 * In funtion 'fxfer_set_inst_name' will do next work.
 */
static struct usb_function_instance *fxfer_alloc_instance(void)
{
	struct f_fxfer_opts *opts = NULL;
	errno_t ret;

	pr_info("%s enter", __func__);

	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (!opts) {
		pr_err("%s alloc failed", __func__);
		return ERR_PTR(-ENOMEM);
	}

	ret = strncpy_s(opts->fxfer_ext_compat_id, FXFER_COMP_ID_LEN,
		"WINUSB", sizeof(opts->fxfer_ext_compat_id));
	if (ret != EOK)
		pr_err("%s strncpy_s ops_name failed, ret %d\n", __func__, ret);

	opts->fxfer_os_desc.ext_compat_id = opts->fxfer_ext_compat_id;
	INIT_LIST_HEAD(&opts->fxfer_os_desc.ext_prop);

	opts->func_inst.set_inst_name = fxfer_set_inst_name;
	opts->func_inst.free_func_inst = fxfer_free_instance;
	config_group_init_type_name(&opts->func_inst.group, "", &f_fxfer_func_type);

	return &opts->func_inst;
}

DECLARE_USB_FUNCTION_INIT(fxfer, fxfer_alloc_instance, fxfer_alloc_func);

