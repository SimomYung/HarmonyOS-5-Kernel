/*
 * hal_lowdelay_cfg_reg.c
 *
 * This is for wireless lowdelay solution.
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
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

#include "hal_venc.h"
#include "hal_wireless_lowdelay_common_cfg.h"
#include "venc_wireless_lowdelay_cfg.h"

static void venc_cfg_force_zeromv(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_FORCE_ZEROMV),
		channel_cfg->all_reg.VEDU_VCPI_FORCE_ZEROMV.u32);
}

static void venc_cfg_slice_start_num(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_CURLD_START_SLICENUM),
		channel_cfg->all_reg.VEDU_VCPI_CURLD_START_SLICENUM.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_LUMA_START_ROWNUM),
		channel_cfg->all_reg.VEDU_VCPI_LUMA_START_ROWNUM.u32);
}

static void venc_cfg_boundry_info(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	if (channel_cfg->channel.wireless_ldy_info.cur_layer_idx != 0) {
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFLD_BOUNDARY_MOD), 0);
	} else {
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFLD_BOUNDARY_MOD),
			channel_cfg->all_reg.VEDU_VCPI_REFLD_BOUNDARY_MOD.u32);
	}

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE),
		channel_cfg->all_reg.VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE),
		channel_cfg->all_reg.VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REF_UPBOUNDARY_STRIDE),
		channel_cfg->all_reg.VEDU_VCPI_REF_UPBOUNDARY_STRIDE.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE),
		channel_cfg->all_reg.VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE.u32);
}

static void venc_cfg_header(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_NAL_UNIT_HEADL),
		channel_cfg->all_reg.VEDU_CABAC_NAL_UNIT_HEADL);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PRIVATE_SLCHDR_PART0),
		channel_cfg->all_reg.VEDU_VCPI_PRIVATE_SLCHDR_PART0.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PRIVATE_SLCHDR_PART1),
		channel_cfg->all_reg.VEDU_VCPI_PRIVATE_SLCHDR_PART1);
}

static void venc_cfg_high_performance(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_FORCE_INTRAPU32_NUM),
		channel_cfg->all_reg.VEDU_VCPI_FORCE_INTRAPU32_NUM.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCTRL_INTRAPU4_REFRESH_START),
		channel_cfg->all_reg.VEDU_VCTRL_INTRAPU4_REFRESH_START.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCTRL_INTRAPU4_REFRESH_END),
		channel_cfg->all_reg.VEDU_VCTRL_INTRAPU4_REFRESH_END.u32);
}

static void venc_cfg_mac_addr(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	const struct wireless_lowdelay_info *info = &channel_cfg->channel.wireless_ldy_info;

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_VLCST_RAL), info->mac_ral);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_VLCST_SAL), info->mac_sal);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_VLCST_RAH_SAH), info->mac_rah_sah);
}

static void venc_cfg_layer_stream_addr(const struct enhance_layer_info *info, uint32_t *reg_base)
{
	venc_cfg_layer_stream_addr_base_info(info, reg_base);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMADDR_L_NEW[0]),
		info->stream_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMADDR_H_NEW[0]), 0);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMBUFLEN_NEW[0]), info->stream_buf_len);
}

static void venc_cfg_layer_rec_ref_addr(const struct enhance_layer_info *info, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REC_YADDR_L),
		info->rec_y_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REC_CADDR_L),
		info->rec_c_addr & 0xffffffff);

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REC_YH_ADDR_L),
		info->rec_yh_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REC_CH_ADDR_L),
		info->rec_ch_addr & 0xffffffff);
	// REF--------------REF----------------REF-------------------REF0
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFY_L0_ADDR_L),
		info->ref_y_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFC_L0_ADDR_L),
		info->ref_c_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFYH_L0_ADDR_L),
		info->ref_yh_addr & 0xffffffff);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFCH_L0_ADDR_L),
		info->ref_ch_addr & 0xffffffff);
}

static void venc_cfg_layer_pack_info(const struct layer_pack_info *pack, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PACKET_INFO), pack->vcpi_pack_info);
}

static void venc_cfg_pack_info(const struct encode_info *channel_cfg, uint32_t *reg_base)
{
	const struct layer_pack_info *layer_pack = NULL;
	uint16_t cur_layer_idx = channel_cfg->channel.wireless_ldy_info.cur_layer_idx;
	if (cur_layer_idx >= WIRELESS_LOWDELAY_MAX_LAYER)
		return;

	layer_pack = &channel_cfg->channel.wireless_ldy_info.layer_pack[cur_layer_idx];

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SLCINFOADDR_L_NEW),
		layer_pack->pack_slice_info_addr & 0xffffffff);

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SLCINFOADDR_H_NEW), 0);
	venc_cfg_layer_pack_info(layer_pack, reg_base);
}

static void venc_cfg_layer_header(const struct encode_info *channel_cfg,
	const struct enhance_layer_header_info *enhance_layer_header, uint32_t *reg_base)
{
	uint32_t i;

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_SLCHDR_PART1.u32),
		enhance_layer_header->slc_hdr_part1);

	for (i = 0; i < 8; i++)
		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_SLCHDR_PART2_SEG1) + i * sizeof(uint32_t),
			enhance_layer_header->cabac_slc_hdr_part2_seg[i]);

	{
		U_VEDU_CABAC_GLB_CFG d32;

		d32.u32 = 0;
		d32.bits.cabac_max_num_mergecand =
			channel_cfg->all_reg.VEDU_CABAC_GLB_CFG.bits.cabac_max_num_mergecand;
		d32.bits.cabac_nal_unit_head = enhance_layer_header->cabac_nal_unit_head;

		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_GLB_CFG.u32), d32.u32);
	}

	{
		U_VEDU_CABAC_SLCHDR_SIZE d32;

		d32.u32 = 0;
		d32.bits.cabac_slchdr_size_part1 = enhance_layer_header->slc_hdr_size_part1;
		d32.bits.cabac_slchdr_size_part2 = enhance_layer_header->slc_hdr_size_part2;

		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_SLCHDR_SIZE.u32), d32.u32);
	}

	{
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CABAC_SLCHDR_SIZE_I.u32), 0);
	}

	{
		U_VEDU_VCPI_PRIVATE_SLCHDR_PART0 d32;
		d32.bits.vcpi_private_slchdr_part0 = enhance_layer_header->first_slice_segment_in_pic_flag;
		d32.bits.vcpi_private_slcadd_size0 = enhance_layer_header->valid_data_low_16bit;
		d32.bits.vcpi_private_slcadd_size1 = enhance_layer_header->valid_data_high_16bit;
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PRIVATE_SLCHDR_PART0), d32.u32);
	}

	{
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PRIVATE_SLCHDR_PART1),
			enhance_layer_header->slice_segment_address);
	}
}

static void venc_cfg_layer_qp(const struct enhance_layer_info *info,
	uint32_t *reg_base, U_VEDU_VCPI_QPCFG base_layer_qpcfg)
{
	base_layer_qpcfg.bits.vcpi_frm_qp = info->frame_qp;
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_QPCFG.u32), base_layer_qpcfg.u32);
}

static void venc_cfg_enhance_layer(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	const struct wireless_lowdelay_info *info = &channel_cfg->channel.wireless_ldy_info;
	uint16_t cfg_layer_idx = info->cur_layer_idx;

	if (cfg_layer_idx - 1 < 0 || cfg_layer_idx >= WIRELESS_LOWDELAY_MAX_LAYER)
		return;

	venc_cfg_layer_pack_info(&channel_cfg->channel.wireless_ldy_info.layer_pack[cfg_layer_idx - 1], reg_base);

	{
		U_VEDU_VCPI_REFLD_BOUNDARY_MOD d32;

		d32.bits.vcpi_ref_boundary_mod = 0; // enhance layer close boundary search
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REFLD_BOUNDARY_MOD), d32.u32);
	}
	{
		U_VEDU_VCPI_CURLD_START_SLICENUM d32 = channel_cfg->all_reg.VEDU_VCPI_CURLD_START_SLICENUM;

		d32.bits.vcpi_curld_inquire_mod = 0; // enhance layer close inquire serch
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_CURLD_START_SLICENUM), d32.u32);
	}
	{
		venc_cfg_enhance_layer_intra_inter_cu_en(channel_cfg, reg_base);
		if (cfg_layer_idx == 2) {
			U_VEDU_VCTRL_INTRAPU4_REFRESH_START d32 = channel_cfg->all_reg.VEDU_VCTRL_INTRAPU4_REFRESH_START;
			d32.bits.vcpi_intrapu4_refresh_en = 1;
			vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCTRL_INTRAPU4_REFRESH_START), d32.u32);
		}
	}
	{
		U_VEDU_VCPI_FORCE_ZEROMV d32 = channel_cfg->all_reg.VEDU_VCPI_FORCE_ZEROMV;

		d32.bits.vcpi_force_zeromv = 1;
		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_FORCE_ZEROMV), d32.u32);
	}
	venc_cfg_layer_stream_addr(&info->enhance_layer[cfg_layer_idx - 1], reg_base);
	venc_cfg_layer_rec_ref_addr(&info->enhance_layer[cfg_layer_idx - 1], reg_base);
	venc_cfg_layer_pack_info(&channel_cfg->channel.wireless_ldy_info.layer_pack[cfg_layer_idx - 1], reg_base);
	// qp
	venc_cfg_layer_qp(&info->enhance_layer[cfg_layer_idx - 1], reg_base, channel_cfg->all_reg.VEDU_VCPI_QPCFG);
	// header
	venc_cfg_layer_header(channel_cfg, &info->enhance_layer[cfg_layer_idx - 1].enhance_layer_header, reg_base);

	// ehance poc need set
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_0), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[0]);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_1), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[1]);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_2), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[2]);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_3), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[3]);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_4), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[4]);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PMV_POC_5), info->enhance_layer[cfg_layer_idx - 1].pmv_poc[5]);
	// todo: check mcu wheather set max/min qp or not

	{
		U_VEDU_QPG_MAX_MIN_QP d32;

		d32.bits.qpg_max_qp = info->enhance_layer[cfg_layer_idx - 1].frame_qp;
		d32.bits.qpg_cu_qp_delta_enable_flag = channel_cfg->all_reg.VEDU_QPG_MAX_MIN_QP.bits.qpg_cu_qp_delta_enable_flag;
		d32.bits.qpg_min_qp = info->enhance_layer[cfg_layer_idx - 1].frame_qp;

		vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_QPG_MAX_MIN_QP.u32), d32.u32);
	}

	// todo : check mcu wheather set sqrt_lambda_ini or not
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_PPFD_ST_CFG.u32),
		channel_cfg->all_reg.VEDU_PPFD_ST_CFG.u32);
}

void venc_hal_cfg_wireless_lowdelay(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	venc_cfg_force_zeromv(channel_cfg, reg_base);
	venc_cfg_slice_start_num(channel_cfg, reg_base);
	venc_cfg_high_performance(channel_cfg, reg_base);
	venc_cfg_boundry_info(channel_cfg, reg_base);
	venc_cfg_pack_info(channel_cfg, reg_base);
	venc_cfg_mac_addr(channel_cfg, reg_base);
	venc_cfg_header(channel_cfg, reg_base);
	venc_cfg_enhance_layer(channel_cfg, reg_base);
}