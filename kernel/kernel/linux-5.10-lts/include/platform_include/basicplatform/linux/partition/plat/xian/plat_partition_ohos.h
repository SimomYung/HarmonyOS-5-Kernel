#ifndef _PISCES_PLAT_PARTITION_OHOS_H_
#define _PISCES_PLAT_PARTITION_OHOS_H_

#include "partition_macro.h"
#include "partition_macro_plus.h"
#include "partition_def.h"

static const struct partition partition_table_emmc[] = {
    {"0", 0, 0, 0},
};

static const struct partition partition_table_ufs[] = {
    {PART_XLOADER,                0,              2 * 1024,    UFS_PART_0},
    {PART_RESERVED0,              0,            2 * 1024, UFS_PART_1},
    {PART_PTABLE,                 0,                 512, UFS_PART_2}, /* ptable              512K p0 */
    {PART_FRP,                         512,          512, UFS_PART_2}, /* frp                512KB p1 */
    {PART_PERSIST,                    1024,     6 * 1024, UFS_PART_2}, /* persist              6MB p2 */
    {PART_RESERVED1,              7 * 1024,         1024, UFS_PART_2}, /* reserved1         1024KB p3 */
    {PART_PTABLE_LU3,                    0,          512, UFS_PART_3}, /* ptable_lu3          512K p0 */
    {PART_VRL,                         512,          512, UFS_PART_3}, /* vrl                512KB p1 */
    {PART_VRL_BACKUP,                 1024,          512, UFS_PART_3}, /* vrl_backup         512KB p2 */
    {PART_MODEM_SECURE,               1536,         8704, UFS_PART_3}, /* modem_secure      8704KB p3 */
    {PART_NVME,                  10 * 1024,     4 * 1024, UFS_PART_3}, /* nvme                 4MB p4 */
    {PART_CONN_CALIDATA,         14 * 1024,         1024, UFS_PART_3}, /* conn_calidata     1024KB p5 */
    {PART_CTF,                   15 * 1024,     1 * 1024, UFS_PART_3}, /* ctf               1024KB p6 */
    {PART_OEMINFO,               16 * 1024,    96 * 1024, UFS_PART_3}, /* oeminfo             96MB p7 */
    {PART_SECURE_STORAGE,       112 * 1024,    32 * 1024, UFS_PART_3}, /* secure_storage      32MB p8 */
    {PART_MODEMNVM_FACTORY,     144 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_factory    16MB p9 */
    {PART_MODEMNVM_BACKUP,      160 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_backup    16MB p10 */
    {PART_MODEMNVM_IMG,         176 * 1024,    46 * 1024, UFS_PART_3}, /* modemnvm_img       46MB p11 */
    {PART_HISEE_ENCOS,          222 * 1024,     4 * 1024, UFS_PART_3}, /* hisee_encos         4MB p12 */
    {PART_VERITYKEY,            226 * 1024,     1 * 1024, UFS_PART_3}, /* veritykey        1024KB p13 */
    {PART_DDR_PARA,             227 * 1024,     1 * 1024, UFS_PART_3}, /* ddr_para         1024KB p14 */
    {PART_LOWPOWER_PARA,        228 * 1024,     1 * 1024, UFS_PART_3}, /* lowpower_para    1024KB p15 */
    {PART_BATT_TP_PARA,         229 * 1024,     1 * 1024, UFS_PART_3}, /* batt_tp_para     1024KB p16 */
    {PART_PANEL_CALIBRATION,    230 * 1024,   100 * 1024, UFS_PART_3}, /* panel_calibration 100MB p17 */
    {PART_RESERVED2,            330 * 1024,    21 * 1024, UFS_PART_3}, /* reserved2          21MB p18 */
    {PART_LOG,                  351 * 1024,    80 * 1024, UFS_PART_3}, /* log                80MB p19 */
    {PART_BOOTFAIL_INFO,        431 * 1024,     2 * 1024, UFS_PART_3}, /* bootfail_info       2MB p20 */
    {PART_MISC,                 433 * 1024,     2 * 1024, UFS_PART_3}, /* misc                2MB p21 */
    {PART_DFX,                  435 * 1024,    16 * 1024, UFS_PART_3}, /* dfx                16MB p22 */
    {PART_RRECORD,              451 * 1024,    16 * 1024, UFS_PART_3}, /* rrecord            16MB p23 */
    {PART_CACHE,                467 * 1024,   104 * 1024, UFS_PART_3}, /* cache             104MB p24 */
    {PART_METADATA,             571 * 1024,    16 * 1024, UFS_PART_3}, /* metadata           16MB p25 */
    {PART_SEPLAT,               587 * 1024,     4 * 1024, UFS_PART_3}, /* seplat              4MB p26 */
    {PART_TOC,                  591 * 1024,     1 * 1024, UFS_PART_3}, /* toc              1024KB p27 */
    {PART_BL2,                  592 * 1024,     4 * 1024, UFS_PART_3}, /* bl2                 4MB p28 */
    {PART_FW_LPM3,              596 * 1024,          512, UFS_PART_3}, /* fw_lpm3           512KB p29 */
    {PART_FW_CPU_LPCTRL,       (596 * 1024 + 512),   256, UFS_PART_3}, /* fw_cpu_lpctrl      256K p30 */
    {PART_FW_GPU_LPCTRL,       (596 * 1024 + 768),   128, UFS_PART_3}, /* fw_gpu_lpctrl      128K p31 */
    {PART_FW_DDR_LPCTRL,       (596 * 1024 + 896),   256, UFS_PART_3}, /* fw_ddr_lpctrl      256K p32 */
    {PART_DDR_XPU_BOOT0,       (597 * 1024 + 128),   256, UFS_PART_3}, /* ddr_xpu_boot0     256KB p33 */
    {PART_DDR_XPU_BOOT1,       (597 * 1024 + 384),  256, UFS_PART_3}, /* ddr_xpu_boot1      256KB p34 */
    {PART_DDR_XPU_BOOT2,       (597 * 1024 + 640),  256, UFS_PART_3}, /* ddr_xpu_boot2      256KB p35 */
    {PART_UCE,                 (597 * 1024 + 896),  160, UFS_PART_3}, /* uce                160KB p36 */
    {PART_RESERVED3,           (598 * 1024 + 32),  5 * 1024 + 2016, UFS_PART_3}, /* reserved3   7136KB p37 */
    {PART_NPU,                  605 * 1024,     8 * 1024, UFS_PART_3}, /* npu                  8MB p38 */
    {PART_IVP,                  613 * 1024,     2 * 1024, UFS_PART_3}, /* ivp                  2MB p39 */
    {PART_DACC,                 615 * 1024,          512, UFS_PART_3}, /* dacc               512KB p40 */
    {PART_VENC,                (615 * 1024 + 512),   512, UFS_PART_3}, /* venc               512KB p41 */
    {PART_HISEE_IMG,            616 * 1024,     4 * 1024, UFS_PART_3}, /* hisee_img            4MB p42 */
    {PART_HHEE,                 620 * 1024,     4 * 1024, UFS_PART_3}, /* hhee                 4MB p43 */
    {PART_HISEE_FS,             624 * 1024,     8 * 1024, UFS_PART_3}, /* hisee_fs             8MB p44 */
    {PART_FASTBOOT,             632 * 1024,    12 * 1024, UFS_PART_3}, /* fastboot            12MB p45 */
    {PART_VECTOR,               644 * 1024,     4 * 1024, UFS_PART_3}, /* vector               4MB p46 */
    {PART_ISP_BOOT,             648 * 1024,     2 * 1024, UFS_PART_3}, /* isp_boot             2MB p47 */
    {PART_ISP_FIRMWARE,         650 * 1024,    14 * 1024, UFS_PART_3}, /* isp_firmware        14MB p48 */
    {PART_FW_HIFI,              664 * 1024,    12 * 1024, UFS_PART_3}, /* fw_hifi             12MB p49 */
    {PART_TEEOS,                676 * 1024,     8 * 1024, UFS_PART_3}, /* teeos                8MB p50 */
    {PART_SENSORHUB,            684 * 1024,    13 * 1024, UFS_PART_3}, /* sensorhub           13MB p51 */
    {PART_SENSORHUB_LOG_DIC,    697 * 1024,     3 * 1024, UFS_PART_3}, /* sensorhub_log_dic    3MB p52 */
    {PART_HKRR,                 700 * 1024,    16 * 1024, UFS_PART_3}, /* hkrr                16MB p53 */
    {PART_FW_UFSDEV,            716 * 1024,     4 * 1024, UFS_PART_3}, /* fw_ufsdev            4MB p54 */
    {PART_FW_DFA,               720 * 1024,     1 * 1024, UFS_PART_3}, /* fw_dfa            1024KB p55 */
    {PART_RESERVED4,            721 * 1024,    15 * 1024, UFS_PART_3}, /* reserved4           15MB p56 */
    {PART_UPDATER_RAMDISK,      736 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk     32MB p57 */
    {PART_UPDATER_VENDOR,       768 * 1024,    28 * 1024, UFS_PART_3}, /* updater_vendor      28MB p58 */
#ifdef CONFIG_ASAN_SUPPORT_ENABLE
    {PART_BOOT,                 796 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p59 */
    {PART_UPDATER_BOOT,         842 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot        64MB p60 */
    {PART_UPDATER_BOOT_BAK,     906 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot_bak    64MB p61 */
    {PART_MODULE_UPDATE,        970 * 1024,          512, UFS_PART_3}, /* module_update      512KB p62 */
    {PART_EXT_RVT,      (970 * 1024 + 512),     2 * 1024, UFS_PART_3}, /* ext_rvt              2MB p63 */
    {PART_RESERVED,     (972 * 1024 + 512),    (44 * 1024+512), UFS_PART_3}, /* reserved       44.5MB p62 */
    {PART_THEE,                1017 * 1024,     4 * 1024, UFS_PART_3}, /* thee                 4MB p65 */
    {PART_TZSP,                1021 * 1024,    12 * 1024, UFS_PART_3}, /* tzsp                12MB p66 */
    {PART_UPDATER_RAMDISK_BAK,  1033 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk_bak  32MB p67 */
    {PART_UPDATER_VENDOR_BAK,  1065 * 1024,    28 * 1024, UFS_PART_3}, /* updater_vendor_bak  28MB p68 */
    {PART_ENG_SYSTEM,          1093 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p69 */
    {PART_ENG_CHIPSET,         1105 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p70 */
    {PART_FW_DTB,              1125 * 1024,    16 * 1024, UFS_PART_3}, /* fw_dtb              16MB p71 */
    {PART_DTBO,                1141 * 1024,    16 * 1024, UFS_PART_3}, /* dtbo                16MB p72 */
    {PART_TRUSTFIRMWARE,       1157 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p73 */
    {PART_MODEM_FW,            1159 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p74 */
    {PART_MODEM_VENDOR,        1283 * 1024,    30 * 1024, UFS_PART_3}, /* modem_vendor        30MB p75 */
    {PART_MODEM_PATCH_NV,      1313 * 1024,     4 * 1024, UFS_PART_3}, /* modem_patch_nv       4MB p76 */
    {PART_MODEM_DRIVER,        1317 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p77 */
    {PART_MODEMNVM_UPDATE,     1337 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p78 */
    {PART_MODEMNVM_CUST,       1353 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p79 */
    {PART_RAMDISK,             1369 * 1024,     3 * 1024, UFS_PART_3}, /* ramdisk              3MB p80 */
    {PART_RVT_SYSTEM,          1372 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system        1024KB p81 */
    {PART_RVT_VENDOR,          1373 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor        1024KB p82 */
    {PART_RVT_CHIP_PROD,       1374 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod     1024KB p83 */
    {PART_RVT_CUST,            1375 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust          1024KB p84 */
    {PART_RVT_SYS_PROD,        1376 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod      1024KB p85 */
    {PART_UPDATER_RVT,         1377 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p86 */
    {PART_UPDATER_BAK_RVT,     1379 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p87 */
    {PART_RVT,                 1381 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p88 */
    {PART_KPATCH,              1385 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p89 */
    {PART_PATCH,               1389 * 1024,    32 * 1024, UFS_PART_3}, /* patch               32MB p90 */
    {PART_CUST,                1421 * 1024,    88 * 1024, UFS_PART_3}, /* cust                88MB p91 */
    {PART_CHIP_PROD,           1509 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p92 */
    {PART_VENDOR,              4527 * 1024,  3284 * 1024, UFS_PART_3}, /* vendor            3284MB p93 */
    {PART_SYS_PROD,            7811 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod          8812MB p94 */
    {PART_SYSTEM,             16623 * 1024,  7968 * 1024, UFS_PART_3}, /* system            7968MB p95 */
    {PART_AI_MODELS,          24591 * 1024,  3994 * 1024, UFS_PART_3}, /* ai_models         3994MB p96 */
    {PART_HYPERHOLD,          28585 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p97 */
    {PART_RESERVED5,          28589 * 1024,    77 * 1024, UFS_PART_3}, /* reserved5           77MB p98 */
    {PART_VERSION,            28666 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p99 */
    {PART_PRELOAD,            29242 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p100 */
    {PART_USERDATA,           30642 * 1024,(10UL) * 1024 * 1024, UFS_PART_3}, /* userdata       6G p101 */
#else
#ifdef CONFIG_SANITIZER_ENABLE
    {PART_BOOT,                 796 * 1024,    46 * 1024, UFS_PART_3}, /* boot                48MB p59 */
    {PART_UPDATER_BOOT,         842 * 1024,    63 * 1024, UFS_PART_3}, /* updater_boot        63MB p60 */
    {PART_UPDATER_BOOT_BAK,     905 * 1024,    63 * 1024, UFS_PART_3}, /* updater_boot_bak    63MB p61 */
    {PART_MODULE_UPDATE,        968 * 1024,          512, UFS_PART_3}, /* module_update      512KB p62 */
    {PART_EXT_RVT,      (968 * 1024 + 512),     2 * 1024, UFS_PART_3}, /* ext_rvt              2MB p63 */
    {PART_RESERVED,     (970 * 1024 + 512),    34 * 1024 +512, UFS_PART_3}, /* reserved        34.5MB p64 */
#else
    {PART_BOOT,                 796 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p59 */
    {PART_UPDATER_BOOT,         842 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot        54MB p60 */
    {PART_UPDATER_BOOT_BAK,     896 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot_bak    54MB p61 */
    {PART_MODULE_UPDATE,        950 * 1024,          512, UFS_PART_3}, /* module_update      512KB p62 */
    {PART_EXT_RVT,      (950 * 1024 + 512),     2 * 1024, UFS_PART_3}, /* ext_rvt              2MB p63 */
    {PART_RESERVED,     (952 * 1024 + 512),    52 * 1024+512, UFS_PART_3}, /* reserved         52.5MB p64 */
#endif
    {PART_THEE,                1005 * 1024,     4 * 1024, UFS_PART_3}, /* thee                 4MB p65 */
    {PART_TZSP,                1009 * 1024,    12 * 1024, UFS_PART_3}, /* tzsp                12MB p66 */
    {PART_UPDATER_RAMDISK_BAK,  1021 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk_bak  32MB p67 */
    {PART_UPDATER_VENDOR_BAK,  1053 * 1024,    28 * 1024, UFS_PART_3}, /* updater_vendor_bak  28MB p68 */
    {PART_ENG_SYSTEM,          1081 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p69 */
    {PART_ENG_CHIPSET,         1093 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p70 */
    {PART_FW_DTB,              1113 * 1024,    16 * 1024, UFS_PART_3}, /* fw_dtb              16MB p71 */
    {PART_DTBO,                1129 * 1024,    16 * 1024, UFS_PART_3}, /* dtbo                16MB p72 */
    {PART_TRUSTFIRMWARE,       1145 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p73 */
    {PART_MODEM_FW,            1147 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p74 */
    {PART_MODEM_VENDOR,        1271 * 1024,    10 * 1024, UFS_PART_3}, /* modem_vendor        10MB p75 */
    {PART_MODEM_PATCH_NV,      1281 * 1024,     4 * 1024, UFS_PART_3}, /* modem_patch_nv       4MB p76 */
    {PART_MODEM_DRIVER,        1285 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p77 */
    {PART_MODEMNVM_UPDATE,     1305 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p78 */
    {PART_MODEMNVM_CUST,       1321 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p79 */
    {PART_RAMDISK,             1337 * 1024,     3 * 1024, UFS_PART_3}, /* ramdisk              3MB p80 */
    {PART_RVT_SYSTEM,          1340 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system        1024KB p81 */
    {PART_RVT_VENDOR,          1341 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor        1024KB p82 */
    {PART_RVT_CHIP_PROD,       1342 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod     1024KB p83 */
    {PART_RVT_CUST,            1343 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust          1024KB p84 */
    {PART_RVT_SYS_PROD,        1344 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod      1024KB p85 */
    {PART_UPDATER_RVT,         1345 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p86 */
    {PART_UPDATER_BAK_RVT,     1347 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p87 */
    {PART_RVT,                 1349 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p88 */
    {PART_KPATCH,              1353 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p89 */
    {PART_PATCH,               1357 * 1024,   128 * 1024, UFS_PART_3}, /* patch              128MB p90 */
    {PART_RESERVED5,           1485 * 1024,   127 * 1024, UFS_PART_3}, /* reserved5          127MB p91 */
#if defined(CONFIG_FACTORY_MODE)
    {PART_HIBENCH_IMG,         1612 * 1024,   128 * 1024, UFS_PART_3}, /* hibench_img        128MB p92 */
    {PART_HIBENCH_LOG,         1740 * 1024,    32 * 1024, UFS_PART_3}, /* hibench_log         32MB p93 */
    {PART_HIBENCH_DATA,        1772 * 1024,   700 * 1024, UFS_PART_3}, /* hibench_data       700MB p94 */
    {PART_FLASH_AGEING,        2472 * 1024,   512 * 1024, UFS_PART_3}, /* flash_ageing       512MB p95 */
    {PART_HIBENCH_LPM3,        2984 * 1024,     2 * 1024, UFS_PART_3}, /* hibench_lpm3         2MB p96 */
    {PART_XLOADER_HITEST,      2986 * 1024,     2 * 1024, UFS_PART_3}, /* xloader_hitest       2MB p97 */
    {PART_CUST,                2988 * 1024,   128 * 1024, UFS_PART_3}, /* cust               128MB p98 */
    {PART_CHIP_PROD,           3116 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p99 */
    {PART_VENDOR,              6134 * 1024,  1132 * 1024, UFS_PART_3}, /* vendor           1132MB p100 */
    {PART_SYS_PROD,            7266 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod         8812MB p101 */
    {PART_SYSTEM,             16078 * 1024,  4948 * 1024, UFS_PART_3}, /* system           4948MB p102 */
    {PART_HYPERHOLD,          21026 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold           4MB p103 */
    {PART_VERSION,            21030 * 1024,   576 * 1024, UFS_PART_3}, /* version           576MB p104 */
    {PART_PRELOAD,            21606 * 1024,  1400 * 1024, UFS_PART_3}, /* preload          1400MB p105 */
    {PART_RESERVED6,          23006 * 1024, 27848 * 1024, UFS_PART_3}, /* reserved6       27848MB p106 */
    {PART_USERDATA,           50854 * 1024,(16UL) * 1024 * 1024, UFS_PART_3}, /* userdata      6G p107 */
#else
    {PART_CUST,                1612 * 1024,   128 * 1024, UFS_PART_3}, /* cust               128MB p92 */
    {PART_CHIP_PROD,           1740 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p93 */
    {PART_VENDOR,              4758 * 1024,  1132 * 1024, UFS_PART_3}, /* vendor            1132MB p94 */
    {PART_SYS_PROD,            5890 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod          8812MB p95 */
    {PART_SYSTEM,             14702 * 1024,  4948 * 1024, UFS_PART_3}, /* system            4948MB p96 */
    {PART_HYPERHOLD,          19650 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p97 */
    {PART_VERSION,            19654 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p98 */
    {PART_PRELOAD,            20230 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p99 */
#if defined(CONFIG_PARTITION_ENG)
    {PART_FTTEST,             21630 * 1024,   192 * 1024, UFS_PART_3}, /* fttest             192MB p100 */
    {PART_DDRTEST,            21822 * 1024,    48 * 1024, UFS_PART_3}, /* ddrtest             48MB p101 */
    {PART_HITEST_GPU,         21870 * 1024,   512 * 1024, UFS_PART_3}, /* hitest_gpu         512MB p102 */
    {PART_HITEST_ASP_DHRYSTONE, 22382 * 1024,    20 * 1024, UFS_PART_3}, /* hitest_asp_dhrystone  20MB p103 */
    {PART_HITEST_ASP_TEST,    22402 * 1024,   300 * 1024, UFS_PART_3}, /* hitest_asp_test    300MB p104 */
    {PART_HITEST_SENSORHUB,   22702 * 1024,     6 * 1024, UFS_PART_3}, /* hitest_sensorhub     6MB p105 */
    {PART_HITEST_ISP,         22708 * 1024,    25 * 1024, UFS_PART_3}, /* hitest_isp          25MB p106 */
    {PART_HITEST_ISP_AVS,     22733 * 1024,    25 * 1024, UFS_PART_3}, /* hitest_isp_avs      25MB p107 */
    {PART_HITEST_NPU,         22758 * 1024,    80 * 1024, UFS_PART_3}, /* hitest_npu          80MB p108 */
    {PART_HITEST_DSS,         22838 * 1024,    18 * 1024, UFS_PART_3}, /* hitest_dss          18MB p109 */
    {PART_DSS_DISP_BUF,       22856 * 1024,   128 * 1024, UFS_PART_3}, /* dss_disp_buf       128MB p110 */
    {PART_HITEST_JPEGENC,     22984 * 1024,   200 * 1024, UFS_PART_3}, /* hitest_jpegenc     200MB p111 */
    {PART_HITEST_HIPP,        23184 * 1024,   100 * 1024, UFS_PART_3}, /* hitest_hipp        100MB p112 */
    {PART_HIBENCH_LPM3,       23284 * 1024,     2 * 1024, UFS_PART_3}, /* hibench_lpm3         2MB p113 */
    {PART_JPEG_DEC_TEST_SRC,  23286 * 1024,    20 * 1024, UFS_PART_3}, /* jpeg_dec_test_src   20MB p114 */
    {PART_JPEG_DEC_TEST_DEST, 23306 * 1024,    80 * 1024, UFS_PART_3}, /* jpeg_dec_test_dest  80MB p115 */
    {PART_VCODE_DATA,         23386 * 1024,   140 * 1024, UFS_PART_3}, /* vcode_data         140MB p116 */
    {PART_HITEST_FFRT,        23526 * 1024,    20 * 1024, UFS_PART_3}, /* hitest_ffrt         20MB p117 */
    {PART_VCODE_MAX_DATA,     23546 * 1024,   140 * 1024, UFS_PART_3}, /* vcode_max_data     140MB p118 */
    {PART_MODEM_TSP_TEST,     23686 * 1024,    36 * 1024, UFS_PART_3}, /* modem_tsp_test      36MB p119 */
    {PART_MODEM_VECTOR_CONFIG, 23722 * 1024,     4 * 1024, UFS_PART_3}, /* modem_vector_config   4MB p120 */
    {PART_MODEM_TSP_DT,       23726 * 1024,     1 * 1024, UFS_PART_3}, /* modem_tsp_dt      1024KB p121 */
    {PART_MODEM_L2DLE,        23727 * 1024,     4 * 1024, UFS_PART_3}, /* modem_l2dle          4MB p122 */
    {PART_MODEM_BBP_TEST,     23731 * 1024,   100 * 1024, UFS_PART_3}, /* modem_bbp_test     100MB p123 */
    {PART_MODEM_PSA_TEST,     23831 * 1024,    20 * 1024, UFS_PART_3}, /* modem_psa_test      20MB p124 */
    {PART_MODEM_PDE_TEST,     23851 * 1024,     5 * 1024, UFS_PART_3}, /* modem_pde_test       5MB p125 */
    {PART_XLOADER_HITEST,     23856 * 1024,     2 * 1024, UFS_PART_3}, /* xloader_hitest       2MB p126 */
    {PART_USERDATA,           23858 * 1024,(16UL) * 1024 * 1024, UFS_PART_3}, /* userdata       6G p127 */
#else // system in ccm is 5716,but now less 768
    {PART_RESERVED6,          21630 * 1024,  1730 * 1024, UFS_PART_3}, /* reserved6         1730MB p100 */
    {PART_USERDATA,           23360 * 1024,(16UL) * 1024 * 1024, UFS_PART_3}, /* userdata       6G p101 */
#endif
#endif
#endif
    {"0", 0, 0, 0},
};

#endif
