/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023.
 * All rights reserved.
 */

#include <linux/kthread.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>
#include <securec.h>

#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_defs.h"
#include "hvgr_regmap.h"
#include "hvgr_gpu_id.h"

#define HVGR_DMD_NV_NUM 512
#define HVGR_DMD_GAF_IDX 20
#define HVGR_DMD_MODE_IDX 28

int hvgr_dmd_nv_read(struct hvgr_device *gdev, struct hvgr_dmd_nv_data *data)
{
	int ret;
	errno_t err;
	struct opt_nve_info_user nve = {0};

	err = strncpy_s(nve.nv_name, sizeof(nve.nv_name),
			"GPUSH", strlen("GPUSH"));
	if (err != EOK || sizeof(*data) > NVE_NV_DATA_SIZE) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv read name cpy fail");
		ret = -1;
		return ret;
	}
	nve.nv_number = HVGR_DMD_NV_NUM;
	nve.valid_size = (uint32_t)sizeof(*data);
	nve.nv_operation = NV_READ;

	ret = nve_direct_access_interface(&nve);
	if (ret) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv read fail, %d", ret);
		return ret;
	}
	ret = memcpy_s(data, sizeof(*data), nve.nv_data, sizeof(*data));
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv cpoy fail");
		return ret;
	}
	return 0;
}

int hvgr_dmd_nv_write(struct hvgr_device *gdev, struct hvgr_dmd_nv_data *data)
{
	int ret;
	errno_t err;
	struct opt_nve_info_user nve = {0};

	err = strncpy_s(nve.nv_name, sizeof(nve.nv_name),
		"GPUSH", strlen("GPUSH"));
	if (err != EOK || sizeof(*data) > NVE_NV_DATA_SIZE) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv write name cpy failed");
		ret = -1;
		return ret;
	}
	nve.nv_number = HVGR_DMD_NV_NUM;
	nve.valid_size = (uint32_t)sizeof(*data);
	nve.nv_operation = NV_WRITE;
	ret = memcpy_s(nve.nv_data, sizeof(*data), data, sizeof(*data));
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv data copy fail");
		return ret;
	}

	ret = nve_direct_access_interface(&nve);
	if (ret) {
		hvgr_err(gdev, HVGR_DM, "higpu_dmd nv write fail, %d", ret);
		return ret;
	}

	return 0;
}

void hvgr_dmd_sr_awake(struct hvgr_device *gdev)
{
	if (gdev->dm_dev.dmd_msg->sr_wakeup_lock != NULL)
		__pm_stay_awake(gdev->dm_dev.dmd_msg->sr_wakeup_lock);
}

void hvgr_dmd_sr_relax(struct hvgr_device *gdev)
{
	if (gdev->dm_dev.dmd_msg->sr_wakeup_lock != NULL)
		__pm_relax(gdev->dm_dev.dmd_msg->sr_wakeup_lock);
}

static int hvgr_dmd_write_gaf(void *data)
{
	struct hvgr_ctx *ctx = (struct hvgr_ctx *)data;
	struct hvgr_device *gdev = NULL;
	struct hvgr_dmd_nv_data nv_data = {0};
	uint32_t dmd_gaf;

	if (ctx == NULL)
		return -1;

	gdev = ctx->gdev;
	mutex_lock(&gdev->dm_dev.dmd_msg->dmd_lock);
	hvgr_dmd_sr_awake(gdev);
	if (hvgr_dmd_nv_read(gdev, &nv_data) != 0)
		goto end;

	if (gdev->dm_dev.dmd_msg->enable_sh_act && gdev->dm_dev.dmd_msg->enable_sh_gaf) {
		dmd_gaf = (uint32_t)atomic_read(&gdev->dm_dev.dmd_msg->dmd_gaf);
		nv_data.buf[HVGR_DMD_GAF_IDX] = (uint8_t)((dmd_gaf >> 24U) & 0xFFU);
		nv_data.buf[HVGR_DMD_GAF_IDX + 1] = (uint8_t)((dmd_gaf >> 16U) & 0xFFU);
		nv_data.buf[HVGR_DMD_GAF_IDX + 2] = (uint8_t)((dmd_gaf >> 8U) & 0xFFU);
		nv_data.buf[HVGR_DMD_GAF_IDX + 3] = (uint8_t)(dmd_gaf & 0xFFU);
		if (hvgr_dmd_nv_write(gdev, &nv_data) != 0)
			goto end;
	}

	hvgr_dmd_sr_relax(gdev);
	mutex_unlock(&gdev->dm_dev.dmd_msg->dmd_lock);
	hvgr_dmd_msg_report(gdev);
	hvgr_dmd_msg_set(gdev, DMD_SH_NV_MARK, "dmd af nv mark");
	hvgr_dmd_msg_report(gdev);
	return 0;
end:
	hvgr_dmd_sr_relax(gdev);
	mutex_unlock(&gdev->dm_dev.dmd_msg->dmd_lock);
	return -1;
}

void hvgr_dmd_set_sid(struct hvgr_ctx *ctx, uint32_t sid)
{
	struct task_struct *tsk = NULL;
	uint32_t dmd_gaf;
	struct hvgr_device *gdev = ctx->gdev;
	struct gpu_dmd_msg *dmd_msg = gdev->dm_dev.dmd_msg;

	if (ctx->gaf_flag != 1)
		return;

	if (!dmd_msg->enable_sh_check)
		return;

	if ((sid < HVGR_MMU_SID_START_OFFSET) || (sid > HVGR_MMU_SID_END_OFFSET)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "sid = %u is invalid!", sid);
		return;
	}

	dmd_gaf = 1u << (sid - HVGR_MMU_SID_START_OFFSET);
	if ((dmd_gaf & (uint32_t)atomic_read(&gdev->dm_dev.dmd_msg->dmd_gaf)) == 1) {
		hvgr_debug(ctx->gdev, HVGR_CQ, "sid = %u already set in nvram!", sid);
		return;
	}

	dmd_gaf = dmd_gaf | (uint32_t)atomic_read(&gdev->dm_dev.dmd_msg->dmd_gaf);
	atomic_set(&gdev->dm_dev.dmd_msg->dmd_gaf, (int)dmd_gaf);
	hvgr_debug(ctx->gdev, HVGR_CQ, "set dmd_gaf = 0x%x", dmd_gaf);

	tsk = kthread_run(hvgr_dmd_write_gaf, (void *)ctx, "dmd_gaf_process");
	if (IS_ERR(tsk))
		hvgr_err(ctx->gdev, HVGR_DM, "higpu_dmd: gaf tsk fail");
}

void hvgr_dmd_config_ctx_gaf(struct hvgr_ctx *ctx, uint32_t flag)
{
	struct hvgr_device *gdev = ctx->gdev;

	if (gdev == NULL || flag != 1)
		return;

	ctx->gaf_flag = flag;
	hvgr_debug(ctx->gdev, HVGR_DM, "process %s in gaf config list!", ctx->process_name);
}

static uint32_t hvgr_dmd_get_reserve_mask(struct hvgr_device * const gdev)
{
	uint32_t switch_mask;
	uint32_t gpc_cores = hvgr_get_cores(gdev);

	switch_mask = (uint32_t)fls(gpc_cores);
	if (switch_mask == 0)
		return 0;
	else
		return (1u << (switch_mask - 1u)) - 1u;
}

static int hvgr_dmd_save_mode_process(void *data)
{
	struct hvgr_device *gdev = (struct hvgr_device *)data;
	struct gpu_dmd_msg *dmd_msg = NULL;
	struct hvgr_pm_pwr *pm_pwr = NULL;
	struct hvgr_dmd_nv_data nv_data = {0};
	union hvgr_dmd_nv_data_gpumode gpumode = {0};
	char log_buf[MAX_DMD_LOG_LENTH] = {0};
	int ret;
	uint32_t res_mask;

	if (unlikely(gdev == NULL))
		return -1;

	dmd_msg = gdev->dm_dev.dmd_msg;
	mutex_lock(&dmd_msg->dmd_lock);
	hvgr_dmd_sr_awake(gdev);
	if (hvgr_dmd_nv_read(gdev, &nv_data) != 0) {
		hvgr_err(gdev, HVGR_DM, "%s read nv failed!", __func__);
		goto end;
	}
	gpumode.value = nv_data.buf[HVGR_DMD_MODE_IDX];
	if (gpumode.bit.gpu_mode == 0x3) {
		hvgr_debug(gdev, HVGR_DM, "%s gpu mode already saved nv!", __func__);
		goto end;
	}
	gpumode.bit.gpu_mode = 0x3;
	nv_data.buf[HVGR_DMD_MODE_IDX] = gpumode.value;
	if (hvgr_dmd_nv_write(gdev, &nv_data) != 0) {
		hvgr_err(gdev, HVGR_DM, "%s write nv failed!", __func__);
		goto end;
	}
	hvgr_dmd_sr_relax(gdev);
	mutex_unlock(&dmd_msg->dmd_lock);

	hvgr_dmd_msg_report(gdev);
	pm_pwr = &gdev->pm_dev.pm_pwr;
	res_mask = hvgr_dmd_get_reserve_mask(gdev);
	ret = sprintf_s(log_buf, MAX_DMD_LOG_LENTH,
		"dmd rda gpu hang nv mark, nve core info is 0x%x",
		pm_pwr->core_mask_cur & res_mask);
	if (ret < 0)
		hvgr_dmd_msg_set(gdev, DMD_SH_HANG_NV_MARK, "dmd change mode nv mark");
	else
		hvgr_dmd_msg_set(gdev, DMD_SH_HANG_NV_MARK, log_buf);
	hvgr_dmd_msg_report(gdev);
	return 0;
end:
	hvgr_dmd_sr_relax(gdev);
	mutex_unlock(&dmd_msg->dmd_lock);
	return -1;
}

static uint32_t hvgr_dmd_get_swicth_mask(struct hvgr_device * const gdev)
{
	uint32_t switch_mask;
	uint32_t gpc_cores = hvgr_get_cores(gdev);

	switch_mask = (uint32_t)fls(gpc_cores);
	if (switch_mask == 0)
		return 0;
	else
		return 1u << (switch_mask - 1u);
}

static uint8_t hvgr_dmd_set_gpumode(void)
{
	struct hvgr_device *gdev = hvgr_get_device();
	struct gpu_dmd_msg *dmd_msg = gdev->dm_dev.dmd_msg;
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;
	struct task_struct *tsk = NULL;
	u32 switch_mask = hvgr_dmd_get_swicth_mask(gdev);

	if (!dmd_msg->enable_sh_mode)
		return 0;

	mutex_lock(&pm_pwr->power_ctrl_lock);
	if (((pm_pwr->core_mask_cur & switch_mask) == 0) ||
		(hweight32(pm_pwr->core_mask_cur) <= 2)) {
		hvgr_err(gdev, HVGR_DM, "%s dest gpc is not available! core_mask_cur=0x%x",
			__func__, pm_pwr->core_mask_cur);
		mutex_unlock(&pm_pwr->power_ctrl_lock);
		return 0;
	}
	mutex_unlock(&pm_pwr->power_ctrl_lock);

	atomic_set(&dmd_msg->dmd_changemode_flag, 1);
	tsk = kthread_run(hvgr_dmd_save_mode_process, (void *)gdev, "dmd_sh_savenv_process");
	if (IS_ERR(tsk))
		hvgr_err(gdev, HVGR_DM, "higpu_dmd: save mode nv tsk fail!");

	return 1;
}

void hvgr_dmd_init_sh(struct hvgr_device *gdev)
{
	struct hvgr_dmd_nv_data data = {0};
	union hvgr_dmd_nv_data_gpumode gpumode = {0};
	union hvgr_dmd_nv_sh_data freq_data = {0};
	struct gpu_dmd_msg *dmd_msg = gdev->dm_dev.dmd_msg;
	uint32_t dmd_gaf = 0;

	if (unlikely(dmd_msg == NULL))
		return;

	atomic_set(&dmd_msg->dmd_gaf, 0);
	dmd_msg->enable_sh_act = false;
	dmd_msg->enable_sh_check = false;
	dmd_msg->enable_sh_gaf = false;
	dmd_msg->enable_sh_freq = false;
	dmd_msg->enable_sh_mode = false;

	if (unlikely(hvgr_dmd_nv_read(gdev, &data) != 0))
		return;

	dmd_msg->enable_sh_act = (data.info.bit.en == 1);
	dmd_msg->enable_sh_check = (data.info.bit.check == 1);
	dmd_msg->enable_sh_gaf = (data.info.bit.gaf_mark == 1);
	if (dmd_msg->enable_sh_act && dmd_msg->enable_sh_gaf) {
		dmd_gaf |= (uint32_t)data.buf[HVGR_DMD_GAF_IDX] << 24U;
		dmd_gaf |= (uint32_t)data.buf[HVGR_DMD_GAF_IDX + 1] << 16U;
		dmd_gaf |= (uint32_t)data.buf[HVGR_DMD_GAF_IDX + 2] << 8U;
		dmd_gaf |= (uint32_t)data.buf[HVGR_DMD_GAF_IDX + 3];
	}
	atomic_set(&dmd_msg->dmd_gaf, (int)dmd_gaf);

	gpumode.value = data.buf[HVGR_DMD_MODE_IDX];
	dmd_msg->enable_sh_mode = (gpumode.bit.en == 1);
	freq_data.value = data.buf[HVGR_DMD_MODE_IDX + 1];
	dmd_msg->enable_sh_freq = (freq_data.bit.en == 1);
	hvgr_debug(gdev, HVGR_DM,
		"sh_act=%d sh_check=%d sh_gaf=%d sh_freq=%d sh_mode=%d dmd_gaf=%d\n",
		dmd_msg->enable_sh_act, dmd_msg->enable_sh_check, dmd_msg->enable_sh_gaf,
		dmd_msg->enable_sh_freq, dmd_msg->enable_sh_mode, atomic_read(&dmd_msg->dmd_gaf));
}
