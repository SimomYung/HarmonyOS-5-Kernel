/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: uapp source file
 * Create: 2022/04/20
 */

#include <platform_include/see/uapp.h>
#include <platform_include/see/efuse_driver.h>
#include <platform_include/see/dev_secinfo.h>
#include <uapp_utils.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/version.h>
#include <linux/arm-smccc.h>       /* arm_smccc_smc */
#include <securec.h>               /* memset_s */

/* kernel to atf smc id */
#define FID_UAPP_GET_ENABLE_STATE              0xC3002F00u
#define FID_UAPP_SET_ENABLE_STATE              0xC3002F01u
#define FID_UAPP_VALID_BINDFILE_PUBKEY         0xC3002F02u
#define FID_UAPP_REVOKE_BINDFILE_PUBKEY        0xC3002F03u

#define UAPP_BINDFILE_PUBKEY_INDEX_MAX         32u

struct smc_arg_wrap {
	uintptr_t x0;
	uintptr_t x1;
	uintptr_t x2;
	uintptr_t x3;
	uintptr_t x4;
};

/*
 * adapt local smc function
 */
static u32 smc_switch_to_atf(struct smc_arg_wrap *arg)
{
	struct arm_smccc_res res;
	arm_smccc_smc(arg->x0, arg->x1, arg->x2, arg->x3,
			arg->x4, 0, 0, 0, &res);
	arg->x1 = res.a1;
	arg->x2 = res.a2;
	arg->x3 = res.a3;
	return (u32)res.a0;
}

/*
 * get uapp enable state
 *
 * @param
 * state              pointer to save state
 *                    [0-DISABLE] [1-ENABLE] [other-INVALID]
 * @return
 * UAPP_OK            operation succeed
 * other              error code
 */
u32 uapp_get_enable_state(u32 *state)
{
	u32 ret;
	u32 val = 0;
	struct efuse_item item = { 0 };

	if (!state) {
		uapp_err ("[%s]: error, state NULL\n", __func__);
		return UAPP_NULL_PTR_ERR;
	}

	item.buf = &val;
	item.buf_size = sizeof(val) / sizeof(u32);
	item.item_vid = EFUSE_KERNEL_UAPP_ENABLE;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		uapp_err("[%s]: error, read efuse ret=0x%08X", __func__, ret);
		return UAPP_READ_EFUSE_ERR;
	}

	*state = (val == 0 ? UAPP_DISABLE : UAPP_ENABLE);
	return UAPP_OK;
}

/*
 * set uapp enable state
 *
 * @param
 * state              the state to set
 *                    [0-DISABLE] [1-ENABLE] [other-INVALID]
 * @return
 * UAPP_OK            operation succeed
 * other              error code
 */
u32 uapp_set_enable_state(u32 state)
{
	u32 ret;

	if (state >= UAPP_ENABLE_STATE_MAX) {
		uapp_err("[%s]: error, invalid state=0x%08X", __func__, state);
		return UAPP_INVALID_PARAM_ERR;
	}

	ret = dev_secinfo_set_uapp_enable_state(state);
	if (ret != DEV_SECINFO_OK) {
		uapp_err("[%s]: error, dev set uapp_en ret=0x%08X", __func__, ret);
		return ret;
	}

	return UAPP_OK;
}

/*
 * valid bindfile authkey by key_idx
 *
 * @param
 * key_idx            the key_idx to valid
 *
 * @return
 * UAPP_OK            operation succeed
 * other              error code
 */
u32 uapp_valid_bindfile_pubkey(u32 key_idx)
{
	u32 ret;
	u32 val = 0;
	struct efuse_item item = { 0 };

	if (key_idx >= UAPP_BINDFILE_PUBKEY_INDEX_MAX) {
		uapp_err("[%s]: error, invalid key_idx=0x%08X", __func__, key_idx);
		return UAPP_INVALID_PARAM_ERR;
	}

	item.buf = &val;
	item.buf_size = sizeof(val) / sizeof(u32);
	item.item_vid = EFUSE_KERNEL_UAPP_KEYREVOKE;
	ret = efuse_read_item(&item);
	if (ret != EFUSE_OK) {
		uapp_err("[%s]: error, read efuse ret=0x%08X", __func__, ret);
		return UAPP_READ_EFUSE_ERR;
	}

	if ((val & (1u << key_idx)) != 0) {
		uapp_err("[%s]: error, uapp key_idx=0x%08X is revoked\n", __func__, key_idx);
		return UAPP_AUTHKEY_IS_REVOKED_ERR;
	}

	return UAPP_OK;
}

/*
 * uapp revoke certain bindfile authkey by key_idx
 *
 * @param
 * key_idx            the key_idx to revoke
 *
 * @return
 * UAPP_OK            operation succeed
 * other              error code
 */
u32 uapp_revoke_bindfile_pubkey(u32 key_idx)
{
	u32 ret;
	struct smc_arg_wrap smc_args = { 0 };

	if (key_idx >= UAPP_BINDFILE_PUBKEY_INDEX_MAX) {
		uapp_err("[%s]: error, invalid key_idx=0x%08X", __func__, key_idx);
		return UAPP_INVALID_PARAM_ERR;
	}

	smc_args.x0 = (uintptr_t)FID_UAPP_REVOKE_BINDFILE_PUBKEY;
	smc_args.x1 = (uintptr_t)key_idx;
	ret = smc_switch_to_atf(&smc_args);
	if (ret != UAPP_OK) {
		uapp_err("[%s]: error, smc failed, smc_id=0x%08X, ret=0x%08X", \
		      __func__, smc_args.x0, ret);
		return UAPP_SMC_PROC_ERR;
	}

	return UAPP_OK;
}
