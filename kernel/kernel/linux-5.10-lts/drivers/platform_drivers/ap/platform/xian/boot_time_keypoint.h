/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: boot time keypoint define
 * Create: 2023/04/25
 * Notes: used for all stage code
 */
#ifndef __BOOT_TIME_KEYPOINT_DEF__H__
#define __BOOT_TIME_KEYPOINT_DEF__H__

#include <mntn_public_interface.h>
#define BOOT_TIME_KEYPOINT_ADDR(base, stage_id)            ((base) + (0x8 * (stage_id)))
 
/* SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE */
#define DDR_BOOT_TIME_KEYPOINT_ADDR 		SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_BASE
#define DDR_BOOT_TIME_KEYPOINT_SIZE 		SUB_RESERVED_BOOT_TIME_KEYPOINT_PYHMEM_SIZE
 
struct boot_time_keypoint{
    unsigned short stage; /* boot stage */
    unsigned short start_time; /* start time */
    unsigned short end_time; /* end time */
    unsigned short value; /* reserve key value */
};
 
enum key_stage {
	BOOTTIME_PRELOADER_STAGE = 0,
	BOOTTIME_XLOADER_STAGE,
	BOOTTIME_BL2_STAGE,
	BOOTTIME_BL31_STAGE,
	BOOTTIME_THEE_STAGE,
	BOOTTIME_TEEOS_STAGE,
	BOOTTIME_TZSP_STAGE,
	BOOTTIME_HHEE_STAGE,
	BOOTTIME_UEFI_STAGE,
	BOOTTIME_KERNEL_STAGE,
	BOOTTIME_ANDROID_STAGE,
	BOOTTIME_MAX_STAGE,
};
 
static inline void set_time_keypoint(struct boot_time_keypoint *info)
{
	struct boot_time_keypoint *boot_time_info = \
		(struct boot_time_keypoint *)((unsigned long)(BOOT_TIME_KEYPOINT_ADDR(DDR_BOOT_TIME_KEYPOINT_ADDR, info->stage)));
 
	boot_time_info->stage = info->stage;
	boot_time_info->start_time = info->start_time ? info->start_time : boot_time_info->start_time;
	boot_time_info->end_time = info->end_time ? info->end_time : boot_time_info->end_time;
	boot_time_info->value = info->value ? info->value : boot_time_info->value;
}
 
#endif