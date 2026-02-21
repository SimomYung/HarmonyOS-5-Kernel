/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: execinfo header
 */
#ifndef _EXECINFOX_H_
#define _EXECINFOX_H_

int backtrace(void **buffer, int size);

char **backtrace_symbols(void * const * buffer, int size);

#endif
