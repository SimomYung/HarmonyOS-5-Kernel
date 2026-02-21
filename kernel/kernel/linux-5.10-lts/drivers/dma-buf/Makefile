# SPDX-License-Identifier: GPL-2.0-only
ccflags-y 	+= -I$(srctree)/platform_source/basicplatform/drivers/dma-buf
ccflags-y 	+= -I$(srctree)/drivers/dma-buf/heaps
ccflags-y	+= -I$(srctree)/platform_source/basicplatform/drivers/secmem
ccflags-y	+= -I$(srctree)/samples/ldk/vendor
obj-y := dma-buf.o dma-fence.o dma-fence-array.o dma-fence-chain.o \
	 dma-resv.o seqno-fence.o dma-buf-trace.o
obj-$(CONFIG_DMABUF_HEAPS)	+= dma-heap.o
obj-$(CONFIG_DMABUF_UDK_EN)	+= dma-buf-udk-pal.o
obj-$(CONFIG_DMABUF_HEAPS)	+= heaps/
obj-$(CONFIG_SYNC_FILE)		+= sync_file.o
obj-$(CONFIG_SW_SYNC)		+= sw_sync.o sync_debug.o
obj-$(CONFIG_UDMABUF)		+= udmabuf.o
obj-$(CONFIG_DMABUF_SYSFS_STATS) += dma-buf-sysfs-stats.o
obj-$(CONFIG_DMABUF_EXT)	+= dma-buf-ext-pal.o

dmabuf_selftests-y := \
	selftest.o \
	st-dma-fence.o \
	st-dma-fence-chain.o

obj-$(CONFIG_DMABUF_SELFTESTS)	+= dmabuf_selftests.o

obj-$(CONFIG_DMABUF_PROCESS_INFO)	+= dma-buf-process-info.o
