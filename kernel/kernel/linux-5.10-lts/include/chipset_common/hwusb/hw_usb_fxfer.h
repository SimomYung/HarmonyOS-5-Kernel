/*
 * hw_usb_fxfer.h
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

#ifndef _HW_USB_FXFER_H
#define _HW_USB_FXFER_H

#include <linux/usb/ch9.h>
#include <linux/usb/composite.h>

int fxfer_aoa_ctrlrequest(struct usb_composite_dev *cdev,
	const struct usb_ctrlrequest *ctrl, bool complete);
void fxfer_aoa_disconnect(void);

#endif

