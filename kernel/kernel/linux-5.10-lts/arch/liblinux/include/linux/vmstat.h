/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_VMSTAT_H__
#define __LIBLINUX_LINUX_VMSTAT_H__

#define zone_page_state_add __zone_page_state_add
#include_next <linux/vmstat.h>
#undef zone_page_state_add

static inline void liblinux_global_free_pages_set(enum zone_stat_item item)
{
	if (item == NR_FREE_PAGES)
		liblinux_set_buddy_dma_free_pages();
}

static inline void zone_page_state_add(long x, struct zone *zone,
				 enum zone_stat_item item)
{
	atomic_long_add(x, &zone->vm_stat[item]);
	atomic_long_add(x, &vm_zone_stat[item]);
	/* It is for devhost buddy zone free pages statistic without wmark
	 * NR_FREE_PAGES change is only used in this func and fold_diff(mm/vmstat.c)
	 */
	liblinux_global_free_pages_set(item);
}

#endif /* __LIBLINUX_LINUX_VMSTAT_H__ */
