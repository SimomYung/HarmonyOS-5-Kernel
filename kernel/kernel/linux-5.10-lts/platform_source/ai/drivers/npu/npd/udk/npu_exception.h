/*
 * npu_exception.h
 *
 * about npu exception
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */
#ifndef __NPU_EXCEPTION_H
#define __NPU_EXCEPTION_H

#include "npu_common.h"
#include "npu_except_mgr.h"

bool is_channel_id_valid(uint16_t channel_id);
bool is_exception_type_valid(uint8_t exception_type);
int npd_dump_exception_without_power_check(struct npu_exception_record *er);
#endif