ccflags-y += -I$(srctree)/platform_source/basicplatform/drivers/secmem
obj-$(CONFIG_DMABUF_HEAPS) += heaps/
obj-$(CONFIG_DMABUF_MM) += mm/
obj-$(CONFIG_DMABUF_RECLAIM) += dmabuf_reclaim/
obj-$(CONFIG_DMABUF_RECLAIM) += dmabuf_reclaim_dev/
obj-$(CONFIG_DMABUF_SWAP) += dmabuf_reclaim/

obj-$(CONFIG_AI_IN_FLASH) += pa_offset/

ifeq ($(CONFIG_LIBLINUX),y)
obj-y += page_pool.o
CFLAGS_page_pool.o += -I$(srctree)/drivers/dma-buf/heaps/ \
			-I$(srctree)/platform_source/basicplatform/drivers/dma-buf/heaps/ \
			-I$(srctree)/samples/ldk/
obj-y += dma_heap_hisi.o
CFLAGS_dma_heap_hisi.o += -I$(srctree)/platform_source/basicplatform/drivers/dma-buf/heaps/ \
						  -I$(srctree)/drivers/dma-buf/heaps

obj-$(CONFIG_DMABUF_HEAPS_CPA) += mm_cpa_heap.o
endif