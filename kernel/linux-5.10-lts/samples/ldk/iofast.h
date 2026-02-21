/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_LINUX_IOFAST_H__
#define __LIBDH_LINUX_IOFAST_H__

#define MAX_ARRAY_LENGTH	16
void *liblinux_iofast_alloc_pt_page(unsigned int order);
void *liblinux_iofast_alloc_pgd_page(unsigned int order);

extern unsigned long native_gfp_waterline;
extern void adjust_present_page_count(struct page *pg, long count);
#endif /* __LIBDH_LINUX_IOFAST_H__ */
