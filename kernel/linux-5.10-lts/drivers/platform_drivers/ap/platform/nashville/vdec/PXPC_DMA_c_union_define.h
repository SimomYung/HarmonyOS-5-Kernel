/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  PXPC_DMA_c_union_define.h
 * Project line  :  Platform And Key Technologies Development
 * Department    :  CAD Development Department
 * Author        :  wushengtao/yanmengting/jieguangwen
 * Version       :  1.0
 * Date          :  2022/3/1
 * Description   :  The description of xxx project
 * Others        :  Generated automatically by nManager V5.1 
 * History       :  wushengtao/yanmengting/jieguangwen 2023/11/14 14:29:16 Create file
 */

#ifndef __PXPC_DMA_C_UNION_DEFINE_H__
#define __PXPC_DMA_C_UNION_DEFINE_H__

/* Define the union U_SYS_CACHE_BUF_SADDR */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: syscache_buf_saddr
        /*
         * 软件分配给pxpc的syscache起始地址，单位128Byte，
         * 地址范围为0~0xF_FFFF_FFFF Byte
         */
        unsigned int    syscache_buf_saddr    : 29  ; /* [28..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 3   ; /* [31..29] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SYS_CACHE_BUF_SADDR;

/* Define the union U_SYS_CACHE_BUF_LEN */
typedef union {
    /* Define the struct bits */
    struct {
        // Comment of field: syscache_buf_len
        /*
         * syscache buf长度，最大1MB，单位128Byte。单个buf的长度，每次申请2个buf，所以该值为申请长度/2/128-1
         */
        unsigned int    syscache_buf_len      : 13  ; /* [12..0] */
        // Comment of field: reserved_0
        /*
         * 保留
         */
        unsigned int    reserved_0            : 19  ; /* [31..13] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} U_SYS_CACHE_BUF_LEN;

//==============================================================================
/* Define the global struct */
typedef struct {
    unsigned int           reserved_0[3]                    ; /* 0x0~0x8 */
    U_SYS_CACHE_BUF_SADDR   SYS_CACHE_BUF_SADDR              ; /* 0xc */
    U_SYS_CACHE_BUF_LEN    SYS_CACHE_BUF_LEN                ; /* 0x10 */
} S_PXPC_DMA_REGS_TYPE;

/* Declare the struct pointer of the module PXPC_DMA */
extern S_PXPC_DMA_REGS_TYPE *gopPXPC_DMAAllReg;

/* Declare the functions that set the member value */
int iSetSYS_CACHE_BUF_SADDRsyscache_buf_saddr(unsigned int usyscache_buf_saddr);


#endif /* __PXPC_DMA_C_UNION_DEFINE_H__ */
