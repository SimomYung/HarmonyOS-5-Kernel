/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat big data fmd report.
 * Create : 2024/06/18
 */
#include "seplat_fmd.h"
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <securec.h>
#include <dsm/dsm_pub.h>

#if defined(CONFIG_HUAWEI_DATA_ACQUISITION) && (defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM))
#define SEPLAR_DSM_DEFAULT_CYCLE      0
#define SEPLAT_DSM_DEFAULT_VERSION    1

/* define dsm buffer size */
#define SEPLAT_DSM_BUF_SIZE_1024      1024

struct seplat_dsm_client {
	enum seplat_dsm_type type;
	const char *name;
	struct dsm_client *client;
	struct dsm_dev *dev;
};

static struct dsm_dev g_seplat_dsm_dev_flash_ecc = {
	.name = "seplat_flash_ecc",
	.device_name = "seplat",
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
	.buff_size = SEPLAT_DSM_BUF_SIZE_1024,
};

/* define seplat dsm struct data */
static struct seplat_dsm_client g_seplat_dsm_client_data[] = {
	{
		SEPLAT_DSM_TYPE_FLASH_ECC,
		"flash_ecc",
		NULL,
		&g_seplat_dsm_dev_flash_ecc,
	},
};

static struct dsm_client *seplat_dsm_get_client(enum seplat_dsm_type type)
{
	u32 idx;
	struct dsm_client *client = NULL;
	struct seplat_dsm_client *p_client = g_seplat_dsm_client_data;

	if (type >= SEPLAT_DSM_TYPE_END || type <= SEPLAT_DSM_TYPE_BEGINE) {
		pr_err("invalid dsm type:%d\n", type);
		return NULL;
	}

	for (idx = 0; idx < ARRAY_SIZE(g_seplat_dsm_client_data); idx++) {
		if (type == p_client[idx].type) {
			client = p_client[idx].client;
			break;
		}
	}

	if (!client)
		pr_err("client[%d] not exist\n", idx);

	return client;
}

static void seplat_dsm_message_pack(struct message *p_msg, struct seplat_dsm_event *event_info, int event_num)
{
	int i;

	p_msg->num_events   = event_num;
	p_msg->data_source  = DATA_FROM_KERNEL;
	p_msg->version      = SEPLAT_DSM_DEFAULT_VERSION;

	for (i = 0; i < event_num; i++) {
		p_msg->events[i].error_code = event_info[i].errno;
		p_msg->events[i].cycle      = SEPLAR_DSM_DEFAULT_CYCLE;
		p_msg->events[i].item_id    = event_info[i].item_id;

		(void)snprintf_s(p_msg->events[i].device_name,
						 SEPLAT_DSM_NAME_MAX_LEN, SEPLAT_DSM_NAME_MAX_LEN - 1, "seplat");
		(void)snprintf_s(p_msg->events[i].result, SEPLAT_DSM_RESULT_MAX_LEN,
						 SEPLAT_DSM_RESULT_MAX_LEN - 1, "%s", event_info[i].result);
		(void)snprintf_s(p_msg->events[i].test_name, SEPLAT_DSM_NAME_MAX_LEN,
						 SEPLAT_DSM_NAME_MAX_LEN - 1, "%s", event_info[i].test_name);
		(void)snprintf_s(p_msg->events[i].value, SEPLAT_DSM_VALUE_MAX_LEN,
						 SEPLAT_DSM_VALUE_MAX_LEN -1, "%s", event_info[i].value);
		(void)snprintf_s(p_msg->events[i].min_threshold, SEPLAT_DSM_VALUE_MAX_LEN,
						 SEPLAT_DSM_VALUE_MAX_LEN -1, "%s", event_info[i].min_threshold);
		(void)snprintf_s(p_msg->events[i].max_threshold, SEPLAT_DSM_VALUE_MAX_LEN,
						 SEPLAT_DSM_VALUE_MAX_LEN -1, "%s", event_info[i].max_threshold);
	}
}

int seplat_dsm_report_fmd(enum seplat_dsm_type type, struct seplat_dsm_event *event_info, int event_num)
{
	int ret;
	int count = -1;
	struct message *p_msg = NULL;
	struct dsm_client *client = seplat_dsm_get_client(type);

	if (!client || !event_info) {
		pr_err("client or msg null\n");
		return SEPLAT_ERR_BASIC_NULL;
	}

	if (event_num > MAX_MSG_EVENT_NUM) {
		pr_err("event num[%d] unsupport\n", event_num);
		return SEPLAT_ERR_BASIC_PARAMS;
	}

	p_msg = kzalloc(sizeof(struct message), GFP_KERNEL);
	if (!p_msg)
		return SEPLAT_ERR_BASIC_MEMORY;

	seplat_dsm_message_pack(p_msg, event_info, event_num);

	if (dsm_client_ocuppy(client) == SEPLAT_KERNEL_OK) {
		count = dsm_client_copy_ext(client, p_msg, sizeof(struct message));
		if (count > 0) {
			pr_debug("seplat dsm report succ\n");
			ret = SEPLAT_KERNEL_OK;
		} else {
			pr_err("seplat dsm report fail, ret count = %d\n", count);
			ret = SEPLAT_ERR_DSM_REPORT_ERR;
		}
		dsm_client_unocuppy(client);
	} else {
		pr_err("seplat dsm client busy\n");
		ret = SEPLAT_ERR_DSM_CLIENT_BUSY;
	}

	kfree((void *)p_msg);

	return ret;
}

static int seplat_dsm_init(void)
{
	u32 i;
	struct seplat_dsm_client *p_client = g_seplat_dsm_client_data;

	for (i = 0; i < ARRAY_SIZE(g_seplat_dsm_client_data); i++) {
		if (!p_client[i].client)
			p_client[i].client = dsm_register_client(p_client[i].dev);

		if (!p_client[i].client)
			pr_err("[%d]%s dsm reg fail\n", i, p_client[i].name);
	}

	pr_info("%s finish.\n", __func__);

	return SEPLAT_KERNEL_OK;
}

static void seplat_dsm_exit(void)
{
	u32 i;
	struct seplat_dsm_client *p_client = g_seplat_dsm_client_data;

	for (i = 0; i < ARRAY_SIZE(g_seplat_dsm_client_data); i++) {
		p_client[i].client = NULL;
		pr_debug("dsm %s un-reg ok\n", p_client[i].name);
	}

	pr_info("%s finish\n", __func__);
}

subsys_initcall_sync(seplat_dsm_init);

module_exit(seplat_dsm_exit);

#else

int seplat_dsm_report_fmd(enum seplat_dsm_type type, struct seplat_dsm_event *event_info, int event_num)
{
	unused(type);
	unused(event_info);
	unused(event_num);
	pr_info("%s, dsm not supported\n", __func__);
	return SEPLAT_KERNEL_OK;
}

#endif
