// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 *
 * Description: Debug improper memory alloc behaviours
 * Author: Li Xilun <lixilun1@huawei.com>
 * Create: 2023-03-23
 */

#define pr_fmt(fmt) "improper_alloc: " fmt

#include <improper_alloc_core.h>

#ifdef CONFIG_HISYSEVENT
#include <linux/stacktrace.h>
#include <dfx/hiview_hisysevent.h>
#include <securec.h>
#endif

/* g_improper_alloc_enable must be false before initialize is completed */
unsigned int __read_mostly g_improper_alloc_enable;

#ifdef CONFIG_HISYSEVENT
static void improper_alloc_report_handler(struct work_struct *work);
static DECLARE_WORK(improper_alloc_report_wk, improper_alloc_report_handler);
struct improper_alloc_report *g_improper_alloc_report;

#define UNRESOLVED_PREFIX "0x"
#define SPACE_PLACEHOLDER 1

static bool improper_alloc_create_msg(void)
{
	int ret_msg, ret_entry, off, i;
	char entry_buf[REPORT_ENTRY_BUF] = {0};
	bool truncated = false;

	off = g_improper_alloc_report->off;
	for (i = 0; i < g_improper_alloc_report->nr_entries; i++) {
		/* Skip unresolved entries which start with 0x */
		ret_entry = snprintf_s(entry_buf, REPORT_ENTRY_BUF, REPORT_ENTRY_BUF - 1,
				       "%pS", g_improper_alloc_report->entries[i]);
		if (ret_entry < 0) {
			pr_err("failed to create entry_buf\n");
			return false;
		}
		if (strncmp(entry_buf, UNRESOLVED_PREFIX, strlen(UNRESOLVED_PREFIX)) == 0)
			break;

		/* Asynchronously showing the stack trace */
		pr_info("%s\n", entry_buf);

		/* Truncate the message if it exceeds the limit and upload it anyway */
		if (ret_entry + SPACE_PLACEHOLDER > MAX_REPORT_MSG_LEN - off - 1) {
			truncated = true;
			continue;
		}
		ret_msg = snprintf_s(g_improper_alloc_report->msg + off, MAX_REPORT_MSG_LEN - off,
				     MAX_REPORT_MSG_LEN - off - 1, "%s ", entry_buf);
		if (ret_msg < 0) {
			pr_err("failed to create stack trace msg\n");
			return false;
		}
		off += ret_msg;
	}

	if (truncated)
		pr_info("The stack trace is truncated due to the message length limit\n");

	return true;
}

static void improper_alloc_report_handler(struct work_struct *work)
{
	struct hiview_hisysevent *event = NULL;
	int ret;

	if (!g_improper_alloc_report)
		return;

	spin_lock(&g_improper_alloc_report->lock);
	event = hisysevent_create("KERNEL_VENDOR", "LOWMEM", FAULT);
	if (!event) {
		pr_err("failed to create event\n");
		goto report_unlock;
	}

	if (!improper_alloc_create_msg())
		goto report_end;

	ret = hisysevent_put_integer(event, "PID", g_improper_alloc_report->pid);
	ret += hisysevent_put_string(event, "PACKAGE_NAME", g_improper_alloc_report->package_name);
	ret += hisysevent_put_string(event, "PROCESS_NAME", "LARGE_ORDER_WARN");
	ret += hisysevent_put_string(event, "MSG", g_improper_alloc_report->msg);
	ret += hisysevent_put_string(event, "REASON", "LARGE_ORDER");
	if (ret != 0) {
		pr_err("failed to put string to event, ret=%d\n", ret);
		goto report_end;
	}

	ret = hisysevent_write(event);
	if (ret < 0)
		pr_err("failed to write event, ret=%d\n", ret);

report_end:
	hisysevent_destroy(&event);
report_unlock:
	spin_unlock(&g_improper_alloc_report->lock);
}

static void report_stacktrace(void)
{
	unsigned int nr_entries;

	if (!g_improper_alloc_report)
		return;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
	nr_entries = stack_trace_save(g_improper_alloc_report->entries,
				      ARRAY_SIZE(g_improper_alloc_report->entries), REPORT_ENTRIES_TO_SKIP);
#else
	struct stack_trace trace;

	trace.nr_entries = 0;
	trace.max_entries = (unsigned int)ARRAY_SIZE(g_improper_alloc_report->entries);
	trace.entries = g_improper_alloc_report->entries;
	trace.skip = ENTRIES_TO_SKIP;
	save_stack_trace(&trace);
	nr_entries = trace.nr_entries;
#endif
	g_improper_alloc_report->nr_entries = nr_entries;
}

static void improper_alloc_report(gfp_t gfp_mask, unsigned int order, size_t size, const char *type_name)
{
	unsigned long flags;
	int locked, ret;

	if (!g_improper_alloc_report)
		return;

	locked = spin_trylock_irqsave(&g_improper_alloc_report->lock, flags);
	if (!locked)
		return;

	g_improper_alloc_report->pid = current->pid;
	(void)memcpy_s(g_improper_alloc_report->package_name, TASK_COMM_LEN, current->comm, TASK_COMM_LEN);

	g_improper_alloc_report->off = 0;
	ret = snprintf_s(g_improper_alloc_report->msg, MAX_REPORT_MSG_LEN, MAX_REPORT_MSG_LEN - 1,
			 "%s order %u %#x(%pGg) size %zu ", type_name, order, gfp_mask, &gfp_mask, size);
	if (ret < 0) {
		pr_err("failed to create report info, ret=%d\n", ret);
		spin_unlock_irqrestore(&g_improper_alloc_report->lock, flags);
		return;
	}
	g_improper_alloc_report->off = ret;

	report_stacktrace();
	spin_unlock_irqrestore(&g_improper_alloc_report->lock, flags);

	schedule_work(&improper_alloc_report_wk);
}
#endif	/* CONFIG_HISYSEVENT */

static inline bool is_slab_large(size_t size)
{
	return !!size;
}

static inline bool high_order_improper(gfp_t gfp_mask, unsigned int order, size_t size)
{
	return !is_slab_large(size) && order > 0 && !(gfp_mask & __GFP_NORETRY);
}

static inline bool atomic_improper(gfp_t gfp_mask, unsigned int order, size_t size)
{
	return !in_interrupt() && preempt_count() == 0 && (gfp_mask & __GFP_ATOMIC);
}

static inline bool large_slab_improper(gfp_t gfp_mask, unsigned int order, size_t size)
{
	return is_slab_large(size) && ((1 << (order + PAGE_SHIFT)) > size);
}

static inline void high_order_extra_info(gfp_t gfp_mask, unsigned int order, size_t size)
{
#ifdef CONFIG_HISYSEVENT
	improper_alloc_report(gfp_mask, order, size, "high_order");
#else
	dump_stack();
#endif
}

static inline void atomic_extra_info(gfp_t gfp_mask, unsigned int order, size_t size)
{
	pr_info("%s: irq=%s preempt=%d\n", __func__, in_interrupt() ? "i" : "o", preempt_count());
	dump_stack();
}

static inline void large_slab_extra_info(gfp_t gfp_mask, unsigned int order, size_t size)
{
	pr_info("%s: size=%zu waste=%lld\n", __func__, size,
		(long long)(1 << (order + PAGE_SHIFT)) - (long long)size);
	dump_stack();
}

#define DEF_INTERVAL (100 * 60)
#define DEF_BURST 1

#define DEFINE_IMPROPER_ALLOC(x) { \
	.name = __stringify(x), \
	.sum = ATOMIC_LONG_INIT(0), \
	.rl = RATELIMIT_STATE_INIT((x).ratelimit, DEF_INTERVAL * HZ, DEF_BURST), \
	.improper = x##_improper, \
	.extra_info = x##_extra_info, \
}

struct improper_alloc g_improper_types[IMPROPER_COUNT] = {
	DEFINE_IMPROPER_ALLOC(high_order),
#ifndef CONFIG_LIBLINUX
	DEFINE_IMPROPER_ALLOC(atomic),
	DEFINE_IMPROPER_ALLOC(large_slab),
#endif
};
