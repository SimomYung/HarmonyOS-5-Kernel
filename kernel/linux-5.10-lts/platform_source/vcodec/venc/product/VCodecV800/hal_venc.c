/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: venc register config
 * Create: 2021-12-06
 */
#include "hal_venc.h"
#include <linux/mm_lb/mm_lb.h>
#include "soc_smmuv3_tbu_interface.h"
#include "smmu_cfg.h"
#include "smmu_regs.h"
#ifdef CONFIG_SOC_PG_INFO
#include <platform_include/basicplatform/linux/soc_spec.h>
#include "soc_spec_common.h"
#endif
#ifdef VENC_MCORE_ENABLE
#include "drv_venc_ipc.h"

void venc_mcore_start_encode(struct encode_info *info, S_HEVC_AVC_REGS_TYPE *vedu_reg)
{
	struct ipc_message msg = {0};
	msg.id = IPC_MSG_START_ENCODE;
	msg.data[0] = info->channel.id;
	venc_ipc_send_msg(MBX1, &msg);
}

static void vedu_hal_set_int_mcu(uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_INTMASK.u32),
				 VENC_MASK_WITH_MCU); /* slice_end/soft int1 */
}
#endif

#ifndef VENC_MCORE_ENABLE
static void vedu_hal_set_int_without_mcu(uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_INTMASK.u32),
		CMDLIST_MASK); /* slice_end/soft int0 */
}
#endif

void vedu_hal_set_int(uint32_t *reg_base)
{
#ifdef VENC_MCORE_ENABLE
	vedu_hal_set_int_mcu(reg_base);
#else
	vedu_hal_set_int_without_mcu(reg_base);
#endif
}

#ifndef VENC_MCORE_ENABLE
static void vedu_hal_cfg_without_mcu(volatile uint32_t *base, uint32_t offset, uint32_t value)
{
	hal_cmdlist_wr_cmd_in_len0(offset, value);
}
#endif

void vedu_hal_cfg(volatile uint32_t *base, uint32_t offset, uint32_t value)
{
#ifdef VENC_MCORE_ENABLE
	base[offset / 4] = value;
#else
	vedu_hal_cfg_without_mcu(base, offset, value);
#endif
}

void venc_hal_get_reg_stream_len(struct stream_info *stream_info, uint32_t *reg_base)
{
	int i;
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base; /*lint !e826 */
	volatile U_FUNC_VLCST_DSRPTR00 *base0 = all_reg->FUNC_VLCST_DSRPTR00;
	volatile U_FUNC_VLCST_DSRPTR01 *base1 = all_reg->FUNC_VLCST_DSRPTR01;

	for (i = 0; i < MAX_SLICE_NUM; i++) {
		stream_info->slice_len[i] = base0[i].bits.slc_len0 - base1[i].bits.invalidnum0;
		stream_info->aligned_slice_len[i] = base0[i].bits.slc_len0;
		stream_info->slice_num++;
		if (base1[i].bits.islastslc0)
			break;
	}
}

void venc_hal_cfg_curld_osd01(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	U_VEDU_CURLD_RGB_CLIP D32;

	D32.bits.rgb_clip_min = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.rgb_clip_min;
	D32.bits.rgb_clip_max = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.rgb_clip_max;
	D32.bits.curld_cmp_clk_gt_en = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.curld_cmp_clk_gt_en;
	D32.bits.curld_cmp_tag_en = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.curld_cmp_tag_en;
	D32.bits.curld_cmp_raw_en = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.curld_cmp_raw_en;
	D32.bits.flbc_lut_qp_delta = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.flbc_lut_qp_delta;
	D32.bits.flbc_chroma_qp_offset_strength = channel_cfg->all_reg.VEDU_CURLD_RGB_CLIP.bits.flbc_chroma_qp_offset_strength;

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_RGB_CLIP.u32), D32.u32);
}

static void venc_hal_cfg_ime(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	uint32_t i;

	/* according to <VcodecV800 IT Environment Constrain> 2.1.1 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_INTER_MODE.u32),
		channel_cfg->all_reg.VEDU_IME_INTER_MODE.u32);

	/* according to <VcodecV800 IT Environment Constrain> 2.2 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_LAYER3TO2_THR.u32),
		channel_cfg->all_reg.VEDU_IME_LAYER3TO2_THR.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_LAYER3TO2_THR1.u32),
		channel_cfg->all_reg.VEDU_IME_LAYER3TO2_THR1.u32);

	/* according to <VcodecV800 IT Environment Constrain> 2.3 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_LAYER3TO1_THR.u32),
		channel_cfg->all_reg.VEDU_IME_LAYER3TO1_THR.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_LAYER3TO1_THR1.u32),
		channel_cfg->all_reg.VEDU_IME_LAYER3TO1_THR1.u32);

	/* according to <VcodecV800 IT Environment Constrain> 2.4 ~ 2.8 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_ET_THR.u32),
		channel_cfg->all_reg.VEDU_ME_ET_THR.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_ET_THR_ME_STAT.u32),
		channel_cfg->all_reg.VEDU_ME_ET_THR_ME_STAT.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_REF_BUF_WORD_NUM.u32),
		channel_cfg->all_reg.VEDU_IME_REF_BUF_WORD_NUM.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_IME_INTERPOLATION_FLAG.u32),
		channel_cfg->all_reg.VEDU_IME_INTERPOLATION_FLAG.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_MAX_REGION.u32),
		channel_cfg->all_reg.VEDU_ME_MAX_REGION.u32);

	/* according to <VcodecV800 IT Environment Constrain> 2.4 ~ 2.8 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_TMV_SCALE),
		channel_cfg->all_reg.VEDU_ME_TMV_SCALE);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_TMV_EN.u32),
		channel_cfg->all_reg.VEDU_ME_TMV_EN.u32);

	/* according to <VcodecV800 IT Environment Constrain> 2.9 */
	for (i = 0; i < 20; i++)
		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_ME_ADD1_RANDOM_POINT_LIST.u32) + i * sizeof(uint32_t),
			*(&channel_cfg->all_reg.VEDU_ME_ADD1_RANDOM_POINT_LIST.u32 + i));
}

static void venc_hal_cfg_buffer(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	uint32_t i;

	/* according to <VcodecV800 IT Environment Constrain> 1.16 and 1.17 */
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REC_HEADER_STRIDE.u32),
		channel_cfg->all_reg.VEDU_VCPI_REC_HEADER_STRIDE.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_REF_L0_HEADER_STRIDE.u32),
		channel_cfg->all_reg.VEDU_VCPI_REF_L0_HEADER_STRIDE.u32);

	/* according to <VcodecV800 IT Environment Constrain> 4.1 */
	for (i = 0; i < 16; i++) {
		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMADDR_L_NEW[i]),
			channel_cfg->all_reg.VEDU_VCPI_STRMADDR_L_NEW[i]);

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMADDR_H_NEW[i]),
			channel_cfg->all_reg.VEDU_VCPI_STRMADDR_H_NEW[i]);

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_STRMBUFLEN_NEW[i]),
			channel_cfg->all_reg.VEDU_VCPI_STRMBUFLEN_NEW[i]);
	}
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SLCINFOADDR_L_NEW),
		channel_cfg->all_reg.VEDU_VCPI_SLCINFOADDR_L_NEW);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SLCINFOADDR_H_NEW),
		channel_cfg->all_reg.VEDU_VCPI_SLCINFOADDR_H_NEW);
}

static void venc_hal_cfg_curld_clip(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	{
		U_VEDU_CURLD_CLIP_THR_Y clip_thr;

		clip_thr.bits.clip_min_y =
			channel_cfg->all_reg.VEDU_CURLD_CLIP_THR_Y.bits.clip_min_y;
		clip_thr.bits.clip_max_y =
			channel_cfg->all_reg.VEDU_CURLD_CLIP_THR_Y.bits.clip_max_y;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_CLIP_THR_Y.u32), clip_thr.u32);
	}

	{
		U_VEDU_CURLD_CLIP_THR_C clip_thr;

		clip_thr.bits.clip_min_c =
			channel_cfg->all_reg.VEDU_CURLD_CLIP_THR_C.bits.clip_min_c;
		clip_thr.bits.clip_max_c =
			channel_cfg->all_reg.VEDU_CURLD_CLIP_THR_C.bits.clip_max_c;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_CLIP_THR_C.u32), clip_thr.u32);
	}
}

static void venc_hal_cfg_curld_pre_mod_para(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	{
		U_VEDU_RGB2YUV_COEF_P3 coef_p3;

		coef_p3.bits.rgb2yuv_v_0coef =
			channel_cfg->all_reg.VEDU_RGB2YUV_COEF_P3.bits.rgb2yuv_v_0coef;
		coef_p3.bits.rgb2yuv_v_1coef =
			channel_cfg->all_reg.VEDU_RGB2YUV_COEF_P3.bits.rgb2yuv_v_1coef;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_RGB2YUV_COEF_P3.u32), coef_p3.u32);
	}

	{
		U_VEDU_RGB2YUV_COEF_P4 coef_p4;

		coef_p4.bits.rgb2yuv_v_2coef =
			channel_cfg->all_reg.VEDU_RGB2YUV_COEF_P4.bits.rgb2yuv_v_2coef;
		coef_p4.bits.rgb_v_rnd =
			channel_cfg->all_reg.VEDU_RGB2YUV_COEF_P4.bits.rgb_v_rnd;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_RGB2YUV_COEF_P4.u32), coef_p4.u32);
	}

	{
		U_VEDU_CURLD_NARROW_CHRM narrow_chrm;

		narrow_chrm.bits.curld_narrow_chrm_max =
			channel_cfg->all_reg.VEDU_CURLD_NARROW_CHRM.bits.curld_narrow_chrm_max;
		narrow_chrm.bits.curld_narrow_chrm_min =
			channel_cfg->all_reg.VEDU_CURLD_NARROW_CHRM.bits.curld_narrow_chrm_min;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_NARROW_CHRM.u32), narrow_chrm.u32);
	}

	{
		U_VEDU_CURLD_NARROW_LUMA narrow_luma;

		narrow_luma.bits.curld_narrow_luma_max =
			channel_cfg->all_reg.VEDU_CURLD_NARROW_LUMA.bits.curld_narrow_luma_max;
		narrow_luma.bits.curld_narrow_luma_min =
			channel_cfg->all_reg.VEDU_CURLD_NARROW_LUMA.bits.curld_narrow_luma_min;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_NARROW_LUMA.u32), narrow_luma.u32);
	}

	{
		U_VEDU_CURLD_SRCH_STRIDE src_header_stride;

		src_header_stride.bits.srcyh_stride =
			channel_cfg->all_reg.VEDU_CURLD_SRCH_STRIDE.bits.srcyh_stride;
		src_header_stride.bits.srcch_stride =
			channel_cfg->all_reg.VEDU_CURLD_SRCH_STRIDE.bits.srcch_stride;

		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_SRCH_STRIDE.u32), src_header_stride.u32);
	}
	venc_hal_cfg_curld_clip(channel_cfg, reg_base);
}

static void venc_hal_cfg_rc(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	/* according to <VcodecV800 IT Environment Constrain> 3.1 */
	uint32_t i;
	uint32_t vedu_vcpi_bgl_addr_l = 0xc8; // v800 reserved
	uint32_t vedu_vcpi_bgl_addr_h = 0xcc; // v800 reserved
	uint32_t vedu_vcpi_bgc_addr_l = 0xd0; // v800 reserved
	// we use these registers to share rate control information with mcu
	bgll_reg_type bgll_reg;
	bglh_reg_type bglh_reg;

	for (i = 0; i < 80; i++)
		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_QPG_QP_LAMBDA_CTRL_REG00.u32) + i * sizeof(uint32_t),
			*(&channel_cfg->all_reg.VEDU_QPG_QP_LAMBDA_CTRL_REG00.u32 + i));

	for (i = 0; i < 48; i++)
		vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_QPG_QP_LAMBDA_CTRL_REG80.u32) + i * sizeof(uint32_t),
			*(&channel_cfg->all_reg.VEDU_QPG_QP_LAMBDA_CTRL_REG80.u32 + i));

	bgll_reg.bits.scenemode = channel_cfg->channel.scenemode;
	bgll_reg.bits.is_reset_rc = channel_cfg->channel.is_reset_rc;
	bgll_reg.bits.rcmode = channel_cfg->channel.rcmode;
	bgll_reg.bits.id = channel_cfg->channel.id;
	bglh_reg.bits.gop = channel_cfg->channel.gop;
	bglh_reg.bits.framerate = channel_cfg->channel.framerate;
	vedu_hal_cfg(reg_base, vedu_vcpi_bgl_addr_l, bgll_reg.u32);
	vedu_hal_cfg(reg_base, vedu_vcpi_bgl_addr_h, bglh_reg.u32);
	vedu_hal_cfg(reg_base, vedu_vcpi_bgc_addr_l, channel_cfg->channel.bitrate);
}

static void venc_hal_cfg_misc(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_REFLD_CMP_CLKGATE_EN.u32),
		channel_cfg->all_reg.VEDU_REFLD_CMP_CLKGATE_EN.u32);
}

static void venc_hal_cfg_srdo(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	uint32_t i;
	uint32_t offs_min = offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SRDO_MIN_WEIGHT0.u32);
	uint32_t *min_weight = (uint32_t *)&channel_cfg->all_reg.VEDU_VCPI_SRDO_MIN_WEIGHT0.u32;
	uint32_t offs_max = offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_SRDO_MAX_WEIGHT0.u32);
	uint32_t *max_weight = (uint32_t *)&channel_cfg->all_reg.VEDU_VCPI_SRDO_MAX_WEIGHT0.u32;

	for (i = 0; i < 26; i++) { // 26 : min / max
		vedu_hal_cfg(reg_base, offs_min + 4 * i, min_weight[i]);
		vedu_hal_cfg(reg_base, offs_max + 4 * i, max_weight[i]);
	}

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_INTRA_INTER_CU_EN.u32),
		channel_cfg->all_reg.VEDU_VCPI_INTRA_INTER_CU_EN.u32);
}

static void venc_hal_cfg_water_mark(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	if (!channel_cfg->all_reg.VEDU_CURLD_GCFG.bits.curld_watermark_en) {
		return;
	}

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_WATERMARK_ADDR_L),
		channel_cfg->all_reg.VEDU_CURLD_WATERMARK_ADDR_L);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_WATERMARK_ADDR_H),
		channel_cfg->all_reg.VEDU_CURLD_WATERMARK_ADDR_H);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_WATERMARK_CFG.u32),
		channel_cfg->all_reg.VEDU_CURLD_WATERMARK_CFG.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_WATERMARK_SIZE.u32),
		channel_cfg->all_reg.VEDU_CURLD_WATERMARK_SIZE.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_CURLD_WATERMARK_STRIDE.u32),
		channel_cfg->all_reg.VEDU_CURLD_WATERMARK_STRIDE.u32);
}

static void venc_hal_cfg_vcpi_mode(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	U_VEDU_VCPI_MODE d32;

	d32.u32 = channel_cfg->all_reg.VEDU_VCPI_MODE.u32;
	if (channel_cfg->all_reg.VEDU_VCPI_MODE.bits.vcpi_frame_type == 0)
		d32.bits.vcpi_idr_pic = 1;
	else
		d32.bits.vcpi_idr_pic = 0;
	d32.bits.vedu_selfrst_en = 1;
	d32.bits.vcpi_lcu_rdo_ctrl_en =
		channel_cfg->all_reg.VEDU_VCPI_MODE.bits.vcpi_lcu_rdo_ctrl_en; // v800 add
	d32.bits.vcpi_lcu_zeromotion_en =
		channel_cfg->all_reg.VEDU_VCPI_MODE.bits.vcpi_lcu_zeromotion_en; // v800 add

	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_MODE.u32), d32.u32);
}

// platform differences configured only in the vedu_hal_cfg_reg interface
void venc_hal_cfg_plat_diff_all_cfg(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	venc_hal_cfg_curld_pre_mod_para(channel_cfg, reg_base);
	venc_hal_cfg_srdo(channel_cfg, reg_base);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PINTRA_THRESH2.u32),
		channel_cfg->all_reg.VEDU_VCPI_PINTRA_THRESH2.u32);
	vedu_hal_cfg(reg_base,
		offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_MRG_FORCE_SKIP_EN.u32),
		channel_cfg->all_reg.VEDU_MRG_FORCE_SKIP_EN.u32);
	vedu_hal_cfg(reg_base,
			offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_PIC_STRONG_EN.u32),
			channel_cfg->all_reg.VEDU_VCPI_PIC_STRONG_EN.u32);
}

void venc_hal_set_tbu_swid(uint32_t *reg_base)
{
	uint32_t i;
	uint32_t offset;
	uint32_t swid_cfg;
	SOC_SMMUv3_TBU_SMMU_TBU_SWID_CFG_UNION swid_reg;
	U_VEDU_REFLD_ESP_HINT ref_esp_cache_hint = {0};
	U_VEDU_CACHE_AS_FUL cache_as_ful = {0};
	uint32_t venc_stream_num = 47; // SMMU SW_ID
	uint32_t refld_esp_hind_offset = offsetof(S_HEVC_AVC_REGS_TYPE, SMMU_PRE[0]) + 0x28;
	uint32_t smmu_tbu_swid_cfg_offset = VCODEC_VENC_SMMU_TBU_OFFSET + SMMU_TBU_SWID_CFG_N;

	for (i = 0; i < venc_stream_num; i++) {
		uint32_t swif_cfg = readl(reg_base + smmu_tbu_swid_cfg_offset / 4 + i);
		swid_reg.value = swif_cfg;
		swid_reg.reg.syscache_hint_sel = 1;
		vedu_hal_cfg(reg_base, smmu_tbu_swid_cfg_offset + i * 4, swid_reg.value);
	}

	ref_esp_cache_hint.bits.sc_esp_hint_en = 1;
	ref_esp_cache_hint.bits.sc_esp_hint_l0 = 0x8;
	ref_esp_cache_hint.bits.sc_esp_hint_l1 = 0x9;
	ref_esp_cache_hint.bits.sc_esp_hint_l2 = 0xB;
	vedu_hal_cfg(reg_base, refld_esp_hind_offset, ref_esp_cache_hint.u32);

	for (i = VENC_REFLD_DATA_START_SID; i < VENC_REFLD_DATA_END_SID; i++) {
		offset = offsetof(S_HEVC_AVC_REGS_TYPE, SMMU_PRE[0]) + i * 0x4 + 0xA00;
		cache_as_ful.bits.domain = 0x3; // L3 cache
		cache_as_ful.bits.cache = 0x3; // L3 cache
		cache_as_ful.bits.sc_gid_hint = PID_VENC; // gid
		cache_as_ful.bits.sc_new_hint = 0x8; // no care, hardware will choose ref_esp_cache_hint
		vedu_hal_cfg(reg_base, offset, cache_as_ful.u32);
	}
}

void venc_hal_cfg_platform_diff(struct encode_info *channel_cfg, uint32_t *reg_base)
{
	venc_hal_cfg_ime(channel_cfg, reg_base);
	venc_hal_cfg_buffer(channel_cfg, reg_base);
	venc_hal_cfg_rc(channel_cfg, reg_base);
	venc_hal_cfg_misc(channel_cfg, reg_base);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE,
		VEDU_ISPLD_CFG.u32), channel_cfg->all_reg.VEDU_ISPLD_CFG.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE,
		VEDU_VCPI_FORCE_ZEROMV.u32), channel_cfg->all_reg.VEDU_VCPI_FORCE_ZEROMV.u32);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE,
		VEDU_CURLD_DEPTHCODING.u32), channel_cfg->all_reg.VEDU_CURLD_DEPTHCODING.u32);
	venc_hal_cfg_vcpi_mode(channel_cfg, reg_base);
	venc_hal_cfg_water_mark(channel_cfg, reg_base);
	vedu_hal_cfg(reg_base, offsetof(S_HEVC_AVC_REGS_TYPE, VEDU_VCPI_FORCE_INTRAPU32_NUM.u32),
			channel_cfg->all_reg.VEDU_VCPI_FORCE_INTRAPU32_NUM.u32);
}

bool venc_device_need_bypass(void)
{
#if defined(CONFIG_SOC_PG_INFO) && !defined(SCOUT)
	struct soc_component_pg_info pg_info;
	get_soc_pg_info_by_index(SOC_COMPONENT_VENC_INFO, &pg_info);
	switch (pg_info.tailor_strategy) {
		case SOC_COMPONENT_TAILOR_STRATEGY_1:
			return true;
		case SOC_COMPONENT_NOT_TAILOR:
			return false;
		default:
			VCODEC_FATAL_VENC("invalid pg info index: %d", pg_info.tailor_strategy);
			return true;
	}
#else
	return false;
#endif
}

static int32_t check_internal_buffer_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, const mem_buffer_t *buffer)
{
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REC_YH_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REC_CH_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L0_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L0_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L0_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L0_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L0_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFY_L1_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFC_L1_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFYH_L1_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_REFCH_L1_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_PMELD_L1_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_PMEST_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVST_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_NBI_MVLD_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_QPGLD_INF_ADDR_L, buffer->size);

	return 0;
}

static int32_t check_image_buffer_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, const mem_buffer_t *buffer)
{
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_TUNLCELL_ADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_YADDR_L, buffer->size);
	d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_CADDR_L, buffer->size);

	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_PLANAR ||
		reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV422_PLANAR)
		d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_SRC_VADDR_L, buffer->size);

	if (reg->VEDU_VCPI_STRFMT.bits.vcpi_str_fmt == YUV420_SEMIPLANAR_CMP) {
		d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_YH_ADDR_L, buffer->size);
		d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_VCPI_CH_ADDR_L, buffer->size);
	}

	return 0;
}

static int32_t check_image_stream_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg,
		const mem_buffer_t *stream_buffer, const mem_buffer_t *head_buffer)
{
	uint32_t i;
	uint64_t start_addr, end_addr;

	/* according to <VcodecV700 IT Environment Constrain> 4.1 */
	for (i = 0; i < MAX_SLICE_NUM; i++) {
		start_addr = (((uint64_t)reg->VEDU_VCPI_STRMADDR_H_NEW[i] << 32) & 0xFFFFFFFF00000000) |
			(uint64_t)reg->VEDU_VCPI_STRMADDR_L_NEW[i];
		end_addr = start_addr + (uint64_t)reg->VEDU_VCPI_STRMBUFLEN_NEW[i];

		d_venc_check_cfg_reg_streamaddr_ret(stream_buffer[i].start_phys_addr,
				start_addr, stream_buffer[i].size, stream_buffer[i].share_fd);

		d_venc_check_cfg_reg_streamendaddr_ret(stream_buffer[i].start_phys_addr,
				end_addr, stream_buffer[i].size, stream_buffer[i].share_fd);
	}

	return 0;
}

static int32_t check_watermark_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, const mem_buffer_t *buffer)
{
	if (reg->VEDU_CURLD_GCFG.bits.curld_watermark_en) {
		d_venc_check_cfg_reg_addr_ret(buffer->start_phys_addr, reg->VEDU_CURLD_WATERMARK_ADDR_L, buffer->size);
	}

	return 0;
}

int32_t vedu_hal_check_addr(const S_HEVC_AVC_REGS_TYPE_CFG *reg, venc_buffer_info_t *mem_info)
{
	int32_t ret;

	ret = check_internal_buffer_addr(reg, &mem_info->internal_buffer);
	if (ret) {
		VCODEC_FATAL_VENC("check internal buffer addr failed");
		return VCODEC_FAILURE;
	}

	ret = check_image_buffer_addr(reg, &mem_info->image_buffer);
	if (ret) {
		VCODEC_FATAL_VENC("check image buffer addr failed");
		return VCODEC_FAILURE;
	}

	ret = check_image_stream_addr(reg, &mem_info->stream_buffer[0], &mem_info->stream_head_buffer);
	if (ret) {
		VCODEC_FATAL_VENC("check stream buffer addr failed");
		return VCODEC_FAILURE;
	}

	ret = check_watermark_addr(reg, &mem_info->watermark_buffer);
	if (ret) {
		VCODEC_FATAL_VENC("check water buffer addr failed");
		return VCODEC_FAILURE;
	}
	return 0;
}

void venc_hal_get_reg_mse(struct stream_info *stream_info, uint32_t *reg_base, uint32_t lcu_block_cnt)
{
	S_HEVC_AVC_REGS_TYPE *all_reg = (S_HEVC_AVC_REGS_TYPE *)reg_base;
	uint64_t sum_mse_h;
	uint64_t sum_mse;
	if (all_reg->VEDU_VCPI_MODE.bits.vcpi_protocol == VEDU_H264)
		return;

	sum_mse_h = (uint64_t)all_reg->FUNC_SAO_MSE_CNT.bits.sao2vcpi_mse_sumh;
	lcu_block_cnt = all_reg->FUNC_SAO_MSE_CNT.bits.sao2vcpi_mse_cnt > 0 ?
					all_reg->FUNC_SAO_MSE_CNT.bits.sao2vcpi_mse_cnt : lcu_block_cnt;
	sum_mse = (uint64_t)all_reg->FUNC_SAO_MSE_SUM + (sum_mse_h << LCU_MSE_HIGH_BITS_SHIFT);
	if (all_reg->VEDU_VCPI_MODE.bits.vcpi_10bit_mode == 0)
		// Restore the calculation process of the chip and multiply 1000 to keep three decimal places.
		stream_info->average_lcu_mse = (sum_mse << LCU_MSE_8BIT_SHIFT) * 1000 / LCU_MSE_STAT_AREA / lcu_block_cnt;
	else if (all_reg->VEDU_VCPI_MODE.bits.vcpi_10bit_mode == 1)
		stream_info->average_lcu_mse = (sum_mse << LCU_MSE_10BIT_SHIFT) * 1000 / LCU_MSE_STAT_AREA / lcu_block_cnt;
}
