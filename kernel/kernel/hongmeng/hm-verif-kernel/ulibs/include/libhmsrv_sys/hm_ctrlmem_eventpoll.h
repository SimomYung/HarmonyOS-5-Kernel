/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 05 12:04:10 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_HM_CTRLMEM_EVENTPOLL_H
#define ULIBS_INCLUDE_LIBHMSRV_HM_CTRLMEM_EVENTPOLL_H
#include <hmasm/types.h>
#include <hmasm/page.h>
#include <hmkernel/ctrlmem/kobjs/polllist.h>
#include <vsync/atomic.h>
#include <libhmsync/bitops/generic.h>

#define REVENTS_PER_PAGE (PAGE_SIZE/sizeof(vatomic32_t))
struct revents_page_s {
	struct dlist_node node;
	int shm_id;
	int used_cnt;
	void *page;
	DEFINE_RAW_BITMAP(bitmap, REVENTS_PER_PAGE);
};

struct polllist_s {
	struct __ctrlmem_kobj_polllist_s *polllist;
	vatomic32_t *revents;
	struct revents_page_s *revents_page;
};
struct polllist_s *hm_create_polllist(unsigned long file, __u32 listener_cnt);
int hm_destroy_polllist(struct polllist_s *polllist);
int hm_update_polllist_events(struct polllist_s *polllist, __u32 revents);
int hm_cleanup_polllist_events(struct polllist_s *polllist, __u32 revents);
#endif
