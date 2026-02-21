/*
 * hw_pmic_dmd.h
 *
 * pmic dmd driver header file
 *
 * Copyright (C) 2024-2024 Huawei Technology Co., Ltd.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef _KERNEL_PMIC_DMD_H
#define _KERNEL_PMIC_DMD_H

#include "securec.h"
#include "cam_log.h"
#include <dsm/dsm_pub.h>

#define PFX "[pmic_dmd]"
#define dmd_log_inf(fmt, args...) pr_info(PFX "%s line:%d " fmt, __func__, __LINE__, ##args)
#define dmd_log_err(fmt, args...) pr_err(PFX "%s line:%d " fmt, __func__, __LINE__, ##args)

/* camera_pmic */
#define DSM_CAMPMIC_I2C_ERROR_NO                   927010000
#define DSM_CAMPMIC_ENABLE_REEOR_NO                927010001
#define DSM_CAMPMIC_OVER_CURRENT_ERROR_NO          927010002
#define DSM_CAMPMIC_TSD_ERROR_NO                   927010003
#define DSM_CAMPMIC_UNDER_VOLTAGE_LOCKOUT_ERROR_NO 927010004

int hw_pmic_dmd(int err_no, int val);

#endif