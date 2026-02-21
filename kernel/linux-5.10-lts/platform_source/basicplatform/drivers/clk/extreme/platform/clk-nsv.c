/*
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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
#include "../clk.h"
#include "../clk-fast-dvfs.h"
#include "../clk_sec.h"
#include "clk-nsv.h"
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/clk-provider.h>

static const struct fixed_rate_clock fixed_clks[] = {
	{ CLKIN_SYS, "clkin_sys", NULL, 0, 38400000, "clkin_sys" },
	{ CLKIN_SYS_SERDES, "clkin_sys_serdes", NULL, 0, 38400000, "clkin_sys_serdes" },
	{ CLKIN_REF, "clkin_ref", NULL, 0, 32764, "clkin_ref" },
	{ CLK_FLL_SRC, "clk_fll_src", NULL, 0, 258053242, "clk_fll_src" },
	{ CLK_PPLL1, "clk_ppll1", NULL, 0, 1440000000, "clk_ppll1" },
	{ CLK_PPLL2, "clk_ppll2", NULL, 0, 964000000, "clk_ppll2" },
	{ CLK_PPLL2_B, "clk_ppll2_b", NULL, 0, 1280000000, "clk_ppll2_b" },
	{ CLK_PPLL3, "clk_ppll3", NULL, 0, 1200000000, "clk_ppll3" },
	{ CLK_PPLL5, "clk_ppll5", NULL, 0, 960000000, "clk_ppll5" },
	{ CLK_PPLL6, "clk_ppll6", NULL, 0, 1320000000, "clk_ppll6" },
	{ CLK_PPLL7, "clk_ppll7", NULL, 0, 1290000000, "clk_ppll7" },
	{ CLK_SPLL, "clk_spll", NULL, 0, 1671168000, "clk_spll" },
	{ CLK_ULPPLL_1, "clk_ulppll_1", NULL, 0, 294913310, "clk_ulppll_1" },
	{ CLK_AUPLL, "clk_aupll", NULL, 0, 1572864000, "clk_aupll" },
	{ CLK_PPLL_PCIE, "clk_ppll_pcie", NULL, 0, 100000000, "clk_ppll_pcie" },
	{ CLK_SDPLL, "clk_sdpll", NULL, 0, 963000000, "clk_sdpll" },
	{ PCLK, "apb_pclk", NULL, 0, 20000000, "apb_pclk" },
	{ CLK_UART6, "uart6clk", NULL, 0, 19200000, "uart6clk" },
	{ OSC32K, "osc32khz", NULL, 0, 32764, "osc32khz" },
	{ OSC19M, "osc19mhz", NULL, 0, 19200000, "osc19mhz" },
	{ CLK_480M, "clk_480m", NULL, 0, 480000000, "clk_480m" },
	{ CLK_INVALID, "clk_invalid", NULL, 0, 10000000, "clk_invalid" },
	{ CLK_FPGA_1P92, "clk_fpga_1p92", NULL, 0, 1920000, "clk_fpga_1p92" },
	{ CLK_FPGA_2M, "clk_fpga_2m", NULL, 0, 2000000, "clk_fpga_2m" },
	{ CLK_FPGA_10M, "clk_fpga_10m", NULL, 0, 10000000, "clk_fpga_10m" },
	{ CLK_FPGA_19M, "clk_fpga_19m", NULL, 0, 19200000, "clk_fpga_19m" },
	{ CLK_FPGA_20M, "clk_fpga_20m", NULL, 0, 20000000, "clk_fpga_20m" },
	{ CLK_FPGA_24M, "clk_fpga_24m", NULL, 0, 24000000, "clk_fpga_24m" },
	{ CLK_FPGA_26M, "clk_fpga_26m", NULL, 0, 26000000, "clk_fpga_26m" },
	{ CLK_FPGA_27M, "clk_fpga_27m", NULL, 0, 27000000, "clk_fpga_27m" },
	{ CLK_FPGA_32M, "clk_fpga_32m", NULL, 0, 32000000, "clk_fpga_32m" },
	{ CLK_FPGA_40M, "clk_fpga_40m", NULL, 0, 40000000, "clk_fpga_40m" },
	{ CLK_FPGA_48M, "clk_fpga_48m", NULL, 0, 48000000, "clk_fpga_48m" },
	{ CLK_FPGA_50M, "clk_fpga_50m", NULL, 0, 50000000, "clk_fpga_50m" },
	{ CLK_FPGA_57M, "clk_fpga_57m", NULL, 0, 57000000, "clk_fpga_57m" },
	{ CLK_FPGA_60M, "clk_fpga_60m", NULL, 0, 60000000, "clk_fpga_60m" },
	{ CLK_FPGA_64M, "clk_fpga_64m", NULL, 0, 64000000, "clk_fpga_64m" },
	{ CLK_FPGA_80M, "clk_fpga_80m", NULL, 0, 80000000, "clk_fpga_80m" },
	{ CLK_FPGA_100M, "clk_fpga_100m", NULL, 0, 100000000, "clk_fpga_100m" },
	{ CLK_FPGA_160M, "clk_fpga_160m", NULL, 0, 160000000, "clk_fpga_160m" },
};

static const struct fixed_factor_clock fixed_factor_clks[] = {
	{ CLK_PPLL_EPS, "clk_ppll_eps", "clk_ap_ppll2", 1, 1, 0, "clk_ppll_eps" },
	{ CLK_GATE_HIFACE, "clk_hiface", "clk_sys_ini", 1, 1, 0, "clk_hiface" },
	{ CLK_SYS_INI, "clk_sys_ini", "clkin_sys", 1, 2, 0, "clk_sys_ini" },
	{ CLK_DIV_SYSBUS, "div_sysbus_pll", "clk_spll", 1, 7, 0, "div_sysbus_pll" },
	{ CLK_GATE_WD0_HIGH, "clk_wd0_high", "div_sysbus_pll", 1, 1, 0, "clk_wd0_high" },
	{ CLK_DIV_AOBUS_334M, "div_aobus_334M", "clk_spll", 1, 5, 0, "div_aobus_334M" },
	{ CLK_DIV_AOBUS, "sc_div_aobus", "div_aobus_334M", 1, 6, 0, "sc_div_aobus" },
	{ CLK_GATE_TIMER5_B, "clk_timer5_b", "clk_spll", 1, 1, 0, "clk_timer5_b" },
	{ HCLK_GATE_USB3OTG, "hclk_usb3otg", "clk_spll", 1, 1, 0, "hclk_usb3otg" },
	{ ACLK_GATE_USB3OTG, "aclk_usb3otg", "clk_spll", 1, 1, 0, "aclk_usb3otg" },
	{ ACLK_GATE_USB3OTG_ASYNCBRG, "aclk_asyncbrg", "clk_spll", 1, 1, 0, "aclk_asyncbrg" },
	{ ATCLK, "clk_at", "clk_atdvfs", 1, 1, 0, "clk_at" },
	{ PCLK_DBG, "pclk_dbg", "pclkdiv_dbg", 1, 1, 0, "pclk_dbg" },
	{ CLK_DIV_CSSYSDBG, "clk_cssys_div", "autodiv_sysbus", 1, 1, 0, "clk_cssys_div" },
	{ CLK_GATE_CSSYSDBG, "clk_cssysdbg", "div_sysbus_pll", 1, 1, 0, "clk_cssysdbg" },
	{ CLK_DIV_DMABUS, "clk_dmabus_div", "div_sysbus_pll", 1, 1, 0, "clk_dmabus_div" },
	{ CLK_GATE_TCLK_ODT, "tclk_odt", "clk_sys_ini", 1, 1, 0, "tclk_odt" },
	{ ACLK_GATE_ISP, "aclk_isp", "clk_invalid", 1, 1, 0, "aclk_isp" },
	{ CLK_GATE_SDIO, "clk_sdio", "clk_spll", 1, 1, 0, "clk_sdio" },
	{ HCLK_GATE_SDIO, "hclk_sdio", "clk_spll", 1, 1, 0, "hclk_sdio" },
	{ CLK_DIV_HSDT1BUS, "clk_div_hsdt1bus", "div_sysbus_pll", 1, 1, 0, "clk_div_hsdt1bus" },
	{ CLK_SD_SYS, "clk_sd_sys", "clk_sd_sys_gt", 1, 6, 0, "clk_sd_sys" },
	{ CLK_DIV_320M, "sc_div_320m", "gt_clk_320m_pll", 1, 5, 0, "sc_div_320m" },
	{ PCLK_GATE_UART0, "pclk_uart0", "clk_uart0", 1, 1, 0, "pclk_uart0" },
	{ CLK_FACTOR_UART0, "clk_uart0_fac", "clkmux_uart0", 1, 1, 0, "clk_uart0_fac" },
	{ CLK_GATE_UART3, "clk_uart3", "clk_gate_peri0", 1, 9, 0, "clk_uart3" },
	{ CLK_GATE_UART7, "clk_uart7", "clk_gate_peri0", 1, 9, 0, "clk_uart7" },
	{ CLK_GATE_I2C0, "clk_i2c0", "clk_fll_src", 1, 2, 0, "clk_i2c0" },
	{ CLK_GATE_I2C2, "clk_i2c2", "clk_fll_src", 1, 2, 0, "clk_i2c2" },
	{ CLK_GATE_IOMCU_PERI0, "clk_gate_peri0", "clk_spll", 1, 1, 0, "clk_gate_peri0" },
	{ HCLK_GATE_DCDR_MUX, "hclk_dcdr_mux", "clk_spll", 1, 1, 0, "hclk_dcdr_mux" },
	{ CLK_USB3_32K_DIV, "clkdiv_usb3_32k", "clkdiv_usb2phy_ref", 1, 586, 0, "clkdiv_usb3_32k" },
	{ CLK_GATE_SNP_USB3_SUSPEND, "snp_usb3_32k", "clk_spll", 1, 1, 0, "snp_usb3_32k" },
	{ CLK_GATE_HSDT1_USBDP_MCU_BUS, "usbdp_mcu_bus", "clk_spll", 1, 1, 0, "usbdp_mcu_bus" },
	{ ACLK_GATE_SNP_USB3, "aclk_snp_usb3", "clk_spll", 1, 1, 0, "aclk_snp_usb3" },
	{ CLK_GATE_SNP_USB3CTRL_REF, "snp_usb3crtl", "clk_spll", 1, 1, 0, "snp_usb3crtl" },
	{ CLK_USB2PHY_REF_DIV, "clkdiv_usb2phy_ref", "clkgt_usb2phy_ref", 1, 2, 0, "clkdiv_usb2phy_ref" },
	{ CLK_GATE_HSDT0_PCIE0_MCU, "clk_hsdt0_pcie0_mcu", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie0_mcu" },
	{ CLK_GATE_HSDT0_PCIE1_MCU, "clk_hsdt0_pcie1_mcu", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie1_mcu" },
	{ CLK_GATE_HSDT0_PCIE0_MCU_BUS, "clk_hsdt0_pcie0_mcu_bus", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie0_mcu_bus" },
	{ CLK_GATE_HSDT0_PCIE1_MCU_BUS, "clk_hsdt0_pcie1_mcu_bus", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie1_mcu_bus" },
	{ CLK_GATE_HSDT0_PCIE0_MCU_32K, "clk_hsdt0_pcie0_mcu_32k", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie0_mcu_32k" },
	{ CLK_GATE_HSDT0_PCIE1_MCU_32K, "clk_hsdt0_pcie1_mcu_32k", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie1_mcu_32k" },
	{ CLK_GATE_HSDT0_PCIE0_MCU_19P2, "clk_hsdt0_pcie0_mcu_19p2", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie0_mcu_19p2" },
	{ CLK_GATE_HSDT0_PCIE1_MCU_19P2, "clk_hsdt0_pcie1_mcu_19p2", "clk_spll", 1, 1, 0, "clk_hsdt0_pcie1_mcu_19p2" },
	{ CLK_HSDT_SUBSYS_INI, "clk_hsdt_subsys_ini", "clk_spll", 1, 7, 0, "clk_hsdt_subsys_ini" },
	{ CLK_GATE_UFSIO_REF, "clk_ufsio_ref", "clkin_sys", 1, 1, 0, "clk_ufsio_ref" },
	{ CLK_GATE_BLPWM, "clk_blpwm", "clk_fll_src", 1, 2, 0, "clk_blpwm" },
	{ CLK_SYSCNT_DIV, "clk_syscnt_div", "clk_sys_ini", 1, 10, 0, "clk_syscnt_div" },
	{ CLK_GATE_GPS_REF, "clk_gps_ref", "clk_sys_ini", 1, 1, 0, "clk_gps_ref" },
	{ CLK_MUX_GPS_REF, "clkmux_gps_ref", "clk_sys_ini", 1, 1, 0, "clkmux_gps_ref" },
	{ CLK_GATE_MDM2GPS0, "clk_mdm2gps0", "clk_sys_ini", 1, 1, 0, "clk_mdm2gps0" },
	{ CLK_GATE_MDM2GPS1, "clk_mdm2gps1", "clk_sys_ini", 1, 1, 0, "clk_mdm2gps1" },
	{ CLK_GATE_MDM2GPS2, "clk_mdm2gps2", "clk_sys_ini", 1, 1, 0, "clk_mdm2gps2" },
	{ EPS_VOLT_HIGH, "eps_volt_high", "peri_volt_hold", 1, 1, 0, "eps_volt_high" },
	{ EPS_VOLT_MIDDLE, "eps_volt_middle", "peri_volt_middle", 1, 1, 0, "eps_volt_middle" },
	{ EPS_VOLT_LOW, "eps_volt_low", "peri_volt_low", 1, 1, 0, "eps_volt_low" },
	{ VENC_VOLT_HOLD, "venc_volt_hold", "peri_volt_hold", 1, 1, 0, "venc_volt_hold" },
	{ VDEC_VOLT_HOLD, "vdec_volt_hold", "peri_volt_hold", 1, 1, 0, "vdec_volt_hold" },
	{ EDC_VOLT_HOLD, "edc_volt_hold", "peri_volt_hold", 1, 1, 0, "edc_volt_hold" },
	{ EFUSE_VOLT_HOLD, "efuse_volt_hold", "peri_volt_middle", 1, 1, 0, "efuse_volt_hold" },
	{ LDI0_VOLT_HOLD, "ldi0_volt_hold", "peri_volt_hold", 1, 1, 0, "ldi0_volt_hold" },
	{ SEPLAT_VOLT_HOLD, "seplat_volt_hold", "peri_volt_hold", 1, 1, 0, "seplat_volt_hold" },
	{ CLK_GATE_ISPI2C, "clk_ispi2c", "clk_sys_ini", 1, 1, 0, "clk_ispi2c" },
	{ CLK_CSI_IDI, "clk_csi_idi", "clk_ppll1_m2", 1, 1, 0, "clk_csi_idi" },
	{ CLK_GATE_JPG_FUNC, "clk_jpg_func", "clk_spll", 1, 1, 0, "clk_jpg_func" },
	{ CLK_GATE_TXDPHY1_CFG, "clk_txdphy1_cfg", "clk_sys_ini", 1, 1, 0, "clk_txdphy1_cfg" },
	{ CLK_GATE_TXDPHY1_REF, "clk_txdphy1_ref", "clkin_sys", 1, 1, 0, "clk_txdphy1_ref" },
	{ CLK_GATE_ARPPFREQ, "clk_arpp_topfreq", "clk_sys_ini", 1, 1, 0, "clk_arpp_topfreq" },
	{ CLK_GATE_ARPP, "clk_arpp_top", "clk_arpp_topfreq", 1, 1, 0, "clk_arpp_top" },
	{ ACLK_GATE_ASC, "aclk_asc", "clk_mmbuf", 1, 1, 0, "aclk_asc" },
	{ CLK_GATE_DSS_AXI_MM, "clk_dss_axi_mm", "clk_spll", 1, 3, 0, "clk_dss_axi_mm" },
	{ CLK_GATE_MMBUF, "clk_mmbuf", "clk_spll", 1, 3, 0, "clk_mmbuf" },
	{ PCLK_GATE_MMBUF, "pclk_mmbuf", "clk_spll", 1, 3, 0, "pclk_mmbuf" },
	{ CLK_GATE_DRA_REF, "clk_dra_ref", "clkin_ref", 1, 1, 0, "clk_dra_ref" },
	{ AUTODIV_ISP_DVFS, "autodiv_isp_dvfs", "autodiv_sysbus", 1, 1, 0, "autodiv_isp_dvfs" },
};

static const struct gate_clock crgctrl_gate_clks[] = {
	{ CLK_GATE_PPLL0_MEDIA, "clk_ppll0_media", "clk_spll", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll0_media" },
	{ CLK_GATE_PPLL1_MEDIA, "clk_ppll1_media", "clk_ap_ppll1", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll1_media" },
	{ CLK_GATE_PPLL2_MEDIA, "clk_ppll2_media", "clk_ap_ppll2", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll2_media" },
	{ CLK_GATE_PPLL2B_MEDIA, "clk_ppll2b_media", "clk_ap_ppll2_b", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll2b_media" },
	{ CLK_GATE_FLL_MEDIA, "clk_fll_media", "clk_fll_src", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_fll_media" },
	{ CLK_GATE_PPLL0_M2, "clk_ppll0_m2", "clk_spll", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll0_m2" },
	{ CLK_GATE_PPLL1_M2, "clk_ppll1_m2", "clk_ap_ppll1", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll1_m2" },
	{ CLK_GATE_PPLL2_M2, "clk_ppll2_m2", "clk_ap_ppll2", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll2_m2" },
	{ CLK_GATE_PPLL2B_M2, "clk_ppll2b_m2", "clk_ap_ppll2_b", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll2b_m2" },
	{ CLK_GATE_PPLL6_M2, "clk_ppll6_m2", "clk_ap_ppll6", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ppll6_m2" },
	{ CLK_GATE_FLL_M2, "clk_fll_m2", "clk_fll_src", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_fll_m2" },
	{ CLK_GATE_AUPLL_M2, "clk_aupll_m2", "clk_ap_aupll", 0, 0x0, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_aupll_m2" },
	{ PCLK_GATE_WD0_HIGH, "pclk_wd0_high", "div_sysbus_pll", 0x020, 0x10000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_wd0_high" },
	{ PCLK_GATE_WD0, "pclk_wd0", "clkin_ref", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_wd0" },
	{ PCLK_GATE_WD1, "pclk_wd1", "div_sysbus_pll", 0x020, 0x20000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_wd1" },
	{ CLK_GATE_CODECSSI, "clk_codecssi", "sel_codeccssi", 0x020, 0x4000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_codecssi" },
	{ PCLK_GATE_CODECSSI, "pclk_codecssi", "div_sysbus_pll", 0x020, 0x4000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_codecssi" },
	{ CLK_GATE_HSDT1_USBDP, "clk_hsdt1_usbdp", "div_hsdt1_usbdp", 0x040, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_hsdt1_usbdp" },
	{ PCLK_GATE_IOC, "pclk_ioc", "clkin_sys", 0x020, 0x2000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_ioc" },
	{ CLK_ATDVFS, "clk_atdvfs", "clk_cssys_div", 0, 0x0, 0,
		NULL, 1, { 0, 0, 0, 0}, { 0, 1, 2, 3, 4}, 4, 1, 0, "clk_atdvfs" },
	{ CLK_GATE_TIME_STAMP, "clk_time_stamp", "clk_timestp_div", 0x000, 0x200000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_time_stamp" },
	{ ACLK_GATE_PERF_STAT, "aclk_perf_stat", "div_sysbus_pll", 0x040, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "aclk_perf_stat" },
	{ PCLK_GATE_PERF_STAT, "pclk_perf_stat", "div_sysbus_pll", 0x040, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_perf_stat" },
	{ CLK_GATE_PERF_STAT, "clk_perf_stat", "clk_perf_div", 0x040, 0x100, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_perf_stat" },
	{ CLK_GATE_PERF_CTRL, "clk_perf_ctrl", "sc_div_320m", 0x040, 0x800, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_perf_ctrl" },
	{ CLK_GATE_DMAC, "clk_dmac", "div_sysbus_pll", 0x030, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_dmac" },
	{ CLK_GATE_ODT_ACPU, "clk_odt_acpu", "clk_dmabus_div", 0x010, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_odt_acpu" },
	{ CLK_PFA_TFT_FREQ, "clk_pfa_tft_freq", "clk_pfa_tft_div", 0x000, 0x80, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pfa_tft_freq" },
	{ HCLK_GATE_PFA_TFT, "hclk_pfa_tft", "clk_dmabus_div", 0x000, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "hclk_pfa_tft" },
	{ CLK_GATE_SD_PERI, "clk_sd_peri", "clk_div_sd_peri", 0x020, 0x200000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_sd_peri" },
	{ CLK_GATE_UART4, "clk_uart4", "clkmux_uarth", 0x020, 0x4000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_uart4" },
	{ PCLK_GATE_UART4, "pclk_uart4", "clkmux_uarth", 0x020, 0x4000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_uart4" },
	{ CLK_GATE_UART5, "clk_uart5", "clkmux_uartl", 0x020, 0x8000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_uart5" },
	{ PCLK_GATE_UART5, "pclk_uart5", "clkmux_uartl", 0x020, 0x8000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_uart5" },
	{ CLK_GATE_UART0, "clk_uart0", "clkmux_uart0", 0x020, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_uart0" },
	{ CLK_GATE_UART8, "clk_uart8", "clkmux_uart8", 0x020, 0x800, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_uart8" },
	{ PCLK_GATE_UART8, "pclk_uart8", "clkmux_uart8", 0x020, 0x800, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_uart8" },
	{ CLK_GATE_I2C2_ACPU, "clk_i2c2_acpu", "clkmux_i2c", 0x020, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i2c2_acpu" },
	{ CLK_GATE_I2C3, "clk_i2c3", "clkmux_i2c", 0x020, 0x80, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i2c3" },
	{ CLK_GATE_I2C4, "clk_i2c4", "clkmux_i2c", 0x020, 0x8000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i2c4" },
	{ CLK_GATE_I2C6_ACPU, "clk_i2c6_acpu", "clkmux_i2c", 0x010, 0x40000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i2c6_acpu" },
	{ CLK_GATE_I2C7, "clk_i2c7", "clkmux_i2c", 0x010, 0x80000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i2c7" },
	{ PCLK_GATE_I2C2, "pclk_i2c2", "clkmux_i2c", 0x020, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i2c2" },
	{ PCLK_GATE_I2C3, "pclk_i2c3", "clkmux_i2c", 0x020, 0x80, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i2c3" },
	{ PCLK_GATE_I2C4, "pclk_i2c4", "clkmux_i2c", 0x020, 0x8000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i2c4" },
	{ PCLK_GATE_I2C6_ACPU, "pclk_i2c6_acpu", "clkmux_i2c", 0x010, 0x40000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i2c6_acpu" },
	{ PCLK_GATE_I2C7, "pclk_i2c7", "clkmux_i2c", 0x010, 0x80000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i2c7" },
	{ CLK_GATE_I3C4, "clk_i3c4", "clkdiv_i3c4", 0x470, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_i3c4" },
	{ PCLK_GATE_I3C4, "pclk_i3c4", "clkdiv_i3c4", 0x470, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_i3c4" },
	{ CLK_GATE_SPI1, "clk_spi1", "clkmux_spi1", 0x020, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_spi1" },
	{ PCLK_GATE_SPI1, "pclk_spi1", "clkmux_spi1", 0x020, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_spi1" },
	{ CLK_GATE_SPI6, "clk_spi6", "clkmux_spi6", 0xE10, 0x20000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_spi6" },
	{ PCLK_GATE_SPI6, "pclk_spi6", "clkmux_spi6", 0xE10, 0x20000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_spi6" },
	{ CLK_GATE_AO_ASP, "clk_ao_asp", "clk_ao_asp_div", 0x000, 0x4000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ao_asp" },
	{ PCLK_GATE_PCTRL, "pclk_pctrl", "div_sysbus_pll", 0x020, 0x80000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_pctrl" },
	{ CLK_GATE_BLPWM1, "clk_blpwm1", "clk_ptp_div", 0x020, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_blpwm1" },
	{ CLK_GATE_BLPWM3, "clk_blpwm3", "clk_ptp_div", 0x020, 0x1000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_blpwm3" },
	{ CLK_GATE_BLPWM4, "clk_blpwm4", "clk_ptp_div", 0x020, 0x800000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_blpwm4" },
	{ PERI_VOLT_NO_AVS_L, "peri_volt_no_avs_l", "clk_sys_ini", 0, 0x0, 0,
		NULL, 1, {0}, {0}, 0, 2, 0, "peri_volt_no_avs_l" },
	{ PERI_VOLT_HOLD, "peri_volt_hold", "clk_sys_ini", 0, 0x0, 0,
		NULL, 1, { 0, 0, 0 }, { 0, 1, 2, 3 }, 3, 3, 0, "peri_volt_hold" },
	{ PERI_VOLT_MIDDLE, "peri_volt_middle", "clk_sys_ini", 0, 0x0, 0,
		NULL, 1, { 0, 0 }, { 0, 1, 2 }, 2, 4, 0, "peri_volt_middle" },
	{ PERI_VOLT_LOW, "peri_volt_low", "clk_sys_ini", 0, 0x0, 0,
		NULL, 1, {0}, { 0, 1 }, 1, 5, 0, "peri_volt_low" },
	{ CLK_GATE_DPCTRL_16M, "clk_dpctrl_16m", "div_dpctrl16m", 0x470, 0x200000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_dpctrl_16m" },
	{ CLK_GATE_DPCTRL_PIXEL, "dpctrl_pixel", "div_dpctr_pixel", 0x470, 0x400000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "dpctrl_pixel" },
	{ CLK_GATE_ISP_SNCLK0, "clk_isp_snclk0", "clk_mux_ispsn0", 0x050, 0x10000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_snclk0" },
	{ CLK_GATE_ISP_SNCLK1, "clk_isp_snclk1", "clk_mux_ispsn1", 0x050, 0x20000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_snclk1" },
	{ CLK_GATE_ISP_SNCLK2, "clk_isp_snclk2", "clk_mux_ispsn2", 0x050, 0x40000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_snclk2" },
	{ CLK_GATE_ISP_SNCLK3, "clk_isp_snclk3", "clk_mux_ispsn3", 0x040, 0x4, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_snclk3" },
	{ CLK_GATE_RXDPHY_CFG, "clk_rxdphy_cfg", "clk_rxdphy_mux", 0x050, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy_cfg" },
	{ CLK_GATE_RXDPHY0_CFG, "clk_rxdphy0_cfg", "clk_rxdphy_cfg", 0x050, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy0_cfg" },
	{ CLK_GATE_RXDPHY1_CFG, "clk_rxdphy1_cfg", "clk_rxdphy_cfg", 0x050, 0x4, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy1_cfg" },
	{ CLK_GATE_RXDPHY2_CFG, "clk_rxdphy2_cfg", "clk_rxdphy_cfg", 0x050, 0x8, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy2_cfg" },
	{ CLK_GATE_RXDPHY3_CFG, "clk_rxdphy3_cfg", "clk_rxdphy_cfg", 0x050, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy3_cfg" },
	{ CLK_GATE_RXDPHY4_CFG, "clk_rxdphy4_cfg", "clk_rxdphy_cfg", 0x050, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy4_cfg" },
	{ CLK_GATE_RXDPHY5_CFG, "clk_rxdphy5_cfg", "clk_rxdphy_cfg", 0x050, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_rxdphy5_cfg" },
	{ CLK_GATE_TXDPHY0_CFG, "clk_txdphy0_cfg", "clk_sys_ini", 0x030, 0x10000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_txdphy0_cfg" },
	{ CLK_GATE_TXDPHY0_REF, "clk_txdphy0_ref", "clkin_sys", 0x030, 0x20000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_txdphy0_ref" },
	{ PCLK_GATE_LOADMONITOR, "pclk_loadmonitor", "div_sysbus_pll", 0x020, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_loadmonitor" },
	{ CLK_GATE_LOADMONITOR, "clk_loadmonitor", "clk_loadmonitor_div", 0x020, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_loadmonitor" },
	{ PCLK_GATE_LOADMONITOR_L, "pclk_loadmonitor_l", "div_sysbus_pll", 0x020, 0x20000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_loadmonitor_l" },
	{ CLK_GATE_LOADMONITOR_L, "clk_loadmonitor_l", "clk_loadmonitor_div", 0x020, 0x10000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_loadmonitor_l" },
	{ CLK_GATE_MEDIA2_TCXO, "clk_media2_tcxo", "clk_sys_ini", 0x050, 0x40000000, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_media2_tcxo" },
	{ CLK_GATE_PFA_REF, "clk_pfa_ref", "clkin_ref", 0x470, 0x8000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pfa_ref" },
	{ HCLK_GATE_PFA, "hclk_pfa", "clk_dmabus_div", 0x470, 0x10000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "hclk_pfa" },
	{ CLK_PFA_FREQ, "clk_pfa_freq", "clk_pfa_div", 0x470, 0x20000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pfa_freq" },
	{ ACLK_GATE_DRA, "aclk_dra", "clk_dmabus_div", 0x470, 0x100000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "aclk_dra" },
	{ CLK_UFS_DFA_CPU, "clk_ufs_dfa_cpu", "div_ufs_dfa_cpu", 0x000, 0x800000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ufs_dfa_cpu" },
	{ CLK_UFS_DFA_SYS, "clk_ufs_dfa_sys", "div_ufs_dfa_sys", 0x000, 0x400000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ufs_dfa_sys" },
	{ AUTODIV_ISP, "autodiv_isp", "autodiv_isp_dvfs", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "autodiv_isp" },
	{ CLK_GATE_ATDIV_HSDT1BUS, "clk_atdiv_hsdt1bus", "clk_div_hsdt1bus", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_hsdt1bus" },
	{ CLK_GATE_ATDIV_DMA, "clk_atdiv_dma", "clk_dmabus_div", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_dma" },
	{ CLK_GATE_ATDIV_CFG, "clk_atdiv_cfg", "div_sysbus_pll", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_cfg" },
	{ CLK_GATE_ATDIV_SYS, "clk_atdiv_sys", "div_sysbus_pll", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_sys" },
};

static const struct fsm_pll_clock fsm_pll_clks[] = {
	{ CLK_GATE_PPLL1, "clk_ap_ppll1", "clk_ppll1", 0x1,
		{ 0x950, 0 }, { 0x958, 0 }, "clk_ap_ppll1" },
	{ CLK_GATE_PPLL2, "clk_ap_ppll2", "clk_ppll2", 0x2,
		{ 0x950, 5 }, { 0x958, 1 }, "clk_ap_ppll2" },
	{ CLK_GATE_PPLL2_B, "clk_ap_ppll2_b", "clk_ppll2_b", 0x9,
		{ 0x954, 5 }, { 0x958, 2 }, "clk_ap_ppll2_b" },
	{ CLK_GATE_PPLL3, "clk_ap_ppll3", "clk_ppll3", 0x3,
		{ 0x950, 10 }, { 0x958, 3 }, "clk_ap_ppll3" },
	{ CLK_GATE_PPLL7, "clk_ap_ppll7", "clk_ppll7", 0x7,
		{ 0x954, 0 }, { 0x958, 4 }, "clk_ap_ppll7" },
};

static const struct pll_freq_info pll6_freq_tables[] = {
	{ 880000000,  0x0000160A, 0x03FFFFFC, 0x02EAAAAA, 0x03FFFFFF },
	{ 900000000,  0x0000170A, 0x03FFFFFC, 0x02700000, 0x03FFFFFF },
	{ 1114000000, 0x00001D0A, 0x03FFFFFC, 0x0202AAAA, 0x03FFFFFF },
	{ 1200000000, 0x00001F0A, 0x03FFFFFC, 0x02400000, 0x03FFFFFF },
	{ 1280000000, 0x0000210A, 0x03FFFFFC, 0x02555555, 0x03FFFFFF },
	{ 1320000000, 0x0000220A, 0x03FFFFFC, 0x02600000, 0x03FFFFFF },
	{ 1618000000, 0x00002A0A, 0x03FFFFFC, 0x0222AAAA, 0x03FFFFFF },
};

static const struct alter_fsm_pll_clock alter_fsm_pll_clks[] = {
	{ CLK_GATE_PPLL6, "clk_ap_ppll6", 0x6,
		{ 0x954, 10 }, { 0x958, 5 }, 0x830, 0x834,
		pll6_freq_tables, ARRAY_SIZE(pll6_freq_tables), "clk_ap_ppll6" },
};

static const struct fsm_pll_clock aocrg_fsm_pll_clks[] = {
	{ CLK_GATE_AUPLL, "clk_ap_aupll", "clk_aupll", 0xC,
		{ 0x2B4, 0 }, { 0x358, 10 }, "clk_ap_aupll" },
};

static const struct scgt_clock crgctrl_scgt_clks[] = {
	{ CLK_ANDGT_HSDT1_USBDP, "clk_hsdt1_usbdp_andgt", "clk_fll_src",
		0x70C, 12, CLK_GATE_HIWORD_MASK, "clk_hsdt1_usbdp_andgt" },
	{ ATCLK_TO_CPU, "clk_at_to_cpu", "clk_atdvfs",
		0xFF0, 9, CLK_GATE_HIWORD_MASK, "clk_at_to_cpu" },
	{ CLK_GATE_TIME_STAMP_GT, "clk_timestp_gt", "clk_sys_ini",
		0x0F0, 1, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clk_timestp_gt" },
	{ CLK_PERF_DIV_GT, "clk_perf_gt", "sc_div_320m",
		0x0F0, 3, CLK_GATE_HIWORD_MASK, "clk_perf_gt" },
	{ CLK_PFA_TFT_GT, "clk_pfa_tft_gt", "clk_pfa_tft_mux",
		0x70C, 8, CLK_GATE_HIWORD_MASK, "clk_pfa_tft_gt" },
	{ CLK_ANDGT_SD_PERI, "clkgt_sd_peri", "clk_ap_ppll2",
		0x70C, 10, CLK_GATE_HIWORD_MASK, "clkgt_sd_peri" },
	{ CLK_A53HPM_ANDGT, "clk_a53hpm_gt", "clk_a53hpm_mux",
		0x0F4, 7, CLK_GATE_HIWORD_MASK, "clk_a53hpm_gt" },
	{ CLK_320M_PLL_GT, "gt_clk_320m_pll", "clk_spll",
		0x0F8, 10, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "gt_clk_320m_pll" },
	{ CLK_ANDGT_UARTH, "clkgt_uarth", "sc_div_320m",
		0x0F4, 11, CLK_GATE_HIWORD_MASK, "clkgt_uarth" },
	{ CLK_ANDGT_UARTL, "clkgt_uartl", "sc_div_320m",
		0x0F4, 10, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clkgt_uartl" },
	{ CLK_ANDGT_UART0, "clkgt_uart0", "sc_div_320m",
		0x0F4, 9, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clkgt_uart0" },
	{ CLK_ANDGT_UART8, "clkgt_uart8", "sc_div_320m",
		0x0F4, 8, CLK_GATE_HIWORD_MASK, "clkgt_uart8" },
	{ CLK_ANDGT_I3C4, "clkgt_i3c4", "sc_div_320m",
		0x70C, 5, CLK_GATE_HIWORD_MASK, "clkgt_i3c4" },
	{ CLK_ANDGT_SPI1, "clkgt_spi1", "sc_div_320m",
		0x0F4, 13, CLK_GATE_HIWORD_MASK, "clkgt_spi1" },
	{ CLK_ANDGT_SPI6, "clkgt_spi6", "sc_div_320m",
		0xB04, 11, CLK_GATE_HIWORD_MASK, "clkgt_spi6" },
	{ CLK_DIV_AO_ASP_GT, "clk_ao_asp_gt", "clkmux_ao_asp",
		0x0F4, 4, CLK_GATE_HIWORD_MASK, "clk_ao_asp_gt" },
	{ CLK_ANDGT_PTP, "clk_ptp_gt", "sc_div_320m",
		0x0F8, 5, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clk_ptp_gt" },
	{ CLK_GT_DPCTRL_16M, "clk_gt_16m_dp", "clk_mux_16m_dp",
		0x0FC, 12, CLK_GATE_HIWORD_MASK, "clk_gt_16m_dp" },
	{ CLK_ANDGT_DPCTR_PIXEL, "gt_dpctr_pixel", "mux_dpctr_pixel",
		0x0F0, 0, CLK_GATE_HIWORD_MASK, "gt_dpctr_pixel" },
	{ CLK_ISP_SNCLK_ANGT, "clk_ispsn_gt", "sc_div_320m",
		0x108, 2, CLK_GATE_HIWORD_MASK, "clk_ispsn_gt" },
	{ CLKANDGT_RXDPHY_FLL, "clkgt_rxdphy_fll", "clk_fll_src",
		0x0FC, 10, CLK_GATE_HIWORD_MASK, "clkgt_rxdphy_fll" },
	{ CLK_GT_LOADMONITOR, "clk_loadmonitor_gt", "sc_div_320m",
		0x0F0, 5, CLK_GATE_HIWORD_MASK, "clk_loadmonitor_gt" },
	{ CLK_GATE_PFA_GT, "clk_pfa_gt", "clk_pfa_mux",
		0x70C, 9, CLK_GATE_HIWORD_MASK, "clk_pfa_gt" },
	{ GT_CLK_UFS_DFA_CPU, "gt_clk_ufs_dfa_cpu", "sel_ufs_dfa_cpu",
		0x0F0, 14, CLK_GATE_HIWORD_MASK, "gt_clk_ufs_dfa_cpu" },
	{ GT_CLK_UFS_DFA_SYS, "gt_clk_ufs_dfa_sys", "sel_ufs_dfa_sys",
		0x0F0, 15, CLK_GATE_HIWORD_MASK, "gt_clk_ufs_dfa_sys" },
	{ AUTODIV_SYSBUS, "autodiv_sysbus", "div_sysbus_pll",
		0xF90, 5, CLK_GATE_HIWORD_MASK, "autodiv_sysbus" },
	{ AUTODIV_HSDT1BUS, "autodiv_hsdt1bus", "autodiv_sysbus",
		0xF90, 1, CLK_GATE_HIWORD_MASK, "autodiv_hsdt1bus" },
	{ AUTODIV_CFGBUS, "autodiv_cfgbus", "autodiv_sysbus",
		0xF90, 4, CLK_GATE_HIWORD_MASK, "autodiv_cfgbus" },
	{ AUTODIV_DMABUS, "autodiv_dmabus", "autodiv_sysbus",
		0xF90, 3, CLK_GATE_HIWORD_MASK, "autodiv_dmabus" },
};

static const struct div_clock crgctrl_div_clks[] = {
	{ CLK_DIV_HSDT1_USBDP, "div_hsdt1_usbdp", "clk_hsdt1_usbdp_andgt",
		0x710, 0x300, 4, 1, 0, "div_hsdt1_usbdp" },
	{ PCLK_DIV_DBG, "pclkdiv_dbg", "clk_cssys_div",
		0xFF4, 0x6000, 4, 1, 0, "pclkdiv_dbg" },
	{ CLK_DIV_TIME_STAMP, "clk_timestp_div", "clk_timestp_gt",
		0x0B8, 0x70, 8, 1, 0, "clk_timestp_div" },
	{ CLK_DIV_PERF_STAT, "clk_perf_div", "clk_perf_gt",
		0x0D0, 0xf000, 16, 1, 0, "clk_perf_div" },
	{ CLK_DIV_PFA_TFT, "clk_pfa_tft_div", "clk_pfa_tft_gt",
		0x718, 0x3f0, 64, 1, 0, "clk_pfa_tft_div" },
	{ CLK_DIV_SD_PERI, "clk_div_sd_peri", "clkgt_sd_peri",
		0x700, 0x1e0, 16, 1, 0, "clk_div_sd_peri" },
	{ CLK_DIV_A53HPM, "clk_a53hpm_div", "clk_a53hpm_gt",
		0x0E0, 0xfc00, 64, 1, 0, "clk_a53hpm_div" },
	{ CLK_DIV_UARTH, "clkdiv_uarth", "clkgt_uarth",
		0x0B0, 0xf000, 16, 1, 0, "clkdiv_uarth" },
	{ CLK_DIV_UARTL, "clkdiv_uartl", "clkgt_uartl",
		0x0B0, 0xf00, 16, 1, 0, "clkdiv_uartl" },
	{ CLK_DIV_UART0, "clkdiv_uart0", "clkgt_uart0",
		0x0B0, 0xf0, 16, 1, 0, "clkdiv_uart0" },
	{ CLK_DIV_UART8, "clkdiv_uart8", "clkgt_uart8",
		0x0B0, 0xf, 16, 1, 0, "clkdiv_uart8" },
	{ CLK_DIV_I2C, "clkdiv_i2c", "sc_div_320m",
		0x0E8, 0xf0, 16, 1, 0, "clkdiv_i2c" },
	{ CLK_DIV_I3C4, "clkdiv_i3c4", "clkgt_i3c4",
		0x710, 0xc00, 4, 1, 0, "clkdiv_i3c4" },
	{ CLK_DIV_SPI1, "clkdiv_spi1", "clkgt_spi1",
		0x0C4, 0xf000, 16, 1, 0, "clkdiv_spi1" },
	{ CLK_DIV_SPI6, "clkdiv_spi6", "clkgt_spi6",
		0xB0C, 0xf0, 16, 1, 0, "clkdiv_spi6" },
	{ CLK_DIV_AO_ASP, "clk_ao_asp_div", "clk_ao_asp_gt",
		0x108, 0x3c0, 16, 1, 0, "clk_ao_asp_div" },
	{ CLK_DIV_PTP, "clk_ptp_div", "clk_ptp_gt",
		0x0D8, 0xf, 16, 1, 0, "clk_ptp_div" },
	{ CLK_DIV_DPCTRL_16M, "div_dpctrl16m", "clk_gt_16m_dp",
		0x0B4, 0xfc0, 64, 1, 0, "div_dpctrl16m" },
	{ CLK_DIV_DPCTR_PIXEL, "div_dpctr_pixel", "gt_dpctr_pixel",
		0x0C0, 0x3f00, 64, 1, 0, "div_dpctr_pixel" },
	{ CLK_ISP_SNCLK_DIV0, "clk_div_ispsn0", "clk_fac_ispsn",
		0x108, 0x3, 4, 1, 0, "clk_div_ispsn0" },
	{ CLK_ISP_SNCLK_DIV1, "clk_div_ispsn1", "clk_fac_ispsn",
		0x10C, 0xc000, 4, 1, 0, "clk_div_ispsn1" },
	{ CLK_ISP_SNCLK_DIV2, "clk_div_ispsn2", "clk_fac_ispsn",
		0x10C, 0x1800, 4, 1, 0, "clk_div_ispsn2" },
	{ CLK_ISP_SNCLK_DIV3, "clk_div_ispsn3", "clk_fac_ispsn",
		0x100, 0x300, 4, 1, 0, "clk_div_ispsn3" },
	{ CLK_ISP_SNCLK_FAC, "clk_fac_ispsn", "clk_ispsn_gt",
		0x108, 0xfc00, 64, 1, 0, "clk_fac_ispsn" },
	{ CLK_RXDPHY_FLL_DIV, "div_rxdphy_fll", "clkgt_rxdphy_fll",
		0x0DC, 0x3, 4, 1, 0, "div_rxdphy_fll" },
	{ CLK_DIV_LOADMONITOR, "clk_loadmonitor_div", "clk_loadmonitor_gt",
		0x0B8, 0xc000, 4, 1, 0, "clk_loadmonitor_div" },
	{ CLK_DIV_PFA, "clk_pfa_div", "clk_pfa_gt",
		0x718, 0xfc00, 64, 1, 0, "clk_pfa_div" },
	{ DIV_UFS_DFA_CPU, "div_ufs_dfa_cpu", "gt_clk_ufs_dfa_cpu",
		0x0A8, 0x3f, 64, 1, 0, "div_ufs_dfa_cpu" },
	{ DIV_UFS_DFA_SYS, "div_ufs_dfa_sys", "gt_clk_ufs_dfa_sys",
		0x0A8, 0xfc0, 64, 1, 0, "div_ufs_dfa_sys" },
};

static const char * const codeccssi_mux_p [] = { "clk_sys_ini", "clk_sys_ini" };
static const char * const clk_mux_pfa_tft_p [] = { "clk_spll", "clk_ap_ppll1" };
static const char * const clk_mux_a53hpm_p [] = { "clk_spll", "clk_ap_ppll1" };
static const char * const clk_mux_uarth_p [] = { "clk_sys_ini", "clkdiv_uarth" };
static const char * const clk_mux_uartl_p [] = { "clk_sys_ini", "clkdiv_uartl" };
static const char * const clk_mux_uart0_p [] = { "clk_sys_ini", "clkdiv_uart0" };
static const char * const clk_mux_uart8_p [] = { "clk_sys_ini", "clkdiv_uart8" };
static const char * const clk_mux_i2c_p [] = { "clk_sys_ini", "clkdiv_i2c" };
static const char * const clk_mux_spi1_p [] = { "clk_sys_ini", "clkdiv_spi1" };
static const char * const clk_mux_spi6_p [] = { "clk_sys_ini", "clkdiv_spi6" };
static const char * const clk_mux_ao_asp_p [] = { "clk_ap_ppll1", "clk_ap_ppll2_b", "clk_ap_ppll2", "clk_ap_ppll2" };
static const char * const clk_mux_dpctrl_16m_p [] = { "clk_fll_src", "clk_a53hpm_div" };
static const char * const clk_mux_dpctr_pixel_p [] = { "clk_spll", "clk_ap_ppll1", "clk_ap_ppll7", "clk_ap_ppll7" };
static const char * const clk_isp_snclk_mux0_p [] = { "clk_sys_ini", "clk_div_ispsn0", "clkin_sys", "clkin_sys" };
static const char * const clk_isp_snclk_mux1_p [] = { "clk_sys_ini", "clk_div_ispsn1", "clkin_sys", "clkin_sys" };
static const char * const clk_isp_snclk_mux2_p [] = { "clk_sys_ini", "clk_div_ispsn2", "clkin_sys", "clkin_sys" };
static const char * const clk_isp_snclk_mux3_p [] = { "clk_sys_ini", "clk_div_ispsn3", "clkin_sys", "clkin_sys" };
static const char * const clk_mux_rxdphy_cfg_p [] = { "clk_a53hpm_div", "clk_a53hpm_div",
	"clk_sys_ini", "div_rxdphy_fll" };
static const char * const clk_mux_pfa_p [] = { "clk_spll", "clk_ap_ppll1" };
static const char * const sel_ufs_dfa_cpu_p [] = {
		"clk_invalid", "clk_ap_ppll7", "clk_spll", "clk_invalid",
		"clk_ap_ppll2_b", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ap_ppll2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const sel_ufs_dfa_sys_p [] = { "clk_ap_ppll1", "clk_spll", "clk_ap_ppll2", "clk_ap_ppll2" };
static const struct mux_clock crgctrl_mux_clks[] = {
	{ CODECCSSI_MUX, "sel_codeccssi", codeccssi_mux_p,
		ARRAY_SIZE(codeccssi_mux_p), 0x0AC, 0x80, CLK_MUX_HIWORD_MASK, "sel_codeccssi" },
	{ CLK_MUX_PFA_TFT, "clk_pfa_tft_mux", clk_mux_pfa_tft_p,
		ARRAY_SIZE(clk_mux_pfa_tft_p), 0x704, 0x8, CLK_MUX_HIWORD_MASK, "clk_pfa_tft_mux" },
	{ CLK_MUX_A53HPM, "clk_a53hpm_mux", clk_mux_a53hpm_p,
		ARRAY_SIZE(clk_mux_a53hpm_p), 0x0D4, 0x200, CLK_MUX_HIWORD_MASK, "clk_a53hpm_mux" },
	{ CLK_MUX_UARTH, "clkmux_uarth", clk_mux_uarth_p,
		ARRAY_SIZE(clk_mux_uarth_p), 0x0AC, 0x10, CLK_MUX_HIWORD_MASK, "clkmux_uarth" },
	{ CLK_MUX_UARTL, "clkmux_uartl", clk_mux_uartl_p,
		ARRAY_SIZE(clk_mux_uartl_p), 0x0AC, 0x8, CLK_MUX_HIWORD_MASK, "clkmux_uartl" },
	{ CLK_MUX_UART0, "clkmux_uart0", clk_mux_uart0_p,
		ARRAY_SIZE(clk_mux_uart0_p), 0x0AC, 0x4, CLK_MUX_HIWORD_MASK, "clkmux_uart0" },
	{ CLK_MUX_UART8, "clkmux_uart8", clk_mux_uart8_p,
		ARRAY_SIZE(clk_mux_uart8_p), 0x0AC, 0x20, CLK_MUX_HIWORD_MASK, "clkmux_uart8" },
	{ CLK_MUX_I2C, "clkmux_i2c", clk_mux_i2c_p,
		ARRAY_SIZE(clk_mux_i2c_p), 0x0AC, 0x2000, CLK_MUX_HIWORD_MASK, "clkmux_i2c" },
	{ CLK_MUX_SPI1, "clkmux_spi1", clk_mux_spi1_p,
		ARRAY_SIZE(clk_mux_spi1_p), 0x0AC, 0x100, CLK_MUX_HIWORD_MASK, "clkmux_spi1" },
	{ CLK_MUX_SPI6, "clkmux_spi6", clk_mux_spi6_p,
		ARRAY_SIZE(clk_mux_spi6_p), 0xB0C, 0x8000, CLK_MUX_HIWORD_MASK, "clkmux_spi6" },
	{ CLK_MUX_AO_ASP, "clkmux_ao_asp", clk_mux_ao_asp_p,
		ARRAY_SIZE(clk_mux_ao_asp_p), 0x100, 0x30, CLK_MUX_HIWORD_MASK, "clkmux_ao_asp" },
	{ CLK_MUX_DPCTRL_16M, "clk_mux_16m_dp", clk_mux_dpctrl_16m_p,
		ARRAY_SIZE(clk_mux_dpctrl_16m_p), 0x0B4, 0x1000, CLK_MUX_HIWORD_MASK, "clk_mux_16m_dp" },
	{ CLK_MUX_DPCTR_PIXEL, "mux_dpctr_pixel", clk_mux_dpctr_pixel_p,
		ARRAY_SIZE(clk_mux_dpctr_pixel_p), 0x0C0, 0xc000, CLK_MUX_HIWORD_MASK, "mux_dpctr_pixel" },
	{ CLK_ISP_SNCLK_MUX0, "clk_mux_ispsn0", clk_isp_snclk_mux0_p,
		ARRAY_SIZE(clk_isp_snclk_mux0_p), 0x108, 0x18, CLK_MUX_HIWORD_MASK, "clk_mux_ispsn0" },
	{ CLK_ISP_SNCLK_MUX1, "clk_mux_ispsn1", clk_isp_snclk_mux1_p,
		ARRAY_SIZE(clk_isp_snclk_mux1_p), 0x10C, 0x300, CLK_MUX_HIWORD_MASK, "clk_mux_ispsn1" },
	{ CLK_ISP_SNCLK_MUX2, "clk_mux_ispsn2", clk_isp_snclk_mux2_p,
		ARRAY_SIZE(clk_isp_snclk_mux2_p), 0x10C, 0xc0, CLK_MUX_HIWORD_MASK, "clk_mux_ispsn2" },
	{ CLK_ISP_SNCLK_MUX3, "clk_mux_ispsn3", clk_isp_snclk_mux3_p,
		ARRAY_SIZE(clk_isp_snclk_mux3_p), 0x100, 0xc0, CLK_MUX_HIWORD_MASK, "clk_mux_ispsn3" },
	{ CLK_MUX_RXDPHY_CFG, "clk_rxdphy_mux", clk_mux_rxdphy_cfg_p,
		ARRAY_SIZE(clk_mux_rxdphy_cfg_p), 0x0DC, 0xc, CLK_MUX_HIWORD_MASK, "clk_rxdphy_mux" },
	{ CLK_MUX_PFA, "clk_pfa_mux", clk_mux_pfa_p,
		ARRAY_SIZE(clk_mux_pfa_p), 0x704, 0x1, CLK_MUX_HIWORD_MASK, "clk_pfa_mux" },
	{ SEL_UFS_DFA_CPU, "sel_ufs_dfa_cpu", sel_ufs_dfa_cpu_p,
		ARRAY_SIZE(sel_ufs_dfa_cpu_p), 0x0BC, 0xf000, CLK_MUX_HIWORD_MASK, "sel_ufs_dfa_cpu" },
	{ SEL_UFS_DFA_SYS, "sel_ufs_dfa_sys", sel_ufs_dfa_sys_p,
		ARRAY_SIZE(sel_ufs_dfa_sys_p), 0x0BC, 0xc00, CLK_MUX_HIWORD_MASK, "sel_ufs_dfa_sys" },
};

static const struct pll_clock hsdtcrg_pll_clks[] = {
	{ CLK_GATE_PCIE0PLL, "clk_ap_pcie0pll", "clk_ppll_pcie", 0xD,
		{ 0x190, 0 }, { 0, 0 }, { 0x190, 1 }, { 0x204, 4 }, 10, 1, "clk_ap_pcie0pll" },
};

static const struct gate_clock hsdtctrl_gate_clks[] = {
	{ CLK_GATE_PCIEPHY_REF, "clk_pciephy_ref", "clk_ap_pcie0pll", 0x010, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pciephy_ref" },
	{ CLK_GATE_PCIE1PHY_REF, "clk_pcie1phy_ref", "clk_ap_pcie0pll", 0x000, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pcie1phy_ref" },
	{ PCLK_GATE_PCIE_SYS, "pclk_pcie_sys", "clk_ptp_div", 0x010, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_pcie_sys" },
	{ PCLK_GATE_PCIE1_SYS, "pclk_pcie1_sys", "clk_ptp_div", 0x000, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_pcie1_sys" },
	{ PCLK_GATE_PCIE_PHY, "pclk_pcie_phy", "clk_ptp_div", 0x010, 0x8, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_pcie_phy" },
	{ PCLK_GATE_PCIE1_PHY, "pclk_pcie1_phy", "clk_ptp_div", 0x000, 0x100, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_pcie1_phy" },
	{ ACLK_GATE_PCIE, "aclk_pcie", "clk_hsdt_subsys_ini", 0x010, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "aclk_pcie" },
	{ CLK_GATE_HSDT_TCU, "clk_hsdt_tcu", "clk_hsdt_subsys_ini", 0x010, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_hsdt_tcu" },
	{ CLK_GATE_HSDT_TBU, "clk_hsdt_tbu", "clk_hsdt_subsys_ini", 0x010, 0x800, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_hsdt_tbu" },
	{ CLK_GATE_PCIE1_TBU, "clk_pcie1_tbu", "clk_hsdt_subsys_ini", 0x000, 0x2000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pcie1_tbu" },
	{ ACLK_GATE_PCIE1, "aclk_pcie1", "clk_hsdt_subsys_ini", 0x000, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "aclk_pcie1" },
};

static const struct div_clock hsdt1ctrl_div_clks[] = {
	{ CLK_DIV_SDPLL, "clkdiv_sdpll", "clkgt_sdpll",
		0x0AC, 0x1e, 16, 1, 0, "clkdiv_sdpll" },
};

static const struct pll_clock hsdt1crg_pll_clks[] = {
	{ CLK_GATE_SDPLL, "clk_ap_sdpll", "clk_sdpll", 0xF,
		{ 0x200, 0 }, { 0x204, 26 }, { 0x200, 1 }, { 0x208, 4 }, 12, 1, "clk_ap_sdpll" },
};

static const char * const clk_mux_sd_sys_p [] = { "clk_sd_sys", "clk_sd_peri", "clkdiv_sdpll", "clkdiv_sdpll" };
static const struct mux_clock hsdt1ctrl_mux_clks[] = {
	{ CLK_MUX_SD_SYS, "clk_sd_muxsys", clk_mux_sd_sys_p,
		ARRAY_SIZE(clk_mux_sd_sys_p), 0x0AC, 0x60, CLK_MUX_HIWORD_MASK, "clk_sd_muxsys" },
};

static const struct gate_clock hsdt1ctrl_gate_clks[] = {
	{ CLK_GATE_SD, "clk_sd", "clk_sd_muxsys", 0x010, 0x200000, 0,
		NULL, 0, {0}, {0}, 0, 0, 1, "clk_sd" },
	{ HCLK_GATE_SD, "hclk_sd", "clk_div_hsdt1bus", 0x000, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "hclk_sd" },
	{ CLK_GATE_USB31PHY_APB, "usb31phy_apb", "clkdiv_usb2phy_ref", 0x000, 0x4000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "usb31phy_apb" },
	{ CLK_GATE_HI_USB3_SUSPEND, "hi_usb3_suspend", "clk_abb_192", 0x000, 0x4000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "hi_usb3_suspend" },
	{ CLK_GATE_HI_USB3_SYS, "clk_hi_usb3_sys", "clk_hsdt1_usbdp", 0x000, 0x8000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_hi_usb3_sys" },
	{ ACLK_GATE_HI_USB3, "aclk_hi_usb3", "clk_hsdt1_usbdp", 0x000, 0x1000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "aclk_hi_usb3" },
	{ PCLK_GATE_USB2PHY, "pclk_usb2phy", "clkdiv_usb2phy_ref", 0x000, 0x4, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_usb2phy" },
	{ CLK_GATE_HI_USB3CTRL_REF, "hi_usb3crtl", "clkdiv_usb2phy_ref", 0x000, 0x200, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "hi_usb3crtl" },
	{ PCLK_GATE_USB_SCTRL, "pclk_usb_sctrl", "clkdiv_usb2phy_ref", 0x000, 0x20000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_usb_sctrl" },
	{ CLK_GATE_USB2PHY_REF, "clk_usb2phy_ref", "clkdiv_usb2phy_ref", 0x000, 0x40000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_usb2phy_ref" },
	{ CLK_GATE_USBDP_AUX, "clk_usbdp_aux", "clk_abb_192", 0x000, 0x800000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_usbdp_aux" },
	{ CLK_GATE_QIC_USB3, "clk_qic_usb3", "clk_hsdt1_usbdp", 0x000, 0x800, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_qic_usb3" },
	{ CLK_GATE_QIC_H1_APB, "clk_qic_h1_apb", "clk_ptp_div", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_qic_h1_apb" },
	{ CLK_GATE_USB3PHY_REF, "clk_usb3phy_ref", "clk_abb_192", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_usb3phy_ref" },
	{ CLK_GATE_UFS1PHY_REF, "clk_ufs1phy_ref", "clk_abb_192", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ufs1phy_ref" },
	{ PCLK_GATE_DPCTRL, "pclk_dpctrl", "clkdiv_usb2phy_ref", 0x000, 0x8, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_dpctrl" },
};

static const struct scgt_clock hsdt1ctrl_scgt_clks[] = {
	{ CLK_ANDGT_SDPLL, "clkgt_sdpll", "clk_ap_sdpll",
		0x0B0, 1, CLK_GATE_HIWORD_MASK, "clkgt_sdpll" },
	{ CLK_SD_SYS_GT, "clk_sd_sys_gt", "clkdiv_usb2phy_ref",
		0x0B0, 0, CLK_GATE_HIWORD_MASK, "clk_sd_sys_gt" },
	{ CLK_ANDGT_USB2PHY_REF, "clkgt_usb2phy_ref", "clk_abb_192",
		0x0A8, 2, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clkgt_usb2phy_ref" },
};

static const struct div_clock aocrg_div_clks[] = {
	{ CLK_DIV_SPI3_ULPPLL, "div_spi3_ulppll", "gt_spi3_ulppll",
		0x2AC, 0xf, 16, 1, 0, "div_spi3_ulppll" },
	{ CLK_DIV_SPI3, "clkdiv_spi3", "clkgt_spi3",
		0x2A8, 0x1f80, 64, 1, 0, "clkdiv_spi3" },
	{ CLK_DIV_SPI5_ULPPLL, "div_spi5_ulppll", "gt_spi5_ulppll",
		0x2A4, 0xf00, 16, 1, 0, "div_spi5_ulppll" },
	{ CLK_DIV_SPI5, "clkdiv_spi5", "clkgt_spi5",
		0x2A4, 0xfc, 64, 1, 0, "clkdiv_spi5" },
	{ CLK_DIV_32KPLL_PCIEAUX, "div_32kpll_pcieaux", "clkgt_32kpll_pcieaux",
		0x298, 0x3f0, 64, 1, 0, "div_32kpll_pcieaux" },
	{ CLK_DIV_BLPWM2, "clkdiv_blpwm2", "clkgt_blpwm2",
		0x2A8, 0x60, 4, 1, 0, "clkdiv_blpwm2" },
	{ CLKDIV_ASP_CODEC, "clkdiv_asp_codec", "clkgt_asp_codec",
		0x284, 0x3f, 64, 1, 0, "clkdiv_asp_codec" },
	{ CLKDIV_DP_AUDIO_PLL_AO, "clk_dp_audio_pll_ao_div", "clk_dp_audio_pll_ao_gt",
		0x2A8, 0xf, 16, 1, 0, "clk_dp_audio_pll_ao_div" },
	{ CLK_DIV_AO_CAMERA, "clkdiv_ao_camera", "clkgt_ao_camera",
		0x260, 0x3f, 64, 1, 0, "clkdiv_ao_camera" },
	{ CLK_DIV_AO_TOF, "clkdiv_ao_tof", "clkgt_ao_tof",
		0x25C, 0xfc00, 64, 1, 0, "clkdiv_ao_tof" },
	{ CLK_DIV_AO_CAMERA1, "clkdiv_ao_camera1", "clkgt_ao_camera1",
		0x260, 0xfc00, 64, 1, 0, "clkdiv_ao_camera1" },
	{ CLK_DIV_AO_LOADMONITOR, "clk_ao_loadmonitor_div", "clk_ao_loadmonitor_gt",
		0x26C, 0x3c00, 16, 1, 0, "clk_ao_loadmonitor_div" },
	{ CLK_DIV_OUT0, "clkdiv_out0", "clkgt_out0",
		0x284, 0x3c00, 16, 1, 0, "clkdiv_out0" },
};

static const char * const clk_mux_spi3_p [] = { "clkdiv_spi3", "div_spi3_ulppll" };
static const char * const clk_mux_spi5_p [] = { "clkdiv_spi5", "div_spi5_ulppll" };
static const char * const clk_mux_pcieaux_p [] = { "clk_sys_ini", "div_32kpll_pcieaux" };
static const char * const clkmux_syscnt_p [] = { "clk_syscnt_div", "clkin_ref" };
static const char * const clk_mux_asp_pll_p [] = {
		"clk_invalid", "clk_spll", "sel_ao_asp_32kpll", "clk_invalid",
		"clk_ao_asp", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_sys_ini", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_ao_asp_32kpll_mux_p [] = { "clk_fll_src", "clk_ulppll_1" };
static const char * const clk_mux_ao_camera_p [] = { "clkin_sys", "clk_fll_src", "div_aobus_334M", "clk_invalid" };
static const char * const clk_mux_ao_tof_p [] = { "clkin_sys", "clk_fll_src", "div_aobus_334M", "clk_invalid" };
static const char * const clk_mux_ao_camera1_p [] = { "clkin_sys", "clk_fll_src", "div_aobus_334M", "clk_invalid" };
static const char * const clk_mux_out0_p [] = { "div_aobus_334M", "clk_fll_src", "clk_tcxo_vote", "clk_fll_src" };
static const char * const sel_clk_out_ufs_p [] = { "clkin_sys", "clk_sys_ini" };
static const char * const sel_clk_out_hiphy_p [] = { "clkin_sys", "clk_sys_ini" };
static const struct mux_clock aocrg_mux_clks[] = {
	{ CLK_MUX_SPI3, "clkmux_spi3", clk_mux_spi3_p,
		ARRAY_SIZE(clk_mux_spi3_p), 0x2A8, 0x8000, CLK_MUX_HIWORD_MASK, "clkmux_spi3" },
	{ CLK_MUX_SPI5, "clkmux_spi5", clk_mux_spi5_p,
		ARRAY_SIZE(clk_mux_spi5_p), 0x2A4, 0x1000, CLK_MUX_HIWORD_MASK, "clkmux_spi5" },
	{ CLK_MUX_PCIEAUX, "clkmux_pcieaux", clk_mux_pcieaux_p,
		ARRAY_SIZE(clk_mux_pcieaux_p), 0x288, 0x4000, CLK_MUX_HIWORD_MASK, "clkmux_pcieaux" },
	{ CLKMUX_SYSCNT, "clkmux_syscnt", clkmux_syscnt_p,
		ARRAY_SIZE(clkmux_syscnt_p), 0x264, 0x2000, CLK_MUX_HIWORD_MASK, "clkmux_syscnt" },
	{ CLK_MUX_ASP_PLL, "clk_asp_pll_sel", clk_mux_asp_pll_p,
		ARRAY_SIZE(clk_mux_asp_pll_p), 0x280, 0xf, CLK_MUX_HIWORD_MASK, "clk_asp_pll_sel" },
	{ CLK_AO_ASP_32KPLL_MUX, "sel_ao_asp_32kpll", clk_ao_asp_32kpll_mux_p,
		ARRAY_SIZE(clk_ao_asp_32kpll_mux_p), 0x250, 0x2000, CLK_MUX_HIWORD_MASK, "sel_ao_asp_32kpll" },
	{ CLK_MUX_AO_CAMERA, "clkmux_ao_camera", clk_mux_ao_camera_p,
		ARRAY_SIZE(clk_mux_ao_camera_p), 0x290, 0x30, CLK_MUX_HIWORD_MASK, "clkmux_ao_camera" },
	{ CLK_MUX_AO_TOF, "clkmux_ao_tof", clk_mux_ao_tof_p,
		ARRAY_SIZE(clk_mux_ao_tof_p), 0x25C, 0x300, CLK_MUX_HIWORD_MASK, "clkmux_ao_tof" },
	{ CLK_MUX_AO_CAMERA1, "clkmux_ao_camera1", clk_mux_ao_camera1_p,
		ARRAY_SIZE(clk_mux_ao_camera1_p), 0x260, 0xc0, CLK_MUX_HIWORD_MASK, "clkmux_ao_camera1" },
	{ CLK_MUX_OUT0, "clkmux_out0", clk_mux_out0_p,
		ARRAY_SIZE(clk_mux_out0_p), 0x284, 0xc0, CLK_MUX_HIWORD_MASK, "clkmux_out0" },
	{ SEL_CLK_OUT_UFS, "sel_clk_out_ufs", sel_clk_out_ufs_p,
		ARRAY_SIZE(sel_clk_out_ufs_p), 0x26C, 0x100, CLK_MUX_HIWORD_MASK, "sel_clk_out_ufs" },
	{ SEL_CLK_OUT_HIPHY, "sel_clk_out_hiphy", sel_clk_out_hiphy_p,
		ARRAY_SIZE(sel_clk_out_hiphy_p), 0x268, 0x100, CLK_MUX_HIWORD_MASK, "sel_clk_out_hiphy" },
};

static const struct gate_clock aocrg_gate_clks[] = {
	{ CLK_GATE_TIMER5, "clk_timer5", "clk_fll_src", 0x170, 0x1000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_timer5" },
	{ CLK_GATE_SPI, "clk_spi3", "clkmux_spi3", 0x1B0, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_spi3" },
	{ PCLK_GATE_SPI, "pclk_spi3", "clkmux_spi3", 0x1B0, 0x400, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_spi3" },
	{ CLK_GATE_SPI5, "clk_spi5", "clkmux_spi5", 0x1B0, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_spi5" },
	{ PCLK_GATE_SPI5, "pclk_spi5", "clkmux_spi5", 0x1B0, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_spi5" },
	{ CLK_GATE_HSDT0_PCIEAUX, "clk_hsdt0_pcie_aux", "clkmux_pcieaux", 0x170, 0x100, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_hsdt0_pcie_aux" },
	{ CLK_GATE_PCIEAUX, "clk_pcie0_aux", "clk_hsdt0_pcie_aux", 0x170, 0x80000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pcie0_aux" },
	{ CLK_GATE_PCIE1AUX, "clk_pcie1_aux", "clk_hsdt0_pcie_aux", 0x170, 0x100000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_pcie1_aux" },
	{ PCLK_GATE_RTC, "pclk_rtc", "sc_div_aobus", 0x160, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_rtc" },
	{ CLK_GATE_BLPWM2, "clk_blpwm2", "clkdiv_blpwm2", 0x160, 0x10000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_blpwm2" },
	{ PCLK_GATE_SYSCNT, "pclk_syscnt", "sc_div_aobus", 0x160, 0x80000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_syscnt" },
	{ CLK_GATE_SYSCNT, "clk_syscnt", "clkmux_syscnt", 0x160, 0x100000, ALWAYS_ON,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_syscnt" },
	{ CLK_ASP_CODEC, "clk_asp_codec", "clkdiv_asp_codec", 0x170, 0x8000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_asp_codec" },
	{ CLK_GATE_ASP_SUBSYS, "clk_asp_subsys", "clk_asp_pll_sel", 0x170, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_asp_subsys" },
	{ CLK_GATE_ASP_TCXO, "clk_asp_tcxo", "clk_sys_ini", 0x160, 0x8000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_asp_tcxo" },
	{ CLK_GATE_DP_AUDIO_PLL, "clk_dp_audio_pll", "clk_dp_audio_pll_ao_div", 0x1B0, 0x80, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_dp_audio_pll" },
	{ CLK_GATE_AO_CAMERA, "clk_ao_camera", "clkdiv_ao_camera", 0x170, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ao_camera" },
	{ CLK_GATE_AO_TOF, "clk_ao_tof", "clkdiv_ao_tof", 0x190, 0x4000000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ao_tof" },
	{ CLK_GATE_AO_CAMERA1, "clk_ao_camera1", "clkdiv_ao_camera1", 0x1D0, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ao_camera1" },
	{ PCLK_GATE_AO_LOADMONITOR, "pclk_ao_loadmonitor", "sc_div_aobus", 0x1B0, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "pclk_ao_loadmonitor" },
	{ CLK_GATE_AO_LOADMONITOR, "clk_ao_loadmonitor", "clk_ao_loadmonitor_div", 0x1B0, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ao_loadmonitor" },
	{ CLK_OUT_IO_DIE, "clk_out_io_die", "clk_sys_ini", 0x1D0, 0x80, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_out_io_die" },
	{ CLK_OUT_UFS, "clk_out_ufs", "sel_clk_out_ufs", 0x1D0, 0x40, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_out_ufs" },
	{ CLK_OUT_HIPHY, "clk_out_hiphy", "sel_clk_out_hiphy", 0x1D0, 0x4000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_out_hiphy" },
	{ CLK_GATE_TIMER15, "clk_timer15", "clk_fll_src", 0x1C0, 0x10, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_timer15" },
};

static const struct scgt_clock aocrg_scgt_clks[] = {
	{ CLK_ANDGT_SPI3_ULPPLL, "gt_spi3_ulppll", "clk_fll_src",
		0x2A8, 13, CLK_GATE_HIWORD_MASK, "gt_spi3_ulppll" },
	{ CLK_ANDGT_SPI3, "clkgt_spi3", "clk_spll",
		0x2A8, 14, CLK_GATE_HIWORD_MASK, "clkgt_spi3" },
	{ CLK_ANDGT_SPI5_ULPPLL, "gt_spi5_ulppll", "clk_fll_src",
		0x2A4, 1, CLK_GATE_HIWORD_MASK, "gt_spi5_ulppll" },
	{ CLK_ANDGT_SPI5, "clkgt_spi5", "clk_spll",
		0x2A4, 0, CLK_GATE_HIWORD_MASK, "clkgt_spi5" },
	{ CLK_ANDGT_32KPLL_PCIEAUX, "clkgt_32kpll_pcieaux", "clk_fll_src",
		0x298, 10, CLK_GATE_HIWORD_MASK, "clkgt_32kpll_pcieaux" },
	{ CLK_ANDGT_BLPWM2, "clkgt_blpwm2", "clk_fll_src",
		0x264, 14, CLK_GATE_HIWORD_MASK, "clkgt_blpwm2" },
	{ CLKGT_ASP_CODEC, "clkgt_asp_codec", "clk_spll",
		0x274, 14, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clkgt_asp_codec" },
	{ CLKGT_DP_AUDIO_PLL_AO, "clk_dp_audio_pll_ao_gt", "clk_ap_aupll",
		0x294, 14, CLK_GATE_HIWORD_MASK, "clk_dp_audio_pll_ao_gt" },
	{ CLK_ANDGT_AO_CAMERA, "clkgt_ao_camera", "clkmux_ao_camera",
		0x290, 14, CLK_GATE_HIWORD_MASK, "clkgt_ao_camera" },
	{ CLK_ANDGT_AO_TOF, "clkgt_ao_tof", "clkmux_ao_tof",
		0x274, 11, CLK_GATE_HIWORD_MASK, "clkgt_ao_tof" },
	{ CLK_ANDGT_AO_CAMERA1, "clkgt_ao_camera1", "clkmux_ao_camera1",
		0x274, 12, CLK_GATE_HIWORD_MASK, "clkgt_ao_camera1" },
	{ CLK_GT_AO_LOADMONITOR, "clk_ao_loadmonitor_gt", "clk_fll_src",
		0x26C, 9, CLK_GATE_HIWORD_MASK, "clk_ao_loadmonitor_gt" },
	{ CLK_ANDGT_OUT0, "clkgt_out0", "clkmux_out0",
		0x284, 8, CLK_GATE_HIWORD_MASK, "clkgt_out0" },
};

static const struct sec_gate_clock aocrg_sec_gate_clks[] = {
	{ CLK_GATE_OUT0, "clk_out0", "clkdiv_out0", { 0x900, 5 }, 0, 0, "clk_out0" },
};

static const struct div_clock media1crg_div_clks[] = {
	{ CLK_DIV_VIVOBUS, "clk_vivobus_div", "clk_vivobus_gt",
		0x06C, 0x3f00, 64, 1, 0, "clk_vivobus_div" },
};

static const char * const clk_mux_vivobus_p [] = {
		"clk_invalid", "clk_ppll2b_media", "clk_ppll0_media", "clk_invalid",
		"clk_ppll1_media", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll2_media", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const struct mux_clock media1crg_mux_clks[] = {
	{ CLK_MUX_VIVOBUS, "clk_vivobus_mux", clk_mux_vivobus_p,
		ARRAY_SIZE(clk_mux_vivobus_p), 0x060, 0xf00, CLK_MUX_HIWORD_MASK, "clk_vivobus_mux" },
};

static const struct gate_clock media1crg_gate_clks[] = {
	{ CLK_GATE_VIVOBUS, "clk_vivobus", "clk_spll", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_vivobus" },
	{ CLK_GATE_ATDIV_VIVO, "clk_atdiv_vivo", "clk_vivobus_div", 0x010, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_vivo" },
	{ CLK_GATE_ATDIV_VDEC, "clk_atdiv_vdec", "clk_spll", 0x010, 0x4, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_vdec" },
	{ CLK_GATE_ATDIV_VENC, "clk_atdiv_venc", "clk_spll", 0x010, 0x8, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_venc" },
};

static const struct scgt_clock media1crg_scgt_clks[] = {
	{ CLK_GATE_VIVOBUS_ANDGT, "clk_vivobus_gt", "clk_vivobus_mux",
		0x084, 1, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clk_vivobus_gt" },
};

static const struct div_clock media2crg_div_clks[] = {
	{ CLK_DIV_VCODECBUS, "clk_vcodbus_div", "clk_vcodbus_gt",
		0x09C, 0x3f, 64, 1, 0, "clk_vcodbus_div" },
	{ CLK_DIV_ISPCPU, "div_ispcpu", "clk_gt_ispcpu",
		0x0A8, 0x3f00, 64, 1, 0, "div_ispcpu" },
	{ CLK_DIV_ISPCPU_GIC, "clk_ispcpu_gic_div", "clk_ispcpu_gic_gt",
		0x0B4, 0x3f00, 64, 1, 0, "clk_ispcpu_gic_div" },
	{ CLK_DIV_ISPCPU_PERI, "clk_ispcpu_peri_div", "clk_ispcpu_peri_gt",
		0x0B4, 0x3f, 64, 1, 0, "clk_ispcpu_peri_div" },
	{ CLK_DIV_ISPCPU_PERI_CORE, "clk_ispcpu_peri_core_div", "clk_ispcpu_peri_core_gt",
		0x0B8, 0x3f, 64, 1, 0, "clk_ispcpu_peri_core_div" },
	{ CLK_DIV_ISPCPU_DJTAG, "clk_ispcpu_djtag_div", "clk_ispcpu_djtag_gt",
		0x0B8, 0x3f00, 64, 1, 0, "clk_ispcpu_djtag_div" },
	{ CLK_DIV_ISPCPU_DLDO, "clk_ispcpu_dldo_div", "clk_ispcpu_dldo_gt",
		0x0BC, 0x3f, 64, 1, 0, "clk_ispcpu_dldo_div" },
	{ CLK_DIV_ISP_I3C, "clkdiv_isp_i3c", "clk_ppll0_m2",
		0x0A0, 0x3f00, 64, 1, 0, "clkdiv_isp_i3c" },
	{ CLK_DIV_ISP_SPI, "clkdiv_isp_spi", "clk_ppll0_m2",
		0x0A4, 0x3f, 64, 1, 0, "clkdiv_isp_spi" },
};

static const char * const clk_mux_vcodecbus_p [] = {
		"clk_invalid", "clk_ppll2b_m2", "clk_fll_m2", "clk_invalid",
		"clk_ppll0_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll1_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_mux_ispcpu_p_es [] = {
		"clk_invalid", "clk_ppll1_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll2_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll6_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_mux_ispcpu_gic_p [] = {
		"clk_invalid", "clk_ppll2b_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll1_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll2_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_mux_ispcpu_peri_p [] = {
		"clk_invalid", "clk_ppll2b_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll1_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll2_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_mux_ispcpu_peri_core_p [] = {
		"clk_invalid", "clk_ppll2b_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll1_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll2_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const char * const clk_mux_ispcpu_dldo_p [] = {
		"clk_invalid", "clk_ppll2b_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll1_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll2_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const struct mux_clock media2crg_mux_clks_es[] = {
	{ CLK_MUX_VCODECBUS, "clk_vcodbus_mux", clk_mux_vcodecbus_p,
		ARRAY_SIZE(clk_mux_vcodecbus_p), 0x088, 0xf0, CLK_MUX_HIWORD_MASK, "clk_vcodbus_mux" },
	{ CLK_MUX_ISPCPU, "sel_ispcpu", clk_mux_ispcpu_p_es,
		ARRAY_SIZE(clk_mux_ispcpu_p_es), 0x088, 0xf, CLK_MUX_HIWORD_MASK, "sel_ispcpu" },
	{ CLK_MUX_ISPCPU_GIC, "sel_ispcpu_gic", clk_mux_ispcpu_gic_p,
		ARRAY_SIZE(clk_mux_ispcpu_gic_p), 0x090, 0xf, CLK_MUX_HIWORD_MASK, "sel_ispcpu_gic" },
	{ CLK_MUX_ISPCPU_PERI, "sel_ispcpu_peri", clk_mux_ispcpu_peri_p,
		ARRAY_SIZE(clk_mux_ispcpu_peri_p), 0x090, 0xf0, CLK_MUX_HIWORD_MASK, "sel_ispcpu_peri" },
	{ CLK_MUX_ISPCPU_PERI_CORE, "sel_ispcpu_peri_core", clk_mux_ispcpu_peri_core_p,
		ARRAY_SIZE(clk_mux_ispcpu_peri_core_p), 0x090, 0xf00, CLK_MUX_HIWORD_MASK, "sel_ispcpu_peri_core" },
	{ CLK_MUX_ISPCPU_DLDO, "sel_ispcpu_dldo", clk_mux_ispcpu_dldo_p,
		ARRAY_SIZE(clk_mux_ispcpu_dldo_p), 0x094, 0xf00, CLK_MUX_HIWORD_MASK, "sel_ispcpu_dldo" },
};

static const char * const clk_mux_ispcpu_p [] = {
		"clk_invalid", "clk_aupll_m2", "clk_ppll0_m2", "clk_invalid",
		"clk_ppll2b_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_ppll6_m2", "clk_invalid", "clk_invalid", "clk_invalid",
		"clk_invalid", "clk_invalid", "clk_invalid", "clk_invalid"
};
static const struct mux_clock media2crg_mux_clks[] = {
	{ CLK_MUX_VCODECBUS, "clk_vcodbus_mux", clk_mux_vcodecbus_p,
		ARRAY_SIZE(clk_mux_vcodecbus_p), 0x088, 0xf0, CLK_MUX_HIWORD_MASK, "clk_vcodbus_mux" },
	{ CLK_MUX_ISPCPU, "sel_ispcpu", clk_mux_ispcpu_p,
		ARRAY_SIZE(clk_mux_ispcpu_p), 0x088, 0xf, CLK_MUX_HIWORD_MASK, "sel_ispcpu" },
	{ CLK_MUX_ISPCPU_GIC, "sel_ispcpu_gic", clk_mux_ispcpu_gic_p,
		ARRAY_SIZE(clk_mux_ispcpu_gic_p), 0x090, 0xf, CLK_MUX_HIWORD_MASK, "sel_ispcpu_gic" },
	{ CLK_MUX_ISPCPU_PERI, "sel_ispcpu_peri", clk_mux_ispcpu_peri_p,
		ARRAY_SIZE(clk_mux_ispcpu_peri_p), 0x090, 0xf0, CLK_MUX_HIWORD_MASK, "sel_ispcpu_peri" },
	{ CLK_MUX_ISPCPU_PERI_CORE, "sel_ispcpu_peri_core", clk_mux_ispcpu_peri_core_p,
		ARRAY_SIZE(clk_mux_ispcpu_peri_core_p), 0x090, 0xf00, CLK_MUX_HIWORD_MASK, "sel_ispcpu_peri_core" },
	{ CLK_MUX_ISPCPU_DLDO, "sel_ispcpu_dldo", clk_mux_ispcpu_dldo_p,
		ARRAY_SIZE(clk_mux_ispcpu_dldo_p), 0x094, 0xf00, CLK_MUX_HIWORD_MASK, "sel_ispcpu_dldo" },
};

static const struct gate_clock media2crg_gate_clks[] = {
	{ CLK_GATE_VCODECBUS, "clk_vcodecbus", "clk_spll", 0, 0x0, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_vcodecbus" },
	{ CLK_GATE_VCODECBUS2DDR, "clk_vcodecbus2", "clk_vcodbus_div", 0x000, 0x1000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_vcodecbus2" },
	{ CLK_GATE_ISP_SYS, "clk_isp_sys", "clk_media2_tcxo", 0x000, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_sys" },
	{ CLK_GATE_ISPCPUFREQ, "clk_ispcpufreq", "div_ispcpu", 0x000, 0x8000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpufreq" },
	{ CLK_GATE_ISPCPU_GICFREQ, "clk_ispcpu_gicfreq", "clk_ispcpu_gic_div", 0x010, 0x1, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpu_gicfreq" },
	{ CLK_GATE_ISPCPU_PERIFREQ, "clk_ispcpu_perifreq", "clk_ispcpu_peri_div", 0x010, 0x2, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpu_perifreq" },
	{ CLK_GATE_ISPCPU_COREFREQ, "clk_ispcpu_corefreq", "clk_ispcpu_peri_core_div", 0x010, 0x4, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpu_corefreq" },
	{ CLK_GATE_ISPCPU_DJTAG, "clk_ispcpu_djtag", "clk_ispcpu_djtag_div", 0x010, 0x8, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpu_djtag" },
	{ CLK_GATE_ISPCPU_DLDOFREQ, "clk_ispcpu_dldofreq", "clk_ispcpu_dldo_div", 0x010, 0x20, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_ispcpu_dldofreq" },
	{ CLK_GATE_ISP_I3C, "clk_isp_i3c", "clkdiv_isp_i3c", 0x000, 0x2000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_i3c" },
	{ CLK_GATE_ISP_SPIFREQ, "clk_isp_spifreq", "clkdiv_isp_spi", 0x000, 0x80000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_isp_spifreq" },
	{ CLK_GATE_AUTODIV_VCODECBUS, "clk_atdiv_vcbus", "clk_vcodbus_div", 0x000, 0x400000, 0,
		NULL, 0, {0}, {0}, 0, 0, 0, "clk_atdiv_vcbus" },
};

static const struct scgt_clock media2crg_scgt_clks[] = {
	{ CLK_GATE_VCODECBUS_GT, "clk_vcodbus_gt", "clk_vcodbus_mux",
		0x0C0, 5, CLK_GATE_HIWORD_MASK | CLK_GATE_ALWAYS_ON_MASK, "clk_vcodbus_gt" },
	{ CLK_ANDGT_ISPCPU, "clk_gt_ispcpu", "sel_ispcpu",
		0x0C0, 4, CLK_GATE_HIWORD_MASK, "clk_gt_ispcpu" },
	{ CLK_ANDGT_ISPCPU_GIC, "clk_ispcpu_gic_gt", "sel_ispcpu_gic",
		0x0C4, 2, CLK_GATE_HIWORD_MASK, "clk_ispcpu_gic_gt" },
	{ CLK_ANDGT_ISPCPU_PERI, "clk_ispcpu_peri_gt", "sel_ispcpu_peri",
		0x0C4, 3, CLK_GATE_HIWORD_MASK, "clk_ispcpu_peri_gt" },
	{ CLK_ANDGT_ISPCPU_PERI_CORE, "clk_ispcpu_peri_core_gt", "sel_ispcpu_peri_core",
		0x0C4, 4, CLK_GATE_HIWORD_MASK, "clk_ispcpu_peri_core_gt" },
	{ CLK_ANDGT_ISPCPU_DJTAG, "clk_ispcpu_djtag_gt", "clk_ppll0_m2",
		0x0C4, 5, CLK_GATE_HIWORD_MASK, "clk_ispcpu_djtag_gt" },
	{ CLK_ANDGT_ISPCPU_DLDO, "clk_ispcpu_dldo_gt", "sel_ispcpu_dldo",
		0x0C4, 6, CLK_GATE_HIWORD_MASK, "clk_ispcpu_dldo_gt" },
};

static const struct xfreq_clock xfreq_clks[] = {
	{ CLK_CLUSTER0, "cpu-cluster.0", 0, 0, 0, 0x41C, { 0x0001030A, 0x0 }, { 0x0001020A, 0x0 }, "cpu-cluster.0" },
	{ CLK_CLUSTER1, "cpu-cluster.1", 1, 0, 0, 0x41C, { 0x0002030A, 0x0 }, { 0x0002020A, 0x0 }, "cpu-cluster.1" },
	{ CLK_G3D0, "clk_g3d", 2, 0, 0, 0x41C, { 0x0003030A, 0x0 }, { 0x0003020A, 0x0 }, "clk_g3d" },
	{ CLK_DDRC_FREQ0, "clk_ddrc_freq", 3, 0, 0, 0x41C, { 0x00040309, 0x0 }, { 0x0004020A, 0x0 }, "clk_ddrc_freq" },
	{ CLK_DDRC_MAX0, "clk_ddrc_max", 5, 1, 0x8000, 0x230, { 0x00040308, 0x0 }, { 0x0004020A, 0x0 }, "clk_ddrc_max" },
	{ CLK_DDRC_MIN0, "clk_ddrc_min", 4, 1, 0x8000, 0x270, { 0x00040309, 0x0 }, { 0x0004020A, 0x0 }, "clk_ddrc_min" },
	{ CLK_L1BUS_MIN0, "clk_l1bus_min", 6, 1, 0x8000, 0x250, { 0x00040309, 0x0 }, { 0x0004020A, 0x0 }, "clk_l1bus_min" },
};

static const struct pmu_clock pmu_clks[] = {
	{ CLK_GATE_ABB_192, "clk_abb_192", "clkin_sys", 0x06B, 0, 9, 0, "clk_abb_192" },
	{ CLK_RF0, "clk_rf0", "clkin_sys", 0x06F, 0, INVALID_HWSPINLOCK_ID, 0, "clk_rf0" },
	{ CLK_RF1, "clk_rf1", "clkin_sys", 0x070, 0, INVALID_HWSPINLOCK_ID, 0, "clk_rf1" },
	{ CLK_IODIE, "clk_iodie", "clkin_sys", 0x071, 0, INVALID_HWSPINLOCK_ID, 0, "clk_iodie" },
	{ CLK_SERDES, "clk_serdes", "clkin_sys", 0x06B, 0, INVALID_HWSPINLOCK_ID, 0, "clk_serdes" },
	{ CLK_PMU32KA, "clk_pmu32ka", "clkin_ref", 0x073, 0, INVALID_HWSPINLOCK_ID, ALWAYS_ON, "clk_pmu32ka" },
	{ CLK_PMU32KB, "clk_pmu32kb", "clkin_ref", 0x074, 0, INVALID_HWSPINLOCK_ID, 0, "clk_pmu32kb" },
	{ CLK_UWB_32K, "clk_uwb_32k", "clkin_ref", 0x075, 0, INVALID_HWSPINLOCK_ID, 0, "clk_uwb_32k" },
	{ CLK_WIFI, "clk_wifi", "clkin_sys", 0x06D, 0, INVALID_HWSPINLOCK_ID, 0, "clk_wifi" },
	{ CLK_NFC, "clk_nfc", "clkin_sys", 0x06E, 0, INVALID_HWSPINLOCK_ID, 0, "clk_nfc" },
	{ CLK_UWB, "clk_uwb", "clkin_sys", 0x072, 0, INVALID_HWSPINLOCK_ID, 0, "clk_uwb" },
};

static const struct mailbox_clock mailbox_clks[] = {
	{ CLK_TCXO_VOTE, "clk_tcxo_vote", "clk_sys_ini", 0,
		{ 0x000D0002, 0x13 }, { 0x000D0102, 0x13 }, 0, "clk_tcxo_vote" },
};

static const struct dvfs_clock dvfs_clks_es[] = {
	{ CLK_GATE_EDC0_M1, "clk_edc0_m1", "clk_edc0freq", 6, -1, 3, 1,
		{335000, 480000, 558000}, {1, 2, 3, 4}, 640000, 1, "clk_edc0_m1" },
	{ CLK_GATE_PFA_TFT, "clk_pfa_tft", "clk_pfa_tft_freq", 7, -1, 2, 1,
		{279000, 418000}, {0, 1, 2}, 558000, 1, "clk_pfa_tft" },
	{ CLK_GATE_VDEC_PXPC_M1, "clk_vdec_pxpc_m1", "clk_vdec_pxpcfreq", 8, -1, 3, 1,
		{335000, 558000, 640000}, {1, 2, 3, 4}, 640000, 1, "clk_vdec_pxpc_m1" },
	{ CLK_GATE_VENC_M1, "clk_venc_m1", "clk_vencfreq", 9, -1, 3, 1,
		{335000, 418000, 640000}, {1, 2, 3, 4}, 640000, 1, "clk_venc_m1" },
	{ CLK_GATE_ISPCPU_GIC, "clk_ispcpu_gic", "clk_ispcpu_gicfreq", 10, -1, 1, 1,
		{168000}, {0, 2}, 335000, 1, "clk_ispcpu_gic" },
	{ CLK_GATE_ISPCPU_PERI, "clk_ispcpu_peri", "clk_ispcpu_perifreq", 11, -1, 1, 1,
		{168000}, {0, 3}, 335000, 1, "clk_ispcpu_peri" },
	{ CLK_GATE_ISPCPU_DLDO, "clk_ispcpu_dldo", "clk_ispcpu_dldofreq", 12, -1, 1, 1,
		{105000}, {0, 1}, 0, 0, "clk_ispcpu_dldo" },
	{ CLK_GATE_ISPFUNC, "clk_ispfunc", "clk_ispfunc_m2", 14, -1, 1, 1,
		{482000}, {0, 2}, 482000, 1, "clk_ispfunc" },
	{ CLK_GATE_ISPFUNC2, "clk_ispfunc2", "clk_ispfunc2_m2", 16, -1, 1, 1,
		{482000}, {0, 2}, 482000, 1, "clk_ispfunc2" },
	{ CLK_GATE_ISPFUNC3, "clk_ispfunc3", "clk_ispfunc3_m2", 18, -1, 1, 1,
		{482000}, {0, 2}, 482000, 1, "clk_ispfunc3" },
	{ CLK_GATE_ISP_OISP_P, "clk_isp_oisp_p", "clk_isp_oispfreq", 19, -1, 3, 1,
		{335000, 418000, 480000}, {0, 1, 2, 3}, 640000, 1, "clk_isp_oisp_p" },
	{ CLK_GATE_IPP_P, "clk_ipp_p", "clk_ippfreq", 20, -1, 3, 1,
		{335000, 418000, 480000}, {0, 1, 2, 3}, 640000, 1, "clk_ipp_p" },
	{ CLK_GATE_ISP_SPI, "clk_isp_spi", "clk_isp_spifreq", 21, -1, 1, 1,
		{105000}, {0, 1}, 168000, 1, "clk_isp_spi" },
	{ CLK_GATE_PFA, "clk_pfa", "clk_pfa_freq", 22, -1, 2, 1,
		{279000, 418000}, {0, 1, 2}, 558000, 1, "clk_pfa" },
	{ CLK_GATE_EDC0, "clk_edc0", "clk_edc0_m1", 23, -1, 1, 1,
		{558000}, {0, 2}, 640000, 1, "clk_edc0" },
	{ CLK_GATE_VDEC_PXPC, "clk_vdec_pxpc", "clk_vdec_pxpc_m1", 24, -1, 1, 1,
		{640000}, {0, 2}, 640000, 1, "clk_vdec_pxpc" },
	{ CLK_GATE_VENC, "clk_venc", "clk_venc_m1", 25, -1, 1, 1,
		{640000}, {0, 2}, 640000, 1, "clk_venc" },
	{ CLK_GATE_ISP_OISP, "clk_isp_oisp", "clk_isp_oisp_p", 26, -1, 1, 1,
		{480000}, {0, 2}, 640000, 1, "clk_isp_oisp" },
	{ CLK_GATE_IPP, "clk_ipp", "clk_ipp_p", 27, -1, 1, 1,
		{480000}, {0, 2}, 640000, 1, "clk_ipp" },
	{ CLK_GATE_ISPCPU, "clk_ispcpu", "clk_ispcpufreq", 28, -1, 1, 1,
		{105000}, {0, 1}, 1320000, 1, "clk_ispcpu" },
	{ CLK_GATE_ISPCPU_CORE, "clk_ispcpu_core", "clk_ispcpu_corefreq", 29, -1, 1, 1,
		{105000}, {0, 1}, 168000, 1, "clk_ispcpu_core" },
};

static const struct dvfs_clock dvfs_clks[] = {
	{ CLK_GATE_EDC0_M1, "clk_edc0_m1", "clk_edc0freq", 6, -1, 3, 1,
		{335000, 480000, 558000}, {1, 2, 3, 4}, 640000, 1, "clk_edc0_m1" },
	{ CLK_GATE_PFA_TFT, "clk_pfa_tft", "clk_pfa_tft_freq", 7, -1, 2, 1,
		{279000, 418000}, {0, 1, 2}, 558000, 1, "clk_pfa_tft" },
	{ CLK_GATE_VDEC_PXPC_M1, "clk_vdec_pxpc_m1", "clk_vdec_pxpcfreq", 8, -1, 3, 1,
		{335000, 558000, 640000}, {1, 2, 3, 4}, 640000, 1, "clk_vdec_pxpc_m1" },
	{ CLK_GATE_VENC_M1, "clk_venc_m1", "clk_vencfreq", 9, -1, 3, 1,
		{335000, 418000, 640000}, {1, 2, 3, 4}, 640000, 1, "clk_venc_m1" },
	{ CLK_GATE_ISPCPU_GIC, "clk_ispcpu_gic", "clk_ispcpu_gicfreq", 10, -1, 1, 1,
		{168000}, {0, 2}, 335000, 1, "clk_ispcpu_gic" },
	{ CLK_GATE_ISPCPU_PERI, "clk_ispcpu_peri", "clk_ispcpu_perifreq", 11, -1, 1, 1,
		{168000}, {0, 3}, 335000, 1, "clk_ispcpu_peri" },
	{ CLK_GATE_ISPCPU_DLDO, "clk_ispcpu_dldo", "clk_ispcpu_dldofreq", 12, -1, 1, 1,
		{105000}, {0, 1}, 0, 0, "clk_ispcpu_dldo" },
	{ CLK_GATE_ISPFUNC, "clk_ispfunc", "clk_ispfunc_m2", 14, -1, 2, 1,
		{480000, 558000}, {0, 1, 2}, 480000, 1, "clk_ispfunc" },
	{ CLK_GATE_ISPFUNC2, "clk_ispfunc2", "clk_ispfunc2_m2", 16, -1, 2, 1,
		{480000, 558000}, {0, 1, 2}, 480000, 1, "clk_ispfunc2" },
	{ CLK_GATE_ISPFUNC3, "clk_ispfunc3", "clk_ispfunc3_m2", 18, -1, 2, 1,
		{482000, 600000}, {0, 1, 2}, 482000, 1, "clk_ispfunc3" },
	{ CLK_GATE_ISP_OISP_P, "clk_isp_oisp_p", "clk_isp_oispfreq", 19, -1, 3, 1,
		{335000, 418000, 480000}, {0, 1, 2, 3}, 640000, 1, "clk_isp_oisp_p" },
	{ CLK_GATE_IPP_P, "clk_ipp_p", "clk_ippfreq", 20, -1, 3, 1,
		{335000, 418000, 480000}, {0, 1, 2, 3}, 640000, 1, "clk_ipp_p" },
	{ CLK_GATE_ISP_SPI, "clk_isp_spi", "clk_isp_spifreq", 21, -1, 1, 1,
		{105000}, {0, 2}, 168000, 1, "clk_isp_spi" },
	{ CLK_GATE_PFA, "clk_pfa", "clk_pfa_freq", 22, -1, 2, 1,
		{279000, 418000}, {0, 1, 2}, 558000, 1, "clk_pfa" },
	{ CLK_GATE_EDC0, "clk_edc0", "clk_edc0_m1", 23, -1, 1, 1,
		{558000}, {0, 2}, 640000, 1, "clk_edc0" },
	{ CLK_GATE_VDEC_PXPC, "clk_vdec_pxpc", "clk_vdec_pxpc_m1", 24, -1, 1, 1,
		{640000}, {0, 2}, 640000, 1, "clk_vdec_pxpc" },
	{ CLK_GATE_VENC, "clk_venc", "clk_venc_m1", 25, -1, 1, 1,
		{640000}, {0, 2}, 640000, 1, "clk_venc" },
	{ CLK_GATE_ISP_OISP, "clk_isp_oisp", "clk_isp_oisp_p", 26, -1, 1, 1,
		{480000}, {0, 2}, 640000, 1, "clk_isp_oisp" },
	{ CLK_GATE_IPP, "clk_ipp", "clk_ipp_p", 27, -1, 1, 1,
		{480000}, {0, 2}, 640000, 1, "clk_ipp" },
	{ CLK_GATE_ISPCPU, "clk_ispcpu", "clk_ispcpufreq", 28, -1, 1, 1,
		{105000}, {0, 1}, 1320000, 1, "clk_ispcpu" },
	{ CLK_GATE_ISPCPU_CORE, "clk_ispcpu_core", "clk_ispcpu_corefreq", 29, -1, 1, 1,
		{105000}, {0, 1}, 168000, 1, "clk_ispcpu_core" },
};

static struct media_pll_info ppll0_m1_info = MEDIA_PLL_INFO("clk_ppll0_media", 1671168);
static struct media_pll_info ppll1_m1_info = MEDIA_PLL_INFO("clk_ppll1_media", 1440000);
static struct media_pll_info ppll2_m1_info = MEDIA_PLL_INFO("clk_ppll2_media", 964000);
static struct media_pll_info ppll2b_m1_info_es = MEDIA_PLL_INFO("clk_ppll2b_media", 640000);
static struct media_pll_info ppll2b_m1_info = MEDIA_PLL_INFO("clk_ppll2b_media", 1280000);
static struct media_pll_info fll_m1_info = MEDIA_PLL_INFO("clk_fll_media", 258053);

static struct media_pll_info *edc0_plls_es[] = {
	&ppll2b_m1_info_es, &ppll0_m1_info, &ppll1_m1_info, &fll_m1_info,
};

static struct media_pll_info *vdec_plls_es[] = {
	&ppll2b_m1_info_es, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static struct media_pll_info *vdec_pxpc_plls_es[] = {
	&ppll2b_m1_info_es, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static struct media_pll_info *venc_plls_es[] = {
	&ppll2b_m1_info_es, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static const struct fast_dvfs_clock fast_dvfs_clks_media1_es[] = {
	{ CLK_GATE_EDC0FREQ, "clk_edc0freq", { 0x060, 0xF, 0 }, { 0x068, 0x3F, 0 },
		{ 0x084, 0 }, { 0x000, 18 }, edc0_plls_es, ARRAY_SIZE(edc0_plls_es),
		{ 104000, 278000, 334000, 480000, 557000, 640000 }, 5, { 1, 1, 1, 2, 1, 0 },
		{ 0xF0002, 0xF0002, 0xF0002, 0xF0004, 0xF0002, 0xF0001 }, { 16, 6, 5, 3, 3, 1 },
		{ 0x3F000F, 0x3F0005, 0x3F0004, 0x3F0002, 0x3F0002, 0x3F0000 }, NO_ALWAYS_ON, "clk_edc0freq" },
	{ CLK_GATE_VDEC, "clk_vdec", { 0x060, 0xF0, 4 }, { 0x068, 0x3F00, 8 },
		{ 0x084, 4 }, { 0x000, 2 }, vdec_plls_es, ARRAY_SIZE(vdec_plls_es),
		{ 104000, 238000, 334000, 417000, 557000, 720000 }, 5, { 1, 1, 1, 1, 1, 2 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00020, 0xF00020, 0xF00040 }, { 16, 7, 5, 4, 3, 2 },
		{ 0x3F000F00, 0x3F000600, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000100 }, NO_ALWAYS_ON, "clk_vdec" },
	{ CLK_GATE_VDEC_PXPCFREQ, "clk_vdec_pxpcfreq", { 0x064, 0xF, 0 }, { 0x06C, 0x3F, 0 },
		{ 0x084, 3 }, { 0x000, 4 }, vdec_pxpc_plls_es, ARRAY_SIZE(vdec_pxpc_plls_es),
		{ 104000, 238000, 334000, 557000, 640000, 835000 }, 5, { 1, 1, 1, 1, 0, 1 },
		{ 0xF0002, 0xF0002, 0xF0002, 0xF0002, 0xF0001, 0xF0002 }, { 16, 7, 5, 3, 1, 2 },
		{ 0x3F000F, 0x3F0006, 0x3F0004, 0x3F0002, 0x3F0000, 0x3F0001 }, NO_ALWAYS_ON, "clk_vdec_pxpcfreq" },
	{ CLK_GATE_VENCFREQ, "clk_vencfreq", { 0x064, 0xF0, 4 }, { 0x070, 0x3F, 0 },
		{ 0x084, 2 }, { 0x000, 8 }, venc_plls_es, ARRAY_SIZE(venc_plls_es),
		{ 104000, 334000, 417000, 640000, 835000 }, 4, { 1, 1, 1, 0, 1 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00010, 0xF00020 }, { 16, 5, 4, 1, 2 },
		{ 0x3F000F, 0x3F0004, 0x3F0003, 0x3F0000, 0x3F0001 }, NO_ALWAYS_ON, "clk_vencfreq" },
};

static struct media_pll_info *edc0_plls[] = {
	&ppll2b_m1_info, &ppll0_m1_info, &ppll1_m1_info, &fll_m1_info,
};

static struct media_pll_info *vdec_plls[] = {
	&ppll2b_m1_info, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static struct media_pll_info *vdec_pxpc_plls[] = {
	&ppll2b_m1_info, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static struct media_pll_info *venc_plls[] = {
	&ppll2b_m1_info, &ppll0_m1_info, &ppll1_m1_info, &ppll2_m1_info,
};

static const struct fast_dvfs_clock fast_dvfs_clks_media1[] = {
	{ CLK_GATE_EDC0FREQ, "clk_edc0freq", { 0x060, 0xF, 0 }, { 0x068, 0x3F, 0 },
		{ 0x084, 0 }, { 0x000, 18 }, edc0_plls, ARRAY_SIZE(edc0_plls),
		{ 104000, 278000, 334000, 480000, 557000, 640000 }, 5, { 1, 1, 1, 2, 1, 0 },
		{ 0xF0002, 0xF0002, 0xF0002, 0xF0004, 0xF0002, 0xF0001 }, { 16, 6, 5, 3, 3, 2 },
		{ 0x3F000F, 0x3F0005, 0x3F0004, 0x3F0002, 0x3F0002, 0x3F0001 }, NO_ALWAYS_ON, "clk_edc0freq" },
	{ CLK_GATE_VDEC, "clk_vdec", { 0x060, 0xF0, 4 }, { 0x068, 0x3F00, 8 },
		{ 0x084, 4 }, { 0x000, 2 }, vdec_plls, ARRAY_SIZE(vdec_plls),
		{ 104000, 238000, 334000, 417000, 557000, 720000 }, 5, { 1, 1, 1, 1, 1, 2 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00020, 0xF00020, 0xF00040 }, { 16, 7, 5, 4, 3, 2 },
		{ 0x3F000F00, 0x3F000600, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000100 }, NO_ALWAYS_ON, "clk_vdec" },
	{ CLK_GATE_VDEC_PXPCFREQ, "clk_vdec_pxpcfreq", { 0x064, 0xF, 0 }, { 0x06C, 0x3F, 0 },
		{ 0x084, 3 }, { 0x000, 4 }, vdec_pxpc_plls, ARRAY_SIZE(vdec_pxpc_plls),
		{ 104000, 238000, 334000, 557000, 640000, 835000 }, 5, { 1, 1, 1, 1, 0, 1 },
		{ 0xF0002, 0xF0002, 0xF0002, 0xF0002, 0xF0001, 0xF0002 }, { 16, 7, 5, 3, 2, 2 },
		{ 0x3F000F, 0x3F0006, 0x3F0004, 0x3F0002, 0x3F0001, 0x3F0001 }, NO_ALWAYS_ON, "clk_vdec_pxpcfreq" },
	{ CLK_GATE_VENCFREQ, "clk_vencfreq", { 0x064, 0xF0, 4 }, { 0x070, 0x3F, 0 },
		{ 0x084, 2 }, { 0x000, 8 }, venc_plls, ARRAY_SIZE(venc_plls),
		{ 104000, 334000, 417000, 640000, 835000 }, 4, { 1, 1, 1, 0, 1 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00010, 0xF00020 }, { 16, 5, 4, 2, 2 },
		{ 0x3F000F, 0x3F0004, 0x3F0003, 0x3F0001, 0x3F0001 }, NO_ALWAYS_ON, "clk_vencfreq" },
};

static struct media_pll_info ppll0_m2_info = MEDIA_PLL_INFO("clk_ppll0_m2", 1671168);
static struct media_pll_info ppll1_m2_info = MEDIA_PLL_INFO("clk_ppll1_m2", 1440000);
static struct media_pll_info ppll2_m2_info = MEDIA_PLL_INFO("clk_ppll2_m2", 964000);
static struct media_pll_info ppll2b_m2_info_es = MEDIA_PLL_INFO("clk_ppll2b_m2", 640000);
static struct media_pll_info ppll2b_m2_info = MEDIA_PLL_INFO("clk_ppll2b_m2", 1280000);
static struct media_pll_info ppll6_m2_info = ALTER_PLL_INFO("clk_ppll6_m2", 1320000, ALTER_PLL);
static struct media_pll_info fll_m2_info = MEDIA_PLL_INFO("clk_fll_m2", 258053);

static struct media_pll_info *ispfunc_plls_es[] = {
	&ppll6_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static struct media_pll_info *ispfunc2_plls_es[] = {
	&ppll6_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static struct media_pll_info *ispfunc3_plls_es[] = {
	&ppll6_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static struct media_pll_info *isp_oisp_plls_es[] = {
	&ppll2b_m2_info_es, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static struct media_pll_info *ipp_plls_es[] = {
	&ppll2b_m2_info_es, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static const struct fast_dvfs_clock fast_dvfs_clks_media2_es[] = {
	{ CLK_GATE_ISPFUNCFREQ, "clk_ispfuncfreq", { 0x080, 0xF0, 4 }, { 0x098, 0x3F00, 8 },
		{ 0x0C0, 8 }, { 0x000, 18 }, ispfunc_plls_es, ARRAY_SIZE(ispfunc_plls_es),
		{ 104000, 278000, 360000, 417000, 480000 }, 4, { 1, 1, 2, 1, 2 },
		{ 0xF00020, 0xF00020, 0xF00040, 0xF00020, 0xF00040 }, { 16, 6, 4, 4, 3 },
		{ 0x3F000F00, 0x3F000500, 0x3F000300, 0x3F000300, 0x3F000200 }, NO_ALWAYS_ON, "clk_ispfuncfreq" },
	{ CLK_GATE_ISPFUNC2FREQ, "clk_ispfunc2freq", { 0x080, 0xF00, 8 }, { 0x0A8, 0x3F, 0 },
		{ 0x0C0, 7 }, { 0x000, 17 }, ispfunc2_plls_es, ARRAY_SIZE(ispfunc2_plls_es),
		{ 104000, 167000, 278000, 360000, 417000, 480000, 557000, 720000 }, 7, { 1, 1, 1, 2, 1, 2, 1, 2 },
		{ 0xF000200, 0xF000200, 0xF000200, 0xF000400, 0xF000200, 0xF000400, 0xF000200, 0xF000400 },
		{ 16, 10, 6, 4, 4, 3, 3, 2 },
		{ 0x3F000F, 0x3F0009, 0x3F0005, 0x3F0003, 0x3F0003, 0x3F0002, 0x3F0002, 0x3F0001 },
		NO_ALWAYS_ON, "clk_ispfunc2freq" },
	{ CLK_GATE_ISPFUNC3FREQ, "clk_ispfunc3freq", { 0x080, 0xF, 0 }, { 0x098, 0x3F, 0 },
		{ 0x0C0, 6 }, { 0x000, 16 }, ispfunc3_plls_es, ARRAY_SIZE(ispfunc3_plls_es),
		{ 104000, 167000, 278000, 360000, 417000, 482000, 557000, 720000 }, 7, { 1, 1, 1, 2, 1, 3, 1, 2 },
		{ 0xF0002, 0xF0002, 0xF0002, 0xF0004, 0xF0002, 0xF0008, 0xF0002, 0xF0004 }, { 16, 10, 6, 4, 4, 2, 3, 2 },
		{ 0x3F000F, 0x3F0009, 0x3F0005, 0x3F0003, 0x3F0003, 0x3F0001, 0x3F0002, 0x3F0001 },
		NO_ALWAYS_ON, "clk_ispfunc3freq" },
	{ CLK_GATE_ISP_OISPFREQ, "clk_isp_oispfreq", { 0x08C, 0xF0, 4 }, { 0x0A4, 0x3F00, 8 },
		{ 0x0C4, 0 }, { 0x000, 1 }, isp_oisp_plls_es, ARRAY_SIZE(isp_oisp_plls_es),
		{ 104000, 334000, 417000, 480000, 640000 }, 4, { 1, 1, 1, 2, 0 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00040, 0xF00010 }, { 16, 5, 4, 3, 1 },
		{ 0x3F000F00, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000000 }, NO_ALWAYS_ON, "clk_isp_oispfreq" },
	{ CLK_GATE_IPPFREQ, "clk_ippfreq", { 0x08C, 0xF00, 8 }, { 0x0B0, 0x3F00, 8 },
		{ 0x0C4, 1 }, { 0x000, 14 }, ipp_plls_es, ARRAY_SIZE(ipp_plls_es),
		{ 104000, 334000, 417000, 480000, 640000 }, 4, { 1, 1, 1, 2, 0 },
		{ 0xF000200, 0xF000200, 0xF000200, 0xF000400, 0xF000100 }, { 16, 5, 4, 3, 1 },
		{ 0x3F000F00, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000000 }, NO_ALWAYS_ON, "clk_ippfreq" },
};

static struct media_pll_info *ispfunc_plls[] = {
	&ppll6_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2b_m2_info,
};

static struct media_pll_info *ispfunc2_plls[] = {
	&ppll6_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2b_m2_info,
};

static struct media_pll_info *ispfunc3_plls[] = {
	&ppll2_m2_info, &ppll2b_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll6_m2_info,
};

static struct media_pll_info *isp_oisp_plls[] = {
	&ppll2b_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static struct media_pll_info *ipp_plls[] = {
	&ppll2b_m2_info, &ppll0_m2_info, &ppll1_m2_info, &ppll2_m2_info,
};

static const struct fast_dvfs_clock fast_dvfs_clks_media2[] = {
	{ CLK_GATE_ISPFUNCFREQ, "clk_ispfuncfreq", { 0x080, 0xF0, 4 }, { 0x098, 0x3F00, 8 },
		{ 0x0C0, 8 }, { 0x000, 18 }, ispfunc_plls, ARRAY_SIZE(ispfunc_plls),
		{ 104000, 167000, 208000, 240000, 278000, 320000, 360000, 417000, 480000 }, 8,
		{ 1, 1, 1, 2, 1, 3, 2, 1, 2 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00040, 0xF00020, 0xF00080, 0xF00040, 0xF00020, 0xF00040 },
		{ 16, 10, 8, 6, 6, 4, 4, 4, 3 },
		{ 0x3F000F00, 0x3F000900, 0x3F000700, 0x3F000500, 0x3F000500, 0x3F000300, 0x3F000300, 0x3F000300, 0x3F000200 },
		NO_ALWAYS_ON, "clk_ispfuncfreq" },
	{ CLK_GATE_ISPFUNC2FREQ, "clk_ispfunc2freq", { 0x080, 0xF00, 8 }, { 0x0A8, 0x3F, 0 },
		{ 0x0C0, 7 }, { 0x000, 17 }, ispfunc2_plls, ARRAY_SIZE(ispfunc2_plls),
		{ 104000, 167000, 208000, 240000, 278000, 320000, 360000, 417000, 450000, 480000, 557000, 640000, 720000 }, 12,
		{ 1, 1, 1, 2, 1, 3, 2, 1, 0, 2, 1, 3, 2 },
		{ 0xF000200, 0xF000200, 0xF000200, 0xF000400, 0xF000200, 0xF000800, 0xF000400,
		0xF000200, 0xF000100, 0xF000400, 0xF000200, 0xF000800, 0xF000400 },
		{ 16, 10, 8, 6, 6, 4, 4, 4, 2, 3, 3, 2, 2 },
		{ 0x3F000F, 0x3F0009, 0x3F0007, 0x3F0005, 0x3F0005, 0x3F0003, 0x3F0003, 0x3F0003, 0x3F0001,
		0x3F0002, 0x3F0002, 0x3F0001, 0x3F0001 },
		NO_ALWAYS_ON, "clk_ispfunc2freq" },
	{ CLK_GATE_ISPFUNC3FREQ, "clk_ispfunc3freq", { 0x080, 0x100F, 0 }, { 0x098, 0x3F, 0 },
		{ 0x0C0, 6 }, { 0x000, 16 }, ispfunc3_plls, ARRAY_SIZE(ispfunc3_plls),
		{ 104000, 167000, 208000, 240000, 278000, 320000, 360000, 417000,
		450000, 482000, 557000, 600000, 640000, 720000 }, 13,
		{ 2, 2, 2, 3, 2, 1, 3, 2, 4, 0, 4, 4, 1, 3 },
		{ 0x100F0002, 0x100F0002, 0x100F0002, 0x100F0004, 0x100F0002, 0x100F0001, 0x100F0004, 0x100F0002,
		0x100F0008, 0x100F1001, 0x100F0008, 0x100F0008, 0x100F0001, 0x100F0004 },
		{ 16, 10, 8, 6, 6, 4, 4, 4, 2, 2, 2, 2, 2, 2 },
		{ 0x3F000F, 0x3F0009, 0x3F0007, 0x3F0005, 0x3F0005, 0x3F0003, 0x3F0003, 0x3F0003,
		0x3F0001, 0x3F0001, 0x3F0001, 0x3F0001, 0x3F0001, 0x3F0001 },
		NO_ALWAYS_ON, "clk_ispfunc3freq" },
	{ CLK_GATE_ISP_OISPFREQ, "clk_isp_oispfreq", { 0x08C, 0xF0, 4 }, { 0x0A4, 0x3F00, 8 },
		{ 0x0C4, 0 }, { 0x000, 1 }, isp_oisp_plls, ARRAY_SIZE(isp_oisp_plls),
		{ 104000, 334000, 417000, 480000, 640000 }, 4, { 1, 1, 1, 2, 0 },
		{ 0xF00020, 0xF00020, 0xF00020, 0xF00040, 0xF00010 }, { 16, 5, 4, 3, 2 },
		{ 0x3F000F00, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000100 }, NO_ALWAYS_ON, "clk_isp_oispfreq" },
	{ CLK_GATE_IPPFREQ, "clk_ippfreq", { 0x08C, 0xF00, 8 }, { 0x0B0, 0x3F00, 8 },
		{ 0x0C4, 1 }, { 0x000, 14 }, ipp_plls, ARRAY_SIZE(ipp_plls),
		{ 104000, 334000, 417000, 480000, 640000 }, 4, { 1, 1, 1, 2, 0 },
		{ 0xF000200, 0xF000200, 0xF000200, 0xF000400, 0xF000100 }, { 16, 5, 4, 3, 2 },
		{ 0x3F000F00, 0x3F000400, 0x3F000300, 0x3F000200, 0x3F000100 }, CLK_ALL_ON, "clk_ippfreq" },
};

static const struct fixed_factor_clock media1_stub_clks[] = {
	{ CLK_MUX_VIVOBUS, "clk_vivobus_mux", "clk_sys_ini", 1, 1, 0, "clk_vivobus_mux" },
	{ CLK_GATE_VIVOBUS_ANDGT, "clk_vivobus_gt", "clk_sys_ini", 1, 1, 0, "clk_vivobus_gt" },
	{ CLK_DIV_VIVOBUS, "clk_vivobus_div", "clk_sys_ini", 1, 1, 0, "clk_vivobus_div" },
	{ CLK_GATE_VIVOBUS, "clk_vivobus", "clk_sys_ini", 1, 1, 0, "clk_vivobus" },
	{ CLK_GATE_ATDIV_VIVO, "clk_atdiv_vivo", "clk_sys_ini", 1, 1, 0, "clk_atdiv_vivo" },
	{ CLK_GATE_ATDIV_VDEC, "clk_atdiv_vdec", "clk_sys_ini", 1, 1, 0, "clk_atdiv_vdec" },
	{ CLK_GATE_ATDIV_VENC, "clk_atdiv_venc", "clk_sys_ini", 1, 1, 0, "clk_atdiv_venc" },
};

static const struct fixed_factor_clock media2_stub_clks[] = {
	{ CLK_GATE_VCODECBUS, "clk_vcodecbus", "clk_sys_ini", 1, 1, 0, "clk_vcodecbus" },
	{ CLK_GATE_VCODECBUS2DDR, "clk_vcodecbus2", "clk_sys_ini", 1, 1, 0, "clk_vcodecbus2" },
	{ CLK_DIV_VCODECBUS, "clk_vcodbus_div", "clk_sys_ini", 1, 1, 0, "clk_vcodbus_div" },
	{ CLK_GATE_VCODECBUS_GT, "clk_vcodbus_gt", "clk_sys_ini", 1, 1, 0, "clk_vcodbus_gt" },
	{ CLK_MUX_VCODECBUS, "clk_vcodbus_mux", "clk_sys_ini", 1, 1, 0, "clk_vcodbus_mux" },
	{ CLK_GATE_ISP_SYS, "clk_isp_sys", "clk_sys_ini", 1, 1, 0, "clk_isp_sys" },
	{ CLK_MUX_ISPCPU, "sel_ispcpu", "clk_sys_ini", 1, 1, 0, "sel_ispcpu" },
	{ CLK_ANDGT_ISPCPU, "clk_gt_ispcpu", "clk_sys_ini", 1, 1, 0, "clk_gt_ispcpu" },
	{ CLK_DIV_ISPCPU, "div_ispcpu", "clk_sys_ini", 1, 1, 0, "div_ispcpu" },
	{ CLK_GATE_ISPCPUFREQ, "clk_ispcpufreq", "clk_sys_ini", 1, 1, 0, "clk_ispcpufreq" },
	{ CLK_MUX_ISPCPU_GIC, "sel_ispcpu_gic", "clk_sys_ini", 1, 1, 0, "sel_ispcpu_gic" },
	{ CLK_ANDGT_ISPCPU_GIC, "clk_ispcpu_gic_gt", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_gic_gt" },
	{ CLK_DIV_ISPCPU_GIC, "clk_ispcpu_gic_div", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_gic_div" },
	{ CLK_GATE_ISPCPU_GICFREQ, "clk_ispcpu_gicfreq", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_gicfreq" },
	{ CLK_MUX_ISPCPU_PERI, "sel_ispcpu_peri", "clk_sys_ini", 1, 1, 0, "sel_ispcpu_peri" },
	{ CLK_ANDGT_ISPCPU_PERI, "clk_ispcpu_peri_gt", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_peri_gt" },
	{ CLK_DIV_ISPCPU_PERI, "clk_ispcpu_peri_div", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_peri_div" },
	{ CLK_GATE_ISPCPU_PERIFREQ, "clk_ispcpu_perifreq", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_perifreq" },
	{ CLK_MUX_ISPCPU_PERI_CORE, "sel_ispcpu_peri_core", "clk_sys_ini", 1, 1, 0, "sel_ispcpu_peri_core" },
	{ CLK_ANDGT_ISPCPU_PERI_CORE, "clk_ispcpu_peri_core_gt", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_peri_core_gt" },
	{ CLK_DIV_ISPCPU_PERI_CORE, "clk_ispcpu_peri_core_div", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_peri_core_div" },
	{ CLK_GATE_ISPCPU_COREFREQ, "clk_ispcpu_corefreq", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_corefreq" },
	{ CLK_ANDGT_ISPCPU_DJTAG, "clk_ispcpu_djtag_gt", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_djtag_gt" },
	{ CLK_DIV_ISPCPU_DJTAG, "clk_ispcpu_djtag_div", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_djtag_div" },
	{ CLK_GATE_ISPCPU_DJTAG, "clk_ispcpu_djtag", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_djtag" },
	{ CLK_MUX_ISPCPU_DLDO, "sel_ispcpu_dldo", "clk_sys_ini", 1, 1, 0, "sel_ispcpu_dldo" },
	{ CLK_ANDGT_ISPCPU_DLDO, "clk_ispcpu_dldo_gt", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_dldo_gt" },
	{ CLK_DIV_ISPCPU_DLDO, "clk_ispcpu_dldo_div", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_dldo_div" },
	{ CLK_GATE_ISPCPU_DLDOFREQ, "clk_ispcpu_dldofreq", "clk_sys_ini", 1, 1, 0, "clk_ispcpu_dldofreq" },
	{ CLK_DIV_ISP_I3C, "clkdiv_isp_i3c", "clk_sys_ini", 1, 1, 0, "clkdiv_isp_i3c" },
	{ CLK_GATE_ISP_I3C, "clk_isp_i3c", "clk_sys_ini", 1, 1, 0, "clk_isp_i3c" },
	{ CLK_DIV_ISP_SPI, "clkdiv_isp_spi", "clk_sys_ini", 1, 1, 0, "clkdiv_isp_spi" },
	{ CLK_GATE_ISP_SPIFREQ, "clk_isp_spifreq", "clk_sys_ini", 1, 1, 0, "clk_isp_spifreq" },
	{ CLK_GATE_AUTODIV_VCODECBUS, "clk_atdiv_vcbus", "clk_sys_ini", 1, 1, 0, "clk_atdiv_vcbus" },
};

static const struct fixed_factor_clock fast_dvfs_clks_stub_media1[] = {
	{ CLK_GATE_EDC0FREQ, "clk_edc0freq", "clk_sys_ini", 1, 1, 0, "clk_edc0freq" },
	{ CLK_GATE_VDEC, "clk_vdec", "clk_sys_ini", 1, 1, 0, "clk_vdec" },
	{ CLK_GATE_VDEC_PXPCFREQ, "clk_vdec_pxpcfreq", "clk_sys_ini", 1, 1, 0, "clk_vdec_pxpcfreq" },
	{ CLK_GATE_VENCFREQ, "clk_vencfreq", "clk_sys_ini", 1, 1, 0, "clk_vencfreq" },
};

static const struct fixed_factor_clock fast_dvfs_clks_stub_media2[] = {
	{ CLK_GATE_ISPFUNCFREQ, "clk_ispfuncfreq", "clk_sys_ini", 1, 1, 0, "clk_ispfuncfreq" },
	{ CLK_GATE_ISPFUNC2FREQ, "clk_ispfunc2freq", "clk_sys_ini", 1, 1, 0, "clk_ispfunc2freq" },
	{ CLK_GATE_ISPFUNC3FREQ, "clk_ispfunc3freq", "clk_sys_ini", 1, 1, 0, "clk_ispfunc3freq" },
	{ CLK_GATE_ISP_OISPFREQ, "clk_isp_oispfreq", "clk_sys_ini", 1, 1, 0, "clk_isp_oispfreq" },
	{ CLK_GATE_IPPFREQ, "clk_ippfreq", "clk_sys_ini", 1, 1, 0, "clk_ippfreq" },
};

static const struct comb_dvfs_clock comb_dvfs_clks_es[] = {
	{ CLK_GATE_ISPFUNC_M2, "clk_ispfunc_m2", "clk_ispfuncfreq",
		30, 0, 0, 1, 482000, "clk_ispfunc_m2" },
	{ CLK_GATE_ISPFUNC2_M2, "clk_ispfunc2_m2", "clk_ispfunc2freq",
		30, 0, 1, 1, 482000, "clk_ispfunc2_m2" },
	{ CLK_GATE_ISPFUNC3_M2, "clk_ispfunc3_m2", "clk_ispfunc3freq",
		30, 0, 2, 1, 482000, "clk_ispfunc3_m2" },
};

static const struct comb_dvfs_clock comb_dvfs_clks[] = {
	{ CLK_GATE_ISPFUNC_M2, "clk_ispfunc_m2", "clk_ispfuncfreq",
		30, 0, 0, 1, 480000, "clk_ispfunc_m2" },
	{ CLK_GATE_ISPFUNC2_M2, "clk_ispfunc2_m2", "clk_ispfunc2freq",
		30, 0, 1, 1, 480000, "clk_ispfunc2_m2" },
	{ CLK_GATE_ISPFUNC3_M2, "clk_ispfunc3_m2", "clk_ispfunc3freq",
		30, 0, 2, 1, 482000, "clk_ispfunc3_m2" },
};

static const struct comb_clock comb_clks_es[] = {
	{"clk_ispfunc_m2", 105000,
		{ 279000, 279000, 279000, 279000, 360000, 279000,
			279000, 482000, 360000, 418000, 720000 }},
	{"clk_ispfunc2_m2", 105000,
		{ 168000, 279000, 279000, 360000, 360000, 418000,
			480000, 482000, 558000, 720000, 720000 }},
	{"clk_ispfunc3_m2", 105000,
		{ 168000, 279000, 360000, 360000, 360000, 418000,
			482000, 482000, 558000, 720000, 720000 }},
};

struct level_info level_infos_es[] = {
	{30, 0}, {31, 0}, {32, 0}, {33, 0}, {33, 0}, {34, 0},
	{35, 0}, {35, 0}, {36, 0}, {37, 0}, {37, 0},
};

static const struct comb_dvfs comb_dvfs_table_es[] = {
	{ 0, "comb_dvfs_isp", comb_clks_es, ARRAY_SIZE(comb_clks_es),
		level_infos_es, ARRAY_SIZE(level_infos_es) },
};

static const struct comb_clock comb_clks[] = {
	{"clk_ispfunc_m2", 105000,
		{ 240000, 279000, 279000, 320000, 320000, 360000, 320000, 320000,
			320000, 480000, 320000, 360000, 558000, 418000, 418000, 720000 }},
	{"clk_ispfunc2_m2", 105000,
		{ 240000, 279000, 320000, 360000, 360000, 360000, 418000, 450000,
			480000, 480000, 480000, 480000, 558000, 640000, 720000, 720000 }},
	{"clk_ispfunc3_m2", 105000,
		{ 240000, 279000, 320000, 320000, 360000, 360000, 418000, 450000,
			482000, 482000, 557000, 600000, 600000, 640000, 720000, 720000 }},
};

struct level_info level_infos[] = {
	{30, 0}, {31, 0}, {32, 0}, {33, 0}, {34, 0}, {34, 0}, {35, 0}, {36, 0},
	{37, 0}, {37, 0}, {38, 0}, {39, 0}, {39, 0}, {40, 0}, {41, 0}, {41, 0},
};

static const struct comb_dvfs comb_dvfs_table[] = {
	{ 0, "comb_dvfs_isp", comb_clks, ARRAY_SIZE(comb_clks), level_infos, ARRAY_SIZE(level_infos) },
};

static void clk_crgctrl_init(struct device_node *np)
{
	struct clock_data *clk_crgctrl_data = NULL;
	unsigned int nr = ARRAY_SIZE(fixed_clks) +
		ARRAY_SIZE(fsm_pll_clks) +
		ARRAY_SIZE(alter_fsm_pll_clks) +
		ARRAY_SIZE(crgctrl_scgt_clks) +
		ARRAY_SIZE(fixed_factor_clks) +
		ARRAY_SIZE(crgctrl_div_clks) +
		ARRAY_SIZE(crgctrl_mux_clks) +
		ARRAY_SIZE(crgctrl_gate_clks);

	clk_crgctrl_data = clk_init(np, nr);
	if (!clk_crgctrl_data)
		return;

	plat_clk_register_fixed_rate(fixed_clks,
		ARRAY_SIZE(fixed_clks), clk_crgctrl_data);

	plat_clk_register_fsm_pll(fsm_pll_clks,
		ARRAY_SIZE(fsm_pll_clks), clk_crgctrl_data);

	plat_clk_register_alter_fsm_pll(alter_fsm_pll_clks,
		ARRAY_SIZE(alter_fsm_pll_clks), clk_crgctrl_data);

	plat_clk_register_scgt(crgctrl_scgt_clks,
		ARRAY_SIZE(crgctrl_scgt_clks), clk_crgctrl_data);

	plat_clk_register_fixed_factor(fixed_factor_clks,
		ARRAY_SIZE(fixed_factor_clks), clk_crgctrl_data);

	plat_clk_register_divider(crgctrl_div_clks,
		ARRAY_SIZE(crgctrl_div_clks), clk_crgctrl_data);

	plat_clk_register_mux(crgctrl_mux_clks,
		ARRAY_SIZE(crgctrl_mux_clks), clk_crgctrl_data);

	plat_clk_register_gate(crgctrl_gate_clks,
		ARRAY_SIZE(crgctrl_gate_clks), clk_crgctrl_data);
}

CLK_OF_DECLARE_DRIVER(clk_peri_crgctrl,
	"hisilicon,clk-extreme-crgctrl", clk_crgctrl_init);

static void clk_hsdt_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	unsigned int nr = ARRAY_SIZE(hsdtcrg_pll_clks) +
		ARRAY_SIZE(hsdtctrl_gate_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	plat_clk_register_pll(hsdtcrg_pll_clks,
		ARRAY_SIZE(hsdtcrg_pll_clks), clk_data);

	plat_clk_register_gate(hsdtctrl_gate_clks,
		ARRAY_SIZE(hsdtctrl_gate_clks), clk_data);
}

CLK_OF_DECLARE_DRIVER(clk_hsdtcrg,
	"hisilicon,clk-extreme-hsdt-crg", clk_hsdt_init);

static void clk_hsdt1_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	unsigned int nr = ARRAY_SIZE(hsdt1crg_pll_clks) +
		ARRAY_SIZE(hsdt1ctrl_scgt_clks) +
		ARRAY_SIZE(hsdt1ctrl_div_clks) +
		ARRAY_SIZE(hsdt1ctrl_mux_clks) +
		ARRAY_SIZE(hsdt1ctrl_gate_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	plat_clk_register_pll(hsdt1crg_pll_clks,
		ARRAY_SIZE(hsdt1crg_pll_clks), clk_data);

	plat_clk_register_scgt(hsdt1ctrl_scgt_clks,
		ARRAY_SIZE(hsdt1ctrl_scgt_clks), clk_data);

	plat_clk_register_divider(hsdt1ctrl_div_clks,
		ARRAY_SIZE(hsdt1ctrl_div_clks), clk_data);

	plat_clk_register_mux(hsdt1ctrl_mux_clks,
		ARRAY_SIZE(hsdt1ctrl_mux_clks), clk_data);

	plat_clk_register_gate(hsdt1ctrl_gate_clks,
		ARRAY_SIZE(hsdt1ctrl_gate_clks), clk_data);
}

CLK_OF_DECLARE_DRIVER(clk_hsdt1crg,
	"hisilicon,clk-extreme-hsdt1-crg", clk_hsdt1_init);

static void clk_aocrg_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	unsigned int nr = ARRAY_SIZE(aocrg_fsm_pll_clks) +
		ARRAY_SIZE(aocrg_scgt_clks) +
		ARRAY_SIZE(aocrg_div_clks) +
		ARRAY_SIZE(aocrg_mux_clks) +
		ARRAY_SIZE(aocrg_gate_clks) +
		ARRAY_SIZE(aocrg_sec_gate_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	plat_clk_register_fsm_pll(aocrg_fsm_pll_clks,
		ARRAY_SIZE(aocrg_fsm_pll_clks), clk_data);

	plat_clk_register_scgt(aocrg_scgt_clks,
		ARRAY_SIZE(aocrg_scgt_clks), clk_data);

	plat_clk_register_divider(aocrg_div_clks,
		ARRAY_SIZE(aocrg_div_clks), clk_data);

	plat_clk_register_mux(aocrg_mux_clks,
		ARRAY_SIZE(aocrg_mux_clks), clk_data);

	plat_clk_register_gate(aocrg_gate_clks,
		ARRAY_SIZE(aocrg_gate_clks), clk_data);

	plat_clk_register_sec_gate(aocrg_sec_gate_clks,
		ARRAY_SIZE(aocrg_sec_gate_clks), clk_data);
}

CLK_OF_DECLARE_DRIVER(clk_aocrg,
	"hisilicon,clk-extreme-aocrg", clk_aocrg_init);

static void clk_media1_init(struct device_node *np)
{
	unsigned int nr;
	struct clock_data *clk_data = NULL;
	int no_media = media_check(MEDIA1);
	if (no_media)
		nr = ARRAY_SIZE(media1_stub_clks);
	else
		nr = ARRAY_SIZE(media1crg_scgt_clks) +
		ARRAY_SIZE(media1crg_div_clks) +
		ARRAY_SIZE(media1crg_mux_clks) +
		ARRAY_SIZE(media1crg_gate_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	if (no_media) {
		pr_err("[%s] media1 clock won't initialize!\n", __func__);
		plat_clk_register_fixed_factor(media1_stub_clks,
			ARRAY_SIZE(media1_stub_clks), clk_data);
	} else {
		plat_clk_register_divider(media1crg_div_clks,
			ARRAY_SIZE(media1crg_div_clks), clk_data);

		plat_clk_register_mux(media1crg_mux_clks,
			ARRAY_SIZE(media1crg_mux_clks), clk_data);

		plat_clk_register_scgt(media1crg_scgt_clks,
			ARRAY_SIZE(media1crg_scgt_clks), clk_data);

		plat_clk_register_gate(media1crg_gate_clks,
			ARRAY_SIZE(media1crg_gate_clks), clk_data);
	}
}
CLK_OF_DECLARE_DRIVER(clk_media1crg,
	"hisilicon,clk-extreme-media1-crg", clk_media1_init);

static void clk_media2_init(struct device_node *np)
{
	unsigned int nr;
	struct clock_data *clk_data = NULL;
	int no_media = media_check(MEDIA2);
	int chip_type = get_clk_chip_type();

	if (no_media) {
		nr = ARRAY_SIZE(media2_stub_clks);
	} else {
		nr = ARRAY_SIZE(media2crg_scgt_clks) +
			ARRAY_SIZE(media2crg_div_clks) +
			ARRAY_SIZE(media2crg_gate_clks);
		if (chip_type == CHIP_TYPE_CS)
			nr += ARRAY_SIZE(media2crg_mux_clks);
		else
			nr += ARRAY_SIZE(media2crg_mux_clks_es);
	}

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	if (no_media) {
		pr_err("[%s] media2 clock won't initialize!\n", __func__);
		plat_clk_register_fixed_factor(media2_stub_clks,
			ARRAY_SIZE(media2_stub_clks), clk_data);
	} else {
		plat_clk_register_divider(media2crg_div_clks,
			ARRAY_SIZE(media2crg_div_clks), clk_data);

		if (chip_type == CHIP_TYPE_CS)
			plat_clk_register_mux(media2crg_mux_clks,
				ARRAY_SIZE(media2crg_mux_clks), clk_data);
		else
			plat_clk_register_mux(media2crg_mux_clks_es,
				ARRAY_SIZE(media2crg_mux_clks_es), clk_data);

		plat_clk_register_scgt(media2crg_scgt_clks,
			ARRAY_SIZE(media2crg_scgt_clks), clk_data);

		plat_clk_register_gate(media2crg_gate_clks,
			ARRAY_SIZE(media2crg_gate_clks), clk_data);
	}
}
CLK_OF_DECLARE_DRIVER(clk_media2crg,
	"hisilicon,clk-extreme-media2-crg", clk_media2_init);

static void clk_xfreq_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	unsigned int nr = ARRAY_SIZE(xfreq_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	plat_clk_register_xfreq(xfreq_clks,
		ARRAY_SIZE(xfreq_clks), clk_data);
}
CLK_OF_DECLARE_DRIVER(clk_xfreq,
	"hisilicon,clk-extreme-xfreq", clk_xfreq_init);

static void clk_pmu_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	unsigned int nr = ARRAY_SIZE(pmu_clks);

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;

	plat_clk_register_clkpmu(pmu_clks,
		ARRAY_SIZE(pmu_clks), clk_data);
}
CLK_OF_DECLARE_DRIVER(clk_pmu,
	"hisilicon,clk-extreme-pmu", clk_pmu_init);

static void clk_interactive_init(struct device_node *np)
{
	struct clock_data *clk_interactive_data = NULL;
	unsigned int nr = ARRAY_SIZE(mailbox_clks);

	clk_interactive_data = clk_init(np, nr);
	if (!clk_interactive_data)
		return;

	plat_clk_register_mclk(mailbox_clks,
		ARRAY_SIZE(mailbox_clks), clk_interactive_data);
}
CLK_OF_DECLARE_DRIVER(clk_interactive,
	"hisilicon,interactive-clk", clk_interactive_init);

static void clk_fast_dvfs_media1_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	int no_media = media_check(MEDIA1);
	int chip_type = get_clk_chip_type();
	unsigned int nr;

	if (no_media) {
		nr = ARRAY_SIZE(fast_dvfs_clks_stub_media1);
	} else {
		if (chip_type == CHIP_TYPE_CS)
			nr = ARRAY_SIZE(fast_dvfs_clks_media1);
		else
			nr = ARRAY_SIZE(fast_dvfs_clks_media1_es);
	}

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;
	if (no_media) {
		plat_clk_register_fixed_factor(fast_dvfs_clks_stub_media1,
			ARRAY_SIZE(fast_dvfs_clks_stub_media1), clk_data);
	} else {
		if (chip_type == CHIP_TYPE_CS)
			plat_clk_register_fast_dvfs_clk(fast_dvfs_clks_media1,
				ARRAY_SIZE(fast_dvfs_clks_media1), clk_data);
		else
			plat_clk_register_fast_dvfs_clk(fast_dvfs_clks_media1_es,
				ARRAY_SIZE(fast_dvfs_clks_media1_es), clk_data);
	}
}
CLK_OF_DECLARE_DRIVER(clk_fast_dvfs_media1,
	"hisilicon,clk-extreme-fast-dvfs-media1", clk_fast_dvfs_media1_init);

static void clk_fast_dvfs_media2_init(struct device_node *np)
{
	struct clock_data *clk_data = NULL;
	int no_media = media_check(MEDIA2);
	int chip_type = get_clk_chip_type();
	unsigned int nr;

	if (no_media) {
		nr = ARRAY_SIZE(fast_dvfs_clks_stub_media2);
	} else {
		if (chip_type == CHIP_TYPE_CS)
			nr = ARRAY_SIZE(fast_dvfs_clks_media2);
		else
			nr = ARRAY_SIZE(fast_dvfs_clks_media2_es);
	}

	clk_data = clk_init(np, nr);
	if (!clk_data)
		return;
	if (no_media) {
		plat_clk_register_fixed_factor(fast_dvfs_clks_stub_media2,
			ARRAY_SIZE(fast_dvfs_clks_stub_media2), clk_data);
	} else {
		if (chip_type == CHIP_TYPE_CS)
			plat_clk_register_fast_dvfs_clk(fast_dvfs_clks_media2,
				ARRAY_SIZE(fast_dvfs_clks_media2), clk_data);
		else
			plat_clk_register_fast_dvfs_clk(fast_dvfs_clks_media2_es,
				ARRAY_SIZE(fast_dvfs_clks_media2_es), clk_data);
	}
}
CLK_OF_DECLARE_DRIVER(clk_fast_dvfs_media2,
	"hisilicon,clk-extreme-fast-dvfs-media2", clk_fast_dvfs_media2_init);

static void clk_dvfs_init(struct device_node *np)
{
	struct clock_data *clk_dvfs_data = NULL;
	unsigned int nr = 0;
	int chip_type = get_clk_chip_type();
	if (chip_type == CHIP_TYPE_CS)
		nr = ARRAY_SIZE(dvfs_clks) + ARRAY_SIZE(comb_dvfs_clks);
	else
		nr = ARRAY_SIZE(dvfs_clks_es) + ARRAY_SIZE(comb_dvfs_clks_es);

	clk_dvfs_data = clk_init(np, nr);
	if (!clk_dvfs_data)
		return;

	if (chip_type == CHIP_TYPE_CS) {
		plat_clk_register_dvfs_clk(dvfs_clks,
			ARRAY_SIZE(dvfs_clks), clk_dvfs_data);

		plat_clk_register_comb_dvfs_clk(comb_dvfs_clks,
			ARRAY_SIZE(comb_dvfs_clks), clk_dvfs_data);

		plat_clk_register_comb_dvfs(comb_dvfs_table,
			ARRAY_SIZE(comb_dvfs_table));
	} else {
		plat_clk_register_dvfs_clk(dvfs_clks_es,
			ARRAY_SIZE(dvfs_clks_es), clk_dvfs_data);

		plat_clk_register_comb_dvfs_clk(comb_dvfs_clks_es,
			ARRAY_SIZE(comb_dvfs_clks_es), clk_dvfs_data);

		plat_clk_register_comb_dvfs(comb_dvfs_table_es,
			ARRAY_SIZE(comb_dvfs_table_es));
	}
}
CLK_OF_DECLARE_DRIVER(clk_dvfs,
	"hisilicon,clk-extreme-dvfs", clk_dvfs_init);
