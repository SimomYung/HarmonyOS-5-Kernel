/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022 ~ 2022. All rights reserved.
 * Description: Atfork hook interfaces for malloc
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 22 15:50:08 2022
 */

#ifndef HM_INCLUDE_HIMALLOC_HEALTHMANAGE_H
#define HM_INCLUDE_HIMALLOC_HEALTHMANAGE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MEM_HM_OPTION_WITH_DAMAGE_CHECK 0x1
#define MEM_HM_OPTION_WITH_STATISTIC 0x2
#define MEM_HM_OPTION_WITH_LEAK_CHECK 0x4

#define MEM_HM_ERRNO_NOMEM (12)
#define MEM_HM_ERRNO_BUSY  (16)
#define MEM_HM_ERRNO_INVAL (22)

void MemHMSwitchSet(uint32_t option);
uint32_t MemHMSwitchGet(void);

void MemHMStatisticsShow(uint32_t option);

int32_t MemHMUserMemDamageCheck(void *userAddr);
int32_t MemHMBatchMemDamageCheck(void);

void MemHMLeakInfoClean(void);
int32_t MemHMLeakScan(void);
int32_t MemHMLeakInfoGet(int32_t *memNum, size_t *memSize);

typedef void (*MemHMLogFunc)(int32_t level, int32_t errorCode, const char *fileName,
			     uint16_t line, const char *fmt, ...);
void MemHMLogFuncReg(MemHMLogFunc logFunc);

#ifdef __cplusplus
}
#endif

#endif /* HM_INCLUDE_HIMALLOC_HEALTHMANAGE_H */
