/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: hisi_pmic_eis.h
 *
 * eis scharger driver header
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef _HISI_SCHARGER_EIS_H_
#define _HISI_SCHARGER_EIS_H_

#include "hisi_scharger_eis.h"
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/basicplatform/linux/power/platform/hisi_eis.h>
#include <soc_schargerV700_interface.h>

#ifndef _STATIC
#define _STATIC static
#endif

#ifdef CONFIG_DFX_DEBUG_FS
#define eis_debug(fmt, args...) \
	pr_debug("[scharger dbg][%s-%d]:" fmt, __func__, __LINE__, ##args)
#else
#define eis_debug(fmt, args...) do {} while (0)
#endif

#define eis_inf(fmt, args...) \
	pr_info("[scharger info][%s-%d]:" fmt, __func__, __LINE__, ##args)
#define eis_err(fmt, args...) \
	pr_err("[scharger error][%s-%d]:" fmt, __func__, __LINE__, ##args)

/* n: lower bit, m: higher bit, n, and m itself is included */
#define eis_reg_getbits(x, n, m) (((x) & ~(~0U << ((m) - (n) + 1)) << (n)) >> (n))

/*
 * shift: the start bit(the lower end, start from 0), including itself;
 * mask: the length of bits(m-n+1)
 * when the set_val is out of the bit range of mask, the set val will be cut
 * from lower bit to higher end for the length of mask, then it will be
 * filled in the reg_val.
 */
#define eis_reg_setbits(reg_val, shift, mask, set_val) \
	(((reg_val)  = ((reg_val) & (~(((1U << (mask)) - 1) << (shift)))) \
	| ((unsigned int)((set_val) & ((1U << (mask)) - 1)) << (shift))))

#define RANGE_ELEMENT_COUNT     2 /* min value and max value */

/* EIS BASIC MACRO */
#define V_BAT_H_SHIFT           8
#define I_BAT_H_SHIFT           8

#define EIS_IRQ_FIFO_CLEAR      0X3C
#define LEN_I_BAT_TIMER_K       7
#define LEN_I_BAT_TIMER_J       4
#define LEN_V_BAT_GAIN          8
#define LEN_I_BAT_GAIN          5
#define LEN_RES_EXTERNAL        2
#define LEN_CURT_PULL_AMPLTD    4
#define LEN_T_ANA2ADC           2
#define LEN_ADC_AVG_TIMES       2
#define LEN_T_CHNL2START        2
#define LEN_T_WAIT              4
#define EIS_F_CUR_PULL          200
#define LEN_FREQ                12

/* sample current threshold and threshold error macro */
#define A_TO_MA                 1000
#define CUR_FREQ_THRES_H        200 /* unit: mA */
#define CUR_FREQ_THRES_L        0 /* unit: mA */
#define CUR_FREQ_THRES_H_ERR    40 /* unit: mA */
#define CUR_FREQ_THRES_L_ERR    40 /* unit: mA */
#define VOL_REF_MV              2500 /* unit: mV */
#define DIGIT2ANA_MAG           10000
#define CUR_TOTAL_STEP          4095
#define R_OHM_1M                1000
#define R_OHM_2M                2000
#define R_OHM_5M                5000
#define R_OHM_DEFAULT           (R_OHM_2M)
#define SCREEN_IS_ON            1
#define SCREEN_IS_OFF           0
#define HIGH_ERR_MAX            0XFF
#define LOW_ERR_MAX             0XFF
#define HIGH_MAX                0XFFF
#define LOW_MAX                 0XFFF
/* default value for 0xA012 */
#define T_WAIT                  0 /* unit mS */
#define T_CHNL2START            30 /* unit uS */
#define ADC_AVG_TIMES           4
#define T_ANA2ADC               50 /* unit US */
/* for vol and cur gain gear adjust */
#define CALBRAT_A_DFT           1000000 /* unit: uA */
/* eis bit */
#define EIS_IRQ_START           7
#define EIS_IRQ_END             6

/* select which battery for eis */
#define EIS_TOP_CFG_REG_0                 REG_EIS_TOP_CFG_REG_0_ADDR(REG_ANA_BASE) /* 0x970 */

/* start of adc register config */
#define EIS_VBAT_OFS_CFG_0                REG_EIS_VBAT_OFS_CFG_0_ADDR(REG_ADC_BASE) /* 0x266 */
#define EIS_CLK_SEL                       REG_EIS_CLK_SEL_ADDR(REG_ADC_BASE) /* 0x252 */
#define ADC_EIS_SEL                       REG_ADC_EIS_SEL_ADDR(REG_ADC_BASE) /* 0x251 */
#define EIS_DET_TIME_JK                   REG_EIS_DET_TIME_JK_ADDR(REG_ADC_BASE)  /* 0x259 */


/* start of ana register config */
#define EIS_TOP_CFG_REG_2                 REG_EIS_TOP_CFG_REG_2_ADDR(REG_ANA_BASE)  /* 0x972 */
#define EIS_TOP_CFG_REG_7                 REG_EIS_TOP_CFG_REG_7_ADDR(REG_ANA_BASE)  /* 0x977 */
#define EIS_TOP_CFG_REG_9                 REG_EIS_TOP_CFG_REG_9_ADDR(REG_ANA_BASE)  /* 0x979 */

/* eis reset */
#define EIS_SOFT_RST_CTRL                 REG_EIS_SOFT_RST_CTRL_ADDR(REG_GLB_BASE) /* 0x317 */
#define SOFT_RST_ON                       0xAC
#define SOFT_RST_OFF                      0x5A

/* eis enable register */
#define EIS_EN                            REG_EIS_EN_ADDR(REG_ADC_BASE) /* 0x250 */
#define EIS_HKADC_START                   REG_EIS_HKADC_START_ADDR(REG_ADC_BASE) /* 0x201 */
#define RST_DELAY_MS                      1

/* eis discharge enable register */
#define EIS_DISCHG_CFG                    REG_EIS_DISCHG_CFG_ADDR(REG_ADC_BASE)  /* 0x25B */

/* eis register time lag */
#define EIS_INIT_CFG                      REG_EIS_INIT_CFG_ADDR(REG_ADC_BASE) /* 0x263 */
#define EIS_TW_CFG                        REG_EIS_TW_CFG_ADDR(REG_ADC_BASE)  /* 0x25A */

/* start of eis current offset register */
#define EISADC_INIT_I_OFFSET_L            REG_EIS_RO_INIT_I_OFS_0_ADDR(REG_ADC_BASE) /* 0x271 */
#define EISADC_INIT_I_OFFSET_H            REG_EIS_RO_INIT_I_OFS_1_ADDR(REG_ADC_BASE) /* 0x272 */

/* start of eis freq. single period register */
#define EIS_DET_TIMER                     REG_EIS_DET_TIME_ADDR(REG_ADC_BASE) /* 0x255 */

/* start of eis empty periods register */
#define EIS_DET_TIMER_N                   REG_EIS_DET_TIME_N_ADDR(REG_ADC_BASE)   /* 0x258 */

/* start of eis sample periods register */
#define EIS_DET_TIMER_M_L                 REG_EIS_DET_TIME_M_0_ADDR(REG_ADC_BASE)  /* 0x256 */
#define EIS_DET_TIMER_M_H                 REG_EIS_DET_TIME_M_1_ADDR(REG_ADC_BASE)  /* 0x257 */

/* start of adc averaged register */
#define EIS_HKADC_AVERAGE                 REG_EIS_HKADC_AVERAGE_ADDR(REG_ADC_BASE) /* 0x25C */

/* start of eis current threshold register */
#define EIS_HTHRESHOLD_ERROR              REG_EIS_HTHRS_ERR_ADDR(REG_ADC_BASE)   /* 0x25D */
#define EIS_ICOMP_HTHRESHOLD_L            REG_EIS_ICOMP_HTHRS_0_ADDR(REG_ADC_BASE)   /* 0x25F */
#define EIS_ICOMP_HTHRESHOLD_H            REG_EIS_ICOMP_HTHRS_1_ADDR(REG_ADC_BASE)   /* 0x260 */
#define EIS_LTHRESHOLD_ERROR              REG_EIS_LTHRS_ERR_ADDR(REG_ADC_BASE)   /* 0x25E */
#define EIS_ICOMP_LTHRESHOLD_L            REG_EIS_ICOMP_LTHRS_0_ADDR(REG_ADC_BASE)   /* 0x261 */
#define EIS_ICOMP_LTHRESHOLD_H            REG_EIS_ICOMP_LTHRS_1_ADDR(REG_ADC_BASE)   /* 0x262 */

/* start of 2k memory v_bat/i_bat register */
#define SAMPLE_MEMORY_BASE                0x1000
#define EIS_VBAT_START_L                  SAMPLE_MEMORY_BASE
#define EIS_VBAT_START_H                  (SAMPLE_MEMORY_BASE + 1)

/* start of eis process status */
#define EIS_DET_FLAG                      REG_EIS_DET_FLAG_ADDR(REG_ADC_BASE)  /* 0x280 */

/* start of eis irq and mask register */
#define EIS_IRQ                           REG_IRQ_FLAG_6_ADDR(REG_IRQ_BASE)   /* 0x807 */
#define EIS_IRQ_MASK                      REG_IRQ_MASK_6_ADDR(REG_IRQ_BASE)   /* 0x814 */
#endif /* _HISI_SCHARGER_EIS_H_ */
