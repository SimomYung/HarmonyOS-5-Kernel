/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 *  Description: the partition table.
 */

#ifndef _SAGITTARIUS_PARTITION_OHOS_H_
#define _SAGITTARIUS_PARTITION_OHOS_H_

#include "partition_macro.h"
#include "partition_macro_plus.h"
#include "partition_def.h"

static const struct partition partition_table_emmc[] = {
    {"0", 0, 0, 0},
};

static const struct partition partition_table_ufs[] = {
    {PART_XLOADER,                0,              2 * 1024,    UFS_PART_0},
    {PART_RESERVED0,              0,              2 * 1024,    UFS_PART_1},
    {PART_PTABLE,                 0,                   512,    UFS_PART_2}, /* ptable           512K    p0 */
    {PART_FRP,                         512,          512, UFS_PART_2}, /* frp                512KB p1 */
    {PART_PERSIST,                    1024,     6 * 1024, UFS_PART_2}, /* persist              6MB p2 */
    {PART_RESERVED1,              7 * 1024,         1024, UFS_PART_2}, /* reserved1         1024KB p3 */
    {PART_PTABLE_LU3,             0,                   512,    UFS_PART_3}, /* ptable_lu3       512K    p0 */
    {PART_VRL,                         512,          512, UFS_PART_3}, /* vrl                512KB p1 */
    {PART_VRL_BACKUP,                 1024,          512, UFS_PART_3}, /* vrl_backup         512KB p2 */
    {PART_MODEM_SECURE,               1536,         8704, UFS_PART_3}, /* modem_secure      8704KB p3 */
    {PART_NVME,                  10 * 1024,     5 * 1024, UFS_PART_3}, /* nvme                 5MB p4 */
    {PART_CTF,                   15 * 1024,     1 * 1024, UFS_PART_3}, /* ctf               1024KB p5 */
    {PART_OEMINFO,               16 * 1024,    96 * 1024, UFS_PART_3}, /* oeminfo             96MB p6 */
    {PART_SECURE_STORAGE,       112 * 1024,    32 * 1024, UFS_PART_3}, /* secure_storage      32MB p7 */
    {PART_MODEMNVM_FACTORY,     144 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_factory    16MB p8 */
    {PART_MODEMNVM_BACKUP,      160 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_backup     16MB p9 */
    {PART_MODEMNVM_IMG,         176 * 1024,    46 * 1024, UFS_PART_3}, /* modemnvm_img        46MB p10 */
    {PART_VERITYKEY,            222 * 1024,     1 * 1024, UFS_PART_3}, /* veritykey         1024KB p11 */
    {PART_DDR_PARA,             223 * 1024,     1 * 1024, UFS_PART_3}, /* ddr_para          1024KB p12 */
    {PART_LOWPOWER_PARA,        224 * 1024,     1 * 1024, UFS_PART_3}, /* lowpower_para     1024KB p13 */
    {PART_BATT_TP_PARA,         225 * 1024,     1 * 1024, UFS_PART_3}, /* batt_tp_para      1024KB p14 */
    {PART_BL2,                  226 * 1024,     4 * 1024, UFS_PART_3}, /* bl2                  4MB p15 */
    {PART_RESERVED2,            230 * 1024,    25 * 1024, UFS_PART_3}, /* reserved2           25MB p16 */
    {PART_LOG,                  255 * 1024,    80 * 1024, UFS_PART_3}, /* log                 80MB p17 */
    {PART_BOOTFAIL_INFO,        335 * 1024,     2 * 1024, UFS_PART_3}, /* bootfail_info        2MB p18 */
    {PART_MISC,                 337 * 1024,     2 * 1024, UFS_PART_3}, /* misc                 2MB p19 */
    {PART_DFX,                  339 * 1024,    16 * 1024, UFS_PART_3}, /* dfx                 16MB p20 */
    {PART_RRECORD,              355 * 1024,    16 * 1024, UFS_PART_3}, /* rrecord             16MB p21 */
    {PART_CACHE,                371 * 1024,   104 * 1024, UFS_PART_3}, /* cache              104MB p22 */
    {PART_METADATA,             475 * 1024,    16 * 1024, UFS_PART_3}, /* metadata            16MB p23 */
    {PART_RESERVED3,            491 * 1024,     4 * 1024, UFS_PART_3}, /* reserved3            4MB p24 */
    {PART_TOC,                  495 * 1024,     1 * 1024, UFS_PART_3}, /* toc               1024KB p25 */   
    {PART_FW_LPM3,              496 * 1024,     1 * 1024, UFS_PART_3}, /* fw_lpm3           1024KB p26 */
    {PART_NPU,                  497 * 1024,     8 * 1024, UFS_PART_3}, /* npu                  8MB p27 */
    {PART_IVP,                  505 * 1024,     2 * 1024, UFS_PART_3}, /* ivp                  2MB p28 */
    {PART_HDCP,                 507 * 1024,     1 * 1024, UFS_PART_3}, /* hdcp              1024KB p29 */
    {PART_HHEE,                 508 * 1024,     4 * 1024, UFS_PART_3}, /* hhee                 4MB p30 */
    {PART_FASTBOOT,             512 * 1024,    12 * 1024, UFS_PART_3}, /* fastboot            12MB p31 */
    {PART_VECTOR,               524 * 1024,     4 * 1024, UFS_PART_3}, /* vector               4MB p32 */
    {PART_ISP_BOOT,             528 * 1024,     2 * 1024, UFS_PART_3}, /* isp_boot             2MB p33 */
    {PART_ISP_FIRMWARE,         530 * 1024,    14 * 1024, UFS_PART_3}, /* isp_firmware        14MB p34 */
    {PART_FW_HIFI,              544 * 1024,    12 * 1024, UFS_PART_3}, /* fw_hifi             12MB p35 */
    {PART_TEEOS,                556 * 1024,     8 * 1024, UFS_PART_3}, /* teeos                8MB p36 */
    {PART_SENSORHUB,            564 * 1024,    13 * 1024, UFS_PART_3}, /* sensorhub           13MB p37 */
    {PART_SENSORHUB_LOG_DIC,    577 * 1024,     3 * 1024, UFS_PART_3}, /* sensorhub log        3MB p37 */
    {PART_HKRR,                 580 * 1024,    16 * 1024, UFS_PART_3}, /* hkrr                16MB p38 */
    {PART_FW_UFSDEV,            596 * 1024,     4 * 1024, UFS_PART_3}, /* fw_ufsdev            4MB p39 */
    {PART_RESERVED4,            600 * 1024,    24 * 1024, UFS_PART_3}, /* reserved4           24MB p40 */
#ifdef CONFIG_ASAN_SUPPORT_ENABLE
    {PART_UPDATER_RAMDISK,      624 * 1024,    64 * 1024, UFS_PART_3}, /* updater_ramdisk     64MB p41 */
    {PART_UPDATER_VENDOR,       688 * 1024,    32 * 1024, UFS_PART_3}, /* updater_vendor      32MB p42 */
    {PART_BOOT,                 720 * 1024,    48 * 1024, UFS_PART_3}, /* boot                48MB p43 */
    {PART_UPDATER_BOOT,         768 * 1024,    55 * 1024, UFS_PART_3}, /* updater_boot        55MB p44 */
    {PART_UPDATER_BOOT_BAK,     823 * 1024,    55 * 1024, UFS_PART_3}, /* updater_boot_bak    55MB p45 */
    {PART_RESERVED,             878 * 1024,     7 * 1024, UFS_PART_3}, /* reserved             7MB p46 */
    {PART_UPDATER_RAMDISK_BAK,   885 * 1024,    64 * 1024, UFS_PART_3}, /* updater_ramdisk_bak  64MB p47 */
    {PART_UPDATER_VENDOR_BAK,   949 * 1024,    32 * 1024, UFS_PART_3}, /* updater_vendor_bak  32MB p48 */
    {PART_ENG_SYSTEM,           981 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p49 */
    {PART_ENG_CHIPSET,          993 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p50 */
    {PART_FW_DTB,              1013 * 1024,     8 * 1024, UFS_PART_3}, /* fw_dtb               8MB p51 */
    {PART_DTBO,                1021 * 1024,    14 * 1024, UFS_PART_3}, /* dtbo                14MB p52 */
    {PART_TRUSTFIRMWARE,       1035 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p53 */
    {PART_MODEM_FW,            1037 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p54 */
    {PART_MODEM_VENDOR,        1161 * 1024,    30 * 1024, UFS_PART_3}, /* modem_vendor        30MB p55 */
    {PART_MODEM_PATCH_NV,      1191 * 1024,     4 * 1024, UFS_PART_3}, /* modem_patch_nv       4MB p56 */
    {PART_MODEM_DRIVER,        1195 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p57 */
    {PART_MODEMNVM_UPDATE,     1215 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p58 */
    {PART_MODEMNVM_CUST,       1231 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p59 */
    {PART_RAMDISK,             1247 * 1024,     3 * 1024, UFS_PART_3}, /* ramdisk              3MB p60 */
    {PART_RVT_SYSTEM,          1250 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system        1024KB p61 */
    {PART_RVT_VENDOR,          1251 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor        1024KB p62 */
    {PART_RVT_CHIP_PROD,       1252 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod     1024KB p63 */
    {PART_RVT_CUST,            1253 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust          1024KB p64 */
    {PART_RVT_SYS_PROD,        1254 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod      1024KB p65 */
    {PART_UPDATER_RVT,         1255 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p66 */
    {PART_UPDATER_BAK_RVT,     1257 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p67 */
    {PART_RVT,                 1259 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p68 */
    {PART_KPATCH,              1263 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p69 */
    {PART_PATCH,               1267 * 1024,    64 * 1024, UFS_PART_3}, /* patch               64MB p70 */
    {PART_CUST,                1331 * 1024,    56 * 1024, UFS_PART_3}, /* cust                56MB p71 */
    {PART_CHIP_PROD,           1387 * 1024,   942 * 1024, UFS_PART_3}, /* chip_prod          942MB p72 */
    {PART_VENDOR,              2329 * 1024,  3072 * 1024, UFS_PART_3}, /* vendor            3072MB p73 */
    {PART_SYS_PROD,            5401 * 1024,  1746 * 1024, UFS_PART_3}, /* sys_prod          1746MB p74 */
    {PART_SYSTEM,              7147 * 1024,  7568 * 1024, UFS_PART_3}, /* system            7568MB p75 */
    {PART_HYPERHOLD,          14715 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p76 */
    {PART_RESERVED5,          14719 * 1024,   127 * 1024, UFS_PART_3}, /* reserved5          127MB p77 */
    {PART_VERSION,            14846 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p78 */
    {PART_PRELOAD,            15422 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p79 */
    {PART_USERDATA,           16822 * 1024,        15176 * 1024, UFS_PART_3}, /* userdata       4G p80 */
#else
#ifdef CONFIG_SANITIZER_ENABLE
    {PART_UPDATER_RAMDISK,      624 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk     32MB p41 */
    {PART_UPDATER_VENDOR,       656 * 1024,    24 * 1024, UFS_PART_3}, /* updater_vendor      24MB p42 */
    {PART_BOOT,                 680 * 1024,    48 * 1024, UFS_PART_3}, /* boot                48MB p43 */
    {PART_UPDATER_BOOT,         728 * 1024,    56 * 1024, UFS_PART_3}, /* updater_boot        54MB p44 */
    {PART_UPDATER_BOOT_BAK,     784 * 1024,    56 * 1024, UFS_PART_3}, /* updater_boot_bak    54MB p45 */
    {PART_UPDATER_RAMDISK_BAK,   840 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk_bak  32MB p46 */
    {PART_UPDATER_VENDOR_BAK,   872 * 1024,    24 * 1024, UFS_PART_3}, /* updater_vendor_bak  24MB p47 */
    {PART_RESERVED,             896 * 1024,    31 * 1024, UFS_PART_3}, /* reserved            35MB p48 */
#else
    {PART_UPDATER_RAMDISK,      624 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk     32MB p41 */
    {PART_UPDATER_VENDOR,       656 * 1024,    24 * 1024, UFS_PART_3}, /* updater_vendor      24MB p42 */
    {PART_BOOT,                 680 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p43 */
    {PART_UPDATER_BOOT,         726 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot        54MB p44 */
    {PART_UPDATER_BOOT_BAK,     780 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot_bak    54MB p45 */
    {PART_UPDATER_RAMDISK_BAK,   834 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk_bak  32MB p46 */
    {PART_UPDATER_VENDOR_BAK,   866 * 1024,    24 * 1024, UFS_PART_3}, /* updater_vendor_bak  24MB p47 */
    {PART_RESERVED,             890 * 1024,    37 * 1024, UFS_PART_3}, /* reserved            37MB p48 */
#endif   
    {PART_ENG_SYSTEM,           927 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p49 */
    {PART_ENG_CHIPSET,          939 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p50 */
    {PART_FW_DTB,               959 * 1024,     8 * 1024, UFS_PART_3}, /* fw_dtb               8MB p51 */
    {PART_DTBO,                 967 * 1024,    14 * 1024, UFS_PART_3}, /* dtbo                14MB p52 */
    {PART_TRUSTFIRMWARE,        981 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p53 */
    {PART_MODEM_FW,             983 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p54 */
    {PART_MODEM_VENDOR,        1107 * 1024,    10 * 1024, UFS_PART_3}, /* modem_vendor        10MB p55 */
    {PART_MODEM_PATCH_NV,      1117 * 1024,    16 * 1024, UFS_PART_3}, /* modem_patch_nv      16MB p56 */
    {PART_MODEM_DRIVER,        1133 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p57 */
    {PART_MODEMNVM_UPDATE,     1153 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p58 */
    {PART_MODEMNVM_CUST,       1169 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p59 */
    {PART_RAMDISK,             1185 * 1024,     3 * 1024, UFS_PART_3}, /* ramdisk              3MB p60 */
    {PART_RVT_SYSTEM,          1188 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system        1024KB p61 */
    {PART_RVT_VENDOR,          1189 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor        1024KB p62 */
    {PART_RVT_CHIP_PROD,       1190 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod     1024KB p63 */
    {PART_RVT_CUST,            1191 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust          1024KB p64 */
    {PART_RVT_SYS_PROD,        1192 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod      1024KB p65 */
    {PART_UPDATER_RVT,         1193 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p66 */
    {PART_UPDATER_BAK_RVT,     1195 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p67 */
    {PART_RVT,                 1197 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p68 */
    {PART_KPATCH,              1201 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p69 */
    {PART_PATCH,               1205 * 1024,    64 * 1024, UFS_PART_3}, /* patch               64MB p70 */
    {PART_CUST,                1269 * 1024,    56 * 1024, UFS_PART_3}, /* cust                56MB p71 */
    {PART_CHIP_PROD,           1325 * 1024,   656 * 1024, UFS_PART_3}, /* chip_prod          656MB p72 */
    {PART_VENDOR,              1981 * 1024,   864 * 1024, UFS_PART_3}, /* vendor             864MB p73 */
    {PART_SYS_PROD,            2845 * 1024,  2064 * 1024, UFS_PART_3}, /* sys_prod          2064MB p74 */
    {PART_SYSTEM,              4909 * 1024,  4948 * 1024, UFS_PART_3}, /* system            4948MB p75 */
    {PART_RESERVED5,           9857 * 1024,     5 * 1024, UFS_PART_3}, /* reserved5            5MB p76 */
    {PART_VERSION,             9862 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p77 */
    {PART_PRELOAD,            10438 * 1024,  1504 * 1024, UFS_PART_3}, /* preload           1504MB p78 */
#if defined(CONFIG_FACTORY_MODE)
    {PART_HIBENCH_IMG,        11942 * 1024,   128 * 1024, UFS_PART_3}, /* hibench_img        128MB p79 */
    {PART_HIBENCH_LOG,        12070 * 1024,    32 * 1024, UFS_PART_3}, /* hibench_log         32MB p80 */
    {PART_HIBENCH_DATA,       12102 * 1024,   512 * 1024, UFS_PART_3}, /* hibench_data       512MB p81 */
    {PART_FLASH_AGEING,       12614 * 1024,   512 * 1024, UFS_PART_3}, /* flash_ageing       512MB p82 */
    {PART_HIBENCH_LPM3,       13126 * 1024,     2 * 1024, UFS_PART_3}, /* hibench_lpm3         2MB p83 */
    {PART_RESERVED6,          13128 * 1024,  1406 * 1024, UFS_PART_3}, /* reserved6         1406MB p84 */
    {PART_USERDATA,           14534 * 1024,(16UL) * 1024 * 1024, UFS_PART_3}, /* userdata       6G p85 */
#else 
    {PART_USERDATA,           11942 * 1024,(16UL) * 1024 * 1024, UFS_PART_3}, /* userdata       6G p79 */
#endif
#endif
    {PART_PTABLE_LU4,                   0,         512,    UFS_PART_4}, /* ptable_lu4        512K   p0 */
    {PART_RESERVED12,                  512,         1536, UFS_PART_4}, /* reserved12        1536KB p1 */
    {PART_USERDATA2,              2 * 1024, (4UL) * 1024 * 1024, UFS_PART_4}, /* userdata2      4G p2 */ 
    {"0", 0, 0, 0},
};

#endif
