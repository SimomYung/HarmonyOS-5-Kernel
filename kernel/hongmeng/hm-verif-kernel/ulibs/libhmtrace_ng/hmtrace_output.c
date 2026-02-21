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

const char *hm_trace_el2_irq_type_str(unsigned int type)
{
	UNUSED(type);

	return NULL;
}

#endif /* CONFIG_TRACEMGR */
