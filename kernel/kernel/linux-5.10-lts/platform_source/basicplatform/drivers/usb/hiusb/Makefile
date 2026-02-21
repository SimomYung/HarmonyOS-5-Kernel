# SPDX-License-Identifier: GPL-2.0
ccflags-y += -Wextra  -Wdate-time -Wfloat-equal -Wshadow -Wformat=2
obj-$(CONFIG_USB_HIUSB)		+= hiusb.o
hiusb-y		+= core.o port.o event.o client.o phy.o test_pd.o stub.o hifi.o bc12.o notify.o wrapper.o class.o

ifneq ($(CONFIG_PLUGUSB_DBGFS_ROOT)$(CONFIG_DFX_DEBUG_FS),)
hiusb-y		+= debugfs.o
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common/lowpm/
ifeq ($(CONFIG_FMEA_FAULT_INJECTION), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common/
endif
