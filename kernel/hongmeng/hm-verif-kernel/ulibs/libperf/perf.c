/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: libperf entry skeleton
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 6 19:20:08 2022
 */
#include <errno.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libmem/types.h>
#include <libperf/perf.h>
#include <hongmeng/errno.h>
#include <libhmucap/ucap.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/drivers/arm_pmu.h>

#define PERF_START_RETRY_MAX_TIMES 5u

static int attach_cpu_context(cref_t perf_cref, unsigned int cpuid)
{
	unsigned int retry_times = 0;
	int err;

	hm_debug("perf context attach cpu\n");
	do {
		err = syscap_CpuContextAttachCPU(perf_cref, cpuid);
		retry_times++;
	} while (err == E_HM_BUSY && retry_times < PERF_START_RETRY_MAX_TIMES);

	return err;
}

static int pmu_start(enum perf_attach_type attach_type,
		     struct __perf_event_config_s *evt_config,
		     unsigned int cpuid, cref_t *pperf_cref)
{
	int err, ret;
	unsigned int retry_times = 0;
	cref_t perf_cref;

	perf_cref = hm_ucap_new_cpuctx(HM_UCAP_CPUCTX_ARM_PMU);
	hm_info("perf_cref = %"PRIxref"\n", perf_cref);
	if (IS_REF_ERR(perf_cref)) {
		hm_warn("failed to hm_ucap_new_cpuctx: %s\n",
			hmstrerror(REF_TO_ERR(perf_cref)));
		return E_HM_ACCES;
	}

	do {
		err = syscap_CpuContextConfig(perf_cref, PERF_PMU_CONFIG_EVTYPE_ACQUIRE, evt_config,
					      sizeof(struct __perf_event_config_s), NULL, 0);
		retry_times++;
	} while (err == E_HM_BUSY && retry_times < PERF_START_RETRY_MAX_TIMES);

	if (err < 0) {
		hm_warn("config cpu failed : %s\n", hmstrerror(err));
		ret = hm_ucap_delete_cpuctx(perf_cref);
		if (ret != E_HM_OK) {
			hm_warn("delete cpuctx failed, err=%s\n", hmstrerror(ret));
			return ret;
		}
		return err;
	}
	if (attach_type == PERF_ATTACH_CPU) {
		err = attach_cpu_context(perf_cref, cpuid);
		if (err < 0) {
			hm_warn("attach cpu ctx failed: %s\n", hmstrerror(err));
			ret = hm_ucap_delete_cpuctx(perf_cref);
			if (ret != E_HM_OK) {
				hm_warn("delete cpuctx failed, err=%s\n", hmstrerror(ret));
				return ret;
			}
			return err;
		}
	} else {
		err = hm_thread_attach_cpu_context(0, perf_cref, __TCB_ATTACH_TYPE_THREAD);
		if (err < 0) {
			hm_warn("attach thread failed: %s\n", hmstrerror(err));
			ret = hm_ucap_delete_cpuctx(perf_cref);
			if (ret != E_HM_OK) {
				hm_warn("delete cpuctx failed, err=%s\n", hmstrerror(ret));
				return ret;
			}
			return err;
		}
	}

	*pperf_cref = perf_cref;

	return 0;
}

static int dsu_start(struct __perf_event_config_s *evt_config,
		     cref_t *pperf_cref)
{
	int err;
	int ret;
	cref_t perf_cref;

	perf_cref = hm_ucap_new_dsuctx();
	hm_info("perf_cref = %"PRIxref"\n", perf_cref);
	if (IS_REF_ERR(perf_cref)) {
		hm_warn("failed to hm_ucap_new_dsuctx: %s\n",
			hmstrerror(REF_TO_ERR(perf_cref)));
		return E_HM_ACCES;
	}

	err = syscap_DsuContextConfig(perf_cref, 0,
				      __PERF_DSU_PMU_CONFIG_EVTYPE_ACQUIRE,
				      evt_config,
				      sizeof(struct __perf_event_config_s),
				      NULL, 0);
	if (err < 0) {
		hm_warn("config dsu ctx failed : %s\n", hmstrerror(err));
		ret = hm_ucap_delete_dsuctx(perf_cref);
		if (ret != E_HM_OK) {
			hm_warn("delete dsuctx failed, err=%s\n", hmstrerror(ret));
			return ret;
		}
		return err;
	}

	hm_info("perf context start dsu\n");
	err = syscap_DsuContextAttach(perf_cref);
	if (err < 0) {
		hm_warn("dsu context start failed: %s\n", hmstrerror(err));
		ret = hm_ucap_delete_dsuctx(perf_cref);
		if (ret != E_HM_OK) {
			hm_warn("delete dsuctx failed, err=%s\n", hmstrerror(ret));
			return ret;
		}
		return err;
	}

	*pperf_cref = perf_cref;

	return 0;
}

int perf_start(enum perf_attach_type attach_type,
	       enum perf_monitor_type monitor_type,
	       struct __perf_event_config_s *evt_config,
	       unsigned int cpuid, cref_t *pperf_cref)
{
	int err;

	if (attach_type != PERF_ATTACH_CPU && attach_type != PERF_ATTACH_THREAD) {
		hm_error("invalid attach type\n");
		return E_HM_INVAL;
	}

	if (monitor_type == PERF_MONITOR_PMU) {
		err = pmu_start(attach_type, evt_config, cpuid, pperf_cref);
	} else if (monitor_type == PERF_MONITOR_DSU) {
		err = dsu_start(evt_config, pperf_cref);
	} else {
		hm_error("invalid monitor type\n");
		err = E_HM_INVAL;
	}

	return err;
}

static int pmu_stop(cref_t perf_cref, enum perf_attach_type attach_type)
{
	int err;
	unsigned int retry_times = 0;

	if (attach_type == PERF_ATTACH_CPU) {
		do {
			err = syscap_CpuContextDetachCPU(perf_cref);
			retry_times++;
		} while (err == E_HM_BUSY && retry_times < PERF_START_RETRY_MAX_TIMES);

		if (err < 0) {
			hm_warn("perf stat ctx detach cpu failed: %s\n", hmstrerror(err));
			return err;
		}
	} else {
		err = hm_thread_detach_cpu_context(0, NULL, __TCB_DETACH_TYPE_THREAD);
		if (err < 0) {
			hm_warn("perf stat ctx detach thread failed: %s\n", hmstrerror(err));
			return err;
		}
	}

	retry_times = 0;
	do {
		err = syscap_CpuContextConfig(perf_cref, PERF_PMU_CONFIG_EVTYPE_RELEASE,
					      NULL, 0, NULL, 0);
		retry_times++;
	} while (err == E_HM_BUSY && retry_times < PERF_START_RETRY_MAX_TIMES);

	if (err < 0) {
		hm_warn("perf dsuctx release failed: %s\n", hmstrerror(err));
		return err;
	}

	err = hm_ucap_delete_cpuctx(perf_cref);
	if (err < 0) {
		hm_warn("perf del cpuctx ref failed: %s\n", hmstrerror(err));
		return err;
	}

	return 0;
}

static int dsu_stop(cref_t perf_cref)
{
	int err;

	err = syscap_DsuContextDetach(perf_cref);
	if (err < 0) {
		hm_warn("perf dsuctx stop failed: %s\n", hmstrerror(err));
		return err;
	}

	err = syscap_DsuContextConfig(perf_cref, 0,
				      __PERF_DSU_PMU_CONFIG_EVTYPE_RELEASE,
				      NULL, 0, NULL, 0);
	if (err < 0) {
		hm_warn("perf dsuctx release failed: %s\n", hmstrerror(err));
		return err;
	}

	err = hm_ucap_delete_dsuctx(perf_cref);
	if (err < 0) {
		hm_warn("perf del dsuctx cref failed: %s\n", hmstrerror(err));
		return err;
	}

	return err;
}

int perf_stop(enum perf_attach_type attach_type,
	      enum perf_monitor_type monitor_type,
	      cref_t perf_cref)
{
	int err;

	if (attach_type != PERF_ATTACH_CPU && attach_type != PERF_ATTACH_THREAD) {
		hm_error("invalid attach type\n");
		return E_HM_INVAL;
	}

	if (monitor_type == PERF_MONITOR_PMU) {
		err = pmu_stop(perf_cref, attach_type);
	} else if (monitor_type == PERF_MONITOR_DSU) {
		err = dsu_stop(perf_cref);
	} else {
		hm_error("invalid monitor type\n");
		err = E_HM_INVAL;
	}

	return err;
}

#define ULIBS_READ_PMU_RETRY_TIMES	3u
static int pmu_read(cref_t perf_cref, struct __perf_event_ret_s *ret)
{
	int err;
	unsigned int i;

	for (i = 0; i < ULIBS_READ_PMU_RETRY_TIMES; i++) {
		err = syscap_CpuContextConfig(perf_cref, PERF_PMU_CONFIG_EVTCNT_GET,
					      NULL, 0, ret,
					      sizeof(struct __perf_event_ret_s));
		if (err < 0) {
			hm_warn("perf cpuctx read failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}

static int dsu_read(cref_t perf_cref, struct __perf_event_ret_s *ret)
{
	int err;

	err = syscap_DsuContextConfig(perf_cref, 0,
				      __PERF_DSU_PMU_CONFIG_EVTCNT_GET,
				      NULL, 0, ret,
				      sizeof(struct __perf_event_ret_s));
	if (err < 0) {
		hm_warn("perf dsuctx read failed: %s\n", hmstrerror(err));
	}

	return err;
}

int perf_read(enum perf_monitor_type monitor_type,
	      cref_t perf_cref, struct __perf_event_ret_s *ret)
{
	int err;

	if (monitor_type == PERF_MONITOR_PMU) {
		err = pmu_read(perf_cref, ret);
	} else if (monitor_type == PERF_MONITOR_DSU) {
		err = dsu_read(perf_cref, ret);
	} else {
		hm_error("invalid monitor type\n");
		err = E_HM_INVAL;
	}

	return err;
}
