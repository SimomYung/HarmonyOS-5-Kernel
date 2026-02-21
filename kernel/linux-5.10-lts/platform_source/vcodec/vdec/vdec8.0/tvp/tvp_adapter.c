/*
 * tvp_adapter.c
 *
 * This is for vdec tvp adapter.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "tvp_adapter.h"
#include <teek_client_api.h>
#include "vfmw.h"
#include "vcodec_osal.h"

static uint32_t g_sec_pint_enable = 0x3;
static int32_t g_sec_init;

#define VCODEC_INVOKE_CODE_A 0x6728661c
#define VCODEC_INVOKE_CODE_B 0x5b9c660c
#define set_invoke_code(value) \
	do { \
		(value).a = VCODEC_INVOKE_CODE_A; \
		(value).b = VCODEC_INVOKE_CODE_B; \
	} while (0)

#define sec_print(type, fmt, arg...) \
	do { \
		if (type == PRN_ALWS || (g_sec_pint_enable & (1 << type)) != 0) { \
			dprint(PRN_ALWS, fmt, ##arg); \
		} \
	} while (0)

typedef enum {
	VCODEC_CMD_ID_INVALID = 0x0,
	VCODEC_CMD_ID_SUSPEND,
	VCODEC_CMD_ID_RESUME,
	VCODEC_CMD_ID_READ_PROC,
	VCODEC_CMD_ID_WRITE_PROC,
	VCODEC_CMD_ID_BSP_OPEN = 0x20,
	VCODEC_CMD_ID_PXP_OPEN = 0x21,
	VCODEC_CMD_ID_BSP_CLOSE = 0x22,
	VCODEC_CMD_ID_PXP_CLOSE = 0x23,
} tee_vcodec_cmd_id;

static TEEC_Context g_tee_context;
static TEEC_Session g_tee_session;

static TEEC_Result ca_invoke_command(TEEC_Session *g_tee_session,
	uint32_t command_id, TEEC_Operation *operation, uint32_t *returnOrigin) 
{
	TEEC_Result result;
	uint64_t invoke_begin_time;
	uint64_t invoke_cost_time;
	
	invoke_begin_time = vcodec_osal_get_time_in_us();

	result = TEEK_InvokeCommand(g_tee_session, command_id, operation, returnOrigin);

	invoke_cost_time = vcodec_osal_get_time_in_us() - invoke_begin_time;
	if (invoke_cost_time > TEE_INVOKE_EXECUTION_TIME_THRESHOLD) {
		sec_print(PRN_ALWS, "The cmd:%d invoke time exceeds the threshold,total cost %u us.\n",
		command_id, invoke_cost_time);
	}
	return result;
}

int32_t tvp_vdec_bsp_open(bsp_powerup_cfg *info)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].tmpref.buffer = (void *)info;
	operation.params[0].tmpref.size = (uint32_t)sizeof(bsp_powerup_cfg);
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_BSP_OPEN,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_BSP_OPEN Failed\n");
		return VDEC_ERR;
	}

	return VDEC_OK;
}

int32_t tvp_vdec_pxp_open(pxp_powerup_cfg *info)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].tmpref.buffer = (void *)info;
	operation.params[0].tmpref.size = (uint32_t)sizeof(pxp_powerup_cfg);
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_PXP_OPEN,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_PXP_OPEN Failed\n");
		return VDEC_ERR;
	}

	return VDEC_OK;
}

int32_t tvp_vdec_bsp_close(void)
{
	int32_t ret;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].value.a = 0;
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_BSP_CLOSE,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_BSP_CLOSE Failed\n");
		ret = VDEC_ERR;
	} else {
		ret = (int32_t)operation.params[0].value.a;
	}

	return ret;
}

int32_t tvp_vdec_pxp_close(void)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_PXP_CLOSE,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_PXP_CLOSE Failed\n");
		return VDEC_ERR;
	}

	return VDEC_OK;
}
int32_t tvp_vdec_suspend(void)
{
	int32_t ret;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].value.a = 0;
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_SUSPEND,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_SUSPEND Failed\n");
		ret = VDEC_ERR;
	} else {
		ret = (int32_t)operation.params[0].value.a;
	}

	return ret;
}

int32_t tvp_vdec_resume(void)
{
	int32_t ret;
	TEEC_Result result;
	TEEC_Operation operation = {0};

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT,
		TEEC_NONE, TEEC_NONE, TEEC_VALUE_INPUT);
	operation.started = 1;
	operation.params[0].value.a = 0;
	operation.params[0].value.b = TEEC_VALUE_UNDEF;
	set_invoke_code(operation.params[3].value);

	result = ca_invoke_command(&g_tee_session, VCODEC_CMD_ID_RESUME,
		&operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "InvokeCommand VCODEC_CMD_ID_RESUME Failed\n");
		ret = VDEC_ERR;
	} else {
		ret = (int32_t)operation.params[0].value.a;
	}

	return ret;
}

static int32_t tvp_vdec_secure_tee_init(void)
{
	TEEC_Result result;
	TEEC_Operation operation = {0};
	uint32_t uid = 0;
	const uint8_t package_name[] = "media.codec";
	TEEC_UUID svc_id = { 0xf92dbe38, 0x4d09, 0x4422, { 0xaa, 0x34, 0x14, 0x99, 0x2e, 0x2a, 0x0b, 0xf2 } };

	result = TEEK_InitializeContext(NULL, &g_tee_context);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "TEEC_InitializeContext Failed\n");
		return VDEC_ERR;
	}

	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	operation.params[TEEC_OPERATION_PARA_INDEX_SECOND].tmpref.buffer = (void *)(&uid);
	operation.params[TEEC_OPERATION_PARA_INDEX_SECOND].tmpref.size = sizeof(uid);
	operation.params[TEEC_OPERATION_PARA_INDEX_THIRD].tmpref.buffer = (void *)(package_name);
	operation.params[TEEC_OPERATION_PARA_INDEX_THIRD].tmpref.size = (uint32_t)strlen(package_name) + 1;
	sec_print(PRN_ALWS, "begin to TEEK_OpenSession\n");
	result = TEEK_OpenSession(&g_tee_context, &g_tee_session,
		&svc_id, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		sec_print(PRN_FATAL, "TEEK_OpenSession Failed\n");
		goto InitWithfree_0;
	}
	sec_print(PRN_ALWS, "TEEK_OpenSession Succ\n");
	return VDEC_OK;

InitWithfree_0:
	TEEK_FinalizeContext(&g_tee_context);
	return VDEC_ERR;
}

int32_t tvp_vdec_secure_init(void)
{
	int32_t ret;

	if (g_sec_init != 0) {
		sec_print(PRN_FATAL, "init already\n");
		return VDEC_OK;
	}

	ret = tvp_vdec_secure_tee_init();
	if (ret != EOK) {
		dprint(PRN_FATAL, "tvp_vdec_secure_tee_init err\n");
		return VDEC_ERR;
	}

	g_sec_init = 1;
	return VDEC_OK;
}

int32_t tvp_vdec_secure_exit(void)
{
	int32_t ret = VDEC_OK;

	sec_print(PRN_ERROR, "Enter\n");
	if (!g_sec_init) {
		sec_print(PRN_FATAL, "FATAL, init first\n");
		return VDEC_ERR;
	}

	TEEK_CloseSession(&g_tee_session);
	TEEK_FinalizeContext(&g_tee_context);

	sec_print(PRN_FATAL, "Success\n");
	g_sec_init = 0;

	return ret;
}
