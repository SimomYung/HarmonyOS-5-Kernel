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
#define SOC_IPF_MID	0x4
#define SOC_SOCP_MID	0xd
#define SOC_SPE_MID	0x14
#define SOC_MAA_MID	0x15
#define SOC_EICC_MID	0x19

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
#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE     (0x2000)
#define RESERVED_KERNEL_DUMP_PROTECT_BASE            0x80000000
#define RESERVED_KERNEL_DUMP_PRORECT_SIZE            0x10000000
#define RESERVED_PSTORE_PHYMEM_BASE                  0x301C0000
#define RESERVED_PSTORE_PHYMEM_SIZE                  0x100000
#define RESERVED_MNTN_PHYMEM_BASE                    0x2FB80000
#define RESERVED_MNTN_PHYMEM_SIZE                    0x620000

#define RDR_DDR_SEC_MEM_MAP_ADDR            (RESERVED_MNTN_PHYMEM_BASE + RESERVED_MNTN_PHYMEM_SIZE - RDR_DDR_SEC_MEM_MAP_SIZE)
#define RDR_DDR_SEC_MEM_MAP_SIZE            0x1000
#define RDR_FFTS_MEM_MAP_ADDR               (RDR_DDR_SEC_MEM_MAP_ADDR - RDR_FFTS_MEM_MAP_SIZE)
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
#define RDR_LPM3_MEM_MAP_SIZE               0x90000
#define RDR_HIFI_MEM_MAP_ADDR               (RDR_LPM3_MEM_MAP_ADDR - RDR_HIFI_MEM_MAP_SIZE)
#define RDR_HIFI_MEM_MAP_SIZE               0x40000
#define RDR_TEEOS_MEM_MAP_ADDR              (RDR_HIFI_MEM_MAP_ADDR - RDR_TEEOS_MEM_MAP_SIZE)
#define RDR_TEEOS_MEM_MAP_SIZE              0x40000
#define RDR_CP_MEM_MAP_ADDR                 (RDR_TEEOS_MEM_MAP_ADDR - RDR_CP_MEM_MAP_SIZE)
#define RDR_CP_MEM_MAP_SIZE                 0x160000

#define RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE         0x108C0000
#define RESERVED_DRV_RESERVE_MEM_PHYMEM_SIZE         0x100000
/* Size: 4K, End at: 0xFDB23FFF */
#define SOC_ACPU_SCTRL_BASE_ADDR                                (0xFDB23000)

#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE     (0x2000)

/* FORM RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE END */
#define SUB_RESERVED_MNTN_DUMP_PHYMEM_SIZE              0x1000
#define SUB_RESERVED_MNTN_DUMP_PHYMEM_BASE              (RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE + \
                                                            RESERVED_DRV_RESERVE_MEM_PHYMEM_SIZE - \
                                                            SUB_RESERVED_MNTN_DUMP_PHYMEM_SIZE)

#define RESERVED_SECOS_CMDMEM_SIZE                      0x1000
#define RESERVED_SECOS_CMDMEM_BASE                      (SUB_RESERVED_MNTN_DUMP_PHYMEM_BASE - \
                                                            RESERVED_SECOS_CMDMEM_SIZE)


/* this memory is submemory of FAST_KER_AND start */
#define RESERVED_FAST_KER_AND_PHYMEM_BASE               RESERVED_DRV_RESERVE_MEM_PHYMEM_BASE
#define RESERVED_FAST_KER_AND_PHYMEM_SIZE               0x40000

#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE           (RESERVED_FAST_KER_AND_PHYMEM_BASE + \
                                                         RESERVED_FAST_KER_AND_PHYMEM_SIZE)
#define SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE           0x40000

#define SUB_RESERVED_SCHARGE_PYHMEM_BASE                (SUB_RESERVED_FASTBOOT_LOG_PYHMEM_BASE + \
                                                         SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE)
#define SUB_RESERVED_SCHARGE_PYHMEM_SIZE                0x1000

/* BL31 & Kernel share this 64K memory*/
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE     (SUB_RESERVED_SCHARGE_PYHMEM_BASE + \
                                                         SUB_RESERVED_SCHARGE_PYHMEM_SIZE)
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE     0x10000

/* for lcd gamma data 4k */
#define SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_BASE          (ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE + \
                                                         ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_SIZE          0x1000

/* for him data 4k */
#define SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_BASE  (SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_LCD_GAMMA_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_SIZE  0x1000

/* for security random number inject 4k */
#define SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_BASE     (SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_BRIGHTNESS_CHROMA_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_SIZE     0x1000

/* for BL2 log 64k*/
#define ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_BASE        (SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_BASE + \
                                                            SUB_RESERVED_SEC_RANDNUM_INJECT_PHYMEM_SIZE)
#define ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_SIZE        0x10000

#define SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE             (ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_BASE + \
                                                            ATF_SUB_RESERVED_BL2_LOG_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE             0x1000

#define HISI_SUB_RESERVED_UEFI_DDRINFO_SHARE_BASE       SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE
#define HISI_SUB_RESERVED_UEFI_DDRINFO_SHARE_SIZE       SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE

#define SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_BASE         (SUB_RESERVED_DDR_REPAIR_PHYMEM_BASE + \
                                                            SUB_RESERVED_DDR_REPAIR_PHYMEM_SIZE)
#define SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_SIZE         0x2000

/* for BL2 USB forcible loading 4K + 200k */
#define SUB_RESERVED_USB_BL2_SHARE_BASE                 (SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_BASE + \
                                                            SUB_RESERVED_THEE_SHARE_MEM_PHYMEM_SIZE)
#define SUB_RESERVED_USB_BL2_SHARE_SIZE                 0x2000

#define RESERVED_SOC_SPEC_INFO_PHYMEM_BASE              (SUB_RESERVED_USB_BL2_SHARE_BASE + \
                                                            SUB_RESERVED_USB_BL2_SHARE_SIZE)
#define RESERVED_SOC_SPEC_INFO_PHYMEM_SIZE              0x1000


#define SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_BASE      (RESERVED_SOC_SPEC_INFO_PHYMEM_BASE + \
                                                            RESERVED_SOC_SPEC_INFO_PHYMEM_SIZE)
#define SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_SIZE      0x1000

#define SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE     (SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_BASE+ \
                                                            SUB_RESERVED_FASTBOOT_LOGLEVEL_PYHMEM_SIZE)
#ifndef SUB_RESERVED_UNUSED_PHYMEM_BASE
#define SUB_RESERVED_UNUSED_PHYMEM_BASE                 (SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE + \
                                                            SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE)
#endif
#ifndef SUB_RESERVED_UNUSED_PHYMEM_SIZE
#define SUB_RESERVED_UNUSED_PHYMEM_SIZE                 (RESERVED_SECOS_CMDMEM_BASE - \
                                                            SUB_RESERVED_UNUSED_PHYMEM_BASE)
#endif
/* Register description: 用于软件使用的存储数据寄存器12
   Bit domain definition UNION:  SOC_SCTRL_SCBAKDATA12_UNION */
#define SOC_SCTRL_SCBAKDATA12_ADDR(base)                      ((base) + 0x43CUL)

/* -------------end---------------- */
#endif /* __MNTN_PUBLIC_INTERFACE_H__ */
