
 
#include "fbex_uece_enhance.h"

#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <securec.h>
#include <teek_client_api.h>

#include "fbex_driver.h"

static DEFINE_MUTEX(g_fbex_uece_enhance_lock);

static struct completion g_fbex_uece_enhance_comp;
static struct work_struct g_fbex_uece_enhance_work;
struct workqueue_struct *g_fbex_uece_enhance_wq;

struct fbex_uece_enhance_req_info {
	struct fbex_uece_enhance_req req;
	u32 ret;
	u32 cmd;
};
static struct fbex_uece_enhance_req_info g_uece_enhance_req_info = {0};

static u32 file_encry_get_current_uece_avail_status(u32 user_id_double, u32 user_id_single, u32 status, u8 *buf _unused,
			 			   u32 length _unused){
	TEEC_Operation operation = { 0 };	
	u32 ret;
	
        operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_VALUE_OUTPUT,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = user_id_double;
	operation.params[0].value.b = user_id_single;
    
 
	operation.started = 1;
	ret = fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_GET_CURRENT_UECE_AVAIL_STATUS, &operation);
	if (ret != 0) {
		pr_err("%s, get current uece avail status fail, ret 0x%x\n", __func__, ret);
	} else {
		g_uece_enhance_req_info.req.status = operation.params[1].value.a;
	}
	return ret;
}
 
static u32 file_encry_delete_uece_authtoken(u32 user_id_double, u32 user_id_single,u32 status _unused, u8 *buf _unused,
			 			   u32 length _unused)
{
	TEEC_Operation operation = { 0 };
 
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = user_id_double;
	operation.params[0].value.b = user_id_single;
    
	operation.started = 1;
	return fbe_ca_invoke_command(SEC_FILE_ENCRY_CMD_ID_DEL_UECE_AUTHTOKEN, &operation);
}


static const struct fbex_uece_enhance_function_table g_fbex_uece_enhance_cmd_table[] = {
	{ HISI_FBEX_GET_CURRENT_UECE_AVAIL_STATUS, file_encry_get_current_uece_avail_status },
	{ HISI_FBEX_DELETE_UECE_AUTHTOKEN, file_encry_delete_uece_authtoken },
};

static const uint32_t g_fbex_uece_enhance_cmd_num =
	sizeof(g_fbex_uece_enhance_cmd_table) / sizeof(g_fbex_uece_enhance_cmd_table[0]);

static void hisi_fbex_uece_enhance_fn(struct work_struct *work)
{
	uint32_t i;

	(void)work;
	for (i = 0; i < g_fbex_uece_enhance_cmd_num; i++) {
		if (g_uece_enhance_req_info.cmd == g_fbex_uece_enhance_cmd_table[i].cmd_id &&
			g_fbex_uece_enhance_cmd_table[i].func != NULL) {
			g_uece_enhance_req_info.ret = g_fbex_uece_enhance_cmd_table[i].func(g_uece_enhance_req_info.req.user_id_double,
								       g_uece_enhance_req_info.req.user_id_single,
								       g_uece_enhance_req_info.req.status,
								       g_uece_enhance_req_info.req.buf,
								       g_uece_enhance_req_info.req.length);
			goto exit;
		}
	}

	g_uece_enhance_req_info.ret = FBE3_ERROR_CMD_UNSUPPORT;
exit:
	complete(&g_fbex_uece_enhance_comp);
}

static long fbex_uece_enhance_unlocked_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	uint32_t ret;
	void __user *argp = (void __user *)(uintptr_t)arg;
 
	(void)file;
	if (argp == NULL) {
		pr_err("fbex uece enhance cmd proc, user buff is NULL\n");
		return FBE3_ERROR_BUFFER_NULL;
	}

	mutex_lock(&g_fbex_uece_enhance_lock);
	ret = (uint32_t)copy_from_user(&g_uece_enhance_req_info.req, argp, sizeof(g_uece_enhance_req_info.req));
	if (ret) {
		pr_err("fbex uece enhance, copy from user failed\n");
		g_uece_enhance_req_info.ret = FBE3_ERROR_COPY_FAIL;
		goto exit;
	}
	g_uece_enhance_req_info.cmd = cmd;
	pr_info("%s, cmd 0x%x, user_id_double 0x%x, user_id_single 0x%x, status 0x%x, len 0x%x\n", __func__,
			g_uece_enhance_req_info.cmd, g_uece_enhance_req_info.req.user_id_double, g_uece_enhance_req_info.req.user_id_single,
			g_uece_enhance_req_info.req.status, g_uece_enhance_req_info.req.length);
	queue_work(g_fbex_uece_enhance_wq, &g_fbex_uece_enhance_work);
	if (wait_for_completion_timeout(&g_fbex_uece_enhance_comp,
									msecs_to_jiffies(FBEX_KEY_TIMEOUT)) == 0) {
		pr_err("%s: timeout!\n", __func__);
		file_encry_record_error(cmd, g_uece_enhance_req_info.req.user_id_double,
								g_uece_enhance_req_info.req.status, FBE4_ERROR_UECE_ENHANCE_TIMER_OUT);
		g_uece_enhance_req_info.ret = FBE4_ERROR_UECE_TIMER_OUT;
		goto exit;
	}

    if (g_uece_enhance_req_info.ret) {
		pr_err("fbex uece enhance cmd = 0x%x invoke failed, ret: 0x%x\n", cmd, g_uece_enhance_req_info.ret);
		file_encry_record_error(g_uece_enhance_req_info.cmd, g_uece_enhance_req_info.req.user_id_double,
					FILE_UECE, g_uece_enhance_req_info.ret);
		goto exit;
	} else {
		pr_err("fbex uece enhance out len %d\n", g_uece_enhance_req_info.req.length);
		ret = (uint32_t)copy_to_user(argp, &g_uece_enhance_req_info.req, sizeof(g_uece_enhance_req_info.req));
		if (ret) {
			pr_err("fbex uece enhance, copy to user failed\n");
			g_uece_enhance_req_info.ret = FBE3_ERROR_COPY_FAIL;
		}
		(void)memset_s(&g_uece_enhance_req_info.req, sizeof(struct fbex_uece_enhance_req),
						0, sizeof(struct fbex_uece_enhance_req));
	}
exit:
	mutex_unlock(&g_fbex_uece_enhance_lock);
	pr_info("%s, cmd 0x%x exit, ret = 0x%x\n", __func__, g_uece_enhance_req_info.cmd, g_uece_enhance_req_info.ret);
	return (long)g_uece_enhance_req_info.ret;
}
 
static const struct file_operations g_fbex_uece_enhance_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = fbex_uece_enhance_unlocked_ioctl,
};
 
static struct miscdevice g_fbex_dev_uece_enhance = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fbex_uece_enhance",
	.fops = &g_fbex_uece_enhance_fops,
	.mode = 0600, /* permission */
};
 
int fbex_init_uece_enhance_dev(void)
{
	int ret;
	
	pr_info("%s, register fbex uece enhance\n", __func__);
	ret = misc_register(&g_fbex_dev_uece_enhance);
	if (ret != 0) {
		pr_err("failed to register fbex uece enhance, ret 0x%x\n", ret);
		return ret;
	}

	init_completion(&g_fbex_uece_enhance_comp);
	g_fbex_uece_enhance_wq = create_singlethread_workqueue("fbex_uece_enhance_work");
	if (!g_fbex_uece_enhance_wq) {
		pr_err("create fbex uece enhance work failed\n");
		fbex_cleanup_uece_enhance_dev();
		return -1;
	}
	INIT_WORK(&g_fbex_uece_enhance_work, hisi_fbex_uece_enhance_fn);
 
	return 0;
}
 
void fbex_cleanup_uece_enhance_dev(void)
{
	if (g_fbex_dev_uece_enhance.list.prev != NULL)
		misc_deregister(&g_fbex_dev_uece_enhance);
}