// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <linux/export.h>
#include <linux/dma-direction.h>
#include <linux/dma-map-ops.h>
#include <linux/device.h>
#include <linux/device/driver.h>
#include <linux/module.h>
#include <asm/cacheflush.h>

#include <liblinux/pal.h>
#ifdef CONFIG_MM_LB
#include <linux/mm_lb/mm_lb.h>
#endif
#include <trace/hooks/liblinux.h>

void __flush_icache_range(unsigned long start, unsigned long end)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_ICACHE_FLUSH_RANGE, start, end);
}

void v7_coherent_kern_range(unsigned long start, unsigned long end)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_ICACHE_FLUSH_RANGE, start, end);
}

void v7_flush_kern_dcache_area(void *addr, size_t len)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_FLUSH_RANGE,
				       (unsigned long)addr,
				       (unsigned long)addr + len);
}
EXPORT_SYMBOL(v7_flush_kern_dcache_area);

void __flush_dcache_area(void *addr, size_t len)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_FLUSH_RANGE,
				       (unsigned long)addr,
				       (unsigned long)addr + len);
}
#ifdef CONFIG_ARM64
EXPORT_SYMBOL(__flush_dcache_area);
#endif

void __clean_dcache_area_pou(void *addr, size_t len)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_CLEAN_RANGE,
				       (unsigned long)addr,
				       (unsigned long)addr + len);
}

void __dma_map_area(const void *addr, size_t len, int dir)
{
	unsigned int cmd;

	if (dir == DMA_FROM_DEVICE) {
		cmd = LIBLINUX_PAL_DCACHE_INVAL_RANGE;
	} else {
		cmd = LIBLINUX_PAL_DCACHE_CLEAN_RANGE;
	}

	liblinux_pal_flush_cache_range(cmd,
				       (unsigned long)addr,
				       (unsigned long)addr + len);
}

void __dma_unmap_area(const void *addr, size_t len, int dir)
{
	if (dir == DMA_TO_DEVICE) {
		return;
	}
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_INVAL_RANGE,
				       (unsigned long)addr,
				       (unsigned long)addr + len);
}

#ifndef CONFIG_ARM64
void __dma_flush_area(const void *start, size_t size)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_FLUSH_RANGE,
				       (unsigned long)(uintptr_t)start,
				       (unsigned long)((uintptr_t)start + size));
}
#endif

#ifdef CONFIG_ARM64
void flush_cache_all(void)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_CACHE_FLUSH_ALL, 0, (unsigned long)-1);
}
EXPORT_SYMBOL(flush_cache_all);

void __inval_dcache_area(void *addr, size_t len)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_INVAL_RANGE,
				       (uintptr_t)addr,
				       (uintptr_t)addr + len);
}
#endif

#ifdef CONFIG_LIBLINUX_USER_INVAL_CACHE
void liblinux_sync_dma_for_device(struct device *dev, phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	if (dir == DMA_FROM_DEVICE && dev->user_inval_cache)
		__dma_flush_area(phys_to_virt(paddr), size);
	else
		arch_sync_dma_for_device(paddr, size, dir);
}

void liblinux_sync_dma_for_cpu(struct device *dev, phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	if (dir == DMA_FROM_DEVICE && dev->user_inval_cache)
		__dma_flush_area(phys_to_virt(paddr), size);
	else
		arch_sync_dma_for_cpu(paddr, size, dir);
}

void liblinux_dma_map_area(struct device *dev, const void *addr, size_t size,
		enum dma_data_direction dir)
{
	if (dir == DMA_FROM_DEVICE && dev->user_inval_cache)
		__dma_flush_area(addr, size);
	else
		__dma_map_area(addr, size, dir);
}

static bool is_user_inval_cache_driver(const char *name)
{
	static const char * const driver[] = {"dra", "rnic", "hi110x"};
	unsigned int i;

	if (name == NULL)
		return false;

	for (i = 0; i < ARRAY_SIZE(driver); i++) {
		if (strncmp(name, driver[i], strlen(driver[i])) == 0)
			return true;
	}

	return false;
}
#endif

static void hook_dev_set_user_inval_cache(void *args, struct device *dev)
{
#ifdef CONFIG_LIBLINUX_USER_INVAL_CACHE
	if (is_user_inval_cache_driver(dev->driver->name))
		dev->user_inval_cache = true;
	else
		dev->user_inval_cache = false;
#endif
}
INIT_VENDOR_HOOK(ldk_vh_dev_set_user_inval_cache, hook_dev_set_user_inval_cache);
