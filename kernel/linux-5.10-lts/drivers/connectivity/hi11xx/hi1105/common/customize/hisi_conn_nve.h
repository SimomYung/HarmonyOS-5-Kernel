/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: hisi_conn_nve.c header file
 * Author: @CompanyNameTag
 */

#ifdef HISI_CONN_NVE_SUPPORT
#ifndef HISI_CONN_NVE_H
#define HISI_CONN_NVE_H

/* 其他头文件包含 */
#include "plat_type.h"
#include "oam_wdk.h"
#include "oal_ext_if.h"
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/compat.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#ifdef CONFIG_SPI_NOR_AGENT
#include <platform_include/basicplatform/linux/spi_nor/spi_nor.h>
#endif
#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HISI_CONN_NVE_H

/* conn_nve分区相关定义 */
#ifdef CONFIG_SPI_NOR_AGENT
#define CONN_NVE_DEVICE_NAME        "oeminfo_nor"
/* hisi_conn_nve大小512KB */
#define CONN_NVE_SIZE               0x80000
#define CONN_NVE_PART_OFFSET        0x180000
#else
#define CONN_NVE_DEVICE_NAME        "/dev/block/by-name/conn_calidata"
#ifdef _PRE_HISI_HERTZ
#define MTD_DEVICE_NM        "wifidata"
#endif
/* hisi_conn_nve大小1MB */
#define CONN_NVE_SIZE               0x100000
#endif
#define CRC32C_REV_SEED             0

/* 固定使用的nv id */
#define CONN_NV_EFUSE_NAME          "efuse"
#define CONN_NV_EFUSE_ID            384
#define CONN_NV_EFUSE_SIZE          104

/* 返回值定义 */
typedef enum {
    CONN_NVE_RET_OK,
    CONN_NVE_ERROR_INIT,
    CONN_NVE_ERROR_WRITE,
    CONN_NVE_ERROR_READ,
    CONN_NVE_ERROR_SET_SIZE,
    CONN_NVE_ERROR_OVERSIZE,
    CONN_NVE_ERROR_NO_PART,
    CONN_NVE_ERROR_BUT,
} hisi_conn_ret;

typedef enum {
    HISI_CONN_WIFI_DATA,
    HISI_CONN_BFGX_DATA,
    // 后续依次往后添加
    HISI_CONN_PTABLE_BUT,
} hisi_conn_ptable_id;

typedef struct {
    uint32_t id;
    uint32_t start;
    uint32_t len;
} hisi_conn_particion_stru;
/* 函数声明 */
/* conn_nve 分区函数接口 */
uint8_t hisi_conn_nve_setup_done(void);
int32_t hisi_conn_nve_alloc_ramdisk(void);
void hisi_conn_nve_free_ramdisk(void);
uint8_t* hisi_conn_nve_get_ramdisk(uint32_t particion_id);
uint32_t crc32c_conn_nve(uint32_t crc, const uint8_t *buf, unsigned long long len);
int32_t hisi_conn_nve_set_particion_size(uint32_t particion_id, uint32_t len);
int32_t hisi_conn_nve_read(uint32_t particion_id, size_t offset, size_t len, uint8_t *buf);
int32_t hisi_conn_nve_write(uint32_t particion_id, size_t offset, size_t len, uint8_t *buf);

extern int bsp_blk_read(const char *part_name, loff_t part_offset, const void *data_addr, size_t data_len);
extern int bsp_blk_write(const char *part_name, loff_t part_offset, const void *data_addr, size_t data_len);
#endif
#endif
