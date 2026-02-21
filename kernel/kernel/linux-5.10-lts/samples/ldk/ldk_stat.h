/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBDH_MEMSTAT_H__
#define __LIBDH_MEMSTAT_H__

#ifdef __KERNEL__
#include <linux/atomic.h>

/* should be consistent with sysmgr: intapi/mem/devhost_stat.h */
typedef struct liblinux_ldk_mem_stat
{
	atomic_long_t dmaheap_free_pages;
	atomic_long_t buddy_dma_free_pages;
	atomic_long_t system_free_pages;
} raw_mem_stat_t;

typedef struct sys_mem_stat {
	atomic_long_t avail_buf_wmark_min;
	atomic_long_t avail_buf_wmark_mid;
	atomic_long_t avail_buf_wmark_high;
	atomic_long_t curr_avail_buf;
} raw_sys_mem_stat_t;

typedef struct liblinux_ldk_stat
{
	raw_mem_stat_t mem_stat;
	raw_sys_mem_stat_t sys_mem_stat;
} raw_stat_t;
#endif

size_t liblinux_get_ldk_stat_size(void);
void liblinux_set_ldk_stat_registered(void);
bool liblinux_is_ldk_stat_registered(void);
void *liblinux_get_ldk_stat_common(void);

#endif /* __LIBDH_MEMSTAT_H__ */
