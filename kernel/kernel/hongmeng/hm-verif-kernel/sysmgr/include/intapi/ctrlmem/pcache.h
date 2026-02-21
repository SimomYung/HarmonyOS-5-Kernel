/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem pcache related functions in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Wed APr 26 203
 */

#ifndef INTAPI_CTRLMEM_PCACHE_H
#define INTAPI_CTRLMEM_PCACHE_H

#include <stdint.h>
#include <stdbool.h>
#include <hmasm/memory.h>
#include <hmkernel/ctrlmem/pcache.h>

extern void ctrlmem_pcache_page_init(struct __ctrlmem_pcache_s *pcache);

struct __ctrlmem_pcache_oplog_entry_data_s {
	__u64 vregion;
	__u64 syspg;
	__u64 vaddr;
};

extern void ctrlmem_pcache_oplog_trans(void);
extern void ctrlmem_pcache_oplog_alloc_mem(void);
extern uint64_t ctrlmem_pcache_global_oplog_pa(void);

extern int __ctrlmem_pcache_oplog_read(struct __ctrlmem_pcache_oplog_s *oplog,
				       struct __ctrlmem_pcache_oplog_entry_data_s *data);
int ctrlmem_pcache_oplog_append(struct __ctrlmem_pcache_oplog_s *oplog,
				__u64 syspg,
				__u64 vfspg,
				__u64 vregion);
extern void __ctrlmem_pcache_oplog_unprocessed_inc(struct __ctrlmem_pcache_oplog_s *oplog);
extern void __ctrlmem_pcache_oplog_unprocessed_dec(struct __ctrlmem_pcache_oplog_s *oplog);
extern bool __ctrlmem_pcache_oplog_has_unprocessed(struct __ctrlmem_pcache_oplog_s *oplog);

extern int ctrlmem_pcache_oplog_consume_all(void);

extern void ctrlmem_pcache_oplog_wait_for_unprocessed(void);

struct vregion_s;
struct page_s;
void ctrlmem_oplog_append_filepage(struct page_s *syspg, uintptr_t vfspg, uint32_t flag);
extern void ctrlmem_pcache_oplog_consume_oob(struct vregion_s *vr, struct page_s *pg,
					     uintptr_t vaddr);

#ifdef CONFIG_SYSMGR_CTRLMEM_PAGEFAULT
unsigned long oplog_consumed_total(void);
unsigned long oplog_consumed_consumer(void);
#else
static inline unsigned long oplog_consumed_total(void)
{
	return 0;
}
static inline unsigned long oplog_consumed_consumer(void)
{
	return 0;
}
#endif
#endif
