// ******************************************************************************
// Copyright     :  Copyright (C) 2020, Hisilicon Technologies Co. Ltd.
// File name     :  cmdlst.h
// Project line  :
// Department    :  K5
// Author        :  AnthonySixta
// Version       :  1.0
// Date          :  2013/5/31
// Description   :  The description of xxx project
// Others        :  Generated automatically by nManager V4.2
// History       :  AnthonySixta 2020/06/02 09:28:28 Create file
// ******************************************************************************

#ifndef __CMDLST_H__
#define __CMDLST_H__

/* Define the union U_CMDLST_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    force_clk_on          : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMDLST_CFG;

/* Define the union U_CMDLST_SW_BUF_LEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    sw_cfg_buf_len0       : 16  ; /* [15..0]  */
        unsigned int    sw_cfg_buf_len1       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMDLST_SW_BUF_LEN;

/* Define the union U_CMDLST_SW_CFG */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    branching             : 1   ; /* [0]  */
        unsigned int    reserved_0            : 31  ; /* [31..1]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMDLST_SW_CFG;

/* Define the union U_CMDLST_HW_BUF_LEN */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    hw_cfg_buf_len0       : 16  ; /* [15..0]  */
        unsigned int    hw_cfg_buf_len1       : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_CMDLST_HW_BUF_LEN;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile U_CMDLST_CFG           CMDLST_CFG;
    volatile unsigned int           RESERVED0[3];
    volatile unsigned int           CMDLST_SW_AXI_ADDR;
    volatile U_CMDLST_SW_BUF_LEN    CMDLST_SW_BUF_LEN;
    volatile U_CMDLST_SW_CFG        CMDLST_SW_CFG;
    volatile unsigned int           RESERVED1;
    volatile unsigned int           CMDLST_HW_AXI_ADDR;
    volatile U_CMDLST_HW_BUF_LEN    CMDLST_HW_BUF_LEN;
    volatile unsigned int           CMDLST_HW_AXI_ADDR_DUMP;
    volatile unsigned int           RESERVED2;
    volatile unsigned int           CMDLST_AXI_ADDR_RO;
    volatile unsigned int           CMDLST_OTHER_RO0;
    volatile unsigned int           CMDLST_OTHER_RO1;
    volatile unsigned int           RESERVED3[1009];
} S_CMDLST_REGS_TYPE;

#endif /* __CMDLST_H__ */
