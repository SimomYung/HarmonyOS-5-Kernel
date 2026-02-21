#ifndef _PISCES_PLAT_PARTITION_OHOS_H_
#define _PISCES_PLAT_PARTITION_OHOS_H_

#include "partition_macro.h"
#include "partition_macro_plus.h"
#include "partition_def.h"

static const struct partition partition_table_emmc[] = {
    {"0", 0, 0, 0},
};

static const struct partition partition_table_ufs[] = {
    {PART_XLOADER,                      0,     4 * 1024, UFS_PART_0},
    {PART_RESERVED0,                    0,     2 * 1024, UFS_PART_1},
    {PART_PTABLE,                       0,          512, UFS_PART_2}, /* ptable             512KB p0 */
    {PART_FRP,                        512,          512, UFS_PART_2}, /* frp                512KB p1 */
    {PART_PERSIST,                   1024,     6 * 1024, UFS_PART_2}, /* persist              6MB p2 */
    {PART_RESERVED1,             7 * 1024,         1024, UFS_PART_2}, /* reserved1            1MB p3 */
    {PART_PTABLE_LU3,                   0,          512, UFS_PART_3}, /* ptable_lu3         512KB p0 */
    {PART_VRL,                        512,          512, UFS_PART_3}, /* vrl                512KB p1 */
    {PART_VRL_BACKUP,                1024,          512, UFS_PART_3}, /* vrl_backup         512KB p2 */
    {PART_MODEM_SECURE,              1536,         8704, UFS_PART_3}, /* modem_secure      8704KB p3 */
    {PART_NVME,                 10 * 1024,     4 * 1024, UFS_PART_3}, /* nvme                 4MB p4 */
    {PART_CONN_CALIDATA,        14 * 1024,     1 * 1024, UFS_PART_3}, /* conn_calidata        1MB p5 */
    {PART_CTF,                  15 * 1024,     1 * 1024, UFS_PART_3}, /* ctf                  1MB p6 */
    {PART_OEMINFO,              16 * 1024,    96 * 1024, UFS_PART_3}, /* oeminfo             96MB p7 */
    {PART_SECURE_STORAGE,      112 * 1024,    32 * 1024, UFS_PART_3}, /* secure_storage      32MB p8 */
    {PART_MODEMNVM_FACTORY,    144 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_factory    16MB p9 */
    {PART_MODEMNVM_BACKUP,     160 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_backup     16MB p10 */
    {PART_MODEMNVM_IMG,        176 * 1024,    46 * 1024, UFS_PART_3}, /* modemnvm_img        46MB p11 */
    {PART_HISEE_ENCOS,         222 * 1024,     4 * 1024, UFS_PART_3}, /* hisee_encos          4MB p12 */
    {PART_VERITYKEY,           226 * 1024,     1 * 1024, UFS_PART_3}, /* veritykey            1MB p13 */
    {PART_DDR_PARA,            227 * 1024,     1 * 1024, UFS_PART_3}, /* ddr_para             1MB p14 */
    {PART_LOWPOWER_PARA,       228 * 1024,     1 * 1024, UFS_PART_3}, /* lowpower_para        1MB p15 */
    {PART_BATT_TP_PARA,        229 * 1024,     1 * 1024, UFS_PART_3}, /* batt_tp_para         1MB p16 */
    {PART_PANEL_CALIBRATION,   230 * 1024,   100 * 1024, UFS_PART_3}, /* panel_calibration  100MB p17 */
    {PART_RESERVED2,           330 * 1024,    21 * 1024, UFS_PART_3}, /* reserved2           21MB p18 */
    {PART_LOG,                 351 * 1024,    80 * 1024, UFS_PART_3}, /* log                 80MB p19 */
    {PART_BOOTFAIL_INFO,       431 * 1024,     2 * 1024, UFS_PART_3}, /* bootfail_info        2MB p20 */
    {PART_MISC,                433 * 1024,     2 * 1024, UFS_PART_3}, /* misc                 2MB p21 */
    {PART_DFX,                 435 * 1024,    16 * 1024, UFS_PART_3}, /* dfx                 16MB p22 */
    {PART_RRECORD,             451 * 1024,    16 * 1024, UFS_PART_3}, /* rrecord             16MB p23 */
    {PART_CACHE,               467 * 1024,   104 * 1024, UFS_PART_3}, /* cache              104MB p24 */
    {PART_METADATA,            571 * 1024,    16 * 1024, UFS_PART_3}, /* metadata            16MB p25 */
    {PART_SEPLAT,              587 * 1024,     4 * 1024, UFS_PART_3}, /* seplat               4MB p26 */
    {PART_TOC,                 591 * 1024,     1 * 1024, UFS_PART_3}, /* toc                  1MB p27 */
    {PART_BL2,                 592 * 1024,     4 * 1024, UFS_PART_3}, /* bl2                  4MB p28 */
    {PART_FW_LPM3,             596 * 1024,          512, UFS_PART_3}, /* fw_lpm3            512KB p29 */
    {PART_FW_CPU_LPCTRL, (596 * 1024 + 512),        256, UFS_PART_3}, /* fw_cpu_lpctrl      256KB p30 */
    {PART_FW_GPU_LPCTRL, (596 * 1024 + 768),        128, UFS_PART_3}, /* fw_gpu_lpctrl      128KB p31 */
    {PART_FW_DDR_LPCTRL, (596 * 1024 + 896),        256, UFS_PART_3}, /* fw_ddr_lpctrl      256KB p32 */
    {PART_DDR_XPU_BOOT0, (597 * 1024 + 128),        256, UFS_PART_3}, /* ddr_xpu_boot0      256KB p33 */
    {PART_DDR_XPU_BOOT1, (597 * 1024 + 384),        256, UFS_PART_3}, /* ddr_xpu_boot1      256KB p34 */
    {PART_DDR_XPU_BOOT2, (597 * 1024 + 640),        256, UFS_PART_3}, /* ddr_xpu_boot2      256KB p35 */
    {PART_UCE,           (597 * 1024 + 896),        160, UFS_PART_3}, /* uce                160KB p36 */
    {PART_RESERVED3, (598 * 1024 + 32), 5 * 1024 + 2016, UFS_PART_3}, /* reserved3         7136KB p37 */
    {PART_NPU,                 605 * 1024,     8 * 1024, UFS_PART_3}, /* npu                  8MB p38 */
    {PART_IVP,                 613 * 1024,     2 * 1024, UFS_PART_3}, /* ivp                  2MB p39 */
    {PART_DACC,                615 * 1024,          512, UFS_PART_3}, /* dacc               512KB p40 */
    {PART_VENC,        (615 * 1024 + 512),          512, UFS_PART_3}, /* venc               512KB p41 */
    {PART_HISEE_IMG,           616 * 1024,     4 * 1024, UFS_PART_3}, /* hisee_img            4MB p42 */
    {PART_HHEE,                620 * 1024,     4 * 1024, UFS_PART_3}, /* hhee                 4MB p43 */
    {PART_HISEE_FS,            624 * 1024,     8 * 1024, UFS_PART_3}, /* hisee_fs             8MB p44 */
    {PART_FASTBOOT,            632 * 1024,    12 * 1024, UFS_PART_3}, /* fastboot            12MB p45 */
    {PART_VECTOR,              644 * 1024,     4 * 1024, UFS_PART_3}, /* vector               4MB p46 */
    {PART_ISP_BOOT,            648 * 1024,     2 * 1024, UFS_PART_3}, /* isp_boot             2MB p47 */
    {PART_ISP_FIRMWARE,        650 * 1024,    14 * 1024, UFS_PART_3}, /* isp_firmware        14MB p48 */
    {PART_FW_HIFI,             664 * 1024,    12 * 1024, UFS_PART_3}, /* fw_hifi             12MB p49 */
    {PART_TEEOS,               676 * 1024,     8 * 1024, UFS_PART_3}, /* teeos                8MB p50 */
    {PART_SENSORHUB,           684 * 1024,    13 * 1024, UFS_PART_3}, /* sensorhub           13MB p51 */
    {PART_SENSORHUB_LOG_DIC,   697 * 1024,     3 * 1024, UFS_PART_3}, /* sensorhub_log_dic    3MB p52 */
    {PART_HKRR,                700 * 1024,    16 * 1024, UFS_PART_3}, /* hkrr                16MB p53 */
    {PART_FW_UFSDEV,           716 * 1024,     4 * 1024, UFS_PART_3}, /* fw_ufsdev            4MB p54 */
    {PART_FW_DFA,              720 * 1024,     1 * 1024, UFS_PART_3}, /* fw_dfa               1MB p55 */
    {PART_RESERVED4,           721 * 1024,    15 * 1024, UFS_PART_3}, /* reserved4           15MB p56 */
#ifdef CONFIG_ASAN_SUPPORT_ENABLE
    {PART_UPDATER_RAMDISK,     736 * 1024,    64 * 1024, UFS_PART_3}, /* updater_ramdisk     64MB p57 */
    {PART_UPDATER_VENDOR,      800 * 1024,    30 * 1024, UFS_PART_3}, /* updater_vendor      30MB p58 */
    {PART_BOOT,                830 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p59 */
    {PART_UPDATER_BOOT,        876 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot        64MB p60 */
    {PART_UPDATER_BOOT_BAK,    940 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot_bak    64MB p61 */
    {PART_RESERVED,           1004 * 1024,    13 * 1024, UFS_PART_3}, /* reserved            13MB p62 */
    {PART_THEE,               1017 * 1024,     4 * 1024, UFS_PART_3}, /* thee                 4MB p63 */
    {PART_TZSP,               1021 * 1024,    12 * 1024, UFS_PART_3}, /* tzsp                12MB p64 */
    {PART_UPDATER_RAMDISK_BAK, 1033 * 1024,   64 * 1024, UFS_PART_3}, /* updater_ramdisk_bak 64MB p65 */
    {PART_UPDATER_VENDOR_BAK, 1097 * 1024,    30 * 1024, UFS_PART_3}, /* updater_vendor_bak  30MB p66 */
    {PART_ENG_SYSTEM,         1127 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p67 */
    {PART_ENG_CHIPSET,        1139 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p68 */
    {PART_FW_DTB,             1159 * 1024,    20 * 1024, UFS_PART_3}, /* fw_dtb               8MB p69 */
    {PART_DTBO,               1179 * 1024,    20 * 1024, UFS_PART_3}, /* dtbo                20MB p70 */
    {PART_TRUSTFIRMWARE,      1199 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p71 */
    {PART_MODEM_FW,           1201 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p72 */
    {PART_MODEM_VENDOR,       1325 * 1024,    30 * 1024, UFS_PART_3}, /* modem_vendor        30MB p73 */
    {PART_MODEM_PATCH_NV,     1355 * 1024,     4 * 1024, UFS_PART_3}, /* modem_patch_nv       4MB p74 */
    {PART_MODEM_DRIVER,       1359 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p75 */
    {PART_MODEMNVM_UPDATE,    1379 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p76 */
    {PART_MODEMNVM_CUST,      1395 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p77 */
    {PART_RAMDISK,            1411 * 1024,     5 * 1024, UFS_PART_3}, /* ramdisk              5MB p78 */
    {PART_RVT_SYSTEM,         1416 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system           1MB p79 */
    {PART_RVT_VENDOR,         1417 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor           1MB p80 */
    {PART_RVT_CHIP_PROD,      1418 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod        1MB p81 */
    {PART_RVT_CUST,           1419 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust             1MB p82 */
    {PART_RVT_SYS_PROD,       1420 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod         1MB p83 */
    {PART_UPDATER_RVT,        1421 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p84 */
    {PART_UPDATER_BAK_RVT,    1423 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p85 */
    {PART_RVT,                1425 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p86 */
    {PART_KPATCH,             1429 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p87 */
    {PART_PATCH,              1433 * 1024,    32 * 1024, UFS_PART_3}, /* patch               32MB p88 */
    {PART_CUST,               1465 * 1024,    88 * 1024, UFS_PART_3}, /* cust                88MB p89 */
    {PART_CHIP_PROD,          1553 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p90 */
    {PART_VENDOR,             4571 * 1024,  3284 * 1024, UFS_PART_3}, /* vendor            3284MB p91 */
    {PART_SYS_PROD,           7855 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod          8812MB p92 */
    {PART_SYSTEM,            16667 * 1024,  7568 * 1024, UFS_PART_3}, /* system            7568 p93 */
    {PART_HYPERHOLD,         24235 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p94 */
    {PART_RESERVED5,         24239 * 1024,   131 * 1024, UFS_PART_3}, /* reserved5          131MB p95 */
    {PART_VERSION,           24370 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p96 */
    {PART_PRELOAD,           24946 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p97 */
    {PART_RESERVED9,         26346 * 1024,  1024 * 1024, UFS_PART_3}, /* reserved9            1GB p98 */
    {PART_USERDATA,  27370 * 1024, (13UL) * 1024 * 1024, UFS_PART_3}, /* userdata            13GB p99 */
#else
    {PART_UPDATER_RAMDISK,     736 * 1024,    32 * 1024, UFS_PART_3}, /* updater_ramdisk     32MB p57 */
    {PART_UPDATER_VENDOR,      768 * 1024,    28 * 1024, UFS_PART_3}, /* updater_vendor      28MB p58 */
#ifdef CONFIG_SANITIZER_ENABLE
    {PART_BOOT,                796 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p59 */
    {PART_UPDATER_BOOT,        842 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot        64MB p60 */
    {PART_UPDATER_BOOT_BAK,    906 * 1024,    64 * 1024, UFS_PART_3}, /* updater_boot_bak    64MB p61 */
    {PART_RESERVED,            970 * 1024,    35 * 1024, UFS_PART_3}, /* reserved            35MB p62 */
#else
    {PART_BOOT,                796 * 1024,    46 * 1024, UFS_PART_3}, /* boot                46MB p59 */
    {PART_UPDATER_BOOT,        842 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot        54MB p60 */
    {PART_UPDATER_BOOT_BAK,    896 * 1024,    54 * 1024, UFS_PART_3}, /* updater_boot_bak    54MB p61 */
    {PART_RESERVED,            950 * 1024,    55 * 1024, UFS_PART_3}, /* reserved            55MB p62 */
#endif
    {PART_THEE,               1005 * 1024,     4 * 1024, UFS_PART_3}, /* thee                 4MB p63 */
    {PART_TZSP,               1009 * 1024,    12 * 1024, UFS_PART_3}, /* tzsp                12MB p64 */
    {PART_UPDATER_RAMDISK_BAK, 1021 * 1024,   32 * 1024, UFS_PART_3}, /* updater_ramdisk_bak 32MB p65 */
    {PART_UPDATER_VENDOR_BAK, 1053 * 1024,    28 * 1024, UFS_PART_3}, /* updater_vendor_bak  28MB p66 */
    {PART_ENG_SYSTEM,         1081 * 1024,    12 * 1024, UFS_PART_3}, /* eng_system          12MB p67 */
    {PART_ENG_CHIPSET,        1093 * 1024,    20 * 1024, UFS_PART_3}, /* eng_chipset         20MB p68 */
    {PART_FW_DTB,             1113 * 1024,    20 * 1024, UFS_PART_3}, /* fw_dtb              20MB p69 */
    {PART_DTBO,               1133 * 1024,    20 * 1024, UFS_PART_3}, /* dtbo                20MB p70 */
    {PART_TRUSTFIRMWARE,      1153 * 1024,     2 * 1024, UFS_PART_3}, /* trustfirmware        2MB p71 */
    {PART_MODEM_FW,           1155 * 1024,   124 * 1024, UFS_PART_3}, /* modem_fw           124MB p72 */
    {PART_MODEM_VENDOR,       1279 * 1024,    10 * 1024, UFS_PART_3}, /* modem_vendor        10MB p73 */
    {PART_MODEM_PATCH_NV,     1289 * 1024,     4 * 1024, UFS_PART_3}, /* modem_patch_nv       4MB p74 */
    {PART_MODEM_DRIVER,       1293 * 1024,    20 * 1024, UFS_PART_3}, /* modem_driver        20MB p75 */
    {PART_MODEMNVM_UPDATE,    1313 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_update     16MB p76 */
    {PART_MODEMNVM_CUST,      1329 * 1024,    16 * 1024, UFS_PART_3}, /* modemnvm_cust       16MB p77 */
    {PART_RAMDISK,            1345 * 1024,     3 * 1024, UFS_PART_3}, /* ramdisk              3MB p78 */
    {PART_RVT_SYSTEM,         1348 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_system           1MB p79 */
    {PART_RVT_VENDOR,         1349 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_vendor           1MB p80 */
    {PART_RVT_CHIP_PROD,      1350 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_chip_prod        1MB p81 */
    {PART_RVT_CUST,           1351 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_cust             1MB p82 */
    {PART_RVT_SYS_PROD,       1352 * 1024,     1 * 1024, UFS_PART_3}, /* rvt_sys_prod         1MB p83 */
    {PART_UPDATER_RVT,        1353 * 1024,     2 * 1024, UFS_PART_3}, /* updater_rvt          2MB p84 */
    {PART_UPDATER_BAK_RVT,    1355 * 1024,     2 * 1024, UFS_PART_3}, /* updater_bak_rvt      2MB p85 */
    {PART_RVT,                1357 * 1024,     4 * 1024, UFS_PART_3}, /* rvt                  4MB p86 */
    {PART_KPATCH,             1361 * 1024,     4 * 1024, UFS_PART_3}, /* kpatch               4MB p87 */
    {PART_PATCH,              1365 * 1024,   128 * 1024, UFS_PART_3}, /* patch              128MB p88 */
#if defined(CONFIG_FACTORY_MODE)
    {PART_RESERVED5,          1493 * 1024,   137 * 1024, UFS_PART_3}, /* reserved5          137MB p89 */
    {PART_HIBENCH_IMG,        1630 * 1024,   128 * 1024, UFS_PART_3}, /* hibench_img        128MB p90 */
    {PART_HIBENCH_LOG,        1758 * 1024,    32 * 1024, UFS_PART_3}, /* hibench_log         32MB p91 */
    {PART_HIBENCH_DATA,       1790 * 1024,  1000 * 1024, UFS_PART_3}, /* hibench_data      1000MB p92 */
    {PART_FLASH_AGEING,       2790 * 1024,   512 * 1024, UFS_PART_3}, /* flash_ageing       512MB p93 */
    {PART_HIBENCH_LPM3,       3302 * 1024,     2 * 1024, UFS_PART_3}, /* hibench_lpm3         2MB p94 */
    {PART_XLOADER_HITEST,     3304 * 1024,     2 * 1024, UFS_PART_3}, /* xloader_hitest       2MB p95 */
    {PART_CUST,               3306 * 1024,   128 * 1024, UFS_PART_3}, /* cust               128MB p96 */
    {PART_CHIP_PROD,          3434 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p97 */
    {PART_VENDOR,             6452 * 1024,  1132 * 1024, UFS_PART_3}, /* vendor            1132MB p98 */
    {PART_SYS_PROD,           7584 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod          8812MB p99 */
    {PART_SYSTEM,            16396 * 1024,  4948 * 1024, UFS_PART_3}, /* system            4948MB p100 */
    {PART_HYPERHOLD,         21344 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p101 */
    {PART_VERSION,           21348 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p102 */
    {PART_PRELOAD,           21924 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p103 */
    {PART_RESERVED6,         23324 * 1024, 12070 * 1024, UFS_PART_3}, /* reserved6         12070MB p104 */
    {PART_USERDATA,  35394 * 1024, (16UL) * 1024 * 1024, UFS_PART_3}, /* userdata              6GB p105 */
#else
    {PART_CUST,               1493 * 1024,   128 * 1024, UFS_PART_3}, /* cust               128MB p89 */
    {PART_CHIP_PROD,          1621 * 1024,  3018 * 1024, UFS_PART_3}, /* chip_prod         3018MB p90 */
    {PART_VENDOR,             4639 * 1024,  1132 * 1024, UFS_PART_3}, /* vendor            1132MB p91 */
    {PART_SYS_PROD,           5771 * 1024,  8812 * 1024, UFS_PART_3}, /* sys_prod          8812MB p92 */
    {PART_SYSTEM,            14583 * 1024,  4948 * 1024, UFS_PART_3}, /* system            4948MB p93 */
    {PART_HYPERHOLD,         19531 * 1024,     4 * 1024, UFS_PART_3}, /* hyperhold            4MB p94 */
    {PART_RESERVED5,         19535 * 1024,   137 * 1024, UFS_PART_3}, /* reserved5          137MB p95 */
    {PART_VERSION,           19672 * 1024,   576 * 1024, UFS_PART_3}, /* version            576MB p96 */
    {PART_PRELOAD,           20248 * 1024,  1400 * 1024, UFS_PART_3}, /* preload           1400MB p97 */
#if defined(CONFIG_PARTITION_ENG)
    {PART_FTTEST,            21648 * 1024,   192 * 1024, UFS_PART_3}, /* fttest             192MB p98 */
    {PART_DDRTEST,           21840 * 1024,    48 * 1024, UFS_PART_3}, /* ddrtest             48MB p99 */
    {PART_HITEST_GPU,        21888 * 1024,   512 * 1024, UFS_PART_3}, /* hitest_gpu         512MB p100 */
    {PART_HITEST_ASP_DHRYSTONE, 22400 * 1024, 20 * 1024, UFS_PART_3}, /* hitest_asp_dhrystone 20MB p101 */
    {PART_HITEST_ASP_TEST,   22420 * 1024,   300 * 1024, UFS_PART_3}, /* hitest_asp_test    300MB p102 */
    {PART_HITEST_SENSORHUB,  22720 * 1024,     6 * 1024, UFS_PART_3}, /* hitest_sensorhub     6MB p103 */
    {PART_HITEST_ISP,        22726 * 1024,    25 * 1024, UFS_PART_3}, /* hitest_isp          25MB p104 */
    {PART_HITEST_ISP_AVS,    22751 * 1024,    65 * 1024, UFS_PART_3}, /* hitest_isp_avs      65MB p105 */
    {PART_HITEST_NPU,        22816 * 1024,    80 * 1024, UFS_PART_3}, /* hitest_npu          80MB p106 */
    {PART_HITEST_DSS,        22896 * 1024,    18 * 1024, UFS_PART_3}, /* hitest_dss          18MB p107 */
    {PART_DSS_DISP_BUF,      22914 * 1024,   128 * 1024, UFS_PART_3}, /* dss_disp_buf       128MB p108 */
    {PART_HITEST_JPEGENC,    23042 * 1024,   200 * 1024, UFS_PART_3}, /* hitest_jpegenc     200MB p109 */
    {PART_HITEST_HIPP,       23242 * 1024,   100 * 1024, UFS_PART_3}, /* hitest_hipp        100MB p110 */
    {PART_HIBENCH_LPM3,      23342 * 1024,     2 * 1024, UFS_PART_3}, /* hibench_lpm3         2MB p111 */
    {PART_JPEG_DEC_TEST_SRC, 23344 * 1024,    20 * 1024, UFS_PART_3}, /* jpeg_dec_test_src   20MB p112 */
    {PART_JPEG_DEC_TEST_DEST, 23364 * 1024,   80 * 1024, UFS_PART_3}, /* jpeg_dec_test_dest  80MB p113 */
    {PART_VCODE_DATA,        23444 * 1024,   140 * 1024, UFS_PART_3}, /* vcode_data         140MB p114 */
    {PART_HITEST_FFRT,       23584 * 1024,    20 * 1024, UFS_PART_3}, /* hitest_ffrt         20MB p115 */
    {PART_VCODE_MAX_DATA,    23604 * 1024,   140 * 1024, UFS_PART_3}, /* vcode_max_data     140MB p116 */
    {PART_MODEM_TSP_TEST,    23744 * 1024,    36 * 1024, UFS_PART_3}, /* modem_tsp_test      36MB p117 */
    {PART_MODEM_VECTOR_CONFIG, 23780 * 1024,   4 * 1024, UFS_PART_3}, /* modem_vector_config  4MB p118 */
    {PART_MODEM_TSP_DT,      23784 * 1024,     1 * 1024, UFS_PART_3}, /* modem_tsp_dt         1MB p119 */
    {PART_MODEM_L2DLE,       23785 * 1024,     4 * 1024, UFS_PART_3}, /* modem_l2dle          4MB p120 */
    {PART_MODEM_BBP_TEST,    23789 * 1024,   100 * 1024, UFS_PART_3}, /* modem_bbp_test     100MB p121 */
    {PART_MODEM_PSA_TEST,    23889 * 1024,    20 * 1024, UFS_PART_3}, /* modem_psa_test      20MB p122 */
    {PART_MODEM_PDE_TEST,    23909 * 1024,     5 * 1024, UFS_PART_3}, /* modem_pde_test       5MB p123 */
    {PART_XLOADER_HITEST,    23914 * 1024,     2 * 1024, UFS_PART_3}, /* xloader_hitest       2MB p124 */
    {PART_HITEST_OISP,       23916 * 1024,    25 * 1024, UFS_PART_3}, /* hitest_oisp         25MB p125 */
    {PART_RESERVED8,         23941 * 1024,     1 * 1024, UFS_PART_3}, /* reserved8            1MB p126 */
    {PART_USERDATA,  23942 * 1024, (16UL) * 1024 * 1024, UFS_PART_3}, /* userdata            16GB p127 */
#else // system in ccm is 5716,but now less 768
    {PART_RESERVED9,         21648 * 1024,  1816 * 1024, UFS_PART_3}, /* reserved9         1816MB p98 */
    {PART_USERDATA,  23464 * 1024, (16UL) * 1024 * 1024, UFS_PART_3}, /* userdata            16GB p99 */
#endif
#endif
#endif
    {"0", 0, 0, 0},
};

#endif
