/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "dpu_conn_mgr.h"
#include "panel_mgr.h"
#include "panel_drv.h"
#include "panel_dev.h"

#define USE_FOR_FPGA 1
#define USE_FOR_UDP 0

/*******************************************************************************
** Display ON/OFF Sequence begin
*/
/* Power ON Sequence(sleep mode to Normal mode) */
static char boe_poweron_param1[] = {
	0xfe,
	0x00,
};

static char boe_poweron_param2[] = {
	0x67,
	0x42,
};

static char boe_poweron_param3[] = {
	0x68,
	0x60,
};

static char boe_poweron_param4[] = {
	0x35,
	0x00,
};

static char boe_poweron_param5[] = {
	0x53,
	0x20,
};

static char boe_poweron_param6[] = {
	0x2a,
	0x00, 0x08, 0x01, 0xD9,
};

static char boe_poweron_param7[] = {
	0x2b,
	0x00, 0x00, 0x01, 0xd1,
};

static char boe_poweron_param8[] = {
	0xFE,
	0x82,
};

static char boe_poweron_param9[] = {
	0x52,
	0x01,
};

static char boe_poweron_param10[] = {
	0xFE,
	0x92,
};

static char boe_poweron_param11[] = {
	0xB1,
	0x2D,
};

static char boe_poweron_param12[] = {
	0xFE,
	0xB0,
};

static char boe_poweron_param13[] = {
	0x21,
	0x11,
};

static char boe_poweron_param14[] = {
	0xFE,
	0xD0,
};

static char boe_poweron_param15[] = {
	0x30,
	0x09,
};

static char boe_poweron_param16[] = {
	0xFE,
	0x00,
};

static char boe_poweron_param17[] = {
	0xF2,
	0xFF,
};

static char boe_poweron_param18[] = {
	0x11,
	0x00,
};

static char boe_poweron_param19[] = {
	0x29,
	0x00,
};

/* Power OFF Sequence(Normal to power off) */
static char g_off_cmd0[] = {
	0xFE,
	0x00,
};

static char g_off_cmd1[] = {
	0x28,
	0x00,
};

static char g_off_cmd2[] = {
	0x10,
	0x00,
};

/* power on sequence */
static struct dsi_cmd_desc lcd_display_on_cmds[] = {
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param1), boe_poweron_param1 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param2), boe_poweron_param2 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param3), boe_poweron_param3 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param4), boe_poweron_param4 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param5), boe_poweron_param5 },
	{ DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param6), boe_poweron_param6 },
	{ DTYPE_DCS_LWRITE, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param7), boe_poweron_param7 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param8), boe_poweron_param8 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param9), boe_poweron_param9 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param10), boe_poweron_param10 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param11), boe_poweron_param11 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param12), boe_poweron_param12 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param13), boe_poweron_param13 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param14), boe_poweron_param14 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param15), boe_poweron_param15 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param16), boe_poweron_param16 },
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(boe_poweron_param17), boe_poweron_param17 },
	{ DTYPE_DCS_WRITE, 0, 60, WAIT_TYPE_MS,
		sizeof(boe_poweron_param18), boe_poweron_param18 },
	{ DTYPE_DCS_WRITE, 0, 64, WAIT_TYPE_US,
		sizeof(boe_poweron_param19), boe_poweron_param19 },
};

/* power off sequence */
static struct dsi_cmd_desc lcd_display_off_cmds[] = {
	{ DTYPE_DCS_WRITE1, 0, 0, WAIT_TYPE_US,
		sizeof(g_off_cmd0), g_off_cmd0 },
	{ DTYPE_DCS_WRITE, 0, 20, WAIT_TYPE_US,
		sizeof(g_off_cmd1), g_off_cmd1 },
	{ DTYPE_DCS_WRITE, 0, 83, WAIT_TYPE_US,
		sizeof(g_off_cmd2), g_off_cmd2 },
};

/*******************************************************************************
** LCD GPIO
*/
#define GPIO_AMOLED_VCC1V8_NAME "gpio_amoled_vcc1v8"
#define GPIO_AMOLED_VCC3V1_NAME "gpio_amoled_vcc3v1"
#define GPIO_AMOLED_VCI_NAME	"gpio_amoled_vci"
#define GPIO_AMOLED_TE0_NAME    "gpio_amoled_te0"
#define GPIO_AMOLED_RESET_NAME  "gpio_amoled_reset"

static uint32_t g_gpio_amoled_vcc1v8;
static uint32_t g_gpio_amoled_vcc3v1;
static uint32_t g_gpio_amoled_vci;
static uint32_t g_gpio_amoled_te0;
static uint32_t g_gpio_amoled_reset;

static struct gpio_desc g_fpga_lcd_gpio_request_cmds[] = {
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_VCC1V8_NAME, &g_gpio_amoled_vcc1v8, 0},
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
};

static struct gpio_desc g_fpga_lcd_gpio_free_cmds[] = {
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 0,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 0,
		GPIO_AMOLED_VCC1V8_NAME, &g_gpio_amoled_vcc1v8, 0},
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 0,
		GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
};

static struct gpio_desc g_fpga_lcd_gpio_lowpower_cmds[] = {
	/* backlight enable */
	/* reset */
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 5,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	/* vcc3v1 disable */
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10,
		GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	/* vcc1v8 disable */
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10,
		GPIO_AMOLED_VCC1V8_NAME, &g_gpio_amoled_vcc1v8, 0},

	/* backlight enable input */
	/* reset input */
	{ DTYPE_GPIO_INPUT, WAIT_TYPE_US, 100,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	/* vcc3v1 disable */
	{ DTYPE_GPIO_INPUT, WAIT_TYPE_US, 100,
		GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 0},
	/* vcc1v8 disable */
	{ DTYPE_GPIO_INPUT, WAIT_TYPE_US, 100,
		GPIO_AMOLED_VCC1V8_NAME, &g_gpio_amoled_vcc1v8, 0},
};

static struct gpio_desc g_fpga_lcd_gpio_normal_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 10,
		GPIO_AMOLED_VCC3V1_NAME, &g_gpio_amoled_vcc3v1, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_VCC1V8_NAME, &g_gpio_amoled_vcc1v8, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 15,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 10,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 50,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
};

static struct gpio_desc g_asic_lcd_gpio_request_cmds[] = {
	{DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_VCI_NAME, &g_gpio_amoled_vci, 0},
	{ DTYPE_GPIO_REQUEST, WAIT_TYPE_MS, 0,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
};

static struct gpio_desc g_asic_lcd_gpio_free_cmds[] = {
	{DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_VCI_NAME, &g_gpio_amoled_vci, 0},
	{ DTYPE_GPIO_FREE, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
};

static struct gpio_desc g_asic_lcd_gpio_normal_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_VCI_NAME, &g_gpio_amoled_vci, 1},
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_US, 50,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{ DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 25,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 1},
};

static struct gpio_desc g_asic_lcd_gpio_lowpower_cmds[] = {
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_RESET_NAME, &g_gpio_amoled_reset, 0},
	{DTYPE_GPIO_OUTPUT, WAIT_TYPE_MS, 20,
		GPIO_AMOLED_VCI_NAME, &g_gpio_amoled_vci, 0 },
};

/*******************************************************************************
** LCD VCC
*/
static struct vcc_desc g_lcd_vcc_init_cmds[] = {
};

static struct vcc_desc g_lcd_vcc_enable_cmds[] = {
};

static struct vcc_desc g_lcd_vcc_disable_cmds[] = {
};

static struct vcc_desc g_lcd_vcc_finit_cmds[] = {
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
	{ DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_DEFAULT },
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_lowpower_cmds[] = {
	{DTYPE_PINCTRL_SET, &g_pctrl, DTYPE_PINCTRL_STATE_IDLE},
};

static struct pinctrl_cmd_desc g_lcd_pinctrl_finit_cmds[] = {
	{DTYPE_PINCTRL_PUT, &g_pctrl, 0},
};

static void panel_cmd_data_setup(struct panel_drv_private *priv, struct device_node *np)
{
	if (priv->connector_info.base.fpga_flag == USE_FOR_FPGA) {
		g_gpio_amoled_reset = (uint32_t)of_get_named_gpio(np, "gpios", 0);
		g_gpio_amoled_vcc3v1 = (uint32_t)of_get_named_gpio(np, "gpios", 1);
		g_gpio_amoled_vcc1v8 = (uint32_t)of_get_named_gpio(np, "gpios", 2);

		dpu_pr_info("used gpio:reset:%d, 3v1:%d, 1v8:%d\n",
			g_gpio_amoled_reset, g_gpio_amoled_vcc3v1, g_gpio_amoled_vcc1v8);

		priv->gpio_request_cmds = g_fpga_lcd_gpio_request_cmds;
		priv->gpio_request_cmds_len = ARRAY_SIZE(g_fpga_lcd_gpio_request_cmds);
		priv->gpio_free_cmds = g_fpga_lcd_gpio_free_cmds;
		priv->gpio_free_cmds_len = ARRAY_SIZE(g_fpga_lcd_gpio_free_cmds);

		priv->gpio_lowpower_cmds = g_fpga_lcd_gpio_lowpower_cmds;
		priv->gpio_lowpower_cmds_len = ARRAY_SIZE(g_fpga_lcd_gpio_lowpower_cmds);
		priv->gpio_normal_cmds = g_fpga_lcd_gpio_normal_cmds;
		priv->gpio_normal_cmds_len = ARRAY_SIZE(g_fpga_lcd_gpio_normal_cmds);
	} else {
		g_gpio_amoled_reset = (uint32_t)of_get_named_gpio(np, "gpios", 0);
		g_gpio_amoled_vci = (uint32_t)of_get_named_gpio(np, "gpios", 1);
		g_gpio_amoled_te0 = (uint32_t)of_get_named_gpio(np, "gpios", 2);

		dpu_pr_info("used gpio:reset:%d, vci:%d, te0:%d\n",
			g_gpio_amoled_reset, g_gpio_amoled_vci, g_gpio_amoled_te0);

		priv->gpio_request_cmds = g_asic_lcd_gpio_request_cmds;
		priv->gpio_request_cmds_len = ARRAY_SIZE(g_asic_lcd_gpio_request_cmds);
		priv->gpio_free_cmds = g_asic_lcd_gpio_free_cmds;
		priv->gpio_free_cmds_len = ARRAY_SIZE(g_asic_lcd_gpio_free_cmds);

		priv->gpio_normal_cmds = g_asic_lcd_gpio_normal_cmds;
		priv->gpio_normal_cmds_len = ARRAY_SIZE(g_asic_lcd_gpio_normal_cmds);
		priv->gpio_lowpower_cmds = g_asic_lcd_gpio_lowpower_cmds;
		priv->gpio_lowpower_cmds_len = ARRAY_SIZE(g_asic_lcd_gpio_lowpower_cmds);

		priv->vcc_init_cmds = g_lcd_vcc_init_cmds;
		priv->vcc_init_cmds_len = ARRAY_SIZE(g_lcd_vcc_init_cmds);
		priv->vcc_finit_cmds = g_lcd_vcc_finit_cmds;
		priv->vcc_finit_cmds_len = ARRAY_SIZE(g_lcd_vcc_finit_cmds);

		priv->vcc_enable_cmds = g_lcd_vcc_enable_cmds;
		priv->vcc_enable_cmds_len = ARRAY_SIZE(g_lcd_vcc_enable_cmds);
		priv->vcc_disable_cmds = g_lcd_vcc_disable_cmds;
		priv->vcc_disable_cmds_len = ARRAY_SIZE(g_lcd_vcc_disable_cmds);

		priv->pinctrl_init_cmds = g_lcd_pinctrl_init_cmds;
		priv->pinctrl_init_cmds_len = ARRAY_SIZE(g_lcd_pinctrl_init_cmds);
		priv->pinctrl_finit_cmds = g_lcd_pinctrl_finit_cmds;
		priv->pinctrl_finit_cmds_len = ARRAY_SIZE(g_lcd_pinctrl_finit_cmds);

		priv->pinctrl_normal_cmds = g_lcd_pinctrl_normal_cmds;
		priv->pinctrl_normal_cmds_len = ARRAY_SIZE(g_lcd_pinctrl_normal_cmds);
		priv->pinctrl_lowpower_cmds = g_lcd_pinctrl_lowpower_cmds;
		priv->pinctrl_lowpower_cmds_len = ARRAY_SIZE(g_lcd_pinctrl_lowpower_cmds);
	}
}

/* dsi param initialized value from panel spec */
static void mipi_lcd_init_dsi_param(struct dkmd_connector_info *pinfo, struct mipi_panel_info *mipi)
{
	if (pinfo->base.fpga_flag == USE_FOR_FPGA) {
		/* ldi */
		dpu_pr_info("fpga!\n");
		mipi->hsa = 11;
		mipi->hbp = 37;
		mipi->dpi_hsize = 812;
		mipi->hline_time = 936;
		mipi->vsa = 10;
		mipi->vbp = 54;
		mipi->vfp = 36;

		/* mipi */
		mipi->dsi_bit_clk = 120;
		mipi->pxl_clk_rate = 20 * 1000000UL;
	} else {
		dpu_pr_info("udp!\n");
		/* ldi */
		mipi->hsa = 2;
		mipi->hbp = 10;
		mipi->dpi_hsize = 1404;
		mipi->hline_time = 1947;
		mipi->vsa = 2;
		mipi->vbp = 25;
		mipi->vfp = 17;

		mipi->pxl_clk_rate = 20 * 1000000UL;
		/* mipi */
		mipi->dsi_bit_clk = 246;
		mipi->dsi_bit_clk_val1 = 200;
		mipi->dsi_bit_clk_val2 = 209;
		mipi->dsi_bit_clk_val3 = 219;
		mipi->dsi_bit_clk_val4 = 230;
		mipi->dsi_bit_clk_val5 = 240;
	}

	mipi->dsi_bit_clk_upt_support = 1;
	mipi->clk_post_adjust = 215;
	mipi->dsi_bit_clk_upt = mipi->dsi_bit_clk;
	mipi->dsi_bit_clk_default = mipi->dsi_bit_clk;

	mipi->lane_nums = DSI_1_LANES;
	mipi->color_mode = DSI_24BITS_1;
	mipi->vc = 0;
	mipi->max_tx_esc_clk = 6 * 1000000;
	mipi->burst_mode = DSI_BURST_SYNC_PULSES_1;
	mipi->non_continue_en = 1;
	mipi->pxl_clk_rate_div = 1;
	mipi->phy_mode = DPHY_MODE;
}

/* dirty region initialized value from panel spec */
static void lcd_init_dirty_region(struct panel_drv_private *priv)
{
	priv->user_pinfo.left_align = -1;
	priv->user_pinfo.right_align = -1;
	priv->user_pinfo.top_align = 2;
	priv->user_pinfo.bottom_align = 2;
	priv->user_pinfo.w_align = -1;
	priv->user_pinfo.h_align = 2;
	priv->user_pinfo.w_min = 466;
	priv->user_pinfo.h_min = -1;
	priv->user_pinfo.top_start = -1;
	priv->user_pinfo.bottom_start = -1;
	priv->user_pinfo.dirty_region_updt_support = priv->connector_info.dirty_region_updt_support;
}

/* replace function of lcd_set_display_region registered in panel_dev.c */
static int32_t customized_lcd_set_display_region(struct panel_drv_private *priv,
	struct dkmd_connector_info *pinfo, const void *value)
{
	char lcd_disp_x[] = {
		0x2A,
		0x00, 0x08,0x01,0xD9
	};
	char lcd_disp_y[] = {
		0x2B,
		0x00, 0x00,0x01,0xD1
	};
	struct dsi_cmd_desc set_display_address[] = {
		{DTYPE_DCS_LWRITE, 0, 5, WAIT_TYPE_US,
			sizeof(lcd_disp_x), lcd_disp_x},
		{DTYPE_DCS_LWRITE, 0, 5, WAIT_TYPE_US,
			sizeof(lcd_disp_y), lcd_disp_y},
	};
	const uint32_t panel_x_coor_offset = 8;
	struct dkmd_rect *dirty = (struct dkmd_rect *)value;

	dpu_check_and_return(!dirty, -EINVAL, err, "dirty rect is null!");
	dpu_pr_info("dirty_rect[%d %d %d %d]", dirty->x, dirty->y, dirty->w, dirty->h);

	lcd_disp_x[1] = (((uint32_t)dirty->x + panel_x_coor_offset) >> 8) & 0xff;
	lcd_disp_x[2] = (((uint32_t)dirty->x + panel_x_coor_offset)) & 0xff;
	lcd_disp_x[3] = (((uint32_t)dirty->x + panel_x_coor_offset + dirty->w - 1) >> 8) & 0xff;
	lcd_disp_x[4] = (((uint32_t)dirty->x + panel_x_coor_offset + dirty->w - 1)) & 0xff;
	lcd_disp_y[1] = (((uint32_t)dirty->y) >> 8) & 0xff;
	lcd_disp_y[2] = (((uint32_t)dirty->y)) & 0xff;
	lcd_disp_y[3] = (((uint32_t)dirty->y + dirty->h - 1) >> 8) & 0xff;
	lcd_disp_y[4] = (((uint32_t)dirty->y + dirty->h - 1)) & 0xff;

	mipi_dsi_cmds_tx(set_display_address,
		ARRAY_SIZE(set_display_address), get_primary_connector(pinfo)->connector_base);

	return 0;
}

static int32_t panel_of_device_setup(struct panel_drv_private *priv)
{
	int32_t ret;
	struct dkmd_connector_info *pinfo = &priv->connector_info;
	struct device_node *np = priv->pdev->dev.of_node;

	dpu_pr_info("enter!\n");

	/* Inheritance based processing */
	panel_base_of_device_setup(priv);

	panel_cmd_data_setup(priv, np);

	/* 1. config base object info
	 * would be used for framebuffer setup
	 */
	pinfo->base.xres = 466; // FIXME: Modified for new panel device
	pinfo->base.yres = 466; // FIXME: Modified for new panel device

	/* When calculating DPI needs the following parameters */
	pinfo->base.width = 36; // FIXME: Modified for new panel device
	pinfo->base.height = 36; // FIXME: Modified for new panel device

	if (pinfo->base.fpga_flag == USE_FOR_FPGA) {
		pinfo->base.fps = 15;
	} else {
		pinfo->base.fps = 60;
	}

	/* 2. config connector info
	 * would be used for dsi & composer setup
	 */
	mipi_lcd_init_dsi_param(pinfo, &get_primary_connector(pinfo)->post_info[0]->mipi);

	/* dsi or composer need this param */
	pinfo->dirty_region_updt_support = 1;
    pinfo->vsync_ctrl_type = VSYNC_IDLE_MIPI_ULPS | VSYNC_IDLE_CLK_OFF | VSYNC_IDLE_ISR_OFF;
	/* 3. config panel private info
	 * would be used for panel setup
	 */
	pinfo->bl_info.bl_min = 1;
	pinfo->bl_info.bl_max = 255;
	pinfo->bl_info.bl_default = 200;
	priv->mipi_brightness_para_type = MIPI_BL_PARA1_DBV0;

	lcd_init_dirty_region(priv);
	register_customized_ops_handle(LCD_SET_DISPLAY_REGION, customized_lcd_set_display_region);

	priv->disp_on_cmds = lcd_display_on_cmds;
	priv->disp_on_cmds_len = ARRAY_SIZE(lcd_display_on_cmds);
	priv->disp_off_cmds = lcd_display_off_cmds;
	priv->disp_off_cmds_len = ARRAY_SIZE(lcd_display_off_cmds);

	if (pinfo->base.fpga_flag == USE_FOR_UDP) {
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

panel_device_match_data(rm6d030_panel_info, PANEL_RM6d030_ID, panel_of_device_setup, panel_of_device_release);

MODULE_LICENSE("GPL");
