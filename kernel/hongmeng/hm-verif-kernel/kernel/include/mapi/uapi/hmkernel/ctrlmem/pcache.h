/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of pcache - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 25 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_PCACHE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_PCACHE_H

#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/ubdqueue.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>

#define __CTRLMEM_PCACHE_PAGE_SIZE	(4096)
#define __CTRLMEM_PCACHE_CPU_MAX	(32)
#define __CTRLMEM_PCACHE_OPLOG_NENTRIES	(128 * 2)
#define __CTRLMEM_PCACHE_OPLOG_MEM_SIZE (4096 * 2)
#ifdef CONFIG_HHP
#define __CTRLMEM_PCACHE_QUEUEHEAD_CNT	(4)
#else
#define __CTRLMEM_PCACHE_QUEUEHEAD_CNT	(2)
#endif

struct __ctrlmem_pcache_pcp_node_s {
	struct __ctrlmem_ubdq_s zqueue_head;
	struct __ctrlmem_ubdq_s nqueue_head;
#ifdef CONFIG_HHP
	struct __ctrlmem_ubdq_s hhp_zqueue_head;
	struct __ctrlmem_ubdq_s hhp_nqueue_head;
#endif
	__u8 padding[(__CTRLMEM_PCACHE_PAGE_SIZE / __CTRLMEM_PCACHE_CPU_MAX) -
		     (__CTRLMEM_PCACHE_QUEUEHEAD_CNT * sizeof(struct __ctrlmem_ubdq_s))];
};

#ifndef __uatomic64
# ifndef __KERNEL__
struct __ctrlmem_pcache_oplog_uatomic64_s {
	__u64 donttouch;
};
typedef struct __ctrlmem_pcache_oplog_uatomic64_s __ctrlmem_pcache_oplog_uatomic64_t;
#  define __uatomic64 __ctrlmem_pcache_oplog_uatomic64_t
# else
#  include <lib/atomic.h>
#  define __uatomic64 vatomic64_t
# endif
#endif

struct __ctrlmem_pcache_oplog_ctrl_s {
	__uatomic64 idx_producer;
	__uatomic64 idx_consumer;
	/*
	 * Each time when an entry is read, before it get
	 * processed, unprocessed should plus by 1. By this way,
	 * if anyone observe unprocessed == 0, it knows that there
	 * is no data unprocess and non-public available.
	 *
	 * It can be used for vspace related actions. After sysmgr
	 * lock a vspace, it first consume all events from oplog, then
	 * wait for unprocessed reduces to 0, then the consumer
	 * ensures that all events related to that vspace has been applied.
	 *
	 * Co-management:
	 *   Kernel doesn't touch it when genearting normal oplog entry.
	 *   In case when OOB action takes place, kernel should increase
	 *   unprocessed by one. It is reduced by sysmgr oob event
	 *   consumer.
	 *
	 *   For sysmgr, each time before it read an entry from oplog,
	 *   it should increase unprocessed by one, and reduce it when
	 *   the data is processed.
	 *
	 *  vatomic64_inc(&unprocessed);  |
	 *  err = read_from_oplog(&data);    |
	 *  if (err == 0) {                  |
	 *      apply(data);                 |
	 *  }                                |
	 *  vatomic64_dec(&unprocessed);  |
	 */
	__uatomic64 unprocessed;
};

struct __ctrlmem_pcache_oplog_entry_s {
	__uatomic64 vregion_syspa;
	__u64 syspg_syspa;
	__u64 vaddr;
};

struct __ctrlmem_pcache_oplog_s {
	struct __ctrlmem_pcache_oplog_ctrl_s ctrl;
	struct __ctrlmem_pcache_oplog_entry_s entries[__CTRLMEM_PCACHE_OPLOG_NENTRIES];
};

struct __ctrlmem_pcache_s {
	struct __ctrlmem_pcache_pcp_node_s pcpnodes[__CTRLMEM_PCACHE_CPU_MAX];
};

#endif
