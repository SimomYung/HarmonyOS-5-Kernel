/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lcdkit parse spr function for lcd driver
 * Author: weiyuantian
 * Create: 2023-05-17
 */

#include "lcd_kit_utils.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_parse_spr_lut.h"

static void lcd_kit_spr_border_rlr_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-r-borderlr-value",
		&pinfo->spr.spr_r_borderlr.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-g-borderlr-value",
		&pinfo->spr.spr_g_borderlr.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-b-borderlr-value",
		&pinfo->spr.spr_b_borderlr.value, 0);
}

static void lcd_kit_spr_border_rlb_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-r-bordertb-value",
		&pinfo->spr.spr_r_bordertb.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-g-bordertb-value",
		&pinfo->spr.spr_g_bordertb.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-b-bordertb-value",
		&pinfo->spr.spr_b_bordertb.value, 0);
}

static void lcd_kit_spr_core_border_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_spr_border_rlr_set(panel_id, np, pinfo);
	lcd_kit_spr_border_rlb_set(panel_id, np, pinfo);

	lcd_kit_parse_u32(np, "lcd-kit,spr-pixgain-reg-value",
		&pinfo->spr.spr_pixgain_reg.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-pixgain-reg1-value",
		&pinfo->spr.spr_pixgain_reg1.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-P0-border-value",
		&pinfo->spr.spr_border_p0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-P1-border-value",
		&pinfo->spr.spr_border_p1.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-P2-border-value",
		&pinfo->spr.spr_border_p2.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-P3-border-value",
		&pinfo->spr.spr_border_p3.value, 0);
}

static void lcd_kit_spr_larea_border_rgb_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-border-r-value",
		&pinfo->spr.spr_larea_border_r.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-border-g-value",
		&pinfo->spr.spr_larea_border_g.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-border-b-value",
		&pinfo->spr.spr_larea_border_b.value, 0);
}

static void lcd_kit_spr_core_larea_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-start-value",
		&pinfo->spr.spr_larea_start.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-end-value",
		&pinfo->spr.spr_larea_end.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-offset-value",
		&pinfo->spr.spr_larea_offset.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-larea-gain-value",
		&pinfo->spr.spr_larea_gain.value, 0);

	lcd_kit_spr_larea_border_rgb_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_coeffs_b_v0h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-b0-value",
		&pinfo->spr.spr_coeff_v0h0_b0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-b1-value",
		&pinfo->spr.spr_coeff_v0h0_b1.value, 0);
}

static void lcd_kit_spr_coeffs_b_v0h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-b0-value",
		&pinfo->spr.spr_coeff_v0h1_b0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-b1-value",
		&pinfo->spr.spr_coeff_v0h1_b1.value, 0);
}

static void lcd_kit_spr_coeffs_b_v1h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-b0-value",
		&pinfo->spr.spr_coeff_v1h0_b0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-b1-value",
		&pinfo->spr.spr_coeff_v1h0_b1.value, 0);
}

static void lcd_kit_spr_coeffs_b_v1h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-b0-value",
		&pinfo->spr.spr_coeff_v1h1_b0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-b1-value",
		&pinfo->spr.spr_coeff_v1h1_b1.value, 0);
}

static void lcd_kit_spr_core_coeffs_b_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_spr_coeffs_b_v0h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_b_v0h1_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_b_v1h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_b_v1h1_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_coeffs_r_v0h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-r0.value",
		&pinfo->spr.spr_coeff_v0h0_r0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-r1-value",
		&pinfo->spr.spr_coeff_v0h0_r1.value, 0);
}

static void lcd_kit_spr_coeffs_r_v0h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-r0-value",
		&pinfo->spr.spr_coeff_v0h1_r0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-r1-value",
		&pinfo->spr.spr_coeff_v0h1_r1.value, 0);
}

static void lcd_kit_spr_coeffs_r_v1h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-r0-value",
		&pinfo->spr.spr_coeff_v1h0_r0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-r1-value",
		&pinfo->spr.spr_coeff_v1h0_r1.value, 0);
}

static void lcd_kit_spr_coeffs_r_v1h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-r0-value",
		&pinfo->spr.spr_coeff_v1h1_r0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-r1-value",
		&pinfo->spr.spr_coeff_v1h1_r1.value, 0);
}

static void lcd_kit_spr_core_coeffs_r_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_spr_coeffs_r_v0h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_r_v0h1_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_r_v1h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_r_v1h1_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_coeffs_g_v0h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-g0-value",
		&pinfo->spr.spr_coeff_v0h0_g0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h0-g1-value",
		&pinfo->spr.spr_coeff_v0h0_g1.value, 0);
}

static void lcd_kit_spr_coeffs_g_v0h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-g0-value",
		&pinfo->spr.spr_coeff_v0h1_g0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v0h1-g1-value",
		&pinfo->spr.spr_coeff_v0h1_g1.value, 0);
}

static void lcd_kit_spr_coeffs_g_v1h0_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-g0-value",
		&pinfo->spr.spr_coeff_v1h0_g0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h0-g1-value",
		&pinfo->spr.spr_coeff_v1h0_g1.value, 0);
}

static void lcd_kit_spr_coeffs_g_v1h1_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-g0-value",
		&pinfo->spr.spr_coeff_v1h1_g0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-coeff-v1h1-g1-value",
		&pinfo->spr.spr_coeff_v1h1_g1.value, 0);
}

static void lcd_kit_spr_core_coeffs_g_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_spr_coeffs_g_v0h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_g_v0h1_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_g_v1h0_set(panel_id, np, pinfo);
	lcd_kit_spr_coeffs_g_v1h1_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_core_edgestr_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-edgestr-r-value",
		&pinfo->spr.spr_edgestr_r.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-edgestr-g-value",
		&pinfo->spr.spr_edgestr_g.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-edgestr-b-value",
		&pinfo->spr.spr_edgestr_b.value, 0);
}

static void lcd_kit_spr_core_dir_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-dir-min-value",
		&pinfo->spr.spr_dir_min.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-dir-max-value",
		&pinfo->spr.spr_dir_max.value, 0);
}

static void lcd_kit_spr_core_blend_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-blend-value",
		&pinfo->spr.spr_blend.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-weight-value",
		&pinfo->spr.spr_weight.value, 0);
}

static void lcd_kit_spr_diffdirgain_r_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-r0-value",
		&pinfo->spr.spr_diff_r0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-r1-value",
		&pinfo->spr.spr_diff_r1.value, 0);
}

static void lcd_kit_spr_diffdirgain_g_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-g0-value",
		&pinfo->spr.spr_diff_g0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-g1-value",
		&pinfo->spr.spr_diff_g1.value, 0);
}

static void lcd_kit_spr_diffdirgain_b_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-b0-value",
		&pinfo->spr.spr_diff_b0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-diff-b1-value",
		&pinfo->spr.spr_diff_b1.value, 0);
}

static void lcd_kit_spr_core_diffdirgain_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_spr_diffdirgain_r_set(panel_id, np, pinfo);
	lcd_kit_spr_diffdirgain_g_set(panel_id, np, pinfo);
	lcd_kit_spr_diffdirgain_b_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_core_horz_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-horzgradblend-value",
		&pinfo->spr.spr_horzgradblend.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-horzbdbld-value",
		&pinfo->spr.spr_horzbdbld.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-horzbdweight-value",
		&pinfo->spr.spr_horzbdweight.value, 0);
}

static void lcd_kit_spr_vert_gain_rgb_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-vertbd-gain-r-value",
		&pinfo->spr.spr_vertbd_gain_r.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-vertbd-gain-g-value",
		&pinfo->spr.spr_vertbd_gain_g.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-vertbd-gain-b-value",
		&pinfo->spr.spr_vertbd_gain_b.value, 0);
}

static void lcd_kit_spr_core_vert_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-vertbdbld-value",
		&pinfo->spr.spr_vertbdbld.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-vertbdweight-value",
		&pinfo->spr.spr_vertbdweight.value, 0);

	lcd_kit_spr_vert_gain_rgb_set(panel_id, np, pinfo);
}

static void lcd_kit_spr_txip_coef_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-coef0-value",
		&pinfo->spr.txip_coef0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-coef1-value",
		&pinfo->spr.txip_coef1.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-coef2-value",
		&pinfo->spr.txip_coef2.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-coef3-value",
		&pinfo->spr.txip_coef3.value, 0);
}

static void lcd_kit_spr_txip_offset_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-offset0-value",
		&pinfo->spr.txip_offset0.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-offset1-value",
		&pinfo->spr.txip_offset1.value, 0);
}

static void lcd_kit_spr_txip_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-ctrl-value",
		&pinfo->spr.txip_ctrl.value, 0);

	pinfo->spr.txip_size.value = 0;
	pinfo->spr.txip_size.reg.txip_height = pinfo->yres - 1;
	pinfo->spr.txip_size.reg.txip_width = pinfo->xres - 1;

	lcd_kit_spr_txip_coef_set(panel_id, np, pinfo);
	lcd_kit_spr_txip_offset_set(panel_id, np, pinfo);

	lcd_kit_parse_u32(np, "lcd-kit,spr-txip-clip-value",
		&pinfo->spr.txip_clip.value, 0);
}

static void lcd_kit_spr_datapack_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-datapack-ctrl-value",
		&pinfo->spr.datapack_ctrl.value, 0);
	pinfo->spr.datapack_size.value = 0;
	pinfo->spr.datapack_size.reg.datapack_width = pinfo->xres - 1;
	pinfo->spr.datapack_size.reg.datapack_height = pinfo->yres - 1;
}

static uint32_t lcd_kit_multi_gain(uint32_t val, uint32_t gain)
{
	uint32_t ret = (val * gain + 5000) / 10000; // add 5000 and divided 10000 to get round value
	return ret;
}

static void lcd_kit_spr_degamma_gamma_set_by_bits(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, uint32_t* lut_buffer, uint32_t* low_lut_buffer, uint32_t gammaBits,
	uint32_t deGammaBits, uint32_t gain)
{
	int i, index;
	bool isPrepared = false;
	if (gammaBits == 0 || deGammaBits == 0) {
   	    for (i = 0; i < LCD_MAX_SIZE; i++) {
	    	if (i < LCD_SPR_GAMMA_SIZE) {
	    		lut_buffer[i] = lcd_kit_multi_gain(spr_lut_table_14_bit[i], gain); // for hisi nsv platform 
	    	} else {
	    		lut_buffer[i] = spr_lut_table_14_bit[i];
	    	}
	    }
	    for (index = 0; index < LCD_LOW_LUT_SIZE; index++) {
	    	low_lut_buffer[index] = lcd_kit_multi_gain(spr_lower_lut_table[index], gain); // gamma degamma low lut
	    }
		isPrepared = true;
	}
	if (deGammaBits == 16 && gammaBits == 14) {
		for (i = 0; i < LCD_MAX_SIZE; i++) {
	    	if (i < LCD_SPR_GAMMA_SIZE) {
	    		lut_buffer[i] = lcd_kit_multi_gain(spr_lut_table_16_14_bit[i], gain);
	    	} else {
	    		lut_buffer[i] = spr_lut_table_16_14_bit[i];
	    	}
	    }
		for (index = 0; index < LCD_LOW_LUT_SIZE; index++) {
	    	low_lut_buffer[index] = lcd_kit_multi_gain(spr_lower_lut_table_16bit_degamma[index], gain);
	    }
		isPrepared = true;
	}
	if (!isPrepared) {
		LCD_KIT_ERR("spr gamma degamma are not prepared\n");
		return;
	}
	pinfo->spr.spr_lut_table = lut_buffer;
	pinfo->spr.spr_gamma_low_lut_table = low_lut_buffer;
	pinfo->spr.spr_lut_table_len = LCD_MAX_SIZE;
	pinfo->spr.spr_gamma_low_lut_table_len = LCD_LOW_LUT_SIZE;
}

static void lcd_kit_spr_degamma_gamma_set(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	unsigned int spr_lut_nondefault = 0;
	uint32_t gain = 0;
	uint32_t* lut_buffer = NULL;
	uint32_t* low_lut_buffer = NULL;
	lcd_kit_parse_u32(np, "lcd-kit,spr-gamma-en-value",
		&pinfo->spr.spr_gamma_en.value, 0);
	lcd_kit_parse_u32(np, "lcd-kit,spr-gamma-shiften-value",
		&pinfo->spr.spr_gamma_shiften.value, 0);
	lcd_kit_parse_u32(np, "lcd-kit,spr-degamma-en-value",
		&pinfo->spr.degamma_en.value, 0);
	pinfo->spr.spr_lut_table = spr_lut_table_temp;
	pinfo->spr.spr_lut_table_len = LCD_MAX_SIZE;
	lcd_kit_parse_u32(np, "lcd-kit,spr-lut-nonDefault",
		&spr_lut_nondefault, 0);
	LCD_KIT_INFO("spr_lut_nondefault %d\n", spr_lut_nondefault);
	if (spr_lut_nondefault != 0) {
		pinfo->spr.spr_lut_table = NULL;
		pinfo->spr.spr_gamma_low_lut_table = NULL;
		lcd_kit_parse_u32(np, "lcd-kit,spr-dataremapping-gain",	&gain, 0);
		lut_buffer = (uint32_t*)kzalloc(sizeof(uint32_t) * LCD_MAX_SIZE, GFP_KERNEL);
		low_lut_buffer = (uint32_t*)kzalloc(sizeof(uint32_t) * LCD_LOW_LUT_SIZE, GFP_KERNEL);
		// gamma degamma lut
		if (lut_buffer == NULL || low_lut_buffer == NULL) {
			LCD_KIT_ERR("failed to alloc for spr lut or low lut\n");
			kfree(lut_buffer);
			kfree(low_lut_buffer);
			return;
		}
		uint32_t gammaBits = 0;
		uint32_t deGammaBits = 0;
		lcd_kit_parse_u32(np, "lcd-kit,spr-gamma-bits",	&gammaBits, 0);
		lcd_kit_parse_u32(np, "lcd-kit,spr-degamma-bits", &deGammaBits, 0);
        lcd_kit_spr_degamma_gamma_set_by_bits(panel_id, np, pinfo, lut_buffer,
		    low_lut_buffer, gammaBits, deGammaBits, gain);
		if (pinfo->spr.spr_lut_table != NULL) {
			LCD_KIT_INFO("spr_lut_table[256]=%u, pinfo->spr.spr_lut_table[1546]=%u\n",
			    pinfo->spr.spr_lut_table[LCD_SPR_GAMMA_LAST_INDEX], pinfo->spr.spr_lut_table[LCD_SPR_DEGAMMA_LAST_INDEX]);
		}
	}
}

static void lcd_kit_parse_spr_config_ctrl(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-ctrl-value",
		&pinfo->spr.spr_ctrl.value, 0);
}

static void lcd_kit_parse_spr_config_pix(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	lcd_kit_parse_u32(np, "lcd-kit,spr-pix-even-value",
		&pinfo->spr.spr_pix_even.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-pix-odd-value",
		&pinfo->spr.spr_pix_odd.value, 0);

	lcd_kit_parse_u32(np, "lcd-kit,spr-pix-panel-value",
		&pinfo->spr.spr_pix_panel.value, 0);
}

static void lcd_kit_parse_spr_config_info(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	pinfo->spr.panel_xres = pinfo->xres;
	pinfo->spr.panel_yres = pinfo->yres;
	pinfo->spr.spr_size.value = 0;
	pinfo->spr.spr_size.reg.spr_width = pinfo->xres - 1;
	pinfo->spr.spr_size.reg.spr_height = pinfo->yres - 1;
	lcd_kit_parse_spr_config_ctrl(panel_id, np, pinfo);
	lcd_kit_parse_spr_config_pix(panel_id, np, pinfo);
}

void lcd_kit_parse_spr_info(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo)
{
	unsigned int support = 0;
	lcd_kit_parse_u32(np, "lcd-kit,spr-support", &support, 0);
	if (!support)
		return;

	lcd_kit_parse_spr_config_info(panel_id, np, pinfo);
	lcd_kit_spr_core_border_set(panel_id, np, pinfo);
	lcd_kit_spr_core_larea_set(panel_id, np, pinfo);
	lcd_kit_spr_core_coeffs_b_set(panel_id, np, pinfo);
	lcd_kit_spr_core_coeffs_r_set(panel_id, np, pinfo);
	lcd_kit_spr_core_coeffs_g_set(panel_id, np, pinfo);
	lcd_kit_spr_core_blend_set(panel_id, np, pinfo);
	lcd_kit_spr_core_edgestr_set(panel_id, np, pinfo);
	lcd_kit_spr_core_dir_set(panel_id, np, pinfo);
	lcd_kit_spr_core_diffdirgain_set(panel_id, np, pinfo);
	lcd_kit_spr_core_horz_set(panel_id, np, pinfo);
	lcd_kit_spr_core_vert_set(panel_id, np, pinfo);
	lcd_kit_spr_txip_set(panel_id, np, pinfo);
	lcd_kit_spr_datapack_set(panel_id, np, pinfo);
	lcd_kit_spr_degamma_gamma_set(panel_id, np, pinfo);
}
