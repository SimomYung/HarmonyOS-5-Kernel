/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: improve_yield_info header file
 * Create: 2025-02-17
 */
#ifndef __IMPROVE_YIELD_INFO_H__
#define __IMPROVE_YIELD_INFO_H__

enum improve_yield_module_id {
	MODULE_ID_START  = 0,
	MODULE_ID_ENABLE = MODULE_ID_START,
	MODULE_ID_CPU,
	MODULE_ID_RESERVED1,
	MODULE_ID_GPU,
	MODULE_ID_VDEC,
	MODULE_ID_CPU_PANIC,
	MODULE_ID_APP_PANIC,
	MODULE_ID_RESERVED2,
	MODULE_ID_MAX,
};

#define IMPROVE_YIELD_ENABLE_INFO_SIZE 1 /* byte 0 */
#define IMPROVE_YIELD_CPU_INFO_SIZE 12   /* byte 1-12 */
#define IMPROVE_YIELD_RESEVED1_SIZE 28   /* byte 13-40 */
#define IMPROVE_YIELD_GPU_INFO_SIZE 29   /* byte 41-69 */
#define IMPROVE_YIELD_VDEC_INFO_SIZE 3   /* byte 70-72 */
#define IMPROVE_YIELD_CPU_PANIC_INFO_SIZE  3   /* byte 73-75 */
#define IMPROVE_YIELD_APP_PANIC_INFO_SIZE  1   /* byte 76 */
#define IMPROVE_YIELD_RESEVED2_SIZE 27   /* byte 77-103 */

static const unsigned int g_improve_yield_nv_info_map[MODULE_ID_MAX] = {
	[MODULE_ID_ENABLE]         = IMPROVE_YIELD_ENABLE_INFO_SIZE,
	[MODULE_ID_CPU]        	   = IMPROVE_YIELD_CPU_INFO_SIZE,
	[MODULE_ID_RESERVED1]      = IMPROVE_YIELD_RESEVED1_SIZE,
	[MODULE_ID_GPU]            = IMPROVE_YIELD_GPU_INFO_SIZE,
	[MODULE_ID_VDEC]           = IMPROVE_YIELD_VDEC_INFO_SIZE,
	[MODULE_ID_CPU_PANIC]      = IMPROVE_YIELD_CPU_PANIC_INFO_SIZE,
	[MODULE_ID_APP_PANIC]      = IMPROVE_YIELD_APP_PANIC_INFO_SIZE,
	[MODULE_ID_RESERVED2]      = IMPROVE_YIELD_RESEVED2_SIZE,
};

static const char *g_improve_yield_module_list[MODULE_ID_MAX] = {
	[MODULE_ID_ENABLE]        = "NEVER_BE_USED",
	[MODULE_ID_CPU]           = "CPU_ECC",
	[MODULE_ID_RESERVED1]     = "RESERVED1",
	[MODULE_ID_GPU]           = "GPU",
	[MODULE_ID_VDEC]          = "VDEC",
	[MODULE_ID_CPU_PANIC]     = "CPU_PANIC",
	[MODULE_ID_APP_PANIC]     = "APP_PANIC",
	[MODULE_ID_RESERVED2] 	  = "RESERVED2",
};

struct improve_yield_info {
	unsigned char EnabInfo[IMPROVE_YIELD_ENABLE_INFO_SIZE];
	unsigned char CpuInfo[IMPROVE_YIELD_CPU_INFO_SIZE];
	unsigned char Reserved1[IMPROVE_YIELD_RESEVED1_SIZE];
	unsigned char GpuInfo[IMPROVE_YIELD_GPU_INFO_SIZE];
	unsigned char VdecInfo[IMPROVE_YIELD_VDEC_INFO_SIZE];
	unsigned char CpuPanicInfo[IMPROVE_YIELD_CPU_PANIC_INFO_SIZE];
	unsigned char AppPanicInfo[IMPROVE_YIELD_APP_PANIC_INFO_SIZE];
	unsigned char Reserved2[IMPROVE_YIELD_RESEVED2_SIZE];
} __attribute__((aligned(1)));

#endif /* __IMPROVE_YIELD_INFO_H__ */
