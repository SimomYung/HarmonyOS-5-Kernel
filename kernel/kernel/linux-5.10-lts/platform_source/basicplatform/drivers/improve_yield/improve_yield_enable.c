/*
 * improve_yield_enable.c
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

#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <platform_include/basicplatform/linux/improve_yield.h>
#include <securec.h>
#include "improve_yield_internal.h"

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
/**
 * Check if a specific module's yield improvement feature is enabled.
 *
 * This function retrieves the enable/disable status of yield improvement for a given module
 * by reading non-volatile (NV) configuration information. It checks the corresponding bit
 * in the retrieved data to determine if the feature is enabled for the specified module.
 *
 * @param[in] module_id   The ID of the module to check, defined in improve_yield_module_id enum.
 *
 * @return            1 if the yield improvement feature is enabled for the specified module,
 *                    0 if disabled, or if an error occurs during NV data retrieval.
 *                    On error, logs an error message with function name, line number, and error code.
 *
 * @note              - Uses get_improve_yield_nv_info() to access persistent configuration data.
 *                    - The 'enabale_info' variable (presumably "enable_info") stores bitwise flags
 *                      where each bit represents the enable status of a module.
 *                    - Returns 0 as a safe default if NV data retrieval fails.
 *                    - Ensure module_id values align with the bit positions in enabale_info.
 */
int is_module_improve_yield_enable(enum improve_yield_module_id module_id)
{
	unsigned int enabale_info;
	int          ret;

	if (module_id >= MODULE_ID_MAX) {
		pr_err("[%s %d] invalid module_id\n", __func__, __LINE__);
        return 0;
	}

	ret = get_improve_yield_nv_info(MODULE_ID_ENABLE, (char *)&enabale_info, IMPROVE_YIELD_ENABLE_INFO_SIZE);
	if (ret != 0) {
		pr_err("[%s %d] get enabale info failed, ret=%d\n", __func__, __LINE__, ret);
		return 0;
	}

	return (enabale_info & (1 << module_id) ? 1 : 0);
}

/**
 * @brief Get the module ID by matching a given module name.
 *
 * This function searches through the predefined module names in the global
 * lookup table `g_module_name_table` to find a match with the provided `module_name`.
 *
 * @param module_name Pointer to the input module name string to search for
 * @param name_len    Length of the module name to compare (prevents over-read)
 *
 * @return enum improve_yield_module_id
 *         - Corresponding module ID if a match is found
 *         - MODULE_ID_MAX if no matching module is found
 *
 * @note The comparison uses strncmp() for safe length-limited string comparison.
 *       Ensure name_len accounts for null-terminator if needed.
 */
static enum improve_yield_module_id get_module_id_by_name(const char *module_name, size_t name_len)
{
	int                          i;
	enum improve_yield_module_id module_id = MODULE_ID_MAX;

	if (module_name == NULL || name_len == 0) {
		pr_err("[%s %d] invalid input\n", __func__, __LINE__);
		return MODULE_ID_MAX;
	}

	for (i = MODULE_ID_START; i < MODULE_ID_MAX; i++) {
		if (strncmp(module_name, g_improve_yield_module_list[i], name_len) == 0) {
			module_id = (enum improve_yield_module_id)i;
			break;
		}
	}

	return module_id;
}

/**
 * @brief Parse key-value pairs from input buffer to generate module enable masks
 *
 * This function processes a space-separated string of "key=value" pairs to:
 * 1. Generate a module mask indicating which modules are referenced
 * 2. Generate a value mask indicating enable(1)/disable(0) states
 *
 * @param[in] buf         Input buffer containing configuration string
 * @param[in] count       Length of input buffer
 * @param[out] module_mask Bitmask where each bit represents a referenced module
 * @param[out] enable_mask Bitmask where each bit represents a module's enable state
 *
 * @return int
 *         - 0 on success
 *         - -EINVAL for invalid input/format
 *         - -ENOMEM for memory allocation failure
 *
 * @note Behavior details:
 * - Input format: "module1=0 module2=1 ..."
 * - Values must be 0 (disable) or 1 (enable)
 * - Newline characters are replaced with spaces
 * - Uses strim() to trim whitespace around keys/values
 * - Invalid entries produce errors but processing continues
 * - Duplicate module entries are OR-combined in masks
 * - Module IDs are resolved via get_module_id_by_name()
 */
int get_enable_mask(char *buf, size_t count, unsigned int *module_mask, unsigned int *enable_mask)
{
	char                        *input, *token, *key, *value, *eq_pos;
	int                          num_val, ret = 0;
	enum improve_yield_module_id module_id = MODULE_ID_MAX;

	if (buf == NULL || count == 0 || module_mask == NULL || enable_mask == NULL) {
		pr_err("[%s %d] invalid input\n", __func__, __LINE__);
		return -EINVAL;
	}

	// Copy the input and ensure that the input ends with NULL.
	input = kstrndup(buf, count, GFP_KERNEL);
	if (!input)
		return -ENOMEM;

	*module_mask = 0;
	*enable_mask = 0;

	while ((token = strsep(&input, " ")) != NULL) {
		if (!*token)
			continue;

		eq_pos = strchr(token, '=');
		if (!eq_pos) {
			pr_err("[%s %d] invalid token: %s\n", __func__, __LINE__, token);
			ret = -EINVAL;
			continue;
		}

		// Split the key value.
		*eq_pos = '\0';
		key     = strim(token);
		value   = strim(eq_pos + 1);

		pr_info("[%s %d] key:[%s], value:[%s]\n", __func__, __LINE__, key, value);

		// Resolve the value.
		ret = kstrtoint(value, 10, &num_val);
		if (ret != 0 || num_val < 0 || num_val > 1) {
			pr_err("[%s %d] invalid value: %s\n", __func__, __LINE__, value);
			ret = -EINVAL;
			continue;
		}

		// Resolve the module ID.
		module_id = get_module_id_by_name(key, strlen(key));
		if (module_id < MODULE_ID_MAX) {
			*module_mask |= (1 << module_id);
			*enable_mask |= ((num_val != 0) << module_id);
		} else {
			pr_err("[%s %d] invalid module:[%s]\n", __func__, __LINE__, key);
			ret = -EINVAL;
		}
	}

	kfree(input);
	return ret;
}

int get_enable_info_str(char *buf, unsigned int *len)
{
	unsigned int enabale_info;
	char         module_enabale_str[IMPROVE_YIELD_INFO_LEN_MAX] = "";
	int          ret, i, str_len;

	if (buf == NULL || len == NULL) {
		pr_err("[%s %d] invalid input\n", __func__, __LINE__);
		return -EINVAL;
	}

	*len = 0;
	memset_s(buf, INFO_MAX_LEN, 0, INFO_MAX_LEN);

	ret = get_improve_yield_nv_info(MODULE_ID_ENABLE, (char *)&enabale_info, IMPROVE_YIELD_ENABLE_INFO_SIZE);
	if (ret != 0) {
		pr_err("[%s %d] get enabale info failed, ret=%d\n", __func__, __LINE__, ret);
		return ret;
	}

	for (i = 0; i < MODULE_ID_MAX; i++) {
		if (enabale_info & (1 << i)) {
			memset_s(module_enabale_str, IMPROVE_YIELD_INFO_LEN_MAX, 0, IMPROVE_YIELD_INFO_LEN_MAX);
			str_len = 0;

			str_len = snprintf_s(module_enabale_str,
				sizeof(module_enabale_str),
				sizeof(module_enabale_str) - 1,
				"[%s]",
				g_improve_yield_module_list[i]);

			ret = strncat_s(buf + *len, INFO_MAX_LEN - *len, module_enabale_str, str_len);
			if (ret != 0) {
				pr_err("[%s %d] get enabale info failed, strncat_s fail, ret=%d\n", __func__, __LINE__, ret);
				continue;
			}

			if (*len + str_len > INFO_MAX_LEN) {
				pr_err("[%s %d] get enabale info failed, info buffer overflow\n", __func__, __LINE__);
				continue;
			}

			*len += str_len;
		}
	}

	return ret;
}
#endif
