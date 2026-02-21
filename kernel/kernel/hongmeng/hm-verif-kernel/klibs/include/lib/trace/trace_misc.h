/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide interfaces serving for enqueue
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 19:27:39 2023
 */
#ifndef KLIBS_TRACE_MISC_H
#define KLIBS_TRACE_MISC_H
#include <lib/rmqk/rmqk_export.h>
#include <lib/utils.h>
#include <lib/trace/trace_event.h>
#include <hmkernel/capability.h>

#ifdef __KERNEL__
#include <hmkernel/kernel.h>
#define INVALID_VADDR UPTR_MAX
#else
#include <libmem/utils.h>
#include <libmem/types.h>
#endif

enum {
	TRACE_RMQ_DEQUEUE_FUTEX,
	TRACE_RMQ_SHRINK_FUTEX,
	TRACE_SNAPSHOT_RMQ_SHRINK_FUTEX,
	TRACE_RMQ_FUTEX_NR,
};

enum {
	TRACE_SHM_PURPOSE_NORMAL,
	TRACE_SHM_PURPOSE_SNAPSHOT,
	TRACE_SHM_PURPOSE_TASKINFO,
	TRACE_SHM_PURPOSE_FUTEX,
	TRACE_SHM_PURPOSE_FUTEX_NOTIFY,
};

enum trace_tcb_state_e {
	TRACE_TCB_STATE_READY = 0x0,
	TRACE_TCB_STATE_INTERRUPTIBLE = 0x1,
	TRACE_TCB_STATE_UNINTERRUPTIBLE = 0x2,
	TRACE_TCB_STATE_DEAD = 0x10,
	TRACE_TCB_STATE_RUNNING = 0x100,
	TRACE_TCB_STATE_INVALID,
};

struct trace_tcb_state_s {
	char name[HMCAP_THREAD_NAME_SIZE];
	int tid;
	int prio;
	long state;
	int pid;
	bool is_idle;
};

#define INVALID_TP_STRING_KEY	(-1)

enum trace_el2_irq_type {
	TRACE_EL2_IRQ_ASSERT_EL1,
	TRACE_EL2_IRQ_HOST,
	TRACE_EL2_IRQ_INJECT,
	TRACE_EL2_IRQ_INJECT_FAIL,
	TRACE_EL2_IRQ_INJECT_WITH_PENDING,
	TRACE_EL2_IRQ_INJECT_WITH_ACTIVE,
	TRACE_EL2_IRQ_INJECT_WITH_ACTIVE_PENDING,
	TRACE_EL2_IRQ_TYPE_NUM,
};

#define HM_SAVED_CPUS		32U
#define HM_DEFAULT_CMDLINES	0x800
#define HM_MAX_CMDLINES		0x10000
#define HM_MAX_TGIDS		0x10000
#define HM_NO_CMDLINE		0xffffffff
#define HM_TPSTRING_SIZE	1024
#define HM_MAX_STR_NUM		0x400
#define HM_MAX_STR_SIZE		0x8000 /* 32K */
#define SAVEDCMD_UNIT_SIZE	(sizeof(struct trace_tid_cmdline_s))
#define SAVEDTGID_OFFSET	(offset_of(struct trace_saved_taskinfo_s, savedtgid))
#define SAVEDSTRING_OFFSET	(offset_of(struct trace_saved_taskinfo_s, savedstring))
#define SAVEDCMD_OFFSET		(offset_of(struct trace_saved_taskinfo_s, savedcmd))
#define SAVED_TASKINFO_SIZE	\
	(PAGE_ALIGN_UP(sizeof(struct trace_saved_taskinfo_s) + HM_DEFAULT_CMDLINES * SAVEDCMD_UNIT_SIZE))

struct trace_tid_cmdline_s {
	__u32 tid;
	char cmdline[HMCAP_THREAD_NAME_SIZE];
};

struct trace_saved_cmdlines_buffer_s {
	unsigned int map_tid_to_cmdline[HM_MAX_CMDLINES];
	vatomic32_t cmdline_idx;
	vatomic32_t savedcmd_lock;
	/* In the future, the num of saved cmdlines wil be flexible, so we use flexible array here */
	struct trace_tid_cmdline_s saved_cmdlines[];
};

struct trace_savedstring_buffer_s {
	unsigned int str_num;
	size_t offset[HM_MAX_STR_NUM];
	size_t size;
	char data[HM_MAX_STR_SIZE];
};

struct trace_saved_taskinfo_s {
	bool taskinfo_save[HM_SAVED_CPUS];
	unsigned int savedtgid[HM_MAX_TGIDS];
	struct trace_savedstring_buffer_s savedstring;
	struct trace_saved_cmdlines_buffer_s savedcmd;
};

extern void __attribute__((format(printf, 1, 2))) hm_trace_warn(const char *fmt, ...);
extern unsigned int hm_trace_current_cpu_of(void);
extern void *hm_trace_shm_record_addr_of(void);
extern void *hm_trace_shm_savedcmd_addr_of(void);
extern void *hm_trace_shm_savedtgid_addr_of(void);
extern void *hm_trace_shm_savedstring_addr_of(void);
extern void *hm_trace_shm_saved_taskinfo_addr_of(void);
extern __s32 hm_trace_tid_of(bool tcb_event);
extern void hm_trace_rmq_dequeue_futex_wake(void);
extern void hm_trace_set_taskinfo_save(unsigned int cpu, bool enable);
extern void sync_kernel_trace_notify_action(void);
#endif  /* KLIBS_TRACE_MISC_H */
