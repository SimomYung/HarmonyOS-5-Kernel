/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: pmic hrst regs functions headfile
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __PMIC_HRST_H__
#define __PMIC_HRST_H__

#include "pmic_interface.h"

/***** common *****/
#define COUL_HARDWARE_SAVE_LAST_SOC              PMIC_HRST_REG6_ADDR(0) /* last soc 0-6bit */
#define COUL_HARDWARE_SAVE_LAST_SOC_VAILD        PMIC_HRST_REG6_ADDR(0) /* last soc vaild 7bit */
#define SAVE_LAST_SOC (BIT(6) | BIT(5) | BIT(4) | BIT(3) | \
	BIT(2) | BIT(1) | BIT(0))
#define SAVE_LAST_SOC_FLAG                       BIT(7)
#define CLEAR_LAST_SOC_FLAG                      0x7F

/***** batt 0 *****/
#define COUL_HARDWARE_BATTERY_MOVE_ADDR_0        PMIC_HRST_REG0_ADDR(0)
#define BATTERY_MOVE_MAGIC_NUM                   0xc3
#define BATTERY_PLUGOUT_SHUTDOWN_MAGIC_NUM       0x18

#define COUL_HARDWARE_OCV_TYPE_0                 PMIC_HRST_REG1_ADDR(0) /* use bit 7 */
#define COUL_HARDWARE_OCV_CHOOSE_0               PMIC_HRST_REG1_ADDR(0) /* use bit 5 */
#define COUL_HARDWARE_TEMP_PROTECT_0             PMIC_HRST_REG1_ADDR(0) /* use bit 4 */
#define COUL_HARDWARE_DELTA_RC_SCENE_0           PMIC_HRST_REG1_ADDR(0) /* use bit 3 */
#define COUL_HARDWARE_NV_READ_SUCCESS_0          PMIC_HRST_REG1_ADDR(0) /* use bit 1 */
#define COUL_HARDWARE_NV_SAVE_SUCCESS_0          PMIC_HRST_REG1_ADDR(0) /* use bit 0 */

#define OCV_TYPE_MASK                            BIT(7)
#define USE_SAVED_OCV_FLAG                       BIT(5)
#define TEMP_PROTECT_BITMASK                     BIT(4)
#define DELTA_RC_SCENE_BITMASK                   BIT(3)
#define PD_BY_OCV_WRONG_BIT                      BIT(2)
#define NV_READ_BITMASK                          BIT(1)
#define NV_SAVE_BITMASK                          BIT(0)

#define COUL_HARDWARE_SAVE_OCV_ADDR_0            PMIC_HRST_REG2_ADDR(0) /* use 2byte,reserved3 and reserved4 */
#define COUL_HARDWARE_SAVE_OCV_RESERVED_0        PMIC_HRST_REG3_ADDR(0)
#define INVALID_TO_UPDATE_FCC_SHIFT              15
#define INVALID_TO_UPDATE_FCC                    (1 << INVALID_TO_UPDATE_FCC_SHIFT)

#define COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_0       PMIC_HRST_REG4_ADDR(0) /* OCV TEMP saved use 2bytes */
#define COUL_HARDWARE_SAVE_OCV_TEMP_RESERVED_0   PMIC_HRST_REG5_ADDR(0)

#define COUL_HARDWARE_OCV_LEVEL_ADDR_0           PMIC_HRST_REG7_ADDR(0) /* last soc 2-5bit */
#define SAVE_OCV_LEVEL                           (BIT(5) | BIT(4) | BIT(3) | BIT(2))
#define OCV_LEVEL_SHIFT                          2

#define COUL_HARDWARE_ECO_OCV_ADDR_0             PMIC_HRST_REG7_ADDR(0) /* 6-7bit */
#define EN_ECO_SAMPLE                            BIT(6)
#define CLR_ECO_SAMPLE                           BIT(7)
#define EN_ECO_SAMPLE_FLAG                       1
#define OUT_ECO_SAMPLE_FLAG                      0

#define DRAINED_BATTERY_FLAG_ADDR_0              PMIC_HRST_REG12_ADDR(0)
#define DRAINED_BATTERY_FLAG_BIT                 BIT(0)
#define COUL_HARDWARE_BOOT_OCV_ADDR_0            PMIC_HRST_REG12_ADDR(0) /* bit 1 */
#define EN_BOOT_OCV_SAMPLE                       BIT(1)

/***** batt 1 *****/
#define COUL_HARDWARE_BATTERY_MOVE_ADDR_1        PMIC_HRST_REG24_ADDR(0)

#define COUL_HARDWARE_OCV_TYPE_1                 PMIC_HRST_REG25_ADDR(0) /* use bit 7 */
#define COUL_HARDWARE_OCV_CHOOSE_1               PMIC_HRST_REG25_ADDR(0) /* use bit 5 */
#define COUL_HARDWARE_TEMP_PROTECT_1             PMIC_HRST_REG25_ADDR(0) /* use bit 4 */
#define COUL_HARDWARE_DELTA_RC_SCENE_1           PMIC_HRST_REG25_ADDR(0) /* use bit 3 */
#define COUL_HARDWARE_NV_READ_SUCCESS_1          PMIC_HRST_REG25_ADDR(0) /* use bit 1 */
#define COUL_HARDWARE_NV_SAVE_SUCCESS_1          PMIC_HRST_REG25_ADDR(0) /* use bit 0 */

#define COUL_HARDWARE_SAVE_OCV_ADDR_1            PMIC_HRST_REG26_ADDR(0) /* use 2byte,reserved3 and reserved4 */
#define COUL_HARDWARE_SAVE_OCV_RESERVED_1        PMIC_HRST_REG27_ADDR(0)

#define COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_1       PMIC_HRST_REG28_ADDR(0) /* OCV TEMP saved use 2bytes */
#define COUL_HARDWARE_SAVE_OCV_TEMP_RESERVED_1   PMIC_HRST_REG29_ADDR(0)

#define COUL_HARDWARE_OCV_LEVEL_ADDR_1           PMIC_HRST_REG30_ADDR(0) /* last soc 2-5bit */
#define COUL_HARDWARE_ECO_OCV_ADDR_1             PMIC_HRST_REG30_ADDR(0) /* 6-7bit */

#define DRAINED_BATTERY_FLAG_ADDR_1              PMIC_HRST_REG31_ADDR(0)
#define COUL_HARDWARE_BOOT_OCV_ADDR_1            PMIC_HRST_REG31_ADDR(0) /* bit 1 */

/***** batt 2 *****/
#ifdef PMIC_HRST_REG39_ADDR
#define COUL_HARDWARE_BATTERY_MOVE_ADDR_2        PMIC_HRST_REG32_ADDR(0)

#define COUL_HARDWARE_OCV_TYPE_2                 PMIC_HRST_REG33_ADDR(0) /* use bit 7 */
#define COUL_HARDWARE_OCV_CHOOSE_2               PMIC_HRST_REG33_ADDR(0) /* use bit 5 */
#define COUL_HARDWARE_TEMP_PROTECT_2             PMIC_HRST_REG33_ADDR(0) /* use bit 4 */
#define COUL_HARDWARE_DELTA_RC_SCENE_2           PMIC_HRST_REG33_ADDR(0) /* use bit 3 */
#define COUL_HARDWARE_NV_READ_SUCCESS_2          PMIC_HRST_REG33_ADDR(0) /* use bit 1 */
#define COUL_HARDWARE_NV_SAVE_SUCCESS_2          PMIC_HRST_REG33_ADDR(0) /* use bit 0 */

#define COUL_HARDWARE_SAVE_OCV_ADDR_2            PMIC_HRST_REG34_ADDR(0) /* use 2byte,reserved3 and reserved4 */
#define COUL_HARDWARE_SAVE_OCV_RESERVED_2        PMIC_HRST_REG35_ADDR(0)

#define COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_2       PMIC_HRST_REG36_ADDR(0) /* OCV TEMP saved use 2bytes */
#define COUL_HARDWARE_SAVE_OCV_TEMP_RESERVED_2   PMIC_HRST_REG37_ADDR(0)

#define COUL_HARDWARE_OCV_LEVEL_ADDR_2           PMIC_HRST_REG38_ADDR(0) /* last soc 2-5bit */
#define COUL_HARDWARE_ECO_OCV_ADDR_2             PMIC_HRST_REG38_ADDR(0) /* 6-7bit */

#define DRAINED_BATTERY_FLAG_ADDR_2              PMIC_HRST_REG39_ADDR(0)
#define COUL_HARDWARE_BOOT_OCV_ADDR_2            PMIC_HRST_REG39_ADDR(0) /* bit 1 */

#else
#define INVALID_ADDR                             0
#define COUL_HARDWARE_BATTERY_MOVE_ADDR_2        INVALID_ADDR

#define COUL_HARDWARE_OCV_TYPE_2                 INVALID_ADDR /* use bit 7 */
#define COUL_HARDWARE_OCV_CHOOSE_2               INVALID_ADDR /* use bit 5 */
#define COUL_HARDWARE_TEMP_PROTECT_2             INVALID_ADDR /* use bit 4 */
#define COUL_HARDWARE_DELTA_RC_SCENE_2           INVALID_ADDR /* use bit 3 */
#define COUL_HARDWARE_NV_READ_SUCCESS_2          INVALID_ADDR /* use bit 1 */
#define COUL_HARDWARE_NV_SAVE_SUCCESS_2          INVALID_ADDR /* use bit 0 */

#define COUL_HARDWARE_SAVE_OCV_ADDR_2            INVALID_ADDR /* use 2byte,reserved3 and reserved4 */
#define COUL_HARDWARE_SAVE_OCV_RESERVED_2        INVALID_ADDR

#define COUL_HARDWARE_SAVE_OCV_TEMP_ADDR_2       INVALID_ADDR /* OCV TEMP saved use 2bytes */
#define COUL_HARDWARE_SAVE_OCV_TEMP_RESERVED_2   INVALID_ADDR

#define COUL_HARDWARE_OCV_LEVEL_ADDR_2           INVALID_ADDR /* last soc 2-5bit */
#define COUL_HARDWARE_ECO_OCV_ADDR_2             INVALID_ADDR /* 6-7bit */

#define DRAINED_BATTERY_FLAG_ADDR_2              INVALID_ADDR
#define COUL_HARDWARE_BOOT_OCV_ADDR_2            INVALID_ADDR /* bit 1 */

#endif

void set_hrst_index(int batt_index, unsigned int hrst_index);
void coul_set_nv_save_flag(int batt_index, int nv_flag);
int coul_get_nv_read_flag(int batt_index);
int coul_get_use_saved_ocv_flag(int batt_index);
int coul_get_delta_rc_ignore_flag(int batt_index);
int coul_is_battery_moved(int batt_index);
void coul_set_battery_move_magic(int batt_index, int move_flag);
void coul_save_ocv_level(int batt_index, u8 level);
void coul_get_ocv_level(int batt_index, u8 *level);
int coul_get_drained_battery_flag(int batt_index);
void coul_clear_drained_battery_flag(int batt_index);
void coul_set_bootocv_sample(int batt_index, u8 set_val);
void coul_save_ocvreg(int batt_index, unsigned short ocvreg);
void coul_save_ocvtype(int batt_index, unsigned int ocv_type);
void coul_save_fcc_flag_reg(int batt_index, int invalid_fcc_up_flag);
int coul_get_fcc_flag_reg(int batt_index);
void coul_clear_ocv(int batt_index);
short coul_get_ocvreg(int batt_index);
void coul_save_ocv_temp(int batt_index, short ocv_temp);
void coul_clear_ocv_temp(int batt_index);
short coul_get_ocv_temp(int batt_index);
unsigned int coul_get_ocv_type(int batt_index);
void coul_hardware_get_last_soc(short *soc);
#endif

