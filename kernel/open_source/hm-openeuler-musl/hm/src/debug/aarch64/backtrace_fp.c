/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of backtrace with fp mode
 * Author: Huawei Rtos
 * Create: Fri Aug 26 14:15:57 2022
 */
#include <execinfo.h>

struct frame {
    struct frame *next;
    void *return_address;
};

int backtrace(void **buffer, int size)
{
    if (!buffer || size <= 0) {
        return 0;
    }

    unsigned long fp = 0;
    unsigned long lr = 0;
    __asm__ __volatile__("mov %0, fp" : "=r"(fp));
    __asm__ __volatile__("mov %0, lr" : "=r"(lr));
    buffer[0] = (void *)lr;
    int count = 1;

	/* The cur will be backtracked to NULL,
		because fp and lr are set to zero in the function _start. */	
    struct frame *cur = (struct frame *)fp;
    while (cur && count < size) {
        buffer[count] = cur->return_address;
        cur = cur->next;
        count++;
    }
    return count;
}

