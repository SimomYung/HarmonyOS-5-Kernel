# SPDX-License-Identifier: GPL-2.0
obj-y += klog_dump.o
obj-y += track_info.o dump_track_info.o
obj-y += kbox_decompress.o
obj-$(CONFIG_LOG_ENCRYPT) += elog_metadata.o
CFLAGS_klog_dump.o += -include $(srctree)/include/linux/export.h
CFLAGS_klog_dump.o += ${CFLAGS_PAC}
CFLAGS_dump_track_info.o += -I$(srctree)/platform_source/basicplatform/drivers/mntn
ccflags-y += -Wno-pointer-sign -std=gnu99

CFLAGS_track_info.o += ${CFLAGS_PAC}
CFLAGS_kbox_decompress.o += ${CFLAGS_PAC}
CFLAGS_elog_metadata.o += ${CFLAGS_PAC}
# mark all non-linux objects as USER_OBJS to remove kernel CFLAGS
USER_OBJS := klog_dump.o track_info.o kbox_decompress.o elog_metadata.o
include arch/liblinux/scripts/Makefile.rules
