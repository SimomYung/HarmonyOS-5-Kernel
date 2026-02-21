/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define headfile of tty async work functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 29 10:02:30 2020
 */

#ifndef TTY_WORKQUEUE_H
#define TTY_WORKQUEUE_H

typedef void (*tty_async_func_t) (void *data);
int tty_workq_init(void);
void tty_workq_exit(void);
void tty_async_schedule(tty_async_func_t func, void *data);

#endif
