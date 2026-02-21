/*
 * Copyright     :  Copyright (C) 2023, Huawei Technologies Co. Ltd.
 * File name     :  HISI_WD_APB_IF_c_union_define.h
 * Project line  :  Kirin
 * Department    :
 * Author        :  xxx
 * Version       :  1.0
 * Date          :  43948
 * Description   :
 * Others        :  Generated automatically by nManager V5.1
 * History       :  xxx 2023/02/28 14:31:02 Create file
 */

#ifndef __IPC_AO_C_UNION_DEFINE_H__
#define __IPC_AO_C_UNION_DEFINE_H__

/*
 * DEFINE REGISTER UNION
 */
/* Define the union MbxSource */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道占用源处理器配置，可配置多个处理器共同占用此邮箱通道，每一bit对应一个占用通道的处理器；
         * 配置后写相同值释放该通道邮箱，该邮箱除数据寄存器、模式寄存器之外的寄存器均清0，配置后写其他值无意义。
         * 在MBX_SOURCE寄存器为非0值时，对MBX_DCLEAR寄存器、MBX_DSET寄存器、MBX_MODE寄存器、MBX_IMASK寄存器、MBX_SEND寄存器可以进行有效写操作，否则无法写入。其他寄存器不受此限制。
         */
        unsigned int source                  : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_SOURCE;

/* Define the union MbxDset */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道目的处理器配置寄存器，即通信中断的接收处理器设置，可配置多个处理器同时接收中断，每一bit对应一个处理器。
         * 0：无影响；
         * 1：设置为目的处理器。
         */
        unsigned int dset                    : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DSET;

/* Define the union MbxDclear */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道目的处理器清除寄存器，每一bit对应一个处理器。
         * 0：无影响；
         * 1：清除。
         */
        unsigned int dclear                  : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DCLEAR;

/* Define the union MbxDstatus */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道目的处理器状态寄存器，每一bit对应一个处理器。 */
        unsigned int dstatus                 : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DSTATUS;

/* Define the union MbxMode */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * Mailbox自动应答使能。
         * 自动应答，即当目的处理器中断被清除0后，mailbox自动发出一个回复中断给源处理器。
         * 0：禁止；
         * 1：使能。
         */
        unsigned int autoAnswer              : 1;       /* [0] */
        /*
         * Mailbox自动链接使能。
         * 自动链接，将几个临近的Mailboxes链接在一起，从而可以将多个消息连续的发送出去，而不需要在消息与消息之间发送中断给源处理器。当所有信息都已经传递完毕，应答中断可以由一个目的处理器配置产生，也可以通过自动应答模式来产生。链的顺序是固定的，Mailbox 0 链接到 Mailbox 1 ，依次最高到最后一个邮箱，最后一个邮箱的自动链接功能配置无效。
         * 0：禁止；
         * 1：使能。
         */
        unsigned int autoLink                : 1;       /* [1] */
        unsigned int reserved0               : 2;       /* [3:2] */
        /*
         * 状态机状态；
         * 4'b0001 空闲状态，邮箱空闲；
         * 4'b0010 源状态，此状态下，邮箱通道已占用，还未发中断给目的处理器；
         * 4'b0100 目的状态，此状态下，目的处理器收到源发中断，未全部清除或返回应答中断。
         * 4'b1000 应答状态，此状态下，源收到应答中断未清除完毕。
         * 其他：异常值
         */
        unsigned int stateStatus             : 4;       /* [7:4] */
        unsigned int reserved1               : 24;      /* [31:8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_MODE;

/* Define the union MbxImask */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道到处理器的中断屏蔽设置，每一bit对应一个处理器。
         * 0：不屏蔽；
         * 1：屏蔽。
         */
        unsigned int intMask                 : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_IMASK;

/* Define the union MbxIclr */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道到处理器屏蔽后中断状态，每一bit对应一个处理器，写1清除。
         * 0：无中断；
         * 1：有中断。
         */
        unsigned int intClear                : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_ICLR;

/* Define the union MbxSend */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 中断发送寄存器，每一bit对应一个处理器。通信过程中，配置此寄存器，只置位bit有效。
         * 当源发中断给目的时，send寄存器配置值与源寄存器值相等时发中断给目的处理器；当目的发应答中断给源时，send寄存器配置值与目的相等时发应答中断给源。自动链接时，被链接邮箱此寄存器不须配置；自动应答时，目的清除中断后，此寄存器自动置位，不须配置。
         */
        unsigned int send                    : 2;       /* [1:0] */
        unsigned int reserved                : 30;      /* [31:2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_SEND;

/* Define the union MbxData0 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器0 */
        unsigned int data0                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA0;

/* Define the union MbxData1 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器1 */
        unsigned int data1                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA1;

/* Define the union MbxData2 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器2 */
        unsigned int data2                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA2;

/* Define the union MbxData3 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器3 */
        unsigned int data3                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA3;

/* Define the union MbxData4 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器4 */
        unsigned int data4                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA4;

/* Define the union MbxData5 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器5 */
        unsigned int data5                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA5;

/* Define the union MbxData6 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器6 */
        unsigned int data6                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA6;

/* Define the union MbxData7 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 邮箱通道数据寄存器7 */
        unsigned int data7                   : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_MBX_DATA7;

/* Define the union CpuImst */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 处理器j屏蔽后的中断状态寄存器，每个bit对应一个邮箱通道。
         * 0：无中断；
         * 1：有中断。
         */
        unsigned int intMaskStatus           : 3;       /* [2:0] */
        unsigned int reserved                : 29;      /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CPU_IMST;

/* Define the union CpuIrst */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 处理器j原始中断状态寄存器，每个bit对应一个邮箱通道。
         * 0：无中断；
         * 1：有中断。
         */
        unsigned int intRawStatus            : 3;       /* [2:0] */
        unsigned int reserved                : 29;      /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_CPU_IRST;

/* Define the union IpcMbxActive */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 邮箱通道有效状态，每个bit对应一个邮箱通道。
         * 0：空闲；
         * 1：有效。
         */
        unsigned int mbxActiveStatus         : 3;       /* [2:0] */
        unsigned int reserved                : 29;      /* [31:3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_IPC_MBX_ACTIVE;

/* Define the union IpcLock */
typedef union {
    /* Define the struct bits */
    struct {
        /*
         * 写lock寄存器：
         * 向Lock寄存器写32bits密钥，解锁寄存器，表示所有寄存器可写；向Lock寄存器写其他任意值，锁定寄存器，Lock保护范围内的寄存器不能写，Lock保护范围外的寄存器可以写；
         * 读任何寄存器不受Lock寄存器的影响，任何时刻都可以读。
         * 读lock寄存器：
         * Lock寄存器读出值表示寄存器的锁定状态：
         * 32’h0000_0000表示寄存器处于解锁状态;
         * 32’h0000_0001表示寄存器处于锁定状态。
         * Lock寄存器锁定范围参考IP LRS。
         */
        unsigned int ipcLock                 : 32;      /* [31:0] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} U_IPC_LOCK;

typedef struct {
    U_MBX_SOURCE                            MBX_SOURCE;
    U_MBX_DSET                              MBX_DSET;
    U_MBX_DCLEAR                            MBX_DCLEAR;
    U_MBX_DSTATUS                           MBX_DSTATUS;
    U_MBX_MODE                              MBX_MODE;
    U_MBX_IMASK                             MBX_IMASK;
    U_MBX_ICLR                              MBX_ICLR;
    U_MBX_SEND                              MBX_SEND;
    U_MBX_DATA0                             MBX_DATA0;
    U_MBX_DATA1                             MBX_DATA1;
    U_MBX_DATA2                             MBX_DATA2;
    U_MBX_DATA3                             MBX_DATA3;
    U_MBX_DATA4                             MBX_DATA4;
    U_MBX_DATA5                             MBX_DATA5;
    U_MBX_DATA6                             MBX_DATA6;
    U_MBX_DATA7                             MBX_DATA7;
}  U_IPC_MBX_REGS;

typedef struct {
    U_CPU_IMST                              CPU_IMST;
    U_CPU_IRST                              CPU_IRST;
} U_IRQ_STATUS_REGS;

/*
 * DEFINE GLOBAL STRUCT
 */
typedef struct {
    U_IPC_MBX_REGS                        IPC_MBX_REGS[17];            /* 0 ~ 43f */
    unsigned int                          reserved0[240];              /* 440 ~ 7ff */
    U_IRQ_STATUS_REGS                     IRQ_STATUS_REGS[8];          /* 800 ~ 83f */
    unsigned int                          reserved1[48];               /* 840 ~ 8ff */
    U_IPC_MBX_ACTIVE                      IPC_MBX_ACTIVE;              /* 900 */
    unsigned int                          reserved2[63];               /* 901 ~ 9ff */
    U_IPC_LOCK                            IPC_LOCK;                    /* a00 */
} S_IPC_AO_REGS_TYPE;


#endif /* __IPC_AO_C_UNION_DEFINE_H__ */