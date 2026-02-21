/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ioctrl cmd for fs crypto source file
 * Author: Huawei OS Kernel Lab
 * Create : Thu Oct 20 11:29:30 2023
 */

#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <securec.h>
#include <platform_include/see/fbe_fscrypto_api.h>
#include "fbex_fscrypto.h"
#include "fbex_driver.h"

static uint32_t fbex_cmd_direct_lock_screen(void __user *argp)
{
	struct fbex_lock_screen_param lock_param = {0};

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	uint32_t ret = (uint32_t)copy_from_user(&lock_param, argp, sizeof(lock_param));
	if (ret) {
		pr_err("fbex lock screen, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}
	return fbex_ca_lock_screen(lock_param.user, lock_param.file);
}

static uint32_t fbex_cmd_direct_unlock_screen(void __user *argp)
{
	struct fbex_unlock_screen_param unlock_param = {0};

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	uint32_t ret = (uint32_t)copy_from_user(&unlock_param, argp, sizeof(unlock_param));
	if (ret) {
		pr_err("fbex unlock screen, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}

	if (unlock_param.iv_len != IV_LEN) {
		pr_err("fbex unlock screen, get param invalid\n");
		return FBE3_ERROR_BUFFER_INVALID;
	}

	ret = fbex_ca_unlock_screen(unlock_param.user, unlock_param.file,
		unlock_param.iv_data, unlock_param.iv_len);
	if (ret) {
		pr_err("fbex unlock screen failed\n");
	} else {
		ret = (uint32_t)copy_to_user(argp, &unlock_param, sizeof(unlock_param));
		if (ret) {
			pr_err("fbex unlock screen, copy to user failed\n");
			ret = FBE3_ERROR_COPY_FAIL;
		}
	}

	return ret;
}

static uint32_t fbex_cmd_slot_clean(void __user *argp)
{
	uint32_t slot;

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	uint32_t ret = (uint32_t)copy_from_user(&slot, argp, sizeof(uint32_t));
	if (ret) {
		pr_err("fbex slot clean stat, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}
	return (fbex_slot_clean(slot) == true) ? 1 : 0;
}

static uint32_t fbex_cmd_set_status(void __user *argp)
{
	struct fbex_set_status_param slot_set = {0};

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	uint32_t ret = (uint32_t)copy_from_user(&slot_set, argp, sizeof(slot_set));
	if (ret) {
		pr_err("fbex slot clean stat, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}

	fbex_set_status(slot_set.user, slot_set.lock);
	return 0;
}

static uint32_t fbex_cmd_restore_key(void __user *argp)
{
	(void)argp;
	return fbex_restore_key();
}

static uint32_t fbex_cmd_get_metadata_sece(void __user *argp)
{
	uint32_t ret = 0;
	struct fbex_sece_param sece_param = {0};
	struct fbex_fscrypto_param fscrypto_param = {0};

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	ret = (uint32_t)copy_from_user(&fscrypto_param, argp, sizeof(struct fbex_fscrypto_param));
	if (ret) {
		pr_err("fbex get metadata sece, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}

	if (fscrypto_param.pubkey_len != PUBKEY_LEN ||
		fscrypto_param.metadata_len != METADATA_LEN) {
		pr_err("fbex get metadata sece, user param invalid\n");
		return FBE3_ERROR_BUFFER_INVALID;
	}

	sece_param.cmd = fscrypto_param.cmd;
	sece_param.idx = fscrypto_param.idx;
	sece_param.pubkey = fscrypto_param.pubkey;
	sece_param.key_len = fscrypto_param.pubkey_len;
	sece_param.metadata = fscrypto_param.metadata;
	sece_param.iv_len = fscrypto_param.metadata_len;
	ret = get_metadata_sece(&sece_param);
	if (ret) {
		pr_err("fbex get metadata sece failed, ret 0x%x\n", ret);
	} else {
		ret = (uint32_t)copy_to_user(argp, &fscrypto_param, sizeof(struct fbex_fscrypto_param));
		if (ret) {
			pr_err("fbex get metadata sece, copy to user failed\n");
			ret = FBE3_ERROR_COPY_FAIL;
		}
	}
	(void)memset_s(&fscrypto_param, sizeof(struct fbex_fscrypto_param),
		0, sizeof(struct fbex_fscrypto_param));
	return ret;
}

static uint32_t fbex_cmd_get_metadata(void __user *argp)
{
	uint32_t ret = 0;
	u8 metadata[METADATA_LEN] = {0};

	ret = get_metadata(metadata, sizeof(metadata));
	if (ret) {
		pr_err("fbex get metadata failed, ret 0x%x\n", ret);
	} else {
		ret = (uint32_t)copy_to_user(argp, metadata, METADATA_LEN);
		if (ret) {
			pr_err("fbex get metadata, copy to user failed\n");
			ret = FBE3_ERROR_COPY_FAIL;
		}
	}
	(void)memset_s(metadata, METADATA_LEN, 0, METADATA_LEN);
	pr_err("fbex_cmd_get_metadata, finish\n");
	return ret;
}

static uint32_t fbex_cmd_enable_kdf(void __user *argp)
{
	(void)argp;
	return fbex_enable_kdf();
}

#ifdef CONFIG_FBE3_1
static uint32_t fbex_cmd_gen_sk(void __user *argp)
{
	int32_t ret;
	uint32_t type = 0;

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	ret = (int32_t)copy_from_user(&type, argp, sizeof(uint32_t));
	if (ret) {
		pr_err("fbex gen sk, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}

	return fbex_gen_sk(type);
}

static uint32_t fbex_cmd_mspc_check(void __user *argp)
{
	uint32_t ret;
	uint32_t flag = 0;

	if (!argp) {
		pr_err("%s, invalid input NULL.\n", __func__);
		file_encry_record_error(0, 0, 0, FBE3_ERROR_INIT_BUFFER);
		return FBE3_ERROR_INIT_BUFFER;
	}
	ret = fbe_ca_msp_available(&flag);
	if (ret) {
		pr_err("fbex get msp status failed\n");
		return ret;
	}

	ret = (uint32_t)copy_to_user(argp, &flag, sizeof(uint32_t));
	if (ret) {
		pr_err("fbex get msp status, copy from user failed\n");
		return FBE3_ERROR_COPY_FAIL;
	}

	return 0;
}
#endif

static const struct fbex_function_table g_fbex_cmd_table[] = {
	{ HISI_FBEX_DIRECT_LOCK_SCREEN, fbex_cmd_direct_lock_screen },
	{ HISI_FBEX_DIRECT_UNLOCK_SCREEN, fbex_cmd_direct_unlock_screen },
	{ HISI_FBEX_SLOT_CLEAN, fbex_cmd_slot_clean },
	{ HISI_FBEX_SET_STATUS, fbex_cmd_set_status },
	{ HISI_FBEX_KEY_RESTORE, fbex_cmd_restore_key },
	{ HISI_FBEX_GET_METADATA_SECE, fbex_cmd_get_metadata_sece },
	{ HISI_FBEX_GET_METADATA, fbex_cmd_get_metadata },
	{ HISI_FBEX_ENABLE_KDF, fbex_cmd_enable_kdf },
#ifdef CONFIG_FBE3_1
	{ HISI_FBEX_GEN_SK, fbex_cmd_gen_sk },
	{ HISI_FBEX_MSPC_CHECK, fbex_cmd_mspc_check },
#endif
};

static const uint32_t g_fbex_cmd_num =
	sizeof(g_fbex_cmd_table) / sizeof(g_fbex_cmd_table[0]);

static long fbex_fscrypto_unlocked_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	uint32_t ret = FBE3_ERROR_CMD_UNSUPPORT;
	void __user *argp = (void __user *)(uintptr_t)arg;
	uint32_t i;

	if (argp == NULL) {
		pr_err("fbex cmd proc, user buff is NULL\n");
		return FBE3_ERROR_BUFFER_NULL;
	}

	for (i = 0; i < g_fbex_cmd_num; i++) {
		if (cmd == g_fbex_cmd_table[i].cmd_id && g_fbex_cmd_table[i].func != NULL) {
			ret = g_fbex_cmd_table[i].func(argp);
			break;
		}
	}

	(void)file;
	return (long)ret;
}

static const struct file_operations g_fbex_fscrypto_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = fbex_fscrypto_unlocked_ioctl,
};

static struct miscdevice g_fbex_dev_fscrypto = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fbex_fscrypto",
	.fops = &g_fbex_fscrypto_fops,
	.mode = 0600, /* permission */
};

int fbex_init_fscrypto_dev(void)
{
	int ret;

	ret = misc_register(&g_fbex_dev_fscrypto);
	if (ret != 0) {
		pr_err("failed to register fbex fs crypto, ret 0x%x\n", ret);
		return ret;
	}

	return 0;
}

void fbex_cleanup_fscrypto_dev(void)
{
	if (g_fbex_dev_fscrypto.list.prev != NULL)
		misc_deregister(&g_fbex_dev_fscrypto);
}
