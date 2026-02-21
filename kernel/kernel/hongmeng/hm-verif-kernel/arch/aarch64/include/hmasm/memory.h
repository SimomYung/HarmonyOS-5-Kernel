/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef AARCH64_ASM_MEMORY_H
#define AARCH64_ASM_MEMORY_H

#include <hmkernel/const.h>
#include <hmasm/privilege_layout.h>

/* Virtual address layout
 *
 # If not config KASLR
 * VA_START  VA_KERNEL_START
 * |         |
 * V         V
 * |<-----------------------------vaddr-space(1/2)-----------------------------------------------------------
 * |<- 1/4 ->|<- TEXT_OFFSET ->|<- KERNEL TEXT ->|<- RODATA ->|<- DATA ->|<- LIVEPATCH MEM ->|<- FREE MEM ->|
 *           |<---------------- 1:1 mapping -----(main bank memory)---------------------------------------->|
 *
 *                                                                                     VA_KERNEL_VSPACE_END
 *                                                                                               |
 *                                                                                               V
 * |<-----------------------------vaddr-space(2/2)--------------------------------------------------------->|
 * |<- kdev ->|<- pages ->|<- kmap ->|<- kstack ->|<- ctrlmem ->|<- dev_boot ->|<- FREE MEM ->|  |<-ERRPTR->|
 *
 * If config KASLR, ALL area will be random in [ VA_KERNEL_START,  VA_KERNEL_VSPACE_END ] area
 *
 * If config SPIN_BASIC
 * VA_START  VA_KERNEL_START                                                                    VA_KERNEL_VSPACE_END
 * |         |                                                                                                  |
 * V         V                                                                                                  V
 * |<---------------------------------vaddr-space-------------------------------------------------------------------|
 * |<- 1/4 ->| |<- FIRST_REGION --->| |<--------------------- SECOND_REGION --------------->|
 *             |<-kernel->|<-idmap->| |<-kdev->|<-pages->|<-kmap->|<-kstack->|<-ctrlmem->|    |<-dev_boot->|
 *
 * If config KDP
  * VA_START  VA_KERNEL_START                                                                   VA_KERNEL_VSPACE_END
 * |         |                                                                                                  |
 * V         V                                                                                                  V
 * |<---------------------------------vaddr-space-------------------------------------------------------------------|
 * |<- 1/4 ->| |<------------------------------------- KDP ----------------------------->|
 *             |<-kernel->| |<-kdev->|<-pages->|<-kmap->|<-kstack->|<-ctrlmem->|<-----idmap---->| |<-dev_boot->|
 *
 * The PAGES Area is reserved for `struct page_s` array. Currently it is
 * (1UL << (VA_BITS - 6)) and the size of `struct page_s` is 4 bytes, so the max continuous physical
 * memory range it can cover is ((1UL << (VA_BITS - 6)) / 4B * PAGE_SIZE).
 * Assume that VA_BITS is 39 and PAGE_SIZE is 4K, then the physical
 * address range is [0, 8T].
 *
 * KSTACK Area is the space available for KSTACK.
 *
 * KMAP Area is the space available for KMAP.
 *
 * Driver Area is the space available for kernel driver.
 *
 * kernel image area is the space available for kernel image.
 *
 * ID MAP AREA is the space available for idmap.
 *
 * LIVEPATCH MEM area is reserved for livepatch, it would be none if CONFIG_KERNEL_LIVPATCH is n.
 */

#define VA_BITS CONFIG_AARCH64_VA_BITS
#define VA_START ((__UL(0xffffffffffffffff) - (__UL(1) << VA_BITS)) + __UL(1))

#define TEXT_OFFSET CONFIG_AARCH64_TEXT_OFFSET
/* This is the largest mapable address in the kernel address space. Last 16k forbid memory mapping */
#define VA_KERNEL_VSPACE_END	((UL(0xffffffffffffffff) - (UL(1) << 14)) + UL(1))

/* memory layout in order */
#define VA_FRONT_START					(VA_START + (UL(1) << (VA_BITS - 2)))

#define VA_KERNEL_START					VA_FRONT_START

#define VA_KDP_SIZE	UL(0x80000000)
#define KDP_ALLOC_SIZE	(VA_KDP_SIZE + (UL(1) << (VA_BITS - 3)) + VA_ID_MAP_ALIGN)

#define VA_IREGION_SIZE					UL(0x80000000)
#ifdef CONFIG_SPIN_BASIC
#ifndef __ASSEMBLY__
#define VA_VREGION_SIZE 				kaslr_area_info[KASLR_AREA_VREGION].size
#endif
#endif

#ifndef __ASSEMBLY__
#define VA_KERNEL_IMG_SIZE				kaslr_area_info[KASLR_AREA_KERNEL_IMG].size
/* VA_ID_MAP_SIZE is still used after initialization */
#ifdef CONFIG_KDP
#define VA_ID_MAP_SIZE					(UL(1) << (VA_BITS - 3))
#elif (!defined(CONFIG_KDP)) && (defined(CONFIG_SPIN_BASIC))
#define VA_ID_MAP_SIZE					(UL(0x60000000) - VA_ID_MAP_ALIGN)
#else
#define VA_ID_MAP_SIZE					(UL(1) << (VA_BITS - 3))
#endif
#define VA_KDEV_SIZE					kaslr_area_info[KASLR_AREA_KDEV].size
#define VA_PAGES_SIZE					kaslr_area_info[KASLR_AREA_PAGES].size
#define VA_KMAP_SIZE					kaslr_area_info[KASLR_AREA_KMAP].size
#define VA_KSTACK_SIZE					(KERNEL_STACK_REGION_SIZE * (unsigned long)CONFIG_NR_CPUS)
#define VA_CTRLMEM_SIZE					kaslr_area_info[KASLR_AREA_CTRLMEM].size
#define VA_DEV_BOOT_SIZE				kaslr_area_info[KASLR_AREA_DEV_BOOT].size
#endif

#define VA_ALIGN_4K					(UL(1) << 12)
#define VA_ALIGN_2M					(UL(1) << 21)
#define VA_ALIGN_1G					(UL(1) << 30)
#define VA_ALIGN_2G					(UL(1) << 31)

#define VA_KDP_ALIGN					0x80000000

#define VA_IREGION_ALIGN				VA_ALIGN_2M
#ifdef CONFIG_SPIN_BASIC
#ifndef __ASSEMBLY__
#define VA_VREGION_ALIGN 				kaslr_area_info[KASLR_AREA_VREGION].align
#endif
#endif

/* The VA_KERNEL_IMG_ALIGN definition should not use global variables since it is used before kernel relocation. */
#if (defined(CONFIG_KDP)) || (defined(CONFIG_SPIN_BASIC))
#define VA_KERNEL_IMG_ALIGN				VA_ALIGN_2M
#else
#define VA_KERNEL_IMG_ALIGN				VA_ALIGN_1G
#endif

#ifndef __ASSEMBLY__
/* VA_ID_MAP_ALIGN is still used after initialization */
#define VA_ID_MAP_ALIGN					VA_ALIGN_2M
#define VA_KDEV_ALIGN					kaslr_area_info[KASLR_AREA_KDEV].align
#define VA_PAGES_ALIGN					kaslr_area_info[KASLR_AREA_PAGES].align
#define VA_KMAP_ALIGN					kaslr_area_info[KASLR_AREA_KMAP].align
#define VA_KSTACK_ALIGN					KERNEL_STACK_REGION_SIZE
#define VA_CTRLMEM_ALIGN				kaslr_area_info[KASLR_AREA_CTRLMEM].align
#define VA_DEV_BOOT_ALIGN				kaslr_area_info[KASLR_AREA_DEV_BOOT].align
#endif

#define VA_KERNEL_IMG_START_NO_PIE  	(VA_KERNEL_START + UL(TEXT_OFFSET))

#ifndef __ASSEMBLY__
#define VA_KDP_START					kaslr_addr_of_kdp()
#define VA_IREGION_START				kaslr_addr_of_iregion()
#define VA_VREGION_START				kaslr_addr_of_vregion()
#define VA_KERNEL_IMG_START				kaslr_addr_of_kernel()
#define VA_ID_MAP_START					kaslr_addr_of_idmap()
#define VA_DEV_KDEV_START				kaslr_addr_of_kdev()
#define VA_PAGES_START					kaslr_addr_of_pages()
#define VA_KMAP_START					kaslr_addr_of_kmap()
#define VA_KSTACK_START					kaslr_addr_of_kstack()
#define VA_CTRLMEM_START				kaslr_addr_of_ctrlmem()
#define VA_DEV_BOOT_START				kaslr_addr_of_dev_boot()

#define VA_KDP_END					get_kaslr_addr_end_of_kdp()
#define VA_IREGION_END					get_kaslr_addr_end_of_iregion()
#define VA_VREGION_END					get_kaslr_addr_end_of_vregion()
#define VA_KERNEL_IMG_END				get_kaslr_addr_end_of_kernel_image()
#define VA_ID_MAP_END					get_kaslr_addr_end_of_idmap()
#define VA_DEV_KDEV_END					get_kaslr_addr_end_of_kdev()
#define VA_PAGES_END					get_kaslr_addr_end_of_pages()
#define VA_KMAP_END					get_kaslr_addr_end_of_kmap()
#define VA_KSTACK_END					get_kaslr_addr_end_of_kstack()
#define VA_CTRLMEM_END					get_kaslr_addr_end_of_ctrlmem()
#define VA_DEV_BOOT_END					get_kaslr_addr_end_of_dev_boot()

#endif  /* end of __ASSEMBLY__ */

#define KASLR_INVALID_ADDR				(-1UL)

#define LOWVISOR_BASE		CONFIG_KERNEL_BASE_ADDR

#define CPU_LOCAL_SHIFT	12UL
#define CPU_LOCAL_SIZE	4096

/* See kernel/include/hmkernel/mm/kstack.h */
#define KERNEL_STACK_REGION_SIZE	(4096UL * 32UL)

#ifdef CONFIG_PIE
#define KERNEL_TEXT_START	0
#else
#define KERNEL_TEXT_START	VA_KERNEL_IMG_START_NO_PIE
#endif

#ifndef __ASSEMBLY__
#include <hmkernel/types.h>
#include <mapi/hmasm/memory.h>
#include <hmkernel/bitops/bitops.h>
#include <hmkernel/mm/klinear/idmap.h>
#include <hmkernel/mm/kaslr_layout.h>
#include <lib/utils.h>

/*
 * type of VA and PA:
 * PA is u64
 * VA is unsigned long (compatible with (void *))
 */
extern u64 pv_offset_of_image(void);
extern void pv_offset_img_init(u64 pv_offset);
extern const unsigned long *priv_vs_size(void);
#define __pa_img(v)	((paddr_t)((u64)ptr_to_ulong(v) - pv_offset_of_image()))
#define __va_img(p)	(ulong_to_ptr((unsigned long)(p) + pv_offset_of_image(), void))

/*
 * Since we use 1G mapping, we have much more free memory
 * than 256M. This is not a hard limitation.
 *
 * On some platforms (Hi1951/Kirin980) the DTB is loaded to a place close to the
 * boot mem area. Having too much BOOT_MEM_SIZE will be overlap with the DTB
 * region and corrupt DTB. For those platforms the bootmem is set to be 32MB.
 *
 * While on some other platforms (Hi1620/Hi1910) there is a far away memory bank
 * which needs larger boot mem to allocate the pagetable. Their bootmem is set to
 * be 256MB.
 *
 * NOTE: The final solution shall be arranging boot elements in an elegant way instead of
 * poking the memory range around.
 */
#define BOOT_MEM_SIZE (U(CONFIG_BOOT_MEM_SIZE) << 20)

#ifdef CONFIG_KDP
extern uptr_t kaslr_addr_of_kdp(void);
extern uptr_t __kaslr_kdp_start;
#endif

#ifdef CONFIG_KDP_DATA_INVISIBLE
extern unsigned long __data_invisible_kdp_start[];
extern unsigned long __data_invisible_kdp_end[];
#endif

#ifdef CONFIG_SPIN_BASIC
extern uptr_t kaslr_addr_of_iregion(void);
extern uptr_t kaslr_addr_of_vregion(void);
#endif
extern uptr_t kaslr_addr_of_kernel(void);
extern uptr_t kaslr_addr_of_kstack(void);
extern uptr_t kaslr_addr_of_ctrlmem(void);
extern uptr_t kaslr_addr_of_kmap(void);
extern uptr_t kaslr_addr_of_kdev(void);
extern uptr_t kaslr_addr_of_pages(void);
extern uptr_t kaslr_addr_of_idmap(void);
extern uptr_t kaslr_addr_of_dev_boot(void);

#ifdef CONFIG_KDP
extern uptr_t get_kaslr_addr_end_of_kdp(void);
#endif
#ifdef CONFIG_SPIN_BASIC
extern uptr_t get_kaslr_addr_end_of_iregion(void);
extern uptr_t get_kaslr_addr_end_of_vregion(void);
#endif
extern uptr_t get_kaslr_addr_end_of_kernel_image(void);
extern uptr_t get_kaslr_addr_end_of_kstack(void);
extern uptr_t get_kaslr_addr_end_of_ctrlmem(void);
extern uptr_t get_kaslr_addr_end_of_kmap(void);
extern uptr_t get_kaslr_addr_end_of_kdev(void);
extern uptr_t get_kaslr_addr_end_of_pages(void);
extern uptr_t get_kaslr_addr_end_of_idmap(void);
extern uptr_t get_kaslr_addr_end_of_dev_boot(void);

#ifdef CONFIG_KASLR
#define __pa_idmap(v) ({ \
	void *____v = (void *)(v); \
	((paddr_t)((u64)ptr_to_ulong(____v) - klinear_idmap_pv_offset_for_pa((void *)(____v)))); \
})

/*
 * __va_idmap should return error through PTR_TO_ERR when we support highmem where we are
 * not sure whether a pa has its direct mapping in kernel space. See
 * kernel/ctrlmem/pgstrtbl/pgstr.c where we use IS_PTR_ERR to decode error from __va_idmap.
 * and kernel/ctrlmem/kobjctnr/kobjctnr.c
 */
#define __va_idmap(p) ({ \
	paddr_t ____p = (paddr_t)(p); \
	(ulong_to_ptr((unsigned long)(____p) + klinear_idmap_pv_offset_for_va((paddr_t)(____p)), void)); \
})

#else
#define __pa_idmap(v)		__pa_img(v)
#define __va_idmap(p)		__va_img(p)
#endif

#define __pa_sysproc(v) \
	((paddr_t)((u64)ptr_to_ulong(v) - pv_offset_of_sysproc())) \

#define __va_sysproc(p) \
	((vaddr_t)(uptr_t)((unsigned long)(p) + pv_offset_of_sysproc())) \

typedef u64 pfn_t;

#define PRIxvaddr __PRIxvaddr
#define PRIxpaddr __PRIxpaddr

#define PADDR_MAX	((paddr_t)(-1L))
#define PFN_MAX		((pfn_t)(-1L))

#ifndef CONFIG_NO_NEED_PHYSMEM_MAX_BITS
#define PHYSMEM_MAX_BITS	CONFIG_ARCH_PA_BITS
#endif

/* See arch/aarch64/include/asm/memory.h for more information. */
#define MSEG_SEGMENT_SIZE_BITS	26U

extern int __cpu_local_start[];
extern int __stack_bottom[];
extern int __stack_top[];
extern int __exception_stack_top[];
#endif

#endif
