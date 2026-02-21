/*
 * vcodec_dev_mgr_formal.c
 *
 * Implement vcodec dev mgr function which is used only by formal version
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include <platform_include/see/sec_auth_ca.h>
#include <teek_client_api.h>
#include "dbg.h"
#include "vcodec_dev_mgr_formal.h"
#include "dev_mgr.h"

/*
 * Function name: teek_init.
 * Discription:Init the TEEC and get the context
 * Parameters:
 *      @ session: the bridge from unsec world to sec world.
 *      @ context: context.
 * Return value:
 *      @ TEEC_SUCCESS-->success, others-->failed.
 * Note: this function is actually a static function so the params are not
 *       checked. The reason for not adding 'static' is only used for
 *       mdpp certification and can guarantee the params isn't null.
 */
static int load_vdec_teek_init(TEEC_Session *session, TEEC_Context *context)
{
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_SECAUTH;
	TEEC_Operation operation = {0};
	char package_name[] = SEC_AUTH_NAME;
	u32 root_id = 0;

	result = TEEK_InitializeContext(NULL, context);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "TEEK_InitializeContext fail res=0x%x\n", result);
		return (int)result;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_INPUT);
	/* 2 is TEEC_RegisteredMemoryReference type */
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = (uint32_t)sizeof(root_id);
	/* 3 is TEEC_Value type */
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = (unsigned int)strlen(package_name) + 1;
	result = TEEK_OpenSession(context, session, &svc_uuid,
				  TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "TEEK_OpenSession failed result=0x%x!\n", result);
		TEEK_FinalizeContext(context);
		return (int)result;
	}

	return LOAD_VDEC_OK;
}

static int load_vdec_node_init(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
						 TEEC_NONE, TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_INIT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "init ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_VDEC_OK;
}

static int load_vdec_node_read_cert(TEEC_Session *session, TEEC_Operation *operation, uintptr_t addr, u32 size)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);

	operation->params[1].tmpref.buffer = (void *)addr;
	operation->params[1].tmpref.size = size;
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_CERT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "read_cert ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_VDEC_OK;
}

static int load_vdec_node_read_data(TEEC_Session *session, TEEC_Operation *operation, uintptr_t addr, u32 size)
{
	TEEC_Result result;
	u32 origin = 0;
	u32 send_size, cur_size;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT,
						 TEEC_VALUE_INPUT, TEEC_NONE);
	send_size = 0;
	while(send_size < size) {
		cur_size = load_vdec_min((size - send_size), SEC_AUTH_SEND_MAX_SIZE);
		operation->params[1].tmpref.buffer = (void *)(addr + (uintptr_t)send_size);
		operation->params[1].tmpref.size = cur_size;
		operation->params[2].value.a = send_size;
		operation->params[2].value.b = size;
		result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_DATA, operation, &origin);
		if (result != TEEC_SUCCESS) {
			dprint(PRN_ERROR, "read_data send_size=%u cur_size=%u ret=0x%x\n", \
				send_size, cur_size, result);
			return (int)result;
		}
		send_size += cur_size;
	}

	return LOAD_VDEC_OK;
}

static int load_vdec_node_verify(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	u32 origin = 0;
	struct authed_data out = {
		.items = {
			{ .item_id = CERT_ITEM_IMAGE_LOAD_SIZE },
			{ .item_id = CERT_ITEM_IMAGE_HASH },
		},
	};

	operation->params[1].tmpref.buffer = (void *)&out;
	operation->params[1].tmpref.size = (uint32_t)sizeof(out);
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEE_PARAM_TYPE_MEMREF_INOUT,
						 TEEC_NONE, TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_VERIFY, operation, &origin);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "verify ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_VDEC_OK;
}

static int load_vdec_node_final(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
						 TEEC_NONE,  TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_FINAL, operation, &origin);
	if (result != TEEC_SUCCESS) {
		dprint(PRN_ERROR, "final ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_VDEC_OK;
}

static int load_verify_vdec_image(void *image_buffer, size_t image_size, struct image_id_t id)
{
	int ret;
	TEEC_Session session;
	TEEC_Context context;
	TEEC_Operation operation = { 0 };

	dprint(PRN_ALWS, "load_vdec_teek_init +\n");
	ret = load_vdec_teek_init(&session, &context);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "sec_auth teek init failed, ret is 0x%x\n", ret);
		return ret;
	}
	dprint(PRN_ALWS, "load_vdec_teek_init -\n");

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.params[0].tmpref.buffer = (void *)&id;
	operation.params[0].tmpref.size = (uint32_t)sizeof(id);

	dprint(PRN_ALWS, "load_vdec_node_init +\n");
	ret = load_vdec_node_init(&session, &operation);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "load_vdec node init failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	dprint(PRN_ALWS, "load_vdec_node_init -\n");

	dprint(PRN_ALWS, "load_vdec_node_read_cert +\n");
	ret = load_vdec_node_read_cert(&session, &operation, (uintptr_t)image_buffer, SEC_AUTH_CERT_SIZE);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "load_vdec node read cert failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	dprint(PRN_ALWS, "load_vdec_node_read_cert -\n");

	dprint(PRN_ALWS, "load_vdec_node_read_data +\n");
	ret = load_vdec_node_read_data(&session, &operation,
		(uintptr_t)image_buffer + SEC_AUTH_CERT_SIZE, (unsigned int)image_size - SEC_AUTH_CERT_SIZE);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "load_vdec node read data failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	dprint(PRN_ALWS, "load_vdec_node_read_data -\n");

	dprint(PRN_ALWS, "load_vdec_node_verify +\n");
	ret = load_vdec_node_verify(&session, &operation);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "load_vdec verify failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	dprint(PRN_ALWS, "load_vdec_node_verify -\n");

	dprint(PRN_ALWS, "load_vdec_node_final +\n");
	ret = load_vdec_node_final(&session, &operation);
	if (ret != LOAD_VDEC_OK) {
		dprint(PRN_ERROR, "load_vdec final failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	dprint(PRN_ALWS, "load_vdec_node_final -\n");

err_out:
	TEEK_CloseSession(&session);
	TEEK_FinalizeContext(&context);

	return ret;
}

static void img_load_work(struct work_struct *work)
{
	int ret;
	const char *image_name;
	struct work_ctx *ctx = NULL;
	struct image_id_t img_id = {
		.partition_name = "vendor",
		.image_name = "",
	};

	ctx = container_of(work, struct work_ctx, work);
	image_name = ctx->is_bsp_image ? IMAGE_ID_BSP_NAME : IMAGE_ID_PXP_NAME;
	if (strcpy_s(img_id.image_name, IMAGE_NAME_LEN_MAX, image_name)) {
		dprint(PRN_ERROR, "failed to copy from image");
		return;
	}
		
	ret = load_verify_vdec_image(ctx->image_buffer, ctx->image_size, img_id);
	dprint(PRN_ERROR, "exit, ret=0x%x!\n", ret);
	ctx->result = ret;
	complete(&ctx->completion);
}

static int work_ctx_init(struct work_ctx *ctx)
{
	struct workqueue_struct *queue = NULL;
	const char *queue_name;

	queue_name = ctx->is_bsp_image ? SEC_AUTH_BSP_QUEUE_NAME : SEC_AUTH_PXP_QUEUE_NAME;
	queue = create_singlethread_workqueue(queue_name);
	if (IS_ERR_OR_NULL(queue)) {
		dprint(PRN_ERROR, "create_workqueue error\n");
		return -EFAULT;
	}

	ctx->queue = queue;
	INIT_WORK(&ctx->work, img_load_work);
	init_completion(&ctx->completion);

	return 0;
}

static void work_ctx_deinit(struct work_ctx *ctx)
{
	destroy_workqueue(ctx->queue);
}

static bool vdec_dev_is_bsp(int vdec_dev_type)
{
	if (vdec_dev_type == VCODEC_DEV_BSP)
		return true;
	else
		return false;
}

static int vdec_image_buffer_validate(void *image_buffer, size_t image_size)
{
	if (!image_buffer)
		return -EFAULT;

	if (image_size <= SEC_AUTH_CERT_SIZE)
		return -EFAULT;

	return 0;
}

int load_vdec_image(int vdec_dev_type, void *image_buffer, size_t image_size)
{
	int ret;
	struct work_ctx ctx = {0};

	dprint(PRN_ALWS, "enter\n");

	ret = vdec_image_buffer_validate(image_buffer, image_size);
	if (ret != 0) {
		dprint(PRN_ERROR, "error, invalid image buffer %d!\n", ret);
		return ret;
	}
	ctx.image_buffer = image_buffer;
	ctx.image_size = image_size;
	ctx.is_bsp_image = vdec_dev_is_bsp(vdec_dev_type);
	ret = work_ctx_init(&ctx);
	if (ret != 0) {
		dprint(PRN_ERROR, "error, init work %d!\n", ret);
		return ret;
	}

	queue_work(ctx.queue, &ctx.work);
	dprint(PRN_ALWS, "wait work complete\n");
	wait_for_completion(&ctx.completion);

	ret = ctx.result;

	work_ctx_deinit(&ctx);

	return ret;
}