# SPDX-License-Identifier: GPL-2.0

obj-y += shm_trans.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_XPM) += debugfs_xpm.o hmxpm.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_BINDER) += debugfs_binder.o hmbinder_info.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_COMPACTION) += debugfs_extfrag.o hmextfrag.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_EXT) += debugfs_ext.o debugfs_ext_if.o

ifeq ($(CONFIG_LIBLINUX_DEBUGFS_BINDER), y)
CFLAGS_debugfs_ext_if.o += -DCONFIG_LIBLINUX_DEBUGFS_BINDER
endif

USER_OBJS := hmbinder_info.o shm_trans.o hmxpm.o hmextfrag.o debugfs_ext_if.o
include arch/liblinux/scripts/Makefile.rules
