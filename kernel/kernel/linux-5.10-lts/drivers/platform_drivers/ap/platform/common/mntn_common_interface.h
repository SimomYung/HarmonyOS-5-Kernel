/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mntn common interface
 * Author: zhaominchao
 * Create: 2020-10-21
 */

#ifndef __MNTN_COMMON_INTERFACE__
#define __MNTN_COMMON_INTERFACE__

/*pmu_reg reserved for record reboot_reason*/
#define PMU_RESET_REG_OFFSET (PMIC_HRST_REG13_ADDR(0))
#define RST_FLAG_MASK (0xFF)
#define PMU_RESET_VALUE_USED 0xFFFFFF00
#define BOOTUP_KEYPOINT_OFFSET (PMIC_HRST_REG14_ADDR(0))

/*
 * From 0x80--0x88, it records AP_S_COLDBOOT in FPGA.
 * From 0x88--0x90, it records reboot_type in FPGA.
 * From 0x90--0x100, it records bootup_keypoint in FPGA
 */
/*---------------------------------start---------------------------------------*/
#ifndef RESERVED_MNTN_PHYMEM_BASE_UNIQUE
#define RESERVED_MNTN_PHYMEM_BASE_UNIQUE            (RESERVED_MNTN_PHYMEM_BASE)
#endif
#define PMU_RESET_RECORD_DDR_AREA_SIZE  0x100

/* The start offset must equal to RDR_BASEINFO_SIZE */
#define BOARD_COLD_START_ADDR       ((RESERVED_MNTN_PHYMEM_BASE_UNIQUE) + 0x2A0)
#define FPGA_RESET_REG_ADDR         ((RESERVED_MNTN_PHYMEM_BASE_UNIQUE) + 0x2A8)
#define FPGA_BOOTUP_KEYPOINT_ADDR   ((RESERVED_MNTN_PHYMEM_BASE_UNIQUE) + 0x2B0)
#define FPGA_EXCSUBTYPE_REG_ADDR   ((RESERVED_MNTN_PHYMEM_BASE_UNIQUE) + 0x2B8)
/*---------------------------------end-----------------------------------------*/

#define DFX_HEAD_SIZE 512
#define TOTAL_NUMBER 5
#define FASTBOOTLOG_SIZE SUB_RESERVED_FASTBOOT_LOG_PYHMEM_SIZE
#define LAST_KMSG_SIZE (RESERVED_PSTORE_PHYMEM_SIZE / 2)
#define LAST_APPLOG_SIZE (RESERVED_PSTORE_PHYMEM_SIZE / 8)
#define EVERY_NUMBER_SIZE (DFX_HEAD_SIZE + LAST_APPLOG_SIZE + LAST_KMSG_SIZE + FASTBOOTLOG_SIZE)
#define DFX_MAGIC_NUMBER 0x2846579
#define DFX_USED_SIZE (EVERY_NUMBER_SIZE*(TOTAL_NUMBER+1)+DFX_HEAD_SIZE)

#define GET_CYCLE_INDEX(idx, max_idx) (((idx + 1) >= (max_idx)) ? (1) : ((idx) + 1))
#define BOOTCHAIN_LOG_VALID_MAGIC 0x5A5A
#define BOOTCHAIN_LOG_MAX_IDX 800
#define BOOTCHAIN_LOG_LEN 128
#define BOOTCHAIN_LOG_MAX_LEN 100 * 1024

struct bootchain_log_header {
    unsigned int magic;
    unsigned int last_read_idx;
    unsigned int last_write_idx;
};

/* special defined for avs keep reboot_reason */
struct mntn_reg_val {
        unsigned int init_flag;
        unsigned int scsysstat;
        unsigned int bootpoint;
};

struct dfx_head_info {
	int magic;
	unsigned int total_number;
	unsigned int cur_number;
	unsigned int need_save_number;
	unsigned long long every_number_addr[TOTAL_NUMBER];
	unsigned long long every_number_size;
	unsigned long long temp_number_addr;
};

struct every_number_info {
	unsigned long long rtc_time;
	unsigned long long boot_time;        /*measure as nanosecond*/
	unsigned long long bootup_keypoint;
	unsigned long long reboot_type;
	unsigned long long exce_subtype;
	unsigned long long fastbootlog_start_addr;
	unsigned long long fastbootlog_size;
	unsigned long long last_kmsg_start_addr;
	unsigned long long last_kmsg_size;
	unsigned long long last_applog_start_addr;
	unsigned long long last_applog_size;
};

enum boot_stage_point
{
	STAGE_START                         = 1,
	STAGE_XLOADER_START                 = STAGE_START,
	STAGE_XLOADER_STORAGE_INIT_FAIL        = 2,
	STAGE_XLOADER_STORAGE_INIT_OK          = 3,
	STAGE_XLOADER_DDR_INIT_FAIL         = 4,
	STAGE_XLOADER_DDR_INIT_OK           = 5,
	STAGE_XLOADER_RD_VRL_FAIL           = 6,
	STAGE_XLOADER_CHECK_VRL_ERROR       = 7,
	STAGE_XLOADER_IMG_TOO_LARGE         = 8,
	STAGE_XLOADER_READ_FASTBOOT_FAIL    = 9,
	STAGE_XLOADER_LOAD_HIBENCH_FAIL     = 10,
	STAGE_XLOADER_SEC_VERIFY_FAIL       = 11,
	STAGE_XLOADER_GET_IMGSIZE_FAIL      = 12,
	STAGE_XLOADER_IMGSIZE_ERROR         = 13,
	STAGE_XLOADER_VRL_CHECK_ERROR       = 14,
	STAGE_XLOADER_SECURE_VERIFY_ERROR   = 15,
	STAGE_XLOADER_READ_UCE_FAIL         = 16,
	STAGE_XLOADER_UCE_SEC_VERIFY_FAIL   = 17,
	STAGE_XLOADER_UCE_SECURE_VERIFY_ERROR   = 18,
	STAGE_XLOADER_XLOADER3_VERIFY_ERROR   = 19,
	STAGE_XLOADER_VECTOR_SEC_VERIFY_FAIL   = 20,
	STAGE_XLOADER_LOAD_SOCBENCH_END = STAGE_XLOADER_VECTOR_SEC_VERIFY_FAIL,
	STAGE_XLOADER_LOAD_FASTBOOT_START   = 21,
	STAGE_XLOADER_LOAD_FASTBOOT_END     = 22,
	STAGE_XLOADER_BINDFILE_VERIFY_ERROR = 23,
	STAGE_XLOADER_DISRESET_MAINCORE_START = 24,
#ifdef CONFIG_XLOADER_DDR_ENABLE
	STAGE_XLOADER_DDR_VERIFY_ERROR      = 24,  /*only used in ddr test xloader*/
#endif
	STAGE_XLOADER_END                   = 25,

	STAGE_FASTBOOT_START                = 26,
	STAGE_FASTBOOT_STORAGE_INIT_START      = 27,
	STAGE_FASTBOOT_STOR_INIT_ERR       = 28,
	STAGE_FASTBOOT_STORAGE_INIT_OK         = 29,
	STAGE_FASTBOOT_DDR_INIT_START       = 30,
	STAGE_FASTBOOT_DISPLAY_INIT_START   = 31,
	STAGE_FASTBOOT_PRE_BOOT_INIT_START  = 32,
	STAGE_FASTBOOT_LD_OTHER_IMGS_START  = 33,
	STAGE_FASTBOOT_LD_KERNEL_IMG_START  = 34,
	STAGE_FASTBOOT_BOOT_KERNEL_START    = 35,
	STAGE_FASTBOOT_LOADLPMCU_FAIL       = 38,
	STAGE_FASTBOOT_SECBOOT_INIT_START   = 39,
	STAGE_UEFI_CENTRYPOINT_START        = 40,
	STAGE_UEFI_MEMORY_INIT_OK           = 41,
	STAGE_UEFI_MPSERVICES_START         = 42,
	STAGE_UEFI_STORAGE_INIT_OK          = 43,
	STAGE_UEFI_BBOX_INIT_START          = 44,
	STAGE_UEFI_WATCHDOG_INIT_OK         = 45,
	STAGE_UEFI_PREBOOT_INIT_START       = 46,
	STAGE_UEFI_BDS_ENTRY_START          = 47,
	STAGE_FASTBOOT_END                  = 50,

	STAGE_BL2_EARLY_INITCALL            = 51,
	STAGE_BL2_LATE_INITCALL             = 52,
	STAGE_BL31_EARLY_INITCALL           = 53,
	STAGE_BL31_LATE_INITCALL            = 54,

	STAGE_CDC_ACE_RDA_START             = 59,
	STAGE_CDC_ACE_RDA_LDK_INIT          = 60,
	STAGE_CDC_ACE_RDA_SUSPEND           = 61,
	STAGE_CDC_ACE_RDA_RESUME            = 62,
	STAGE_CDC_ACE_RDA_END               = 63,
	STAGE_ALOADER_START                 = 64,
	STAGE_ALOADER_STORAGE_INIT_OK       = 65,
	STAGE_ALOADER_STORAGE_INIT_START    = 66,
	STAGE_ALOADER_STOR_INIT_ERR         = 67,
	STAGE_ALOADER_PRE_BOOT_INIT_START   = 68,

	STAGE_KERNEL_EARLY_INITCALL         = 75,
	STAGE_KERNEL_PURE_INITCALL          = 77,
	STAGE_KERNEL_CORE_INITCALL          = 79,
	STAGE_KERNEL_CORE_INITCALL_SYNC     = 81,
	STAGE_KERNEL_POSTCORE_INITCALL      = 83,
	STAGE_KERNEL_POSTCORE_INITCALL_SYNC = 85,
	STAGE_KERNEL_ARCH_INITCALL          = 87,
	STAGE_KERNEL_ARCH_INITCALLC         = 89,
	STAGE_KERNEL_SUBSYS_INITCALL        = 81,
	STAGE_KERNEL_SUBSYS_INITCALL_SYNC   = 83,
	STAGE_KERNEL_FS_INITCALL            = 85,
	STAGE_KERNEL_FS_INITCALL_SYNC       = 87,
	STAGE_KERNEL_ROOTFS_INITCALL        = 89,
	STAGE_KERNEL_DEVICE_INITCALL        = 91,
	STAGE_KERNEL_DEVICE_INITCALL_SYNC   = 93,
	STAGE_KERNEL_LATE_INITCALL          = 95,
	STAGE_KERNEL_LATE_INITCALL_SYNC     = 97,
	STAGE_KERNEL_CONSOLE_INITCALL       = 99,
	STAGE_KERNEL_SECURITY_INITCALL      = 101,
	STAGE_KERNEL_BOOTANIM_COMPLETE      = 103,

	STAGE_INIT_INIT_START      = 110,
	STAGE_INIT_ON_EARLY_INIT   = 111,
	STAGE_INIT_ON_INIT         = 112,
	STAGE_INIT_ON_EARLY_FS     = 113,
	STAGE_INIT_ON_FS           = 114,
	STAGE_INIT_ON_POST_FS      = 115,
	STAGE_INIT_ON_POST_FS_DATA = 116,
	STAGE_INIT_ON_EARLY_BOOT   = 117,
	STAGE_INIT_ON_BOOT         = 118,

	STAGE_OS_ZYGOTE_START = 150,
	STAGE_OS_VM_START     = 151,
	STAGE_OS_PHASE_WAIT_FOR_DEFAULT_DISPLAY   = 152,
	STAGE_OS_PHASE_LOCK_SETTINGS_READY        = 153,
	STAGE_OS_PHASE_SYSTEM_SERVICES_READY      = 154,
	STAGE_OS_PHASE_ACTIVITY_MANAGER_READY     = 155,
	STAGE_OS_PHASE_THIRD_PARTY_APPS_CAN_START = 156,

	/* [160, 250] is used for kernel bootup_keypoint. */
	STAGE_HMKERNEL_START			= 160,
	STAGE_HMKERNEL_ELFLOADER_START		= STAGE_HMKERNEL_START,
	STAGE_HMKERNEL_ELFLOADER_FINISH		= 161,
	STAGE_HMKERNEL_KERNEL_START		= 162,
	STAGE_HMKERNEL_KERNEL_POST_START	= 163,
	STAGE_HMKERNEL_KERNEL_FINISH		= 164,
	STAGE_HMKERNEL_KERNEL_NR		= 165,
	STAGE_HMKERNEL_SYSMGR_START		= 170,
	STAGE_HMKERNEL_SYSMGR_PRE_INIT		= 172,
	STAGE_HMKERNEL_SYSMGR_EARLY_BOOT_DONE	= 174,
	STAGE_HMKERNEL_SYSMGR_MEM_INIT		= 176,
	STAGE_HMKERNEL_SYSMGR_PROCESS_INIT	= 178,
	STAGE_HMKERNEL_SYSMGR_BOOT_DONE		= 180,
	STAGE_HMKERNEL_END			= 240,

	/* [160, 250] is used for kernel bootup_keypoint. */
	STAGE_OS_BOOT_SUCCESS = 250,
	STAGE_BOOTUP_END                   = STAGE_OS_BOOT_SUCCESS,

	STAGE_KERNEL_ULSR_START				= 251,
	STAGE_KERNEL_ULSR_SUSPEND           = STAGE_KERNEL_ULSR_START,
	STAGE_KERNEL_ULSR_RESUME            = 252,
	STAGE_KERNEL_ULSR_END				= STAGE_KERNEL_ULSR_RESUME,

	STAGE_SHUTDOWN_START				= 253,
	STAGE_SHUTDOWN_VM					= STAGE_SHUTDOWN_START,
	STAGE_SHUTDOWN_END					= 255,
	STAGE_END                           = STAGE_SHUTDOWN_END,
};

enum mntn_switch
{
	MNTN_NVE_VALID = 0,
	MNTN_WDT_MIN,
	MNTN_AP_WDT = MNTN_WDT_MIN,
	MNTN_GLOBAL_WDT,
	MNTN_MODEM_WDT,
	MNTN_LPM3_WDT,
	MNTN_IOM3_WDT,
	MNTN_HIFI_WDT,
	MNTN_SECOS_WDT,
	MNTN_ISP_WDT,
	MNTN_IVP_WDT,
	MNTN_OCBC_WDT = 10,
	MNTN_UCE_WDT,
	MNTN_RESERVED_WDT3,
	MNTN_WDT_MAX = MNTN_RESERVED_WDT3,
	MNTN_FST_DUMP_MEM,
	MNTN_MNTN_DUMP_MEM,
	MNTN_SD2JTAG,
	MNTN_PRESS_KEY_TO_FASTBOOT,
	MNTN_PANIC_INTO_LOOP,
	MNTN_GOBAL_RESETLOG,
	MNTN_NOC_INT_HAPPEN,
	MNTN_NOC_ERROR_REBOOT = 20,
	MNTN_DFXPARTITION_TO_FILE,
	MNTN_DDR_ERROR_REBOOT,
	MNTN_GENERAL_SEE, /* GENERAL_SEE */
	MNTN_WATCHPOINT_EN,
	MNTN_KMEMLEAK_SWITCH,
	MNTN_FB_PANIC_REBOOT,
	MNTN_MEM_TRACE = 27, /*Bit:27: Memory Trace hook switch.*/
	MNTN_FTRACE,
	MNTN_EAGLE_EYE,
	MNTN_KERNEL_DUMP_ENABLE = 30, /*Hook switch is the same one of kdump.*/
	MNTN_SD2DJTAG,
	MNTN_MMC_TRACE,
	MNTN_LPM3_PANIC_INTO_LOOP,
	MNTN_TRACE_CLK_REGULATOR,
	MNTN_CORESIGHT,
	MNTN_DMSSPT,
	MNTN_HHEE,
	MNTN_KASLR,
	MNTN_SD2UART6,
	MNTN_L3CACHE_ECC,
	MNTN_NOC_TRACE,
	MNTN_STM_TRACE,
	MNTN_AGING_WDT, /* aging watchdog test */
	MNTN_CMA_TRACE,
	MNTN_SD2TXPJTAG,
	MNTN_XOM,
	MNTN_HKRR,
	MNTN_THEE,
	MNTN_NON_RDA_WDT, /* Non-RDA vwatchdog */
	MNTN_NON_RDA_RESET_SOLO, /* Non-RDA Reset Solo */
	/*Add above, and keep the same as definition in reboot_reason.h in fastboot !!!!*/
	MNTN_BOTTOM
};

typedef enum
{
    AP_S_COLDBOOT          = 0x0,
    bootloader             = 0x01,
    recovery               = 0x02,
    resetfactory           = 0x03,
    resetuser              = 0x04,
    sdupdate               = 0x05,
    chargereboot           = 0x06,
    resize                 = 0x07,
    erecovery              = 0x08,
    usbupdate              = 0x09,
    cust                   = 0x0a,
    oem_rtc                = 0x0c,
    UNKNOWN                = 0x0d,
    mountfail              = 0x0e,
    hungdetect             = 0x0f,
    COLDBOOT               = 0x10,
    updatedataimg          = 0x11,
    at2resetfactory        = 0x12,
    AP_S_FASTBOOTFLASH     = 0x13,
    AP_S_PRESS6S           = 0x14,
    BR_UPDATE_USB          = 0x15,
    BR_UPDATA_SD_FORCE     = 0x16,
    BR_KEY_VOLUMN_UP       = 0x17,
    BR_PRESS_1S            = 0x18,
    BR_PRESS_10S           = 0x19,
    BR_CHECK_RECOVERY      = 0x1a,
    BR_CHECK_ERECOVERY     = 0x1b,
    BR_CHECK_SDUPDATE      = 0x1c,
    BR_CHECK_USBUPDATE     = 0x1d,
    BR_CHECK_RESETFACTORY  = 0x1e,
    BR_CHECK_HOTAUPDATE    = 0x1f,
    BR_POWERONNOBAT        = 0x20,
    BR_NOGUI               = 0x21,
    BR_FACTORY_VERSION     = 0x22,
    BR_RESET_HAPPEN        = 0x23,
    BR_POWEROFF_ALARM      = 0x24,
    BR_POWEROFF_CHARGE     = 0x25,
    BR_POWERON_BY_SMPL     = 0x26,
    BR_CHECK_UPDATEDATAIMG = 0x27,
    BR_REBOOT_CPU_BUCK     = 0x28,
    BR_L2_CACHE_FAIL       = 0x29,
    BR_POWERON_CHARGE      = 0x2a,
    gpscoldboot            = 0x2b, /* solve gps chip hardware bug */
    atfactoryreset0        = 0x2c,
    AP_S_PRESSKEY_COUNT    = 0x2d,
    fastbootd              = 0x2e,
    hibench_slt            = 0x2f,
    hibench_fastboot       = 0x30,
	BR_CHECK_PXEUPDATE     = 0X31,
	BR_CHECK_F10UPDATE     = 0X32,
	BR_FASTBOOT_KEY        = 0X34,
    REBOOT_REASON_LABEL1   = 0x40,    /*小于REBOOT_REASON_LABEL1的复位原因是一个域，
                                    表示非异常或非blackbox框架使用，流程上是不需要通过框架保存log的*/
    AP_S_ABNORMAL        = REBOOT_REASON_LABEL1,
    AP_S_TSENSOR0        = 0x41,
    AP_S_TSENSOR1        = 0x42,
    AP_S_AWDT            = 0x43,
    CHARGER_S_WDT        = 0x44,
    G3D_S_G3DTSENSOR     = 0x45,
    LPM3_S_LPMCURST      = 0x46,
    CP_S_CPTSENSOR       = 0x47,
    IOM3_S_IOMCURST      = 0x48,
    ASP_S_ASPWD          = 0x49,
    CP_S_CPWD            = 0x4a,
    IVP_S_IVPWD          = 0x4b,
    ISP_S_ISPWD          = 0x4c,
    AP_S_DDR_UCE_WD      = 0x4d,
    AP_S_DDR_FATAL_INTER = 0x4e,
    OCBC_S_WD            = 0x4f,
    AP_S_PMU             = 0x50,
    AP_S_SMPL            = 0x51,
    AP_S_SCHARGER        = 0x52,
    LPM3_S_EXCEPTION     = 0x53,
    MMC_S_EXCEPTION      = 0x54,
    LPCPU_S_EXCEPTION    = 0x55,
    LPGPU_S_EXCEPTION    = 0x56,
    AP_S_VWDT            = 0x57,
    SILS_S_EXCEPTION     = 0x58,
    VENC_S_VENCWD        = 0x59,
    VDEC_S_VDECWD        = 0x5a,
    AO_S_AO_WD           = 0x5b, /* to reduce exception number, ao wd0 and ao wd1 is called AO_S_AO_WD, fastbootlog scsysstate value can diff them */
    DDR_LP_CTRL_S_WD     = 0x5c,
    SYS_CTRL_S_WD        = 0x5d,
    GPU_LP_CTRL_S_WD     = 0x5e,
    CPU_LP_CTRL_S_WD     = 0x5f,
    REBOOT_REASON_LABEL2 = 0x60,    /*大于等于REBOOT_REASON_LABEL1，且小于REBOOT_REASON_LABEL2的复位原因是一个域，
                                     由系统硬件触发的异常，需要在系统重启后保存*/
    AP_S_PANIC           = REBOOT_REASON_LABEL2,
    AP_S_NOC             = 0x61,
    AP_S_RESUME_SLOWY    = 0x62,
    AP_S_DDRC_SEC        = 0x63,
    AP_S_F2FS            = 0x64,
    AP_S_COMBINATIONKEY  = 0x65,
    AP_S_BL31_PANIC      = 0x66,
    AP_S_MAILBOX         = 0x67,
    AP_S_HHEE_PANIC      = 0x68,
    AP_S_SUBPMU          = 0x69,
    AP_S_VENDOR_PANIC    = 0x6A,
    AP_S_PMU2            = 0x6B,
    REBOOT_REASON_LABEL3 = 0x70,    /*大于等于REBOOT_REASON_LABEL2，且小于REBOOT_REASON_LABEL3的复位原因是一个域，
                                    表示是ap测发起的异常，流程上需要复位之后，通过框架保存log*/
    CP_S_EXCEPTION_START = REBOOT_REASON_LABEL3,
    CP_S_MODEMDMSS       = CP_S_EXCEPTION_START,
    CP_S_MODEMNOC        = 0x71,
    CP_S_MODEMAP         = 0x72,
    CP_S_EXCEPTION       = 0x73,
    CP_S_RESETFAIL       = 0x74,
    CP_S_NORMALRESET     = 0x75,
    CP_S_RILD_EXCEPTION  = 0x76,
    CP_S_3RD_EXCEPTION   = 0x77,

    CP_S_DRV_EXC       = 0x78,
    CP_S_PAM_EXC       = 0x79,
    CP_S_GUAS_EXC      = 0x7a,
    CP_S_CTTF_EXC      = 0x7b,
    CP_S_CAS_CPROC_EXC = 0x7c,
    CP_S_GUDSP_EXC     = 0x7d,
    CP_S_TLPS_EXC      = 0x7e,
    CP_S_TLDSP_EXC     = 0x7f,
    CP_S_CPHY_EXC      = 0x80,
    CP_S_GUCNAS_EXC    = 0x81,
    CP_S_EXCEPTION_END = CP_S_GUCNAS_EXC,

    SOCHIFI_S_EXCEPTION   = 0x82,
    HIFI_S_RESETFAIL      = 0x83,
    ISP_S_EXCEPTION       = 0x84,
    IVP_S_EXCEPTION       = 0x85,
    IOM3_S_EXCEPTION      = 0x86,
    TEE_S_EXCEPTION       = 0x87,
    CODECHIFI_S_EXCEPTION = 0x88,
    IOM3_S_USER_EXCEPTION = 0x89,
    GENERAL_SEE_S_EXCEPTION     = 0x8a,
    NPU_S_EXCEPTION       = 0x8b,
    WIFI_S_EXCEPTION      = 0x8c,
    BFGX_S_EXCEPTION      = 0x8d,
    AUDIO_CODEC_EXCEPTION = 0x8e,
    DSS_S_EXCEPTION      = 0x8f,
    GPU_S_EXCEPTION      = 0x90,
	FFTS_S_EXCEPTION      = 0x91,
    IODIE_S_EXCEPTION    = 0x92,
	DPA_S_EXCEPTION      = 0x93,
	DPA_S_HOST           = 0x94,
	IOM3_S_EXCEPTION_PANIC = 0x95, 
    REBOOT_REASON_LABEL4  = 0xa0,    /*大于等于REBOOT_REASON_LABEL3，且小于REBOOT_REASON_LABEL4的复位原因是一个域，
                                    表示是非ap发起的复位，包括lpm3、modem、hifi、isp等
                                    流程上是复位之前，通过框架保存log，复位起来之后还需要框架保存一些ap的log*/
    XLOADER_S_DDRINIT_FAIL            = REBOOT_REASON_LABEL4,
    XLOADER_STOR_INIT_ERR           = 0xa1,
    XLOADER_S_LOAD_FAIL               = 0xa2,
    XLOADER_S_VERIFY_FAIL             = 0xa3,
    XLOADER_S_WATCHDOG                = 0xa4,
    XLOADER_SE_S_PANIC              = 0xa5,
    XLOADER_MEMORY_REPAIR             = 0xa6,
    XLOADER_AUTH_USB_FAIL             = 0xa7,
    XLOADER_PART_LOAD_FAIL            = 0xa8,

    FASTBOOT_STOR_INIT_ERR            = 0xb0,
    FASTBOOT_S_PANIC                  = 0xb1,
    FASTBOOT_S_WATCHDOG               = 0xb2,
    FASTBOOT_OCV_VOL_ERR              = 0xb3,
    FASTBOOT_BAT_TEMP_ERR             = 0xb4,
    FASTBOOT_MISC_ERR                 = 0xb5,
    FASTBOOT_LD_DTIMG_ERR             = 0xb6,
    FASTBOOT_LD_OTHRIMG_ERR           = 0xb7,
    FASTBOOT_KERNELIMG_ERR            = 0xb8,
    FASTBOOT_LOADLPMCU_FAIL           = 0xb9,
    FASTBOOT_VERIFY_FAIL              = 0xba,
    FASTBOOT_SOC_TEMP_ERR             = 0xbb,
    FASTBOOT_FLASHCERT_FAIL           = 0xbc,
    FASTBOOT_MULCOREON_FAIL           = 0xbd,
    FASTBOOT_MULCOREOFF_FAIL          = 0xbe,
    FASTBOOT_S_SOC_SPEC               = 0xbf,
    FASTBOOT_LOADIOMCU_FAIL           = 0xc0,
    FASTBOOT_TEE_S_EXCEPTION          = 0xc1,
    FASTBOOT_SEPLAT_FAIL              = 0xc2,
    REBOOT_REASON_LABEL5              = 0xd0,    /*大于等于REBOOT_REASON_LABEL4，且小于REBOOT_REASON_LABEL5的复位原因是一个域，
                                                     表示FASTBOOT、XLOADER FASTBOOT阶段发生的异常*/
    BFM_S_NATIVE_BOOT_FAIL       = REBOOT_REASON_LABEL5,
    BFM_S_BOOT_TIMEOUT,
    BFM_S_FW_BOOT_FAIL,
    BFM_S_NATIVE_DATA_FAIL,
    IOM3_S_FDUL_EXCEPTION             = 0xd4,   // FD_UL excepition
    REBOOT_REASON_LABEL6              = 0xe0,    /*大于等于REBOOT_REASON_LABEL5，且小于REBOOT_REASON_LABEL6的复位原因是一个域，
                                                     表示检测到的app 阶段发生的不开机异常*/

    ALOADER_PANIC                     = REBOOT_REASON_LABEL6,  /* 大于等于REBOOT_REASON_LABEL6，且小于REBOOT_REASON_LABEL7的
                                                                  复位原因是一个域，虚拟化场景中aloader发生异常 */
    ALOADER_VWDT,
    REBOOT_REASON_LABEL7              = 0xf0,
    HM_PANIC                          = REBOOT_REASON_LABEL7, /* 大于等于REBOOT_REASON_LABEL7的复位原因是一个域，虚拟化场景中鸿蒙侧发生异常 */
    HM_POWERKEY_REBOOT
} EXCH_SOURCE;

enum MODID_LIST {
    DFX_BB_MOD_MODEM_DRV_START             = 0x00000000,
    DFX_BB_MOD_MODEM_DRV_END               = 0x0fffffff,
    DFX_BB_MOD_MODEM_OSA_START             = 0x10000000,
    DFX_BB_MOD_MODEM_OSA_END               = 0x1fffffff,
    DFX_BB_MOD_MODEM_OM_START              = 0x20000000,
    DFX_BB_MOD_MODEM_OM_END                = 0x2fffffff,
    DFX_BB_MOD_MODEM_GU_L2_START           = 0x30000000,
    DFX_BB_MOD_MODEM_GU_L2_END             = 0x3fffffff,
    DFX_BB_MOD_MODEM_GU_WAS_START          = 0x40000000,
    DFX_BB_MOD_MODEM_GU_WAS_END            = 0x4fffffff,
    DFX_BB_MOD_MODEM_GU_GAS_START          = 0x50000000,
    DFX_BB_MOD_MODEM_GU_GAS_END            = 0x5fffffff,
    DFX_BB_MOD_MODEM_GU_NAS_START          = 0x60000000,
    DFX_BB_MOD_MODEM_GU_NAS_END            = 0x6fffffff,
    DFX_BB_MOD_MODEM_GU_DSP_START          = 0x70000000,
    DFX_BB_MOD_MODEM_GU_DSP_END            = 0x7fffffff,
    DFX_BB_MOD_AP_START                    = 0x80000000,
    DFX_BB_MOD_AP_END                      = 0x81fff0ff,
    DFX_BB_MOD_BFM_START                  = 0x81fff100,
    DFX_BB_MOD_BFM_END                    = 0x81fff1ff,
    DFX_BB_MOD_FASTBOOT_START              = 0x81fff200,
    DFX_BB_MOD_FASTBOOT_END                = 0x81fffcff,
    DFX_BB_MOD_ISP_START                   = 0x81fffd00,
    DFX_BB_MOD_ISP_END                     = 0x81fffeff,
    DFX_BB_MOD_EMMC_START                  = 0x81ffff00,
    DFX_BB_MOD_EMMC_END                    = 0x81ffffff,
    DFX_BB_MOD_CP_START                    = 0x82000000,
    DFX_BB_MOD_CP_END                      = 0x82ffffff,
    DFX_BB_MOD_TEE_START                   = 0x83000000,
    DFX_BB_MOD_TEE_END                     = 0x83ffffff,
    DFX_BB_MOD_HIFI_START                  = 0x84000000,
    DFX_BB_MOD_HIFI_END                    = 0x84ffffff,
    DFX_BB_MOD_LPM_START                   = 0x85000000,
    DFX_BB_MOD_LPM_END                     = 0x85ffffff,
    DFX_BB_MOD_IOM_START                   = 0x86000000,
    DFX_BB_MOD_IOM_END                     = 0x86ffffff,
    DFX_BB_MOD_GENERAL_SEE_START           = 0x87000000,
    DFX_BB_MOD_GENERAL_SEE_END             = 0x87ffffff,
    DFX_BB_MOD_RESERVED_START              = 0x88000000,
    DFX_BB_MOD_RESERVED_END                = 0x9fffffff,
    DFX_BB_MOD_MODEM_LPS_START             = 0xa0000000,
    DFX_BB_MOD_MODEM_LPS_END               = 0xafffffff,
    DFX_BB_MOD_MODEM_LMSP_START            = 0xb0000000,
    DFX_BB_MOD_MODEM_LMSP_END              = 0xbfffffff,
    DFX_BB_MOD_NPU_START                   = 0xc0000000,
    DFX_BB_MOD_NPU_END                     = 0xc0000fff,
    DFX_BB_MOD_CONN_START                  = 0xc0001000,
    DFX_BB_MOD_CONN_END                    = 0xc0001fff,
    DFX_BB_MOD_IVP_START                   = 0xc0002000,
    DFX_BB_MOD_IVP_END                     = 0xc0002fff,
    DFX_BB_MOD_DSS_START                   = 0xc0003000,
    DFX_BB_MOD_DSS_END                     = 0xc0003fff,
    DFX_BB_MOD_ALOADER_START               = 0xc0004000,
    DFX_BB_MOD_ALOADER_END                 = 0xc00040ff,
    DFX_BB_MOD_VMSOLO_START                = 0xc0004100,
    DFX_BB_MOD_VMSOLO_END                  = 0xc00041ff,
    DFX_BB_MOD_SILS_START                  = 0xc0005000U,
    DFX_BB_MOD_SILS_END                    = 0xc0005fffU,
    DFX_BB_MOD_GPU_START                   = 0xc0006000U,
    DFX_BB_MOD_GPU_END                     = 0xc0006fffU,
    DFX_BB_MOD_FFTS_START                   = 0xc0007000U,
    DFX_BB_MOD_FFTS_END                     = 0xc0007fffU,
    DFX_BB_MOD_DPA_START                   = 0xc0008000U,
    DFX_BB_MOD_DPA_END                     = 0xc0008fffU,
    DFX_BB_MOD_RANDOM_ALLOCATED_START      = 0xd0000000,
    DFX_BB_MOD_RANDOM_ALLOCATED_END        = 0xf0ffffff
};

enum CORE_LIST {
	RDR_AP      = 0x1,
	RDR_CP      = 0x2,
	RDR_TEEOS   = 0x4,
	RDR_HIFI    = 0x8,
	RDR_LPM3    = 0x10,
	RDR_IOM3    = 0x20,
	RDR_ISP     = 0x40,
	RDR_IVP     = 0x80,
	RDR_EMMC    = 0x100,
	RDR_MODEMAP = 0x200,
	RDR_CLK     = 0x400,
	RDR_REGULATOR = 0x800,
	RDR_BFM       = 0x1000,
	RDR_GENERAL_SEE     = 0x2000,
	RDR_NPU       = 0x4000,
	RDR_CONN      = 0x8000,
	RDR_EXCEPTION_TRACE = 0x10000,
	RDR_DSS = 0x20000,
	RDR_SILS = 0x40000,
	RDR_RDA_AP = 0x80000,
	RDR_DDR = 0x100000,
	RDR_GPU = 0x200000,
	RDR_FFTS = 0x400000,
	RDR_DFA = 0x800000,
	RDR_VDEC = 0x1000000,
	RDR_DDR_SEC = 0x2000000,
	RDR_DPA = 0x4000000,
	RDR_CORE_MAX  = 27
};

enum REGS_TYPE {
	REGS_SCTRL = 0x0,
	REGS_PCTRL = 0x1,
	REGS_PMCTRL  = 0x2,
	REGS_PERICRG = 0x3,
	RDR_REGS_MAX
};
#define REG_SCTRL_SZIE 0x1000
#define REG_PCTRL_SZIE 0x2000
#define REG_PMCTRL_SZIE 0x1000
#define REG_PERICRG_SZIE 0x1000

enum {
	BL31_TRACE_EXCEPTION,
	BL31_TRACE_IRQ_SMC,
	BL31_TRACE_DMSS,
	BL31_TRACE_ENUM,
};

/* assure aligned by 8 */
typedef struct bl31_trace_irq_smc_head_s {
	unsigned long long cpu_num;
	unsigned long long offset[0];
} bl31_trace_irq_smc_head_t;

/**
 * exception trace core
 * EXCEPTION_TRACE_AP: exception come from whole system which be recorded in preserved memory
 * EXCEPTION_TRACE_BL31: exception come from bl31 which be recorded in preserved memory
 */
enum {
	EXCEPTION_TRACE_AP,
	EXCEPTION_TRACE_BL31,
	EXCEPTION_TRACE_ENUM,
};

/**
 * exception trace for each core
 * @e_32k_time: record the 32k time when exception happen
 * @e_reset_core_mask: notify which core need to be reset, when include
 *  the ap core to be reset that will reboot the whole system
 * @e_from_core: exception triggered from which core
 * @e_exce_type: exception type
 * @e_exce_subtype: exception subtype
 */
typedef struct rdr_exception_trace_s {
	unsigned long long e_32k_time;
	unsigned long long e_reset_core_mask;
	unsigned long long e_from_core;
	unsigned int e_exce_type;
	unsigned int e_exce_subtype;
} rdr_exception_trace_t;

enum {
	BL31_MNTN_TST_PANIC,
	BL31_MNTN_TST_ASSERT,

};

/**
 * the way into bl31 include smc and interrupt(irq or fiq), please infer
 * runtime_exception.S in bl31.
 * BL31_TRACE_TYPE_SMC: the way into bl31 in smc call
 * BL31_TRACE_TYPE_INTERRUPT: the way into bl31 in interrupt
 */
enum {
	BL31_TRACE_TYPE_SMC,
	BL31_TRACE_TYPE_INTERRUPT,
	BL31_TRACE_TYPE_ENUM,
};

/**
 * the direction into or out of bl31
 */
enum {
	BL31_TRACE_IN,
	BL31_TRACE_OUT
};

/**
 * exception trace for each core
 * @32k_time: record the 32k time
 * @type: smc or interrupt(irq&fiq)
 * @dir: the direction into or out of bl31, 0 signify the 'in' direction otherwise out,
 *       please refer the above enum define.
 * @ns: the trace source coming from secure or nonsecure world, 1 means nonsecure otherwise secure
 * @func_id: fucntion identifier passed on smc call
 * @id: interrupt identifier for interrupt type
 */
typedef struct bl31_trace_s {
	unsigned long long bl31_32k_time;
	unsigned char type;
	unsigned char dir;
	unsigned char ns;

	union {
		struct {
			unsigned long long func_id;
		} smc;
		struct {
			unsigned int id;
		} interrupt;
	};

} bl31_trace_t;


#define DFX_AP_KEYS_MAX  71
/* attention: must be aligned by 8, or it will data abort for unaligned data access */
typedef struct dfx_ap_ringbuffer_s {
	unsigned int max_num;
	unsigned int field_count;
	unsigned int rear;//write pointer, where the buffer is available to write in
	unsigned int r_idx;// read pointer, where the buffer is to be read
	unsigned int count;/* how many items in the buffer are not read  */
	unsigned int is_full;
	char keys[DFX_AP_KEYS_MAX + 1];	/*For parsing with PC tools */
	unsigned char data[1];
} dfx_ap_ringbuffer_t;

#endif /* __MNTN_COMMON_INTERFACE__ */
