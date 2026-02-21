
 
#include "fbex_uece.h"

#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <securec.h>
#include <teek_client_api.h>

#include "fbex_fscrypto.h"
#include "fbex_driver.h"
#include <platform_include/see/fbe_ctrl.h>
#include <platform_include/basicplatform/linux/ufs/hufs_hcd.h>
 
#define FBEX_UECE_KEY_BLOCK_LEN 60
#define FBEX_APPKEY2_LEN 64

static DEFINE_MUTEX(g_fbex_uece_lock);

static struct completion g_fbex_uece_comp;
static struct work_struct g_fbex_uece_work;
struct workqueue_struct *g_fbex_uece_wq;

struct fbex_uece_req_info {
	struct fbex_uece_req req;
	u32 ret;
	u32 cmd;
};
static struct fbex_uece_req_info g_uece_req_info = {0};

static u32 file_encry_add_uece(u32 user_id_double, u32 user_id_single, u32 status _unused, u8 *buf _unused,
			 			   u32 length _unused)
{
	TEEC_Operation operation = { 0 };

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
											TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = user_id_double;
	operation.params[0].value.b = user_id_single;
 
	operation.started = 1;
	return fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_ADD_UECE, &operation);
}

static u32 file_encry_read_uece(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf, u32 length)
{
	TEEC_Operation operation = { 0 };
	u32 ret;

	if (!buf || length != FBEX_UECE_KEY_BLOCK_LEN) {
		pr_err("%s, input buffer is error 0x%x\n", __func__, length);
		return FBE3_ERROR_IV_BUFFER;
	}
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_OUTPUT);
	operation.params[0].tmpref.buffer = (void *)buf;
	operation.params[0].tmpref.size = length;
	operation.params[1].value.a = user_id_double;
	operation.params[1].value.b = user_id_single;
	operation.params[2].value.a = status;
 
	operation.started = 1;

	ret = fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_READ_UECE, &operation);
	if (ret != 0) {
		pr_err("%s, read uece fail, ret 0x%x\n", __func__, ret);
	} else {
		g_uece_req_info.req.length = operation.params[3].value.a;
	}
	return ret;
}

static u32 file_encry_write_uece(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf, u32 length)
{
	TEEC_Operation operation = { 0 };
 
	if (!buf || length != FBEX_UECE_KEY_BLOCK_LEN) {
		pr_err("%s, input buffer is error 0x%x\n", __func__, length);
		return FBE3_ERROR_IV_BUFFER;
	}
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_INPUT,
						TEEC_NONE);
	operation.params[0].tmpref.buffer = (void *)buf;
	operation.params[0].tmpref.size = length;
	operation.params[1].value.a = user_id_double;
	operation.params[1].value.b = user_id_single;
	operation.params[2].value.a = status;
 
	operation.started = 1;
	return fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_WRITE_UECE, &operation);
}

static u32 file_encry_del_user_pincode(u32 user_id_double, u32 user_id_single, u32 status _unused, u8 *buf _unused,
			 			   u32 length _unused)
{
	TEEC_Operation operation = { 0 };
 
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = user_id_double;
	operation.params[0].value.b = user_id_single;
 
	operation.started = 1;
	return fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_DEL_USER_PINCODE, &operation);
}

static u32 file_encry_add_appkey2(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf, u32 length)
{
	TEEC_Operation operation = { 0 };
	u32 ret;
 
	if (!buf || length != FBEX_APPKEY2_LEN) {
		pr_err("%s, input buffer is error 0x%x\n", __func__, length);
		return FBE3_ERROR_IV_BUFFER;
	}
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_OUTPUT);
	operation.params[0].tmpref.buffer = (void *)buf;
	operation.params[0].tmpref.size = length;
	operation.params[1].value.a = user_id_double;
	operation.params[1].value.b = user_id_single;
	operation.params[2].value.a = status;

	operation.started = 1;

	ret = fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_ADD_APPKEY2, &operation);
	if (ret != 0) {
		pr_err("%s, add appkey2 fail, ret 0x%x\n", __func__, ret);
	} else {
		g_uece_req_info.req.length = operation.params[3].value.a;
	}

	return ret;
}

static u32 file_encry_change_pincode(u32 user_id_double, u32 user_id_single, u32 status _unused, u8 *buf _unused,
			 			   u32 length _unused)
{
	TEEC_Operation operation = { 0 };

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = user_id_double;
	operation.params[0].value.b = user_id_single;
 
	operation.started = 1;
	return fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_CHANGE_PINCODE, &operation);
}

static u32 file_encry_lock_uece(u32 user_id_double, u32 user_id_single, u32 status _unused, u8 *buf _unused,
			 			   u32 length _unused)
{
	u32 ret = 0;
	ret = fbex_ca_lock_screen(user_id_single, FILE_UECE);
	if (ret != 0)
		pr_err("%s, single user 0x%x (double user 0x%x) lock uece error, ret %x\n", __func__, user_id_single, user_id_double, ret);
	return ret;
}

#ifdef CONFIG_FBE_DE_DOUBLE_TO_SINGLE
static u32 file_encry_add_de_double_to_single(u32 user_id_double, u32 user_id_single, u32 flag, u8 *iv,
			 			   u32 iv_len)
{
	u32 ret;
	u32 mspc_flag;
	u32 file = 0;
	u8 slot;
	
	file |= (flag << FBEX_FILE_LEN);

	TEEC_Operation operation = { 0 };

	pr_err("%s, this is double to single version interface, the double id is %d and the single id is %d\n", __func__, user_id_double, user_id_single);

#if !defined(CONFIG_FBE_MSPC_ABSENT) && defined(CONFIG_FBE3_1)
	ret = (u32)ufshcd_get_sk_state();
	if( ret != 0) {
		pr_err("%s, ufs set sk timeout, ret = 0x%x\n", __func__, ret);
		return ret;
	}
#endif
	if (!iv || iv_len != KEY_LEN) {
		pr_err("%s, input iv buffer is error 0x%x\n", __func__, iv_len);
		return FBE3_ERROR_IV_BUFFER;
	}

	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
						TEEC_VALUE_INPUT,
						TEEC_VALUE_INPUT,
						TEEC_NONE);
	operation.params[0].tmpref.buffer = (void *)iv;
	operation.params[0].tmpref.size = iv_len;
	operation.params[1].value.a = user_id_double;
	operation.params[1].value.b = user_id_single;
	operation.params[2].value.a = file;

	operation.started = 1;
	ret = fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_ADD_DE_CHANGE_USER_ID, &operation);
	if (ret == 0) {
		slot = iv[iv_len - 1];

#ifndef CONFIG_FBE_MSPC_ABSENT
#ifdef CONFIG_FBE3_1
		mspc_flag = FBE_CA_MSP_UNAVAILABLE;
		ret = fbe_ca_msp_available(&mspc_flag);
		if(ret != 0) {
			pr_err("%s, msp available is error, ret = 0x%x\n", __func__, ret);
			return ret;
		}
		if (mspc_flag == FBE_CA_MSP_AVAILABLE)
			fbex_try_preloading(user_id_single, file);
#else
		fbex_try_preloading(user_id_single, file);
#endif
#endif
		fbex_init_slot(user_id_single, file, slot);
	}
	(void)mspc_flag;
	return ret;
}
#endif


static const struct fbex_uece_function_table g_fbex_cmd_table[] = {
	{ HISI_FBEX_ADD_UECE, file_encry_add_uece },
	{ HISI_FBEX_READ_UECE, file_encry_read_uece },
	{ HISI_FBEX_WRITE_UECE, file_encry_write_uece },
	{ HISI_FBEX_DEL_USER_PINCODE, file_encry_del_user_pincode },
	{ HISI_FBEX_ADD_APPKEY2, file_encry_add_appkey2 },
	{ HISI_FBEX_CHANGE_PINCODE, file_encry_change_pincode },
	{ HISI_FBEX_LOCK_UECE, file_encry_lock_uece },
#ifdef CONFIG_FBE_DE_DOUBLE_TO_SINGLE
	{HISI_FBEX_ADD_DE_CHANGE_USER_ID, file_encry_add_de_double_to_single },
#endif
};

static const uint32_t g_fbex_cmd_num =
	sizeof(g_fbex_cmd_table) / sizeof(g_fbex_cmd_table[0]);

static void hisi_fbex_uece_fn(struct work_struct *work)
{
	uint32_t i;

	(void)work;
	for (i = 0; i < g_fbex_cmd_num; i++) {
		if (g_uece_req_info.cmd == g_fbex_cmd_table[i].cmd_id &&
			g_fbex_cmd_table[i].func != NULL) {
			g_uece_req_info.ret = g_fbex_cmd_table[i].func(g_uece_req_info.req.user_id_double,
								       g_uece_req_info.req.user_id_single,
								       g_uece_req_info.req.status,
								       g_uece_req_info.req.buf,
								       g_uece_req_info.req.length);
			goto exit;
		}
	}

	g_uece_req_info.ret = FBE3_ERROR_CMD_UNSUPPORT;
exit:
	complete(&g_fbex_uece_comp);
}

static long fbex_uece_unlocked_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	uint32_t ret;
	void __user *argp = (void __user *)(uintptr_t)arg;
 
	(void)file;
	if (argp == NULL) {
		pr_err("fbex cmd proc, user buff is NULL\n");
		return FBE3_ERROR_BUFFER_NULL;
	}

	mutex_lock(&g_fbex_uece_lock);
	ret = (uint32_t)copy_from_user(&g_uece_req_info.req, argp, sizeof(g_uece_req_info.req));
	if (ret) {
		pr_err("fbex uece, copy from user failed\n");
		g_uece_req_info.ret = FBE3_ERROR_COPY_FAIL;
		goto exit;
	}
	g_uece_req_info.cmd = cmd;
	pr_info("%s, cmd 0x%x, user_id_double 0x%x, user_id_single 0x%x, status 0x%x, len 0x%x\n", __func__,
			g_uece_req_info.cmd, g_uece_req_info.req.user_id_double, g_uece_req_info.req.user_id_single,
			g_uece_req_info.req.status, g_uece_req_info.req.length);
	queue_work(g_fbex_uece_wq, &g_fbex_uece_work);
	if (wait_for_completion_timeout(&g_fbex_uece_comp,
									msecs_to_jiffies(FBEX_KEY_TIMEOUT)) == 0) {
		pr_err("%s: timeout!\n", __func__);
		file_encry_record_error(cmd, g_uece_req_info.req.user_id_double,
								g_uece_req_info.req.status, FBE4_ERROR_UECE_TIMER_OUT);
		g_uece_req_info.ret = FBE4_ERROR_UECE_TIMER_OUT;
		goto exit;
	}

    if (g_uece_req_info.ret) {
		pr_err("fbex uece cmd = 0x%x invoke failed, ret: 0x%x\n", cmd, g_uece_req_info.ret);
		file_encry_record_error(g_uece_req_info.cmd, g_uece_req_info.req.user_id_double,
					FILE_UECE, g_uece_req_info.ret);
		goto exit;
	} else {
		pr_err("fbex uece out len %d\n", g_uece_req_info.req.length);
		ret = (uint32_t)copy_to_user(argp, &g_uece_req_info.req, sizeof(g_uece_req_info.req));
		if (ret) {
			pr_err("fbex uece, copy to user failed\n");
			g_uece_req_info.ret = FBE3_ERROR_COPY_FAIL;
		}
		(void)memset_s(&g_uece_req_info.req, sizeof(struct fbex_uece_req),
						0, sizeof(struct fbex_uece_req));
	}
exit:
	mutex_unlock(&g_fbex_uece_lock);
	pr_info("%s, cmd 0x%x exit, ret = 0x%x\n", __func__, g_uece_req_info.cmd, g_uece_req_info.ret);
	return (long)g_uece_req_info.ret;
}
 
static const struct file_operations g_fbex_uece_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = fbex_uece_unlocked_ioctl,
};
 
static struct miscdevice g_fbex_dev_uece = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fbex_uece",
	.fops = &g_fbex_uece_fops,
	.mode = 0600, /* permission */
};
 
int fbex_init_uece_dev(void)
{
	int ret;
	
	pr_info("%s, register fbex uece\n", __func__);
	ret = misc_register(&g_fbex_dev_uece);
	if (ret != 0) {
		pr_err("failed to register fbex uece, ret 0x%x\n", ret);
		return ret;
	}

	init_completion(&g_fbex_uece_comp);
	g_fbex_uece_wq = create_singlethread_workqueue("fbex_uece_work");
	if (!g_fbex_uece_wq) {
		pr_err("create fbex uece work failed\n");
		fbex_cleanup_uece_dev();
		return -1;
	}
	INIT_WORK(&g_fbex_uece_work, hisi_fbex_uece_fn);
 
	return 0;
}
 
void fbex_cleanup_uece_dev(void)
{
	if (g_fbex_dev_uece.list.prev != NULL)
		misc_deregister(&g_fbex_dev_uece);
}