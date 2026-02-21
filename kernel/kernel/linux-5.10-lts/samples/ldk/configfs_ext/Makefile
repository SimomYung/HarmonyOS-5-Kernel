# SPDX-License-Identifier: GPL-2.0

obj-y += configfs_ext.o config_hmdfs.o config_sharefs.o configfs_ext_if.o
CFLAGS_configfs_ext_if.o += $(CFLAGS_PAC)

USER_OBJS := configfs_ext_if.o
include arch/liblinux/scripts/Makefile.rules
