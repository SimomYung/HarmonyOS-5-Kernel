/*
 *  ISP driver, hisp_msg.c
 *
 * Copyright (c) 2013 ISP Technologies CO., Ltd.
 *
 */

#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/syscalls.h>
#include <securec.h>
#include <platform_include/isp/linux/hisp_mempool.h>
#include <platform_include/isp/linux/v500/hisp_msg_handle.h>
#include <platform_include/isp/linux/hisp_remoteproc.h>
#include <platform_include/cee/linux/npu_pm.h>
#include "hisp_internel.h"

#define MSG_MASK_KERNL        0xf000
#define MSG_MASK_KERNL_TO_ISP 0xf400
#define MSG_MASK_ISP_TO_KERNL 0xf800
#define MSG_MASK_ID           0xf0000000

typedef enum {
	/* Kernel Request items */
	KCMD_MEM_POOL_INIT_REQUEST = MSG_MASK_KERNL,
	KCMD_MEM_POOL_DEINIT_REQUEST,
	KCMD_MEM_RECORD_REQUEST,
	KCMD_MEM_UNRECORD_REQUEST,

	/* ISP Request Kernel items */
	KCMD_NPU_CLK_RATE_SET_ACK = MSG_MASK_KERNL_TO_ISP,

	KCMD_ISP_ACK_MSG_MAX,
	/* ISP Request Kernel items */
	KCMD_NPU_CLK_RATE_SET_REQUEST = MSG_MASK_ISP_TO_KERNL,

	KCMD_MSG_MAX
} kcmd_type;

struct hiail_npu_clk {
    int type;
    unsigned int clk_rate;
};

struct hisp_msg_handle {
	atomic_t msg_message_id;
	struct mutex hisp_msg_proc_lock;
    union {
		/* NPU items. */
		struct hiail_npu_clk hiail_clk;
	} u;
};

typedef struct {
	const char *msgName;
	void (*handler)(struct hisp_msg_handle *handle, struct isp_msg_content *msg);
} MsgHub;


struct hisp_msg_handle *g_hisp_msg_handle = NULL;

void hisp_msg_set_npu_clk(struct hisp_msg_handle *handle, struct isp_msg_content *msg);

static const MsgHub g_kmd_isp_request[KCMD_MSG_MAX - MSG_MASK_ISP_TO_KERNL] = {
	/* Kernel ack */
	[KCMD_NPU_CLK_RATE_SET_REQUEST   - MSG_MASK_ISP_TO_KERNL] =
		{"KCMD_SET_NPU_CLK_REQUEST", hisp_msg_set_npu_clk},
};

static int hisp_msg_cmd_check(kcmd_type cmd)
{
	return (cmd < MSG_MASK_KERNL) || (cmd >= MSG_MASK_ISP_TO_KERNL);
}

static int hisp_msg_fill_ispmsg(kcmd_type cmd,
				struct isp_msg_content *isp_msg,
				void *data,
				size_t size)
{
	int ret;
	struct hisp_msg_handle *handle = g_hisp_msg_handle;
	size_t max_size = ISP_MSG_MAX_PAYLOAD -
			sizeof(struct rpmsg_hdr) - sizeof(struct hisp_msg_info);
	unsigned int message_id;

	if (hisp_msg_cmd_check(cmd)) {
		pr_err("[%s] cmd.0x%x invalid\n", __func__, cmd);
		return -EINVAL;
	}
	if (handle == NULL) {
		pr_err("[%s] handle is invalid\n", __func__);
		return -EINVAL;
	}

	isp_msg->info.api_name = cmd;
	message_id = (unsigned int)atomic_add_return(1, &handle->msg_message_id);
	message_id %= MSG_MASK_ID;
	isp_msg->info.message_id = message_id | MSG_MASK_ID;

	ret = memcpy_s(&isp_msg->data[0], max_size, data, size);
	if (ret != 0) {
		pr_err("[%s] memcpy_s error\n", __func__);
		return -ENOMEM;
	}
	return 0;
}

int hisp_msg_type_check(void *data, unsigned int len, int *type)
{
	struct isp_msg_content *isp_msg;
	if (!data) {
		pr_err("[%s] data is NULL\n", __func__);
		return -EINVAL;
	}
	if (!type) {
		pr_err("[%s] type is NULL\n", __func__);
		return -EINVAL;
	}
	if (len > sizeof(struct isp_msg_content)) {
		pr_err("[%s] len.0x%x error\n", __func__, len);
		return -EINVAL;
	}

	isp_msg = (struct isp_msg_content *)data;
	/* type: 0 - normal type; others - kernel type */
	*type = ISPMSG_TYPE_OTHER;
	if ((isp_msg->info.api_name & MSG_MASK_KERNL) == MSG_MASK_KERNL) {
		*type = ISPMSG_TYPE_KERNL;
	}

	return 0;
}

int hisp_msg_mem_record(void *args)
{
	int ret = 0;
	struct isp_msg_content *isp_msg;
	struct hisp_msg_req_mem_record_s *mem =
			(struct hisp_msg_req_mem_record_s *)args;
	if (mem == NULL) {
		pr_err("[%s] arg mem is null\n", __func__);
		ret = -EINVAL;
		goto exit;
	}

	isp_msg = kzalloc(sizeof(struct isp_msg_content), GFP_KERNEL);
	if (isp_msg == NULL) {
		pr_err("[%s] isp_msg kmalloc failed\n", __func__);
		return -ENOMEM;
	}

	ret = hisp_msg_fill_ispmsg(KCMD_MEM_RECORD_REQUEST, isp_msg,
			(void *)mem, sizeof(struct hisp_msg_req_mem_record_s));
	if (ret != 0)
		goto free_msg;

	ret = hisp_msg_service_send((void *)isp_msg,
				sizeof(struct isp_msg_content));
	if (ret != 0) {
		pr_err("[%s] msg send fail\n", __func__);
		ret = -ENOMSG;
	}

free_msg:
	kfree(isp_msg);
exit:
	return ret;
}

int hisp_msg_mem_unrecord(struct hisp_mem_type type, void *args)
{
	(void)type;
	int ret = 0;
	struct isp_msg_content *isp_msg;
	struct hisp_msg_req_mem_record_s *mem =
						(struct hisp_msg_req_mem_record_s *)args;
	if (mem == NULL) {
		pr_err("[%s] arg mem is null\n", __func__);
		ret = -EINVAL;
	}

	isp_msg = kzalloc(sizeof(struct isp_msg_content), GFP_KERNEL);
	if (isp_msg == NULL) {
		pr_err("[%s] isp_msg kmalloc failed\n", __func__);
		return -ENOMEM;
	}

	ret = hisp_msg_fill_ispmsg(KCMD_MEM_UNRECORD_REQUEST, isp_msg,
					(void *)mem, sizeof(struct hisp_msg_req_mem_unrecord_s));
	if (ret != 0)
		goto EXIT;

	ret = hisp_msg_service_send((void *)isp_msg, sizeof(struct isp_msg_content));
	if (ret != 0) {
		pr_err("msg send fail\n");
		ret = -ENOMSG;
	}

EXIT:
	kfree(isp_msg);
	return ret;
}

void hisp_msg_set_npu_clk(struct hisp_msg_handle *handle, struct isp_msg_content *msg)
{
	struct hisp_msg_req_npu_clk_set_s *req =
		(struct hisp_msg_req_npu_clk_set_s *)msg->data;
	struct hiail_npu_clk *ipc_clk = &handle->u.hiail_clk;

	pr_info("%s: clk_rate.%d", __FUNCTION__, req->clk_rate);
	mutex_lock(&handle->hisp_msg_proc_lock);
	isp_nputop_freq_dnlimit(req->clk_rate);
	ipc_clk->type = req->type;
	ipc_clk->clk_rate = req->clk_rate;
	mutex_unlock(&handle->hisp_msg_proc_lock);
}

int hisp_msg_from_isp_req_handle(struct isp_msg_content *msg)
{
	struct hisp_msg_handle *handle = g_hisp_msg_handle;

	if (msg == NULL) {
		pr_err("[%s] msg is NULL\n", __func__);
		return -ESRCH;
	}

	if (handle == NULL) {
		pr_err("[%s] handle is NULL\n", __func__);
		return -ESRCH;
	}

	if ((msg->info.api_name < MSG_MASK_ISP_TO_KERNL) \
		|| (msg->info.api_name >= KCMD_MSG_MAX)) {
		pr_err("[%s] error api_name.0x%x\n", __func__, msg->info.api_name);
		return -EPERM;
	}
	if (g_kmd_isp_request[msg->info.api_name - MSG_MASK_ISP_TO_KERNL].handler != NULL) {
		g_kmd_isp_request[msg->info.api_name - MSG_MASK_ISP_TO_KERNL].handler(handle, msg);
	}
	return 0;
}

void hisp_msg_handle_init(void)
{
	struct hisp_msg_handle *msg_handle;

	msg_handle = kzalloc(sizeof(struct hisp_msg_handle), GFP_KERNEL);
	if (msg_handle == NULL) {
		pr_err("hisp_msg_handle is NULL\n");
		return;
	}
	atomic_set(&msg_handle->msg_message_id, 0);
	mutex_init(&msg_handle->hisp_msg_proc_lock);

	g_hisp_msg_handle = msg_handle;
}

void hisp_msg_handle_deinit(void)
{
	struct hisp_msg_handle *msg_handle = g_hisp_msg_handle;

	if (msg_handle == NULL) {
		pr_err("msg_handle is NULL\n");
		return;
	}

	atomic_set(&msg_handle->msg_message_id, 0);
	mutex_destroy(&msg_handle->hisp_msg_proc_lock);

	kfree(msg_handle);
	g_hisp_msg_handle = NULL;
}
