/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Asan test header
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 03 20:27:35 2022
 */

#ifndef ASAN_TEST_HEADER
#define ASAN_TEST_HEADER

#ifdef __HOST_LLT__
#define INNER
#define __builtin_trap() return
#else
#define INNER static
#endif /* __HOST_LLT__ */

#endif // ASAN_TEST_HEADER