# SPDX-License-Identifier: GPL-2.0

ccflags-y += -I$(srctree)/drivers/tty/serial
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/common/lowpm

obj-$(CONFIG_SERIAL_UART_V500) += uart_v500.o uart_v500_console.o
obj-$(CONFIG_SERIAL_HI9030_UART) += uart_v500_hi9030.o