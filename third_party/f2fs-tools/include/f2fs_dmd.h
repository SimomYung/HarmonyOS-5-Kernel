/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/

#ifndef __F2FS_DMD_H__
#define __F2FS_DMD_H__

#include "f2fs_dfx_common.h"
#include "f2fs_dmd_errno.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MB_BLK_SHIFT 8
#define NR_ERR_BITMAP_UINT64 3
#define FSCK_REPORT_MSG_SIZE 1536
#define FSCK_OVERTIME_MS 1000

struct DmdMsg {
    char *buffer;
    unsigned int size;
    unsigned int offset;
};

#ifndef CONF_TARGET_HOST
struct __attribute__((packed)) DmdReport {
    uint64_t errBitmap[NR_ERR_BITMAP_UINT64];
    unsigned int propBitmap;  /* used as flag bitmap of properties */
    unsigned long usedSpace;  /* space taken by node and data blocks in MB */
    unsigned long freeSpace;  /* free space in MB */
    unsigned int features;
    unsigned short segsPerSec;
    unsigned short secsPerZone;
    unsigned long totalFsSectors;
    unsigned long ckptVersion;
    unsigned int ckptState;
    unsigned long costTime;
    char msg[FSCK_REPORT_MSG_SIZE];
};

#define IS_UNISTORE_FL  0x00000001
#define FB_LOCKED_FL    0x00000002

extern struct DmdReport g_dmdReport;
extern struct DmdMsg g_reportMsg;
extern struct DmdMsg g_assertMsg;

#define DMD_SET_VALUE(field, value) ((g_dmdReport.field) = (value))
#define DMD_ADD_ERROR(type, err) DmdInsertError(type, err, __func__, __LINE__)
#define DMD_ADD_MSG_ERROR(type, err, fmt, ...) DmdInsertMsgError(type, err, __func__, __LINE__, \
    "[ERRMSG(%s:%d)"fmt"]", __func__, __LINE__, ##__VA_ARGS__)
#define DMD_ASSERT_MSG(func, line, fmt, ...) DmdAssertMsg("[ASSERT(%s:%d)"fmt"]", func, line, ##__VA_ARGS__)
#define DmdInsertMsgError(type, err, func, line, fmt, ...)   \
    do {                                                     \
        DmdInsertError(type, err, func, line);               \
        PrintToMsg(&g_reportMsg, fmt, ##__VA_ARGS__);        \
    } while (0)
#define DmdAssertMsg(fmt, ...)  PrintToMsg(&g_assertMsg, fmt, ##__VA_ARGS__)
#define COMPUTE_SIZE(sbi)   \
    do {                    \
            uint64_t nodeSecs = round_up((sbi)->total_valid_node_count, BLKS_PER_SEC(sbi));         \
            uint64_t dataSecs = round_up((sbi)->total_valid_block_count -                           \
                        (sbi)->total_valid_node_count, BLKS_PER_SEC(sbi));                          \
            uint64_t freeBlks = ((sbi)->total_sections - dataSecs - nodeSecs) * BLKS_PER_SEC(sbi);  \
            uint64_t totalSize = g_dmdReport.totalFsSectors << (sbi)->log_sectors_per_block >> MB_BLK_SHIFT; \
            g_dmdReport.freeSpace = freeBlks >> MB_BLK_SHIFT;                                       \
            g_dmdReport.usedSpace = totalSize - g_dmdReport.freeSpace;                           \
    } while (0)
#define DMD_CHECK_COST_TIME(sbi, costMs)        \
    do {                                        \
        g_dmdReport.costTime = (costMs);        \
        if (g_dmdReport.usedSpace == 0) {    \
            COMPUTE_SIZE(sbi); /* Compute usedSpace if not set by fsck_verify */ \
        }                                       \
        DmdCheckCostTime(__func__, __LINE__);   \
    } while (0)

void DmdInsertError(int type, unsigned int err, const char *func, int line);
void PrintToMsg(struct DmdMsg *dmdMsg, const char *fmt, ...);
void DmdCheckCostTime(const char *func, int line);

#else
#define DMD_SET_VALUE(field, value)
#define DMD_ADD_ERROR(type, err)
#define DMD_ADD_MSG_ERROR(type, err, fmt, ...)
#define DMD_ASSERT_MSG(func, line, fmt, ...)
#define DMD_CHECK_COST_TIME(sbi, costMs)
#endif /* CONF_TARGET_HOST */

int DmdReport(void);

#ifdef __cplusplus
}
#endif
#endif
