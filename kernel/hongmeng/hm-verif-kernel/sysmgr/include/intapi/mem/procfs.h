/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Add interfaces for querying system memory information
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 08 16:10:57 2022
 */

#ifndef SYSMGR_MEM_PROCFS_H
#define SYSMGR_MEM_PROCFS_H

#include <stddef.h>
#include <intapi/process/process.h>
#include <intapi/procfs/seq_file.h>

/* 
 Lmkd_dbg_dump print too much information that not relevant to the memleak and
 cost performance & power issues;
 Only printing certain type of memory detail when it's leaked;
*/
struct dump_control {
	/* in the unit of bytes */
	uint64_t mem_total;
	uint64_t ion_used_total;
	uint64_t slab_used_total;
	uint64_t ashmem_used_total;
	uint64_t ashmem_alloc_total;
};

int mem_fill_meminfo(char *data, size_t data_len);
int mem_fill_vmstat(char *data, size_t data_len);
#ifdef CONFIG_HARMONYOS
int mem_proc_meminfo_lite(char *data, size_t data_len);
#endif
#ifdef CONFIG_MEM_STAT_OVERVIEW
int mem_proc_memview(char *data, size_t data_len);
void mem_proc_memview_dump(struct dump_control *dc);
#endif
int mem_fill_zoneinfo(char *data, size_t data_len);
int mem_fill_buddyinfo(char *data, size_t data_len);
int mem_fill_statm(struct process_s *process, struct seq_file_s *seq);
int mem_fill_status(struct process_s *process, char *buf, size_t buf_len, int *olen);
int mem_fill_smaps(struct process_s *process, struct seq_file_s *seq);
int mem_fill_maps(struct process_s *process, char *buf, size_t buf_len, int *olen);
int mem_fill_smaps_rollup(struct process_s *process, struct seq_file_s *seq);
#ifdef CONFIG_MEM_STAT_CLEAR_REFS
int mem_proc_clear_refs(struct process_s *process);
#endif
#ifdef CONFIG_COMPACTION
int mem_fill_buddy_extfrag_index(char *data, size_t data_len);
#else
static inline int mem_fill_buddy_extfrag_index(char *data, size_t data_len)
{
	UNUSED(data, data_len);
	return E_HM_NOSYS;
}
#endif
#ifdef CONFIG_SWAP
int mem_fill_swapstat(char *data, size_t data_len);
int mem_proc_swaps(char *data, size_t data_len);
#endif
ssize_t mem_proc_mem_read(pid_t pid, uint64_t seq, unsigned int nsid, void *buf, size_t count, uint64_t *offset);
ssize_t mem_proc_mem_write(pid_t pid, uint64_t seq, unsigned int nsid, void *buf, size_t count, uint64_t *offset);
ssize_t mem_proc_pagemap_read(pid_t pid, uint64_t seq, unsigned int nsid, void *buf, size_t count, uint64_t *offset);
int mem_proc_maps_open(pid_t pid, uint64_t seq, unsigned int nsid, struct maps_private_s **priv);
int mem_proc_maps_close(struct maps_private_s *priv);
const struct seq_operations *mem_seq_maps_ops(void);
#ifdef CONFIG_ALLOC_ACCT
int mem_fill_allocacct(char *data, size_t data_len);
#endif
#ifdef CONFIG_RECLAIM_ACCT
int mem_fill_reclaim_acct(char *data, size_t data_len);
int mem_fill_reclaim_efficiency(char *data, size_t data_len);
#endif
#ifdef CONFIG_HYPERVISOR
int mem_fill_vm_smaps(struct process_s *process, struct seq_file_s *seq);
#endif /* CONFIG_HYPERVISOR */
#ifdef CONFIG_SECURITY_XPM
int dsmm_developer_read(char *data, size_t data_len, const char *boot_cmdline);
#endif
int mem_proc_pagetypeinfo(char *data, size_t data_len);
int mem_proc_slabinfo(char *data, size_t data_len);
#ifdef CONFIG_CONTIGUOUS_MEM_ZONE
int mem_read_cma_info(char *data, size_t data_len);
void mem_cma_info_dump(void);
#endif
#ifdef CONFIG_RSS_THRESHOLD
int mem_fill_vm_rss(struct process_s *process, char *buf, size_t buf_len, int *olen);
int mem_fill_vm_rss_threshold(struct process_s *process, char *buf, size_t buf_len, int *olen);
int mem_set_vm_rss_threshold(struct process_s *process, long val);
#endif /* CONFIG_RSS_THRESHOLD */
#ifdef CONFIG_LMKD_DBG
int mem_lmkd_dbg_trigger_read(char *data, size_t data_len);
int mem_lmkd_dbg_trigger_write(int val);
void mem_lmkd_dbg_emergency_dump_detail(void);
void mem_lmkd_dbg_init(void);
#endif /* CONFIG_LMKD_DBG */
#ifdef CONFIG_DMABUF_MM
int mem_proc_dmaheap(char *data, size_t data_len);
void mem_proc_dmaheap_dump(void);
#endif /* CONFIG_DMABUF_MM */
uint64_t get_process_dmaheap_size_inbyte(struct process_s *process);
#ifdef CONFIG_DFX_MEMCHECK
int mem_proc_ion(char *data, size_t data_len);
int mem_proc_fence(char *data, size_t data_len);
void mem_proc_ion_dump(void);
void mem_proc_fence_dump(void);
#endif /* CONFIG_DFX_MEMCHECK */
#ifdef CONFIG_DMABUF_PROCESS_INFO
int mem_proc_dmabuf_process(char *data, size_t data_len);
void mem_proc_dmabuf_process_dump(void);
#endif /* CONFIG_DMABUF_PROCESS_INFO */

#endif /* SYSMGR_MEM_PROCFS_H */
