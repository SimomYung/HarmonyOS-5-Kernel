/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_LINUX_BUDDY_TRACKER_H__
#define __LIBLINUX_LINUX_BUDDY_TRACKER_H__

#define PAGE_SHIFT_KB (2) /* Bit shift for converting order to kilobytes */
#define MAX_ENTRIES 128    /* Maximum number of entries in the tracking map */
#define PAGE_ORDER_TO_KB(order) ((1 << (order)) << PAGE_SHIFT_KB)

struct tracker_pair {
	unsigned long caller;
	atomic_t value;
	struct hlist_node hnode;
};

struct buddy_tracker {
	struct tracker_pair *trackmap;
	unsigned long *callermap;
	struct hlist_head *tracker_hashmap;
	atomic_t entry_index;
	bool initialized;
};

void liblinux_sub_caller_use_cnt(struct page *page, unsigned int order);
void liblinux_add_caller_use_cnt(struct page *page, unsigned int order, unsigned long caller);

void liblinux_dump_ldk_buddy_tracker_info(void);
void liblinux_buddy_tracker_set_vmemmap_size(unsigned long long vmemmap_size);
void liblinux_buddy_tracker_init(unsigned long long vmemmap_size);
 
#endif /* !__LIBLINUX_LINUX_BUDDY_TRACKER_H__ */