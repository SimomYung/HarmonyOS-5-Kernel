/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Base driver for Seplat, including initialization.
 * Create: 2023/09/21
 */
#include <linux/module.h>
#include <linux/of.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <teek_client_api.h>
#include <teek_client_id.h>
#include <teek_client_constants.h>
#include <securec.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include "seplat_mntn.h"
#include "seplat.h"
#include <seplat_common.h>
#include <seplat_factory.h>
#include "seplat_tpm.h"

#define SEPLAT_WAIT_TIMEOUT_MS 10
#define MAX_ATTRIBUTE_BUFFER_SIZE       128

static struct task_struct *g_notifier_thread;
static DECLARE_WAIT_QUEUE_HEAD(g_seplat_shutdown_wait);
static bool g_enter_off_mode_finish = false;
static ssize_t seplat_chip_type_show(struct device *dev, struct device_attribute *attr, char *buf);

static DEVICE_ATTR(seplat_chip_type, (S_IRUSR | S_IRGRP | S_IROTH), seplat_chip_type_show, NULL);

static int seplat_teec_send_cmd(TEEC_Session *session, u32 cmd)
{
	TEEC_Result result;
	TEEC_Operation op = {0};
	u32 origin = 0;

	pr_debug("%s\n", __func__);

	if (!session) {
		pr_err("%s: param error!\n", __func__);
		return -EINVAL;
	}

	op.started = 1;
	op.cancel_flag = 0;
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
					TEEC_NONE, TEEC_NONE);

	result = TEEK_InvokeCommand(session, cmd, &op, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("%s, send cmd fail, res=0x%x, origin=0x%x\n", __func__, result, origin);
		return -EFAULT;
	}

	pr_debug("%s succ\n", __func__);

	return 0;
}

static int seplat_teec_open(TEEC_Context *context, TEEC_Session *session)
{
	int ret;
	u32 root_id = ROOTID;
	u32 origin = 0;
	const char *package_name = TEE_SERVICE_NAME;
	TEEC_UUID svc_id = UUID_TEE_SERVICE_SEPLAT;
	TEEC_Operation op;
	TEEC_Result result;

	pr_debug("%s\n", __func__);

	ret = memset_s(&op, sizeof(TEEC_Operation), 0, sizeof(TEEC_Operation));
	if (ret) {
		pr_err("memset_s failed\n");
		return ret;
	}

	result = TEEK_InitializeContext(NULL, context);
	if (result != TEEC_SUCCESS) {
		pr_err("InitializeContext failed, result=0x%x\n", result);
		goto cleanup_1;
	}

	op.started = 1;
	op.cancel_flag = 0;
	op.params[2].tmpref.buffer = (void *)&root_id;
	op.params[2].tmpref.size = sizeof(root_id);
	op.params[3].tmpref.buffer = (void *)package_name;
	op.params[3].tmpref.size = (size_t)(strlen(package_name) + 1);
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
					TEEC_NONE,
					TEEC_MEMREF_TEMP_INPUT,
					TEEC_MEMREF_TEMP_INPUT);

	result = TEEK_OpenSession(context, session, &svc_id,
				TEEC_LOGIN_IDENTIFY, NULL, &op, &origin);
	if (result != TEEC_SUCCESS) {
		pr_err("%s, OpenSession fail, rt=0x%x, origin=0x%x\n", __func__, result, origin);
		goto cleanup_2;
	}

	pr_debug("%s succ\n", __func__);

	return 0;
cleanup_2:
	TEEK_FinalizeContext(context);
cleanup_1:
	return -EINVAL;
}

static void seplat_teec_close(TEEC_Context *context, TEEC_Session *session)
{
	TEEK_CloseSession(session);
	TEEK_FinalizeContext(context);
}

static int seplat_enter_off_mode(void)
{
	int ret;
	TEEC_Context ctx;
	TEEC_Session session;

	pr_debug("%s\n", __func__);

	ret = seplat_teec_open(&ctx, &session);
	if (ret) {
		pr_err("%s, open session failed!\n", __func__);
		return ret;
	}

	ret = seplat_teec_send_cmd(&session, SEPLAT_TA_CMD_ID_SHUTDOWN_POWER_OFF);
	if (ret)
		pr_err("%s, send cmd failed!\n", __func__);

	seplat_teec_close(&ctx, &session);

	pr_debug("%s, exit\n", __func__);

	return ret;
}

static int seplat_shutdown_notifier_call(struct notifier_block *nb,
				unsigned long action, void *data)
{
	int ret;

	unsigned long timeout = msecs_to_jiffies(SEPLAT_WAIT_TIMEOUT_MS);
	unused(nb);
	unused(action);
	unused(data);

	pr_debug("%s\n", __func__);

	ret = wake_up_process(g_notifier_thread);
	if (ret == 0) {
		pr_err("%s, %d, seplat shutdown thread wake up failed!\n", __func__, __LINE__);
		return NOTIFY_OK;
	}
	while (!g_enter_off_mode_finish)
		(void)wait_event_timeout(g_seplat_shutdown_wait, g_enter_off_mode_finish, (long)timeout);

	pr_debug("%s succ\n", __func__);

	return NOTIFY_OK;
}

static struct notifier_block seplat_notifier_block = {
	.notifier_call = seplat_shutdown_notifier_call,
	.priority = 0,
};

bool g_seplat_chiptype_exist = false;

static void seplat_chiptype_exist_set(bool exist)
{
	g_seplat_chiptype_exist = exist;
}

bool seplat_chiptype_exist_get(void)
{
	return g_seplat_chiptype_exist;
}

/* FUNCTION: seplat_chip_type_show
 * DESCRIPTION: show seplat_chip_type, which config in device tree system.
 * Parameters
 *  struct device *dev:device structure
 *  struct device_attribute *attr:device attribute
 *  const char *buf:user buffer
 * RETURN VALUE
 *  ssize_t:  result
 */
static ssize_t seplat_chip_type_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    struct device_node *np = dev->of_node;
    const char *out_value = NULL;
    unused(attr);
    if (of_property_read_string(np, "chip_type", &out_value) != 0) {
        pr_err("seplat_chip_type_show failed.\n");
        return SEPLAT_ERR_BASIC_READ;
    }
    pr_info("seplat_chip_type = %s\n", out_value);
    return (ssize_t)(snprintf_s(buf, MAX_ATTRIBUTE_BUFFER_SIZE, MAX_ATTRIBUTE_BUFFER_SIZE - 1, "%s", out_value));
}

static int32_t seplat_chip_type_create_files(struct device *pdevice)
{
    int32_t ret = SEPLAT_KERNEL_OK;

	if (device_create_file(pdevice, &dev_attr_seplat_chip_type) != 0) {
		ret = SEPLAT_ERR_FT_FILE_CREATE;
		pr_err("%s: failed!\n", __func__);
	}

    return ret;
}

static void seplat_chip_type_remove_files(struct device *pdevice)
{
    device_remove_file(pdevice, &dev_attr_seplat_chip_type);
    pr_err("%s success!\n", __func__);
}

static int seplat_get_chiptype(const struct device_node *of_node,
				const char **chip_type)
{
	int ret;

	ret = of_property_read_string(of_node, "chip_type", chip_type);
	if (ret) {
		pr_err("%s: get chip type failed!\n", __func__);
		return ret;
	}

	pr_debug("%s succ\n", __func__);

	return 0;
}

static int seplat_shutdown_notifier_thread(void *arg)
{
	int ret;
	unused(arg);

	pr_debug("%s succ\n", __func__);

	ret = seplat_enter_off_mode();
	if (ret) {
		pr_err("seplat_enter_off_mode failed, ret=%d\n", ret);
		goto exit;
	}

	pr_debug("%s succ\n", __func__);
exit:
	g_enter_off_mode_finish = true;
	wake_up(&g_seplat_shutdown_wait);

	do_exit(ret);
}

#define NOEXIST                   (-1)

int seplat_get_dieid(char *dieid, u32 len)
{
	/* DieID Print Format: name:0x...xxx...\r\n\0 */
	if (!dieid || len < SEPLAT_FT_DIEID_SIZE_MAX + strlen(SEPLAT_DESENSITIVE_NAME) + SEPLAT_GET_DIEID_PADD_LEN) {
		pr_err("%s, para null or len[%d] invalid\n", __func__, len);
		return SEPLAT_ERR_BASIC_PARAMS;
	}

	if (!seplat_chiptype_exist_get()) {
		pr_info("seplat not exist, skip\n");
		return NOEXIST;
	}
#ifdef CONFIG_SEPLAT_FACTORY
	return seplat_ft_get_dieid(dieid, len);
#else
	pr_info("unsupport get dieid\n");
	return SEPLAT_ERR_BASIC_UNSUPPORT;
#endif
}

static int __init seplat_probe(struct platform_device *pdev)
{
	int ret;
	const char *chip_type = NULL;
	struct device *pdevice = &(pdev->dev);

	pr_debug("%s\n", __func__);
#ifdef CONFIG_SEPLAT_TPM
	struct seplat_private *pvt_data = NULL;

	pvt_data = devm_kzalloc(pdevice, sizeof(struct seplat_private),
				GFP_KERNEL);
	if (!pvt_data) {
		pr_err("seplat alloc pvt_data err\n");
		return -ENOMEM;
	}
	dev_set_drvdata(pdevice, pvt_data);
#endif
#ifndef CONFIG_SEPLAT_MNTN_ABSENT
	ret = seplat_mntn_init();
	if (ret)
		pr_err("seplat_mntn_init failed, ret=%d\n", ret);
#endif
	ret = seplat_get_chiptype(pdev->dev.of_node, &chip_type);
	if (ret) {
		pr_err("seplat_get_chiptype failed, ret=%d\n", ret);
		return ret;
	}

	if (strncmp(chip_type, SEPLAT_CHIP_TYPE, strlen(SEPLAT_CHIP_TYPE)) != 0) {
		seplat_chiptype_exist_set(false);
		pr_err("chip not valid, skip\n");
		return 0;
	}
	seplat_chiptype_exist_set(true);

#ifdef CONFIG_SEPLAT_TPM
	ret = seplat_tpm_init(pdev);
	if (ret)
		pr_err("seplat_tpm_init failed, ret=%d\n", ret);
#endif
	ret = seplat_chip_type_create_files(pdevice);
	if (ret != 0) {
		pr_err("%s:create seplat_chip_type files failed!\n", __func__);
		return ret;
	}

#ifdef CONFIG_SEPLAT_FACTORY
	/* Create seplat Factory-Test files. */
	ret = seplat_ft_create_files(pdevice);
	if (ret != 0) {
		pr_err("%s:create files failed!\n", __func__);
		return ret;
	}
#endif

	g_notifier_thread = kthread_create(seplat_shutdown_notifier_thread,
									   NULL, "seplat_shutdown_notifier_thread");
	if (IS_ERR((const void *)g_notifier_thread)) {
		pr_err("kthread_create failed, ret=%ld\n", PTR_ERR((const void *)g_notifier_thread));
		return PTR_ERR((const void *)g_notifier_thread);
	}

	ret = register_reboot_notifier(&seplat_notifier_block);
	if (ret) {
		pr_err("%s, notifier register err, ret=%d\n", __func__, ret);
		return ret;
	}

	pr_debug("%s succ\n", __func__);

	return 0;
}

static int seplat_remove(struct platform_device *pdev)
{
	int ret = 0;
	unused(pdev);

	pr_debug("%s\n", __func__);

	/* no seplat, ignore file remove */
	if (seplat_chiptype_exist_get()) {
		pr_debug("%s: seplat_chip_type file remove\n", __func__);
		seplat_chip_type_remove_files(&(pdev->dev));

#ifdef CONFIG_SEPLAT_FACTORY
		pr_debug("%s: seplat_ft file remove\n", __func__);
		seplat_ft_remove_files(&(pdev->dev));
#endif
	}

	ret = unregister_reboot_notifier(&seplat_notifier_block);
	if (ret)
		pr_err("%s, notifier unregister err, ret=%d\n", __func__, ret);

	pr_debug("%s succ\n", __func__);

	return ret;
}

static const struct of_device_id seplat_of_match[] = {
	{.compatible = "hisilicon,seplat" },
	{ }
};
MODULE_DEVICE_TABLE(of, seplat_of_match);

static struct platform_driver seplat_driver = {
	.probe =  seplat_probe,
	.remove = seplat_remove,
	.driver = {
		.name =  SEPLAT_DEVICE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(seplat_of_match),
	},
};

static int __init seplat_module_init(void)
{
	int ret = 0;

	pr_debug("%s\n", __func__);

	ret = platform_driver_register(&seplat_driver);
	if (ret)
		pr_err("register seplat driver failed, ret=%d\n", ret);

	pr_debug("%s succ\n", __func__);

	return ret;
}

static void __exit seplat_module_exit(void)
{
	platform_driver_unregister(&seplat_driver);
}

module_init(seplat_module_init);
module_exit(seplat_module_exit);

MODULE_DESCRIPTION("SEPLAT KERNEL DRIVER");
MODULE_LICENSE("GPL V2");
