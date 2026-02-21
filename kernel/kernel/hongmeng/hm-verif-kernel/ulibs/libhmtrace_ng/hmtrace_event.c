/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of hmtrace event
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 29 09:18:51 2023
 */
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <hmkernel/capability.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmtrace_ng/hmtrace.h>
#include <lib/trace/trace_misc.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libsysif/sysmgr/api.h>

#ifdef CONFIG_TRACEMGR
unsigned int hm_trace_current_cpu_of(void)
{
	return hm_actv_local_thread_cpu_of();
}

void hm_trace_tcb_name_of(char *buf)
{
	BUG_ON(buf == NULL);

	const char *name = hm_actv_local_thread_name_of();
	if (name != NULL) {
		NOFAIL(memcpy_s(buf, HMCAP_THREAD_NAME_SIZE, name, HMCAP_THREAD_NAME_SIZE));
	} else {
		buf[0] = '\0';
	}
}

__s32 hm_trace_tid_of(bool tcb_event)
{
	UNUSED(tcb_event);
	return hm_actv_local_thread_tid_of();
}

#ifdef __HOST_LLT__
static __u64 fake_boot_ts = 0ULL;
static __u64 fake_mono_ts = 0ULL;
#endif
static __u64 hm_trace_rmq_boot_timestamp(void)
{
	__u64 timestamp = 0;
#ifdef __HOST_LLT__
	timestamp = fake_boot_ts++;
#else
	struct timespec time;
	if (hm_clock_gettime(CLOCK_BOOTTIME, &time) == 0) {
		timestamp = (__u64)time.tv_sec * NSEC_PER_SEC + (__u64)time.tv_nsec;
	} else {
		hm_warn("Fail to get time while trace writes event");
	}
#endif
	return timestamp;
}

static __u64 hm_trace_rmq_mono_timestamp(void)
{
	__u64 timestamp = 0;
#ifdef __HOST_LLT__
	timestamp = fake_mono_ts++;
#else
	struct timespec time;
	if (hm_clock_gettime(CLOCK_MONOTONIC, &time) == 0) {
		timestamp = (__u64)time.tv_sec * NSEC_PER_SEC + (__u64)time.tv_nsec;
	} else {
		hm_warn("Fail to get time while trace writes event");
	}
#endif
	return timestamp;
}

__u64 hm_trace_rmq_timestamp(void *rmq)
{
	int clock_id = hm_rmq_clock_id_of(rmq);
	__u64 timestamp = 0U;

	switch (clock_id) {
	case TRACE_CLOCK_BOOT:
		timestamp = hm_trace_rmq_boot_timestamp();
		break;
	case TRACE_CLOCK_MONO:
		timestamp = hm_trace_rmq_mono_timestamp();
		break;
	default:
		hm_error("trace clock %u is invalid. trace clock clock will be set to default boot clock", clock_id);
		(void)hm_rmq_set_clock_id(rmq, TRACE_CLOCK_BOOT, false);
		timestamp = hm_trace_rmq_boot_timestamp();
		break;
	}

	return timestamp;
}

void __attribute__((format(printf, 1, 2))) hm_trace_warn(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	__hmlogv_warn(fmt, args);
	va_end(args);
}

void hm_trace_rmq_dequeue_futex_wake(void)
{
	rmq_global_t *global = hm_rmq_get_global(hm_trace_shm_addr_of());
	unsigned int waiters = (unsigned int)vatomic32_read(&global->dequeue_futex_waiters);
	if (unlikely(waiters != 0)) {
		(void)actvcall_hmcall_tracemgr_futex_wake(TRACE_RMQ_DEQUEUE_FUTEX);
	}
}

void hm_trace_rmq_shrink_wake(void)
{
	(void)actvcall_hmcall_tracemgr_futex_wake(TRACE_RMQ_SHRINK_FUTEX);
	if (hm_rmq_is_snapshot_enabled(hm_trace_shm_addr_of())) {
		(void)actvcall_hmcall_tracemgr_futex_wake(TRACE_SNAPSHOT_RMQ_SHRINK_FUTEX);
	}
}

int hm_trace_register_events(enum trace_event_module module)
{
	int ret = E_HM_OK;

	if (((unsigned int)module <= (unsigned int)TRACE_EVENT_MODULE_SYSMGR) ||
	    ((unsigned int)module == (unsigned int)TRACE_EVENT_MODULE_LDK) ||
	    ((unsigned int)module >= (unsigned int)TRACE_EVENT_MODULE_MAX)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = actvcall_hmcall_tracemgr_register_events(module);
	}
	return ret;
}

int hm_trace_register_ldk_events(struct tracepoint_config *tps, unsigned int *tp_type, unsigned int tp_num)
{
	int ret = E_HM_OK;

	if ((tp_num > TRACE_EVENT_CONFIG_MAX_NUM) || (tps == NULL) || (tp_type == NULL)) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		struct trace_register_events_param param = {
			.tps = tps,
			.tp_type = tp_type,
			.tp_num = tp_num
		};
		ret = actvcall_hmcall_tracemgr_register_ldk_events(param);
	}
	return ret;
}

int hm_trace_add_ldk_printk_format(char *str)
{
	int rc = E_HM_OK;

	size_t size = strnlen(str, MAX_TP_STRING_SIZE);
	if (size >= MAX_TP_STRING_SIZE) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = actvcall_hmcall_tracemgr_add_ldk_printk_format(str, size);
	}
	return rc;
}

bool hm_trace_is_tracing_block_enabled(void)
{
	bool ret = false;

	if (hm_trace_shm_addr_of() != NULL) {
		ret = hm_rmq_is_tracing_block_enabled(hm_trace_shm_addr_of());
	}
	return ret;
}

void hm_trace_set_tracing_block_enabled(bool enable)
{
	if (hm_trace_shm_addr_of() != NULL) {
		hm_rmq_set_tracing_block_enabled(hm_trace_shm_addr_of(), enable);
	}
}
#else
unsigned int hm_trace_current_cpu_of(void)
{
	return 0U;
}

void hm_trace_tcb_name_of(char *buf)
{
	UNUSED(buf);
}

__s32 hm_trace_tid_of(bool tcb_event)
{
	UNUSED(tcb_event);
	return 0;
}

__u64 hm_trace_rmq_timestamp(void *rmq)
{
	UNUSED(rmq);
	return 0ULL;
}

void __attribute__((format(printf, 1, 2))) hm_trace_warn(const char *fmt, ...)
{
	UNUSED(fmt);
}

void hm_trace_rmq_dequeue_futex_wake(void)
{
}

void hm_trace_rmq_shrink_wake(void)
{
}

int hm_trace_register_events(enum trace_event_module module)
{
	UNUSED(module);

	return E_HM_NOSYS;
}

int hm_trace_register_ldk_events(struct tracepoint_config *tps, unsigned int *tp_type, unsigned int tp_num)
{
	UNUSED(tps, tp_type, tp_num);

	return E_HM_NOSYS;
}

int hm_trace_add_ldk_printk_format(char *str)
{
	UNUSED(str);

	return E_HM_NOSYS;
}

bool hm_trace_is_tracing_block_enabled(void)
{
	return false;
}

void hm_trace_set_tracing_block_enabled(bool enable)
{
	UNUSED(enable);
}
#endif /* CONFIG_TRACEMGR */

/* the interface of rmq_dequeue_batch is only called in ldk */
__attribute__((weak)) int hm_trace_rmq_dequeue_batch_copy(void *dst, void *src, size_t size)
{
	UNUSED(dst, src, size);
	return E_HM_NOSYS;
}
