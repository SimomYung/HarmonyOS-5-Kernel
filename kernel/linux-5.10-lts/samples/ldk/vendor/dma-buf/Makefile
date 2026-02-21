# SPDX-License-Identifier: GPL-2.0-only

obj-y += page_pool.o
CFLAGS_page_pool.o += -I$(srctree)/drivers/dma-buf/heaps/ \
			-I$(srctree)/platform_source/basicplatform/drivers/dma-buf/heaps/ \
			-I$(srctree)/samples/ldk/

obj-$(CONFIG_DMABUF_HEAPS_CPA) += mm_cpa_heap.o
