/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat factory test function
 * Create : 2024/3/12
 */
#include <linux/module.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <teek_client_api.h>
#include <teek_client_constants.h>
#include <securec.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <linux/dma-mapping.h>
#include <platform_include/basicplatform/linux/partition/partition_ap_kernel.h>
#include <linux/io.h>
#include <linux/of_reserved_mem.h>
#include <linux/syscalls.h>
#include <seplat.h>
#include <seplat_common.h>
#include <seplat_factory.h>
#include <seplat_fmd.h>
#include <securec.h>

#define SEPLAT_CMD_MAX_PARAM_NUM      4
#define SEPLAT_FALSH_ECC_RT_TIMES     64

static struct task_struct *g_ft_flash_thread;

static DEVICE_ATTR(seplat_flash, (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP),
		   seplat_flash_ecc_show, seplat_flash_ecc_store);

enum flash_rt_status {
    FLASH_RT_STS_UNKNOWN,
    FLASH_RT_STS_START,
    FLASH_RT_STS_DOING,
    FLASH_RT_STS_FINISH,
};

struct flash_ecc_data {
    u32 err_code;
    u32 cnt_1bit;
    u32 cnt_xbit;
};

struct flash_ecc_info {
    u32 result;
    enum flash_rt_status status;
    struct flash_ecc_data rcd_data;
};
struct flash_ecc_info g_seplat_flash_rt_info = { 0 };
typedef struct seplat_teeu_param {
  enum TEEC_ParamType type;
  void *buffer; /* used for transmit buffer */
  u32 buffer_size;
  u32 value;
} seplat_teec_param;

typedef struct seplat_teeu_cmd {
  enum seplat_cmd_id cmd;
  u32 return_origin;
  seplat_teec_param params[SEPLAT_CMD_MAX_PARAM_NUM];
} seplat_teec_cmd;

static void seplat_teec_value_set_back(seplat_teec_cmd *cmd_data, TEEC_Operation *op)
{
	u32 idx;
	for (idx = 0; idx < SEPLAT_CMD_MAX_PARAM_NUM; idx++) {
		if(cmd_data->params[idx].type == TEEC_VALUE_OUTPUT || cmd_data->params[idx].type == TEEC_VALUE_INOUT) {
			cmd_data->params[idx].value = op->params[idx].value.a;
		}
	}
}

static int seplat_teec_send_cmd(TEEC_Session *session, seplat_teec_cmd *cmd_data)
{
    u32 i;
	TEEC_Result ret;
	TEEC_Operation op = { 0 };

	pr_debug("%s\n", __func__);

	if (!session || !cmd_data) {
		pr_err("%s: param error!\n", __func__);
		return SEPLAT_ERR_BASIC_NULL;
	}

	op.started = 1;
	op.cancel_flag = 0;
	op.paramTypes = TEEC_PARAM_TYPES((u8)cmd_data->params[0].type, (u8)cmd_data->params[1].type,
                                     (u8)cmd_data->params[2].type, (u8)cmd_data->params[3].type);

    for (i = 0; i < SEPLAT_CMD_MAX_PARAM_NUM; i++) {
        switch (cmd_data->params[i].type) {
        case TEEC_MEMREF_TEMP_INPUT:
        case TEEC_MEMREF_TEMP_OUTPUT:
        case TEEC_MEMREF_TEMP_INOUT:
            if ((cmd_data->params[i].buffer_size == 0) || (cmd_data->params[i].buffer == NULL)) {
                pr_err("Param error, buffer:0x%x, Size:%u!\n", cmd_data->params[i].buffer,
                       cmd_data->params[i].buffer_size);
                return SEPLAT_ERR_CMD_PARAM_DATA_INVALID;
            }
            op.params[i].tmpref.buffer = cmd_data->params[i].buffer;
            op.params[i].tmpref.size = cmd_data->params[i].buffer_size;
            break;
        case TEEC_VALUE_OUTPUT:
        case TEEC_VALUE_INPUT:
        case TEEC_VALUE_INOUT:
            op.params[i].value.a = cmd_data->params[i].value;
            break;
        case TEEC_NONE:
            pr_debug("TeecNone\n");
            break;
        default:
            pr_err("Param type 0x%x not supported!\n", cmd_data->params[i].type);
            return SEPLAT_ERR_CMD_PARAM_INVALID;
        }
    }

	ret = TEEK_InvokeCommand(session, cmd_data->cmd, &op, &cmd_data->return_origin);
	if (ret != TEEC_SUCCESS) {
		pr_err("%s, send cmd fail, res=0x%x, origin=0x%x\n", __func__, ret, cmd_data->return_origin);
		return ret;
	}

	seplat_teec_value_set_back(cmd_data, &op);

	pr_debug("%s succ\n", __func__);

	return SEPLAT_KERNEL_OK;
}

static int seplat_teec_open_session(TEEC_Context *context, TEEC_Session *session)
{
	u32 root_id = ROOTID;
	u32 origin = 0;
	const char *package_name = TEE_SERVICE_NAME;
	TEEC_UUID svc_id = UUID_TEE_SERVICE_SEPLAT;
	TEEC_Operation op = { 0 };
	TEEC_Result ret;

	pr_debug("%s\n", __func__);

	ret = TEEK_InitializeContext(NULL, context);
	if (ret != TEEC_SUCCESS) {
		pr_err("InitializeContext failed, ret=0x%x\n", ret);
		goto cleanup_1;
	}

	op.started = 1;
	op.cancel_flag = 0;
	op.params[2].tmpref.buffer = (void *)&root_id;
	op.params[2].tmpref.size = sizeof(root_id);
	op.params[3].tmpref.buffer = (void *)package_name;
	op.params[3].tmpref.size = (u32)(strlen(package_name) + 1);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);

	ret = TEEK_OpenSession(context, session, &svc_id, TEEC_LOGIN_IDENTIFY, NULL, &op, &origin);
	if (ret != TEEC_SUCCESS) {
		pr_err("%s, OpenSession fail, ret=0x%x, origin=0x%x\n", __func__, ret, origin);
		goto cleanup_2;
	}

	pr_debug("%s succ\n", __func__);
	return SEPLAT_KERNEL_OK;

cleanup_2:
	TEEK_FinalizeContext(context);
cleanup_1:
	return SEPLAT_ERR_CMD_OPENSESSION_FAIL;
}

static void seplat_teec_close_session(TEEC_Context *context, TEEC_Session *session)
{
	TEEK_CloseSession(session);
	TEEK_FinalizeContext(context);
}

static int seplat_ft_rt_flash_ecc_fmd_report(u32 result, u32 num_xbit, u32 num_1bit)
{
#ifndef CONFIG_SEPLAT_FMD_REPORT
	unused(result);
	unused(num_xbit);
	unused(num_1bit);

	pr_info("%s, fmd report close\n", __func__);

	return SEPLAT_ERR_DSM_UNSUPPORT;
#else
	int ret;
	struct seplat_dsm_event flash_ecc_event = {
		.errno           = SEPLAT_ERR_FLASH_1BIT_ECC,
		.item_id         = SEPLAT_ITEM_ID_RT_FLASH_ECC,
		.test_name       = "seplat_rt_flash_ecc",
		.min_threshold   = "xbit[0],1bit:[1]",
	};

	(void)snprintf_s(flash_ecc_event.value, SEPLAT_DSM_VALUE_MAX_LEN,
					 SEPLAT_DSM_VALUE_MAX_LEN - 1, "%u,%u", num_xbit, num_1bit);

	(void)snprintf_s(flash_ecc_event.max_threshold, SEPLAT_DSM_VALUE_MAX_LEN,
					 SEPLAT_DSM_VALUE_MAX_LEN - 1, "xbit[%d],1bit[%d]",
					 SEPLAR_FT_FLASHECC_xBIT_THRESHOLD, SEPLAR_FT_FLASHECC_1BIT_THRESHOLD);

	if (result == SEPLAT_KERNEL_OK)
		(void)snprintf_s(flash_ecc_event.result, SEPLAT_DSM_VALUE_MAX_LEN,
						 SEPLAT_DSM_VALUE_MAX_LEN - 1, "PASS");
	else
		(void)snprintf_s(flash_ecc_event.result, SEPLAT_DSM_VALUE_MAX_LEN,
						 SEPLAT_DSM_VALUE_MAX_LEN - 1, "FAIL");

	ret = seplat_dsm_report_fmd(SEPLAT_DSM_TYPE_FLASH_ECC, &flash_ecc_event, SEPLAT_FMD_EVENT_NUM_1);
	if (ret != SEPLAT_KERNEL_OK) {
		pr_err("%s fail, ret = 0x%x", __func__, ret);
		return ret;
	}

	pr_info("%s, xbit:%d, 1bit:%d\n", __func__, num_xbit, num_1bit);

	return SEPLAT_KERNEL_OK;
#endif
}

static void seplat_flash_ecc_set_success(int *ret_val)
{
    if (ret_val != NULL)
        *ret_val = SEPLAT_KERNEL_OK;

    g_seplat_flash_rt_info.rcd_data.err_code = SEPLAT_BSP_RET_OK;
    g_seplat_flash_rt_info.rcd_data.cnt_1bit = 0;
    g_seplat_flash_rt_info.rcd_data.cnt_xbit = 0;
    g_seplat_flash_rt_info.status = FLASH_RT_STS_FINISH;
}

#define SEPLAT_TA_APP_FLASH_NOT_SUPPORT   0xA05A0020
static int seplat_flash_ecc_test_proc(u32 rt_times)
{
    int ret;
    u32 idx;
    TEEC_Context ctx;
	TEEC_Session session;
    seplat_teec_cmd cmd_data = { 0 };
    struct flash_ecc_data data_temp = { 0 };

    pr_debug("%s\n", __func__);

    cmd_data.cmd = SEPLAT_TA_CMD_ID_FLASH_TEST;
    cmd_data.params[0].type = TEEC_MEMREF_TEMP_OUTPUT;
    cmd_data.params[0].buffer = (void *)&data_temp;
    cmd_data.params[0].buffer_size = sizeof(data_temp);

    ret = seplat_teec_open_session(&ctx, &session);
	if (ret != SEPLAT_KERNEL_OK) {
		pr_err("%s, open session failed!\n", __func__);
		return ret;
	}

    for (idx = 0; idx < rt_times; idx++) {
        (void)memset_s(&data_temp, sizeof(data_temp), 0, sizeof(data_temp));

        ret = seplat_teec_send_cmd(&session, &cmd_data);
        if (ret == SEPLAT_TA_APP_FLASH_NOT_SUPPORT) {
            pr_err("%s,command not supported in sm_dis\n", __func__);
            seplat_flash_ecc_set_success(&ret);
            break;
        }
        if (ret != SEPLAT_KERNEL_OK) {
            pr_err("%s, send cmd failed, run %d/%d\n", __func__, idx, rt_times);
            g_seplat_flash_rt_info.result = SEPLAT_ERR_FT_RESULT_FAIL;
            break;
        }

        g_seplat_flash_rt_info.rcd_data.cnt_1bit += data_temp.cnt_1bit;
        g_seplat_flash_rt_info.rcd_data.cnt_xbit += data_temp.cnt_xbit;
        g_seplat_flash_rt_info.rcd_data.err_code = data_temp.err_code;

        if (g_seplat_flash_rt_info.rcd_data.err_code != SEPLAT_BSP_RET_OK) {
            pr_err("%s, errcode exit, run %d/%d\n", __func__, idx, rt_times);
            break;
        }
    }

    pr_debug("%s, [pass/Total]:%u/%u\n", __func__, idx, rt_times);
    seplat_teec_close_session(&ctx, &session);

    pr_debug("%s, exit\n", __func__);

    return ret;
}

static void seplat_flash_ecc_test_report(struct flash_ecc_info *data)
{
    u32 result_data = SEPLAT_KERNEL_OK;
    pr_debug("%s\n", __func__);

    if (data->result != SEPLAT_ERR_FT_RESULT_DEFAULT && data->result != SEPLAT_KERNEL_OK) {
        bbox_diaginfo_record(DMD_HISES_FLASH_ECC_E, NULL, "seplat_host_expt:0x%x", data->result);
        result_data = data->result;
    } else if (data->rcd_data.err_code != SEPLAT_BSP_RET_OK) {
        bbox_diaginfo_record(DMD_HISES_FLASH_ECC_E, NULL,
                             "seplat_expt:0x%x seplat_xbit_ecc:%u seplat_1bit_ecc:%u",
                             data->rcd_data.err_code,
                             data->rcd_data.cnt_xbit,
                             data->rcd_data.cnt_1bit);
        result_data = SEPLAT_ERR_FT_RESULT_FAIL;
    } else if (data->rcd_data.cnt_xbit != 0) {
        bbox_diaginfo_record(DMD_HISES_FLASH_ECC_2B, NULL, "seplat_xbit_ecc:%u seplat_1bit_ecc:%u",
                             data->rcd_data.cnt_xbit, data->rcd_data.cnt_1bit);
        result_data = SEPLAT_ERR_FT_RESULT_FAIL;
    } else if (data->rcd_data.cnt_1bit != 0) {
        if (data->rcd_data.cnt_1bit >= SEPLAR_FT_FLASHECC_1BIT_THRESHOLD) {
            bbox_diaginfo_record(DMD_HISES_FLASH_ECC_1B, NULL, "seplat_1bit_ecc:%u", data->rcd_data.cnt_1bit);
            result_data = SEPLAT_ERR_FT_RESULT_FAIL;
        }
    }

    data->result = result_data;

    pr_debug("%s log:\n", __func__);
    pr_debug("result: 0x%x\n", data->result);
    pr_debug("status: %d\n", data->status);
    pr_debug("err_code: 0x%x\n", data->rcd_data.err_code);
    pr_debug("cnt_1bit: %d\n", data->rcd_data.cnt_1bit);
    pr_debug("cnt_xbit: %d\n", data->rcd_data.cnt_xbit);

	if (data->rcd_data.cnt_xbit != 0 || data->rcd_data.cnt_1bit != 0)
		(void)seplat_ft_rt_flash_ecc_fmd_report(result_data, data->rcd_data.cnt_xbit, data->rcd_data.cnt_1bit);
}

static int seplat_flash_ecc_proc_thread(void *arg)
{
    int ret;

    unused(arg);

    g_seplat_flash_rt_info.status = FLASH_RT_STS_DOING;

    ret = seplat_flash_ecc_test_proc(SEPLAT_FALSH_ECC_RT_TIMES);
    if (ret != SEPLAT_KERNEL_OK) {
        pr_err("%s, expt ret 0x%x\n", __func__, ret);
        g_seplat_flash_rt_info.result = ret;
    }

    g_seplat_flash_rt_info.status = FLASH_RT_STS_FINISH;
    seplat_flash_ecc_test_report(&g_seplat_flash_rt_info);

    kthread_stop(g_ft_flash_thread);
    return SEPLAT_KERNEL_OK;
}

static int seplat_ft_flash_ecc_init(struct flash_ecc_info *rt_info)
{
    int ret;

	ret = memset_s(rt_info, sizeof(struct flash_ecc_info), 0, sizeof(struct flash_ecc_info));
	if (ret != EOK) {
		pr_err("memset_s failed, ret:0x%x\n", ret);
        rt_info->status = FLASH_RT_STS_UNKNOWN;
		return SEPLAT_ERR_BASIC_MEMORY;
	}

    rt_info->result = SEPLAT_ERR_FT_RESULT_DEFAULT;
    rt_info->status = FLASH_RT_STS_START;
    rt_info->rcd_data.err_code = SEPLAT_BSP_RET_OK;

    return SEPLAT_KERNEL_OK;
}

static int seplat_ft_flash_ecc_start(void)
{
	int ret;

    pr_debug("%s\n", __func__);

    /* 1. check chiptype */
    if (!seplat_chiptype_exist_get()) {
        pr_err("%s, chiptype not exist \n", __func__);
        return SEPLAT_KERNEL_OK;
    }

    if (g_seplat_flash_rt_info.status == FLASH_RT_STS_DOING) {
         pr_err("%s, rt doing, wait\n", __func__);
         return SEPLAT_ERR_FT_STATUS_BUSY;
    }

    /* 2. flash_ecc rt init */
    ret = seplat_ft_flash_ecc_init(&g_seplat_flash_rt_info);
    if (ret != SEPLAT_KERNEL_OK) {
        pr_err("%s, init fail, ret = 0x%x\n", __func__, ret);
        return ret;
    }

    /* 3. flash_ecc rt proc at thread */
	g_ft_flash_thread = kthread_run(seplat_flash_ecc_proc_thread, NULL, "seplat_rt_flash_ecc");
	if (IS_ERR(g_ft_flash_thread)) {
        g_seplat_flash_rt_info.status = FLASH_RT_STS_UNKNOWN;
		pr_err("cat kthread_create failed\n");
		return SEPLAT_ERR_FT_THREAD_START_FAIL;
	}

    return SEPLAT_KERNEL_OK;
}

static int seplat_ft_flash_ecc_result(u32 *result_data)
{
    if (g_seplat_flash_rt_info.status == FLASH_RT_STS_DOING) {
        pr_err("%s flash rt not finish\n", __func__);
        return SEPLAT_ERR_FT_STATUS_WRONG;
    }

    *result_data = (u32)g_seplat_flash_rt_info.result;

    return SEPLAT_KERNEL_OK;
}

/**
 * @brief      : seplat_flash_ecc_show : get flash ECC RT result.
 *
 * @param[in]  : dev  : Devices pointer.
 * @param[in]  : attr : Devices attribution.
 * @param[out] : buf  : The buffer to store out data.
 *
 * @return     : >= 0: The size of output data, Others: failed.
 */
ssize_t seplat_flash_ecc_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int ret;
    int result;
    unused(dev);
    unused(attr);

    if (buf == NULL) {
        pr_err("%s buf null\n", __func__);
        return SEPLAT_ERR_BASIC_NULL;
    }

    ret = seplat_ft_flash_ecc_result(&result);
    if (ret != SEPLAT_KERNEL_OK)
        return ret;

    if (memcpy_s(buf, sizeof(result), &result, sizeof(result)) != EOK) {
        pr_err("%s memcpy fail\n", __func__);
        return SEPLAT_ERR_FT_FILE_WRITE_OUT;
    }

    /* clear data after read */
    ret = seplat_ft_flash_ecc_init(&g_seplat_flash_rt_info);
    if (ret != SEPLAT_KERNEL_OK) {
        pr_err("%s, clear fail, ret = 0x%x\n", __func__, ret);
        return ret;
    }

    pr_info("%s, result:0x%x\n", __func__, result);

	return sizeof(result);
}

ssize_t seplat_flash_ecc_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int ret;

    unused(dev);
    unused(attr);
    unused(buf);
    unused(count);

    if (buf == NULL) {
        pr_err("%s buf null\n", __func__);
        return SEPLAT_ERR_FT_FILE_LEN_INVALID;
    }

    ret = seplat_ft_flash_ecc_start();
    if (ret != SEPLAT_KERNEL_OK)
        return SEPLAT_ERR_FT_FILE_LEN_INVALID;

    pr_debug("%s, start\n", __func__);

	return strlen(buf);
}

int32_t seplat_ft_create_files(struct device *pdevice)
{
    int32_t ret = SEPLAT_KERNEL_OK;

	if (device_create_file(pdevice, &dev_attr_seplat_flash) != 0) {
		ret = SEPLAT_ERR_FT_FILE_CREATE;
		pr_err("%s: failed!\n", __func__);
	}

    return ret;
}

void seplat_ft_remove_files(struct device *pdevice)
{
    device_remove_file(pdevice, &dev_attr_seplat_flash);
    pr_err("%s success!\n", __func__);
}

static int seplat_ft_get_dieid_proc(u8 *buff, u32 len, u32 *out_len)
{
	int ret;
	TEEC_Context ctx;
	TEEC_Session session;
	seplat_teec_cmd cmd_data = { 0 };

	if (!buff || !out_len) {
		pr_err("%s, para err\n", __func__);
		return SEPLAT_ERR_BASIC_PARAMS;
	}

	cmd_data.cmd = SEPLAT_TA_CMD_ID_GET_DIEID_INFO;
	cmd_data.params[TEEC_PARAM_INDEX_0].type         = TEEC_MEMREF_TEMP_OUTPUT;
	cmd_data.params[TEEC_PARAM_INDEX_0].buffer       = buff;
	cmd_data.params[TEEC_PARAM_INDEX_0].buffer_size  = len;
	cmd_data.params[TEEC_PARAM_INDEX_1].type         = TEEC_VALUE_OUTPUT;
	cmd_data.params[TEEC_PARAM_INDEX_1].value        = 0;

	ret = seplat_teec_open_session(&ctx, &session);
	if (ret != SEPLAT_KERNEL_OK) {
		pr_err("%s, open session fail!\n", __func__);
		return ret;
	}

	ret = seplat_teec_send_cmd(&session, &cmd_data);
	if (ret != SEPLAT_KERNEL_OK) {
		pr_err("%s, send cmd fail, ret = 0x%x\n", __func__, ret);
		return ret;
	}

	*out_len = cmd_data.params[TEEC_PARAM_INDEX_1].value;

	seplat_teec_close_session(&ctx, &session);

	return SEPLAT_KERNEL_OK;
}

int seplat_ft_get_dieid(char *dieid, u32 len)
{
	int ret;
	u32 i;
	u32 out_len = 0;
	u8 buff[SEPLAT_FT_DIEID_SIZE_MAX] = { 0 };

	pr_info("%s\n", __func__);

	ret = seplat_ft_get_dieid_proc(buff, sizeof(buff), &out_len);
	if (ret != SEPLAT_KERNEL_OK) {
		pr_err("%s fail, out_len = %d, ret = 0x%x\n", __func__, out_len, ret);
		return ret;
	}

	if (out_len >= SEPLAT_FT_DIEID_SIZE_MAX)
		return SEPLAT_ERR_FT_DIEID_LEN_INVALID;

	ret = snprintf_s(dieid, len, len - 1, "%s:0x", SEPLAT_DESENSITIVE_NAME);
	seplat_chk_err_return_len(ret);

	for (i = 0; i < out_len; i++) {
		ret = snprintf_s(dieid + strlen(dieid), len - strlen(dieid), len - strlen(dieid) - 1, "%02x", buff[i]);
		seplat_chk_err_return_len(ret);
	}

	ret = snprintf_s(dieid + strlen(dieid), len - strlen(dieid), len - strlen(dieid) - 1, "\r\n");
	seplat_chk_err_return_len(ret);

	pr_info("%s, exit, dieid=%s\n", __func__, dieid);

	return SEPLAT_KERNEL_OK;
}
