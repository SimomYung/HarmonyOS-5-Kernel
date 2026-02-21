/*
 * improve_yield.h
 *
 * running kernel at slt.
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#ifndef __IMPROVE_YIELD_H__
#define __IMPROVE_YIELD_H__
#include <improve_yield_info.h>

#define IMPROVE_YIELD_INFO_LEN_MAX 256

typedef int (*get_improve_yield_info_str)(char *, unsigned int *);

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
int get_improve_yield_nv_info(enum improve_yield_module_id module_id, char *improve_yield_info, unsigned int info_len);

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
int set_improve_yield_nv_info(enum improve_yield_module_id module_id, char *improve_yield_info, unsigned int info_len);

/**
 * @brief Registers a component's improve yield information function into a linked list.
 *
 * This function adds a new node containing the component name and the information function
 * to the head of the linked list, ensuring thread safety with a mutex lock.
 *
 * @param[in] component_name The name of the component to register.
 * @param[in] info_func The function pointer to retrieve the improve yield information.
 *
 * @return int
 *     - 0: Success
 *     - Negative value: Failure (e.g., -EINVAL for NULL parameters, -ENOMEM for memory allocation failure)
 */
int register_improve_yield_info(const char *component_name, get_improve_yield_info_str info_func);

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
int is_module_improve_yield_enable(enum improve_yield_module_id module_id);
#else
static inline int is_module_improve_yield_enable(enum improve_yield_module_id module_id)
{
	(void)(module_id);
	return 0;
}

static inline int set_improve_yield_nv_info(enum improve_yield_module_id module_id, char *improve_yield_info, unsigned int info_len)
{
	(void)(module_id);
	(void)(improve_yield_info);
	(void)(info_len);
	return 0;
}

static inline int register_improve_yield_info(const char *component_name, get_improve_yield_info_str info_func)
{
	(void)(info_func);
	(void)(component_name);
	return 0;
}
#endif

#endif /* __IMPROVE_YIELD_H__ */
