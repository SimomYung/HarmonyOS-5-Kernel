/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: libperf entry skeleton
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 26 16:00:08 2018
 */
#include <errno.h>
#include <libperf/perf.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libmem/types.h>
#include <hongmeng/errno.h>
#include <libhmucap/ucap.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/drivers/arm_pmu.h>
#include <libperf/arm_pmu_common.h>

static struct generic_pmu generic_pmu = {0};

static cref_t perf_ctx;

static int detach_perf_ctx(enum perf_attach_type type)
{
	int err;

	if (type == PERF_ATTACH_CPU) {
		err = syscap_CpuContextDetachCPU(perf_ctx);
		if (err < 0) {
			hm_warn("perf stat ctx detach cpu failed: %s\n", hmstrerror(err));
			return -EFAULT;
		}
	} else {
		err = hm_thread_detach_cpu_context(0, NULL, __TCB_DETACH_TYPE_THREAD);
		if (err < 0) {
			hm_warn("perf stat ctx detach thread failed: %s\n", hmstrerror(err));
			return -EFAULT;
		}
	}
	return 0;
}

static int attach_perf_ctx(enum perf_attach_type type)
{
	int err;

	perf_ctx = hm_ucap_new_cpuctx(HM_UCAP_CPUCTX_ARM_PMU);
	hm_info("perf_ctx = %"PRIxref"\n", perf_ctx);
	if (IS_REF_ERR(perf_ctx)) {
		hm_warn("failed to hm_ucap_new_cpuctx: %s\n",
			 hmstrerror(REF_TO_ERR(perf_ctx)));
		return -EACCES;
	}

	err = syscap_CpuContextConfig(perf_ctx, ARM_PMU_CONFIG_USER_ENABLE,
				       NULL, 0, NULL, 0);
	hm_info("syscap_CpuContextConfig returns %d, err=(%s)\n", err, hmstrerror(err));

	if (type == PERF_ATTACH_CPU) {
		hm_info("perf context attach cpu\n");
		err = syscap_CpuContextAttachCPU(perf_ctx, 0);
		if (err < 0) {
			hm_warn("attach cpu failed: %s\n", hmstrerror(err));
			return -EFAULT;
		}
	} else {
		hm_info("perf context attach thread\n");
		err = hm_thread_attach_cpu_context(0, perf_ctx, __TCB_ATTACH_TYPE_THREAD);
		if (err < 0) {
			hm_warn("attach thread failed: %s\n", hmstrerror(err));
			return -EFAULT;
		}
	}
	return 0;
}

static int perf_profilling_report(const struct generic_pmu *pmu,
				  unsigned int *stat_array, unsigned int array_len)
{
	int err = 0;

	if (pmu->profilling_report) {
		err = pmu->profilling_report(pmu, stat_array, array_len);
	}

	return err;
}

static void perf_start_profilling(struct generic_pmu *pmu)
{
	pmu->state = PERF_STATE_RUNNING;

	/*
	 * on a warm reset, PMU registers reset to architecturally unknown value,
	 * before starting profile, restore registers to initial value.
	 */
	if (pmu->reset) {
		pmu->reset(pmu);
	}

	/* call driver to start profill countering. */
	if (pmu->pmu_start) {
		pmu->pmu_start(pmu);
	}
}

static void perf_stop_profilling(struct generic_pmu *pmu)
{
	pmu->state = PERF_STATE_STOPPED;

	/* call driver to stop countering. */
	if (pmu->pmu_stop) {
		pmu->pmu_stop(pmu);
	}
}

static int perf_config_events(const struct generic_pmu *pmu)
{
	int err = 0;

	/* config_events gonna set default attrs right now. */
	if (pmu->config_events) {
		err = pmu->config_events(pmu);
	}

	return err;
}

int hm_perf_start(const struct perf_evset_attr *attr)
{
	int err;
	int err_ctx;
	struct generic_pmu *pmu = &generic_pmu;

	if (attr == NULL || !(attr->type & PERF_TYPE_ALL) || attr->eventbits >= HM_HW_PMU_ALLEVENT_BIT) {
		hm_warn("invalid args of hm_perf_start \n");
		return -EINVAL;
	}

	/* check attach type */
	if (attr->attach_type >= PERF_ATTACH_MAX) {
		hm_warn("invalid attach type\n");
		return -EINVAL;
	}

	RAW_MUTEX_GUARD(perf_mtx, &(pmu->mtx));

	/* set perf attach type */
	pmu->attach_type = attr->attach_type;

	err_ctx = attach_perf_ctx(attr->attach_type);
	hm_info("result of attach perfctx: %d\n", err_ctx);
	if (err_ctx != 0) {
		return err_ctx;
	}

	if (attr->type & PERF_TYPE_HARDWARE) {
		err = hw_pmu_init(pmu, attr);
		/* support calling sys_perf_start multi times */
		if (err != 0 && err != -EEXIST) {
			err_ctx = detach_perf_ctx(attr->attach_type);
			hm_info("result of detach perfctx: %d\n", err_ctx);
			return err;
		}
	}
	if (attr->type & PERF_TYPE_SOFTWARE) {
		err = sw_pmu_init(pmu, attr);
		if (err != 0 && err != -EEXIST) {
			err_ctx = detach_perf_ctx(attr->attach_type);
			hm_info("result of detach thread: %s\n", hmstrerror(err_ctx));
			return err;
		}
	}

	if (perf_config_events(pmu) != 0) {
		hm_warn("[PERF ERROR] perf_config_events error\n");
	}

	/* the performance counters should be working now, any check? */
	hm_info("[PERF INFO] start profilling\n");
	perf_start_profilling(pmu);

	return 0;
}

int hm_perf_stop(unsigned int *stat_array, unsigned int array_len)
{
	int err;
	struct generic_pmu *pmu = &generic_pmu;

	RAW_MUTEX_GUARD(perf_mtx, &(pmu->mtx));
	/* here stop PMU on current cpu, and set profilling_stop_flag
	 * to stop PMU on other cpu(s).
	 */
	perf_stop_profilling(pmu);

	/* handle perf stat to transfer stat data to EL0 array */
	perf_profilling_report(pmu, stat_array, array_len);

	err = detach_perf_ctx(pmu->attach_type);
	hm_info("result of detach thread: %s\n", hmstrerror(err));

	/* NOTE:
	 *   - delete perf cap by ucap_new_cap
	 *   - delete pmem of perf cap
	 * But now, delete pmem and delete cap are not supported
	 */
	return 0;
}

int hm_perf_profilling_report(unsigned int *stat_array, unsigned int array_len)
{
	struct generic_pmu *pmu = &generic_pmu;

	RAW_MUTEX_GUARD(perf_mtx, &(pmu->mtx));
	return perf_profilling_report(pmu, stat_array, array_len);
}

#ifdef CONFIG_WCET_PLUGIN
/*
 * The following 3 interface:
 * 1. hm_perf_enable
 * 2. hm_perf_disable
 * 3. perf_read_hw_counters
 * are special plugin functions for wcet.so
 */
void hm_perf_enable(void)
{
	struct generic_pmu *pmu = &generic_pmu;

	if (pmu->pmu_enable)
		pmu->pmu_enable(pmu);
}

void hm_perf_disable(void)
{
	struct generic_pmu *pmu = &generic_pmu;

	if (pmu->pmu_disable)
		pmu->pmu_disable(pmu);
}

void hm_perf_read_counters(unsigned int *stat_array, unsigned int array_len)
{
	struct generic_pmu *pmu = &generic_pmu;

	if (pmu->read_counters)
		pmu->read_counters(pmu, stat_array, array_len);
}
#endif
