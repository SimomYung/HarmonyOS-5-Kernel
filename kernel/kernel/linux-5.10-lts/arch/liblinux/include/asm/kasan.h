/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_KASAN_H
#define __ASM_KASAN_H

#ifndef __ASSEMBLY__

#include <linux/linkage.h>
#include <asm/memory.h>
#include <asm/pgtable-types.h>

#ifdef CONFIG_KASAN
#ifdef CONFIG_KASAN_SW_TAGS
#define KASAN_SHADOW_SCALE_SHIFT 4
#else /* !CONFIG_KASAN_SW_TAGS */
#define KASAN_SHADOW_SCALE_SHIFT 3
#endif /* CONFIG_KASAN_SW_TAGS */

#define KASAN_SHADOW_START  (KASAN_SHADOW_OFFSET + (PAGE_OFFSET >> KASAN_SHADOW_SCALE_SHIFT))
#define KASAN_SHADOW_SIZE  PAGE_ALIGN(LIBLINUX_KMAP_SIZE >> KASAN_SHADOW_SCALE_SHIFT)
#define KASAN_SHADOW_END (KASAN_SHADOW_START + KASAN_SHADOW_SIZE)

void kasan_init(void);
void kasan_poison_shadow(const void *address, size_t size, u8 value);
#define KASAN_FREE_PAGE         0xFF  /* page was freed */

#else /* !CONFIG_KASAN */
static inline void kasan_init(void) { }
static inline void kasan_poison_shadow(const void *address, size_t size, u8 value) { }

#endif /* CONFIG_KASAN */
#endif /* !__ASSEMBLY__ */
#endif /* __ASM_KASAN_H */
