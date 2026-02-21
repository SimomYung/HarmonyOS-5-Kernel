/**
 * @copyright   Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @brief       User data processing external header file
 */

#ifndef BSL_USER_DATA_H
#define BSL_USER_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Modify the BSL_MAX_EX_TYPE if a new index is added */
#define BSL_USER_DATA_EX_INDEX_SSL              0
#define BSL_USER_DATA_EX_INDEX_X509_STORE_CTX   1
#define BSL_USER_DATA_EX_INDEX_SSL_CTX          2
#define BSL_USER_DATA_EX_INDEX_X509_STORE       3
#define BSL_USER_DATA_EX_INDEX_UIO              4

#define BSL_MAX_EX_TYPE 5
#define BSL_MAX_EX_DATA 20

typedef struct {
    void *sk[BSL_MAX_EX_DATA];
} BSL_USER_ExData;

typedef void BSL_USER_ExDataNew(void *parent, void *ptr, BSL_USER_ExData *ad, int idx, long argl, void *argp);
typedef void BSL_USER_ExDataFree(void *parent, void *ptr, BSL_USER_ExData *ad, int idx, long argl, void *argp);
typedef int BSL_USER_ExDataDup(BSL_USER_ExData *to, const BSL_USER_ExData *from, void **fromD, int idx, long argl,
    void *argp);

int BSL_USER_SetExData(BSL_USER_ExData *ad, int32_t idx, void *val);

void *BSL_USER_GetExData(const BSL_USER_ExData *ad, int32_t idx);

int BSL_USER_GetExDataNewIndex(int32_t index, int64_t argl, const void *argp, const void *newFunc, const void *dupFunc,
    void *freeFunc);

void BSL_USER_FreeExDataIndex(int32_t index, void *obj, BSL_USER_ExData *ad);

#ifdef __cplusplus
}
#endif

#endif // BSL_USER_DATA_H
