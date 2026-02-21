/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: module module_dev_secinfo do things in tee source
 * Create: 2025/1/18
 */
#include <module_update_by_tee.h>
#include <module_update_errno.h>
#include <module_update_plat.h>
#include <platform_include/see/dev_secinfo_auth_ca.h>
#include <teek_client_api.h>
#include <teek_client_id.h>
#include <securec.h>

static u32 module_update_teek_init_ctx(TEEC_Context *ctx)
{
	TEEC_Result ret;

	ret = TEEK_InitializeContext(NULL, ctx);
	if (ret != TEEC_SUCCESS) {
		module_update_err("error,InitCtx ret=0x%x\n", ret);
		return MODULE_UPDATE_ERR_TEEK_INIT_CTX;
	}

	return MODULE_UPDATE_OK;
}

static u32 module_update_teek_open_session(TEEC_Context *ctx, TEEC_Session *session)
{
	TEEC_Result ret;
	TEEC_UUID uuid = TEE_SERVICE_DEV_SECINFO_AUTH;
	TEEC_Operation operation = {0};
	char package_name[] = DEV_SECINFO_AUTH_NAME;
	u32 root_id = 0;

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT);
	/* 2 is TEEC_RegisteredMemoryReference type */
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = (u32)sizeof(root_id);
	/* 3 is TEEC_Value type */
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = (u32)strlen(package_name) + 1;
	ret = TEEK_OpenSession(ctx, session, &uuid, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (ret != TEEC_SUCCESS) {
		module_update_err("error,OpenSession ret=0x%x\n", ret);
		return MODULE_UPDATE_ERR_TEEK_OPEN_SESSION;
	}

	return MODULE_UPDATE_OK;
}

static u32 module_update_teek_close_session(TEEC_Session *session)
{
	TEEK_CloseSession(session);

	return MODULE_UPDATE_OK;
}

static u32 module_update_teek_final_ctx(TEEC_Context *ctx)
{
	TEEK_FinalizeContext(ctx);

	return MODULE_UPDATE_OK;
}

u32 module_update_verify_hash_by_tee(char *img_name, u32 img_name_size, u8 *data, u32 size)
{
	if (data == NULL || size == 0) {
		module_update_err("error, input param invalid\n");
		return MODULE_UPDATE_PTR_NULL;
	}
	u32 ret;
	TEEC_Session session;
	TEEC_Context ctx;
	TEEC_Operation operation = { 0 };
	u32 origin = 0;
	u32 cmd = DEV_SECINFO_AUTH_CMD_ID_MODULE_UPDATE_VERIFY_HASH;
	ret = module_update_teek_init_ctx(&ctx);
	if (ret != MODULE_UPDATE_OK) {
		module_update_err("error, init_ctx ret=0x%x\n", ret);
		return ret;
	}

	ret = module_update_teek_open_session(&ctx, &session);
	if (ret != MODULE_UPDATE_OK) {
		module_update_err("error, open_session ret=0x%x\n", ret);
		goto final_ctx;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = img_name;
	operation.params[0].tmpref.size = img_name_size;
	operation.params[1].tmpref.buffer = data;
	operation.params[1].tmpref.size = size;
	ret = TEEK_InvokeCommand(&session, cmd, &operation, &origin);
	if (ret != 0) {
		module_update_err("error,invoke cmd 0x%x ret=0x%x\n", cmd, ret);
		goto close_session;
	}

close_session:
	(void)module_update_teek_close_session(&session);

final_ctx:
	(void)module_update_teek_final_ctx(&ctx);

	return ret;
}

u32 module_update_verify_bindfile_by_tee(u8 *data, u32 size)
{
	if (data == NULL || size == 0) {
		module_update_err("error, input param invalid\n");
		return MODULE_UPDATE_PTR_NULL;
	}
	u32 ret;
	TEEC_Session session;
	TEEC_Context ctx;
	TEEC_Operation operation = { 0 };
	u32 origin = 0;
	u32 cmd = DEV_SECINFO_AUTH_CMD_ID_MODULE_UPDATE_VERIFY_BF;
	ret = module_update_teek_init_ctx(&ctx);
	if (ret != MODULE_UPDATE_OK) {
		module_update_err("error, init_ctx ret=0x%x\n", ret);
		return ret;
	}

	ret = module_update_teek_open_session(&ctx, &session);
	if (ret != MODULE_UPDATE_OK) {
		module_update_err("error, open_session ret=0x%x\n", ret);
		goto final_ctx;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	operation.params[0].tmpref.buffer = data;
	operation.params[0].tmpref.size = size;
	ret = TEEK_InvokeCommand(&session, cmd, &operation, &origin);
	if (ret != 0) {
		module_update_err("error,invoke cmd 0x%x ret=0x%x\n", cmd, ret);
		goto close_session;
	}

close_session:
	(void)module_update_teek_close_session(&session);

final_ctx:
	(void)module_update_teek_final_ctx(&ctx);

	return ret;
}
