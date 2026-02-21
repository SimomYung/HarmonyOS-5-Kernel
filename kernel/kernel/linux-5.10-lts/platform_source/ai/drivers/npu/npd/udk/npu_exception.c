#include "npu_exception.h"

#include "npu_log.h"
#include "npu_common.h"
#include "npu_common_resource.h"
#include "npu_except_mgr.h"
#include "npu_platform_exception.h"
#ifdef CONFIG_NPU_FFRT
#include "npu_ffts_exception_udk.h"
#endif
#include "npu_platform.h"

bool is_channel_id_valid(uint16_t channel_id)
{
#ifdef CONFIG_NPU_HWTS
	if (channel_id >= NPU_HWTS_CHANNEL_NUM) {
		npu_drv_err("invalid param channel_id = %u\n", channel_id);
		return false;
	}
#endif
	return true;
}

bool is_exception_type_valid(uint8_t exception_type)
{
	if (exception_type >= NPU_EXCEPTION_TYPE_HWTS_TASK_EXCEPTION && exception_type < NPU_EXCEPTION_TYPE_MAX) {
		return true;
	}

	npu_drv_err("invalid exception_type %u\n", exception_type);
	return false;
}

static int npd_dump_exception_ts(struct hwts_exception_report_info *report)
{
	npu_hwts_exception_dump(report->exception_type, 0, report->channel_id);
	return NPU_EXCEPTION_CONTINUE;
}

int npd_dump_exception_without_power_check(struct npu_exception_record *er)
{
	npu_drv_warn("exception record with source %d\n", er->ex_source);
	switch (er->ex_source) {
	case NPU_EXCEPTION_SRC_TASK_TIMEOUT:
		return npu_timeout_exception_dump();
	case NPU_EXCEPTION_SRC_TS:
		return npd_dump_exception_ts(&er->ex_hwts_report);
#ifdef CONFIG_NPU_FFRT
	case NPU_EXCEPTION_SRC_FFTS:
		return npd_dump_exception_ffts(&er->ex_ffts_report);
#endif
	default: 
		npu_drv_warn("bad exception record with source %d\n", er->ex_source);
	}

	return NPU_EXCEPTION_CONTINUE;
}