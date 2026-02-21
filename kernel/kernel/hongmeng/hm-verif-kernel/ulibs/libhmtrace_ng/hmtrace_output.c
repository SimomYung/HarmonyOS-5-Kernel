/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of hmtrace output
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 03 04:28:02 2024
 */
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_dump.h>
#include <libhmtrace_ng/hmtrace.h>
#include <lib/trace/trace_misc.h>
#include <hmkernel/sched_module.h>

#ifdef CONFIG_TRACEMGR
#define MAX_TCB_STATE_SIZE	3
static char tcb_state_str[MAX_TCB_STATE_SIZE];
const char *hm_trace_tcb_state2str(long state)
{
	switch (state) {
	case TRACE_TCB_STATE_READY:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "R", strlen("R")));
		break;
	case TRACE_TCB_STATE_RUNNING:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "R+", strlen("R+")));
		break;
	case TRACE_TCB_STATE_INTERRUPTIBLE:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "S", strlen("S")));
		break;
	case TRACE_TCB_STATE_UNINTERRUPTIBLE:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "D", strlen("D")));
		break;
	case TRACE_TCB_STATE_DEAD:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "X", strlen("X")));
		break;
	default:
		NOFAIL(strncpy_s(tcb_state_str, MAX_TCB_STATE_SIZE, "?", strlen("?")));
		break;
	}

	return tcb_state_str;
}

#define MAX_SCHED_INFO_SIZE	24
static char sched_info_str[MAX_SCHED_INFO_SIZE];
static const char* trace_sched_info_format[NR_SCHED_INFO] = {
	"%x,",
	"%u,",
	"%u,",
	"%u,",
	"%u,",
	"%u,"
};

typedef unsigned int (*sched_info_get_func)(sched_info_struct *sched_info);
static unsigned int schedinfo_get_affinity(sched_info_struct *sched_info);
static unsigned int schedinfo_get_load(sched_info_struct *sched_info);
static unsigned int schedinfo_get_group(sched_info_struct *sched_info);
static unsigned int schedinfo_get_restricted(sched_info_struct *sched_info);
static unsigned int schedinfo_get_expel(sched_info_struct *sched_info);
static unsigned int schedinfo_get_lt_cgroup_id(sched_info_struct *sched_info);

static sched_info_get_func g_sched_info_get_array[NR_SCHED_INFO] = {
	schedinfo_get_affinity,
	schedinfo_get_load,
	schedinfo_get_group,
	schedinfo_get_restricted,
	schedinfo_get_expel,
	schedinfo_get_lt_cgroup_id
};

static unsigned int schedinfo_get_affinity(sched_info_struct *sched_info)
{
	return sched_info->affinity;
}

static unsigned int schedinfo_get_load(sched_info_struct *sched_info)
{
	/* The load will have a precision loss of 1 */
	return sched_info->load << 1;
}

static unsigned int schedinfo_get_group(sched_info_struct *sched_info)
{
	return sched_info->group;
}

static unsigned int schedinfo_get_restricted(sched_info_struct *sched_info)
{
	return sched_info->restricted;
}

static unsigned int schedinfo_get_expel(sched_info_struct *sched_info)
{
	return sched_info->expel;
}

static unsigned int schedinfo_get_lt_cgroup_id(sched_info_struct *sched_info)
{
	return sched_info->lt_cgroup_id;
}

const char *hm_trace_tcb_unpack_schedinfo(unsigned char *sched_info, unsigned int len)
{
	int offset = 0;
	int err;
	unsigned int res;

	if (len == sizeof(sched_info_struct)) {
		for (int i = 0; i < NR_SCHED_INFO; i++) {
			res = g_sched_info_get_array[i]((sched_info_struct *)sched_info);
			err = snprintf_s(sched_info_str + offset, MAX_SCHED_INFO_SIZE - offset,
					 (size_t)(MAX_SCHED_INFO_SIZE - offset),
					 trace_sched_info_format[i], res);
			BUG_ON_D(err < 0);
			offset += err;
		}

		if (offset > 0) {
			sched_info_str[offset - 1] = '\0';
		}
	} else {
		mem_zero_s(sched_info_str);
	}

	return sched_info_str;
}

static const char *trace_el2_irq_texts[TRACE_EL2_IRQ_TYPE_NUM] = {
	[TRACE_EL2_IRQ_ASSERT_EL1] = "assert el1",
	[TRACE_EL2_IRQ_HOST] = "host irq",
	[TRACE_EL2_IRQ_INJECT] = "inject",
	[TRACE_EL2_IRQ_INJECT_FAIL] = "inject fail",
	[TRACE_EL2_IRQ_INJECT_WITH_PENDING] = "inject w/ P",
	[TRACE_EL2_IRQ_INJECT_WITH_ACTIVE] = "inject w/ A",
	[TRACE_EL2_IRQ_INJECT_WITH_ACTIVE_PENDING] = "inject w/ A&P",
};

const char *hm_trace_el2_irq_type_str(unsigned int type)
{
	const char *str = "invalid type";

	if (type < TRACE_EL2_IRQ_TYPE_NUM) {
		str = trace_el2_irq_texts[type];
	}

	return str;
}

#else
const char *hm_trace_tcb_state2str(long state)
{
	UNUSED(state);

	return NULL;
}

const char *hm_trace_tcb_unpack_schedinfo(unsigned char *sched_info, unsigned int len)
{
	UNUSED(sched_info, len);

	return NULL;
}
const char *hm_trace_el2_irq_type_str(unsigned int type)
{
	UNUSED(type);

	return NULL;
}

#endif /* CONFIG_TRACEMGR */
