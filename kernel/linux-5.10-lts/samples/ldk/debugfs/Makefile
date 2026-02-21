# SPDX-License-Identifier: GPL-2.0

obj-y += shm_trans.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_XPM) += debugfs_xpm.o hmxpm.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_BINDER) += debugfs_binder.o hmbinder_info.o
obj-$(CONFIG_LIBLINUX_DEBUGFS_COMPACTION) += debugfs_extfrag.o hmextfrag.o

USER_OBJS := hmbinder_info.o shm_trans.o hmxpm.o hmextfrag.o
include arch/liblinux/scripts/Makefile.rules
