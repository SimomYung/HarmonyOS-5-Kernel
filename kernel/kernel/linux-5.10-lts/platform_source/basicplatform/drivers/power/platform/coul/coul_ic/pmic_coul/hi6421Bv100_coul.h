/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: pmic6421Bv100 coulometer driver headfile
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

#ifndef _PMIC6421BV100_COUL_H_
#define _PMIC6421BV100_COUL_H_

#include <pmic_interface.h>

#ifndef BIT
#define BIT(x)                          (1 << (x))
#endif

#define VERSION_VAL      0x06
#define COUL_HARDWARE    0x06

#define COUL_HARDWARE_I_OUT_GATE               PMIC_I_OUT_GATE0_ADDR(0)
#define COUL_HARDWARE_I_IN_GATE                PMIC_I_IN_GATE0_ADDR(0)

#define COUL_HARDWARE_VOL_OFFSET_B_ADDR        PMIC_NP_EFUSE0_59_R_ADDR(0)
#define COUL_HARDWARE_VOL_OFFSET_A_ADDR_0      PMIC_NP_EFUSE0_59_R_ADDR(0)
#define COUL_HARDWARE_VOL_OFFSET_A_ADDR_1      PMIC_NP_EFUSE0_58_R_ADDR(0)

/* vol offset a/b value */
#define VOL_OFFSET_A_STEP               39
#define VOL_OFFSET_B_STEP               78125
#define VOL_OFFSET_A_BASE               990000
#define VOL_OFFSET_B_BASE               (-5000000)
#define VOL_OFFSET_B_VALID_MASK         0xFE
#define VOL_OFFSET_A_HIGH_VALID_MASK    0x1FE
#define VOL_OFFSET_A_LOW_VALID_MASK     0x001
#define VOL_OFFSET_A_VALID_MASK         0x1FF

#define COUL_HARDWARE_DEBUG1_REG               PMIC_CLJ_DEBUG1_ADDR(0)
#define COUL_HARDWARE_WAIT_COMP_ADDR      PMIC_CLJ_CTRL_REGS3_ADDR(0)

#define WAIT_COMP_EN_SHIFT              PMIC_CLJ_CTRL_REGS3_wait_comp_en_START
#define WAIT_COMP_EN_MASK               (1 << WAIT_COMP_EN_SHIFT)

#define ECO_DELAY_EN_SHIFT              PMIC_CLJ_CTRL_REGS3_eco_delay_en_START
#define ECO_DELAY_EN_MASK               (1 << ECO_DELAY_EN_SHIFT)
#define ECO_DELAY_SEL_SHIFT         PMIC_CLJ_CTRL_REGS3_coul_eco_dly_sel_START
#define ECO_DELAY_SEL_MASK              (0x03 << ECO_DELAY_SEL_SHIFT)

#define ECO_DELAY_500MS                 0
#define ECO_DELAY_2S                    1
#define ECO_DELAY_5S                    2
#define ECO_DELAY_10S                   3

#define COUL_HARDWARE_TEMP_CTRL           PMIC_COUL_TEMP_CTRL_ADDR(0)
#define TEMP_EN                         BIT(0)
#define TEMP_RDY                        BIT(1)
#define VOUT_RDY                        BIT(2)

#define COUL_HARDWARE_TEMP_DATA           PMIC_TEMP0_RDATA_ADDR(0)
#define COUL_HARDWARE_OCV_TEMP_DATA       PMIC_OCV_TEMP0_ADDR(0)
#define COUL_HARDWARE_ECOOUT_TEMP_DATA    PMIC_ECO_OUT_TEMP_0_ADDR(0)

#define COUL_HARDWARE_SOH_TBAT_DATA_BASE       PMIC_ACRADC_TBAT_DATA_L_ADDR(0)
#define COUL_HARDWARE_SOH_TDIE_DATA_BASE       PMIC_ACRADC_TDIE_DATA_L_ADDR(0)

#define COUL_HARDWARE_SOH_EN_BASE              PMIC_SOH_EN_ADDR(0)

#define COUL_HARDWARE_CURRENT                  PMIC_CURRENT_0_ADDR(0)
#define COUL_HARDWARE_V_OUT                    PMIC_V_OUT_0_ADDR(0)
#define COUL_HARDWARE_CL_OUT_BASE              PMIC_CL_OUT0_ADDR(0)
#define COUL_HARDWARE_CL_IN_BASE               PMIC_CL_IN0_ADDR(0)
#define COUL_HARDWARE_CHG_TIMER_BASE           PMIC_CHG_TIMER0_ADDR(0)
#define COUL_HARDWARE_LOAD_TIMER_BASE          PMIC_LOAD_TIMER0_ADDR(0)
#define COUL_HARDWARE_CL_INT_BASE              PMIC_CL_INT0_ADDR(0)
#define COUL_HARDWARE_VOL_INT_BASE             PMIC_V_INT0_ADDR(0)
#define COUL_HARDWARE_OFFSET_CURRENT           PMIC_OFFSET_CURRENT0_ADDR(0)
#define COUL_HARDWARE_OFFSET_VOLTAGE           PMIC_OFFSET_VOLTAGE0_ADDR(0)
#define COUL_HARDWARE_OCV_VOLTAGE_BASE         PMIC_OCV_VOLTAGE0_ADDR(0)
#define COUL_HARDWARE_OCV_CURRENT_BASE         PMIC_OCV_CURRENT0_ADDR(0)
#define COUL_HARDWARE_ECO_OUT_CLIN_REG_BASE    PMIC_ECO_OUT_CLIN_0_ADDR(0)
#define COUL_HARDWARE_ECO_OUT_CLOUT_REG_BASE   PMIC_ECO_OUT_CLOUT_0_ADDR(0)

#define COUL_HARDWARE_VOL_FIFO_BASE            PMIC_V_PRE0_OUT0_ADDR(0)
#define COUL_HARDWARE_CUR_FIFO_BASE            PMIC_CURRENT_PRE0_OUT0_ADDR(0)
#define COUL_HAADWARE_V_DROP_PRE0_OUT0_BASE    PMIC_V_DROP_PRE0_OUT0_ADDR(0)

#define COUL_HARDWARE_ECO_VOL_FIFO_BASE        PMIC_V_OCV_PRE1_OUT0_ADDR(0)
#define COUL_HARDWARE_ECO_I_FIFO_BASE          PMIC_I_OCV_PRE1_OUT0_ADDR(0)
#define COUL_HARDWARE_ECO_TEMP_FIFO_BASE       PMIC_T_OCV_PRE1_OUT0_ADDR(0)

#define COUL_HARDWARE_COUL_ECO_MASK_0          PMIC_COUL_ECO_MASK0_ADDR(0)
#define COUL_HARDWARE_COUL_ECO_MASK_1          PMIC_COUL_ECO_MASK1_ADDR(0)

#define COUL_HARDWARE_FIFO_CLEAR               PMIC_CLJ_CTRL_REGS2_ADDR(0) /* use bit 1 */

#define COUL_HARDWARE_ECO_FIFO_CLEAR           PMIC_CLJ_CTRL_REGS4_ADDR(0)
#define ECO_FIFO_CLEAR              BIT(PMIC_CLJ_CTRL_REGS4_eco_data_clr_START)
#define ECO_FIFO_EN                 BIT(PMIC_CLJ_CTRL_REGS4_data_eco_en_START)
#define ECO_FIFO_DEPTH                  4

#define COUL_HARDWARE_OFFSET_CUR_MODIFY_BASE   PMIC_OFFSET_CURRENT_MOD_0_ADDR(0)
#define COUL_HARDWARE_OFFSET_VOL_MODIFY_BASE   PMIC_OFFSET_VOLTAGE_MOD_0_ADDR(0)

/* coul register of smartstar */
#define COUL_HARDWARE_CTRL_REG            PMIC_CLJ_CTRL_REG_ADDR(0)
#define COUL_CALI_ENABLE                BIT(7)

#define REG_NUM                         3
#define CC_REG_NUM                      5

#define COUL_HARDWARE_ECO_CONFIG_ADDR     PMIC_CLJ_CTRL_REGS3_ADDR(0)

#define COUL_ECO_FLT_60MS               0
#define COUL_ECO_FLT_50MS               BIT(4)
#define COUL_ECO_FLT_100MS              BIT(5)
#define COUL_ECO_FLT_200MS              (BIT(4) | BIT(5))

#define COUL_ALL_REFLASH                0
#define COUL_ECO_REFLASH                BIT(3)
#define COUL_ECO_ENABLE                 BIT(2)
#define COUL_ECO_PMU_EN                 (BIT(0) | BIT(1))
#define COUL_ECO_DISABLE                0
#define COUL_FIFO_CLEAR                 BIT(1)
#define DEFAULT_COUL_CTRL_VAL (COUL_ECO_FLT_100MS | COUL_ALL_REFLASH | \
	COUL_ECO_DISABLE)
#define ECO_COUL_CTRL_VAL     (COUL_ECO_FLT_200MS | COUL_ECO_REFLASH | \
	COUL_ECO_PMU_EN)

#define CALI_AUTO_ONOFF_CTRL            BIT(4)
#define CALI_AUTO_TIME_15S              0
#define CALI_AUTO_TIME_60S              BIT(2)
#define CALI_AUTO_TIME_8MIN             BIT(3)
#define CALI_AUTO_TIME_32MIN            (BIT(2) | BIT(3))

#define COUL_HARDWARE_CLJ_CTRL                 PMIC_CLJ_CTRL_REGS2_ADDR(0)
/* Warning: bit change */
#define CALI_CLJ_DEFAULT_VALUE (CALI_AUTO_TIME_4MIN | CALI_AUTO_ONOFF_CTRL)
#define MASK_CALI_AUTO_OFF     (CALI_AUTO_ONOFF_CTRL)

#define COUL_CLK_MODE_ADDR              PMIC_STATUS1_ADDR(0)
#define XO32K_MODE_MSK                  BIT(2) /* tells 32k or others */
#define COUL_32K_CLK_MODE               BIT(2)
#define NO_32K_MODE                     0

#define COUL_HARDWARE_IRQ_REG             PMIC_COUL_IRQ_ADDR(0)
#define COUL_HARDWARE_IRQ_MASK_REG        PMIC_COUL_IRQ_MASK_ADDR(0)
#define COUL_CLEAR_IRQ                  0x7F
#define COUL_VBAT_DROP_MASK             BIT(6)
#define COUL_I_OUT_MASK                 BIT(5)
#define COUL_I_IN_MASK                  BIT(4)
#define COUL_VBAT_INT_MASK              BIT(3)
#define COUL_CL_IN_MASK                 BIT(2)
#define COUL_CL_OUT_MASK                BIT(1)
#define COUL_CL_INT_MASK                BIT(0)
#define COUL_INT_MASK_ALL (COUL_I_OUT_MASK | COUL_I_IN_MASK | \
	COUL_VBAT_INT_MASK | COUL_CL_IN_MASK | \
	COUL_CL_OUT_MASK | COUL_CL_INT_MASK | COUL_VBAT_DROP_MASK)
#define COUL_INT_MASK_PART (~((unsigned char)(COUL_VBAT_INT_MASK | \
	COUL_CL_IN_MASK | COUL_CL_OUT_MASK | COUL_VBAT_DROP_MASK)))


#define COUL_HARDWARE_VERSION_ADDR        PMIC_VERSION0_ADDR(0)

#define COUL_HARDWARE_STATE_REG           PMIC_STATE_TEST_ADDR(0) /* Warning: bit change */
#define COUL_WORKING                    0x5
#define COUL_CALI_ING                   0x4

#define COUL_MSTATE_MASK                0x0f

#define FIFO_DEPTH                      8
#define V_DROP_FIFO_DEPTH               3

/* register write lock/unlock */
#define COUL_HARDWARE_DEBUG_WRITE_PRO          PMIC_DEBUG_WRITE_PRO_ADDR(0)
#define COUL_WRITE_LOCK                 0x56
#define COUL_WRITE_UNLOCK               0xA9

#define SOH_EN_ADDR                     PMIC_SOH_EN_ADDR(0)
#define SOH_ADC_CTRL_ADDR               PMIC_ACRADC_CTRL_ADDR(0)
#define SOH_ADC_EN_DEFAULT              0x80

#define SOH_DIS                         0
#define SOH_EN                          1

#define SOH_ADC_CALI_DATA0_ADDR         PMIC_ACRADC_DATA_L_ADDR(0)
#define SOH_ADC_CALI_DATA1_ADDR         PMIC_ACRADC_DATA_H_ADDR(0)
#define ADC_DATA_L_SHIFT                0
#define ADC_DATA_H_SHIFT                4

#define ADC_EN_MASK     BIT(PMIC_ACRADC_CTRL_hkadc2_bypass_START)
#define ADC_EN                          0
#define ADC_DIS         BIT(PMIC_ACRADC_CTRL_hkadc2_bypass_START)
#define ADC_CHANEL_MASK                 0x1f
#define ADC_CALI_PATH1                  0x0f
#define ADC_CALI_PATH2                  0x10
#define SOH_ADC_WAIT_US                 15
#define ADC_RETRY                       100
#define SOH_ADC_START_ADDR              PMIC_ACRADC_START_ADDR(0)

#define SOH_ADC_STATUS_ADDR             PMIC_ACRCONV_STATUS_ADDR(0)
#define SOH_ADC_START                   0x1
#define SOH_ADC_READY                   1
#define ADC_CALI_A_MIN                  800
#define ADC_CALI_A_MAX                  1200
#define ADC_CALI_B_MIN                  (-100)
#define ADC_CALI_B_MAX                  100

#define DEFAULT_SOH_ADC_A               1000

#define COUL_VBAT_DROP_EN               PMIC_V_DROP_CTRL0_ADDR(0)
#define COUL_VBAT_DROP_EN_SHIFT         PMIC_V_DROP_CTRL0_vbat_drop_det_en_START
#define COUL_VBAT_DROP_EN_MASK_NUM      1
#define COUL_VBAT_DROP_TH               PMIC_V_DROP_CTRL0_ADDR(0)
#define COUL_VBAT_DROP_TH_SHIFT         PMIC_V_DROP_CTRL0_vbat_drop_th_START
#define COUL_VBAT_DROP_TH_MASK_NUM      3
#define COUL_VBAT_DROP_VTH              150
#define COUL_VBAT_DROP_TH_GAP           50

#define REGVAL2PHYS_COEF                1293

#endif
