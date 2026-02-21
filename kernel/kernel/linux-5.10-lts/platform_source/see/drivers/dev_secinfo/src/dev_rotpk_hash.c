/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 *  Description: socid
 *  Create : 2021/08/17
 */
#include <platform_include/see/dev_secinfo.h>
#include <dev_secinfo_errno.h>
#include <dev_secinfo_plat.h>
#include <platform_include/see/efuse_driver.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/version.h>
#include <linux/arm-smccc.h>       /* arm_smccc_smc */
#include <securec.h>               /* memset_s */

#define UAPP_CERT_PATH             "/uapp/cert"

static u32 is_uapp_x509_cert(void)
{
	int ret;
	const char *format = NULL;
	struct device_node *np = NULL;

	dev_secinfo_info("enter %s\n", __func__);

	np = of_find_node_by_path(UAPP_CERT_PATH);
	if (!np) {
		dev_secinfo_err("error, cannot find node %s\n", UAPP_CERT_PATH);
		return 0;
	}

	ret = of_property_read_string(np, "cert_format", &format);
	if (ret != 0) {
		dev_secinfo_err("error 0x%x, cannot read cert_format\n", ret);
		return 0;
	}

	if (strcmp(format, "x509") != 0) {
		dev_secinfo_err("uapp not x509 cert\n");
		return 0;
	}

	return 1;
}

#define SHA256_BYTES   0x20
static u32 _get_hbk_hash(struct dev_rotpk_hash *dev_rotpk_hash)
{
	u32 ret;
	errno_t err;
	uint32_t read_size = 0;
	struct efuse_item item = { 0 };
	uint32_t buf[SHA256_BYTES / sizeof(uint32_t)] = { 0 };

	if (!dev_rotpk_hash) {
		dev_secinfo_err("[%s]: error, dev_rotpk_hash NULL\n", __func__);
		return DEV_SECINFO_ERR_NULL_POINTER;
	}

	item.buf = buf;
	item.buf_size = (uint32_t)(ARRAY_SIZE(buf));
	item.item_vid = EFUSE_KERNEL_ROTPK_HASH0;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		dev_secinfo_err("[%s]: error 0x%x, read rotpk hash0", __func__, ret);
		return DEV_SECINFO_ERR_RD_ROTPK_HASH;
	}

	/* add the read size of first part */
	read_size += item.buf_size;

	item.buf = buf + read_size;
	item.buf_size = (uint32_t)(ARRAY_SIZE(buf)) - read_size;
	item.item_vid = EFUSE_KERNEL_ROTPK_HASH1;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		dev_secinfo_err("[%s]: error 0x%x, read rotpk hash1", __func__, ret);
		return DEV_SECINFO_ERR_RD_ROTPK_HASH;
	}

	/* add the read size of second part */
	read_size += item.buf_size;
	err = memcpy_s(&dev_rotpk_hash->val[0], sizeof(dev_rotpk_hash->val), \
			buf, read_size * sizeof(uint32_t));
	if (err != EOK) {
		dev_secinfo_err("[%s]: error 0x%x, cp rotph hash", __func__, err);
		return DEV_SECINFO_ERR_CP_ROTPK_HASH;
	}

	/* get the total size of read data */
	dev_rotpk_hash->bytes = read_size * sizeof(uint32_t);
	return DEV_SECINFO_OK;
}

static u32 _get_gjrotpk_hash(struct dev_rotpk_hash *dev_rotpk_hash)
{
	u32 ret;
	errno_t err;
	struct efuse_item item = { 0 };
	uint32_t buf[SHA256_BYTES / sizeof(uint32_t)] = { 0 };

	if (!dev_rotpk_hash) {
		dev_secinfo_err("[%s]: error, dev_rotpk_hash NULL\n", __func__);
		return DEV_SECINFO_ERR_NULL_POINTER;
	}

	item.buf = buf;
	item.buf_size = (uint32_t)(ARRAY_SIZE(buf));
	item.item_vid = EFUSE_KERNEL_GJROTPK_HASH;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		dev_secinfo_err("[%s]: error 0x%x, read rotph hash0", __func__, ret);
		return DEV_SECINFO_ERR_RD_ROTPK_HASH;
	}

	err = memcpy_s(&dev_rotpk_hash->val[0], sizeof(dev_rotpk_hash->val), \
			buf, item.buf_size * sizeof(uint32_t));
	if (err != EOK) {
		dev_secinfo_err("[%s]: error 0x%x, cp rotph hash", __func__, err);
		return DEV_SECINFO_ERR_CP_ROTPK_HASH;
	}

	/* get the total size of read data */
	dev_rotpk_hash->bytes = item.buf_size * sizeof(uint32_t);
	return DEV_SECINFO_OK;
}

u32 dev_secinfo_rotpk_hash(struct dev_rotpk_hash *rotpk_hash)
{
	if (is_uapp_x509_cert() != 0)
		return _get_gjrotpk_hash(rotpk_hash);

	return _get_hbk_hash(rotpk_hash);
}
