// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the hw_kernel_sg_uploader.c for kernel data uploading through uevent
 * Create: 2022-07-23
 */
#include <linux/workqueue.h>
#include "securec.h"
#include "hw_kernel_sg_uploader.h"

#define KSG_LOG_TAG "kernel_sg_uploader"
static unsigned int kernel_sg_upload_cmd(const event_info *info, enum sg_item_fun_type cmd);
static void kernel_sg_upload_func(struct work_struct *work);
static struct kobject *g_kernel_sg_kobj;
static struct kset *g_kernel_sg_kset;

static int g_sg_white_init = KSG_LIST_UNINIT;
struct kernel_sg_white g_sg_white_node;
static struct workqueue_struct *g_event_upload_wq;
struct work_ctx {
	struct delayed_work event_upload_work;
	event_info *info;
	enum sg_item_fun_type cmd;
};

static DEFINE_MUTEX(sg_white_lock);

const struct sg_item_fun g_kernel_sg_funcs[] = {
	{KERNEL_SG_UPLOAD, kernel_sg_upload_parse},
};

static void kernel_sg_free_white(void)
{
	struct kernel_sg_white *pos = NULL;
	struct kernel_sg_white *next = NULL;

	if (g_sg_white_init != KSG_LIST_INIT) {
		ksg_log_trace(KSG_LOG_TAG, "uploader white not init");
		return;
	}

	mutex_lock(&sg_white_lock);
	list_for_each_entry_safe(pos, next, &g_sg_white_node.sg_white, sg_white) {
		list_del(&pos->sg_white);
		kfree(pos);
		pos = NULL;
	}
	mutex_unlock(&sg_white_lock);
}

int kernel_sg_uploader_init(void)
{
	const char *kernel_sg_kobj_name = "hw_kernel_sg_uploader";
	const char *kernel_sg_kset_name = "hw_kernel_sg_kset";
	int ret;
	g_event_upload_wq = create_workqueue(kernel_sg_kobj_name);
	do {
		if (g_sg_white_init == KSG_LIST_UNINIT) {
			INIT_LIST_HEAD(&g_sg_white_node.sg_white);
			g_sg_white_init = KSG_LIST_INIT;
			ksg_log_trace(KSG_LOG_TAG, "uploader white init done");
		}

		g_kernel_sg_kobj = kobject_create_and_add(kernel_sg_kobj_name, kernel_kobj);
		if (g_kernel_sg_kobj == NULL) {
			ksg_log_error(KSG_LOG_TAG, "creat sg kobject failed");
			ret = KSG_ERRCODE;
			break;
		}

		g_kernel_sg_kset =  kset_create_and_add(kernel_sg_kset_name, NULL, kernel_kobj);
		if (g_kernel_sg_kset == NULL) {
			ksg_log_error(KSG_LOG_TAG, "creat sg kset failed");
			ret = KSG_ERRCODE;
			break;
		}
		g_kernel_sg_kobj->kset = g_kernel_sg_kset;

		ret = kobject_uevent(g_kernel_sg_kobj, KOBJ_ADD);
		if (ret != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "kobj_uevent add failed, result is %d", ret);
			break;
		}
	} while (0);

	if (ret != KSG_SUCCESS) {
		kernel_sg_uploader_exit();
		ksg_log_error(KSG_LOG_TAG, "uploader init failed");
		return ret;
	}

	ksg_log_trace(KSG_LOG_TAG, "uploader init ok");
	return ret;
}

void kernel_sg_uploader_exit(void)
{
	if (g_kernel_sg_kobj != NULL) {
		kobject_put(g_kernel_sg_kobj);
		g_kernel_sg_kobj = NULL;
	}

	if (g_kernel_sg_kset != NULL) {
		kset_unregister(g_kernel_sg_kset);
		g_kernel_sg_kset = NULL;
	}

	kernel_sg_free_white();
	destroy_workqueue(g_event_upload_wq);
	ksg_log_trace(KSG_LOG_TAG, "uploader_exit done");
}

int kernel_sg_upload_parse(const event_info *info, char *upload_info, int upload_size)
{
	int ret;
	if ((upload_info == NULL) || (info == NULL) || (upload_size != KSG_INFO_MAXLEN)) {
		ksg_log_error(KSG_LOG_TAG, "input arguments invalid");
		return -EINVAL;
	}

	// format upload sg msg
	ret = snprintf_s(upload_info, upload_size, upload_size - 1,
		"SG_KERNEL_COLLECT_DATA_CMD=%lu-%u-%u-%s",
		info->event_id, info->version, info->content_len, info->content);
	return (ret <= 0) ? KSG_ERRCODE : KSG_SUCCESS;
}

/* concatenate kernel_sg data of type int and type char array */
static int kernel_sg_data_adapter(char **uevent_envp, char *result)
{
	int index;
	if ((uevent_envp == NULL) || (result == NULL)) {
		ksg_log_error(KSG_LOG_TAG, "input arguments invalid");
		return -EINVAL;
	}

	for (index = 0; index < KSG_UEVENT_LEN - 1; index++) {
		uevent_envp[index] = result;
		ksg_log_debug(KSG_LOG_TAG, "uevent_envp[%d] is %s", index, uevent_envp[index]);
	}

	return KSG_SUCCESS;
}

/* different uploader have different parse info function */
static int kernel_sg_upload_parse_cmd(enum sg_item_fun_type cmd,
	const event_info *info, char *upload_info, int upload_size)
{
	int i;
	for (i = 0; i < KERNEL_SG_UPLOAD_MAX; i++) {
		if (g_kernel_sg_funcs[i].type == cmd)
			return g_kernel_sg_funcs[i].func(info, upload_info, upload_size);
	}
	return -EINVAL;
}

static void kernel_sg_upload_func(struct work_struct *work)
{
	ksg_log_trace(KSG_LOG_TAG, "begin upload info to sg");
	struct work_ctx *ctx = container_of(to_delayed_work(work), struct work_ctx, event_upload_work);
	if (ctx == NULL) {
		ksg_log_error(KSG_LOG_TAG, "container of work failed");
		return;
	}
	if (kernel_sg_upload_cmd(ctx->info, ctx->cmd) != KSG_SUCCESS) {
		ksg_log_error(KSG_LOG_TAG, "upload cmd fail");
	}
	ksg_log_trace(KSG_LOG_TAG, "end upload info to sg %lu", ctx->info->event_id);
	if (ctx->info != NULL)
		kfree(ctx->info);
	if (ctx != NULL)
		kfree(ctx);
	return;
}

/* parse format and upload msg by uevent */
static unsigned int kernel_sg_upload_cmd(const event_info *info, enum sg_item_fun_type cmd)
{
	int ret;
	char *upload_info = NULL;
	char *uevent_envp[KSG_UEVENT_LEN] = { NULL };

	if (g_kernel_sg_kobj == NULL) {
		ksg_log_debug(KSG_LOG_TAG, "kobj no creat");
		return KSG_ERRCODE;
	}

	do {
		upload_info = kzalloc(KSG_INFO_MAXLEN, GFP_KERNEL);
		if (upload_info == NULL) {
			ksg_log_error(KSG_LOG_TAG, "failed to alloc upload_info");
			return KSG_ERRCODE;
		}

		ret = kernel_sg_upload_parse_cmd(cmd, info, upload_info, KSG_INFO_MAXLEN);
		if (ret != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "data parse failed, ret is %d", ret);
			break;
		}

		ret = kernel_sg_data_adapter(uevent_envp, upload_info);
		if (ret != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "data adpter failed, ret is %d", ret);
			break;
		}

		ret = kobject_uevent_env(g_kernel_sg_kobj, KOBJ_CHANGE, uevent_envp);
		if (ret != KSG_SUCCESS) {
			ksg_log_error(KSG_LOG_TAG, "kobj upload failed, ret is %d", ret);
			break;
		}

		ksg_log_trace(KSG_LOG_TAG, "event upload finished. result: %d", ret);
	} while (0);

	if (upload_info != NULL)
		kfree(upload_info);
	return ret == KSG_SUCCESS ? KSG_SUCCESS : KSG_ERRCODE;
}

/* uploader call this fun to report info */
unsigned int report_security_info(const event_info *info)
{
	int id_count = 0;
	struct kernel_sg_white *tmp = NULL;

	if (info == NULL) {
		ksg_log_error(KSG_LOG_TAG, "event_info NULL");
		return KSG_ERRCODE;
	}

	// check uploader in white
	if (g_sg_white_init == KSG_LIST_INIT) {
		mutex_lock(&sg_white_lock);
		list_for_each_entry(tmp, &g_sg_white_node.sg_white, sg_white) {
			if (info->event_id == tmp->event_id) {
				id_count++;
				ksg_log_debug(KSG_LOG_TAG, "event_id had in list");
				break;
			}
		}
		mutex_unlock(&sg_white_lock);
	}

	if (id_count == 0) {
		ksg_log_error(KSG_LOG_TAG, "event_id %lu no upload permission", info->event_id);
		return KSG_NO_PERMISSION;
	}
	struct work_ctx *ctx = kzalloc(sizeof(struct work_ctx), GFP_KERNEL);
	if (ctx == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kzalloc event info failed");
		return KSG_ERRCODE;
	}
	if (g_event_upload_wq != NULL) {
		INIT_DELAYED_WORK(&ctx->event_upload_work, kernel_sg_upload_func);
	}
	ctx->info = kzalloc(sizeof(event_info) + MAX_EVENT_CONTENT_LEN, GFP_KERNEL);
	ctx->cmd = KERNEL_SG_UPLOAD;
	if (ctx->info == NULL) {
		ksg_log_error(KSG_LOG_TAG, "kzalloc event info failed");
		kfree(ctx);
		return KSG_ERRCODE;
	}
	(void)memset_s(ctx->info, sizeof(event_info) + MAX_EVENT_CONTENT_LEN, 0,
		sizeof(event_info) + MAX_EVENT_CONTENT_LEN);
	if (memcpy_s(ctx->info, sizeof(event_info) + MAX_EVENT_CONTENT_LEN,
		info, sizeof(event_info) + info->content_len) != EOK) {
		ksg_log_error(KSG_LOG_TAG, "memcpy_s event info failed");
		kfree(ctx->info);
		kfree(ctx);
		return KSG_ERRCODE;
	}
	if (queue_delayed_work(g_event_upload_wq, &ctx->event_upload_work, msecs_to_jiffies(1)) != true) {
		kfree(ctx->info);
		kfree(ctx);
		return KSG_ERRCODE;
	}
	return KSG_SUCCESS;
}

static void kernel_sg_update_white_by_id(unsigned long value)
{
	struct kernel_sg_white *listnode = NULL;
	struct kernel_sg_white *tmp = NULL;
	int repeat_count = 0;
	int list_size = 0;

	if (g_sg_white_init == KSG_LIST_UNINIT) {
		INIT_LIST_HEAD(&g_sg_white_node.sg_white);
		g_sg_white_init = KSG_LIST_INIT;
		ksg_log_trace(KSG_LOG_TAG, "uploader white init in update white");
	}

	// event_id 0 means replace all white
	if (value == KSG_REPLACE_WHITE) {
		kernel_sg_free_white();
		ksg_log_trace(KSG_LOG_TAG, "white clear done");
		return;
	}
	listnode = kzalloc(sizeof(struct kernel_sg_white), GFP_KERNEL);
	if (listnode == NULL) {
		ksg_log_error(KSG_LOG_TAG, "listnode init failed");
		return;
	}

	listnode->event_id = value;
	ksg_log_error(KSG_LOG_TAG, "event_id want add %lu", value);

	mutex_lock(&sg_white_lock);
	list_for_each_entry(tmp, &g_sg_white_node.sg_white, sg_white) {
		list_size++;
		if (listnode->event_id == tmp->event_id) {
			repeat_count++;
			ksg_log_debug(KSG_LOG_TAG, "event_id had in list, no need add again");
			break;
		}
	}

	if ((list_size >= KSG_MAX_WHITE_SIZE) && (repeat_count == 0)) {
		ksg_log_error(KSG_LOG_TAG, "white too many %d, can not add any", list_size);
		kfree(listnode);
		mutex_unlock(&sg_white_lock);
		return;
	}

	if (repeat_count == 0) {
		list_add_tail(&listnode->sg_white, &g_sg_white_node.sg_white);
		ksg_log_trace(KSG_LOG_TAG, "add white node success");
	} else {
		kfree(listnode);
	}
	mutex_unlock(&sg_white_lock);
}

/* sg uploader update the white */
int kernel_sg_update_white(char *str)
{
	if ((str == NULL) || (*str == '\0')) {
		ksg_log_error(KSG_LOG_TAG, "update_white str error");
		return KSG_ERRCODE;
	}

	while ((str != NULL) && (*str != '\0')) {
		unsigned long value = 0;
		char *tmp_str = strsep(&str, SPLIT_STR);
		if ((tmp_str == NULL) || (*tmp_str == '\0')) {
			ksg_log_error(KSG_LOG_TAG, "strsep error");
			continue;
		}
		if (strlen(tmp_str) > KSG_NUMBER_MAX_LEN) {
			ksg_log_error(KSG_LOG_TAG, "str len error");
			continue;
		}

		if (kstrtoul(tmp_str, KSG_NUMBER_DECIMAL, &value)) {
			ksg_log_error(KSG_LOG_TAG, "kstrtoul error");
			continue;
		}

		if (value >= KSG_NUMBER_MAX_ALLOW) {
			ksg_log_error(KSG_LOG_TAG, "value error %lu", value);
			continue;
		}

		kernel_sg_update_white_by_id(value);
	}

	return KSG_SUCCESS;
}

