/*
 * improve_yield_register_info.c
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
#include "improve_yield_internal.h"
#include <securec.h>

#define PASS_FLAG "pass\n"

typedef struct improve_yield_info_node_t {
	char                             *component_name;
	get_improve_yield_info_str        info_func;
	struct improve_yield_info_node_t *next;
} improve_yield_info_node;

static improve_yield_info_node *g_improve_yield_info_head = NULL;
struct mutex                    g_improve_yield_info_mutex;

#if defined(CONFIG_IMPROVE_YIELD) && defined(CONFIG_FACTORY_MODE)
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
int register_improve_yield_info(const char *component_name, get_improve_yield_info_str info_func)
{
	if (component_name == NULL || info_func == NULL) {
		pr_err("%s:register improve yield info func fail, param is null\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&g_improve_yield_info_mutex);

	improve_yield_info_node *new_node = kmalloc(sizeof(improve_yield_info_node), GFP_KERNEL);
	if (new_node == NULL) {
		pr_err("%s:register improve yield info func fail, kzalloc fail\n", __func__);
		mutex_unlock(&g_improve_yield_info_mutex);
		return -ENOMEM;
	}

	new_node->component_name = kstrdup(component_name, GFP_KERNEL);
	if (new_node->component_name == NULL) {
		pr_err("%s:register improve yield info func fail, strdup fail\n", __func__);

		kfree(new_node);
		mutex_unlock(&g_improve_yield_info_mutex);
		return -ENOMEM;
	}

	new_node->info_func       = info_func;
	new_node->next            = g_improve_yield_info_head;
	g_improve_yield_info_head = new_node;

	pr_err("%s:register improve yield info func success, component_name:%s\n", __func__, component_name);

	mutex_unlock(&g_improve_yield_info_mutex);
	return 0;
}
#endif

void free_improve_yield_info_list(void)
{
	improve_yield_info_node *cur_node  = g_improve_yield_info_head;
	improve_yield_info_node *next_node = NULL;

	mutex_lock(&g_improve_yield_info_mutex);

	while (cur_node != NULL) {
		next_node = cur_node->next;
		kfree(cur_node);
		cur_node = next_node;
	}

	g_improve_yield_info_head = NULL;

	mutex_unlock(&g_improve_yield_info_mutex);
}

int get_improve_yield_info(char *info, unsigned int *len)
{
	improve_yield_info_node *cur_node                              = g_improve_yield_info_head;
	int                      get_info_len                          = 0;
	char                     cur_info[IMPROVE_YIELD_INFO_LEN_MAX]  = {0};
	char                     fail_info[IMPROVE_YIELD_INFO_LEN_MAX] = {0};
	int                      ret;

	if (info == NULL || len == NULL) {
		pr_err("%s:get improve yield info fail, param is null\n", __func__);
		return -EINVAL;
	}

	*len = 0;
	memset_s(info, INFO_MAX_LEN, 0, INFO_MAX_LEN);

	mutex_lock(&g_improve_yield_info_mutex);

	while (cur_node != NULL) {
		if (cur_node->info_func != NULL) {
			memset_s(cur_info, IMPROVE_YIELD_INFO_LEN_MAX, 0, IMPROVE_YIELD_INFO_LEN_MAX);
			memset_s(fail_info, IMPROVE_YIELD_INFO_LEN_MAX, 0, IMPROVE_YIELD_INFO_LEN_MAX);
			get_info_len = 0;

			ret = cur_node->info_func(cur_info, &get_info_len);
			if (ret != 0) {
				pr_err("%s:get improve yield info fail, component[%s]\n", __func__, cur_node->component_name);

				get_info_len = snprintf_s(fail_info,
					sizeof(fail_info),
					sizeof(fail_info),
					"component[%s] func execute fail!\n",
					cur_node->component_name);

				ret = strncat_s(info, INFO_MAX_LEN - get_info_len, fail_info, strlen(fail_info));
				if (ret != 0)
					pr_err("%s:fail info create failed, ret=%d\n", __func__, ret);
				else {
					if (*len + get_info_len + 1 > INFO_MAX_LEN) {
						pr_err("%s:get improve yield info fail, info buffer overflow\n", __func__);
						cur_node = cur_node->next;
						continue;
					}
					*len += get_info_len;
				}
			} else {
				if (get_info_len > IMPROVE_YIELD_INFO_LEN_MAX) {
					pr_err("%s:get improve yield info fail, invalid info length from component[%s]\n",
						__func__,
						cur_node->component_name);
					cur_node = cur_node->next;
					continue;
				}

				if (*len + get_info_len + 1 > INFO_MAX_LEN) {
					pr_err("%s:get improve yield info fail, info buffer overflow\n", __func__);
					ret = -ENOMEM;
					break;
				}

				ret = strncat_s(info + *len, INFO_MAX_LEN - *len, cur_info, get_info_len);
				if (ret != 0) {
					pr_err("%s:get improve yield info fail, strncat_s fail, ret=%d\n", __func__, ret);
					ret = -EINVAL;
					break;
				}

				*len += get_info_len;
				pr_err("%s:get improve yield info success, component[%s], info len[%d],\n",
					__func__,
					cur_node->component_name,
					get_info_len);
			}
		}
		cur_node = cur_node->next;
	}

	if (*len == 0) {
		pr_err("%s:test passed, no need to get improve yield info!\n", __func__);

		*len += strlen(PASS_FLAG);
		ret = strncat_s(info, INFO_MAX_LEN - *len, PASS_FLAG, strlen(PASS_FLAG));
		if (ret != 0)
			pr_err("%s:pass flag create failed!\n", __func__);
	}

	mutex_unlock(&g_improve_yield_info_mutex);

	return ret;
}
