/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: coulometer hardware schargerv800 configure
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
#include "scharger_v800_regs.h"
#include "scharger_coul.h"

static void batt_h_regs_init(struct coul_device_info *di)
{
	struct batt_regs *batt_reg = &di->h_batt;

	batt_reg->offset_voltage = COUL_REG_H_OFFSET_VOLTAGE0_ADDR;
	batt_reg->cl_out = COUL_REG_H_CL_OUT_RO0_ADDR;
	batt_reg->cl_in = COUL_REG_H_CL_IN_RO0_ADDR;
	batt_reg->cl_in_time_ro = COUL_REG_H_CHG_TIMER_RO0_ADDR;
	batt_reg->cl_out_time_ro = COUL_REG_H_LOAD_TIMER_RO0_ADDR;
	batt_reg->cl_in_time_rw = COUL_REG_H_CHG_TIMER0_ADDR;
	batt_reg->cl_out_time_rw = COUL_REG_H_LOAD_TIMER0_ADDR;
	batt_reg->vol = COUL_REG_H_V_OUT_0_ADDR;
	batt_reg->cur = COUL_REG_H_CURRENT_0_ADDR;
	batt_reg->vol_fifo = COUL_REG_H_V_PRE0_OUT0_ADDR;
	batt_reg->cur_fifo = COUL_REG_H_CURRENT_PRE0_OUT0_ADDR;
	batt_reg->eco_out_clin = COUL_REG_H_ECO_OUT_CLIN_0_ADDR;
	batt_reg->eco_out_clout = COUL_REG_H_ECO_OUT_CLOUT_0_ADDR;
	batt_reg->irq_mask = COUL_REG_COUL_H_IRQ_MASK_ADDR;
	batt_reg->cl_out_rw = COUL_REG_H_CL_OUT0_ADDR;
	batt_reg->cl_in_rw = COUL_REG_H_CL_IN0_ADDR;
	batt_reg->irq_flag = COUL_REG_COUL_H_IRQ_FLAG_ADDR;

	batt_reg->vbat_drop_fifo = COUL_H_V_DROP_PRE0_OUT0;
	batt_reg->vbat_drop = COUL_V_DROP_CTRL0_ADDR;
	batt_reg->vbat_drop_en_shift = VBAT_H_DROP_EN_SHIFT;
	batt_reg->vbat_drop_th_shift = VBAT_H_DROP_TH_SHIFT;
}

static void batt_l_regs_init(struct coul_device_info *di)
{
	struct batt_regs *batt_reg = &di->l_batt;

	batt_reg->offset_voltage = COUL_REG_L_OFFSET_VOLTAGE0_ADDR;
	batt_reg->cl_out = COUL_REG_L_CL_OUT_RO0_ADDR;
	batt_reg->cl_in = COUL_REG_L_CL_IN_RO0_ADDR;
	batt_reg->cl_in_time_ro = COUL_REG_L_CHG_TIMER_RO0_ADDR;
	batt_reg->cl_out_time_ro = COUL_REG_L_LOAD_TIMER_RO0_ADDR;
	batt_reg->cl_in_time_rw = COUL_REG_L_CHG_TIMER0_ADDR;
	batt_reg->cl_out_time_rw = COUL_REG_L_LOAD_TIMER0_ADDR;
	batt_reg->vol = COUL_REG_L_V_OUT_0_ADDR;
	batt_reg->cur = COUL_REG_L_CURRENT_0_ADDR;
	batt_reg->vol_fifo = COUL_REG_L_V_PRE0_OUT0_ADDR;
	batt_reg->cur_fifo = COUL_REG_L_CURRENT_PRE0_OUT0_ADDR;
	batt_reg->eco_out_clin = COUL_REG_L_ECO_OUT_CLIN_0_ADDR;
	batt_reg->eco_out_clout = COUL_REG_L_ECO_OUT_CLOUT_0_ADDR;
	batt_reg->irq_mask = COUL_REG_COUL_L_IRQ_MASK_ADDR;
	batt_reg->irq_flag = COUL_REG_COUL_L_IRQ_FLAG_ADDR;
	batt_reg->cl_out_rw = COUL_REG_L_CL_OUT0_ADDR;
	batt_reg->cl_in_rw = COUL_REG_L_CL_IN0_ADDR;

	batt_reg->vbat_drop_fifo = COUL_L_V_DROP_PRE0_OUT0;
	batt_reg->vbat_drop = COUL_V_DROP_CTRL0_ADDR;
	batt_reg->vbat_drop_en_shift = VBAT_L_DROP_EN_SHIFT;
	batt_reg->vbat_drop_th_shift = VBAT_L_DROP_TH_SHIFT;
}

static void regs_val_init(struct coul_device_info *di)
{
	struct batt_reg_val *regs_val = &di->regs_val;

	regs_val->coul_cali_enable = COUL_CALI_ENABLE;
	regs_val->eco_coul_ctrl_val = ECO_COUL_CTRL_VAL;
	regs_val->default_coul_ctrl_val = DEFAULT_COUL_CTRL_VAL;
	regs_val->mask_cali_auto_off = MASK_CALI_AUTO_OFF;
	regs_val->wait_comp_en = WAIT_COMP_EN;
	regs_val->coul_clear_irq = COUL_CLEAR_IRQ;
	regs_val->coul_vbat_drop_mask = COUL_VBAT_DROP_MASK;
	regs_val->coul_i_out_mask = COUL_I_OUT_MASK;
	regs_val->coul_i_in_mask = COUL_I_IN_MASK;
	regs_val->coul_vbat_int_mask = COUL_VBAT_INT_MASK;
	regs_val->coul_cl_in_mask = COUL_CL_IN_MASK;
	regs_val->coul_cl_out_mask = COUL_CL_OUT_MASK;
	regs_val->coul_cl_int_mask = COUL_CL_INT_MASK;
	regs_val->coul_int_mask_all = COUL_INT_MASK_ALL;
	regs_val->eco_fifo_clear = ECO_FIFO_CLEAR;
	regs_val->eco_fifo_en = ECO_FIFO_EN;
	regs_val->eco_enter = ECO_ENTER;
	regs_val->eco_filter_out_en = ECO_FILTER_OUT_EN;
	regs_val->data_clear = DATA_CLEAR;
}

void coul_schargerv800_regs_cfg(struct coul_device_info *di)
{
	if (!di) {
		coul_hardware_err("%s di is null.\n", __func__);
		return;
	}

	di->wp.reg = COUL_REG_DEBUG_WRITE_PRO_ADDR;
	di->wp.lock_val = COUL_WRITE_LOCK;
	di->wp.unlock_val = COUL_WRITE_UNLOCK;
	di->bat_cnct_sel = COUL_REG_BAT_CNCT_SEL_ADDR;
	di->v_int = COUL_REG_V_INT0_ADDR;
	di->i_out_gate = COUL_REG_I_OUT_GATE0_ADDR;
	di->i_in_gate = COUL_REG_I_IN_GATE0_ADDR;
	di->clj_ctrl = COUL_REG_CLJ_CTRL_REG_ADDR;
	di->clj_ctrl1 = COUL_REG_CLJ_CTRL_REG1_ADDR;
	di->clj_ctrl2 = COUL_REG_CLJ_CTRL_REGS2_ADDR;
	di->clj_ctrl3 = COUL_REG_CLJ_CTRL_REGS3_ADDR;
	di->clj_ctrl4 = COUL_REG_CLJ_CTRL_REGS4_ADDR;
	di->coul_mstate = COUL_REG_STATE_TEST_ADDR;
	di->irq_flag = COUL_REG_COUL_IRQ_FLAG_ADDR;
	di->version0 = COUL_REG_CHIP_VERSION0_ADDR;
	di->version4 = COUL_REG_CHIP_VERSION4_ADDR;
	di->rd_req = COUL_RD_REQ_ADDR;
	batt_l_regs_init(di);
	batt_h_regs_init(di);
	regs_val_init(di);
	coul_hardware_inf("%s succ\n", __func__);
}

