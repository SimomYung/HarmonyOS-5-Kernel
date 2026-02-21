/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#ifndef HVGR_LOCAL_DUMP_H
#define HVGR_LOCAL_DUMP_H

#include "hvgr_defs.h"

#define HVGR_DEBUG_BUS_TIME             "%4d-%02d-%02d_%02d-%02d-%02d"
#define HVGR_DEBUG_BUS_FILE_PREFIX      "gpu_debugbus_"
#define INVALID_REGISTER_VALUE          0xdeadbeef
#define RECORD_FORMAT                   "0x%08x:0x%08x\n"
#define GPU_CONTROL_TITLE               "gpu control register:\n"
#define MISC_CONTROL_TITLE              "misc control register:\n"
#define JOB_CONTROL_TITLE               "job control register:\n"
#define MMU_CONTROL_TITLE               "mmu control register:\n"
#define FCP_CONTROL_TITLE               "fcp control register:\n"
#define CQ_CONTROL_TITLE                "cq control register:\n"
#define HVGR_DEBUG_BUS_DUMP_FIR         0
#define HVGR_DEBUG_BUS_DUMP_SEC         1
#define HVGR_DEBUG_BUS_CNT_SEC          2
#define MIN_FREE_SPACE                  1024
#define DUMP_DATA_MAX_SIZE              64
#define FAULT_NUMBER                    5
#define MAX_TIME_SIZE                   20
#define MAX_READ_TIMES                  50
#define HVGR_DEBUG_BUS_MAX_NUMBER       99
#define HVGR_DEBUG_BUS_DATA_DIR_MODE    0777
#define BASE_YEAR                       1900
#ifdef CONFIG_LIBLINUX_CDC
#define HVGR_DEBUG_BUS_RECORD_LEN       320
#define HVGR_DEBUG_BUS_RECORD           "src_file[%s] dest_file[%s]\n"
#define HVGR_DEBUG_BUS_DEBUG_FS         "/sys/kernel/debug/hvgr0/datan/debugbus"
#endif

u32 hvgr_debugbus_get_buffer_size(struct hvgr_device * const gdev);

void hvgr_local_debugbus_write_to_file(char *buffer, char *file_name, size_t buf_len);

bool hvgr_local_debugbus_check_disk(void);

void hvgr_local_debugbus_dump(struct hvgr_device *gdev);

#endif /* HVGR_LOCAL_DUMP_H END */
