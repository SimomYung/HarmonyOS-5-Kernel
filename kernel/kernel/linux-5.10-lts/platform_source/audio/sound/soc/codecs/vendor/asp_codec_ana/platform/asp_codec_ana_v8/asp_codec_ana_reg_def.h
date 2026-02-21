/*
 * asp_codec ana v8 reg definition.
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#ifndef __ASP_CODEC_ANA_V8_REG_DEF_H__
#define __ASP_CODEC_ANA_V8_REG_DEF_H__

#define PAGE_VIRCODEC_ANA          0x9000
#define PAGE_VIRCODEC_ANA_END      (PAGE_VIRCODEC_ANA + VIR_ANA_REG_CNT - 1)
#define PAGE_ANACODEC              0xB000
#define PAGE_ANACODEC_END          0xB04c
#define PAGE_ANACTRL               0x0
#define PAGE_ANACTRL_END           0x1277
#define PAGE_ANAHKADC              0x4000
#define PAGE_ANAHKADC_END          0x4010

/* ANA_CODEC Base address of Module's Register */
#define CODEC_ANA_V8_BASE                       (0x0)

/******************************************************************************/
/*                      DCODEC_ANA_V8 ANA_CODEC Registers' Definitions                            */
/******************************************************************************/

#define CLK_EN_CFG_REG         (CODEC_ANA_V8_BASE + 0xB000)
#define CODEC_ANA_EN_LEN    1
#define CODEC_ANA_EN_OFFSET 6
#define SIF_EN_LEN          1
#define SIF_EN_OFFSET       5
#define ADC_MIC3_EN_LEN     1
#define ADC_MIC3_EN_OFFSET  4
#define ADCR_EN_LEN         1
#define ADCR_EN_OFFSET      3
#define ADCL_EN_LEN         1
#define ADCL_EN_OFFSET      2
#define DACL_EN_LEN         1
#define DACL_EN_OFFSET      0

#define CLK_EDGE_CFG_REG       (CODEC_ANA_V8_BASE + 0xB001)
#define ADC_MIC3_CLK_EDGE_SEL_LEN    1
#define ADC_MIC3_CLK_EDGE_SEL_OFFSET 4
#define ADCR_CLK_EDGE_SEL_LEN        1
#define ADCR_CLK_EDGE_SEL_OFFSET     3
#define ADCL_CLK_EDGE_SEL_LEN        1
#define ADCL_CLK_EDGE_SEL_OFFSET     2
#define DACL_CLK_EDGE_SEL_LEN        1
#define DACL_CLK_EDGE_SEL_OFFSET     0

#define SIF_LOOPBACK_CFG_REG   (CODEC_ANA_V8_BASE + 0xB002)
#define ADC_LOOPBACK_LEN    2
#define ADC_LOOPBACK_OFFSET 2
#define DAC_LOOPBACK_LEN    2
#define DAC_LOOPBACK_OFFSET 0

#define DAC_CHAN_CTRL_REG      (CODEC_ANA_V8_BASE + 0xB003)
#define DACL_DIN_SEL_LEN       2
#define DACL_DIN_SEL_OFFSET    2
#define DACL_DWA_EN_LEN        1
#define DACL_DWA_EN_OFFSET     0

#define ADC_CHAN_CTRL_REG      (CODEC_ANA_V8_BASE + 0xB004)
#define ADC_MIC3_DOUT_SEL_LEN    2
#define ADC_MIC3_DOUT_SEL_OFFSET 4
#define ADCR_DOUT_SEL_LEN        2
#define ADCR_DOUT_SEL_OFFSET     2
#define ADCL_DOUT_SEL_LEN        2
#define ADCL_DOUT_SEL_OFFSET     0

#define SIF_MAD_REG            (CODEC_ANA_V8_BASE + 0xB005)
#define MAD_CIC_CLKEN_LEN      1
#define MAD_CIC_CLKEN_OFFSET   2
#define SIF_MAD_ADC_EN_LEN     1
#define SIF_MAD_ADC_EN_OFFSET  1
#define SIF_MAD_EN_LEN         1
#define SIF_MAD_EN_OFFSET      0

#define SIF_MAD_CIC_GAIN_REG   (CODEC_ANA_V8_BASE + 0xB006)
#define MAD_CIC_GAIN_LEN    6
#define MAD_CIC_GAIN_OFFSET 0

#define CODEC_ANA_RW1_REG      (CODEC_ANA_V8_BASE + 0xB00C) /* vref/ctcm/ibias en register */
#define IBIAS_PD_LEN      1
#define IBIAS_PD_OFFSET   2
#define AVREF_SEL_LEN     2
#define AVREF_SEL_OFFSET  0

#define CODEC_ANA_RW2_REG      (CODEC_ANA_V8_BASE + 0xB00D) /* micpga en register */
#define MAINPGA_PD_LEN              1
#define MAINPGA_PD_OFFSET           6
#define AUXPGA_PD_LEN               1
#define AUXPGA_PD_OFFSET            5
#define MIC3PGA_PD_LEN              1
#define MIC3PGA_PD_OFFSET           4
#define MICPGA_WORK_MODE_SEL_LEN    1
#define MICPGA_WORK_MODE_SEL_OFFSET 3
#define MAINPGA_MUTE_LEN            1
#define MAINPGA_MUTE_OFFSET         2
#define AUXPGA_MUTE_LEN             1
#define AUXPGA_MUTE_OFFSET          1
#define MIC3PGA_MUTE_LEN            1
#define MIC3PGA_MUTE_OFFSET         0

#define CODEC_ANA_RW3_REG      (CODEC_ANA_V8_BASE + 0xB00E) /* adc en register */
#define ADCL_PD_LEN       1
#define ADCL_PD_OFFSET    6
#define ADCR_PD_LEN       1
#define ADCR_PD_OFFSET    5
#define ADC3_PD_LEN       1
#define ADC3_PD_OFFSET    4
#define ADCL_MUTE_LEN     1
#define ADCL_MUTE_OFFSET  2
#define ADCR_MUTE_LEN     1
#define ADCR_MUTE_OFFSET  1
#define ADC3_MUTE_LEN     1
#define ADC3_MUTE_OFFSET  0

#define CODEC_ANA_RW4_REG      (CODEC_ANA_V8_BASE + 0xB00F) /* dac en register */
#define DACS_PD_LEN       1
#define DACS_PD_OFFSET    2
#define DACL_PD_LEN       1
#define DACL_PD_OFFSET    1
#define DACR_PD_LEN       1
#define DACR_PD_OFFSET    0

#define CODEC_ANA_RW5_REG      (CODEC_ANA_V8_BASE + 0xB010) /* headphone en register */
#define CHARGEPUMP_PD_LEN    1
#define CHARGEPUMP_PD_OFFSET 7
#define POP_PULL_EN_LEN      1
#define POP_PULL_EN_OFFSET   6
#define MIXOUT_HPL_PD_LEN    1
#define MIXOUT_HPL_PD_OFFSET 5
#define MIXOUT_HPR_PD_LEN    1
#define MIXOUT_HPR_PD_OFFSET 4
#define HPL_PD_LEN           1
#define HPL_PD_OFFSET        3
#define HPR_PD_LEN           1
#define HPR_PD_OFFSET        2
#define HPL_MUTE_LEN         1
#define HPL_MUTE_OFFSET      1
#define HPR_MUTE_LEN         1
#define HPR_MUTE_OFFSET      0

#define CODEC_ANA_RW6_REG      (CODEC_ANA_V8_BASE + 0xB011) /* earphone/lineout en register */
#define LOMIXER_PD_LEN       1
#define LOMIXER_PD_OFFSET    6
#define LOUT_PD_LEN          1
#define LOUT_PD_OFFSET       5
#define LOUT_MUTE_LEN        1
#define LOUT_MUTE_OFFSET     4
#define MIXOUT_EAR_PD_LEN    1
#define MIXOUT_EAR_PD_OFFSET 3
#define EPVCM_PD_LEN         1
#define EPVCM_PD_OFFSET      2
#define EAR_PD_LEN           1
#define EAR_PD_OFFSET        1
#define EAR_MUTE_LEN         1
#define EAR_MUTE_OFFSET      0

#define CODEC_ANA_RW7_REG      (CODEC_ANA_V8_BASE + 0xB012) /* micbias en register */
#define HSMICB_PD_LEN       1
#define HSMICB_PD_OFFSET    6
#define MICB1_PD_LEN        1
#define MICB1_PD_OFFSET     5
#define MICB2_PD_LEN        1
#define MICB2_PD_OFFSET     4
#define HSMICB_DSCHG_LEN    1
#define HSMICB_DSCHG_OFFSET 2
#define MICB1_DSCHG_LEN     1
#define MICB1_DSCHG_OFFSET  1
#define MICB2_DSCHG_LEN     1
#define MICB2_DSCHG_OFFSET  0

#define CODEC_ANA_RW8_REG      (CODEC_ANA_V8_BASE + 0xB013) /* mbhd en register */
#define MBHD_COMP_PD_LEN                1
#define MBHD_COMP_PD_OFFSET             2
#define MBHD_BUFF_PD_LEN                1
#define MBHD_BUFF_PD_OFFSET             1
#define MBHD_ECO_EN_LEN                 1
#define MBHD_ECO_EN_OFFSET              0

#define CODEC_ANA_RW9_REG      (CODEC_ANA_V8_BASE + 0xB014) /* mainpga ctrl register */
#define MICPGA_UNLOCK_ENABLE_LEN    1
#define MICPGA_UNLOCK_ENABLE_OFFSET 6
#define MAINPGA_SEL_LEN             2
#define MAINPGA_SEL_OFFSET          4
#define MAINPGA_BOOST_LEN           1
#define MAINPGA_BOOST_OFFSET        3
#define MAINPGA_GAIN_LEN            3
#define MAINPGA_GAIN_OFFSET         0

#define CODEC_ANA_RW10_REG     (CODEC_ANA_V8_BASE + 0xB015) /* auxpga ctrl register */
#define DISCHARGE_DISABLE_LEN    1
#define DISCHARGE_DISABLE_OFFSET 7
#define HSMIC_VCM_PULL_EN_LEN    1
#define HSMIC_VCM_PULL_EN_OFFSET 6
#define AUXPGA_SEL_LEN           2
#define AUXPGA_SEL_OFFSET        4
#define AUXPGA_BOOST_LEN         1
#define AUXPGA_BOOST_OFFSET      3
#define AUXPGA_GAIN_LEN          3
#define AUXPGA_GAIN_OFFSET       0

#define CODEC_ANA_RW11_REG     (CODEC_ANA_V8_BASE + 0xB016) /* mic3pga ctrl register */
#define MIC3_IDEL_TONE_LEN        2
#define MIC3_IDEL_TONE_OFFSET     6
#define MIC3PGA_SEL_LEN           2
#define MIC3PGA_SEL_OFFSET        4
#define ADC3PGA_BOOST_LEN         1
#define ADC3PGA_BOOST_OFFSET      3
#define MIC3PGA_GAIN_LEN          3
#define MIC3PGA_GAIN_OFFSET       0

#define CODEC_ANA_RW12_REG     (CODEC_ANA_V8_BASE + 0xB017) /* adcl ctrl register */
#define IDEL_TONE_CTRL_LEN        2
#define IDEL_TONE_CTRL_OFFSET     6
#define ADCL_MIXIN_DACR_LEN       1
#define ADCL_MIXIN_DACR_OFFSET    5
#define ADCL_MIXIN_DACL_LEN       1
#define ADCL_MIXIN_DACL_OFFSET    4
#define ADCL_MIXIN_REV_LEN        2
#define ADCL_MIXIN_REV_OFFSET     2
#define ADCL_MIXIN_MICPGA2_LEN    1
#define ADCL_MIXIN_MICPGA2_OFFSET 1
#define ADCL_MIXIN_MICPGA1_LEN    1
#define ADCL_MIXIN_MICPGA1_OFFSET 0

#define CODEC_ANA_RW13_REG     (CODEC_ANA_V8_BASE + 0xB018) /* adcl ctrl register */
#define ADCL_DWA_BPS_LEN      1
#define ADCL_DWA_BPS_OFFSET   7
#define ADCL_CLK_DELAY_LEN    3
#define ADCL_CLK_DELAY_OFFSET 4
#define ADCL_DAC_BIAS_LEN     2
#define ADCL_DAC_BIAS_OFFSET  2
#define ADCL_FLSTN_LEN        2
#define ADCL_FLSTN_OFFSET     0

#define CODEC_ANA_RW14_REG     (CODEC_ANA_V8_BASE + 0xB019) /* adcr ctrl register */
#define ADCR_IDEL_TONE_LEN        2
#define ADCR_IDEL_TONE_OFFSET     6
#define ADCR_MIXIN_DACR_LEN       1
#define ADCR_MIXIN_DACR_OFFSET    5
#define ADCR_MIXIN_DACL_LEN       1
#define ADCR_MIXIN_DACL_OFFSET    4
#define ADCR_MIXIN_MICPGA1_LEN    1
#define ADCR_MIXIN_MICPGA1_OFFSET 1
#define ADCR_MIXIN_MICPGA2_LEN    1
#define ADCR_MIXIN_MICPGA2_OFFSET 0

#define CODEC_ANA_RW15_REG     (CODEC_ANA_V8_BASE + 0xB01A) /* adcr ctrl register */
#define ADCR_DWA_BPS_LEN      1
#define ADCR_DWA_BPS_OFFSET   7
#define ADCR_CLK_DELAY_LEN    3
#define ADCR_CLK_DELAY_OFFSET 4
#define ADCR_DAC_BIAS_LEN     2
#define ADCR_DAC_BIAS_OFFSET  2
#define ADCR_FLSTN_LEN        2
#define ADCR_FLSTN_OFFSET     0

#define CODEC_ANA_RW16_REG     (CODEC_ANA_V8_BASE + 0xB01B) /* adc3 ctrl register */
#define ADC3_MIXIN_ULTRA_LEN      1
#define ADC3_MIXIN_ULTRA_OFFSET   5
#define ADC3_MIXIN_DACL_LEN       1
#define ADC3_MIXIN_DACL_OFFSET    4
#define ADC3_MIXIN_MICPGA1_LEN    1
#define ADC3_MIXIN_MICPGA1_OFFSET 1
#define ADC3_MIXIN_MICPGA3_LEN    1
#define ADC3_MIXIN_MICPGA3_OFFSET 0

#define CODEC_ANA_RW17_REG     (CODEC_ANA_V8_BASE + 0xB01C) /* adc3 ctrl register */
#define ADC3_DWA_BPS_LEN      1
#define ADC3_DWA_BPS_OFFSET   7
#define ADC3_TIMING_LEN       3
#define ADC3_TIMING_OFFSET    4
#define ADC3_CLK_DELAY_LEN    2
#define ADC3_CLK_DELAY_OFFSET 2
#define ADC3_FLSTN_LEN        2
#define ADC3_FLSTN_OFFSET     0

#define CODEC_ANA_RW18_REG     (CODEC_ANA_V8_BASE + 0xB01D) /* dac ctrl register */
#define PULL_CPN_ENABLE_LEN    1
#define PULL_CPN_ENABLE_OFFSET 7
#define DACS_ICTRL_LEN         2
#define DACS_ICTRL_OFFSET      4
#define DACL_ICTRL_LEN         2
#define DACL_ICTRL_OFFSET      2
#define DACR_ICTRL_LEN         2
#define DACR_ICTRL_OFFSET      0

#define CODEC_ANA_RW19_REG     (CODEC_ANA_V8_BASE + 0xB01E) /* chargepump ctrl register */
#define CP_CLKSET_LEN          1
#define CP_CLKSET_OFFSET       7
#define CP_AUTOSOFT_DIS_LEN    1
#define CP_AUTOSOFT_DIS_OFFSET 6
#define CP_SOFT_DLY_CFG_LEN    2
#define CP_SOFT_DLY_CFG_OFFSET 4
#define CP_SW_CFG_LEN          2
#define CP_SW_CFG_OFFSET       2
#define CP_DT_CFG_LEN          2
#define CP_DT_CFG_OFFSET       0

#define CODEC_ANA_RW20_REG     (CODEC_ANA_V8_BASE + 0xB01F) /* hp mixer din ctrl register */
#define MIXOUT_HSL_DACL_LEN    1
#define MIXOUT_HSL_DACL_OFFSET 7
#define MIXOUT_HSL_DACR_LEN    1
#define MIXOUT_HSL_DACR_OFFSET 6
#define MIXOUT_HSR_DACR_LEN    1
#define MIXOUT_HSR_DACR_OFFSET 3
#define MIXOUT_HSR_DACL_LEN    1
#define MIXOUT_HSR_DACL_OFFSET 2

#define CODEC_ANA_RW21_REG     (CODEC_ANA_V8_BASE + 0xB020) /* hp gain ctrl register */
#define HSL_GAIN_LEN    4
#define HSL_GAIN_OFFSET 4
#define HSR_GAIN_LEN    4
#define HSR_GAIN_OFFSET 0

#define CODEC_ANA_RW22_REG     (CODEC_ANA_V8_BASE + 0xB021) /* hp ctrl register */
#define HSL_MIN_GAIN_LEN    1
#define HSL_MIN_GAIN_OFFSET 5
#define HSR_MIN_GAIN_LEN    1
#define HSR_MIN_GAIN_OFFSET 4
#define STB_CTRL_SEC_LEN    2
#define STB_CTRL_SEC_OFFSET 2
#define STB_CTRL_LEN        2
#define STB_CTRL_OFFSET     0

#define CODEC_ANA_RW23_REG     (CODEC_ANA_V8_BASE + 0xB022) /* hp ctrl register */
#define STB_ACTIVE_CTRL_LEN    1
#define STB_ACTIVE_CTRL_OFFSET 5
#define STB_1N_LEN             1
#define STB_1N_OFFSET          4
#define HPOUT_1K_LOAD_LEN      1
#define HPOUT_1K_LOAD_OFFSET   3
#define STB_N12DB_GAIN_LEN     1
#define STB_N12DB_GAIN_OFFSET  2
#define THD_CTRL_SEL_LEN       2
#define THD_CTRL_SEL_OFFSET    0

#define CODEC_ANA_RW24_REG     (CODEC_ANA_V8_BASE + 0xB023) /* hp pop ctrl register */
#define POP_DIS_LEN        1
#define POP_DIS_OFFSET     6
#define RST_POP_LEN        1
#define RST_POP_OFFSET     5
#define RL_SYNC_EN_LEN     1
#define RL_SYNC_EN_OFFSET  4
#define POP_DLY_CFG_LEN    2
#define POP_DLY_CFG_OFFSET 2
#define PLO_SPD_CFG_LEN    2
#define PLO_SPD_CFG_OFFSET 0

#define CODEC_ANA_RW25_REG     (CODEC_ANA_V8_BASE + 0xB024) /* ep mixer ctrl register */
#define MIXOUT_EAR_DACL_LEN          1
#define MIXOUT_EAR_DACL_OFFSET       7
#define MIXOUT_EAR_DACR_LEN          1
#define MIXOUT_EAR_DACR_OFFSET       6
#define MIXOUT_EAR_DAC_ULTRA_LEN     1
#define MIXOUT_EAR_DAC_ULTRA_OFFSET  5
#define CR_MIXER_EP_DACL_LEN         1
#define CR_MIXER_EP_DACL_OFFSET      3
#define CR_MIXER_EP_DACR_LEN         1
#define CR_MIXER_EP_DACR_OFFSET      2
#define CR_MIXER_EP_DAC_ULTRA_LEN    1
#define CR_MIXER_EP_DAC_ULTRA_OFFSET 1

#define CODEC_ANA_RW26_REG     (CODEC_ANA_V8_BASE + 0xB025) /* earphone ctrl register */
#define EAR_CM_CTRL_LEN    1
#define EAR_CM_CTRL_OFFSET 4
#define EAR_GAIN_LEN       4
#define EAR_GAIN_OFFSET    0

#define CODEC_ANA_RW27_REG     (CODEC_ANA_V8_BASE + 0xB026) /* lineout mixer ctrl register */
#define LOMIX_SEL_DACL_LEN              1
#define LOMIX_SEL_DACL_OFFSET           7
#define LOMIX_SEL_DACR_LEN              1
#define LOMIX_SEL_DACR_OFFSET           6
#define LOMIX_SEL_DAC_ULTRA_LEN         1
#define LOMIX_SEL_DAC_ULTRA_OFFSET      5
#define LOMIX_BOOSTGAIN_DACL_LEN        1
#define LOMIX_BOOSTGAIN_DACL_OFFSET     3
#define LOMIX_BOOSTGAIN_DACR_LEN        1
#define LOMIX_BOOSTGAIN_DACR_OFFSET     2
#define LOMIX_BOOSTGAIN_DACULTRA_LEN    1
#define LOMIX_BOOSTGAIN_DACULTRA_OFFSET 1

#define CODEC_ANA_RW28_REG     (CODEC_ANA_V8_BASE + 0xB027) /* lineout ctrl register */
#define LOUT_VCM_CTRL_LEN    1
#define LOUT_VCM_CTRL_OFFSET 4
#define LOUT_GAIN_LEN        4
#define LOUT_GAIN_OFFSET     0

#define CODEC_ANA_RW29_REG     (CODEC_ANA_V8_BASE + 0xB028) /* himicbias ctrl register */
#define HSMICB_ENB_SDET_LEN       1
#define HSMICB_ENB_SDET_OFFSET    5
#define HSMICB_LONOISE_1P2_LEN    1
#define HSMICB_LONOISE_1P2_OFFSET 4
#define HSMICB_ADJ_LEN            4
#define HSMICB_ADJ_OFFSET         0

#define CODEC_ANA_RW30_REG     (CODEC_ANA_V8_BASE + 0xB029) /* micbias1 ctrl register */
#define MICB1_ENB_SDET_LEN       1
#define MICB1_ENB_SDET_OFFSET    5
#define MICB1_LONOISE_1P2_LEN    1
#define MICB1_LONOISE_1P2_OFFSET 4
#define MICB1_ADJ_LEN            4
#define MICB1_ADJ_OFFSET         0

#define CODEC_ANA_RW31_REG     (CODEC_ANA_V8_BASE + 0xB02A) /* micbias2 ctrl register */
#define MICB2_ENB_SDET_LEN       1
#define MICB2_ENB_SDET_OFFSET    5
#define MICB2_LONOISE_1P2_LEN    1
#define MICB2_LONOISE_1P2_OFFSET 4
#define MICB2_ADJ_LEN            4
#define MICB2_ADJ_OFFSET         0

#define CODEC_ANA_RW32_REG     (CODEC_ANA_V8_BASE + 0xB02B) /* hp insertion detection register */
#define HSD_INV_LEN              1
#define HSD_INV_OFFSET           6
#define HSD_PULLDOWN_LEN         1
#define HSD_PULLDOWN_OFFSET      5
#define HSE_SEL_LEN              1
#define HSE_SEL_OFFSET           4
#define HSD_VTH_CFG_LEN          2
#define HSD_VTH_CFG_OFFSET       2
#define HSD_EN_LEN               2
#define HSD_EN_OFFSET            0

#define CODEC_ANA_RW33_REG     (CODEC_ANA_V8_BASE + 0xB02C) /* pullout pop register */
#define ANALOG_LOOP_LEN                    1
#define ANALOG_LOOP_OFFSET                 3
#define PULLOUT_POP_EN1_LEN                1
#define PULLOUT_POP_EN1_OFFSET             2
#define PULLOUT_POP_EN2_LEN                1
#define PULLOUT_POP_EN2_OFFSET             1
#define PULLOUT_HSD_DIR_SEL_LEN            1
#define PULLOUT_HSD_DIR_SEL_OFFSET         0

#define CODEC_ANA_RW34_REG     (CODEC_ANA_V8_BASE + 0xB02D) /* mbhd ref ctrl register */
#define MBHD_VREF_PD_LEN     1
#define MBHD_VREF_PD_OFFSET  7
#define ECO_VTH_SEL_LEN      3
#define ECO_VTH_SEL_OFFSET   4
#define COMPH_VTH_SEL_LEN    2
#define COMPH_VTH_SEL_OFFSET 2
#define COMPL_VTH_SEL_LEN    2
#define COMPL_VTH_SEL_OFFSET 0

#define CODEC_ANA_RW35_REG     (CODEC_ANA_V8_BASE + 0xB02E) /* adc/dac clk register */
#define ADC_CLK_SYS_EDGE_LEN    2
#define ADC_CLK_SYS_EDGE_OFFSET 2
#define DAC_CLK_SEL_EDGE_LEN    2
#define DAC_CLK_SEL_EDGE_OFFSET 0

#define CODEC_ANA_RW36_REG     (CODEC_ANA_V8_BASE + 0xB02F) /* rx chopper clk register */
#define RX_CHOP_BPS_LEN         1
#define RX_CHOP_BPS_OFFSET      6
#define ADC_CHOP_CLK_SEL_LEN    2
#define ADC_CHOP_CLK_SEL_OFFSET 4
#define CTCM_CHOP_BPS_LEN       1
#define CTCM_CHOP_BPS_OFFSET    3
#define CAPLESS_CHOP_BPS_LEN    1
#define CAPLESS_CHOP_BPS_OFFSET 2
#define MICPGA_CHOP_BPS_LEN     1
#define MICPGA_CHOP_BPS_OFFSET  1
#define ADC_SDM_CHOP_BPS_LEN    1
#define ADC_SDM_CHOP_BPS_OFFSET 0

#define CODEC_ANA_RW37_REG     (CODEC_ANA_V8_BASE + 0xB030) /* tx chopper clk register */
#define TX_CHOP_BPS_LEN         1
#define TX_CHOP_BPS_OFFSET      6
#define DAC_CHOP_CLK_SEL_LEN    2
#define DAC_CHOP_CLK_SEL_OFFSET 4
#define DAC_CHOP_BPS_LEN        1
#define DAC_CHOP_BPS_OFFSET     3
#define HS_CHOP_BPS_LEN         1
#define HS_CHOP_BPS_OFFSET      2
#define EPMIX_CHOP_BPS_LEN      1
#define EPMIX_CHOP_BPS_OFFSET   1
#define LOUT_CHOP_BPS_LEN       1
#define LOUT_CHOP_BPS_OFFSET    0

#define CODEC_ANA_RW38_REG     (CODEC_ANA_V8_BASE + 0xB031) /* cp/pop clk register */
#define HS_SYS_CLK_PD_LEN     1
#define HS_SYS_CLK_PD_OFFSET  5
#define CLKPOP_SEL_LEN        1
#define CLKPOP_SEL_OFFSET     4
#define CLKCP_SRC_FREQ_LEN    1
#define CLKCP_SRC_FREQ_OFFSET 3
#define CLKCP_SEL_LEN         3
#define CLKCP_SEL_OFFSET      0

#define CODEC_ANA_RW39_REG     (CODEC_ANA_V8_BASE + 0xB032) /* capless clk register */
#define CLK_CAPLESS_BPS_LEN       1
#define CLK_CAPLESS_BPS_OFFSET    3
#define CAPLESS_CLK_PH_SEL_LEN    1
#define CAPLESS_CLK_PH_SEL_OFFSET 2
#define CAPLESS_CLK_FO_SEL_LEN    2
#define CAPLESS_CLK_FO_SEL_OFFSET 0

#define CODEC_ANA_RW40_REG     (CODEC_ANA_V8_BASE + 0xB033) /* bias current register */
#define CODEC_IBIAS_ADJ_LEN     1
#define CODEC_IBIAS_ADJ_OFFSET  7
#define IB05_CTCM_ADJ_LEN       3
#define IB05_CTCM_ADJ_OFFSET    4
#define IB05_CAPLESS_ADJ_LEN    3
#define IB05_CAPLESS_ADJ_OFFSET 0

#define CODEC_ANA_RW41_REG     (CODEC_ANA_V8_BASE + 0xB034) /* bias current register */
#define IB05_MICPGA_ADJ_LEN         3
#define IB05_MICPGA_ADJ_OFFSET      4
#define IB05_MICPGALOGIC_ADJ_LEN    3
#define IB05_MICPGALOGIC_ADJ_OFFSET 0

#define CODEC_ANA_RW42_REG     (CODEC_ANA_V8_BASE + 0xB035) /* bias current register */
#define IB05_ADCOP1_ADJ_LEN    3
#define IB05_ADCOP1_ADJ_OFFSET 4
#define IB05_ADCOP2_ADJ_LEN    3
#define IB05_ADCOP2_ADJ_OFFSET 0

#define CODEC_ANA_RW43_REG     (CODEC_ANA_V8_BASE + 0xB036) /* bias current register */
#define IB05_ADCCOMP_ADJ_LEN    3
#define IB05_ADCCOMP_ADJ_OFFSET 4
#define IB05_DACI2V_ADJ_LEN     3
#define IB05_DACI2V_ADJ_OFFSET  0

#define CODEC_ANA_RW44_REG     (CODEC_ANA_V8_BASE + 0xB037) /* bias current register */
#define IB05_HPMIX_ADJ_LEN    3
#define IB05_HPMIX_ADJ_OFFSET 4
#define IB05_HP_ADJ_LEN       3
#define IB05_HP_ADJ_OFFSET    0

#define CODEC_ANA_RW45_REG     (CODEC_ANA_V8_BASE + 0xB038) /* bias current register */
#define IB05_HPLOGIC_ADJ_LEN     3
#define IB05_HPLOGIC_ADJ_OFFSET  4
#define IB05_EARLOGIC_ADJ_LEN    3
#define IB05_EARLOGIC_ADJ_OFFSET 0

#define CODEC_ANA_RW46_REG     (CODEC_ANA_V8_BASE + 0xB039) /* bias current register */
#define IB05_EARMIX_ADJ_LEN    3
#define IB05_EARMIX_ADJ_OFFSET 4
#define IB05_EAR_ADJ_LEN       3
#define IB05_EAR_ADJ_OFFSET    0

#define CODEC_ANA_RW47_REG     (CODEC_ANA_V8_BASE + 0xB03A) /* bias current register */
#define IB05_LO_ADJ_LEN         3
#define IB05_LO_ADJ_OFFSET      4
#define IB05_LOLOGIC_ADJ_LEN    3
#define IB05_LOLOGIC_ADJ_OFFSET 0

#define CODEC_ANA_RW48_REG     (CODEC_ANA_V8_BASE + 0xB03B) /* bias current register */
#define IB05_MICB_ADJ_LEN    3
#define IB05_MICB_ADJ_OFFSET 0

#define CODEC_ANA_RW49_REG     (CODEC_ANA_V8_BASE + 0xB03C) /* bias current register */
#define IB05_LOMIX_ADJ_LEN       3
#define IB05_LOMIX_ADJ_OFFSET    4
#define IB05_DACS_I2V_ADJ_LEN    3
#define IB05_DACS_I2V_ADJ_OFFSET 0

#define CODEC_ANA_RW50_REG     (CODEC_ANA_V8_BASE + 0xB03D)
#define CODEC_ANA_RW_50_LEN    8
#define CODEC_ANA_RW_50_OFFSET 0

#define CODEC_ANA_RW51_REG     (CODEC_ANA_V8_BASE + 0xB03E)
#define CODEC_ANA_RW_51_LEN    8
#define CODEC_ANA_RW_51_OFFSET 0

#define CODEC_ANA_RW52_REG     (CODEC_ANA_V8_BASE + 0xB03F)
#define CODEC_ANA_RW_52_LEN    8
#define CODEC_ANA_RW_52_OFFSET 0

#define CODEC_ANA_RW53_REG     (CODEC_ANA_V8_BASE + 0xB040)
#define CODEC_ANA_RW_53_LEN    8
#define CODEC_ANA_RW_53_OFFSET 0

#define CODEC_ANA_RW54_REG     (CODEC_ANA_V8_BASE + 0xB041)
#define CODEC_ANA_RW_54_LEN    8
#define CODEC_ANA_RW_54_OFFSET 0

#define CODEC_ANA_RW55_REG     (CODEC_ANA_V8_BASE + 0xB042)
#define CODEC_ANA_RW_55_LEN    8
#define CODEC_ANA_RW_55_OFFSET 0

#define CODEC_ANA_RW56_REG     (CODEC_ANA_V8_BASE + 0xB043)
#define CODEC_ANA_RW_56_LEN    8
#define CODEC_ANA_RW_56_OFFSET 0

#define CODEC_ANA_RW57_REG     (CODEC_ANA_V8_BASE + 0xB044)
#define CODEC_ANA_RW_57_LEN    8
#define CODEC_ANA_RW_57_OFFSET 0

#define CODEC_ANA_RW58_REG     (CODEC_ANA_V8_BASE + 0xB045) /* mad adc route register */
#define VCM_MAD_LP_EN_LEN    1
#define VCM_MAD_LP_EN_OFFSET 6
#define MAD_PGA_SEL_LEN      4
#define MAD_PGA_SEL_OFFSET   0

#define CODEC_ANA_RW59_REG     (CODEC_ANA_V8_BASE + 0xB046) /* mad adc route register */
#define PGA_LEN    5
#define PGA_OFFSET 0

#define CODEC_ANA_RW60_REG     (CODEC_ANA_V8_BASE + 0xB047) /* mad adc route register */
#define PD_MAD_SDM_LEN         1
#define PD_MAD_SDM_OFFSET      5
#define PD_MAD_PGA_LEN         1
#define PD_MAD_PGA_OFFSET      4
#define PD_MAD_CLK_GEN_LEN     1
#define PD_MAD_CLK_GEN_OFFSET  3
#define PD_DOUT_MAD_LEN        1
#define PD_DOUT_MAD_OFFSET     2
#define PD_MUTE_MAD_LEN        1
#define PD_MUTE_MAD_OFFSET     1
#define PD_MUTE_MAD_PGA_LEN    1
#define PD_MUTE_MAD_PGA_OFFSET 0

#define CODEC_ANA_RW61_REG     (CODEC_ANA_V8_BASE + 0xB048)
#define CODEC_ANA_RW62_REG     (CODEC_ANA_V8_BASE + 0xB049) /* mad adc route register */
#define DIN_MAD_MIC_LEN    1
#define DIN_MAD_MIC_OFFSET 0

#define CODEC_ANA_RW63_REG     (CODEC_ANA_V8_BASE + 0xB04A)

#define CODEC_ANA_RO01_REG     (CODEC_ANA_V8_BASE + 0xB04B)
#define CODEC_ANA_RO_01_LEN    8
#define CODEC_ANA_RO_01_OFFSET 0

#define CODEC_ANA_RO02_REG     (CODEC_ANA_V8_BASE + 0xB04C)
#define CODEC_ANA_RO_02_LEN    8
#define CODEC_ANA_RO_02_OFFSET 0

/* CTRL_REGA Base address of Module's Register */
#define CODEC_ANA_V8_CTRL_REGA_BASE                  (0x0)

#define CTRL_REG_PMU_SOFT_RST_REG       (CODEC_ANA_V8_CTRL_REGA_BASE + 0xCA)
#define CTRL_REG_SOFT_RST_N_LEN    8
#define CTRL_REG_SOFT_RST_N_OFFSET 0

/* XOADC Base address of Module's Register */
#define CODEC_ANA_V8_XOADC_BASE                       (0x0)

#define CTRL_REG_DIG_IO_DS_CODEC_CFG_REG  (CODEC_ANA_V8_XOADC_BASE + 0x3F9)

#define CTRL_REG_DIG_IO_DS_SEL1_CODEC_CFG_REG  (CODEC_ANA_V8_XOADC_BASE + 0x3FA)

#endif /* __ASP_CODEC_ANA_V8_REG_DEF_H__ */
