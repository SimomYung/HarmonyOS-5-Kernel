/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_ipcns_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_IPCNS_INTERFACE_H__
#define __SOC_IPCNS_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_define
 ****************************************************************************/
#ifndef __SOC_H_FOR_ASM__


/* 寄存器说明：源处理器配置寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_SOURCE_UNION */
#define SOC_IPCNS_MBX_SOURCE_ADDR(base, i)                    ((base) + (0x000+(i)*128UL))

/* 寄存器说明：目的处理器设置寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DSET_UNION */
#define SOC_IPCNS_MBX_DSET_ADDR(base, i)                      ((base) + (0x004+(i)*128UL))

/* 寄存器说明：目的处理器清除寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DCLEAR_UNION */
#define SOC_IPCNS_MBX_DCLEAR_ADDR(base, i)                    ((base) + (0x008+(i)*128UL))

/* 寄存器说明：目的处理器状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DSTATUS_UNION */
#define SOC_IPCNS_MBX_DSTATUS_ADDR(base, i)                   ((base) + (0x00C+(i)*128UL))

/* 寄存器说明：邮箱通道模式状态机状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_MODE_UNION */
#define SOC_IPCNS_MBX_MODE_ADDR(base, i)                      ((base) + (0x010+(i)*128UL))

/* 寄存器说明：处理器中断屏蔽寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_IMASK_UNION */
#define SOC_IPCNS_MBX_IMASK_ADDR(base, i)                     ((base) + (0x014+(i)*128UL))

/* 寄存器说明：处理器中断状态清除寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_ICLR_UNION */
#define SOC_IPCNS_MBX_ICLR_ADDR(base, i)                      ((base) + (0x018+(i)*128UL))

/* 寄存器说明：发送寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_SEND_UNION */
#define SOC_IPCNS_MBX_SEND_ADDR(base, i)                      ((base) + (0x01C+(i)*128UL))

/* 寄存器说明：数据0寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA0_UNION */
#define SOC_IPCNS_MBX_DATA0_ADDR(base, i)                     ((base) + (0x020+(i)*128UL))

/* 寄存器说明：数据1寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA1_UNION */
#define SOC_IPCNS_MBX_DATA1_ADDR(base, i)                     ((base) + (0x024+(i)*128UL))

/* 寄存器说明：数据2寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA2_UNION */
#define SOC_IPCNS_MBX_DATA2_ADDR(base, i)                     ((base) + (0x028+(i)*128UL))

/* 寄存器说明：数据3寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA3_UNION */
#define SOC_IPCNS_MBX_DATA3_ADDR(base, i)                     ((base) + (0x02C+(i)*128UL))

/* 寄存器说明：数据4寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA4_UNION */
#define SOC_IPCNS_MBX_DATA4_ADDR(base, i)                     ((base) + (0x030+(i)*128UL))

/* 寄存器说明：数据5寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA5_UNION */
#define SOC_IPCNS_MBX_DATA5_ADDR(base, i)                     ((base) + (0x034+(i)*128UL))

/* 寄存器说明：数据6寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA6_UNION */
#define SOC_IPCNS_MBX_DATA6_ADDR(base, i)                     ((base) + (0x038+(i)*128UL))

/* 寄存器说明：数据7寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA7_UNION */
#define SOC_IPCNS_MBX_DATA7_ADDR(base, i)                     ((base) + (0x03C+(i)*128UL))

/* 寄存器说明：数据8寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA8_UNION */
#define SOC_IPCNS_MBX_DATA8_ADDR(base, i)                     ((base) + (0x040+(i)*128UL))

/* 寄存器说明：数据9寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA9_UNION */
#define SOC_IPCNS_MBX_DATA9_ADDR(base, i)                     ((base) + (0x044+(i)*128UL))

/* 寄存器说明：数据10寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA10_UNION */
#define SOC_IPCNS_MBX_DATA10_ADDR(base, i)                    ((base) + (0x048+(i)*128UL))

/* 寄存器说明：数据11寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA11_UNION */
#define SOC_IPCNS_MBX_DATA11_ADDR(base, i)                    ((base) + (0x04C+(i)*128UL))

/* 寄存器说明：CPUj对应Mailbox[32*n+31:32*n]的中断屏蔽状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_CPUj_IMST_UNION */
#define SOC_IPCNS_CPUj_IMST_ADDR(base, j, n)                  ((base) + (0x8000+(j)*8+(n)*256UL))

/* 寄存器说明：CPUj对应Mailbox[32*n+31:32*n]中断原始状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_CPUj_IRST_UNION */
#define SOC_IPCNS_CPUj_IRST_ADDR(base, j, n)                  ((base) + (0x8004+(j)*8+(n)*256UL))

/* 寄存器说明：有效通道状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_MBX_ACTIVE_UNION */
#define SOC_IPCNS_IPC_MBX_ACTIVE_ADDR(base, n)                ((base) + (0x9000+(n)*4UL))

/* 寄存器说明：低功耗状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_LP_STATE_UNION */
#define SOC_IPCNS_IPC_LP_STATE_ADDR(base)                     ((base) + (0x9024UL))

/* 寄存器说明：IPC lock寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_LOCK_UNION */
#define SOC_IPCNS_IPC_LOCK_ADDR(base)                         ((base) + (0xA000UL))


#else


/* 寄存器说明：源处理器配置寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_SOURCE_UNION */
#define SOC_IPCNS_MBX_SOURCE_ADDR(base, i)                    ((base) + (0x000+(i)*128))

/* 寄存器说明：目的处理器设置寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DSET_UNION */
#define SOC_IPCNS_MBX_DSET_ADDR(base, i)                      ((base) + (0x004+(i)*128))

/* 寄存器说明：目的处理器清除寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DCLEAR_UNION */
#define SOC_IPCNS_MBX_DCLEAR_ADDR(base, i)                    ((base) + (0x008+(i)*128))

/* 寄存器说明：目的处理器状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DSTATUS_UNION */
#define SOC_IPCNS_MBX_DSTATUS_ADDR(base, i)                   ((base) + (0x00C+(i)*128))

/* 寄存器说明：邮箱通道模式状态机状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_MODE_UNION */
#define SOC_IPCNS_MBX_MODE_ADDR(base, i)                      ((base) + (0x010+(i)*128))

/* 寄存器说明：处理器中断屏蔽寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_IMASK_UNION */
#define SOC_IPCNS_MBX_IMASK_ADDR(base, i)                     ((base) + (0x014+(i)*128))

/* 寄存器说明：处理器中断状态清除寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_ICLR_UNION */
#define SOC_IPCNS_MBX_ICLR_ADDR(base, i)                      ((base) + (0x018+(i)*128))

/* 寄存器说明：发送寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_SEND_UNION */
#define SOC_IPCNS_MBX_SEND_ADDR(base, i)                      ((base) + (0x01C+(i)*128))

/* 寄存器说明：数据0寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA0_UNION */
#define SOC_IPCNS_MBX_DATA0_ADDR(base, i)                     ((base) + (0x020+(i)*128))

/* 寄存器说明：数据1寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA1_UNION */
#define SOC_IPCNS_MBX_DATA1_ADDR(base, i)                     ((base) + (0x024+(i)*128))

/* 寄存器说明：数据2寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA2_UNION */
#define SOC_IPCNS_MBX_DATA2_ADDR(base, i)                     ((base) + (0x028+(i)*128))

/* 寄存器说明：数据3寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA3_UNION */
#define SOC_IPCNS_MBX_DATA3_ADDR(base, i)                     ((base) + (0x02C+(i)*128))

/* 寄存器说明：数据4寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA4_UNION */
#define SOC_IPCNS_MBX_DATA4_ADDR(base, i)                     ((base) + (0x030+(i)*128))

/* 寄存器说明：数据5寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA5_UNION */
#define SOC_IPCNS_MBX_DATA5_ADDR(base, i)                     ((base) + (0x034+(i)*128))

/* 寄存器说明：数据6寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA6_UNION */
#define SOC_IPCNS_MBX_DATA6_ADDR(base, i)                     ((base) + (0x038+(i)*128))

/* 寄存器说明：数据7寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA7_UNION */
#define SOC_IPCNS_MBX_DATA7_ADDR(base, i)                     ((base) + (0x03C+(i)*128))

/* 寄存器说明：数据8寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA8_UNION */
#define SOC_IPCNS_MBX_DATA8_ADDR(base, i)                     ((base) + (0x040+(i)*128))

/* 寄存器说明：数据9寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA9_UNION */
#define SOC_IPCNS_MBX_DATA9_ADDR(base, i)                     ((base) + (0x044+(i)*128))

/* 寄存器说明：数据10寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA10_UNION */
#define SOC_IPCNS_MBX_DATA10_ADDR(base, i)                    ((base) + (0x048+(i)*128))

/* 寄存器说明：数据11寄存器。
   位域定义UNION结构:  SOC_IPCNS_MBX_DATA11_UNION */
#define SOC_IPCNS_MBX_DATA11_ADDR(base, i)                    ((base) + (0x04C+(i)*128))

/* 寄存器说明：CPUj对应Mailbox[32*n+31:32*n]的中断屏蔽状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_CPUj_IMST_UNION */
#define SOC_IPCNS_CPUj_IMST_ADDR(base, j, n)                  ((base) + (0x8000+(j)*8+(n)*256))

/* 寄存器说明：CPUj对应Mailbox[32*n+31:32*n]中断原始状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_CPUj_IRST_UNION */
#define SOC_IPCNS_CPUj_IRST_ADDR(base, j, n)                  ((base) + (0x8004+(j)*8+(n)*256))

/* 寄存器说明：有效通道状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_MBX_ACTIVE_UNION */
#define SOC_IPCNS_IPC_MBX_ACTIVE_ADDR(base, n)                ((base) + (0x9000+(n)*4))

/* 寄存器说明：低功耗状态寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_LP_STATE_UNION */
#define SOC_IPCNS_IPC_LP_STATE_ADDR(base)                     ((base) + (0x9024))

/* 寄存器说明：IPC lock寄存器。
   位域定义UNION结构:  SOC_IPCNS_IPC_LOCK_UNION */
#define SOC_IPCNS_IPC_LOCK_ADDR(base)                         ((base) + (0xA000))


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
                     (1/1) register_define
 ****************************************************************************/
/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_SOURCE_UNION
 结构说明  : MBX_SOURCE 寄存器结构定义。地址偏移量:0x000+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 源处理器配置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  source   : 13; /* bit[0-12] : 邮箱通道占用源处理器配置，可配置多个处理器共同占用此邮箱通道，每一bit对应一个占用通道的处理器；
                                                    配置后写相同值释放该通道邮箱，该邮箱除数据寄存器、模式寄存器之外的寄存器均清0，配置后写其他值无意义。
                                                    在MBX_SOURCE寄存器为非0值时，对MBX_DCLEAR寄存器、MBX_DSET寄存器、MBX_MODE寄存器、MBX_IMASK寄存器、MBX_SEND寄存器可以进行有效写操作，否则无法写入。其他寄存器不受此限制。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_SOURCE_UNION;
#endif
#define SOC_IPCNS_MBX_SOURCE_source_START    (0)
#define SOC_IPCNS_MBX_SOURCE_source_END      (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DSET_UNION
 结构说明  : MBX_DSET 寄存器结构定义。地址偏移量:0x004+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 目的处理器设置寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dset     : 13; /* bit[0-12] : 邮箱通道目的处理器配置寄存器，即通信中断的接收处理器设置，可配置多个处理器同时接收中断，每一bit对应一个处理器。
                                                    0：无影响；
                                                    1：设置为目的处理器。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_DSET_UNION;
#endif
#define SOC_IPCNS_MBX_DSET_dset_START      (0)
#define SOC_IPCNS_MBX_DSET_dset_END        (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DCLEAR_UNION
 结构说明  : MBX_DCLEAR 寄存器结构定义。地址偏移量:0x008+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 目的处理器清除寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dclear   : 13; /* bit[0-12] : 邮箱通道目的处理器清除寄存器，每一bit对应一个处理器。
                                                    0：无影响；
                                                    1：清除。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_DCLEAR_UNION;
#endif
#define SOC_IPCNS_MBX_DCLEAR_dclear_START    (0)
#define SOC_IPCNS_MBX_DCLEAR_dclear_END      (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DSTATUS_UNION
 结构说明  : MBX_DSTATUS 寄存器结构定义。地址偏移量:0x00C+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 目的处理器状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  dstatus  : 13; /* bit[0-12] : 邮箱通道目的处理器状态寄存器，每一bit对应一个处理器。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_DSTATUS_UNION;
#endif
#define SOC_IPCNS_MBX_DSTATUS_dstatus_START   (0)
#define SOC_IPCNS_MBX_DSTATUS_dstatus_END     (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_MODE_UNION
 结构说明  : MBX_MODE 寄存器结构定义。地址偏移量:0x010+(i)*128，初值:0x00000010，宽度:32
 寄存器说明: 邮箱通道模式状态机状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  auto_answer  : 1;  /* bit[0]   : Mailbox自动应答使能。
                                                       自动应答，即当目的处理器中断被清除0后，mailbox自动发出一个回复中断给源处理器。
                                                       0：禁止；
                                                       1：使能。 */
        unsigned int  auto_link    : 1;  /* bit[1]   : Mailbox自动链接使能。
                                                       自动链接，将几个临近的Mailboxes链接在一起，从而可以将多个消息连续的发送出去，而不需要在消息与消息之间发送中断给源处理器。当所有信息都已经传递完毕，应答中断可以由一个目的处理器配置产生，也可以通过自动应答模式来产生。链的顺序是固定的，Mailbox 0 链接到 Mailbox 1 ，依次最高到最后一个邮箱，最后一个邮箱的自动链接功能配置无效。
                                                       0：禁止；
                                                       1：使能。 */
        unsigned int  reserved_0   : 2;  /* bit[2-3] : reserved */
        unsigned int  state_status : 4;  /* bit[4-7] : 状态机状态；
                                                       4'b0001 空闲状态，邮箱空闲；
                                                       4'b0010 源状态，此状态下，邮箱通道已占用，还未发中断给目的处理器；
                                                       4'b0100 目的状态，此状态下，目的处理器收到源发中断，未全部清除或返回应答中断。
                                                       4'b1000 应答状态，此状态下，源收到应答中断未清除完毕。
                                                       其他：异常值 */
        unsigned int  reserved_1   : 24; /* bit[8-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_MODE_UNION;
#endif
#define SOC_IPCNS_MBX_MODE_auto_answer_START   (0)
#define SOC_IPCNS_MBX_MODE_auto_answer_END     (0)
#define SOC_IPCNS_MBX_MODE_auto_link_START     (1)
#define SOC_IPCNS_MBX_MODE_auto_link_END       (1)
#define SOC_IPCNS_MBX_MODE_state_status_START  (4)
#define SOC_IPCNS_MBX_MODE_state_status_END    (7)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_IMASK_UNION
 结构说明  : MBX_IMASK 寄存器结构定义。地址偏移量:0x014+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 处理器中断屏蔽寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_mask : 13; /* bit[0-12] : 邮箱通道到处理器的中断屏蔽设置，每一bit对应一个处理器。
                                                    0：不屏蔽；
                                                    1：屏蔽。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_IMASK_UNION;
#endif
#define SOC_IPCNS_MBX_IMASK_int_mask_START  (0)
#define SOC_IPCNS_MBX_IMASK_int_mask_END    (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_ICLR_UNION
 结构说明  : MBX_ICLR 寄存器结构定义。地址偏移量:0x018+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 处理器中断状态清除寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_clear : 13; /* bit[0-12] : 邮箱通道到处理器屏蔽后中断状态，每一bit对应一个处理器，写1清除。
                                                     0：无中断；
                                                     1：有中断。 */
        unsigned int  reserved  : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_ICLR_UNION;
#endif
#define SOC_IPCNS_MBX_ICLR_int_clear_START  (0)
#define SOC_IPCNS_MBX_ICLR_int_clear_END    (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_SEND_UNION
 结构说明  : MBX_SEND 寄存器结构定义。地址偏移量:0x01C+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 发送寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  send     : 13; /* bit[0-12] : 中断发送寄存器，每一bit对应一个处理器。通信过程中，配置此寄存器，只置位bit有效。
                                                    当源发中断给目的时，send寄存器配置值与源寄存器值相等时发中断给目的处理器；当目的发应答中断给源时，send寄存器配置值与目的相等时发应答中断给源。自动链接时，被链接邮箱此寄存器不须配置；自动应答时，目的清除中断后，此寄存器自动置位，不须配置。 */
        unsigned int  reserved : 19; /* bit[13-31]: reserved */
    } reg;
} SOC_IPCNS_MBX_SEND_UNION;
#endif
#define SOC_IPCNS_MBX_SEND_send_START      (0)
#define SOC_IPCNS_MBX_SEND_send_END        (12)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA0_UNION
 结构说明  : MBX_DATA0 寄存器结构定义。地址偏移量:0x020+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据0寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data0 : 32; /* bit[0-31]: 邮箱通道数据寄存器0 */
    } reg;
} SOC_IPCNS_MBX_DATA0_UNION;
#endif
#define SOC_IPCNS_MBX_DATA0_data0_START  (0)
#define SOC_IPCNS_MBX_DATA0_data0_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA1_UNION
 结构说明  : MBX_DATA1 寄存器结构定义。地址偏移量:0x024+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据1寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data1 : 32; /* bit[0-31]: 邮箱通道数据寄存器1 */
    } reg;
} SOC_IPCNS_MBX_DATA1_UNION;
#endif
#define SOC_IPCNS_MBX_DATA1_data1_START  (0)
#define SOC_IPCNS_MBX_DATA1_data1_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA2_UNION
 结构说明  : MBX_DATA2 寄存器结构定义。地址偏移量:0x028+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据2寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data2 : 32; /* bit[0-31]: 邮箱通道数据寄存器2 */
    } reg;
} SOC_IPCNS_MBX_DATA2_UNION;
#endif
#define SOC_IPCNS_MBX_DATA2_data2_START  (0)
#define SOC_IPCNS_MBX_DATA2_data2_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA3_UNION
 结构说明  : MBX_DATA3 寄存器结构定义。地址偏移量:0x02C+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据3寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data3 : 32; /* bit[0-31]: 邮箱通道数据寄存器3 */
    } reg;
} SOC_IPCNS_MBX_DATA3_UNION;
#endif
#define SOC_IPCNS_MBX_DATA3_data3_START  (0)
#define SOC_IPCNS_MBX_DATA3_data3_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA4_UNION
 结构说明  : MBX_DATA4 寄存器结构定义。地址偏移量:0x030+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据4寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data4 : 32; /* bit[0-31]: 邮箱通道数据寄存器4 */
    } reg;
} SOC_IPCNS_MBX_DATA4_UNION;
#endif
#define SOC_IPCNS_MBX_DATA4_data4_START  (0)
#define SOC_IPCNS_MBX_DATA4_data4_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA5_UNION
 结构说明  : MBX_DATA5 寄存器结构定义。地址偏移量:0x034+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据5寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data5 : 32; /* bit[0-31]: 邮箱通道数据寄存器5 */
    } reg;
} SOC_IPCNS_MBX_DATA5_UNION;
#endif
#define SOC_IPCNS_MBX_DATA5_data5_START  (0)
#define SOC_IPCNS_MBX_DATA5_data5_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA6_UNION
 结构说明  : MBX_DATA6 寄存器结构定义。地址偏移量:0x038+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据6寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data6 : 32; /* bit[0-31]: 邮箱通道数据寄存器6 */
    } reg;
} SOC_IPCNS_MBX_DATA6_UNION;
#endif
#define SOC_IPCNS_MBX_DATA6_data6_START  (0)
#define SOC_IPCNS_MBX_DATA6_data6_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA7_UNION
 结构说明  : MBX_DATA7 寄存器结构定义。地址偏移量:0x03C+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据7寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data7 : 32; /* bit[0-31]: 邮箱通道数据寄存器7 */
    } reg;
} SOC_IPCNS_MBX_DATA7_UNION;
#endif
#define SOC_IPCNS_MBX_DATA7_data7_START  (0)
#define SOC_IPCNS_MBX_DATA7_data7_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA8_UNION
 结构说明  : MBX_DATA8 寄存器结构定义。地址偏移量:0x040+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据8寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data8 : 32; /* bit[0-31]: 邮箱通道数据寄存器8 */
    } reg;
} SOC_IPCNS_MBX_DATA8_UNION;
#endif
#define SOC_IPCNS_MBX_DATA8_data8_START  (0)
#define SOC_IPCNS_MBX_DATA8_data8_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA9_UNION
 结构说明  : MBX_DATA9 寄存器结构定义。地址偏移量:0x044+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据9寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data9 : 32; /* bit[0-31]: 邮箱通道数据寄存器9 */
    } reg;
} SOC_IPCNS_MBX_DATA9_UNION;
#endif
#define SOC_IPCNS_MBX_DATA9_data9_START  (0)
#define SOC_IPCNS_MBX_DATA9_data9_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA10_UNION
 结构说明  : MBX_DATA10 寄存器结构定义。地址偏移量:0x048+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据10寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data10 : 32; /* bit[0-31]: 邮箱通道数据寄存器10 */
    } reg;
} SOC_IPCNS_MBX_DATA10_UNION;
#endif
#define SOC_IPCNS_MBX_DATA10_data10_START  (0)
#define SOC_IPCNS_MBX_DATA10_data10_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_MBX_DATA11_UNION
 结构说明  : MBX_DATA11 寄存器结构定义。地址偏移量:0x04C+(i)*128，初值:0x00000000，宽度:32
 寄存器说明: 数据11寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  data11 : 32; /* bit[0-31]: 邮箱通道数据寄存器11 */
    } reg;
} SOC_IPCNS_MBX_DATA11_UNION;
#endif
#define SOC_IPCNS_MBX_DATA11_data11_START  (0)
#define SOC_IPCNS_MBX_DATA11_data11_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_CPUj_IMST_UNION
 结构说明  : CPUj_IMST 寄存器结构定义。地址偏移量:0x8000+(j)*8+(n)*256，初值:0x00000000，宽度:32
 寄存器说明: CPUj对应Mailbox[32*n+31:32*n]的中断屏蔽状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_mask_status : 32; /* bit[0-31]: 处理器j屏蔽后的中断状态寄存器，每个bit对应一个邮箱通道。
                                                          0：无中断；
                                                          1：有中断。
                                                          总位宽与宏定义IPC_MBX_NUM一致
                                                          注意：如果不存在对应的Mailbox，此处就成了Reserved域。 */
    } reg;
} SOC_IPCNS_CPUj_IMST_UNION;
#endif
#define SOC_IPCNS_CPUj_IMST_int_mask_status_START  (0)
#define SOC_IPCNS_CPUj_IMST_int_mask_status_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_CPUj_IRST_UNION
 结构说明  : CPUj_IRST 寄存器结构定义。地址偏移量:0x8004+(j)*8+(n)*256，初值:0x00000000，宽度:32
 寄存器说明: CPUj对应Mailbox[32*n+31:32*n]中断原始状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  int_raw_status : 32; /* bit[0-31]: 处理器j原始中断状态寄存器，每个bit对应一个邮箱通道。
                                                         0：无中断；
                                                         1：有中断。
                                                         总位宽与宏定义IPC_MBX_NUM一致
                                                         注意：如果不存在对应的Mailbox，此处就成了Reserved域。 */
    } reg;
} SOC_IPCNS_CPUj_IRST_UNION;
#endif
#define SOC_IPCNS_CPUj_IRST_int_raw_status_START  (0)
#define SOC_IPCNS_CPUj_IRST_int_raw_status_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_IPC_MBX_ACTIVE_UNION
 结构说明  : IPC_MBX_ACTIVE 寄存器结构定义。地址偏移量:0x9000+(n)*4，初值:0x00000000，宽度:32
 寄存器说明: 有效通道状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  mbx_active_status : 32; /* bit[0-31]: 邮箱通道有效状态，每个bit对应一个邮箱通道。
                                                            0：空闲；
                                                            1：有效。
                                                            总位宽与宏定义IPC_MBX_NUM一致
                                                            注意：如果不存在对应的Mailbox，此处就成了Reserved域。 */
    } reg;
} SOC_IPCNS_IPC_MBX_ACTIVE_UNION;
#endif
#define SOC_IPCNS_IPC_MBX_ACTIVE_mbx_active_status_START  (0)
#define SOC_IPCNS_IPC_MBX_ACTIVE_mbx_active_status_END    (31)


/*****************************************************************************
 结构名    : SOC_IPCNS_IPC_LP_STATE_UNION
 结构说明  : IPC_LP_STATE 寄存器结构定义。地址偏移量:0x9024，初值:0x00000000，宽度:32
 寄存器说明: 低功耗状态寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  lp_state : 3;  /* bit[0-2] : 低功耗状态机状态；
                                                   000：空闲状态，模块复位状态；
                                                   001：工作状态，此状态表明功能模块处于工作状态；
                                                   010：低功耗请求状态，此状态表明外部有低功耗请求，模块需要做进入低功耗前的准备工作；
                                                   011：低功耗确认状态，此状态表明功能模块处于空闲，可以进入低功耗；
                                                   100：低功耗拒绝状态，此状态表明功能模块正忙无法进入低功耗；
                                                   101：等待退出低功耗状态，此状态等待外部送入低功耗退出请求（csysreq_sync为高）；
                                                   110：等待确认退出低功耗状态，此状态等待功能模块给出退出低功耗响应（exit_ack）信号。 */
        unsigned int  reserved : 29; /* bit[3-31]: reserved */
    } reg;
} SOC_IPCNS_IPC_LP_STATE_UNION;
#endif
#define SOC_IPCNS_IPC_LP_STATE_lp_state_START  (0)
#define SOC_IPCNS_IPC_LP_STATE_lp_state_END    (2)


/*****************************************************************************
 结构名    : SOC_IPCNS_IPC_LOCK_UNION
 结构说明  : IPC_LOCK 寄存器结构定义。地址偏移量:0xA000，初值:0x00000000，宽度:32
 寄存器说明: IPC lock寄存器。
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union
{
    unsigned int      value;
    struct
    {
        unsigned int  ipc_lock : 32; /* bit[0-31]: 写lock寄存器：
                                                   向Lock寄存器写32bits密钥（密钥宏定义配置，32'h1ACCE551），解锁寄存器，表示所有寄存器可写；向Lock寄存器写其他任意值，锁定寄存器，Lock保护范围内的寄存器不能写，Lock保护范围外的寄存器可以写；
                                                   读任何寄存器不受Lock寄存器的影响，任何时刻都可以读。
                                                   读lock寄存器：
                                                   Lock寄存器读出值表示寄存器的锁定状态：
                                                   32’h0000_0000表示寄存器处于解锁状态;
                                                   32’h0000_0001表示寄存器处于锁定状态。
                                                   Lock寄存器锁定范围参考IP LRS。
                                                   lock寄存器复位值宏定义配置，请查看宏定义文件。 */
    } reg;
} SOC_IPCNS_IPC_LOCK_UNION;
#endif
#define SOC_IPCNS_IPC_LOCK_ipc_lock_START  (0)
#define SOC_IPCNS_IPC_LOCK_ipc_lock_END    (31)






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

#endif /* end of soc_ipcns_interface.h */
