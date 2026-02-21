// ******************************************************************************
// Copyright     :  Copyright (C) 2020, Hisilicon Technologies Co. Ltd.
// File name     :  mcu_ctrl.h
// Project line  :
// Department    :  K5
// Author        :  AnthonySixta
// Version       :  1.0
// Date          :  2013/5/31
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2
// History       :  AnthonySixta 2020/06/02 09:29:19 Create file
// ******************************************************************************

#ifndef __MCU_CTRL_H__
#define __MCU_CTRL_H__

/* Define the union U_CU16_QP_GRP0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_qp0              : 6   ; /* [5..0]  */
        unsigned int    cu16_qp1              : 6   ; /* [11..6]  */
        unsigned int    cu16_qp2              : 6   ; /* [17..12]  */
        unsigned int    cu16_qp3              : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CU16_QP_GRP0;

/* Define the union U_CU16_QP_GRP1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_qp4              : 6   ; /* [5..0]  */
        unsigned int    cu16_qp5              : 6   ; /* [11..6]  */
        unsigned int    cu16_qp6              : 6   ; /* [17..12]  */
        unsigned int    cu16_qp7              : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CU16_QP_GRP1;

/* Define the union U_CU16_QP_GRP2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_qp8              : 6   ; /* [5..0]  */
        unsigned int    cu16_qp9              : 6   ; /* [11..6]  */
        unsigned int    cu16_qp10             : 6   ; /* [17..12]  */
        unsigned int    cu16_qp11             : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CU16_QP_GRP2;

/* Define the union U_CU16_QP_GRP3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_qp12             : 6   ; /* [5..0]  */
        unsigned int    cu16_qp13             : 6   ; /* [11..6]  */
        unsigned int    cu16_qp14             : 6   ; /* [17..12]  */
        unsigned int    cu16_qp15             : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CU16_QP_GRP3;

/* Define the union U_CU32_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu32_qp0              : 6   ; /* [5..0]  */
        unsigned int    cu32_qp1              : 6   ; /* [11..6]  */
        unsigned int    cu32_qp2              : 6   ; /* [17..12]  */
        unsigned int    cu32_qp3              : 6   ; /* [23..18]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CU32_QP;

/* Define the union U_LCU_QP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_qp                : 6   ; /* [5..0]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_LCU_QP;

/* Define the union U_MCU_LCU_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_int_mask          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MCU_LCU_INTMASK;

/* Define the union U_MCU_LCU_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    clr_lcu_int           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MCU_LCU_INTCLR;

/* Define the union U_VEDU_MCU_INTMASK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_enable_ve_eop     : 1   ; /* [0]  */
        unsigned int    mcu_enable_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    mcu_enable_ve_buffull : 1   ; /* [2]  */
        unsigned int    mcu_enable_ve_pbitsover : 1   ; /* [3]  */
        unsigned int    mcu_enable_line_pos   : 1   ; /* [4]  */
        unsigned int    mcu_enable_ppdf_dec_err : 1   ; /* [5]  */
        unsigned int    mcu_enable_vedu_timeout : 1   ; /* [6]  */
        unsigned int    mcu_cmdlst_sop_mask   : 1   ; /* [7]  */
        unsigned int    mcu_cmdlst_eop_mask   : 1   ; /* [8]  */
        unsigned int    mcu_soft_int0_mask    : 1   ; /* [9]  */
        unsigned int    mcu_soft_int1_mask    : 1   ; /* [10]  */
        unsigned int    mcu_soft_int2_mask    : 1   ; /* [11]  */
        unsigned int    mcu_soft_int3_mask    : 1   ; /* [12]  */
        unsigned int    mcu_lowlatency_slcint_mask : 1   ; /* [13]  */
        unsigned int    mcu_enhance2_buffull_int_mask : 1   ; /* [14] */
        unsigned int    mcu_enhance2_pbitsover_int_mask : 1   ; /* [15] */
        unsigned int    mcu_enhance3_buffull_int_mask : 1   ; /* [16] */
        unsigned int    mcu_enhance3_pbitsover_int_mask : 1   ; /* [17] */
        unsigned int    mcu_enhance4_buffull_int_mask : 1   ; /* [18] */
        unsigned int    mcu_enhance4_pbitsover_int_mask : 1   ; /* [19] */
        unsigned int    mcu_hts_error_int_mask : 1   ; /* [20] */
        unsigned int    mcu_hts_event_int_mask : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MCU_INTMASK;

/* Define the union U_VEDU_MCU_INTCLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_clr_ve_eop        : 1   ; /* [0]  */
        unsigned int    mcu_clr_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    mcu_clr_ve_buffull    : 1   ; /* [2]  */
        unsigned int    mcu_clr_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    mcu_clr_line_pos      : 1   ; /* [4]  */
        unsigned int    mcu_clr_ppdf_dec_err  : 1   ; /* [5]  */
        unsigned int    mcu_clr_vedu_timeout  : 1   ; /* [6]  */
        unsigned int    mcu_clr_cmdlst_sop    : 1   ; /* [7]  */
        unsigned int    mcu_clr_cmdlst_eop    : 1   ; /* [8]  */
        unsigned int    mcu_clr_soft_int0     : 1   ; /* [9]  */
        unsigned int    mcu_clr_soft_int1     : 1   ; /* [10]  */
        unsigned int    mcu_clr_soft_int2     : 1   ; /* [11]  */
        unsigned int    mcu_clr_soft_int3     : 1   ; /* [12]  */
        unsigned int    mcu_clr_lowlatency_slcint : 1   ; /* [13]  */
        unsigned int    mcu_clr_enhance2_buffull_int : 1   ; /* [14] */
        unsigned int    mcu_clr_enhance2_pbitsover_int : 1   ; /* [15] */
        unsigned int    mcu_clr_enhance3_buffull_int : 1   ; /* [16] */
        unsigned int    mcu_clr_enhance3_pbitsover_int : 1   ; /* [17] */
        unsigned int    mcu_clr_enhance4_buffull_int : 1   ; /* [18] */
        unsigned int    mcu_clr_enhance4_pbitsover_int : 1   ; /* [19] */
        unsigned int    mcu_clr_hts_error_int : 1   ; /* [20] */
        unsigned int    mcu_clr_hts_event_int : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_MCU_INTCLR;

/* Define the union U_MCU_LCU_INTSTAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_int               : 1   ; /* [0]  */
        unsigned int    lcu_rint              : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MCU_LCU_INTSTAT;

/* Define the union U_FUNC_MCU_INTSTAT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_int_ve_eop        : 1   ; /* [0]  */
        unsigned int    mcu_int_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    mcu_int_ve_buffull    : 1   ; /* [2]  */
        unsigned int    mcu_int_ve_pbitsover  : 1   ; /* [3]  */
        unsigned int    mcu_int_line_pos      : 1   ; /* [4]  */
        unsigned int    mcu_int_ppfd_dec_err  : 1   ; /* [5]  */
        unsigned int    mcu_int_vedu_timeout  : 1   ; /* [6]  */
        unsigned int    mcu_cmdlst_sop        : 1   ; /* [7]  */
        unsigned int    mcu_cmdlst_eop        : 1   ; /* [8]  */
        unsigned int    mcu_soft_int0         : 1   ; /* [9]  */
        unsigned int    mcu_soft_int1         : 1   ; /* [10]  */
        unsigned int    mcu_soft_int2         : 1   ; /* [11]  */
        unsigned int    mcu_soft_int3         : 1   ; /* [12]  */
        unsigned int    mcu_lowlatency_slcint : 1   ; /* [13]  */
        unsigned int    mcu_enhance2_buffull_int : 1   ; /* [14] */
        unsigned int    mcu_enhance2_pbitsover_int : 1   ; /* [15] */
        unsigned int    mcu_enhance3_buffull_int : 1   ; /* [16] */
        unsigned int    mcu_enhance3_pbitsover_int : 1   ; /* [17] */
        unsigned int    mcu_enhance4_buffull_int : 1   ; /* [18] */
        unsigned int    mcu_enhance4_pbitsover_int : 1   ; /* [19] */
        unsigned int    mcu_hts_error_int     : 1   ; /* [20] */
        unsigned int    mcu_hts_event_int     : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FUNC_MCU_INTSTAT;

/* Define the union U_FUNC_MCU_RAWINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_rint_ve_eop       : 1   ; /* [0]  */
        unsigned int    mcu_rint_vedu_slice_end : 1   ; /* [1]  */
        unsigned int    mcu_rint_ve_buffull   : 1   ; /* [2]  */
        unsigned int    mcu_rint_ve_pbitsover : 1   ; /* [3]  */
        unsigned int    mcu_rint_line_pos     : 1   ; /* [4]  */
        unsigned int    mcu_rint_ppfd_dec_err : 1   ; /* [5]  */
        unsigned int    mcu_rint_vedu_timeout : 1   ; /* [6]  */
        unsigned int    mcu_cmdlst_sop_raw    : 1   ; /* [7]  */
        unsigned int    mcu_cmdlst_eop_raw    : 1   ; /* [8]  */
        unsigned int    mcu_soft_rint0        : 1   ; /* [9]  */
        unsigned int    mcu_soft_rint1        : 1   ; /* [10]  */
        unsigned int    mcu_soft_rint2        : 1   ; /* [11]  */
        unsigned int    mcu_soft_rint3        : 1   ; /* [12]  */
        unsigned int    mcu_rint_lowlatency_slcint : 1   ; /* [13]  */
        unsigned int    mcu_enhance2_buffull_rint : 1   ; /* [14] */
        unsigned int    mcu_enhance2_pbitsover_rint : 1   ; /* [15] */
        unsigned int    mcu_enhance3_buffull_rint : 1   ; /* [16] */
        unsigned int    mcu_enhance3_pbitsover_rint : 1   ; /* [17] */
        unsigned int    mcu_enhance4_buffull_rint : 1   ; /* [18] */
        unsigned int    mcu_enhance4_pbitsover_rint : 1   ; /* [19] */
        unsigned int    mcu_hts_error_rint    : 1   ; /* [20] */
        unsigned int    mcu_hts_event_rint    : 1   ; /* [21] */
        unsigned int    reserved_0            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_FUNC_MCU_RAWINT;

/* Define the union U_MCU_EOP */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    mcu_eop               : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_MCU_EOP;

typedef union
{
    struct
    {
        unsigned int    cu16_qp0_layer2       : 6   ; /* [5..0] */
        unsigned int    cu16_qp1_layer2       : 6   ; /* [11..6] */
        unsigned int    cu16_qp2_layer2       : 6   ; /* [17..12] */
        unsigned int    cu16_qp3_layer2       : 6   ; /* [23..18] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;
    unsigned int    u32;
} U_CU16_QP_GRP0_LAYER2;
typedef union
{
    struct
    {
        unsigned int    lcu_qp_layer2         : 6   ; /* [5..0] */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;
    unsigned int    u32;
} U_LCU_QP_LAYER2;
typedef union
{
    struct
    {
        unsigned int    cu16_qp0_layer3       : 6   ; /* [5..0] */
        unsigned int    cu16_qp1_layer3       : 6   ; /* [11..6] */
        unsigned int    cu16_qp2_layer3       : 6   ; /* [17..12] */
        unsigned int    cu16_qp3_layer3       : 6   ; /* [23..18] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;
    unsigned int    u32;
} U_CU16_QP_GRP0_LAYER3;
/* Define the union U_LCU_QP_LAYER3 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_qp_layer3         : 6   ; /* [5..0] */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_QP_LAYER3;

/* Define the union U_CU16_QP_GRP0_LAYER4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cu16_qp0_layer4       : 6   ; /* [5..0] */
        unsigned int    cu16_qp1_layer4       : 6   ; /* [11..6] */
        unsigned int    cu16_qp2_layer4       : 6   ; /* [17..12] */
        unsigned int    cu16_qp3_layer4       : 6   ; /* [23..18] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_CU16_QP_GRP0_LAYER4;

/* Define the union U_LCU_QP_LAYER4 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_qp_layer4         : 6   ; /* [5..0] */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_QP_LAYER4;

/* Define the union U_LCU_INTRA_CU_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_intra_cu_en       : 5   ; /* [4..0] */
        unsigned int    reserved_0            : 27  ; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_INTRA_CU_EN;

/* Define the union U_LCU_INTRA_CU_RDO_NUM */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_intra_cu16_rdo_num : 3   ; /* [2..0] */
        unsigned int    lcu_intra_cu32_rdo_num : 3   ; /* [5..3] */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_INTRA_CU_RDO_NUM;

/* Define the union U_LCU_INTER_CU_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_inter_cu_en       : 21  ; /* [20..0] */
        unsigned int    reserved_0            : 11  ; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_INTER_CU_EN;

/* Define the union U_LCU_INTER_CU_ZERO_RES_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_inter_cu_zero_res_en : 21  ; /* [20..0] */
        unsigned int    reserved_0            : 11  ; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_INTER_CU_ZERO_RES_EN;

/* Define the union U_LCU_INTER_CU_MRG_EN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    lcu_inter_cu_mrg_en   : 21  ; /* [20..0] */
        unsigned int    reserved_0            : 11  ; /* [31..21] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_LCU_INTER_CU_MRG_EN;

/* Define the union U_PINTRA_PU8_STRUCTURE_EDGE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pintra_pu8_structure_edge : 16  ; /* [15..0] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_PINTRA_PU8_STRUCTURE_EDGE;
//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_CU16_QP_GRP0         CU16_QP_GRP0;
    volatile U_CU16_QP_GRP1         CU16_QP_GRP1;
    volatile U_CU16_QP_GRP2         CU16_QP_GRP2;
    volatile U_CU16_QP_GRP3         CU16_QP_GRP3;
    volatile U_CU32_QP              CU32_QP;
    volatile U_LCU_QP               LCU_QP;
    volatile unsigned int           RESERVED0[2];
    volatile U_MCU_LCU_INTMASK      MCU_LCU_INTMASK;
    volatile U_MCU_LCU_INTCLR       MCU_LCU_INTCLR;
    volatile unsigned int           RESERVED1[2];
    volatile U_VEDU_MCU_INTMASK     VEDU_MCU_INTMASK;
    volatile U_VEDU_MCU_INTCLR      VEDU_MCU_INTCLR;
    volatile unsigned int           RESERVED6[2];
    volatile unsigned int           BEST_BITS;
    volatile unsigned int           REAL_BITS;
    volatile unsigned int           TEXTURE_COMPLEX_16[16];
    volatile unsigned int           INTRA_BEST_BITS;
    volatile U_MCU_LCU_INTSTAT      MCU_LCU_INTSTAT;
    volatile unsigned int           RESERVED2[28];
    volatile unsigned int           LCU_INTRA_BEST_BITS_LAST1;
    volatile unsigned int           LCU_INTRA_BEST_BITS_LAST2;
    volatile unsigned int           LCU_INTRA_BEST_BITS_LAST3;
    volatile unsigned int           LCU_BEST_BITS_LAST1;
    volatile unsigned int           LCU_BEST_BITS_LAST2;
    volatile unsigned int           LCU_BEST_BITS_LAST3;
    volatile unsigned int           LCU_REAL_BITS_LAST1;
    volatile unsigned int           LCU_REAL_BITS_LAST2;
    volatile unsigned int           LCU_REAL_BITS_LAST3;
    volatile unsigned int           LCU_REAL_BITS_LAST4;
    volatile unsigned int           LCU_REAL_BITS_LAST5;
    volatile unsigned int           RESERVED3[21];
    volatile U_FUNC_MCU_INTSTAT     FUNC_MCU_INTSTAT;
    volatile U_FUNC_MCU_RAWINT      FUNC_MCU_RAWINT;
    volatile unsigned int           FUNC_DBG0;
    volatile unsigned int           FUNC_DBG1;
    volatile unsigned int           RESERVED4[12];
    volatile U_MCU_EOP              MCU_EOP;
    volatile unsigned int           reserved_8[3]                    ; /* 0x1c4~0x1cc */
    volatile U_CU16_QP_GRP0_LAYER2   CU16_QP_GRP0_LAYER2              ; /* 0x1d0 */
    volatile unsigned int           reserved_9[4]                    ; /* 0x1d4~0x1e0 */
    volatile U_LCU_QP_LAYER2        LCU_QP_LAYER2                    ; /* 0x1e4 */
    volatile U_CU16_QP_GRP0_LAYER3   CU16_QP_GRP0_LAYER3              ; /* 0x1e8 */
    volatile unsigned int           reserved_10[4]                   ; /* 0x1ec~0x1f8 */
    volatile U_LCU_QP_LAYER3        LCU_QP_LAYER3                    ; /* 0x1fc */
    volatile unsigned int           REAL_BITS_LAYER2                 ; /* 0x200 */
    volatile unsigned int           REAL_BITS_LAYER3                 ; /* 0x204 */
    volatile unsigned int           LCU_REAL_BITS_LAST1_LAYER2       ; /* 0x208 */
    volatile unsigned int           LCU_REAL_BITS_LAST2_LAYER2       ; /* 0x20c */
    volatile unsigned int           LCU_REAL_BITS_LAST3_LAYER2       ; /* 0x210 */
    volatile unsigned int           LCU_REAL_BITS_LAST4_LAYER2       ; /* 0x214 */
    volatile unsigned int           LCU_REAL_BITS_LAST5_LAYER2       ; /* 0x218 */
    volatile unsigned int           LCU_REAL_BITS_LAST1_LAYER3       ; /* 0x21c */
    volatile unsigned int           LCU_REAL_BITS_LAST2_LAYER3       ; /* 0x220 */
    volatile unsigned int           LCU_REAL_BITS_LAST3_LAYER3       ; /* 0x224 */
    volatile unsigned int           LCU_REAL_BITS_LAST4_LAYER3       ; /* 0x228 */
    volatile unsigned int           LCU_REAL_BITS_LAST5_LAYER3       ; /* 0x22c */
    volatile unsigned int           LCU_REAL_BITS_LAST6_LAYER3       ; /* 0x230 */
    volatile unsigned int           LUMA_MEAN_BLK8[4]                ; /* 0x234~0x240 */
    volatile unsigned int           reserved_11[12]                  ; /* 0x244~0x270 */
    volatile unsigned int           LUMA_DIFFERENCE_BLK8[4]          ; /* 0x274~0x280 */
    volatile unsigned int           reserved_12[12]                  ; /* 0x284~0x2b0 */
    volatile unsigned int           MIN_MADI_BLK8                    ; /* 0x2b4 */
    volatile unsigned int           reserved_13[5]                   ; /* 0x2b8~0x2c8 */
    volatile U_CU16_QP_GRP0_LAYER4   CU16_QP_GRP0_LAYER4              ; /* 0x2cc */
    volatile unsigned int           reserved_14[4]                   ; /* 0x2d0~0x2dc */
    volatile U_LCU_QP_LAYER4        LCU_QP_LAYER4                    ; /* 0x2e0 */
    volatile unsigned int           REAL_BITS_LAYER4                 ; /* 0x2e4 */
    volatile unsigned int           LCU_REAL_BITS_LAST1_LAYER4       ; /* 0x2e8 */
    volatile unsigned int           LCU_REAL_BITS_LAST2_LAYER4       ; /* 0x2ec */
    volatile unsigned int           LCU_REAL_BITS_LAST3_LAYER4       ; /* 0x2f0 */
    volatile unsigned int           LCU_REAL_BITS_LAST4_LAYER4       ; /* 0x2f4 */
    volatile unsigned int           LCU_REAL_BITS_LAST5_LAYER4       ; /* 0x2f8 */
    volatile unsigned int           LCU_REAL_BITS_LAST6_LAYER4       ; /* 0x2fc */
    volatile unsigned int           LCU_REAL_BITS_LAST7_LAYER4       ; /* 0x300 */
    volatile unsigned int           reserved_15[63]                  ; /* 0x304~0x3fc */
    volatile U_LCU_INTRA_CU_EN      LCU_INTRA_CU_EN                  ; /* 0x400 */
    volatile U_LCU_INTRA_CU_RDO_NUM   LCU_INTRA_CU_RDO_NUM             ; /* 0x404 */
    volatile U_LCU_INTER_CU_EN      LCU_INTER_CU_EN                  ; /* 0x408 */
    volatile U_LCU_INTER_CU_ZERO_RES_EN   LCU_INTER_CU_ZERO_RES_EN         ; /* 0x40c */
    volatile U_LCU_INTER_CU_MRG_EN   LCU_INTER_CU_MRG_EN              ; /* 0x410 */
    volatile U_PINTRA_PU8_STRUCTURE_EDGE   PINTRA_PU8_STRUCTURE_EDGE        ; /* 0x414 */
    volatile unsigned int           reserved_16[762];
} S_MCU_CTRL_REGS_TYPE;

#endif /* __MCU_CTRL_H__ */
