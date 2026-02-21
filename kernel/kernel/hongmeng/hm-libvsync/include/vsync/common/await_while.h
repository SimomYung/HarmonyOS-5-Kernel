/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Author: Huawei Dresden Research Center
 * Create: Wed 22 Jan 2020 01:00:33 PM CET
 */
#ifndef VSYNC_AWAIT_WHILE_H
#define VSYNC_AWAIT_WHILE_H

#if defined(VSYNC_VERIFICATION) && !defined(VSYNC_DISABLE_SPIN_ANNOTATION)
#include <vsync/common/internal/await_while.h>
#else
#define await_while(cond) while (cond)
#define await_do do
#define while_await(cond) while (cond)
#define await_break break
#endif

#endif /* VSYNC_AWAIT_WHILE_H */
