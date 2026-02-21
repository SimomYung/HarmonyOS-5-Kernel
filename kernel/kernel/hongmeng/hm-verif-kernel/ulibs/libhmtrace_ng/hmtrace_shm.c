/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of hmtrace shm
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 29 09:13:58 2023
 */
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmtrace_ng/hmtrace.h>
#include <lib/trace/trace_misc.h>

#ifdef CONFIG_TRACEMGR

static uintptr_t trace_shm_addr = INVALID_VADDR;
static uintptr_t trace_shm_snapshot_addr = INVALID_VADDR;
static uintptr_t trace_shm_saved_taskinfo_addr = INVALID_VADDR;

void hm_trace_shm_set_addr(uintptr_t addr)
{
	trace_shm_addr = addr;
}

void hm_trace_shm_set_snapshot_addr(uintptr_t addr)
{
	trace_shm_snapshot_addr = addr;
}

void hm_trace_shm_set_saved_taskinfo_addr(uintptr_t addr)
{
	trace_shm_saved_taskinfo_addr = addr;
}

void *hm_trace_shm_addr_of(void)
{
	return trace_shm_addr == INVALID_VADDR ? NULL : (void *)trace_shm_addr;
}

#ifdef CONFIG_TRACE_SNAPSHOT
void *hm_trace_shm_snapshot_addr_of(void)
{
	return trace_shm_snapshot_addr == INVALID_VADDR ? NULL : (void *)trace_shm_snapshot_addr;
}
#else
void *hm_trace_shm_snapshot_addr_of(void)
{
	return hm_trace_shm_addr_of();
}
#endif

void *hm_trace_shm_record_addr_of(void)
{
	bool enabled = hm_rmq_is_snapshot_enabled((void *)trace_shm_addr);
	return ((enabled) ? hm_trace_shm_snapshot_addr_of() : hm_trace_shm_addr_of());
}

void *hm_trace_shm_idle_addr_of(void)
{
	bool enabled = hm_rmq_is_snapshot_enabled((void *)trace_shm_addr);
	return ((enabled) ? hm_trace_shm_addr_of() : hm_trace_shm_snapshot_addr_of());
}

void *hm_trace_shm_saved_taskinfo_addr_of(void)
{
	BUG_ON(trace_shm_saved_taskinfo_addr == INVALID_VADDR);

	return (void *)trace_shm_saved_taskinfo_addr;
}

void *hm_trace_shm_savedcmd_addr_of(void)
{
	return ptr_cast(ptr_add(hm_trace_shm_saved_taskinfo_addr_of(), SAVEDCMD_OFFSET), void);
}

void *hm_trace_shm_savedtgid_addr_of(void)
{
	return ptr_cast(ptr_add(hm_trace_shm_saved_taskinfo_addr_of(), SAVEDTGID_OFFSET), void);
}

void *hm_trace_shm_savedstring_addr_of(void)
{
	return ptr_cast(ptr_add(hm_trace_shm_saved_taskinfo_addr_of(), SAVEDSTRING_OFFSET), void);
}

#ifdef CONFIG_TRACE_SNAPSHOT
uint64_t hm_trace_pmem_used(void)
{
	return hm_rmq_get_memory_usage(hm_trace_shm_addr_of())
		+ hm_rmq_get_memory_usage(hm_trace_shm_snapshot_addr_of())
		+ SAVED_TASKINFO_SIZE;
}
#else
uint64_t hm_trace_pmem_used(void)
{
	return hm_rmq_get_memory_usage(hm_trace_shm_addr_of())
		+ SAVED_TASKINFO_SIZE;
}
#endif /* CONFIG_TRACE_SNAPSHOT */

int hm_trace_write_savedcmd(int tid, const char *name)
{
	int ret = E_HM_OK;
	int tid_idx = (unsigned int)tid & (HM_MAX_CMDLINES - 1);
	struct trace_saved_cmdlines_buffer_s *savedcmd =
		(struct trace_saved_cmdlines_buffer_s *)hm_trace_shm_savedcmd_addr_of();
	if (savedcmd == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		while (vatomic32_cmpxchg(&savedcmd->savedcmd_lock, 0, 1)) {};
		unsigned int cmd_idx = savedcmd->map_tid_to_cmdline[tid_idx];
		if (cmd_idx == HM_NO_CMDLINE || cmd_idx >= HM_DEFAULT_CMDLINES) {
			cmd_idx = vatomic32_get_add(&savedcmd->cmdline_idx, 1) % HM_DEFAULT_CMDLINES;
			savedcmd->map_tid_to_cmdline[tid_idx] = cmd_idx;
		}
		if (savedcmd->saved_cmdlines[cmd_idx].tid != (unsigned int)tid) {
			unsigned int covered_tid_idx = savedcmd->saved_cmdlines[cmd_idx].tid &
						       (HM_MAX_CMDLINES - 1U);
			savedcmd->map_tid_to_cmdline[covered_tid_idx] = HM_NO_CMDLINE;
			savedcmd->saved_cmdlines[cmd_idx].tid = (unsigned int)tid;
		}
		NOFAIL(strncpy_s(savedcmd->saved_cmdlines[cmd_idx].cmdline, HMCAP_THREAD_NAME_SIZE,
				 name, HMCAP_THREAD_NAME_SIZE - 1));
		vatomic32_sub(&savedcmd->savedcmd_lock, 1);
	}
	return ret;
}

int hm_trace_write_savedtgid(int tid, int pid)
{
	int ret = E_HM_OK;
	unsigned int *savedtgid = (unsigned int *)hm_trace_shm_savedtgid_addr_of();
	if (savedtgid == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (tid < HM_MAX_TGIDS) {
			savedtgid[tid] = (unsigned int)pid;
		} else {
			ret = E_HM_INVAL;
		}
	}
	return ret;
}

#else
void hm_trace_shm_set_addr(uintptr_t addr)
{
	UNUSED(addr);
}

void hm_trace_shm_set_snapshot_addr(uintptr_t addr)
{
	UNUSED(addr);
}

void hm_trace_shm_set_saved_taskinfo_addr(uintptr_t addr)
{
	UNUSED(addr);
}

void *hm_trace_shm_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_snapshot_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_record_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_idle_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_saved_taskinfo_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_savedcmd_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_savedtgid_addr_of(void)
{
	return NULL;
}

void *hm_trace_shm_savedstring_addr_of(void)
{
	return NULL;
}

uint64_t hm_trace_pmem_used(void)
{
	return 0ULL;
}

int hm_trace_write_savedcmd(int tid, const char *name)
{
	UNUSED(tid, name);
	return E_HM_NOSYS;
}

int hm_trace_write_savedtgid(int tid, int pid)
{
	UNUSED(tid, pid);
	return E_HM_NOSYS;
}
#endif /* CONFIG_TRACEMGR */
