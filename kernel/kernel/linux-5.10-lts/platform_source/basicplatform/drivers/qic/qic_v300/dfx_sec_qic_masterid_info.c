/*
 * QIC Master Id Info.
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#include "platform_include/basicplatform/linux/rdr_platform.h"
#include "dfx_sec_qic_masterid_info.h"
#include "dfx_sec_qic_init.h"

static struct masterid_parse g_masterid_info[] = {
	{"CPU_RTM", MODID_AP_S_NOC_CPU_RTM},
	{"PERF_STAT", MODID_AP_S_NOC_PERF_STAT},
	{"IPF", MODID_AP_S_NOC_IPF},
	{"DJTAG_M", MODID_AP_S_NOC_DJTAG_M},
	{"AO_TCP", MODID_AP_S_NOC_AO_TCP},
	{"QSPI", MODID_AP_S_NOC_QSPI},
	{"GICV600", MODID_AP_S_NOC_GICV600},
	{"SPI3_DMA", MODID_AP_S_NOC_SPI3_DMA},
	{"TOP_CSSYS", MODID_AP_S_NOC_TOP_CSSYS},
	{"IOMCU_M7", MODID_AP_S_NOC_IOMCU},
	{"SOCP", MODID_AP_S_NOC_SOCP},
	{"USB3", MODID_AP_S_NOC_USB3},
	{"ASP", MODID_AP_S_NOC_ASP},
	{"DMAC", MODID_AP_S_NOC_DMAC},
	{"FFTS", MODID_AP_S_NOC_FFTS},
	{"SPE", MODID_AP_S_NOC_SPE},
	{"MAA", MODID_AP_S_NOC_MAA},
	{"POWER_STAT", MODID_AP_S_NOC_POWER_STAT},
	{"MSPC", MODID_AP_S_NOC_MSPC},
	{"DDR_LPCTRL", MODID_AP_S_NOC_LPMCU},
	{"EICC", MODID_AP_S_NOC_EICC},
	{"IOMCU_DMA", MODID_AP_S_NOC_IOMCU},
	{"SDIO", MODID_AP_S_NOC_SDIO},
	{"LPMCU", MODID_AP_S_NOC_LPMCU},
	{"NPU", MODID_AP_S_NOC_NPU},
	{"GPU0", MODID_AP_S_NOC_GPU},
	{"GPU_LPCTRL", MODID_AP_S_NOC_GPU},
	{"GPU_SCS", MODID_AP_S_NOC_GPU},
	{"FCM_PP_QICE", MODID_AP_S_NOC_ACPU},
	{"CPU_LPCTRL", MODID_AP_S_NOC_LPMCU},
	{"FCM", MODID_AP_S_NOC_ACPU},
	{"ISP_CPU", MODID_AP_S_NOC_ISP},
	{"VENC", MODID_AP_S_NOC_VENC},
	{"VDEC_0", MODID_AP_S_NOC_VDEC},
	{"VDEC_1", MODID_AP_S_NOC_VDEC},
	{"VDEC_2", MODID_AP_S_NOC_VDEC},
	{"VDEC_3", MODID_AP_S_NOC_VDEC},
	{"VDEC_4", MODID_AP_S_NOC_VDEC},
	{"DSS", MODID_AP_S_NOC_DSS},
	{"MEDIA1_TCU", MODID_AP_S_NOC_MEDIA},
	{"MEDIA2_TCU", MODID_AP_S_NOC_MEDIA},
	{"ISP", MODID_AP_S_NOC_ISP},
	{"IDI2AXI", MODID_AP_S_NOC_IDI2AXI},
	{"ISP_ACE", MODID_AP_S_NOC_ISP},
	{"MINIISP", MODID_AP_S_NOC_ISP},
	{"ISPDMA", MODID_AP_S_NOC_ISP},
	{"ISP_I3C", MODID_AP_S_NOC_ISP},
	{"ISP_OISP_CPU", MODID_AP_S_NOC_ISP},
	{"MUDP", MODID_AP_S_NOC_MUDP},
	{"IPP_SUBSYS", MODID_AP_S_NOC_ISP},
	{"SystemCache", MODID_AP_S_NOC_SYSCACHE},
	{"modem", MODID_AP_S_NOC_MODEM},
	{"MID_HTS_MSG", MODID_AP_S_NOC_MID_MSG},
	{"MID_FFTS_MSG", MODID_AP_S_NOC_MID_MSG},
	{"MID_FFA2_MSG", MODID_AP_S_NOC_MID_MSG},
	{"MID_FFA1_MSG", MODID_AP_S_NOC_MID_MSG},
	{"DFA_HCI_TRACE_WOP", MODID_AP_S_NOC_DFA},
	{"DFA", MODID_AP_S_NOC_DFA},
	{"PCIE_TCU", MODID_AP_S_NOC_PCIE},
	{"PCIe_0", MODID_AP_S_NOC_PCIE},
	{"PCIe_1", MODID_AP_S_NOC_PCIE},
	{"HIEPS_TEE_SCE1", MODID_AP_S_NOC_TEE},
	{"HIEPS_TEE_SCE2", MODID_AP_S_NOC_TEE},
};

static u32 get_mid_info_max(void)
{
	return sizeof(g_masterid_info) / sizeof(g_masterid_info[0]);
}

static struct masterid_parse *get_mid_info(void)
{
	return &g_masterid_info[0];
}

static char *get_mid_name_by_master_id(const struct dfx_sec_qic_device *qic_dev, u32 mid)
{
	u32 i;
 
	for (i = 0; i < qic_dev->mid_info_num; i++) {
		if ((mid >= (qic_dev->mid_info)[i].start) && (mid <= (qic_dev->mid_info)[i].end))
			return (qic_dev->mid_info)[i].mid_name;
	}
 
	return NULL;
}

u32 get_mid_info_sub_reason(const struct dfx_sec_qic_device *qic_dev, u32 mid)
{
	u32 i;
	struct masterid_parse *mid_info = NULL;
	char *mid_name = NULL; 
	mid_info = get_mid_info();
	if (mid_info == NULL) {
		pr_err("[qic]: mid_info is NULL\n");
		(void)(qic_dev);
		return MODID_AP_S_NOC;
	}
	mid_name = get_mid_name_by_master_id(qic_dev, mid);
	if (mid_name == NULL)
		return MODID_AP_S_NOC;
	for (i = 0; i < get_mid_info_max(); i++) {
		if (strncmp(mid_name, mid_info[i].master_name,
			    strlen(mid_info[i].master_name)) == 0)
		{
			return mid_info[i].sub_reason;
		}
	}
	return MODID_AP_S_NOC;
}