/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Header file or chip_usb_dubug.c.
 * Create: 2022-6-16
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#ifndef _HIUSB_USB_DEBUG_H_
#define _HIUSB_USB_DEBUG_H_

#include <linux/platform_drivers/usb/chip_usb_debugfs_common.h>
#include "core.h"

struct hiusb_port_slt_mode {
	struct dentry *debugfs_root;
	struct notifier_block host_nb;
	u8 port_count;
	u8 port_id[HIUSB_PORT_MAXSIZE];
	u8 host_slt_result[HIUSB_PORT_MAXSIZE];
	u8 hifi_slt_result[HIUSB_PORT_MAXSIZE];
	u8 slt_test_flag;
};

#if defined(CONFIG_PLUGUSB_DBGFS_ROOT) || defined(CONFIG_DFX_DEBUG_FS)
int hiusb_port_create_attr_file(struct hiusb_port *port);
void hiusb_port_remove_attr_file(struct hiusb_port *port);
#else
static inline int hiusb_port_create_attr_file(struct hiusb_port *port)
{
	return 0;
}
static inline void hiusb_port_remove_attr_file(struct hiusb_port *port) {}
#endif

#ifdef CONFIG_DFX_DEBUG_FS
int hiusb_core_debugfs_init (struct device *dev);
void hiusb_core_debugfs_exit(void);
#else
static inline int hiusb_core_debugfs_init (struct device *dev)
{
	return 0;
}
static inline void hiusb_core_debugfs_exit(void) {}
#endif
#endif /* _HIUSB_USB_DEBUG_H_ */

