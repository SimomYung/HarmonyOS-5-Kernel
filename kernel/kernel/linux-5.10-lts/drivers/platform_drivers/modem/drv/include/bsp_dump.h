/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __BSP_DUMP_H__
#define __BSP_DUMP_H__
#include <product_config.h>
#include <linux/stddef.h>
#include <linux/semaphore.h>
#include <osl_spinlock.h>
#include <osl_common.h>
#include <osl_list.h>
#include "mdrv_om.h"
#include "mdrv_errno.h"
#include "drv_comm.h"
#include "bsp_print.h"
#include "bsp_om_enum.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_MODU_OTHER_CORE 0xe
#define OM_SYSCTRL_MAGIC 0x5B5C5C5B
#define NR_EXCEPTION 0xaa
#define MDMAP_EXCEPTION 0xbb
#define L2HAC_EXCEPTION 0xcc
#define NRRDR_EXCEPTION 0xdd

#define EXC_VEC_RESET 0x00    /* reset */
#define EXC_VEC_UNDEF 0x04    /* undefined instruction */
#define EXC_VEC_SWI 0x08      /* software interrupt */
#define EXC_VEC_PREFETCH 0x0c /* prefetch abort */
#define EXC_VEC_DATA 0x10     /* data abort */
#define EXC_VEC_IRQ 0x18      /* interrupt */
#define EXC_VEC_FIQ 0x1C      /* fast interrupt */

#define DUMP_INT_IN_FLAG 0xAAAA
#define DUMP_FLAG_INT_ENTER 0xAAAA0000
#define DUMP_INT_EXIT_FLAG 0xBBBB
#define DUMP_FLAG_INT_EXIT 0xBBBB0000
#define DUMP_INT_UNLOCK_FLAG 0xCCCC
#define DUMP_SAVE_SUCCESS 0xA4A4A4A4
#define DUMP_TASK_INFO_SIZE 0x150
#define DUMP_TASK_INFO_STACK_SIZE (DUMP_TASK_INFO_SIZE - 32 * 4)

#define MODEM_DUMP_FILE_NAME_LENGTH 128
#define DUMP_FIELD_CPUINFO_SIZE 1024
#define ARM_REGS_NUM 17
#define DUMP_REG_SET_MAX 0x1000
#define NVID_DUMP NV_ID_DRV_DUMP
#define DUMP_LOAD_MAGIC 0xEEEE1234

#define DUMP_BUILD_TIME_LEN 32
#define DUMP_PRODUCT_LEN 32
#define DUMP_PRODUCT_VERSION_LEN 32
#define DUMP_UUID_LEN 40
#define DUMP_IMAGEID_LEN 72
#define DUMP_DESC_LEN 48
#define DUMP_NR_DESC_LEN 32
#define DUMP_DATETIME_LEN 24
#define DUMP_BASEINFO_EXC_DESC_LEN 48
#define DUMP_SECDUMP_NAME_LEN 28
#define DUMP_NAME_LEN 16
#define DUMP_NAME_LEN_SHORT 12

typedef enum {
    DUMP_MBB = 0,
    DUMP_PHONE,
    DUMP_PRODUCT_BUTT
} dump_product_type_e;

typedef enum {
    DUMP_ACCESS_MDD_DDR_NON_SEC = 0,
    DUMP_ACCESS_MDD_DDR_SEC = 0x1,
    DUMP_ACCESS_MDD_DDR_SEC_BUTT = 0x2,
} dump_access_mdmddr_type_e;

typedef enum {
    DUMP_CCPU_ARCH_VSMA = 0,
    DUMP_CCPU_ARCH_PSMA,
    DUMP_CCPU_ARCH_BUTT
} dump_ccpu_arch_e;

/* dump文件结点，ioctl命令字 */
typedef enum {
    DUMP_SAVE_FILE_NORMAL = 0x0,
    DUMP_SAVE_FILE_NEED = 0xAABBCCDD,
    DUMP_SAVE_FILE_END = 0x5A5A5A5A
} dump_save_flag_e;

typedef enum {
    DUMP_START_POWER_ON = 0,
    DUMP_START_REBOOT = 0x5A5A1111,
    DUMP_START_EXCH = 0x5A5A2222,
    DUMP_START_CRASH = 0x5A5A3333
} dump_start_flag_e;

typedef enum {
    DUMP_CPU_APP = 0x01000000,
    DUMP_CPU_LRCCPU = 0x02000000,
    DUMP_CPU_BBE = 0x03000000,
    DUMP_CPU_LPM3 = 0x04000000,
    DUMP_CPU_IOM3 = 0x05000000,
    DUMP_CPU_HIFI = 0x06000000,
    DUMP_CPU_NRCCPU = 0x07000000,
    DUMP_CPU_NRL2HAC = 0x08000000,
    DUMP_CPU_NRLL1C = 0x09000000,
    DUMP_CPU_MDMM3 = 0x0a000000,
    DUMP_CPU_BUTTON = 0x0b000000
} dump_reboot_cpu_e;

typedef enum {
    DUMP_REASON_NORMAL = 0x0,
    DUMP_REASON_ARM = 0x1,
    DUMP_REASON_STACKFLOW = 0x2,
    DUMP_REASON_WDT = 0x3,
    DUMP_REASON_PMU = 0x4,
    DUMP_REASON_REBOOT = 0x5,
    DUMP_REASON_NOC = 0x6,
    DUMP_REASON_DMSS = 0x7,
    DUMP_REASON_RST_FAIL = 0x8,
    DUMP_REASON_RST_NOT_SUPPORT = 0x9,
    DUMP_REASON_DLOCK = 0x10,
    DUMP_REASON_RST_FREQ = 0x11,
    DUMP_REASON_PDLOCK = 0x12,
    DUMP_REASON_AMON = 0x13,
    DUMP_REASON_UNDEF = 0xff
} dump_reboot_reason_e;

/* 注意这个枚举在dts中也用到了(file_attr,见log_strategy.c)，如果调整顺序，需要同步修改dts */
enum dump_subsys {
    DUMP_SUBSYS_MDM_START = 0,
    DUMP_SUBSYS_MDMAP = DUMP_SUBSYS_MDM_START,
    DUMP_SUBSYS_CP, /* C核 */
    DUMP_SUBSYS_DPA,
    DUMP_SUBSYS_MDMLPM, /* PHONE：modem在lpmcu上的驱动，MBB：与DUMP_SUBSYS_LPMCU无差异 */
    DUMP_SUBSYS_COMM,
    DUMP_SUBSYS_MDMBUTT,
    DUMP_SUBSYS_MDMMAX = 31,
    DUMP_SUBSYS_OTHER_START = 32,
    DUMP_SUBSYS_AP = DUMP_SUBSYS_OTHER_START, /* 非modem子系统，对应于ACPU子系统总体 */
    DUMP_SUBSYS_LPMCU, /* 非modem子系统，对应于LPMCU子系统总体 */
    DUMP_SUBSYS_HIFI, /* 非modem子系统，对应于HIFI子系统总体 */
    DUMP_SUBSYS_OTHER_BUTT,
    DUMP_SUBSYS_MAX = 63
};
#define DUMP_SUBSYS_MASK(n) (1ul << (n))
enum dump_subsys_mask {
    DUMP_SUBSYS_MASK_MDM_START = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDM_START),
    DUMP_SUBSYS_MASK_MDMAP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMAP),
    DUMP_SUBSYS_MASK_CP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_CP),
    DUMP_SUBSYS_MASK_DPA = DUMP_SUBSYS_MASK(DUMP_SUBSYS_DPA),
    DUMP_SUBSYS_MASK_MDMLPM = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMLPM),
    DUMP_SUBSYS_MASK_COMM = DUMP_SUBSYS_MASK(DUMP_SUBSYS_COMM),
    DUMP_SUBSYS_MASK_MDMBUTT = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMBUTT),
    DUMP_SUBSYS_MASK_MDMMAX = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MDMMAX),
    DUMP_SUBSYS_MASK_OTHER_START = DUMP_SUBSYS_MASK(DUMP_SUBSYS_OTHER_START),
    DUMP_SUBSYS_MASK_AP = DUMP_SUBSYS_MASK(DUMP_SUBSYS_AP),
    DUMP_SUBSYS_MASK_LPMCU = DUMP_SUBSYS_MASK(DUMP_SUBSYS_LPMCU),
    DUMP_SUBSYS_MASK_HIFI = DUMP_SUBSYS_MASK(DUMP_SUBSYS_HIFI),
    DUMP_SUBSYS_MASK_OTHER_BUTT = DUMP_SUBSYS_MASK(DUMP_SUBSYS_OTHER_BUTT),
    DUMP_SUBSYS_MASK_MAX = DUMP_SUBSYS_MASK(DUMP_SUBSYS_MAX),
};
#define DUMP_EXC_DESC_SIZE 24 // rdr i结构体中该字段是48， 该宏只要小于都可以，减小该宏以节约空间
#define DUMP_EXCINFO_TASKNAME_LEN 16
typedef struct {
    u64 reset_subsys_mask;
    u64 notify_subsys_mask;
    u32 exc_type;
    u32 src_modid_start; //子系统内部的modid start
    u32 src_modid_end; //子系统内部的modid end
    u32 exc_modid; //映射到rdr的modid
    char exc_desc[DUMP_EXC_DESC_SIZE]; //异常描述
} dump_exc_cfg_s;
typedef struct {
    /* system error 入参信息 */
    u32 e_from_subsys; // 只允许一个subsys，@see enum dump_subsys
    u32 modid;
    u32 arg1;
    u32 arg2;
    char *data;
    u32 length;
    /* 异常现场，本子系统的基本信息 */
    u32 reboot_context;
    union {
        u32 int_no;
        u32 task_id;
    }; // reboot_contex是任务时，这里是taskid；是中断时，这里是int_id
    u32 reason;
    u8 task_name[DUMP_EXCINFO_TASKNAME_LEN];
} dump_subsys_exc_info_s;

typedef struct {
    u32 modid;
    u32 arg1;
    u32 arg2;
    u32 etype;
} dump_rdr_exc_info_s;

typedef struct {
    const dump_exc_cfg_s *exc_cfg;
    dump_rdr_exc_info_s rdr_info;
    dump_subsys_exc_info_s subsys_info;
    u32 timestamp;
    u32 voice; // 是否在通话中，由CP设置
    int rpt_status; // @see enum dump_exc_rpt_ret
} dump_exc_info_s;

typedef struct {
    dump_reboot_cpu_e reboot_core;
    dump_reboot_reason_e reboot_reason;
} dump_reboot_contex_s;

/* 地址随机化需要将偏移offset记录到dump空间尾部 */
typedef struct {
    u32 start_magic; /* 0x5a5a5a5a */
    /* 添加aslr需要按照逆序添加 */
    u64 acpu_aslr_offset;
    u32 mtee_aslr_offset;
    u32 nrccpu_aslr_offset;
    u32 ccpu_aslr_offset;
    u32 end_magic; /* 0xa5a5a5a5 */
} dump_aslr_info_s;

typedef struct {
    u32 task_id;      /* 任务id */
    u8 task_name[DUMP_NAME_LEN_SHORT]; /* 任务名 */
} dump_task_name_s;

typedef struct {
    u32 reboot_context; /* 0x00  */
    u32 reboot_task;    /* 0x04  */
    u8 task_name[DUMP_NAME_LEN];   /* 0x08  */
    u32 reboot_int;     /* 0x18  */
} dump_cp_reboot_contex_s;

typedef enum {
    DUMP_CTX_TASK = 0x0,
    DUMP_CTX_INT = 0x1
} dump_reboot_ctx_e;

typedef struct {
    u32 cpu_state;                 /* cpu 状态 */
    dump_reboot_ctx_e current_ctx; /* cpu上下文 */
    u32 current_int;               /* 当前的中断 */
    u32 current_task;              /* 当前正在运行的任务 */
    u8 task_name[DUMP_NAME_LEN];   /* 任务名 */
    u32 reg_set[ARM_REGS_NUM];     /* 寄存器信息 */
    u8 callstack[DUMP_FIELD_CPUINFO_SIZE - sizeof(dump_reboot_ctx_e) - sizeof(u32) - sizeof(u32) - DUMP_NAME_LEN -
                 sizeof(u32) * ARM_REGS_NUM];
} dump_cpu_info_s;

#define DUMP_QUEUE_MAGIC_NUM (0xabcd6789)
typedef struct {
    u32 magic; /* 0xabcd6789 */
    u32 max_num;
    u32 front;
    u32 rear;
    u32 num;
    u32 *data;
} dump_queue_s;

typedef struct {
    u32 magic; /* 0xabcd6789 */
    u32 maxNum;
    u32 front;
    u32 rear;
    u32 num;
    u32 data;
} dump_cp_queue_s;
typedef enum {
    TASK_SCHED_NORMAL = 0,
    TASK_SCHED_FIFO,
    TASK_SCHED_RR,
    TASK_SCHED_BATCH,
    TASK_SCHED_IDLE
} dump_sched_policy_e;

/*  CPSR R16
  31 30  29  28   27  26    7   6   5   4    3    2    1    0
----------------------------------------------------------------
| N | Z | C | V | Q | RAZ | I | F | T | M4 | M3 | M2 | M1 | M0 |
---------------------------------------------------------------- */
/*  REG
R0 R1 R2 R3 R4 R5 R6 R7 R8 R9 R10 R11 R12 R13/SP R14/LR R15/PC R16/CPSR */
typedef struct {
    u32 reboot_cpu;      /* 0x00  */
    u32 reboot_context;  /* 0x00  */
    u32 reboot_task;     /* 0x04  */
    u32 reboot_task_tcb; /* 0x04  */
    u8 task_name[DUMP_NAME_LEN];    /* 0x08  */
    u32 reboot_int;      /* 0x18  */
    u32 reboot_time;     /* 0x84 */

    u32 mod_id;      /* 0x1c  */
    u32 arg1;        /* 0x20  */
    u32 arg2;        /* 0x24  */
    u32 arg3;        /* 0x28  */
    u32 arg3_length; /* 0x2c  */
    u32 vec;         /* 0x30  */

    u32 cpu_max_num;     /* 0x88 */
    u32 cpu_online_num;  /* 0x88 */
    u8 version[DUMP_PRODUCT_VERSION_LEN];      /* 0xA0 */
    u8 compile_time[DUMP_BUILD_TIME_LEN]; /* 0xB0 */
    u32 reboot_reason;   /* m3专用 */
    u8 version_uuid[DUMP_IMAGEID_LEN];
    struct {
        char biref[DUMP_BASEINFO_EXC_DESC_LEN];
        char module[DUMP_BASEINFO_EXC_DESC_LEN];
        char detail[DUMP_BASEINFO_EXC_DESC_LEN];
    } extra_exc_desc;
} dump_base_info_s;

typedef bool (*exc_hook)(void *param);

#define CCORE "CP"
#define ACORE "AP"
#define NRCCORE "NRCP"
#define NRL2HACORE "NRL2HAC"
#define MDM_LPMCU "LPMCU"
#define UNKNOW_CORE "UNKNOW"
#define UNKNOW "UNKNOW"
#define DATA_ABORT "abort"
#define WDT "wdg"
#define NOARMAL "syserr"
#define UNDEFFINE "undefine"
#define NOC "noc"
#define DMSS "dmss"
#define RST_FAIL "rst_fail"
#define RST_NOT_SUPPORT "rst_not_support"
#define DLOCK "mdm_bus_err"
#define RST_FREQ "reset_freq"

#define DUMP_REBOOT_INT "Interrupt_No"
#define DUMP_REBOOT_TASK "task_name"

#define DUMP_MODID_OFFSET (offsetof(struct dump_baseinfo_head, mod_id))
#define DUMP_TASK_NAME_OFFSET (offsetof(struct dump_baseinfo_head, task_name))

typedef enum {
    DUMP_SEC_MDM_DDR = 0x0,              /**< TSP核使用的DDR */
    DUMP_SEC_SEC_SHARED = 0x1,           /**< 安全共享内存 */
    DUMP_SEC_LPHY_DUMP = 0x2,            /**< LPHY日志 */
    DUMP_SEC_NRPHY_DUMP = 0x3,           /**< NPHY 日志 */
    DUMP_SEC_EASYRF_DUMP = 0x4,          /**< EASYRF 日志 */
    DUMP_SEC_NRCCPU_DDR = 0x5,           /**< NRCCPU使用的DDR */
    DUMP_SEC_L2HAC_LLRAM = 0x6,          /**< L2HAC使用的LLRAM */
    DUMP_SEC_PDE = 0x7,                  /**< PDE 日志 */
    DUMP_SEC_NRCCPU_LLRAM = 0x8,         /**< NRCCPU使用LLRAM */
    DUMP_SEC_NRSHARE = 0x9,              /**< NR的共享内存 */
    DUMP_SEC_LTEVPHY_DUMP = 0xa,         /**< LTEV PHY的日志 */
    DUMP_SEC_LR_LLRAM = 0xb,             /**< LR使用的LLRAM */
    DUMP_SEC_TSP_L2MEM_DUMP = 0xc,       /**< TSP核使用的L2MEM */
    DUMP_SEC_TSP_DSS0_L1MEM_DUMP = 0xd,  /**< TSP DSS0使用的L1MEM */
    DUMP_SEC_TSP_DSS1_L1MEM_DUMP = 0xe,  /**< TSP DSS0使用的L1MEM */
    DUMP_SEC_TSP_DSS2_L1MEM_DUMP = 0xf,  /**< TSP DSS0使用的L1MEM */
    DUMP_SEC_TSP_DSS3_L1MEM_DUMP = 0x10, /**< TSP DSS0使用的L1MEM */
    DUMP_SEC_TVP_L1MEM_DUMP = 0x11,      /**< TVP核使用的L1 MEM */
    DUMP_SEC_TVP_L2MEM_DUMP = 0x12,      /**< TVP使用L2 MEM */
    DUMP_SEC_TVP_DUMP = 0x13,            /**< TVP的DDR日志 */
    DUMP_SEC_DS_DUMP = 0x14,
    DUMP_SEC_LPMCU_TCM = 0x15,
    DUMP_SEC_PHY_DUMP = 0x16,            /**< 物理层融合架构日志 */
    DUMP_SEC_MDM_DTS = 0x17,
    DUMP_SEC_MDM_RSV_DDR = 0x18,
    DUMP_SEC_MDM_UNCACHE_DDR = 0x19,
    DUMP_SEC_LPMCU_DDR = 0x1A,
    DUMP_SEC_TSP_DSS0_L1ITCM_DUMP = 0x1B, /**< TSP DSS0使用的L1 ITCM */
    DUMP_SEC_TSP_DSS1_L1ITCM_DUMP = 0x1C, /**< TSP DSS1使用的L1 ITCM */
    DUMP_SEC_TSP_DSS2_L1ITCM_DUMP = 0x1D, /**< TSP DSS2使用的L1 ITCM */
    DUMP_SEC_TSP_DSS3_L1ITCM_DUMP = 0x1E, /**< TSP DSS3使用的L1 ITCM */
    DUMP_SEC_PSA_DDR = 0x1F, /**< PSA DDR */
    DUMP_SEC_PSA_L2MEM = 0x20, /**< PSA L2MEM */
    DUMP_SEC_PSA_L1TCM0 = 0x21, /**< PSA CORE0 L1TCM */
    DUMP_SEC_PSA_L1TCM1 = 0x22, /**< PSA CORE0 L1TCM */
    DUMP_SEC_PSA_L1TCM2 = 0x23, /**< PSA CORE0 L1TCM */
    DUMP_SEC_PSA_L1TCM3 = 0x24, /**< PSA CORE0 L1TCM */
    DUMP_SEC_PSA_DUMP = 0x25, /**< PSA商用MINI_DUMP */
    DUMP_SEC_MDM_FWDTB_DUMP = 0x26, /**< modem_fw_dtb ddr,回收做堆区，fulldump需要出 */
    DUMP_SEC_MDM_WBNA_DUMP = 0x27, /**< mdm_wbna_ddr 堆区内存，fulldump导出 */
    DUMP_SEC_MDM_SYNC_BUFFER = 0x28, /**< mdm sync buffer，fulldump导出 */
    DUMP_SEC_PHY_TSP_DUMP = 0x29, /**< phy tsp 商用MINI_DUMP */
    DUMP_SEC_FILE_BUTT,
} dump_sec_file_e;

typedef enum {
    DUMP_SAVE_MDM_DDR = 0,            /* 注册顺序3 */
    DUMP_SAVE_SEC_SHARE = 1,          /* 注册顺序4 */
    DUMP_SAVE_LPHY = 2,               /* 注册顺序8 */
    DUMP_SAVE_NRPHY = 3,              /* 注册顺序14 */
    DUMP_SAVE_RFDSP = 4,              /* 注册顺序10 */
    DUMP_SAVE_NRCCPU_DDR = 5,         /* 注册顺序13 */
    DUMP_SAVE_PDE = 7,                /* 注册顺序5 */
    DUMP_SAVE_NR_SHARE = 9,           /* 注册顺序12 */
    DUMP_SAVE_LTEV = 10,              /* 注册顺序9 */
    DUMP_SAVE_NPHY = 19,              /* 注册顺序11 */
    DUMP_SAVE_MDM_DUMP = 100,         /* 注册顺序15 */
    DUMP_SAVE_MDM_SHARE = 101,        /* 注册顺序0 */
    DUMP_SAVE_SHARE_UNSEC = 102,      /* 注册顺序1 */
    DUMP_SAVE_SHARE_NSRO = 103,       /* 注册顺序2 */
    DUMP_SAVE_L1MEM = 104,            /* 注册顺序6 */
    DUMP_SAVE_L2MEM = 105,            /* 注册顺序7 */
    DUMP_SAVE_RESET_LOG = 106,        /* 注册顺序16 */
    DUMP_SAVE_SEC_LOGS = 107,         /* 注册顺序17 */
    DUMP_SAVE_MDM_DUMP_BACKUP = 108,  /* 注册顺序18 */
    DUMP_SAVE_LOGMEM = 109,        /* 注册顺序最早 */
    DUMP_SAVE_MDM_KO = 110,
    DUMP_SAVE_FIPC_INFO = 111, /* ipcmsg buf in phone */
    DUMP_SAVE_MCU_DDR = 112,       /* mcu_ddr */
    DUMP_SAVE_AP_DTS_DDR = 113,       /* ap_dts_ddr */
    DUMP_SAVE_DIAG_CYCLELOG = 116,
    DUMP_SAVE_DPA_DTCM = 117,
    DUMP_SAVE_DPA_ITCM = 118,
    DUMP_SAVE_MTEE_DDR = 119,
    DUMP_SAVE_LOG_BUTT,
} dump_save_file_id_e;

typedef void (*save_hook)(const char *path);

typedef struct {
    struct list_head list;
    dump_save_file_id_e id;
    save_hook func;
} dump_log_notifier_s;
typedef enum {
    STATE_IDLE,
    STATE_BUSY,
} dump_modem_state_e;
#define DUMP_MODEM_OFF 0x78564321

typedef struct {
    unsigned int magic;        /* magic num  0x5678fedc */
    unsigned int packet_num;   /* BD包个数 */
    unsigned int total_length; /* 总包长 */
    unsigned int lp_length;    /* 最后一个包的长度 */
    char file_name[DUMP_SECDUMP_NAME_LEN];        /* 需要保存的文件名 */
    unsigned int resv;
} dump_ddr_trans_head_info_s;

typedef enum {
    DUMP_MODEM_ACCESS_TOP = 0,
    DUMP_MODEM_ACCESS_TSP_MPSS,
    DUMP_MODEM_ACCESS_TSP_DSS0,
    DUMP_MODEM_ACCESS_TSP_DSS1,
    DUMP_MODEM_ACCESS_TSP_DSS2,
    DUMP_MODEM_ACCESS_TSP_DSS3,
    DUMP_MODEM_ACCESS_BUTT
} dump_modem_access_state_e;

typedef enum {
    DUMP_MDMACCESS_ON = 1, /* 钟电状态on，可以访问 */
    DUMP_MDMACCESS_OFF = 0, /* 钟电状态off，可以访问 */
    DUMP_MDMACCESS_API_NOTSUPPORT = -1, /* 当前平台不支持查询 */
} dump_modem_access_ret_e;
/*****************************************************************************
  3 函数声明
*****************************************************************************/
#ifdef ENABLE_BUILD_OM

void system_error(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length);
s32 bsp_dump_init(void);
dump_handle bsp_dump_register_hook(const char *name, dump_hook func);
s32 bsp_dump_unregister_hook(dump_handle handle);
u8 *bsp_dump_register_field(u32 mod_id, const char *name, u32 length, u16 version_id);
u8 *bsp_dump_get_field_addr(u32 field_id);
u8 *bsp_dump_get_field_phy_addr(u32 field_id);
s32 bsp_dump_register_log_notifier(dump_save_file_id_e id, save_hook func);
int bsp_dump_log_save(const char *dir, const char *file_name, const void *address, const void *phy_addr, u32 length);
int bsp_dump_log_append(const char *dir, const char *file_name, const void *address, const void *phy_addr, u32 length);
void bsp_dump_get_avaiable_size(u32 *size);
dump_product_type_e bsp_dump_get_product_type(void);
void bsp_dump_sec_traverse_file(void);
struct rdr_exception_info_s *bsp_dump_get_exc_node_info(void);
/* 查询modem中的一些区域当前是否可以访问(只判断钟电状态是否ok)
 * 返回值：dump_modem_access_ret_e，详见该枚举的注释
 */
int bsp_dump_get_modem_access_state(dump_modem_access_state_e state);
const dump_exc_info_s *bsp_dump_get_current_exc_info(void);

enum dump_callback_subsys {
    DUMP_MDMAP_CALLBACK = 0, /* 主要用于异步回调，在mdmap子系统完成后执行 */
    DUMP_CP_CALLBACK, /* 主要用于异步回调，在cp子系统完成或超时后执行 */
    DUMP_COMMON_CALLBACK, /* 主要用于异步回调，在所有子系统完成或超时之后执行 */
    DUMP_CALLBACK_BUTT,
};

#define DUMP_MOUDLE_CALLBACK_PRIO_LOWEST 49
/* @berif 注册dump异步回调接口
 * @par 描述:
 *      与dump_register_hook的区别在于，异步回调在任务上下文调用，若是PANIC这类异常，异步回调不会被调用
 * @param[in]  subsys, 回调归属于哪个子系统，例如如果归属于CP，那么在CP dump完成后就会被调用，不用等其他所有子系统完成
 * @param[in]  prio, 优先级，越低越好，最大不超过49，否则返回失败
 * @param[in]  func, 回调函数
 * @retval BSP_ERROR, 失败
 * @retval 其他值, 成功
 */
int bsp_dump_register_async_callback(enum dump_callback_subsys subsys, u32 prio, dump_hook func);

/* @berif 查询当前是否有正在处理的异常
 * @par 描述:
 *      从MDMAP收到异常，到单独复位/整机复位完成期间，都会返回true
 */
bool bsp_dump_check_if_exception_exist(void);

#ifdef CONFIG_EARLY_LOG
void bsp_show_mdmext_pstore(void);
#else
static inline void bsp_show_mdmext_pstore(void)
{
    return;
}
#endif
#else

static void inline system_error(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length)
{
    return;
}
static s32 inline bsp_dump_init(void)
{
    return 0;
}

static dump_handle inline bsp_dump_register_hook(const char *name, dump_hook func)
{
    return 0;
}

static inline s32 bsp_dump_unregister_hook(dump_handle handle)
{
    return 0;
}

static inline u8 *bsp_dump_register_field(u32 mod_id, const char *name, u32 length, u16 version_id)
{
    return NULL;
}

static inline u8 *bsp_dump_get_field_addr(u32 field_id)
{
    return 0;
}

static inline u8 *bsp_dump_get_field_phy_addr(u32 field_id)
{
    return 0;
}

static inline s32 bsp_dump_register_log_notifier(dump_save_file_id_e id, save_hook func)
{
    return 0;
}
static inline int bsp_dump_log_save(const char *dir, const char *file_name, const void *address, const void *phy_addr,
                                    u32 length)
{
    return 0;
}
static inline dump_product_type_e bsp_dump_get_product_type(void)
{
    return DUMP_MBB;
}

static inline void bsp_dump_sec_traverse_file(void)
{
    return;
}

static inline struct rdr_exception_info_s *bsp_dump_get_exc_node_info(void)
{
    return NULL;
}

static inline const dump_exc_info_s *bsp_dump_get_current_exc_info(void)
{
    return NULL;
}

static inline void bsp_dump_enable_cp_reset_for_str(u32 enable)
{
    return;
}
#endif

#ifdef __cplusplus
}
#endif

#endif
