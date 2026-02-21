# SPDX-License-Identifier: GPL-2.0
ccflags-$(CONFIG_DMABUF_MM)          += -I$(srctree)/platform_source/basicplatform/drivers/secmem
obj-$(CONFIG_HAS_DMA)			+= mapping.o direct.o
obj-$(CONFIG_DMA_OPS)			+= ops_helpers.o
obj-$(CONFIG_DMA_OPS)			+= dummy.o
obj-$(CONFIG_DMA_CMA)			+= contiguous.o
obj-$(CONFIG_DMA_DECLARE_COHERENT)	+= coherent.o
obj-$(CONFIG_DMA_VIRT_OPS)		+= virt.o
obj-$(CONFIG_DMA_API_DEBUG)		+= debug.o
obj-$(CONFIG_SWIOTLB)			+= swiotlb.o
obj-$(CONFIG_DMA_COHERENT_POOL)		+= pool.o
obj-$(CONFIG_DMA_REMAP)			+= remap.o
