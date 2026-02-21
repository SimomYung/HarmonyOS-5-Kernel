
// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/******************************************************************************
 * Copyright (c) 2020, STMicroelectronics - All Rights Reserved

 This file is part of VL53L3 and is dual licensed,
 either GPL-2.0+
 or 'BSD 3-clause "New" or "Revised" License' , at your option.
 ******************************************************************************
 */




#include "vl53l3_ll_def.h"
#include "vl53l3_ll_device.h"
#include "vl53l3_register_structs.h"
#include "vl53l3_hist_structs.h"
#include "vl53l3_nvm_structs.h"
#include "vl53l3_nvm.h"
#include "vl53l3_core.h"
#include "vl53l3_api_debug.h"


#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L3_TRACE_MODULE_CORE, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L3_TRACE_MODULE_CORE, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L3_TRACE_MODULE_CORE, status, \
	fmt, ##__VA_ARGS__)

#define trace_print(level, ...) \
	_LOG_TRACE_PRINT(trace_flags, \
	level, VL53L3_TRACE_FUNCTION_NONE, ##__VA_ARGS__)


VL53L3_Error VL53L3_decode_calibration_data_buffer(
	uint16_t                   buf_size,
	uint8_t                   *pbuffer,
	VL53L3_calibration_data_t *pdata)
{
	VL53L3_Error  status = VL53L3_ERROR_NONE;

	LOG_FUNCTION_START("");

	if (sizeof(VL53L3_calibration_data_t) > buf_size)
		return VL53L3_ERROR_COMMS_BUFFER_TOO_SMALL;

	memcpy(pdata, pbuffer, sizeof(VL53L3_calibration_data_t));

	LOG_FUNCTION_END(status);

	return status;
}


VL53L3_Error VL53L3_get_nvm_debug_data(
	VL53L3_DEV                          Dev,
	VL53L3_decoded_nvm_data_t          *pdata)
{


	VL53L3_Error  status = VL53L3_ERROR_NONE;

	LOG_FUNCTION_START("");

	status = VL53L3_read_nvm(Dev, 0, pdata);


	LOG_FUNCTION_END(status);

	return status;
}


VL53L3_Error VL53L3_get_histogram_debug_data(
	VL53L3_DEV                          Dev,
	VL53L3_histogram_bin_data_t        *pdata)
{


	VL53L3_Error  status = VL53L3_ERROR_NONE;

	VL53L3_LLDriverData_t *pdev = VL53L3DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");


	memcpy(
		pdata,
		&(pdev->hist_data),
		sizeof(VL53L3_histogram_bin_data_t));

	LOG_FUNCTION_END(status);

	return status;
}




VL53L3_Error VL53L3_get_additional_data(
	VL53L3_DEV                       Dev,
	VL53L3_additional_data_t        *pdata)
{


	VL53L3_Error  status = VL53L3_ERROR_NONE;

	VL53L3_LLDriverData_t *pdev = VL53L3DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");



	pdata->preset_mode             = pdev->preset_mode;
	pdata->zone_preset             = pdev->zone_preset;
	pdata->measurement_mode        = pdev->measurement_mode;
	pdata->offset_calibration_mode = pdev->offset_calibration_mode;
	pdata->offset_correction_mode  = pdev->offset_correction_mode;
	pdata->dmax_mode               = pdev->dmax_mode;

	pdata->phasecal_config_timeout_us  = pdev->phasecal_config_timeout_us;
	pdata->mm_config_timeout_us        = pdev->mm_config_timeout_us;
	pdata->range_config_timeout_us     = pdev->range_config_timeout_us;
	pdata->inter_measurement_period_ms = pdev->inter_measurement_period_ms;
	pdata->dss_config__target_total_rate_mcps =
			pdev->dss_config__target_total_rate_mcps;



	status =
		VL53L3_get_histogram_debug_data(
			Dev,
			&(pdata->VL53L3_p_010));

	LOG_FUNCTION_END(status);

	return status;
}




VL53L3_Error VL53L3_get_xtalk_debug_data(
	VL53L3_DEV                          Dev,
	VL53L3_xtalk_debug_data_t          *pdata)
{


	VL53L3_Error  status = VL53L3_ERROR_NONE;

	VL53L3_LLDriverData_t *pdev = VL53L3DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");


	memcpy(
		&(pdata->customer),
		&(pdev->customer),
		sizeof(VL53L3_customer_nvm_managed_t));

	memcpy(
		&(pdata->xtalk_cfg),
		&(pdev->xtalk_cfg),
		sizeof(VL53L3_xtalk_config_t));

	memcpy(
		&(pdata->hist_data),
		&(pdev->hist_data),
		sizeof(VL53L3_histogram_bin_data_t));

	memcpy(
		&(pdata->xtalk_shapes),
		&(pdev->xtalk_shapes),
		sizeof(VL53L3_xtalk_histogram_data_t));

	memcpy(
		&(pdata->xtalk_results),
		&(pdev->xtalk_results),
		sizeof(VL53L3_xtalk_range_results_t));

	LOG_FUNCTION_END(status);

	return status;
}


VL53L3_Error VL53L3_get_offset_debug_data(
	VL53L3_DEV                          Dev,
	VL53L3_offset_debug_data_t         *pdata)
{


	VL53L3_Error  status = VL53L3_ERROR_NONE;

	VL53L3_LLDriverData_t *pdev = VL53L3DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");


	memcpy(
		&(pdata->customer),
		&(pdev->customer),
		sizeof(VL53L3_customer_nvm_managed_t));

	memcpy(
		&(pdata->fmt_dmax_cal),
		&(pdev->fmt_dmax_cal),
		sizeof(VL53L3_dmax_calibration_data_t));

	memcpy(
		&(pdata->cust_dmax_cal),
		&(pdev->cust_dmax_cal),
		sizeof(VL53L3_dmax_calibration_data_t));

	memcpy(
		&(pdata->add_off_cal_data),
		&(pdev->add_off_cal_data),
		sizeof(VL53L3_additional_offset_cal_data_t));

	memcpy(
		&(pdata->offset_results),
		&(pdev->offset_results),
		sizeof(VL53L3_offset_range_results_t));

	LOG_FUNCTION_END(status);

	return status;
}



