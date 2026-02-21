/*
 * npu_ldk.c
 *
 * about udk call ldk callback interfaces
 *
 * Copyright (c) 2012-2024 Huawei Technologies Co., Ltd.
 *
 */
#include "npu_ldk.h"
#include <linux/types.h>
#include <liblinux/pal.h>
#include "bbox/npu_dfx_black_box.h"
#include "npu_log.h"
#include "sd_proxy.h"
#ifdef CONFIG_NPU_FFRT
#include "npu_pm_ffap1.h"
#include "npu_platform_adapter.h"
#endif

enum LDK_NPU_MINOR_CMD {
	LDK_MINOR_CMD_NPU_SAVE_BBOX = 0,
	LDK_MINOR_CMD_NPU_RDR_REPORT,
#ifdef CONFIG_NPU_FFRT
	LDK_MINOR_CMD_NPU_HOLD,
	LDK_MINOR_CMD_NPU_RELEASE,
	LDK_MINOR_CMD_NPU_CLEAR_ENTRY,
#endif
	LDK_MINOR_CMD_NPU_MAX,
};

static int npu_ldk_mntn_copy_reg_to_bbox(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	unsigned long bytes_remain = 0;
	char log_buf[NPU_BUF_LEN_MAX + 1] = {0};
	if (rd == NULL || rd_size > NPU_BUF_LEN_MAX) {
		npu_drv_err("npu_ldk_mntn_copy_reg_to_bbox arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}

	bytes_remain = copy_from_user((void *)log_buf, rd, rd_size);
	if (bytes_remain != 0) {
		npu_drv_err("fail to copy arg, bytes_remain = %lu\n", bytes_remain);
		return -EFAULT;
	}

	return npu_mntn_copy_reg_to_bbox(log_buf, (unsigned int)rd_size);
}

struct rdr_report_info{
	uint32_t error_code;
	uint32_t uid;
};
static int npu_ldk_rdr_exception_report(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	unsigned long bytes_remain = 0;
	struct rdr_report_info report_info = {0};
	if (rd == NULL || rd_size != sizeof(report_info)) {
		npu_drv_err("npu_ldk_rdr_report arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}

	bytes_remain = copy_from_user((void *)&report_info, rd, sizeof(report_info));
	if (bytes_remain != 0) {
		npu_drv_err("fail to copy arg, bytes_remain = %lu\n", bytes_remain);
		return -EFAULT;
	}

	npu_rdr_exception_report(report_info.error_code, report_info.uid);
	return 0;
}

#ifdef CONFIG_NPU_FFRT
static int npu_ldk_ffap1_status_hold(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	return npu_ffap1_status_hold();
}
#ifdef CONFIG_NPU_FFRT_V100
static int npu_ldk_proc_block_resource_clear(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	unsigned long bytes_remain = 0;
	uint16_t uid = 0;
	if (rd == NULL || rd_size != sizeof(uint16_t)) {
		npu_drv_err("sd_proxy_npu_cb arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}
	bytes_remain = copy_from_user((void *)&uid, rd, sizeof(uint16_t));
	if (bytes_remain != 0) {
		npu_drv_err("fail to copy arg, bytes_remain = %lu\n", bytes_remain);
		return -EFAULT;
	}
	return npu_plat_proc_block_resource_clear(uid);
}

static int npu_ldk_ffts_clear_entry(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	unsigned long bytes_remain = 0;
	uint16_t task_idx = 0;
	if (rd == NULL || rd_size != sizeof(uint16_t)) {
		npu_drv_err("sd_proxy_npu_cb arg is invalid, rd = 0x%pK, rd_size = %u\n", rd, rd_size);
		return -1;
	}
	bytes_remain = copy_from_user((void *)&task_idx, rd, sizeof(uint16_t));
	if (bytes_remain != 0) {
		npu_drv_err("fail to copy arg, bytes_remain = %lu\n", bytes_remain);
		return -EFAULT;
	}
	return npu_plat_ffts_clear_entry_by_taskidx(task_idx);
}
#endif
#endif

static int npu_udk_call_ldk_cb(unsigned int cmd, void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	unsigned int i = 0;
	unsigned int major_cmd = PAL_PROXY_MAJOR_CMD_OF(cmd);
	unsigned int minor_cmd = PAL_PROXY_MINOR_CMD_OF(cmd);
	npu_drv_debug("npu udk call ldk: major_cmd = %u, minor_cmd = %u.\n", major_cmd, minor_cmd);
	if (major_cmd != NPU_MAJOR_CMD) {
		npu_drv_err("npu callback major_cmd match fail, %u\n", major_cmd);
		return -1;
	}

	struct {
		unsigned int minor_cmd;
		int (*func)(void *rd, size_t rd_size, void *wr, size_t wr_size);
	} cmd_func [] = {
		{ LDK_MINOR_CMD_NPU_SAVE_BBOX,		npu_ldk_mntn_copy_reg_to_bbox },
		{ LDK_MINOR_CMD_NPU_RDR_REPORT,		npu_ldk_rdr_exception_report },
#ifdef CONFIG_NPU_FFRT
		{ LDK_MINOR_CMD_NPU_HOLD,			npu_ldk_ffap1_status_hold },
#ifdef CONFIG_NPU_FFRT_V100
		{ LDK_MINOR_CMD_NPU_RELEASE,		npu_ldk_proc_block_resource_clear },
		{ LDK_MINOR_CMD_NPU_CLEAR_ENTRY,	npu_ldk_ffts_clear_entry },
#endif
#endif
	};

	for (i = 0; i < ARRAY_SIZE(cmd_func); i++) {
		if (minor_cmd == cmd_func[i].minor_cmd) {
			return cmd_func[i].func(rd, rd_size, wr, wr_size);
		}
	}

	npu_drv_err("fail minor_cmd, cmd = %u\n", minor_cmd);
	return -1;
}

void npu_ldk_register_callback(void)
{
	int ret = liblinux_pal_proxy_register(PAL_PROXY_MAJOR(NPU_MAJOR_CMD), npu_udk_call_ldk_cb);
	if (ret != 0) {
		npu_drv_err("npu ldk proxy register callback failed, ret = %d\n", ret);
	}
	npu_drv_warn("npu ldk proxy register callback success.\n");
}