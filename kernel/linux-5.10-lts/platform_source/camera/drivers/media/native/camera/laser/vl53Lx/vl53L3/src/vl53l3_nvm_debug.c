
// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved

 This file is part of VL53L3 and is dual licensed,
 either GPL-2.0+
 or 'BSD 3-clause "New" or "Revised" License' , at your option.
 ******************************************************************************
 */




#include "vl53l3_ll_def.h"
#include "vl53l3_platform.h"
#include "vl53l3_platform_log.h"
#include "vl53l3_register_map.h"
#include "vl53l3_api_debug.h"
#include "vl53l3_nvm_structs.h"
#include "vl53l3_nvm_debug.h"

#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L3_TRACE_MODULE_NVM, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L3_TRACE_MODULE_NVM, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L3_TRACE_MODULE_NVM,\
		status, fmt, ##__VA_ARGS__)

#define trace_print(level, ...) \
	_LOG_TRACE_PRINT(trace_flags, \
	level, VL53L3_TRACE_FUNCTION_NONE, ##__VA_ARGS__)



