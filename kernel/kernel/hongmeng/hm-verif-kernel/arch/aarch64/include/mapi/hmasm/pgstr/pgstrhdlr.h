/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: arch page struct table handler for aarch64 - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 11:49:59 2022
 */

#ifndef AARCH64_INCLUDE_MAPI_HMASM_PGSTR_PGSTRHDLR_H
#define AARCH64_INCLUDE_MAPI_HMASM_PGSTR_PGSTRHDLR_H

#include <uapi/hmasm/pgstr/pgstrhdlr.h>

#define PGSTR_MARK_INTERNAL	__A64_PGSTR_MARK_INTERNAL
#define PGSTR_MARK_TABLE	__A64_PGSTR_MARK_TABLE
#define PGSTR_MARK_BLOCK	__A64_PGSTR_MARK_BLOCK
#define PGSTR_MARK_NONE		__A64_PGSTR_MARK_NONE
#define PGSTR_MARK_MASK		__A64_PGSTR_MARK_MASK

#endif
