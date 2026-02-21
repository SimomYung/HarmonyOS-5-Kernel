/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Common function implementation of arm pmu
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 25 17:29:29 2018
 */
#include <libhmlog/hmlog.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <libhwsecurec/securec.h>
#include <libperf/perf.h>

#include <libstrict/strict.h>
#include <libperf/arm_pmu_common.h>
#include <hmkernel/attributes/hmattributes.h>
#include <hmasm/pmu.h>

static unsigned int nr_hw_counters = 0U;

#ifndef __PERF_HOST_LLT__
static inline void isb(void)
{
	asm volatile("isb" : : : "memory");
}

static unsigned int arm_pmu_event_counters_num(void)
{
	unsigned int nr = arm_pmu_pmcr_read();
	/* PMCR_EL0.N(RO Field) given the number of event counters */
	return ARM_PMU_PMCR_N_extract(nr);
}
#else
unsigned int arm_pmu_event_counters_num(void);
#endif

static void arm_pmu_init_counters_num(void)
{
	/* Take the default cycles counter into account */
	nr_hw_counters = arm_pmu_event_counters_num() + 1;
	if (nr_hw_counters > HW_MAX_COUNTER_NUM) {
		nr_hw_counters = HW_MAX_COUNTER_NUM;
	}
}

#ifndef __PERF_HOST_LLT__
static inline void arm_pmu_select_counter(unsigned int idx)
{
	unsigned int counter = ARM_PMU_IDX_TO_COUNTER(idx);
	arm_pmu_pmselr_write(counter);
	isb();
}

static inline void arm_pmu_write_evtype(unsigned int idx, unsigned int val)
{
	arm_pmu_select_counter(idx);
	val &= ARM_PMU_EVTYPE_MASK;
	arm_pmu_pmxevtyper_write(val);
}

static inline void arm_pmu_enable_counter(unsigned int idx)
{
	unsigned int counter = ARM_PMU_IDX_TO_COUNTER(idx);
	arm_pmu_pmcntenset_write((uint32_t)BIT(counter));
}

static inline void arm_pmu_disable_counter(unsigned int idx)
{
	unsigned int counter = ARM_PMU_IDX_TO_COUNTER(idx);
	arm_pmu_pmcntenclr_write((uint32_t)BIT(counter));
}

static inline unsigned int arm_pmu_read_event_counter(unsigned int idx)
{
	unsigned int value = 0U;

	if (idx == ARM_PMU_IDX_COUNTER_START) {
		value = (unsigned int)arm_pmu_pmccntr_read();
	} else {
		arm_pmu_select_counter(idx);
		value = arm_pmu_pmxevcntr_read();
	}

	return value;
}
#else
void arm_pmu_write_evtype(unsigned int idx, unsigned int val);
void arm_pmu_enable_counter(unsigned int idx);
void arm_pmu_disable_counter(unsigned int idx);
unsigned int arm_pmu_read_event_counter(unsigned int idx);
#endif

static void arm_pmu_config_event_counter(unsigned int idx, unsigned int eventid)
{
	/* Disable counter */
	arm_pmu_disable_counter(idx);

	/* Set event type. */
	arm_pmu_write_evtype(idx, eventid);

	/* Enable corresponding counter */
	arm_pmu_enable_counter(idx);
}

#ifndef __PERF_HOST_LLT__
static inline unsigned int arm_pmu_clear_overflow_flag(unsigned int idx)
{
	unsigned int counter = ARM_PMU_IDX_TO_COUNTER(idx);
	/* Clear the overflow flag in case an interrupt is pending. */
	arm_pmu_pmovsclr_write((uint32_t)BIT(counter));
	isb();
	return idx;
}
#else
unsigned int arm_pmu_clear_overflow_flag(unsigned int idx);
#endif

static void arm_pmu_disable_event_counter(unsigned int idx)
{
	/* Disable counter and interrupt */
	arm_pmu_disable_counter(idx);
	(void)arm_pmu_clear_overflow_flag(idx);
}

static void arm_pmu_clear_overflow_status(const struct generic_pmu *pmu)
{
	unsigned int idx;
	unsigned int eventbits = pmu->events;
	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; ++idx) {
		if (eventbits & (1U << idx)) {
			(void)arm_pmu_clear_overflow_flag(idx);
		}
	}
}

static int arm_pmu_read_counter(const struct generic_pmu *pmu, unsigned int *stat_array, unsigned int array_len)
{
	unsigned int idx;
	unsigned int eventbits = pmu->events;

	/* user parameters checking. */
	if (stat_array == NULL || (array_len == 0)) {
		hm_error("parameter error: stat_array is NULL or array_len is 0 !\n");
		return -EINVAL;
	}

	/* read hardware counters */
	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; ((idx < nr_hw_counters) &&
					       (idx < array_len)); idx++) {
		if (eventbits & (1U << idx)) {
			unsigned int val;
			val = arm_pmu_read_event_counter(idx);

			stat_array[idx] = val;
		} else {
			stat_array[idx] = 0;
		}
	}

	return 0;
}

static void arm_pmu_write_counter(const struct generic_pmu *pmu)
{
	UNUSED(pmu);
	return;
}

static void arm_pmu_enable(const struct generic_pmu *pmu)
{
	unsigned int idx;
	unsigned int eventbits = pmu->events;

	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; ++idx) {
		if (eventbits & (1U << idx)) {
			arm_pmu_enable_counter(idx);
		}
	}
	return;
}

static void arm_pmu_disable(const struct generic_pmu *pmu)
{
	unsigned int idx;
	unsigned int eventbits = pmu->events;

	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; ++idx) {
		if (eventbits & (1U << idx)) {
			arm_pmu_disable_counter(idx);
		}
	}
	return;
}

static int arm_pmu_config_events(const struct generic_pmu *pmu)
{
	int err;
	unsigned int eventid, idx;
	unsigned int eventbits = pmu->events;

	/* Now config default cycle counter and all event counters */
	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; ++idx) {
		if (eventbits & (1U << idx)) {
			/* Note: should check PMCEIDx_EL0 if event implemented or not */
			err = check_implemented_event(idx, &eventid);
			if (err != -EINVAL) {
				arm_pmu_config_event_counter(idx, eventid);
			} else {
				return err;
			}
		}
	}
	return 0;
}

static void arm_pmu_start(const struct generic_pmu *pmu)
{
	arm_pmu_enable(pmu);

	/* Enable all counters */
#ifndef __PERF_HOST_LLT__
	arm_pmu_pmcr_write(arm_pmu_pmcr_read() | ARM_PMU_PMCR_E(1));
#endif
	return;
}

static void arm_pmu_stop(struct generic_pmu *pmu)
{
	arm_pmu_disable(pmu);

	/* Disable all counters, and corresponding pmovsclr_el0 bits. */
#ifndef __PERF_HOST_LLT__
	arm_pmu_pmcr_write(arm_pmu_pmcr_read() & ~ARM_PMU_PMCR_E(1));
#endif
	arm_pmu_clear_overflow_status(pmu);
	pmu->initialized = false;
	return;
}

static void arm_pmu_reset(const struct generic_pmu *pmu)
{
	unsigned int idx;
	unsigned int eventbits = pmu->events;

	/* The counter and interrupt enable registers are unknown at reset. */
	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; ++idx) {
		if (eventbits & (1U << idx)) {
			arm_pmu_disable_event_counter(idx);
		}
	}

	/* Init & Reset PMNC: C and P bits. warm reset counters to 0.
	 * set long cycle to false to generate overflow interrupt from bit[31].
	 */
#ifndef __PERF_HOST_LLT__
	arm_pmu_pmcr_write(ARM_PMU_PMCR_P(1) | ARM_PMU_PMCR_C(1));
#endif
	return;
}

#define MAX_EVENT_SYMBOL_LEN	40
static int arm_pmu_profilling_report(const struct generic_pmu *pmu, unsigned int *stat_array,
				     unsigned int array_len)
{
	unsigned int idx;
	unsigned stat_idx;
	char name[MAX_EVENT_SYMBOL_LEN];
	unsigned int max_counters;
	unsigned int eventbits = pmu->events;
	unsigned int val;
	int ret;

	/* NOTE: max_counters should be sum of hardware counters and software counters,
	 * at present only hardware counters are supported
	 */
	max_counters = nr_hw_counters;
	if (array_len > max_counters) {
		hm_warn("Warning: array_len=%u exceeding limits=%u\n", array_len,
			max_counters);
		array_len = max_counters;
		hm_warn("Set array_len to Max limits %u\n", max_counters);
	}

	/*
	 * user parameters checking:
	 * stat_array == NULL or array_len ==0 means user doesn't want to get stat data.
	 */
	if (stat_array == NULL || (array_len == 0)) {
		hm_info("Performance counter stats:\n");
	}

	stat_idx = 0;
	/* hardware counters */
	__hmattr_simple_for_loop(HW_MAX_COUNTER_NUM, nr_hw_counters, idx,
		"nr_hw_counters is limited by hardware. Its max value is HW_MAX_COUNTER_NUM(7). "
		"We may use at most 6 event counters and 1 cycles counter simultaneously")
	for (idx = ARM_PMU_IDX_COUNTER_START; idx < nr_hw_counters; idx++) {
		if ((eventbits & (1U << idx)) == 0) {
			if (stat_array != NULL && stat_idx < array_len) {
				stat_array[stat_idx++] = 0;
			}
			continue;
		}

		val = arm_pmu_read_event_counter(idx);

		if (stat_array == NULL || (array_len == 0)) {
			/* user doesn't need to get data back, just print stat data here. */
			mem_zero_a(name);
			ret = strncpy_s(name, MAX_EVENT_SYMBOL_LEN,
					(event_symbols[idx]).symbol,
					(event_symbols[idx]).symlen);
			if (ret != 0) {
				return -EINVAL;
			}
			hm_info("%u %s [%s]\n", val, name,
				perf_type_names[(event_symbols[idx]).type]);
		} else if (stat_idx < array_len) {
			/* copy hw stat to user stat_array */
			stat_array[stat_idx++] = val;
		}
	}

	/* Fixme: software counters handling here */
	return 0;
}

int hw_pmu_init(struct generic_pmu *pmu, const struct perf_evset_attr *attr)
{
	pmu->name = "perf";

	arm_pmu_init_counters_num();

	if (attr->eventbits == 0) {
		pmu->events = HM_HW_PMU_ALLEVENT_BIT;
	} else {
		pmu->events = attr->eventbits & HM_HW_PMU_ALLEVENT_BIT;
	}

	if (pmu->initialized) {
		return -EEXIST;
	}

	pmu->config_events = arm_pmu_config_events;
	pmu->pmu_start = arm_pmu_start;
	pmu->pmu_stop = arm_pmu_stop;
	pmu->reset = arm_pmu_reset;

	/* special wcet related config interface. */
	pmu->pmu_enable = arm_pmu_enable;
	pmu->pmu_disable = arm_pmu_disable;
	pmu->read_counters = arm_pmu_read_counter;
	pmu->write_counters = arm_pmu_write_counter;

	/* generate profilling report without fileio */
	pmu->profilling_report = arm_pmu_profilling_report;

	pmu->initialized = true;
	return 0;
}

int sw_pmu_init(const struct generic_pmu *pmu, const struct perf_evset_attr *attr)
{
	UNUSED(pmu, attr);
	return -ENOSYS;
}
