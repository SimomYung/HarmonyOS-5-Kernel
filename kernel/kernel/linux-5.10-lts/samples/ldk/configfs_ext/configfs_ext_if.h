/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __DEVHOST_CONFIGFS_EXT_IF_H__
#define __DEVHOST_CONFIGFS_EXT_IF_H__

int devhost_register_configfs_ext(void);

/* hmdfs config */
int get_bid(const char *bname);
int __init hmdfs_init_configfs(void);
void hmdfs_exit_configfs(void);

/* sharefs config */
int get_bid_config(const char *bname);
int __init sharefs_init_configfs(void);
void sharefs_exit_configfs(void);

#endif
