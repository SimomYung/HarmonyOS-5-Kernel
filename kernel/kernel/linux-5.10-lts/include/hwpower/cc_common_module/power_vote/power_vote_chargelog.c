// SPDX-License-Identifier: GPL-2.0
/*
 * power_vote_chargelog.c
 *
 * log power vote result to chargelog
 *
 * Copyright (C) 2024-2024 Huawei TechCnologies Co., Ltd.
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

#include <chipset_common/hwpower/common_module/power_log.h>
#include <chipset_common/hwpower/common_module/power_vote.h>

struct power_vote_chargelog_info {
	const char *vote_object_name;
	const char *log_head;
	const char *log_content_format;
};

static const struct power_vote_chargelog_info g_infos[] = {
	{
		.vote_object_name = "DC:current",
		.log_head = "[DC:current              value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "DC:ibus",
		.log_head = "[DC:ibus                 value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "DC:cable_curr",
		.log_head = "[DC:cable_curr           value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "DC:adpt_curr",
		.log_head = "[DC:adpt_curr            value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "BATT:fcc",
		.log_head = "[BATT:fcc                value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "BATT:usb_icl",
		.log_head = "[BAT:usb_icl             value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "BATT:vterm",
		.log_head = "[BATT:vterm              value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "BATT:iterm",
		.log_head = "[BATT:iterm              value]    ",
		.log_content_format = " %-24s%-10d",
	},
	{
		.vote_object_name = "BATT:dis_chg",
		.log_head = "[BAT:dis_chg             value]    ",
		.log_content_format = " %-24s%-10d",
	},
};

static bool power_vote_is_client_valid(const char *vote_object_name)
{
	return power_vote_has_vote_object(vote_object_name);
}

static int power_vote_chargelog_head(char *buffer, int size, void *dev_data)
{
	size_t i;
	int len = 0;

	if (!buffer)
		return -EPERM;

	for (i = 0; i < ARRAY_SIZE(g_infos); ++i) {
		if (size > len)
			len += snprintf(buffer + len, size - len, "%s", g_infos[i].log_head);
	}
	return 0;
}

static const char *power_vote_chargelog_get_client(const struct power_vote_chargelog_info *info)
{
	const char *client = NULL;

	if (power_vote_is_client_valid(info->vote_object_name))
		client = power_vote_get_effective_client_locked(info->vote_object_name, true);

	if (!client)
		client = "None";

	return client;
}

static int power_vote_chargelog_get_result(const struct power_vote_chargelog_info *info)
{
	int result = 0;

	if (power_vote_is_client_valid(info->vote_object_name))
		result = power_vote_get_effective_result_locked(info->vote_object_name, true);

	return result;
}

static int power_vote_chargelog_content(char *buffer, int size, void *dev_data)
{
	size_t i;
	int len = 0;

	if (!buffer)
		return -EPERM;

	for (i = 0; i < ARRAY_SIZE(g_infos); ++i) {
		if (size > len)
			len += snprintf(buffer + len,
				size - len,
				g_infos[i].log_content_format,
				power_vote_chargelog_get_client(&g_infos[i]),
				power_vote_chargelog_get_result(&g_infos[i]));
		power_vote_print_object(g_infos[i].vote_object_name);
	}
	return 0;
}

static struct power_log_ops power_vote_chargelog_ops = {
	.dev_name = "power_vote_info",
	.dump_log_head = power_vote_chargelog_head,
	.dump_log_content = power_vote_chargelog_content,
};

static bool power_vote_chargelog_is_valid(void)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(g_infos); ++i) {
		if (!g_infos[i].vote_object_name ||
			!g_infos[i].log_head ||
			!g_infos[i].log_content_format)
			return false;
	}
	return true;
}

static int __init power_vote_chargelog_init(void)
{
	if (!power_vote_chargelog_is_valid())
		return -EPERM;
	return power_log_ops_register(&power_vote_chargelog_ops);
}

static void __exit power_vote_chargelog_exit(void)
{
}

fs_initcall(power_vote_chargelog_init);
module_exit(power_vote_chargelog_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("power vote charge log module");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
