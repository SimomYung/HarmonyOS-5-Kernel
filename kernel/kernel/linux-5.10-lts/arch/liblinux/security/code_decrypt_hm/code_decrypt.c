// SPDX-License-Identifier: GPL-2.0
#include "code_decrypt.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <securec.h>
#include <teek_client_api.h>

#define MAX_DEV 1
#define COMMAND_BUNDLE_ID 0xb
#define COMMAND_APP_ID 0x10

#define TEE_CODE_PROTECT								\
{														\
	0xa0f02c44, 0xf7a5, 0x4691,							\
	{													\
		0x94, 0xc4, 0x98, 0x56, 0x5a, 0x44, 0xd9, 0x34	\
	}													\
}

#define CODE_DECRYPT_PATH "/vendor/bin/a0f02c44-f7a5-4691-94c4-98565a44d934.sec"

static int dev_major;
static struct class *code_decrypt_class;
static struct cdev code_decrypt_cdev;

static int init_code_protect_ta_context(TEEC_Context *context, TEEC_Session *session)
{
	int uid = 0;
	char package_name[] = "code_decrypt";
	TEEC_UUID ta_uuid = TEE_CODE_PROTECT;
	TEEC_Operation open_operation = {0};

	TEEC_Result ret = TEEK_InitializeContext(NULL, context);
	if (ret != TEEC_SUCCESS) {
		pr_err("[cdecrypt] TEEK_InitializeContext fail, ret=%#x\n", ret);
		return -EINVAL;
	}

	context->ta_path = (uint8_t *)CODE_DECRYPT_PATH;

	open_operation.started = SESSION_START_DEFAULT;
	open_operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_NONE,
		TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT);
	open_operation.params[TEE_PARAM_2].tmpref.buffer = (void *)(&uid);
	open_operation.params[TEE_PARAM_2].tmpref.size = sizeof(uid);
	open_operation.params[TEE_PARAM_3].tmpref.buffer = (void *)package_name;
	open_operation.params[TEE_PARAM_3].tmpref.size = sizeof(package_name) + 1;

	ret = TEEK_OpenSession(context,
		session,
		&ta_uuid,
		TEEC_LOGIN_IDENTIFY,
		NULL,
		&open_operation,
		NULL);
	if (ret != TEEC_SUCCESS) {
		pr_err("[cdecrypt] TEEK_OpenSession fail, ret=%#x\n", ret);
		TEEK_FinalizeContext(context);
		return -EINVAL;
	}

	return 0;
}

static int get_key_from_tee(uint8_t *app_key, size_t size, uint32_t bundleid)
{
	TEEC_Context context = {0};
	TEEC_Session session = {0};
	TEEC_Operation invoke_operation = {0};

	pr_info("[cdecrypt] get_key_from_tee, bundleid=%u\n", bundleid);

	TEEC_Result ret = init_code_protect_ta_context(&context, &session);
	if (ret != 0) {
		pr_err("[cdecrypt] init code_protect context fail %d\n", ret);
		return ret;
	}

	invoke_operation.started = SESSION_START_DEFAULT;
	invoke_operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_VALUE_INPUT,
		TEEC_VALUE_OUTPUT,
		TEEC_MEMREF_TEMP_OUTPUT,
		TEEC_NONE);
	invoke_operation.params[TEE_PARAM_0].value.a = bundleid;
	invoke_operation.params[TEE_PARAM_2].tmpref.buffer = app_key;
	invoke_operation.params[TEE_PARAM_2].tmpref.size = size;
	ret = TEEK_InvokeCommand(&session, COMMAND_BUNDLE_ID, &invoke_operation, NULL);
	if (ret != TEEC_SUCCESS) {
		pr_err("[cdecrypt] TEEK_InvokeCommand fail, cmd=%#x bundleid=%u, ret=%#x\n", COMMAND_BUNDLE_ID, bundleid, ret);
		TEEK_CloseSession(&session);
		TEEK_FinalizeContext(&context);
		return -EINTR;
	}

	TEEK_CloseSession(&session);
	TEEK_FinalizeContext(&context);
	return 0;
}

static int get_key_from_tee_v2(uint8_t *app_key, size_t size,
	char *app_id, uint32_t version_code, uint32_t *ret_code)
{
	TEEC_Context context = {0};
	TEEC_Session session = {0};
	TEEC_Operation invoke_operation = {0};

	pr_info("[cdecrypt] get_key_from_tee_v2 appid=%s, version=%u\n", app_id, version_code);

	TEEC_Result ret = init_code_protect_ta_context(&context, &session);
	if (ret != 0) {
		pr_err("[cdecrypt] init code_protect context fail %d\n", ret);
		return ret;
	}

	invoke_operation.started = SESSION_START_DEFAULT;
	invoke_operation.paramTypes = TEEC_PARAM_TYPES(
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_INPUT,
		TEEC_MEMREF_TEMP_OUTPUT,
		TEEC_VALUE_OUTPUT);
	invoke_operation.params[TEE_PARAM_0].tmpref.buffer = app_id;
	invoke_operation.params[TEE_PARAM_0].tmpref.size = strlen(app_id);
	invoke_operation.params[TEE_PARAM_1].value.a = version_code;
	invoke_operation.params[TEE_PARAM_2].tmpref.buffer = app_key;
	invoke_operation.params[TEE_PARAM_2].tmpref.size = size;
	ret = TEEK_InvokeCommand(&session, COMMAND_APP_ID, &invoke_operation, NULL);
	if (ret != TEEC_SUCCESS) {
		pr_err("[cdecrypt] TEEK_InvokeCommand fail, cmd=%#x app_id=%s, version=%u, ret=%#x\n",
			COMMAND_APP_ID, app_id, version_code, ret);
		TEEK_CloseSession(&session);
		TEEK_FinalizeContext(&context);
		return -EINTR;
	}

	*ret_code = invoke_operation.params[TEE_PARAM_3].value.a;
	TEEK_CloseSession(&session);
	TEEK_FinalizeContext(&context);
	return 0;
}

void code_decrypt_tee_single_work(struct work_struct *work)
{
	struct code_decrypt_tee_task *task = container_of(work, struct code_decrypt_tee_task, work);
	struct tee_code_decrypt_key *tee_key = task->tee_key;

	switch (tee_key->version) {
	case CODE_DECRYPT_KEY_VERSION_1:
		task->ret = get_key_from_tee(tee_key->key, CIPHER_KEY_LEN, tee_key->bundleid);
		break;
	case CODE_DECRYPT_KEY_VERSION_2:
		task->ret = get_key_from_tee_v2(tee_key->key, CIPHER_KEY_LEN,
			tee_key->app_id, tee_key->version_code, &tee_key->ret_code);
		break;
	default:
		task->ret = -EINVAL;
	}
}

static int single_queue_work_to_get_key(struct tee_code_decrypt_key *key)
{
	int ret;
	struct code_decrypt_tee_task *task = NULL;
	struct workqueue_struct *code_decrypt_wq = create_singlethread_workqueue("code_decrypt");
	if (!code_decrypt_wq) {
		pr_err("[cdecrypt] create code_decrypt failed\n");
		return -ENOMEM;
	}

	task = kmalloc(sizeof(struct code_decrypt_tee_task), GFP_KERNEL);
	if (task == NULL) {
		pr_err("[cdecrypt] kmalloc tee task failed\n");
		destroy_workqueue(code_decrypt_wq);
		return -ENOMEM;
	}

	task->tee_key = key;
	task->ret = -EBUSY;
	INIT_WORK(&task->work, code_decrypt_tee_single_work);
	queue_work(code_decrypt_wq, &task->work);
	flush_work(&task->work);
	ret = task->ret;
	kfree(task);
	destroy_workqueue(code_decrypt_wq);

	return ret;
}

static int code_decrypt_get_key(void *argp)
{
	int ret;
	struct tee_code_decrypt_key tee_key = { 0 };

	if (!argp) {
		pr_err("[cdecrypt] invalid input NULL.\n");
		return -EINVAL;
	}

	ret = copy_from_user(&tee_key, argp, sizeof(tee_key));
	if (ret) {
		pr_err("[cdecrypt] copy from user fail\n");
		return -EINVAL;
	}

	ret = single_queue_work_to_get_key(&tee_key);
	if (ret == 0) {
		ret = copy_to_user(argp, &tee_key, sizeof(tee_key));
		if (ret)
			pr_err("[cdecrypt] copy to user fail\n");
	}
	(void)memset_s((void *)&tee_key, sizeof(struct tee_code_decrypt_key), 0, sizeof(struct tee_code_decrypt_key));

	return ret;
}

static long code_decrypt_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	(void)file;
	int ret;

	switch (cmd) {
	case CODE_DECRYPT_GET_TEE_KEY:
		ret = code_decrypt_get_key(arg);
		break;
	default:
		pr_err("[cdecrypt] abe_code_decrypt invalid cmd!\n");
		ret = -EINVAL;
		break;
	}
	return ret;
}


static const struct file_operations code_decrypt_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = code_decrypt_ioctl,
};

static int code_decrypt_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	add_uevent_var(env, "DEVMODE=%#o", DECRYPT_DEV_MODE);
	return 0;
}

static int code_decrypt_create_chrdev(void)
{
	int err;
	dev_t dev;

	err = alloc_chrdev_region(&dev, 0, MAX_DEV, "abe_code_decrypt");
	if (err < 0)
		return err;

	dev_major = MAJOR(dev);

	code_decrypt_class = class_create(THIS_MODULE, "abe_code_decrypt");
	code_decrypt_class->dev_uevent = code_decrypt_uevent;

	cdev_init(&code_decrypt_cdev, &code_decrypt_fops);
	code_decrypt_cdev.owner = THIS_MODULE;

	cdev_add(&code_decrypt_cdev, MKDEV(dev_major, 0), 1);

	device_create(code_decrypt_class, NULL, MKDEV(dev_major, 0), NULL, "abe_code_decrypt");
	return 0;
}

static void destroy_chrdev(void)
{
	device_destroy(code_decrypt_class, MKDEV(dev_major, 0));
	class_destroy(code_decrypt_class);

	cdev_del(&code_decrypt_cdev);
	unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK);
}

static int __init code_decrypt_init(void)
{
	int err;

	err = code_decrypt_create_chrdev();
	if (err < 0) {
		pr_err("[cdecrypt] code decrypt device node init failed!\n");
		return err;
	}
	return 0;
}

static void __exit code_decrypt_exit(void)
{
	destroy_chrdev();
}

MODULE_LICENSE("GPL");

module_init(code_decrypt_init);
module_exit(code_decrypt_exit);
