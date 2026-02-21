/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: pmic hrst regs functions
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
 
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include "../coul_algo/coul_nv.h"
#include "pmic_hrst.h"

#define reg_spmi_read(regAddr) \
					pmic_read_reg(regAddr)
#define reg_spmi_write(regAddr, regval) \
					pmic_write_reg((int)(regAddr), (int)(regval))
#define reg_spmi_bulk_read(regAddr, buf, size) \
					pmic_array_read((int)(regAddr), (char *)(buf), (int)(size))
#define reg_spmi_bulk_write(regAddr, buf, size) \
					pmic_array_write((int)(regAddr), (char *)(buf), (int)(size))

#define set_reg_bit(reg_val, shift, mask, set_val) \
				(((reg_val)  = ((reg_val) & (~(((1U << (mask)) - 1) << (shift)))) \
				| ((unsigned int)((set_val) & ((1U << (mask)) - 1)) << (shift))))

#define coul_hardware_err(fmt, args...) \
	printk(KERN_ERR    "[pmic_hrst]" fmt, ## args)
#define coul_hardware_evt(fmt, args...) \
	printk(KERN_WARNING"[pmic_hrst]" fmt, ## args)
#define coul_hardware_inf(fmt, args...) \
	printk(KERN_INFO   "[pmic_hrst]" fmt, ## args)
#define coul_hardware_dbg(fmt, args...) do {} while (0)

static int g_hrst_index[BATT_MAX] = { 0, 1, 2 }; /* batt_index, hrst_index */

void set_hrst_index(int batt_index, unsigned int hrst_index)
{
	if (batt_index >= BATT_MAX || batt_index < 0 || hrst_index >= BATT_MAX) {
		coul_hardware_err("%s error, batt index=%d, hrst_index=%d\n",
			__func__, batt_index, hrst_index);
		return;
	}
	g_hrst_index[batt_index] = hrst_index;
}

static int get_hrst_index(int batt_index)
{
	if (batt_index < 0 || batt_index >= BATT_MAX) {
		coul_hardware_err("%s error, batt index=%d\n", __func__, batt_index);
		return -1;
	}
	return g_hrst_index[batt_index];
}

/*
 * set coul nv save success flag
 * Input: nv_flag: success 1, fail 0
 * Remark: the flag is read by fastboot
 */
void coul_set_nv_save_flag(int batt_index, int nv_flag)
{
	unsigned char val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_NV_SAVE_SUCCESS_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_NV_SAVE_SUCCESS_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_NV_SAVE_SUCCESS_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	val = reg_spmi_read(reg);
	if (nv_flag == NV_SAVE_SUCCESS)
		reg_spmi_write(reg, (val | NV_SAVE_BITMASK));
	else
		reg_spmi_write(reg, (val & (~NV_SAVE_BITMASK)));
}

/*
 * Description: get coul nv read success flag
 * Return:  success:1 fail:0
 * Remark:  the flag is written by fastboot
 */
int coul_get_nv_read_flag(int batt_index)
{
	unsigned char val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_NV_READ_SUCCESS_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_NV_READ_SUCCESS_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_NV_READ_SUCCESS_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return NV_READ_FAIL;
	}

	val = reg_spmi_read(reg);
	if (val & NV_READ_BITMASK)
		return NV_READ_SUCCESS;
	return NV_READ_FAIL;
}

/*
 * Return: 1:saved ocv, 0:not saved ocv
 * Remark:  the flag is written by fastboot
 */
int coul_get_use_saved_ocv_flag(int batt_index)
{
	unsigned char val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_OCV_CHOOSE_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_OCV_CHOOSE_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_OCV_CHOOSE_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}

	val = reg_spmi_read(reg);
	if (val & USE_SAVED_OCV_FLAG)
		return 1;
	return 0;
}

/*
 * Return: 1:not calculate delta rc,  0:calculate delta rc
 * Remark: the flag is written by fastboot
 */
int coul_get_delta_rc_ignore_flag(int batt_index)
{
	unsigned char use_delta_rc_flag;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_DELTA_RC_SCENE_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_DELTA_RC_SCENE_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_DELTA_RC_SCENE_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}

	use_delta_rc_flag = reg_spmi_read(reg);
	if (use_delta_rc_flag & DELTA_RC_SCENE_BITMASK) {
		use_delta_rc_flag &=
			~DELTA_RC_SCENE_BITMASK; /* clear the flag after read */
		reg_spmi_write(reg, use_delta_rc_flag);
		return 1;
	}
	return 0;
}

/* Return: 0:not moved, 1: moved */
int coul_is_battery_moved(int batt_index)
{
	unsigned char val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 1;
	}

	val = reg_spmi_read(reg);
	if (val == BATTERY_MOVE_MAGIC_NUM) {
		coul_hardware_inf("Battey not moved\n");
		return 0;
	}
	coul_hardware_inf("Battey moved\n");
	reg_spmi_write(reg, BATTERY_MOVE_MAGIC_NUM);
	return 1;
}

/*
 * Description: set battery move magic num
 * Input: move flag 1:plug out 0:plug in
 */
void coul_set_battery_move_magic(int batt_index, int move_flag)
{
	unsigned char val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_BATTERY_MOVE_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	if (move_flag)
		val = BATTERY_PLUGOUT_SHUTDOWN_MAGIC_NUM;
	else
		val = BATTERY_MOVE_MAGIC_NUM;
	reg_spmi_write(reg, val);
}

void coul_save_ocv_level(int batt_index, u8 level)
{
	u8 val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	val = reg_spmi_read(reg);
	val &= ~SAVE_OCV_LEVEL;
	val |= ((level << OCV_LEVEL_SHIFT) & SAVE_OCV_LEVEL);
	reg_spmi_write(reg, val);
}

void coul_get_ocv_level(int batt_index, u8 *level)
{
	u8 val;
	unsigned int reg;

	if (level == NULL) {
		coul_hardware_err("%s ptr null!\n", __func__);
		return;
	}

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_OCV_LEVEL_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	val = reg_spmi_read(reg);
	val &= SAVE_OCV_LEVEL;
	*level = val >> OCV_LEVEL_SHIFT;
}

int coul_get_drained_battery_flag(int batt_index)
{
	u8 val = 0;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = DRAINED_BATTERY_FLAG_ADDR_0;
		break;
	case BATT_1:
		reg = DRAINED_BATTERY_FLAG_ADDR_1;
		break;
	case BATT_2:
		reg = DRAINED_BATTERY_FLAG_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}

	val = reg_spmi_read(reg);
	coul_hardware_err("%s get reg value %d!!!\n", __func__, val);
	val &= DRAINED_BATTERY_FLAG_BIT;
	return val;
}

void coul_clear_drained_battery_flag(int batt_index)
{
	u8 val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = DRAINED_BATTERY_FLAG_ADDR_0;
		break;
	case BATT_1:
		reg = DRAINED_BATTERY_FLAG_ADDR_1;
		break;
	case BATT_2:
		reg = DRAINED_BATTERY_FLAG_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	val = reg_spmi_read(reg);
	reg_spmi_write(reg, val & (~DRAINED_BATTERY_FLAG_BIT));
	val = reg_spmi_read(reg);
	coul_hardware_err("%s after clear reg value %d!!!\n", __func__, val);
}

void coul_set_bootocv_sample(int batt_index, u8 set_val)
{
	u8 val;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_BOOT_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_BOOT_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_BOOT_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	val = reg_spmi_read(reg);
	if (set_val)
		val |= EN_BOOT_OCV_SAMPLE;
	else
		val &= (~EN_BOOT_OCV_SAMPLE);
	reg_spmi_write((reg), val);
	val = reg_spmi_read((reg));
	coul_hardware_err("%s set_bootocv:%d!!!\n", __func__, val);
}

/*
 * Description: coul save ocv
 */
void coul_save_ocvreg(int batt_index, unsigned short ocvreg)
{
	unsigned int reg;
	unsigned short regval = 0;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}
	reg_spmi_bulk_read(reg, &regval, 2);
	set_reg_bit(regval, 0, INVALID_TO_UPDATE_FCC_SHIFT, ocvreg);
	reg_spmi_bulk_write(reg, &regval, 2);
	/* save ocv type */
}

void coul_save_ocvtype(int batt_index, unsigned int ocv_type)
{
	unsigned int reg;
	unsigned short regval = 0;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_OCV_TYPE_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_OCV_TYPE_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_OCV_TYPE_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}
	reg_spmi_bulk_read(reg, &regval, 1);
	if (ocv_type == OCV_CHG)
		regval |= OCV_TYPE_MASK; /* set to 1 */
	else
		regval &= ~OCV_TYPE_MASK; /* set to 0 */
	reg_spmi_bulk_write(reg, &regval, 1);
}

/*
 * Description: coul save fcc flag to hrst reg
 * invalid_fcc_up_flag: no update fcc 1, update 0
 */
void coul_save_fcc_flag_reg(int batt_index, int invalid_fcc_up_flag)
{
	unsigned int reg;
	unsigned short regval = 0;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}

	reg_spmi_bulk_read(reg, &regval, 2);
	if (invalid_fcc_up_flag)
		set_reg_bit(regval, INVALID_TO_UPDATE_FCC_SHIFT, 1, 1);
	else
		set_reg_bit(regval, INVALID_TO_UPDATE_FCC_SHIFT, 1, 0);
	reg_spmi_bulk_write(reg, &regval, 2);
}

/*
 * Description: coul get fcc flag to hrst reg
 * return: no update fcc 1, update 0
 */
int coul_get_fcc_flag_reg(int batt_index)
{
	int invalid_fcc_up_flag;
	unsigned int reg;
	unsigned short regval = 0;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 1;
	}

	reg_spmi_bulk_read(reg, &regval, 2);
	if (regval & (1 << INVALID_TO_UPDATE_FCC_SHIFT))
		return 1;
	else
		return 0;
}


/*
  For test case can "ecall scharger_coul_clear_ocv",
  this func not set STATIC.
*/
void coul_clear_ocv(int batt_index)
{
	unsigned short ocvreg = 0;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}
	reg_spmi_bulk_write(reg, &ocvreg, 2); /* 2: regs_num */
}

/*
 * Description: get saved ocv regval
 * Return: OCV(reg value)
 */
short coul_get_ocvreg(int batt_index)
{
	unsigned short ocvreg = 0;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}
	reg_spmi_bulk_read(reg, &ocvreg, 2); /* 2: regs_num */
	return (short)ocvreg;
}

/*
 * Description: coul save ocv temp
 * Input: ocv_temp: temp*10
 */
void coul_save_ocv_temp(int batt_index, short ocv_temp)
{
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}
	reg_spmi_bulk_write(reg, &ocv_temp, 2); /* 2: regs_num */
	coul_hardware_inf("batt_index %d, save ocv temp, ocv_temp=%d\n", batt_index, ocv_temp);
}

void coul_clear_ocv_temp(int batt_index)
{
	short ocv_temp = 0;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return;
	}
	reg_spmi_bulk_write(reg, &ocv_temp, 2); /* 2: regs_num */
}

/*
 * Description: get saved ocv temp
 * Return: OCV temp(temp*10)
 */
short coul_get_ocv_temp(int batt_index)
{
	short ocv_temp = 0;
	unsigned int reg;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}
	reg_spmi_bulk_read(reg, &ocv_temp, 2); /* 2: regs_num */
	return ocv_temp;
}

/*
 * Description: get saved ocv temp
 * Input: ocv_type. 1:chg ocv. 0:dis chg ocv
 */
unsigned int coul_get_ocv_type(int batt_index)
{
	unsigned int reg;
	unsigned int regval = 0;

	switch(get_hrst_index(batt_index)) {
	case BATT_0:
		reg = COUL_HARDWARE_OCV_TYPE_0;
		break;
	case BATT_1:
		reg = COUL_HARDWARE_OCV_TYPE_1;
		break;
	case BATT_2:
		reg = COUL_HARDWARE_OCV_TYPE_2;
		break;
	default:
		coul_hardware_err("%s error batt index=%d\n", __func__, batt_index);
		return 0;
	}

	reg_spmi_bulk_read(reg, &regval, 2);
	if ((regval & OCV_TYPE_MASK) == OCV_TYPE_MASK) {
		return OCV_CHG;
	} else {
		return OCV_DISCHG;
	}
}

void coul_hardware_get_last_soc(short *soc)
{
	unsigned short val;
	if (soc == NULL)
		return;

	val = reg_spmi_read((int)COUL_HARDWARE_SAVE_LAST_SOC);
	*soc = (short)(val & SAVE_LAST_SOC);
	return;
}
