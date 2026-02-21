/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: v350 platform cvdr register config
 * Create: 2021-07-20
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "soc_ipp_cvdr_interface.h"
#include "jpeg_platform_config.h"
#include "hjpeg_common.h"
#include "cam_log.h"

#define EXP_PIX 1
#if defined(CONFIG_KERNEL_CAMERA_ISPV350)
#define MEDIA1_CTRL_BASE_ADDR                 (0xFF7C9000)
#endif

#if defined(CONFIG_KERNEL_CAMERA_ISPV370)
#define MEDIA2_CTRL_BASE_ADDR                 (0xFF008000)
#endif
enum cvdr_pix_fmt {
	PIX_FMT_1PF8 = 0x0,
	PIX_FMT_2PF8 = 0x1,
	PIX_FMT_3PF8 = 0x2,
	PIX_FMT_4PF8 = 0x3,
	PIX_FMT_1PF10 = 0x4,
	PIX_FMT_2PF10 = 0x5,
	PIX_FMT_3PF10 = 0x6,
	PIX_FMT_4PF10 = 0x7,
	PIX_FMT_1PF12 = 0x8,
	PIX_FMT_2PF12 = 0x9,
	PIX_FMT_3PF12 = 0xA,
	PIX_FMT_4PF12 = 0xB,
	PIX_FMT_1PF14 = 0xC,
	PIX_FMT_2PF14 = 0xD,
	PIX_FMT_3PF14 = 0xE,
	PIX_FMT_4PF14 = 0xF,
	PIX_FMT_D32 = 0x1C,
	PIX_FMT_D48 = 0x1D,
	PIX_FMT_D64 = 0x1E,
	PIX_FMT_D128 = 0x1F,
	PIX_FMT_INVALID,
};

static void platform_config_cvdr_cfg(unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.max_axiwrite_id = 0x7;
	reg.reg.max_axiread_id = 0xF;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_wr_qos(unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.wr_qos_threshold_01_stop  = 0x1;
	reg.reg.wr_qos_threshold_01_start = 0x1;
	reg.reg.wr_qos_threshold_10_stop  = 0x3;
	reg.reg.wr_qos_threshold_10_start = 0x3;
	reg.reg.wr_qos_threshold_11_stop  = 0x3;
	reg.reg.wr_qos_threshold_11_start = 0x3;
	reg.reg.wr_qos_max = 0x1;
	reg.reg.wr_qos_sr = 0x0;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_rd_qos(unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.rd_qos_threshold_01_stop  = 0x1;
	reg.reg.rd_qos_threshold_01_start = 0x1;
	reg.reg.rd_qos_threshold_10_stop  = 0x3;
	reg.reg.rd_qos_threshold_10_start = 0x3;
	reg.reg.rd_qos_threshold_11_stop  = 0x3;
	reg.reg.rd_qos_threshold_11_start = 0x3;
	reg.reg.rd_qos_max = 0x1;
	reg.reg.rd_qos_sr = 0x0;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_vp_wr_limiter(hjpeg_hw_ctl_t *hw_ctl, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.vpwr_access_limiter_0_30 = hw_ctl->cvdr_prop.wr_limiter;
	reg.reg.vpwr_access_limiter_1_30 = hw_ctl->cvdr_prop.wr_limiter;
	reg.reg.vpwr_access_limiter_2_30 = hw_ctl->cvdr_prop.wr_limiter;
	reg.reg.vpwr_access_limiter_3_30 = hw_ctl->cvdr_prop.wr_limiter;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_vp_wr_cfg(jpgenc_config_t *config, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.vpwr_pixel_format_sid0_30 = PIX_FMT_D64;
	reg.reg.vpwr_pixel_expansion_sid0_30 = EXP_PIX;
	reg.reg.vpwr_last_page_sid0_30  = (config->buffer.output_buffer + config->buffer.output_size) >> 15;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_vp_wr_axi_line(jpgenc_config_t *config, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_UNION reg;

	(void)config;
	reg.value = ioread32(reg_addr);
	reg.reg.vpwr_line_stride_sid0_30 = 0x3F;
	reg.reg.vpwr_line_start_wstrb_sid0_30 = 0xF;
	reg.reg.vpwr_line_wrap_sid0_30  = 0x0;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_vp_wr_if_cfg(hjpeg_hw_ctl_t *hw_ctl, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_UNION reg;

	reg.value = ioread32(reg_addr);
	if (hw_ctl->smmu_bypass == BYPASS_YES || hw_ctl->prefetch_bypass == PREFETCH_BYPASS_YES)
		reg.reg.vpwr_prefetch_bypass_30 = 1;

	if (hw_ctl->smmu_type == ST_SMMUV3) /* v350 is sure SMMUv3, but might be smmu ipp. */
		reg.reg.vp_wr_stop_enable_flux_ctrl_30  = 0;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_vp_wr_axi_fs(jpgenc_config_t *config, unsigned char *cvdr_base)
{
	unsigned int buf_addr = config->buffer.output_buffer + JPGENC_HEAD_SIZE;
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.vpwr_address_frame_start_sid0_30 = buf_addr >> 4;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_nr_rd_cfg(hjpeg_hw_ctl_t *hw_ctl, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_UNION reg;

	reg.value = 0; // use default 0 instead of reading register
	reg.reg.nrrd_allocated_du_4 = hw_ctl->cvdr_prop.allocated_du;
	reg.reg.nrrd_enable_4 = 1;
	reg.reg.nr_rd_stop_enable_pressure_4 = 1;
	reg.reg.nr_rd_stop_enable_flux_ctrl_4 = hw_ctl->smmu_type == ST_SMMUV3 ? 0 : 1;
	iowrite32(reg.value, reg_addr);
}

static void platform_config_cvdr_nr_rd_limiter(hjpeg_hw_ctl_t *hw_ctl, unsigned char *cvdr_base)
{
	void __iomem *reg_addr = SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_ADDR(cvdr_base);
	SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_UNION reg;

	reg.value = ioread32(reg_addr);
	reg.reg.nrrd_access_limiter_0_4 = hw_ctl->cvdr_prop.rd_limiter;
	reg.reg.nrrd_access_limiter_1_4 = hw_ctl->cvdr_prop.rd_limiter;
	reg.reg.nrrd_access_limiter_2_4 = hw_ctl->cvdr_prop.rd_limiter;
	reg.reg.nrrd_access_limiter_3_4 = hw_ctl->cvdr_prop.rd_limiter;
	reg.reg.nrrd_access_limiter_reload_4 = 0xF;
	iowrite32(reg.value, reg_addr);
}

static void platform_dump_cvdr(void __iomem *virtaddr)
{
	uint32_t cvdr_offsets[] = {
		SOC_IPP_CVDR_CVDR_IPP_CVDR_CFG_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_CVDR_WR_QOS_CFG_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_CVDR_RD_QOS_CFG_ADDR(0),

		SOC_IPP_CVDR_CVDR_IPP_VP_WR_LIMITER_30_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_CFG_sid0_30_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_LINE_sid0_30_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_VP_WR_IF_CFG_30_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_AXI_FS_sid0_30_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_NR_RD_CFG_4_ADDR(0),
		SOC_IPP_CVDR_CVDR_IPP_NR_RD_LIMITER_4_ADDR(0),
	};

	platform_dump_jpeg_regs("CVDR", virtaddr, cvdr_offsets, ARRAY_SIZE(cvdr_offsets));
}

void platform_dump_cvdr_debug_reg(hjpeg_hw_ctl_t *hw_ctl)
{
	if (!hw_ctl || !hw_ctl->cvdr_viraddr) {
		cam_err("%s: hw_ctl or virt addr null", __func__);
		return;
	}

	uint32_t i;
	void __iomem *reg_addr;
	for (i = 0; i <= SOC_IPP_CVDR_CVDR_IPP_VP_WR_SID_DEBUG_sid0_30_ADDR(0); i += 4) {
		reg_addr = (void __iomem *)((char *)hw_ctl->cvdr_viraddr + i);
		cam_info("%s: CVDR reg: Offset:%#08x, Value:%#08x", __func__, i, ioread32(reg_addr));
	}

#if defined(CONFIG_KERNEL_CAMERA_ISPV350)
	// dump media1ctrl debug regs
	void __iomem *media1ctrl_viraddr = NULL;
	media1ctrl_viraddr = ioremap(MEDIA1_CTRL_BASE_ADDR, 0x1000);
	if (IS_ERR_OR_NULL(media1ctrl_viraddr)) {
		cam_err("%s: media1 ctrl addr map failed", __func__);
		return;
	}
	uint32_t media1ctrl_offsets[] = { 0x408, 0x40c, 0x800, 0x804, 0x808, 0x818, 0x81c, 0x820, 0xc00 };

	for (i = 0; i < ARRAY_SIZE(media1ctrl_offsets); i++) {
		reg_addr = (void __iomem *)((char *)media1ctrl_viraddr + media1ctrl_offsets[i]);
		cam_info("%s: media1ctrl reg: Offset:%#08x, Value:%#08x", __func__, media1ctrl_offsets[i], ioread32(reg_addr));
	}
	// 0x828 - 0x878
	for (i = 0x828; i <= 0x878; i += 4) {
        reg_addr = (void __iomem *)((char *)media1ctrl_viraddr + i);
        cam_info("%s: media1ctrl reg: Offset:%#08x, Value:%#08x", __func__, i, ioread32(reg_addr));
	}
	iounmap(media1ctrl_viraddr);
#endif

#if defined(CONFIG_KERNEL_CAMERA_ISPV370)
	// dump media2ctrl debug regs
	void __iomem *media2ctrl_viraddr = NULL;
	media2ctrl_viraddr = ioremap(MEDIA2_CTRL_BASE_ADDR, 0x1000);
    if (IS_ERR_OR_NULL(media2ctrl_viraddr)) {
        cam_err("%s: media2 ctrl addr map failed", __func__);
		return;
	}
	uint32_t media2ctrl_offsets[] = {
		0x800, 0x804, 0x808, 0x80c, 0x82c, 0x830, 0x834, 0x838, 0x83c, 0x844,
		0x848, 0x854, 0x858, 0x85c, 0x8a8, 0x8ac, 0x8b0, 0x8b4, 0x8b8
	};
	for (i = 0; i < ARRAY_SIZE(media2ctrl_offsets); i++) {
		reg_addr = (void __iomem *)((char *)media2ctrl_viraddr + media2ctrl_offsets[i]);
		cam_info("%s: media2ctrl reg: Offset:%#08x, Value:%#08x", __func__, media2ctrl_offsets[i], ioread32(reg_addr));
	}
	iounmap(media2ctrl_viraddr);
#endif
}

void platform_config_jpeg_cvdr(hjpeg_hw_ctl_t *hw_ctl, jpgenc_config_t *config)
{
	unsigned char *cvdr_base = hw_ctl->cvdr_viraddr;

	if (cvdr_base == NULL) {
		cam_err("cvdr base null");
		return;
	}

	if (hw_ctl->cvdr_prop.flag == 0) {
		cam_err("%s: need configure 'vendor,cvdr_limiter_du' field in dts", __func__);
		return;
	}

	if (hw_ctl->chip_type == CT_CS) {
		platform_config_cvdr_cfg(cvdr_base);
		platform_config_cvdr_wr_qos(cvdr_base);
		platform_config_cvdr_rd_qos(cvdr_base);
	}

	platform_config_cvdr_vp_wr_limiter(hw_ctl, cvdr_base);
	platform_config_cvdr_vp_wr_if_cfg(hw_ctl, cvdr_base);
	platform_config_cvdr_vp_wr_cfg(config, cvdr_base);
	platform_config_cvdr_vp_wr_axi_line(config, cvdr_base);
	platform_config_cvdr_vp_wr_axi_fs(config, cvdr_base);

	platform_config_cvdr_nr_rd_cfg(hw_ctl, cvdr_base);
	platform_config_cvdr_nr_rd_limiter(hw_ctl, cvdr_base);
	platform_dump_cvdr(cvdr_base);
}
