// ******************************************************************************
// Copyright     :  Copyright (C) 2020, Hisilicon Technologies Co. Ltd.
// File name     :  smmu_pre.h
// Project line  :
// Department    :  K5
// Author        :  AnthonySixta
// Version       :  1.0
// Date          :  2013/5/31
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2
// History       :  AnthonySixta 2020/06/02 09:30:30 Create file
// ******************************************************************************

#ifndef __SMMU_PRE_H__
#define __SMMU_PRE_H__

/* Define the union U_VEDU_DPM_FREQ_SEL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dpm_freq_sel          : 3   ; /* [2..0]  */
        unsigned int    smmu_clkgate          : 1   ; /* [3]  */
        unsigned int    smmu_softrst          : 1   ; /* [4]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DPM_FREQ_SEL;

typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_addr_intercept_en : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DDR_ADDR_INTERCEPT_EN;

/* Define the union U_VEDU_DDR_ADDR_OVER_STATE_CLR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_waddr_over_state_clr : 1   ; /* [0]  */
        unsigned int    ddr_raddr_over_state_clr : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DDR_ADDR_OVER_STATE_CLR;

/* Define the union U_VEDU_DDR_ADDR_OVER_STATE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ddr_waddr_over_state  : 1   ; /* [0]  */
        unsigned int    ddr_raddr_over_state  : 1   ; /* [1]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DDR_ADDR_OVER_STATE;

/* Define the union U_VEDU_REFLD_ESP_HINT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sc_esp_hint_l0        : 4   ; /* [3..0]  */
        unsigned int    sc_esp_hint_l1        : 4   ; /* [7..4]  */
        unsigned int    sc_esp_hint_l2        : 4   ; /* [11..8]  */
        unsigned int    sc_esp_hint_en        : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_REFLD_ESP_HINT;

/* Define the union U_VEDU_DDR_ADDR_OVERID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    waddr_over_id         : 7   ; /* [6..0] */
        unsigned int    raddr_over_id         : 6   ; /* [11..6]  */
        unsigned int    reserved_0            : 19  ; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DDR_ADDR_OVERID;

/* Define the union U_VEDU_DDR_QOS */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    awqos                 : 4   ; /* [3..0]  */
        unsigned int    arqos                 : 4   ; /* [7..4]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_DDR_QOS;

/* Define the union U_VEDU_HIAXI_DBG_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    ckg_en_2to1_matrix    : 1   ; /* [0] */
        unsigned int    mst_priority_m        : 2   ; /* [2..1]  */
        unsigned int    dlock_and_dbg_clr     : 1   ; /* [3]  */
        unsigned int    ckg_en_64to128        : 1   ; /* [4] */
        unsigned int    reserved_0            : 27  ; /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_CTRL;

/* Define the union U_VEDU_HIAXI_DBG_DLOCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dlock_slv             : 2   ; /* [1..0]  */
        unsigned int    dlock_mst             : 4   ; /* [5..2]  */
        unsigned int    reserved_0            : 26  ; /* [31..6]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_DLOCK;

/* Define the union U_VEDU_HIAXI_IDLE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hiaxi_2to1_idle       : 1   ; /* [0] */
        unsigned int    hiaxi_64to128_idle    : 1   ; /* [1] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_IDLE;

/* Define the union U_VEDU_HIAXI_DBG_AWID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awid              : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWID;

/* Define the union U_VEDU_HIAXI_DBG_AWADDR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awaddr_0          : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWADDR_0;

/* Define the union U_VEDU_HIAXI_DBG_AWADDR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awaddr_1          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWADDR_1;

/* Define the union U_VEDU_HIAXI_DBG_AWLEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awlen             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWLEN;

/* Define the union U_VEDU_HIAXI_DBG_AWSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awsize            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWSIZE;

/* Define the union U_VEDU_HIAXI_DBG_AWBURST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awburst           : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWBURST;

/* Define the union U_VEDU_HIAXI_DBG_AWLOCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awlock            : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWLOCK;

/* Define the union U_VEDU_HIAXI_DBG_AWCACHE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awcache           : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWCACHE;

/* Define the union U_VEDU_HIAXI_DBG_AWPROT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awprot            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWPROT;

/* Define the union U_VEDU_HIAXI_DBG_AWVALID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_awvalid           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_AWVALID;

/* Define the union U_VEDU_HIAXI_DBG_ARID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arid              : 9   ; /* [8..0]  */
        unsigned int    reserved_0            : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARID;

/* Define the union U_VEDU_HIAXI_DBG_ARADDR_0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_araddr_0          : 31  ; /* [30..0]  */
        unsigned int    reserved_0            : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARADDR_0;

/* Define the union U_VEDU_HIAXI_DBG_ARADDR_1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_araddr_1          : 5   ; /* [4..0]  */
        unsigned int    reserved_0            : 27  ; /* [31..5]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARADDR_1;

/* Define the union U_VEDU_HIAXI_DBG_ARLEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arlen             : 8   ; /* [7..0]  */
        unsigned int    reserved_0            : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARLEN;

/* Define the union U_VEDU_HIAXI_DBG_ARSIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arsize            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARSIZE;

/* Define the union U_VEDU_HIAXI_DBG_ARBURST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arburst           : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARBURST;

/* Define the union U_VEDU_HIAXI_DBG_ARLOCK */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arlock            : 2   ; /* [1..0]  */
        unsigned int    reserved_0            : 30  ; /* [31..2]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARLOCK;

/* Define the union U_VEDU_HIAXI_DBG_ARCACHE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arcache           : 4   ; /* [3..0]  */
        unsigned int    reserved_0            : 28  ; /* [31..4]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARCACHE;

/* Define the union U_VEDU_HIAXI_DBG_ARPROT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arprot            : 3   ; /* [2..0]  */
        unsigned int    reserved_0            : 29  ; /* [31..3]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARPROT;

/* Define the union U_VEDU_HIAXI_DBG_ARVALID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_arvalid           : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_HIAXI_DBG_ARVALID;

/* Define the union U_VEDU_SMMU_SECSID */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    dbg_ar_dcdrerr_secerr_slverr_viserr_s0 : 4   ; /* [3..0] */
        unsigned int    dbg_aw_dcdrerr_secerr_slverr_viserr_s0 : 4   ; /* [7..4] */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } bits;

    unsigned int    u32;
} U_VEDU_HIAXI_DBG_ERR;

typedef union
{
    struct
    {
        unsigned int    va_str                : 24  ; /* [23..0] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_DDR_ADDR_VA_STR;

/* Define the union U_VEDU_SMMU_SID_SAFE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    va_end                : 24  ; /* [23..0] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_VEDU_DDR_ADDR_VA_END;

/* Define the union U_VEDU_SMMU_SSID_SAFE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cache_as_ful          : 1   ; /* [0] */
        unsigned int    sc_gid_hint           : 6   ; /* [6..1] */
        unsigned int    sc_new_hint           : 4   ; /* [10..7] */
        unsigned int    cache                 : 4   ; /* [14..11] */
        unsigned int    domain                : 2   ; /* [16..15] */
        unsigned int    reserved_0            : 15  ; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CACHE_AS_FUL;

/* Define the union U_VEDU_SMMU_SSIDV_SAFE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cache_as_ful_safe     : 1   ; /* [0] */
        unsigned int    sc_gid_hint_safe      : 6   ; /* [6..1] */
        unsigned int    sc_new_hint_safe      : 4   ; /* [10..7] */
        unsigned int    cache_safe            : 4   ; /* [14..11] */
        unsigned int    domain_safe           : 2   ; /* [16..15] */
        unsigned int    reserved_0            : 15  ; /* [31..17] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_CACHE_AS_FUL_SAFE;

/* Define the union U_VEDU_REFLD_ESP_HINT_SAFE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sc_esp_hint_l0_safe   : 4   ; /* [3..0]  */
        unsigned int    sc_esp_hint_l1_safe   : 4   ; /* [7..4]  */
        unsigned int    sc_esp_hint_l2_safe   : 4   ; /* [11..8]  */
        unsigned int    sc_esp_hint_en_safe   : 1   ; /* [12]  */
        unsigned int    reserved_0            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_VEDU_REFLD_ESP_HINT_SAFE;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_VEDU_DPM_FREQ_SEL    VEDU_DPM_FREQ_SEL                ; /* 0x0 */
    volatile unsigned int           reserved_0[3]                    ; /* 0x4~0xc */
    volatile U_VEDU_DDR_ADDR_INTERCEPT_EN   VEDU_DDR_ADDR_INTERCEPT_EN       ; /* 0x10 */
    volatile U_VEDU_DDR_ADDR_OVER_STATE_CLR   VEDU_DDR_ADDR_OVER_STATE_CLR[32] ; /* 0x14 */
    volatile U_VEDU_DDR_ADDR_OVER_STATE   VEDU_DDR_ADDR_OVER_STATE[32]     ; /* 0x18 */
    volatile unsigned int           reserved_1                       ; /* 0x1c */
    volatile unsigned int           VEDU_DDR_RADDR_OVER_ADDR[32]     ; /* 0x20 */
    volatile unsigned int           VEDU_DDR_WADDR_OVER_ADDR[32]     ; /* 0x24 */
    volatile U_VEDU_REFLD_ESP_HINT   VEDU_REFLD_ESP_HINT[32]          ; /* 0x28 */
    volatile unsigned int           reserved_2[245]                  ; /* 0x2c~0x3fc */
    volatile U_VEDU_DDR_ADDR_OVERID   VEDU_DDR_ADDR_OVERID[32]         ; /* 0x400 */
    volatile U_VEDU_DDR_QOS         VEDU_DDR_QOS[32]                 ; /* 0x404 */
    volatile unsigned int           reserved_3[62]                   ; /* 0x408~0x4fc */
    volatile U_VEDU_HIAXI_DBG_CTRL   VEDU_HIAXI_DBG_CTRL[32]          ; /* 0x500 */
    volatile U_VEDU_HIAXI_DBG_DLOCK   VEDU_HIAXI_DBG_DLOCK[32]         ; /* 0x504 */
    volatile U_VEDU_HIAXI_IDLE      VEDU_HIAXI_IDLE[32]              ; /* 0x508 */
    volatile U_VEDU_HIAXI_DBG_AWID   VEDU_HIAXI_DBG_AWID[32]          ; /* 0x50c */
    volatile U_VEDU_HIAXI_DBG_AWADDR_0   VEDU_HIAXI_DBG_AWADDR_0[32]      ; /* 0x510 */
    volatile U_VEDU_HIAXI_DBG_AWADDR_1   VEDU_HIAXI_DBG_AWADDR_1[32]      ; /* 0x514 */
    volatile U_VEDU_HIAXI_DBG_AWLEN   VEDU_HIAXI_DBG_AWLEN[32]         ; /* 0x518 */
    volatile U_VEDU_HIAXI_DBG_AWSIZE   VEDU_HIAXI_DBG_AWSIZE[32]        ; /* 0x51c */
    volatile U_VEDU_HIAXI_DBG_AWBURST   VEDU_HIAXI_DBG_AWBURST[32]       ; /* 0x520 */
    volatile U_VEDU_HIAXI_DBG_AWLOCK   VEDU_HIAXI_DBG_AWLOCK[32]        ; /* 0x524 */
    volatile U_VEDU_HIAXI_DBG_AWCACHE   VEDU_HIAXI_DBG_AWCACHE[32]       ; /* 0x528 */
    volatile U_VEDU_HIAXI_DBG_AWPROT   VEDU_HIAXI_DBG_AWPROT[32]        ; /* 0x52c */
    volatile U_VEDU_HIAXI_DBG_AWVALID   VEDU_HIAXI_DBG_AWVALID[32]       ; /* 0x530 */
    volatile U_VEDU_HIAXI_DBG_ARID   VEDU_HIAXI_DBG_ARID[32]          ; /* 0x534 */
    volatile U_VEDU_HIAXI_DBG_ARADDR_0   VEDU_HIAXI_DBG_ARADDR_0[32]      ; /* 0x538 */
    volatile U_VEDU_HIAXI_DBG_ARADDR_1   VEDU_HIAXI_DBG_ARADDR_1[32]      ; /* 0x53c */
    volatile U_VEDU_HIAXI_DBG_ARLEN   VEDU_HIAXI_DBG_ARLEN[32]         ; /* 0x540 */
    volatile U_VEDU_HIAXI_DBG_ARSIZE   VEDU_HIAXI_DBG_ARSIZE[32]        ; /* 0x544 */
    volatile U_VEDU_HIAXI_DBG_ARBURST   VEDU_HIAXI_DBG_ARBURST[32]       ; /* 0x548 */
    volatile U_VEDU_HIAXI_DBG_ARLOCK   VEDU_HIAXI_DBG_ARLOCK[32]        ; /* 0x54c */
    volatile U_VEDU_HIAXI_DBG_ARCACHE   VEDU_HIAXI_DBG_ARCACHE[32]       ; /* 0x550 */
    volatile U_VEDU_HIAXI_DBG_ARPROT   VEDU_HIAXI_DBG_ARPROT[32]        ; /* 0x554 */
    volatile U_VEDU_HIAXI_DBG_ARVALID   VEDU_HIAXI_DBG_ARVALID[32]       ; /* 0x558 */
    volatile U_VEDU_HIAXI_DBG_ERR   VEDU_HIAXI_DBG_ERR[32]           ; /* 0x55c */
    volatile unsigned int           reserved_4[40]                   ; /* 0x560~0x5fc */
    volatile U_VEDU_DDR_ADDR_VA_STR   VEDU_DDR_ADDR_VA_STR[32]         ; /* 0x600~0x778 */
    volatile unsigned int           reserved_5[33]                   ; /* 0x77c~0x7fc */
    volatile U_VEDU_DDR_ADDR_VA_END   VEDU_DDR_ADDR_VA_END[32]         ; /* 0x800~0x978 */
    volatile unsigned int           reserved_6[33]                   ; /* 0x97c~0x9fc */
    volatile U_VEDU_CACHE_AS_FUL    VEDU_CACHE_AS_FUL[32]            ; /* 0xa00~0xb78 */
    volatile unsigned int           reserved_7[33]                   ; /* 0xb7c~0xbfc */
    volatile U_VEDU_CACHE_AS_FUL_SAFE   VEDU_CACHE_AS_FUL_SAFE[32]       ; /* 0xc00~0xd78 */
    volatile unsigned int           reserved_8[101]                  ; /* 0xd7c~0xf0c */
    volatile U_VEDU_REFLD_ESP_HINT_SAFE   VEDU_REFLD_ESP_HINT_SAFE[32]     ; /* 0xf10 */
} S_SMMU_PRE_REGS_TYPE;

#endif /* __SMMU_PRE_H__ */
