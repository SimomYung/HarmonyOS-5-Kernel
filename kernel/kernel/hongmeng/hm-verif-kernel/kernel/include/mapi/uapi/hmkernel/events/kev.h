/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kev
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 22 10:25:38 2019
 */
#ifndef UAPI_HMKERNEL_KEV_H
#define UAPI_HMKERNEL_KEV_H

#include <hmkernel/mm/udata.h>
#include <hmkernel/events/kev_part.h>

/* remove this when vsync atomic is ready */
#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif
#define __KEV_VERSION	__U(6)

struct kev_notify_event_s {
	__u64 water;
	__u64 threshold;
};

/*
 * note: the struct field be used in klog must before the name,
 * and is not used in klog must after the name.
 */
struct kev_tcb_info_s {
	__s32 pid;
	__s32 tid;
	char tcb_name[HMCAP_THREAD_NAME_SIZE];
};

/* klog data include metainfo such as tcb info and log content */
#define KLOG_TCB_INFO_NAME_LEN(name) strnlen(name, HMCAP_THREAD_NAME_SIZE - 1)
#define KLOG_TCB_INFO_BASE_OFFSET offset_of(struct kev_tcb_info_s, tcb_name[0])

#ifdef CONFIG_KEV_KLOG_TCB_INFO
	/* pid_size + tid_size + name_len + '\0' */
#define KLOG_TCB_INFO_NAME_OFFSET(name) \
		((unsigned int)(KLOG_TCB_INFO_BASE_OFFSET + KLOG_TCB_INFO_NAME_LEN(name) + 1U))
#else
#define KLOG_TCB_INFO_NAME_OFFSET(name) (0U)
#endif

#define KLOG_TCB_INFO_OFFSET(klog_data) \
		KLOG_TCB_INFO_NAME_OFFSET(((struct kev_tcb_info_s *)(klog_data))->tcb_name)

#define KEV_KLOG_DATA_OFFSET offset_of(struct kev_klog_s, data[0])
#define KEV_KLOG_TEXT_OFFSET(klog) (KEV_KLOG_DATA_OFFSET + KLOG_TCB_INFO_OFFSET((klog)->data))
#define KEV_KLOG_TEXT(klog) ((char *)ptr_add(klog, KEV_KLOG_TEXT_OFFSET(klog)))

/*
 * first: sequence number and index of the first valid klog stored in the rb
 * next:  sequence number and index of the next klog to write to the rb
 *        next_idx is the write_head indeed
 * clear: sequence number and index of the next klog to read after the last `clear` command
 */
struct ring_buffer_s {
	vatomic64_t write_head;
	__u64 read_head;
	vatomic32_t readable;
	__u8 mode;
	__u8 state;
	__u16 rsvd1;
	struct kev_notify_event_s notify_event;
	__u64 size;
	vatomic64_t first_seq;
	vatomic64_t next_seq;
	vatomic64_t clear_seq;
	vatomic32_t first_idx;
	vatomic32_t next_idx;
	vatomic32_t clear_idx;
	unsigned char data[] __aligned(sizeof(__u64));
} __attribute__((packed));

#define KEV_MAX_SYSCALL_PARAM 7u
enum kev_syscall_trace_type {
	KEV_SC_ARM64_ENTER,
	KEV_SC_ARM64_EXIT,
	KEV_SC_ARM32_ENTER,
	KEV_SC_ARM32_EXIT,
};

enum kev_vsmmu_trace_type {
	KEV_VSMMU_CMD,
	KEV_VSMMU_EVT,
	KEV_VSMMU_PRI,
	KEV_VSMMU_STE,
};

enum kev_trap_el2_type {
	KEV_EL2_ENTER,
	KEV_EL2_EXIT
};

enum kev_el2_irq_type {
	KEV_EL2_IRQ_ASSERT_EL1,
	KEV_EL2_IRQ_HOST,
	KEV_EL2_IRQ_INJECT,
	KEV_EL2_IRQ_INJECT_FAIL,
	KEV_EL2_IRQ_INJECT_WITH_PENDING,
	KEV_EL2_IRQ_INJECT_WITH_ACTIVE,
	KEV_EL2_IRQ_INJECT_WITH_ACTIVE_PENDING,
	KEV_EL2_IRQ_TYPE_NUM,
};

static const char *kev_el2_irq_texts[KEV_EL2_IRQ_TYPE_NUM] = {
	[KEV_EL2_IRQ_ASSERT_EL1] = "assert el1",
	[KEV_EL2_IRQ_HOST] = "host irq",
	[KEV_EL2_IRQ_INJECT] = "inject",
	[KEV_EL2_IRQ_INJECT_FAIL] = "inject fail",
	[KEV_EL2_IRQ_INJECT_WITH_PENDING] = "inject w/ P",
	[KEV_EL2_IRQ_INJECT_WITH_ACTIVE] = "inject w/ A",
	[KEV_EL2_IRQ_INJECT_WITH_ACTIVE_PENDING] = "inject w/ A&P",
};

static inline const char *el2_irq_type_str(enum kev_el2_irq_type type)
{
	const char *str = "invalid type";

	if (type < KEV_EL2_IRQ_TYPE_NUM) {
		str = kev_el2_irq_texts[type];
	}

	return str;
}

enum __kev_vnotify_type {
	__KEV_VNOTIFY_NOTITY,
	__KEV_VNOTIFY_RECEIVE,
	__KEV_VNOTIFY_EPOLL_CTL,
	__KEV_VNOTIFY_EPOLL_BIND,
	__KEV_VNOTIFY_SELECT,
	__KEV_VNOTIFY_PPOLL,
	__KEV_VNOTIFY_UPDATE_FUTEX,
	__KEV_VNOTIFY_CREATE,
	__KEV_VNOTIFY_DESTROY,
	__KEV_VNOTIFY_TYPE_NUM,
};

static inline const char *vnotify_type_str(enum __kev_vnotify_type type)
{
	const char *str = "invalid type";

	switch (type) {
	case __KEV_VNOTIFY_NOTITY:
		str = "notify";
		break;
	case __KEV_VNOTIFY_EPOLL_CTL:
		str = "epoll ctl";
		break;
	case __KEV_VNOTIFY_EPOLL_BIND:
		str = "epoll bind";
		break;
	case __KEV_VNOTIFY_SELECT:
		str = "select";
		break;
	case __KEV_VNOTIFY_PPOLL:
		str = "ppoll";
		break;
	case __KEV_VNOTIFY_RECEIVE:
		str = "receive";
		break;
	case __KEV_VNOTIFY_UPDATE_FUTEX:
		str = "update futex";
		break;
	case __KEV_VNOTIFY_CREATE:
		str = "create";
		break;
	case __KEV_VNOTIFY_DESTROY:
		str = "destroy";
		break;
	default:
		break;
	}

	return str;
}

/* convert time tock into nsec */
static inline __u64
kev_tock2time(__u64 tock, __u64 tock_start, __u64 freq)
{
	__u64 nsec = 0ULL;
	__u64 tock_pure = tock - tock_start;

	if (freq != 0U) {
		nsec = (tock_pure / freq) * __NSEC_PER_SEC +
		       ((tock_pure % freq) * __NSEC_PER_SEC) / freq;
	}

	return nsec;
}

#endif
