
/*
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved
*
* This file is part of VL53L1 Core and is dual licensed,
* either 'STMicroelectronics
* Proprietary license'
* or 'BSD 3-clause "New" or "Revised" License' , at your option.
*
********************************************************************************
*
* 'STMicroelectronics Proprietary license'
*
********************************************************************************
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms at www.st.com/sla0044
*
* STMicroelectronics confidential
* Reproduction and Communication of this document is strictly prohibited unless
* specifically authorized in writing by STMicroelectronics.
*
*
********************************************************************************
*
* Alternatively, VL53L1 Core may be distributed under the terms of
* 'BSD 3-clause "New" or "Revised" License', in which case the following
* provisions apply instead of the ones
* mentioned above :
*
********************************************************************************
*
* License terms: BSD 3-clause "New" or "Revised" License.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
********************************************************************************
*
*/



































#include "vl53l1_ll_def.h"
#include "vl53l1_ll_device.h"
#include "vl53l1_register_structs.h"
#include "vl53l1_hist_structs.h"
#include "vl53l1_nvm_structs.h"
#include "vl53l1_nvm.h"
#include "vl53l1_core.h"
#include "vl53l1_api_debug.h"


#define LOG_FUNCTION_START(fmt, ...) \
	_LOG_FUNCTION_START(VL53L1_TRACE_MODULE_CORE, fmt, ##__VA_ARGS__)
#define LOG_FUNCTION_END(status, ...) \
	_LOG_FUNCTION_END(VL53L1_TRACE_MODULE_CORE, status, ##__VA_ARGS__)
#define LOG_FUNCTION_END_FMT(status, fmt, ...) \
	_LOG_FUNCTION_END_FMT(VL53L1_TRACE_MODULE_CORE, status, \
	fmt, ##__VA_ARGS__)

#define trace_print(level, ...) \
	_LOG_TRACE_PRINT(trace_flags, \
	level, VL53L1_TRACE_FUNCTION_NONE, ##__VA_ARGS__)
extern int memcpy_s(void *dest, size_t destMax, const void *src, size_t count);


VL53L1_Error VL53L1_decode_calibration_data_buffer(
	uint16_t                         buf_size,
	uint8_t                          *pbuffer,
	struct VL53L1_calibration_data_t *pdata)
{
	VL53L1_Error  status = VL53L1_ERROR_NONE;

	LOG_FUNCTION_START("");

	if (sizeof(struct VL53L1_calibration_data_t) > buf_size)
		return VL53L1_ERROR_COMMS_BUFFER_TOO_SMALL;

	memcpy_s(pdata, sizeof(struct VL53L1_calibration_data_t),
		pbuffer, sizeof(struct VL53L1_calibration_data_t));

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_get_nvm_debug_data(
	VL53L1_DEV                          Dev,
	struct VL53L1_decoded_nvm_data_t    *pdata)
{





	VL53L1_Error  status = VL53L1_ERROR_NONE;

	LOG_FUNCTION_START("");

	status = VL53L1_read_nvm(Dev, 0, pdata);


	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_get_histogram_debug_data(
	VL53L1_DEV                          Dev,
	struct VL53L1_histogram_bin_data_t  *pdata)
{





	VL53L1_Error  status = VL53L1_ERROR_NONE;

	struct VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");



	memcpy_s(
		pdata,sizeof(struct VL53L1_histogram_bin_data_t),
		&(pdev->hist_data),
		sizeof(struct VL53L1_histogram_bin_data_t));

	LOG_FUNCTION_END(status);

	return status;
}





VL53L1_Error VL53L1_get_additional_data(
	VL53L1_DEV                       Dev,
	struct VL53L1_additional_data_t  *pdata)
{





	VL53L1_Error  status = VL53L1_ERROR_NONE;

	struct VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

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
		VL53L1_get_histogram_debug_data(
			Dev,
			&(pdata->VL53L1_PRM_00009));

	LOG_FUNCTION_END(status);

	return status;
}





VL53L1_Error VL53L1_get_xtalk_debug_data(
	VL53L1_DEV                          Dev,
	struct VL53L1_xtalk_debug_data_t    *pdata)
{





	VL53L1_Error  status = VL53L1_ERROR_NONE;

	struct VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");



	memcpy_s(
		&(pdata->customer),
		sizeof(struct VL53L1_customer_nvm_managed_t),
		&(pdev->customer),
		sizeof(struct VL53L1_customer_nvm_managed_t));

	memcpy_s(
		&(pdata->xtalk_cfg),
		sizeof(struct VL53L1_xtalk_config_t),
		&(pdev->xtalk_cfg),
		sizeof(struct VL53L1_xtalk_config_t));

	memcpy_s(
		&(pdata->hist_data),
		sizeof(struct VL53L1_histogram_bin_data_t),
		&(pdev->hist_data),
		sizeof(struct VL53L1_histogram_bin_data_t));

	memcpy_s(
		&(pdata->xtalk_shapes),
		sizeof(struct VL53L1_xtalk_histogram_data_t),
		&(pdev->xtalk_shapes),
		sizeof(struct VL53L1_xtalk_histogram_data_t));

	memcpy_s(
		&(pdata->xtalk_results),
		sizeof(struct VL53L1_xtalk_range_results_t),
		&(pdev->xtalk_results),
		sizeof(struct VL53L1_xtalk_range_results_t));

	LOG_FUNCTION_END(status);

	return status;
}


VL53L1_Error VL53L1_get_offset_debug_data(
	VL53L1_DEV                          Dev,
	struct VL53L1_offset_debug_data_t   *pdata)
{





	VL53L1_Error  status = VL53L1_ERROR_NONE;

	struct VL53L1_LLDriverData_t *pdev = VL53L1DevStructGetLLDriverHandle(Dev);

	LOG_FUNCTION_START("");



	memcpy_s(
		&(pdata->customer),
		sizeof(struct VL53L1_customer_nvm_managed_t),
		&(pdev->customer),
		sizeof(struct VL53L1_customer_nvm_managed_t));

	memcpy_s(
		&(pdata->fmt_dmax_cal),
		sizeof(struct VL53L1_dmax_calibration_data_t),
		&(pdev->fmt_dmax_cal),
		sizeof(struct VL53L1_dmax_calibration_data_t));

	memcpy_s(
		&(pdata->cust_dmax_cal),
		sizeof(struct VL53L1_dmax_calibration_data_t),
		&(pdev->cust_dmax_cal),
		sizeof(struct VL53L1_dmax_calibration_data_t));

	memcpy_s(
		&(pdata->add_off_cal_data),
		sizeof(struct VL53L1_additional_offset_cal_data_t),
		&(pdev->add_off_cal_data),
		sizeof(struct VL53L1_additional_offset_cal_data_t));

	memcpy_s(
		&(pdata->offset_results),
		sizeof(struct VL53L1_offset_range_results_t),
		&(pdev->offset_results),
		sizeof(struct VL53L1_offset_range_results_t));

	LOG_FUNCTION_END(status);

	return status;
}




