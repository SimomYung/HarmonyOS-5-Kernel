/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_MM_H__
#define __LIBLINUX_LINUX_MM_H__

#define totalram_pages_inc __totalram_pages_inc
#define totalram_pages_dec __totalram_pages_dec
#define totalram_pages_add __totalram_pages_add
#include_next <linux/mm.h>
#undef totalram_pages_inc
#undef totalram_pages_dec
#undef totalram_pages_add

static inline void totalram_pages_inc(void) {}

static inline void totalram_pages_dec(void) {}

static inline void totalram_pages_add(long count) {}

vm_fault_t vmf_insert_pfn_batch_prot(struct vm_area_struct *vma, unsigned long addr,
				     const struct pfn_range *array, unsigned long cnt, pgprot_t prot);
vm_fault_t vmf_insert_pfn_batch(struct vm_area_struct *vma, unsigned long addr,
			const struct pfn_range *array, unsigned long cnt);
vm_fault_t vmf_insert_pfn_batch_sp(struct vm_area_struct *vma, unsigned long addr,
			const struct pfn_range *array, unsigned long cnt, int tgid);

unsigned long vm_flags_to_prot(unsigned long vm_flags);
unsigned int pgprot_to_liblinux_flags(pgprot_t prot);

/* `tgid` is root pid_ns val */
int liblinux_zap_vma_ptes_sp(unsigned long address, unsigned long size, int tgid);
int liblinux_zap_vma_ptes(struct vm_area_struct *vma, unsigned long address,
			  unsigned long size);

#if defined(CONFIG_LIBLINUX_NATIVE_BUDDY) && defined(CONFIG_LIBLINUX_HIBERNATE)
void pm_clean_all_free_pages(void);
#else
static inline void pm_clean_all_free_pages(void) {}
#endif

void dmabuf_systrace_begin(const char *fmt, ...);
void dmabuf_systrace_end(void);

#endif /* __LIBLINUX_LINUX_MM_H__ */
