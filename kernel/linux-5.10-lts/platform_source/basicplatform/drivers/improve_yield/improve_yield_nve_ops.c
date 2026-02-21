/*
 * improve_yield_nv_ops.c
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#include <linux/errno.h>
#include <linux/printk.h>
#include <platform_include/basicplatform/linux/improve_yield.h>
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>
#include <securec.h>

#define IMPROVE_YIELD_NV_NUMBER 532
#define IMPROVE_YIELD_NV_NAME "IPR_YLD"

/**
 * @brief Mutex to protect concurrent access to NV storage operations.
 */
struct mutex g_improve_yield_nv_access_mutex;

static unsigned int get_improve_yield_info_offset(enum improve_yield_module_id module_id)
{
	unsigned int offset = 0;
	int          i;

	for (i = MODULE_ID_START; i < (unsigned int)module_id; i++) {
		offset += g_improve_yield_info_map[i];
	}

	return offset;
}

/**
 * @brief Retrieves improve yield information from non-volatile (NV) storage.
 *
 * This function reads specific improve yield data from NV storage based on the provided module ID.
 * It ensures thread safety by using a mutex to protect access to the NV data.
 *
 * @param[in] module_id The identifier of the improve yield module. Must be a valid module ID.
 * @param[out] improve_yield_info A pointer to the buffer where the retrieved data will be stored.
 * @param[in] info_len The length of the buffer. Must be a positive integer and not exceed NVE_NV_DATA_SIZE.
 *
 * @return int
 * - 0 on success.
 * - -EINVAL if any input parameter is invalid.
 * - -ENOMEM if memory operations fail.
 * - -EFAULT if data copy operations fail.
 * - Other error codes from underlying NV access operations.
 */
int get_improve_yield_nv_info(enum improve_yield_module_id module_id, char *improve_yield_info, unsigned int info_len)
{
	struct opt_nve_info_user nve_info;
	int                      ret;

	if (module_id >= MODULE_ID_MAX || improve_yield_info == NULL || info_len == 0 ||
		info_len > g_improve_yield_info_map[module_id]) {
		pr_err("[%s %d]: invalid param\n", __func__, __LINE__);
		return -EINVAL;
	}

	mutex_lock(&g_improve_yield_nv_access_mutex);

	nve_info.nv_operation = NV_READ;
	nve_info.nv_number    = IMPROVE_YIELD_NV_NUMBER;
	nve_info.valid_size   = NVE_NV_DATA_SIZE;

	ret = strncpy_s(nve_info.nv_name, NV_NAME_LENGTH, (char *)IMPROVE_YIELD_NV_NAME, strlen(IMPROVE_YIELD_NV_NAME));
	if (ret != 0) {
		pr_err("[%s %d]: strncpy_s nv name fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = nve_direct_access_interface(&nve_info);
	if (ret != 0) {
		pr_err("[%s %d]: get nv info fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = memset_s(improve_yield_info, info_len, 0, info_len);
	if (ret != 0) {
		pr_err("[%s %d]: memset_s fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = memcpy_s(improve_yield_info, info_len, nve_info.nv_data + get_improve_yield_info_offset(module_id), info_len);
	if (ret != 0) {
		pr_err("[%s %d]: memcpy_s nv data fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = 0;
err:
	mutex_unlock(&g_improve_yield_nv_access_mutex);
	return ret;
}

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
/**
 * @brief Sets the improve yield module information into the Non-Volatile (NV) storage.
 *
 * This function reads the current NV data, updates the specified module's information,
 * and writes the updated data back to NV storage using the nve_direct_access_interface.
 *
 * @param[in] module_id The identifier of the improve yield module.
 * @param[in] improve_yield_info Pointer to the buffer containing the information to be stored.
 * @param[in] info_len The length of the information to be stored.
 *
 * @return int
 *     - 0: Success
 *     - Negative value: Failure (e.g., -EINVAL for invalid parameters, -EFAULT for memory copy failure, -EIO for NV
 *       operation failure)
 */
int set_improve_yield_nv_info(enum improve_yield_module_id module_id, char *improve_yield_info, unsigned int info_len)
{
	struct opt_nve_info_user nve_info;
	int                      ret;

	if (module_id >= MODULE_ID_MAX || improve_yield_info == NULL || info_len == 0 ||
		info_len > g_improve_yield_info_map[module_id]) {
		pr_err("[%s %d]: invalid param\n", __func__, __LINE__);
		return -EINVAL;
	}

	mutex_lock(&g_improve_yield_nv_access_mutex);

	nve_info.nv_operation = NV_READ;
	nve_info.nv_number    = IMPROVE_YIELD_NV_NUMBER;
	nve_info.valid_size   = NVE_NV_DATA_SIZE;

	ret = strncpy_s(nve_info.nv_name, NV_NAME_LENGTH, (char *)IMPROVE_YIELD_NV_NAME, strlen(IMPROVE_YIELD_NV_NAME));
	if (ret != 0) {
		pr_err("[%s %d]: strncpy_s nv name fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = nve_direct_access_interface(&nve_info);
	if (ret != 0) {
		pr_err("[%s %d]: get nv info fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = memcpy_s(nve_info.nv_data + get_improve_yield_info_offset(module_id),
		g_improve_yield_info_map[module_id],
		improve_yield_info,
		info_len);
	if (ret != 0) {
		pr_err("[%s %d]: memcpy_s nv data fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	nve_info.nv_operation = NV_WRITE;

	ret = nve_direct_access_interface(&nve_info);
	if (ret != 0) {
		pr_err("[%s %d]: get nv info fail, ret=%d\n", __func__, __LINE__, ret);
		goto err;
	}

	ret = 0;
err:
	mutex_unlock(&g_improve_yield_nv_access_mutex);
	return ret;
}
#endif
