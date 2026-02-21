# SPDX-License-Identifier: GPL-2.0-only

obj-y += firmware.o
CFLAGS_firmware.o += -I$(srctree)/drivers/base/firmware_loader/ \
		     -I$(srctree)/samples/ldk/
obj-y += unimpl.o

obj-$(CONFIG_USB_GADGET) += usb_gadget.o
obj-y += liblinux_vendor_hooks.o
obj-y += tty/
obj-y += char/
obj-y += base/
