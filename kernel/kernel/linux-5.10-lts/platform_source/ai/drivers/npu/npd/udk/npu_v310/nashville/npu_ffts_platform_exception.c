#include "npu_ffts_exception_udk.h"
#include "ffts_hw_config.h"
#include "hwts_driver.h"
#include "npu_common.h"
#include "npu_common_resource.h"
#include "npu_exception.h"
#include "npu_except_mgr.h"
#include "npu_hwts_plat.h"
#include "npu_log.h"
#include "npu_platform_exception.h"
#include "npu_soc_defines.h"
#ifdef CONFIG_HISI_NPU_UDK
#include "npu_interfaces.h"
#else
#include "bbox/npu_dfx_black_box.h"
#endif

static void npu_read_entry_info(SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION *ctrl0,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION *info1, SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION *info2,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION *dfx0, int entry_num)
{
	uint64_t hwts_base = npu_hwts_get_base_addr();
 
	cond_return_void(hwts_base == 0ULL, "hwts_base is NULL\n");
 
	ctrl0->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, entry_num));
	info1->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_ADDR(hwts_base, entry_num));
	info2->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_ADDR(hwts_base, entry_num));
	dfx0->value = readl(
		(const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_ADDR(hwts_base));
}
 
static int npu_find_pushtask_entry(SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION *ctrl0,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION *info1, SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION *info2,
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION *dfx0, int type)
{
	int i;
	int tasksync_entry = 0xff;
	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		npu_read_entry_info(ctrl0, info1, info2, dfx0, i);
		if (type == PUSHTASKSYNCALL && ctrl0->reg.task_vld == 1 && info1->reg.opcode == 1 && info2->reg.ssidv == 0) {
			tasksync_entry = i;
			break;
		} else if (type == PUSHTASKSYNC &&
			ctrl0->reg.task_vld == 1 && info1->reg.opcode == 1 && info2->reg.ssidv == 1 && dfx0->value == 0) {
			tasksync_entry = i;
			break;
		}
	}
	npu_drv_warn("tasksync_entry is %d", tasksync_entry);
	return tasksync_entry;
}
 
int npu_handle_global_exception_for_ffts(void)
{
	int ret = 0;
	int cnt = 0;
	int i = 0;
	int trytimes = HWTS_RETRY_TIMES;
	int taskallsync_entry = 0xff;
	struct npu_dev_ctx *dev_ctx = get_dev_ctx_by_id(0);
	uint64_t hwts_base = npu_hwts_get_base_addr();
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_UNION ctrl0 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO1_UNION info1 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_INFO2_UNION info2 = { 0 };
	SOC_NPU_HWTS_HWTS_FFRT_ENTRY_DFX0_UNION dfx0 = { 0 };
 
	npu_drv_warn("npu handle global exception for ffts begin");
	cond_return_error(dev_ctx == NULL, -1, "dev_ctx is null\n");
	cond_return_error(hwts_base == 0ULL, -1, "hwts_base is NULL\n");
 
	// 1. block hw
	(void)ffts_hw_block_en(FFTS_HW_NPU);
 
	// 2. polling TaskSyncAll
	mutex_lock(&dev_ctx->ffrt_entry_mutex);
	trytimes = HWTS_RETRY_TIMES;
	while (cnt < trytimes) {
		taskallsync_entry = npu_find_pushtask_entry(&ctrl0, &info1, &info2, &dfx0, PUSHTASKSYNCALL);
		if (taskallsync_entry == 0xff)
			mdelay(1);
		else
			break;
		cnt++;
	}
 
	// 3. if failed, reset system
	if (taskallsync_entry == 0xff) {
		npu_drv_err("npu don't receive pushtasksyncall signal");
		mutex_unlock(&dev_ctx->ffrt_entry_mutex);
		npu_rdr_exception_report(EXC_TYPE_WAIT_FFTS_EXCEPTION, 0);
		return ret;
	}
 
	// 4. release entries
	for (i = 0; i < MAX_NPU_ENTRY; i++) {
		if (i != taskallsync_entry) {
			npu_read_entry_info(&ctrl0, &info1, &info2, &dfx0, i);
			if (ctrl0.reg.task_vld == 1 && info2.reg.ssidv == 1) {
				npu_drv_warn("ffrt entry %d has task, ctrl0 0x%x, info1 0x%x, info2 0x%x, dfx0 0x%x",
					i, ctrl0.value, info1.value, info2.value, dfx0.value);
				ffts_insert_global_excp_info(info2.reg.ssid, info1.reg.modelid, info1.reg.taskidx);
				// clear entry
				writel(0x1, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, i));
			}
		} else {
			continue;
		}
	}
	writel(0x62, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_ENTRY_CTRL0_ADDR(hwts_base, taskallsync_entry));
	mutex_unlock(&dev_ctx->ffrt_entry_mutex);
 
	// wait notification from ffts driver to power down
	trytimes = HWTS_RETRY_TIMES;
	while (trytimes-- > 0) {
		if (ffts_check_global_block_state(FFTS_HW_NPU) == 0) {
			break;
		} else {
			msleep(10);
		}
	}
	if (ffts_check_hw_block_state(FFTS_HW_NPU) != HW_BLOCK_COMPLETE)
		npu_drv_err("ffts has not receive TaskDoneSyncAll");
	npu_drv_warn("npu handle global exception for ffts end");
	return ret;
}