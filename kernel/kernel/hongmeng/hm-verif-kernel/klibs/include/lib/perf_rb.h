/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: header for perf ring buffer
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 24 15:46:25 CST 2023
 */

#ifndef KLIBS_PERF_RB_H
#define KLIBS_PERF_RB_H

#include <lib/dlist.h>
#include <hmkernel/types.h>
#include <hmasm/registers.h>

#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif

#define MMAP_PAGE_HEAD_OFFSET 1024
struct perf_rb_mmap_page_s {
	__u32 version;

	vatomic64_t data_head __attribute__ ((aligned (MMAP_PAGE_HEAD_OFFSET)));
	vatomic64_t data_tail;
	__u64 data_offset;
	__u64 data_size;

	vatomic64_t aux_head;
	vatomic64_t aux_tail;
	__u64 aux_offset;
	__u64 aux_size;
};

#ifdef __KERNEL__
static_assert(__builtin_offsetof(struct perf_rb_mmap_page_s, data_head) == MMAP_PAGE_HEAD_OFFSET,
	      adjust_mmap_page_head_offset);
#endif

enum perf_kobj_status {
	__PERF_KOBJ_UNINITED,
	__PERF_KOBJ_INITING,
	__PERF_KOBJ_INITED,
	__PERF_KOBJ_WAKEUP,
};

struct perf_kobj_list_s {
	struct dlist_node node;
	__u64 sysmgr_poll_addr;
	__u64 ctrlmem_polllist;
	vatomic32_t status;
};

struct perf_rb_info_s {
	__u64 data_size;

	__u64 offset;
	__u64 watermark;

	vatomic64_t nest;
	vatomic64_t head;
	vatomic64_t wakeup;
	vatomic64_t lost;

	__u8 overwrite;

	/* AUX area for SPE */
	vatomic64_t aux_nest;
	__u64 aux_head;
	__u64 aux_wakeup;
	__u64 aux_watermark;

	void *aux_data;
	__u64 aux_data_size;

	struct perf_rb_mmap_page_s *user_page;
	void *data;

	struct perf_kobj_list_s kobj_list;
};

struct perf_rb_entry_s {
	__u64 wakeup;
	__u64 offset;
	__u64 aux_flags;
	__u64 head;
	__u64 size;

	struct perf_rb_info_s *rb;
};

static inline __u64 perf_rb_calc_offset(const struct perf_rb_info_s *rb,
					 __u64 offset)
{
	return (offset & (rb->data_size - 1));
}

int perf_rb_alloc_entry(struct perf_rb_entry_s *entry,
			struct perf_rb_info_s *rb,
			__u32 size);
void perf_rb_output_data(struct perf_rb_entry_s *entry,
			 const void *data, __u32 size);
bool perf_rb_commit_entry(struct perf_rb_entry_s *entry);
void perf_rb_output_uregs(struct perf_rb_entry_s *entry, __u64 mask, struct arch_regs *uregs);

int perf_aux_rb_alloc_entry(struct perf_rb_entry_s *entry,
			    struct perf_rb_info_s *rb,
			    __u32 size);
bool perf_aux_rb_output_skip(struct perf_rb_entry_s *entry,
			     struct perf_rb_info_s *rb,
			     __u64 size);
void perf_aux_rb_output_flag(struct perf_rb_entry_s *entry,
			     __u64 flags);
bool perf_aux_rb_commit_entry(struct perf_rb_entry_s *entry, __u64 size);

#define perf_rb_output(entry, x) perf_rb_output_data((entry), &(x), sizeof(x))
#endif
