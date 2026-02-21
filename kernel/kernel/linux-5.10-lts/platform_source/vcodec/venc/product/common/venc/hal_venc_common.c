/*
 * hal_venc_common.c
 *
 * venc register config
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

#include "hal_venc.h"
#include "hal_venc_product.h"
#include "drv_venc_efl.h"

void venc_hal_clr_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg)
{
	vedu_reg->VEDU_VCPI_INTCLR.u32 = 0xFFFFFFFF;
}

void venc_hal_disable_all_int(S_HEVC_AVC_REGS_TYPE *vedu_reg)
{
	vedu_reg->VEDU_VCPI_INTMASK.u32 = 0;
}

void venc_hal_start_encode(S_HEVC_AVC_REGS_TYPE *vedu_reg)
{
	if (vedu_reg) {
		vedu_reg->VEDU_VCPI_START.bits.vcpi_vstart = 0;
		vedu_reg->VEDU_VCPI_START.bits.vcpi_vstart = 1;
	}
}

static void venc_hal_get_reg_qp_venc(struct stream_info *stream_info, uint32_t *reg_base)
{
	uint32_t i;
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;
	volatile U_FUNC_SEL_LUMA_QP0_CNT* qp_cnt = &all_reg->FUNC_SEL_LUMA_QP0_CNT;

	for (i = 0; i < QP_HISTOGRAM_NUM; i++)
		stream_info->luma_qp_cnt[i] = qp_cnt[i].bits.luma_qp0_cnt;
}

void venc_hal_get_reg_venc(struct stream_info *stream_info, uint32_t *reg_base)
{
	uint32_t lcu_size;
	uint32_t lcu_block_num;
	uint32_t lcu8_block_num;
	uint32_t lcu16_block_num;
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;
	uint32_t width = all_reg->VEDU_VCPI_PICSIZE_PIX.bits.vcpi_imgwidth_pix + 1;
	uint32_t height = all_reg->VEDU_VCPI_PICSIZE_PIX.bits.vcpi_imgheight_pix + 1;

	stream_info->is_buf_full =
		all_reg->FUNC_VCPI_RAWINT.bits.vcpi_rint_ve_buffull;
	stream_info->is_eop = all_reg->FUNC_VCPI_RAWINT.bits.vcpi_rint_ve_eop;
	if (all_reg->VEDU_VCPI_MODE.bits.vcpi_protocol == VEDU_H265) {
		stream_info->pic_stream_size = all_reg->FUNC_CABAC_PIC_STRMSIZE;
		stream_info->block_cnt[BLOCK_4X4] =
			all_reg->FUNC_SEL_OPT_4X4_CNT.bits.opt_4x4_cnt;
		stream_info->block_cnt[BLOCK_8X8] =
			all_reg->FUNC_SEL_INTRA_OPT_8X8_CNT.bits.intra_opt_8x8_cnt;
		stream_info->block_cnt[BLOCK_16X16] =
			all_reg->FUNC_SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt * 4;
		stream_info->block_cnt[BLOCK_32X32] =
			all_reg->FUNC_SEL_INTRA_OPT_32X32_CNT.bits.intra_opt_32x32_cnt * 16;
		lcu_size = LCU_SIZE_H265;
		lcu8_block_num = (((width + BASE_BLOCK_SIZE - 1) / BASE_BLOCK_SIZE) *
			((height + BASE_BLOCK_SIZE - 1) / BASE_BLOCK_SIZE));
		stream_info->average_luma_qp =
			all_reg->FUNC_SEL_TOTAL_LUMA_QP.bits.total_luma_qp / lcu8_block_num;
	} else {
		stream_info->pic_stream_size = all_reg->FUNC_VLC_PIC_STRMSIZE;
		stream_info->block_cnt[BLOCK_4X4] =
			all_reg->FUNC_SEL_OPT_4X4_CNT.bits.opt_4x4_cnt;
		stream_info->block_cnt[BLOCK_8X8] =
			all_reg->FUNC_SEL_INTRA_OPT_8X8_CNT.bits.intra_opt_8x8_cnt;
		stream_info->block_cnt[BLOCK_16X16] =
			all_reg->FUNC_SEL_INTRA_OPT_16X16_CNT.bits.intra_opt_16x16_cnt;
		stream_info->block_cnt[BLOCK_32X32] = 0;
		lcu_size = LCU_SIZE_H264;
		lcu16_block_num = (((width + LCU_SIZE_H264 - 1) / LCU_SIZE_H264) *
			((height + LCU_SIZE_H264 - 1) / LCU_SIZE_H264));
		stream_info->average_luma_qp =
			all_reg->FUNC_SEL_TOTAL_LUMA_QP.bits.total_luma_qp / lcu16_block_num;
	}

	lcu_block_num = (((width + lcu_size - 1) / lcu_size) *
		((height + lcu_size - 1) / lcu_size));

	stream_info->ave_madi =
		all_reg->FUNC_PME_MADI_SUM.bits.pme_madi_sum / lcu_block_num;
	stream_info->frame_madp = all_reg->FUNC_PME_MADP_SUM.bits.pme_madp_sum;

	venc_hal_get_reg_mse(stream_info, reg_base, lcu_block_num);
	venc_hal_get_reg_stream_len(stream_info, reg_base);
	venc_hal_get_reg_qp_venc(stream_info, reg_base);
#ifdef VENC_IDLE_LOWPOWER
	all_reg->VEDU_VCPI_LOW_POWER.u32 = 0x7F770005;
#endif
}

#ifdef VENC_MUTI_REF_SUPPORT
void venc_hal_get_pic_stat(struct picture_stats *picture_info, uint32_t *reg_base)
{
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;
	uint32_t pmv_count[23];
	uint32_t skip_count[4]; // 8x8, 16x16, 32x32, 64x64(reserve)
	uint32_t i;

	picture_info->skip_mode_acc = 0;
	picture_info->low_mv_magnitude_sum = 0;
	picture_info->all_mv_magnitude_sum = 0;

	for (i = 0; i < 23; i++) {
		pmv_count[i] = *(&all_reg->FUNC_PMV_MV0_COUNT + i);
	}
	skip_count[0] = all_reg->FUNC_SEL_INTER_SKIP_OPT_8X8_CNT.bits.mrg_skip_opt_8x8_cnt;
	skip_count[1] = all_reg->FUNC_SEL_INTER_SKIP_OPT_16X16_CNT.bits.mrg_skip_opt_16x16_cnt;
	skip_count[2] = all_reg->FUNC_SEL_INTER_SKIP_OPT_32X32_CNT.bits.mrg_skip_opt_32x32_cnt;
	skip_count[3] = all_reg->FUNC_SEL_INTER_SKIP_OPT_64X64_CNT.bits.mrg_skip_opt_64x64_cnt;

	// consider skip to 8x8
	// 8x8                                = 2^0 x (8x8)
	// 16x16 = (2x8) x (2x8) = 4  x (8x8) = 2^2 x (8x8)
	// 32x32 = (4x8) x (4x8) = 16 x (8x8) = 2^4 x (8x8)
	for (i = 0; i < 3; i++) {
		picture_info->skip_mode_acc += (skip_count[i] << (i * 2));
	}

	// consider mv
	for (i = 0; i < 6; i++) {
		picture_info->low_mv_magnitude_sum += pmv_count[i];
	}
	picture_info->all_mv_magnitude_sum = picture_info->low_mv_magnitude_sum;
	for (i = 6; i < 23; i++) {
		picture_info->all_mv_magnitude_sum += pmv_count[i];
	}
}

void venc_hal_printf_pic_stat(struct picture_stats *picture_info, uint32_t *reg_base, uint32_t id, uint32_t frame)
{
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;

	VCODEC_INFO_VENC("chan %u frame %u, skip 8x8 to 64x64: [%u %u %u] %u",
		id, frame,
		all_reg->FUNC_SEL_INTER_SKIP_OPT_8X8_CNT.bits.mrg_skip_opt_8x8_cnt,
		all_reg->FUNC_SEL_INTER_SKIP_OPT_16X16_CNT.bits.mrg_skip_opt_16x16_cnt,
		all_reg->FUNC_SEL_INTER_SKIP_OPT_32X32_CNT.bits.mrg_skip_opt_32x32_cnt,
		all_reg->FUNC_SEL_INTER_SKIP_OPT_64X64_CNT.bits.mrg_skip_opt_64x64_cnt);
	VCODEC_INFO_VENC("chan %u frame %u, mv count: [%u %u %u %u %u %u] %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",
		id, frame,
		all_reg->FUNC_PMV_MV0_COUNT,
		all_reg->FUNC_PMV_MV1_COUNT,
		all_reg->FUNC_PMV_MV2_COUNT,
		all_reg->FUNC_PMV_MV3_COUNT,
		all_reg->FUNC_PMV_MV4_COUNT,
		all_reg->FUNC_PMV_MV5_COUNT,
		all_reg->FUNC_PMV_MV6_COUNT,
		all_reg->FUNC_PMV_MV7_COUNT,
		all_reg->FUNC_PMV_MV8_COUNT,
		all_reg->FUNC_PMV_MV9_COUNT,
		all_reg->FUNC_PMV_MV10_COUNT,
		all_reg->FUNC_PMV_MV11_COUNT,
		all_reg->FUNC_PMV_MV12_COUNT,
		all_reg->FUNC_PMV_MV13_COUNT,
		all_reg->FUNC_PMV_MV14_COUNT,
		all_reg->FUNC_PMV_MV15_COUNT,
		all_reg->FUNC_PMV_MV16_COUNT,
		all_reg->FUNC_PMV_MV17_COUNT,
		all_reg->FUNC_PMV_MV18_COUNT,
		all_reg->FUNC_PMV_MV19_COUNT,
		all_reg->FUNC_PMV_MV20_COUNT,
		all_reg->FUNC_PMV_MV21_COUNT,
		all_reg->FUNC_PMV_MV22_COUNT);

	VCODEC_INFO_VENC("chan %u frame %u, skip_mode_acc %u", id, frame, picture_info->skip_mode_acc);
	VCODEC_INFO_VENC("chan %u frame %u, low_mv_magnitude_sum %u, all_mv_magnitude_sum %u",
		id, frame, picture_info->low_mv_magnitude_sum, picture_info->all_mv_magnitude_sum);
}
#endif

void venc_hal_get_slice_reg(struct stream_info *stream_info, uint32_t *reg_base)
{
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;

	stream_info->is_buf_full =
		all_reg->FUNC_VCPI_RAWINT.bits.vcpi_rint_ve_buffull;
	venc_hal_get_reg_stream_len(stream_info, reg_base);
}
