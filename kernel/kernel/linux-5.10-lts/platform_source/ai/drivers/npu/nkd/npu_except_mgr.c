/*
 * npu_except_mgr.c
 *
 * Central logic for exception raising, dispathcing and handling.
 *
 * Copyright (c) 2012-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "npu_except_mgr.h"

#include <securec.h>
#include "npu_osal.h"
#include "npu_log.h"
#include "bbox/npu_dfx_black_box.h"
#include "npu_platform_adapter.h"
#include "npu_common_resource.h"
#include "npu_atf_subsys.h"
#include "npu_pm_framework.h"
#include "npu_recycle.h"
#include "npu_svm.h"
#ifdef CONFIG_NPU_SINK_UDK
#include "npu_udk.h"
#endif
#ifdef CONFIG_NPU_FFRT
#ifdef CONFIG_NPU_SINK_UDK
#include "ffts_black_box.h"
#else
#include "ffts_hw_config.h"
#endif
#endif
#include "npu_platform_exception.h"
#ifdef CONFIG_NPU_SWTS
#include "schedule_stream_engine.h"
#include "response.h"
#endif

static neh_reply_t npu_save_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	if (er->ex_source == NPU_EXCEPTION_SRC_TS) {
		switch (er->ex_hwts_report.exception_type) {
		case NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION:
		case NPU_EXCEPTION_TYPE_HWTS_TASK_TIMEOUT:
		case NPU_EXCEPTION_TYPE_HWTS_TRAP_EXCEPTION:
			dev_ctx->except_manager.task_info.is_task_exception = 1;
			dev_ctx->except_manager.task_info.persist_stream_id = er->ex_hwts_report.persist_stream_id;
			dev_ctx->except_manager.task_info.persist_task_id = er->ex_hwts_report.persist_task_id;
			dev_ctx->except_manager.task_info.stream_id = er->ex_hwts_report.stream_id;
			dev_ctx->except_manager.task_info.task_id = er->ex_hwts_report.task_id;
			return 0;
		default:
			npu_drv_warn("ts report message type:%u", er->ex_hwts_report.exception_type);
			break;
		}
	}

	dev_ctx->except_manager.task_info.is_task_exception = 0;
	dev_ctx->except_manager.task_info.persist_stream_id = UINT16_MAX;
	dev_ctx->except_manager.task_info.persist_task_id = UINT16_MAX;
	dev_ctx->except_manager.task_info.stream_id = UINT16_MAX;
	dev_ctx->except_manager.task_info.task_id = UINT16_MAX;
	return 0;
}

static void npu_dump_start_time(enum npu_exception_source ex_source)
{
	struct timespec64 tv;
	struct rtc_time tm;
	char log_buf[NPU_BUF_LEN_MAX + 1] = {0};
	int ret = 0;

	ktime_get_real_ts64(&tv);
	tv.tv_sec -= (long)sys_tz.tz_minuteswest * 60;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	rtc_time64_to_tm(tv.tv_sec, &tm);
#else
	rtc_time_to_tm(tv.tv_sec, &tm);
#endif
	ret = snprintf_s(log_buf, NPU_BUF_LEN_MAX + 1, NPU_BUF_LEN_MAX,
		"\r\n%d-%d-%d %d:%d:%d: ex_source=%d\r\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec, ex_source);
	if (ret == 0)
		(void)npu_mntn_copy_reg_to_bbox(log_buf, (unsigned int)strlen(log_buf));
}

static neh_reply_t npu_dump_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	int ret = NPU_EXCEPTION_CONTINUE;
	u32 uid = 0;

	npu_dump_current_voltage();
	npu_dump_start_time(er->ex_source);
	switch (er->ex_source) {
	case NPU_EXCEPTION_SRC_TS:
#ifdef CONFIG_NPU_FFRT
	case NPU_EXCEPTION_SRC_FFTS:
#endif
		ret = npu_plat_dump_exception(dev_ctx, er);
		break;
	case NPU_EXCEPTION_SRC_TASK_TIMEOUT:
		ret = npu_plat_dump_exception(dev_ctx, er);
		if (ret == NPU_EXCEPTION_CONTINUE)
			npu_rdr_exception_report(RDR_EXC_TYPE_TS_RUNNING_TIMEOUT, uid);
		break;
	default:
		npu_drv_debug("unexpected exception source %d\n", er->ex_source);
		break;
	}
	return ret;
}

static neh_reply_t npu_tm_handle_exception(struct npu_dev_ctx *dev_ctx)
{
	struct npu_proc_ctx *proc_ctx = NULL;
	struct npu_proc_ctx *next_proc_ctx = NULL;
#ifdef CONFIG_NPU_SWTS
	struct response_queue *resp_queue = NULL;
#endif

	// TODO mark all the pending tasks, they met exception...

	mutex_lock(&dev_ctx->pm.npu_power_mutex);
	list_for_each_entry_safe(proc_ctx, next_proc_ctx, &dev_ctx->proc_ctx_list,
		dev_ctx_list) {
		mutex_lock(&dev_ctx->pm.task_set_lock);
		if (list_empty_careful(&proc_ctx->task_set) == 0) {
			list_splice_tail_init(&proc_ctx->task_set, &proc_ctx->excpt_task_set);
#ifndef CONFIG_NPU_SWTS
			wake_up(&proc_ctx->report_wait);
#else
			resp_queue = proc_ctx->resp_queue;
			wake_up(&resp_queue->report_wait);
#endif			
		}
		mutex_unlock(&dev_ctx->pm.task_set_lock);
	}
	mutex_unlock(&dev_ctx->pm.npu_power_mutex);

	return NPU_EXCEPTION_HANDLED;
}

static void npu_tm_handle_server_exception(struct npu_dev_ctx *dev_ctx, pid_t server_pid)
{
	struct npu_proc_ctx *proc_ctx = NULL;
	struct npu_proc_ctx *next_proc_ctx = NULL;
#ifdef CONFIG_NPU_SWTS
	struct response_queue *resp_queue = NULL;
#endif
	pid_t tmp_pid = 0;

	// mark clients pending tasks, they met not return report...
	list_for_each_entry_safe(proc_ctx, next_proc_ctx, &dev_ctx->proc_ctx_list,
		dev_ctx_list) {
		mutex_lock(&dev_ctx->pm.task_set_lock);
		npu_get_server_pid(dev_ctx, proc_ctx->pid, &tmp_pid);
		if ((list_empty_careful(&proc_ctx->task_set) == 0) && (tmp_pid == server_pid)) {
			list_splice_tail_init(&proc_ctx->task_set, &proc_ctx->excpt_task_set);
#ifndef CONFIG_NPU_SWTS
			wake_up(&proc_ctx->report_wait);
#else
			resp_queue = proc_ctx->resp_queue;
			wake_up(&resp_queue->report_wait);
#endif			
		}
		tmp_pid = 0;
		mutex_unlock(&dev_ctx->pm.task_set_lock);
	}
}
//
// unified function to dispatch exceptions based on npu_handle_exception of v210
// and the npu_exception_common_proc of others
//
static int npu_dispatch_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	int ret = 0;

	dev_ctx->except_manager.status = NPU_STATUS_EXCEPTION;

	// step 0: save
	ret = npu_save_exception(dev_ctx, er);
	if (ret == NPU_EXCEPTION_HANDLED) {
		npu_drv_warn("exception from %d has handled", er->ex_source);
		goto COMPLETE;
	}

	// step 1: dump
	ret = npu_dump_exception(dev_ctx, er);
	if (ret == NPU_EXCEPTION_HANDLED) {
		npu_drv_warn("exception from %d has handled", er->ex_source);
		goto COMPLETE;
	}

	// step 2: dispatch to npu for platform specific logic
	ret = npu_plat_handle_exception(dev_ctx, er);
	cond_return_error(ret == -1, ret, "npu plat handle exception failed\n");

	if (ret == NPU_EXCEPTION_HANDLED) {
		goto COMPLETE;
	}

	// step 3: dispatch to power manager
	if (er->ex_source != NPU_EXCEPTION_SRC_GLOBAL_REG)
		npu_set_exception_bits();
	ret = npu_pm_handle_exception(dev_ctx, er);

	// step 4: dispatch to task manager
	(void)npu_tm_handle_exception(dev_ctx);

	if (ret != 0)
		return ret;
COMPLETE:
	if (er->ex_source != NPU_EXCEPTION_SRC_SERVER)
		dev_ctx->except_manager.status = NPU_STATUS_NORMAL;
	return ret;
}

void npu_raise_timeout_exception(struct npu_dev_ctx *dev_ctx)
{
	struct npu_exception_record er = {0};

	npu_drv_err("drv receive response wait timeout\n");
	cond_return_void(dev_ctx == NULL, "dev_ctx is null\n");

	er.ex_source = NPU_EXCEPTION_SRC_TASK_TIMEOUT;
	down_write(&dev_ctx->except_manager.status_lock);
	npu_dispatch_exception(dev_ctx, &er);
	up_write(&dev_ctx->except_manager.status_lock);
}

void npu_raise_hwts_exception(struct npu_dev_ctx *dev_ctx, struct hwts_exception_report_info *report)
{
	struct npu_exception_record er = {0};
#ifdef CONFIG_NPU_FFRT
	struct ffts_exception_report_info ffts_report = {0};
#endif
	cond_return_void(report == NULL, "report is NULL\n");

#ifdef CONFIG_NPU_FFRT
	if (report->service_type == NPU_SERVICE_TYPE_FFRT) {
		ffts_report.service_type = NPU_SERVICE_TYPE_FFRT;
		ffts_report.channel_id = report->channel_id;
		ffts_report.exception_type = report->exception_type;
		ffts_report.entry_id = report->entry_id;
		ffts_report.rtsq_idx = report->rtsq_idx;
		er.ex_ffts_report = ffts_report;
		er.ex_source = NPU_EXCEPTION_SRC_FFTS;
		npu_drv_err("get ffts exception report info: "
			"channel_id=%u, exception_type=%u, entry_id=%u, rtsq_idx=%u, model_id=%u\n",
			report->channel_id, report->exception_type, report->entry_id, report->rtsq_idx, report->model_id);
	} else
#endif
	{
		er.ex_hwts_report = *report;
		er.ex_source = NPU_EXCEPTION_SRC_TS;
		npu_drv_err("get hwts exception report info: model_id = %u, persist_stream_id = %u, persist_task_id = %u"
			", channel_id = %u, hwts_sq_id = %u, stream_id = %u, task_id = %u, exception_type = %u\n",
			report->model_id, report->persist_stream_id, report->persist_task_id, report->channel_id,
			report->hwts_sq_id, report->stream_id, report->task_id, report->exception_type);
	}

	down_write(&dev_ctx->except_manager.status_lock);
	npu_dispatch_exception(dev_ctx, &er);

	npu_execute_err_cnt(dev_ctx->devid, report->model_id);
	up_write(&dev_ctx->except_manager.status_lock);
}

void npu_raise_qic_exception(struct npu_dev_ctx *dev_ctx)
{
	struct npu_exception_record er = {0};

	cond_return_void(dev_ctx == NULL, "dev_ctx is null\n");

	er.ex_source = NPU_EXCEPTION_SRC_QIC;
	down_write(&dev_ctx->except_manager.status_lock);
	npu_dispatch_exception(dev_ctx, &er);
	up_write(&dev_ctx->except_manager.status_lock);
}
EXPORT_SYMBOL(npu_raise_qic_exception);

#if defined(CONFIG_NPU_FFRT) && defined(CONFIG_NPU_FFRT_V100)
static void npu_ffts_mode_delayed_powerdown(struct npu_dev_ctx *dev_ctx)
{
	int workmode_idx;
	u32 goal_work_mode_set;
	int ret;
	int trytimes;
	npu_drv_warn("npu ffts mode delayed powerdown start");
	mutex_lock(&dev_ctx->pm.npu_power_mutex);
	for (workmode_idx = NPU_FFRT; workmode_idx <= NPU_FFRT_EFF; workmode_idx++) {
		trytimes = NPU_RETRY_TIMES;
		while (npu_bitmap_get(dev_ctx->pm.work_mode, workmode_idx) != 0) {
			if (ffts_check_global_block_state(FFTS_HW_NPU) != 0) {
				mdelay(1);
			} else {
				goal_work_mode_set = npu_bitmap_clear(dev_ctx->pm.work_mode, workmode_idx);
				ret = npu_ffts_mode_pm_unvote(dev_ctx, (u32)workmode_idx);
				if(ret != 0) {
					npu_drv_err("power down fail: ret = %d, workmode = %d\n",ret, workmode_idx);
				}
				dev_ctx->pm.work_mode = goal_work_mode_set;
				break;
			}
			trytimes--;
			if (trytimes == 0) {
				mutex_unlock(&dev_ctx->pm.npu_power_mutex);
				npu_drv_err("npu dont receive ffts powerdown flag, need reset");
				npu_rdr_exception_report(EXC_TYPE_WAIT_FFTS_EXCEPTION, 0);
				return;
			}
		}
		if (ffts_check_hw_block_state(FFTS_HW_NPU) == HW_BLOCK_COMPLETE)
			break;
	}
	for (workmode_idx = 0; workmode_idx < NPU_WORKMODE_MAX; workmode_idx++) {
		if (workmode_idx == NPU_SEC)
			continue;
		if (npu_bitmap_get(dev_ctx->pm.work_mode, workmode_idx) != 0) {
			goal_work_mode_set = npu_bitmap_clear(dev_ctx->pm.work_mode, workmode_idx);
			ret = npu_ffts_mode_pm_unvote(dev_ctx, (u32)workmode_idx);
			if(ret != 0) {
				npu_drv_err("power down fail: ret = %d, workmode = %d\n",ret, workmode_idx);
			}
			dev_ctx->pm.work_mode = goal_work_mode_set;
		}
	}
	mutex_unlock(&dev_ctx->pm.npu_power_mutex);
	(void)ffts_hw_block_dis(FFTS_HW_NPU);
	npu_drv_warn("npu ffts mode delayed powerdown end");
}
#endif

void npu_raise_global_reg_exception(struct npu_dev_ctx *dev_ctx)
{
	struct npu_exception_record er = {0};
	uint32_t except_bits;

	cond_return_void(dev_ctx == NULL, "dev_ctx is null\n");
	down_write(&dev_ctx->except_manager.status_lock);
	except_bits = (uint32_t)npu_get_exception_bits();
	if (except_bits == UINT32_MAX) {
		npu_drv_err("get exception bits faild");
		up_write(&dev_ctx->except_manager.status_lock);
		return;
	}

	if (dev_ctx->except_manager.status == NPU_STATUS_NORMAL) {
		if ((except_bits & NPU_GLOB_EXCEPT_MASK_LITE) != 0) {
			npu_drv_warn("exception %u found", except_bits);
			er.ex_source = NPU_EXCEPTION_SRC_GLOBAL_REG;
			npu_dispatch_exception(dev_ctx, &er);
		}
	} else {
#if defined(CONFIG_NPU_FFRT) && defined(CONFIG_NPU_FFRT_V100)
		npu_ffts_mode_delayed_powerdown(dev_ctx);
#endif
		// wait tee or tiny power down
		while ((except_bits & NPU_GLOB_EXCEPT_MASK_ALL) != 0) {
			msleep(1);
			except_bits = (uint32_t)npu_get_exception_bits();
		}
		npu_drv_warn("enter exception pu\n");
		(void)npu_pm_handle_power_up(dev_ctx);
		dev_ctx->except_manager.status = NPU_STATUS_NORMAL;
	}
	up_write(&dev_ctx->except_manager.status_lock);
}
EXPORT_SYMBOL(npu_raise_global_reg_exception);

void npu_raise_server_released_exception(struct npu_dev_ctx *dev_ctx, struct npu_proc_ctx *proc_ctx)
{
	struct npu_exception_record er = {0};
#ifndef CONFIG_NPU_SWTS
	int inform_ts = NPU_NO_NEED_TO_INFORM;
#endif

	npu_drv_err("npu server released");
	cond_return_void(dev_ctx == NULL, "dev_ctx is null\n");
	cond_return_void(proc_ctx == NULL, "dev_ctx is null\n");

	er.ex_source = NPU_EXCEPTION_SRC_SERVER;
	down_write(&dev_ctx->except_manager.status_lock);
#ifndef CONFIG_NPU_SWTS
	mutex_lock(&dev_ctx->pm.npu_power_mutex);
	inform_ts = (dev_ctx->pm.ts_work_status == NPU_TS_WORK) ? NPU_NEED_TO_INFORM : NPU_NO_NEED_TO_INFORM;
	if (inform_ts == NPU_NEED_TO_INFORM) {
		npu_send_cancel_mailbox(dev_ctx->devid, proc_ctx->pid);
	}
	npu_tm_handle_server_exception(dev_ctx, proc_ctx->pid);
	mutex_unlock(&dev_ctx->pm.npu_power_mutex);
#else
	npu_dispatch_exception(dev_ctx, &er);
#endif
	npu_recycle_npu_resources(proc_ctx);
	dev_ctx->except_manager.status = NPU_STATUS_NORMAL;
	up_write(&dev_ctx->except_manager.status_lock);
}

// get exception bits in hw register via ATF
int32_t npu_get_exception_bits(void)
{
	int32_t ret;
	struct timespec64 start;
	struct timespec64 stop;
	u64 cost_time;
	u32 feature_mask = 0;

	ret = npu_plat_get_feature_mask(&feature_mask);
	cond_return_error(ret == -1, ret, "npu plat get feature mask bits failed\n");
	if ((feature_mask & NPU_FEATUE_GLOBAL_EXCEPTION_REG) == 0)
		return 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&start);
#else
	start = current_kernel_time();
#endif
	ret = atfd_service_npu_smc(NPU_EXCEPTION_GET_FLAG, 0, 0, 0);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&stop);
#else
	stop = current_kernel_time();
#endif
	cost_time = (u64)((stop.tv_sec * 1000 + stop.tv_nsec / 1000000) - (start.tv_sec * 1000 + start.tv_nsec / 1000000));
	if (ret != 0) {
		npu_drv_warn("get exception bits:0x%x, cost_time:%ull", ret, cost_time);
		return ret;
	}
	if (cost_time > NPU_ATF_TIMEOUT_IN_US)
		npu_drv_err("get exception timeout:%ull", cost_time);

	return ret;
}
EXPORT_SYMBOL(npu_get_exception_bits);

void npu_set_exception_bits(void)
{
	struct timespec64 start;
	struct timespec64 stop;
	u64 cost_time;
	int ret;
	u32 feature_mask = 0;

	npu_drv_debug("set exception bits start");
	ret = npu_plat_get_feature_mask(&feature_mask);
	cond_return_void(ret == -1, "npu plat get feature mask bits failed\n");
	if ((feature_mask & NPU_FEATUE_GLOBAL_EXCEPTION_REG) == 0)
		return;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&start);
#else
	start = current_kernel_time();
#endif
	atfd_service_npu_smc(NPU_EXCEPTION_SET_FLAG, 0, 0, 0);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&stop);
#else
	stop = current_kernel_time();
#endif
	cost_time = (u64)((stop.tv_sec * 1000 + stop.tv_nsec / 1000000) - (start.tv_sec * 1000 + start.tv_nsec / 1000000));

	if (cost_time > NPU_ATF_TIMEOUT_IN_US)
		npu_drv_err("set exception timeout:%ull", cost_time);

	npu_drv_debug("set exception bits end");
}
EXPORT_SYMBOL(npu_set_exception_bits);

void npu_clear_exception_bits(void)
{
	int32_t ret;
	struct timespec64 start;
	struct timespec64 stop;
	u64 cost_time;
	u32 feature_mask = 0;

	npu_drv_debug("clear exception bits start");
	ret = npu_plat_get_feature_mask(&feature_mask);
	cond_return_void(ret == -1, "npu plat get feature mask bits failed\n");
	if ((feature_mask & NPU_FEATUE_GLOBAL_EXCEPTION_REG) == 0)
		return;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&start);
#else
	start = current_kernel_time();
#endif
	ret = atfd_service_npu_smc(NPU_EXCEPTION_CLEAR_FLAG, 0, 0, 0);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	ktime_get_coarse_real_ts64(&stop);
#else
	stop = current_kernel_time();
#endif
	cost_time = (u64)((stop.tv_sec * 1000 + stop.tv_nsec / 1000000) - (start.tv_sec * 1000 + start.tv_nsec / 1000000));

	if (ret != 0) {
		npu_drv_err("clear exception bits failed, ret:%d, cost_time:%ull", ret, cost_time);
		return;
	}
	if (cost_time > NPU_ATF_TIMEOUT_IN_US)
		npu_drv_err("clear exception timeout:%ull", cost_time);

	npu_drv_debug("clear exception bits end");
}
EXPORT_SYMBOL(npu_clear_exception_bits);