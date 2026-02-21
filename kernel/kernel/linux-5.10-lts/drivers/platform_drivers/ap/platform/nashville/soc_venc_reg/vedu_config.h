// ******************************************************************************
// Copyright     :  Copyright (C) 2020, Hisilicon Technologies Co. Ltd.
// File name     :  vedu_config.h
// Project line  :
// Department    :  K5
// Author        :  AnthonySixta
// Version       :  1.0
// Date          :  2013/5/31
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2
// History       :  AnthonySixta 2020/06/02 09:31:24 Create file
// ******************************************************************************

#ifndef __VEDU_CONFIG_H__
#define __VEDU_CONFIG_H__

/* Define the union U_VEDU_VCPI_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_enable_ve_eop    : 1   ; /* [0]  */
        unsigned int    vcpi_enable_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    vcpi_enable_ve_buffull : 1   ; /* [2]  */
        unsigned int    vcpi_enable_ve_pbitsover : 1   ; /* [3]  */
        unsigned int    vcpi_enable_line_pos  : 1   ; /* [4]  */
        unsigned int    vcpi_enable_ppdf_dec_err : 1   ; /* [5]  */
        unsigned int    vcpi_enable_vedu_timeout : 1   ; /* [6]  */
        unsigned int    vcpi_cmdlst_sop_mask  : 1   ; /* [7]  */
        unsigned int    vcpi_cmdlst_eop_mask  : 1   ; /* [8]  */
        unsigned int    vcpi_soft_int0_mask   : 1   ; /* [9]  */
        unsigned int    vcpi_soft_int1_mask   : 1   ; /* [10]  */
        unsigned int    vcpi_soft_int2_mask   : 1   ; /* [11]  */
        unsigned int    vcpi_soft_int3_mask   : 1   ; /* [12]  */
        unsigned int    vcpi_lowlatency_slcint_mask : 1   ; /* [13]  */
        unsigned int    vcpi_enhance0_buffull_int_mask : 1   ; /* [14] */
        unsigned int    vcpi_enhance0_pbitsover_int_mask : 1   ; /* [15] */
        unsigned int    vcpi_enhance1_buffull_int_mask : 1   ; /* [16] */
        unsigned int    vcpi_enhance1_pbitsover_int_mask : 1   ; /* [17] */
        unsigned int    vcpi_enhance2_buffull_int_mask : 1   ; /* [18] */
        unsigned int    vcpi_enhance2_pbitsover_int_mask : 1   ; /* [19] */
        unsigned int    vcpi_hts_error_int_mask : 1   ; /* [20] */
        unsigned int    vcpi_hts_event_int_mask : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTMASK;

/* Define the union U_VEDU_VCPI_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_clr_ve_eop       : 1   ; /* [0]  */
        unsigned int    vcpi_clr_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    vcpi_clr_ve_buffull   : 1   ; /* [2]  */
        unsigned int    vcpi_clr_ve_pbitsover : 1   ; /* [3]  */
        unsigned int    vcpi_clr_line_pos     : 1   ; /* [4]  */
        unsigned int    vcpi_clr_ppdf_dec_err : 1   ; /* [5]  */
        unsigned int    vcpi_clr_vedu_timeout : 1   ; /* [6]  */
        unsigned int    vcpi_clr_cmdlst_sop   : 1   ; /* [7]  */
        unsigned int    vcpi_clr_cmdlst_eop   : 1   ; /* [8]  */
        unsigned int    vcpi_clr_soft_int0    : 1   ; /* [9]  */
        unsigned int    vcpi_clr_soft_int1    : 1   ; /* [10]  */
        unsigned int    vcpi_clr_soft_int2    : 1   ; /* [11]  */
        unsigned int    vcpi_clr_soft_int3    : 1   ; /* [12]  */
        unsigned int    vcpi_clr_lowlatency_slcint : 1   ; /* [13]  */
        unsigned int    vcpi_clr_enhance0_buffull_int : 1   ; /* [14] */
        unsigned int    vcpi_clr_enhance0_pbitsover_int : 1   ; /* [15] */
        unsigned int    vcpi_clr_enhance1_buffull_int : 1   ; /* [16] */
        unsigned int    vcpi_clr_enhance1_pbitsover_int : 1   ; /* [17] */
        unsigned int    vcpi_clr_enhance2_buffull_int : 1   ; /* [18] */
        unsigned int    vcpi_clr_enhance2_pbitsover_int : 1   ; /* [19] */
        unsigned int    vcpi_clr_hts_error_int : 1   ; /* [20] */
        unsigned int    vcpi_clr_hts_event_int : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTCLR;

/* Define the union U_VEDU_VCPI_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vstart           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_START;

/* Define the union U_VEDU_BUS_IDLE_REQ */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vedu_bus_idle_req     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_BUS_IDLE_REQ;

/* Define the union U_VEDU_VCPI_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vedsel           : 1   ; /* [0]  */
        unsigned int    vedu_selfrst_en       : 1   ; /* [1]  */
        unsigned int    vcpi_protocol         : 2   ; /* [3..2]  */
        unsigned int    vcpi_cfg_mode         : 1   ; /* [4]  */
        unsigned int    vcpi_slice_int_en     : 1   ; /* [5]  */
        unsigned int    vcpi_sao_luma         : 1   ; /* [6]  */
        unsigned int    vcpi_sao_chroma       : 1   ; /* [7]  */
        unsigned int    vcpi_rec_cmp_en       : 1   ; /* [8]  */
        unsigned int    vcpi_img_improve_en   : 1   ; /* [9]  */
        unsigned int    vcpi_frame_type       : 2   ; /* [11..10]  */
        unsigned int    vcpi_entropy_mode     : 1   ; /* [12]  */
        unsigned int    vcpi_long_term_refpic : 1   ; /* [13]  */
        unsigned int    vcpi_ref_num          : 1   ; /* [14]  */
        unsigned int    vcpi_pr_inter_en      : 1   ; /* [15]  */
        unsigned int    vcpi_idr_pic          : 1   ; /* [16]  */
        unsigned int    vcpi_pskip_en         : 1   ; /* [17]  */
        unsigned int    vcpi_trans_mode       : 2   ; /* [19..18]  */
        unsigned int    vcpi_blk8_inter       : 1   ; /* [20]  */
        unsigned int    vcpi_sobel_weight_en  : 1   ; /* [21]  */
        unsigned int    vcpi_high_speed_en    : 1   ; /* [22]  */
        unsigned int    vcpi_tiles_en         : 1   ; /* [23]  */
        unsigned int    vcpi_10bit_mode       : 2   ; /* [25..24]  */
        unsigned int    vcpi_lcu_zeromotion_en : 1   ; /* [26] */
        unsigned int    reserved_0            : 1   ; /* [27] */
        unsigned int    vcpi_time_en          : 1   ; /* [28]  */
        unsigned int    vcpi_lcu_rdo_ctrl_en  : 1   ; /* [29] */
        unsigned int    vcpi_ref_cmp_en       : 1   ; /* [30]  */
        unsigned int    vcpi_refc_nload       : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_MODE;

/* Define the union U_VEDU_VCPI_SOFTINTSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_set_soft_int0    : 1   ; /* [0]  */
        unsigned int    vcpi_set_soft_int1    : 1   ; /* [1]  */
        unsigned int    vcpi_set_soft_int2    : 1   ; /* [2]  */
        unsigned int    vcpi_set_soft_int3    : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_SOFTINTSET;

/* Define the union U_VEDU_VCPI_CMDLST_CLKGATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cmdlst_clkgate   : 1   ; /* [0]  */
        unsigned int    vcpi_vcpi2cmdlst_eop_en : 1   ; /* [1]  */
        unsigned int    vcpi_mcu2cmdlst_eop_en : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CMDLST_CLKGATE;

/* Define the union U_VEDU_VCPI_CMD_DPM_SOFTRST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_dpm_softrst      : 1   ; /* [0]  */
        unsigned int    vcpi_cmdlst_softrst   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CMD_DPM_SOFTRST;

/* Define the union U_VEDU_ISPLD_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ime_with_isp_en  : 1   ; /* [0] */
        unsigned int    vcpi_adaptive_ref_mode : 2   ; /* [2..1] */
        unsigned int    vcpi_adapref_skip_mb_num : 6   ; /* [8..3] */
        unsigned int    vcpi_ime_isp_mvconfidient_thr : 4   ; /* [12..9] */
        unsigned int    vcpi_ispld_input_detection_en : 1   ; /* [13] */
        unsigned int    reserved_0            : 18  ; /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_ISPLD_CFG;
/* Define the union U_VEDU_EMAR_SCRAMBLE_TYPE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    emar_rec_b7_scramble  : 1   ; /* [0]  */
        unsigned int    emar_rec_b8_scramble  : 2   ; /* [2..1]  */
        unsigned int    emar_ori_y_b7_scramble : 2   ; /* [4..3]  */
        unsigned int    emar_ori_y_b8_scramble : 2   ; /* [6..5]  */
        unsigned int    emar_ori_y_b9_scramble : 2   ; /* [8..7]  */
        unsigned int    emar_ori_uv_b7_scramble : 2   ; /* [10..9]  */
        unsigned int    emar_ori_uv_b8_scramble : 2   ; /* [12..11]  */
        unsigned int    emar_ori_uv_b9_scramble : 2   ; /* [14..13]  */
        unsigned int    reserved_0            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_EMAR_SCRAMBLE_TYPE;

/* Define the union U_VEDU_VCPI_RC_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_cu_qp_en      : 1   ; /* [0]  */
        unsigned int    vcpi_rc_row_qp_en     : 1   ; /* [1]  */
        unsigned int    vcpi_move_scene_en    : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    vcpi_strong_edge_move_en : 1   ; /* [4]  */
        unsigned int    reserved_1            : 1   ; /* [5]  */
        unsigned int    reserved_2            : 2   ; /* [7..6]  */
        unsigned int    vcpi_rc_low_luma_en   : 1   ; /* [8]  */
        unsigned int    reserved_3            : 1   ; /* [9]  */
        unsigned int    reserved_4            : 1   ; /* [10]  */
        unsigned int    vcpi_rd_min_sad_flag_en : 1   ; /* [11]  */
        unsigned int    reserved_5            : 2   ; /* [13..12]  */
        unsigned int    vcpi_low_min_sad_en   : 1   ; /* [14]  */
        unsigned int    vcpi_prev_min_sad_en  : 1   ; /* [15]  */
        unsigned int    vcpi_qpgld_en         : 1   ; /* [16]  */
        unsigned int    vcpi_map_roikeep_en   : 1   ; /* [17]  */
        unsigned int    vcpi_flat_region_en   : 1   ; /* [18]  */
        unsigned int    vcpi_qp_restrain_large_sad : 1   ; /* [19]  */
        unsigned int    reserved_6            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_RC_ENABLE;

/* Define the union U_VEDU_VLCST_PTBITS_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlcst_ptbits_en       : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VLCST_PTBITS_EN;

/* Define the union U_VEDU_PPFD_ST_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ppfd_st_bypass_en     : 1   ; /* [0]  */
        unsigned int    sqrt_lambda_ini       : 17  ; /* [17..1]  */
        unsigned int    rc_mode               : 1   ; /* [18]  */
        unsigned int    reserved_0            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PPFD_ST_CFG;

/* Define the union U_VEDU_VCPI_QPCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cr_qp_offset     : 5   ; /* [4..0]  */
        unsigned int    vcpi_cb_qp_offset     : 5   ; /* [9..5]  */
        unsigned int    vcpi_frm_qp           : 6   ; /* [15..10]  */
        unsigned int    reserved_0            : 2   ; /* [17..16]  */
        unsigned int    vcpi_intra_det_qp_en  : 1   ; /* [18]  */
        unsigned int    vcpi_rc_cu_madi_en    : 1   ; /* [19]  */
        unsigned int    reserved_1            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_QPCFG;

/* Define the union U_VEDU_QPG_MAX_MIN_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_min_qp            : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_max_qp            : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_cu_qp_delta_enable_flag : 1   ; /* [16]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_MAX_MIN_QP;

/* Define the union U_VEDU_QPG_SMART_REG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 2   ; /* [1..0]  */
        unsigned int    qpg_smart_get_cu32_qp_mode : 2   ; /* [3..2]  */
        unsigned int    qpg_smart_get_cu64_qp_mode : 2   ; /* [5..4]  */
        unsigned int    qpg_qp_detlta_size_cu64 : 1   ; /* [6]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_SMART_REG;

/* Define the union U_VEDU_QPG_ROW_TARGET_BITS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_delta          : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 2   ; /* [5..4]  */
        unsigned int    qpg_row_target_bits   : 25  ; /* [30..6]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_ROW_TARGET_BITS;

/* Define the union U_VEDU_QPG_AVERAGE_LCU_BITS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_ave_lcu_bits      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_AVERAGE_LCU_BITS;

/* Define the union U_VEDU_QPG_CU_QP_DELTA_THRESH_REG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh0 : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh1 : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh2 : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU_QP_DELTA_THRESH_REG0;

/* Define the union U_VEDU_QPG_CU_QP_DELTA_THRESH_REG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh4 : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh5 : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh6 : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh7 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU_QP_DELTA_THRESH_REG1;

/* Define the union U_VEDU_QPG_CU_QP_DELTA_THRESH_REG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh8 : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh9 : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh10 : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh11 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU_QP_DELTA_THRESH_REG2;

/* Define the union U_VEDU_QPG_CU_QP_DELTA_THRESH_REG3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu_qp_delta_thresh12 : 8   ; /* [7..0]  */
        unsigned int    qpg_cu_qp_delta_thresh13 : 8   ; /* [15..8]  */
        unsigned int    qpg_cu_qp_delta_thresh14 : 8   ; /* [23..16]  */
        unsigned int    qpg_cu_qp_delta_thresh15 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU_QP_DELTA_THRESH_REG3;

/* Define the union U_VEDU_QPG_DELTA_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_delta_level_0  : 2   ; /* [1..0]  */
        unsigned int    qpg_qp_delta_level_1  : 2   ; /* [3..2]  */
        unsigned int    qpg_qp_delta_level_2  : 2   ; /* [5..4]  */
        unsigned int    qpg_qp_delta_level_3  : 2   ; /* [7..6]  */
        unsigned int    qpg_qp_delta_level_4  : 2   ; /* [9..8]  */
        unsigned int    qpg_qp_delta_level_5  : 2   ; /* [11..10]  */
        unsigned int    qpg_qp_delta_level_6  : 2   ; /* [13..12]  */
        unsigned int    qpg_qp_delta_level_7  : 2   ; /* [15..14]  */
        unsigned int    qpg_qp_delta_level_8  : 2   ; /* [17..16]  */
        unsigned int    qpg_qp_delta_level_9  : 2   ; /* [19..18]  */
        unsigned int    qpg_qp_delta_level_10 : 2   ; /* [21..20]  */
        unsigned int    qpg_qp_delta_level_11 : 2   ; /* [23..22]  */
        unsigned int    qpg_qp_delta_level_12 : 2   ; /* [25..24]  */
        unsigned int    qpg_qp_delta_level_13 : 2   ; /* [27..26]  */
        unsigned int    qpg_qp_delta_level_14 : 2   ; /* [29..28]  */
        unsigned int    qpg_qp_delta_level_15 : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_DELTA_LEVEL;

/* Define the union U_VEDU_QPG_MADI_SWITCH_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_madi_switch_thr : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_MADI_SWITCH_THR;

/* Define the union U_VEDU_QPG_CURR_SAD_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_cu_sad_en     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vcpi_sad_switch_thr   : 5   ; /* [8..4]  */
        unsigned int    reserved_1            : 7   ; /* [15..9]  */
        unsigned int    vcpi_rc_cu_sad_mod    : 2   ; /* [17..16]  */
        unsigned int    vcpi_rc_cu_sad_offset : 8   ; /* [25..18]  */
        unsigned int    vcpi_rc_cu_sad_gain   : 4   ; /* [29..26]  */
        unsigned int    reserved_2            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_EN;

/* Define the union U_VEDU_QPG_CURR_SAD_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curr_sad_level_0 : 2   ; /* [1..0]  */
        unsigned int    vcpi_curr_sad_level_1 : 2   ; /* [3..2]  */
        unsigned int    vcpi_curr_sad_level_2 : 2   ; /* [5..4]  */
        unsigned int    vcpi_curr_sad_level_3 : 2   ; /* [7..6]  */
        unsigned int    vcpi_curr_sad_level_4 : 2   ; /* [9..8]  */
        unsigned int    vcpi_curr_sad_level_5 : 2   ; /* [11..10]  */
        unsigned int    vcpi_curr_sad_level_6 : 2   ; /* [13..12]  */
        unsigned int    vcpi_curr_sad_level_7 : 2   ; /* [15..14]  */
        unsigned int    vcpi_curr_sad_level_8 : 2   ; /* [17..16]  */
        unsigned int    vcpi_curr_sad_level_9 : 2   ; /* [19..18]  */
        unsigned int    vcpi_curr_sad_level_10 : 2   ; /* [21..20]  */
        unsigned int    vcpi_curr_sad_level_11 : 2   ; /* [23..22]  */
        unsigned int    vcpi_curr_sad_level_12 : 2   ; /* [25..24]  */
        unsigned int    vcpi_curr_sad_level_13 : 2   ; /* [27..26]  */
        unsigned int    vcpi_curr_sad_level_14 : 2   ; /* [29..28]  */
        unsigned int    vcpi_curr_sad_level_15 : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_LEVEL;

/* Define the union U_VEDU_QPG_CURR_SAD_THRESH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curr_sad_thresh_0 : 8   ; /* [7..0]  */
        unsigned int    vcpi_curr_sad_thresh_1 : 8   ; /* [15..8]  */
        unsigned int    vcpi_curr_sad_thresh_2 : 8   ; /* [23..16]  */
        unsigned int    vcpi_curr_sad_thresh_3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_THRESH0;

/* Define the union U_VEDU_QPG_CURR_SAD_THRESH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curr_sad_thresh_4 : 8   ; /* [7..0]  */
        unsigned int    vcpi_curr_sad_thresh_5 : 8   ; /* [15..8]  */
        unsigned int    vcpi_curr_sad_thresh_6 : 8   ; /* [23..16]  */
        unsigned int    vcpi_curr_sad_thresh_7 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_THRESH1;

/* Define the union U_VEDU_QPG_CURR_SAD_THRESH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curr_sad_thresh_8 : 8   ; /* [7..0]  */
        unsigned int    vcpi_curr_sad_thresh_9 : 8   ; /* [15..8]  */
        unsigned int    vcpi_curr_sad_thresh_10 : 8   ; /* [23..16]  */
        unsigned int    vcpi_curr_sad_thresh_11 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_THRESH2;

/* Define the union U_VEDU_QPG_CURR_SAD_THRESH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curr_sad_thresh_12 : 8   ; /* [7..0]  */
        unsigned int    vcpi_curr_sad_thresh_13 : 8   ; /* [15..8]  */
        unsigned int    vcpi_curr_sad_thresh_14 : 8   ; /* [23..16]  */
        unsigned int    vcpi_curr_sad_thresh_15 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CURR_SAD_THRESH3;

/* Define the union U_VEDU_LUMA_RC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_en       : 1   ; /* [0]  */
        unsigned int    vcpi_rc_luma_mode     : 2   ; /* [2..1]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    vcpi_rc_luma_switch_thr : 5   ; /* [8..4]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    vcpi_rc_luma_low_madi_thr : 8   ; /* [19..12]  */
        unsigned int    vcpi_rc_luma_high_madi_thr : 8   ; /* [27..20]  */
        unsigned int    reserved_2            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_RC;

/* Define the union U_VEDU_LUMA_LEVEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_level_0  : 2   ; /* [1..0]  */
        unsigned int    vcpi_rc_luma_level_1  : 2   ; /* [3..2]  */
        unsigned int    vcpi_rc_luma_level_2  : 2   ; /* [5..4]  */
        unsigned int    vcpi_rc_luma_level_3  : 2   ; /* [7..6]  */
        unsigned int    vcpi_rc_luma_level_4  : 2   ; /* [9..8]  */
        unsigned int    vcpi_rc_luma_level_5  : 2   ; /* [11..10]  */
        unsigned int    vcpi_rc_luma_level_6  : 2   ; /* [13..12]  */
        unsigned int    vcpi_rc_luma_level_7  : 2   ; /* [15..14]  */
        unsigned int    vcpi_rc_luma_level_8  : 2   ; /* [17..16]  */
        unsigned int    vcpi_rc_luma_level_9  : 2   ; /* [19..18]  */
        unsigned int    vcpi_rc_luma_level_10 : 2   ; /* [21..20]  */
        unsigned int    vcpi_rc_luma_level_11 : 2   ; /* [23..22]  */
        unsigned int    vcpi_rc_luma_level_12 : 2   ; /* [25..24]  */
        unsigned int    vcpi_rc_luma_level_13 : 2   ; /* [27..26]  */
        unsigned int    vcpi_rc_luma_level_14 : 2   ; /* [29..28]  */
        unsigned int    vcpi_rc_luma_level_15 : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_LEVEL;

/* Define the union U_VEDU_LUMA_THRESH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_thresh_0 : 8   ; /* [7..0]  */
        unsigned int    vcpi_rc_luma_thresh_1 : 8   ; /* [15..8]  */
        unsigned int    vcpi_rc_luma_thresh_2 : 8   ; /* [23..16]  */
        unsigned int    vcpi_rc_luma_thresh_3 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_THRESH0;

/* Define the union U_VEDU_LUMA_THRESH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_thresh_4 : 8   ; /* [7..0]  */
        unsigned int    vcpi_rc_luma_thresh_5 : 8   ; /* [15..8]  */
        unsigned int    vcpi_rc_luma_thresh_6 : 8   ; /* [23..16]  */
        unsigned int    vcpi_rc_luma_thresh_7 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_THRESH1;

/* Define the union U_VEDU_LUMA_THRESH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_thresh_8 : 8   ; /* [7..0]  */
        unsigned int    vcpi_rc_luma_thresh_9 : 8   ; /* [15..8]  */
        unsigned int    vcpi_rc_luma_thresh_10 : 8   ; /* [23..16]  */
        unsigned int    vcpi_rc_luma_thresh_11 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_THRESH2;

/* Define the union U_VEDU_LUMA_THRESH3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_rc_luma_thresh_12 : 8   ; /* [7..0]  */
        unsigned int    vcpi_rc_luma_thresh_13 : 8   ; /* [15..8]  */
        unsigned int    vcpi_rc_luma_thresh_14 : 8   ; /* [23..16]  */
        unsigned int    vcpi_rc_luma_thresh_15 : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_LUMA_THRESH3;

/* Define the union U_VEDU_VCPI_BG_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_info_st_en    : 1   ; /* [0]  */
        unsigned int    vcpi_bg_refresh_st_en : 1   ; /* [1]  */
        unsigned int    vcpi_bg_ld_en         : 1   ; /* [2]  */
        unsigned int    vcpi_bg_en            : 1   ; /* [3]  */
        unsigned int    vcpi_bg_stat_frame    : 1   ; /* [4]  */
        unsigned int    vcpi_bg_th_frame      : 1   ; /* [5]  */
        unsigned int    vcpi_bg_percentage_en : 1   ; /* [6]  */
        unsigned int    vcpi_bg_start_idc     : 1   ; /* [7]  */
        unsigned int    vcpi_bg_start_frame   : 1   ; /* [8]  */
        unsigned int    vcpi_bg_reset_diff_en : 1   ; /* [9]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_ENABLE;

/* Define the union U_VEDU_VCPI_BG_FLT_PARA0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_alpha_fix_0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    vcpi_bg_alpha_fix_1   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_FLT_PARA0;

/* Define the union U_VEDU_VCPI_BG_FLT_PARA1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_belta_fix_0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    vcpi_bg_belta_fix_1   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_FLT_PARA1;

/* Define the union U_VEDU_VCPI_BG_FLT_PARA2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_delta_fix_0   : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 5   ; /* [15..11]  */
        unsigned int    vcpi_bg_delta_fix_1   : 11  ; /* [26..16]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_FLT_PARA2;

/* Define the union U_VEDU_VCPI_BG_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_th_ave        : 10  ; /* [9..0]  */
        unsigned int    vcpi_bg_ave_update_th : 10  ; /* [19..10]  */
        unsigned int    vcpi_bg_stat_th       : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_THR0;

/* Define the union U_VEDU_VCPI_BG_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bg_dist_th       : 10  ; /* [9..0]  */
        unsigned int    vcpi_bg_frame_num     : 10  ; /* [19..10]  */
        unsigned int    vcpi_bg_min_diff      : 10  ; /* [29..20]  */
        unsigned int    reserved_0            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_THR1;

/* Define the union U_VEDU_VCPI_BG_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bgl_stride       : 16  ; /* [15..0]  */
        unsigned int    vcpi_bgc_stride       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_STRIDE;

/* Define the union U_VEDU_VCPI_BG_EXT_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bgl_ext_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_bgc_ext_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_BG_EXT_STRIDE;

/* Define the union U_VEDU_VCPI_OUTSTD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_r_outstanding0   : 7   ; /* [6..0]  */
        unsigned int    vcpi_w_outstanding    : 5   ; /* [11..7] */
        unsigned int    vcpi_r_outstanding1   : 7   ; /* [18..12]  */
        unsigned int    reserved_0            : 13  ; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_OUTSTD;

/* Define the union U_VEDU_VCTRL_LCU_BASELINE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_performance_baseline : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_LCU_BASELINE;

/* Define the union U_VEDU_VCPI_SPMEM_CTRL_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rashds_svt_rtsel      : 2   ; /* [1..0]  */
        unsigned int    rashds_lvt_rtsel      : 2   ; /* [3..2]  */
        unsigned int    rashds_svt_wtsel      : 2   ; /* [5..4]  */
        unsigned int    rashds_lvt_wtsel      : 2   ; /* [7..6]  */
        unsigned int    rashdm_svt_rtsel      : 2   ; /* [9..8]  */
        unsigned int    rashdm_lvt_rtsel      : 2   ; /* [11..10]  */
        unsigned int    rashdm_svt_wtsel      : 2   ; /* [13..12]  */
        unsigned int    rashdm_lvt_wtsel      : 2   ; /* [15..14]  */
        unsigned int    rasshcsu_svt_rtsel    : 2   ; /* [17..16]  */
        unsigned int    rasshcsu_lvt_rtsel    : 2   ; /* [19..18]  */
        unsigned int    rasshcsu_svt_wtsel    : 2   ; /* [21..20]  */
        unsigned int    rasshcsu_lvt_wtsel    : 2   ; /* [23..22]  */
        unsigned int    rasshcss_svt_rtsel    : 2   ; /* [25..24]  */
        unsigned int    rasshcss_lvt_rtsel    : 2   ; /* [27..26]  */
        unsigned int    rasshcss_svt_wtsel    : 2   ; /* [29..28]  */
        unsigned int    rasshcss_lvt_wtsel    : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_SPMEM_CTRL_1;

/* Define the union U_VEDU_VCPI_SPMEM_CTRL_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rasshcms_svt_rtsel    : 2   ; /* [1..0]  */
        unsigned int    rasshcms_lvt_rtsel    : 2   ; /* [3..2]  */
        unsigned int    rasshcms_svt_wtsel    : 2   ; /* [5..4]  */
        unsigned int    rasshcms_lvt_wtsel    : 2   ; /* [7..6]  */
        unsigned int    rasshcmu_svt_rtsel    : 2   ; /* [9..8]  */
        unsigned int    rasshcmu_lvt_rtsel    : 2   ; /* [11..10]  */
        unsigned int    rasshcmu_svt_wtsel    : 2   ; /* [13..12]  */
        unsigned int    rasshcmu_lvt_wtsel    : 2   ; /* [15..14]  */
        unsigned int    ras_svt_rtsel         : 2   ; /* [17..16]  */
        unsigned int    ras_lvt_rtsel         : 2   ; /* [19..18]  */
        unsigned int    ras_svt_wtsel         : 2   ; /* [21..20]  */
        unsigned int    ras_lvt_wtsel         : 2   ; /* [23..22]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_SPMEM_CTRL_2;

typedef union
{
    struct
    {
        unsigned int    layer_en              : 4   ; /* [3..0] */
        unsigned int    reserved_0            : 4   ; /* [7..4] */
        unsigned int    flux_req_en_l         : 1   ; /* [8] */
        unsigned int    flux_req_en_h         : 1   ; /* [9] */
        unsigned int    reserved_1            : 22  ; /* [31..10] */
    } bits;
    unsigned int    u32;
} U_VEDU_VCTRL_GCFG;
typedef union
{
    struct
    {
        unsigned int    flux_req_gran_l       : 8   ; /* [7..0] */
        unsigned int    flux_req_gran_h       : 8   ; /* [15..8] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;
    unsigned int    u32;
} U_VEDU_VCTRL_GRAN;
/* Define the union U_VEDU_VCTRL_THRESHOLD_L */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flux_req_thres_l      : 16  ; /* [15..0] */
        unsigned int    flux_req_recover_l    : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCTRL_THRESHOLD_L;

/* Define the union U_VEDU_VCTRL_THRESHOLD_H */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flux_req_thres_h      : 16  ; /* [15..0] */
        unsigned int    flux_req_recover_h    : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCTRL_THRESHOLD_H;

/* Define the union U_VEDU_VCTRL_INTRAPU4_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    flux_req_thres_intrapu4_offset : 16  ; /* [15..0] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCTRL_INTRAPU4_OFFSET;

/* Define the union U_VEDU_CURLD_DEPTHCODING */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_depthcoding_u_pixel : 10  ; /* [9..0] */
        unsigned int    curld_depthcoding_v_pixel : 10  ; /* [19..10] */
        unsigned int    reserved_0            : 12  ; /* [31..20] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_DEPTHCODING;

/* Define the union U_VEDU_CURLD_WATERMARK_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_watermark_startx : 16  ; /* [15..0] */
        unsigned int    curld_watermark_starty : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_WATERMARK_CFG;

/* Define the union U_VEDU_CURLD_WATERMARK_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_watermark_width : 16  ; /* [15..0] */
        unsigned int    curld_watermark_height : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_WATERMARK_SIZE;
/* Define the union U_VEDU_CURLD_GCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    wide_ynarrow_en       : 1   ; /* [0]  */
        unsigned int    wide_cnarrow_en       : 1   ; /* [1]  */
        unsigned int    rgb_clip_en           : 1   ; /* [2]  */
        unsigned int    mlsb_sel              : 1   ; /* [3]  */
        unsigned int    curld_watermark_en    : 1   ; /* [4] */
        unsigned int    curld_depthcoding_en  : 1   ; /* [5] */
        unsigned int    reserved_0            : 1   ; /* [4]  */
        unsigned int    reserved_1            : 1   ; /* [5]  */
        unsigned int    reserved_2            : 6   ; /* [13..8] */
        unsigned int    curld_col2gray_en     : 1   ; /* [14]  */
        unsigned int    curld_clip_en         : 1   ; /* [15]  */
        unsigned int    reserved_3            : 4   ; /* [19..16] */
        unsigned int    curld_read_interval   : 8   ; /* [27..20]  */
        unsigned int    curld_read_interval_mode : 1   ; /* [28] */
        unsigned int    reserved_4            : 1   ; /* [29] */
        unsigned int    reserved_5            : 2   ; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_GCFG;

/* Define the union U_VEDU_VCPI_OSD_ENABLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_osd0_en          : 1   ; /* [0]  */
        unsigned int    vcpi_osd1_en          : 1   ; /* [1]  */
        unsigned int    vcpi_osd2_en          : 1   ; /* [2]  */
        unsigned int    vcpi_osd3_en          : 1   ; /* [3]  */
        unsigned int    vcpi_osd4_en          : 1   ; /* [4]  */
        unsigned int    vcpi_osd5_en          : 1   ; /* [5]  */
        unsigned int    vcpi_osd6_en          : 1   ; /* [6]  */
        unsigned int    vcpi_osd7_en          : 1   ; /* [7]  */
        unsigned int    vcpi_osd0_absqp       : 1   ; /* [8]  */
        unsigned int    vcpi_osd1_absqp       : 1   ; /* [9]  */
        unsigned int    vcpi_osd2_absqp       : 1   ; /* [10]  */
        unsigned int    vcpi_osd3_absqp       : 1   ; /* [11]  */
        unsigned int    vcpi_osd4_absqp       : 1   ; /* [12]  */
        unsigned int    vcpi_osd5_absqp       : 1   ; /* [13]  */
        unsigned int    vcpi_osd6_absqp       : 1   ; /* [14]  */
        unsigned int    vcpi_osd7_absqp       : 1   ; /* [15]  */
        unsigned int    vcpi_roi_osd_sel_0    : 1   ; /* [16]  */
        unsigned int    vcpi_roi_osd_sel_1    : 1   ; /* [17]  */
        unsigned int    vcpi_roi_osd_sel_2    : 1   ; /* [18]  */
        unsigned int    vcpi_roi_osd_sel_3    : 1   ; /* [19]  */
        unsigned int    vcpi_roi_osd_sel_4    : 1   ; /* [20]  */
        unsigned int    vcpi_roi_osd_sel_5    : 1   ; /* [21]  */
        unsigned int    vcpi_roi_osd_sel_6    : 1   ; /* [22]  */
        unsigned int    vcpi_roi_osd_sel_7    : 1   ; /* [23]  */
        unsigned int    vcpi_osd_en           : 1   ; /* [24]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_OSD_ENABLE;

/* Define the union U_VEDU_VCPI_STRFMT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_crop_en          : 1   ; /* [0]  */
        unsigned int    vcpi_scale_en         : 1   ; /* [1]  */
        unsigned int    vcpi_store_mode       : 1   ; /* [2]  */
        unsigned int    vcpi_blk_type         : 3   ; /* [5..3]  */
        unsigned int    vcpi_str_fmt          : 4   ; /* [9..6]  */
        unsigned int    vcpi_package_sel      : 4   ; /* [13..10]  */
        unsigned int    vcpi_recst_disable    : 1   ; /* [14]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_STRFMT;

/* Define the union U_VEDU_VCPI_INTRA_INTER_CU_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra_cu_en      : 4   ; /* [3..0]  */
        unsigned int    vcpi_ipcm_en          : 1   ; /* [4]  */
        unsigned int    vcpi_intra_h264_cutdiag : 1   ; /* [5]  */
        unsigned int    vcpi_cu8_rdo_distortion_sel : 1   ; /* [6] */
        unsigned int    vcpi_subjective_rdo_en : 1   ; /* [7] */
        unsigned int    vcpi_fme_cu_en        : 4   ; /* [11..8]  */
        unsigned int    vcpi_mrg_cu_en        : 4   ; /* [15..12]  */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTRA_INTER_CU_EN;

/* Define the union U_VEDU_VCPI_CROSS_TILE_SLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cross_slice      : 1   ; /* [0]  */
        unsigned int    vcpi_cross_tile       : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CROSS_TILE_SLC;

/* Define the union U_VEDU_VCPI_MULTISLC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_slice_size       : 16  ; /* [15..0]  */
        unsigned int    vcpi_slcspilt_mod     : 1   ; /* [16]  */
        unsigned int    vcpi_multislc_en      : 1   ; /* [17]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_MULTISLC;

/* Define the union U_VEDU_VCTRL_LCU_TARGET_BIT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_lcu_target_bit  : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_LCU_TARGET_BIT;

/* Define the union U_VEDU_VCPI_SW_L0_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_sw_l0_width      : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vcpi_sw_l0_height     : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_SW_L0_SIZE;

/* Define the union U_VEDU_VCPI_SW_L1_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_sw_l1_width      : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 9   ; /* [15..7]  */
        unsigned int    vcpi_sw_l1_height     : 6   ; /* [21..16]  */
        unsigned int    reserved_1            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_SW_L1_SIZE;

/* Define the union U_VEDU_VCPI_I_SLC_INSERT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_insert_i_slc_idx : 8   ; /* [7..0]  */
        unsigned int    vcpi_insert_i_slc_en  : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_I_SLC_INSERT;

/* Define the union U_VEDU_PME_SAFE_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_safe_line         : 16  ; /* [15..0]  */
        unsigned int    pme_safe_line_val     : 1   ; /* [16]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SAFE_CFG;

/* Define the union U_VEDU_PME_IBLK_REFRESH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_refresh_start_num : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_IBLK_REFRESH;

/* Define the union U_VEDU_PME_IBLK_REFRESH_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_refresh_num  : 18  ; /* [17..0]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_IBLK_REFRESH_NUM;

/* Define the union U_VEDU_INTRA_CHNL4_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_mode0_en    : 1   ; /* [0]  */
        unsigned int    intra_cu4_mode1_en    : 1   ; /* [1]  */
        unsigned int    intra_cu4_mode2_en    : 1   ; /* [2]  */
        unsigned int    intra_cu4_mode3_en    : 1   ; /* [3]  */
        unsigned int    intra_cu4_mode4_en    : 1   ; /* [4]  */
        unsigned int    intra_cu4_mode5_en    : 1   ; /* [5]  */
        unsigned int    intra_cu4_mode6_en    : 1   ; /* [6]  */
        unsigned int    intra_cu4_mode7_en    : 1   ; /* [7]  */
        unsigned int    intra_cu4_mode8_en    : 1   ; /* [8]  */
        unsigned int    intra_cu4_mode9_en    : 1   ; /* [9]  */
        unsigned int    intra_cu4_mode10_en   : 1   ; /* [10]  */
        unsigned int    intra_cu4_mode11_en   : 1   ; /* [11]  */
        unsigned int    intra_cu4_mode12_en   : 1   ; /* [12]  */
        unsigned int    intra_cu4_mode13_en   : 1   ; /* [13]  */
        unsigned int    intra_cu4_mode14_en   : 1   ; /* [14]  */
        unsigned int    intra_cu4_mode15_en   : 1   ; /* [15]  */
        unsigned int    intra_cu4_mode16_en   : 1   ; /* [16]  */
        unsigned int    intra_cu4_mode17_en   : 1   ; /* [17]  */
        unsigned int    intra_cu4_mode18_en   : 1   ; /* [18]  */
        unsigned int    intra_cu4_mode19_en   : 1   ; /* [19]  */
        unsigned int    intra_cu4_mode20_en   : 1   ; /* [20]  */
        unsigned int    intra_cu4_mode21_en   : 1   ; /* [21]  */
        unsigned int    intra_cu4_mode22_en   : 1   ; /* [22]  */
        unsigned int    intra_cu4_mode23_en   : 1   ; /* [23]  */
        unsigned int    intra_cu4_mode24_en   : 1   ; /* [24]  */
        unsigned int    intra_cu4_mode25_en   : 1   ; /* [25]  */
        unsigned int    intra_cu4_mode26_en   : 1   ; /* [26]  */
        unsigned int    intra_cu4_mode27_en   : 1   ; /* [27]  */
        unsigned int    intra_cu4_mode28_en   : 1   ; /* [28]  */
        unsigned int    intra_cu4_mode29_en   : 1   ; /* [29]  */
        unsigned int    intra_cu4_mode30_en   : 1   ; /* [30]  */
        unsigned int    intra_cu4_mode31_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL4_ANG_0EN;

/* Define the union U_VEDU_INTRA_CHNL4_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_mode32_en   : 1   ; /* [0]  */
        unsigned int    intra_cu4_mode33_en   : 1   ; /* [1]  */
        unsigned int    intra_cu4_mode34_en   : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL4_ANG_1EN;

/* Define the union U_VEDU_INTRA_CHNL8_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu8_mode0_en    : 1   ; /* [0]  */
        unsigned int    intra_cu8_mode1_en    : 1   ; /* [1]  */
        unsigned int    intra_cu8_mode2_en    : 1   ; /* [2]  */
        unsigned int    intra_cu8_mode3_en    : 1   ; /* [3]  */
        unsigned int    intra_cu8_mode4_en    : 1   ; /* [4]  */
        unsigned int    intra_cu8_mode5_en    : 1   ; /* [5]  */
        unsigned int    intra_cu8_mode6_en    : 1   ; /* [6]  */
        unsigned int    intra_cu8_mode7_en    : 1   ; /* [7]  */
        unsigned int    intra_cu8_mode8_en    : 1   ; /* [8]  */
        unsigned int    intra_cu8_mode9_en    : 1   ; /* [9]  */
        unsigned int    intra_cu8_mode10_en   : 1   ; /* [10]  */
        unsigned int    intra_cu8_mode11_en   : 1   ; /* [11]  */
        unsigned int    intra_cu8_mode12_en   : 1   ; /* [12]  */
        unsigned int    intra_cu8_mode13_en   : 1   ; /* [13]  */
        unsigned int    intra_cu8_mode14_en   : 1   ; /* [14]  */
        unsigned int    intra_cu8_mode15_en   : 1   ; /* [15]  */
        unsigned int    intra_cu8_mode16_en   : 1   ; /* [16]  */
        unsigned int    intra_cu8_mode17_en   : 1   ; /* [17]  */
        unsigned int    intra_cu8_mode18_en   : 1   ; /* [18]  */
        unsigned int    intra_cu8_mode19_en   : 1   ; /* [19]  */
        unsigned int    intra_cu8_mode20_en   : 1   ; /* [20]  */
        unsigned int    intra_cu8_mode21_en   : 1   ; /* [21]  */
        unsigned int    intra_cu8_mode22_en   : 1   ; /* [22]  */
        unsigned int    intra_cu8_mode23_en   : 1   ; /* [23]  */
        unsigned int    intra_cu8_mode24_en   : 1   ; /* [24]  */
        unsigned int    intra_cu8_mode25_en   : 1   ; /* [25]  */
        unsigned int    intra_cu8_mode26_en   : 1   ; /* [26]  */
        unsigned int    intra_cu8_mode27_en   : 1   ; /* [27]  */
        unsigned int    intra_cu8_mode28_en   : 1   ; /* [28]  */
        unsigned int    intra_cu8_mode29_en   : 1   ; /* [29]  */
        unsigned int    intra_cu8_mode30_en   : 1   ; /* [30]  */
        unsigned int    intra_cu8_mode31_en   : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL8_ANG_0EN;

/* Define the union U_VEDU_INTRA_CHNL8_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu8_mode32_en   : 1   ; /* [0]  */
        unsigned int    intra_cu8_mode33_en   : 1   ; /* [1]  */
        unsigned int    intra_cu8_mode34_en   : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL8_ANG_1EN;

/* Define the union U_VEDU_INTRA_CHNL16_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_mode0_en   : 1   ; /* [0]  */
        unsigned int    intra_cu16_mode1_en   : 1   ; /* [1]  */
        unsigned int    intra_cu16_mode2_en   : 1   ; /* [2]  */
        unsigned int    intra_cu16_mode3_en   : 1   ; /* [3]  */
        unsigned int    intra_cu16_mode4_en   : 1   ; /* [4]  */
        unsigned int    intra_cu16_mode5_en   : 1   ; /* [5]  */
        unsigned int    intra_cu16_mode6_en   : 1   ; /* [6]  */
        unsigned int    intra_cu16_mode7_en   : 1   ; /* [7]  */
        unsigned int    intra_cu16_mode8_en   : 1   ; /* [8]  */
        unsigned int    intra_cu16_mode9_en   : 1   ; /* [9]  */
        unsigned int    intra_cu16_mode10_en  : 1   ; /* [10]  */
        unsigned int    intra_cu16_mode11_en  : 1   ; /* [11]  */
        unsigned int    intra_cu16_mode12_en  : 1   ; /* [12]  */
        unsigned int    intra_cu16_mode13_en  : 1   ; /* [13]  */
        unsigned int    intra_cu16_mode14_en  : 1   ; /* [14]  */
        unsigned int    intra_cu16_mode15_en  : 1   ; /* [15]  */
        unsigned int    intra_cu16_mode16_en  : 1   ; /* [16]  */
        unsigned int    intra_cu16_mode17_en  : 1   ; /* [17]  */
        unsigned int    intra_cu16_mode18_en  : 1   ; /* [18]  */
        unsigned int    intra_cu16_mode19_en  : 1   ; /* [19]  */
        unsigned int    intra_cu16_mode20_en  : 1   ; /* [20]  */
        unsigned int    intra_cu16_mode21_en  : 1   ; /* [21]  */
        unsigned int    intra_cu16_mode22_en  : 1   ; /* [22]  */
        unsigned int    intra_cu16_mode23_en  : 1   ; /* [23]  */
        unsigned int    intra_cu16_mode24_en  : 1   ; /* [24]  */
        unsigned int    intra_cu16_mode25_en  : 1   ; /* [25]  */
        unsigned int    intra_cu16_mode26_en  : 1   ; /* [26]  */
        unsigned int    intra_cu16_mode27_en  : 1   ; /* [27]  */
        unsigned int    intra_cu16_mode28_en  : 1   ; /* [28]  */
        unsigned int    intra_cu16_mode29_en  : 1   ; /* [29]  */
        unsigned int    intra_cu16_mode30_en  : 1   ; /* [30]  */
        unsigned int    intra_cu16_mode31_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL16_ANG_0EN;

/* Define the union U_VEDU_INTRA_CHNL16_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_mode32_en  : 1   ; /* [0]  */
        unsigned int    intra_cu16_mode33_en  : 1   ; /* [1]  */
        unsigned int    intra_cu16_mode34_en  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL16_ANG_1EN;

/* Define the union U_VEDU_INTRA_CHNL32_ANG_0EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu32_mode0_en   : 1   ; /* [0]  */
        unsigned int    intra_cu32_mode1_en   : 1   ; /* [1]  */
        unsigned int    intra_cu32_mode2_en   : 1   ; /* [2]  */
        unsigned int    intra_cu32_mode3_en   : 1   ; /* [3]  */
        unsigned int    intra_cu32_mode4_en   : 1   ; /* [4]  */
        unsigned int    intra_cu32_mode5_en   : 1   ; /* [5]  */
        unsigned int    intra_cu32_mode6_en   : 1   ; /* [6]  */
        unsigned int    intra_cu32_mode7_en   : 1   ; /* [7]  */
        unsigned int    intra_cu32_mode8_en   : 1   ; /* [8]  */
        unsigned int    intra_cu32_mode9_en   : 1   ; /* [9]  */
        unsigned int    intra_cu32_mode10_en  : 1   ; /* [10]  */
        unsigned int    intra_cu32_mode11_en  : 1   ; /* [11]  */
        unsigned int    intra_cu32_mode12_en  : 1   ; /* [12]  */
        unsigned int    intra_cu32_mode13_en  : 1   ; /* [13]  */
        unsigned int    intra_cu32_mode14_en  : 1   ; /* [14]  */
        unsigned int    intra_cu32_mode15_en  : 1   ; /* [15]  */
        unsigned int    intra_cu32_mode16_en  : 1   ; /* [16]  */
        unsigned int    intra_cu32_mode17_en  : 1   ; /* [17]  */
        unsigned int    intra_cu32_mode18_en  : 1   ; /* [18]  */
        unsigned int    intra_cu32_mode19_en  : 1   ; /* [19]  */
        unsigned int    intra_cu32_mode20_en  : 1   ; /* [20]  */
        unsigned int    intra_cu32_mode21_en  : 1   ; /* [21]  */
        unsigned int    intra_cu32_mode22_en  : 1   ; /* [22]  */
        unsigned int    intra_cu32_mode23_en  : 1   ; /* [23]  */
        unsigned int    intra_cu32_mode24_en  : 1   ; /* [24]  */
        unsigned int    intra_cu32_mode25_en  : 1   ; /* [25]  */
        unsigned int    intra_cu32_mode26_en  : 1   ; /* [26]  */
        unsigned int    intra_cu32_mode27_en  : 1   ; /* [27]  */
        unsigned int    intra_cu32_mode28_en  : 1   ; /* [28]  */
        unsigned int    intra_cu32_mode29_en  : 1   ; /* [29]  */
        unsigned int    intra_cu32_mode30_en  : 1   ; /* [30]  */
        unsigned int    intra_cu32_mode31_en  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL32_ANG_0EN;

/* Define the union U_VEDU_INTRA_CHNL32_ANG_1EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu32_mode32_en  : 1   ; /* [0]  */
        unsigned int    intra_cu32_mode33_en  : 1   ; /* [1]  */
        unsigned int    intra_cu32_mode34_en  : 1   ; /* [2]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CHNL32_ANG_1EN;

/* Define the union U_VEDU_PACK_CU_PARAMETER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pack_vcpi2cu_tq_bypass_enabled_flag : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    pack_vcpi2cu_qp_min_cu_size : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PACK_CU_PARAMETER;

/* Define the union U_VEDU_PACK_PCM_PARAMETER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pack_vcpi2pu_log2_min_ipcm_cbsizey : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    pack_vcpi2pu_log2_max_ipcm_cbsizey : 3   ; /* [6..4]  */
        unsigned int    reserved_1            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PACK_PCM_PARAMETER;

/* Define the union U_VEDU_QPG_READLINE_INTERVAL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tile_qpg_readline_interval : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_READLINE_INTERVAL;

/* Define the union U_VEDU_PMV_READLINE_INTERVAL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pmv_readline_interval : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PMV_READLINE_INTERVAL;

/* Define the union U_VEDU_RGB2YUV_COEF_P0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_y_0coef       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_y_1coef       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_COEF_P0;

/* Define the union U_VEDU_RGB2YUV_COEF_P1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_y_2coef       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_u_0coef       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_COEF_P1;

/* Define the union U_VEDU_RGB2YUV_COEF_P2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_u_1coef       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_u_2coef       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_COEF_P2;

/* Define the union U_VEDU_RGB2YUV_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb_y_rnd             : 16  ; /* [15..0]  */
        unsigned int    rgb_u_rnd             : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_OFFSET;

/* Define the union U_VEDU_RGB2YUV_CLIP_THR_Y */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_min_y            : 10  ; /* [9..0] */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    clip_max_y            : 10  ; /* [25..16] */
        unsigned int    reserved_1            : 6   ; /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_CLIP_THR_Y;

/* Define the union U_VEDU_RGB2YUV_CLIP_THR_C */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clip_min_c            : 10  ; /* [9..0] */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    clip_max_c            : 10  ; /* [25..16] */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_CLIP_THR_C;

/* Define the union U_VEDU_RGB2YUV_SHIFT_WIDTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_shift_width   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_SHIFT_WIDTH;

/* Define the union U_VEDU_VCPI_OSD_POS_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_v_0coef       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb2yuv_v_1coef       : 15  ; /* [30..16]  */
        unsigned int    reserved_1            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_COEF_P3;

/* Define the union U_VEDU_VCPI_OSD_POS_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb2yuv_v_2coef       : 15  ; /* [14..0]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    rgb_v_rnd             : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_RGB2YUV_COEF_P4;

/* Define the union U_VEDU_VCPI_OSD_POS_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_narrow_chrm_min : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    curld_narrow_chrm_max : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_NARROW_CHRM;

/* Define the union U_VEDU_VCPI_OSD_POS_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_narrow_luma_min : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    curld_narrow_luma_max : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_NARROW_LUMA;

/* Define the union U_VEDU_CURLD_WATERMARK_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0] */
        unsigned int    vcpi_curld_watermark_stride : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_WATERMARK_STRIDE;

/* Define the union U_VEDU_VCPI_OSD_QP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_osd0_qp          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    vcpi_osd1_qp          : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    vcpi_osd2_qp          : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    vcpi_osd3_qp          : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_OSD_QP0;

/* Define the union U_VEDU_VCPI_OSD_QP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_osd4_qp          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    vcpi_osd5_qp          : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    vcpi_osd6_qp          : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    vcpi_osd7_qp          : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_OSD_QP1;

/* Define the union U_VEDU_CURLD_OSD01_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    rgb_clip_min          : 10  ; /* [9..0]  */
        unsigned int    rgb_clip_max          : 10  ; /* [19..10]  */
        unsigned int    curld_cmp_clk_gt_en   : 1   ; /* [20] */
        unsigned int    curld_cmp_tag_en      : 1   ; /* [21] */
        unsigned int    curld_cmp_raw_en      : 1   ; /* [22] */
        unsigned int    flbc_lut_qp_delta     : 2   ; /* [24..23] */
        unsigned int    flbc_chroma_qp_offset_strength : 2   ; /* [26..25] */
        unsigned int    reserved_0            : 5   ; /* [31..27] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_CURLD_RGB_CLIP;

/* Define the union U_VEDU_CURLD_OSD23_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_filter_hor_0coef : 4   ; /* [3..0]  */
        unsigned int    vcpi_filter_hor_1coef : 4   ; /* [7..4]  */
        unsigned int    vcpi_filter_hor_2coef : 4   ; /* [11..8]  */
        unsigned int    vcpi_filter_hor_3coef : 4   ; /* [15..12]  */
        unsigned int    vcpi_filter_hor_rnd   : 4   ; /* [19..16]  */
        unsigned int    vcpi_filter_hor_shift : 3   ; /* [22..20]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_OSD23_ALPHA;

/* Define the union U_VEDU_CURLD_OSD45_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_filter_ver_0coef : 4   ; /* [3..0]  */
        unsigned int    vcpi_filter_ver_1coef : 4   ; /* [7..4]  */
        unsigned int    vcpi_filter_ver_2coef : 4   ; /* [11..8]  */
        unsigned int    vcpi_filter_ver_3coef : 4   ; /* [15..12]  */
        unsigned int    vcpi_filter_ver_rnd   : 4   ; /* [19..16]  */
        unsigned int    vcpi_filter_ver_shift : 3   ; /* [22..20]  */
        unsigned int    reserved_0            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_OSD45_ALPHA;

/* Define the union U_VEDU_CURLD_OSD67_ALPHA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    srcyh_stride          : 16  ; /* [15..0]  */
        unsigned int    srcch_stride          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_SRCH_STRIDE;

/* Define the union U_VEDU_TBLDST_READLINE_INTERVAL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tbldst_readline_interval : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_TBLDST_READLINE_INTERVAL;

/* Define the union U_VEDU_NEW_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    irq_position_cnt_y    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 7   ; /* [16..10]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_NEW_CFG0;

/* Define the union U_VEDU_VCTRL_ROI_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_region0en       : 1   ; /* [0]  */
        unsigned int    vctrl_region1en       : 1   ; /* [1]  */
        unsigned int    vctrl_region2en       : 1   ; /* [2]  */
        unsigned int    vctrl_region3en       : 1   ; /* [3]  */
        unsigned int    vctrl_region4en       : 1   ; /* [4]  */
        unsigned int    vctrl_region5en       : 1   ; /* [5]  */
        unsigned int    vctrl_region6en       : 1   ; /* [6]  */
        unsigned int    vctrl_region7en       : 1   ; /* [7]  */
        unsigned int    vctrl_absqp0          : 1   ; /* [8]  */
        unsigned int    vctrl_absqp1          : 1   ; /* [9]  */
        unsigned int    vctrl_absqp2          : 1   ; /* [10]  */
        unsigned int    vctrl_absqp3          : 1   ; /* [11]  */
        unsigned int    vctrl_absqp4          : 1   ; /* [12]  */
        unsigned int    vctrl_absqp5          : 1   ; /* [13]  */
        unsigned int    vctrl_absqp6          : 1   ; /* [14]  */
        unsigned int    vctrl_absqp7          : 1   ; /* [15]  */
        unsigned int    vctrl_region0keep     : 1   ; /* [16]  */
        unsigned int    vctrl_region1keep     : 1   ; /* [17]  */
        unsigned int    vctrl_region2keep     : 1   ; /* [18]  */
        unsigned int    vctrl_region3keep     : 1   ; /* [19]  */
        unsigned int    vctrl_region4keep     : 1   ; /* [20]  */
        unsigned int    vctrl_region5keep     : 1   ; /* [21]  */
        unsigned int    vctrl_region6keep     : 1   ; /* [22]  */
        unsigned int    vctrl_region7keep     : 1   ; /* [23]  */
        unsigned int    vctrl_roi_en          : 1   ; /* [24]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_CFG0;

/* Define the union U_VEDU_VCTRL_ROI_CFG1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roiqp0          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    vctrl_roiqp1          : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    vctrl_roiqp2          : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    vctrl_roiqp3          : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_CFG1;

/* Define the union U_VEDU_VCTRL_ROI_CFG2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_roiqp4          : 7   ; /* [6..0]  */
        unsigned int    reserved_0            : 1   ; /* [7]  */
        unsigned int    vctrl_roiqp5          : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    vctrl_roiqp6          : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 1   ; /* [23]  */
        unsigned int    vctrl_roiqp7          : 7   ; /* [30..24]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_CFG2;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size0_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size0_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_0;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size1_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size1_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_1;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size2_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size2_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_2;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size3_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size3_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_3;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size4_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size4_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_4;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size5_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size5_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_5;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size6_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size6_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_6;

/* Define the union U_VEDU_VCTRL_ROI_SIZE_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_size7_roiwidth  : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vctrl_size7_roiheight : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_SIZE_7;

/* Define the union U_VEDU_VCTRL_ROI_START_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start0_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start0_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_0;

/* Define the union U_VEDU_VCTRL_ROI_START_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start1_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start1_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_1;

/* Define the union U_VEDU_VCTRL_ROI_START_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start2_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start2_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_2;

/* Define the union U_VEDU_VCTRL_ROI_START_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start3_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start3_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_3;

/* Define the union U_VEDU_VCTRL_ROI_START_4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start4_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start4_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_4;

/* Define the union U_VEDU_VCTRL_ROI_START_5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start5_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start5_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_5;

/* Define the union U_VEDU_VCTRL_ROI_START_6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start6_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start6_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_6;

/* Define the union U_VEDU_VCTRL_ROI_START_7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_start7_roistartx : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vctrl_start7_roistarty : 12  ; /* [27..16]  */
        unsigned int    reserved_1            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ROI_START_7;

/* Define the union U_VEDU_TILE_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vedu_curr_cell_mode   : 1   ; /* [0]  */
        unsigned int    vedu_left_cell_mode   : 1   ; /* [1]  */
        unsigned int    vedu_top_cell_mode    : 1   ; /* [2]  */
        unsigned int    vedu_topleft_cell_mode : 1   ; /* [3]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_TILE_MODE;

/* Define the union U_VEDU_VCPI_PICSIZE_PIX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_imgwidth_pix     : 16  ; /* [15..0]  */
        unsigned int    vcpi_imgheight_pix    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PICSIZE_PIX;

/* Define the union U_VEDU_TILE_POS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vedu_tile_start_x     : 16  ; /* [15..0]  */
        unsigned int    vedu_tile_start_y     : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_TILE_POS;

/* Define the union U_VEDU_VCPI_TILE_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tile_width       : 16  ; /* [15..0]  */
        unsigned int    vcpi_tile_height      : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_TILE_SIZE;

/* Define the union U_VEDU_VCPI_VLC_CONFIG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ref_idc          : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 2   ; /* [3..2]  */
        unsigned int    vcpi_cabac_init_idc   : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 6   ; /* [11..6]  */
        unsigned int    vcpi_byte_stuffing    : 1   ; /* [12]  */
        unsigned int    reserved_2            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_VLC_CONFIG;

/* Define the union U_VEDU_VCPI_REF_FLAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    vcpi_col_from_l0_flag : 1   ; /* [1]  */
        unsigned int    vcpi_curr_ref_long_flag : 2   ; /* [3..2]  */
        unsigned int    vcpi_col_long_flag    : 2   ; /* [5..4]  */
        unsigned int    vcpi_predflag_sel     : 2   ; /* [7..6]  */
        unsigned int    reserved_1            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_FLAG;

/* Define the union U_VEDU_PMV_TMV_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pmv_tmv_en            : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PMV_TMV_EN;

/* Define the union U_VEDU_VCPI_TMV_LOAD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tmv_wr_rd_avail  : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_TMV_LOAD;

/* Define the union U_VEDU_CABAC_GLB_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 8   ; /* [7..0]  */
        unsigned int    cabac_max_num_mergecand : 3   ; /* [10..8]  */
        unsigned int    reserved_1            : 5   ; /* [15..11]  */
        unsigned int    cabac_nal_unit_head   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CABAC_GLB_CFG;

/* Define the union U_VEDU_CABAC_SLCHDR_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_size_part1 : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    cabac_slchdr_size_part2 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CABAC_SLCHDR_SIZE;

/* Define the union U_VEDU_CABAC_SLCHDR_PART1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_part1    : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CABAC_SLCHDR_PART1;

/* Define the union U_VEDU_CABAC_SLCHDR_SIZE_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_size_part1_i : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 11  ; /* [15..5]  */
        unsigned int    cabac_slchdr_size_part2_i : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CABAC_SLCHDR_SIZE_I;

/* Define the union U_VEDU_CABAC_SLCHDR_PART1_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cabac_slchdr_part1_i  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CABAC_SLCHDR_PART1_I;

/* Define the union U_VEDU_VLC_SLCHDRPARA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_markingbit        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    vlc_reorderbit        : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    vlc_parabit           : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VLC_SLCHDRPARA;

/* Define the union U_VEDU_VLC_SVC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_svc_en            : 1   ; /* [0]  */
        unsigned int    vlc_svc_strm          : 24  ; /* [24..1]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VLC_SVC;

/* Define the union U_VEDU_VLC_SLCHDRPARA_I */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlc_markingbit_i      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    vlc_reorderbit_i      : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    vlc_parabit_i         : 7   ; /* [22..16]  */
        unsigned int    reserved_2            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VLC_SLCHDRPARA_I;

/* Define the union U_VEDU_ICE_CMC_MODE_CFG0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcfg_hebc_clkgate_en  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_REFLD_CMP_CLKGATE_EN;

/* Define the union U_VEDU_QPG_LOWLUMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lowluma_min_qp    : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_lowluma_max_qp    : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_lowluma_qp_delta  : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_LOWLUMA;

/* Define the union U_VEDU_QPG_HEDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_hedge_mim_qp      : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_hedge_max_qp      : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_hedge_qp_delta    : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_HEDGE;

/* Define the union U_VEDU_QPG_HEDGE_MOVE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_hedge_move_min_qp : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_hedge_move_max_qp : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_hedge_move_qp_delta : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_HEDGE_MOVE;

/* Define the union U_VEDU_QPG_SKIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_skin_min_qp       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_skin_max_qp       : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_skin_qp_delta     : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_SKIN;

/* Define the union U_VEDU_QPG_INTRA_DET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_intra_det_min_qp  : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_intra_det_max_qp  : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    qpg_intra_det_qp_delta : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_INTRA_DET;

/* Define the union U_VEDU_QPG_CU32_DELTA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_cu32_delta_low    : 4   ; /* [3..0]  */
        unsigned int    qpg_cu32_delta_high   : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU32_DELTA;

/* Define the union U_VEDU_QPG_LAMBDA_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda_qp_offset  : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    qpg_rdo_lambda_choose_mode : 2   ; /* [9..8]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    qpg_lambda_inter_stredge_en : 1   ; /* [12]  */
        unsigned int    reserved_2            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_LAMBDA_MODE;

/* Define the union U_VEDU_QPG_QP_RESTRAIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_qp_restrain_madi_thr : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    qpg_qp_restrain_en    : 1   ; /* [8]  */
        unsigned int    qpg_qp_restrain_mode  : 2   ; /* [10..9]  */
        unsigned int    reserved_1            : 1   ; /* [11]  */
        unsigned int    qpg_qp_restrain_delta_blk16 : 4   ; /* [15..12]  */
        unsigned int    qpg_qp_restrain_delta_blk32 : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_RESTRAIN;

/* Define the union U_VEDU_QPG_CU_MIN_SAD_REG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_min_sad_level     : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 8   ; /* [15..8]  */
        unsigned int    qpg_low_min_sad_mode  : 2   ; /* [17..16]  */
        unsigned int    reserved_1            : 2   ; /* [19..18]  */
        unsigned int    qpg_min_sad_madi_en   : 1   ; /* [20]  */
        unsigned int    qpg_min_sad_qp_restrain_en : 1   ; /* [21]  */
        unsigned int    reserved_2            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_CU_MIN_SAD_REG;

/* Define the union U_VEDU_QPG_FLAT_REGION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_flat_region_qp_delta : 4   ; /* [3..0]  */
        unsigned int    qpg_flat_region_max_qp : 6   ; /* [9..4]  */
        unsigned int    reserved_0            : 2   ; /* [11..10]  */
        unsigned int    qpg_flat_region_min_qp : 6   ; /* [17..12]  */
        unsigned int    reserved_1            : 6   ; /* [23..18]  */
        unsigned int    vcpi_cu32_use_cu16_mean_en : 1   ; /* [24]  */
        unsigned int    reserved_2            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_FLAT_REGION;

/* Define the union U_VEDU_QPG_RES_COEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_res_coef_en      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vcpi_large_res_coef   : 5   ; /* [8..4]  */
        unsigned int    reserved_1            : 3   ; /* [11..9]  */
        unsigned int    vcpi_small_res_coef   : 5   ; /* [16..12]  */
        unsigned int    reserved_2            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_RES_COEF;

/* Define the union U_VEDU_TILE_RC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_tile_level_rc_en : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vcpi_tile_line_qp_delta : 4   ; /* [7..4]  */
        unsigned int    vcpi_tile_bound_qp_delta : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_TILE_RC;

/* Define the union U_VEDU_CHROMA_PROTECT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_prot_en   : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    vcpi_chroma_max_qp    : 6   ; /* [9..4]  */
        unsigned int    reserved_1            : 2   ; /* [11..10]  */
        unsigned int    vcpi_chroma_in_qp     : 6   ; /* [17..12]  */
        unsigned int    reserved_2            : 2   ; /* [19..18]  */
        unsigned int    vcpi_chroma_qp_delta  : 4   ; /* [23..20]  */
        unsigned int    reserved_3            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_PROTECT;

/* Define the union U_VEDU_PME_QPG_RC_THR0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 16  ; /* [15..0]  */
        unsigned int    pme_madi_dif_thr      : 7   ; /* [22..16]  */
        unsigned int    pme_cur_madi_dif_thr  : 7   ; /* [29..23]  */
        unsigned int    reserved_1            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_QPG_RC_THR0;

/* Define the union U_VEDU_PME_QPG_RC_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_min_sad_thr_offset : 8   ; /* [7..0]  */
        unsigned int    pme_min_sad_thr_gain  : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 8   ; /* [19..12]  */
        unsigned int    pme_min_sad_thr_offset_cur : 8   ; /* [27..20]  */
        unsigned int    pme_min_sad_thr_gain_cur : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_QPG_RC_THR1;

/* Define the union U_VEDU_PME_LOW_LUMA_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_low_luma_thr      : 8   ; /* [7..0]  */
        unsigned int    pme_low_luma_madi_thr : 8   ; /* [15..8]  */
        unsigned int    pme_high_luma_thr     : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_LOW_LUMA_THR;

/* Define the union U_VEDU_PME_CHROMA_FLAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_flat_v_thr_high   : 8   ; /* [7..0]  */
        unsigned int    pme_flat_v_thr_low    : 8   ; /* [15..8]  */
        unsigned int    pme_flat_u_thr_high   : 8   ; /* [23..16]  */
        unsigned int    pme_flat_u_thr_low    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_CHROMA_FLAT;

/* Define the union U_VEDU_PME_LUMA_FLAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_flat_pmemv_thr    : 8   ; /* [7..0]  */
        unsigned int    pme_flat_luma_madi_thr : 8   ; /* [15..8]  */
        unsigned int    pme_flat_low_luma_thr : 8   ; /* [23..16]  */
        unsigned int    pme_flat_high_luma_thr : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_LUMA_FLAT;

/* Define the union U_VEDU_PME_MADI_FLAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_flat_pmesad_thr   : 14  ; /* [13..0]  */
        unsigned int    pme_flat_icount_thr   : 9   ; /* [22..14]  */
        unsigned int    pme_flat_region_cnt   : 5   ; /* [27..23]  */
        unsigned int    pme_flat_madi_times   : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_MADI_FLAT;

/* Define the union U_VEDU_VLCST_DESCRIPTOR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vlcst_idrind          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 15  ; /* [15..1]  */
        unsigned int    vlcst_chnid           : 8   ; /* [23..16]  */
        unsigned int    reserved_1            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VLCST_DESCRIPTOR;

/* Define the union U_VEDU_CURLD_CLIP_LUMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_clip_luma_min   : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    curld_clip_luma_max   : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_CLIP_LUMA;

/* Define the union U_VEDU_CURLD_CLIP_CHROMA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    curld_clip_chroma_min : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    curld_clip_chroma_max : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CURLD_CLIP_CHROMA;

/* Define the union U_VEDU_TQITQ_DEADZONE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tqitq_deadzone_intra_slice : 8   ; /* [7..0]  */
        unsigned int    tqitq_deadzone_inter_slice : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_TQITQ_DEADZONE;

/* Define the union U_VEDU_VCPI_PME_PARAM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    vcpi_move_sad_en      : 1   ; /* [1]  */
        unsigned int    reserved_1            : 14  ; /* [15..2]  */
        unsigned int    vcpi_pblk_pre_mvx_thr : 8   ; /* [23..16]  */
        unsigned int    vcpi_pblk_pre_mvy_thr : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PME_PARAM;

/* Define the union U_VEDU_VCPI_PIC_STRONG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_skin_en          : 1   ; /* [0]  */
        unsigned int    vcpi_strong_edge_en   : 1   ; /* [1]  */
        unsigned int    vcpi_still_en         : 1   ; /* [2]  */
        unsigned int    vcpi_skin_close_angle : 1   ; /* [3]  */
        unsigned int    vcpi_rounding_sobel_en : 1   ; /* [4]  */
        unsigned int    vcpi_pintra_structure_edge_thr : 18 ; /* [22..5]  */
        unsigned int    vcpi_pintra_structure_edge_qp_offset : 6 ; /* [28..23]  */
        unsigned int    reserved_0            : 3  ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PIC_STRONG_EN;

/* Define the union U_VEDU_VCPI_PINTRA_THRESH0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_amp_th : 8   ; /* [7..0]  */
        unsigned int    vcpi_pintra_pu32_amp_th : 8   ; /* [15..8]  */
        unsigned int    vcpi_pintra_pu64_amp_th : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PINTRA_THRESH0;

/* Define the union U_VEDU_VCPI_PINTRA_THRESH1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_std_th : 8   ; /* [7..0]  */
        unsigned int    vcpi_pintra_pu32_std_th : 8   ; /* [15..8]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PINTRA_THRESH1;

/* Define the union U_VEDU_VCPI_PINTRA_THRESH2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pintra_pu16_angel_cost_th : 10  ; /* [9..0]  */
        unsigned int    vcpi_pintra_pu32_angel_cost_th : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 3   ; /* [22..20] */
        unsigned int    vcpi_rpintra_only_once : 1   ; /* [23] */
        unsigned int    vcpi_rpintra_pu4_strong_edge_th : 3   ; /* [26..24]  */
        unsigned int    reserved_1            : 1   ; /* [27]  */
        unsigned int    vcpi_rpintra_pu4_mode_distance_th : 2   ; /* [29..28]  */
        unsigned int    vcpi_rpintra_bypass   : 1   ; /* [30]  */
        unsigned int    reserved_2            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PINTRA_THRESH2;

/* Define the union U_VEDU_VCPI_INTRA32_LOW_POWER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra32_low_power_thr : 16  ; /* [15..0]  */
        unsigned int    vcpi_intra32_low_power_en : 1   ; /* [16]  */
        unsigned int    reserved_0            : 3   ; /* [19..17]  */
        unsigned int    vcpi_intra32_low_power_gain : 6   ; /* [25..20]  */
        unsigned int    vcpi_intra32_low_power_offset : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTRA32_LOW_POWER;

/* Define the union U_VEDU_VCPI_INTRA16_LOW_POWER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra16_low_power_thr : 16  ; /* [15..0]  */
        unsigned int    vcpi_intra16_low_power_en : 1   ; /* [16]  */
        unsigned int    reserved_0            : 3   ; /* [19..17]  */
        unsigned int    vcpi_intra16_low_power_gain : 6   ; /* [25..20]  */
        unsigned int    vcpi_intra16_low_power_offset : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTRA16_LOW_POWER;

/* Define the union U_VEDU_VCPI_INTRA_REDUCE_RDO_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra_reduce_rdo_num_thr : 12  ; /* [11..0]  */
        unsigned int    reserved_0            : 4   ; /* [15..12]  */
        unsigned int    vcpi_intra_reduce_rdo_num_en : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_INTRA_REDUCE_RDO_NUM;

/* Define the union U_VEDU_VCPI_NOFORCEZERO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_bislayer0flag    : 1   ; /* [0]  */
        unsigned int    vcpi_bnoforcezero_flag : 1   ; /* [1]  */
        unsigned int    vcpi_bnoforcezero_posx : 4   ; /* [5..2]  */
        unsigned int    vcpi_bnoforcezero_posy : 4   ; /* [9..6]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_NOFORCEZERO;

/* Define the union U_VEDU_PME_SKIP_LARGE_RES */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skip_sad_thr_offset : 8   ; /* [7..0]  */
        unsigned int    pme_skip_sad_thr_gain : 4   ; /* [11..8]  */
        unsigned int    pme_skip_large_res_det : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SKIP_LARGE_RES;

/* Define the union U_VEDU_PME_SKIN_SAD_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_skin_sad_thr_offset : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 4   ; /* [11..8]  */
        unsigned int    vcpi_skin_sad_thr_gain : 4   ; /* [15..12]  */
        unsigned int    reserved_1            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SKIN_SAD_THR;

/* Define the union U_VEDU_VCTRL_NM_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_ChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_ChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_IChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_LowFreqACBlk16  : 4   ; /* [15..12]  */
        unsigned int    vctrl_ChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_IChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_LowFreqACBlk32  : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_NM_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_NM_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_ChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_ChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_ChosOthBlkThr3  : 6   ; /* [13..8]  */
        unsigned int    vctrl_ChosOthBlkThr2  : 6   ; /* [19..14]  */
        unsigned int    vctrl_ChosOthBlkThr   : 5   ; /* [24..20]  */
        unsigned int    vctrl_RmAllHighACThr  : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_NM_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_NM_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_RingEngThr      : 4   ; /* [3..0]  */
        unsigned int    vctrl_RingACThr       : 4   ; /* [7..4]  */
        unsigned int    vctrl_PChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_PChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_IChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_LowFreqACBlk8   : 3   ; /* [22..20]  */
        unsigned int    vctrl_Blk8EnableFlag  : 1   ; /* [23]  */
        unsigned int    vctrl_ChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_ChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_NM_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_SK_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_SkinChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_SkinChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_SkinIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_SkinLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_SkinChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_SkinIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_SkinLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SK_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_SK_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_SkinChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_SkinChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_SkinChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_SkinChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_SkinChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_SkinRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SK_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_SK_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_SkinRingEngThr  : 4   ; /* [3..0]  */
        unsigned int    vctrl_SkinRingACThr   : 4   ; /* [7..4]  */
        unsigned int    vctrl_SkinPChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_SkinPChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_SkinIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_SkinLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_SkinBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_SkinChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_SkinChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SK_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_ST_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StillChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_StillChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_StillIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_StillLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_StillChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_StillIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_StillLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ST_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_ST_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StillChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_StillChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_StillChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_StillChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_StillChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_StillRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ST_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_ST_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StillRingEngThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_StillRingACThr  : 4   ; /* [7..4]  */
        unsigned int    vctrl_StillPChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_StillPChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_StillIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_StillLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_StillBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_StillChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_StillChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_ST_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_SE_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgeIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_EdgeLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_EdgeChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_EdgeIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_EdgeLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SE_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_SE_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgeChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_EdgeChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_EdgeChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_EdgeRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SE_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_SE_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeRingEngThr  : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeRingACThr   : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgePChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_EdgePChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_EdgeIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_EdgeLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_EdgeBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_EdgeChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_EdgeChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SE_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_SELM_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeAndMoveChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgeAndMoveIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_EdgeAndMoveLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_EdgeAndMoveIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_EdgeAndMoveLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SELM_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_SELM_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeAndMoveChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_EdgeAndMoveRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SELM_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_SELM_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_EdgeAndMoveRingEngThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_EdgeAndMoveRingACThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_EdgeAndMovePChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_EdgeAndMovePChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_EdgeAndMoveIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_EdgeAndMoveLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_EdgeAndMoveBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_EdgeAndMoveChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SELM_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_WS_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_WeakChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_WeakChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_WeakIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_WeakLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_WeakChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_WeakIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_WeakLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_WS_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_WS_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_WeakChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_WeakChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_WeakChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_WeakChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_WeakChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_WeakRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_WS_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_WS_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_WeakRingEngThr  : 4   ; /* [3..0]  */
        unsigned int    vctrl_WeakRingACThr   : 4   ; /* [7..4]  */
        unsigned int    vctrl_WeakPChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_WeakPChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_WeakIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_WeakLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_WeakBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_WeakChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_WeakChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_WS_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_SSSE_ACOFFSET_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StrongChosOthBlkSpecOffset : 4   ; /* [3..0]  */
        unsigned int    vctrl_StrongChosOthBlkOffset16 : 4   ; /* [7..4]  */
        unsigned int    vctrl_StrongIChosCurBlkOffset16 : 4   ; /* [11..8]  */
        unsigned int    vctrl_StrongLowFreqACBlk16 : 4   ; /* [15..12]  */
        unsigned int    vctrl_StrongChosOthBlkOffset32 : 4   ; /* [19..16]  */
        unsigned int    vctrl_StrongIChosCurBlkOffset32 : 4   ; /* [23..20]  */
        unsigned int    vctrl_StrongLowFreqACBlk32 : 4   ; /* [27..24]  */
        unsigned int    reserved_0            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SSSE_ACOFFSET_DENOISE;

/* Define the union U_VEDU_VCTRL_SSSE_ENGTHR_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StrongChosOthBlkSpecThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_StrongChosOthBlkEngThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_StrongChosOthBlkThr3 : 6   ; /* [13..8]  */
        unsigned int    vctrl_StrongChosOthBlkThr2 : 6   ; /* [19..14]  */
        unsigned int    vctrl_StrongChosOthBlkThr : 5   ; /* [24..20]  */
        unsigned int    vctrl_StrongRmAllHighACThr : 4   ; /* [28..25]  */
        unsigned int    reserved_0            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SSSE_ENGTHR_DENOISE;

/* Define the union U_VEDU_VCTRL_SSSE_TU8_DENOISE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_StrongRingEngThr : 4   ; /* [3..0]  */
        unsigned int    vctrl_StrongRingACThr : 4   ; /* [7..4]  */
        unsigned int    vctrl_StrongPChosOthBlkOffset : 4   ; /* [11..8]  */
        unsigned int    vctrl_StrongPChosOthBlkOffset8 : 4   ; /* [15..12]  */
        unsigned int    vctrl_StrongIChosCurBlkOffset8 : 4   ; /* [19..16]  */
        unsigned int    vctrl_StrongLowFreqACBlk8 : 3   ; /* [22..20]  */
        unsigned int    vctrl_StrongBlk8EnableFlag : 1   ; /* [23]  */
        unsigned int    vctrl_StrongChosOthBlkOffsetSec16 : 4   ; /* [27..24]  */
        unsigned int    vctrl_StrongChosOthBlkOffsetSec32 : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_SSSE_TU8_DENOISE;

/* Define the union U_VEDU_VCTRL_INTRA_RDO_FACTOR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_intra_cu4_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_intra_cu8_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_intra_cu16_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_intra_cu32_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_intra_cu4_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_intra_cu8_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_intra_cu16_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_intra_cu32_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_INTRA_RDO_FACTOR_0;

/* Define the union U_VEDU_VCTRL_INTRA_RDO_FACTOR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_intra_cu4_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_intra_cu8_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_intra_cu16_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_intra_cu32_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_intra_cu4_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_intra_cu8_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_intra_cu16_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_intra_cu32_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_INTRA_RDO_FACTOR_1;

/* Define the union U_VEDU_VCTRL_INTRA_RDO_FACTOR_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_intra_cu4_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_intra_cu8_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_intra_cu16_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_intra_cu32_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_intra_cu4_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_intra_cu8_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_intra_cu16_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_intra_cu32_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_INTRA_RDO_FACTOR_2;

/* Define the union U_VEDU_VCTRL_MRG_RDO_FACTOR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_mrg_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_mrg_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_mrg_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_mrg_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_mrg_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_mrg_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_mrg_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_mrg_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_MRG_RDO_FACTOR_0;

/* Define the union U_VEDU_VCTRL_MRG_RDO_FACTOR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_mrg_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_mrg_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_mrg_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_mrg_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_mrg_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_mrg_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_mrg_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_mrg_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_MRG_RDO_FACTOR_1;

/* Define the union U_VEDU_VCTRL_MRG_RDO_FACTOR_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_mrg_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_mrg_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_mrg_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_mrg_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_mrg_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_mrg_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_mrg_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_mrg_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_MRG_RDO_FACTOR_2;

/* Define the union U_VEDU_VCTRL_FME_RDO_FACTOR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_norm_fme_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_norm_fme_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_norm_fme_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_norm_fme_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_strmov_fme_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_strmov_fme_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_strmov_fme_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_strmov_fme_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_FME_RDO_FACTOR_0;

/* Define the union U_VEDU_VCTRL_FME_RDO_FACTOR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_skin_fme_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_skin_fme_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_skin_fme_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_skin_fme_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_str_fme_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_str_fme_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_str_fme_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_str_fme_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_FME_RDO_FACTOR_1;

/* Define the union U_VEDU_VCTRL_FME_RDO_FACTOR_2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vctrl_hedge_fme_cu8_rdcost_offset : 4   ; /* [3..0]  */
        unsigned int    vctrl_hedge_fme_cu16_rdcost_offset : 4   ; /* [7..4]  */
        unsigned int    vctrl_hedge_fme_cu32_rdcost_offset : 4   ; /* [11..8]  */
        unsigned int    vctrl_hedge_fme_cu64_rdcost_offset : 4   ; /* [15..12]  */
        unsigned int    vctrl_sobel_tex_fme_cu8_rdcost_offset : 4   ; /* [19..16]  */
        unsigned int    vctrl_sobel_tex_fme_cu16_rdcost_offset : 4   ; /* [23..20]  */
        unsigned int    vctrl_sobel_tex_fme_cu32_rdcost_offset : 4   ; /* [27..24]  */
        unsigned int    vctrl_sobel_tex_fme_cu64_rdcost_offset : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_FME_RDO_FACTOR_2;

/* Define the union U_VEDU_PME_NEW_COST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_cost_lamda0       : 4   ; /* [3..0]  */
        unsigned int    pme_cost_lamda1       : 4   ; /* [7..4]  */
        unsigned int    pme_cost_lamda2       : 4   ; /* [11..8]  */
        unsigned int    pme_new_cost_en       : 2   ; /* [13..12]  */
        unsigned int    reserved_0            : 2   ; /* [15..14]  */
        unsigned int    pme_cost_lamda_en     : 2   ; /* [17..16]  */
        unsigned int    pme_mvp3median_en     : 1   ; /* [18]  */
        unsigned int    reserved_1            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_NEW_COST;

/* Define the union U_VEDU_PME_COST_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_cost_offset    : 16  ; /* [15..0]  */
        unsigned int    pme_l1_cost_offset    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_COST_OFFSET;

/* Define the union U_VEDU_PME_ADJUST_PMEMV_H264 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_adjust_pmemv_dist_times : 8   ; /* [7..0]  */
        unsigned int    pme_adjust_pmemv_en   : 1   ; /* [8]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_ADJUST_PMEMV_H264;

/* Define the union U_VEDU_PME_INTRABLK_DET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intrablk_det_cost_thr0 : 16  ; /* [15..0]  */
        unsigned int    pme_pskip_mvy_consistency_thr : 8   ; /* [23..16]  */
        unsigned int    pme_pskip_mvx_consistency_thr : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_INTRABLK_DET;

/* Define the union U_VEDU_PME_INTRABLK_DET_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intrablk_det_mv_dif_thr1 : 8   ; /* [7..0]  */
        unsigned int    pme_intrablk_det_mv_dif_thr0 : 8   ; /* [15..8]  */
        unsigned int    pme_intrablk_det_mvy_thr : 8   ; /* [23..16]  */
        unsigned int    pme_intrablk_det_mvx_thr : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_INTRABLK_DET_THR;

/* Define the union U_VEDU_PME_SKIN_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skin_u_max_thr    : 8   ; /* [7..0]  */
        unsigned int    pme_skin_u_min_thr    : 8   ; /* [15..8]  */
        unsigned int    pme_skin_v_max_thr    : 8   ; /* [23..16]  */
        unsigned int    pme_skin_v_min_thr    : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SKIN_THR;

/* Define the union U_VEDU_PME_STRONG_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skin_num          : 9   ; /* [8..0]  */
        unsigned int    pme_strong_edge_thr   : 8   ; /* [16..9]  */
        unsigned int    pme_strong_edge_cnt   : 5   ; /* [21..17]  */
        unsigned int    pme_still_scene_thr   : 9   ; /* [30..22]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_STRONG_EDGE;

/* Define the union U_VEDU_PME_LARGE_MOVE_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_move_scene_thr    : 8   ; /* [7..0]  */
        unsigned int    pme_move_sad_thr      : 14  ; /* [21..8]  */
        unsigned int    reserved_0            : 10  ; /* [31..22]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_LARGE_MOVE_THR;

/* Define the union U_VEDU_PME_INTER_STRONG_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_interdiff_max_min_madi_abs : 8   ; /* [7..0]  */
        unsigned int    pme_interdiff_max_min_madi_times : 8   ; /* [15..8]  */
        unsigned int    pme_interstrongedge_madi_thr : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_INTER_STRONG_EDGE;

/* Define the union U_VEDU_PME_CHROMA_STRONG_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_strong_edge_thr_u : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 2   ; /* [9..8]  */
        unsigned int    vcpi_strong_edge_cnt_u : 5   ; /* [14..10]  */
        unsigned int    reserved_1            : 1   ; /* [15]  */
        unsigned int    vcpi_strong_edge_thr_v : 8   ; /* [23..16]  */
        unsigned int    reserved_2            : 2   ; /* [25..24]  */
        unsigned int    vcpi_strong_edge_cnt_v : 5   ; /* [30..26]  */
        unsigned int    reserved_3            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_CHROMA_STRONG_EDGE;

/* Define the union U_VEDU_CHROMA_SAD_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_sad_thr_offset : 8   ; /* [7..0]  */
        unsigned int    vcpi_chroma_sad_thr_gain : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_SAD_THR;

/* Define the union U_VEDU_CHROMA_FG_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_u0_thr_min : 8   ; /* [7..0]  */
        unsigned int    vcpi_chroma_u0_thr_max : 8   ; /* [15..8]  */
        unsigned int    vcpi_chroma_v0_thr_min : 8   ; /* [23..16]  */
        unsigned int    vcpi_chroma_v0_thr_max : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_FG_THR;

/* Define the union U_VEDU_CHROMA_BG_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_u1_thr_min : 8   ; /* [7..0]  */
        unsigned int    vcpi_chroma_u1_thr_max : 8   ; /* [15..8]  */
        unsigned int    vcpi_chroma_v1_thr_min : 8   ; /* [23..16]  */
        unsigned int    vcpi_chroma_v1_thr_max : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_BG_THR;

/* Define the union U_VEDU_CHROMA_SUM_FG_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_uv0_thr_min : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    vcpi_chroma_uv0_thr_max : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_SUM_FG_THR;

/* Define the union U_VEDU_CHROMA_SUM_BG_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_uv1_thr_min : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    vcpi_chroma_uv1_thr_max : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_SUM_BG_THR;

/* Define the union U_VEDU_CHROMA_FG_COUNT_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_count0_thr_min : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    vcpi_chroma_count0_thr_max : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_FG_COUNT_THR;

/* Define the union U_VEDU_CHROMA_BG_COUNT_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_chroma_count1_thr_min : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 7   ; /* [15..9]  */
        unsigned int    vcpi_chroma_count1_thr_max : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CHROMA_BG_COUNT_THR;

/* Define the union U_VEDU_PME_MOVE_SCENE_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_move_scene_mv_thr : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 3   ; /* [15..13]  */
        unsigned int    vcpi_move_scene_mv_en : 1   ; /* [16]  */
        unsigned int    reserved_1            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_MOVE_SCENE_THR;

/* Define the union U_VEDU_PME_NEW_MADI_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_new_madi_th0     : 8   ; /* [7..0]  */
        unsigned int    vcpi_new_madi_th1     : 8   ; /* [15..8]  */
        unsigned int    vcpi_new_madi_th2     : 8   ; /* [23..16]  */
        unsigned int    vcpi_new_madi_th3     : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_NEW_MADI_TH;

/* Define the union U_VEDU_PME_NEW_LAMBDA */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_new_lambda       : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_NEW_LAMBDA;

/* Define the union U_VEDU_VCPI_DBLKCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_dblk_beta        : 4   ; /* [3..0]  */
        unsigned int    vcpi_dblk_alpha       : 4   ; /* [7..4]  */
        unsigned int    vcpi_dblk_filter_flag : 2   ; /* [9..8]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_DBLKCFG;

/* Define the union U_VEDU_FME_BIAS_COST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_pu8_bias_cost     : 16  ; /* [15..0]  */
        unsigned int    fme_pu16_bias_cost    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_FME_BIAS_COST0;

/* Define the union U_VEDU_FME_BIAS_COST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_pu32_bias_cost    : 16  ; /* [15..0]  */
        unsigned int    fme_pu64_bias_cost    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_FME_BIAS_COST1;

/* Define the union U_VEDU_MRG_BIAS_COST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_pu8_bias_cost     : 16  ; /* [15..0]  */
        unsigned int    mrg_pu16_bias_cost    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_BIAS_COST0;

/* Define the union U_VEDU_MRG_BIAS_COST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_pu32_bias_cost    : 16  ; /* [15..0]  */
        unsigned int    mrg_pu64_bias_cost    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_BIAS_COST1;

/* Define the union U_VEDU_MRG_ABS_OFFSET0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_pu8_abs_offset    : 16  ; /* [15..0]  */
        unsigned int    mrg_pu16_abs_offset   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_ABS_OFFSET0;

/* Define the union U_VEDU_MRG_ABS_OFFSET1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_pu32_abs_offset   : 16  ; /* [15..0]  */
        unsigned int    mrg_pu64_abs_offset   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_ABS_OFFSET1;

/* Define the union U_VEDU_MRG_ADJ_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu8_fz_weight         : 4   ; /* [3..0]  */
        unsigned int    cu16_fz_weight        : 4   ; /* [7..4]  */
        unsigned int    cu32_fz_weight        : 4   ; /* [11..8]  */
        unsigned int    cu64_fz_weight        : 4   ; /* [15..12]  */
        unsigned int    cu8_fz_adapt_weight   : 4   ; /* [19..16]  */
        unsigned int    cu16_fz_adapt_weight  : 4   ; /* [23..20]  */
        unsigned int    cu32_fz_adapt_weight  : 4   ; /* [27..24]  */
        unsigned int    cu64_fz_adapt_weight  : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_ADJ_WEIGHT;

/* Define the union U_VEDU_INTRA_BIT_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_bit_weight      : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_BIT_WEIGHT;

/* Define the union U_VEDU_INTRA_RDO_COST_OFFSET_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_rdo_cost_offset : 16  ; /* [15..0]  */
        unsigned int    intra_cu32_rdo_cost_offset : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_RDO_COST_OFFSET_0;

/* Define the union U_VEDU_INTRA_RDO_COST_OFFSET_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_rdo_cost_offset : 16  ; /* [15..0]  */
        unsigned int    intra_cu8_rdo_cost_offset : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_RDO_COST_OFFSET_1;

/* Define the union U_VEDU_INTRA_NO_DC_COST_OFFSET_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu16_non_dc_mode_offset : 16  ; /* [15..0]  */
        unsigned int    intra_cu32_non_dc_mode_offset : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_NO_DC_COST_OFFSET_0;

/* Define the union U_VEDU_INTRA_NO_DC_COST_OFFSET_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_cu4_non_dc_mode_offset : 16  ; /* [15..0]  */
        unsigned int    intra_cu8_non_dc_mode_offset : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_NO_DC_COST_OFFSET_1;

/* Define the union U_VEDU_INTRA_RDO_COST_OFFSET_3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    intra_h264_rdo_cost_offset : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_RDO_COST_OFFSET_3;

/* Define the union U_VEDU_SEL_OFFSET_STRENGTH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_offset_strength   : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SEL_OFFSET_STRENGTH;

/* Define the union U_VEDU_SEL_CU32_DC_AC_TH_OFFSET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_cu32_dc_ac_th_offset : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SEL_CU32_DC_AC_TH_OFFSET;

/* Define the union U_VEDU_SEL_CU32_QP_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_cu32_qp0_th       : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    sel_cu32_qp1_th       : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SEL_CU32_QP_TH;

/* Define the union U_VEDU_SEL_RES_DC_AC_TH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sel_res16_luma_dc_th  : 4   ; /* [3..0]  */
        unsigned int    sel_res16_chroma_dc_th : 4   ; /* [7..4]  */
        unsigned int    sel_res16_luma_ac_th  : 4   ; /* [11..8]  */
        unsigned int    sel_res16_chroma_ac_th : 4   ; /* [15..12]  */
        unsigned int    sel_res32_luma_dc_th  : 4   ; /* [19..16]  */
        unsigned int    sel_res32_chroma_dc_th : 4   ; /* [23..20]  */
        unsigned int    sel_res32_luma_ac_th  : 4   ; /* [27..24]  */
        unsigned int    sel_res32_chroma_ac_th : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SEL_RES_DC_AC_TH;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda00          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG00;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda01          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG01;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda02          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG02;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG03 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda03          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG03;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG04 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda04          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG04;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG05 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda05          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG05;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG06 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda06          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG06;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG07 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda07          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG07;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG08 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda08          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG08;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG09 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda09          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG09;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda10          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG10;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda11          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG11;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda12          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG12;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda13          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG13;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda14          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG14;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda15          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG15;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda16          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG16;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda17          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG17;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda18          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG18;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda19          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG19;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda20          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG20;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda21          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG21;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda22          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG22;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda23          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG23;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda24          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG24;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda25          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG25;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG26 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda26          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG26;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG27 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda27          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG27;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG28 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda28          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG28;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG29 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda29          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG29;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG30 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda30          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG30;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG31 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda31          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG31;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG32 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda32          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG32;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG33 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda33          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG33;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG34 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda34          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG34;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG35 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda35          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG35;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG36 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda36          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG36;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG37 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda37          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG37;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG38 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda38          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG38;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG39 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda39          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG39;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG40 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda40          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG40;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG41 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda41          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG41;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG42 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda42          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG42;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG43 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda43          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG43;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG44 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda44          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG44;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG45 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda45          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG45;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG46 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda46          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG46;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG47 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda47          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG47;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG48 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda48          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG48;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG49 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda49          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG49;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG50 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda50          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG50;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG51 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda51          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG51;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG52 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda52          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG52;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG53 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda53          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG53;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG54 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda54          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG54;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG55 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda55          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG55;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG56 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda56          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG56;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG57 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda57          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG57;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG58 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda58          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG58;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG59 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda59          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG59;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG60 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda60          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG60;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG61 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda61          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG61;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG62 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda62          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG62;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG63 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_lambda63          : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG63;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG64 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda00     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG64;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG65 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda01     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG65;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG66 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda02     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG66;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG67 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda03     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG67;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG68 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda04     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG68;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG69 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda05     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG69;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG70 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda06     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG70;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG71 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda07     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG71;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG72 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda08     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG72;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG73 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda09     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG73;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG74 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda10     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG74;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG75 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda11     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG75;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG76 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda12     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG76;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG77 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda13     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG77;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG78 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda14     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG78;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG79 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda15     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG79;

/* Define the union U_VEDU_IME_RDOCFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_lambdaoff8        : 16  ; /* [15..0]  */
        unsigned int    ime_lambdaoff16       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_RDOCFG;

/* Define the union U_VEDU_MRG_FORCE_ZERO_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mrg_force_zero_en     : 1   ; /* [0]  */
        unsigned int    mrg_force_y_zero_en   : 1   ; /* [1]  */
        unsigned int    mrg_force_u_zero_en   : 1   ; /* [2]  */
        unsigned int    mrg_force_v_zero_en   : 1   ; /* [3]  */
        unsigned int    fme_rdo_lpw_en        : 1   ; /* [4]  */
        unsigned int    dct4_en               : 1   ; /* [5]  */
        unsigned int    force_adapt_en        : 1   ; /* [6]  */
        unsigned int    reserved_0            : 5   ; /* [11..7]  */
        unsigned int    rqt_bias_weight       : 4   ; /* [15..12]  */
        unsigned int    fme_rdo_lpw_th        : 10  ; /* [25..16]  */
        unsigned int    mrg_skip_weight_en    : 1   ; /* [26]  */
        unsigned int    reserved_1            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_FORCE_ZERO_EN;

/* Define the union U_VEDU_VCPI_LOW_POWER */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intra_lowpow_en  : 1   ; /* [0]  */
        unsigned int    vcpi_fme_lowpow_en    : 1   ; /* [1]  */
        unsigned int    vcpi_ime_lowpow_en    : 1   ; /* [2]  */
        unsigned int    powerpro_func_bypass  : 1   ; /* [3]  */
        unsigned int    vcpi_ddr_cross_idx    : 11  ; /* [14..4]  */
        unsigned int    reserved_0            : 1   ; /* [15]  */
        unsigned int    vcpi_tqitq_gtck_en    : 1   ; /* [16]  */
        unsigned int    vcpi_mrg_gtck_en      : 1   ; /* [17]  */
        unsigned int    vcpi_recst_hebc_clkgate_en : 1   ; /* [18]  */
        unsigned int    vcpi_clkgate_en       : 2   ; /* [20..19]  */
        unsigned int    vcpi_mem_clkgate_en   : 1   ; /* [21]  */
        unsigned int    venc_dpm_apb_clk_en   : 1   ; /* [22]  */
        unsigned int    vcpi_ddr_cross_en     : 1   ; /* [23]  */
        unsigned int    vcpi_osd_clkgete_en   : 1   ; /* [24]  */
        unsigned int    vcpi_ghdr_clkgate_en  : 1   ; /* [25]  */
        unsigned int    vcpi_curld_dcmp_clkgate_en : 1   ; /* [26]  */
        unsigned int    vcpi_refld_dcmp_clkgate_en : 1   ; /* [27]  */
        unsigned int    vcpi_cpi_clkgate_en   : 1   ; /* [28]  */
        unsigned int    vcpi_vlc_clkgate_en   : 1   ; /* [29]  */
        unsigned int    venc_axi_dfx_clk_en   : 1   ; /* [30]  */
        unsigned int    venc_apb_cfg_force_clk_on : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_LOW_POWER;

/* Define the union U_VEDU_IME_INTER_MODE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_layer3to2_en      : 1   ; /* [0]  */
        unsigned int    ime_inter8x8_en       : 1   ; /* [1]  */
        unsigned int    ime_flat_region_force_low3layer : 1   ; /* [2]  */
        unsigned int    reserved_0            : 1   ; /* [3]  */
        unsigned int    ime_high3pre_en       : 2   ; /* [5..4]  */
        unsigned int    reserved_1            : 2   ; /* [7..6]  */
        unsigned int    ime_intra4_lowpow_en  : 1   ; /* [8]  */
        unsigned int    tile_boundry_improve_en : 1   ; /* [9]  */
        unsigned int    reserved_2            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_INTER_MODE;

/* Define the union U_VEDU_VCPI_PRE_JUDGE_EXT_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    reserved_0            : 1   ; /* [0]  */
        unsigned int    vcpi_pblk_pre_en      : 1   ; /* [1]  */
        unsigned int    vcpi_force_inter      : 1   ; /* [2]  */
        unsigned int    vcpi_pintra_inter_flag_disable : 1   ; /* [3]  */
        unsigned int    vcpi_ext_edge_en      : 1   ; /* [4]  */
        unsigned int    reserved_1            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PRE_JUDGE_EXT_EN;

/* Define the union U_VEDU_PME_SW_ADAPT_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_psw_adapt_en   : 1   ; /* [0]  */
        unsigned int    pme_l1_psw_adapt_en   : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SW_ADAPT_EN;

/* Define the union U_VEDU_PME_WINDOW_SIZE0_L0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l0_win0_width     : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    pme_l0_win0_height    : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_WINDOW_SIZE0_L0;

/* Define the union U_VEDU_PME_WINDOW_SIZE0_L1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_l1_win0_width     : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    pme_l1_win0_height    : 9   ; /* [24..16]  */
        unsigned int    reserved_1            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_WINDOW_SIZE0_L1;

/* Define the union U_VEDU_PME_SKIP_PRE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_skipblk_pre_cost_thr : 16  ; /* [15..0]  */
        unsigned int    pme_skipblk_pre_en    : 1   ; /* [16]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SKIP_PRE;

/* Define the union U_VEDU_PME_PBLK_PRE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_pblk_pre_mv_dif_thr1 : 8   ; /* [7..0]  */
        unsigned int    pme_pblk_pre_mv_dif_thr0 : 8   ; /* [15..8]  */
        unsigned int    pme_pblk_pre_mv_dif_cost_thr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_PBLK_PRE1;

/* Define the union U_VEDU_VCPI_PRE_JUDGE_COST_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_iblk_pre_cost_thr : 16  ; /* [15..0]  */
        unsigned int    vcpi_pblk_pre_cost_thr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PRE_JUDGE_COST_THR;

/* Define the union U_VEDU_VCPI_IBLK_PRE_MV_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_iblk_pre_mv_dif_thr0 : 8   ; /* [7..0]  */
        unsigned int    vcpi_iblk_pre_mv_dif_thr1 : 8   ; /* [15..8]  */
        unsigned int    vcpi_iblk_pre_mvx_thr : 8   ; /* [23..16]  */
        unsigned int    vcpi_iblk_pre_mvy_thr : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_IBLK_PRE_MV_THR;

/* Define the union U_VEDU_PME_IBLK_COST_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_iblk_pre_cost_thr_h264 : 16  ; /* [15..0]  */
        unsigned int    pme_intrablk_det_cost_thr1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_IBLK_COST_THR;

/* Define the union U_VEDU_PME_TR_WEIGHTX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_tr_weightx_0      : 9   ; /* [8..0]  */
        unsigned int    pme_tr_weightx_1      : 9   ; /* [17..9]  */
        unsigned int    pme_tr_weightx_2      : 9   ; /* [26..18]  */
        unsigned int    reserved_0            : 5   ; /* [31..27]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_TR_WEIGHTX;

/* Define the union U_VEDU_PME_TR_WEIGHTY */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_tr_weighty_0      : 8   ; /* [7..0]  */
        unsigned int    pme_tr_weighty_1      : 8   ; /* [15..8]  */
        unsigned int    pme_tr_weighty_2      : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_TR_WEIGHTY;

/* Define the union U_VEDU_PME_SR_WEIGHT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_sr_weight_0       : 4   ; /* [3..0]  */
        unsigned int    pme_sr_weight_1       : 4   ; /* [7..4]  */
        unsigned int    pme_sr_weight_2       : 4   ; /* [11..8]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SR_WEIGHT;

/* Define the union U_VEDU_PME_INTRA_LOWPOW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_intra16_madi_thr  : 8   ; /* [7..0]  */
        unsigned int    pme_intra32_madi_thr  : 8   ; /* [15..8]  */
        unsigned int    pme_intra_lowpow_en   : 1   ; /* [16]  */
        unsigned int    pme_inter_first       : 1   ; /* [17]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_INTRA_LOWPOW;

/* Define the union U_VEDU_IME_FME_LPOW_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_lowpow_fme_thr0   : 6   ; /* [5..0]  */
        unsigned int    ime_lowpow_fme_thr1   : 6   ; /* [11..6]  */
        unsigned int    reserved_0            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_FME_LPOW_THR;

/* Define the union U_VEDU_PME_SKIP_FLAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_pskip_strongedge_madi_thr : 8   ; /* [7..0]  */
        unsigned int    vcpi_pskip_strongedge_madi_times : 8   ; /* [15..8]  */
        unsigned int    vcpi_pskip_flatregion_madi_thr : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_SKIP_FLAG;

/* Define the union U_VEDU_PME_PSW_LPW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_psw_lp_diff_thy   : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    pme_psw_lp_diff_thx   : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 20  ; /* [31..12]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_PSW_LPW;

/* Define the union U_VEDU_PME_PBLK_PRE2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pme_pblk_pre_madi_times : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_PME_PBLK_PRE2;

/* Define the union U_VEDU_IME_LAYER3TO2_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_layer3to2_thr0    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 6   ; /* [15..10]  */
        unsigned int    ime_layer3to2_thr1    : 10  ; /* [25..16]  */
        unsigned int    reserved_1            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_LAYER3TO2_THR;

/* Define the union U_VEDU_IME_LAYER3TO2_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_layer3to2_cost_diff_thr : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_LAYER3TO2_THR1;

/* Define the union U_VEDU_IME_LAYER3TO1_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_layer3to1_en      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 7   ; /* [7..1]  */
        unsigned int    ime_layer3to1_pu64_madi_thr : 7   ; /* [14..8]  */
        unsigned int    reserved_1            : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_LAYER3TO1_THR;

/* Define the union U_VEDU_IME_LAYER3TO1_THR1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_layer3to1_pu32_cost_thr : 16  ; /* [15..0]  */
        unsigned int    ime_layer3to1_pu64_cost_thr : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_LAYER3TO1_THR1;

/* Define the union U_VEDU_FME_PU64_LWP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fme_pu64_lwp_flag     : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_FME_PU64_LWP;

/* Define the union U_VEDU_MRG_FORCE_SKIP_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    inter32_use_tu16_en   : 1   ; /* [0]  */
        unsigned int    mrg_not_use_sad_en    : 1   ; /* [1]  */
        unsigned int    mrg_pu32_no_sad_pk_en : 1   ; /* [2] */
        unsigned int    reserved_0            : 1   ; /* [3] */
        unsigned int    mrg_force_skip_en     : 1   ; /* [4]  */
        unsigned int    mrg_not_use_sad_th    : 18  ; /* [22..5]  */
        unsigned int    reserved_1            : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MRG_FORCE_SKIP_EN;

/* Define the union U_VEDU_INTRA_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    constrained_intra_pred_flag : 1   ; /* [0]  */
        unsigned int    reserved_0            : 3   ; /* [3..1]  */
        unsigned int    intra_smooth          : 1   ; /* [4]  */
        unsigned int    reserved_1            : 3   ; /* [7..5]  */
        unsigned int    vcpi_force_cu16_low_pow : 1   ; /* [8]  */
        unsigned int    reserved_2            : 3   ; /* [11..9]  */
        unsigned int    vcpi_force_cu32_low_pow : 1   ; /* [12]  */
        unsigned int    reserved_3            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_CFG;

/* Define the union U_VEDU_INTRA_LOW_POW */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_weak_ang_thr     : 4   ; /* [3..0]  */
        unsigned int    cu16_medium_ang_thr   : 4   ; /* [7..4]  */
        unsigned int    cu16_strong_ang_thr   : 4   ; /* [11..8]  */
        unsigned int    cu16_rdo_num          : 3   ; /* [14..12]  */
        unsigned int    cu16_adaptive_reduce_rdo_en : 1   ; /* [15]  */
        unsigned int    cu32_weak_ang_thr     : 4   ; /* [19..16]  */
        unsigned int    cu32_medium_ang_thr   : 4   ; /* [23..20]  */
        unsigned int    cu32_strong_ang_thr   : 4   ; /* [27..24]  */
        unsigned int    cu32_rdo_num          : 3   ; /* [30..28]  */
        unsigned int    cu32_adaptive_reduce_rdo_en : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_INTRA_LOW_POW;

/* Define the union U_VEDU_VCPI_INFORLD_MV_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_inforld_mv_confidence_stride : 16  ; /* [15..0] */
        unsigned int    vcpi_inforld_mv_stride : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_INFORLD_MV_STRIDE;

/* Define the union U_VEDU_VCPI_INFORLD_FEATURE_DETECT_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_inforld_last_feature_detect_stride : 16  ; /* [15..0] */
        unsigned int    vcpi_inforld_feature_detect_stride : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_INFORLD_FEATURE_DETECT_STRIDE;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp1 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp0 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT0;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp3 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp2 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT1;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp5 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp4 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT2;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp7 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp6 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT3;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp9 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp8 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT4;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp11 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp10 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT5;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp13 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp12 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT6;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp15 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp14 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT7;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp17 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp16 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT8;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp19 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp18 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT9;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp21 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp20 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT10;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp23 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp22 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT11;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp25 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp24 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT12;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp27 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp26 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT13;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp29 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp28 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT14;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp31 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp30 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT15;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp33 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp32 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT16;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp35 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp34 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT17;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp37 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp36 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT18;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp39 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp38 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT19;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp41 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp40 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT20;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp43 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp42 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT21;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp45 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp44 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT22;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp47 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp46 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT23;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp49 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp48 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT24;

/* Define the union U_VEDU_VCPI_SRDO_MAX_WEIGHT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_max_weight_qp51 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_max_weight_qp50 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MAX_WEIGHT25;
/* Define the union U_VEDU_SAO_SSD_AREA0_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area0_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area0_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA0_START;

/* Define the union U_VEDU_SAO_SSD_AREA0_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area0_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area0_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA0_END;

/* Define the union U_VEDU_SAO_SSD_AREA1_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area1_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area1_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA1_START;

/* Define the union U_VEDU_SAO_SSD_AREA1_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area1_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area1_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA1_END;

/* Define the union U_VEDU_SAO_SSD_AREA2_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area2_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area2_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA2_START;

/* Define the union U_VEDU_SAO_SSD_AREA2_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area2_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area2_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA2_END;

/* Define the union U_VEDU_SAO_SSD_AREA3_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area3_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area3_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA3_START;

/* Define the union U_VEDU_SAO_SSD_AREA3_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area3_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area3_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA3_END;

/* Define the union U_VEDU_SAO_SSD_AREA4_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area4_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area4_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA4_START;

/* Define the union U_VEDU_SAO_SSD_AREA4_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area4_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area4_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA4_END;

/* Define the union U_VEDU_SAO_SSD_AREA5_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area5_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area5_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA5_START;

/* Define the union U_VEDU_SAO_SSD_AREA5_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area5_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area5_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA5_END;

/* Define the union U_VEDU_SAO_SSD_AREA6_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area6_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area6_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA6_START;

/* Define the union U_VEDU_SAO_SSD_AREA6_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area6_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area6_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA6_END;

/* Define the union U_VEDU_SAO_SSD_AREA7_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area7_start_lcux  : 16  ; /* [15..0]  */
        unsigned int    sao_area7_start_lcuy  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA7_START;

/* Define the union U_VEDU_SAO_SSD_AREA7_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sao_area7_end_lcux    : 16  ; /* [15..0]  */
        unsigned int    sao_area7_end_lcuy    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SAO_SSD_AREA7_END;

/* Define the union U_VEDU_VCPI_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curld_c_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_curld_y_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_STRIDE;

/* Define the union U_VEDU_VCPI_EXT_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curld_ext_c_stride : 16  ; /* [15..0]  */
        unsigned int    vcpi_curld_ext_y_stride : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_EXT_STRIDE;

/* Define the union U_VEDU_VCPI_REF_L0_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_l0_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_l0_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_L0_STRIDE;

/* Define the union U_VEDU_VCPI_REF_L1_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_l1_stride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_l1_stride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_L1_STRIDE;

/* Define the union U_VEDU_VCPI_REC_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_recst_ystride    : 16  ; /* [15..0]  */
        unsigned int    vcpi_recst_cstride    : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REC_STRIDE;

/* Define the union U_VEDU_VCPI_REC_HEADER_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_recst_yhstride   : 16  ; /* [15..0]  */
        unsigned int    vcpi_recst_chstride   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REC_HEADER_STRIDE;

/* Define the union U_VEDU_VCPI_REF_L0_HEADER_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_l0_hstride  : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_l0_hstride  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_L0_HEADER_STRIDE;

/* Define the union U_VEDU_ME_ET_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_early_termi_thr   : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ET_THR;

/* Define the union U_VEDU_ME_ET_THR_ME_STAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_mestat_output_thr : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ET_THR_ME_STAT;

/* Define the union U_VEDU_IME_REF_BUF_WORD_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_refbuf_ver_words_num : 8   ; /* [7..0]  */
        unsigned int    ime_refbuf_hor_words_num : 5   ; /* [12..8]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_REF_BUF_WORD_NUM;

/* Define the union U_VEDU_IME_INTERPOLATION_FLAG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_interpolation_flag : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_IME_INTERPOLATION_FLAG;

/* Define the union U_VEDU_ME_MAX_REGION */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_max_region        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_MAX_REGION;

/* Define the union U_VEDU_ME_ADD1_RANDOM_POINT_LIST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add1_random_point0 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD1_RANDOM_POINT_LIST;

/* Define the union U_VEDU_ME_ADD2_RANDOM_POINT_LIST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add2_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add2_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD2_RANDOM_POINT_LIST;

/* Define the union U_VEDU_ME_ADD3_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add3_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add3_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD3_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD3_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add3_random_point2 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD3_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD4_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add4_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add4_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD4_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD4_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add4_random_point2 : 16  ; /* [15..0]  */
        unsigned int    ime_add4_random_point3 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD4_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD5_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add5_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add5_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD5_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD5_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add5_random_point2 : 16  ; /* [15..0]  */
        unsigned int    ime_add5_random_point3 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD5_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD5_RANDOM_POINT_LIST2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add5_random_point4 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD5_RANDOM_POINT_LIST2;

/* Define the union U_VEDU_ME_ADD6_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add6_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add6_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD6_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD6_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add6_random_point2 : 16  ; /* [15..0]  */
        unsigned int    ime_add6_random_point3 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD6_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD6_RANDOM_POINT_LIST2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add6_random_point4 : 16  ; /* [15..0]  */
        unsigned int    ime_add6_random_point5 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD6_RANDOM_POINT_LIST2;

/* Define the union U_VEDU_ME_ADD7_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add7_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add7_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD7_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD7_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add7_random_point2 : 16  ; /* [15..0]  */
        unsigned int    ime_add7_random_point3 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD7_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD7_RANDOM_POINT_LIST2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add7_random_point4 : 16  ; /* [15..0]  */
        unsigned int    ime_add7_random_point5 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD7_RANDOM_POINT_LIST2;

/* Define the union U_VEDU_ME_ADD7_RANDOM_POINT_LIST3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add7_random_point6 : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD7_RANDOM_POINT_LIST3;

/* Define the union U_VEDU_ME_ADD8_RANDOM_POINT_LIST0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add8_random_point0 : 16  ; /* [15..0]  */
        unsigned int    ime_add8_random_point1 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD8_RANDOM_POINT_LIST0;

/* Define the union U_VEDU_ME_ADD8_RANDOM_POINT_LIST1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add8_random_point2 : 16  ; /* [15..0]  */
        unsigned int    ime_add8_random_point3 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD8_RANDOM_POINT_LIST1;

/* Define the union U_VEDU_ME_ADD8_RANDOM_POINT_LIST2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add8_random_point4 : 16  ; /* [15..0]  */
        unsigned int    ime_add8_random_point5 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD8_RANDOM_POINT_LIST2;

/* Define the union U_VEDU_ME_ADD8_RANDOM_POINT_LIST3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ime_add8_random_point6 : 16  ; /* [15..0]  */
        unsigned int    ime_add8_random_point7 : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_ADD8_RANDOM_POINT_LIST3;

/* Define the union U_VEDU_ME_TMV_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ime_temporal_en  : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_ME_TMV_EN;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp1 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp0 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT0;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp3 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp2 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT1;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp5 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp4 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT2;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp7 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp6 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT3;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp9 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp8 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT4;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp11 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp10 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT5;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp13 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp12 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT6;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp15 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp14 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT7;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp17 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp16 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT8;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT9 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp19 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp18 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT9;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp21 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp20 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT10;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp23 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp22 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT11;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp25 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp24 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT12;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT13 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp27 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp26 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT13;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT14 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp29 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp28 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT14;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT15 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp31 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp30 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT15;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT16 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp33 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp32 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT16;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT17 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp35 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp34 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT17;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT18 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp37 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp36 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT18;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT19 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp39 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp38 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT19;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp41 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp40 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT20;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp43 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp42 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT21;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp45 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp44 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT22;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT23 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp47 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp46 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT23;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT24 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp49 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp48 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT24;

/* Define the union U_VEDU_VCPI_SRDO_MIN_WEIGHT25 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_srdo_min_weight_qp51 : 16  ; /* [15..0] */
        unsigned int    vcpi_srdo_min_weight_qp50 : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_VCPI_SRDO_MIN_WEIGHT25;

/* Define the union U_VEDU_GHDR_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_en               : 1   ; /* [0]  */
        unsigned int    ghdr_ck_gt_en         : 1   ; /* [1]  */
        unsigned int    reserved_0            : 10  ; /* [11..2]  */
        unsigned int    ghdr_demo_en          : 1   ; /* [12]  */
        unsigned int    ghdr_demo_mode        : 1   ; /* [13]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    ghdr_demo_pos         : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_CTRL;

/* Define the union U_VEDU_GHDR_DEGAMMA_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_degmm_en         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DEGAMMA_CTRL;

/* Define the union U_VEDU_GHDR_DEGAMMA_STEP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_degmm_x1_step       : 4   ; /* [3..0]  */
        unsigned int    g_degmm_x2_step       : 4   ; /* [7..4]  */
        unsigned int    g_degmm_x3_step       : 4   ; /* [11..8]  */
        unsigned int    g_degmm_x4_step       : 4   ; /* [15..12]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DEGAMMA_STEP;

/* Define the union U_VEDU_GHDR_DEGAMMA_POS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_degmm_x1_pos        : 10  ; /* [9..0]  */
        unsigned int    g_degmm_x2_pos        : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DEGAMMA_POS1;

/* Define the union U_VEDU_GHDR_DEGAMMA_POS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_degmm_x3_pos        : 10  ; /* [9..0]  */
        unsigned int    g_degmm_x4_pos        : 10  ; /* [19..10]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DEGAMMA_POS2;

/* Define the union U_VEDU_GHDR_DEGAMMA_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_degmm_x1_num        : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    g_degmm_x2_num        : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    g_degmm_x3_num        : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    g_degmm_x4_num        : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DEGAMMA_NUM;

/* Define the union U_VEDU_GHDR_GAMUT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_en         : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_CTRL;

/* Define the union U_VEDU_GHDR_GAMUT_COEF00 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef00     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF00;

/* Define the union U_VEDU_GHDR_GAMUT_COEF01 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef01     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF01;

/* Define the union U_VEDU_GHDR_GAMUT_COEF02 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef02     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF02;

/* Define the union U_VEDU_GHDR_GAMUT_COEF10 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef10     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF10;

/* Define the union U_VEDU_GHDR_GAMUT_COEF11 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef11     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF11;

/* Define the union U_VEDU_GHDR_GAMUT_COEF12 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef12     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF12;

/* Define the union U_VEDU_GHDR_GAMUT_COEF20 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef20     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF20;

/* Define the union U_VEDU_GHDR_GAMUT_COEF21 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef21     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF21;

/* Define the union U_VEDU_GHDR_GAMUT_COEF22 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_coef22     : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_COEF22;

/* Define the union U_VEDU_GHDR_GAMUT_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_scale      : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_SCALE;

/* Define the union U_VEDU_GHDR_GAMUT_CLIP_MIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_clip_min   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_CLIP_MIN;

/* Define the union U_VEDU_GHDR_GAMUT_CLIP_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamut_clip_max   : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMUT_CLIP_MAX;

/* Define the union U_VEDU_GHDR_TONEMAP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_en          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_CTRL;

/* Define the union U_VEDU_GHDR_TONEMAP_REN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_rd_en       : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_REN;

/* Define the union U_VEDU_GHDR_TONEMAP_STEP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x1_step        : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    g_tmap_x2_step        : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    g_tmap_x3_step        : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    g_tmap_x4_step        : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_STEP;

/* Define the union U_VEDU_GHDR_TONEMAP_POS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x1_pos         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_POS1;

/* Define the union U_VEDU_GHDR_TONEMAP_POS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x2_pos         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_POS2;

/* Define the union U_VEDU_GHDR_TONEMAP_POS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x3_pos         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_POS3;

/* Define the union U_VEDU_GHDR_TONEMAP_POS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x4_pos         : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_POS4;

/* Define the union U_VEDU_GHDR_TONEMAP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_tmap_x1_num         : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 3   ; /* [7..5]  */
        unsigned int    g_tmap_x2_num         : 5   ; /* [12..8]  */
        unsigned int    reserved_1            : 3   ; /* [15..13]  */
        unsigned int    g_tmap_x3_num         : 5   ; /* [20..16]  */
        unsigned int    reserved_2            : 3   ; /* [23..21]  */
        unsigned int    g_tmap_x4_num         : 5   ; /* [28..24]  */
        unsigned int    reserved_3            : 3   ; /* [31..29]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_NUM;

/* Define the union U_VEDU_GHDR_TONEMAP_LUMA_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_luma_coef0  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_LUMA_COEF0;

/* Define the union U_VEDU_GHDR_TONEMAP_LUMA_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_luma_coef1  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_LUMA_COEF1;

/* Define the union U_VEDU_GHDR_TONEMAP_LUMA_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_luma_coef2  : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_LUMA_COEF2;

/* Define the union U_VEDU_GHDR_TONEMAP_LUMA_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_luma_scale  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_LUMA_SCALE;

/* Define the union U_VEDU_GHDR_TONEMAP_COEF_SCALE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_coef_scale  : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_COEF_SCALE;

/* Define the union U_VEDU_GHDR_TONEMAP_OUT_CLIP_MIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_out_clip_min : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_OUT_CLIP_MIN;

/* Define the union U_VEDU_GHDR_TONEMAP_OUT_CLIP_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_tmap_out_clip_max : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_TONEMAP_OUT_CLIP_MAX;

/* Define the union U_VEDU_GHDR_GAMMA_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gmm_en           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_CTRL;

/* Define the union U_VEDU_GHDR_GAMMA_REN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_gamma_rd_en      : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_REN;

/* Define the union U_VEDU_GHDR_GAMMA_STEP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x1_step         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    g_gmm_x2_step         : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    g_gmm_x3_step         : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    g_gmm_x4_step         : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_STEP1;

/* Define the union U_VEDU_GHDR_GAMMA_STEP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x5_step         : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 4   ; /* [7..4]  */
        unsigned int    g_gmm_x6_step         : 4   ; /* [11..8]  */
        unsigned int    reserved_1            : 4   ; /* [15..12]  */
        unsigned int    g_gmm_x7_step         : 4   ; /* [19..16]  */
        unsigned int    reserved_2            : 4   ; /* [23..20]  */
        unsigned int    g_gmm_x8_step         : 4   ; /* [27..24]  */
        unsigned int    reserved_3            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_STEP2;

/* Define the union U_VEDU_GHDR_GAMMA_POS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x1_pos          : 16  ; /* [15..0]  */
        unsigned int    g_gmm_x2_pos          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_POS1;

/* Define the union U_VEDU_GHDR_GAMMA_POS2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x3_pos          : 16  ; /* [15..0]  */
        unsigned int    g_gmm_x4_pos          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_POS2;

/* Define the union U_VEDU_GHDR_GAMMA_POS3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x5_pos          : 16  ; /* [15..0]  */
        unsigned int    g_gmm_x6_pos          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_POS3;

/* Define the union U_VEDU_GHDR_GAMMA_POS4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x7_pos          : 16  ; /* [15..0]  */
        unsigned int    g_gmm_x8_pos          : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_POS4;

/* Define the union U_VEDU_GHDR_GAMMA_NUM1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x1_num          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    g_gmm_x2_num          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    g_gmm_x3_num          : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    g_gmm_x4_num          : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_NUM1;

/* Define the union U_VEDU_GHDR_GAMMA_NUM2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    g_gmm_x5_num          : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 2   ; /* [7..6]  */
        unsigned int    g_gmm_x6_num          : 6   ; /* [13..8]  */
        unsigned int    reserved_1            : 2   ; /* [15..14]  */
        unsigned int    g_gmm_x7_num          : 6   ; /* [21..16]  */
        unsigned int    reserved_2            : 2   ; /* [23..22]  */
        unsigned int    g_gmm_x8_num          : 6   ; /* [29..24]  */
        unsigned int    reserved_3            : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_GAMMA_NUM2;

/* Define the union U_VEDU_GHDR_DITHER_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_tap_mode  : 2   ; /* [1..0]  */
        unsigned int    ghdr_dither_domain_mode : 1   ; /* [2]  */
        unsigned int    ghdr_dither_round     : 1   ; /* [3]  */
        unsigned int    ghdr_dither_mode      : 1   ; /* [4]  */
        unsigned int    ghdr_dither_en        : 1   ; /* [5]  */
        unsigned int    ghdr_dither_round_unlim : 1   ; /* [6]  */
        unsigned int    reserved_0            : 25  ; /* [31..7]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_CTRL;

/* Define the union U_VEDU_GHDR_DITHER_THR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_thr_min   : 16  ; /* [15..0]  */
        unsigned int    ghdr_dither_thr_max   : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_THR;

/* Define the union U_VEDU_GHDR_DITHER_SED_Y0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_y0    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_Y0;

/* Define the union U_VEDU_GHDR_DITHER_SED_U0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_u0    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_U0;

/* Define the union U_VEDU_GHDR_DITHER_SED_V0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_v0    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_V0;

/* Define the union U_VEDU_GHDR_DITHER_SED_W0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_w0    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_W0;

/* Define the union U_VEDU_GHDR_DITHER_SED_Y1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_y1    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_Y1;

/* Define the union U_VEDU_GHDR_DITHER_SED_U1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_u1    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_U1;

/* Define the union U_VEDU_GHDR_DITHER_SED_V1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_v1    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_V1;

/* Define the union U_VEDU_GHDR_DITHER_SED_W1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ghdr_dither_sed_w1    : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_GHDR_DITHER_SED_W1;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_en          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_CTRL;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef00      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF0;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef01      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF1;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef02      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF2;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef10      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF3;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef11      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF4;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF5 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef12      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF5;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF6 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef20      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF6;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF7 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef21      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF7;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_COEF8 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_coef22      : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_COEF8;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_SCALE2P */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_scale2p     : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_SCALE2P;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_IDC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_idc0        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_IDC0;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_IDC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_idc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_IDC1;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_IDC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_idc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_IDC2;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_ODC0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_odc0        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_ODC0;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_ODC1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_odc1        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_ODC1;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_ODC2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_odc2        : 11  ; /* [10..0]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_ODC2;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_MIN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_clip_min    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_MIN;

/* Define the union U_VEDU_HIHDR_G_RGB2YUV_MAX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hihdr_r2y_clip_max    : 10  ; /* [9..0]  */
        unsigned int    reserved_0            : 22  ; /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIHDR_G_RGB2YUV_MAX;

/* Define the union U_VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_downboundary_stride : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_downboundary_stride : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE;

/* Define the union U_VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refch_downboundary_stride : 16  ; /* [15..0]  */
        unsigned int    vcpi_refyh_downboundary_stride : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG80 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda16     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG80;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG81 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda17     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG81;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG82 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda18     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG82;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG83 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda19     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG83;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG84 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda20     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG84;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG85 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda21     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG85;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG86 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda22     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG86;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG87 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda23     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG87;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG88 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda24     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG88;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG89 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda25     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG89;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG90 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda26     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG90;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG91 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda27     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG91;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG92 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda28     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG92;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG93 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda29     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG93;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG94 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda30     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG94;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG95 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda31     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG95;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG96 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda32     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG96;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG97 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda33     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG97;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG98 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda34     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG98;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG99 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda35     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG99;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG100 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda36     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG100;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG101 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda37     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG101;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG102 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda38     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG102;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG103 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda39     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG103;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG104 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda40     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG104;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG105 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda41     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG105;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG106 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda42     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG106;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG107 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda43     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG107;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG108 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda44     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG108;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG109 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda45     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG109;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG110 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda46     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG110;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG111 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda47     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG111;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG112 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda48     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG112;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG113 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda49     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG113;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG114 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda50     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG114;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG115 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda51     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG115;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG116 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda52     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG116;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG117 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda53     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG117;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG118 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda54     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG118;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG119 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda55     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG119;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG120 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda56     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG120;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG121 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda57     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG121;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG122 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda58     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG122;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG123 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda59     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG123;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG124 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda60     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG124;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG125 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda61     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG125;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG126 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda62     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG126;

/* Define the union U_VEDU_QPG_QP_LAMBDA_CTRL_REG127 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    qpg_sqrt_lambda63     : 20  ; /* [19..0]  */
        unsigned int    reserved_0            : 12  ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_QPG_QP_LAMBDA_CTRL_REG127;

/* Define the union U_VEDU_VCPI_LUMA_START_ROWNUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_luma_start_rownum : 16  ; /* [15..0]  */
        unsigned int    vcpi_chroma_start_rownum : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_LUMA_START_ROWNUM;

/* Define the union U_VEDU_VCPI_CURLD_START_SLICENUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_curld_start_slicenum : 16  ; /* [15..0]  */
        unsigned int    vcpi_curld_inquire_mod : 2   ; /* [17..16]  */
        unsigned int    vcpi_curld_hts_en     : 1   ; /* [18] */
        unsigned int    reserved_0            : 13  ; /* [31..19] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CURLD_START_SLICENUM;


/* Define the union U_VEDU_VCPI_REFLD_BOUNDARY_MOD */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_ref_boundary_mod : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REFLD_BOUNDARY_MOD;

/* Define the union U_VEDU_VCPI_REF_UPBOUNDARY_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refc_upboundary_stride : 16  ; /* [15..0]  */
        unsigned int    vcpi_refy_upboundary_stride : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_UPBOUNDARY_STRIDE;

/* Define the union U_VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_refch_upboundary_stride : 16  ; /* [15..0]  */
        unsigned int    vcpi_refyh_upboundary_stride : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE;

/* Define the union U_VEDU_VCPI_PRIVATE_SLCHDR_PART0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_private_slchdr_part0 : 1   ; /* [0]  */
        unsigned int    vcpi_private_slcadd_size0 : 5   ; /* [5..1]  */
        unsigned int    vcpi_private_slcadd_size1 : 5   ; /* [10..6]  */
        unsigned int    reserved_0            : 21  ; /* [31..11]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PRIVATE_SLCHDR_PART0;

/* Define the union U_VEDU_VCPI_LOWLATENCY_SLICE_INTEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_lowlatency_slice_inten : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_LOWLATENCY_SLICE_INTEN;

/* Define the union U_VEDU_VCTRL_INTRAPU4_REFRESH_START */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intrapu4_refresh_startx : 12  ; /* [11..0]  */
        unsigned int    vcpi_intrapu4_refresh_starty : 12  ; /* [23..12]  */
        unsigned int    vcpi_intrapu4_refresh_en : 1   ; /* [24]  */
        unsigned int    reserved_0            : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_INTRAPU4_REFRESH_START;

/* Define the union U_VEDU_VCTRL_INTRAPU4_REFRESH_END */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_intrapu4_refresh_endx : 12  ; /* [11..0]  */
        unsigned int    vcpi_intrapu4_refresh_endy : 12  ; /* [23..12]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCTRL_INTRAPU4_REFRESH_END;

/* Define the union U_VEDU_VCPI_CMDLST_GRP_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cmdlst_grp_num   : 16  ; /* [15..0]  */
        unsigned int    vcpi_cmdlst_grp_start : 1   ; /* [16]  */
        unsigned int    vcpi_cmdlst_wait_en   : 1   ; /* [17]  */
        unsigned int    reserved_0            : 14  ; /* [31..18]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CMDLST_GRP_NUM;

/* Define the union U_VEDU_VCPI_CMDLST_SOFT_FINISH_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_cmdlst_soft_finish_num : 16  ; /* [15..0]  */
        unsigned int    reserved_0            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_CMDLST_SOFT_FINISH_NUM;

/* Define the union U_VEDU_VCPI_FORCE_ZEROMV */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_force_zeromv     : 1   ; /* [0]  */
        unsigned int    vcpi_vlcst_payload_len : 11  ; /* [11..1]  */
        unsigned int    vcpi_vlcst_packet_en  : 1   ; /* [12]  */
        unsigned int    vcpi_private_slchdr_en : 1   ; /* [13]  */
        unsigned int    vcpi_search_window_height_mode : 2   ; /* [15..14] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_FORCE_ZEROMV;

/* Define the union U_VEDU_VCPI_FORCE_INTRAPU32_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_force_intrapu4_num : 4   ; /* [3..0]  */
        unsigned int    vcpi_force_intrapu8_num : 4   ; /* [7..4]  */
        unsigned int    vcpi_force_intrapu16_num : 4   ; /* [11..8]  */
        unsigned int    vcpi_force_intrapu32_num : 4   ; /* [15..12]  */
        unsigned int    vcpi_force_intrapu_num_en : 1   ; /* [16]  */
        unsigned int    reserved_0            : 15  ; /* [31..17]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_FORCE_INTRAPU32_NUM;

/* Define the union U_VEDU_VCPI_VLCST_RAH_SAH */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vlcst_sah        : 16  ; /* [15..0]  */
        unsigned int    vcpi_vlcst_rah        : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_VLCST_RAH_SAH;

/* Define the union U_VEDU_VCPI_PACKET_INFO */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    vcpi_vlcst_type       : 16  ; /* [15..0] */
        unsigned int    vcpi_vlcst_qos        : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_VCPI_PACKET_INFO;

/* Define the union U_VEDU_SECURE_SET */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    venc_top_secure_n     : 1   ; /* [0]  */
        unsigned int    mcu_secure_n          : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_SECURE_SET;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VEDU_VCPI_INTMASK    VEDU_VCPI_INTMASK;
    volatile U_VEDU_VCPI_INTCLR     VEDU_VCPI_INTCLR;
    volatile U_VEDU_VCPI_START      VEDU_VCPI_START;
    volatile U_VEDU_BUS_IDLE_REQ    VEDU_BUS_IDLE_REQ;
    volatile unsigned int           VEDU_VCPI_FRAMENO;
    volatile unsigned int           VEDU_VCPI_TIMEOUT;
    volatile U_VEDU_VCPI_MODE       VEDU_VCPI_MODE;
    volatile U_VEDU_VCPI_SOFTINTSET VEDU_VCPI_SOFTINTSET;
    volatile U_VEDU_VCPI_CMDLST_CLKGATE VEDU_VCPI_CMDLST_CLKGATE;
    volatile U_VEDU_VCPI_CMD_DPM_SOFTRST VEDU_VCPI_CMD_DPM_SOFTRST;
    volatile U_VEDU_ISPLD_CFG       VEDU_ISPLD_CFG                   ; /* 0x28 */
    volatile unsigned int           VEDU_RESERVED_04;
    volatile U_VEDU_EMAR_SCRAMBLE_TYPE VEDU_EMAR_SCRAMBLE_TYPE;
    volatile unsigned int           VEDU_VCPI_LLILD_ADDR_L;
    volatile unsigned int           VEDU_VCPI_LLILD_ADDR_H;
    volatile U_VEDU_VCPI_RC_ENABLE  VEDU_VCPI_RC_ENABLE;
    volatile U_VEDU_VLCST_PTBITS_EN VEDU_VLCST_PTBITS_EN;
    volatile unsigned int           VEDU_VLCST_PTBITS;
    volatile U_VEDU_PPFD_ST_CFG     VEDU_PPFD_ST_CFG;
    volatile U_VEDU_VCPI_QPCFG      VEDU_VCPI_QPCFG;
    volatile U_VEDU_QPG_MAX_MIN_QP  VEDU_QPG_MAX_MIN_QP;
    volatile U_VEDU_QPG_SMART_REG   VEDU_QPG_SMART_REG;
    volatile U_VEDU_QPG_ROW_TARGET_BITS VEDU_QPG_ROW_TARGET_BITS;
    volatile U_VEDU_QPG_AVERAGE_LCU_BITS VEDU_QPG_AVERAGE_LCU_BITS;
    volatile U_VEDU_QPG_CU_QP_DELTA_THRESH_REG0 VEDU_QPG_CU_QP_DELTA_THRESH_REG0;
    volatile U_VEDU_QPG_CU_QP_DELTA_THRESH_REG1 VEDU_QPG_CU_QP_DELTA_THRESH_REG1;
    volatile U_VEDU_QPG_CU_QP_DELTA_THRESH_REG2 VEDU_QPG_CU_QP_DELTA_THRESH_REG2;
    volatile U_VEDU_QPG_CU_QP_DELTA_THRESH_REG3 VEDU_QPG_CU_QP_DELTA_THRESH_REG3;
    volatile U_VEDU_QPG_DELTA_LEVEL VEDU_QPG_DELTA_LEVEL;
    volatile U_VEDU_QPG_MADI_SWITCH_THR VEDU_QPG_MADI_SWITCH_THR;
    volatile U_VEDU_QPG_CURR_SAD_EN VEDU_QPG_CURR_SAD_EN;
    volatile U_VEDU_QPG_CURR_SAD_LEVEL VEDU_QPG_CURR_SAD_LEVEL;
    volatile U_VEDU_QPG_CURR_SAD_THRESH0 VEDU_QPG_CURR_SAD_THRESH0;
    volatile U_VEDU_QPG_CURR_SAD_THRESH1 VEDU_QPG_CURR_SAD_THRESH1;
    volatile U_VEDU_QPG_CURR_SAD_THRESH2 VEDU_QPG_CURR_SAD_THRESH2;
    volatile U_VEDU_QPG_CURR_SAD_THRESH3 VEDU_QPG_CURR_SAD_THRESH3;
    volatile U_VEDU_LUMA_RC         VEDU_LUMA_RC;
    volatile U_VEDU_LUMA_LEVEL      VEDU_LUMA_LEVEL;
    volatile U_VEDU_LUMA_THRESH0    VEDU_LUMA_THRESH0;
    volatile U_VEDU_LUMA_THRESH1    VEDU_LUMA_THRESH1;
    volatile U_VEDU_LUMA_THRESH2    VEDU_LUMA_THRESH2;
    volatile U_VEDU_LUMA_THRESH3    VEDU_LUMA_THRESH3;
    volatile U_VEDU_VCPI_BG_ENABLE  VEDU_VCPI_BG_ENABLE;
    volatile U_VEDU_VCPI_BG_FLT_PARA0 VEDU_VCPI_BG_FLT_PARA0;
    volatile U_VEDU_VCPI_BG_FLT_PARA1 VEDU_VCPI_BG_FLT_PARA1;
    volatile U_VEDU_VCPI_BG_FLT_PARA2 VEDU_VCPI_BG_FLT_PARA2;
    volatile U_VEDU_VCPI_BG_THR0    VEDU_VCPI_BG_THR0;
    volatile U_VEDU_VCPI_BG_THR1    VEDU_VCPI_BG_THR1;
    volatile U_VEDU_VCPI_BG_STRIDE  VEDU_VCPI_BG_STRIDE;
    volatile U_VEDU_VCPI_BG_EXT_STRIDE VEDU_VCPI_BG_EXT_STRIDE;
    volatile unsigned int           VEDU_VCPI_BGL_ADDR_L;
    volatile unsigned int           VEDU_VCPI_BGL_ADDR_H;
    volatile unsigned int           VEDU_VCPI_BGC_ADDR_L;
    volatile unsigned int           VEDU_VCPI_BGC_ADDR_H;
    volatile unsigned int           VEDU_VCPI_BGL_EXT_ADDR_L;
    volatile unsigned int           VEDU_VCPI_BGL_EXT_ADDR_H;
    volatile unsigned int           VEDU_VCPI_BGC_EXT_ADDR_L;
    volatile unsigned int           VEDU_VCPI_BGC_EXT_ADDR_H;
    volatile unsigned int           VEDU_VCPI_BGINF_ADDR_L;
    volatile unsigned int           VEDU_VCPI_BGINF_ADDR_H;
    volatile U_VEDU_VCPI_OUTSTD     VEDU_VCPI_OUTSTD;
    volatile U_VEDU_VCTRL_LCU_BASELINE VEDU_VCTRL_LCU_BASELINE;
    volatile unsigned int           VEDU_VCPI_SPMEM_CTRL_0;
    volatile U_VEDU_VCPI_SPMEM_CTRL_1 VEDU_VCPI_SPMEM_CTRL_1;
    volatile U_VEDU_VCPI_SPMEM_CTRL_2 VEDU_VCPI_SPMEM_CTRL_2;
    volatile unsigned int           VEDU_VCPI_TPMEM_CTRL_0;
    volatile unsigned int           VEDU_VCPI_TPMEM_CTRL_1;
    volatile unsigned int           VEDU_ENHANCED_LAYER_RESERVED[52];
    volatile U_VEDU_VCTRL_GCFG      VEDU_VCTRL_GCFG                  ; /* 0x1dc */
    volatile U_VEDU_VCTRL_GRAN      VEDU_VCTRL_GRAN                  ; /* 0x1e0 */
    volatile U_VEDU_VCTRL_THRESHOLD_L   VEDU_VCTRL_THRESHOLD_L           ; /* 0x1e4 */
    volatile U_VEDU_VCTRL_THRESHOLD_H   VEDU_VCTRL_THRESHOLD_H           ; /* 0x1e8 */
    volatile U_VEDU_VCTRL_INTRAPU4_OFFSET   VEDU_VCTRL_INTRAPU4_OFFSET       ; /* 0x1ec */
    volatile U_VEDU_CURLD_DEPTHCODING   VEDU_CURLD_DEPTHCODING           ; /* 0x1f0 */
    volatile U_VEDU_CURLD_WATERMARK_CFG   VEDU_CURLD_WATERMARK_CFG         ; /* 0x1f4 */
    volatile U_VEDU_CURLD_WATERMARK_SIZE   VEDU_CURLD_WATERMARK_SIZE        ; /* 0x1f8 */
    volatile unsigned int           VEDU_RESERVED_06              ; /* 0x1fc */
    volatile U_VEDU_CURLD_GCFG      VEDU_CURLD_GCFG;
    volatile U_VEDU_VCPI_OSD_ENABLE VEDU_VCPI_OSD_ENABLE;
    volatile U_VEDU_VCPI_STRFMT     VEDU_VCPI_STRFMT;
    volatile U_VEDU_VCPI_INTRA_INTER_CU_EN VEDU_VCPI_INTRA_INTER_CU_EN;
    volatile U_VEDU_VCPI_CROSS_TILE_SLC VEDU_VCPI_CROSS_TILE_SLC;
    volatile U_VEDU_VCPI_MULTISLC   VEDU_VCPI_MULTISLC;
    volatile U_VEDU_VCTRL_LCU_TARGET_BIT VEDU_VCTRL_LCU_TARGET_BIT;
    volatile U_VEDU_VCPI_SW_L0_SIZE VEDU_VCPI_SW_L0_SIZE;
    volatile U_VEDU_VCPI_SW_L1_SIZE VEDU_VCPI_SW_L1_SIZE;
    volatile U_VEDU_VCPI_I_SLC_INSERT VEDU_VCPI_I_SLC_INSERT;
    volatile U_VEDU_PME_SAFE_CFG    VEDU_PME_SAFE_CFG;
    volatile U_VEDU_PME_IBLK_REFRESH VEDU_PME_IBLK_REFRESH;
    volatile U_VEDU_PME_IBLK_REFRESH_NUM VEDU_PME_IBLK_REFRESH_NUM;
    volatile U_VEDU_INTRA_CHNL4_ANG_0EN VEDU_INTRA_CHNL4_ANG_0EN;
    volatile U_VEDU_INTRA_CHNL4_ANG_1EN VEDU_INTRA_CHNL4_ANG_1EN;
    volatile U_VEDU_INTRA_CHNL8_ANG_0EN VEDU_INTRA_CHNL8_ANG_0EN;
    volatile U_VEDU_INTRA_CHNL8_ANG_1EN VEDU_INTRA_CHNL8_ANG_1EN;
    volatile U_VEDU_INTRA_CHNL16_ANG_0EN VEDU_INTRA_CHNL16_ANG_0EN;
    volatile U_VEDU_INTRA_CHNL16_ANG_1EN VEDU_INTRA_CHNL16_ANG_1EN;
    volatile U_VEDU_INTRA_CHNL32_ANG_0EN VEDU_INTRA_CHNL32_ANG_0EN;
    volatile U_VEDU_INTRA_CHNL32_ANG_1EN VEDU_INTRA_CHNL32_ANG_1EN;
    volatile U_VEDU_PACK_CU_PARAMETER VEDU_PACK_CU_PARAMETER;
    volatile U_VEDU_PACK_PCM_PARAMETER VEDU_PACK_PCM_PARAMETER;
    volatile U_VEDU_QPG_READLINE_INTERVAL VEDU_QPG_READLINE_INTERVAL;
    volatile U_VEDU_PMV_READLINE_INTERVAL VEDU_PMV_READLINE_INTERVAL;
    volatile U_VEDU_RGB2YUV_COEF_P0 VEDU_RGB2YUV_COEF_P0;
    volatile U_VEDU_RGB2YUV_COEF_P1 VEDU_RGB2YUV_COEF_P1;
    volatile U_VEDU_RGB2YUV_COEF_P2 VEDU_RGB2YUV_COEF_P2;
    volatile U_VEDU_RGB2YUV_OFFSET  VEDU_RGB2YUV_OFFSET;
    volatile U_VEDU_CURLD_CLIP_THR_Y   VEDU_CURLD_CLIP_THR_Y            ; /* 0x274 */
    volatile U_VEDU_CURLD_CLIP_THR_C   VEDU_CURLD_CLIP_THR_C            ; /* 0x278 */
    volatile U_VEDU_RGB2YUV_SHIFT_WIDTH VEDU_RGB2YUV_SHIFT_WIDTH;
    volatile U_VEDU_RGB2YUV_COEF_P3   VEDU_RGB2YUV_COEF_P3             ; /* 0x280 */
    volatile U_VEDU_RGB2YUV_COEF_P4   VEDU_RGB2YUV_COEF_P4             ; /* 0x284 */
    volatile unsigned int           VEDU_CURLD_HEBC_CTRL             ; /* 0x288 */
    volatile unsigned int           VEDU_RECST_HEBC_CTRL             ; /* 0x28c */
    volatile U_VEDU_CURLD_NARROW_CHRM   VEDU_CURLD_NARROW_CHRM           ; /* 0x290 */
    volatile U_VEDU_CURLD_NARROW_LUMA   VEDU_CURLD_NARROW_LUMA           ; /* 0x294 */
    volatile unsigned int           VEDU_REFLD_DBG_CTRL              ; /* 0x298 */
    volatile unsigned int           VEDU_CREFLD_DBG_CTRL             ; /* 0x29c */
    volatile unsigned int           VEDU_CURLD_WATERMARK_ADDR_L      ; /* 0x2a0 */
    volatile unsigned int           VEDU_CURLD_WATERMARK_ADDR_H      ; /* 0x2a4 */
    volatile U_VEDU_CURLD_WATERMARK_STRIDE   VEDU_CURLD_WATERMARK_STRIDE      ; /* 0x2a8 */
    volatile unsigned int           VEDU_RESERVED_02AC               ; /* 0x2ac */
    volatile unsigned int           reserved_69[7]                   ; /* 0x2b0~0x2c8 */
    volatile U_VEDU_CURLD_RGB_CLIP   VEDU_CURLD_RGB_CLIP              ; /* 0x2cc */
    volatile unsigned int           VEDU_RESERVED_02D0               ; /* 0x2d0 */
    volatile unsigned int           reserved_70                      ; /* 0x2d4 */
    volatile U_VEDU_CURLD_SRCH_STRIDE   VEDU_CURLD_SRCH_STRIDE           ; /* 0x2d8 */
    volatile unsigned int           VEDU_RESERVED_07              ; /* 0x2dc */
    volatile unsigned int           reserved_71[28]                  ; /* 0x2e0~0x34c */
    volatile U_VEDU_NEW_CFG0        VEDU_NEW_CFG0;
    volatile unsigned int           VEDU_RESERVED_08[11];
    volatile U_VEDU_VCTRL_ROI_CFG0  VEDU_VCTRL_ROI_CFG0;
    volatile U_VEDU_VCTRL_ROI_CFG1  VEDU_VCTRL_ROI_CFG1;
    volatile U_VEDU_VCTRL_ROI_CFG2  VEDU_VCTRL_ROI_CFG2;
    volatile U_VEDU_VCTRL_ROI_SIZE_0 VEDU_VCTRL_ROI_SIZE_0;
    volatile U_VEDU_VCTRL_ROI_SIZE_1 VEDU_VCTRL_ROI_SIZE_1;
    volatile U_VEDU_VCTRL_ROI_SIZE_2 VEDU_VCTRL_ROI_SIZE_2;
    volatile U_VEDU_VCTRL_ROI_SIZE_3 VEDU_VCTRL_ROI_SIZE_3;
    volatile U_VEDU_VCTRL_ROI_SIZE_4 VEDU_VCTRL_ROI_SIZE_4;
    volatile U_VEDU_VCTRL_ROI_SIZE_5 VEDU_VCTRL_ROI_SIZE_5;
    volatile U_VEDU_VCTRL_ROI_SIZE_6 VEDU_VCTRL_ROI_SIZE_6;
    volatile U_VEDU_VCTRL_ROI_SIZE_7 VEDU_VCTRL_ROI_SIZE_7;
    volatile U_VEDU_VCTRL_ROI_START_0 VEDU_VCTRL_ROI_START_0;
    volatile U_VEDU_VCTRL_ROI_START_1 VEDU_VCTRL_ROI_START_1;
    volatile U_VEDU_VCTRL_ROI_START_2 VEDU_VCTRL_ROI_START_2;
    volatile U_VEDU_VCTRL_ROI_START_3 VEDU_VCTRL_ROI_START_3;
    volatile U_VEDU_VCTRL_ROI_START_4 VEDU_VCTRL_ROI_START_4;
    volatile U_VEDU_VCTRL_ROI_START_5 VEDU_VCTRL_ROI_START_5;
    volatile U_VEDU_VCTRL_ROI_START_6 VEDU_VCTRL_ROI_START_6;
    volatile U_VEDU_VCTRL_ROI_START_7 VEDU_VCTRL_ROI_START_7;
    volatile unsigned int           VEDU_RESERVED_09[13];
    volatile U_VEDU_TILE_MODE       VEDU_TILE_MODE;
    volatile U_VEDU_VCPI_PICSIZE_PIX VEDU_VCPI_PICSIZE_PIX;
    volatile U_VEDU_TILE_POS        VEDU_TILE_POS;
    volatile U_VEDU_VCPI_TILE_SIZE  VEDU_VCPI_TILE_SIZE;
    volatile U_VEDU_VCPI_VLC_CONFIG VEDU_VCPI_VLC_CONFIG;
    volatile U_VEDU_VCPI_REF_FLAG   VEDU_VCPI_REF_FLAG;
    volatile U_VEDU_PMV_TMV_EN      VEDU_PMV_TMV_EN;
    volatile U_VEDU_VCPI_TMV_LOAD   VEDU_VCPI_TMV_LOAD;
    volatile unsigned int           VEDU_PMV_POC_0;
    volatile unsigned int           VEDU_PMV_POC_1;
    volatile unsigned int           VEDU_PMV_POC_2;
    volatile unsigned int           VEDU_PMV_POC_3;
    volatile unsigned int           VEDU_PMV_POC_4;
    volatile unsigned int           VEDU_PMV_POC_5;
    volatile U_VEDU_CABAC_GLB_CFG   VEDU_CABAC_GLB_CFG;
    volatile U_VEDU_CABAC_SLCHDR_SIZE VEDU_CABAC_SLCHDR_SIZE;
    volatile U_VEDU_CABAC_SLCHDR_PART1 VEDU_CABAC_SLCHDR_PART1;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG1;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG2;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG3;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG4;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG5;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG6;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG7;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG8;
    volatile U_VEDU_CABAC_SLCHDR_SIZE_I VEDU_CABAC_SLCHDR_SIZE_I;
    volatile U_VEDU_CABAC_SLCHDR_PART1_I VEDU_CABAC_SLCHDR_PART1_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG1_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG2_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG3_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG4_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG5_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG6_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG7_I;
    volatile unsigned int           VEDU_CABAC_SLCHDR_PART2_SEG8_I;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM0;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM1;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM2;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM3;
    volatile unsigned int           VEDU_VLC_REORDERSTRM0;
    volatile unsigned int           VEDU_VLC_REORDERSTRM1;
    volatile unsigned int           VEDU_VLC_MARKINGSTRM0;
    volatile unsigned int           VEDU_VLC_MARKINGSTRM1;
    volatile U_VEDU_VLC_SLCHDRPARA  VEDU_VLC_SLCHDRPARA;
    volatile U_VEDU_VLC_SVC         VEDU_VLC_SVC;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM0_I;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM1_I;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM2_I;
    volatile unsigned int           VEDU_VLC_SLCHDRSTRM3_I;
    volatile unsigned int           VEDU_VLC_REORDERSTRM0_I;
    volatile unsigned int           VEDU_VLC_REORDERSTRM1_I;
    volatile unsigned int           VEDU_VLC_MARKINGSTRM0_I;
    volatile unsigned int           VEDU_VLC_MARKINGSTRM1_I;
    volatile U_VEDU_VLC_SLCHDRPARA_I VEDU_VLC_SLCHDRPARA_I;
    volatile unsigned int           VEDU_VLCST_STRMBUFLEN;
    volatile unsigned int           VEDU_VLCST_SLC_CFG0;
    volatile unsigned int           VEDU_VLCST_SLC_CFG1;
    volatile unsigned int           VEDU_VLCST_SLC_CFG2;
    volatile unsigned int           VEDU_VLCST_SLC_CFG3;
    volatile unsigned int           VEDU_ICE_V2R1_SEG_256X1_DCMP_CTRL;
    volatile U_VEDU_REFLD_CMP_CLKGATE_EN   VEDU_REFLD_CMP_CLKGATE_EN        ; /* 0x4f0 */
    volatile unsigned int           VEDU_RESERVED_10              ; /* 0x4f4 */
    volatile unsigned int           reserved_75[2]                   ; /* 0x4f8~0x4fc */
    volatile U_VEDU_QPG_LOWLUMA     VEDU_QPG_LOWLUMA;
    volatile U_VEDU_QPG_HEDGE       VEDU_QPG_HEDGE;
    volatile U_VEDU_QPG_HEDGE_MOVE  VEDU_QPG_HEDGE_MOVE;
    volatile U_VEDU_QPG_SKIN        VEDU_QPG_SKIN;
    volatile U_VEDU_QPG_INTRA_DET   VEDU_QPG_INTRA_DET;
    volatile unsigned int           VEDU_QPG_H264_SMOOTH;
    volatile U_VEDU_QPG_CU32_DELTA  VEDU_QPG_CU32_DELTA;
    volatile U_VEDU_QPG_LAMBDA_MODE VEDU_QPG_LAMBDA_MODE;
    volatile U_VEDU_QPG_QP_RESTRAIN VEDU_QPG_QP_RESTRAIN;
    volatile unsigned int           VEDU_QPG_CU_MIN_SAD_THRESH_0;
    volatile U_VEDU_QPG_CU_MIN_SAD_REG VEDU_QPG_CU_MIN_SAD_REG;
    volatile U_VEDU_QPG_FLAT_REGION VEDU_QPG_FLAT_REGION;
    volatile U_VEDU_QPG_RES_COEF    VEDU_QPG_RES_COEF;
    volatile U_VEDU_TILE_RC         VEDU_TILE_RC;
    volatile U_VEDU_CHROMA_PROTECT  VEDU_CHROMA_PROTECT;
    volatile U_VEDU_PME_QPG_RC_THR0 VEDU_PME_QPG_RC_THR0;
    volatile U_VEDU_PME_QPG_RC_THR1 VEDU_PME_QPG_RC_THR1;
    volatile U_VEDU_PME_LOW_LUMA_THR VEDU_PME_LOW_LUMA_THR;
    volatile U_VEDU_PME_CHROMA_FLAT VEDU_PME_CHROMA_FLAT;
    volatile U_VEDU_PME_LUMA_FLAT   VEDU_PME_LUMA_FLAT;
    volatile U_VEDU_PME_MADI_FLAT   VEDU_PME_MADI_FLAT;
    volatile U_VEDU_VLCST_DESCRIPTOR VEDU_VLCST_DESCRIPTOR;
    volatile unsigned int           VEDU_PPFD_ST_LEN0;
    volatile U_VEDU_CURLD_CLIP_LUMA VEDU_CURLD_CLIP_LUMA;
    volatile U_VEDU_CURLD_CLIP_CHROMA VEDU_CURLD_CLIP_CHROMA;
    volatile U_VEDU_TQITQ_DEADZONE  VEDU_TQITQ_DEADZONE;
    volatile U_VEDU_VCPI_PME_PARAM  VEDU_VCPI_PME_PARAM;
    volatile U_VEDU_VCPI_PIC_STRONG_EN VEDU_VCPI_PIC_STRONG_EN;
    volatile U_VEDU_VCPI_PINTRA_THRESH0 VEDU_VCPI_PINTRA_THRESH0;
    volatile U_VEDU_VCPI_PINTRA_THRESH1 VEDU_VCPI_PINTRA_THRESH1;
    volatile U_VEDU_VCPI_PINTRA_THRESH2 VEDU_VCPI_PINTRA_THRESH2;
    volatile U_VEDU_VCPI_INTRA32_LOW_POWER VEDU_VCPI_INTRA32_LOW_POWER;
    volatile U_VEDU_VCPI_INTRA16_LOW_POWER VEDU_VCPI_INTRA16_LOW_POWER;
    volatile U_VEDU_VCPI_INTRA_REDUCE_RDO_NUM VEDU_VCPI_INTRA_REDUCE_RDO_NUM;
    volatile U_VEDU_VCPI_NOFORCEZERO VEDU_VCPI_NOFORCEZERO;
    volatile U_VEDU_PME_SKIP_LARGE_RES VEDU_PME_SKIP_LARGE_RES;
    volatile U_VEDU_PME_SKIN_SAD_THR VEDU_PME_SKIN_SAD_THR;
    volatile U_VEDU_VCTRL_NM_ACOFFSET_DENOISE VEDU_VCTRL_NM_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_NM_ENGTHR_DENOISE VEDU_VCTRL_NM_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_NM_TU8_DENOISE VEDU_VCTRL_NM_TU8_DENOISE;
    volatile U_VEDU_VCTRL_SK_ACOFFSET_DENOISE VEDU_VCTRL_SK_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_SK_ENGTHR_DENOISE VEDU_VCTRL_SK_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_SK_TU8_DENOISE VEDU_VCTRL_SK_TU8_DENOISE;
    volatile U_VEDU_VCTRL_ST_ACOFFSET_DENOISE VEDU_VCTRL_ST_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_ST_ENGTHR_DENOISE VEDU_VCTRL_ST_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_ST_TU8_DENOISE VEDU_VCTRL_ST_TU8_DENOISE;
    volatile U_VEDU_VCTRL_SE_ACOFFSET_DENOISE VEDU_VCTRL_SE_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_SE_ENGTHR_DENOISE VEDU_VCTRL_SE_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_SE_TU8_DENOISE VEDU_VCTRL_SE_TU8_DENOISE;
    volatile U_VEDU_VCTRL_SELM_ACOFFSET_DENOISE VEDU_VCTRL_SELM_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_SELM_ENGTHR_DENOISE VEDU_VCTRL_SELM_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_SELM_TU8_DENOISE VEDU_VCTRL_SELM_TU8_DENOISE;
    volatile U_VEDU_VCTRL_WS_ACOFFSET_DENOISE VEDU_VCTRL_WS_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_WS_ENGTHR_DENOISE VEDU_VCTRL_WS_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_WS_TU8_DENOISE VEDU_VCTRL_WS_TU8_DENOISE;
    volatile U_VEDU_VCTRL_SSSE_ACOFFSET_DENOISE VEDU_VCTRL_SSSE_ACOFFSET_DENOISE;
    volatile U_VEDU_VCTRL_SSSE_ENGTHR_DENOISE VEDU_VCTRL_SSSE_ENGTHR_DENOISE;
    volatile U_VEDU_VCTRL_SSSE_TU8_DENOISE VEDU_VCTRL_SSSE_TU8_DENOISE;
    volatile unsigned int           VEDU_RESERVED_11[6];
    volatile U_VEDU_VCTRL_INTRA_RDO_FACTOR_0 VEDU_VCTRL_INTRA_RDO_FACTOR_0;
    volatile U_VEDU_VCTRL_INTRA_RDO_FACTOR_1 VEDU_VCTRL_INTRA_RDO_FACTOR_1;
    volatile U_VEDU_VCTRL_INTRA_RDO_FACTOR_2 VEDU_VCTRL_INTRA_RDO_FACTOR_2;
    volatile U_VEDU_VCTRL_MRG_RDO_FACTOR_0 VEDU_VCTRL_MRG_RDO_FACTOR_0;
    volatile U_VEDU_VCTRL_MRG_RDO_FACTOR_1 VEDU_VCTRL_MRG_RDO_FACTOR_1;
    volatile U_VEDU_VCTRL_MRG_RDO_FACTOR_2 VEDU_VCTRL_MRG_RDO_FACTOR_2;
    volatile U_VEDU_VCTRL_FME_RDO_FACTOR_0 VEDU_VCTRL_FME_RDO_FACTOR_0;
    volatile U_VEDU_VCTRL_FME_RDO_FACTOR_1 VEDU_VCTRL_FME_RDO_FACTOR_1;
    volatile U_VEDU_VCTRL_FME_RDO_FACTOR_2 VEDU_VCTRL_FME_RDO_FACTOR_2;
    volatile U_VEDU_PME_NEW_COST    VEDU_PME_NEW_COST;
    volatile U_VEDU_PME_COST_OFFSET VEDU_PME_COST_OFFSET;
    volatile U_VEDU_PME_ADJUST_PMEMV_H264 VEDU_PME_ADJUST_PMEMV_H264;
    volatile U_VEDU_PME_INTRABLK_DET VEDU_PME_INTRABLK_DET;
    volatile U_VEDU_PME_INTRABLK_DET_THR VEDU_PME_INTRABLK_DET_THR;
    volatile U_VEDU_PME_SKIN_THR    VEDU_PME_SKIN_THR;
    volatile U_VEDU_PME_STRONG_EDGE VEDU_PME_STRONG_EDGE;
    volatile U_VEDU_PME_LARGE_MOVE_THR VEDU_PME_LARGE_MOVE_THR;
    volatile U_VEDU_PME_INTER_STRONG_EDGE VEDU_PME_INTER_STRONG_EDGE;
    volatile U_VEDU_PME_CHROMA_STRONG_EDGE VEDU_PME_CHROMA_STRONG_EDGE;
    volatile U_VEDU_CHROMA_SAD_THR  VEDU_CHROMA_SAD_THR;
    volatile U_VEDU_CHROMA_FG_THR   VEDU_CHROMA_FG_THR;
    volatile U_VEDU_CHROMA_BG_THR   VEDU_CHROMA_BG_THR;
    volatile U_VEDU_CHROMA_SUM_FG_THR VEDU_CHROMA_SUM_FG_THR;
    volatile U_VEDU_CHROMA_SUM_BG_THR VEDU_CHROMA_SUM_BG_THR;
    volatile U_VEDU_CHROMA_FG_COUNT_THR VEDU_CHROMA_FG_COUNT_THR;
    volatile U_VEDU_CHROMA_BG_COUNT_THR VEDU_CHROMA_BG_COUNT_THR;
    volatile U_VEDU_PME_MOVE_SCENE_THR VEDU_PME_MOVE_SCENE_THR;
    volatile U_VEDU_PME_NEW_MADI_TH VEDU_PME_NEW_MADI_TH;
    volatile U_VEDU_PME_NEW_LAMBDA  VEDU_PME_NEW_LAMBDA;
    volatile U_VEDU_VCPI_DBLKCFG    VEDU_VCPI_DBLKCFG;
    volatile U_VEDU_FME_BIAS_COST0  VEDU_FME_BIAS_COST0;
    volatile U_VEDU_FME_BIAS_COST1  VEDU_FME_BIAS_COST1;
    volatile U_VEDU_MRG_BIAS_COST0  VEDU_MRG_BIAS_COST0;
    volatile U_VEDU_MRG_BIAS_COST1  VEDU_MRG_BIAS_COST1;
    volatile U_VEDU_MRG_ABS_OFFSET0 VEDU_MRG_ABS_OFFSET0;
    volatile U_VEDU_MRG_ABS_OFFSET1 VEDU_MRG_ABS_OFFSET1;
    volatile U_VEDU_MRG_ADJ_WEIGHT  VEDU_MRG_ADJ_WEIGHT;
    volatile U_VEDU_INTRA_BIT_WEIGHT VEDU_INTRA_BIT_WEIGHT;
    volatile U_VEDU_INTRA_RDO_COST_OFFSET_0 VEDU_INTRA_RDO_COST_OFFSET_0;
    volatile U_VEDU_INTRA_RDO_COST_OFFSET_1 VEDU_INTRA_RDO_COST_OFFSET_1;
    volatile U_VEDU_INTRA_NO_DC_COST_OFFSET_0 VEDU_INTRA_NO_DC_COST_OFFSET_0;
    volatile U_VEDU_INTRA_NO_DC_COST_OFFSET_1 VEDU_INTRA_NO_DC_COST_OFFSET_1;
    volatile U_VEDU_INTRA_RDO_COST_OFFSET_3 VEDU_INTRA_RDO_COST_OFFSET_3;
    volatile U_VEDU_SEL_OFFSET_STRENGTH VEDU_SEL_OFFSET_STRENGTH;
    volatile U_VEDU_SEL_CU32_DC_AC_TH_OFFSET VEDU_SEL_CU32_DC_AC_TH_OFFSET;
    volatile U_VEDU_SEL_CU32_QP_TH  VEDU_SEL_CU32_QP_TH;
    volatile U_VEDU_SEL_RES_DC_AC_TH VEDU_SEL_RES_DC_AC_TH;
    volatile unsigned int           VEDU_RESERVED_12[17];
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG00 VEDU_QPG_QP_LAMBDA_CTRL_REG00;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG01 VEDU_QPG_QP_LAMBDA_CTRL_REG01;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG02 VEDU_QPG_QP_LAMBDA_CTRL_REG02;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG03 VEDU_QPG_QP_LAMBDA_CTRL_REG03;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG04 VEDU_QPG_QP_LAMBDA_CTRL_REG04;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG05 VEDU_QPG_QP_LAMBDA_CTRL_REG05;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG06 VEDU_QPG_QP_LAMBDA_CTRL_REG06;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG07 VEDU_QPG_QP_LAMBDA_CTRL_REG07;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG08 VEDU_QPG_QP_LAMBDA_CTRL_REG08;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG09 VEDU_QPG_QP_LAMBDA_CTRL_REG09;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG10 VEDU_QPG_QP_LAMBDA_CTRL_REG10;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG11 VEDU_QPG_QP_LAMBDA_CTRL_REG11;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG12 VEDU_QPG_QP_LAMBDA_CTRL_REG12;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG13 VEDU_QPG_QP_LAMBDA_CTRL_REG13;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG14 VEDU_QPG_QP_LAMBDA_CTRL_REG14;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG15 VEDU_QPG_QP_LAMBDA_CTRL_REG15;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG16 VEDU_QPG_QP_LAMBDA_CTRL_REG16;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG17 VEDU_QPG_QP_LAMBDA_CTRL_REG17;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG18 VEDU_QPG_QP_LAMBDA_CTRL_REG18;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG19 VEDU_QPG_QP_LAMBDA_CTRL_REG19;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG20 VEDU_QPG_QP_LAMBDA_CTRL_REG20;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG21 VEDU_QPG_QP_LAMBDA_CTRL_REG21;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG22 VEDU_QPG_QP_LAMBDA_CTRL_REG22;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG23 VEDU_QPG_QP_LAMBDA_CTRL_REG23;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG24 VEDU_QPG_QP_LAMBDA_CTRL_REG24;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG25 VEDU_QPG_QP_LAMBDA_CTRL_REG25;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG26 VEDU_QPG_QP_LAMBDA_CTRL_REG26;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG27 VEDU_QPG_QP_LAMBDA_CTRL_REG27;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG28 VEDU_QPG_QP_LAMBDA_CTRL_REG28;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG29 VEDU_QPG_QP_LAMBDA_CTRL_REG29;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG30 VEDU_QPG_QP_LAMBDA_CTRL_REG30;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG31 VEDU_QPG_QP_LAMBDA_CTRL_REG31;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG32 VEDU_QPG_QP_LAMBDA_CTRL_REG32;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG33 VEDU_QPG_QP_LAMBDA_CTRL_REG33;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG34 VEDU_QPG_QP_LAMBDA_CTRL_REG34;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG35 VEDU_QPG_QP_LAMBDA_CTRL_REG35;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG36 VEDU_QPG_QP_LAMBDA_CTRL_REG36;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG37 VEDU_QPG_QP_LAMBDA_CTRL_REG37;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG38 VEDU_QPG_QP_LAMBDA_CTRL_REG38;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG39 VEDU_QPG_QP_LAMBDA_CTRL_REG39;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG40 VEDU_QPG_QP_LAMBDA_CTRL_REG40;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG41 VEDU_QPG_QP_LAMBDA_CTRL_REG41;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG42 VEDU_QPG_QP_LAMBDA_CTRL_REG42;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG43 VEDU_QPG_QP_LAMBDA_CTRL_REG43;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG44 VEDU_QPG_QP_LAMBDA_CTRL_REG44;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG45 VEDU_QPG_QP_LAMBDA_CTRL_REG45;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG46 VEDU_QPG_QP_LAMBDA_CTRL_REG46;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG47 VEDU_QPG_QP_LAMBDA_CTRL_REG47;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG48 VEDU_QPG_QP_LAMBDA_CTRL_REG48;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG49 VEDU_QPG_QP_LAMBDA_CTRL_REG49;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG50 VEDU_QPG_QP_LAMBDA_CTRL_REG50;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG51 VEDU_QPG_QP_LAMBDA_CTRL_REG51;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG52 VEDU_QPG_QP_LAMBDA_CTRL_REG52;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG53 VEDU_QPG_QP_LAMBDA_CTRL_REG53;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG54 VEDU_QPG_QP_LAMBDA_CTRL_REG54;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG55 VEDU_QPG_QP_LAMBDA_CTRL_REG55;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG56 VEDU_QPG_QP_LAMBDA_CTRL_REG56;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG57 VEDU_QPG_QP_LAMBDA_CTRL_REG57;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG58 VEDU_QPG_QP_LAMBDA_CTRL_REG58;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG59 VEDU_QPG_QP_LAMBDA_CTRL_REG59;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG60 VEDU_QPG_QP_LAMBDA_CTRL_REG60;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG61 VEDU_QPG_QP_LAMBDA_CTRL_REG61;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG62 VEDU_QPG_QP_LAMBDA_CTRL_REG62;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG63 VEDU_QPG_QP_LAMBDA_CTRL_REG63;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG64 VEDU_QPG_QP_LAMBDA_CTRL_REG64;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG65 VEDU_QPG_QP_LAMBDA_CTRL_REG65;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG66 VEDU_QPG_QP_LAMBDA_CTRL_REG66;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG67 VEDU_QPG_QP_LAMBDA_CTRL_REG67;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG68 VEDU_QPG_QP_LAMBDA_CTRL_REG68;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG69 VEDU_QPG_QP_LAMBDA_CTRL_REG69;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG70 VEDU_QPG_QP_LAMBDA_CTRL_REG70;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG71 VEDU_QPG_QP_LAMBDA_CTRL_REG71;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG72 VEDU_QPG_QP_LAMBDA_CTRL_REG72;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG73 VEDU_QPG_QP_LAMBDA_CTRL_REG73;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG74 VEDU_QPG_QP_LAMBDA_CTRL_REG74;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG75 VEDU_QPG_QP_LAMBDA_CTRL_REG75;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG76 VEDU_QPG_QP_LAMBDA_CTRL_REG76;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG77 VEDU_QPG_QP_LAMBDA_CTRL_REG77;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG78 VEDU_QPG_QP_LAMBDA_CTRL_REG78;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG79 VEDU_QPG_QP_LAMBDA_CTRL_REG79;
    volatile U_VEDU_IME_RDOCFG      VEDU_IME_RDOCFG;
    volatile U_VEDU_MRG_FORCE_ZERO_EN VEDU_MRG_FORCE_ZERO_EN;
    volatile unsigned int           VEDU_VCPI_INTRA8_LOW_POWER;
    volatile U_VEDU_VCPI_LOW_POWER  VEDU_VCPI_LOW_POWER;
    volatile U_VEDU_IME_INTER_MODE  VEDU_IME_INTER_MODE;
    volatile U_VEDU_VCPI_PRE_JUDGE_EXT_EN VEDU_VCPI_PRE_JUDGE_EXT_EN;
    volatile unsigned int           VEDU_RESERVED_13[10];
    volatile U_VEDU_PME_SW_ADAPT_EN VEDU_PME_SW_ADAPT_EN;
    volatile U_VEDU_PME_WINDOW_SIZE0_L0 VEDU_PME_WINDOW_SIZE0_L0;
    volatile U_VEDU_PME_WINDOW_SIZE0_L1 VEDU_PME_WINDOW_SIZE0_L1;
    volatile U_VEDU_PME_SKIP_PRE    VEDU_PME_SKIP_PRE;
    volatile U_VEDU_PME_PBLK_PRE1   VEDU_PME_PBLK_PRE1;
    volatile U_VEDU_VCPI_PRE_JUDGE_COST_THR VEDU_VCPI_PRE_JUDGE_COST_THR;
    volatile U_VEDU_VCPI_IBLK_PRE_MV_THR VEDU_VCPI_IBLK_PRE_MV_THR;
    volatile U_VEDU_PME_IBLK_COST_THR VEDU_PME_IBLK_COST_THR;
    volatile U_VEDU_PME_TR_WEIGHTX  VEDU_PME_TR_WEIGHTX;
    volatile U_VEDU_PME_TR_WEIGHTY  VEDU_PME_TR_WEIGHTY;
    volatile U_VEDU_PME_SR_WEIGHT   VEDU_PME_SR_WEIGHT;
    volatile U_VEDU_PME_INTRA_LOWPOW VEDU_PME_INTRA_LOWPOW;
    volatile U_VEDU_IME_FME_LPOW_THR VEDU_IME_FME_LPOW_THR;
    volatile U_VEDU_PME_SKIP_FLAG   VEDU_PME_SKIP_FLAG;
    volatile U_VEDU_PME_PSW_LPW     VEDU_PME_PSW_LPW;
    volatile U_VEDU_PME_PBLK_PRE2   VEDU_PME_PBLK_PRE2;
    volatile U_VEDU_IME_LAYER3TO2_THR VEDU_IME_LAYER3TO2_THR;
    volatile U_VEDU_IME_LAYER3TO2_THR1 VEDU_IME_LAYER3TO2_THR1;
    volatile U_VEDU_IME_LAYER3TO1_THR VEDU_IME_LAYER3TO1_THR;
    volatile U_VEDU_IME_LAYER3TO1_THR1 VEDU_IME_LAYER3TO1_THR1;
    volatile U_VEDU_FME_PU64_LWP    VEDU_FME_PU64_LWP;
    volatile U_VEDU_MRG_FORCE_SKIP_EN VEDU_MRG_FORCE_SKIP_EN;
    volatile U_VEDU_INTRA_CFG       VEDU_INTRA_CFG;
    volatile U_VEDU_INTRA_LOW_POW   VEDU_INTRA_LOW_POW;
    volatile unsigned int           VEDU_RESERVED_14[8];
    volatile unsigned int           VEDU_VCPI_TUNLCELL_ADDR_L;
    volatile unsigned int           VEDU_VCPI_TUNLCELL_ADDR_H;
    volatile unsigned int           VEDU_VCPI_SRC_YADDR_L;
    volatile unsigned int           VEDU_VCPI_SRC_YADDR_H;
    volatile unsigned int           VEDU_VCPI_SRC_EXT_YADDR_L;
    volatile unsigned int           VEDU_VCPI_SRC_EXT_YADDR_H;
    volatile unsigned int           VEDU_VCPI_SRC_CADDR_L;
    volatile unsigned int           VEDU_VCPI_SRC_CADDR_H;
    volatile unsigned int           VEDU_VCPI_SRC_EXT_CADDR_L;
    volatile unsigned int           VEDU_VCPI_SRC_EXT_CADDR_H;
    volatile unsigned int           VEDU_VCPI_SRC_VADDR_L;
    volatile unsigned int           VEDU_VCPI_SRC_VADDR_H;
    volatile unsigned int           VEDU_VCPI_YH_ADDR_L;
    volatile unsigned int           VEDU_VCPI_YH_ADDR_H;
    volatile unsigned int           VEDU_VCPI_CH_ADDR_L;
    volatile unsigned int           VEDU_VCPI_CH_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFY_L0_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFY_L0_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFC_L0_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFC_L0_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFYH_L0_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFYH_L0_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFCH_L0_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFCH_L0_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFY_L1_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFY_L1_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFC_L1_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFC_L1_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFYH_L1_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFYH_L1_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFCH_L1_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFCH_L1_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REC_YADDR_L;
    volatile unsigned int           VEDU_VCPI_REC_YADDR_H;
    volatile unsigned int           VEDU_VCPI_REC_CADDR_L;
    volatile unsigned int           VEDU_VCPI_REC_CADDR_H;
    volatile unsigned int           VEDU_VCPI_REC_YH_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REC_YH_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REC_CH_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REC_CH_ADDR_H;
    volatile unsigned int           VEDU_VCPI_PMELD_L0_ADDR_L;
    volatile unsigned int           VEDU_VCPI_PMELD_L0_ADDR_H;
    volatile unsigned int           VEDU_VCPI_PMELD_L1_ADDR_L;
    volatile unsigned int           VEDU_VCPI_PMELD_L1_ADDR_H;
    volatile unsigned int           VEDU_VCPI_PMEST_ADDR_L;
    volatile unsigned int           VEDU_VCPI_PMEST_ADDR_H;
    volatile unsigned int           VEDU_VCPI_NBI_MVST_ADDR_L;
    volatile unsigned int           VEDU_VCPI_NBI_MVST_ADDR_H;
    volatile unsigned int           VEDU_VCPI_NBI_MVLD_ADDR_L;
    volatile unsigned int           VEDU_VCPI_NBI_MVLD_ADDR_H;
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_ADDR_L      ; /* 0x9c8 */
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_ADDR_H      ; /* 0x9cc */
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_STAT_ADDR_L ; /* 0x9d0 */
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_STAT_ADDR_H ; /* 0x9d4 */
    volatile unsigned int           VEDU_VCPI_INFORLD_FEATURE_DETECT_ADDR_L ; /* 0x9d8 */
    volatile unsigned int           VEDU_VCPI_INFORLD_FEATURE_DETECT_ADDR_H ; /* 0x9dc */
    volatile unsigned int           VEDU_VCPI_LAST_INFORLD_FEATURE_DETECT_ADDR_L ; /* 0x9e0 */
    volatile unsigned int           VEDU_VCPI_LAST_INFORLD_FEATURE_DETECT_ADDR_H ; /* 0x9e4 */
    volatile U_VEDU_VCPI_INFORLD_MV_STRIDE   VEDU_VCPI_INFORLD_MV_STRIDE      ; /* 0x9e8 */
    volatile U_VEDU_VCPI_INFORLD_FEATURE_DETECT_STRIDE   VEDU_VCPI_INFORLD_FEATURE_DETECT_STRIDE ; /* 0x9ec */
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_CONFIDENCE_ADDR_L ; /* 0x9f0 */
    volatile unsigned int           VEDU_VCPI_INFORLD_MV_CONFIDENCE_ADDR_H ; /* 0x9f4 */
    volatile unsigned int           VEDU_VCPI_QPGLD_INF_ADDR_L;
    volatile unsigned int           VEDU_VCPI_QPGLD_INF_ADDR_H;
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT0   VEDU_VCPI_SRDO_MAX_WEIGHT0       ; /* 0xa00 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT1   VEDU_VCPI_SRDO_MAX_WEIGHT1       ; /* 0xa04 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT2   VEDU_VCPI_SRDO_MAX_WEIGHT2       ; /* 0xa08 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT3   VEDU_VCPI_SRDO_MAX_WEIGHT3       ; /* 0xa0c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT4   VEDU_VCPI_SRDO_MAX_WEIGHT4       ; /* 0xa10 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT5   VEDU_VCPI_SRDO_MAX_WEIGHT5       ; /* 0xa14 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT6   VEDU_VCPI_SRDO_MAX_WEIGHT6       ; /* 0xa18 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT7   VEDU_VCPI_SRDO_MAX_WEIGHT7       ; /* 0xa1c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT8   VEDU_VCPI_SRDO_MAX_WEIGHT8       ; /* 0xa20 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT9   VEDU_VCPI_SRDO_MAX_WEIGHT9       ; /* 0xa24 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT10   VEDU_VCPI_SRDO_MAX_WEIGHT10      ; /* 0xa28 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT11   VEDU_VCPI_SRDO_MAX_WEIGHT11      ; /* 0xa2c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT12   VEDU_VCPI_SRDO_MAX_WEIGHT12      ; /* 0xa30 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT13   VEDU_VCPI_SRDO_MAX_WEIGHT13      ; /* 0xa34 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT14   VEDU_VCPI_SRDO_MAX_WEIGHT14      ; /* 0xa38 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT15   VEDU_VCPI_SRDO_MAX_WEIGHT15      ; /* 0xa3c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT16   VEDU_VCPI_SRDO_MAX_WEIGHT16      ; /* 0xa40 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT17   VEDU_VCPI_SRDO_MAX_WEIGHT17      ; /* 0xa44 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT18   VEDU_VCPI_SRDO_MAX_WEIGHT18      ; /* 0xa48 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT19   VEDU_VCPI_SRDO_MAX_WEIGHT19      ; /* 0xa4c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT20   VEDU_VCPI_SRDO_MAX_WEIGHT20      ; /* 0xa50 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT21   VEDU_VCPI_SRDO_MAX_WEIGHT21      ; /* 0xa54 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT22   VEDU_VCPI_SRDO_MAX_WEIGHT22      ; /* 0xa58 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT23   VEDU_VCPI_SRDO_MAX_WEIGHT23      ; /* 0xa5c */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT24   VEDU_VCPI_SRDO_MAX_WEIGHT24      ; /* 0xa60 */
    volatile U_VEDU_VCPI_SRDO_MAX_WEIGHT25   VEDU_VCPI_SRDO_MAX_WEIGHT25      ; /* 0xa64 */

    volatile unsigned int           VEDU_TOP_TB_INFO_RADDR_L;
    volatile unsigned int           VEDU_TOP_TB_INFO_RADDR_H;
    volatile unsigned int           VEDU_LEFT_TB_INFO_RADDR_L;
    volatile unsigned int           VEDU_LEFT_TB_INFO_RADDR_H;
    volatile unsigned int           VEDU_TOPLEFT_TB_INFO_RADDR_L;
    volatile unsigned int           VEDU_TOPLEFT_TB_INFO_RADDR_H;
    volatile unsigned int           VEDU_PPFD_ST_ADDR0_L;
    volatile unsigned int           VEDU_PPFD_ST_ADDR0_H;
    volatile unsigned int           VEDU_SKIPWEIGHT_LD_ADDR_L;
    volatile unsigned int           VEDU_SKIPWEIGHT_LD_ADDR_H;
    volatile unsigned int           VEDU_RESERVED_15[28];
    volatile U_VEDU_SAO_SSD_AREA0_START VEDU_SAO_SSD_AREA0_START;
    volatile U_VEDU_SAO_SSD_AREA0_END VEDU_SAO_SSD_AREA0_END;
    volatile U_VEDU_SAO_SSD_AREA1_START VEDU_SAO_SSD_AREA1_START;
    volatile U_VEDU_SAO_SSD_AREA1_END VEDU_SAO_SSD_AREA1_END;
    volatile U_VEDU_SAO_SSD_AREA2_START VEDU_SAO_SSD_AREA2_START;
    volatile U_VEDU_SAO_SSD_AREA2_END VEDU_SAO_SSD_AREA2_END;
    volatile U_VEDU_SAO_SSD_AREA3_START VEDU_SAO_SSD_AREA3_START;
    volatile U_VEDU_SAO_SSD_AREA3_END VEDU_SAO_SSD_AREA3_END;
    volatile U_VEDU_SAO_SSD_AREA4_START VEDU_SAO_SSD_AREA4_START;
    volatile U_VEDU_SAO_SSD_AREA4_END VEDU_SAO_SSD_AREA4_END;
    volatile U_VEDU_SAO_SSD_AREA5_START VEDU_SAO_SSD_AREA5_START;
    volatile U_VEDU_SAO_SSD_AREA5_END VEDU_SAO_SSD_AREA5_END;
    volatile U_VEDU_SAO_SSD_AREA6_START VEDU_SAO_SSD_AREA6_START;
    volatile U_VEDU_SAO_SSD_AREA6_END VEDU_SAO_SSD_AREA6_END;
    volatile U_VEDU_SAO_SSD_AREA7_START VEDU_SAO_SSD_AREA7_START;
    volatile U_VEDU_SAO_SSD_AREA7_END VEDU_SAO_SSD_AREA7_END;
    volatile unsigned int           VEDU_RESERVED_16[16];
    volatile U_VEDU_VCPI_STRIDE     VEDU_VCPI_STRIDE;
    volatile U_VEDU_VCPI_EXT_STRIDE VEDU_VCPI_EXT_STRIDE;
    volatile U_VEDU_VCPI_REF_L0_STRIDE VEDU_VCPI_REF_L0_STRIDE;
    volatile U_VEDU_VCPI_REF_L1_STRIDE VEDU_VCPI_REF_L1_STRIDE;
    volatile U_VEDU_VCPI_REC_STRIDE VEDU_VCPI_REC_STRIDE;
    volatile unsigned int           VEDU_VCPI_PMEST_STRIDE;
    volatile unsigned int           VEDU_VCPI_PMELD_STRIDE;
    volatile U_VEDU_VCPI_REC_HEADER_STRIDE VEDU_VCPI_REC_HEADER_STRIDE;
    volatile U_VEDU_VCPI_REF_L0_HEADER_STRIDE VEDU_VCPI_REF_L0_HEADER_STRIDE;
    volatile unsigned int           VEDU_RESERVED_17[23];
    volatile U_VEDU_ME_ET_THR       VEDU_ME_ET_THR;
    volatile U_VEDU_ME_ET_THR_ME_STAT VEDU_ME_ET_THR_ME_STAT;
    volatile U_VEDU_IME_REF_BUF_WORD_NUM VEDU_IME_REF_BUF_WORD_NUM;
    volatile U_VEDU_IME_INTERPOLATION_FLAG VEDU_IME_INTERPOLATION_FLAG;
    volatile U_VEDU_ME_MAX_REGION   VEDU_ME_MAX_REGION;
    volatile U_VEDU_ME_ADD1_RANDOM_POINT_LIST VEDU_ME_ADD1_RANDOM_POINT_LIST;
    volatile U_VEDU_ME_ADD2_RANDOM_POINT_LIST VEDU_ME_ADD2_RANDOM_POINT_LIST;
    volatile U_VEDU_ME_ADD3_RANDOM_POINT_LIST0 VEDU_ME_ADD3_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD3_RANDOM_POINT_LIST1 VEDU_ME_ADD3_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD4_RANDOM_POINT_LIST0 VEDU_ME_ADD4_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD4_RANDOM_POINT_LIST1 VEDU_ME_ADD4_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD5_RANDOM_POINT_LIST0 VEDU_ME_ADD5_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD5_RANDOM_POINT_LIST1 VEDU_ME_ADD5_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD5_RANDOM_POINT_LIST2 VEDU_ME_ADD5_RANDOM_POINT_LIST2;
    volatile U_VEDU_ME_ADD6_RANDOM_POINT_LIST0 VEDU_ME_ADD6_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD6_RANDOM_POINT_LIST1 VEDU_ME_ADD6_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD6_RANDOM_POINT_LIST2 VEDU_ME_ADD6_RANDOM_POINT_LIST2;
    volatile U_VEDU_ME_ADD7_RANDOM_POINT_LIST0 VEDU_ME_ADD7_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD7_RANDOM_POINT_LIST1 VEDU_ME_ADD7_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD7_RANDOM_POINT_LIST2 VEDU_ME_ADD7_RANDOM_POINT_LIST2;
    volatile U_VEDU_ME_ADD7_RANDOM_POINT_LIST3 VEDU_ME_ADD7_RANDOM_POINT_LIST3;
    volatile U_VEDU_ME_ADD8_RANDOM_POINT_LIST0 VEDU_ME_ADD8_RANDOM_POINT_LIST0;
    volatile U_VEDU_ME_ADD8_RANDOM_POINT_LIST1 VEDU_ME_ADD8_RANDOM_POINT_LIST1;
    volatile U_VEDU_ME_ADD8_RANDOM_POINT_LIST2 VEDU_ME_ADD8_RANDOM_POINT_LIST2;
    volatile U_VEDU_ME_ADD8_RANDOM_POINT_LIST3 VEDU_ME_ADD8_RANDOM_POINT_LIST3;
    volatile unsigned int           VEDU_ME_TMV_SCALE;
    volatile U_VEDU_ME_TMV_EN       VEDU_ME_TMV_EN;
    volatile unsigned int           VEDU_CABAC_NAL_UNIT_HEADL;
    volatile unsigned int           VEDU_RESERVED_17_new[4];
    volatile U_VEDU_GHDR_CTRL       VEDU_GHDR_CTRL;
    volatile U_VEDU_GHDR_DEGAMMA_CTRL VEDU_GHDR_DEGAMMA_CTRL;
    volatile U_VEDU_GHDR_DEGAMMA_STEP VEDU_GHDR_DEGAMMA_STEP;
    volatile U_VEDU_GHDR_DEGAMMA_POS1 VEDU_GHDR_DEGAMMA_POS1;
    volatile U_VEDU_GHDR_DEGAMMA_POS2 VEDU_GHDR_DEGAMMA_POS2;
    volatile U_VEDU_GHDR_DEGAMMA_NUM VEDU_GHDR_DEGAMMA_NUM;
    volatile U_VEDU_GHDR_GAMUT_CTRL VEDU_GHDR_GAMUT_CTRL;
    volatile U_VEDU_GHDR_GAMUT_COEF00 VEDU_GHDR_GAMUT_COEF00;
    volatile U_VEDU_GHDR_GAMUT_COEF01 VEDU_GHDR_GAMUT_COEF01;
    volatile U_VEDU_GHDR_GAMUT_COEF02 VEDU_GHDR_GAMUT_COEF02;
    volatile U_VEDU_GHDR_GAMUT_COEF10 VEDU_GHDR_GAMUT_COEF10;
    volatile U_VEDU_GHDR_GAMUT_COEF11 VEDU_GHDR_GAMUT_COEF11;
    volatile U_VEDU_GHDR_GAMUT_COEF12 VEDU_GHDR_GAMUT_COEF12;
    volatile U_VEDU_GHDR_GAMUT_COEF20 VEDU_GHDR_GAMUT_COEF20;
    volatile U_VEDU_GHDR_GAMUT_COEF21 VEDU_GHDR_GAMUT_COEF21;
    volatile U_VEDU_GHDR_GAMUT_COEF22 VEDU_GHDR_GAMUT_COEF22;
    volatile U_VEDU_GHDR_GAMUT_SCALE VEDU_GHDR_GAMUT_SCALE;
    volatile U_VEDU_GHDR_GAMUT_CLIP_MIN VEDU_GHDR_GAMUT_CLIP_MIN;
    volatile U_VEDU_GHDR_GAMUT_CLIP_MAX VEDU_GHDR_GAMUT_CLIP_MAX;
    volatile U_VEDU_GHDR_TONEMAP_CTRL VEDU_GHDR_TONEMAP_CTRL;
    volatile U_VEDU_GHDR_TONEMAP_REN VEDU_GHDR_TONEMAP_REN;
    volatile unsigned int           VEDU_GHDR_RESERVED0;
    volatile U_VEDU_GHDR_TONEMAP_STEP VEDU_GHDR_TONEMAP_STEP;
    volatile U_VEDU_GHDR_TONEMAP_POS1 VEDU_GHDR_TONEMAP_POS1;
    volatile U_VEDU_GHDR_TONEMAP_POS2 VEDU_GHDR_TONEMAP_POS2;
    volatile U_VEDU_GHDR_TONEMAP_POS3 VEDU_GHDR_TONEMAP_POS3;
    volatile U_VEDU_GHDR_TONEMAP_POS4 VEDU_GHDR_TONEMAP_POS4;
    volatile U_VEDU_GHDR_TONEMAP_NUM VEDU_GHDR_TONEMAP_NUM;
    volatile U_VEDU_GHDR_TONEMAP_LUMA_COEF0 VEDU_GHDR_TONEMAP_LUMA_COEF0;
    volatile U_VEDU_GHDR_TONEMAP_LUMA_COEF1 VEDU_GHDR_TONEMAP_LUMA_COEF1;
    volatile U_VEDU_GHDR_TONEMAP_LUMA_COEF2 VEDU_GHDR_TONEMAP_LUMA_COEF2;
    volatile U_VEDU_GHDR_TONEMAP_LUMA_SCALE VEDU_GHDR_TONEMAP_LUMA_SCALE;

    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT0   VEDU_VCPI_SRDO_MIN_WEIGHT0       ; /* 0xd00 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT1   VEDU_VCPI_SRDO_MIN_WEIGHT1       ; /* 0xd04 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT2   VEDU_VCPI_SRDO_MIN_WEIGHT2       ; /* 0xd08 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT3   VEDU_VCPI_SRDO_MIN_WEIGHT3       ; /* 0xd0c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT4   VEDU_VCPI_SRDO_MIN_WEIGHT4       ; /* 0xd10 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT5   VEDU_VCPI_SRDO_MIN_WEIGHT5       ; /* 0xd14 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT6   VEDU_VCPI_SRDO_MIN_WEIGHT6       ; /* 0xd18 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT7   VEDU_VCPI_SRDO_MIN_WEIGHT7       ; /* 0xd1c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT8   VEDU_VCPI_SRDO_MIN_WEIGHT8       ; /* 0xd20 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT9   VEDU_VCPI_SRDO_MIN_WEIGHT9       ; /* 0xd24 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT10   VEDU_VCPI_SRDO_MIN_WEIGHT10      ; /* 0xd28 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT11   VEDU_VCPI_SRDO_MIN_WEIGHT11      ; /* 0xd2c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT12   VEDU_VCPI_SRDO_MIN_WEIGHT12      ; /* 0xd30 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT13   VEDU_VCPI_SRDO_MIN_WEIGHT13      ; /* 0xd34 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT14   VEDU_VCPI_SRDO_MIN_WEIGHT14      ; /* 0xd38 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT15   VEDU_VCPI_SRDO_MIN_WEIGHT15      ; /* 0xd3c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT16   VEDU_VCPI_SRDO_MIN_WEIGHT16      ; /* 0xd40 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT17   VEDU_VCPI_SRDO_MIN_WEIGHT17      ; /* 0xd44 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT18   VEDU_VCPI_SRDO_MIN_WEIGHT18      ; /* 0xd48 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT19   VEDU_VCPI_SRDO_MIN_WEIGHT19      ; /* 0xd4c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT20   VEDU_VCPI_SRDO_MIN_WEIGHT20      ; /* 0xd50 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT21   VEDU_VCPI_SRDO_MIN_WEIGHT21      ; /* 0xd54 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT22   VEDU_VCPI_SRDO_MIN_WEIGHT22      ; /* 0xd58 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT23   VEDU_VCPI_SRDO_MIN_WEIGHT23      ; /* 0xd5c */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT24   VEDU_VCPI_SRDO_MIN_WEIGHT24      ; /* 0xd60 */
    volatile U_VEDU_VCPI_SRDO_MIN_WEIGHT25   VEDU_VCPI_SRDO_MIN_WEIGHT25      ; /* 0xd64 */
    volatile unsigned int           VEDU_RESERVED_0D68            ; /* 0xd68 */
    volatile unsigned int           reserved_89[16]                  ; /* 0xd6c~0xda8 */

    volatile unsigned int           VEDU_VCPI_REFY_DOWNBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFY_DOWNBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFYH_DOWNBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFYH_DOWNBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFC_DOWNBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFC_DOWNBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFCH_DOWNBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFCH_DOWNBOUNDARY_ADDR_H;
    volatile U_VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE VEDU_VCPI_REF_DOWNBOUNDARY_STRIDE;
    volatile U_VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE VEDU_VCPI_REF_DOWNBOUNDARY_HEADER_STRIDE;
    volatile unsigned int           VEDU_VCPI_PACKET_RESERVED_INFO_L ; /* 0xdd4 */
    volatile unsigned int           VEDU_VCPI_PACKET_RESERVED_INFO_M ; /* 0xdd8 */
    volatile unsigned int           VEDU_VCPI_PACKET_RESERVED_INFO_H ; /* 0xddc */
    volatile unsigned int           VEDU_RESERVED_18              ; /* 0xde0 */
    volatile unsigned int           reserved_90[7]                   ; /* 0xde4~0xdfc */
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG80 VEDU_QPG_QP_LAMBDA_CTRL_REG80;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG81 VEDU_QPG_QP_LAMBDA_CTRL_REG81;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG82 VEDU_QPG_QP_LAMBDA_CTRL_REG82;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG83 VEDU_QPG_QP_LAMBDA_CTRL_REG83;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG84 VEDU_QPG_QP_LAMBDA_CTRL_REG84;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG85 VEDU_QPG_QP_LAMBDA_CTRL_REG85;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG86 VEDU_QPG_QP_LAMBDA_CTRL_REG86;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG87 VEDU_QPG_QP_LAMBDA_CTRL_REG87;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG88 VEDU_QPG_QP_LAMBDA_CTRL_REG88;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG89 VEDU_QPG_QP_LAMBDA_CTRL_REG89;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG90 VEDU_QPG_QP_LAMBDA_CTRL_REG90;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG91 VEDU_QPG_QP_LAMBDA_CTRL_REG91;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG92 VEDU_QPG_QP_LAMBDA_CTRL_REG92;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG93 VEDU_QPG_QP_LAMBDA_CTRL_REG93;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG94 VEDU_QPG_QP_LAMBDA_CTRL_REG94;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG95 VEDU_QPG_QP_LAMBDA_CTRL_REG95;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG96 VEDU_QPG_QP_LAMBDA_CTRL_REG96;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG97 VEDU_QPG_QP_LAMBDA_CTRL_REG97;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG98 VEDU_QPG_QP_LAMBDA_CTRL_REG98;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG99 VEDU_QPG_QP_LAMBDA_CTRL_REG99;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG100 VEDU_QPG_QP_LAMBDA_CTRL_REG100;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG101 VEDU_QPG_QP_LAMBDA_CTRL_REG101;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG102 VEDU_QPG_QP_LAMBDA_CTRL_REG102;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG103 VEDU_QPG_QP_LAMBDA_CTRL_REG103;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG104 VEDU_QPG_QP_LAMBDA_CTRL_REG104;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG105 VEDU_QPG_QP_LAMBDA_CTRL_REG105;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG106 VEDU_QPG_QP_LAMBDA_CTRL_REG106;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG107 VEDU_QPG_QP_LAMBDA_CTRL_REG107;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG108 VEDU_QPG_QP_LAMBDA_CTRL_REG108;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG109 VEDU_QPG_QP_LAMBDA_CTRL_REG109;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG110 VEDU_QPG_QP_LAMBDA_CTRL_REG110;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG111 VEDU_QPG_QP_LAMBDA_CTRL_REG111;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG112 VEDU_QPG_QP_LAMBDA_CTRL_REG112;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG113 VEDU_QPG_QP_LAMBDA_CTRL_REG113;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG114 VEDU_QPG_QP_LAMBDA_CTRL_REG114;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG115 VEDU_QPG_QP_LAMBDA_CTRL_REG115;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG116 VEDU_QPG_QP_LAMBDA_CTRL_REG116;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG117 VEDU_QPG_QP_LAMBDA_CTRL_REG117;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG118 VEDU_QPG_QP_LAMBDA_CTRL_REG118;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG119 VEDU_QPG_QP_LAMBDA_CTRL_REG119;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG120 VEDU_QPG_QP_LAMBDA_CTRL_REG120;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG121 VEDU_QPG_QP_LAMBDA_CTRL_REG121;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG122 VEDU_QPG_QP_LAMBDA_CTRL_REG122;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG123 VEDU_QPG_QP_LAMBDA_CTRL_REG123;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG124 VEDU_QPG_QP_LAMBDA_CTRL_REG124;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG125 VEDU_QPG_QP_LAMBDA_CTRL_REG125;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG126 VEDU_QPG_QP_LAMBDA_CTRL_REG126;
    volatile U_VEDU_QPG_QP_LAMBDA_CTRL_REG127 VEDU_QPG_QP_LAMBDA_CTRL_REG127;
    volatile U_VEDU_VCPI_LUMA_START_ROWNUM VEDU_VCPI_LUMA_START_ROWNUM;
    volatile U_VEDU_VCPI_CURLD_START_SLICENUM VEDU_VCPI_CURLD_START_SLICENUM;
    volatile U_VEDU_VCPI_REFLD_BOUNDARY_MOD VEDU_VCPI_REFLD_BOUNDARY_MOD;
    volatile unsigned int           VEDU_VCPI_REFY_UPBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFY_UPBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFYH_UPBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFYH_UPBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFC_UPBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFC_UPBOUNDARY_ADDR_H;
    volatile unsigned int           VEDU_VCPI_REFCH_UPBOUNDARY_ADDR_L;
    volatile unsigned int           VEDU_VCPI_REFCH_UPBOUNDARY_ADDR_H;
    volatile U_VEDU_VCPI_REF_UPBOUNDARY_STRIDE VEDU_VCPI_REF_UPBOUNDARY_STRIDE;
    volatile U_VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE VEDU_VCPI_REF_UPBOUNFARY_HEADER_STRIDE;
    volatile U_VEDU_VCPI_PRIVATE_SLCHDR_PART0 VEDU_VCPI_PRIVATE_SLCHDR_PART0;
    volatile unsigned int           VEDU_VCPI_PRIVATE_SLCHDR_PART1;
    volatile U_VEDU_VCPI_LOWLATENCY_SLICE_INTEN VEDU_VCPI_LOWLATENCY_SLICE_INTEN;
    volatile unsigned int           VEDU_VCPI_STRMADDR_L_NEW[16];
    volatile unsigned int           VEDU_VCPI_STRMADDR_H_NEW[16];
    volatile unsigned int           VEDU_VCPI_STRMBUFLEN_NEW[16];
    volatile unsigned int           VEDU_VCPI_SLCINFOADDR_L_NEW;
    volatile unsigned int           VEDU_VCPI_SLCINFOADDR_H_NEW;
    volatile U_VEDU_VCTRL_INTRAPU4_REFRESH_START VEDU_VCTRL_INTRAPU4_REFRESH_START;
    volatile U_VEDU_VCTRL_INTRAPU4_REFRESH_END VEDU_VCTRL_INTRAPU4_REFRESH_END;
    volatile U_VEDU_VCPI_CMDLST_GRP_NUM VEDU_VCPI_CMDLST_GRP_NUM;
    volatile U_VEDU_VCPI_CMDLST_SOFT_FINISH_NUM VEDU_VCPI_CMDLST_SOFT_FINISH_NUM;
    volatile U_VEDU_VCPI_FORCE_ZEROMV VEDU_VCPI_FORCE_ZEROMV;
    volatile U_VEDU_VCPI_FORCE_INTRAPU32_NUM VEDU_VCPI_FORCE_INTRAPU32_NUM;
    volatile unsigned int           VEDU_VCPI_VLCST_RAL;
    volatile unsigned int           VEDU_VCPI_VLCST_SAL;
    volatile U_VEDU_VCPI_VLCST_RAH_SAH VEDU_VCPI_VLCST_RAH_SAH;
    volatile U_VEDU_VCPI_PACKET_INFO VEDU_VCPI_PACKET_INFO;
    volatile U_VEDU_SECURE_SET      VEDU_SECURE_SET;
    volatile unsigned int           VEDU_RESERVED_21[3];
    volatile unsigned int           VEDU_RESERVED_22[2048];

} S_VEDU_CONFIG_REGS_TYPE;

#endif /* __VEDU_CONFIG_H__ */
