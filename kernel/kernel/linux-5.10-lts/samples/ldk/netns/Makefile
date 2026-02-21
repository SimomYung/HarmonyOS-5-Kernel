# SPDX-License-Identifier: GPL-2.0
obj-y += netns.o
obj-y += net_procfs.o
obj-y += procfs_ops.o
CFLAGS_net_procfs.o += ${CFLAGS_PAC}

USER_OBJS := net_procfs.o
include arch/liblinux/scripts/Makefile.rules
