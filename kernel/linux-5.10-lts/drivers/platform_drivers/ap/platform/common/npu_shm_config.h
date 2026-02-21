/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: npu share mem config
 * Create: 2021-12-2
 */
#ifndef __NPU_SHM_CONFIG_H__
#define __NPU_SHM_CONFIG_H__
#include <npu_ddr_map.h>

#define NPU_LOAD_TSFW_CONFIG_SIZE           0x1000
#define NPU_AUTOBOOT_DATA_SIZE              0x2000

struct npu_chip_cfg {
	unsigned int valid_magic; /* if value is 0x5a5a5a5a, valid_magic is ok */
	/* bit0 is aicore0, bit1 is aicore1;each bit:0:enable 1:disable */
	unsigned int aicore_disable_bitmap;
	unsigned int platform_specification; /* follow efuse Grading chip type */
};

typedef union {
	struct {
		struct npu_chip_cfg chip_cfg;
	} cfg;
	unsigned char reserved[NPU_S_NPU_CONFIG_SIZE];
} npu_shm_cfg;
#pragma pack(push)
#pragma pack(4)
typedef union
{
	struct {
		unsigned char load_random; // use v200+
		unsigned int canary; // use charlotte
		volatile unsigned int boot_status; // use charlotte
		volatile unsigned int ts_pd_stage; // use charlotte
	} cfg;
	unsigned char reserved[NPU_LOAD_TSFW_CONFIG_SIZE];
} npu_load_tsfw_cfg;
#pragma pack(pop)

#define NPU_AUTOBOOT_BASE_ADDR     (RESERVED_NPU_AI_TS_FW_PHYMEM_BASE + RESERVED_NPU_AI_TS_FW_PHYMEM_SIZE -\
	NPU_LOAD_TSFW_CONFIG_SIZE - NPU_AUTOBOOT_DATA_SIZE) /* 0x30340000 + 0x400000 - 0x1000 - 0x2000 = 0x3073D000 */

#endif /* __NPU_SHM_CONFIG_H__ */
