/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "dpu_conn_mgr.h"
#include "panel_mgr.h"
#include "panel_drv.h"

/* if test other fuction, please open:
	"#define BIST_MODE" or "#define ENABLE_DSC" or "#define OP_INITIAL_CODE_ONLY" */

#define ENABLE_DSC

static char g_lcdkit_on_cmd_0[] = {
	0xFE, 0x74,
};
static char g_lcdkit_on_cmd_1[] = {
	0x19, 0x03,
};
static char g_lcdkit_on_cmd_2[] = {
	0x1A, 0xFF,
};
static char g_lcdkit_on_cmd_3[] = {
	0x1B, 0x5C,
};
static char g_lcdkit_on_cmd_4[] = {
	0x1C, 0x5C,
};
static char g_lcdkit_on_cmd_5[] = {
	0x1D, 0xFF,
};
static char g_lcdkit_on_cmd_6[] = {
	0x1E, 0x5C,
};
static char g_lcdkit_on_cmd_7[] = {
	0x1F, 0x5C,
};
static char g_lcdkit_on_cmd_8[55][2] = {
	{0xFE, 0x43},
	{0x8B, 0x00},
	{0x8C, 0x70},
	{0x8D, 0xA0},
	{0x8E, 0x90},
	{0xAD, 0x00},
	{0xAE, 0x38},
    {0xAF, 0x5F},
	{0xB0, 0x5F},
	{0xCD, 0x00},
	{0xCE, 0x2A},
	{0xCF, 0x2A},
	{0xD0, 0x3A},
	{0xFE, 0x49},
    {0x60, 0x03},
	{0x61, 0x5B},
	{0xFE, 0x1A},
	{0x56, 0x5C},
	{0x58, 0x11},
	{0xFE, 0x9C},
	{0x74, 0xBB},
    {0x75, 0x8B},
	{0x76, 0x8B},
	{0x77, 0x88},
	{0x78, 0x88},
	{0x79, 0x88},
	{0x7A, 0x08},
	{0xFE, 0xA0},
    {0x06, 0x36},
	{0x7C, 0x1F},
	{0xFE, 0x42},
	{0x17, 0x02},
	{0xFE, 0xB0},
	{0x55, 0xE7},
	{0x56, 0xE7},
    {0xFE, 0xA1},
	{0x75, 0xA7},
	{0xCD, 0x00},
	{0xCE, 0x00},
	{0x74, 0x72},
	{0xC3, 0x83},
	{0xC4, 0xFF},
    {0xC5, 0x7F},
	{0xFE, 0XD4},
	{0x40, 0x03},
	{0xFE, 0xFD},
	{0x80, 0x06},
	{0x83, 0x00},
	{0xFE, 0x40},
    {0xBC, 0x00},
	{0xFE, 0x44},
	{0x78, 0x00},
	{0x79, 0x00},
	{0xFE, 0x00},
	{0x2F, 0x04},
};
static char g_lcdkit_on_cmd_9[] = { /* 29 */
	0x44, 0x00, 0x00,
};
static char g_lcdkit_on_cmd_10[6][2] = {
	{0x8A, 0x10},
	{0xFE, 0x40},
	{0x98, 0x12},
	{0xFE, 0x62},
	{0x4D, 0xFE},
	{0x50, 0x3E},
};
 
// swire modify 
 
#ifdef ENABLE_DSC
// PPS
static char g_lcdkit_on_cmd_11[92][2] = {
	{0xFE, 0xD2}, 
	{0x97, 0x08},
	{0x36, 0x11},
	{0x39, 0xab},
	{0x3A, 0x30},
	{0x3B, 0x80},
	{0x3D, 0x0a},
	{0x3F, 0xd8},
	{0x40, 0x04},
    {0x41, 0xc8},
	{0x42, 0x00},
	{0x43, 0x08},
	{0x44, 0x02},
	{0x45, 0x64},
	{0x46, 0x02},
	{0x47, 0x64},
	{0x48, 0x02},
	{0x49, 0x00},
    {0x4A, 0x02},
	{0x4B, 0x32},
	{0x4D, 0x20},
	{0x4E, 0x00},
	{0x4F, 0xc6},
    {0x50, 0x00},
	{0x51, 0x08},
	{0x53, 0x0c},
	{0x54, 0x0d},
	{0x55, 0xb7},
	{0x56, 0x0b}, 
	{0x58, 0x38},
	{0x59, 0x18},
    {0x5A, 0x00},
	{0x5B, 0x10},
	{0x5C, 0xf0},
	{0x5D, 0x07},
	{0x5E, 0x10},
	{0x5F, 0x20},
    {0x60, 0x00},
	{0x61, 0x06},
	{0x62, 0x0f},
	{0x63, 0x0f},
	{0x64, 0x33},
	{0x65, 0x0e},
	{0x66, 0x1c},
	{0x67, 0x2a},
	{0x68, 0x38},
	{0x69, 0x46},
    {0x6A, 0x54},
	{0x6B, 0x62},
	{0x6C, 0x69},
	{0x6D, 0x70},
	{0x6E, 0x77},
	{0x6F, 0x79},
    {0x70, 0x7b},
	{0x71, 0x7d},
	{0x72, 0x7e},
	{0x73, 0x02},
	{0x74, 0x02},
	{0x75, 0x22},
	{0x76, 0x00},
	{0x77, 0x2a},
	{0x78, 0x40},
	{0x79, 0x2a},
    {0x7A, 0xbe},
	{0x7B, 0x3a},
	{0x7C, 0xfc},
	{0x7D, 0x3a},
	{0x7E, 0xfa},
	{0x7F, 0x3a},
    {0x80, 0xf8},
	{0x81, 0x3b},
	{0x82, 0x38},
	{0x83, 0x3b},
	{0x84, 0x78},
	{0x85, 0x3b},
	{0x86, 0xb6},
	{0x87, 0x4b},
	{0x88, 0xf6},
	{0x89, 0x4c},
    {0x8A, 0x34},
	{0x8B, 0x4c},
	{0x8C, 0x74},
	{0x8D, 0x5c},
	{0x8E, 0x74},
	{0x8F, 0x8c},
    {0x90, 0xf4},
	{0x91, 0x00},
	{0x92, 0x00},
	{0x93, 0x00},
	{0x94, 0x00},
	{0x95, 0x00},
	{0x96, 0x00},
};
#endif
// dimming config

static char g_on_cmd0[4][2] = {
	{0xFE, 0x00}, 
	{0xFA, 0x01},
	{0xC2, 0x08},
	{0x35, 0x00},
};
 
static char g_on_cmd1[] = {
	0x11
};
// display on
static char g_on_cmd2[] = {
	0x29
};
 
// backlight cmd
static char g_on_cmd3[] = { /* 39 */
    0x51, 0x00, 0x00,
};
//////////////Power on initial code end/////////////////////////
 
 
//////////////Power off code start/////////////////////////
static char g_off_cmd0[] = {
	0xFE, 0x00,
};
static char g_off_cmd1[] = {
	0x28,
};
static char g_off_cmd2[] = {
	0x10,
};
 
//////////////Power off code end/////////////////////////
 
static struct dsi_cmd_desc g_lcd_display_on_cmds[] = {
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_0), g_lcdkit_on_cmd_0 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_1), g_lcdkit_on_cmd_1 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_2), g_lcdkit_on_cmd_2 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_3), g_lcdkit_on_cmd_3 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_4), g_lcdkit_on_cmd_4 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_5), g_lcdkit_on_cmd_5 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_6), g_lcdkit_on_cmd_6 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_7), g_lcdkit_on_cmd_7 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[0]), g_lcdkit_on_cmd_8[0] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[1]), g_lcdkit_on_cmd_8[1] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[2]), g_lcdkit_on_cmd_8[2] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[3]), g_lcdkit_on_cmd_8[3] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[4]), g_lcdkit_on_cmd_8[4] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[5]), g_lcdkit_on_cmd_8[5] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[6]), g_lcdkit_on_cmd_8[6] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[7]), g_lcdkit_on_cmd_8[7] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[8]), g_lcdkit_on_cmd_8[8] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[9]), g_lcdkit_on_cmd_8[9] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[10]), g_lcdkit_on_cmd_8[10] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[11]), g_lcdkit_on_cmd_8[11] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[12]), g_lcdkit_on_cmd_8[12] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[13]), g_lcdkit_on_cmd_8[13] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[14]), g_lcdkit_on_cmd_8[14] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[15]), g_lcdkit_on_cmd_8[15] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[16]), g_lcdkit_on_cmd_8[16] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[17]), g_lcdkit_on_cmd_8[17] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[18]), g_lcdkit_on_cmd_8[18] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[19]), g_lcdkit_on_cmd_8[19] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[20]), g_lcdkit_on_cmd_8[20] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[21]), g_lcdkit_on_cmd_8[21] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[22]), g_lcdkit_on_cmd_8[22] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[23]), g_lcdkit_on_cmd_8[23] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[24]), g_lcdkit_on_cmd_8[24] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[25]), g_lcdkit_on_cmd_8[25] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[26]), g_lcdkit_on_cmd_8[26] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[27]), g_lcdkit_on_cmd_8[27] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[28]), g_lcdkit_on_cmd_8[28] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[29]), g_lcdkit_on_cmd_8[29] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[30]), g_lcdkit_on_cmd_8[30] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[31]), g_lcdkit_on_cmd_8[31] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[32]), g_lcdkit_on_cmd_8[32] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[33]), g_lcdkit_on_cmd_8[33] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[34]), g_lcdkit_on_cmd_8[34] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[35]), g_lcdkit_on_cmd_8[35] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[36]), g_lcdkit_on_cmd_8[36] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[37]), g_lcdkit_on_cmd_8[37] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[38]), g_lcdkit_on_cmd_8[38] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[39]), g_lcdkit_on_cmd_8[39] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[40]), g_lcdkit_on_cmd_8[40] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[41]), g_lcdkit_on_cmd_8[41] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[42]), g_lcdkit_on_cmd_8[42] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[43]), g_lcdkit_on_cmd_8[43] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[44]), g_lcdkit_on_cmd_8[44] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[45]), g_lcdkit_on_cmd_8[45] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[46]), g_lcdkit_on_cmd_8[46] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[47]), g_lcdkit_on_cmd_8[47] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[48]), g_lcdkit_on_cmd_8[48] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[49]), g_lcdkit_on_cmd_8[49] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[50]), g_lcdkit_on_cmd_8[50] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[51]), g_lcdkit_on_cmd_8[51] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[52]), g_lcdkit_on_cmd_8[52] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[53]), g_lcdkit_on_cmd_8[53] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_8[54]), g_lcdkit_on_cmd_8[54] },
	{ DTYPE_GEN_LWRITE,  0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_9), g_lcdkit_on_cmd_9 },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[0]), g_lcdkit_on_cmd_10[0] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[1]), g_lcdkit_on_cmd_10[1] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[2]), g_lcdkit_on_cmd_10[2] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[3]), g_lcdkit_on_cmd_10[3] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[4]), g_lcdkit_on_cmd_10[4] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_10[5]), g_lcdkit_on_cmd_10[5] },
 #ifdef ENABLE_DSC
    { DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[0]), g_lcdkit_on_cmd_11[0] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[1]), g_lcdkit_on_cmd_11[1] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[2]), g_lcdkit_on_cmd_11[2] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[3]), g_lcdkit_on_cmd_11[3] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[4]), g_lcdkit_on_cmd_11[4] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[5]), g_lcdkit_on_cmd_11[5] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[6]), g_lcdkit_on_cmd_11[6] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[7]), g_lcdkit_on_cmd_11[7] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[8]), g_lcdkit_on_cmd_11[8] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[9]), g_lcdkit_on_cmd_11[9] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[10]), g_lcdkit_on_cmd_11[10] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[11]), g_lcdkit_on_cmd_11[11] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[12]), g_lcdkit_on_cmd_11[12] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[13]), g_lcdkit_on_cmd_11[13] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[14]), g_lcdkit_on_cmd_11[14] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[15]), g_lcdkit_on_cmd_11[15] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[16]), g_lcdkit_on_cmd_11[16] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[17]), g_lcdkit_on_cmd_11[17] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[18]), g_lcdkit_on_cmd_11[18] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[19]), g_lcdkit_on_cmd_11[19] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[20]), g_lcdkit_on_cmd_11[20] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[21]), g_lcdkit_on_cmd_11[21] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[22]), g_lcdkit_on_cmd_11[22] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[23]), g_lcdkit_on_cmd_11[23] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[24]), g_lcdkit_on_cmd_11[24] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[25]), g_lcdkit_on_cmd_11[25] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[26]), g_lcdkit_on_cmd_11[26] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[27]), g_lcdkit_on_cmd_11[27] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[28]), g_lcdkit_on_cmd_11[28] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[29]), g_lcdkit_on_cmd_11[29] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[30]), g_lcdkit_on_cmd_11[30] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[31]), g_lcdkit_on_cmd_11[31] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[32]), g_lcdkit_on_cmd_11[32] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[33]), g_lcdkit_on_cmd_11[33] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[34]), g_lcdkit_on_cmd_11[34] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[35]), g_lcdkit_on_cmd_11[35] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[36]), g_lcdkit_on_cmd_11[36] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[37]), g_lcdkit_on_cmd_11[37] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[38]), g_lcdkit_on_cmd_11[38] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[39]), g_lcdkit_on_cmd_11[39] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[40]), g_lcdkit_on_cmd_11[40] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[41]), g_lcdkit_on_cmd_11[41] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[42]), g_lcdkit_on_cmd_11[42] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[43]), g_lcdkit_on_cmd_11[43] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[44]), g_lcdkit_on_cmd_11[44] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[45]), g_lcdkit_on_cmd_11[45] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[46]), g_lcdkit_on_cmd_11[46] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[47]), g_lcdkit_on_cmd_11[47] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[48]), g_lcdkit_on_cmd_11[48] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[49]), g_lcdkit_on_cmd_11[49] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[50]), g_lcdkit_on_cmd_11[50] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[51]), g_lcdkit_on_cmd_11[51] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[52]), g_lcdkit_on_cmd_11[52] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[53]), g_lcdkit_on_cmd_11[53] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[54]), g_lcdkit_on_cmd_11[54] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[55]), g_lcdkit_on_cmd_11[55] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[56]), g_lcdkit_on_cmd_11[56] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[57]), g_lcdkit_on_cmd_11[57] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[58]), g_lcdkit_on_cmd_11[58] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[59]), g_lcdkit_on_cmd_11[59] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[60]), g_lcdkit_on_cmd_11[60] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[61]), g_lcdkit_on_cmd_11[61] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[62]), g_lcdkit_on_cmd_11[62] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[63]), g_lcdkit_on_cmd_11[63] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[64]), g_lcdkit_on_cmd_11[64] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[65]), g_lcdkit_on_cmd_11[65] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[66]), g_lcdkit_on_cmd_11[66] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[67]), g_lcdkit_on_cmd_11[67] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[68]), g_lcdkit_on_cmd_11[68] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[69]), g_lcdkit_on_cmd_11[69] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[70]), g_lcdkit_on_cmd_11[70] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[71]), g_lcdkit_on_cmd_11[71] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[72]), g_lcdkit_on_cmd_11[72] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[73]), g_lcdkit_on_cmd_11[73] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[74]), g_lcdkit_on_cmd_11[74] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[75]), g_lcdkit_on_cmd_11[75] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[76]), g_lcdkit_on_cmd_11[76] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[77]), g_lcdkit_on_cmd_11[77] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[78]), g_lcdkit_on_cmd_11[78] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[79]), g_lcdkit_on_cmd_11[79] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[80]), g_lcdkit_on_cmd_11[80] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[81]), g_lcdkit_on_cmd_11[81] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[82]), g_lcdkit_on_cmd_11[82] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[83]), g_lcdkit_on_cmd_11[83] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[84]), g_lcdkit_on_cmd_11[84] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[85]), g_lcdkit_on_cmd_11[85] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[86]), g_lcdkit_on_cmd_11[86] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[87]), g_lcdkit_on_cmd_11[87] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[88]), g_lcdkit_on_cmd_11[88] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[89]), g_lcdkit_on_cmd_11[89] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[90]), g_lcdkit_on_cmd_11[90] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_lcdkit_on_cmd_11[91]), g_lcdkit_on_cmd_11[91] },
 #endif
    { DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd0[0]), g_on_cmd0[0] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd0[1]), g_on_cmd0[1] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd0[2]), g_on_cmd0[2] },
	{ DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd0[3]), g_on_cmd0[3] },
    { DTYPE_DCS_WRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd1), g_on_cmd1 },
    { DTYPE_DCS_WRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd2), g_on_cmd2 },
	/* backlight */
	{ DTYPE_DCS_LWRITE, 0, 10, WAIT_TYPE_US, sizeof(g_on_cmd3), g_on_cmd3 },
};
 
static struct dsi_cmd_desc g_lcd_display_off_cmds[] = {
    { DTYPE_DCS_WRITE1, 0, 10, WAIT_TYPE_MS, sizeof(g_off_cmd0), g_off_cmd0 },
	{ DTYPE_DCS_WRITE, 0, 0, WAIT_TYPE_MS, sizeof(g_off_cmd1), g_off_cmd1 },
    { DTYPE_DCS_WRITE, 0, 0, WAIT_TYPE_MS, sizeof(g_off_cmd2), g_off_cmd2 },
};

/*******************************************************************************
 ** LCD GPIO
 */
#define GPIO_AMOLED_RESET_NAME "gpio_amoled_reset"
#define GPIO_AMOLED_TE0_NAME "gpio_amoled_te0"
#define GPIO_AMOLED_VCC1V8_NAME "gpio_amoled_vcc1v8"
#define GPIO_AMOLED_VCC1V2_NAME "gpio_amoled_vcc1v2"
#define GPIO_AMOLED_VCC3V1_NAME "gpio_amoled_vcc3v1"

static uint32_t g_gpio_amoled_reset;
static uint32_t g_gpio_amoled_te0;

static struct gpio_desc g_lcd_gpio_request_cmds[] = {
	/* reset */
	{ DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0 },
};

static struct gpio_desc g_lcd_gpio_normal_cmds[] = {
	/* reset */
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1 },
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0 },
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1 },
	/* backlight enable */
};

static struct gpio_desc g_lcd_gpio_lowpower_cmds[] = {
	/* reset */
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0 },
};

static struct gpio_desc g_lcd_gpio_free_cmds[] = {
	/* reset */
	{ DTYPE_GPIO_FREE, WAIT_TYPE_MS, 0, GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0 },
};

/*******************************************************************************
 ** LCD VCC
 */
#define VCC_LCDIO_NAME   "lcdio-vcc" // 1V8
#define VCC_LCDVCI_NAME  "lcdvci-vcc" // 3V1
#define VCC_LCDDDIC_NAME "lcdddic-vcc" // 1V2

static struct regulator *g_vcc_lcdio;
static struct regulator *g_vcc_lcdvci;
static struct regulator *g_vcc_lcdddic;

static struct vcc_desc g_lcd_vcc_init_cmds[] = {
	/* vcc get */
	{ DTYPE_VCC_GET, VCC_LCDIO_NAME, &g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0},
	{ DTYPE_VCC_GET, VCC_LCDVCI_NAME, &g_vcc_lcdvci, 0, 0, WAIT_TYPE_MS, 0 },
	{ DTYPE_VCC_GET, VCC_LCDDDIC_NAME, &g_vcc_lcdddic, 0, 0, WAIT_TYPE_MS, 0 },

	/* vcc set voltage */
	/* io set voltage */
	{ DTYPE_VCC_SET_VOLTAGE, VCC_LCDIO_NAME, &g_vcc_lcdio, 1850000, 1850000, WAIT_TYPE_MS, 0 },
	{ DTYPE_VCC_SET_VOLTAGE, VCC_LCDVCI_NAME, &g_vcc_lcdvci, 3100000, 3100000, WAIT_TYPE_MS, 0 },
	{ DTYPE_VCC_SET_VOLTAGE, VCC_LCDDDIC_NAME, &g_vcc_lcdddic, 1225000, 1225000, WAIT_TYPE_MS, 0 },
};
static struct vcc_desc g_lcd_vcc_finit_cmds[] = {
	/* vcc put */
	{ DTYPE_VCC_PUT, VCC_LCDIO_NAME, &g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 0 },
	{ DTYPE_VCC_PUT, VCC_LCDVCI_NAME, &g_vcc_lcdvci, 0, 0, WAIT_TYPE_MS, 0 },
	{ DTYPE_VCC_PUT, VCC_LCDDDIC_NAME, &g_vcc_lcdddic, 0, 0, WAIT_TYPE_MS, 0 },
};

static struct vcc_desc g_lcd_vcc_enable_cmds[] = {
	/* vcc enable */
	{ DTYPE_VCC_ENABLE, VCC_LCDIO_NAME, &g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 3 },
	{ DTYPE_VCC_ENABLE, VCC_LCDVCI_NAME, &g_vcc_lcdvci, 0, 0, WAIT_TYPE_MS, 3 },
	{ DTYPE_VCC_ENABLE, VCC_LCDDDIC_NAME, &g_vcc_lcdddic, 0, 0, WAIT_TYPE_MS, 3 },
};

static struct vcc_desc g_lcd_vcc_disable_cmds[] = {
	/* vcc disable */
	{ DTYPE_VCC_DISABLE, VCC_LCDIO_NAME, &g_vcc_lcdio, 0, 0, WAIT_TYPE_MS, 3 },
	{ DTYPE_VCC_DISABLE, VCC_LCDVCI_NAME, &g_vcc_lcdvci, 0, 0, WAIT_TYPE_MS, 3 },
	{ DTYPE_VCC_DISABLE, VCC_LCDDDIC_NAME, &g_vcc_lcdddic, 0, 0, WAIT_TYPE_MS, 3 },
};

/*******************************************************************************
 ** LCD IOMUX
 */
static struct pinctrl_data g_pctrl;

static struct pinctrl_cmd_desc g_lcd_pinctrl_init_cmds[] = {
	{DTYPE_PINCTRL_GET, &g_pctrl, 0},
	{DTYPE_PINCTRL_STATE_GET, &g_pctrl, DTYPE_PINCTRL_STATE_DEFAULT},
	{DTYPE_PINCTRL_STATE_GET, &g_pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_normal_cmds[] = {
	{DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_DEFAULT},
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_lowpower_cmds[] = {
	{DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_finit_cmds[] = {
	{DTYPE_PINCTRL_PUT, &g_pctrl, 0},
};

static void panel_drv_private_data_setup(struct panel_drv_private *priv, struct device_node *np)
{
	g_gpio_amoled_reset = (uint32_t)of_get_named_gpio(np, "gpios", 0);
    g_gpio_amoled_te0 = (uint32_t)of_get_named_gpio(np, "gpios", 1);

	dpu_pr_info("used gpio:[rst: %d, te: %d]\n", g_gpio_amoled_reset, g_gpio_amoled_te0);
	priv->gpio_request_cmds = g_lcd_gpio_request_cmds;
	priv->gpio_request_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_gpio_request_cmds);
	priv->gpio_free_cmds = g_lcd_gpio_free_cmds;
	priv->gpio_free_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_gpio_free_cmds);

	priv->gpio_normal_cmds = g_lcd_gpio_normal_cmds;
	priv->gpio_normal_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_gpio_normal_cmds);
	priv->gpio_lowpower_cmds = g_lcd_gpio_lowpower_cmds;
	priv->gpio_lowpower_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_gpio_lowpower_cmds);

	priv->vcc_init_cmds = g_lcd_vcc_init_cmds;
	priv->vcc_init_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_vcc_init_cmds);
	priv->vcc_finit_cmds = g_lcd_vcc_finit_cmds;
	priv->vcc_finit_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_vcc_finit_cmds);

	priv->vcc_enable_cmds = g_lcd_vcc_enable_cmds;
	priv->vcc_enable_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_vcc_enable_cmds);
	priv->vcc_disable_cmds = g_lcd_vcc_disable_cmds;
	priv->vcc_disable_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_vcc_disable_cmds);

	priv->pinctrl_init_cmds = g_lcd_pinctrl_init_cmds;
	priv->pinctrl_init_cmds_len = 0;
	priv->pinctrl_finit_cmds = g_lcd_pinctrl_finit_cmds;
	priv->pinctrl_finit_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_pinctrl_finit_cmds);

	priv->pinctrl_normal_cmds = g_lcd_pinctrl_normal_cmds;
	priv->pinctrl_normal_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_pinctrl_normal_cmds);
	priv->pinctrl_lowpower_cmds = g_lcd_pinctrl_lowpower_cmds;
	priv->pinctrl_lowpower_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_pinctrl_lowpower_cmds);
}

#ifdef ENABLE_DSC
static void dsc_config(struct dkmd_connector_info *pinfo, struct dsc_calc_info *dsc)
{
	// IFBC_TYPE_VESA2X_SINGLE
	// IFBC_TYPE_VESA2X_DUAL
	// IFBC_TYPE_VESA3X_DUAL;
	// IFBC_TYPE_VESA3X_SINGLE;
	// IFBC_TYPE_VESA3_75X_SINGLE;
	// IFBC_TYPE_VESA3_75X_DUAL
	pinfo->ifbc_type = IFBC_TYPE_VESA3_75X_DUAL;
	dsc->dsc_en = 1;

	dsc->dsc_info.dsc_version_major = 1;
	dsc->dsc_info.dsc_version_minor = 1;
	dsc->dsc_info.pic_height = pinfo->base.yres;
	dsc->dsc_info.pic_width = pinfo->base.xres;
	dsc->dsc_info.dsc_bpc = 10; // 10:3.75x; 8:3x
	dsc->dsc_info.dsc_bpp = 8;
	dsc->dsc_info.slice_height = 8;
	dsc->dsc_info.slice_width = pinfo->base.xres / 2;
	dsc->dsc_info.chunk_size = dsc->dsc_info.dsc_bpp * (dsc->dsc_info.slice_width) / 8;

	/* DSC_CTRL */
	dsc->dsc_info.block_pred_enable = 1;
	dsc->dsc_info.linebuf_depth = 11;

	/* INITIAL_DELAY */
	dsc->dsc_info.initial_dec_delay = 0x0232;
	dsc->dsc_info.initial_xmit_delay = 512;
	/* RC_PARAM0 */
	dsc->dsc_info.initial_scale_value = 0x20;
	dsc->dsc_info.scale_increment_interval = 0x00c6;
	/* RC_PARAM1 */
	dsc->dsc_info.scale_decrement_interval = 0x8;
	dsc->dsc_info.first_line_bpg_offset = 12;
	/* RC_PARAM2 */
	dsc->dsc_info.nfl_bpg_offset = 0x0db7;
	dsc->dsc_info.slice_bpg_offset = 0x0b38;
	/* RC_PARAM3 */
	dsc->dsc_info.initial_offset = 6144;
	dsc->dsc_info.final_offset = 0x10f0;
	/* FLATNESS_QP_TH */
	dsc->dsc_info.flatness_min_qp = 7;
	dsc->dsc_info.flatness_max_qp = 16;
	/* DSC_PARAM4 */
	dsc->dsc_info.rc_edge_factor = 0x6;
	dsc->dsc_info.rc_model_size = 8192;
	/* DSC_RC_PARAM5: 0x330f0f */
	dsc->dsc_info.rc_tgt_offset_lo = (0x330f0f >> 20) & 0xF;
	dsc->dsc_info.rc_tgt_offset_hi = (0x330f0f >> 16) & 0xF;
	dsc->dsc_info.rc_quant_incr_limit1 = (0x330f0f >> 8) & 0x1F;
	dsc->dsc_info.rc_quant_incr_limit0 = (0x330f0f >> 0) & 0x1F;
	/* DSC_RC_BUF_THRESH0: 0xe1c2a38 */
	dsc->dsc_info.rc_buf_thresh[0] = (0xe1c2a38 >> 24) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[1] = (0xe1c2a38 >> 16) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[2] = (0xe1c2a38 >> 8) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[3] = (0xe1c2a38 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH1: 0x46546269 */
	dsc->dsc_info.rc_buf_thresh[4] = (0x46546269 >> 24) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[5] = (0x46546269 >> 16) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[6] = (0x46546269 >> 8) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[7] = (0x46546269 >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH2: 0x7077797b */
	dsc->dsc_info.rc_buf_thresh[8] = (0x7077797b >> 24) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[9] = (0x7077797b >> 16) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[10] = (0x7077797b >> 8) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[11] = (0x7077797b >> 0) & 0xFF;
	/* DSC_RC_BUF_THRESH3: 0x7d7e0000 */
	dsc->dsc_info.rc_buf_thresh[12] = (0x7d7e0000 >> 24) & 0xFF;
	dsc->dsc_info.rc_buf_thresh[13] = (0x7d7e0000 >> 16) & 0xFF;
	/* DSC_RC_RANGE_PARAM0: 0x2022200 */
	dsc->dsc_info.rc_range[0].min_qp = 0;
	dsc->dsc_info.rc_range[0].max_qp = 8;
	dsc->dsc_info.rc_range[0].offset = 2;
	dsc->dsc_info.rc_range[1].min_qp = 4;
	dsc->dsc_info.rc_range[1].max_qp = 8;
	dsc->dsc_info.rc_range[1].offset = 0;
	/* DSC_RC_RANGE_PARAM1: 0x2a402abe */
	dsc->dsc_info.rc_range[2].min_qp = 5;
	dsc->dsc_info.rc_range[2].max_qp = 9;
	dsc->dsc_info.rc_range[2].offset = 0;
	dsc->dsc_info.rc_range[3].min_qp = 5;
	dsc->dsc_info.rc_range[3].max_qp = 10;
	dsc->dsc_info.rc_range[3].offset = 62;
	/* DSC_RC_RANGE_PARAM2, 0x3afc3afa */
	dsc->dsc_info.rc_range[4].min_qp = 7;
	dsc->dsc_info.rc_range[4].max_qp = 11;
	dsc->dsc_info.rc_range[4].offset = 60;
	dsc->dsc_info.rc_range[5].min_qp = 7;
	dsc->dsc_info.rc_range[5].max_qp = 11;
	dsc->dsc_info.rc_range[5].offset = 58;
	/* DSC_RC_RANGE_PARAM3, 0x3af83b38 */
	dsc->dsc_info.rc_range[6].min_qp = 7;
	dsc->dsc_info.rc_range[6].max_qp = 11;
	dsc->dsc_info.rc_range[6].offset = 56;
	dsc->dsc_info.rc_range[7].min_qp = 7;
	dsc->dsc_info.rc_range[7].max_qp = 12;
	dsc->dsc_info.rc_range[7].offset = 56;
	/* DSC_RC_RANGE_PARAM4, 0x3b783bb6 */
	dsc->dsc_info.rc_range[8].min_qp = 7;
	dsc->dsc_info.rc_range[8].max_qp = 13;
	dsc->dsc_info.rc_range[8].offset = 56;
	dsc->dsc_info.rc_range[9].min_qp = 8;
	dsc->dsc_info.rc_range[9].max_qp = 14;
	dsc->dsc_info.rc_range[9].offset = 54;
	/* DSC_RC_RANGE_PARAM5, 0x4bf64c34 */
	dsc->dsc_info.rc_range[10].min_qp = 9;
	dsc->dsc_info.rc_range[10].max_qp = 14;
	dsc->dsc_info.rc_range[10].offset = 54;
	dsc->dsc_info.rc_range[11].min_qp = 9;
	dsc->dsc_info.rc_range[11].max_qp = 15;
	dsc->dsc_info.rc_range[11].offset = 54;
	/* DSC_RC_RANGE_PARAM6, 0x4c745c74 */
	dsc->dsc_info.rc_range[12].min_qp = 9;
	dsc->dsc_info.rc_range[12].max_qp = 15;
	dsc->dsc_info.rc_range[12].offset = 52;
	dsc->dsc_info.rc_range[13].min_qp = 12;
	dsc->dsc_info.rc_range[13].max_qp = 16;
	dsc->dsc_info.rc_range[13].offset = 52;
	/* DSC_RC_RANGE_PARAM7, 0x8cf40000 */
	dsc->dsc_info.rc_range[14].min_qp = 16;
	dsc->dsc_info.rc_range[14].max_qp = 17;
	dsc->dsc_info.rc_range[14].offset = 52;
}
#endif

static void mipi_lcd_init_dsi_param(struct dkmd_connector_info *pinfo, struct mipi_panel_info *mipi)
{
	dpu_pr_info("[tcl_rm692h5 probe] udp mipi param fpga flag %d\n", pinfo->base.fpga_flag);
	if (pinfo->base.fpga_flag == 1) {
		dpu_pr_info("[tcl_rm692h5 probe] udp mipi param set dsc en\n");
		mipi->hsa = 32;
		mipi->hbp = 32;
		mipi->dpi_hsize = 308;
		mipi->hline_time = 412;
		mipi->vsa = 4;
		mipi->vbp = 12;
		mipi->vfp = 16;

		mipi->dsi_bit_clk = 120;
		mipi->pxl_clk_rate = 20 * 1000000UL;
	} else {
		dpu_pr_info("[tcl_rm692h5 probe] udp mipi param set dsc en\n");
		mipi->hsa = 16;
		mipi->hbp = 16;
		mipi->dpi_hsize = 308;
		mipi->hline_time = 352;
		mipi->vsa = 42;
		mipi->vbp = 70;
		mipi->vfp = 36;

		mipi->dsi_bit_clk = 510;
		mipi->pxl_clk_rate = 136 * 1000000UL;
	}

	mipi->dsi_bit_clk_upt_support = 0;
	mipi->dsi_bit_clk_upt = mipi->dsi_bit_clk;
	mipi->dsi_bit_clk_default = mipi->dsi_bit_clk;
	mipi->pxl_clk_rate_div = 1;

	mipi->clk_post_adjust = 16;
	mipi->lane_nums = DSI_4_LANES;
	mipi->color_mode = DSI_24BITS_1;

	mipi->vc = 0;
	mipi->max_tx_esc_clk = 10 * 1000000;
	mipi->burst_mode = DSI_BURST_SYNC_PULSES_1;
	mipi->non_continue_en = 1;
	mipi->phy_mode = DPHY_MODE;
	mipi->dsi_version = DSI_1_1_VERSION;
}

/* dirty region initialized value from panel spec */
static void lcd_init_dirty_region(struct panel_drv_private *priv)
{
	priv->user_pinfo.left_align = -1;
	priv->user_pinfo.right_align = -1;
	priv->user_pinfo.top_align = 32;
	priv->user_pinfo.bottom_align = 32;
	priv->user_pinfo.w_align = -1;
	priv->user_pinfo.h_align = -1;
	priv->user_pinfo.w_min = 1224;
	priv->user_pinfo.h_min = -1;
	priv->user_pinfo.top_start = -1;
	priv->user_pinfo.bottom_start = -1;
	priv->user_pinfo.dirty_region_updt_support = priv->connector_info.dirty_region_updt_support;
}
 
static int32_t panel_of_device_setup(struct panel_drv_private *priv)
{
	int32_t ret;
	struct dkmd_connector_info *pinfo = &priv->connector_info;
	struct device_node *np = priv->pdev->dev.of_node;
	struct dpu_connector *connector = NULL;

	connector = get_primary_connector(pinfo);
	if (!connector) {
		dpu_pr_err("connector_id=%u is not available!\n", pinfo->connector_idx[PRIMARY_CONNECT_CHN_IDX]);
		return -1;
	}

	dpu_pr_info("enter!\n");

	/* Inheritance based processing */
	panel_base_of_device_setup(priv);
	panel_drv_private_data_setup(priv, np);
	pinfo->ifbc_type = IFBC_TYPE_VESA3_75X_DUAL;

	/* 1. config base object info
	 * would be used for framebuffer setup
	 */
	pinfo->base.xres = 1224;
	pinfo->base.yres = 2776;

	/* When calculating DPI needs the following parameters */
	pinfo->base.width = 69;
	pinfo->base.height = 157;
	if (pinfo->base.fpga_flag == 1) {
		/* report to surfaceFlinger, remain: caculate fps by mipi timing para */
		pinfo->base.fps = 60;
	} else {
		pinfo->base.fps = 120;
	}

	/* 2. config connector info
	 * would be used for dsi & composer setup
	 */
#ifdef ENABLE_DSC
	dsc_config(pinfo, &connector->post_info[0]->dsc);
#endif
	mipi_lcd_init_dsi_param(pinfo, &connector->post_info[0]->mipi);

	/* dsi or composer need this param */
	pinfo->dirty_region_updt_support = 0;
	pinfo->vsync_ctrl_type = VSYNC_IDLE_NONE;

	/* 3. config panel private info
	 * would be used for panel setup
	 */
	pinfo->bl_info.bl_min = 3;
	pinfo->bl_info.bl_max = 4095;
	pinfo->bl_info.bl_default = 2047;
	priv->mipi_brightness_para_type = MIPI_BL_PARA1_DBV8_AND_PARA2_DBV0;

	lcd_init_dirty_region(priv);
	
	priv->disp_on_cmds = g_lcd_display_on_cmds;
	priv->disp_on_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_display_on_cmds);
	priv->disp_off_cmds = g_lcd_display_off_cmds;
	priv->disp_off_cmds_len = (uint32_t)ARRAY_SIZE(g_lcd_display_off_cmds);

	if (pinfo->base.fpga_flag == 0) {
		ret = peri_vcc_cmds_tx(priv->pdev, priv->vcc_init_cmds, (int32_t)(priv->vcc_init_cmds_len));
		if (ret != 0)
			dpu_pr_info("vcc init failed!\n");

		ret = peri_pinctrl_cmds_tx(priv->pdev, priv->pinctrl_init_cmds, (int32_t)(priv->pinctrl_init_cmds_len));
		if (ret != 0)
			dpu_pr_info("pinctrl init failed\n");

		ret = peri_vcc_cmds_tx(priv->pdev, priv->vcc_enable_cmds, (int32_t)(priv->vcc_enable_cmds_len));
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
		ret = peri_gpio_cmds_tx(priv->gpio_free_cmds, (int32_t)(priv->gpio_free_cmds_len));
		if (ret)
			dpu_pr_info("gpio free handle err!\n");
	}

	dpu_pr_info("exit!\n");
}

panel_device_match_data(rm692h5_panel_info, PANEL_RM692H5_ID, panel_of_device_setup, panel_of_device_release);

MODULE_LICENSE("GPL");
