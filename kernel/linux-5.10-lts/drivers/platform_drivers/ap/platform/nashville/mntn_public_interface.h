/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: mntn_public_interface.h
 */
#ifndef __MNTN_PUBLIC_INTERFACE_H__
#define __MNTN_PUBLIC_INTERFACE_H__

#include "pmic_interface.h"
#include "../common/mntn_dump_interface.h"
#include "../common/mntn_common_interface.h"

#define PMU_SAVE_CPU_ID (PMIC_HRST_REG18_ADDR(0))

#define MEMUPLOAD_SEC_MEM 0
#define MEMUPLOAD_NONE_SEC_MEM 1
#define AP_WATCHDOG_INT_NUMBER 431

#define SOC_MID_IOMCU_M7 0xC
#define SOC_IPF_MID     0x4
#define SOC_SOCP_MID    0xd
#define SOC_SPE_MID     0x14
#define SOC_MAA_MID     0x15
#define SOC_EICC_MID    0x19

enum {
    BL31_TRACE_EXCEPTION_SIZE = 0x100,
    BL31_TRACE_IRQ_SMC_SIZE = 0x3F000,
    BL31_TRACE_DMSS_SIZE = 0x0,
};

/* dmss exception trace */
typedef struct dmss_exception_trace_s {
    unsigned long long e_32k_time;
} dmss_exception_trace_t;

/* dump sec mem for memdump */
enum SECMEM_MODULE {
    M3_DDR_MEM,
    BL31_LOG_MEM,
    THEE_LOG_MEM,
    MAX_MEM,
};

#if defined(CONFIG_GCOV_KERNEL) || defined(CONFIG_HISI_GCOV_FASTBOOT)
struct mdump_gcov {
    unsigned long gcda_addr;
    unsigned int gcda_size;
};
#endif

/* special defined for avs keep reboot_reason */
struct mntn_reg_val {
        unsigned int init_flag;
        unsigned int scsysstat;
        unsigned int bootpoint;
};
void mntn_reg_save(void *mntn_reg);
void mntn_reg_restore(void *mntn_reg);

/*
 * reserved 4bytes for SD_JTAG_GPIO_NP_FLAG,
 * the flag is written in xloader, and read by UEFI IoInit,
 * 0x280 is the value of macro RDR_BASEINFO_SIZE
 */
#define JTAGMUX_GPIO_NP_MAGIC               0xABACADAE
#define RDR_JTAGMUX_GPIO_NP_FLAG_ADDR       (RESERVED_MNTN_PHYMEM_BASE + 0x280 - 0x4)

#define RDR_DDR_SEC_MEM_MAP_ADDR            (RESERVED_MNTN_PHYMEM_BASE + RESERVED_MNTN_PHYMEM_SIZE - RDR_DDR_SEC_MEM_MAP_SIZE)
#define RDR_DDR_SEC_MEM_MAP_SIZE            0x1000
#define RDR_VDEC_MEM_MAP_ADDR               (RDR_DDR_SEC_MEM_MAP_ADDR - RDR_VDEC_MEM_MAP_SIZE)
#define RDR_VDEC_MEM_MAP_SIZE               0x20000
#define RDR_DFA_MEM_MAP_ADDR                (RDR_VDEC_MEM_MAP_ADDR - RDR_DFA_MEM_MAP_SIZE)
#define RDR_DFA_MEM_MAP_SIZE                0x11000
#define RDR_FFTS_MEM_MAP_ADDR               (RDR_DFA_MEM_MAP_ADDR - RDR_FFTS_MEM_MAP_SIZE)
#define RDR_FFTS_MEM_MAP_SIZE               0x10000
#define RDR_DDR_MEM_MAP_ADDR                (RDR_FFTS_MEM_MAP_ADDR - RDR_DDR_MEM_MAP_SIZE)
#define RDR_DDR_MEM_MAP_SIZE                 0x40000
#define RDR_DSS_MEM_MAP_ADDR                (RDR_DDR_MEM_MAP_ADDR - RDR_DSS_MEM_MAP_SIZE)
#define RDR_DSS_MEM_MAP_SIZE                 0x0
#define RDR_EXCEPTION_TRACE_MEM_MAP_ADDR    (RDR_DSS_MEM_MAP_ADDR - RDR_EXCEPTION_TRACE_MEM_MAP_SIZE)
#define RDR_EXCEPTION_TRACE_MEM_MAP_SIZE    0x40000
#define RDR_CONN_MEM_MAP_ADDR               (RDR_EXCEPTION_TRACE_MEM_MAP_ADDR - RDR_CONN_TRACE_MEM_MAP_SIZE)
#define RDR_CONN_TRACE_MEM_MAP_SIZE         0x2000
#define RDR_NPU_MEM_MAP_ADDR                (RDR_CONN_MEM_MAP_ADDR - RDR_NPU_TRACE_MEM_MAP_SIZE)
#define RDR_NPU_TRACE_MEM_MAP_SIZE          0x10000
#define RDR_GENERAL_SEE_MEM_MAP_ADDR        (RDR_NPU_MEM_MAP_ADDR - RDR_GENERAL_SEE_MEM_MAP_SIZE)
#define RDR_GENERAL_SEE_MEM_MAP_SIZE        0x10000
#define RDR_BFM_MEM_MAP_ADDR                (RDR_GENERAL_SEE_MEM_MAP_ADDR - RDR_BFM_MEM_MAP_SIZE)
#define RDR_BFM_MEM_MAP_SIZE                0x10000
#define RDR_REGULATOR_MEM_MAP_ADDR          (RDR_BFM_MEM_MAP_ADDR - RDR_REGULATOR_MEM_MAP_SIZE)
#define RDR_REGULATOR_MEM_MAP_SIZE          0x10000
#define RDR_CLK_MEM_MAP_ADDR                (RDR_REGULATOR_MEM_MAP_ADDR - RDR_CLK_MEM_MAP_SIZE)
#define RDR_CLK_MEM_MAP_SIZE                0x10000
#define RDR_MODEMAP_MEM_MAP_ADDR            (RDR_CLK_MEM_MAP_ADDR - RDR_MODEMAP_MEM_MAP_SIZE)
#define RDR_MODEMAP_MEM_MAP_SIZE            0x0
#define RDR_EMMC_MEM_MAP_ADDR               (RDR_MODEMAP_MEM_MAP_ADDR - RDR_EMMC_MEM_MAP_SIZE)
#define RDR_EMMC_MEM_MAP_SIZE               0x40000
#define RDR_IVP_MEM_MAP_ADDR                (RDR_EMMC_MEM_MAP_ADDR - RDR_IVP_MEM_MAP_SIZE)
#define RDR_IVP_MEM_MAP_SIZE                0x40000
#define RDR_ISP_MEM_MAP_ADDR                (RDR_IVP_MEM_MAP_ADDR - RDR_ISP_MEM_MAP_SIZE)
#define RDR_ISP_MEM_MAP_SIZE                0x40000
#define RDR_IOM3_MEM_MAP_ADDR               (RDR_ISP_MEM_MAP_ADDR - RDR_IOM3_MEM_MAP_SIZE)
#define RDR_IOM3_MEM_MAP_SIZE               0x40000
#define RDR_LPM3_MEM_MAP_ADDR               (RDR_IOM3_MEM_MAP_ADDR - RDR_LPM3_MEM_MAP_SIZE)
#define RDR_LPM3_MEM_MAP_SIZE               0x70000
#define RDR_HIFI_MEM_MAP_ADDR               (RDR_LPM3_MEM_MAP_ADDR - RDR_HIFI_MEM_MAP_SIZE)
#define RDR_HIFI_MEM_MAP_SIZE               0x40000
#define RDR_TEEOS_MEM_MAP_ADDR              (RDR_HIFI_MEM_MAP_ADDR - RDR_TEEOS_MEM_MAP_SIZE)
#define RDR_TEEOS_MEM_MAP_SIZE              0x40000
#define RDR_CP_MEM_MAP_ADDR                 (RDR_TEEOS_MEM_MAP_ADDR - RDR_CP_MEM_MAP_SIZE)
#define RDR_CP_MEM_MAP_SIZE                 0x160000

/* Size: 4K, End at: 0xFDB23FFF */
#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFDB23000)

#define RESERVED_MNTN_PHYMEM_BASE                   0x2F880000
#define RESERVED_MNTN_PHYMEM_SIZE                   (0x620000)

/* only for DDR sec protect :kernel dump */
#define RESERVED_KERNEL_DUMP_PROTECT_BASE           0x80000000
#define RESERVED_KERNEL_DUMP_PRORECT_SIZE           (0x10000000)

#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE       0x10900000
#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE       (0x40000)

#define RESERVED_PSTORE_PHYMEM_BASE                 0x2FEC0000
#define RESERVED_PSTORE_PHYMEM_SIZE                 (0x100000)

#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE 0x1099C000
#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE (0x2000)

/* BL2 USB data buffer */
#define SUB_RESERVED_UNUSED_PHYMEM_BASE             0x10968000
#define SUB_RESERVED_UNUSED_PHYMEM_SIZE             (0x32000)

#define RESERVED_LPMX_CORE_PHYMEM_BASE              0x127C0000
#define RESERVED_LPMX_CORE_PHYMEM_SIZE              (0x80000)
/* -------------end---------------- */
#endif /* __MNTN_PUBLIC_INTERFACE_H__ */
