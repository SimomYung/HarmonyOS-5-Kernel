/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_MM_INTERNAL_H__
#define __LIBLINUX_MM_INTERNAL_H__

#include <asm/page.h>
#ifdef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
#include <linux/shrinker.h>

void shrink_zones(struct zonelist *zonelist, struct scan_control *sc);
#endif

unsigned long __pgprot_to_posix_prot(pgprot_t pgprot);
void liblinux_free_mm(struct mm_struct *mm);
bool _check_vma_mm_valid(const struct vm_area_struct *vma);

#endif /* __LIBLINUX_MM_INTERNAL_H__ */
