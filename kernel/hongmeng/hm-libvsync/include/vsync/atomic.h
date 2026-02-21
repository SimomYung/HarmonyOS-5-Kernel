/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: Backwards compatibility file
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_ATOMIC_H
#define VSYNC_ATOMIC_H
/*******************************************************************************
 * @file atomic.h
 * @brief Rich interface of atomic operations and fences.
 *
 * Atomics are implemented in custom assembly or compiler builtins.
 * See atomic/core.h for the documentation of the core implementation.
 * interface.
 *
 * This file also includes the backwards compatibility layer. For details, see
 * atomic/compat.h.
 ******************************************************************************/

#include <vsync/atomic/await.h>
#include <vsync/atomic/core.h>
#include <vsync/atomic/compat.h>

#endif
