/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: dev_secinfo do things in tee source
 * Create: 2025/01/20
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

u32 dev_secinfo_se_type(u32 *type)
{
	u32 ret;
	u32 val = 0;
	struct efuse_item item = { 0 };

	if (!type) {
		dev_secinfo_err ("[%s]: error, state NULL\n", __func__);
		return DEV_SECINFO_ERR_NULL_POINTER;
	}

	item.buf = &val;
	item.buf_size = (u32)(sizeof(val) / sizeof(u32));
	item.item_vid = EFUSE_KERNEL_SECDBG_CTRL;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		dev_secinfo_err("[%s]: error, read efuse ret=0x%08X", __func__, ret);
		return DEV_SECINFO_ERR_GET_SE;
	}

	*type = (val == 0 ? DEV_SECINFO_ENG_DEVICE : DEV_SECINFO_SHIP_DEVICE);
	return DEV_SECINFO_OK;
}
