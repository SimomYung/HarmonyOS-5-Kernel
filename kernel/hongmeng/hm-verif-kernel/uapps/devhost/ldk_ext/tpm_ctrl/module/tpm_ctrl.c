/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: devhost tpm ctrl operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 10 15:30:00 2023
 */

#include <stddef.h>
#include <asm/uaccess.h>
#include <asm/ioctls.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/securec.h>
#include <linux/tpm.h>
#include <linux/types.h>

#include "tpm_ctrl.h"
#include "tpm_ctrl_if.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("tpm ctrl ops");
MODULE_VERSION("0.01");

static DEFINE_MUTEX(tpm_ctrl_lock);
static struct tpm_chip *tpm_chip = NULL;
static struct tpm_digest *digests = NULL;

static int tpm_ctrl_tpm_init(void)
{
	int i;
	if (tpm_chip != NULL) {
		return 0;
	}

	tpm_chip = tpm_default_chip();
	if (tpm_chip == NULL) {
		return -ENODEV;
	}

	digests = kzalloc(sizeof(struct tpm_digest) * tpm_chip->nr_allocated_banks,
			  GFP_KERNEL);
	if (digests == NULL) {
		tpm_chip = NULL;
		return -ENOMEM;
	}
	for (i = 0; i < tpm_chip->nr_allocated_banks; i++) {
		digests[i].alg_id = tpm_chip->allocated_banks[i].alg_id;
	}
	return 0;
}

static void tpm_ctrl_tpm_destroy(void)
{
	put_device(&tpm_chip->dev);
	tpm_chip = NULL;
	kfree(digests);
	digests = NULL;
}

static int tpm_ctrl_pcr_extend(int pcr, size_t size, uint8_t *digest)
{
	int i;
	int ret = 0;

	/* The data length used by the tpm extension pcr is specified by
	 * the macro definition TPM_DIGEST_SIZE, which is not the actual
	 * length of the input parameter digest. Ensure that the valid
	 * length of the input parameter digest is greater than or equal
	 * to TPM_DIGEST_SIZE.
	 */
	if ((digest == NULL) || (size < TPM_DIGEST_SIZE)) {
		return -EINVAL;
	}

	if (tpm_chip == NULL) {
		return -EINVAL;
	}

	mutex_lock(&tpm_ctrl_lock);
	if (tpm_chip->flags & TPM_CHIP_FLAG_TPM2) {
		for (i = 0; i < tpm_chip->nr_allocated_banks; i++) {
			ret = memcpy_s(digests[i].digest, TPM_MAX_DIGEST_SIZE,
				       digest, TPM_DIGEST_SIZE);
			if (ret != 0) {
				ret = -EFAULT;
				goto out_lock;
			}
		}
	} else {
		ret = memcpy_s(digests->digest, TPM_MAX_DIGEST_SIZE,
			       digest, TPM_DIGEST_SIZE);
		if (ret != 0) {
			ret = -EFAULT;
			goto out_lock;
		}
	}

	ret = tpm_pcr_extend(tpm_chip, pcr, digests);
	if (ret != 0) {
		pr_err("failed to communicate to tpm chip, err=%d\n", ret);
	}

out_lock:
	mutex_unlock(&tpm_ctrl_lock);
	return ret;
}

static int tpm_ctrl_pcr_read(int pcr, size_t size, uint8_t __user *digest)
{
	int ret = 0;
	struct tpm_digest tmp_digest= {0};

	if ((digest == NULL) || (size < TPM_DIGEST_SIZE)) {
		return -EINVAL;
	}

	if (tpm_chip == NULL) {
		return -EINVAL;
	}

	mutex_lock(&tpm_ctrl_lock);
	ret = tpm_pcr_read(tpm_chip, pcr, &tmp_digest);
	if (ret != 0) {
		pr_err("failed to communicate to tpm chip, err=%d\n", ret);
		goto out_lock;
	}

	ret = copy_to_user((void __user *)(uintptr_t)digest, tmp_digest.digest, TPM_DIGEST_SIZE);
	if (ret != 0) {
		pr_err("failed to copy to user, err=%d\n", ret);
		ret = -EFAULT;
		goto out_lock;
	}

out_lock:
	mutex_unlock(&tpm_ctrl_lock);
	return ret;
}

static struct tpm_ctrl_ops tpm_ctrl_ops __read_mostly = {
	.pcr_extend = tpm_ctrl_pcr_extend,
	.pcr_read = tpm_ctrl_pcr_read,
};

static int __init devhost_tpm_ctrl_init(void)
{
	int ret;

	ret = tpm_ctrl_tpm_init();
	if (ret < 0) {
		/* if no valid tpm driver, donot register tpm info to tpm ctrl */
		pr_err("failed to init tpm ctrl, err=%d\n", ret);
		return ret;
	}

	ret = devhost_register_tpm_ctrl(&tpm_ctrl_ops);
	if (ret < 0) {
		tpm_ctrl_tpm_destroy();
		pr_err("failed to init tpm_ctrl, err=%d\n", ret);
	}

	return ret;
}

module_init(devhost_tpm_ctrl_init);
