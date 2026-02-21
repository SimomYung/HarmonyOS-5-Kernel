/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: dev_secinfo source
 * Create: 2023/3/31
 */
#include <platform_include/see/dev_secinfo.h>
#include <dev_secinfo_by_tee.h>
#include <dev_secinfo_errno.h>
#include <dev_secinfo_plat.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/version.h>
#include <linux/arm-smccc.h>       /* arm_smccc_smc */
#include <securec.h>               /* memset_s */

u32 dev_secinfo_set_uapp_enable_state(u32 state)
{
	return set_uapp_enable_state_by_tee(state);
}

u32 dev_secinfo_update_all_nvcnt(void)
{
	return dev_secinfo_update_all_nvcnt_by_tee();
}

u32 dev_secinfo_huk2_enhance_update_cnt(void)
{
	return huk2_enhance_update_cnt_by_tee();
}

#define EMPOWER_CERT_PATH           "/empower_cert"

static u32 _get_file_pos(struct dev_file_pos *pos, char *node_path)
{
	int ret;
	errno_t err;
	const char *ptn = NULL;
	struct device_node *np = NULL;

	dev_secinfo_info("enter %s\n", __func__);

	np = of_find_node_by_path(node_path);
	if (!np) {
		dev_secinfo_err("error, cannot find node %s\n", node_path);
		return DEV_SECINFO_ERR_FIND_DTS_NODE;
	}

	ret = of_property_read_string(np, "ptn", &ptn);
	if (ret != 0) {
		dev_secinfo_err("error 0x%x, cannot read ptn\n", ret);
		return DEV_SECINFO_ERR_READ_DTS_NODE;
	}

	err = strcpy_s(pos->ptn, sizeof(pos->ptn), ptn);
	if (err != EOK) {
		dev_secinfo_err("error 0x%x, copy ptn\n", err);
		return DEV_SECINFO_ERR_CP_PTN;
	}

	ret = of_property_read_u64(np, "offset", &pos->offset);
	if (ret != 0) {
		dev_secinfo_err("error 0x%x, cannot read offset\n", ret);
		return DEV_SECINFO_ERR_READ_DTS_NODE;
	}

	ret = of_property_read_u64(np, "size", &pos->size);
	if (ret != 0) {
		dev_secinfo_err("error 0x%x, cannot read size\n", ret);
		return DEV_SECINFO_ERR_READ_DTS_NODE;
	}

	return DEV_SECINFO_OK;
}

u32 dev_secinfo_empower_cert_pos(struct dev_file_pos *empower_cert)
{
	u32 ret;

	if (!empower_cert) {
		dev_secinfo_err("error, pos is NULL\n");
		return DEV_SECINFO_ERR_NULL_POINTER;
	}

	ret = _get_file_pos(empower_cert, EMPOWER_CERT_PATH);
	if (ret != DEV_SECINFO_OK) {
		dev_secinfo_err("error 0x%x, get empower cert pos\n", ret);
		return ret;
	}

	return DEV_SECINFO_OK;
}
