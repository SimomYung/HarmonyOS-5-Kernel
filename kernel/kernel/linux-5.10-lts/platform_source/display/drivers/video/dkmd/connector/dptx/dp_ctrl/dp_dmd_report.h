/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __DP_DMD_REPORT_H__
#define __DP_DMD_REPORT_H__

enum {
	DMD_DP_DISCONNECT = 0,
	DMD_DP_CONNECT,
	DMD_DP_FAILED,
};

enum {
	DMD_DP_SUCCESS = 0,
	DMD_DP_FAILED_COMMON,
	DMD_DP_FAILED_AUXERROR,
	DMD_DP_FAILED_LTERROR,
	DMD_DP_FAILED_SAFEMODE,
	DMD_DP_FAILED_EDIDERROR,
	DMD_DP_FAILED_TIMINGERROR,
	DMD_DP_FAILED_PSRINITERROR,
	DMD_DP_FAILED_FCTERROR,
	DMD_DP_FAILED_CFGTIMEERROR,
	DMD_DP_FAILED_DPCDDSCERROR,
};

void dp_dmd_print_dfx(struct dp_ctrl *dptx, int dfx_type, int error_type);
void dp_add_error_log(struct dp_ctrl *dptx, const char *error_str);
void dp_reset_error_log(struct dp_ctrl *dptx);

#endif