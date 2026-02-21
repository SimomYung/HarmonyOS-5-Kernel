# SPDX-License-Identifier: GPL-2.0
ccflags-y 	+= -I$(srctree)/platform_source/basicplatform/drivers/dma-buf/heaps
obj-y		+= deferred-free-helper.o page_pool.o
obj-$(CONFIG_LIBLINUX_CDC)	+= system_heap.o heap-helpers.o