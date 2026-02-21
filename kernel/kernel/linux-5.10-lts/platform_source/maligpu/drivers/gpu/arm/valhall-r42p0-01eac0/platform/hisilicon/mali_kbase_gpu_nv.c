/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2014-2022. All rights reserved.
 * Description: This file describe GPU ewp self healing soulution
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * A copy of the licence is included with the program, and can also be obtained
 * from Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 */

#include <securec.h>
#include "mali_kbase.h"
#include "mali_kbase_platform.h"
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>

#if CONFIG_GPU_EWP_HEAL
#define DMD_NV_NUM 512
#define DMD_NV_DATA_SIZE 15

struct gpu_dmd_nv_data {
	union {
		uint8_t value;
		struct {
			uint8_t en      : 1;
			uint8_t sh_mark : 1;
			uint8_t res     : 6;
		} bit;
	} info;

	uint8_t buf[DMD_NV_DATA_SIZE];
};

static int g_check_succ = 0;
static int g_runtime_sh_succ = 0;

static int gpu_dmd_nv_read(struct kbase_device *kbdev, struct gpu_dmd_nv_data *data)
{
	int ret;
	errno_t err;
	struct opt_nve_info_user nve = {0};

	err = strncpy_s(nve.nv_name, sizeof(nve.nv_name),
			"GPUSH", strlen("GPUSH"));
	if (err != EOK || sizeof(*data) > NVE_NV_DATA_SIZE) {
		dev_err(kbdev->dev, "gpu_dmd nv read name cpy fail");
		ret = -1;
		return ret;
	}
	nve.nv_number = DMD_NV_NUM;
	nve.valid_size = (uint32_t)sizeof(*data);
	nve.nv_operation = NV_READ;

	ret = nve_direct_access_interface(&nve);
	if (ret) {
		dev_err(kbdev->dev, "gpu_dmd nv read fail, %d", ret);
		return ret;
	}
	(void)memcpy_s(data, sizeof(*data), nve.nv_data, sizeof(*data));
	return 0;
}

static int gpu_dmd_nv_write(struct kbase_device *kbdev, struct gpu_dmd_nv_data *data)
{
	int ret;
	errno_t err;
	struct opt_nve_info_user nve = {0};

	err = strncpy_s(nve.nv_name, sizeof(nve.nv_name),
		"GPUSH", strlen("GPUSH"));
	if (err != EOK || sizeof(*data) > NVE_NV_DATA_SIZE) {
		dev_err(kbdev->dev, "gpu_dmd nv write name cpy failed");
		ret = -1;
		return ret;
	}
	nve.nv_number = DMD_NV_NUM;
	nve.valid_size = (uint32_t)sizeof(*data);
	nve.nv_operation = NV_WRITE;
	(void)memcpy_s(nve.nv_data, sizeof(*data), data, sizeof(*data));

	ret = nve_direct_access_interface(&nve);
	if (ret) {
		dev_err(kbdev->dev, "gpu_dmd nv write fail, %d", ret);
		return ret;
	}

	return 0;
}

static bool gpu_dmd_need_check(struct kbase_device *kbdev)
{
	u64 *check_ts = &(kbdev->gpu_dev_data.error_num.dmd_check_ts[0]);
	if (check_ts == NULL)
		return false;

	u32 last_idx = (kbdev->gpu_dev_data.error_num.dmd_check_cnt - 1) % MAX_DMD_CHECK_CNT;
	u32 first_idx = (1 + last_idx) % MAX_DMD_CHECK_CNT;

	if (check_ts[last_idx] == 0 || check_ts[first_idx] == 0 ||
		check_ts[last_idx] < check_ts[first_idx])
		return false;
	if ((check_ts[last_idx] - check_ts[first_idx]) <= MAX_DMD_CHECK_INTERVAL)
		return true;

	return false;
}

void gpu_dmd_msg_check(struct kbase_device *kbdev)
{
	u32 check_idx;
	struct kbase_pm_device_data *pm = &kbdev->pm;
	if (pm == NULL)
		return;

	if (g_check_succ != 0)
		return;

	if (atomic_read(&(pm->cs_fault_work_flag)) == 1) {
		dev_err(kbdev->dev, "cs_fault_work is doing\n");
		return;
	}

	check_idx = kbdev->gpu_dev_data.error_num.dmd_check_cnt % MAX_DMD_CHECK_CNT;
	kbdev->gpu_dev_data.error_num.dmd_check_ts[check_idx] = dfx_getcurtime() / 1000000000;
	kbdev->gpu_dev_data.error_num.dmd_check_cnt++;

	/* not need check, Directly self-healing */
	atomic_set(&kbdev->pm.cs_fault_work_flag, 1);
	queue_work(pm->sh_cs_fault_wq, &pm->sh_cs_fault_work);
}

static void kbase_platform_sh_runtime(struct kbase_device *kbdev)
{
	if (kbdev == NULL)
		return;

	if (atomic_read(&(kbdev->pm.runtime_sh_flag)) == 1) {
		dev_warn(kbdev->dev, "run_sh_runtime_flag is 1\n");
		return;
	}
	dev_warn(kbdev->dev, "set run_sh_runtime_flag = 1\n");
	atomic_set(&kbdev->pm.runtime_sh_flag, 1);
	return;
}

u32 kbase_sh_runtime_check(void)
{
	struct kbase_device *kbdev = kbase_platform_get_device();
	if (kbdev == NULL)
		return 0;

	if ((g_runtime_sh_succ == 0) && (atomic_read(&(kbdev->pm.runtime_sh_flag)) == 1)) {
		g_runtime_sh_succ++;
		return 1;
	}
	return 0;
}

void kbase_pm_cs_faults_proc(struct work_struct *work)
{
	struct gpu_dmd_nv_data nv_data = {0};
	struct kbase_device *kbdev = kbase_platform_get_device();
	if (kbdev == NULL)
		return;

	gpu_dmd_nv_read(kbdev, &nv_data);
	dev_err(kbdev->dev, "gpu_dmd nv read, en = %d, mark = %d", nv_data.info.bit.en, nv_data.info.bit.sh_mark);
	if (!nv_data.info.bit.en) {
		dev_err(kbdev->dev, "gpu_dmd nv disable, %d\n", nv_data.info.bit.en);
		goto end;
	}

	if (nv_data.info.bit.sh_mark == 1) {
		/* sh_mark is 1, Do not recover 0 */
			goto end;
	}
	/* mark, write dmd nv to mark value */
	nv_data.info.bit.sh_mark = 1;
	if (gpu_dmd_nv_write(kbdev, &nv_data) == 0) {
		dev_err(kbdev->dev, "gpu_dmd nv mark, sh_mark = %d\n", nv_data.info.bit.sh_mark);
		g_check_succ = 1;
		kbase_platform_sh_runtime(kbdev);
#if defined (CONFIG_HUAWEI_DSM) || defined (CONFIG_HUAWEI_OHOS_DSM)
		gpu_dsm_report(kbdev, GPU_JOB_FAIL_DSM_NO,
				"gpu_dmd nv mark, sh_mark = %d\n", nv_data.info.bit.sh_mark);
#endif
		goto end;
	}

end:
	atomic_set(&kbdev->pm.cs_fault_work_flag, 0);
	return;
}
#endif
