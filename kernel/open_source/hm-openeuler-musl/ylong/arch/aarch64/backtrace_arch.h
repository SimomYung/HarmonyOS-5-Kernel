/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: execinfo header
 */
#ifndef _BACKTRACE_ARCH_H_
#define _BACKTRACE_ARCH_H_

#include <stdint.h>
#include <pthread.h>

#define AARCH_GET_FP()                                      \
    ({                                                      \
        uintptr_t _regFp;                                   \
        __asm__ __volatile__("mov %0, X29" : "=r"(_regFp)); \
        _regFp;                                             \
    })

#define AARCH_GET_LR()                                      \
    ({                                                      \
        uintptr_t _regFp;                                   \
        __asm__ __volatile__("mov %0, X30" : "=r"(_regFp)); \
        _regFp;                                             \
    })

int backtrace_aarch(void **buffer, int size)
{
    if (buffer == NULL || size < 1) {
        return 0;
    }

    pthread_attr_t attr;
    size_t stackSize;
    uintptr_t stackBegin, stackEnd;

    (void)pthread_getattr_np(pthread_self(), &attr);
    (void)pthread_attr_getstack(&attr, (void **)&stackBegin, &stackSize);
    stackEnd = stackBegin + stackSize;

    uintptr_t tmpFp;
    uintptr_t backLr = AARCH_GET_LR();
    uintptr_t backFp = AARCH_GET_FP();
    uintptr_t count = 0;
    uintptr_t *_buffer = (uintptr_t *)buffer;

    while ((backFp >= stackBegin) && (backFp < stackEnd)) {
        tmpFp = backFp;
        backFp = *((uintptr_t *)(tmpFp));
        backLr = *((uintptr_t *)(tmpFp + sizeof(char *)));

        if (backLr == 0) {
            break;
        }

        _buffer[count++] = backLr;

        if (count == size) {
            break;
        }
    }
    return count;
}

#endif
