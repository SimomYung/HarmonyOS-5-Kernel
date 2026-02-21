/*
 * smmu_tbu.c
 *
 * This is for smmu tbu.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/delay.h>
#include <linux/io.h>
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec_regulator.h"
#include "vfmw_ext.h"
#include "smmu_tbu.h"
#include "vcodec_mcu_ddr_map.h"

#define TBU_ACK_TIMEOUT 1000
// smmu_pre channle id
#define VDE_RCHN_RV_INST	28
#define VDE_RCHN_RV_DATA	29
#define VDE_WCHN_RV_DATA	47

uint32_t axi_press_config = 0;

static uint32_t get_axi_press_config(void)
{
	dprint(PRN_ALWS, "get axi press config %u", axi_press_config);
	return axi_press_config;
}

uint32_t g_AWVALID_DELAY_CFG = 0x10064;
uint32_t g_ARVALID_DELAY_CFG = 0x10064;
uint32_t g_RREADY_DELAY_CFG = 0x103ff;
uint32_t g_WVALID_DELAY_CFG = 0x103ff;
uint32_t g_BVALID_DELAY_CFG = 0x103ff;
// 可以通过ecall调用这个函数调整反压值
static uint32_t set_axi_press_value(uint32_t aw, uint32_t ar,uint32_t rr, uint32_t w, uint32_t b)
{
	g_AWVALID_DELAY_CFG = aw;
	g_ARVALID_DELAY_CFG = ar;
	g_RREADY_DELAY_CFG = rr;
	g_WVALID_DELAY_CFG = w;
	g_BVALID_DELAY_CFG = b;
	return 0;
}

static void axipress(void)
{
	uint8_t *reg_vaddr;
	vdec_entry *vdec;

	if (get_axi_press_config() == 0) {
		return;
	}

	dprint(PRN_ALWS, "axi press config\n");
	vdec = platform_get_drvdata(vdec_get_device());
	if (!vdec->reg_base_vir_addr) {
		dprint(PRN_ERROR, "map smmu pre axi reg error\n");
		return;
	}

	reg_vaddr = vdec->reg_base_vir_addr + 0x406900;

	// parameters
	writel(g_AWVALID_DELAY_CFG, reg_vaddr);
	writel(g_ARVALID_DELAY_CFG, reg_vaddr + 0x4);
	writel(g_RREADY_DELAY_CFG, reg_vaddr + 0x8);
	writel(g_WVALID_DELAY_CFG, reg_vaddr + 0xc);
	writel(g_BVALID_DELAY_CFG, reg_vaddr + 0x10);

	// AXI_PRESS_START
	writel(0, reg_vaddr + 0x20);
	writel(1, reg_vaddr + 0x20);

	dprint(PRN_ALWS, "AXI awvalid:0x%x,arvalid:0x%x,rready:0x%x,wvalid:0x%x,bvalid:0x%x\n",
		   g_AWVALID_DELAY_CFG, g_ARVALID_DELAY_CFG, g_RREADY_DELAY_CFG, g_WVALID_DELAY_CFG, g_BVALID_DELAY_CFG);
}

static bool smmu_is_init(void)
{
	struct smmu_entry *entry = smmu_get_entry();
	return (entry->smmu_init == 1);
}

static int smmu_pre_init_intercept(void)
{
	// enable ddr_addr_intercept_en
	writel(1, sid_reg_vir() + 0x50);
	// mcu inst read range
	writel(MCU_TEXT_BASE / 4096, sid_reg_vir() + 0x200 + 4 * VDE_RCHN_RV_INST);
	writel((MCU_TEXT_BASE + MCU_TEXT_SIZE - 1) / 4096, sid_reg_vir() + 0x400 + 4 * VDE_RCHN_RV_INST);
	// mcu data read range, change with memory alloc and free
	writel((VDEC_SEG_BUF_BASE + VDEC_SEG_BUF_TOTAL_SIZE) / 4096, sid_reg_vir() + 0x200 + 4 * VDE_RCHN_RV_DATA);
	writel((VDEC_MCU_HEAP_BASE - 1) / 4096, sid_reg_vir() + 0x400 + 4 * VDE_RCHN_RV_DATA);

	dprint(PRN_DBG, "after set smmu pre mcu: istart:0x%x,iend:0x%x,drstart:0x%x,drend:0x%x,dwstart:0x%x,dwend:0x%x",
		   readl(sid_reg_vir() + 0x200 + 4 * VDE_RCHN_RV_INST),
		   readl(sid_reg_vir() + 0x400 + 4 * VDE_RCHN_RV_INST),
		   readl(sid_reg_vir() + 0x200 + 4 * VDE_RCHN_RV_DATA),
		   readl(sid_reg_vir() + 0x400 + 4 * VDE_RCHN_RV_DATA),
		   readl(sid_reg_vir() + 0x200 + 4 * VDE_WCHN_RV_DATA),
		   readl(sid_reg_vir() + 0x400 + 4 * VDE_WCHN_RV_DATA));
	return 0;
}

void smmu_pre_update_rdstart(uint32_t dstart)
{
	uint8_t *vir = sid_reg_vir();
	if (vir == NULL || !smmu_is_init()) {
		dprint(PRN_WARN, "smmu pre is power off");
		return;
	}

	dprint(PRN_DBG, "before set smmu pre mcu: istart:0x%x,iend:0x%x,drstart:0x%x,drend:0x%x,dwstart:0x%x,dwend:0x%x",
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x200 + 4 * VDE_WCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_WCHN_RV_DATA));

	writel(dstart / 4096, vir + 0x200 + 4 * VDE_RCHN_RV_DATA);

	dprint(PRN_DBG, "after set smmu pre mcu: istart:0x%x,iend:0x%x,drstart:0x%x,drend:0x%x,dwstart:0x%x,dwend:0x%x",
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x200 + 4 * VDE_WCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_WCHN_RV_DATA));
}

void smmu_pre_update_rdend(uint32_t dend)
{
	uint8_t *vir = sid_reg_vir();
	if (vir == NULL || !smmu_is_init()) {
		dprint(PRN_WARN, "smmu pre is power off");
		return;
	}

	dprint(PRN_DBG, "before set smmu pre mcu: istart:0x%x,iend:0x%x,drstart:0x%x,drend:0x%x,dwstart:0x%x,dwend:0x%x",
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x200 + 4 * VDE_WCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_WCHN_RV_DATA));

	writel((dend - 1) / 4096, vir + 0x400 + 4 * VDE_RCHN_RV_DATA);

	dprint(PRN_DBG, "after set smmu pre mcu: istart:0x%x,iend:0x%x,drstart:0x%x,drend:0x%x,dwstart:0x%x,dwend:0x%x",
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_INST),
		   readl(vir + 0x200 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_RCHN_RV_DATA),
		   readl(vir + 0x200 + 4 * VDE_WCHN_RV_DATA),
		   readl(vir + 0x400 + 4 * VDE_WCHN_RV_DATA));
}

int show_smmu_pre_intercept(void)
{
	uint32_t val;
	dprint(PRN_ALWS, "DDR_ADDR_INTERCEPT_EN:0x%x", readl(sid_reg_vir() + 0x50));
	val = readl(sid_reg_vir() + 0x58);
	dprint(PRN_ALWS, "VDEC_DDR_ADDR_OVER_STATE:0x%x,ddr_waddr_over_state:%u,ddr_raddr_over_state:%u", val, val&0x1, (val>>1)&0x1);
	dprint(PRN_ALWS, "VDEC_DDR_RADDR_OVER_ADDR:0x%x", readl(sid_reg_vir() + 0x60));
	dprint(PRN_ALWS, "VDEC_DDR_WADDR_OVER_ADDR:0x%x", readl(sid_reg_vir() + 0x64));
	val = readl(sid_reg_vir() + 0x100);
	dprint(PRN_ALWS, "VDEC_DDR_ADDR_OVERID:0x%x, waddr_over_chid:%u, raddr_over_chid:%u", val, val&0x1f, (val>>6)&0x1f);
	return 0;
}

void smmu_init_one_sid(uint32_t reg_base_offset)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();

	set_sid_reg(reg_base_offset + SMMU_NORM_RSID, 4, 8, 0); /* 8: bit width */
	set_sid_reg(reg_base_offset + SMMU_NORM_WSID, 4, 8, 0); /* 8: bit width */

	set_sid_reg(reg_base_offset + SMMU_NORM_RSSID, 0, 1, 0);
	set_sid_reg(reg_base_offset + SMMU_NORM_WSSID, 0, 1, 0);

	set_sid_reg(reg_base_offset + SMMU_NORM_RSSIDV, 1, 1, 0);
	set_sid_reg(reg_base_offset + SMMU_NORM_WSSIDV, 1, 1, 0);

	if (ops->smmu_cfg_mcu_ssid)
		ops->smmu_cfg_mcu_ssid(reg_base_offset);

	set_sid_reg(reg_base_offset + 0x20, 1, 1, 0); // smmu en

	smmu_pre_init_intercept();
	axipress();
}

int32_t smmu_init_one_tbu(uint32_t reg_base_offset)
{
	uint32_t i;
	uint32_t rel;
	uint32_t rel_cr;
	uint32_t ack_timeout;

	vdec_dts *dts_info = &(vdec_plat_get_entry()->dts_info);

	ack_timeout = dts_info->is_fpga ? TBU_ACK_TIMEOUT * 10 : TBU_ACK_TIMEOUT;

	dprint(PRN_ALWS, "is_fpga %d ack_timeout is %d\n", dts_info->is_fpga, ack_timeout);

	set_tbu_reg(reg_base_offset + SMMU_TBU_CR, 0x1, 1, 0);

	for (i = 0; i < ack_timeout; i++) {
		udelay(1);
		rd_smmu_tbu_vreg((reg_base_offset + SMMU_TBU_CRACK), rel);
		if ((rel & 0x3) == 0x3)
			break;
	}

	if (i == ack_timeout) {
		rd_smmu_tbu_vreg((reg_base_offset + SMMU_TBU_CR), rel_cr);
		dprint(PRN_FATAL, "connect timeout, CR 0x%x, CRACK 0x%x\n", rel_cr, rel);
		return SMMU_ERR;
	}

	rd_smmu_tbu_vreg((reg_base_offset + SMMU_TBU_CR), rel_cr);
	if ((rel & 0xff00) < (rel_cr & 0xff00)) {
		dprint(PRN_FATAL, "check tok_trans_gnt failed\n");
		return SMMU_ERR;
	}

	return SMMU_OK;
}

int32_t smmu_deinit_one_tbu(uint32_t reg_base_offset)
{
	uint32_t i;
	uint32_t rel;
	uint32_t ack_timeout;

	vdec_dts *dts_info = &(vdec_plat_get_entry()->dts_info);
	ack_timeout = dts_info->is_fpga ? TBU_ACK_TIMEOUT * 10 : TBU_ACK_TIMEOUT;

	set_tbu_reg(reg_base_offset + SMMU_TBU_CR, 0x0, 1, 0);
	for (i = 0; i < ack_timeout; i++) {
		udelay(1);
		rd_smmu_tbu_vreg((reg_base_offset + SMMU_TBU_CRACK), rel);
		if ((rel & 0x3) == 0x1)
			break;
	}

	if (i == ack_timeout)
		return SMMU_ERR;

	return SMMU_OK;
}

void smmu_init_entry(struct smmu_entry *entry)
{
	struct smmu_tbu_info *tbu_info = &(vdec_plat_get_entry()->smmu_info);
	entry->tbu_info.mmu_tbu_num = tbu_info->mmu_tbu_num;
	entry->tbu_info.mmu_tbu_offset = tbu_info->mmu_tbu_offset;
	entry->tbu_info.mmu_sid_offset = tbu_info->mmu_sid_offset;
}
