/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: coulometer hardware driver headfile
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <securec.h>
#include <linux/spmi_platform.h>
#include <linux/of_platform_spmi.h>
#include <linux/regmap.h>

#include "../../coul_algo/coul_nv.h"
#include <platform_include/basicplatform/linux/power/platform/coul/coul_private_interface.h>
#include "scharger_coul.h"
#include "../pmic_hrst.h"

#ifdef CONFIG_SCHARGER_V700
#include "../../../../../mfd/scharger_v700/scharger_v700_adc.h"
#include "../../../charger/schargerV700/scharger_v700_batctl.h"
#endif
#ifdef CONFIG_SCHARGER_V800
#include "../../../charger/schargerV800/scharger_v800_adc.h"
#endif

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif

#define DSM_BUFF_SIZE_MAX 1024
#define TIMESTAMP_STR_SIZE      32
#define SEC_PER_MIN     60
#define BASIC_YEAR      1900

#define set_reg_bit(reg_val, shift, mask, set_val) \
			(((reg_val)  = ((reg_val) & (~(((1U << (mask)) - 1) << (shift)))) \
			| ((unsigned int)((set_val) & ((1U << (mask)) - 1)) << (shift))))

static void coul_enable_vbat_drop(int batt_index, int en);

struct coul_device_info *g_scharger_coul;
static unsigned int g_batt_num = 0;

static int g_saved_abs_cc_mah_h;
static int g_saved_abs_cc_mah_l;
static int g_r_coul_uohm = R_COUL_UOHM;

#define BATT_H_NODE   "batt_h"
#define BATT_L_NODE   "batt_l"

static int is_batt_h(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;
	if (di == NULL)
		return 0;

	if(di->batt_h_index == batt_index)
		return 1;

	return 0;
}

static void coul_cali_params_get(int batt_index, int *v_offset_a, int *v_offset_b,
	int *c_offset_a, int *c_offset_b)
{
	struct coul_device_info *di = g_scharger_coul;
	struct coul_cali_params *params = NULL;
	int mode = BAT_PARALLEL_MODE;

	if (di == NULL)
		return;

#ifdef CONFIG_SCHARGER_V700
	if (di->chip_version == CHIP_VERSION_V700)
		mode = bat_cnct_mode_get();
#endif

	if(is_batt_h(batt_index))
		params = &di->batt_h_cali_params;
	else
		params = &di->batt_l_cali_params;

	/* The parameter may be is null */
	if(v_offset_a)
		*v_offset_a = (mode == BAT_SERIES_MODE) ?
			params->v_offset_series_a : params->v_offset_a;

	if(v_offset_b)
		*v_offset_b = (mode == BAT_SERIES_MODE) ?
			params->v_offset_series_b : params->v_offset_b;

	if(c_offset_a)
		*c_offset_a = (mode == BAT_SERIES_MODE) ?
			params->c_offset_series_a : params->c_offset_a;

	if(c_offset_b)
		*c_offset_b = (mode == BAT_SERIES_MODE) ?
			params->c_offset_series_b : params->c_offset_b;
}


static unsigned int coul_reg_read(unsigned int reg)
{
	struct coul_device_info *di = g_scharger_coul;
	unsigned int val = 0;

	if (di == NULL || di->regmap == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return 0;
	}

	(void)regmap_read(di->regmap, reg, &val);
	return val;
}

static void coul_reg_write(unsigned int reg, u8 val)
{
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL || di->regmap == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return;
	}

	(void)regmap_write(di->regmap, reg, (unsigned int)val);
}

static void coul_regs_read(unsigned int reg, u8 *val, int val_count)
{
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL || di->regmap == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return;
	}

	(void)regmap_bulk_read(di->regmap, reg, val, val_count);
}

/*
 * schargerV800 coul spmi reg read need enable sc_coul_rd_req and wait 3*32Khz(93.75us) first
 */
static void coul_regs_req_read(unsigned int reg, u8 *val, int val_count)
{
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return;
	}
	if (di->chip_version == CHIP_VERSION_V700) {
		coul_regs_read(reg, val, val_count);
	} else {
		coul_reg_write(di->rd_req, 0x1);
		udelay(100);
		coul_regs_read(reg, val, val_count);
		coul_reg_write(di->rd_req, 0x0);
	}
}

static void coul_regs_write(unsigned int reg, u8 *val, int val_count)
{
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL || di->regmap == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return;
	}

	(void)regmap_bulk_write(di->regmap, reg, val, val_count);
}

static void coul_cc_write_pro(u8 lock)
{
	u8 val;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if (lock == UNLOCK)
		val = di->wp.unlock_val;
	else
		val = di->wp.lock_val;

	coul_reg_write(di->wp.reg, val);
}

static int coul_get_fifo_depth(void)
{
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	return FIFO_DEPTH;
}

/*
 * Description: get coulomb total(in and out) time
 * Return:  sum of total time
 */
static unsigned int coul_get_coul_time(int batt_index)
{
	unsigned int cl_in_time_ro = 0;
	unsigned int cl_out_time_ro = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	udelay(110); /* 110: delay time, us */
	if(is_batt_h(batt_index)) {
		coul_regs_req_read(di->h_batt.cl_out_time_ro, (u8 *)&cl_in_time_ro, 4); /* 4: regs_num */
		coul_regs_req_read(di->h_batt.cl_in_time_ro, (u8 *)&cl_out_time_ro, 4); /* 4: regs_num */
	} else {
		coul_regs_req_read(di->l_batt.cl_out_time_ro, (u8 *)&cl_in_time_ro, 4); /* 4: regs_num */
		coul_regs_req_read(di->l_batt.cl_in_time_ro, (u8 *)&cl_out_time_ro, 4); /* 4: regs_num */
	}

	return (cl_in_time_ro + cl_out_time_ro);
}

/* Description: clear coulomb total(in and out) time */
static void coul_hardware_clear_coul_time(int batt_index)
{
	unsigned int cl_time = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	udelay(110); /* 110: delay time, us */

	coul_cc_write_pro(UNLOCK);
	if(is_batt_h(batt_index)) {
		coul_regs_write(di->h_batt.cl_in_time_rw, (u8 *)&cl_time, 4); /* 4: regs_num */
		coul_regs_write(di->h_batt.cl_out_time_rw, (u8 *)&cl_time, 4); /* 4: regs_num */
	} else {
		coul_regs_write(di->l_batt.cl_in_time_rw, (u8 *)&cl_time, 4); /* 4: regs_num */
		coul_regs_write(di->l_batt.cl_out_time_rw, (u8 *)&cl_time, 4); /* 4: regs_num */
	}
	coul_cc_write_pro(LOCK);
}

/*
 * Description:  convert register value to uv
 * Input: reg_val:voltage reg value
 * Return: value of register in uV
 */
static int coul_convert_ocv_regval2uv(int batt_index, short reg)
{
	s64 temp;
	unsigned short reg_val;
	int v_offset_a = DEFAULT_V_OFF_A;
	int v_offset_b = DEFAULT_V_OFF_B;

	coul_cali_params_get(batt_index, &v_offset_a, &v_offset_b, NULL, NULL);

	reg_val = (unsigned short)reg;
	if (reg_val & INVALID_TO_UPDATE_FCC)
		reg_val &= (~INVALID_TO_UPDATE_FCC);

	/*
	 * code(15bit) * 1.3 * 5 * a / 2^15 + b
	 *  = code * 13 *a / 2^16 + b (a = 1000000, b = 0)
	 *  1bit = 198.364 uV (High 16bit)
	 */
	temp = (s64)reg_val * 13;
	temp = temp * (s64)(v_offset_a);
	temp = div_s64(temp, 65536);
	temp += v_offset_b;
	return (int)temp;
}

/* Description: convert uv value to Bit for register */
static unsigned short coul_convert_ocv_uv2regval(int batt_index, int uv_val)
{
	unsigned short ret;
	s64 temp;
	int v_offset_a = DEFAULT_V_OFF_A;
	int v_offset_b = DEFAULT_V_OFF_B;

	coul_cali_params_get(batt_index, &v_offset_a, &v_offset_b, NULL, NULL);

	/*
	 * uv_val = code(15bit) * 1.3 * 5 *a / 2^15 + b
	 *     = code * 13 *a / 2^16 + b
	 */
	temp = (s64)(uv_val - v_offset_b) * 65536;
	temp = div_s64(temp, 13);
	if (v_offset_a != 0) {
		temp = div_s64(temp, v_offset_a);
	} else {
		coul_hardware_err("v_offset_a is 0, division by default value %d\n",
			DEFAULT_V_OFF_A);
		temp = div_s64(temp, DEFAULT_V_OFF_A);
	}
	ret = (unsigned short)temp;
	return ret;
}

/* Description: convert register value to current(uA) */
static int coul_convert_ocv_regval2ua(int batt_index, short reg_val)
{
	int ret;
	s64 temp;
	int c_offset_a = DEFAULT_C_OFF_A;
	int c_offset_b = DEFAULT_C_OFF_B;
	struct coul_device_info *di = g_scharger_coul;

	coul_cali_params_get(batt_index, NULL, NULL, &c_offset_a, &c_offset_b);

	/*
	 * 6x21V700:Current
	 *   = code * / 2^(n-1) * 1.3 * (1000/10) * (1000/Rsense)
	 *   = code *130000 / Rsense / 2^(n-1)
	 * 6x21V800:Current
	 *   = code * / 2^(n-1) * 0.65 * (1000/10) * (1000/Rsense)
	 *   = code *130000 / Rsense / 2^(n-1)
	 *   if Rsense in mohm, Current in mA
	 *   if Rsense in uohm, Current in uA
	 *   high bit = 1 is in, 0 is out
	 * 1000000: gain to ensure calculation accuracy
	 */
	temp = (s64)reg_val * (s64)130000;
	temp = div_s64(temp * 1000, g_r_coul_uohm);
	temp = temp * 1000; /* mA to uA */
	temp = div_s64(temp, 32768);

	temp = (s64)c_offset_a * temp;
	ret = div_s64(temp, 1000000);
	ret += c_offset_b;
	if (di->chip_version != CHIP_VERSION_V700)
		ret = ret / 2;

	coul_hardware_dbg(
		"[%s] reg_val 0x%lx, ua %d\n", __func__, reg_val, ret);

	return ret;
}

static int convert_regval2uv(int batt_index, unsigned int reg_val, unsigned int use_cali_params)
{
	s64 temp;
	int v_offset_a = DEFAULT_V_OFF_A;
	int v_offset_b = DEFAULT_V_OFF_B;

	int val;
	int ret;

	if(use_cali_params)
	    coul_cali_params_get(batt_index, &v_offset_a, &v_offset_b, NULL, NULL);

	/*
	 * uv_val = code(23bit) * 1.3 * 5 *a / 2^23 + b
	 *     = code * 13 *a / 2^24 + b (a = 1000000, b = 0)
	 * 1bit = 0.77486 uV (Total 24bit)
	 */
	if (reg_val & COUL_BIT_MASK) {
		reg_val |= COUL_NEG_EXPAND;
		val = ((~reg_val) + 1) & (~COUL_BIT_MASK);
	} else {
		val = (int)reg_val;
	}

	temp = (s64)val * 13;
	temp = temp * (s64)(v_offset_a);
	temp = div_s64(temp, 16777216);
	temp += v_offset_b;
	ret = (int)temp;
	if (reg_val & COUL_BIT_MASK)
		return -ret;
	return ret;
}

/*
 * Description:  convert register value to uv
 * Input: reg_val:voltage reg value
 * Return: value of register in uV
 */
static int coul_hardware_convert_regval2uv(int batt_index, unsigned int reg_val)
{
	return convert_regval2uv(batt_index, reg_val, 1);
}

/*
 *  Description: convert register value to uv
 *  Input: reg_val:voltage reg value
 *  Output: NULL
 *  Return: value of register
 *  Remark: vol = code(23bit) * 1.3 / 2^23
 *         temp = (vol - 358.68mv )/ 1.3427  (vol = 1.3427 * temp + 358.68)
 *         For 6x21v700/6x21v800
 */
static int coul_hardware_convert_regval2temp(unsigned int reg_val)
{
	s64 temp;
	int val;
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return -ENODEV;
	}

	if (reg_val & COUL_BIT_MASK) {
		reg_val |= COUL_NEG_EXPAND;
		val = ((~reg_val) + 1) & (~COUL_BIT_MASK);
		val = -val;
	} else {
		val = (int)reg_val;
	}

	/* reg2uv */
	temp = (s64)val * 1300000;
	temp = div_s64(temp, 8388608);
	temp += 650000;

	/* uv2temp */
	temp = (temp - 358680) * 1000;
	temp = div_s64(temp, 1342700);
	coul_hardware_dbg(
		"[%s] reg_val 0x%lx, temp %d\n", __func__, reg_val, temp);

	return (int)temp;
}

/*
 * Description:  convert uv value to Bit for register
 * Remark: mv_val = code(23bit) * 1.3 * 5 *a / 2^23 + b  = code * 13 *a /2^24 + b
 */
static unsigned int coul_convert_uv2regval(int batt_index, int uv_val)
{
	unsigned int ret;
	s64 temp;
	int v_offset_a = DEFAULT_V_OFF_A;
	int v_offset_b = DEFAULT_V_OFF_B;

	coul_cali_params_get(batt_index, &v_offset_a, &v_offset_b, NULL, NULL);

	/*
	 * uv_val = code(23bit) * 1.3 * 5 *a / 2^23 + b
	 *     = code * 13 *a / 2^24 + b (a = 1000000, b = 0)
	 * 1bit = 0.77486 uV (Total 24bit)
	 */
	temp = (s64)(uv_val - v_offset_b) * 16777216;
	temp = div_s64(temp, 13);
	if (v_offset_a != 0) {
		temp = div_s64(temp, v_offset_a);
	} else {
		coul_hardware_err("v_offset_a is 0, division by default value %d\n",
			DEFAULT_V_OFF_A);
		temp = div_s64(temp, DEFAULT_V_OFF_A);
	}

	ret = (unsigned int)temp;
	return ret;
}

/*
 * Description: convert register value to current(uA)
 * Remark:
 *  if Rsense in mohm, Current in mA
 *  if Rsense in uohm, Current in uA
 *  high bit = 1 is in, 0 is out
 */
static int coul_hardware_convert_regval2ua(int batt_index, unsigned int reg_val)
{
	int ret;
	s64 temp;
	int c_offset_a = DEFAULT_C_OFF_A;
	int c_offset_b = DEFAULT_C_OFF_B;
	int val;

	/*
	 *  6x21V700:Current = code * / 2^(n-1) * 1.3 * (1000/10) * (1000/Rsense)
	 *                 = code *130000 / Rsense / 2^(n-1)
	 *  6x21V800:Current = code * / 2^(n-1) * 0.65 * (1000/10) * (1000/Rsense)
	 *                 = code *130000 / Rsense / 2^(n-1)
	 * 1000000: gain to ensure calculation accuracy
	 */
	coul_cali_params_get(batt_index, NULL, NULL, &c_offset_a, &c_offset_b);
	if (reg_val & COUL_BIT_MASK) {
		reg_val |= COUL_NEG_EXPAND;
		val = ((~reg_val) + 1) & (~COUL_BIT_MASK);
	} else {
		val = (int)reg_val;
	}

	temp = (s64)val * 130000;
	temp = div_s64(temp * 1000, g_r_coul_uohm);
	temp = temp * 1000;
	temp = div_s64(temp, 8388608);

	if (g_scharger_coul->chip_version != CHIP_VERSION_V700)
		temp = temp / 2;

	temp = (s64)c_offset_a * temp;
	ret = div_s64(temp, 1000000);
	ret += c_offset_b;
	if (reg_val & COUL_BIT_MASK)
		return -ret;

	return ret;
}

/*
 * Description: convert register value to uah
 * Input: reg_val:cc reg val
 * Return: uah value of reg
 */
static s64 coul_hardware_convert_regval2uah(int batt_index, u64 reg_val)
{
	s64 ret;
	s64 temp;
	int c_offset_a = DEFAULT_C_OFF_A;

	/*
	 * 1bit is 1.107619E-06 mAh (v700)
	 */
	coul_cali_params_get(batt_index, NULL, NULL, &c_offset_a, NULL);
	temp = (s64)reg_val;
	temp = temp * 107619 * 10 / ((s64)g_r_coul_uohm / 100);
	temp = (s64)div_s64(temp, 100000000);

	temp = (s64)c_offset_a * temp;
	ret = div_s64(temp, 1000000);

	if (g_scharger_coul->chip_version != CHIP_VERSION_V700)
		ret = ret / 2;

	return ret;
}

/* Description: convert uah value to reg */
static u64 coul_convert_uah2regval(int batt_index, unsigned int uah)
{
	u64 ret;
	u64 temp;
	int c_offset_a = DEFAULT_C_OFF_A;

	coul_cali_params_get(batt_index, NULL, NULL, &c_offset_a, NULL);

	temp = uah;
	temp = temp * 1000000;
	if (c_offset_a != 0) {
		temp = (u64)div_s64(temp, c_offset_a);
	} else {
		coul_hardware_err("c_offset_a is 0, division by default value %d\n",
			DEFAULT_C_OFF_A);
		temp = (u64)div_s64(temp, DEFAULT_C_OFF_A);
	}

	temp = temp * 100000;
	temp = temp * (u64)g_r_coul_uohm;
	ret = (u64)div_s64(temp, 107619);

	if (g_scharger_coul->chip_version != CHIP_VERSION_V700)
		ret = ret * 2;

	coul_hardware_dbg("[%s] uah %d, ret 0x%lx\n", __func__, uah, ret);

	return ret;
}

/* For schargerV700 chip bug */
static void recalc_battery_volt(int batt_index, u32 *regval)
{
	int offset_l = 0;
	int offset_h = 0;
	int offset_h_is_error = 0;
	int offset_uv_h = 0;
	int ori = *regval;
	u8 cnct_sel = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}
	if (di->chip_version != CHIP_VERSION_V700)
		return;
	if ((di->scharger_version != CHIP_ID_V700) && (di->scharger_version != CHIP_ID_V710))
		return;

	cnct_sel = coul_reg_read(di->bat_cnct_sel);
	coul_regs_read(di->l_batt.offset_voltage, (u8 *)&offset_l, REG_NUM);
	coul_regs_read(di->h_batt.offset_voltage, (u8 *)&offset_h, REG_NUM);

	/* Check offset_h is error. */
	offset_uv_h = convert_regval2uv(batt_index, offset_h, 0);
	if ((offset_uv_h > 20000) || (offset_uv_h < -20000)) {
		offset_h_is_error = 1;
		coul_hardware_err(
			"%s:batt_index %d, cnct_sel %d, offset_l 0x%x, offset_h 0x%x, ori 0x%x, offset_uv_h %duV\n",
			__func__, batt_index, (int)cnct_sel, offset_l, offset_h, ori, offset_uv_h);
	}

	if (offset_h_is_error) {
		if (cnct_sel && is_batt_h(batt_index)) {
			*regval = (u32)(((ori << 8) >> 8) + ((offset_h << 8) >> 8));
			return;
		}
		offset_h = 0;
	}

	if (cnct_sel && is_batt_h(batt_index))
		return;

	/* Converts the 24-bit code value to the signed int. */
	*regval = (u32)(((ori << 8) >> 8) + ((offset_l << 8) >> 8) - ((offset_h << 8) >> 8));
}

/*
 * Description:  value of out_uah - in_uah recorded by  coulomb
 * Return: value of uah through coulomb
 * Remark: adjusted by offset integrated on time
 */
static int coul_calculate_cc_uah(int batt_index)
{
	u64 cc_in = 0;
	u64 cc_out = 0;
	unsigned int cl_in_time_ro = 0;
	unsigned int cl_out_time_ro = 0;
	s64 cc_uah_in;
	s64 cc_uah_out;
	int cc_uah;
	int c_offset_b = DEFAULT_C_OFF_B;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	coul_cali_params_get(batt_index, NULL, NULL, NULL, &c_offset_b);

	if(is_batt_h(batt_index)) {
		coul_regs_req_read(di->h_batt.cl_in, (u8 *)&cc_in, CC_REG_NUM);
		coul_regs_req_read(di->h_batt.cl_out, (u8 *)&cc_out, CC_REG_NUM);
		coul_regs_req_read(di->h_batt.cl_in_time_ro, (u8 *)&cl_in_time_ro, 4); /* 4: regs_num */
		coul_regs_req_read(di->h_batt.cl_out_time_ro, (u8 *)&cl_out_time_ro, 4); /* 4: regs_num */
	} else {
		coul_regs_req_read(di->l_batt.cl_in, (u8 *)&cc_in, CC_REG_NUM);
		coul_regs_req_read(di->l_batt.cl_out, (u8 *)&cc_out, CC_REG_NUM);
		coul_regs_req_read(di->l_batt.cl_in_time_ro, (u8 *)&cl_in_time_ro, 4); /* 4: regs_num */
		coul_regs_req_read(di->l_batt.cl_out_time_ro, (u8 *)&cl_out_time_ro, 4); /* 4: regs_num */
	}

	cc_uah_out = coul_hardware_convert_regval2uah(batt_index, cc_out);
	cc_uah_in = coul_hardware_convert_regval2uah(batt_index, cc_in);

	/* uah = uas/3700 = ua*s/3700 */
	cc_uah_in -= div_s64((s64)cl_in_time_ro * c_offset_b, SECONDS_PER_HOUR);
	cc_uah_out += div_s64((s64)cl_out_time_ro * c_offset_b, SECONDS_PER_HOUR);
	cc_uah = (int)(cc_uah_out - cc_uah_in);

	coul_hardware_dbg(
		"batt_index%d: cc_out=0x%llx,cc_in=0x%llx,cout_time=%d,cin_time=%d,cc_adj=%duah\n",
		batt_index, cc_out, cc_in, cl_out_time_ro, cl_in_time_ro, cc_uah);
	coul_hardware_dbg(
		"batt_index%d: cc_out=%d,cc_in=%d, cc_adj=%duah\n",
		batt_index, cc_uah_out, cc_uah_in, cc_uah);
	return cc_uah;
}

static void coul_save_cc_uah(int batt_index, int cc_uah)
{
	u64 reg;
	unsigned int cl_out_reg;
	unsigned int cl_in_reg;
	struct coul_device_info *di = g_scharger_coul;

	coul_hardware_inf("%s, batt_index %d, cc_uah %d\n", __func__, batt_index, cc_uah);

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if(is_batt_h(batt_index)) {
		cl_out_reg = di->h_batt.cl_out_rw;
		cl_in_reg = di->h_batt.cl_in_rw;
	} else {
		cl_out_reg = di->l_batt.cl_out_rw;
		cl_in_reg = di->l_batt.cl_in_rw;
	}

	coul_cc_write_pro(UNLOCK);
	if (cc_uah > 0) {
		reg = coul_convert_uah2regval(batt_index, cc_uah);
		udelay(110); /* 110: delay time, us */
		coul_regs_write(cl_out_reg, (u8 *)&reg, CC_REG_NUM);
		reg = 0;
		coul_regs_write(cl_in_reg, (u8 *)&reg, CC_REG_NUM);
	} else {
		reg = coul_convert_uah2regval(batt_index, -cc_uah);
		udelay(110); /* 110: delay time, us */
		coul_regs_write(cl_in_reg, (u8 *)&reg, CC_REG_NUM);
		reg = 0;
		coul_regs_write(cl_out_reg, (u8 *)&reg, CC_REG_NUM);
	}
	coul_cc_write_pro(LOCK);
}

/*
 * Description: coul save ocv
 * Input: ocv:ocv vol uv
 * invalid_fcc_up_flag: no update fcc 1, update 0
 */
static void coul_save_ocv(int batt, int ocv)
{
	unsigned short ocvreg;

	ocvreg = coul_convert_ocv_uv2regval(batt, ocv);
	coul_save_ocvreg(batt, ocvreg);
	coul_hardware_inf("save ocv, ocv=%d,reg=%d", ocv, ocvreg);
}

static int coul_hardware_get_battery_voltage_uv(int batt_index)
{
	int uv;
	unsigned int regval = 0;
	struct coul_device_info *di = g_scharger_coul;
	static int log_cnt;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if(is_batt_h(batt_index))
		coul_regs_req_read(di->h_batt.vol, (u8 *)&regval, REG_NUM);
	else
		coul_regs_req_read(di->l_batt.vol, (u8 *)&regval, REG_NUM);

	recalc_battery_volt(batt_index, &regval);
	uv = coul_hardware_convert_regval2uv(batt_index, regval);

	if (log_cnt == 300) {
		coul_hardware_inf("%s:batt_index %d, regval 0x%x, uv %d\n",
			__func__, batt_index, regval, uv);
		log_cnt = 0;
	}
	log_cnt++;
	return uv;
}

static int coul_get_battery_current_ua(int batt_index)
{
	int ua;
	unsigned int regval = 0;
	struct coul_device_info *di = g_scharger_coul;
	static int log_cnt;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if(is_batt_h(batt_index))
		coul_regs_req_read(di->h_batt.cur, (u8 *)&regval, REG_NUM);
	else
		coul_regs_req_read(di->l_batt.cur, (u8 *)&regval, REG_NUM);

	ua = coul_hardware_convert_regval2ua(batt_index, regval);

	if (log_cnt == 100) {
		coul_hardware_inf("%s:batt_index %d, regval 0x%x, ua %d\n",
			__func__, batt_index, regval, ua);
		log_cnt = 0;
	}
	log_cnt++;
	return ua;
}

/*
 * Description:   get battery vol in uv from fifo
 * Intput: fifo_order:fifo serial number 0-9
 */
static int coul_get_battery_vol_uv_from_fifo(int batt_index, short fifo_order)
{
	int uv;
	unsigned int regval = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if (fifo_order > FIFO_DEPTH)
		fifo_order = 0;

	if(is_batt_h(batt_index))
		coul_regs_req_read((di->h_batt.vol_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);
	else
		coul_regs_req_read((di->l_batt.vol_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);

	recalc_battery_volt(batt_index, &regval);
	uv = coul_hardware_convert_regval2uv(batt_index, regval);

	coul_hardware_dbg("[%s]batt_index%d [%d]0x%x, %duv\n", __func__, batt_index, fifo_order, regval, uv);
	return uv;
}

/*
 * Description: get battery cur in ua from fifo
 * Intput: fifo_order:fifo serial number 0-9
 */
static int coul_get_battery_cur_ua_from_fifo(int batt_index, short fifo_order)
{
	int ua;
	unsigned int regval = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if (fifo_order > FIFO_DEPTH)
		fifo_order = 0;

	if(is_batt_h(batt_index))
		coul_regs_req_read((di->h_batt.cur_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);
	else
		coul_regs_req_read((di->l_batt.cur_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);

	ua = coul_hardware_convert_regval2ua(batt_index, regval);

	coul_hardware_dbg("[%s] batt_index %d, i %d, 0x%x[%dua], ua %d\n",
		__func__, batt_index, fifo_order, regval, ua);
	return ua;
}

static int coul_get_battery_vol_uv_drop_from_fifo(int batt_index, short fifo_order)
{
	int uv;
	unsigned int regval = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if (fifo_order > V_DROP_FIFO_DEPTH)
		fifo_order = 0;

	if(is_batt_h(batt_index))
		coul_regs_req_read((di->h_batt.vbat_drop_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);
	else
		coul_regs_req_read((di->l_batt.vbat_drop_fifo + REG_NUM * fifo_order),
			(u8 *)&regval, REG_NUM);

	uv = coul_hardware_convert_regval2uv(0, regval);
	return uv;
}

static int coul_get_vbat_drop_th(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return -1;
	}

	if(is_batt_h(batt_index))
		return (int)(di->vbat_h_drop_th);
	else
		return (int)(di->vbat_l_drop_th);
}

/* Return: current offset mod value */
static short coul_get_offset_current_mod(void)
{
	return 0;
}

/* Return: vol offset mod value */
static short coul_get_offset_vol_mod(void)
{
	return 0;
}

static void coul_set_offset_vol_mod(void)
{
}

static void get_vc_by_cur(struct vcdata *vc, int cur)
{
	if (cur > vc->max_c)
		vc->max_c = cur;
	else if (cur < vc->min_c)
		vc->min_c = cur;
}
/*
 * Description:  get coul fifo average vol/current value(uv/ua)
 * Output: struct vcdata:avg , max and min cur, vol
 */
static void coul_get_fifo_avg_data(int batt_index, struct vcdata *vc)
{
	short i;
	int abnormal_value_cnt = 0;
	unsigned int vol_fifo[FIFO_DEPTH] = {0};
	unsigned int cur_fifo[FIFO_DEPTH] = {0};
	int cur, vols, curs;
	struct coul_device_info *di = g_scharger_coul;

	if(!di || !vc) {
		coul_hardware_err("%s di or vc is null.\n", __func__);
		return;
	}

	for (i = 0; i < FIFO_DEPTH; i++) {
		if(is_batt_h(batt_index)) {
			coul_regs_req_read(di->h_batt.vol_fifo + i * REG_NUM,
						(u8 *)&vol_fifo[i], REG_NUM);
			coul_regs_req_read(di->h_batt.cur_fifo + i * REG_NUM,
						(u8 *)&cur_fifo[i], REG_NUM);
		} else {
			coul_regs_req_read(di->l_batt.vol_fifo + i * REG_NUM,
						(u8 *)&vol_fifo[i], REG_NUM);
			coul_regs_req_read(di->l_batt.cur_fifo + i * REG_NUM,
						(u8 *)&cur_fifo[i], REG_NUM);
		}

		if (vol_fifo[i] != COUL_FIFO_VOL_DEFAULT)
			recalc_battery_volt(batt_index, &(vol_fifo[i]));
	}

	/* COUL_FIFO_VOL_DEFAULT:    vol_reset_value */
	if (vol_fifo[0] != COUL_FIFO_VOL_DEFAULT) {
		vols = coul_hardware_convert_regval2uv(batt_index, vol_fifo[0]) / PERMILLAGE;
		cur = coul_hardware_convert_regval2ua(batt_index, cur_fifo[0]) / PERMILLAGE;
	} else {
		vols = 0;
		cur = 0;
		abnormal_value_cnt++;
	}

	curs = cur;
	vc->max_c = cur;
	vc->min_c = cur;

	for (i = 1; i < FIFO_DEPTH; i++) {
		if (vol_fifo[i] != COUL_FIFO_VOL_DEFAULT) {
			vols += coul_hardware_convert_regval2uv(batt_index, vol_fifo[i]) / PERMILLAGE;
			cur = coul_hardware_convert_regval2ua(batt_index, cur_fifo[i]) / PERMILLAGE;
			curs += cur;
			get_vc_by_cur(vc, cur);
		} else {
			abnormal_value_cnt++;
		}
	}
	if (abnormal_value_cnt == FIFO_DEPTH) {
		vc->avg_v = 0;
		vc->avg_c = 0;
	} else {
		vc->avg_v = vols / (FIFO_DEPTH - abnormal_value_cnt);
		vc->avg_c = curs / (FIFO_DEPTH - abnormal_value_cnt);
	}

	coul_hardware_inf("batt %d, avg_v = %d, avg_c = %d, max_c = %d, min_c = %d, invalid_cnt=%d\n",
		batt_index, vc->avg_v, vc->avg_c, vc->max_c, vc->min_c, abnormal_value_cnt);
}

static int coul_get_abs_cc(int batt_index)
{
	if(is_batt_h(batt_index))
		return g_saved_abs_cc_mah_h;
	else
		return g_saved_abs_cc_mah_l;
}

/* Description: get v_offset a value */
static int coul_get_ate_a(void)
{
	return 0;
}

/* get v_offset b value */
static int coul_get_ate_b(void)
{
	return 0;
}

static void coul_hardware_clear_cc_register(int batt_index)
{
	u64 ccregval = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if(is_batt_h(batt_index)) {
		g_saved_abs_cc_mah_h += (coul_calculate_cc_uah(batt_index) / PERMILLAGE);
		udelay(110); /* 110: delay time, us */
		coul_cc_write_pro(UNLOCK);
		coul_regs_write(di->h_batt.cl_in_rw, (u8 *)&ccregval, CC_REG_NUM);
		coul_regs_write(di->h_batt.cl_out_rw, (u8 *)&ccregval, CC_REG_NUM);
		coul_cc_write_pro(LOCK);
	} else {
		g_saved_abs_cc_mah_l += (coul_calculate_cc_uah(batt_index) / PERMILLAGE);
		udelay(110); /* 110: delay time, us */
		coul_cc_write_pro(UNLOCK);
		coul_regs_write(di->l_batt.cl_in_rw, (u8 *)&ccregval, CC_REG_NUM);
		coul_regs_write(di->l_batt.cl_out_rw, (u8 *)&ccregval, CC_REG_NUM);
		coul_cc_write_pro(LOCK);
	}
	coul_hardware_inf("[%s]batt_index%d: cc_mah_l %d, cc_mah_h %d\n", __func__,
		batt_index, g_saved_abs_cc_mah_l, g_saved_abs_cc_mah_h);
}

/*
 * Description: set low int vol val
 * Input: vol_value:low int vol val(mV)
 */
static void coul_set_low_vol_val(int vol_mv)
{
	unsigned int regval;
	struct coul_device_info *di = g_scharger_coul;
	int offset_l = 0;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	di->low_vol_mv = vol_mv;

	regval = coul_convert_uv2regval(di->batt_l_index, vol_mv * PERMILLAGE);

	/* --chip bug start-- */
	if (di->chip_version == CHIP_VERSION_V700 ) {
		if ((di->scharger_version == CHIP_ID_V700) ||
			(di->scharger_version == CHIP_ID_V710)) {
			coul_regs_read(di->l_batt.offset_voltage, (u8 *)&offset_l, REG_NUM);
			offset_l = ((offset_l<< 8) >> 8);
			regval -= (unsigned int)offset_l;
		}
	}
	/* --chip bug end-- */

	coul_hardware_dbg("[%s] mv %d, reg 0x%x\n", __func__, vol_mv, regval);
	udelay(110); /* 110: delay time, us */
	coul_regs_write(di->v_int, (u8 *)&regval, REG_NUM);
}

/* Description: coul calibration */
static void coul_hardware_cali_adc(int batt_index)
{
	unsigned char reg_val = 0;
	struct coul_device_info *di = g_scharger_coul;

	/* Just calibrate one channel. */
	if(is_batt_h(batt_index))
		return;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	coul_regs_req_read(di->coul_mstate, (u8 *)&reg_val, 1);
	if (reg_val == COUL_CALI_ING) {
		coul_hardware_inf("cali ing, don't do it again!\n");
		return;
	}

	coul_hardware_inf("calibrate!\n");
	reg_val = coul_reg_read(di->clj_ctrl1);
	/* Mode */
	reg_val = reg_val | di->regs_val.coul_cali_enable;
	udelay(110); /* 110: delay time, us */
	coul_reg_write(di->clj_ctrl1, reg_val);

	__pm_wakeup_event(di->wake_lock, ADC_CALI_TIMEOUT + ADC_DELAY);
	queue_delayed_work(
		system_power_efficient_wq, &di->chip_cali_work, msecs_to_jiffies(ADC_CALI_TIMEOUT));
}

static void coul_hardware_cali_work(struct work_struct *work)
{
	struct coul_device_info *di = container_of(
		work, struct coul_device_info, chip_cali_work.work);

	coul_set_low_vol_val(di->low_vol_mv);

	__pm_relax(di->wake_lock);
}

/*
 * Description: clear coul interrupt
 * Remark: clear low vol/capacity INT before coul self_adjust when start up
 */
static void coul_clear_irq(void)
{
	struct coul_device_info *di = g_scharger_coul;
	char val = di->regs_val.coul_clear_irq;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	coul_reg_write(di->h_batt.irq_flag, val);
	coul_reg_write(di->l_batt.irq_flag, val);
}

static void coul_enable_irq(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;
	unsigned char irq_enable_flag;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	irq_enable_flag = ~((unsigned char)(di->regs_val.coul_vbat_int_mask | \
		di->regs_val.coul_cl_in_mask | di->regs_val.coul_cl_out_mask | \
		di->regs_val.coul_vbat_drop_mask));

	/* disable cl_int interrupt, disable i out/in */
	if(is_batt_h(batt_index))
		coul_reg_write(di->h_batt.irq_mask, irq_enable_flag);
	else
		coul_reg_write(di->l_batt.irq_mask, irq_enable_flag);
	coul_hardware_inf("Enable coul irq, batt_index %d!\n", batt_index);
}

static void coul_disable_irq(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if(is_batt_h(batt_index))
		coul_reg_write(di->h_batt.irq_mask, di->regs_val.coul_int_mask_all);
	else
		coul_reg_write(di->l_batt.irq_mask, di->regs_val.coul_int_mask_all);
	coul_hardware_inf("Mask coul irq, batt_index %d\n", batt_index);
}

/*
 * Description:  convert register value to current(uA)
 * Input:  reg_val:current reg value
 * Return: value of register in uA
 * Remark:
 *  6x21V700: Code = mA * 2^23 / 1.3 * 10 * Rsense / 1000 / 1000
 *  6x21V800&6x55V300: Code = mA * 2^23 / 0.65 * 10 * Rsense / 1000 / 1000
 *  high bit = 1 is in, 0 is out,  Rsense in mohm
 *  only for 6x21V700 & V800 & V300
 * 1000000: gain to ensure calculation accuracy
 */
static unsigned int coul_convert_ua2regval(int batt_index, int ua)
{
	unsigned int ret;
	s64 temp;
	int val = ua;
	int c_offset_a = DEFAULT_C_OFF_A;

	coul_cali_params_get(batt_index, NULL, NULL, &c_offset_a, NULL);
	val = val * 2;
	temp = (s64)val * 1000000;
	if (c_offset_a != 0) {
		temp = div_s64(temp, c_offset_a);
	} else {
		coul_hardware_err("c_offset_a = 0, division by default value %d\n",
			DEFAULT_C_OFF_A);
		temp = div_s64(temp, DEFAULT_C_OFF_A);
	}

	temp = temp * 8388608;
	temp = temp * 100 / 1000 * (s64)g_r_coul_uohm;
	temp = div_s64(temp, 13);
	temp = div_s64(temp, 1000000000);

	if (g_scharger_coul->chip_version != CHIP_VERSION_V700)
		temp = temp * 2;

	ret = (unsigned int)temp;

	coul_hardware_dbg("[%s] ua %d, ret 0x%x\n", __func__, ua, ret);
	return ret;
}

/*
 * Description: set i in gate
 * Input: ma, should < 0
 */
static void coul_set_i_in_event_gate(int batt_index, int ma)
{
	unsigned int reg_val;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if (ma > 0)
		ma = -ma;
	reg_val = coul_convert_ua2regval(batt_index, ma * PERMILLAGE);
	udelay(110); /* 110: delay time, us */

	coul_regs_write(di->i_in_gate, (u8 *)&reg_val, REG_NUM);
}

/*
 * Description: set i out gate
 * Input: ma, should > 0
 */
static void coul_set_i_out_event_gate(int batt_index, int ma)
{
	unsigned int reg_val;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if (ma < 0)
		ma = -ma;
	reg_val = coul_convert_ua2regval(batt_index, ma * PERMILLAGE);
	udelay(110); /* 110: delay time, us */
	coul_regs_write(di->i_out_gate, (u8 *)&reg_val, REG_NUM);
}

static void coul_wait_comp_enable(struct coul_device_info *di)
{
	u8 val;

	val = coul_reg_read(di->clj_ctrl3);
	val = (val | di->regs_val.wait_comp_en);
	coul_reg_write(di->clj_ctrl3, val);
}

/* Description: config init */
static void coul_chip_init(struct coul_device_info *di)
{
	coul_clear_irq();
	coul_disable_irq(di->batt_l_index);
	coul_disable_irq(di->batt_h_index);

	coul_set_i_in_event_gate(di->batt_l_index, DEFAULT_I_GATE_VALUE);
	coul_set_i_out_event_gate(di->batt_l_index, DEFAULT_I_GATE_VALUE);

	/* config coul Mode */
	udelay(110); /* 110: delay time, us */
	coul_reg_write(di->clj_ctrl1, di->regs_val.default_coul_ctrl_val);

	coul_wait_comp_enable(di);
}

static void coul_enable_vbat_drop(int batt_index, int en);

static void _coul_interrupt_notifier(struct coul_device_info *di,
	unsigned char intstat, int batt_index)
{
	if (intstat & di->regs_val.coul_i_out_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_I_OUT_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_I_OUT, NULL);
	}
	if (intstat & di->regs_val.coul_i_in_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_I_IN_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_I_IN, NULL);
	}
	if (intstat & di->regs_val.coul_vbat_int_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_CCOUT_LOW_VOL_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_LOW_VOL, NULL);
	}
	if (intstat & di->regs_val.coul_cl_int_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_CCOUT_BIG_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_CL_INT, NULL);
	}
	if (intstat & di->regs_val.coul_cl_in_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_CCIN_CNT_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_CL_IN, NULL);
	}
	if (intstat & di->regs_val.coul_cl_out_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_CCOUT_CNT_INT\n", batt_index);
		coul_core_notify_fault(batt_index, COUL_FAULT_CL_OUT, NULL);
	}
	if (intstat & di->regs_val.coul_vbat_drop_mask) {
		coul_hardware_inf("BATT %d IRQ: COUL_VBAT_DROP_INT\n", batt_index);
		coul_enable_vbat_drop(batt_index, DIS); /* for chip bug: multiple irq after only one drop */
		coul_core_notify_fault(batt_index, COUL_FAULT_VBAT_DROP, NULL);
	}
}

/*
 * Description: interrupt_notifier_work - send a notifier event to battery monitor.
 * Remark: capacity INT : low level and shutdown level.
 */
static void coul_interrupt_notifier_work(struct work_struct *work)
{
	struct coul_device_info *di =
		container_of(work, struct coul_device_info, irq_work);
	struct irq_desc *desc = NULL;
	unsigned char irq_val_l;
	unsigned char irq_val_h;

	coul_hardware_inf("%s +\n", __func__);

	(void)coul_reg_read(di->irq_flag);
	irq_val_l = coul_reg_read(di->l_batt.irq_flag);
	irq_val_h = coul_reg_read(di->h_batt.irq_flag);
	/* clear irq */
	coul_reg_write(di->l_batt.irq_flag, irq_val_l);
	coul_reg_write(di->h_batt.irq_flag, irq_val_h);

	_coul_interrupt_notifier(di, irq_val_l, di->batt_l_index);
	_coul_interrupt_notifier(di, irq_val_h, di->batt_h_index);

	if (di->chip_version != CHIP_VERSION_V700) {
		desc = irq_to_desc(di->irq);
		if (desc == NULL) {
			coul_hardware_err("[%s]irq_to_desc failed\n", __func__);
			return;
		}
		if (desc->irq_data.chip->irq_unmask != NULL)
			desc->irq_data.chip->irq_unmask(&desc->irq_data);
	}
}

/*
 * Description: coul irq handler
 * Input: int irq ---- irq number
 *     void *_di ---- coul device
 * Return: IRQ_NONE: irq not exist,  IRQ_HANDLED: be handled
 */
static irqreturn_t coul_irq_cb(int irq, void *_di)
{
	struct coul_device_info *di = _di;
	struct irq_desc *desc = NULL;

	coul_hardware_inf("%s +\n", __func__);

	if (di->chip_version != CHIP_VERSION_V700) {
		desc = irq_to_desc(irq);
		if (desc == NULL) {
			coul_hardware_err("[%s]irq_to_desc failed\n", __func__);
			return IRQ_NONE;
		}

		if (desc->irq_data.chip->irq_mask != NULL)
			desc->irq_data.chip->irq_mask(&desc->irq_data);
	}

	schedule_work(&di->irq_work);
	return IRQ_HANDLED;
}

/*
 * Description: calculate capacity leak from existing ECO MODE to calc soc first time
 * Remark: ECO uah register keep the same value after exist from ECO
 */
static int coul_calculate_eco_leak_uah(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;
	struct eco_leak_info info = {0};
	struct batt_regs *batt_reg = NULL;
	u64 last_eco_in;
	u64 last_eco_out;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

	if (is_batt_h(batt_index)) {
		batt_reg = &di->h_batt;
		last_eco_in = di->g_last_eco_in_h;
		last_eco_out = di->g_last_eco_out_h;
	} else {
		batt_reg = &di->l_batt;
		last_eco_in = di->g_last_eco_in_l;
		last_eco_out = di->g_last_eco_out_l;
	}

	coul_regs_read(batt_reg->eco_out_clin, (u8 *)&info.in_val, CC_REG_NUM);
	coul_regs_read(batt_reg->eco_out_clout, (u8 *)&info.out_val, CC_REG_NUM);

	/* if: first time to calc soc after exiting from ECO Mode */
	if ((last_eco_in != info.in_val) || (last_eco_out != info.out_val)) {
		info.eco_out_uah = coul_hardware_convert_regval2uah(batt_index, info.out_val);
		info.eco_in_uah = coul_hardware_convert_regval2uah(batt_index, info.in_val);
		info.eco_uah = (int)(info.eco_out_uah - info.eco_in_uah);
		/* current cc */
		coul_regs_req_read(batt_reg->cl_out, (u8 *)&info.out_val, CC_REG_NUM);
		coul_regs_req_read(batt_reg->cl_in, (u8 *)&info.in_val, CC_REG_NUM);
		info.present_in_uah = coul_hardware_convert_regval2uah(batt_index, info.in_val);
		info.present_out_uah = coul_hardware_convert_regval2uah(batt_index, info.out_val);
		info.cur_uah = (int)(info.present_out_uah - info.present_in_uah);
		/* leak cc from exisingt eco mode to first calc soc */
		info.rst_uah = info.cur_uah - info.eco_uah;
		coul_hardware_err(
			"batt_index %d, eco_in=%d,eco_out=%d,cc_in=%d,cc_out=%d,leakcc=%d\n",
			batt_index, info.eco_in_uah, info.eco_out_uah, info.present_in_uah,
			info.present_out_uah, info.rst_uah);
	} else {
		info.rst_uah = 0;
		coul_hardware_inf(
			"batt_index %d, Not the FIRST calc soc out eco, leak cc=0\n", batt_index);
	}
	return info.rst_uah;
}

/* Description: clear coul vol/current fifo value */
static void coul_clear_fifo(struct coul_device_info *di)
{
	unsigned char reg_value;

	reg_value = coul_reg_read(di->clj_ctrl2);
	coul_reg_write(di->clj_ctrl2, (reg_value | di->regs_val.data_clear));
}

/* Description: clear coul vol/current eco fifo value */
static void coul_clear_enable_eco_fifo(struct coul_device_info *di)
{
	unsigned char reg_value;

	reg_value = coul_reg_read(di->clj_ctrl4);
	coul_reg_write(di->clj_ctrl4,
		(reg_value | di->regs_val.eco_fifo_clear | di->regs_val.eco_fifo_en));
}

static void coul_eco_filter_out_enable(struct coul_device_info *di)
{
	unsigned char reg_value;

	/* eco filter enable (eco_filter_time set 200ms) */
	reg_value = coul_reg_read(di->clj_ctrl);
	coul_reg_write(di->clj_ctrl, reg_value | di->regs_val.eco_filter_out_en);
}

/* Remark: coul eco follow pmu eco */
static void coul_enter_eco(int batt_index)
{
	unsigned char reg_val;
	struct coul_device_info *di = g_scharger_coul;

	if (!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}
	/*
	 * Regardless of whether it is a high-side battery or a low-side battery,
	 * whoever enters suspend first will have the chip enter eco.
	 */
	if (is_batt_h(batt_index))
		di->vote_count |= (1 << 1);
	else
		di->vote_count |= (1 << 0);
	if ((g_batt_num > 1) && (di->vote_count & 0x03) != 0x03) /* 0b11 */
		return;

	coul_hardware_inf("%s +\n", __func__);

	coul_regs_read(di->h_batt.eco_out_clin, (u8 *)&di->g_last_eco_in_h, CC_REG_NUM);
	coul_regs_read(di->h_batt.eco_out_clout, (u8 *)&di->g_last_eco_out_h, CC_REG_NUM);
	coul_regs_read(di->l_batt.eco_out_clin, (u8 *)&di->g_last_eco_in_l, CC_REG_NUM);
	coul_regs_read(di->l_batt.eco_out_clout, (u8 *)&di->g_last_eco_out_l, CC_REG_NUM);

	/* exit eco mode filter_time 200ms; only eco reflash */
	reg_val = di->regs_val.eco_coul_ctrl_val;
	udelay(110); /* 110: delay time, us */
	coul_reg_write(di->clj_ctrl1, reg_val);

	/* clear volt&current fifo */
	coul_clear_fifo(di);
	coul_clear_enable_eco_fifo(di);

	coul_eco_filter_out_enable(di);

	/* Enter the ECO mode in the LPMCU,
	   SCHARGER_V700_AP_ECO not need to be defined */
#ifdef SCHARGER_V700_AP_ECO
	reg_val = coul_reg_read(di->clj_ctrl);
	coul_reg_write(di->clj_ctrl, reg_val | di->regs_val.eco_enter);
#endif
}

static void coul_exit_eco(int batt_index)
{
	struct coul_device_info *di = g_scharger_coul;
	u8 reg_val;

	if (!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}
	/*
	 * Only after the high-side battery and low-side battery have entered resume
	 * can the chip exit eco.
	 */
	if (is_batt_h(batt_index))
		di->vote_count &= ~(1 << 1);
	else
		di->vote_count &= ~(1 << 0);
	if ((g_batt_num > 1) && (di->vote_count & 0x03) != 0x00) /* 0b11 */
		return;

	coul_hardware_err("%s +\n", __func__);

	coul_clear_fifo(di);
	udelay(110); /* 110: delay time, us */
	/* restore reflash fifo */
	coul_reg_write(di->clj_ctrl1, di->regs_val.default_coul_ctrl_val);

	reg_val = coul_reg_read(di->clj_ctrl);
	coul_reg_write(di->clj_ctrl, reg_val & (~di->regs_val.eco_enter) );
}

static void coul_cancel_auto_cali(void)
{
	u8 val;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	val = (u8)coul_reg_read(di->clj_ctrl2);
	val = val & (~di->regs_val.mask_cali_auto_off);
	udelay(110); /* 110: delay time, us */
	coul_reg_write(di->clj_ctrl2, val);
}

static int coul_get_tdie(void)
{
	int ret = 0;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return ret;
	}

#ifdef CONFIG_SCHARGER_V700
	if (di->chip_version == CHIP_VERSION_V700)
		ret = scharger_get_tdie();
#endif
#ifdef CONFIG_SCHARGER_V800
	if (di->chip_version == CHIP_VERSION_V800)
		ret = scharger_v800_get_tdie();
#endif
	return ret;
}

static int coul_get_tbat(int batt_index)
{
	int temp = 0;
	struct coul_device_info *di = g_scharger_coul;

	if (!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

#ifdef CONFIG_SCHARGER_V700
	if (di->chip_version == CHIP_VERSION_V700) {
		if (is_batt_h(batt_index))
			temp = scharger_get_tsbat_h();
		else
			temp = scharger_get_tsbat_l();
	}
#endif
#ifdef CONFIG_SCHARGER_V800
	if (di->chip_version == CHIP_VERSION_V800)
		temp = scharger_v800_get_tsbat();
#endif

	coul_hardware_dbg("%s batt_index %d, tbat %d\n", __func__, batt_index, temp);
	return temp;
}

static void coul_set_vbat_drop_enable(struct batt_regs *batt_reg, int index, int en_inp)
{
	unsigned char reg_value, en;

	en = (en_inp == EN) ? 1 : 0;
	/* Set vbat drop en/disable */
	reg_value = coul_reg_read(batt_reg->vbat_drop);
	set_reg_bit(reg_value, batt_reg->vbat_drop_en_shift, VBAT_EN_MASK_NUM, en);
	coul_reg_write(batt_reg->vbat_drop, reg_value);
	coul_hardware_inf("%s batt %d status: %d\n", __func__, index, en);
}

static void coul_set_vbat_drop_th(struct batt_regs *batt_reg, unsigned int th)
{
	unsigned char reg_value;
	unsigned int n;

	/* Set vbat drop th */
	if (th % COUL_VBAT_DROP_TH_GAP == 0)
		n = (th - COUL_VBAT_DROP_VTH) / COUL_VBAT_DROP_TH_GAP;
	else
		n = (th - COUL_VBAT_DROP_VTH) / COUL_VBAT_DROP_TH_GAP + 1;
	if (n > 0x7)
		n = 0x7;

	reg_value = coul_reg_read(batt_reg->vbat_drop);
	set_reg_bit(reg_value, batt_reg->vbat_drop_th_shift, VBAT_TH_MASK_NUM, n);
	coul_reg_write(batt_reg->vbat_drop, reg_value);
}

static void coul_enable_vbat_drop(int batt_index, int en)
{
	struct coul_device_info *di = g_scharger_coul;
	if (di == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return;
	}

	if(is_batt_h(batt_index) && di->vbat_h_drop_en)
		coul_set_vbat_drop_enable(&di->h_batt, batt_index, en);
	else if (!is_batt_h(batt_index) && di->vbat_l_drop_en)
		coul_set_vbat_drop_enable(&di->l_batt, batt_index, en);
}

static void coul_set_vbat_drop_initial(struct device *dev,
	struct coul_device_info *di)
{
	int ret;

	ret = of_property_read_u32(dev->of_node, "vbat-l-drop-en", &di->vbat_l_drop_en);
	if (!ret) {
		ret = of_property_read_u32(dev->of_node, "vbat-l-drop-th", &di->vbat_l_drop_th);
		if (ret) {
			coul_hardware_err("%s coul vbat l drop dts err \n", __func__);
			di->vbat_l_drop_en = 0;
		}
	}
	ret = of_property_read_u32(dev->of_node, "vbat-h-drop-en", &di->vbat_h_drop_en);
	if (!ret) {
		ret = of_property_read_u32(dev->of_node, "vbat-h-drop-th", &di->vbat_h_drop_th);
		if (ret) {
			coul_hardware_err("%s coul vbat h drop dts err\n", __func__);
			di->vbat_h_drop_en = 0;
		}
	}

	if (di->vbat_l_drop_en) {
		coul_set_vbat_drop_enable(&di->l_batt,  di->batt_l_index, EN);
		coul_set_vbat_drop_th(&di->l_batt, di->vbat_l_drop_th);
	}
	if (di->vbat_h_drop_en) {
		coul_set_vbat_drop_enable(&di->h_batt,  di->batt_h_index, EN);
		coul_set_vbat_drop_th(&di->h_batt, di->vbat_h_drop_th);
	}
}

/* check whether coul is ok 0: success  1: fail */
static int coul_ic_check(void)
{
	u8 val;
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 1;
	}

	val = coul_reg_read(di->version0);
	if (val == VERSION_VAL) {
		return 0;
	} else {
		coul_hardware_err("%s fail, 0x%x\n", __func__, val);
		return 1;
	}
}

static void coul_show_key_reg(void)
{
}

static int coul_set_batt_mode(int mod)
{
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return 0;
	}

#ifdef CONFIG_SCHARGER_V700
	if (di->chip_version == CHIP_VERSION_V700)
		return bat_cnct_ctrl_test(mod);
#endif
	return 0;
}

static void coul_set_cali_param(int batt_index, struct coul_cali_params *params)
{
	struct coul_device_info *di = g_scharger_coul;

	if(!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	if(is_batt_h(batt_index))
		(void)memcpy_s(&di->batt_h_cali_params,
			sizeof(struct coul_cali_params), params, sizeof(*params));
	else
		(void)memcpy_s(&di->batt_l_cali_params,
			sizeof(struct coul_cali_params), params, sizeof(*params));

	coul_hardware_inf("%s batt_index %d, v_offset_a %d, v_offset_b %d, c_offset_a %d, c_offset_b %d\n",
		__func__, batt_index, params->v_offset_a, params->v_offset_b,
		params->c_offset_a, params->c_offset_b);
	coul_hardware_inf("%s batt_index %d, v_offset_series_a %d, v_offset_series_b %d, "
		"c_offset_series_a %d, c_offset_series_b %d\n",
		__func__, batt_index, params->v_offset_series_a, params->v_offset_series_b,
		params->c_offset_series_a, params->c_offset_series_b);
}

static void coul_set_default_cali_param(struct coul_device_info *di)
{
	di->batt_l_cali_params.v_offset_a = DEFAULT_V_OFF_A;
	di->batt_l_cali_params.v_offset_b = DEFAULT_V_OFF_B;
	di->batt_l_cali_params.c_offset_a = DEFAULT_C_OFF_A;
	di->batt_l_cali_params.c_offset_b = DEFAULT_C_OFF_B;

	di->batt_h_cali_params.v_offset_a = DEFAULT_V_OFF_A;
	di->batt_h_cali_params.v_offset_b = DEFAULT_V_OFF_B;
	di->batt_h_cali_params.c_offset_a = DEFAULT_C_OFF_A;
	di->batt_h_cali_params.c_offset_b = DEFAULT_C_OFF_B;

	di->batt_l_cali_params.v_offset_series_a = DEFAULT_V_OFF_A;
	di->batt_l_cali_params.v_offset_series_b = DEFAULT_V_OFF_B;
	di->batt_l_cali_params.c_offset_series_a = DEFAULT_C_OFF_A;
	di->batt_l_cali_params.c_offset_series_b = DEFAULT_C_OFF_B;

	di->batt_h_cali_params.v_offset_series_a = DEFAULT_V_OFF_A;
	di->batt_h_cali_params.v_offset_series_b = DEFAULT_V_OFF_B;
	di->batt_h_cali_params.c_offset_series_a = DEFAULT_C_OFF_A;
	di->batt_h_cali_params.c_offset_series_b = DEFAULT_C_OFF_B;
}

struct coul_device_ops scharger_coul_ops = {
	.show_key_reg = coul_show_key_reg,
	.calculate_cc_uah = coul_calculate_cc_uah,
	.save_cc_uah = coul_save_cc_uah,
	.get_fifo_depth = coul_get_fifo_depth,
	.get_fifo_avg_data = coul_get_fifo_avg_data,
	.is_battery_moved = coul_is_battery_moved,
	.set_battery_moved_magic_num = coul_set_battery_move_magic,
	.get_delta_rc_ignore_flag = coul_get_delta_rc_ignore_flag,
	.get_nv_read_flag = coul_get_nv_read_flag,
	.set_nv_save_flag = coul_set_nv_save_flag,
	.get_use_saved_ocv_flag = coul_get_use_saved_ocv_flag,
	.get_fcc_invalid_up_flag = coul_get_fcc_flag_reg,
	.save_fcc_invalid_up_flag = coul_save_fcc_flag_reg,
	.get_ocvreg = coul_get_ocvreg,
	.save_ocv = coul_save_ocv,
	.save_ocv_temp = coul_save_ocv_temp,
	.get_ocv_temp = coul_get_ocv_temp,
	.clear_ocv_temp = coul_clear_ocv_temp,
	.clear_ocv = coul_clear_ocv,
	.cali_adc = coul_hardware_cali_adc,
	.set_low_low_int_val = coul_set_low_vol_val,
	.get_abs_cc = coul_get_abs_cc,
	.get_coul_time = coul_get_coul_time,
	.clear_coul_time = coul_hardware_clear_coul_time,
	.clear_cc_register = coul_hardware_clear_cc_register,
	.get_battery_voltage_uv = coul_hardware_get_battery_voltage_uv,
	.get_battery_current_ua = coul_get_battery_current_ua,
	.get_battery_vol_uv_from_fifo = coul_get_battery_vol_uv_from_fifo,
	.get_battery_cur_ua_from_fifo = coul_get_battery_cur_ua_from_fifo,
	.get_battery_vol_uv_drop_from_fifo = coul_get_battery_vol_uv_drop_from_fifo,
	.get_coul_drop_th = coul_get_vbat_drop_th,
	.coul_drop_enable = coul_enable_vbat_drop,
	.get_offset_current_mod = coul_get_offset_current_mod,
	.get_offset_vol_mod = coul_get_offset_vol_mod,
	.set_offset_vol_mod = coul_set_offset_vol_mod,
	.get_ate_a = coul_get_ate_a,
	.get_ate_b = coul_get_ate_b,
	.enter_eco = coul_enter_eco,
	.exit_eco = coul_exit_eco,
	.calculate_eco_leak_uah = coul_calculate_eco_leak_uah,
	.irq_enable = coul_enable_irq,
	.irq_disable = coul_disable_irq,
	.cali_auto_off = coul_cancel_auto_cali,
	.get_ocv_level = coul_get_ocv_level,
	.save_ocv_level = coul_save_ocv_level,
	.get_drained_battery_flag = coul_get_drained_battery_flag,
	.clear_drained_battery_flag = coul_clear_drained_battery_flag,
	.set_i_in_event_gate = coul_set_i_in_event_gate,
	.set_i_out_event_gate = coul_set_i_out_event_gate,
	.set_bootocv_sample = coul_set_bootocv_sample,
	.convert_regval2uv = coul_hardware_convert_regval2uv,
	.convert_regval2ua = coul_hardware_convert_regval2ua,
	.convert_regval2temp = coul_hardware_convert_regval2temp,
	.convert_uv2regval = coul_convert_uv2regval,
	.convert_regval2uah = coul_hardware_convert_regval2uah,
	.convert_ocv_regval2ua = coul_convert_ocv_regval2ua,
	.convert_ocv_regval2uv = coul_convert_ocv_regval2uv,

	.get_bat_temp = coul_get_tbat,
	.get_chip_temp = coul_get_tdie,

	.set_dev_cali_param = coul_set_cali_param,
	.coul_dev_check = coul_ic_check,
	.set_batt_mode = coul_set_batt_mode,
};

static int get_batt_index(struct coul_device_info *di,
	const struct device_node *np, int *batt_index)
{
	int ret;

	ret = of_property_read_u32(np, "batt_index", (unsigned int *)batt_index);
	if (ret)
		coul_core_err("%s batt_index error %d\n", __func__, ret);

	coul_core_info("%s batt_index %d\n", __func__, *batt_index);
	return ret;
}

static int coul_core_algo_register(struct coul_device_info *di, struct device_node *root)
{
	struct device_node *child;
	int ret;
	ret = 0;

	for_each_child_of_node(root, child) {
		ret = coul_core_register(&scharger_coul_ops, child);
		if (ret) {
			coul_hardware_err("%s failed, node %s\n", __func__, child->name);
			return ret;
		} else {
			coul_hardware_inf("%s node %s coul core succ\n",
				__func__, child->name);
		}
	}
	return ret;
}

static int batt_index_cfg(struct device_node *np, struct coul_device_info *di)
{
	int ret;
	struct device_node *child = NULL;
	unsigned int batt_index = 0;
	unsigned int hrst_index = 0;

	di->batt_l_index = -1;
	di->batt_h_index = -1;
	for_each_child_of_node(np, child) {
		ret = of_property_read_u32(child, "batt_index", (unsigned int *)&batt_index);
		if (ret) {
			coul_core_err("%s batt_index error %d\n", __func__, ret);
			return ret;
		}
		coul_core_info("%s batt_index %d\n", __func__, batt_index);

		g_batt_num += 1;
		if (strstr(child->name, BATT_H_NODE) != NULL) {
			di->batt_h_index = batt_index;
		} else if (strstr(child->name, BATT_L_NODE) != NULL) {
			di->batt_l_index = batt_index;
		} else {
			coul_hardware_err("invalid child node name %s!\n", child->name);
			return -1;
		}

		ret = of_property_read_u32(child, "hrst_index", (unsigned int *)(&hrst_index));
		if (!ret)
			set_hrst_index(batt_index, hrst_index);
	}
	return 0;
}

static unsigned short scharger_get_device_version(void)
{
	unsigned short scharger_version = 0;

	struct coul_device_info *di = g_scharger_coul;

	if (di == NULL) {
		coul_hardware_err("[%s]di is null\n", __func__);
		return 0;
	}
	coul_regs_read(di->version4, (u8 *)&scharger_version, 2);

	coul_hardware_inf("%s, chip version is 0x%x\n", __func__, scharger_version);
	return scharger_version;
}

static int coul_hardware_probe(struct platform_device *pdev)
{
	struct coul_device_info *di = NULL;
	struct device_node *np = NULL;
	struct device *dev = &pdev->dev;
	int retval;

	coul_hardware_dbg("%s +\n", __func__);

	di = (struct coul_device_info *)devm_kzalloc(
		     &pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di) {
		coul_hardware_err("%s failed to alloc di struct\n", __func__);
		return -1;
	}
	di->dev = &pdev->dev;
	np = di->dev->of_node;
	if (np == NULL) {
		coul_hardware_err("%s np is null!\n", __func__);
		return -1;
	}

	di->irq = of_irq_get_byname(dev->of_node, "coul_irq");
	if (di->irq < 0) {
		coul_hardware_err("failed to get irq resource!\n");
		return -1;
	}

	di->regmap = dev_get_regmap(dev->parent, NULL);
	if (!di->regmap) {
		coul_hardware_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}

	retval = of_property_read_u32(dev->parent->of_node, "chip_version", &di->chip_version);
	if (retval) {
		coul_hardware_err("Parent chip_version invalid\n");
		return -1;
	}

	g_scharger_coul = di;
	dev_set_drvdata(&pdev->dev, di);

	retval = batt_index_cfg(np, di);
	if (retval)
		goto failed_0;

	/* config coul ctrl and irq */
#ifdef CONFIG_SCHARGER_V700
	if (di->chip_version == CHIP_VERSION_V700)
		coul_schargerv700_regs_cfg(di);
#endif
#ifdef CONFIG_SCHARGER_V800
	if (di->chip_version == CHIP_VERSION_V800)
		coul_schargerv800_regs_cfg(di);
#endif
	coul_set_vbat_drop_initial(dev, di);

	di->scharger_version = scharger_get_device_version();

	coul_set_default_cali_param(di);

	coul_chip_init(di);

	di->wake_lock = wakeup_source_register(dev, "scharger coul wakelock");
	INIT_DELAYED_WORK(&di->chip_cali_work, coul_hardware_cali_work);

	/* Init interrupt notifier work */
	INIT_WORK(&di->irq_work, coul_interrupt_notifier_work);

	if (di->chip_version == CHIP_VERSION_V700)
		retval = request_threaded_irq(di->irq, NULL, coul_irq_cb, IRQF_ONESHOT,
			"scharger_coul_irq", di);
	 else
	 	retval = request_irq(di->irq, coul_irq_cb, IRQF_NO_SUSPEND,
	 		"scharger_coul_irq", di);
	if (retval) {
		coul_hardware_err("Failed to request coul irq\n");
		goto failed_1;
	}

	/* set shutdown vol level */
	coul_set_low_vol_val(DEFAULT_BATTERY_VOL_0_PERCENT);

	coul_enable_irq(di->batt_l_index);
	if (di->batt_h_index >= 0)
		coul_enable_irq(di->batt_h_index);

	retval = of_property_read_u32(
		of_find_compatible_node(NULL, NULL, "hisilicon,coul_algo_core"),
		"r_coul_uohm", (u32 *)&g_r_coul_uohm);
	if (retval) {
		g_r_coul_uohm = R_COUL_UOHM;
		coul_hardware_err(
			"get r_coul_uohm fail, use default value 10000 uohm!\n");
	}
	coul_hardware_inf("%s r_coul_uohm %d\n", __func__, g_r_coul_uohm);

	retval = coul_core_algo_register(di, np);
	coul_hardware_inf("%s -\n", __func__);
	return retval;

failed_1:
	coul_disable_irq(di->batt_l_index);
	coul_disable_irq(di->batt_h_index);
	free_irq(di->irq, di);
failed_0:
	dev_set_drvdata(dev, NULL);
	kfree(di);
	g_scharger_coul = NULL;
	coul_hardware_err("coul hardware probe failed!\n");
	return retval;
}

static int coul_hardware_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id coul_match_table[] = {
	{
		.compatible = "hisilicon,scharger_coul",
	},
	{
	},
};

static struct platform_driver coul_driver = {
	.driver = {
		.name	= "scharger_coul",
		.owner  = THIS_MODULE,
		.of_match_table = coul_match_table,
	},
	.probe	= coul_hardware_probe,
	.remove	= coul_hardware_remove,
};

static int __init scharger_coul_init(void)
{
	return platform_driver_register(&coul_driver);
}

static void __exit scharger_coul_exit(void)
{
	platform_driver_unregister(&coul_driver);
}

fs_initcall(scharger_coul_init);
module_exit(scharger_coul_exit);

MODULE_DESCRIPTION("scharger coul driver");
MODULE_LICENSE("GPL v2");
