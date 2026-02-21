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

typedef union {
	unsigned int value;
	struct {
		unsigned int opcode : 1;
		unsigned int taskIdx : 16;
		unsigned int error : 1;
		unsigned int process : 1;
        unsigned int reserved : 13;
	} data;
} hwts_task_error_u;

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
	hwts_task_error_u taskerror = {0};
	SOC_NPU_HWTS_HWTS_FFRT_CTRL1_UNION ffrt_ctrl1 = {0};
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
				// HWTS has not returned a response to FFTS
				npu_drv_warn("ffrt entry %d has task, modelid: %d, taskidx: %d",
					i, info1.reg.modelid, info1.reg.taskidx);
				// tell ffts driver to be in the global exception handling process
				taskerror.data.opcode = 0;
				taskerror.data.taskIdx = info1.reg.taskidx;
				taskerror.data.error = 1;
				taskerror.data.process = 0;
				taskerror.data.reserved = 0;
				ffrt_ctrl1.value = readl((const volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_CTRL1_ADDR(hwts_base));
				npu_drv_warn("ffrt_ctrl1.reg.ffrt_task_done: %d", ffrt_ctrl1.reg.ffrt_task_done);
				if (ffrt_ctrl1.reg.ffrt_task_done == 0) {
					writel(taskerror.value, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_CTRL0_ADDR(hwts_base));
					writel(0x1, (volatile void *)(uintptr_t)SOC_NPU_HWTS_HWTS_FFRT_CTRL1_ADDR(hwts_base));
				}
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