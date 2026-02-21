/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_lock_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_LOCK_INTERFACE_H__
#define __SOC_LOCK_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) reg_define
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：通用资源锁定寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_LOCK_UNION */
#define SOC_LOCK_RESOURCE0_LOCK_ADDR(base)                    ((base) + (0x000UL))

/* 寄存器说明：通用资源解锁寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE0_UNLOCK_ADDR(base)                  ((base) + (0x004UL))

/* 寄存器说明：通用资源锁状态寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE0_LOCK_ST_ADDR(base)                 ((base) + (0x008UL))

/* 寄存器说明：通用资源锁定寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_LOCK_UNION */
#define SOC_LOCK_RESOURCE1_LOCK_ADDR(base)                    ((base) + (0x00CUL))

/* 寄存器说明：通用资源解锁寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE1_UNLOCK_ADDR(base)                  ((base) + (0x010UL))

/* 寄存器说明：通用资源锁状态寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE1_LOCK_ST_ADDR(base)                 ((base) + (0x014UL))

/* 寄存器说明：通用资源锁定寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_LOCK_UNION */
#define SOC_LOCK_RESOURCE2_LOCK_ADDR(base)                    ((base) + (0x018UL))

/* 寄存器说明：通用资源解锁寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE2_UNLOCK_ADDR(base)                  ((base) + (0x01CUL))

/* 寄存器说明：通用资源锁状态寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE2_LOCK_ST_ADDR(base)                 ((base) + (0x020UL))

/* 寄存器说明：通用资源锁定寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_LOCK_UNION */
#define SOC_LOCK_RESOURCE3_LOCK_ADDR(base)                    ((base) + (0x024UL))

/* 寄存器说明：通用资源解锁寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE3_UNLOCK_ADDR(base)                  ((base) + (0x028UL))

/* 寄存器说明：通用资源锁状态寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE3_LOCK_ST_ADDR(base)                 ((base) + (0x02CUL))

/* 寄存器说明：通用资源锁定寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_LOCK_UNION */
#define SOC_LOCK_RESOURCE4_LOCK_ADDR(base)                    ((base) + (0x400UL))

/* 寄存器说明：通用资源解锁寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE4_UNLOCK_ADDR(base)                  ((base) + (0x404UL))

/* 寄存器说明：通用资源锁状态寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE4_LOCK_ST_ADDR(base)                 ((base) + (0x408UL))

/* 寄存器说明：通用资源锁定寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_LOCK_UNION */
#define SOC_LOCK_RESOURCE5_LOCK_ADDR(base)                    ((base) + (0x40CUL))

/* 寄存器说明：通用资源解锁寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE5_UNLOCK_ADDR(base)                  ((base) + (0x410UL))

/* 寄存器说明：通用资源锁状态寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE5_LOCK_ST_ADDR(base)                 ((base) + (0x414UL))

/* 寄存器说明：通用资源锁定寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_LOCK_UNION */
#define SOC_LOCK_RESOURCE6_LOCK_ADDR(base)                    ((base) + (0x418UL))

/* 寄存器说明：通用资源解锁寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE6_UNLOCK_ADDR(base)                  ((base) + (0x41CUL))

/* 寄存器说明：通用资源锁状态寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE6_LOCK_ST_ADDR(base)                 ((base) + (0x420UL))

/* 寄存器说明：通用资源锁定寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_LOCK_UNION */
#define SOC_LOCK_RESOURCE7_LOCK_ADDR(base)                    ((base) + (0x424UL))

/* 寄存器说明：通用资源解锁寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE7_UNLOCK_ADDR(base)                  ((base) + (0x428UL))

/* 寄存器说明：通用资源锁状态寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE7_LOCK_ST_ADDR(base)                 ((base) + (0x42CUL))


#else


/* 寄存器说明：通用资源锁定寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_LOCK_UNION */
#define SOC_LOCK_RESOURCE0_LOCK_ADDR(base)                    ((base) + (0x000))

/* 寄存器说明：通用资源解锁寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE0_UNLOCK_ADDR(base)                  ((base) + (0x004))

/* 寄存器说明：通用资源锁状态寄存器0。
   位域定义UNION结构:  SOC_LOCK_RESOURCE0_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE0_LOCK_ST_ADDR(base)                 ((base) + (0x008))

/* 寄存器说明：通用资源锁定寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_LOCK_UNION */
#define SOC_LOCK_RESOURCE1_LOCK_ADDR(base)                    ((base) + (0x00C))

/* 寄存器说明：通用资源解锁寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE1_UNLOCK_ADDR(base)                  ((base) + (0x010))

/* 寄存器说明：通用资源锁状态寄存器1。
   位域定义UNION结构:  SOC_LOCK_RESOURCE1_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE1_LOCK_ST_ADDR(base)                 ((base) + (0x014))

/* 寄存器说明：通用资源锁定寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_LOCK_UNION */
#define SOC_LOCK_RESOURCE2_LOCK_ADDR(base)                    ((base) + (0x018))

/* 寄存器说明：通用资源解锁寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE2_UNLOCK_ADDR(base)                  ((base) + (0x01C))

/* 寄存器说明：通用资源锁状态寄存器2。
   位域定义UNION结构:  SOC_LOCK_RESOURCE2_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE2_LOCK_ST_ADDR(base)                 ((base) + (0x020))

/* 寄存器说明：通用资源锁定寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_LOCK_UNION */
#define SOC_LOCK_RESOURCE3_LOCK_ADDR(base)                    ((base) + (0x024))

/* 寄存器说明：通用资源解锁寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE3_UNLOCK_ADDR(base)                  ((base) + (0x028))

/* 寄存器说明：通用资源锁状态寄存器3。
   位域定义UNION结构:  SOC_LOCK_RESOURCE3_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE3_LOCK_ST_ADDR(base)                 ((base) + (0x02C))

/* 寄存器说明：通用资源锁定寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_LOCK_UNION */
#define SOC_LOCK_RESOURCE4_LOCK_ADDR(base)                    ((base) + (0x400))

/* 寄存器说明：通用资源解锁寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE4_UNLOCK_ADDR(base)                  ((base) + (0x404))

/* 寄存器说明：通用资源锁状态寄存器4。
   位域定义UNION结构:  SOC_LOCK_RESOURCE4_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE4_LOCK_ST_ADDR(base)                 ((base) + (0x408))

/* 寄存器说明：通用资源锁定寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_LOCK_UNION */
#define SOC_LOCK_RESOURCE5_LOCK_ADDR(base)                    ((base) + (0x40C))

/* 寄存器说明：通用资源解锁寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE5_UNLOCK_ADDR(base)                  ((base) + (0x410))

/* 寄存器说明：通用资源锁状态寄存器5。
   位域定义UNION结构:  SOC_LOCK_RESOURCE5_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE5_LOCK_ST_ADDR(base)                 ((base) + (0x414))

/* 寄存器说明：通用资源锁定寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_LOCK_UNION */
#define SOC_LOCK_RESOURCE6_LOCK_ADDR(base)                    ((base) + (0x418))

/* 寄存器说明：通用资源解锁寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE6_UNLOCK_ADDR(base)                  ((base) + (0x41C))

/* 寄存器说明：通用资源锁状态寄存器6。
   位域定义UNION结构:  SOC_LOCK_RESOURCE6_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE6_LOCK_ST_ADDR(base)                 ((base) + (0x420))

/* 寄存器说明：通用资源锁定寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_LOCK_UNION */
#define SOC_LOCK_RESOURCE7_LOCK_ADDR(base)                    ((base) + (0x424))

/* 寄存器说明：通用资源解锁寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_UNLOCK_UNION */
#define SOC_LOCK_RESOURCE7_UNLOCK_ADDR(base)                  ((base) + (0x428))

/* 寄存器说明：通用资源锁状态寄存器7。
   位域定义UNION结构:  SOC_LOCK_RESOURCE7_LOCK_ST_UNION */
#define SOC_LOCK_RESOURCE7_LOCK_ST_ADDR(base)                 ((base) + (0x42C))


#endif




/*****************************************************************************
  3 枚举定义
*****************************************************************************/



/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/



/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/



/*****************************************************************************
  7 UNION定义
*****************************************************************************/

/****************************************************************************
                     (1/1) reg_define
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE0_LOCK_UNION
 结构说明  : RESOURCE0_LOCK 寄存器结构定义。地址偏移量:0x000，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource0lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的锁定master ID号0。 */
        unsigned int  resource0lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的锁定master ID号1。 */
        unsigned int  resource0lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id2  : 3;  /* bit[9-11] : 通用资源锁0的锁定master ID号2。 */
        unsigned int  resource0lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id3  : 3;  /* bit[13-15]: 通用资源锁0的锁定master ID号3。 */
        unsigned int  resource0lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id4  : 3;  /* bit[17-19]: 通用资源锁0的锁定master ID号4。 */
        unsigned int  resource0lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id5  : 3;  /* bit[21-23]: 通用资源锁0的锁定master ID号5。 */
        unsigned int  resource0lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id6  : 3;  /* bit[25-27]: 通用资源锁0的锁定master ID号6。 */
        unsigned int  resource0lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource0lock_id7  : 3;  /* bit[29-31]: 通用资源锁0的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE0_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id0_START   (1)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id0_END     (3)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id1_START   (5)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id1_END     (7)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id2_START   (9)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id2_END     (11)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id3_START   (13)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id3_END     (15)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id4_START   (17)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id4_END     (19)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id5_START   (21)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id5_END     (23)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id6_START   (25)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id6_END     (27)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id7_START   (29)
#define SOC_LOCK_RESOURCE0_LOCK_resource0lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE0_UNLOCK_UNION
 结构说明  : RESOURCE0_UNLOCK 寄存器结构定义。地址偏移量:0x004，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource0unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁0的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁0的解锁master ID号0。 */
        unsigned int  resource0unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁0的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁0的解锁master ID号1。 */
        unsigned int  resource0unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁0的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id2  : 3;  /* bit[9-11] : 通用资源锁0的解锁master ID号2。 */
        unsigned int  resource0unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁0的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id3  : 3;  /* bit[13-15]: 通用资源锁0的解锁master ID号3。 */
        unsigned int  resource0unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁0的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id4  : 3;  /* bit[17-19]: 通用资源锁0的解锁master ID号4。 */
        unsigned int  resource0unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁0的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id5  : 3;  /* bit[21-23]: 通用资源锁0的解锁master ID号5。 */
        unsigned int  resource0unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁0的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id6  : 3;  /* bit[25-27]: 通用资源锁0的解锁master ID号6。 */
        unsigned int  resource0unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁0的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource0unlock_id7  : 3;  /* bit[29-31]: 通用资源锁0的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE0_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE0_UNLOCK_resource0unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE0_LOCK_ST_UNION
 结构说明  : RESOURCE0_LOCK_ST 寄存器结构定义。地址偏移量:0x008，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器0。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource0lock_st0    : 1;  /* bit[0-0]  : 通用资源锁0的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁0的锁定状态master ID号0。 */
        unsigned int  resource0lock_st1    : 1;  /* bit[4-4]  : 通用资源锁0的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁0的锁定状态master ID号1。 */
        unsigned int  resource0lock_st2    : 1;  /* bit[8-8]  : 通用资源锁0的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁0的锁定状态master ID号2。 */
        unsigned int  resource0lock_st3    : 1;  /* bit[12-12]: 通用资源锁0的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁0的锁定状态master ID号3。 */
        unsigned int  resource0lock_st4    : 1;  /* bit[16-16]: 通用资源锁0的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁0的锁定状态master ID号4。 */
        unsigned int  resource0lock_st5    : 1;  /* bit[20-20]: 通用资源锁0的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁0的锁定状态master ID号5。 */
        unsigned int  resource0lock_st6    : 1;  /* bit[24-24]: 通用资源锁0的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁0的锁定状态master ID号6。 */
        unsigned int  resource0lock_st7    : 1;  /* bit[28-28]: 通用资源锁0的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource0lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁0的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE0_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st0_START     (0)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st0_END       (0)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st1_START     (4)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st1_END       (4)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st2_START     (8)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st2_END       (8)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st3_START     (12)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st3_END       (12)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st4_START     (16)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st4_END       (16)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st5_START     (20)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st5_END       (20)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st6_START     (24)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st6_END       (24)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st7_START     (28)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st7_END       (28)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE0_LOCK_ST_resource0lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE1_LOCK_UNION
 结构说明  : RESOURCE1_LOCK 寄存器结构定义。地址偏移量:0x00C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource1lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁1的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id0  : 3;  /* bit[1-3]  : 通用资源锁1的锁定master ID号0。 */
        unsigned int  resource1lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁1的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id1  : 3;  /* bit[5-7]  : 通用资源锁1的锁定master ID号1。 */
        unsigned int  resource1lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁1的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id2  : 3;  /* bit[9-11] : 通用资源锁1的锁定master ID号2。 */
        unsigned int  resource1lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁1的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id3  : 3;  /* bit[13-15]: 通用资源锁1的锁定master ID号3。 */
        unsigned int  resource1lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁1的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id4  : 3;  /* bit[17-19]: 通用资源锁1的锁定master ID号4。 */
        unsigned int  resource1lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁1的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id5  : 3;  /* bit[21-23]: 通用资源锁1的锁定master ID号5。 */
        unsigned int  resource1lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁1的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id6  : 3;  /* bit[25-27]: 通用资源锁1的锁定master ID号6。 */
        unsigned int  resource1lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁1的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource1lock_id7  : 3;  /* bit[29-31]: 通用资源锁1的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE1_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id0_START   (1)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id0_END     (3)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id1_START   (5)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id1_END     (7)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id2_START   (9)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id2_END     (11)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id3_START   (13)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id3_END     (15)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id4_START   (17)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id4_END     (19)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id5_START   (21)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id5_END     (23)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id6_START   (25)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id6_END     (27)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id7_START   (29)
#define SOC_LOCK_RESOURCE1_LOCK_resource1lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE1_UNLOCK_UNION
 结构说明  : RESOURCE1_UNLOCK 寄存器结构定义。地址偏移量:0x010，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource1unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁1的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁1的解锁master ID号0。 */
        unsigned int  resource1unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁1的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁1的解锁master ID号1。 */
        unsigned int  resource1unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁1的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id2  : 3;  /* bit[9-11] : 通用资源锁1的解锁master ID号2。 */
        unsigned int  resource1unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁1的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id3  : 3;  /* bit[13-15]: 通用资源锁1的解锁master ID号3。 */
        unsigned int  resource1unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁1的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id4  : 3;  /* bit[17-19]: 通用资源锁1的解锁master ID号4。 */
        unsigned int  resource1unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁1的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id5  : 3;  /* bit[21-23]: 通用资源锁1的解锁master ID号5。 */
        unsigned int  resource1unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁1的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id6  : 3;  /* bit[25-27]: 通用资源锁1的解锁master ID号6。 */
        unsigned int  resource1unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁1的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource1unlock_id7  : 3;  /* bit[29-31]: 通用资源锁1的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE1_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE1_UNLOCK_resource1unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE1_LOCK_ST_UNION
 结构说明  : RESOURCE1_LOCK_ST 寄存器结构定义。地址偏移量:0x014，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器1。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource1lock_st0    : 1;  /* bit[0-0]  : 通用资源锁1的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁1的锁定状态master ID号0。 */
        unsigned int  resource1lock_st1    : 1;  /* bit[4-4]  : 通用资源锁1的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁1的锁定状态master ID号1。 */
        unsigned int  resource1lock_st2    : 1;  /* bit[8-8]  : 通用资源锁1的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁1的锁定状态master ID号2。 */
        unsigned int  resource1lock_st3    : 1;  /* bit[12-12]: 通用资源锁1的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁1的锁定状态master ID号3。 */
        unsigned int  resource1lock_st4    : 1;  /* bit[16-16]: 通用资源锁1的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁1的锁定状态master ID号4。 */
        unsigned int  resource1lock_st5    : 1;  /* bit[20-20]: 通用资源锁1的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁1的锁定状态master ID号5。 */
        unsigned int  resource1lock_st6    : 1;  /* bit[24-24]: 通用资源锁1的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁1的锁定状态master ID号6。 */
        unsigned int  resource1lock_st7    : 1;  /* bit[28-28]: 通用资源锁1的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource1lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁1的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE1_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st0_START     (0)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st0_END       (0)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st1_START     (4)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st1_END       (4)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st2_START     (8)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st2_END       (8)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st3_START     (12)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st3_END       (12)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st4_START     (16)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st4_END       (16)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st5_START     (20)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st5_END       (20)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st6_START     (24)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st6_END       (24)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st7_START     (28)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st7_END       (28)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE1_LOCK_ST_resource1lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE2_LOCK_UNION
 结构说明  : RESOURCE2_LOCK 寄存器结构定义。地址偏移量:0x018，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource2lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁2的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id0  : 3;  /* bit[1-3]  : 通用资源锁2的锁定master ID号0。 */
        unsigned int  resource2lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁2的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id1  : 3;  /* bit[5-7]  : 通用资源锁2的锁定master ID号1。 */
        unsigned int  resource2lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁2的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id2  : 3;  /* bit[9-11] : 通用资源锁2的锁定master ID号2。 */
        unsigned int  resource2lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁2的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id3  : 3;  /* bit[13-15]: 通用资源锁2的锁定master ID号3。 */
        unsigned int  resource2lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁2的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id4  : 3;  /* bit[17-19]: 通用资源锁2的锁定master ID号4。 */
        unsigned int  resource2lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁2的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id5  : 3;  /* bit[21-23]: 通用资源锁2的锁定master ID号5。 */
        unsigned int  resource2lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁2的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id6  : 3;  /* bit[25-27]: 通用资源锁2的锁定master ID号6。 */
        unsigned int  resource2lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁2的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource2lock_id7  : 3;  /* bit[29-31]: 通用资源锁2的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE2_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id0_START   (1)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id0_END     (3)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id1_START   (5)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id1_END     (7)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id2_START   (9)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id2_END     (11)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id3_START   (13)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id3_END     (15)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id4_START   (17)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id4_END     (19)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id5_START   (21)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id5_END     (23)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id6_START   (25)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id6_END     (27)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id7_START   (29)
#define SOC_LOCK_RESOURCE2_LOCK_resource2lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE2_UNLOCK_UNION
 结构说明  : RESOURCE2_UNLOCK 寄存器结构定义。地址偏移量:0x01C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource2unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁2的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁2的解锁master ID号0。 */
        unsigned int  resource2unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁2的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁2的解锁master ID号1。 */
        unsigned int  resource2unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁2的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id2  : 3;  /* bit[9-11] : 通用资源锁2的解锁master ID号2。 */
        unsigned int  resource2unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁2的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id3  : 3;  /* bit[13-15]: 通用资源锁2的解锁master ID号3。 */
        unsigned int  resource2unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁2的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id4  : 3;  /* bit[17-19]: 通用资源锁2的解锁master ID号4。 */
        unsigned int  resource2unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁2的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id5  : 3;  /* bit[21-23]: 通用资源锁2的解锁master ID号5。 */
        unsigned int  resource2unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁2的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id6  : 3;  /* bit[25-27]: 通用资源锁2的解锁master ID号6。 */
        unsigned int  resource2unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁2的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource2unlock_id7  : 3;  /* bit[29-31]: 通用资源锁2的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE2_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE2_UNLOCK_resource2unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE2_LOCK_ST_UNION
 结构说明  : RESOURCE2_LOCK_ST 寄存器结构定义。地址偏移量:0x020，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器2。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource2lock_st0    : 1;  /* bit[0-0]  : 通用资源锁2的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁2的锁定状态master ID号0。 */
        unsigned int  resource2lock_st1    : 1;  /* bit[4-4]  : 通用资源锁2的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁2的锁定状态master ID号1。 */
        unsigned int  resource2lock_st2    : 1;  /* bit[8-8]  : 通用资源锁2的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁2的锁定状态master ID号2。 */
        unsigned int  resource2lock_st3    : 1;  /* bit[12-12]: 通用资源锁2的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁2的锁定状态master ID号3。 */
        unsigned int  resource2lock_st4    : 1;  /* bit[16-16]: 通用资源锁2的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁2的锁定状态master ID号4。 */
        unsigned int  resource2lock_st5    : 1;  /* bit[20-20]: 通用资源锁2的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁2的锁定状态master ID号5。 */
        unsigned int  resource2lock_st6    : 1;  /* bit[24-24]: 通用资源锁2的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁2的锁定状态master ID号6。 */
        unsigned int  resource2lock_st7    : 1;  /* bit[28-28]: 通用资源锁2的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource2lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁2的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE2_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st0_START     (0)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st0_END       (0)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st1_START     (4)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st1_END       (4)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st2_START     (8)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st2_END       (8)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st3_START     (12)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st3_END       (12)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st4_START     (16)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st4_END       (16)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st5_START     (20)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st5_END       (20)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st6_START     (24)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st6_END       (24)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st7_START     (28)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st7_END       (28)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE2_LOCK_ST_resource2lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE3_LOCK_UNION
 结构说明  : RESOURCE3_LOCK 寄存器结构定义。地址偏移量:0x024，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource3lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁3的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id0  : 3;  /* bit[1-3]  : 通用资源锁3的锁定master ID号0。 */
        unsigned int  resource3lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁3的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id1  : 3;  /* bit[5-7]  : 通用资源锁3的锁定master ID号1。 */
        unsigned int  resource3lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁3的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id2  : 3;  /* bit[9-11] : 通用资源锁3的锁定master ID号2。 */
        unsigned int  resource3lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁3的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id3  : 3;  /* bit[13-15]: 通用资源锁3的锁定master ID号3。 */
        unsigned int  resource3lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁3的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id4  : 3;  /* bit[17-19]: 通用资源锁3的锁定master ID号4。 */
        unsigned int  resource3lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁3的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id5  : 3;  /* bit[21-23]: 通用资源锁3的锁定master ID号5。 */
        unsigned int  resource3lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁3的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id6  : 3;  /* bit[25-27]: 通用资源锁3的锁定master ID号6。 */
        unsigned int  resource3lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁3的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource3lock_id7  : 3;  /* bit[29-31]: 通用资源锁3的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE3_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id0_START   (1)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id0_END     (3)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id1_START   (5)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id1_END     (7)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id2_START   (9)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id2_END     (11)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id3_START   (13)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id3_END     (15)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id4_START   (17)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id4_END     (19)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id5_START   (21)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id5_END     (23)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id6_START   (25)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id6_END     (27)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id7_START   (29)
#define SOC_LOCK_RESOURCE3_LOCK_resource3lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE3_UNLOCK_UNION
 结构说明  : RESOURCE3_UNLOCK 寄存器结构定义。地址偏移量:0x028，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource3unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁3的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁3的解锁master ID号0。 */
        unsigned int  resource3unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁3的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁3的解锁master ID号1。 */
        unsigned int  resource3unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁3的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id2  : 3;  /* bit[9-11] : 通用资源锁3的解锁master ID号2。 */
        unsigned int  resource3unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁3的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id3  : 3;  /* bit[13-15]: 通用资源锁3的解锁master ID号3。 */
        unsigned int  resource3unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁3的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id4  : 3;  /* bit[17-19]: 通用资源锁3的解锁master ID号4。 */
        unsigned int  resource3unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁3的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id5  : 3;  /* bit[21-23]: 通用资源锁3的解锁master ID号5。 */
        unsigned int  resource3unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁3的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id6  : 3;  /* bit[25-27]: 通用资源锁3的解锁master ID号6。 */
        unsigned int  resource3unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁3的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource3unlock_id7  : 3;  /* bit[29-31]: 通用资源锁3的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE3_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE3_UNLOCK_resource3unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE3_LOCK_ST_UNION
 结构说明  : RESOURCE3_LOCK_ST 寄存器结构定义。地址偏移量:0x02C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器3。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource3lock_st0    : 1;  /* bit[0-0]  : 通用资源锁3的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁3的锁定状态master ID号0。 */
        unsigned int  resource3lock_st1    : 1;  /* bit[4-4]  : 通用资源锁3的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁3的锁定状态master ID号1。 */
        unsigned int  resource3lock_st2    : 1;  /* bit[8-8]  : 通用资源锁3的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁3的锁定状态master ID号2。 */
        unsigned int  resource3lock_st3    : 1;  /* bit[12-12]: 通用资源锁3的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁3的锁定状态master ID号3。 */
        unsigned int  resource3lock_st4    : 1;  /* bit[16-16]: 通用资源锁3的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁3的锁定状态master ID号4。 */
        unsigned int  resource3lock_st5    : 1;  /* bit[20-20]: 通用资源锁3的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁3的锁定状态master ID号5。 */
        unsigned int  resource3lock_st6    : 1;  /* bit[24-24]: 通用资源锁3的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁3的锁定状态master ID号6。 */
        unsigned int  resource3lock_st7    : 1;  /* bit[28-28]: 通用资源锁3的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource3lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁3的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE3_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st0_START     (0)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st0_END       (0)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st1_START     (4)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st1_END       (4)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st2_START     (8)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st2_END       (8)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st3_START     (12)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st3_END       (12)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st4_START     (16)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st4_END       (16)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st5_START     (20)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st5_END       (20)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st6_START     (24)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st6_END       (24)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st7_START     (28)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st7_END       (28)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE3_LOCK_ST_resource3lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE4_LOCK_UNION
 结构说明  : RESOURCE4_LOCK 寄存器结构定义。地址偏移量:0x400，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource4lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁4的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id0  : 3;  /* bit[1-3]  : 通用资源锁4的锁定master ID号0。 */
        unsigned int  resource4lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁4的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id1  : 3;  /* bit[5-7]  : 通用资源锁4的锁定master ID号1。 */
        unsigned int  resource4lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁4的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id2  : 3;  /* bit[9-11] : 通用资源锁4的锁定master ID号2。 */
        unsigned int  resource4lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁4的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id3  : 3;  /* bit[13-15]: 通用资源锁4的锁定master ID号3。 */
        unsigned int  resource4lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁4的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id4  : 3;  /* bit[17-19]: 通用资源锁4的锁定master ID号4。 */
        unsigned int  resource4lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁4的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id5  : 3;  /* bit[21-23]: 通用资源锁4的锁定master ID号5。 */
        unsigned int  resource4lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁4的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id6  : 3;  /* bit[25-27]: 通用资源锁4的锁定master ID号6。 */
        unsigned int  resource4lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁4的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource4lock_id7  : 3;  /* bit[29-31]: 通用资源锁4的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE4_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id0_START   (1)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id0_END     (3)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id1_START   (5)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id1_END     (7)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id2_START   (9)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id2_END     (11)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id3_START   (13)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id3_END     (15)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id4_START   (17)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id4_END     (19)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id5_START   (21)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id5_END     (23)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id6_START   (25)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id6_END     (27)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id7_START   (29)
#define SOC_LOCK_RESOURCE4_LOCK_resource4lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE4_UNLOCK_UNION
 结构说明  : RESOURCE4_UNLOCK 寄存器结构定义。地址偏移量:0x404，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource4unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁4的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁4的解锁master ID号0。 */
        unsigned int  resource4unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁4的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁4的解锁master ID号1。 */
        unsigned int  resource4unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁4的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id2  : 3;  /* bit[9-11] : 通用资源锁4的解锁master ID号2。 */
        unsigned int  resource4unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁4的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id3  : 3;  /* bit[13-15]: 通用资源锁4的解锁master ID号3。 */
        unsigned int  resource4unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁4的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id4  : 3;  /* bit[17-19]: 通用资源锁4的解锁master ID号4。 */
        unsigned int  resource4unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁4的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id5  : 3;  /* bit[21-23]: 通用资源锁4的解锁master ID号5。 */
        unsigned int  resource4unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁4的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id6  : 3;  /* bit[25-27]: 通用资源锁4的解锁master ID号6。 */
        unsigned int  resource4unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁4的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource4unlock_id7  : 3;  /* bit[29-31]: 通用资源锁4的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE4_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE4_UNLOCK_resource4unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE4_LOCK_ST_UNION
 结构说明  : RESOURCE4_LOCK_ST 寄存器结构定义。地址偏移量:0x408，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器4。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource4lock_st0    : 1;  /* bit[0-0]  : 通用资源锁4的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁4的锁定状态master ID号0。 */
        unsigned int  resource4lock_st1    : 1;  /* bit[4-4]  : 通用资源锁4的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁4的锁定状态master ID号1。 */
        unsigned int  resource4lock_st2    : 1;  /* bit[8-8]  : 通用资源锁4的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁4的锁定状态master ID号2。 */
        unsigned int  resource4lock_st3    : 1;  /* bit[12-12]: 通用资源锁4的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁4的锁定状态master ID号3。 */
        unsigned int  resource4lock_st4    : 1;  /* bit[16-16]: 通用资源锁4的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁4的锁定状态master ID号4。 */
        unsigned int  resource4lock_st5    : 1;  /* bit[20-20]: 通用资源锁4的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁4的锁定状态master ID号5。 */
        unsigned int  resource4lock_st6    : 1;  /* bit[24-24]: 通用资源锁4的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁4的锁定状态master ID号6。 */
        unsigned int  resource4lock_st7    : 1;  /* bit[28-28]: 通用资源锁4的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource4lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁4的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE4_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st0_START     (0)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st0_END       (0)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st1_START     (4)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st1_END       (4)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st2_START     (8)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st2_END       (8)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st3_START     (12)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st3_END       (12)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st4_START     (16)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st4_END       (16)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st5_START     (20)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st5_END       (20)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st6_START     (24)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st6_END       (24)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st7_START     (28)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st7_END       (28)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE4_LOCK_ST_resource4lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE5_LOCK_UNION
 结构说明  : RESOURCE5_LOCK 寄存器结构定义。地址偏移量:0x40C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource5lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁5的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id0  : 3;  /* bit[1-3]  : 通用资源锁5的锁定master ID号0。 */
        unsigned int  resource5lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁5的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id1  : 3;  /* bit[5-7]  : 通用资源锁5的锁定master ID号1。 */
        unsigned int  resource5lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁5的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id2  : 3;  /* bit[9-11] : 通用资源锁5的锁定master ID号2。 */
        unsigned int  resource5lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁5的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id3  : 3;  /* bit[13-15]: 通用资源锁5的锁定master ID号3。 */
        unsigned int  resource5lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁5的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id4  : 3;  /* bit[17-19]: 通用资源锁5的锁定master ID号4。 */
        unsigned int  resource5lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁5的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id5  : 3;  /* bit[21-23]: 通用资源锁5的锁定master ID号5。 */
        unsigned int  resource5lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁5的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id6  : 3;  /* bit[25-27]: 通用资源锁5的锁定master ID号6。 */
        unsigned int  resource5lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁5的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource5lock_id7  : 3;  /* bit[29-31]: 通用资源锁5的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE5_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id0_START   (1)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id0_END     (3)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id1_START   (5)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id1_END     (7)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id2_START   (9)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id2_END     (11)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id3_START   (13)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id3_END     (15)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id4_START   (17)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id4_END     (19)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id5_START   (21)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id5_END     (23)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id6_START   (25)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id6_END     (27)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id7_START   (29)
#define SOC_LOCK_RESOURCE5_LOCK_resource5lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE5_UNLOCK_UNION
 结构说明  : RESOURCE5_UNLOCK 寄存器结构定义。地址偏移量:0x410，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource5unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁5的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁5的解锁master ID号0。 */
        unsigned int  resource5unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁5的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁5的解锁master ID号1。 */
        unsigned int  resource5unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁5的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id2  : 3;  /* bit[9-11] : 通用资源锁5的解锁master ID号2。 */
        unsigned int  resource5unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁5的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id3  : 3;  /* bit[13-15]: 通用资源锁5的解锁master ID号3。 */
        unsigned int  resource5unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁5的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id4  : 3;  /* bit[17-19]: 通用资源锁5的解锁master ID号4。 */
        unsigned int  resource5unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁5的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id5  : 3;  /* bit[21-23]: 通用资源锁5的解锁master ID号5。 */
        unsigned int  resource5unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁5的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id6  : 3;  /* bit[25-27]: 通用资源锁5的解锁master ID号6。 */
        unsigned int  resource5unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁5的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource5unlock_id7  : 3;  /* bit[29-31]: 通用资源锁5的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE5_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE5_UNLOCK_resource5unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE5_LOCK_ST_UNION
 结构说明  : RESOURCE5_LOCK_ST 寄存器结构定义。地址偏移量:0x414，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器5。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource5lock_st0    : 1;  /* bit[0-0]  : 通用资源锁5的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁5的锁定状态master ID号0。 */
        unsigned int  resource5lock_st1    : 1;  /* bit[4-4]  : 通用资源锁5的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁5的锁定状态master ID号1。 */
        unsigned int  resource5lock_st2    : 1;  /* bit[8-8]  : 通用资源锁5的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁5的锁定状态master ID号2。 */
        unsigned int  resource5lock_st3    : 1;  /* bit[12-12]: 通用资源锁5的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁5的锁定状态master ID号3。 */
        unsigned int  resource5lock_st4    : 1;  /* bit[16-16]: 通用资源锁5的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁5的锁定状态master ID号4。 */
        unsigned int  resource5lock_st5    : 1;  /* bit[20-20]: 通用资源锁5的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁5的锁定状态master ID号5。 */
        unsigned int  resource5lock_st6    : 1;  /* bit[24-24]: 通用资源锁5的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁5的锁定状态master ID号6。 */
        unsigned int  resource5lock_st7    : 1;  /* bit[28-28]: 通用资源锁5的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource5lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁5的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE5_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st0_START     (0)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st0_END       (0)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st1_START     (4)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st1_END       (4)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st2_START     (8)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st2_END       (8)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st3_START     (12)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st3_END       (12)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st4_START     (16)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st4_END       (16)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st5_START     (20)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st5_END       (20)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st6_START     (24)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st6_END       (24)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st7_START     (28)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st7_END       (28)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE5_LOCK_ST_resource5lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE6_LOCK_UNION
 结构说明  : RESOURCE6_LOCK 寄存器结构定义。地址偏移量:0x418，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource6lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁6的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id0  : 3;  /* bit[1-3]  : 通用资源锁6的锁定master ID号0。 */
        unsigned int  resource6lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁6的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id1  : 3;  /* bit[5-7]  : 通用资源锁6的锁定master ID号1。 */
        unsigned int  resource6lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁6的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id2  : 3;  /* bit[9-11] : 通用资源锁6的锁定master ID号2。 */
        unsigned int  resource6lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁6的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id3  : 3;  /* bit[13-15]: 通用资源锁6的锁定master ID号3。 */
        unsigned int  resource6lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁6的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id4  : 3;  /* bit[17-19]: 通用资源锁6的锁定master ID号4。 */
        unsigned int  resource6lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁6的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id5  : 3;  /* bit[21-23]: 通用资源锁6的锁定master ID号5。 */
        unsigned int  resource6lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁6的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id6  : 3;  /* bit[25-27]: 通用资源锁6的锁定master ID号6。 */
        unsigned int  resource6lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁6的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource6lock_id7  : 3;  /* bit[29-31]: 通用资源锁6的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE6_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id0_START   (1)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id0_END     (3)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id1_START   (5)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id1_END     (7)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id2_START   (9)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id2_END     (11)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id3_START   (13)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id3_END     (15)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id4_START   (17)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id4_END     (19)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id5_START   (21)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id5_END     (23)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id6_START   (25)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id6_END     (27)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id7_START   (29)
#define SOC_LOCK_RESOURCE6_LOCK_resource6lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE6_UNLOCK_UNION
 结构说明  : RESOURCE6_UNLOCK 寄存器结构定义。地址偏移量:0x41C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource6unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁6的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁6的解锁master ID号0。 */
        unsigned int  resource6unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁6的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁6的解锁master ID号1。 */
        unsigned int  resource6unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁6的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id2  : 3;  /* bit[9-11] : 通用资源锁6的解锁master ID号2。 */
        unsigned int  resource6unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁6的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id3  : 3;  /* bit[13-15]: 通用资源锁6的解锁master ID号3。 */
        unsigned int  resource6unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁6的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id4  : 3;  /* bit[17-19]: 通用资源锁6的解锁master ID号4。 */
        unsigned int  resource6unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁6的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id5  : 3;  /* bit[21-23]: 通用资源锁6的解锁master ID号5。 */
        unsigned int  resource6unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁6的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id6  : 3;  /* bit[25-27]: 通用资源锁6的解锁master ID号6。 */
        unsigned int  resource6unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁6的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource6unlock_id7  : 3;  /* bit[29-31]: 通用资源锁6的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE6_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE6_UNLOCK_resource6unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE6_LOCK_ST_UNION
 结构说明  : RESOURCE6_LOCK_ST 寄存器结构定义。地址偏移量:0x420，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器6。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource6lock_st0    : 1;  /* bit[0-0]  : 通用资源锁6的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁6的锁定状态master ID号0。 */
        unsigned int  resource6lock_st1    : 1;  /* bit[4-4]  : 通用资源锁6的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁6的锁定状态master ID号1。 */
        unsigned int  resource6lock_st2    : 1;  /* bit[8-8]  : 通用资源锁6的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁6的锁定状态master ID号2。 */
        unsigned int  resource6lock_st3    : 1;  /* bit[12-12]: 通用资源锁6的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁6的锁定状态master ID号3。 */
        unsigned int  resource6lock_st4    : 1;  /* bit[16-16]: 通用资源锁6的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁6的锁定状态master ID号4。 */
        unsigned int  resource6lock_st5    : 1;  /* bit[20-20]: 通用资源锁6的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁6的锁定状态master ID号5。 */
        unsigned int  resource6lock_st6    : 1;  /* bit[24-24]: 通用资源锁6的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁6的锁定状态master ID号6。 */
        unsigned int  resource6lock_st7    : 1;  /* bit[28-28]: 通用资源锁6的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource6lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁6的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE6_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st0_START     (0)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st0_END       (0)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st1_START     (4)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st1_END       (4)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st2_START     (8)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st2_END       (8)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st3_START     (12)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st3_END       (12)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st4_START     (16)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st4_END       (16)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st5_START     (20)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st5_END       (20)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st6_START     (24)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st6_END       (24)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st7_START     (28)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st7_END       (28)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE6_LOCK_ST_resource6lock_st_id7_END    (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE7_LOCK_UNION
 结构说明  : RESOURCE7_LOCK 寄存器结构定义。地址偏移量:0x424，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁定寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource7lock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁7的锁定命令位0。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id0  : 3;  /* bit[1-3]  : 通用资源锁7的锁定master ID号0。 */
        unsigned int  resource7lock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁7的锁定命令位1。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id1  : 3;  /* bit[5-7]  : 通用资源锁7的锁定master ID号1。 */
        unsigned int  resource7lock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁7的锁定命令位2。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id2  : 3;  /* bit[9-11] : 通用资源锁7的锁定master ID号2。 */
        unsigned int  resource7lock_cmd3 : 1;  /* bit[12-12]: 通用资源锁7的锁定命令位3。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id3  : 3;  /* bit[13-15]: 通用资源锁7的锁定master ID号3。 */
        unsigned int  resource7lock_cmd4 : 1;  /* bit[16-16]: 通用资源锁7的锁定命令位4。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id4  : 3;  /* bit[17-19]: 通用资源锁7的锁定master ID号4。 */
        unsigned int  resource7lock_cmd5 : 1;  /* bit[20-20]: 通用资源锁7的锁定命令位5。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id5  : 3;  /* bit[21-23]: 通用资源锁7的锁定master ID号5。 */
        unsigned int  resource7lock_cmd6 : 1;  /* bit[24-24]: 通用资源锁7的锁定命令位6。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id6  : 3;  /* bit[25-27]: 通用资源锁7的锁定master ID号6。 */
        unsigned int  resource7lock_cmd7 : 1;  /* bit[28-28]: 通用资源锁7的锁定命令位7。
                                                              0：无效；
                                                              1：锁定。 */
        unsigned int  resource7lock_id7  : 3;  /* bit[29-31]: 通用资源锁7的锁定master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE7_LOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id0_START   (1)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id0_END     (3)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id1_START   (5)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id1_END     (7)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id2_START   (9)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id2_END     (11)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id3_START   (13)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id3_END     (15)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id4_START   (17)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id4_END     (19)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id5_START   (21)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id5_END     (23)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id6_START   (25)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id6_END     (27)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id7_START   (29)
#define SOC_LOCK_RESOURCE7_LOCK_resource7lock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE7_UNLOCK_UNION
 结构说明  : RESOURCE7_UNLOCK 寄存器结构定义。地址偏移量:0x428，初值:0x00000000，宽度:32
 寄存器说明: 通用资源解锁寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource7unlock_cmd0 : 1;  /* bit[0-0]  : 通用资源锁7的解锁命令位0。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id0  : 3;  /* bit[1-3]  : 通用资源锁7的解锁master ID号0。 */
        unsigned int  resource7unlock_cmd1 : 1;  /* bit[4-4]  : 通用资源锁7的解锁命令位1。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id1  : 3;  /* bit[5-7]  : 通用资源锁7的解锁master ID号1。 */
        unsigned int  resource7unlock_cmd2 : 1;  /* bit[8-8]  : 通用资源锁7的解锁命令位2。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id2  : 3;  /* bit[9-11] : 通用资源锁7的解锁master ID号2。 */
        unsigned int  resource7unlock_cmd3 : 1;  /* bit[12-12]: 通用资源锁7的解锁命令位3。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id3  : 3;  /* bit[13-15]: 通用资源锁7的解锁master ID号3。 */
        unsigned int  resource7unlock_cmd4 : 1;  /* bit[16-16]: 通用资源锁7的解锁命令位4。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id4  : 3;  /* bit[17-19]: 通用资源锁7的解锁master ID号4。 */
        unsigned int  resource7unlock_cmd5 : 1;  /* bit[20-20]: 通用资源锁7的解锁命令位5。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id5  : 3;  /* bit[21-23]: 通用资源锁7的解锁master ID号5。 */
        unsigned int  resource7unlock_cmd6 : 1;  /* bit[24-24]: 通用资源锁7的解锁命令位6。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id6  : 3;  /* bit[25-27]: 通用资源锁7的解锁master ID号6。 */
        unsigned int  resource7unlock_cmd7 : 1;  /* bit[28-28]: 通用资源锁7的解锁命令位7。
                                                                0：无效；
                                                                1：锁定。 */
        unsigned int  resource7unlock_id7  : 3;  /* bit[29-31]: 通用资源锁7的解锁master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE7_UNLOCK_UNION;
#endif
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd0_START  (0)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd0_END    (0)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id0_START   (1)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id0_END     (3)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd1_START  (4)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd1_END    (4)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id1_START   (5)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id1_END     (7)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd2_START  (8)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd2_END    (8)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id2_START   (9)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id2_END     (11)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd3_START  (12)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd3_END    (12)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id3_START   (13)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id3_END     (15)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd4_START  (16)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd4_END    (16)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id4_START   (17)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id4_END     (19)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd5_START  (20)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd5_END    (20)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id5_START   (21)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id5_END     (23)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd6_START  (24)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd6_END    (24)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id6_START   (25)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id6_END     (27)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd7_START  (28)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_cmd7_END    (28)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id7_START   (29)
#define SOC_LOCK_RESOURCE7_UNLOCK_resource7unlock_id7_END     (31)


/*****************************************************************************
 结构名    : SOC_LOCK_RESOURCE7_LOCK_ST_UNION
 结构说明  : RESOURCE7_LOCK_ST 寄存器结构定义。地址偏移量:0x42C，初值:0x00000000，宽度:32
 寄存器说明: 通用资源锁状态寄存器7。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  resource7lock_st0    : 1;  /* bit[0-0]  : 通用资源锁7的锁定状态0。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id0 : 3;  /* bit[1-3]  : 通用资源锁7的锁定状态master ID号0。 */
        unsigned int  resource7lock_st1    : 1;  /* bit[4-4]  : 通用资源锁7的锁定状态1。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id1 : 3;  /* bit[5-7]  : 通用资源锁7的锁定状态master ID号1。 */
        unsigned int  resource7lock_st2    : 1;  /* bit[8-8]  : 通用资源锁7的锁定状态2。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id2 : 3;  /* bit[9-11] : 通用资源锁7的锁定状态master ID号2。 */
        unsigned int  resource7lock_st3    : 1;  /* bit[12-12]: 通用资源锁7的锁定状态3。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id3 : 3;  /* bit[13-15]: 通用资源锁7的锁定状态master ID号3。 */
        unsigned int  resource7lock_st4    : 1;  /* bit[16-16]: 通用资源锁7的锁定状态4。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id4 : 3;  /* bit[17-19]: 通用资源锁7的锁定状态master ID号4。 */
        unsigned int  resource7lock_st5    : 1;  /* bit[20-20]: 通用资源锁7的锁定状态5。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id5 : 3;  /* bit[21-23]: 通用资源锁7的锁定状态master ID号5。 */
        unsigned int  resource7lock_st6    : 1;  /* bit[24-24]: 通用资源锁7的锁定状态6。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id6 : 3;  /* bit[25-27]: 通用资源锁7的锁定状态master ID号6。 */
        unsigned int  resource7lock_st7    : 1;  /* bit[28-28]: 通用资源锁7的锁定状态7。
                                                                0：未锁定；
                                                                1：锁定。 */
        unsigned int  resource7lock_st_id7 : 3;  /* bit[29-31]: 通用资源锁7的锁定状态master ID号7。 */
    } reg;
} SOC_LOCK_RESOURCE7_LOCK_ST_UNION;
#endif
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st0_START     (0)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st0_END       (0)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id0_START  (1)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id0_END    (3)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st1_START     (4)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st1_END       (4)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id1_START  (5)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id1_END    (7)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st2_START     (8)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st2_END       (8)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id2_START  (9)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id2_END    (11)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st3_START     (12)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st3_END       (12)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id3_START  (13)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id3_END    (15)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st4_START     (16)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st4_END       (16)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id4_START  (17)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id4_END    (19)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st5_START     (20)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st5_END       (20)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id5_START  (21)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id5_END    (23)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st6_START     (24)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st6_END       (24)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id6_START  (25)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id6_END    (27)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st7_START     (28)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st7_END       (28)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id7_START  (29)
#define SOC_LOCK_RESOURCE7_LOCK_ST_resource7lock_st_id7_END    (31)






/*****************************************************************************
  8 OTHERS定义
*****************************************************************************/



/*****************************************************************************
  9 全局变量声明
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_lock_interface.h */
