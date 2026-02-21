/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: allowed_ext_agent_ca list and functions
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "agent.h"
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <securec.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include "tc_ns_log.h"
#include "tc_current_info.h"

#define MAX_AGENT_NUM 16
static struct ca_info *g_allowed_ext_agent_ca = NULL;
static int g_agent_whitelist_num = 0;


void free_agent_whitelist_mem(void)
{
	if (g_allowed_ext_agent_ca != NULL) {
		kfree(g_allowed_ext_agent_ca);
		g_allowed_ext_agent_ca = NULL;
		g_agent_whitelist_num = 0;
	}
}

static int parse_agent_node(struct device_node *node, struct ca_info *agent_info)
{
	errno_t result;

	if (node == NULL || agent_info == NULL) {
		tloge("the params is NULL");
		return -1;
	}
	const char *service_name = of_get_property(node, "service_name", NULL);
	if (service_name == NULL) {
		tloge("get the agent service_name failed\n");
		return -1;
	}
	result = memcpy_s(agent_info->path, MAX_PATH_SIZE - 1, service_name, strlen(service_name));
	if (result != EOK) {
		tloge("memcpy_s failed result: %d\n", result);
		return -1;
	}
	if (of_property_read_u32(node, "uid", &(agent_info->uid)) != 0) {
		tloge("get the uid failed\n");
		return -1;
	}

	if (of_property_read_u32(node, "agentid", &(agent_info->agent_id)) != 0) {
		tloge("get the agentid failed\n");
		return -1;
	}
	return 0;
}

static int parse_whitelist(unsigned int *agent_whitelist, int agent_num, int *index)
{
	int i;
	int result;
	for (i = 0; i < agent_num; i++) {
		struct device_node *entry_node = of_find_node_by_phandle(agent_whitelist[i]);
		result = parse_agent_node(entry_node, &g_allowed_ext_agent_ca[*index]);
		if (result != 0) {
			tloge("parse agent node failed index is %d\n", i);
			return -1;
		}
		(*index)++;
	}
	return 0;
}

static int get_whitelist_item_num(struct device_node *dev_node, char *item_name,
	unsigned int *whitelist, const int whitelist_len)
{
	int num = of_property_read_variable_u32_array(dev_node, item_name, whitelist, 0, whitelist_len);
	if (num >= 0)
		return num;
	if (num == -EINVAL || num == -ENODATA) {
		tlogi("the agent item is empty or not exist\n");
		return 0;
	}
	tloge("get the array failed the num is %d\n", num);
	return -1;
}

int load_agent_whitelist(struct device_node *dev_node)
{
	int result;
	int agent_len;
	if (dev_node == NULL)
		return -1;

	unsigned agent_whitelist[MAX_AGENT_NUM] = {0};
	agent_len = get_whitelist_item_num(dev_node, "agent_whitelist", agent_whitelist, MAX_AGENT_NUM);
	if (agent_len < 0)
		return -1;
	g_agent_whitelist_num += agent_len;
#ifdef DEF_ENG
	int eng_agent_len;
	unsigned eng_agent_whitelist[MAX_AGENT_NUM] = {0};
	eng_agent_len = get_whitelist_item_num(dev_node, "agent_whitelist_eng",
														eng_agent_whitelist, MAX_AGENT_NUM);
	if (eng_agent_len < 0)
		return -1;
	g_agent_whitelist_num += eng_agent_len;
#endif

	if (g_agent_whitelist_num > 0) {
		int buf_len = g_agent_whitelist_num * sizeof(struct ca_info);
		g_allowed_ext_agent_ca = kmalloc(buf_len, GFP_KERNEL);
		if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)g_allowed_ext_agent_ca)) {
			tloge("alloc mem failed\n");
			return -1;
		}
		memset_s(g_allowed_ext_agent_ca, buf_len, 0, buf_len);
		int index = 0;
		result = parse_whitelist(agent_whitelist, agent_len, &index);
		if (result != 0) {
			free_agent_whitelist_mem();
			return -1;
		}

#ifdef DEF_ENG
		result = parse_whitelist(eng_agent_whitelist, eng_agent_len, &index);
		if (result != 0) {
			free_agent_whitelist_mem();
			return -1;
		}
#endif
	}
	return 0;
}

int is_allowed_agent_ca(const struct ca_info *ca,
	bool check_agent_id)
{
	uint32_t i;
	struct ca_info *tmp_ca = g_allowed_ext_agent_ca;
	const uint32_t nr = g_agent_whitelist_num;
	if(tmp_ca == NULL) {
		tloge("the tmp_ca is null\n");
		return -EFAULT;
	}

	if (!ca)
		return -EFAULT;

	if (!check_agent_id) {
		for (i = 0; i < nr; i++) {
			if (!strncmp(ca->path, tmp_ca->path,
				strlen(tmp_ca->path) + 1) &&
				ca->uid == tmp_ca->uid)
				return 0;
			tmp_ca++;
		}
	} else {
		for (i = 0; i < nr; i++) {
			if (!strncmp(ca->path, tmp_ca->path,
				strlen(tmp_ca->path) + 1) &&
				ca->uid == tmp_ca->uid &&
				ca->agent_id == tmp_ca->agent_id)
				return 0;
			tmp_ca++;
		}
	}
	tlogd("ca-uid is %u, ca_path is %s, agent id is %x\n", ca->uid,
		ca->path, ca->agent_id);

	return -EACCES;
}

