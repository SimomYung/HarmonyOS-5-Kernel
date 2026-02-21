/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <product_config.h>
#include <securec.h>
#include <asm/memory.h>
#include <bsp_sec_call.h>
#include <bsp_print.h>
#include <bsp_efuse.h>
#include <bsp_dt.h>
#include <bsp_blk.h>
#include <bsp_onoff.h>
#include <teek_client_id.h>
#include <teek_client_api.h>
#include <mdrv_bmi_uapp.h>

#define THIS_MODU mod_uapp

#define UAPP_NONCE_READ 0
#define UAPP_NONCE_WRITE 1

#define UAPP_NONCE_RPMB 0
#define UAPP_NONCE_SFS 1

unsigned int g_nonce_storage_type = 0;

static int teek_init(struct teec_session *session, struct teec_context *context)
{
    unsigned int result;
    struct teec_uuid svc_uuid = TEE_SERVICE_SECBOOT;
    struct teec_operation operation = { 0 };
    u8 package_name[] = "sec_boot"; // SEC_TA_NAME
    u32 root_id = 0;

    result = teek_initialize_context(NULL, context);
    if (result != TEEC_SUCCESS) {
        bsp_err("teek_initialize_context failed, result = 0x%x!\n", result);
        return BSP_ERROR;
    }

    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramtypes = TEEC_PARAM_TYPES(
            TEEC_NONE,
            TEEC_NONE,
            TEEC_MEMREF_TEMP_INPUT,
            TEEC_MEMREF_TEMP_INPUT);
    operation.params[0x2].tmpref.buffer = (void *)(&root_id);
    operation.params[0x2].tmpref.size = sizeof(root_id);
    operation.params[0x3].tmpref.buffer = (void *)(package_name);
    operation.params[0x3].tmpref.size = strlen((char*)package_name) + 0x01;
    result = teek_open_session(
                context,
                session,
                &svc_uuid,
                TEEC_LOGIN_IDENTIFY,
                NULL,
                &operation,
                NULL);
    if (result != TEEC_SUCCESS) {
        bsp_err("teek_open_session failed,result = 0x%x!\n", result);
        teek_finalize_context(context);
        return BSP_ERROR;
    }

    return BSP_OK;
}

static int teek_cmd_session(struct teec_session *session, enum SVC_SECBOOT_CMD_ID cmd_id, unsigned int nonce_op_type, unsigned int nonce_storage_type)
{
    unsigned int result;
    struct teec_operation operation;
    unsigned int   origin;

    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramtypes = TEEC_PARAM_TYPES(
                           TEEC_VALUE_INPUT,
                           TEEC_VALUE_INPUT,
                           TEEC_NONE,
                           TEEC_NONE);
    operation.params[0].value.a = nonce_op_type;
    operation.params[0].value.b = nonce_storage_type;
    operation.params[1].value.a = bsp_sysboot_boot_is_back();

    result = teek_invoke_command(
                           session,
                           cmd_id,
                           &operation,
                           &origin);
    if (result != TEEC_SUCCESS) {
        bsp_err("invoke failed!result = 0x%x!\n", result);
        return BSP_ERROR;
    }

    return BSP_OK;
}

static inline int uapp_parse_nonce_storage_type(void)
{
    int ret;

    struct device_node *node = bsp_dt_find_compatible_node(NULL, NULL, "iot,uapp");
    if (node == NULL) {
        bsp_err("get uapp dts node failed!\n");
        return BSP_ERROR;
    }

    ret = bsp_dt_property_read_u32_array(node, "nonce_storage_type", &g_nonce_storage_type, 1);
    if (ret) {
        bsp_err("get nonce_storage_type failed 0x%x!\n", ret);
        return BSP_ERROR;
    }

    return BSP_OK;
}

static int uapp_is_support(void)
{
    if (bsp_boot_mode_is_emmc() == 1) {
        return 1;
    } else {
        return 0;
    }
}

int bsp_uapp_nonce_init(void)
{
    int ret;
    struct teec_session session;
    struct teec_context context;

    if (uapp_is_support() == 0) {
        return 0;
    }

    ret = uapp_parse_nonce_storage_type();
    if (ret == BSP_ERROR) {
        bsp_err("uapp_parse_nonce_storage_type failed, use default rpmb!\n");
    }

    ret = teek_init(&session, &context);
    if (ret == BSP_ERROR) {
        bsp_err("teek initialize context failed!\n");
        return ret;
    }

    ret = teek_cmd_session(&session, SECBOOT_CMD_ID_UAPP_NONCE, UAPP_NONCE_READ, g_nonce_storage_type);
    if (ret == BSP_ERROR) {
        bsp_err("teek_cmd_session fail 0x%x!\n", ret);
    }

    teek_close_session(&session);
    teek_finalize_context(&context);
    bsp_err("bsp_uapp_nonce_init done, type 0x%x.\n", g_nonce_storage_type);
    return ret;
}

int bsp_uapp_nonce_write(void)
{
    int ret;
    struct teec_session session;
    struct teec_context context;

    if (uapp_is_support() == 0) {
        return 0;
    }

    ret = teek_init(&session, &context);
    if (ret == BSP_ERROR) {
        bsp_err("teek initialize context failed!\n");
        return ret;
    }

    ret = teek_cmd_session(&session, SECBOOT_CMD_ID_UAPP_NONCE, UAPP_NONCE_WRITE, g_nonce_storage_type);
    if (ret == BSP_ERROR) {
        bsp_err("teek_cmd_session fail!\n");
    }

    teek_close_session(&session);
    teek_finalize_context(&context);

    return ret;
}
EXPORT_SYMBOL(bsp_uapp_nonce_write);

int mdrv_uapp_efuse_write(unsigned int cmd, unsigned int value)
{
    int ret;

    if (uapp_is_support() == 0) {
        bsp_err("uapp is not support!\n");
        return UAPP_UNSUPPORT_ERR;
    }

    // acore: pmu接口上电; teeos: efuse写
    ret = bsp_efuse_write_prepare();
    if (ret) {
        bsp_err("efuse_write_prepare failed : %d\n", ret);
        return UAPP_EFUSE_WRITE_PREPARE_ERR;
    }

    ret = bsp_sec_call_ext(FUNC_MDRV_UAPP_ENABLE, cmd, (void *)&value, sizeof(value));
    bsp_efuse_write_complete();
    if (ret) {
        bsp_err("bsp_sec_call_ext failed : %d\n", ret);
        return ret;
    }

    return 0;
}
EXPORT_SYMBOL(mdrv_uapp_efuse_write);