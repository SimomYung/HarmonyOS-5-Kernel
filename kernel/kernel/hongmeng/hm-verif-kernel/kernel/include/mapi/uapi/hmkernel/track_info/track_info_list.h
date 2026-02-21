/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: track info event list
 * Author: Huawei OS Kernel Lab
 * Create: Thur Jan 4 14:15:14 2024
 */

#include <hmkernel/types.h>

DEFINE_TRACK_INFO(IRQ, irq,
	TRACK_INFO_ARGS(2,
		__u32, irqno,
		__u8, direction),
	TRACK_INFO_STRUCT(2,
		__u32, irqno,
		__u8, direction),
	TRACK_INFO_ASSIGN(
		record->irqno = irqno;
		record->direction = direction),
	TRACK_INFO_PARSE("%llu.%06llu: logic_irq=%u dir=%u\n",
			 record->timestamp / NSEC_PER_SEC,
			 record->timestamp % NSEC_PER_SEC / 1000ULL,
			 record->irqno, record->direction),
	TRACK_INFO_FILE_NAME("last_kirq.txt"))

DEFINE_TRACK_INFO(SCHED_SWITCH, sched_switch,
	TRACK_INFO_ARGS(1,
		struct tcb_s *, next_tcb),
	TRACK_INFO_STRUCT(2,
		__s32, pid,
		char, __TRACK_INFO_STRING(comm, HMCAP_THREAD_NAME_SIZE)),
	TRACK_INFO_ASSIGN(
		str_cpy_p_to_a(record->comm, tcb_name_of(next_tcb));
		record->pid = (__s32)(*tcb_miscdata_of(next_tcb, MISCDATA_TCB_TID))),
	TRACK_INFO_PARSE("%llu.%06llu: comm=%16s tid=%d\n",
			 record->timestamp / NSEC_PER_SEC,
			 record->timestamp % NSEC_PER_SEC / 1000ULL,
			 record->comm, record->pid),
	TRACK_INFO_FILE_NAME("last_ktask.txt"))

DEFINE_TRACK_INFO(CPU_IDLE, cpu_idle,
	TRACK_INFO_ARGS(1, __u32, state),
	TRACK_INFO_STRUCT(1, __u32, state),
	TRACK_INFO_ASSIGN(record->state = state),
	TRACK_INFO_PARSE("%llu.%06llu: state=0x%x\n",
			 record->timestamp / NSEC_PER_SEC,
			 record->timestamp % NSEC_PER_SEC / 1000ULL,
			 record->state),
	TRACK_INFO_FILE_NAME("cpuidle.txt"))

DEFINE_TRACK_INFO(SCHED_CPU_HOTPLUG, sched_cpu_hotplug,
	TRACK_INFO_ARGS(1, __u8, status),
	TRACK_INFO_STRUCT(1, __u8, status),
	TRACK_INFO_ASSIGN(record->status = status),
	TRACK_INFO_PARSE("%llu.%06llu: status=%u\n",
			 record->timestamp / NSEC_PER_SEC,
			 record->timestamp % NSEC_PER_SEC / 1000ULL,
			 record->status),
	TRACK_INFO_FILE_NAME("cpu_onoff.txt"))

DEFINE_TRACK_INFO(IPI, ipi,
	TRACK_INFO_ARGS(2,
		__u32, ipi_type,
		__u32, target),
	TRACK_INFO_STRUCT(2,
		__u32, ipi_type,
		__u32, target),
	TRACK_INFO_ASSIGN(
		record->ipi_type = ipi_type;
		record->target = target),
	TRACK_INFO_PARSE("%llu.%06llu: type=%u target=0x%x\n",
		record->timestamp / NSEC_PER_SEC,
		record->timestamp % NSEC_PER_SEC / 1000ULL,
		record->ipi_type, record->target),
	TRACK_INFO_FILE_NAME("last_ipi.txt"))

DEFINE_TRACK_INFO(TICKLESS, tickless,
	TRACK_INFO_ARGS(2,
		struct tcb_s *, tcb,
		int, tickless),
	TRACK_INFO_STRUCT(4,
		char, __TRACK_INFO_STRING(comm, HMCAP_THREAD_NAME_SIZE),
		__u32, pid,
		__u32, tid,
		__s32, tickless),
	TRACK_INFO_ASSIGN(
		str_cpy_p_to_a(record->comm, tcb_name_of(tcb));
		record->pid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_PID), __s32));
		record->tid = *(ptr_cast(tcb_miscdata_of(tcb, MISCDATA_TCB_TID), __s32));
		record->tickless = tickless),
	TRACK_INFO_PARSE("%llu.%06llu: comm=%16s pid=%u tid=%u tickless=%d\n",
		record->timestamp / NSEC_PER_SEC,
		record->timestamp % NSEC_PER_SEC / 1000ULL,
		record->comm,
		record->pid,
		record->tid,
		record->tickless),
	TRACK_INFO_FILE_NAME("last_tickless.txt"))

DEFINE_TRACK_INFO(GIC_PMR, gic_pmr,
	TRACK_INFO_ARGS(1,
		__u32, icc_pmr_el1),
	TRACK_INFO_STRUCT(1,
		__u32, icc_pmr_el1),
	TRACK_INFO_ASSIGN(
		record->icc_pmr_el1 = icc_pmr_el1),
	TRACK_INFO_PARSE("%llu.%06llu: type=%u\n",
		record->timestamp / NSEC_PER_SEC,
		record->timestamp % NSEC_PER_SEC / 1000ULL,
		record->icc_pmr_el1),
	TRACK_INFO_FILE_NAME("last_gicpmr.txt"))

DEFINE_TRACK_INFO(USER_SMP_CALL, user_smp_call,
	TRACK_INFO_ARGS(2,
		__u8, dir,
		__u32, target),
	TRACK_INFO_STRUCT(2,
		__u8, dir,
		__u32, target),
	TRACK_INFO_ASSIGN(
		record->dir = dir;
		record->target = target),
	TRACK_INFO_PARSE("%llu.%06llu: dir=%u target=0x%x\n",
			 record->timestamp / NSEC_PER_SEC,
			 record->timestamp % NSEC_PER_SEC / 1000ULL,
			 record->dir, record->target),
	TRACK_INFO_FILE_NAME("last_usmpcall.txt"))
