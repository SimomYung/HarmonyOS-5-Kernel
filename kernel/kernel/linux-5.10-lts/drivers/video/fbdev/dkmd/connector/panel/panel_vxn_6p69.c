/* Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Code in this file used to drive the oled panel.
 * Author:DSS
 * Create: 2022
 */
#include "dpu_conn_mgr.h"
#include "panel_mgr.h"
#include "panel_drv.h"
#include "dsc/dsc_output_calc.h"
#include "dsc/dsc_algorithm_manager.h"

#ifndef array_size
#define array_size(array) (sizeof(array) / sizeof((array)[0]))
#endif

#define FPS_120HZ_EN
#define DSC_CALC_EN
// vxn begin
static char g_on_cmd_one[] = {
	0x11
};
static char g_on_cmd_two[] = {
	0x50, 0x10
};
static char g_on_cmd_three[] = {
	0xf0, 0x5a, 0x5a
};
static char g_on_cmd_four[] = {
	0xff, 0x00, 0x00, 0x00
};
static char g_on_cmd_five[] = {
	0xF0, 0x5A, 0x5A, 0x00, 0x00
};

static char g_on_cmd_5byte[6][5] = {
	{0xF0, 0x5A, 0x5A, 0x00, 0x00},
	{0xF1, 0x5A, 0x5A, 0x00, 0x00},
	{0xF2, 0x5A, 0x5A, 0x00, 0x00},
	{0x7F, 0x5A, 0x5A, 0x00, 0x00},
	{0x50, 0x10, 0x00, 0x00, 0x00},
	{0x10, 0x19, 0x00, 0x00, 0x00}
};

static char g_on_cmd_panel_1_4byte[1][4] = {
	{0xff, 0x00, 0x00, 0x00},
};

static char g_on_cmd_panel_1_5byte[1][5] = {
	{0xC9, 0x05, 0x01, 0x00, 0x00},
};

static char g_on_cmd_rnc_off_4byte[1][4] = {
	{0xFF, 0x07, 0x00, 0x00},
};

static char g_on_cmd_rnc_off_5byte[1][5] = {
	{0x98, 0x0F, 0x00, 0x00, 0x00},
};

static char g_on_cmd_vint_diming_4byte[1][4] = {
	{0xFF, 0x07, 0x00, 0x00},
};
static char g_on_cmd_vint_diming_5byte[1][5] = {
	{0xbc, 0xFF, 0x22, 0xC0, 0x40},
};
static char g_on_cmd_elvss_diming_4byte[1][4] = {
	{0xFF, 0x02, 0x00, 0x10},
};
static char g_on_cmd_elvss_diming_5byte[1][5] = {
	{0x30, 0x0F, 0x05, 0x00, 0x00},
};

static char g_on_cmd_pmu_vrgh_4byte[3][4] = {
	{0xFF, 0x03, 0x00, 0xB4},
	{0xFF, 0x03, 0x00, 0xB8},
	{0xFF, 0x03, 0x00, 0xC4},
};
static char g_on_cmd_pmu_vrgh_5byte[3][5] = {
	{0x00, 0x19, 0x50, 0x03, 0x02},
	{0x00, 0x24, 0x4A, 0x00, 0x00},
	{0x00, 0x24, 0x04, 0x00, 0x00},
};

static char g_on_cmd_pcd_seq_4byte[2][4] = {
	{0xFF, 0x0A, 0x00, 0x18},
	{0xFF, 0x0A, 0x00, 0x00},
};
static char g_on_cmd_pcd_seq_5byte[2][5] = {
	{0x02, 0x01, 0x00, 0x01, 0x00},
	{0x01, 0x80, 0x00, 0x01, 0x00},
};

static char g_on_cmd_for_pmic_4byte[2][4] = {
	{0xFF, 0x01, 0x00, 0x2C},
	{0xFF, 0x02, 0x00, 0x0C},
};
static char g_on_cmd_for_pmic_5byte[2][5] = {
	{0x00, 0x00, 0x0E, 0x01, 0x00},
	{0x30, 0x28, 0x28, 0x09, 0x00},
};

static char g_on_cmd_4byte[12][4] = {
	{0xff, 0x06, 0x00, 0x3C},
	{0xff, 0x01, 0x00, 0x11},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x01, 0x00, 0x11},
	{0xFF, 0x00, 0x00, 0x00},
	{0x51, 0x00, 0x00, 0x01},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x00, 0x00, 0x00},
	{0xFF, 0x07, 0x00, 0x00},
};
static char g_on_cmd_3byte[1][3] = {
	{0xf0, 0x5a, 0x5a},
};

static char g_on_cmd_2byte[1][2] = {
	{0x50, 0x10},
};

static char g_on_te_config_4_byte[2][4] = {
	{0xFF, 0x06, 0x00, 0x54},
	{0xff, 0x00, 0x00, 0x00},
};
static char g_on_te_config_5_byte[1][5] = {
	{0xC9, 0x00, 0x00, 0x00, 0x01},
};
static char g_on_panel_vesa_4byte[31][4] = {
	{0xFF, 0x07, 0x00, 0x04},
	{0xFF, 0x07, 0x00, 0x08},
	{0xFF, 0x07, 0x00, 0x0C},
	{0xFF, 0x07, 0x00, 0x10},
	{0xFF, 0x07, 0x00, 0x14},
	{0xFF, 0x07, 0x00, 0x18},
	{0xFF, 0x07, 0x00, 0x1C},
	{0xFF, 0x07, 0x00, 0x20},
	{0xFF, 0x07, 0x00, 0x24},
	{0xFF, 0x07, 0x00, 0x28},
	{0xFF, 0x07, 0x00, 0x2C},
	{0xFF, 0x07, 0x00, 0x30},
	{0xFF, 0x07, 0x00, 0x34},
	{0xFF, 0x07, 0x00, 0x38},
	{0xFF, 0x07, 0x00, 0x3C},
	{0xFF, 0x07, 0x00, 0x40},
	{0xFF, 0x07, 0x00, 0x44},
	{0xFF, 0x07, 0x00, 0x48},
	{0xFF, 0x07, 0x00, 0x4C},
	{0xFF, 0x07, 0x00, 0x50},
	{0xFF, 0x07, 0x00, 0x54},
	{0xFF, 0x07, 0x00, 0x58},
	{0xFF, 0x07, 0x00, 0x5C},
	{0xFF, 0x07, 0x00, 0x60},
	{0xFF, 0x05, 0x00, 0x00},
	{0xFF, 0x05, 0x00, 0x18},
	{0xFF, 0x05, 0x00, 0x20},
	{0xFF, 0x05, 0x00, 0x0C},
	{0xFF, 0x05, 0x00, 0x40},
	{0xff, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00},
};
static char g_on_panel_vesa_5byte[29][5] = {
	{0x10, 0xAB, 0x00, 0x00, 0x11},
	{0x10, 0x80, 0x0A, 0x80, 0x30},
	{0x10, 0x20, 0x00, 0xC0, 0x04},
	{0x10, 0x60, 0x02, 0x60, 0x02},
	{0x10, 0x64, 0x02, 0x00, 0x02},
	{0x10, 0x01, 0x03, 0x20, 0x00},
	{0x10, 0x0E, 0x00, 0x08, 0x00},
	{0x10, 0xCD, 0x02, 0x9D, 0x03},
	{0x10, 0xE0, 0x10, 0x00, 0x18},
	{0x10, 0x00, 0x20, 0x10, 0x07},
	{0x10, 0x33, 0x0F, 0x0F, 0x06},
	{0x10, 0x38, 0x2A, 0x1C, 0x0E},
	{0x10, 0x69, 0x62, 0x54, 0x46},
	{0x10, 0x7B, 0x79, 0x77, 0x70},
	{0x10, 0x02, 0x02, 0x7E, 0x7D},
	{0x10, 0x40, 0x2A, 0x00, 0x22},
	{0x10, 0xFC, 0x3A, 0xBE, 0x2A},
	{0x10, 0xF8, 0x3A, 0xFA, 0x3A},
	{0x10, 0x78, 0x3B, 0x38, 0x3B},
	{0x10, 0xB6, 0x4B, 0xB6, 0x43},
	{0x10, 0xF4, 0x4B, 0xF6, 0x4B},
	{0x10, 0x74, 0x84, 0x34, 0x64},
	{0x10, 0x00, 0x00, 0x00, 0x00},
	{0x10, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x03, 0x00, 0x03, 0x02},
	{0x00, 0xC0, 0x0C, 0x00, 0x00},
	{0x00, 0xC0, 0x0C, 0x00, 0x00},
	{0x00, 0xBF, 0x04, 0x7F, 0x0A},
	{0x00, 0x60, 0x02, 0x20, 0x00},
};

static char g_on_panel_config_4byte[1][4] = {
	{0xFF, 0x02, 0x00, 0x88},
};

static char g_on_panel_config_5byte[1][5] = {
	{0x67, 0x1F, 0x00, 0x00, 0x02},
};

static char g_on_mipi_config_4byte[8][4] = {
	{0xFF, 0x02, 0x00, 0xc8},
	{0x7c, 0x09, 0x00, 0x00},
	{0xFF, 0x02, 0x00, 0x08},
	{0x7d, 0x09, 0x00, 0x00},
	{0xFF, 0x02, 0x00, 0x48},
	{0x7d, 0x09, 0x00, 0x00},
	{0xFF, 0x02, 0x00, 0x88},
	{0x7d, 0x09, 0x00, 0x00},
};

static char g_on_fps_config_4byte[1][4] = {
	{0xFF, 0x00, 0x00, 0x00},
};

static char g_on_fps_config_2byte[1][2] = {
	{0x82, 0x00},
};

static char g_on_check_reg_4byte[2][4] = {
	{0xFF, 0x01, 0x00, 0x88},
	{0xFF, 0x00, 0x00, 0x00},
};

static char g_on_check_reg_5byte[1][5] = {
	{0x40, 0x00, 0x80, 0xF6, 0x0D},
};

static char g_on_info_add_byte[1][4] = {
	{0xFF, 0x00, 0x00, 0x00},
};

static char g_on_info_1byte[2][1] = {
	{0x11},
	{0x29},
};

static char g_on_info_4byte[2][4] = {
	{0xFF, 0x00, 0x00, 0x00},
	{0x51, 0x00, 0x00, 0x01},
};

static char g_on_bl_set4byte[1][4] = {
	{0x51, 0xFF, 0x03, 0x01},
};

/* Power OFF Sequence */
static char g_off_cmd0[] = {
	0xFF, 0x00, 0x00, 0x00,
};
static char g_off_cmd1[] = {
	0x28,
};
static char g_off_cmd2[] = {
	0x10,
};

static char g_off_120_timming_4byte[1][4] = {
	{0xFF, 0x02, 0x00, 0x00},
};	
static char g_off_120_timming_5byte[1][5] = {
	{0x30, 0x01, 0x01, 0x00, 0x00},
};	

// vxn end

/* Power ON Sequence end */


static struct dsi_cmd_desc g_display_on_cmds_vxn_12[] = {
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_panel_1_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_panel_1_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_three), g_on_cmd_3byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[1]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_two), g_on_cmd_2byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[2]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[3]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[1]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[4]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[2]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[5]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[3]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[6]},  
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[4]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[7]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[8]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[9]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[10]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_4byte[11]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_5byte[5]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_te_config_4_byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_te_config_5_byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_te_config_4_byte[1]},
	/* Panel vesa information begin */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[1]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[1]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[2]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[2]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[3]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[3]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[4]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[4]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[5]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[5]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[6]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[6]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[7]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[7]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[8]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[8]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[9]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[9]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[10]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[10]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[11]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[11]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[12]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[12]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[13]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[13]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[14]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[14]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[15]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[15]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[16]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[16]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[17]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[17]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[18]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[18]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[19]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[19]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[20]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[20]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[21]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[21]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[22]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[22]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[23]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[23]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[24]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[24]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[25]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[25]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[26]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[26]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[27]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[27]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[28]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_vesa_5byte[28]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[29]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_vesa_4byte[30]},
	/* Panel vesa information end */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_rnc_off_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_rnc_off_5byte[0]},
	/* RNC off */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_panel_config_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_panel_config_5byte[0]},
	/* Panel config */ 
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_fps_config_4byte[0]},
	{DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_two), g_on_fps_config_2byte[0]},
	/* Fps info */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_vint_diming_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_vint_diming_5byte[0]},
	/* Vint2 diming */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_elvss_diming_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_elvss_diming_5byte[0]},
	/* Elvss diming max */
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_check_reg_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_check_reg_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_check_reg_4byte[1]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_pmu_vrgh_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_pmu_vrgh_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_pmu_vrgh_4byte[1]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_pmu_vrgh_5byte[1]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_pmu_vrgh_4byte[2]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_pmu_vrgh_5byte[2]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_pcd_seq_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_pcd_seq_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_pcd_seq_4byte[1]},
	{DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_pcd_seq_5byte[1]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_for_pmic_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_for_pmic_5byte[0]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_cmd_for_pmic_4byte[1]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_five), g_on_cmd_for_pmic_5byte[1]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[0]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[1]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[2]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[3]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[4]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[5]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[6]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_mipi_config_4byte[7]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_info_add_byte[0]},
	{DTYPE_DCS_WRITE, 0, 120, WAIT_TYPE_MS, sizeof(g_on_cmd_one), g_on_info_1byte[0]},
	{DTYPE_GEN_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_info_4byte[0]},
	{DTYPE_DCS_WRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_one), g_on_info_1byte[1]},
	{DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US, sizeof(g_on_cmd_four), g_on_bl_set4byte[0]},

};

static struct dsi_cmd_desc g_lcd_display_off_cmd[] = {
	{ DTYPE_GEN_LWRITE, 0, 10, 0, sizeof(g_off_cmd0), g_off_cmd0 },
	{ DTYPE_DCS_WRITE, 0, 10, 0, sizeof(g_off_cmd1), g_off_cmd1 },
	{ DTYPE_GEN_LWRITE, 0, 10, 0, sizeof(g_on_cmd_four), g_off_120_timming_4byte[0] },
	{ DTYPE_DCS_WRITE, 0, 10, 0, sizeof(g_on_cmd_five), g_off_120_timming_5byte[0] },
	{ DTYPE_GEN_LWRITE, 0, 10, 0, sizeof(g_off_cmd0), g_off_cmd0 },
	{ DTYPE_DCS_WRITE, 0, 100, WAIT_TYPE_MS, sizeof(g_off_cmd2), g_off_cmd2 },
};

/*******************************************************************************
 ** LCD GPIO
 */
#define GPIO_AMOLED_RESET_NAME  "gpio_amoled_reset"
#define GPIO_AMOLED_VCC1V2_NAME "gpio_amoled_vcc1v2"
#define GPIO_AMOLED_VCC1V8_NAME "gpio_amoled_vcc1v8"
#define GPIO_AMOLED_VCC3V1_NAME "gpio_amoled_vcc3v1"
#define GPIO_AMOLED_SW_NAME     "gpio_amoled_sw"
#define GPIO_AMOLED_TE0_NAME    "gpio_amoled_te0"

static uint32_t g_gpio_amoled_reset;
static uint32_t g_gpio_amoled_vcc1v2;
static uint32_t g_gpio_amoled_vcc3v1;
static uint32_t g_gpio_amoled_te0;

/*******************************************************************************
** LCD IOMUX
*/

static struct gpio_desc g_asic_lcd_gpio_request_cmds[] = {
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, GPIO_AMOLED_VCC1V2_NAME, &g_gpio_amoled_vcc1v2, 0},
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
};

static struct gpio_desc g_asic_lcd_gpio_normal_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5, GPIO_AMOLED_VCC1V2_NAME, &g_gpio_amoled_vcc1v2, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20, GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 35, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
};

/* power off */
static struct gpio_desc g_asic_lcd_gpio_lowpower_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 1, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10, GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10, GPIO_AMOLED_VCC1V2_NAME, &g_gpio_amoled_vcc1v2, 0},
};

/* power off */
static struct gpio_desc g_asic_lcd_gpio_free_cmds[] = {
	{DTYPE_GPIO_FREE, WAIT_TYPE_MS, 1, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_FREE, WAIT_TYPE_MS, 10, GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	{DTYPE_GPIO_FREE, WAIT_TYPE_MS, 10, GPIO_AMOLED_VCC1V2_NAME, &g_gpio_amoled_vcc1v2, 0},
};

/*******************************************************************************
 * LCD VCC
 */
#define VCC_LCDIO_NAME	"lcdio-vcc"

static struct regulator *g_vcc_lcdio;

static struct vcc_desc g_lcd_vcc_init_cmds[] = {
	/* vcc get */
	{ DTYPE_VCC_GET, VCC_LCDIO_NAME,
		&g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0 },

	/* io set voltage */
	{ DTYPE_VCC_SET_VOLTAGE, VCC_LCDIO_NAME,
		&g_vcc_lcdio, 1850000, 1850000, WAIT_TYPE_MS, 0 },
};

static struct vcc_desc g_lcd_vcc_finit_cmds[] = {
	/* vcc put */
	{ DTYPE_VCC_PUT, VCC_LCDIO_NAME,
		&g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0 },
};

static struct vcc_desc g_lcd_vcc_enable_cmds[] = {
	/* vcc enable */
	{ DTYPE_VCC_ENABLE, VCC_LCDIO_NAME,
		&g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 5 },
};

static struct vcc_desc g_lcd_vcc_disable_cmds[] = {
	/* vcc disable */
	{ DTYPE_VCC_DISABLE, VCC_LCDIO_NAME,
		&g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 5 },
};

static char refresh_cmd_0[3] = {0x6D, 0x00, 0x00};
static struct dsi_cmd_desc refresh_cmd[1] = {
	{0x39, 0, 0, 0, 3, refresh_cmd_0},
};

static char refresh_1hz_cmd_0[4] = {0xFF, 0x00, 0x00, 0x00};
static char refresh_1hz_cmd_1[2] = {0x82, 0x05};
static char refresh_10hz_cmd_1[2] = {0x82, 0x04};
static char refresh_1hz_cmd_2[4] = {0xFF, 0x00, 0x00, 0x00};
static struct dsi_cmd_desc refresh_1hz_cmd[3] = {
	{0x29, 0, 0, 0, 4, refresh_1hz_cmd_0},
	{0x15, 0, 0, 0, 2, refresh_1hz_cmd_1},
	{0x29, 0, 0, 0, 4, refresh_1hz_cmd_2},
};
static struct dsi_cmd_desc refresh_10hz_cmd[3] = {
	{0x29, 0, 0, 0, 4, refresh_1hz_cmd_0},
	{0x15, 0, 0, 0, 2, refresh_10hz_cmd_1},
	{0x29, 0, 0, 0, 4, refresh_1hz_cmd_2},
};

/*******************************************************************************
 ** LCD IOMUX
 */
static struct pinctrl_data g_pctrl;

static struct pinctrl_cmd_desc g_lcd_pinctrl_init_cmds[] = {
	{ DTYPE_PINCTRL_GET, &g_pctrl, 0 },
	{ DTYPE_PINCTRL_STATE_GET, &g_pctrl, DTYPE_PINCTRL_STATE_DEFAULT },
	{ DTYPE_PINCTRL_STATE_GET, &g_pctrl, DTYPE_PINCTRL_STATE_IDLE },
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_normal_cmds[] = {
	{ DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_DEFAULT },
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_lowpower_cmds[] = {
	{DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_finit_cmds[] = {
	{ DTYPE_PINCTRL_PUT, &g_pctrl, 0 },
};

/*******************************************************************************
 */
static void panel_drv_private_data_setup(struct panel_drv_private *priv, struct device_node *np)
{
	if (priv->connector_info.base.fpga_flag == 1) {
		g_gpio_amoled_vcc3v1 = (uint32_t)of_get_named_gpio(np, "gpios", 0);
		g_gpio_amoled_vcc1v2 = (uint32_t)of_get_named_gpio(np, "gpios", 1);
		g_gpio_amoled_reset = (uint32_t)of_get_named_gpio(np, "gpios", 2);

		dpu_pr_info("used gpio:[rst: %d, vcc3v1: %d, vcc1v2: %d]\n",
			g_gpio_amoled_reset, g_gpio_amoled_vcc3v1, g_gpio_amoled_vcc1v2);

		priv->gpio_request_cmds = g_asic_lcd_gpio_request_cmds;
		priv->gpio_request_cmds_len = (uint32_t)array_size(g_asic_lcd_gpio_request_cmds);
		priv->gpio_free_cmds = g_asic_lcd_gpio_free_cmds;
		priv->gpio_free_cmds_len = (uint32_t)array_size(g_asic_lcd_gpio_free_cmds);

		priv->gpio_normal_cmds = g_asic_lcd_gpio_normal_cmds;
		priv->gpio_normal_cmds_len = (uint32_t)array_size(g_asic_lcd_gpio_normal_cmds);
		priv->gpio_lowpower_cmds = g_asic_lcd_gpio_lowpower_cmds;
		priv->gpio_lowpower_cmds_len = (uint32_t)array_size(g_asic_lcd_gpio_lowpower_cmds);
	} else {
		g_gpio_amoled_vcc3v1 = (uint32_t)of_get_named_gpio(np, "gpios", 0);
		g_gpio_amoled_vcc1v2 = (uint32_t)of_get_named_gpio(np, "gpios", 1);
		g_gpio_amoled_reset = (uint32_t)of_get_named_gpio(np, "gpios", 2);
		g_gpio_amoled_te0 = (uint32_t)of_get_named_gpio(np, "gpios", 3);

		dpu_pr_info("used gpio:[rst: %d, vcc3v1: %d, vcc1v2: %d, te0: %d]\n",
			g_gpio_amoled_reset, g_gpio_amoled_vcc3v1, g_gpio_amoled_vcc1v2, g_gpio_amoled_te0);

		priv->gpio_request_cmds = g_asic_lcd_gpio_request_cmds;
		priv->gpio_request_cmds_len = array_size(g_asic_lcd_gpio_request_cmds);
		priv->gpio_free_cmds = g_asic_lcd_gpio_free_cmds;
		priv->gpio_free_cmds_len = array_size(g_asic_lcd_gpio_free_cmds);

		priv->gpio_normal_cmds = g_asic_lcd_gpio_normal_cmds;
		priv->gpio_normal_cmds_len = array_size(g_asic_lcd_gpio_normal_cmds);
		priv->gpio_lowpower_cmds = g_asic_lcd_gpio_lowpower_cmds;
		priv->gpio_lowpower_cmds_len = array_size(g_asic_lcd_gpio_lowpower_cmds);

		priv->vcc_init_cmds = g_lcd_vcc_init_cmds;
		priv->vcc_init_cmds_len = array_size(g_lcd_vcc_init_cmds);
		priv->vcc_finit_cmds = g_lcd_vcc_finit_cmds;
		priv->vcc_finit_cmds_len = array_size(g_lcd_vcc_finit_cmds);

		priv->vcc_enable_cmds = g_lcd_vcc_enable_cmds;
		priv->vcc_enable_cmds_len = array_size(g_lcd_vcc_enable_cmds);
		priv->vcc_disable_cmds = g_lcd_vcc_disable_cmds;
		priv->vcc_disable_cmds_len = array_size(g_lcd_vcc_disable_cmds);

		priv->pinctrl_init_cmds = g_lcd_pinctrl_init_cmds;
		priv->pinctrl_init_cmds_len = array_size(g_lcd_pinctrl_init_cmds);
		priv->pinctrl_finit_cmds = g_lcd_pinctrl_finit_cmds;
		priv->pinctrl_finit_cmds_len = array_size(g_lcd_pinctrl_finit_cmds);

		priv->pinctrl_normal_cmds = g_lcd_pinctrl_normal_cmds;
		priv->pinctrl_normal_cmds_len = array_size(g_lcd_pinctrl_normal_cmds);
		priv->pinctrl_lowpower_cmds = g_lcd_pinctrl_lowpower_cmds;
		priv->pinctrl_lowpower_cmds_len = array_size(g_lcd_pinctrl_lowpower_cmds);
	}
}

#ifdef DSC_CALC_EN
static void dsc_param_set(struct dkmd_connector_info *pinfo, struct dsc_calc_info *dsc)
{
	struct dsc_algorithm_manager *pt = get_dsc_algorithm_manager_instance();
	struct input_dsc_info input_dsc_info;

	if (!pt) {
		dpu_pr_err("pt is null!\n");
		return;
	}

	dpu_pr_info("+\n");

	input_dsc_info.dsc_version = DSC_VERSION_V_1_1;
	input_dsc_info.format = DSC_RGB;
	input_dsc_info.pic_width = pinfo->base.xres;
	input_dsc_info.pic_height = pinfo->base.yres;
	input_dsc_info.slice_width = 608;
	input_dsc_info.slice_height = 32;
	input_dsc_info.dsc_bpp = 8;
	input_dsc_info.dsc_bpc = 10;
	input_dsc_info.block_pred_enable = 1;
	input_dsc_info.linebuf_depth = 11;
	input_dsc_info.gen_rc_params = 1;

	pt->vesa_dsc_info_calc(&input_dsc_info, &(dsc->dsc_info), NULL);
	dsc->dsc_en = 1;
	dpu_pr_info("-\n");
}
#endif

/* dsi param initialized value from panel spec */
static void mipi_lcd_init_dsi_param(struct dkmd_connector_info *pinfo, struct mipi_panel_info *mipi)
{
#if defined(FPS_120HZ_EN)
	mipi->hsa = 4;
	mipi->hbp = 16;
	mipi->dpi_hsize = 306;
	mipi->hline_time = 342;

	mipi->vsa = 8;
	mipi->vbp = 92;
	mipi->vfp = 80;
	mipi->pxl_clk_rate = 192 * 1000000UL;
	mipi->dsi_bit_clk = 499;
#else
	if (pinfo->base.fpga_flag == 1) {
		mipi->hsa = 4;
		mipi->hbp = 16;
		mipi->dpi_hsize = 306;
		mipi->hline_time = 500;

		mipi->vsa = 8;
		mipi->vbp = 92;
		mipi->vfp = 80;
		mipi->pxl_clk_rate = 20 * 1000000UL;
		mipi->dsi_bit_clk = 120;
	}
	else {
		mipi->hsa = 4;
		mipi->hbp = 16;
		mipi->dpi_hsize = 306;
		mipi->hline_time = 342;

		mipi->vsa = 8;
		mipi->vbp = 92;
		mipi->vfp = 80;
		mipi->pxl_clk_rate = 192 * 1000000UL;
		mipi->dsi_bit_clk = 499;
	}
#endif
	mipi->dsi_bit_clk_val1 = 471;
	mipi->dsi_bit_clk_val2 = 480;
	mipi->dsi_bit_clk_val3 = 490;
	mipi->dsi_bit_clk_val4 = 500;

	mipi->dsi_bit_clk_upt = mipi->dsi_bit_clk;
	mipi->dsi_bit_clk_default = mipi->dsi_bit_clk;

	mipi->pxl_clk_rate_div = 1;
	mipi->dsi_bit_clk_upt_support = 0;

	mipi->clk_post_adjust = 16;
	mipi->lane_nums = DSI_4_LANES;
	mipi->color_mode = DSI_24BITS_1;
	mipi->phy_mode = DPHY_MODE;

	/* for video mode */
	mipi->vc = 0;
	mipi->max_tx_esc_clk = 10 * 1000000;
	mipi->burst_mode = DSI_BURST_SYNC_PULSES_1;
	mipi->non_continue_en = 1;
}

static void ltpo_para_init(struct panel_drv_private *priv)
{
	priv->dfr_info.dfr_mode = DFR_MODE_TE_SKIP_BY_MCU;
	priv->dfr_info.ddic_type = DDIC_TYPE_H01;
	priv->dfr_info.oled_info.oled_type = PANEL_OLED_LTPO;

	priv->dfr_info.oled_info.fps_sup_num = 5;
	priv->dfr_info.oled_info.fps_sup_seq[0] = 120;
	priv->dfr_info.oled_info.fps_sup_seq[1] = 90;
	priv->dfr_info.oled_info.fps_sup_seq[2] = 72;
	priv->dfr_info.oled_info.fps_sup_seq[3] = 60;
	priv->dfr_info.oled_info.fps_sup_seq[4] = 30;

	priv->dfr_info.oled_info.ltpo_info.ver = PANEL_LTPO_V2;
	priv->dfr_info.oled_info.ltpo_info.te_freq_num = 1;
	priv->dfr_info.oled_info.ltpo_info.te_freqs[0] = 360;
	priv->dfr_info.oled_info.ltpo_info.te_mask_num = 1;
	priv->dfr_info.oled_info.ltpo_info.te_masks[0] = 2;
	priv->dfr_info.oled_info.ltpo_info.dimming_enable = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_mode = DIMMING_MODE_PLATFORM;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].type = DIMMING_NORMAL;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_num = 11;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[0].frm_rate = 60;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[0].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[1].frm_rate = 30;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[1].repeat_num = 2;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[2].frm_rate = 10;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[2].repeat_num = 6;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[3].frm_rate = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[3].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[4].frm_rate = 45;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[4].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[5].frm_rate = 30;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[5].repeat_num = 2;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[6].frm_rate = 10;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[6].repeat_num = 6;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[7].frm_rate = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[7].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[8].frm_rate = 36;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[8].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[9].frm_rate = 12;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[9].repeat_num = 5;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[10].frm_rate = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_sequence[0].dimming_seq_list[10].repeat_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_len = 4;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[0].frm_rate = 120;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[0].dimming_gear1 = 0;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[0].dimming_gear2 = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[0].dimming_end = 3;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[1].frm_rate = 90;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[1].dimming_gear1 = 4;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[1].dimming_gear2 = 5;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[1].dimming_end = 7;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[2].frm_rate = 72;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[2].dimming_gear1 = 8;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[2].dimming_gear2 = 9;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[2].dimming_end = 10;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[3].frm_rate = 60;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[3].dimming_gear1 = 0;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[3].dimming_gear2 = 1;
	priv->dfr_info.oled_info.ltpo_info.dimming_gear_config[3].dimming_end = 3;
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[2].cmd_num = 1;
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[2].cmds[0] = refresh_cmd[0];
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[3].cmd_num = 3;
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[3].cmds[0] = refresh_1hz_cmd[0];
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[3].cmds[1] = refresh_1hz_cmd[1];
	priv->dfr_info.oled_info.ltpo_info.dsi_cmds[3].cmds[2] = refresh_1hz_cmd[2];

	/* safe frm rate cmd */
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates_num = 2;
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[0].safe_frm_rate = 10;
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[0].dsi_cmds.cmd_num = 3;
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[0].dsi_cmds.cmds[0] = refresh_10hz_cmd[0];
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[0].dsi_cmds.cmds[1] = refresh_10hz_cmd[1];
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[0].dsi_cmds.cmds[2] = refresh_10hz_cmd[2];
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[1].safe_frm_rate = 1;
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[1].dsi_cmds.cmd_num = 3;
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[1].dsi_cmds.cmds[0] = refresh_1hz_cmd[0];
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[1].dsi_cmds.cmds[1] = refresh_1hz_cmd[1];
	priv->dfr_info.oled_info.ltpo_info.safe_frm_rates[1].dsi_cmds.cmds[2] = refresh_1hz_cmd[2];
}

static int32_t panel_of_device_setup(struct panel_drv_private *priv)
{
	int32_t ret;
	struct dkmd_connector_info *pinfo = &priv->connector_info;
	struct device_node *np = priv->pdev->dev.of_node;

	dpu_pr_info("enter!\n");

	/* Inheritance based processing */
	panel_base_of_device_setup(priv);
	panel_drv_private_data_setup(priv, np);

	pinfo->base.type = PANEL_MIPI_CMD;

	/* 1. config base object info
	 * would be used for framebuffer setup
	 */
	pinfo->base.xres = 1216;
	pinfo->base.yres = 2688;

	/* When calculating DPI needs the following parameters */
	pinfo->base.width = 70;
	pinfo->base.height = 154;

	/* ltpo parameter init */
	ltpo_para_init(priv);
	// caculate fps by mipi timing para
	pinfo->base.fps = 120;

	/* 2. config connector info
	 * would be used for dsi & composer setup
	 */
	mipi_lcd_init_dsi_param(pinfo, &get_primary_connector(pinfo)->post_info[0]->mipi);

#ifdef DSC_CALC_EN
	pinfo->ifbc_type = IFBC_TYPE_VESA3_75X_DUAL;
	dsc_param_set(pinfo, &get_primary_connector(pinfo)->post_info[0]->dsc);
#endif
	/* dsi or composer need this param */
	pinfo->dirty_region_updt_support = 0;
	pinfo->vsync_ctrl_type = VSYNC_IDLE_MIPI_ULPS | VSYNC_IDLE_CLK_OFF | VSYNC_IDLE_ISR_OFF;
	/* 3. config panel private info
	 * would be used for panel setup
	 */
	pinfo->bl_info.bl_min = 12;
	pinfo->bl_info.bl_max = 16380;
	pinfo->bl_info.bl_default = 6420;
	priv->mipi_brightness_para_type = MIPI_BL_PARA1_DBV8_AND_PARA2_DBV0;

	/* mipi dsi cmds */
	priv->disp_on_cmds = g_display_on_cmds_vxn_12;
	priv->disp_on_cmds_len = array_size(g_display_on_cmds_vxn_12);
	priv->disp_off_cmds = g_lcd_display_off_cmd;
	priv->disp_off_cmds_len = array_size(g_lcd_display_off_cmd);

	if (pinfo->base.fpga_flag == 0) {
		ret = peri_vcc_cmds_tx(priv->pdev, priv->vcc_init_cmds, priv->vcc_init_cmds_len);
		if (ret != 0)
			dpu_pr_info("vcc init failed!\n");

		ret = peri_pinctrl_cmds_tx(priv->pdev, priv->pinctrl_init_cmds, priv->pinctrl_init_cmds_len);
		if (ret != 0)
			dpu_pr_info("pinctrl init failed\n");

		ret = peri_vcc_cmds_tx(priv->pdev, priv->vcc_enable_cmds, priv->vcc_enable_cmds_len);
		if (ret)
			dpu_pr_warn("vcc enable cmds handle fail!\n");
	}
	dpu_pr_info("exit!\n");

	return 0;
}

static void panel_of_device_release(struct panel_drv_private *priv)
{
	int32_t ret = 0;
	panel_base_of_device_release(priv);
	if (priv->gpio_free_cmds && (priv->gpio_free_cmds_len > 0)) {
		ret = peri_gpio_cmds_tx(priv->gpio_free_cmds, priv->gpio_free_cmds_len);
		if (ret)
			dpu_pr_info("gpio free handle err!\n");
	}
	dpu_pr_info("exit!\n");
}

panel_device_match_data(vxn_6p69_panel_info, PANEL_VXN_6P69_ID, panel_of_device_setup, panel_of_device_release);

MODULE_LICENSE("GPL");