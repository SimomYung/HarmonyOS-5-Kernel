/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpeg jpu utils
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "jpu.h"
#include "jpu_irq.h"
#include "jpu_def.h"
#include <platform_include/camera/jpeg/jpeg_base.h>

#include <linux/interrupt.h>

#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
/* balitmore new smmu version, IPP only need to config smmuv3 tbu */
static int32_t do_cfg_smmuv3_on_ippcomm(const struct jpu_data_type *jpu_device)
{
	int32_t ret;
	jpu_check_null_return(jpu_device->jpgd_drv, -EINVAL);
	jpu_check_null_return(jpu_device->jpgd_drv->enable_smmu, -EINVAL);
	jpu_check_null_return(jpu_device->jpgd_drv->setsid_smmu, -EINVAL);
	jpu_check_null_return(jpu_device->jpgd_drv->set_pref, -EINVAL);

	ret = jpu_device->jpgd_drv->enable_smmu(jpu_device->jpgd_drv);
	if (ret != 0) {
		jpu_err("enable_smmu failed:%d\n", ret);
		return ret;
	}

	return ret;
}
#endif

static int32_t jpu_smmu_on(struct jpu_data_type *jpu_device)
{
	uint32_t fama_ptw_msb;
	int32_t ret = 0;

	jpu_debug("+\n");
#if   defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	ret = do_cfg_smmuv3_on_ippcomm(jpu_device);
	if (ret != 0)
		jpu_err("do_cfg_smmuv3_on_ippcomm failed:%d\n", ret);

	jpu_debug("-\n");
	jpu_info("do_cfg_smmuv3_on_ippcomm\n");
#else
	/*
	 * Set global mode:
	 * SMMU_SCR_S.glb_nscfg = 0x3
	 * SMMU_SCR_P.glb_prot_cfg = 0x0
	 * SMMU_SCR.glb_bypass = 0x0
	 */
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_SCR, 0x0, 1, 0);

	/* for performance Ptw_mid: 0x1d, Ptw_pf: 0x1 */
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_SCR, 0x1, REG_SET_4_BIT,
		REG_SET_16_BIT);
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_SCR, 0x1D, REG_SET_6_BIT,
		REG_SET_20_BIT);

	/*
	 * Set interrupt mode:
	 * Clear all interrupt state: SMMU_INCLR_NS = 0xFF
	 * Enable interrupt: SMMU_INTMASK_NS = 0x0
	 */
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_INTRAW_NS, 0xFF,
		REG_SET_32_BIT, 0);
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_INTMASK_NS, 0x0,
		REG_SET_32_BIT, 0);

	/*
	 * Set non-secure pagetable addr:
	 * SMMU_CB_TTBR0 = non-secure pagetable address
	 * Set non-secure pagetable type:
	 * SMMU_CB_TTBCR.cb_ttbcr_des= 0x1 (long descriptor)
	 */
	if (jpu_domain_get_ttbr(jpu_device) == 0) {
		jpu_err("get ttbr failed\n");
		return -EINVAL;
	}
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_CB_TTBR0,
		(uint32_t)jpu_domain_get_ttbr(jpu_device), REG_SET_32_BIT, 0);
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_CB_TTBCR, 0x1, 1, 0);

	/* FAMA configuration */
	fama_ptw_msb = (jpu_domain_get_ttbr(jpu_device) >> SHIFT_32_BIT) &
		JPEGDEC_FAMA_PTW_MSB;
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_FAMA_CTRL0, JPEGDEC_FAMA_PTW_MID,
		REG_SET_14_BIT, 0);
	jpu_set_reg(jpu_device->jpu_smmu_base + SMMU_FAMA_CTRL1,
		fama_ptw_msb, REG_SET_7_BIT, 0);
	jpu_debug("-\n");
#endif
	return ret;
}

#if (!defined CONFIG_DPU_FB_V600) && (!defined CONFIG_DKMD_DPU_VERSION)
static int32_t jpu_clk_try_low_freq(const struct jpu_data_type *jpu_device)
{
	int32_t ret;

	ret = jpeg_dec_set_rate(jpu_device->jpg_func_clk, JPGDEC_LOWLEVEL_CLK_RATE);
	if (ret != 0) {
		jpu_err("jpg_func_clk set clk failed, error = %d\n", ret);
		return -EINVAL;
	}

	ret = jpeg_dec_clk_prepare_enable(jpu_device->jpg_func_clk);
	if (ret != 0) {
		jpu_err("jpg_func_clk clk_prepare failed, error = %d\n", ret);
		return -EINVAL;
	}

	return ret;
}

static int32_t jpu_clk_enable(struct jpu_data_type *jpu_device)
{
	int32_t ret;

	jpu_debug("+\n");
	jpu_check_null_return(jpu_device->jpg_func_clk, -EINVAL);

	jpu_debug("jpg func clk default rate is: %ld\n",
		JPGDEC_DEFALUTE_CLK_RATE);

	ret = jpeg_dec_set_rate(jpu_device->jpg_func_clk, JPGDEC_DEFALUTE_CLK_RATE);
	if (ret != 0) {
		jpu_err("jpg_func_clk set clk failed, error = %d\n", ret);
		goto TRY_LOW_FREQ;
	}

	ret = jpeg_dec_clk_prepare_enable(jpu_device->jpg_func_clk);
	if (ret != 0) {
		jpu_err("jpg_func_clk clk_prepare failed, error = %d\n", ret);
		goto TRY_LOW_FREQ;
	}

	jpu_debug("-\n");
	return ret;

TRY_LOW_FREQ:
	return jpu_clk_try_low_freq(jpu_device);
}

static int32_t jpu_clk_disable(const struct jpu_data_type *jpu_device)
{
	int32_t ret;

	jpu_debug("+\n");
	jpu_check_null_return(jpu_device->jpg_func_clk, -EINVAL);

	jpeg_dec_clk_disable_unprepare(jpu_device->jpg_func_clk);

	ret = jpeg_dec_set_rate(jpu_device->jpg_func_clk, JPGDEC_POWERDOWN_CLK_RATE);
	if (ret != 0) {
		jpu_err("fail to set power down rate, ret = %d\n", ret);
		return -EINVAL;
	}

	jpu_debug("-\n");
	return ret;
}

static int32_t jpu_media1_regulator_disable(
	const struct jpu_data_type *jpu_device)
{
	int32_t ret;

	jpu_debug("+\n");
	ret = regulator_disable(jpu_device->media1_regulator);
	if (ret != 0)
		jpu_err("jpu media1 regulator_disable failed, error = %d\n", ret);

	jpu_debug("-\n");
	return ret;
}

static int32_t jpu_media1_regulator_enable(struct jpu_data_type *jpu_device)
{
	int32_t ret = 0;

	jpu_debug("+\n");
	ret = regulator_enable(jpu_device->media1_regulator);
	if (ret != 0)
		jpu_err("jpu media1_regulator failed, error = %d\n", ret);

	jpu_debug("-\n");
	return ret;
}

static int32_t jpu_jpu_regulator_enable(struct jpu_data_type *jpu_device)
{
	int32_t ret = 0;

	jpu_debug("+\n");
	ret = regulator_enable(jpu_device->jpu_regulator);
	if (ret != 0)
		jpu_err("jpu regulator_enable failed, error = %d\n", ret);

	jpu_debug("-\n");
	return ret;
}
#endif

static int32_t jpu_on_v600(struct jpu_data_type *jpu_device)
{
	int32_t ret;
#if (!defined CONFIG_DPU_FB_V600) && (!defined CONFIG_DKMD_DPU_VERSION)
	/* step 1 mediasubsys enable */
	ret = jpu_media1_regulator_enable(jpu_device);
	if (ret != 0) {
		jpu_err("media1_regulator_enable fail\n");
		return ret;
	}

	/* step 2 clk_enable */
	ret = jpu_clk_enable(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_clk_enable fail\n");
		jpu_media1_regulator_disable(jpu_device);
		return ret;
	}

	/* step 3 regulator_enable ispsubsys */
	ret = jpu_jpu_regulator_enable(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_regulator_enable fail\n");
		jpu_clk_disable(jpu_device);
		jpu_media1_regulator_disable(jpu_device);
		return ret;
	}
#endif

#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	if ((jpu_device->jpgd_drv == NULL) || (jpu_device->jpgd_drv->power_up == NULL) ||
		(jpu_device->jpgd_drv->set_jpgclk_rate == NULL)) {
		jpu_err("jpu_device->jpgd_drv is NULL\n");
		return -EINVAL;
	}

	ret = jpu_device->jpgd_drv->power_up(jpu_device->jpgd_drv);
	if (ret != 0) {
		jpu_info("power_up failed:%d\n", ret);
		return ret;
	}
#endif

	return 0;
}

#if (!defined CONFIG_DPU_FB_V600) && (!defined CONFIG_DKMD_DPU_VERSION)
static int jpu_regulator_disable(
	const struct jpu_data_type *jpu_device)
{
	int ret;

	jpu_debug("+\n");
	ret = regulator_disable(jpu_device->jpu_regulator);
	if (ret != 0)
		jpu_err("jpu regulator_disable failed, error = %d\n", ret);

	jpu_debug("-\n");
	return ret;
}
#endif

static int32_t jpu_off_v600(struct jpu_data_type *jpu_device)
{
	int32_t ret = 0;
#if (!defined CONFIG_DPU_FB_V600) && (!defined CONFIG_DKMD_DPU_VERSION)
	/* ispsubsys regulator disable */
	ret = jpu_regulator_disable(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_regulator_disable failed\n");
		return -EINVAL;
	}

	/* clk disable */
	ret = jpu_clk_disable(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_clk_disable failed\n");
		return -EINVAL;
	}

	/* media disable */
	ret = jpu_media1_regulator_disable(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_media1_regulator_disable failed\n");
		return -EINVAL;
	}
#endif

#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	if ((jpu_device->jpgd_drv == NULL) || (jpu_device->jpgd_drv->set_jpgclk_rate == NULL) ||
		(jpu_device->jpgd_drv->power_dn == NULL)) {
		jpu_err("jpu_device->jpgd_drv is NULL\n");
		return -EINVAL;
	}

	ret = jpu_device->jpgd_drv->power_dn(jpu_device->jpgd_drv);
	if (ret != 0)
		jpu_err("Failed: fail to jpu powerdown%d\n", ret);
#endif
	return ret;
}

int32_t jpu_on(struct jpu_data_type *jpu_device)
{
	int32_t ret = 0;

	jpu_check_null_return(jpu_device, -EINVAL);
	if (jpu_device->power_on == 1) {
		jpu_info("jpu_device has already power on\n");
		return ret;
	}

	__pm_stay_awake(g_ws);
	jpu_info("jpu stay awake\n");

	ret = jpu_on_v600(jpu_device);
	if (ret != 0) {
		__pm_relax(g_ws);
		return ret;
	}

	ret = jpu_smmu_on(jpu_device);
	if (ret != 0) {
		if (jpu_off_v600(jpu_device) != 0) {
			jpu_err("jpu off failed\n");
		}
		__pm_relax(g_ws);
		jpu_err("jpu smmu on failed\n");
		return ret;
	}

	jpu_device->power_on = 1;
	jpu_info("power on succ");
	return ret;
}

int32_t jpu_aft_on(struct jpu_data_type *jpu_device)
{
	int32_t ret;
#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	ret = jpu_device->jpgd_drv->setsid_smmu(jpu_device->jpgd_drv,
		jpu_device->jpgd_secadapt_prop.swid[0], MAX_SECADAPT_SWID_NUM,
		jpu_device->jpgd_secadapt_prop.sid, jpu_device->jpgd_secadapt_prop.ssid_ns);
	if (ret != 0) {
		jpu_err("setsid_smmu failed:%d\n", ret);
		if ((jpu_device->jpgd_drv->disable_smmu(jpu_device->jpgd_drv)) != 0)
			jpu_err("failed to disable smmu\n");

		return ret;
	}

	ret = jpu_device->jpgd_drv->set_pref(jpu_device->jpgd_drv,
		jpu_device->jpgd_secadapt_prop.swid[0], MAX_SECADAPT_SWID_NUM);
	if (ret != 0) {
		jpu_err("set_pref failed:%d\n", ret);
		if ((jpu_device->jpgd_drv->disable_smmu(jpu_device->jpgd_drv)) != 0)
			jpu_err("failed to disable smmu\n");
	}
	/* set clk rate 640M */
	ret = jpu_device->jpgd_drv->set_jpgclk_rate(jpu_device->jpgd_drv, CLK_RATE_TURBO);
	if (ret != 0) {
		jpu_err("set_jpgclk_rate fail, change to set low temp rate %d\n", ret);
		/* low temp, when clk set fail, set clk rate to 480M */
		ret = jpu_device->jpgd_drv->set_jpgclk_rate(jpu_device->jpgd_drv, CLK_RATE_NORMAL);
		if (ret != 0) {
			jpu_err("set_jpgclk_rate fail %d\n", ret);
			return ret;
		}
	}
#endif
	/* step 4 jpeg decoder inside clock enable */
	outp32(jpu_device->jpu_top_base + JPGDEC_CRG_CFG0, 0x1);

	if (jpu_device->jpu_support_platform == DSS_V400)
		outp32(jpu_device->jpu_top_base + JPGDEC_MEM_CFG, 0x02605550);

	jpu_dec_interrupt_mask(jpu_device);
	jpu_dec_interrupt_clear(jpu_device);

	ret = jpgdec_enable_irq(jpu_device);
	if (ret != 0) {
		jpu_err("jpu_irq_enable failed\n");
#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
		/* V600 should disable smmu */
		if ((jpu_device->jpgd_drv->disable_smmu(jpu_device->jpgd_drv)) != 0)
			jpu_err("failed to disable smmu\n");
#endif
		return -EINVAL;
	}

	jpu_dec_interrupt_unmask(jpu_device);
	return ret;
}

int32_t jpu_off(struct jpu_data_type *jpu_device)
{
	int32_t ret = 0;

	jpu_check_null_return(jpu_device, -EINVAL);
	if (jpu_device->power_on == 0) {
		jpu_debug("jpu_device has already power off\n");
		return ret;
	}

#if defined(CONFIG_DPU_FB_V501) || defined(CONFIG_DPU_FB_V510) || \
	defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	if (jpu_device->jpgd_drv && jpu_device->jpgd_drv->disable_smmu) {
		if ((jpu_device->jpgd_drv->disable_smmu(jpu_device->jpgd_drv)) != 0)
			jpu_err("failed to disable smmu\n");
	}
#endif

	ret = jpu_off_v600(jpu_device);
	if (ret != 0) {
		goto err_jpu_off;
	}

	jpu_device->power_on = 0;
	jpu_info("power off succ");
err_jpu_off:
	__pm_relax(g_ws);
	jpu_info("jpu relax\n");
	return ret;
}

int32_t jpu_pre_off(struct jpu_data_type *jpu_device)
{
	int32_t ret;
	jpu_dec_interrupt_mask(jpu_device);
	jpgdec_disable_irq(jpu_device);

	/* jpeg decoder inside clock disable */
	outp32(jpu_device->jpu_top_base + JPGDEC_CRG_CFG0, 0x0);
#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	/* set clk rate 104M */
	ret = jpu_device->jpgd_drv->set_jpgclk_rate(jpu_device->jpgd_drv, CLK_RATE_OFF);
	if (ret != 0) {
		jpu_err("set_jpgclk_rate fail %d\n", ret);
		return ret;
	}
#endif
	return ret;
}
#pragma GCC diagnostic pop