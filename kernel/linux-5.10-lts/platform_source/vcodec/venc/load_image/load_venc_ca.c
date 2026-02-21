/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: image load driver
 * Create: 2023/12/27
 */

#include "load_venc.h"
#include <platform_include/see/sec_auth_ca.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/basicplatform/linux/partition/partition_ap_kernel.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <platform_include/basicplatform/linux/partition/partition_macro.h>
#include <teek_client_api.h>
#include <teek_client_id.h>
#include <linux/version.h>
#include <securec.h>

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
static int load_venc_teek_init(TEEC_Session *session, TEEC_Context *context)
{
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_SECAUTH;
	TEEC_Operation operation = {0};
	char package_name[] = SEC_AUTH_NAME;
	u32 root_id = 0;

	result = TEEK_InitializeContext(NULL, context);
	if (result != TEEC_SUCCESS) {
		pr_err("TEEK_InitializeContext fail res=0x%x\n", result);
		return (int)result;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT,
						TEEC_MEMREF_TEMP_INPUT);
	/* 2 is TEEC_RegisteredMemoryReference type */
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	/* 3 is TEEC_Value type */
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = (u32)strlen(package_name) + 1;
	result = TEEK_OpenSession(context, session, &svc_uuid,
				  TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		pr_err("TEEK_OpenSession failed result=0x%x!\n", result);
		TEEK_FinalizeContext(context);
		return (int)result;
	}

	return LOAD_VENC_OK;
}

static int load_venc_node_init(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
						 TEEC_NONE, TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_INIT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("[%s]: init ret=0x%x\n", __func__, result);
		return (int)result;
	}

	return LOAD_VENC_OK;
}
 
static int load_venc_node_read_cert(TEEC_Session *session, TEEC_Operation *operation, uintptr_t addr, u32 size)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);
 
	operation->params[1].tmpref.buffer = (void *)addr;
	operation->params[1].tmpref.size = size;
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_CERT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("[%s]: read_cert ret=0x%x\n", __func__, result);
		return (int)result;
	}

	return LOAD_VENC_OK;
}
 
static int load_venc_node_read_data(TEEC_Session *session, TEEC_Operation *operation, uintptr_t addr, u32 size)
{
	TEEC_Result result;
	u32 origin = 0;
	u32 send_size, cur_size;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT,
						 TEEC_VALUE_INPUT, TEEC_NONE);
	send_size = 0;
	while(send_size < size) {
		cur_size = load_venc_min((size - send_size), SEC_AUTH_SEND_MAX_SIZE);
		operation->params[1].tmpref.buffer = (void *)(addr + (uintptr_t)send_size);
		operation->params[1].tmpref.size = cur_size;
		operation->params[2].value.a = send_size;
		operation->params[2].value.b = size;
		result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_DATA, operation, &origin);
		if (result != TEEC_SUCCESS) {
			pr_err("[%s]: read_data send_size=%u cur_size=%u ret=0x%x\n", \
				__func__, send_size, cur_size, result);
			return (int)result;
		}
		send_size += cur_size;
	}

	return LOAD_VENC_OK;
}

static int load_venc_node_verify(TEEC_Session *session, TEEC_Operation *operation)
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
		pr_err("[%s]: verify ret=0x%x\n", __func__, result);
		return (int)result;
	}

	return LOAD_VENC_OK;
}
 
static int load_venc_node_final(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	u32 origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
						 TEEC_NONE,  TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_FINAL, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("[%s]: final ret=0x%x\n", __func__, result);
		return (int)result;
	}

	return LOAD_VENC_OK;
}

int load_verify_venc_image(void *image_buffer, size_t image_size)
{
	int ret;
	TEEC_Session session;
	TEEC_Context context;
	TEEC_Operation operation = { 0 };

	struct image_id_t id = {
		.partition_name = "vendor",
		.image_name = "venc_mcore",
	};

	pr_err("load_venc_teek_init +\n");
	ret = load_venc_teek_init(&session, &context);
	if (ret != LOAD_VENC_OK) {
		pr_err("sec_auth teek init failed, ret is 0x%x\n", ret);
		return ret;
	}
	pr_err("load_venc_teek_init -\n");

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.params[0].tmpref.buffer = (void *)&id;
	operation.params[0].tmpref.size = (uint32_t)sizeof(id);

	pr_err("load_venc_node_init +\n");
	ret = load_venc_node_init(&session, &operation);
	if (ret != LOAD_VENC_OK) {
		pr_err("load_venc node init failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_err("load_venc_node_init -\n");

	pr_err("load_venc_node_read_cert +\n");
	ret = load_venc_node_read_cert(&session, &operation, (uintptr_t)image_buffer, SEC_AUTH_CERT_SIZE);
	if (ret != LOAD_VENC_OK) {
		pr_err("load_venc node read cert failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_err("load_venc_node_read_cert -\n");

	pr_err("load_venc_node_read_data +\n");

	ret = load_venc_node_read_data(&session, &operation,
		(uintptr_t)image_buffer + SEC_AUTH_CERT_SIZE, (unsigned int)image_size - SEC_AUTH_CERT_SIZE);
	if (ret != LOAD_VENC_OK) {
		pr_err("load_venc node read data failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_err("load_venc_node_read_data -\n");

	pr_err("load_venc_node_verify +\n");
	ret = load_venc_node_verify(&session, &operation);
	if (ret != LOAD_VENC_OK) {
		pr_err("load_venc verify failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_err("load_venc_node_verify -\n");

	pr_err("load_venc_node_final +\n");
	ret = load_venc_node_final(&session, &operation);
	if (ret != LOAD_VENC_OK) {
		pr_err("load_venc final failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_err("load_venc_node_final -\n");

err_out:
	TEEK_CloseSession(&session);
	TEEK_FinalizeContext(&context);

	return ret;
}
