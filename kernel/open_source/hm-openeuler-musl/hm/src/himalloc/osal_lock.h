/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef OSAL_LOCK_H
#define OSAL_LOCK_H

#include <pthread.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(4)
typedef struct tagOsalLock {
    pthread_mutex_t mutex;
} OSAL_LOCK_T;

#pragma pack()

#define OSAL_OK 0

/**
 * @ingroup osal_lock
 * 0xE 地址错误。
 */
#define OSAL_ERRNO_FAULT 14 /* Bad address */

/**
 * @ingroup osal_lock
 * 0x16 参数错误。
 */
#define OSAL_ERRNO_INVAL 22 /* Invalid argument */

static inline uint32_t OSAL_LockCreate(OSAL_LOCK_T *pOsalLock)
{
    pthread_mutexattr_t recurAttr;
    uint32_t uiRet = OSAL_OK;

    (void)pthread_mutexattr_init(&recurAttr);
    (void)pthread_mutexattr_settype(&recurAttr, PTHREAD_MUTEX_RECURSIVE);

    if (pthread_mutex_init(&pOsalLock->mutex, &recurAttr) != OSAL_OK) {
        (void)pthread_mutexattr_destroy(&recurAttr);
        uiRet = OSAL_ERRNO_INVAL;
    }

    return uiRet;
}

static inline uint32_t OSAL_LockDestroy(OSAL_LOCK_T *pOsalLock)
{
    uint32_t uiRet = OSAL_OK;

    if (pthread_mutex_destroy(&pOsalLock->mutex) != OSAL_OK) {
        uiRet = OSAL_ERRNO_FAULT;
    }
    return uiRet;
}

#define OSAL_LockLock(pOsalLock) (uint32_t)pthread_mutex_lock(&(pOsalLock)->mutex)

#define OSAL_LockUnLock(pOsalLock, uiRetValue)           \
    do {                                                 \
        (void)pthread_mutex_unlock(&(pOsalLock)->mutex); \
        (void)(uiRetValue);                              \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif
