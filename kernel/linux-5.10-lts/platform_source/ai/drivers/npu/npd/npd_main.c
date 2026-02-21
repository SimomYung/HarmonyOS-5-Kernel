#include <linux/irq.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/cpu.h>
#include <linux/kthread.h>
#include <linux/dma-mapping.h>

#include "npd_interface.h"
#include "npu_platform.h"
#include "npu_adapter.h"
#include "npu_pm_internal.h"
#include "npu_pm_framework.h"
#include "npu_platform_exception.h"
#include "npu_arch_exception.h"
#include "npu_platform_pm.h"
#include "npu_autofs_plat.h"
#include "npu_resmem.h"
#include "npu_mntn.h"
#include "npu_sink_task_verify.h"
#include "npu_sink_sqe_fmt.h"
#include "npu_except_mgr.h"
#include "npu_log.h"
#include "npu_shm.h"
#ifdef CONFIG_NPU_FFRT
#include "npu_ffts_exception.h"
#endif
#ifdef CONFIG_NPU_OHOS
#include "npu_udk.h"
#endif

#ifndef CONFIG_NPU_OHOS
static int npd_dump_exception_ts(struct hwts_exception_report_info *report)
{
	if (report == NULL) {
		npu_drv_warn("exception form ts has no report");
		return NPU_EXCEPTION_CONTINUE;
	}
	if (!is_channel_id_valid(report->channel_id))
		return NPU_EXCEPTION_CONTINUE;

	if (report->service_type == NPU_SERVICE_TYPE_ISPNN) {
		npu_ispnn_exception_dump(report->exception_type, report->channel_id);
		return NPU_EXCEPTION_CONTINUE;
	}
	if (report->exception_type >= NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION
		&& report->exception_type < NPU_EXCEPTION_TYPE_MAX){
		npu_hwts_exception_dump(report->exception_type, report->hwts_sq_id, report->channel_id);
	} else {
		npu_drv_warn("invalid exception_type %d", report->exception_type);
	}
	return NPU_EXCEPTION_CONTINUE;
}

static int npd_dump_exception_without_power_check(struct npu_exception_record *er)
{
	if (er->ex_source == NPU_EXCEPTION_SRC_TASK_TIMEOUT) {
		return npu_timeout_exception_dump();
	} else if (er->ex_source == NPU_EXCEPTION_SRC_TS) {
		return npd_dump_exception_ts(er->u.ex_hwts_report);
#ifdef CONFIG_NPU_FFRT
	} else if (er->ex_source == NPU_EXCEPTION_SRC_FFTS) {
		return npd_dump_exception_ffts(er->u.ex_ffts_report);
#endif
	} else {
		npu_drv_warn("bad exception record with source %d\n", er->ex_source);
	}

	return NPU_EXCEPTION_CONTINUE;
}
#endif

static int npd_dump_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	return npd_dump_exception_with_power_check(dev_ctx, er, npd_dump_exception_without_power_check);
}

int __attribute__((weak)) npd_handle_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
	unused(dev_ctx);
	unused(er);
	return NPU_EXCEPTION_CONTINUE;
}

static int npd_switch_unit(struct npu_dev_ctx *dev_ctx, enum npu_sfu_e sfu, enum npu_workmode_flag mode, void *para)
{
	int ret = 0;
	switch(sfu)
	{
#ifdef CONFIG_NPU_AUTOFS
	case NPU_SFU_AUTOFS:
		if(mode == POWER_ON)
			npu_autofs_on(para);
		else
			npu_autofs_off(para);
		break;
#endif
	case NPU_SFU_HWTS_AICORE_POOL:
		ret = npu_plat_switch_hwts_aicore_pool(dev_ctx, (npu_work_mode_info_t *)para, mode);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

static bool npd_query_sku_value(enum npu_sku_var_e sv)
{
	if(sv == NPU_SKU_VAR_SUPPORT_SC)
		return npu_plat_is_support_sc();
	return false;
}

static int npd_notify(struct npu_dev_ctx *dev_ctx, npd_notice_t pn, long para1, long para2)
{
	int ret = 0;
	unused(para2);

	switch(pn)
	{
	case NPD_NOTICE_PM_INIT:
		ret = npu_pm_init(dev_ctx);
		break;
	case NPD_NOTICE_POWER_UP:
		ret = npu_plat_pm_powerup(dev_ctx, (u32)para1);
		break;
	case NPD_NOTICE_POWER_DOWN:
		ret = npu_plat_pm_powerdown(dev_ctx, (u32)para1);
		break;
	case NPD_NOTICE_SUSPEND:
		ret = npu_plat_dev_pm_suspend();
		break;
	case NPD_NOTICE_MNTN_RESET:
		npu_plat_mntn_reset();
		break;
	case NPD_NOTICE_INFORM_TS_LIMIT_AICORE:
		ret = npu_plat_send_ts_ctrl_core((u32)para1);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

int __attribute__((weak)) npu_format_sw_sink_sqe(npu_model_desc_t *model_desc, void *stream_buf_addr,
	u16 *first_task_id, u8 devid, int stream_idx)
{
	unused(model_desc);
	unused(stream_buf_addr);
	unused(first_task_id);
	unused(devid);
	unused(stream_idx);
	return 0;
}

int __attribute__((weak)) npu_format_hw_sink_sqe(npu_model_desc_info_t *model_desc_info,
	void *stream_buf_addr, void *hwts_sq_addr, u64 ts_sq_len)
{
	unused(model_desc_info);
	unused(stream_buf_addr);
	unused(hwts_sq_addr);
	unused(ts_sq_len);
	return 0;
}

int __attribute__((weak)) npu_nosec_get_workmode(struct npu_proc_ctx *proc_ctx,
	npu_rt_task_t *comm_task)
{
	unused(proc_ctx);
	unused(comm_task);
	return NPU_NONSEC;
}

int __attribute__((weak)) npu_send_ipc_with_power_check(u32 cmd_type, u32 msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type)
{
	unused(cmd_type);
	unused(msg_sync_type);
	unused(send_msg);
	unused(send_len);
	unused(ack_type);
	return 0;
}

#ifdef CONFIG_NPU_FFRT
int __attribute__((weak)) npu_ffts_entry_is_clear(void)
{
	return 0;
}
#endif

struct npd_registry g_npd_registry = {
	.version = NPD_REGISTRY_V1,
	.npd_notify = npd_notify,
	.npd_dump_exception = npd_dump_exception,
	.npd_handle_exception = npd_handle_exception,
	.npd_switch_unit = npd_switch_unit,
	.npd_query_sku_value = npd_query_sku_value,
	.npd_send_mail = npu_plat_res_mailbox_send,
	.npd_set_resmem = npu_plat_set_resmem,
	.npd_verify_sink_tasks = npu_verify_sink_tasks,
	.npd_format_sw_sqe = npu_format_sw_sink_sqe,
	.npd_format_hw_sqe = npu_format_hw_sink_sqe,
	.npd_nosec_get_workmode = npu_nosec_get_workmode,
	.npd_send_ipc_with_power_check = npu_send_ipc_with_power_check,
#ifdef CONFIG_NPU_FFRT
	.npd_ffts_entry_is_clear = npu_ffts_entry_is_clear,
#endif
};

static int __init npd_init(void)
{
	int ret;
	struct npu_dev_ctx *dev_ctx = NULL;

	npu_drv_warn("npd init\n");

	if (npu_plat_bypass_status() == NPU_BYPASS) {
#ifdef CONFIG_NPU_OHOS
		npu_udk_bypass();
#endif
		return 0;
	}

	g_npd_registry.feature_mask = npu_get_feature_bits();

	nkd_register_plat_driver(&g_npd_registry);
	dev_ctx = get_dev_ctx_by_id(0);
	if (dev_ctx == NULL)
		return -1;
	ret = npu_exception_svc_init(dev_ctx);
	cond_return_error(ret != 0, ret, "npu_exception_svc_init_fail, ret = %d\n", ret);
	return 0;
}
module_init(npd_init);

static void __exit npd_exit(void)
{
	struct npu_dev_ctx *dev_ctx = NULL;

	if (npu_plat_bypass_status() == NPU_BYPASS)
		return;

	nkd_unregister_plat_driver();
	dev_ctx = get_dev_ctx_by_id(0);
	if (dev_ctx == NULL)
		return;
	npu_exception_svc_deinit(dev_ctx);
}
module_exit(npd_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NPU Platform Driver(NPD)");
MODULE_VERSION("V1.0");