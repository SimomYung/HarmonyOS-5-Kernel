# SPDX-License-Identifier: GPL-2.0
obj-y += tracefs.o
obj-y += tracefs_proxy.o
obj-y += tracefs_rmqk.o
obj-y += tracefs_helper.o
obj-y += tracefs_symbols.o
ccflags-y += -Wno-pointer-sign -std=gnu99

CFLAGS_tracefs_proxy.o += ${CFLAGS_PAC}
CFLAGS_tracefs_rmqk.o += ${CFLAGS_PAC}
CFLAGS_tracefs_helper.o += ${CFLAGS_PAC}
USER_OBJS := tracefs_proxy.o tracefs_rmqk.o tracefs_helper.o
include arch/liblinux/scripts/Makefile.rules
