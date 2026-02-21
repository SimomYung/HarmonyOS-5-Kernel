/*
 * Copyright (c) Huawei Technopr_infoes Co., Ltd. 2016-2019. All rights reserved.
 * Description: iomcu image load driver
 * Create: 2023/06/15
 */
#include "load_iomcu_ca.h"
#include <platform_include/see/sec_auth_ca.h>
#include <linux/version.h>
#include <linux/fs.h>
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
#ifdef CONFIG_HEADER_FILE_REPLACE_EXTRACT
#include <iomcu_mem.h>
#else
#include <iomcu_ddr_map.h>
#endif

#define DEVICE_PATH  "/dev/block/bootdevice/by-name/"
#ifndef DDR_F2L_IN_IMAGE_AREA_SIZE
#define DDR_F2L_IN_IMAGE_AREA_SIZE 0x1000
#endif
#define IOMCU_IMG_MAX_SIZE            (DDR_IMAGE_SIZE - SEC_AUTH_CERT_SIZE - DDR_F2L_IN_IMAGE_AREA_SIZE)
#define IOMCU_CA_SEND_MAX_SIZE        0x100000 /* 1M */
#define load_iomcu_min(a, b)          ((a) < (b) ? (a) : (b))
#define SEC_AUTH_NAME                 "sec_auth"

static struct image_id_t iomcu_image_id = {
	.partition_name = PART_SENSORHUB,
	.image_name = "sensorhub",
};

static int iomcu_read_bin(unsigned int offset, unsigned int length, char *buffer)
{
	int ret = LOAD_IOMCU_ERROR;
	char *pathname = NULL;
	u64 pathlen;
	struct file *fp = NULL;
	loff_t read_offset = offset;

	if (!buffer) {
		pr_err("buffer is null\n");
		return ret;
	}

	/* get resource, add 1 for last '\0' */
	pathlen = strlen(DEVICE_PATH) + 1 + strnlen(iomcu_image_id.partition_name, (u64)PARTITION_NAME_LEN_MAX);
	pathname = kzalloc(pathlen, GFP_KERNEL);
	if (!pathname) {
		pr_err("pathname kzalloc failed\n");
		return ret;
	}

	ret = flash_find_ptn_s((const char *)iomcu_image_id.partition_name, pathname, pathlen);
	if (ret != 0) {
		pr_err("partion_name(%s) not in ptable, ret=0x%x\n",
			      iomcu_image_id.partition_name, ret);
		goto free_pname;
	}
	/* Only read file */
	fp = filp_open(pathname, O_RDONLY, 0400);
	if (IS_ERR(fp)) {
		pr_err("filp_open(%s) failed, errorno:%ld\n", pathname, PTR_ERR(fp));
		ret = LOAD_IOMCU_ERROR;
		goto free_pname;
	}

	ret = kernel_read(fp, buffer, length, &read_offset);
	if (ret != (int)length) {
		pr_err("read ops fail ret=0x%x len=0x%x\n", ret, length);
		goto close_file;
	}
	ret = LOAD_IOMCU_OK;

close_file:
	filp_close(fp, NULL);

free_pname:
	kfree(pathname);

	return ret;
}

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
static int load_iomcu_teek_init(TEEC_Session *session, TEEC_Context *context)
{
	TEEC_Result result;
	TEEC_UUID svc_uuid = TEE_SERVICE_SECAUTH;
	TEEC_Operation operation = { 0 };
	char package_name[] = SEC_AUTH_NAME;
	unsigned int root_id = 0;

	result = TEEK_InitializeContext(NULL, context);
	if (result != TEEC_SUCCESS) {
		pr_err("TEEK_InitializeContext fail res=0x%x\n", result);
		return (int)result;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);
	/* 2 is TEEC_RegisteredMemoryReference type */
	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = sizeof(root_id);
	/* 3 is TEEC_Value type */
	operation.params[3].tmpref.buffer = (void *)(package_name);
	operation.params[3].tmpref.size = strlen(package_name) + 1;
	result = TEEK_OpenSession(context, session, &svc_uuid,
				  TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		pr_err("TEEK_OpenSession failed result=0x%x!\n", result);
		TEEK_FinalizeContext(context);
		return (int)result;
	}

	return LOAD_IOMCU_OK;
}

static int load_iomcu_node_init(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	unsigned int origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_INIT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("init ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_IOMCU_OK;
}

static int load_iomcu_node_read_cert(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	int ret = LOAD_IOMCU_ERROR;
	unsigned int origin = 0;
	uintptr_t cert_addr = 0;

	cert_addr = (uintptr_t)vmalloc(SEC_AUTH_CERT_SIZE);
	if (!cert_addr) {
		pr_err("cert addr malloc failed\n");
		return ret;
	}
	ret = iomcu_read_bin(0, SEC_AUTH_CERT_SIZE, (char *)cert_addr);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("prepare iomcu cert ret=%d\n", ret);
		goto err;
	}

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE);
	operation->params[1].tmpref.buffer = (void *)cert_addr;
	operation->params[1].tmpref.size = SEC_AUTH_CERT_SIZE;
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_CERT, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("read_cert ret=0x%x\n", result);
		ret = (int)result;
		goto err;
	}

err:
	if (cert_addr != 0) {
		(void)memset_s((void*)cert_addr, SEC_AUTH_CERT_SIZE, 0, SEC_AUTH_CERT_SIZE);
		vfree((void*)cert_addr);
	}

	return ret;
}

static int load_iomcu_node_read_data(TEEC_Session *session, TEEC_Operation *operation)
{
	int ret = LOAD_IOMCU_ERROR;
	TEEC_Result result;
	unsigned int origin = 0;
	unsigned int send_size, cur_size;
	uintptr_t data_addr = 0;

	data_addr = (uintptr_t)vmalloc(IOMCU_CA_SEND_MAX_SIZE);
	if (!data_addr) {
		pr_err("data addr malloc failed\n");
		return ret;
	}

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT, TEEC_VALUE_INPUT, TEEC_NONE);
	send_size = 0;
	while(send_size < IOMCU_IMG_MAX_SIZE) {
		cur_size = load_iomcu_min((IOMCU_IMG_MAX_SIZE - send_size), IOMCU_CA_SEND_MAX_SIZE);
		ret = iomcu_read_bin(SEC_AUTH_CERT_SIZE + send_size, cur_size, (char *)data_addr);
		if (ret != LOAD_IOMCU_OK) {
			pr_err("prepare iomcu data ret=%d\n", ret);
			goto err;
		}
		operation->params[1].tmpref.buffer = (void *)data_addr;
		operation->params[1].tmpref.size = cur_size;
		operation->params[2].value.a = send_size;
		operation->params[2].value.b = IOMCU_IMG_MAX_SIZE;
		result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_READ_DATA, operation, &origin);
		if (result != TEEC_SUCCESS) {
			pr_err("read_data send_size=%u cur_size=%u ret=0x%x\n", send_size, cur_size, result);
			ret = (int)result;
			goto err;
		}
		send_size += cur_size;
	}

err:
	if (data_addr != 0) {
		(void)memset_s((void*)data_addr, IOMCU_CA_SEND_MAX_SIZE, 0, IOMCU_CA_SEND_MAX_SIZE);
		vfree((void*)data_addr);
	}

	return ret;
}

static int load_iomcu_node_verify(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	unsigned int origin = 0;
	struct authed_data out = { 0 };

	operation->params[1].tmpref.buffer = (void *)&out;
	operation->params[1].tmpref.size = sizeof(out);
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEE_PARAM_TYPE_MEMREF_INOUT, TEEC_NONE, TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_VERIFY, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("verify ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_IOMCU_OK;
}

static int load_iomcu_node_final(TEEC_Session *session, TEEC_Operation *operation)
{
	TEEC_Result result;
	unsigned int origin = 0;

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE, TEEC_NONE,  TEEC_NONE);
	result = TEEK_InvokeCommand(session, SEC_AUTH_CMD_ID_FINAL, operation, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("final ret=0x%x\n", result);
		return (int)result;
	}

	return LOAD_IOMCU_OK;
}

int load_and_verify_iomcu_image(void)
{
	int ret;
	TEEC_Session session;
	TEEC_Context context;
	TEEC_Operation operation = { 0 };

	pr_info("load_iomcu_teek_init +\n");
	ret = load_iomcu_teek_init(&session, &context);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("sec_auth teek init failed, ret is 0x%x\n", ret);
		return ret;
	}
	pr_info("load_iomcu_teek_init -\n");

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.params[0].tmpref.buffer = (void *)&iomcu_image_id;
	operation.params[0].tmpref.size = sizeof(iomcu_image_id);

	pr_info("load_iomcu_node_init +\n");
	ret = load_iomcu_node_init(&session, &operation);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("load_iomcu node init failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_info("load_iomcu_node_init -\n");

	pr_info("load_iomcu_node_read_cert +\n");
	ret = load_iomcu_node_read_cert(&session, &operation);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("load_iomcu node read cert failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_info("load_iomcu_node_read_cert -\n");

	pr_info("load_iomcu_node_read_data +\n");
	ret = load_iomcu_node_read_data(&session, &operation);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("load_iomcu node read data failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_info("load_iomcu_node_read_data -\n");

	pr_info("load_iomcu_node_verify +\n");
	ret = load_iomcu_node_verify(&session, &operation);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("load_iomcu verify failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_info("load_iomcu_node_verify -\n");

	pr_info("load_iomcu_node_final +\n");
	ret = load_iomcu_node_final(&session, &operation);
	if (ret != LOAD_IOMCU_OK) {
		pr_err("load_iomcu final failed, ret is 0x%x\n", ret);
		goto err_out;
	}
	pr_info("load_iomcu_node_final -\n");

err_out:
	TEEK_CloseSession(&session);
	TEEK_FinalizeContext(&context);

	return ret;
}
