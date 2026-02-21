/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: hisi_scharger_eis.h
 *
 * eis scharger driver header
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/bitops.h>
#include <linux/completion.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/regmap.h>
#include <linux/atomic.h>
#include <linux/time.h>
#include <uapi/linux/time.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>
#include "../hisi_eis_core_freq.h"
#include "hisi_scharger_eis.h"

int get_adc_flag(void);
void set_adc_flag(unsigned int val);

struct eis_scharger_device {
	int eis_support;
	int eis_time_support;
	int eis_wavelet_support;
	int eis_freq_support;
	unsigned int batt_l_index;
	unsigned int batt_h_index;
	struct device *dev;
	struct regmap *regmap;
};

/*
 * freq method real coefficient when vol gain is 1, if vol gain is N, then
 * each term in this list would miltiply 1 / N, magnified 100000 TIMES,
 * here N = 15
 */
static long long k_real[] = {
	-11618, -33083, -49518, -58407, -58407, -49518, -33083, -11618,
	11618, 33083, 49518, 58417, 58407, 49518, 33083, 11618
};
/*
 * freq method imag coefficient when vol gain is 1, if vol gain is N, then
 * each term in this list would miltiply 1 / N, magnified 100000 TIMES
 */
static long long k_imag[] = {
	-58407, -49518, -33083, -11618,  11618,  33083,  49518,  58407,
	58407, 49518, 33083, 11618, -11618, -33083, -49518, -58407
};

/* the gear of vol gain 12 */
static unsigned int v_bat_gain[] = { 1, 3, 4, 6, 9, 12, 15, 18 };

/* the gear of cur gain when external resistance is 5 mohm */
static unsigned int i_bat_gain[] = { 32, 64, 128, 256, 512 };
/* unit: mA 198 modified  to 0 */

/* unit:mS, do not change the array order */
static unsigned int _t_freq[] = {
	1, 2, 4, 5, 10, 16, 20, 50, 100, 200,
	500, 1000, 2000, 5000, 10000, 20000
};

static unsigned int i_bat_timer_k[] = { 0, 1, 2, 3, 4, 5, 6 };
static unsigned int i_bat_timer_j[] = { 1, 2, 3, 4 };

#define DEFAULT_C_OFF_A         1000000
#define DEFAULT_C_OFF_B         0

#define WAIT_ADC_TIME           100

static struct eis_scharger_device *g_eis_scharger_di;
static atomic_t g_work_flag = ATOMIC_INIT(0);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
typedef struct timespec64 timespec_64;
static void get_real_time_of_day(timespec_64 *tv)
{
	ktime_get_real_ts64(tv);
}
#else
typedef struct timespec timespec_64;
static void get_real_time_of_day(timespec_64 *tv)
{
	do_gettimeofday(tv);
}
#endif

static void eis_reg_write(unsigned int reg, u8 val)
{
	struct eis_scharger_device *di = g_eis_scharger_di;

	if (di == NULL || di->regmap == NULL) {
		eis_err("[%s]di is null\n");
		return;
	}

	(void)regmap_write(di->regmap, reg, (unsigned int)val);
}

static unsigned int eis_reg_read(unsigned int reg)
{
	struct eis_scharger_device *di = g_eis_scharger_di;
	unsigned int val = 0;

	if (di == NULL || di->regmap == NULL) {
		eis_err("[%s]di is null\n");
		return 0;
	}

	(void)regmap_read(di->regmap, reg, &val);
	return val;
}

static int locate_in_array(const unsigned int *array, int len, unsigned int val)
{
	int i;
	int index = -1;

	if (array == NULL || len <= 0)
		return -1;
	for (i = 0; i < len; i++) {
		if (array[i] == val) {
			index = i;
			break;
		}
	}
	return index;
}

/* return -1 means locate fail, others value means sucessed */
static int locate_in_2d_array(
	const unsigned int (*table)[RANGE_ELEMENT_COUNT],
	int tbl_len, long long val)
{
	int i;
	int index = -1;

	if (table == NULL || tbl_len < 1) {
		eis_err("table_len = %d, or null ptr!\n", tbl_len);
		return index;
	}
	for (i = tbl_len - 1; i >= 0; i--) {
		if (val > table[i][0] * DIGIT2ANA_MAG &&
			val <= table[i][1] * DIGIT2ANA_MAG) {
			index = i;
			break;
		}
	}
	eis_inf("index = %d!\n", index);
	return index;
}

static void eis_scharger_set_work_flag(int flag)
{
	atomic_set(&g_work_flag, flag);
}

static int eis_scharger_read_work_flag(void)
{
	return atomic_read(&g_work_flag);
}

static void eis_scharger_select_batt(unsigned int batt_index)
{
	u8 reg_val;

	reg_val = eis_reg_read(EIS_TOP_CFG_REG_0);
	if (batt_index == BATT_INDEX_0)
		eis_reg_setbits(reg_val, 2, 1, 0x0);
	else if (batt_index == BATT_INDEX_1)
		eis_reg_setbits(reg_val, 2, 1, 0x1);
	else
		eis_err("Error batt index = %d!\n", batt_index);

	eis_reg_write(EIS_TOP_CFG_REG_0, reg_val);
}

unsigned int eis_scharger_get_periods_order_now(void)
{
	return 0;
}

unsigned int eis_scharger_get_v_fifo_order_now(void)
{
	return 0;
}

unsigned int eis_scharger_get_v_bat_data(unsigned int order)
{
	u8 reg_val_l;
	u16 reg_val_h;
	int tmp_addr_l;
	int tmp_addr_h;
	unsigned int v_data;

	if ((order + 1) % V_I_BAT_GROUP_SIZE == 0) {
		eis_err("invalid order\n");
		return 0;
	}

	/*
	 * 2: the tmp_addr_l and tmp_addr_h are continuous with interval of 1;
	 * 1: minus 1 to convert fifo_order from [1, 16] to [0, 15]
	 */
	tmp_addr_l = (int)(EIS_VBAT_START_L + 2 * order);
	tmp_addr_h = (int)(EIS_VBAT_START_H + 2 * order);

	reg_val_l = eis_reg_read(tmp_addr_l);
	/* 0, 7: get the reg value of <7:0> */
	reg_val_l = eis_reg_getbits(reg_val_l, 0, 7);
	reg_val_h = eis_reg_read(tmp_addr_h);
	/* the current overflow status is in <4>, <3:0> is pure voltage data */
	reg_val_h = eis_reg_getbits(reg_val_h, 0, 4);
	v_data = ((reg_val_h << V_BAT_H_SHIFT) | reg_val_l);
	eis_debug("v_data = %u\n", v_data);

	return v_data;
}

unsigned int eis_scharger_get_i_bat_data(unsigned int order)
{
	u8 reg_val_l;
	u16 reg_val_h;
	int tmp_addr_l;
	int tmp_addr_h;
	unsigned int i_data;

	if ((order + 1) % V_I_BAT_GROUP_SIZE > 0) {
		eis_err("invalid order\n");
		return 0;
	}

	/*
	 * 2: the tmp_addr_l and tmp_addr_h are continuous with interval of 1;
	 * 1: minus 1 to convert fifo_order from [1, 4] to [0, 3]
	 */
	tmp_addr_l = (int)(EIS_VBAT_START_L + 2 * order);
	tmp_addr_h = (int)(EIS_VBAT_START_H + 2 * order);

	reg_val_l = eis_reg_read(tmp_addr_l);
	/* 0, 7: get the reg value of <7:0> */
	reg_val_l = eis_reg_getbits(reg_val_l, 0, 7);
	reg_val_h = eis_reg_read(tmp_addr_h);
	/* 0, 3: get the reg value of <3:0> */
	reg_val_h = eis_reg_getbits(reg_val_h, 0, 3);
	i_data = ((reg_val_h << I_BAT_H_SHIFT) | reg_val_l);
	eis_debug("i_data  = %u\n", i_data);
	return i_data;
}

_STATIC void eis_scharger_set_adc_eis_sel(unsigned int val)
{
	u8 reg_val;

	eis_reg_setbits(reg_val, 0, 1, val);
	eis_reg_write(ADC_EIS_SEL, reg_val);
}

_STATIC void eis_scharger_freq_set_bat_dischg_mode(
	enum eis_dischg_mode dischg_mode, enum eis_dischg_enable en)
{
	u8 reg_val = eis_reg_read(EIS_DISCHG_CFG);

	if (dischg_mode != EIS_HALF_EMPTY && dischg_mode != EIS_ANY_TIME) {
		eis_err("dischg_mode = %u,invalid!\n", dischg_mode);
		return;
	}
	if (en != EIS_DISCHG_DIS && en != EIS_DISCHG_EN) {
		eis_err("en = %u,invalid!\n", en);
		return;
	}
	/*
	 * 0: starting from <0>, 1: length including starting bit
	 * 1: starting from <1>, 1: length including starting bit
	 */
	eis_reg_setbits(reg_val, 0, 1, dischg_mode);
	eis_reg_setbits(reg_val, 1, 1, en);
	eis_reg_write(EIS_DISCHG_CFG, reg_val);

	eis_debug("eis freq mode, after set <1:0>, EIS_DISCHG_CFG = %u!\n",
		eis_reg_read(EIS_DISCHG_CFG));
}

static void eis_scharger_set_t_n_m(unsigned int t, unsigned int n, unsigned int m)
{
	u16 reg_val, reg_tmp;
	int index;

	if (m < 1)
		return; /* m: according to nManager, m at least 1 */

	index = locate_in_array(_t_freq, LEN_T_FREQ, t);
	if (index == -1) {
		eis_err("t = %u, invalid input!\n", t);
		return;
	}

	/* set <3:0> of EIS_DET_TIMER as t */
	reg_val = eis_reg_read(EIS_DET_TIMER);
	eis_reg_setbits(reg_val, 0, 4, (unsigned int)index);
	eis_reg_write(EIS_DET_TIMER, reg_val);

	reg_val = eis_reg_read(EIS_DET_TIMER);
	/* get <3:0> of EIS_DET_TIMER */
	reg_tmp = eis_reg_getbits(reg_val, 0, 3);
	eis_debug("EIS_DET_TIMER after set is: 0x%x\n", reg_tmp);

	/* set <3:0> of EIS_DET_TIMER_N as n */
	reg_val = eis_reg_read(EIS_DET_TIMER_N);
	eis_reg_setbits(reg_val, 0, 4, n);
	eis_reg_write(EIS_DET_TIMER_N, reg_val);

	/* get <3:0> of EIS_DET_TIMER_N */
	reg_val = eis_reg_read(EIS_DET_TIMER_N);
	reg_tmp = eis_reg_getbits(reg_val, 0, 3);
	eis_debug("EIS_DET_TIMER_N after set is: 0x%x\n", reg_tmp);

	/* set <7:0> of EIS_DET_TIMER_M_L as m lower part */
	reg_val = eis_reg_read(EIS_DET_TIMER_M_L);
	reg_val = eis_reg_getbits((m - 1), 0, 7);
	eis_reg_write(EIS_DET_TIMER_M_L, reg_val);

	/* get <7:0> of EIS_DET_TIMER_M_L */
	reg_tmp = eis_reg_read(EIS_DET_TIMER_M_L);
	reg_tmp = eis_reg_getbits(reg_tmp, 0, 7);
	/* set <1:0> of EIS_DET_TIMER_M_H as m higher part */
	reg_val = eis_reg_read(EIS_DET_TIMER_M_H);
	eis_reg_setbits(reg_val, 0, 2, eis_reg_getbits((m - 1), 8, 9));
	eis_reg_write(EIS_DET_TIMER_M_H, reg_val);

	/* get <1:0> of EIS_DET_TIMER_M_H */
	reg_val = eis_reg_read(EIS_DET_TIMER_M_H);
	reg_val = eis_reg_getbits(reg_val, 0, 1);
	reg_tmp = ((reg_val << BITS_PER_BYTE) | reg_tmp);
	eis_debug("EIS_DET_TIMER_M after set is: 0x%x\n", reg_tmp);

	/* set eis_dischg_cfg after tnm. */
	eis_scharger_freq_set_bat_dischg_mode(EIS_HALF_EMPTY, EIS_DISCHG_EN);
}

static void eis_scharger_get_t_n_m(unsigned int *t, unsigned int *n, unsigned int *m)
{
	unsigned int m_tmp;
	u8 reg_val;
	u8 reg_val_h;

	if (t == NULL || n == NULL || m == NULL) {
		eis_err("the input params are incorrect\n");
		return;
	}
	reg_val = eis_reg_read(EIS_DET_TIMER);
	/* get <3:0> of EIS_DET_TIMER */
	reg_val = eis_reg_getbits(reg_val, 0, 3);
	*t = _t_freq[reg_val];

	reg_val = eis_reg_read(EIS_DET_TIMER_N);
	/* get <3:0> of EIS_DET_TIMER_N */
	reg_val = eis_reg_getbits(reg_val, 0, 3);
	*n = reg_val;

	reg_val_h = eis_reg_read(EIS_DET_TIMER_M_H);
	reg_val_h = eis_reg_getbits(reg_val_h, 0, 1);
	reg_val = eis_reg_read(EIS_DET_TIMER_M_L);
	/* get <7:0> of EIS_DET_TIMER_L */
	reg_val = eis_reg_getbits(reg_val, 0, 7);
	m_tmp = reg_val_h;
	m_tmp = (m_tmp << V_BAT_H_SHIFT) | reg_val;
	*m = m_tmp;
}

static int eis_scharger_set_curt_sample_interval(unsigned int k, unsigned int j)
{
	u8 reg_val = eis_reg_read(EIS_DET_TIME_JK);
	int idx_k, idx_j;

	if (locate_in_array(i_bat_timer_k, LEN_I_BAT_TIMER_K, k) == -1
	    || locate_in_array(i_bat_timer_j, LEN_I_BAT_TIMER_J, j) == -1) {
		eis_err("input t, k or j is not eis allowed value\n");
		return _ERROR;
	}
	/*
	 * According to HI6526V700_nManager, both 0b111 and 0b110 stand for
	 * k is 6, so only the <2:1> is enough to describe state 6
	 */
	idx_k = locate_in_array(i_bat_timer_k, LEN_I_BAT_TIMER_K, k);
	idx_j = locate_in_array(i_bat_timer_j, LEN_I_BAT_TIMER_J, j);
	if (k == 6) {
		/*
		 * 1: starting from <1>, 2: length including starting bit
		 * 1, 2: <2:1> bit of idx_k
		 * 0, 2: <2:0> bit of reg_val
		 */
		eis_reg_setbits(reg_val, 1, 2, eis_reg_getbits(idx_k, 1, 2));
		eis_reg_write(EIS_DET_TIME_JK, reg_val);
		reg_val = eis_reg_read(EIS_DET_TIME_JK);
		reg_val = eis_reg_getbits(reg_val, 0, 2);
		eis_debug("k = %u, EIS_IBAT_DET_CTRL <2:0> after set is: 0x%x\n",
			 k, reg_val);
	} else {
		/*
		 * 0: starting from <0>, 3: length including starting bit
		 * 0, 2: <2:0> bit of reg_val
		 */
		eis_reg_setbits(reg_val, 0, 3, idx_k);
		eis_reg_write(EIS_DET_TIME_JK, reg_val);
		reg_val = eis_reg_read(EIS_DET_TIME_JK);
		reg_val = eis_reg_getbits(reg_val, 0, 2);
		eis_debug("k = %u, EIS_IBAT_DET_CTRL <2:0> after set is: 0x%x\n",
			 k, reg_val);
	}
	/*
	 * 3: starting from <3>, 2: length including starting bit
	 * 3, 4: <4:3> bit of reg_val
	 */
	reg_val = eis_reg_read(EIS_DET_TIME_JK);
	eis_reg_setbits(reg_val, 3, 2, idx_j);
	eis_reg_write(EIS_DET_TIME_JK, reg_val);

	reg_val = eis_reg_read(EIS_DET_TIME_JK);
	eis_debug("j = %u, EIS_DET_TIME_JK  after set is: 0x%x\n", j, reg_val);
	reg_val = eis_reg_getbits(reg_val, 3, 4);
	eis_debug("j = %u, EIS_DET_TIME_JK <4:3> after set is: 0x%x\n", j, reg_val);

	return _SUCC;
}

_STATIC void eis_charger_set_vbat_offset_0(void)
{
	/* 0~672.987mV, 1.317/step, EIS_TOP_CFG_REG_9 is low 8 bit  */
	eis_reg_write(EIS_VBAT_OFS_CFG_0, 0x5C);
}


_STATIC void eis_scharger_set_chopper_cur(void)
{
	u8 reg_val = eis_reg_read(EIS_TOP_CFG_REG_2);

	/* 5: starting from <5>, 3: length including starting bit, 0x100: 512x */
	eis_reg_setbits(reg_val, 4, 3, 0x4);
	eis_reg_write(EIS_TOP_CFG_REG_2, reg_val);
}

_STATIC void eis_scharger_set_chopper_vol(void)
{
	u8 reg_val = (u8)eis_reg_read(EIS_TOP_CFG_REG_9);

	/* 4: starting from <4>, 3: length including starting bit, 0x6: 15x */
	eis_reg_setbits(reg_val, 4, 3, 0x6);
	eis_reg_write(EIS_TOP_CFG_REG_9, reg_val);
}

_STATIC void eis_scharger_set_chopper_vcom(void)
{
	u8 reg_val = eis_reg_read(EIS_TOP_CFG_REG_7);

	/* 1: starting from <1>, 2: length including starting bit, 0x3: 1.6V */
	eis_reg_setbits(reg_val, 1, 2, 0x3);
	eis_reg_write(EIS_TOP_CFG_REG_7, reg_val);
}

_STATIC void eis_scharger_set_clk_sel(void)
{
	u8 reg_val = eis_reg_read(EIS_CLK_SEL);

	/* 0: starting from <1>, 1: length including starting bit, 0x1: inside clk */
	eis_reg_setbits(reg_val, 0, 1, 0x1);
	eis_reg_write(EIS_CLK_SEL, reg_val);
}

_STATIC int eis_scharger_get_r_uohm(void)
{
	u32 r_coul_uohm;
	int ret;

	ret = of_property_read_u32(of_find_compatible_node(NULL, NULL,
		"hisilicon,coul_algo_core"), "r_coul_mohm", &r_coul_uohm);
	if (ret != 0) {
		ret = of_property_read_u32(of_find_compatible_node(NULL, NULL,
			"hisilicon,coul_algo_core"), "r_coul_uohm", &r_coul_uohm);
		if (ret != 0) {
			r_coul_uohm = R_OHM_DEFAULT;
			eis_err("get r_coul_mohm/r_coul_uohm err,use default value 2000 uohm!\n");
		}
	} else {
		r_coul_uohm *= 1000;
	}
	eis_debug("r_coul_uohm = %u\n", r_coul_uohm);

	return (int)r_coul_uohm;
}

_STATIC int eis_scharger_get_ana_cur_offset(void)
{
	int ret;
	u8 reg_l = eis_reg_read(EISADC_INIT_I_OFFSET_L);
	u16 reg_h = eis_reg_read(EISADC_INIT_I_OFFSET_H);

	/* 0, 3: <3:0> bit of reg_val */
	reg_h = eis_reg_getbits(reg_h, 0, 3);
	ret = (int)((reg_h << BITS_PER_BYTE) | reg_l);
	eis_inf("cur_offset = %d\n", ret);

	return ret;
}

_STATIC void eis_scharger_set_v_bat_gain(unsigned int v_gain)
{
	u8 reg_val = eis_reg_read(EIS_TOP_CFG_REG_9);
	unsigned int v_gain_tmp;

	if (locate_in_array(v_bat_gain, LEN_V_BAT_GAIN, v_gain) == -1) {
		eis_err("v_gain = %u, invalid input\n", v_gain);
		return;
	}

	/*
	 * config the vol gain
	 * 3: starting from <4>, 3: length including starting bit
	 * 0, 2: <2:0> bit of reg_val
	 * 4, 6: <6:4> bit of reg_val
	 */
	v_gain_tmp = (unsigned int)locate_in_array(v_bat_gain, LEN_V_BAT_GAIN, v_gain);
	eis_reg_setbits(reg_val, 4, 3, eis_reg_getbits(v_gain_tmp, 0, 2));
	eis_reg_write(EIS_TOP_CFG_REG_9, reg_val);

	reg_val = eis_reg_read(EIS_TOP_CFG_REG_9);
	reg_val = eis_reg_getbits(reg_val, 4, 6);
	eis_debug("EIS_TOP_CFG_REG_9 <6:4> set to: 0x%x\n", reg_val);
}

_STATIC int eis_scharger_get_v_bat_gain(void)
{
	u8 reg_val;
	int v_gain;

	reg_val = eis_reg_read(EIS_TOP_CFG_REG_9);
	/* 4, 6: the <6:4> bit of reg_val */
	reg_val = eis_reg_getbits(reg_val, 4, 6);
	if (reg_val > LEN_V_BAT_GAIN - 1)
		reg_val = LEN_V_BAT_GAIN - 1;

	v_gain = (unsigned int)v_bat_gain[reg_val];
	eis_debug("EIS_TOP_CFG_REG_9 <6:4> val: 0x%x\n", reg_val);
	return v_gain;
}

_STATIC void eis_scharger_set_i_bat_gain(unsigned int i_gain)
{
	u8 reg_val = eis_reg_read(EIS_TOP_CFG_REG_2);
	unsigned int i_gain_tmp;

	if (locate_in_array(i_bat_gain, LEN_I_BAT_GAIN, i_gain) == -1) {
		eis_err("i_gain = %d, invalid input\n", i_gain);
		return;
	}

	/*
	 * config the cur gain
	 * 4: starting from <4>, 3: length including starting bit
	 * 6, 4: <6:4> bit of i_gain_tmp
	 */
	i_gain_tmp = (unsigned int)locate_in_array(i_bat_gain, LEN_I_BAT_GAIN, i_gain);
	eis_reg_setbits(reg_val, 4, 3, eis_reg_getbits(i_gain_tmp, 0, 2));
	eis_reg_write(EIS_TOP_CFG_REG_2, reg_val);

	reg_val = eis_reg_read(EIS_TOP_CFG_REG_2);
	reg_val = eis_reg_getbits(reg_val, 4, 6);
	eis_debug("EIS_TOP_CFG_REG_2 <6:4> set to: 0x%x\n", reg_val);
}

_STATIC unsigned int eis_scharger_get_i_bat_gain(void)
{
	u8 reg_val;
	unsigned int i_gain;
	/* 4, 6: <6:4> bit of reg_val */
	reg_val = eis_reg_read(EIS_TOP_CFG_REG_2);
	reg_val = eis_reg_getbits(reg_val, 4, 6);
	/* -1: if larger than 4, value is the same as 4, according to nManager */
	if (reg_val > LEN_I_BAT_GAIN - 1)
		reg_val = LEN_I_BAT_GAIN - 1;

	i_gain = i_bat_gain[reg_val];
	eis_debug("EIS_TOP_CFG_REG_2 <6:4> val: 0x%x\n", reg_val);
	return i_gain;
}

static void eis_scharger_set_thres_val(int low, int high, int low_err, int high_err)
{
	u8 reg_val;

	if (high_err > HIGH_ERR_MAX)
		high_err = HIGH_ERR_MAX;
	if (low_err > LOW_ERR_MAX)
		low_err = LOW_ERR_MAX;
	if (high > HIGH_MAX)
		high = HIGH_MAX;
	if (low > LOW_MAX)
		low = LOW_MAX;
	/*
	 * 0: starting from <0>, 8: length including starting bit
	 * 0, 7: <7:0> bit of high_err
	 */
	reg_val = eis_reg_read(EIS_HTHRESHOLD_ERROR);
	eis_reg_setbits(reg_val, 0, 8, eis_reg_getbits(high_err, 0, 7));
	eis_debug("high_err = %d!\n", reg_val);
	eis_reg_write(EIS_HTHRESHOLD_ERROR, reg_val);
	/*
	 * 0: starting from <0>, 8: length including starting bit
	 * 0, 7: <7:0> bit of low_err
	 */
	reg_val = eis_reg_read(EIS_LTHRESHOLD_ERROR);
	eis_reg_setbits(reg_val, 0, 8, eis_reg_getbits(low_err, 0, 7));
	eis_debug("low_err = %d!\n", reg_val);
	eis_reg_write(EIS_LTHRESHOLD_ERROR, reg_val);

	/*
	 * 0: starting from <0>, 8: length including starting bit
	 * 0, 7: <7:0> bit of high
	 */
	reg_val = eis_reg_read(EIS_ICOMP_HTHRESHOLD_L);
	eis_reg_setbits(reg_val, 0, 8, eis_reg_getbits(high, 0, 7));
	eis_debug("high<7:0> = %d!\n", reg_val);
	eis_reg_write(EIS_ICOMP_HTHRESHOLD_L, reg_val);
	/*
	 * 0: starting from <0>, 4: length including starting bit
	 * 8, 11: <11:8> bit of high
	 */
	reg_val = eis_reg_read(EIS_ICOMP_HTHRESHOLD_H);
	eis_reg_setbits(reg_val, 0, 4, eis_reg_getbits(high, 8, 11));
	eis_debug("high<11:8> = %d!\n", reg_val);
	eis_reg_write(EIS_ICOMP_HTHRESHOLD_H, reg_val);

	/*
	 * 0: starting from <0>, 8: length including starting bit
	 * 0, 7: <7:0> bit of low
	 */
	reg_val = eis_reg_read(EIS_ICOMP_LTHRESHOLD_L);
	eis_reg_setbits(reg_val, 0, 8, eis_reg_getbits(low, 0, 7));
	eis_debug("low<7:0> = %d!\n", reg_val);
	eis_reg_write(EIS_ICOMP_LTHRESHOLD_L, reg_val);
	/*
	 * 0: starting from <0>, 4: length including starting bit
	 * 8, 11: <11:8> bit of low
	 */
	reg_val = eis_reg_read(EIS_ICOMP_LTHRESHOLD_H);
	eis_reg_setbits(reg_val, 0, 4, eis_reg_getbits(low, 8, 11));
	eis_debug("low<11:8> = %d!\n", reg_val);
	eis_reg_write(EIS_ICOMP_LTHRESHOLD_H, reg_val);
}

_STATIC void get_calibrated_ana_cur(int batt_index, int cur_befo_ma, int *cur_after_ma)
{
	int offset_a = CALBRAT_A_DFT;
	int offset_b = 0;
	int ret;
	struct coul_batt_cali_info info = { 0 };

	info.batt_index = batt_index;
	info.data_type = BATT_DATA_CUR_CALI;
	info.a = DEFAULT_C_OFF_A;
	info.b = DEFAULT_C_OFF_B;

	ret = coul_merge_drv_get_cali(&info);
	if (ret != 0)
		eis_err("coulobmeter_cur_calibration err!\n");

	offset_a = info.a;
	offset_b = info.b;
	eis_debug("offset_a = %d, offset_b = %d!\n", offset_a, offset_b);
	if (offset_a == 0) {
		eis_err("offset_a = %d, set to 1000000!\n", offset_a);
		offset_a = CALBRAT_A_DFT;
	}
	*cur_after_ma = (cur_befo_ma - offset_b) * CALBRAT_A_DFT / offset_a;
	eis_debug("cur_after_ma = %d!\n", *cur_after_ma);
}

_STATIC void eis_scharger_freq_set_curt_sample_thres(int batt, int cur_offset)
{
	int low, high;
	int low_err, high_err;
	int ana2digit;
	int i_gain;
	int r_uohm;
	s64 temp;

	i_gain = (int)eis_scharger_get_i_bat_gain();
	r_uohm = eis_scharger_get_r_uohm();
	temp = (s64)i_gain * (s64)r_uohm / 1000 * CUR_TOTAL_STEP / A_TO_MA;
	ana2digit = (int)temp;

	get_calibrated_ana_cur(batt, CUR_FREQ_THRES_H, &high);
	get_calibrated_ana_cur(batt, CUR_FREQ_THRES_L, &low);
	eis_inf("high_new = %d, low_new = %d!\n", high, low);
	high = high * ana2digit / VOL_REF_MV + cur_offset;
	low = low  * ana2digit / VOL_REF_MV + cur_offset;
	eis_inf("high_final_digit = %d, low_final_digit = %d!\n", high, low);

	get_calibrated_ana_cur(batt, CUR_FREQ_THRES_H_ERR, &high_err);
	get_calibrated_ana_cur(batt, CUR_FREQ_THRES_L_ERR, &low_err);
	eis_inf("h_err_new = %d, l_err_new = %d!\n", high_err, low_err);

	high_err = high_err * ana2digit / VOL_REF_MV;
	low_err = low_err * ana2digit / VOL_REF_MV;
	eis_inf("high = %d, low = %d, high_err = %d, low_err = %d!\n",
		high, low, high_err, low_err);
	eis_scharger_set_thres_val(low, high, low_err, high_err);
}

_STATIC int eis_scharger_read_and_clear_irq(
	unsigned int irq_num, enum eis_irq_ops irq_ops)
{
	u8 reg_val;
	int bit_val;

	/* 5: only 0~5 are valid irq mask number */
	if (!(irq_num == EIS_IRQ_END || irq_num == EIS_IRQ_START)) {
		eis_err("mask number is invalid\n");
		return _ERROR;
	}

	reg_val = eis_reg_read(EIS_IRQ);
	switch (irq_ops) {
	case _READ:
		bit_val = (int)eis_reg_getbits(reg_val, irq_num, irq_num);
		break;
	case _CLEAR:
		eis_reg_write(EIS_IRQ, (1 << irq_num));
		reg_val = eis_reg_read(EIS_IRQ);
		bit_val = (int)eis_reg_getbits(reg_val, irq_num, irq_num);
		break;
	default:
		bit_val = -1;
		eis_inf("not read or clear, 0x%x <%u> = %d\n",
			EIS_IRQ, irq_num, bit_val);
		break;
	}
	return bit_val;
}

void eis_scharger_irq_seq_clear(enum eis_irq_range irq)
{
	int ret;
	unsigned int i;
	unsigned int start, end;

	switch (irq) {
	case IRQ_ALL:
	case IRQ_START_END:
		start = EIS_IRQ_START;
		end = EIS_IRQ_END;
		break;
	default:
		eis_err("no fifo!\n");
		return;
	}

	for (i = start; i >= end; i--) {
		ret = eis_scharger_read_and_clear_irq(i, _CLEAR);
		if (ret == -1)
			eis_err("read and clear irq ret invalid\n");
	}
}

static int eis_scharger_irq_seq_read(enum eis_irq_range irq)
{
	u8 reg_val;
	unsigned int start, end;

	switch (irq) {
	case IRQ_ALL:
	case IRQ_START_END:
		start = EIS_IRQ_END;
		end = EIS_IRQ_START;
		break;
	default:
		eis_err("no fifo!\n");
		return 0;
	}

	reg_val = eis_reg_read(EIS_IRQ);
	return (int)eis_reg_getbits(reg_val, start, end);
}

static int eis_scharger_get_eis_det_flag(void)
{
	u8 reg_val = eis_reg_read(EIS_DET_FLAG);
	/*
	 * eis_reg_getbits(x, n, m), x is the value, n: the starting bit,
	 * m: the end bit. 0, 0 means get the <0:0> bit of reg_val
	 */
	reg_val = eis_reg_getbits(reg_val, 0, 0);
	return (int)reg_val;
}

/* warning: DO NOT enable twice in a row. */
static void eis_scharger_eis_enable(bool enable)
{
	u8 reg_val;
	int flag;
	long long time_b, time_a, time_span;
	timespec_64 tv = { 0 };

	(void)eis_reg_read(ADC_EIS_SEL);

	/* eis adc and default adc are mutually exclusive. */
	if (enable == true) {
		get_real_time_of_day(&tv);
		time_a = (long long)(tv.tv_sec * S_TO_MS) + tv.tv_nsec /
			US_TO_NS / MS_TO_US;
		flag = get_adc_flag();
		while (flag == 1) {
			udelay(1);
			get_real_time_of_day(&tv);
			time_b = (long long)(tv.tv_sec * S_TO_MS) + tv.tv_nsec /
				US_TO_NS / MS_TO_US;

			time_span = time_a - time_b;
			if (time_span > WAIT_ADC_TIME) {
				eis_err("err, eis enable wait adc timeout!\n");
				return;
			}
			flag = get_adc_flag();
		}

		/* if eis work, schargerv700 adc will read fail and return default */
		set_adc_flag(1);
	}

	/* hkadc_sel */
	eis_scharger_set_adc_eis_sel(enable);

	/* eis enable */
	reg_val = eis_reg_read(EIS_EN);
	eis_reg_setbits(reg_val, 0, 1, enable ? 1 : 0);
	eis_reg_write(EIS_EN, reg_val);

	/* eis hkadc enable */
	reg_val = eis_reg_read(EIS_HKADC_START);
	eis_reg_setbits(reg_val, 0, 1, enable ? 1 : 0);
	eis_reg_write(EIS_HKADC_START, reg_val);

	if (enable == false)
		set_adc_flag(0);

	eis_inf("eis status is:%s\n", enable ? "enabled" : "disabled");
	(void)eis_reg_read(ADC_EIS_SEL);
}

/* this func is to get the <11:0> of vol data, as <12> is cur overflow flag */
static int eis_scharger_get_pure_vol_digit_data(
	const int *raw_sample_vol_data, int len_raw,
	int *pure_sample_vol_data, int len_pure)
{
	int i;
	int pure_data;

	if (raw_sample_vol_data == NULL || len_raw < 1 ||
		pure_sample_vol_data == NULL || len_pure < len_raw) {
		eis_err("invalid parameter\n");
		return _ERROR;
	}

	for (i = 0; i < len_raw; i++) {
		/*
		 * eis_reg_getbits(x, n, m), x is the value, n: the starting bit,
		 * m: the end bit. 0, 11 means get the <11:0> bit of reg_val
		 */
		pure_data = eis_reg_getbits(raw_sample_vol_data[i], 0, 11);
		pure_sample_vol_data[i] = pure_data;
	}
	return _SUCC;
}

static void eis_scharger_get_raw_eis_ri(long long *p_r, long long *p_i,
	int v_gain, const int *raw_sample_vol_data, unsigned int len)
{
	int j;
	long long r_t = 0;
	long long i_t = 0;
	int v_tmp[V_BAT_FIFO_DEPTH];

	if (p_r == NULL || p_i == NULL || raw_sample_vol_data == NULL
		|| len > V_BAT_FIFO_DEPTH || v_gain == 0) {
		eis_err("input null or error\n");
		return;
	}

	(void)eis_scharger_get_pure_vol_digit_data(
		raw_sample_vol_data, len, v_tmp, len);
	for (j = 0; j < len; j++) {
		r_t += k_real[j] / v_gain * v_tmp[j];
		i_t += k_imag[j] / v_gain * v_tmp[j];
	}
	*p_r = r_t;
	*p_i = i_t;
}

static int eis_scharger_get_freq_num(void)
{
	return LEN_FREQ;
}

_STATIC void eis_scharger_freq_start_sampling_init(unsigned int batt)
{
	int ret;

	/* select batt */
	eis_scharger_select_batt(batt);

	/* set vbat0 offset */
	eis_charger_set_vbat_offset_0();

	/* set chopper */
	eis_scharger_set_chopper_vol();
	eis_scharger_set_chopper_cur();

	/* select eis clk */
	eis_scharger_set_clk_sel();

	/* set vcm */
	eis_scharger_set_chopper_vcom();

	/* k = 1, j = 1t */
	ret = eis_scharger_set_curt_sample_interval(1, 1);
	if (ret != _SUCC)
		eis_err("scharger set curt sample interval error");
}

static int eis_scharger_count_over_thres_sample_data(
	const int *sample_vol_data, int len)
{
	int i;
	int cnt = 0;
	unsigned int cur_over_flag;

	if (sample_vol_data == NULL || len < 1) {
		eis_err("len = %d, or invalid null input ptr\n", len);
		return _ERROR;
	}

	for (i = 0; i < len; i++) {
		/*
		 * eis_reg_getbits(x, n, m), x is the value, n: the starting bit,
		 * m: the end bit. 12, 12 means get the <12:12> bit of reg_val
		 */
		cur_over_flag = eis_reg_getbits(sample_vol_data[i], 12, 12);
		if (cur_over_flag == CUR_OVERFLOW_FLAG)
			cnt += 1;
	}
	return cnt;
}

/* this func is to get the <11:0> of vol data, then convert to analog data */
static void eis_scharger_get_pure_vol_digit2ana_data(
	const int *raw_sample_vol_data, int len_raw,
	unsigned long *pure_sample_ana_vol_data, int len_pure)
{
	int i;
	int pure_data;
	long digit2ana;
	int v_gain;

	v_gain = eis_scharger_get_v_bat_gain();
	if (v_gain <= 0) {
		eis_err("v_gain = %d, invalid\n", v_gain);
		return;
	}
	digit2ana = DIGIT2ANA_MAG * VOL_REF_MV / (CUR_TOTAL_STEP * v_gain);
	eis_debug("v_gain = %d, digit2ana = %ld\n", v_gain, digit2ana);

	for (i = 0; i < len_raw; i++) {
		/*
		 * eis_reg_getbits(x, n, m), x is the value, n: the starting bit,
		 * m: the end bit. 0, 11 means get the <11:0> bit of reg_val
		 */
		pure_data = eis_reg_getbits(raw_sample_vol_data[i], 0, 11);
		pure_sample_ana_vol_data[i] = (unsigned long)(pure_data * digit2ana);
		eis_debug("%dth pure_data = %d, pure_sample_ana_vol_data = %lu\n",
			i, pure_data, pure_sample_ana_vol_data[i]);
	}
}

/* this func is to get the <12:0> of cur data, then convert to analog data */
static void eis_scharger_get_pure_cur_digit2ana_data(
	const int *raw_sample_cur_data, int len_raw,
	unsigned long *pure_sample_ana_cur_data, int len_pure)
{
	int i;
	int pure_data;
	long long digit2ana;
	int i_gain;
	int r_uohm;
	s64 temp;

	i_gain = (int)eis_scharger_get_i_bat_gain();
	r_uohm = eis_scharger_get_r_uohm();
	if (i_gain <= 0 || r_uohm <= 0) {
		eis_err("i_gain = %d, r_uohm = %d, invalid\n", i_gain, r_uohm);
		return;
	}

	temp = CUR_TOTAL_STEP * (s64)i_gain * (s64)r_uohm / 1000;
	digit2ana = DIGIT2ANA_MAG * VOL_REF_MV / (int)temp;

	for (i = 0; i < len_raw; i++) {
		/*
		 * eis_reg_getbits(x, n, m), x is the value, n: the starting bit,
		 * m: the end bit. 0, 11 means get the <11:0> bit of reg_val
		 */
		pure_data = eis_reg_getbits(raw_sample_cur_data[i], 0, 11);
		pure_sample_ana_cur_data[i] = (unsigned long)(pure_data * digit2ana);
	}
}

static int eis_scharger_freq_cur_gain_check_and_adjust(
	int *i_gain, int periods, int (*raw_sample_cur_data)[I_BAT_FIFO_DEPTH])
{
	int i;
	unsigned long pure_cur_ana[I_BAT_FIFO_DEPTH];
	int adjusted = I_GAIN_UNADJUSTED;
	int i_gain_min;

	if (raw_sample_cur_data == NULL || periods < 1 ||
		periods > FREQ_SAMP_PRDS_10 || i_gain == NULL) {
		eis_err("periods = %d, or null ptr, invalid input!\n",
			periods);
		return adjusted;
	}
	i_gain_min = (int)eis_scharger_get_i_bat_gain();
	for (i = 0; i < periods; i++)
		eis_scharger_get_pure_cur_digit2ana_data(
			&raw_sample_cur_data[i][0], I_BAT_FIFO_DEPTH,
			pure_cur_ana, I_BAT_FIFO_DEPTH);

	*i_gain = i_gain_min;
	eis_debug("print pure_cur_ana: %d, %d, %d, %d\n",
		pure_cur_ana[0], pure_cur_ana[1], pure_cur_ana[2], pure_cur_ana[3]);
	return adjusted;
}

static int eis_scharger_freq_vol_gain_check_and_adjust(
	int *v_gain, int periods, int (*raw_sample_vol_data)[V_BAT_FIFO_DEPTH])
{
	int i;
	unsigned long pure_vol_ana[V_BAT_FIFO_DEPTH];
	int adjusted = V_GAIN_UNADJUSTED;
	int v_gain_anchor;

	if (raw_sample_vol_data == NULL || periods < 1 ||
		periods > FREQ_SAMP_PRDS_10 || v_gain == NULL) {
		eis_err("periods = %d, or null ptr, invalid input!\n",
			periods);
		return adjusted;
	}
	v_gain_anchor = eis_scharger_get_v_bat_gain();
	for (i = 0; i < periods; i++)
		eis_scharger_get_pure_vol_digit2ana_data(
			&raw_sample_vol_data[i][0], V_BAT_FIFO_DEPTH,
			pure_vol_ana, V_BAT_FIFO_DEPTH);

	*v_gain = v_gain_anchor;
	return adjusted;
}

static void parse_scharger_freq_dts(struct eis_scharger_device *di)
{
	int ret;
	struct device_node *np = NULL;

	np = di->dev->of_node;
	ret = of_property_read_u32(np, "scharger_eis_freq_support",
		(u32 *)&di->eis_freq_support);
	if (ret) {
		eis_err("eis freq support dts not exist!\n");
		di->eis_freq_support = 0;
	}
	eis_inf("support = %d\n", di->eis_freq_support);

	ret = of_property_read_u32(np, "batt_l_index",
		(u32 *)&di->batt_l_index);
	if (ret) {
		eis_err("eis batt_l_index dts not exist!\n");
		di->batt_l_index = 1;
	}
	eis_inf("batt_l_index = %d\n", di->batt_l_index);

	ret = of_property_read_u32(np, "batt_h_index",
		(u32 *)&di->batt_h_index);
	if (ret) {
		eis_err("eis batt_h_index dts not exist!\n");
		di->batt_h_index = 0;
	}
	eis_inf("batt_h_index = %d\n", di->batt_h_index);
}

static struct eis_freq_device_ops hisi_scharger_freq_ops = {
	.locate_in_ary = locate_in_array,
	.get_v_bat_gain = eis_scharger_get_v_bat_gain,
	.chk_adjust_freq_i_gain = eis_scharger_freq_cur_gain_check_and_adjust,
	.chk_adjust_freq_v_gain = eis_scharger_freq_vol_gain_check_and_adjust,
	.eis_sampling_init = eis_scharger_freq_start_sampling_init,
	.set_v_gain = eis_scharger_set_v_bat_gain,
	.set_i_gain = eis_scharger_set_i_bat_gain,
	.set_cur_thres = eis_scharger_freq_set_curt_sample_thres,
	.set_work_flag = eis_scharger_set_work_flag,
	.read_work_flag = eis_scharger_read_work_flag,
	.get_raw_eis_ri = eis_scharger_get_raw_eis_ri,
	.get_freq_num = eis_scharger_get_freq_num,
};

static struct eis_device_ops hisi_eis_ops = {
	.get_fifo_period_now = eis_scharger_get_periods_order_now,
	.get_fifo_order_now = eis_scharger_get_v_fifo_order_now,
	.get_v_fifo_data = eis_scharger_get_v_bat_data,
	.get_i_fifo_data = eis_scharger_get_i_bat_data,
	.read_and_clear_irq = eis_scharger_read_and_clear_irq,
	.irq_seq_clear = eis_scharger_irq_seq_clear,
	.irq_seq_read = eis_scharger_irq_seq_read,
	.eis_detect_flag = eis_scharger_get_eis_det_flag,
	.set_t_n_m = eis_scharger_set_t_n_m,
	.get_t_n_m = eis_scharger_get_t_n_m,
	.eis_enable = eis_scharger_eis_enable,
	.cnt_overflow_num = eis_scharger_count_over_thres_sample_data,
	.cur_offset = eis_scharger_get_ana_cur_offset,
};

static int eis_scharger_freq_chip_init(struct eis_scharger_device *di)
{
	int ret;

	parse_scharger_freq_dts(di);

	ret = eis_core_drv_ops_register(&hisi_scharger_freq_ops, FREQ_DRV_OPS, di->batt_l_index);
	if (ret != _SUCC) {
		eis_err("scharger freq batt_l init fail!\n");
		return _ERROR;
	}

	ret = eis_core_drv_ops_register(&hisi_scharger_freq_ops, FREQ_DRV_OPS, di->batt_h_index);
	if (ret != _SUCC) {
		eis_err("scharger freq batt_h init fail!\n");
		return _ERROR;
	}

	eis_inf("freq init success!\n");
	return _SUCC;
}

static int hisi_eis_core_chip_init(struct eis_scharger_device *di)
{
	int ret;

	ret = eis_core_drv_ops_register(&hisi_eis_ops, EIS_OPS, di->batt_l_index);
	if (ret != _SUCC) {
		eis_err("eis core init fail!\n");
		return _ERROR;
	}

	ret = eis_core_drv_ops_register(&hisi_eis_ops, EIS_OPS, di->batt_h_index);
	if (ret != _SUCC) {
		eis_err("eis core init fail!\n");
		return _ERROR;
	}

	eis_inf("eis core init success!\n");
	return _SUCC;
}

static int eis_scharger_probe(struct platform_device *pdev)
{
	int ret;
	struct eis_scharger_device *di = NULL;
	struct device_node *np = NULL;

	di = (struct eis_scharger_device *)devm_kzalloc(&pdev->dev, sizeof(*di),
		GFP_KERNEL);
	if (di == NULL) {
		eis_err("failed to alloc *di!\n");
		return -ENOMEM;
	}
	di->dev = &pdev->dev;
	np = di->dev->of_node;
	if (np == NULL) {
		eis_err("np is null!\n");
		return -1;
	}

	di->regmap = dev_get_regmap(di->dev->parent, NULL);
	if (!di->regmap) {
		eis_err("Parent regmap unavailable.\n");
		return -1;
	}

	dev_set_drvdata(&pdev->dev, di);

	ret = eis_scharger_freq_chip_init(di);
	if (ret == _ERROR)
		goto eis_probe_err;
	ret = hisi_eis_core_chip_init(di);
	if (ret == _ERROR)
		goto eis_probe_err;
	g_eis_scharger_di = di;
	eis_inf("success!\n");
	return 0;

eis_probe_err:
	dev_set_drvdata(&pdev->dev, NULL);
	(void)eis_core_drv_ops_register(NULL, FREQ_DRV_OPS, di->batt_h_index);
	(void)eis_core_drv_ops_register(NULL, FREQ_DRV_OPS, di->batt_l_index);
	(void)eis_core_drv_ops_register(NULL, EIS_OPS, di->batt_h_index);
	(void)eis_core_drv_ops_register(NULL, EIS_OPS, di->batt_l_index);
	eis_err("fail!\n");
	return -ENODEV;
}

static int eis_scharger_remove(struct platform_device *pdev)
{
	struct eis_scharger_device *di;

	di = g_eis_scharger_di;
	if (di == NULL) {
		eis_err("di is null!\n");
		return -ENODEV;
	}
	dev_set_drvdata(&pdev->dev, NULL);
	(void)eis_core_drv_ops_register(NULL, FREQ_DRV_OPS, di->batt_h_index);
	(void)eis_core_drv_ops_register(NULL, FREQ_DRV_OPS, di->batt_l_index);
	(void)eis_core_drv_ops_register(NULL, EIS_OPS, di->batt_h_index);
	(void)eis_core_drv_ops_register(NULL, EIS_OPS, di->batt_l_index);
	devm_kfree(&pdev->dev, di);

	return 0;
}

static const struct of_device_id eis_scharger_match_table[] = {
	{
		.compatible = "hisilicon,scharger_eis",
	},
	{
	},
};

static struct platform_driver hisi_eis_scharger_driver = {
	.probe    = eis_scharger_probe,
	.remove   = eis_scharger_remove,
	.driver   = {
		.name           = "schargerV700_eis",
		.owner          = THIS_MODULE,
		.of_match_table = eis_scharger_match_table,
	},
};

int __init eis_scharger_init(void)
{
	return platform_driver_register(&hisi_eis_scharger_driver);
}

void __exit eis_scharger_exit(void)
{
	platform_driver_unregister(&hisi_eis_scharger_driver);
}

module_init(eis_scharger_init);
module_exit(eis_scharger_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("eis scharger driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
