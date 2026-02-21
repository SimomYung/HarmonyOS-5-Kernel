// SPDX-License-Identifier: GPL-2.0
#include <libhmsync/atomic.h>
#include <lib/trace/trace_misc.h>
#include <libhmtrace_ng/hmtrace.h>
#include <liblinux/pal.h>

uintptr_t hmtracefs_record_shm_addr_of(void)
{
	return (uintptr_t)hm_trace_shm_record_addr_of();
}

int hmtracefs_rmq_is_enabled(void)
{
	return hm_rmq_is_record_enabled(hmtracefs_record_shm_addr_of()) ? 1 : 0;
}

uint64_t hmtracefs_rmq_enqueue_get_entry(uintptr_t rmq_addr, uint64_t len, void *ppe)
{
	return (uint64_t)hm_rmq_enqueue_get_entry((void *)rmq_addr, hm_trace_current_cpu_of(), ppe, len);
}

void hmtracefs_rmq_enqueue_confirm_entry(void *ppe)
{
	hm_rmq_enqueue_confirm_entry(ppe);
}

void hmtracefs_rmq_dequeue_futex_wake(void)
{
	hm_trace_rmq_dequeue_futex_wake();
}
