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
#ifdef CONFIG_NPU_SWTS
#include "npu_platform_exception_spec.h"
#include "npu_except_mgr.h"
#include "npu_common.h"
#endif
#include "npd_interface.h"
#include "npu_platform.h"
#include "npu_adapter.h"
#include "npu_pm_internal.h"
#include "npu_pm_framework.h"
#include "npu_exception.h"
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
#include "npu_ipc.h"
#ifdef CONFIG_NPU_SINK_UDK
#include "npu_udk.h"
#else
#ifdef CONFIG_NPU_FFRT
#include "npu_ffts_exception_udk.h"
#include "npu_ffts_model.h"
#endif
#endif
#if defined(CONFIG_NPU_FFRT) && defined(CONFIG_NPU_FFRT_V100)
#include "npu_ffts_exception_ldk.h"
#endif

static int npd_dump_exception(struct npu_dev_ctx *dev_ctx, struct npu_exception_record *er)
{
#ifdef CONFIG_NPU_SWTS
	switch(er->ex_hwts_report.exception_type) {
		case NPU_EXCEPTION_TYPE_HWTS_SQE_ERROR:
			npu_exception_sqe_error_proc(dev_ctx, er->ex_hwts_report.hwts_sq_id, er->ex_hwts_report.channel_id);			
			return 0;
		default:
			return npd_dump_exception_with_power_check(dev_ctx, er, npd_dump_exception_without_power_check);
	}
#endif
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

int __attribute__((weak)) npu_send_ipc_with_power_check(u32 cmd_type, enum ipc_msg_sync_type msg_sync_type,
	const u8 *send_msg, u32 send_len, enum ipc_send_ack_type ack_type)
{
	unused(cmd_type);
	unused(msg_sync_type);
	unused(send_msg);
	unused(send_len);
	unused(ack_type);
	return 0;
}

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
#ifdef CONFIG_NPU_FFRT_V100
	.npd_ffts_clear_entry_by_taskidx = npu_ffts_clear_entry_by_taskidx,
	.npd_proc_block_resource_clear = npu_proc_block_resource_clear,
#endif
	.npd_ffrt_load_model_buf = npu_ffrt_load_model_buf,
	.npd_ffrt_set_ssid = npu_ffrt_set_ssid,
#endif
};

static int __init npd_init(void)
{
	int ret;
	struct npu_dev_ctx *dev_ctx = NULL;

	npu_drv_warn("npd init\n");

	if (npu_plat_bypass_status() == NPU_BYPASS) {
#ifdef CONFIG_NPU_SINK_UDK
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

	dev_ctx = get_dev_ctx_by_id(0);
	if (dev_ctx == NULL)
		return;
	npu_exception_svc_deinit(dev_ctx);
	nkd_unregister_plat_driver();
}
module_exit(npd_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NPU Platform Driver(NPD)");
MODULE_VERSION("V1.0");