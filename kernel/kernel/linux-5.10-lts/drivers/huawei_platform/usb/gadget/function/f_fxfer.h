/*
 * f_fxfer.h
 *
 * Header file of common function driver of fxfer
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _F_FXFER_H
#define _F_FXFER_H

#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>

#define XFER_FLAG_IO_SYNC   (unsigned int)(1 << 0)
#define XFER_FLAG_OS_DESC   (unsigned int)(1 << 1)
#define XFER_FLAG_ENABLED   (unsigned int)(1 << 2)
#define XFER_DEP_IN         (unsigned int)(1 << 3)
#define XFER_DEP_OUT        (unsigned int)(1 << 4)
#define XFER_DEP_INTR       (unsigned int)(1 << 5)
#define XFER_FLAG_IO_BULK   (unsigned int)(1 << 6)
#define XFER_FLAG_PRE_REQ   (unsigned int)(1 << 7)

#define NOT_NULL(v)         ((v) ? 1 : 0)
#define MIN_VAL(a, b)       (((a) < (b)) ? (a) : (b))
#define MAX_VAL(a, b)       (((a) > (b)) ? (a) : (b))

#define client_info(client, fmt, args...) \
	if (unlikely((client)->debug)) \
		pr_info(fmt, ## args)

struct fxfer_io_req {
	unsigned char *buffer;
	unsigned int offset;
	unsigned int len;
	bool tx;
	char *client_data;
	char *port_data;
};

/* Context of fxfer client driver */
struct fxfer_client_info {
	const char *name;
	unsigned int flag;
	bool debug;
	bool client_online;  /* client opened */
	bool port_online;    /* usb connected */
	int open_count;

	unsigned int buf_tx_sz;  /* Sync write buffer size */
	unsigned int buf_rx_sz;  /* Sync read buffer size */

	unsigned int buf_bulk_tx_num;
	unsigned int buf_bulk_rx_num;

	/* Callback function of client called by fxfer port */
	/* Bind function */
	void (*fxfer_init_function)(struct fxfer_client_info *client, struct usb_function *func);
	/* Setup function */
	int (*fxfer_ctrl_req)(struct fxfer_client_info *client,
		struct usb_function *f, const struct usb_ctrlrequest *ctrl);
	/* Unbind function */
	void (*fxfer_clear)(struct fxfer_client_info *client);
	/* Remove function instance */
	void (*fxfer_destroy)(struct fxfer_client_info *client);
	/* Connect to PC, start IO transfer. */
	void (*fxfer_connect)(struct fxfer_client_info *client);
	void (*fxfer_io_finish)(struct fxfer_client_info *client, struct fxfer_io_req *req, int result);

	void *context;      /* Pointer to f_fxfer_port */
	void *private;      /* Pointer to private context of client */
};

struct fxfer_client_info *fxfer_aoa_get_info(void);
void fxfer_aoa_init(void);

struct fxfer_client_info *fxfer_hisuite_get_info(void);

int fxfer_sync_write(struct fxfer_client_info *client, const char *buf, int sz);
int fxfer_sync_read(struct fxfer_client_info *client, char *buf, int sz);
void fxfer_sync_io_cancel(struct fxfer_client_info *client);

int fxfer_bulk_io_request(struct fxfer_client_info *client, struct fxfer_io_req *io_req);
int fxfer_bulk_io_allocreq(struct fxfer_client_info *client, struct fxfer_io_req *io_req);
int fxfer_bulk_io_freereq(struct fxfer_client_info *client, struct fxfer_io_req *io_req);
void fxfer_bulk_io_disable(struct fxfer_client_info *client);
void fxfer_bulk_io_enable(struct fxfer_client_info *client);

int fxfer_send_notify(struct fxfer_client_info *client, const char *buf, int sz);

#endif

