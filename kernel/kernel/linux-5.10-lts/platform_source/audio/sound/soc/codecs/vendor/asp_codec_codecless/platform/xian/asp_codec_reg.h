/*
 * asp_codec_reg.h -- codec driver
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#ifndef __ASP_CODEC_REG_H__
#define __ASP_CODEC_REG_H__

/*
 * Due to the limitation of ALSA that the register address is of type int,
 * while the actual base address of the codec registers is of type unsigned int,
 * it is necessary to clear the sign bit of the base address.
 */
#define clear_31_bit(x)     ((x) & (0x7FFFFFFF))

#define PAGE_SOCCODEC         clear_31_bit(0xFD150000)
#define PAGE_ASPCFG           clear_31_bit(0xFD14F000)
#define PAGE_AXI2TDM          clear_31_bit(0xFD1DA000)
#define PAGE_MAD              clear_31_bit(0xFDA80000)
#define PAGE_VIRCODEC         0x8000

#define PAGE_SOCSMARTPA1      (PAGE_SOCCODEC + 0x2000)
#define SPA1BYPASS_CFG        (PAGE_SOCSMARTPA1 + 0x0)
#define SPA1_SPA_I_DATA_SEL_OFFSET               22
#define SPA1_SPA_I_DATA_SEL_OFFSET_LEN           4
#define SPA1_SPA_V_DATA_SEL_OFFSET               18
#define SPA1_SPA_V_DATA_SEL_OFFSET_LEN           4
#define SPA1_BYPASS_EN_OFFSET                    0
#define SPA1_BYPASS_EN_LEN                       18
#define CLK_RST_CFG1          (PAGE_SOCSMARTPA1 + 0x4)
#define SPA1_SMARTPA_PROTECT_CLK_RST_EN_OFFSET   15
#define SPA1_CLK_RST_CFG_CLK_RST_EN_LEN          17
#define SPA1_PARAMS_PASSTHROUGH_EN_OFFSET        12
#define SPA1_PARAMS_PASSTHROUGH_EN_LEN           1
#define SPA1_LTM_CLK_RST_EN_OFFSET               2
#define SPA1_LTM_CLK_RST_EN_LEN                  1
#define SPA1_BCN_CLK_RST_EN_OFFSET               1
#define SPA1_BCN_CLK_RST_EN_LEN                  1
#define SPA1_PRE_CFG0         (PAGE_SOCSMARTPA1 + 0x8)
#define SPA1_CFG_PRE_PGA_BYPASS_OFFSET           4
#define SPA1_CFG_PRE_PGA_BYPASS_LEN              1
#define SPA1_CFG_PRE_HPS_BYPASS_OFFSET           3
#define SPA1_CFG_PRE_HPS_BYPASS_LEN              1
#define SPA1_CFG_PRE_LPS_BYPASS_OFFSET           2
#define SPA1_CFG_PRE_LPS_BYPASS_LEN              1
#define SPA1_CFG_PRE_PILOTTONE_CLKEN_OFFSET      0
#define SPA1_CFG_PRE_LPS_BYPASS_LEN              1

#define PAGE_SOCSMARTPA2      (PAGE_SOCCODEC + 0x3000)
#define SPA2BYPASS_CFG        (PAGE_SOCSMARTPA2 + 0x0)
#define SPA2_SPA_I_DATA_SEL_OFFSET               22
#define SPA2_SPA_I_DATA_SEL_OFFSET_LEN           4
#define SPA2_SPA_V_DATA_SEL_OFFSET               18
#define SPA2_SPA_V_DATA_SEL_OFFSET_LEN           4
#define SPA2_BYPASS_EN_OFFSET                    0
#define SPA2_BYPASS_EN_LEN                       18
#define CLK_RST_CFG2          (PAGE_SOCSMARTPA2 + 0x4)
#define SPA2_SMARTPA_PROTECT_CLK_RST_EN_OFFSET   15
#define SPA2_CLK_RST_CFG_CLK_RST_EN_LEN          17
#define SPA2_PARAMS_PASSTHROUGH_EN_OFFSET        12
#define SPA2_PARAMS_PASSTHROUGH_EN_LEN           1
#define SPA2_LTM_CLK_RST_EN_OFFSET               2
#define SPA2_LTM_CLK_RST_EN_LEN                  1
#define SPA2_BCN_CLK_RST_EN_OFFSET               1
#define SPA2_BCN_CLK_RST_EN_LEN                  1
#define SPA2_PRE_CFG0         (PAGE_SOCSMARTPA2 + 0x8)
#define SPA2_CFG_PRE_PGA_BYPASS_OFFSET           4
#define SPA2_CFG_PRE_PGA_BYPASS_LEN              1
#define SPA2_CFG_PRE_HPS_BYPASS_OFFSET           3
#define SPA2_CFG_PRE_HPS_BYPASS_LEN              1
#define SPA2_CFG_PRE_LPS_BYPASS_OFFSET           2
#define SPA2_CFG_PRE_LPS_BYPASS_LEN              1
#define SPA2_CFG_PRE_PILOTTONE_CLKEN_OFFSET      0
#define SPA2_CFG_PRE_LPS_BYPASS_LEN              1

#define PAGE_SOCSMARTPA3      (PAGE_SOCCODEC + 0x4000)
#define SPA3BYPASS_CFG        (PAGE_SOCSMARTPA3 + 0x0)
#define SPA3_SPA_I_DATA_SEL_OFFSET               22
#define SPA3_SPA_I_DATA_SEL_OFFSET_LEN           4
#define SPA3_SPA_V_DATA_SEL_OFFSET               18
#define SPA3_SPA_V_DATA_SEL_OFFSET_LEN           4
#define SPA3_BYPASS_EN_OFFSET                    0
#define SPA3_BYPASS_EN_LEN                       18
#define CLK_RST_CFG3          (PAGE_SOCSMARTPA3 + 0x4)
#define SPA3_SMARTPA_PROTECT_CLK_RST_EN_OFFSET   15
#define SPA3_CLK_RST_CFG_CLK_RST_EN_LEN          17
#define SPA3_PARAMS_PASSTHROUGH_EN_OFFSET        12
#define SPA3_PARAMS_PASSTHROUGH_EN_LEN           1
#define SPA3_LTM_CLK_RST_EN_OFFSET               2
#define SPA3_LTM_CLK_RST_EN_LEN                  1
#define SPA3_BCN_CLK_RST_EN_OFFSET               1
#define SPA3_BCN_CLK_RST_EN_LEN                  1
#define SPA3_PRE_CFG0         (PAGE_SOCSMARTPA3 + 0x8)
#define SPA3_CFG_PRE_PGA_BYPASS_OFFSET           4
#define SPA3_CFG_PRE_PGA_BYPASS_LEN              1
#define SPA3_CFG_PRE_HPS_BYPASS_OFFSET           3
#define SPA3_CFG_PRE_HPS_BYPASS_LEN              1
#define SPA3_CFG_PRE_LPS_BYPASS_OFFSET           2
#define SPA3_CFG_PRE_LPS_BYPASS_LEN              1
#define SPA3_CFG_PRE_PILOTTONE_CLKEN_OFFSET      0
#define SPA3_CFG_PRE_LPS_BYPASS_LEN              1

#define PAGE_SOCSMARTPA4      (PAGE_SOCCODEC + 0x5000)
#define SPA4BYPASS_CFG        (PAGE_SOCSMARTPA4 + 0x0)
#define SPA4_SPA_I_DATA_SEL_OFFSET               22
#define SPA4_SPA_I_DATA_SEL_OFFSET_LEN           4
#define SPA4_SPA_V_DATA_SEL_OFFSET               18
#define SPA4_SPA_V_DATA_SEL_OFFSET_LEN           4
#define SPA4_BYPASS_EN_OFFSET                    0
#define SPA4_BYPASS_EN_LEN                       18
#define CLK_RST_CFG4          (PAGE_SOCSMARTPA4 + 0x4)
#define SPA4_SMARTPA_PROTECT_CLK_RST_EN_OFFSET   15
#define SPA4_CLK_RST_CFG_CLK_RST_EN_LEN          17
#define SPA4_PARAMS_PASSTHROUGH_EN_OFFSET        12
#define SPA4_PARAMS_PASSTHROUGH_EN_LEN           1
#define SPA4_LTM_CLK_RST_EN_OFFSET               2
#define SPA4_LTM_CLK_RST_EN_LEN                  1
#define SPA4_BCN_CLK_RST_EN_OFFSET               1
#define SPA4_BCN_CLK_RST_EN_LEN                  1
#define SPA4_PRE_CFG0         (PAGE_SOCSMARTPA4 + 0x8)
#define SPA4_CFG_PRE_PGA_BYPASS_OFFSET           4
#define SPA4_CFG_PRE_PGA_BYPASS_LEN              1
#define SPA4_CFG_PRE_HPS_BYPASS_OFFSET           3
#define SPA4_CFG_PRE_HPS_BYPASS_LEN              1
#define SPA4_CFG_PRE_LPS_BYPASS_OFFSET           2
#define SPA4_CFG_PRE_LPS_BYPASS_LEN              1
#define SPA4_CFG_PRE_PILOTTONE_CLKEN_OFFSET      0
#define SPA4_CFG_PRE_LPS_BYPASS_LEN              1

#define SOCCODEC_END_OFFSET   0x274
#define VIRCODEC_END_OFFSET   (VIR_REG_CNT - 1)
#define ASPCFG_END_OFFSET     0xFFF
#define AXI2TDM_END           0xD0
#define MAD_END               0xFFF
#define SOCSMARTPA_END_OFFSET 0x8
#define MAD_CFG_BASE          (0xFDA80000)

#include "asp_codec_axi2tdm_reg.h"
#include "asp_codec_mad_regs.h"

#define VERSION_REG              (PAGE_SOCCODEC + 0x0)
#define VERSION_OFFSET                            0
#define VERSION_LEN                               32

#define CODEC_CLK_EN0_REG        (PAGE_SOCCODEC + 0x4)
#define SPA1_UP_AFIFO_CLKEN_OFFSET                0
#define SPA1_UP_AFIFO_CLKEN_LEN                   1
#define SPA2_UP_AFIFO_CLKEN_OFFSET                1
#define SPA2_UP_AFIFO_CLKEN_LEN                   1
#define SPA3_UP_AFIFO_CLKEN_OFFSET                2
#define SPA3_UP_AFIFO_CLKEN_LEN                   1
#define SPA4_UP_AFIFO_CLKEN_OFFSET                3
#define SPA4_UP_AFIFO_CLKEN_LEN                   1
#define SPA5_UP_AFIFO_CLKEN_OFFSET                4
#define SPA5_UP_AFIFO_CLKEN_LEN                   1
#define SPA6_UP_AFIFO_CLKEN_OFFSET                5
#define SPA6_UP_AFIFO_CLKEN_LEN                   1
#define AUDIO_UP_L_AFIFO_CLKEN_OFFSET             6
#define AUDIO_UP_L_AFIFO_CLKEN_LEN                1
#define AUDIO_UP_R_AFIFO_CLKEN_OFFSET             7
#define AUDIO_UP_R_AFIFO_CLKEN_LEN                1
#define VOICE_UP_L_AFIFO_CLKEN_OFFSET             8
#define VOICE_UP_L_AFIFO_CLKEN_LEN                1
#define VOICE_UP_R_AFIFO_CLKEN_OFFSET             9
#define VOICE_UP_R_AFIFO_CLKEN_LEN                1
#define MIC3_UP_AFIFO_CLKEN_OFFSET                10
#define MIC3_UP_AFIFO_CLKEN_LEN                   1
#define MIC4_UP_AFIFO_CLKEN_OFFSET                11
#define MIC4_UP_AFIFO_CLKEN_LEN                   1
#define MIC5_UP_AFIFO_CLKEN_OFFSET                12
#define MIC5_UP_AFIFO_CLKEN_LEN                   1
#define MIC6_UP_AFIFO_CLKEN_OFFSET                13
#define MIC6_UP_AFIFO_CLKEN_LEN                   1
#define MIC7_UP_AFIFO_CLKEN_OFFSET                14
#define MIC7_UP_AFIFO_CLKEN_LEN                   1
#define MIC8_UP_AFIFO_CLKEN_OFFSET                15
#define MIC8_UP_AFIFO_CLKEN_LEN                   1
#define ULTR_UP_AFIFO_CLKEN_OFFSET                16
#define ULTR_UP_AFIFO_CLKEN_LEN                   1
#define ULTR_DN_AFIFO_CLKEN_OFFSET                17
#define ULTR_DN_AFIFO_CLKEN_LEN                   1
#define CODEC_0_DN_AFIFO_CLKEN_OFFSET             18
#define CODEC_0_DN_AFIFO_CLKEN_LEN                1
#define CODEC_1_DN_AFIFO_CLKEN_OFFSET             19
#define CODEC_1_DN_AFIFO_CLKEN_LEN                1
#define CODEC_2_DN_AFIFO_CLKEN_OFFSET             20
#define CODEC_2_DN_AFIFO_CLKEN_LEN                1
#define AUDIO_0_DN_AFIFO_CLKEN_OFFSET             21
#define AUDIO_0_DN_AFIFO_CLKEN_LEN                1
#define AUDIO_1_DN_AFIFO_CLKEN_OFFSET             22
#define AUDIO_1_DN_AFIFO_CLKEN_LEN                1
#define AUDIO_2_DN_AFIFO_CLKEN_OFFSET             23
#define AUDIO_2_DN_AFIFO_CLKEN_LEN                1
#define AUDIOZOOM_UP_AFIFO_CLKEN_OFFSET           24
#define AUDIOZOOM_UP_AFIFO_CLKEN_LEN              1
#define NOISE_METER_UP_AFIFO_CLKEN_OFFSET         25
#define NOISE_METER_UP_AFIFO_CLKEN_LEN            1
#define I2S1_PCM_CLKEN_OFFSET                     26
#define I2S1_PCM_CLKEN_LEN                        1
#define I2S1_TDM128_CLKEN_OFFSET                  27
#define I2S1_TDM128_CLKEN_LEN                     1
#define I2S1_TDM256_CLKEN_OFFSET                  28
#define I2S1_TDM256_CLKEN_LEN                     1
#define I2S2_PCM_CLKEN_OFFSET                     29
#define I2S2_PCM_CLKEN_LEN                        1
#define I2S2_TX_MIXER2_CLKEN_OFFSET               30
#define I2S2_TX_MIXER2_CLKEN_LEN                  1
#define SPA_PROTECT_3WAY_CLK_EN_OFFSET            31
#define SPA_PROTECT_3WAY_CLK_EN_LEN               1

#define CODEC_CLK_EN1_REG        (PAGE_SOCCODEC + 0x8)
#define AUDIO_L_UP_PGA_CLKEN_OFFSET               0
#define AUDIO_L_UP_PGA_CLKEN_LEN                  1
#define AUDIO_R_UP_PGA_CLKEN_OFFSET               1
#define AUDIO_R_UP_PGA_CLKEN_LEN                  1
#define VOICE_L_UP_PGA_CLKEN_OFFSET               2
#define VOICE_L_UP_PGA_CLKEN_LEN                  1
#define VOICE_R_UP_PGA_CLKEN_OFFSET               3
#define VOICE_R_UP_PGA_CLKEN_LEN                  1
#define MIC3_UP_PGA_CLKEN_OFFSET                  4
#define MIC3_UP_PGA_CLKEN_LEN                     1
#define MIC4_UP_PGA_CLKEN_OFFSET                  5
#define MIC4_UP_PGA_CLKEN_LEN                     1
#define MIC5_UP_PGA_CLKEN_OFFSET                  6
#define MIC5_UP_PGA_CLKEN_LEN                     1
#define MIC6_UP_PGA_CLKEN_OFFSET                  7
#define MIC6_UP_PGA_CLKEN_LEN                     1
#define MIC7_UP_PGA_CLKEN_OFFSET                  8
#define MIC7_UP_PGA_CLKEN_LEN                     1
#define MIC8_UP_PGA_CLKEN_OFFSET                  9
#define MIC8_UP_PGA_CLKEN_LEN                     1
#define CODEC_0_DN_PGA_CLKEN_OFFSET               10
#define CODEC_0_DN_PGA_CLKEN_LEN                  1
#define CODEC_1_DN_PGA_CLKEN_OFFSET               11
#define CODEC_1_DN_PGA_CLKEN_LEN                  1
#define CODEC_2_DN_PGA_CLKEN_OFFSET               12
#define CODEC_2_DN_PGA_CLKEN_LEN                  1
#define AUDIO_0_DN_PGA_CLKEN_OFFSET               13
#define AUDIO_0_DN_PGA_CLKEN_LEN                  1
#define AUDIO_1_DN_PGA_CLKEN_OFFSET               14
#define AUDIO_1_DN_PGA_CLKEN_LEN                  1
#define AUDIO_2_DN_PGA_CLKEN_OFFSET               15
#define AUDIO_2_DN_PGA_CLKEN_LEN                  1
#define AUDIOZOOM_UP_PGA_CLKEN_OFFSET             16
#define AUDIOZOOM_UP_PGA_CLKEN_LEN                1
#define NOISE_METER_UP_PGA_CLKEN_OFFSET           17
#define NOISE_METER_UP_PGA_CLKEN_LEN              1
#define I2S2_RX_L_PGA_CLKEN_OFFSET                18
#define I2S2_RX_L_PGA_CLKEN_LEN                   1
#define I2S2_RX_R_PGA_CLKEN_OFFSET                19
#define I2S2_RX_R_PGA_CLKEN_LEN                   1
#define AUDIO_L_UP_SRCDN_CLKEN_OFFSET             20
#define AUDIO_L_UP_SRCDN_CLKEN_LEN                1
#define AUDIO_R_UP_SRCDN_CLKEN_OFFSET             21
#define AUDIO_R_UP_SRCDN_CLKEN_LEN                1
#define VOICE_L_UP_SRCDN_CLKEN_OFFSET             22
#define VOICE_L_UP_SRCDN_CLKEN_LEN                1
#define VOICE_R_UP_SRCDN_CLKEN_OFFSET             23
#define VOICE_R_UP_SRCDN_CLKEN_LEN                1
#define MIC3_UP_SRCDN_CLKEN_OFFSET                24
#define MIC3_UP_SRCDN_CLKEN_LEN                   1
#define MIC4_UP_SRCDN_CLKEN_OFFSET                25
#define MIC4_UP_SRCDN_CLKEN_LEN                   1
#define MIC5_UP_SRCDN_CLKEN_OFFSET                26
#define MIC5_UP_SRCDN_CLKEN_LEN                   1
#define MIC6_UP_SRCDN_CLKEN_OFFSET                27
#define MIC6_UP_SRCDN_CLKEN_LEN                   1
#define MIC7_UP_SRCDN_CLKEN_OFFSET                28
#define MIC7_UP_SRCDN_CLKEN_LEN                   1
#define MIC8_UP_SRCDN_CLKEN_OFFSET                29
#define MIC8_UP_SRCDN_CLKEN_LEN                   1
#define ULTR_DN_PGA_CLKEN_OFFSET                  30
#define ULTR_DN_PGA_CLKEN_LEN                     1
#define NOISE_METER_UP_SRCDN_CLKEN_OFFSET         31
#define NOISE_METER_UP_SRCDN_CLKEN_LEN            1

#define CODEC_CLK_EN2_REG        (PAGE_SOCCODEC + 0xC)
#define I2S2_TX_L_SRCDN_CLKEN_OFFSET              0
#define I2S2_TX_L_SRCDN_CLKEN_LEN                 1
#define I2S2_TX_R_SRCDN_CLKEN_OFFSET              1
#define I2S2_TX_R_SRCDN_CLKEN_LEN                 1
#define I2S2_RX_L_SRCDN_CLKEN_OFFSET              2
#define I2S2_RX_L_SRCDN_CLKEN_LEN                 1
#define I2S2_RX_R_SRCDN_CLKEN_OFFSET              3
#define I2S2_RX_R_SRCDN_CLKEN_LEN                 1
#define ULTR_DN_SRCUP_CLKEN_OFFSET                4
#define ULTR_DN_SRCUP_CLKEN_LEN                   1
#define CODEC_0_DN_SRCUP_CLKEN_OFFSET             5
#define CODEC_0_DN_SRCUP_CLKEN_LEN                1
#define CODEC_1_DN_SRCUP_CLKEN_OFFSET             6
#define CODEC_1_DN_SRCUP_CLKEN_LEN                1
#define CODEC_2_DN_SRCUP_CLKEN_OFFSET             7
#define CODEC_2_DN_SRCUP_CLKEN_LEN                1
#define AUDIO_0_DN_SRCUP_CLKEN_OFFSET             8
#define AUDIO_0_DN_SRCUP_CLKEN_LEN                1
#define AUDIO_1_DN_SRCUP_CLKEN_OFFSET             9
#define AUDIO_1_DN_SRCUP_CLKEN_LEN                1
#define AUDIO_2_DN_SRCUP_CLKEN_OFFSET             10
#define AUDIO_2_DN_SRCUP_CLKEN_LEN                1
#define I2S2_RX_L_SRCUP_CLKEN_OFFSET              11
#define I2S2_RX_L_SRCUP_CLKEN_LEN                 1
#define I2S2_RX_R_SRCUP_CLKEN_OFFSET              12
#define I2S2_RX_R_SRCUP_CLKEN_LEN                 1
#define DACL_SRCUP_CLKEN_OFFSET                   13
#define DACL_SRCUP_CLKEN_LEN                      1
#define DACR_SRCUP_CLKEN_OFFSET                   14
#define DACR_SRCUP_CLKEN_LEN                      1
#define ADC1_CLKEN_OFFSET                         15
#define ADC1_CLKEN_LEN                            1
#define ADC2_CLKEN_OFFSET                         16
#define ADC2_CLKEN_LEN                            1
#define ADC3_CLKEN_OFFSET                         17
#define ADC3_CLKEN_LEN                            1
#define ADC4_CLKEN_OFFSET                         18
#define ADC4_CLKEN_LEN                            1
#define ADC5_CLKEN_OFFSET                         19
#define ADC5_CLKEN_LEN                            1
#define ADC6_CLKEN_OFFSET                         20
#define ADC6_CLKEN_LEN                            1
#define ADC7_CLKEN_OFFSET                         21
#define ADC7_CLKEN_LEN                            1
#define ADC8_CLKEN_OFFSET                         22
#define ADC8_CLKEN_LEN                            1
#define ADC_ULTR_CLKEN_OFFSET                     23
#define ADC_ULTR_CLKEN_LEN                        1
#define DACL_CLKEN_OFFSET                         24
#define DACL_CLKEN_LEN                            1
#define DACR_CLKEN_OFFSET                         25
#define DACR_CLKEN_LEN                            1
#define MIXER4_SPA0_CLKEN_OFFSET                  26
#define MIXER4_SPA0_CLKEN_LEN                     1
#define MIXER4_SPA1_CLKEN_OFFSET                  27
#define MIXER4_SPA1_CLKEN_LEN                     1
#define MIXER4_SPA2_CLKEN_OFFSET                  28
#define MIXER4_SPA2_CLKEN_LEN                     1
#define MIXER2_ULTR0_CLKEN_OFFSET                 29
#define MIXER2_ULTR0_CLKEN_LEN                    1
#define MIXER2_ULTR1_CLKEN_OFFSET                 30
#define MIXER2_ULTR1_CLKEN_LEN                    1
#define MIXER2_ULTR2_CLKEN_OFFSET                 31
#define MIXER2_ULTR2_CLKEN_LEN                    1

#define CODEC_CLK_EN3_REG        (PAGE_SOCCODEC + 0x10)
#define ADC1_0P5_PGA_CLKEN_OFFSET                 0
#define ADC1_0P5_PGA_CLKEN_LEN                    1
#define ADC2_0P5_PGA_CLKEN_OFFSET                 1
#define ADC2_0P5_PGA_CLKEN_LEN                    1
#define ADC3_0P5_PGA_CLKEN_OFFSET                 2
#define ADC3_0P5_PGA_CLKEN_LEN                    1
#define ADC4_0P5_PGA_CLKEN_OFFSET                 3
#define ADC4_0P5_PGA_CLKEN_LEN                    1
#define ADC5_0P5_PGA_CLKEN_OFFSET                 4
#define ADC5_0P5_PGA_CLKEN_LEN                    1
#define ADC6_0P5_PGA_CLKEN_OFFSET                 5
#define ADC6_0P5_PGA_CLKEN_LEN                    1
#define ADC7_0P5_PGA_CLKEN_OFFSET                 6
#define ADC7_0P5_PGA_CLKEN_LEN                    1
#define ADC8_0P5_PGA_CLKEN_OFFSET                 7
#define ADC8_0P5_PGA_CLKEN_LEN                    1
#define ADC1_CICDN16_CLKEN_OFFSET                 8
#define ADC1_CICDN16_CLKEN_LEN                    1
#define ADC2_CICDN16_CLKEN_OFFSET                 9
#define ADC2_CICDN16_CLKEN_LEN                    1
#define ADC3_CICDN16_CLKEN_OFFSET                 10
#define ADC3_CICDN16_CLKEN_LEN                    1
#define ADC4_CICDN16_CLKEN_OFFSET                 11
#define ADC4_CICDN16_CLKEN_LEN                    1
#define ADC5_CICDN16_CLKEN_OFFSET                 12
#define ADC5_CICDN16_CLKEN_LEN                    1
#define ADC6_CICDN16_CLKEN_OFFSET                 13
#define ADC6_CICDN16_CLKEN_LEN                    1
#define ADC7_CICDN16_CLKEN_OFFSET                 14
#define ADC7_CICDN16_CLKEN_LEN                    1
#define ADC8_CICDN16_CLKEN_OFFSET                 15
#define ADC8_CICDN16_CLKEN_LEN                    1
#define ADC1_CICDN16_4STAGE_CLKEN_OFFSET          16
#define ADC1_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC2_CICDN16_4STAGE_CLKEN_OFFSET          17
#define ADC2_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC3_CICDN16_4STAGE_CLKEN_OFFSET          18
#define ADC3_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC4_CICDN16_4STAGE_CLKEN_OFFSET          19
#define ADC4_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC5_CICDN16_4STAGE_CLKEN_OFFSET          20
#define ADC5_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC6_CICDN16_4STAGE_CLKEN_OFFSET          21
#define ADC6_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC7_CICDN16_4STAGE_CLKEN_OFFSET          22
#define ADC7_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC8_CICDN16_4STAGE_CLKEN_OFFSET          23
#define ADC8_CICDN16_4STAGE_CLKEN_LEN             1
#define ADC1_CIC_ULTR_CLKEN_OFFSET                24
#define ADC1_CIC_ULTR_CLKEN_LEN                   1
#define ADC2_CIC_ULTR_CLKEN_OFFSET                25
#define ADC2_CIC_ULTR_CLKEN_LEN                   1
#define ADC3_CIC_ULTR_CLKEN_OFFSET                26
#define ADC3_CIC_ULTR_CLKEN_LEN                   1
#define ADC4_CIC_ULTR_CLKEN_OFFSET                27
#define ADC4_CIC_ULTR_CLKEN_LEN                   1
#define DMIC1_CLKEN_OFFSET                        28
#define DMIC1_CLKEN_LEN                           1
#define DMIC2_CLKEN_OFFSET                        29
#define DMIC2_CLKEN_LEN                           1
#define DMIC3_CLKEN_OFFSET                        30
#define DMIC3_CLKEN_LEN                           1
#define DMIC4_CLKEN_OFFSET                        31
#define DMIC4_CLKEN_LEN                           1

#define CODEC_CLK_EN4_REG        (PAGE_SOCCODEC + 0x14)
#define ULTR_HPF_CLKEN_OFFSET                     0
#define ULTR_HPF_CLKEN_LEN                        1
#define DACL_SDM_CLKEN_OFFSET                     1
#define DACL_SDM_CLKEN_LEN                        1
#define DACR_SDM_CLKEN_OFFSET                     2
#define DACR_SDM_CLKEN_LEN                        1
#define SMARTPA_FEEDFORWARD_CLKEN_OFFSET          3
#define SMARTPA_FEEDFORWARD_CLKEN_LEN             1
#define AUDIOZOOM_CLKEN_OFFSET                    4
#define AUDIOZOOM_CLKEN_LEN                       1
#define SIDETONE_PGA_CLKEN_OFFSET                 5
#define SIDETONE_PGA_CLKEN_LEN                    1
#define BT_L_DN_AFIFO_CLKEN_OFFSET                6
#define BT_L_DN_AFIFO_CLKEN_LEN                   1
#define BT_R_DN_AFIFO_CLKEN_OFFSET                7
#define BT_R_DN_AFIFO_CLKEN_LEN                   1
#define SIF_MST_CLKEN_OFFSET                      8
#define SIF_MST_CLKEN_LEN                         1
#define NOISE_METER_PGA_CLKEN_OFFSET              9
#define NOISE_METER_PGA_CLKEN_LEN                 1
#define NOISE_METER_SRCDN_CLKEN_OFFSET            10
#define NOISE_METER_SRCDN_CLKEN_LEN               1
#define DACL_UP16_CLKEN_OFFSET                    11
#define DACL_UP16_CLKEN_LEN                       1
#define SPA_TEST_AFIFO_CLKEN_OFFSET               12
#define SPA_TEST_AFIFO_CLKEN_LEN                  1
#define DACL_SRCUP_ULTR_CLKEN_OFFSET              13
#define DACL_SRCUP_ULTR_CLKEN_LEN                 1

#define CODEC_SW_RST_N_REG       (PAGE_SOCCODEC + 0x18)
#define CODEC_SW_RST_N_OFFSET                     0
#define CODEC_SW_RST_N_LEN                        1
#define RST_4MIC_VOICE_ACCESS_IRQ_OFFSET          2
#define RST_4MIC_VOICE_ACCESS_IRQ_LEN             1
#define RST_4MIC_AUDIO_ACCESS_IRQ_OFFSET          3
#define RST_4MIC_AUDIO_ACCESS_IRQ_LEN             1
#define RST_4MIC_DN_ACCESS_IRQ_OFFSET             5
#define RST_4MIC_DN_ACCESS_IRQ_LEN                1
#define RST_8MIC_DN_ACCESS_IRQ_OFFSET             6
#define RST_8MIC_DN_ACCESS_IRQ_LEN                1
#define RST_8MIC_UP_ACCESS_IRQ_OFFSET             7
#define RST_8MIC_UP_ACCESS_IRQ_LEN                1
#define RST_8PA_DN_ACCESS_IRQ_OFFSET              8
#define RST_8PA_DN_ACCESS_IRQ_LEN                 1

#define I2S1_CTRL_REG            (PAGE_SOCCODEC + 0x1C)
#define FS_I2S1_OFFSET                            13
#define FS_I2S1_LEN                               3
#define I2S1_IF_TX_ENA_OFFSET                     16
#define I2S1_IF_TX_ENA_LEN                        1
#define I2S1_IF_RX_ENA_OFFSET                     17
#define I2S1_IF_RX_ENA_LEN                        1
#define I2S1_MST_SLV_OFFSET                       18
#define I2S1_MST_SLV_LEN                          1
#define I2S1_DIRECT_LOOP_OFFSET                   19
#define I2S1_DIRECT_LOOP_LEN                      2
#define I2S1_FUNC_MODE_OFFSET                     21
#define I2S1_FUNC_MODE_LEN                        3
#define I2S1_FRAME_MODE_OFFSET                    24
#define I2S1_FRAME_MODE_LEN                       1
#define I2S1_LRCLK_MODE_OFFSET                    25
#define I2S1_LRCLK_MODE_LEN                       1
#define I2S1_CHNNL_MODE_OFFSET                    26
#define I2S1_CHNNL_MODE_LEN                       1
#define I2S1_CODEC_DATA_FORMAT_OFFSET             27
#define I2S1_CODEC_DATA_FORMAT_LEN                1
#define I2S1_CODEC_IO_WORDLENGTH_OFFSET           28
#define I2S1_CODEC_IO_WORDLENGTH_LEN              2
#define I2S1_TX_CLK_SEL_OFFSET                    30
#define I2S1_TX_CLK_SEL_LEN                       1
#define I2S1_RX_CLK_SEL_OFFSET                    31
#define I2S1_RX_CLK_SEL_LEN                       1

#define I2S1_TDM_CTRL0_REG       (PAGE_SOCCODEC + 0x20)
#define S1_FIFO_CLR_OFFSET                        0
#define S1_FIFO_CLR_LEN                           1
#define S1_MEM_ICG_BP_EN_OFFSET                   1
#define S1_MEM_ICG_BP_EN_LEN                      1
#define S1_TDM_FRAME_MODE_OFFSET                  2
#define S1_TDM_FRAME_MODE_LEN                     3
#define S1_TDM_DIRECT_LOOP_OFFSET                 5
#define S1_TDM_DIRECT_LOOP_LEN                    2
#define S1_TDM_IF_EN_OFFSET                       7
#define S1_TDM_IF_EN_LEN                          1
#define S1_TDM_TX_CH3_SEL_OFFSET                  8
#define S1_TDM_TX_CH3_SEL_LEN                     2
#define S1_TDM_TX_CH2_SEL_OFFSET                  10
#define S1_TDM_TX_CH2_SEL_LEN                     2
#define S1_TDM_TX_CH1_SEL_OFFSET                  12
#define S1_TDM_TX_CH1_SEL_LEN                     2
#define S1_TDM_TX_CH0_SEL_OFFSET                  14
#define S1_TDM_TX_CH0_SEL_LEN                     2
#define S1_TDM_RX_SLOT_SEL_V0_OFFSET              16
#define S1_TDM_RX_SLOT_SEL_V0_LEN                 3
#define S1_TDM_RX_SLOT_SEL_I0_OFFSET              19
#define S1_TDM_RX_SLOT_SEL_I0_LEN                 3
#define S1_TDM_TX_CLK_SEL_OFFSET                  22
#define S1_TDM_TX_CLK_SEL_LEN                     1
#define S1_TDM_RX_CLK_SEL_OFFSET                  23
#define S1_TDM_RX_CLK_SEL_LEN                     1
#define S1_TDM_RX_SLOT_SEL_V1_OFFSET              24
#define S1_TDM_RX_SLOT_SEL_V1_LEN                 3
#define S1_TDM_RX_SLOT_SEL_I1_OFFSET              27
#define S1_TDM_RX_SLOT_SEL_I1_LEN                 3

#define I2S1_TDM_CTRL1_REG       (PAGE_SOCCODEC + 0x24)
#define S1_TDM_RX_SLOT_SEL_V2_OFFSET              0
#define S1_TDM_RX_SLOT_SEL_V2_LEN                 3
#define S1_TDM_RX_SLOT_SEL_I2_OFFSET              3
#define S1_TDM_RX_SLOT_SEL_I2_LEN                 3
#define S1_TDM_MODE_OFFSET                        6
#define S1_TDM_MODE_LEN                           2
#define S1_TDM_RX_SLOT_SEL_V3_OFFSET              8
#define S1_TDM_RX_SLOT_SEL_V3_LEN                 3
#define S1_TDM_RX_SLOT_SEL_I3_OFFSET              11
#define S1_TDM_RX_SLOT_SEL_I3_LEN                 3
#define S1_TDM_MST_SLV_OFFSET                     16
#define S1_TDM_MST_SLV_LEN                        1
#define S1_TDM_ERR_STAT_OFFSET                    18
#define S1_TDM_ERR_STAT_LEN                       2
#define S2_FIFO_CLR_OFFSET                        21
#define S2_FIFO_CLR_LEN                           1

#define I2S2_PCM_CTRL_REG        (PAGE_SOCCODEC + 0x28)
#define FS_I2S2_OFFSET                            13
#define FS_I2S2_LEN                               3
#define I2S2_IF_TX_ENA_OFFSET                     16
#define I2S2_IF_TX_ENA_LEN                        1
#define I2S2_IF_RX_ENA_OFFSET                     17
#define I2S2_IF_RX_ENA_LEN                        1
#define I2S2_MST_SLV_OFFSET                       18
#define I2S2_MST_SLV_LEN                          1
#define I2S2_DIRECT_LOOP_OFFSET                   19
#define I2S2_DIRECT_LOOP_LEN                      2
#define I2S2_FUNC_MODE_OFFSET                     21
#define I2S2_FUNC_MODE_LEN                        3
#define I2S2_FRAME_MODE_OFFSET                    24
#define I2S2_FRAME_MODE_LEN                       1
#define I2S2_LRCLK_MODE_OFFSET                    25
#define I2S2_LRCLK_MODE_LEN                       1
#define I2S2_CHNNL_MODE_OFFSET                    26
#define I2S2_CHNNL_MODE_LEN                       1
#define I2S2_CODEC_DATA_FORMAT_OFFSET             27
#define I2S2_CODEC_DATA_FORMAT_LEN                1
#define I2S2_CODEC_IO_WORDLENGTH_OFFSET           28
#define I2S2_CODEC_IO_WORDLENGTH_LEN              2
#define I2S2_TX_CLK_SEL_OFFSET                    30
#define I2S2_TX_CLK_SEL_LEN                       1
#define I2S2_RX_CLK_SEL_OFFSET                    31
#define I2S2_RX_CLK_SEL_LEN                       1

#define I2S1_TDM256_CTRL0_REG    (PAGE_SOCCODEC + 0x2C)
#define S1_TDM_FIFO_CLR_OFFSET                    4
#define S1_TDM_FIFO_CLR_LEN                       1
#define S1_TDM_MEM_ICG_BP_EN_OFFSET               5
#define S1_TDM_MEM_ICG_BP_EN_LEN                  1
#define S1_TDM_256_FRAME_MODE_OFFSET              6
#define S1_TDM_256_FRAME_MODE_LEN                 2
#define S1_TDM_256_DIRECT_LOOP_OFFSET             8
#define S1_TDM_256_DIRECT_LOOP_LEN                2
#define S1_TDM_RX_SLOT_SEL_V3_32_OFFSET           18
#define S1_TDM_RX_SLOT_SEL_V3_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_V2_32_OFFSET           19
#define S1_TDM_RX_SLOT_SEL_V2_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_V1_32_OFFSET           20
#define S1_TDM_RX_SLOT_SEL_V1_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_V0_32_OFFSET           21
#define S1_TDM_RX_SLOT_SEL_V0_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_I3_32_OFFSET           22
#define S1_TDM_RX_SLOT_SEL_I3_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_I2_32_OFFSET           23
#define S1_TDM_RX_SLOT_SEL_I2_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_I1_32_OFFSET           24
#define S1_TDM_RX_SLOT_SEL_I1_32_LEN              1
#define S1_TDM_RX_SLOT_SEL_I0_32_OFFSET           25
#define S1_TDM_RX_SLOT_SEL_I0_32_LEN              1
#define I2S1_TDM_SLOT_SEL_OFFSET                  26
#define I2S1_TDM_SLOT_SEL_LEN                     1
#define S1_TDM_256_MST_SLV_OFFSET                 27
#define S1_TDM_256_MST_SLV_LEN                    1
#define S1_TDM_256_IF_EN_OFFSET                   28
#define S1_TDM_256_IF_EN_LEN                      1

#define I2S1_TDM256_CTRL1_REG    (PAGE_SOCCODEC + 0x30)
#define S1_TDM_RX_SLOT_SEL_VB3_16_OFFSET          0
#define S1_TDM_RX_SLOT_SEL_VB3_16_LEN             2
#define S1_TDM_RX_SLOT_SEL_VB2_16_OFFSET          2
#define S1_TDM_RX_SLOT_SEL_VB2_16_LEN             2
#define S1_TDM_RX_SLOT_SEL_VB1_16_OFFSET          4
#define S1_TDM_RX_SLOT_SEL_VB1_16_LEN             2
#define S1_TDM_RX_SLOT_SEL_VB0_16_OFFSET          6
#define S1_TDM_RX_SLOT_SEL_VB0_16_LEN             2
#define S1_TDM_RX_SLOT_SEL_D3_16_OFFSET           8
#define S1_TDM_RX_SLOT_SEL_D3_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_D2_16_OFFSET           10
#define S1_TDM_RX_SLOT_SEL_D2_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_D1_16_OFFSET           12
#define S1_TDM_RX_SLOT_SEL_D1_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_D0_16_OFFSET           14
#define S1_TDM_RX_SLOT_SEL_D0_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_V3_16_OFFSET           16
#define S1_TDM_RX_SLOT_SEL_V3_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_V2_16_OFFSET           18
#define S1_TDM_RX_SLOT_SEL_V2_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_V1_16_OFFSET           20
#define S1_TDM_RX_SLOT_SEL_V1_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_V0_16_OFFSET           22
#define S1_TDM_RX_SLOT_SEL_V0_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_I3_16_OFFSET           24
#define S1_TDM_RX_SLOT_SEL_I3_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_I2_16_OFFSET           26
#define S1_TDM_RX_SLOT_SEL_I2_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_I1_16_OFFSET           28
#define S1_TDM_RX_SLOT_SEL_I1_16_LEN              2
#define S1_TDM_RX_SLOT_SEL_I0_16_OFFSET           30
#define S1_TDM_RX_SLOT_SEL_I0_16_LEN              2

#define PGA_THRE_CTRL0_REG       (PAGE_SOCCODEC + 0x34)
#define PGA_MIXER_THRE_CTRL0_OFFSET               0
#define PGA_MIXER_THRE_CTRL0_LEN                  32

#define PGA_THRE_CTRL1_REG       (PAGE_SOCCODEC + 0x38)
#define PGA_MIXER_THRE_CTRL1_OFFSET               0
#define PGA_MIXER_THRE_CTRL1_LEN                  32

#define PGA_THRE_CTRL2_REG       (PAGE_SOCCODEC + 0x3C)
#define PGA_MIXER_THRE_CTRL2_OFFSET               0
#define PGA_MIXER_THRE_CTRL2_LEN                  32

#define PGA_GAINOFFSET_CTRL0_REG (PAGE_SOCCODEC + 0x40)
#define CODEC_0_DN_PGA_GAINOFFSET_OFFSET          0
#define CODEC_0_DN_PGA_GAINOFFSET_LEN             8
#define CODEC_1_DN_PGA_GAINOFFSET_OFFSET          8
#define CODEC_1_DN_PGA_GAINOFFSET_LEN             8
#define CODEC_2_DN_PGA_GAINOFFSET_OFFSET          16
#define CODEC_2_DN_PGA_GAINOFFSET_LEN             8
#define AUDIO_0_DN_PGA_GAINOFFSET_OFFSET          24
#define AUDIO_0_DN_PGA_GAINOFFSET_LEN             8

#define PGA_GAINOFFSET_CTRL1_REG (PAGE_SOCCODEC + 0x44)
#define MIC3_UP_PGA_GAINOFFSET_OFFSET             0
#define MIC3_UP_PGA_GAINOFFSET_LEN                8
#define MIC4_UP_PGA_GAINOFFSET_OFFSET             8
#define MIC4_UP_PGA_GAINOFFSET_LEN                8
#define AUDIO_1_DN_PGA_GAINOFFSET_OFFSET          16
#define AUDIO_1_DN_PGA_GAINOFFSET_LEN             8
#define AUDIO_2_DN__PGA_GAINOFFSET_OFFSET         24
#define AUDIO_2_DN__PGA_GAINOFFSET_LEN            8

#define PGA_GAINOFFSET_CTRL2_REG (PAGE_SOCCODEC + 0x48)
#define AUDIO_UP_L_PGA_GAINOFFSET_OFFSET          0
#define AUDIO_UP_L_PGA_GAINOFFSET_LEN             8
#define AUDIO_UP_R_PGA_GAINOFFSET_OFFSET          8
#define AUDIO_UP_R_PGA_GAINOFFSET_LEN             8
#define VOICE_UP_L_PGA_GAINOFFSET_OFFSET          16
#define VOICE_UP_L_PGA_GAINOFFSET_LEN             8
#define VOICE_UP_R_PGA_GAINOFFSET_OFFSET          24
#define VOICE_UP_R_PGA_GAINOFFSET_LEN             8

#define PGA_GAINOFFSET_CTRL3_REG (PAGE_SOCCODEC + 0x4C)
#define I2S2_RX_L_PGA_GAINOFFSET_OFFSET           0
#define I2S2_RX_L_PGA_GAINOFFSET_LEN              8
#define I2S2_RX_R_PGA_GAINOFFSET_OFFSET           8
#define I2S2_RX_R_PGA_GAINOFFSET_LEN              8
#define SIDETONE_PGA_GAINOFFSET_OFFSET            16
#define SIDETONE_PGA_GAINOFFSET_LEN               8
#define AUDIOZOOM_UP_PGA_GAINOFFSET_OFFSET        24
#define AUDIOZOOM_UP_PGA_GAINOFFSET_LEN           8

#define PGA_GAINOFFSET_CTRL4_REG (PAGE_SOCCODEC + 0x50)
#define ULTR_DN_PGA_GAINOFFSET_OFFSET             0
#define ULTR_DN_PGA_GAINOFFSET_LEN                8
#define NOISE_METER_PGA_GAINOFFSET_OFFSET         8
#define NOISE_METER_PGA_GAINOFFSET_LEN            8

#define CODEC_0_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x54)
#define CODEC_0_DN_PGA_LINEAR_SEL_OFFSET          1
#define CODEC_0_DN_PGA_LINEAR_SEL_LEN             1
#define CODEC_0_DN_PGA_ZERO_NUM_OFFSET            2
#define CODEC_0_DN_PGA_ZERO_NUM_LEN               5
#define CODEC_0_DN_PGA_THRE_ID_OFFSET             7
#define CODEC_0_DN_PGA_THRE_ID_LEN                2
#define CODEC_0_DN_PGA_NOISE_EN_OFFSET            9
#define CODEC_0_DN_PGA_NOISE_EN_LEN               1
#define CODEC_0_DN_PGA_BYPASS_OFFSET              10
#define CODEC_0_DN_PGA_BYPASS_LEN                 1
#define CODEC_0_DN_PGA_FADE_OUT_OFFSET            11
#define CODEC_0_DN_PGA_FADE_OUT_LEN               5
#define CODEC_0_DN_PGA_FADE_IN_OFFSET             16
#define CODEC_0_DN_PGA_FADE_IN_LEN                5
#define CODEC_0_DN_PGA_CFG_OFFSET                 21
#define CODEC_0_DN_PGA_CFG_LEN                    3
#define CODEC_0_DN_PGA_GAIN_OFFSET                24
#define CODEC_0_DN_PGA_GAIN_LEN                   8

#define CODEC_1_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x58)
#define CODEC_1_DN_PGA_LINEAR_SEL_OFFSET          1
#define CODEC_1_DN_PGA_LINEAR_SEL_LEN             1
#define CODEC_1_DN_PGA_ZERO_NUM_OFFSET            2
#define CODEC_1_DN_PGA_ZERO_NUM_LEN               5
#define CODEC_1_DN_PGA_THRE_ID_OFFSET             7
#define CODEC_1_DN_PGA_THRE_ID_LEN                2
#define CODEC_1_DN_PGA_NOISE_EN_OFFSET            9
#define CODEC_1_DN_PGA_NOISE_EN_LEN               1
#define CODEC_1_DN_PGA_BYPASS_OFFSET              10
#define CODEC_1_DN_PGA_BYPASS_LEN                 1
#define CODEC_1_DN_PGA_FADE_OUT_OFFSET            11
#define CODEC_1_DN_PGA_FADE_OUT_LEN               5
#define CODEC_1_DN_PGA_FADE_IN_OFFSET             16
#define CODEC_1_DN_PGA_FADE_IN_LEN                5
#define CODEC_1_DN_PGA_CFG_OFFSET                 21
#define CODEC_1_DN_PGA_CFG_LEN                    3
#define CODEC_1_DN_PGA_GAIN_OFFSET                24
#define CODEC_1_DN_PGA_GAIN_LEN                   8

#define CODEC_2_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x5C)
#define CODEC_2_DN_PGA_LINEAR_SEL_OFFSET          1
#define CODEC_2_DN_PGA_LINEAR_SEL_LEN             1
#define CODEC_2_DN_PGA_ZERO_NUM_OFFSET            2
#define CODEC_2_DN_PGA_ZERO_NUM_LEN               5
#define CODEC_2_DN_PGA_THRE_ID_OFFSET             7
#define CODEC_2_DN_PGA_THRE_ID_LEN                2
#define CODEC_2_DN_PGA_NOISE_EN_OFFSET            9
#define CODEC_2_DN_PGA_NOISE_EN_LEN               1
#define CODEC_2_DN_PGA_BYPASS_OFFSET              10
#define CODEC_2_DN_PGA_BYPASS_LEN                 1
#define CODEC_2_DN_PGA_FADE_OUT_OFFSET            11
#define CODEC_2_DN_PGA_FADE_OUT_LEN               5
#define CODEC_2_DN_PGA_FADE_IN_OFFSET             16
#define CODEC_2_DN_PGA_FADE_IN_LEN                5
#define CODEC_2_DN_PGA_CFG_OFFSET                 21
#define CODEC_2_DN_PGA_CFG_LEN                    3
#define CODEC_2_DN_PGA_GAIN_OFFSET                24
#define CODEC_2_DN_PGA_GAIN_LEN                   8

#define AUDIO_0_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x60)
#define AUDIO_0_DN_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_0_DN_PGA_LINEAR_SEL_LEN             1
#define AUDIO_0_DN_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_0_DN_PGA_ZERO_NUM_LEN               5
#define AUDIO_0_DN_PGA_THRE_ID_OFFSET             7
#define AUDIO_0_DN_PGA_THRE_ID_LEN                2
#define AUDIO_0_DN_PGA_NOISE_EN_OFFSET            9
#define AUDIO_0_DN_PGA_NOISE_EN_LEN               1
#define AUDIO_0_DN_PGA_BYPASS_OFFSET              10
#define AUDIO_0_DN_PGA_BYPASS_LEN                 1
#define AUDIO_0_DN_PGA_FADE_OUT_OFFSET            11
#define AUDIO_0_DN_PGA_FADE_OUT_LEN               5
#define AUDIO_0_DN_PGA_FADE_IN_OFFSET             16
#define AUDIO_0_DN_PGA_FADE_IN_LEN                5
#define AUDIO_0_DN_PGA_CFG_OFFSET                 21
#define AUDIO_0_DN_PGA_CFG_LEN                    3
#define AUDIO_0_DN_PGA_GAIN_OFFSET                24
#define AUDIO_0_DN_PGA_GAIN_LEN                   8

#define AUDIO_1_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x64)
#define AUDIO_1_DN_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_1_DN_PGA_LINEAR_SEL_LEN             1
#define AUDIO_1_DN_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_1_DN_PGA_ZERO_NUM_LEN               5
#define AUDIO_1_DN_PGA_THRE_ID_OFFSET             7
#define AUDIO_1_DN_PGA_THRE_ID_LEN                2
#define AUDIO_1_DN_PGA_NOISE_EN_OFFSET            9
#define AUDIO_1_DN_PGA_NOISE_EN_LEN               1
#define AUDIO_1_DN_PGA_BYPASS_OFFSET              10
#define AUDIO_1_DN_PGA_BYPASS_LEN                 1
#define AUDIO_1_DN_PGA_FADE_OUT_OFFSET            11
#define AUDIO_1_DN_PGA_FADE_OUT_LEN               5
#define AUDIO_1_DN_PGA_FADE_IN_OFFSET             16
#define AUDIO_1_DN_PGA_FADE_IN_LEN                5
#define AUDIO_1_DN_PGA_CFG_OFFSET                 21
#define AUDIO_1_DN_PGA_CFG_LEN                    3
#define AUDIO_1_DN_PGA_GAIN_OFFSET                24
#define AUDIO_1_DN_PGA_GAIN_LEN                   8

#define AUDIO_2_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x68)
#define AUDIO_2_DN_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_2_DN_PGA_LINEAR_SEL_LEN             1
#define AUDIO_2_DN_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_2_DN_PGA_ZERO_NUM_LEN               5
#define AUDIO_2_DN_PGA_THRE_ID_OFFSET             7
#define AUDIO_2_DN_PGA_THRE_ID_LEN                2
#define AUDIO_2_DN_PGA_NOISE_EN_OFFSET            9
#define AUDIO_2_DN_PGA_NOISE_EN_LEN               1
#define AUDIO_2_DN_PGA_BYPASS_OFFSET              10
#define AUDIO_2_DN_PGA_BYPASS_LEN                 1
#define AUDIO_2_DN_PGA_FADE_OUT_OFFSET            11
#define AUDIO_2_DN_PGA_FADE_OUT_LEN               5
#define AUDIO_2_DN_PGA_FADE_IN_OFFSET             16
#define AUDIO_2_DN_PGA_FADE_IN_LEN                5
#define AUDIO_2_DN_PGA_CFG_OFFSET                 21
#define AUDIO_2_DN_PGA_CFG_LEN                    3
#define AUDIO_2_DN_PGA_GAIN_OFFSET                24
#define AUDIO_2_DN_PGA_GAIN_LEN                   8

#define SIDETONE_PGA_CTRL_REG    (PAGE_SOCCODEC + 0x6C)
#define SIDETONE_PGA_LINEAR_SEL_OFFSET            1
#define SIDETONE_PGA_LINEAR_SEL_LEN               1
#define SIDETONE_PGA_ZERO_NUM_OFFSET              2
#define SIDETONE_PGA_ZERO_NUM_LEN                 5
#define SIDETONE_PGA_THRE_ID_OFFSET               7
#define SIDETONE_PGA_THRE_ID_LEN                  2
#define SIDETONE_PGA_NOISE_EN_OFFSET              9
#define SIDETONE_PGA_NOISE_EN_LEN                 1
#define SIDETONE_PGA_BYPASS_OFFSET                10
#define SIDETONE_PGA_BYPASS_LEN                   1
#define SIDETONE_PGA_FADE_OUT_OFFSET              11
#define SIDETONE_PGA_FADE_OUT_LEN                 5
#define SIDETONE_PGA_FADE_IN_OFFSET               16
#define SIDETONE_PGA_FADE_IN_LEN                  5
#define SIDETONE_PGA_CFG_OFFSET                   21
#define SIDETONE_PGA_CFG_LEN                      3
#define SIDETONE_PGA_GAIN_OFFSET                  24
#define SIDETONE_PGA_GAIN_LEN                     8

#define AUDIO_L_UP_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x70)
#define AUDIO_L_UP_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_L_UP_PGA_LINEAR_SEL_LEN             1
#define AUDIO_L_UP_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_L_UP_PGA_ZERO_NUM_LEN               5
#define AUDIO_L_UP_PGA_THRE_ID_OFFSET             7
#define AUDIO_L_UP_PGA_THRE_ID_LEN                2
#define AUDIO_L_UP_PGA_NOISE_EN_OFFSET            9
#define AUDIO_L_UP_PGA_NOISE_EN_LEN               1
#define AUDIO_L_UP_PGA_BYPASS_OFFSET              10
#define AUDIO_L_UP_PGA_BYPASS_LEN                 1
#define AUDIO_L_UP_PGA_FADE_OUT_OFFSET            11
#define AUDIO_L_UP_PGA_FADE_OUT_LEN               5
#define AUDIO_L_UP_PGA_FADE_IN_OFFSET             16
#define AUDIO_L_UP_PGA_FADE_IN_LEN                5
#define AUDIO_L_UP_PGA_CFG_OFFSET                 21
#define AUDIO_L_UP_PGA_CFG_LEN                    3
#define AUDIO_L_UP_PGA_GAIN_OFFSET                24
#define AUDIO_L_UP_PGA_GAIN_LEN                   8

#define AUDIO_R_UP_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x74)
#define AUDIO_R_UP_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_R_UP_PGA_LINEAR_SEL_LEN             1
#define AUDIO_R_UP_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_R_UP_PGA_ZERO_NUM_LEN               5
#define AUDIO_R_UP_PGA_THRE_ID_OFFSET             7
#define AUDIO_R_UP_PGA_THRE_ID_LEN                2
#define AUDIO_R_UP_PGA_NOISE_EN_OFFSET            9
#define AUDIO_R_UP_PGA_NOISE_EN_LEN               1
#define AUDIO_R_UP_PGA_BYPASS_OFFSET              10
#define AUDIO_R_UP_PGA_BYPASS_LEN                 1
#define AUDIO_R_UP_PGA_FADE_OUT_OFFSET            11
#define AUDIO_R_UP_PGA_FADE_OUT_LEN               5
#define AUDIO_R_UP_PGA_FADE_IN_OFFSET             16
#define AUDIO_R_UP_PGA_FADE_IN_LEN                5
#define AUDIO_R_UP_PGA_CFG_OFFSET                 21
#define AUDIO_R_UP_PGA_CFG_LEN                    3
#define AUDIO_R_UP_PGA_GAIN_OFFSET                24
#define AUDIO_R_UP_PGA_GAIN_LEN                   8

#define VOICE_L_UP_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x78)
#define VOICE_L_UP_PGA_LINEAR_SEL_OFFSET          1
#define VOICE_L_UP_PGA_LINEAR_SEL_LEN             1
#define VOICE_L_UP_PGA_ZERO_NUM_OFFSET            2
#define VOICE_L_UP_PGA_ZERO_NUM_LEN               5
#define VOICE_L_UP_PGA_THRE_ID_OFFSET             7
#define VOICE_L_UP_PGA_THRE_ID_LEN                2
#define VOICE_L_UP_PGA_NOISE_EN_OFFSET            9
#define VOICE_L_UP_PGA_NOISE_EN_LEN               1
#define VOICE_L_UP_PGA_BYPASS_OFFSET              10
#define VOICE_L_UP_PGA_BYPASS_LEN                 1
#define VOICE_L_UP_PGA_FADE_OUT_OFFSET            11
#define VOICE_L_UP_PGA_FADE_OUT_LEN               5
#define VOICE_L_UP_PGA_FADE_IN_OFFSET             16
#define VOICE_L_UP_PGA_FADE_IN_LEN                5
#define VOICE_L_UP_PGA_CFG_OFFSET                 21
#define VOICE_L_UP_PGA_CFG_LEN                    3
#define VOICE_L_UP_PGA_GAIN_OFFSET                24
#define VOICE_L_UP_PGA_GAIN_LEN                   8

#define VOICE_R_UP_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x7C)
#define VOICE_R_UP_PGA_LINEAR_SEL_OFFSET          1
#define VOICE_R_UP_PGA_LINEAR_SEL_LEN             1
#define VOICE_R_UP_PGA_ZERO_NUM_OFFSET            2
#define VOICE_R_UP_PGA_ZERO_NUM_LEN               5
#define VOICE_R_UP_PGA_THRE_ID_OFFSET             7
#define VOICE_R_UP_PGA_THRE_ID_LEN                2
#define VOICE_R_UP_PGA_NOISE_EN_OFFSET            9
#define VOICE_R_UP_PGA_NOISE_EN_LEN               1
#define VOICE_R_UP_PGA_BYPASS_OFFSET              10
#define VOICE_R_UP_PGA_BYPASS_LEN                 1
#define VOICE_R_UP_PGA_FADE_OUT_OFFSET            11
#define VOICE_R_UP_PGA_FADE_OUT_LEN               5
#define VOICE_R_UP_PGA_FADE_IN_OFFSET             16
#define VOICE_R_UP_PGA_FADE_IN_LEN                5
#define VOICE_R_UP_PGA_CFG_OFFSET                 21
#define VOICE_R_UP_PGA_CFG_LEN                    3
#define VOICE_R_UP_PGA_GAIN_OFFSET                24
#define VOICE_R_UP_PGA_GAIN_LEN                   8

#define MIC3_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x80)
#define MIC3_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC3_UP_PGA_LINEAR_SEL_LEN                1
#define MIC3_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC3_UP_PGA_ZERO_NUM_LEN                  5
#define MIC3_UP_PGA_THRE_ID_OFFSET                7
#define MIC3_UP_PGA_THRE_ID_LEN                   2
#define MIC3_UP_PGA_NOISE_EN_OFFSET               9
#define MIC3_UP_PGA_NOISE_EN_LEN                  1
#define MIC3_UP_PGA_BYPASS_OFFSET                 10
#define MIC3_UP_PGA_BYPASS_LEN                    1
#define MIC3_UP_PGA_FADE_OUT_OFFSET               11
#define MIC3_UP_PGA_FADE_OUT_LEN                  5
#define MIC3_UP_PGA_FADE_IN_OFFSET                16
#define MIC3_UP_PGA_FADE_IN_LEN                   5
#define MIC3_UP_PGA_CFG_OFFSET                    21
#define MIC3_UP_PGA_CFG_LEN                       3
#define MIC3_UP_PGA_GAIN_OFFSET                   24
#define MIC3_UP_PGA_GAIN_LEN                      8

#define MIC4_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x84)
#define MIC4_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC4_UP_PGA_LINEAR_SEL_LEN                1
#define MIC4_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC4_UP_PGA_ZERO_NUM_LEN                  5
#define MIC4_UP_PGA_THRE_ID_OFFSET                7
#define MIC4_UP_PGA_THRE_ID_LEN                   2
#define MIC4_UP_PGA_NOISE_EN_OFFSET               9
#define MIC4_UP_PGA_NOISE_EN_LEN                  1
#define MIC4_UP_PGA_BYPASS_OFFSET                 10
#define MIC4_UP_PGA_BYPASS_LEN                    1
#define MIC4_UP_PGA_FADE_OUT_OFFSET               11
#define MIC4_UP_PGA_FADE_OUT_LEN                  5
#define MIC4_UP_PGA_FADE_IN_OFFSET                16
#define MIC4_UP_PGA_FADE_IN_LEN                   5
#define MIC4_UP_PGA_CFG_OFFSET                    21
#define MIC4_UP_PGA_CFG_LEN                       3
#define MIC4_UP_PGA_GAIN_OFFSET                   24
#define MIC4_UP_PGA_GAIN_LEN                      8

#define MIC5_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x88)
#define MIC5_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC5_UP_PGA_LINEAR_SEL_LEN                1
#define MIC5_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC5_UP_PGA_ZERO_NUM_LEN                  5
#define MIC5_UP_PGA_THRE_ID_OFFSET                7
#define MIC5_UP_PGA_THRE_ID_LEN                   2
#define MIC5_UP_PGA_NOISE_EN_OFFSET               9
#define MIC5_UP_PGA_NOISE_EN_LEN                  1
#define MIC5_UP_PGA_BYPASS_OFFSET                 10
#define MIC5_UP_PGA_BYPASS_LEN                    1
#define MIC5_UP_PGA_FADE_OUT_OFFSET               11
#define MIC5_UP_PGA_FADE_OUT_LEN                  5
#define MIC5_UP_PGA_FADE_IN_OFFSET                16
#define MIC5_UP_PGA_FADE_IN_LEN                   5
#define MIC5_UP_PGA_CFG_OFFSET                    21
#define MIC5_UP_PGA_CFG_LEN                       3
#define MIC5_UP_PGA_GAIN_OFFSET                   24
#define MIC5_UP_PGA_GAIN_LEN                      8

#define MIC6_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x8C)
#define MIC6_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC6_UP_PGA_LINEAR_SEL_LEN                1
#define MIC6_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC6_UP_PGA_ZERO_NUM_LEN                  5
#define MIC6_UP_PGA_THRE_ID_OFFSET                7
#define MIC6_UP_PGA_THRE_ID_LEN                   2
#define MIC6_UP_PGA_NOISE_EN_OFFSET               9
#define MIC6_UP_PGA_NOISE_EN_LEN                  1
#define MIC6_UP_PGA_BYPASS_OFFSET                 10
#define MIC6_UP_PGA_BYPASS_LEN                    1
#define MIC6_UP_PGA_FADE_OUT_OFFSET               11
#define MIC6_UP_PGA_FADE_OUT_LEN                  5
#define MIC6_UP_PGA_FADE_IN_OFFSET                16
#define MIC6_UP_PGA_FADE_IN_LEN                   5
#define MIC6_UP_PGA_CFG_OFFSET                    21
#define MIC6_UP_PGA_CFG_LEN                       3
#define MIC6_UP_PGA_GAIN_OFFSET                   24
#define MIC6_UP_PGA_GAIN_LEN                      8

#define MIC7_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x90)
#define MIC7_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC7_UP_PGA_LINEAR_SEL_LEN                1
#define MIC7_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC7_UP_PGA_ZERO_NUM_LEN                  5
#define MIC7_UP_PGA_THRE_ID_OFFSET                7
#define MIC7_UP_PGA_THRE_ID_LEN                   2
#define MIC7_UP_PGA_NOISE_EN_OFFSET               9
#define MIC7_UP_PGA_NOISE_EN_LEN                  1
#define MIC7_UP_PGA_BYPASS_OFFSET                 10
#define MIC7_UP_PGA_BYPASS_LEN                    1
#define MIC7_UP_PGA_FADE_OUT_OFFSET               11
#define MIC7_UP_PGA_FADE_OUT_LEN                  5
#define MIC7_UP_PGA_FADE_IN_OFFSET                16
#define MIC7_UP_PGA_FADE_IN_LEN                   5
#define MIC7_UP_PGA_CFG_OFFSET                    21
#define MIC7_UP_PGA_CFG_LEN                       3
#define MIC7_UP_PGA_GAIN_OFFSET                   24
#define MIC7_UP_PGA_GAIN_LEN                      8

#define MIC8_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0x94)
#define MIC8_UP_PGA_LINEAR_SEL_OFFSET             1
#define MIC8_UP_PGA_LINEAR_SEL_LEN                1
#define MIC8_UP_PGA_ZERO_NUM_OFFSET               2
#define MIC8_UP_PGA_ZERO_NUM_LEN                  5
#define MIC8_UP_PGA_THRE_ID_OFFSET                7
#define MIC8_UP_PGA_THRE_ID_LEN                   2
#define MIC8_UP_PGA_NOISE_EN_OFFSET               9
#define MIC8_UP_PGA_NOISE_EN_LEN                  1
#define MIC8_UP_PGA_BYPASS_OFFSET                 10
#define MIC8_UP_PGA_BYPASS_LEN                    1
#define MIC8_UP_PGA_FADE_OUT_OFFSET               11
#define MIC8_UP_PGA_FADE_OUT_LEN                  5
#define MIC8_UP_PGA_FADE_IN_OFFSET                16
#define MIC8_UP_PGA_FADE_IN_LEN                   5
#define MIC8_UP_PGA_CFG_OFFSET                    21
#define MIC8_UP_PGA_CFG_LEN                       3
#define MIC8_UP_PGA_GAIN_OFFSET                   24
#define MIC8_UP_PGA_GAIN_LEN                      8

#define I2S2_L_RX_PGA_CTRL_REG   (PAGE_SOCCODEC + 0x98)
#define I2S2_L_RX_PGA_LINEAR_SEL_OFFSET           1
#define I2S2_L_RX_PGA_LINEAR_SEL_LEN              1
#define I2S2_L_RX_PGA_ZERO_NUM_OFFSET             2
#define I2S2_L_RX_PGA_ZERO_NUM_LEN                5
#define I2S2_L_RX_PGA_THRE_ID_OFFSET              7
#define I2S2_L_RX_PGA_THRE_ID_LEN                 2
#define I2S2_L_RX_PGA_NOISE_EN_OFFSET             9
#define I2S2_L_RX_PGA_NOISE_EN_LEN                1
#define I2S2_L_RX_PGA_BYPASS_OFFSET               10
#define I2S2_L_RX_PGA_BYPASS_LEN                  1
#define I2S2_L_RX_PGA_FADE_OUT_OFFSET             11
#define I2S2_L_RX_PGA_FADE_OUT_LEN                5
#define I2S2_L_RX_PGA_FADE_IN_OFFSET              16
#define I2S2_L_RX_PGA_FADE_IN_LEN                 5
#define I2S2_L_RX_PGA_CFG_OFFSET                  21
#define I2S2_L_RX_PGA_CFG_LEN                     3
#define I2S2_L_RX_PGA_GAIN_OFFSET                 24
#define I2S2_L_RX_PGA_GAIN_LEN                    8

#define I2S2_R_RX_PGA_CTRL_REG   (PAGE_SOCCODEC + 0x9C)
#define I2S2_R_RX_PGA_LINEAR_SEL_OFFSET           1
#define I2S2_R_RX_PGA_LINEAR_SEL_LEN              1
#define I2S2_R_RX_PGA_ZERO_NUM_OFFSET             2
#define I2S2_R_RX_PGA_ZERO_NUM_LEN                5
#define I2S2_R_RX_PGA_THRE_ID_OFFSET              7
#define I2S2_R_RX_PGA_THRE_ID_LEN                 2
#define I2S2_R_RX_PGA_NOISE_EN_OFFSET             9
#define I2S2_R_RX_PGA_NOISE_EN_LEN                1
#define I2S2_R_RX_PGA_BYPASS_OFFSET               10
#define I2S2_R_RX_PGA_BYPASS_LEN                  1
#define I2S2_R_RX_PGA_FADE_OUT_OFFSET             11
#define I2S2_R_RX_PGA_FADE_OUT_LEN                5
#define I2S2_R_RX_PGA_FADE_IN_OFFSET              16
#define I2S2_R_RX_PGA_FADE_IN_LEN                 5
#define I2S2_R_RX_PGA_CFG_OFFSET                  21
#define I2S2_R_RX_PGA_CFG_LEN                     3
#define I2S2_R_RX_PGA_GAIN_OFFSET                 24
#define I2S2_R_RX_PGA_GAIN_LEN                    8

#define ULTR_DN_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xA0)
#define ULTR_DN_PGA_LINEAR_SEL_OFFSET             1
#define ULTR_DN_PGA_LINEAR_SEL_LEN                1
#define ULTR_DN_PGA_ZERO_NUM_OFFSET               2
#define ULTR_DN_PGA_ZERO_NUM_LEN                  5
#define ULTR_DN_PGA_THRE_ID_OFFSET                7
#define ULTR_DN_PGA_THRE_ID_LEN                   2
#define ULTR_DN_PGA_NOISE_EN_OFFSET               9
#define ULTR_DN_PGA_NOISE_EN_LEN                  1
#define ULTR_DN_PGA_BYPASS_OFFSET                 10
#define ULTR_DN_PGA_BYPASS_LEN                    1
#define ULTR_DN_PGA_FADE_OUT_OFFSET               11
#define ULTR_DN_PGA_FADE_OUT_LEN                  5
#define ULTR_DN_PGA_FADE_IN_OFFSET                16
#define ULTR_DN_PGA_FADE_IN_LEN                   5
#define ULTR_DN_PGA_CFG_OFFSET                    21
#define ULTR_DN_PGA_CFG_LEN                       3
#define ULTR_DN_PGA_GAIN_OFFSET                   24
#define ULTR_DN_PGA_GAIN_LEN                      8

#define AUDIOZOOM_PGA_CTRL_REG   (PAGE_SOCCODEC + 0xA4)
#define AUDIOZOOM_PGA_LINEAR_SEL_OFFSET           1
#define AUDIOZOOM_PGA_LINEAR_SEL_LEN              1
#define AUDIOZOOM_PGA_ZERO_NUM_OFFSET             2
#define AUDIOZOOM_PGA_ZERO_NUM_LEN                5
#define AUDIOZOOM_PGA_THRE_ID_OFFSET              7
#define AUDIOZOOM_PGA_THRE_ID_LEN                 2
#define AUDIOZOOM_PGA_NOISE_EN_OFFSET             9
#define AUDIOZOOM_PGA_NOISE_EN_LEN                1
#define AUDIOZOOM_PGA_BYPASS_OFFSET               10
#define AUDIOZOOM_PGA_BYPASS_LEN                  1
#define AUDIOZOOM_PGA_FADE_OUT_OFFSET             11
#define AUDIOZOOM_PGA_FADE_OUT_LEN                5
#define AUDIOZOOM_PGA_FADE_IN_OFFSET              16
#define AUDIOZOOM_PGA_FADE_IN_LEN                 5
#define AUDIOZOOM_PGA_CFG_OFFSET                  21
#define AUDIOZOOM_PGA_CFG_LEN                     3
#define AUDIOZOOM_PGA_GAIN_OFFSET                 24
#define AUDIOZOOM_PGA_GAIN_LEN                    8

#define NOISE_METER_PGA_CTRL_REG (PAGE_SOCCODEC + 0xA8)
#define NOISE_METER_PGA_LINEAR_SEL_OFFSET         1
#define NOISE_METER_PGA_LINEAR_SEL_LEN            1
#define NOISE_METER_PGA_ZERO_NUM_OFFSET           2
#define NOISE_METER_PGA_ZERO_NUM_LEN              5
#define NOISE_METER_PGA_THRE_ID_OFFSET            7
#define NOISE_METER_PGA_THRE_ID_LEN               2
#define NOISE_METER_PGA_NOISE_EN_OFFSET           9
#define NOISE_METER_PGA_NOISE_EN_LEN              1
#define NOISE_METER_PGA_BYPASS_OFFSET             10
#define NOISE_METER_PGA_BYPASS_LEN                1
#define NOISE_METER_PGA_FADE_OUT_OFFSET           11
#define NOISE_METER_PGA_FADE_OUT_LEN              5
#define NOISE_METER_PGA_FADE_IN_OFFSET            16
#define NOISE_METER_PGA_FADE_IN_LEN               5
#define NOISE_METER_PGA_CFG_OFFSET                21
#define NOISE_METER_PGA_CFG_LEN                   3
#define NOISE_METER_PGA_GAIN_OFFSET               24
#define NOISE_METER_PGA_GAIN_LEN                  8

#define ADC0_PGA_GAIN_OFFSET_REG (PAGE_SOCCODEC + 0xAC)
#define ADC1_PGA_GAIN_OFFSET_OFFSET               0
#define ADC1_PGA_GAIN_OFFSET_LEN                  8
#define ADC2_PGA_GAIN_OFFSET_OFFSET               8
#define ADC2_PGA_GAIN_OFFSET_LEN                  8
#define ADC3_PGA_GAIN_OFFSET_OFFSET               16
#define ADC3_PGA_GAIN_OFFSET_LEN                  8
#define ADC4_PGA_GAIN_OFFSET_OFFSET               24
#define ADC4_PGA_GAIN_OFFSET_LEN                  8

#define ADC1_PGA_GAIN_OFFSET_REG (PAGE_SOCCODEC + 0xB0)
#define ADC5_PGA_GAIN_OFFSET_OFFSET               0
#define ADC5_PGA_GAIN_OFFSET_LEN                  8
#define ADC6_PGA_GAIN_OFFSET_OFFSET               8
#define ADC6_PGA_GAIN_OFFSET_LEN                  8
#define ADC7_PGA_GAIN_OFFSET_OFFSET               16
#define ADC7_PGA_GAIN_OFFSET_LEN                  8
#define ADC8_PGA_GAIN_OFFSET_OFFSET               24
#define ADC8_PGA_GAIN_OFFSET_LEN                  8

#define ADC1_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xB4)
#define ADC1_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC1_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC1_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC1_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC1_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC1_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC1_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC1_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC1_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC1_UP_PGA_0P5_BYPASS_LEN                1
#define ADC1_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC1_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC1_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC1_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC1_UP_PGA_0P5_CFG_OFFSET                21
#define ADC1_UP_PGA_0P5_CFG_LEN                   3
#define ADC1_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC1_UP_PGA_0P5_GAIN_LEN                  8

#define ADC2_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xB8)
#define ADC2_UP_PGA_LINEAR_SEL_OFFSET             1
#define ADC2_UP_PGA_LINEAR_SEL_LEN                1
#define ADC2_UP_PGA_ZERO_NUM_OFFSET               2
#define ADC2_UP_PGA_ZERO_NUM_LEN                  5
#define ADC2_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC2_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC2_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC2_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC2_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC2_UP_PGA_0P5_BYPASS_LEN                1
#define ADC2_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC2_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC2_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC2_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC2_UP_PGA_0P5_CFG_OFFSET                21
#define ADC2_UP_PGA_0P5_CFG_LEN                   3
#define ADC2_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC2_UP_PGA_0P5_GAIN_LEN                  8

#define ADC3_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xBC)
#define ADC3_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC3_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC3_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC3_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC3_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC3_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC3_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC3_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC3_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC3_UP_PGA_0P5_BYPASS_LEN                1
#define ADC3_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC3_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC3_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC3_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC3_UP_PGA_0P5_CFG_OFFSET                21
#define ADC3_UP_PGA_0P5_CFG_LEN                   3
#define ADC3_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC3_UP_PGA_0P5_GAIN_LEN                  8

#define ADC4_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xC0)
#define ADC4_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC4_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC4_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC4_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC4_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC4_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC4_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC4_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC4_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC4_UP_PGA_0P5_BYPASS_LEN                1
#define ADC4_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC4_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC4_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC4_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC4_UP_PGA_0P5_CFG_OFFSET                21
#define ADC4_UP_PGA_0P5_CFG_LEN                   3
#define ADC4_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC4_UP_PGA_0P5_GAIN_LEN                  8

#define ADC5_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xC4)
#define ADC5_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC5_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC5_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC5_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC5_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC5_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC5_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC5_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC5_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC5_UP_PGA_0P5_BYPASS_LEN                1
#define ADC5_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC5_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC5_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC5_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC5_UP_PGA_0P5_CFG_OFFSET                21
#define ADC5_UP_PGA_0P5_CFG_LEN                   3
#define ADC5_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC5_UP_PGA_0P5_GAIN_LEN                  8

#define ADC6_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xC8)
#define ADC6_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC6_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC6_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC6_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC6_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC6_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC6_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC6_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC6_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC6_UP_PGA_0P5_BYPASS_LEN                1
#define ADC6_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC6_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC6_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC6_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC6_UP_PGA_0P5_CFG_OFFSET                21
#define ADC6_UP_PGA_0P5_CFG_LEN                   3
#define ADC6_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC6_UP_PGA_0P5_GAIN_LEN                  8

#define ADC7_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xCC)
#define ADC7_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC7_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC7_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC7_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC7_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC7_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC7_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC7_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC7_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC7_UP_PGA_0P5_BYPASS_LEN                1
#define ADC7_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC7_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC7_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC7_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC7_UP_PGA_0P5_CFG_OFFSET                21
#define ADC7_UP_PGA_0P5_CFG_LEN                   3
#define ADC7_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC7_UP_PGA_0P5_GAIN_LEN                  8

#define ADC8_UP_PGA_CTRL_REG     (PAGE_SOCCODEC + 0xD0)
#define ADC8_UP_PGA_0P5_LINEAR_SEL_OFFSET         1
#define ADC8_UP_PGA_0P5_LINEAR_SEL_LEN            1
#define ADC8_UP_PGA_0P5_ZERO_NUM_OFFSET           2
#define ADC8_UP_PGA_0P5_ZERO_NUM_LEN              5
#define ADC8_UP_PGA_0P5_THRE_ID_OFFSET            7
#define ADC8_UP_PGA_0P5_THRE_ID_LEN               2
#define ADC8_UP_PGA_0P5_NOISE_EN_OFFSET           9
#define ADC8_UP_PGA_0P5_NOISE_EN_LEN              1
#define ADC8_UP_PGA_0P5_BYPASS_OFFSET             10
#define ADC8_UP_PGA_0P5_BYPASS_LEN                1
#define ADC8_UP_PGA_0P5_FADE_OUT_OFFSET           11
#define ADC8_UP_PGA_0P5_FADE_OUT_LEN              5
#define ADC8_UP_PGA_0P5_FADE_IN_OFFSET            16
#define ADC8_UP_PGA_0P5_FADE_IN_LEN               5
#define ADC8_UP_PGA_0P5_CFG_OFFSET                21
#define ADC8_UP_PGA_0P5_CFG_LEN                   3
#define ADC8_UP_PGA_0P5_GAIN_OFFSET               24
#define ADC8_UP_PGA_0P5_GAIN_LEN                  8

#define SRCUP_CTRL0_REG          (PAGE_SOCCODEC + 0xD4)
#define CODEC_0_DN_SRCUP_SRC_MODE_OFFSET          0
#define CODEC_0_DN_SRCUP_SRC_MODE_LEN             3
#define CODEC_0_DN_SRCUP_FIFO_CLR_OFFSET          3
#define CODEC_0_DN_SRCUP_FIFO_CLR_LEN             1
#define CODEC_1_DN_SRCUP_SRC_MODE_OFFSET          4
#define CODEC_1_DN_SRCUP_SRC_MODE_LEN             3
#define CODEC_1_DN_SRCUP_FIFO_CLR_OFFSET          7
#define CODEC_1_DN_SRCUP_FIFO_CLR_LEN             1
#define CODEC_2_DN_SRCUP_SRC_MODE_OFFSET          8
#define CODEC_2_DN_SRCUP_SRC_MODE_LEN             3
#define CODEC_2_DN_SRCUP_FIFO_CLR_OFFSET          11
#define CODEC_2_DN_SRCUP_FIFO_CLR_LEN             1
#define AUDIO_0_DN_SRCUP_SRC_MODE_OFFSET          12
#define AUDIO_0_DN_SRCUP_SRC_MODE_LEN             3
#define AUDIO_0_DN_SRCUP_FIFO_CLR_OFFSET          15
#define AUDIO_0_DN_SRCUP_FIFO_CLR_LEN             1
#define AUDIO_1_SRCUP_SRC_MODE_OFFSET             16
#define AUDIO_1_SRCUP_SRC_MODE_LEN                3
#define AUDIO_1_UP_SRCUP_FIFO_CLR_OFFSET          19
#define AUDIO_1_UP_SRCUP_FIFO_CLR_LEN             1
#define AUDIO_2_SRCUP_SRC_MODE_OFFSET             20
#define AUDIO_2_SRCUP_SRC_MODE_LEN                3
#define AUDIO_2_UP_SRCUP_FIFO_CLR_OFFSET          23
#define AUDIO_2_UP_SRCUP_FIFO_CLR_LEN             1
#define DACL_SRCUP_SRC_MODE_OFFSET                24
#define DACL_SRCUP_SRC_MODE_LEN                   3
#define DACL_SRCUP_FIFO_CLR_OFFSET                27
#define DACL_SRCUP_FIFO_CLR_LEN                   1
#define DACR_MIXER4_SRCUP_SRC_MODE_OFFSET         28
#define DACR_MIXER4_SRCUP_SRC_MODE_LEN            3
#define DACR_MIXER4_SRCUP_FIFO_CLR_OFFSET         31
#define DACR_MIXER4_SRCUP_FIFO_CLR_LEN            1

#define SRCUP_CTRL1_REG          (PAGE_SOCCODEC + 0xD8)
#define ULTR_DN_SRCUP_SRC_MODE_OFFSET             0
#define ULTR_DN_SRCUP_SRC_MODE_LEN                3
#define ULTR_DN_SRCUP_FIFO_CLR_OFFSET             3
#define ULTR_DN_SRCUP_FIFO_CLR_LEN                1
#define I2S2_RX_L_SRCUP_SRC_MODE_OFFSET           4
#define I2S2_RX_L_SRCUP_SRC_MODE_LEN              3
#define I2S2_RX_L_SRCUP_FIFO_CLR_OFFSET           7
#define I2S2_RX_L_SRCUP_FIFO_CLR_LEN              1
#define I2S2_RX_R_SRCUP_SRC_MODE_OFFSET           8
#define I2S2_RX_R_SRCUP_SRC_MODE_LEN              3
#define I2S2_RX_R_SRCUP_FIFO_CLR_OFFSET           11
#define I2S2_RX_R_SRCUP_FIFO_CLR_LEN              1

#define SRCDN_CTRL0_REG          (PAGE_SOCCODEC + 0xDC)
#define AUDIO_L_UP_SRCDN_SRC_MODE_OFFSET          0
#define AUDIO_L_UP_SRCDN_SRC_MODE_LEN             3
#define AUDIO_L_UP_SRCDN_FIFO_CLR_OFFSET          3
#define AUDIO_L_UP_SRCDN_FIFO_CLR_LEN             1
#define AUDIO_R_UP_SRCDN_SRC_MODE_OFFSET          4
#define AUDIO_R_UP_SRCDN_SRC_MODE_LEN             3
#define AUDIO_R_UP_SRCDN_FIFO_CLR_OFFSET          7
#define AUDIO_R_UP_SRCDN_FIFO_CLR_LEN             1
#define VOICE_L_UP_SRCDN_SRC_MODE_OFFSET          8
#define VOICE_L_UP_SRCDN_SRC_MODE_LEN             3
#define VOICE_L_UP_SRCDN_FIFO_CLR_OFFSET          11
#define VOICE_L_UP_SRCDN_FIFO_CLR_LEN             1
#define VOICE_R_UP_SRCDN_SRC_MODE_OFFSET          12
#define VOICE_R_UP_SRCDN_SRC_MODE_LEN             3
#define VOICE_R_UP_SRCDN_FIFO_CLR_OFFSET          15
#define VOICE_R_UP_SRCDN_FIFO_CLR_LEN             1
#define MIC3_UP_SRCDN_SRC_MODE_OFFSET             16
#define MIC3_UP_SRCDN_SRC_MODE_LEN                3
#define MIC3_UP_SRCDN_FIFO_CLR_OFFSET             19
#define MIC3_UP_SRCDN_FIFO_CLR_LEN                1
#define MIC4_UP_SRCDN_SRC_MODE_OFFSET             20
#define MIC4_UP_SRCDN_SRC_MODE_LEN                3
#define MIC4_UP_SRCDN_FIFO_CLR_OFFSET             23
#define MIC4_UP_SRCDN_FIFO_CLR_LEN                1
#define MIC5_UP_SRCDN_SRC_MODE_OFFSET             24
#define MIC5_UP_SRCDN_SRC_MODE_LEN                3
#define MIC5_UP_SRCDN_FIFO_CLR_OFFSET             27
#define MIC5_UP_SRCDN_FIFO_CLR_LEN                1
#define MIC6_UP_SRCDN_SRC_MODE_OFFSET             28
#define MIC6_UP_SRCDN_SRC_MODE_LEN                3
#define MIC6_UP_SRCDN_FIFO_CLR_OFFSET             31
#define MIC6_UP_SRCDN_FIFO_CLR_LEN                1

#define SRCDN_CTRL1_REG          (PAGE_SOCCODEC + 0xE0)
#define I2S2_RX_L_SRCDN_SRC_MODE_OFFSET           0
#define I2S2_RX_L_SRCDN_SRC_MODE_LEN              3
#define I2S2_RX_L_SRCDN_FIFO_CLR_OFFSET           3
#define I2S2_RX_L_SRCDN_FIFO_CLR_LEN              1
#define I2S2_RX_R_SRCDN_SRC_MODE_OFFSET           4
#define I2S2_RX_R_SRCDN_SRC_MODE_LEN              3
#define I2S2_RX_R_SRCDN_FIFO_CLR_OFFSET           7
#define I2S2_RX_R_SRCDN_FIFO_CLR_LEN              1
#define I2S2_TX_L_SRCDN_SRC_MODE_OFFSET           8
#define I2S2_TX_L_SRCDN_SRC_MODE_LEN              3
#define I2S2_TX_L_SRCDN_FIFO_CLR_OFFSET           11
#define I2S2_TX_L_SRCDN_FIFO_CLR_LEN              1
#define I2S2_TX_R_SRCDN_SRC_MODE_OFFSET           12
#define I2S2_TX_R_SRCDN_SRC_MODE_LEN              3
#define I2S2_TX_R_SRCDN_FIFO_CLR_OFFSET           15
#define I2S2_TX_R_SRCDN_FIFO_CLR_LEN              1
#define NOISE_METER_UP_SRCDN_SRC_MODE_OFFSET      16
#define NOISE_METER_UP_SRCDN_SRC_MODE_LEN         3
#define NOISE_METER_UP_SRCDN_FIFO_CLR_OFFSET      19
#define NOISE_METER_UP_SRCDN_FIFO_CLR_LEN         1
#define MIC7_UP_SRCDN_SRC_MODE_OFFSET             24
#define MIC7_UP_SRCDN_SRC_MODE_LEN                3
#define MIC7_UP_SRCDN_FIFO_CLR_OFFSET             27
#define MIC7_UP_SRCDN_FIFO_CLR_LEN                1
#define MIC8_UP_SRCDN_SRC_MODE_OFFSET             28
#define MIC8_UP_SRCDN_SRC_MODE_LEN                3
#define MIC8_UP_SRCDN_FIFO_CLR_OFFSET             31
#define MIC8_UP_SRCDN_FIFO_CLR_LEN                1

#define SPA0_MIXER4_CTRL0_REG    (PAGE_SOCCODEC + 0xE4)
#define SPA0_MIXER4_GAIN1_OFFSET                  12
#define SPA0_MIXER4_GAIN1_LEN                     2
#define SPA0_MIXER4_GAIN2_OFFSET                  14
#define SPA0_MIXER4_GAIN2_LEN                     2
#define SPA0_MIXER4_GAIN3_OFFSET                  16
#define SPA0_MIXER4_GAIN3_LEN                     2
#define SPA0_MIXER4_GAIN4_OFFSET                  18
#define SPA0_MIXER4_GAIN4_LEN                     2
#define SPA0_MIXER4_IN1_MUTE_OFFSET               20
#define SPA0_MIXER4_IN1_MUTE_LEN                  1
#define SPA0_MIXER4_IN2_MUTE_OFFSET               21
#define SPA0_MIXER4_IN2_MUTE_LEN                  1
#define SPA0_MIXER4_IN3_MUTE_OFFSET               22
#define SPA0_MIXER4_IN3_MUTE_LEN                  1
#define SPA0_MIXER4_IN4_MUTE_OFFSET               23
#define SPA0_MIXER4_IN4_MUTE_LEN                  1
#define SPA0_MIXER4_IN1_ID_OFFSET                 24
#define SPA0_MIXER4_IN1_ID_LEN                    2
#define SPA0_MIXER4_IN2_ID_OFFSET                 26
#define SPA0_MIXER4_IN2_ID_LEN                    2
#define SPA0_MIXER4_IN3_ID_OFFSET                 28
#define SPA0_MIXER4_IN3_ID_LEN                    2
#define SPA0_MIXER4_IN4_ID_OFFSET                 30
#define SPA0_MIXER4_IN4_ID_LEN                    2

#define SPA0_MIXER4_CTRL1_REG    (PAGE_SOCCODEC + 0xE8)
#define SPA0_MIXER4_ZERO_NUM_OFFSET               16
#define SPA0_MIXER4_ZERO_NUM_LEN                  5
#define SPA0_MIXER4_FADE_OUT_OFFSET               21
#define SPA0_MIXER4_FADE_OUT_LEN                  5
#define SPA0_MIXER4_FADE_IN_OFFSET                26
#define SPA0_MIXER4_FADE_IN_LEN                   5
#define SPA0_MIXER4_FADE_EN_OFFSET                31
#define SPA0_MIXER4_FADE_EN_LEN                   1

#define SPA1_MIXER4_CTRL0_REG    (PAGE_SOCCODEC + 0xEC)
#define SPA1_MIXER4_GAIN1_OFFSET                  12
#define SPA1_MIXER4_GAIN1_LEN                     2
#define SPA1_MIXER4_GAIN2_OFFSET                  14
#define SPA1_MIXER4_GAIN2_LEN                     2
#define SPA1_MIXER4_GAIN3_OFFSET                  16
#define SPA1_MIXER4_GAIN3_LEN                     2
#define SPA1_MIXER4_GAIN4_OFFSET                  18
#define SPA1_MIXER4_GAIN4_LEN                     2
#define SPA1_MIXER4_IN1_MUTE_OFFSET               20
#define SPA1_MIXER4_IN1_MUTE_LEN                  1
#define SPA1_MIXER4_IN2_MUTE_OFFSET               21
#define SPA1_MIXER4_IN2_MUTE_LEN                  1
#define SPA1_MIXER4_IN3_MUTE_OFFSET               22
#define SPA1_MIXER4_IN3_MUTE_LEN                  1
#define SPA1_MIXER4_IN4_MUTE_OFFSET               23
#define SPA1_MIXER4_IN4_MUTE_LEN                  1
#define SPA1_MIXER4_IN1_ID_OFFSET                 24
#define SPA1_MIXER4_IN1_ID_LEN                    2
#define SPA1_MIXER4_IN2_ID_OFFSET                 26
#define SPA1_MIXER4_IN2_ID_LEN                    2
#define SPA1_MIXER4_IN3_ID_OFFSET                 28
#define SPA1_MIXER4_IN3_ID_LEN                    2
#define SPA1_MIXER4_IN4_ID_OFFSET                 30
#define SPA1_MIXER4_IN4_ID_LEN                    2

#define SPA1_MIXER4_CTRL1_REG    (PAGE_SOCCODEC + 0xF0)
#define SPA1_MIXER4_ZERO_NUM_OFFSET               16
#define SPA1_MIXER4_ZERO_NUM_LEN                  5
#define SPA1_MIXER4_FADE_OUT_OFFSET               21
#define SPA1_MIXER4_FADE_OUT_LEN                  5
#define SPA1_MIXER4_FADE_IN_OFFSET                26
#define SPA1_MIXER4_FADE_IN_LEN                   5
#define SPA1_MIXER4_FADE_EN_OFFSET                31
#define SPA1_MIXER4_FADE_EN_LEN                   1

#define SPA2_MIXER4_CTRL0_REG    (PAGE_SOCCODEC + 0xF4)
#define SPA2_MIXER4_GAIN1_OFFSET                  12
#define SPA2_MIXER4_GAIN1_LEN                     2
#define SPA2_MIXER4_GAIN2_OFFSET                  14
#define SPA2_MIXER4_GAIN2_LEN                     2
#define SPA2_MIXER4_GAIN3_OFFSET                  16
#define SPA2_MIXER4_GAIN3_LEN                     2
#define SPA2_MIXER4_GAIN4_OFFSET                  18
#define SPA2_MIXER4_GAIN4_LEN                     2
#define SPA2_MIXER4_IN1_MUTE_OFFSET               20
#define SPA2_MIXER4_IN1_MUTE_LEN                  1
#define SPA2_MIXER4_IN2_MUTE_OFFSET               21
#define SPA2_MIXER4_IN2_MUTE_LEN                  1
#define SPA2_MIXER4_IN3_MUTE_OFFSET               22
#define SPA2_MIXER4_IN3_MUTE_LEN                  1
#define SPA2_MIXER4_IN4_MUTE_OFFSET               23
#define SPA2_MIXER4_IN4_MUTE_LEN                  1
#define SPA2_MIXER4_IN1_ID_OFFSET                 24
#define SPA2_MIXER4_IN1_ID_LEN                    2
#define SPA2_MIXER4_IN2_ID_OFFSET                 26
#define SPA2_MIXER4_IN2_ID_LEN                    2
#define SPA2_MIXER4_IN3_ID_OFFSET                 28
#define SPA2_MIXER4_IN3_ID_LEN                    2
#define SPA2_MIXER4_IN4_ID_OFFSET                 30
#define SPA2_MIXER4_IN4_ID_LEN                    2

#define SPA2_MIXER4_CTRL1_REG    (PAGE_SOCCODEC + 0xF8)
#define SPA2_MIXER4_ZERO_NUM_OFFSET               16
#define SPA2_MIXER4_ZERO_NUM_LEN                  5
#define SPA2_MIXER4_FADE_OUT_OFFSET               21
#define SPA2_MIXER4_FADE_OUT_LEN                  5
#define SPA2_MIXER4_FADE_IN_OFFSET                26
#define SPA2_MIXER4_FADE_IN_LEN                   5
#define SPA2_MIXER4_FADE_EN_OFFSET                31
#define SPA2_MIXER4_FADE_EN_LEN                   1

#define I2S2_TX_MIXER2_CTRL_REG  (PAGE_SOCCODEC + 0xFC)
#define I2S2_TX_MIXER2_ZERO_NUM_OFFSET            6
#define I2S2_TX_MIXER2_ZERO_NUM_LEN               5
#define I2S2_TX_MIXER2_FADE_OUT_OFFSET            11
#define I2S2_TX_MIXER2_FADE_OUT_LEN               5
#define I2S2_TX_MIXER2_FADE_IN_OFFSET             16
#define I2S2_TX_MIXER2_FADE_IN_LEN                5
#define I2S2_TX_MIXER2_FADE_EN_OFFSET             21
#define I2S2_TX_MIXER2_FADE_EN_LEN                1
#define I2S2_TX_MIXER2_GAIN1_OFFSET               22
#define I2S2_TX_MIXER2_GAIN1_LEN                  2
#define I2S2_TX_MIXER2_GAIN2_OFFSET               24
#define I2S2_TX_MIXER2_GAIN2_LEN                  2
#define I2S2_TX_MIXER2_IN1_MUTE_OFFSET            26
#define I2S2_TX_MIXER2_IN1_MUTE_LEN               1
#define I2S2_TX_MIXER2_IN2_MUTE_OFFSET            27
#define I2S2_TX_MIXER2_IN2_MUTE_LEN               1
#define I2S2_TX_MIXER2_IN1_ID_OFFSET              28
#define I2S2_TX_MIXER2_IN1_ID_LEN                 2
#define I2S2_TX_MIXER2_IN2_ID_OFFSET              30
#define I2S2_TX_MIXER2_IN2_ID_LEN                 2

#define ULTR0_MIXER2_CTRL_REG    (PAGE_SOCCODEC + 0x100)
#define ULTR0_MIXER2_ZERO_NUM_OFFSET              6
#define ULTR0_MIXER2_ZERO_NUM_LEN                 5
#define ULTR0_MIXER2_FADE_OUT_OFFSET              11
#define ULTR0_MIXER2_FADE_OUT_LEN                 5
#define ULTR0_MIXER2_FADE_IN_OFFSET               16
#define ULTR0_MIXER2_FADE_IN_LEN                  5
#define ULTR0_MIXER2_FADE_EN_OFFSET               21
#define ULTR0_MIXER2_FADE_EN_LEN                  1
#define ULTR0_MIXER2_GAIN1_OFFSET                 22
#define ULTR0_MIXER2_GAIN1_LEN                    2
#define ULTR0_MIXER2_GAIN2_OFFSET                 24
#define ULTR0_MIXER2_GAIN2_LEN                    2
#define ULTR0_MIXER2_IN1_MUTE_OFFSET              26
#define ULTR0_MIXER2_IN1_MUTE_LEN                 1
#define ULTR0_MIXER2_IN2_MUTE_OFFSET              27
#define ULTR0_MIXER2_IN2_MUTE_LEN                 1
#define ULTR0_MIXER2_IN1_ID_OFFSET                28
#define ULTR0_MIXER2_IN1_ID_LEN                   2
#define ULTR0_MIXER2_IN2_ID_OFFSET                30
#define ULTR0_MIXER2_IN2_ID_LEN                   2

#define ULTR1_MIXER2_CTRL_REG    (PAGE_SOCCODEC + 0x104)
#define ULTR1_MIXER2_ZERO_NUM_OFFSET              6
#define ULTR1_MIXER2_ZERO_NUM_LEN                 5
#define ULTR1_MIXER2_FADE_OUT_OFFSET              11
#define ULTR1_MIXER2_FADE_OUT_LEN                 5
#define ULTR1_MIXER2_FADE_IN_OFFSET               16
#define ULTR1_MIXER2_FADE_IN_LEN                  5
#define ULTR1_MIXER2_FADE_EN_OFFSET               21
#define ULTR1_MIXER2_FADE_EN_LEN                  1
#define ULTR1_MIXER2_GAIN1_OFFSET                 22
#define ULTR1_MIXER2_GAIN1_LEN                    2
#define ULTR1_MIXER2_GAIN2_OFFSET                 24
#define ULTR1_MIXER2_GAIN2_LEN                    2
#define ULTR1_MIXER2_IN1_MUTE_OFFSET              26
#define ULTR1_MIXER2_IN1_MUTE_LEN                 1
#define ULTR1_MIXER2_IN2_MUTE_OFFSET              27
#define ULTR1_MIXER2_IN2_MUTE_LEN                 1
#define ULTR1_MIXER2_IN1_ID_OFFSET                28
#define ULTR1_MIXER2_IN1_ID_LEN                   2
#define ULTR1_MIXER2_IN2_ID_OFFSET                30
#define ULTR1_MIXER2_IN2_ID_LEN                   2

#define ULTR2_MIXER2_CTRL_REG    (PAGE_SOCCODEC + 0x108)
#define ULTR2_MIXER2_ZERO_NUM_OFFSET              6
#define ULTR2_MIXER2_ZERO_NUM_LEN                 5
#define ULTR2_MIXER2_FADE_OUT_OFFSET              11
#define ULTR2_MIXER2_FADE_OUT_LEN                 5
#define ULTR2_MIXER2_FADE_IN_OFFSET               16
#define ULTR2_MIXER2_FADE_IN_LEN                  5
#define ULTR2_MIXER2_FADE_EN_OFFSET               21
#define ULTR2_MIXER2_FADE_EN_LEN                  1
#define ULTR2_MIXER2_GAIN1_OFFSET                 22
#define ULTR2_MIXER2_GAIN1_LEN                    2
#define ULTR2_MIXER2_GAIN2_OFFSET                 24
#define ULTR2_MIXER2_GAIN2_LEN                    2
#define ULTR2_MIXER2_IN1_MUTE_OFFSET              26
#define ULTR2_MIXER2_IN1_MUTE_LEN                 1
#define ULTR2_MIXER2_IN2_MUTE_OFFSET              27
#define ULTR2_MIXER2_IN2_MUTE_LEN                 1
#define ULTR2_MIXER2_IN1_ID_OFFSET                28
#define ULTR2_MIXER2_IN1_ID_LEN                   2
#define ULTR2_MIXER2_IN2_ID_OFFSET                30
#define ULTR2_MIXER2_IN2_ID_LEN                   2

#define ADC_FILTER_CTRL0_REG     (PAGE_SOCCODEC + 0x10C)
#define ADC8_CIC_GAIN_OFFSET                      4
#define ADC8_CIC_GAIN_LEN                         6
#define ADC7_CIC_GAIN_OFFSET                      10
#define ADC7_CIC_GAIN_LEN                         6
#define ADC6_CIC_GAIN_OFFSET                      20
#define ADC6_CIC_GAIN_LEN                         6
#define ADC5_CIC_GAIN_OFFSET                      26
#define ADC5_CIC_GAIN_LEN                         6

#define ADC_FILTER_CTRL1_REG     (PAGE_SOCCODEC + 0x110)
#define ADC4_CIC_GAIN_OFFSET                      4
#define ADC4_CIC_GAIN_LEN                         6
#define ADC3_CIC_GAIN_OFFSET                      10
#define ADC3_CIC_GAIN_LEN                         6
#define ADC2_CIC_GAIN_OFFSET                      20
#define ADC2_CIC_GAIN_LEN                         6
#define ADC1_CIC_GAIN_OFFSET                      26
#define ADC1_CIC_GAIN_LEN                         6

#define DMIC_CTRL_REG            (PAGE_SOCCODEC + 0x114)
#define DMIC1_SW_DMIC_MODE_OFFSET                 0
#define DMIC1_SW_DMIC_MODE_LEN                    1
#define DMIC1_REVERSE_OFFSET                      1
#define DMIC1_REVERSE_LEN                         1
#define DMIC2_SW_DMIC_MODE_OFFSET                 2
#define DMIC2_SW_DMIC_MODE_LEN                    1
#define DMIC2_REVERSE_OFFSET                      3
#define DMIC2_REVERSE_LEN                         1
#define DMIC3_SW_DMIC_MODE_OFFSET                 4
#define DMIC3_SW_DMIC_MODE_LEN                    1
#define DMIC3_REVERSE_OFFSET                      5
#define DMIC3_REVERSE_LEN                         1
#define DMIC4_SW_DMIC_MODE_OFFSET                 6
#define DMIC4_SW_DMIC_MODE_LEN                    1
#define DMIC4_REVERSE_OFFSET                      7
#define DMIC4_REVERSE_LEN                         1

#define DMIC_DIV_REG             (PAGE_SOCCODEC + 0x118)
#define FS_DMIC1_OFFSET                           0
#define FS_DMIC1_LEN                              4
#define FS_DMIC2_OFFSET                           4
#define FS_DMIC2_LEN                              4
#define FS_DMIC3_OFFSET                           8
#define FS_DMIC3_LEN                              4
#define FS_DMIC4_OFFSET                           12
#define FS_DMIC4_LEN                              4

#define SPA12_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x11C)
#define SPA_1_UP_FIFO_AEMPTY_TH_OFFSET            5
#define SPA_1_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_1_UP_FIFO_AFULL_TH_OFFSET             10
#define SPA_1_UP_FIFO_AFULL_TH_LEN                5
#define SPA_1_UP_FIFO_CLR_OFFSET                  15
#define SPA_1_UP_FIFO_CLR_LEN                     1
#define SPA_2_UP_FIFO_AEMPTY_TH_OFFSET            21
#define SPA_2_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_2_UP_FIFO_AFULL_TH_OFFSET             26
#define SPA_2_UP_FIFO_AFULL_TH_LEN                5
#define SPA_2_UP_FIFO_CLR_OFFSET                  31
#define SPA_2_UP_FIFO_CLR_LEN                     1

#define SPA34_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x120)
#define SPA_3_UP_FIFO_AEMPTY_TH_OFFSET            5
#define SPA_3_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_3_UP_FIFO_AFULL_TH_OFFSET             10
#define SPA_3_UP_FIFO_AFULL_TH_LEN                5
#define SPA_3_UP_FIFO_CLR_OFFSET                  15
#define SPA_3_UP_FIFO_CLR_LEN                     1
#define SPA_4_UP_FIFO_AEMPTY_TH_OFFSET            21
#define SPA_4_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_4_UP_FIFO_AFULL_TH_OFFSET             26
#define SPA_4_UP_FIFO_AFULL_TH_LEN                5
#define SPA_4_UP_FIFO_CLR_OFFSET                  31
#define SPA_4_UP_FIFO_CLR_LEN                     1

#define SPA56_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x124)
#define SPA_5_UP_FIFO_AEMPTY_TH_OFFSET            5
#define SPA_5_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_5_UP_FIFO_AFULL_TH_OFFSET             10
#define SPA_5_UP_FIFO_AFULL_TH_LEN                5
#define SPA_5_UP_FIFO_CLR_OFFSET                  15
#define SPA_5_UP_FIFO_CLR_LEN                     1
#define SPA_6_UP_FIFO_AEMPTY_TH_OFFSET            21
#define SPA_6_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_6_UP_FIFO_AFULL_TH_OFFSET             26
#define SPA_6_UP_FIFO_AFULL_TH_LEN                5
#define SPA_6_UP_FIFO_CLR_OFFSET                  31
#define SPA_6_UP_FIFO_CLR_LEN                     1

#define CODEC_0_DN_AFIFO_CTRL_REG (PAGE_SOCCODEC + 0x128)
#define CODEC_1_DN_FIFO_AEMPTY_TH_OFFSET          0
#define CODEC_1_DN_FIFO_AEMPTY_TH_LEN             7
#define CODEC_1_DN_FIFO_AFULL_TH_OFFSET           8
#define CODEC_1_DN_FIFO_AFULL_TH_LEN              7
#define CODEC_1_DN_FIFO_CLR_OFFSET                15
#define CODEC_1_DN_FIFO_CLR_LEN                   1
#define CODEC_0_DN_FIFO_AEMPTY_TH_OFFSET          16
#define CODEC_0_DN_FIFO_AEMPTY_TH_LEN             7
#define CODEC_0_DN_FIFO_AFULL_TH_OFFSET           24
#define CODEC_0_DN_FIFO_AFULL_TH_LEN              7
#define CODEC_0_DN_FIFO_CLR_OFFSET                31
#define CODEC_0_DN_FIFO_CLR_LEN                   1

#define CODEC_1_DN_AFIFO_CTRL_REG (PAGE_SOCCODEC + 0x12C)
#define CODEC_2_DN_FIFO_AEMPTY_TH_OFFSET          0
#define CODEC_2_DN_FIFO_AEMPTY_TH_LEN             7
#define CODEC_2_DN_FIFO_AFULL_TH_OFFSET           8
#define CODEC_2_DN_FIFO_AFULL_TH_LEN              7
#define CODEC_2_DN_FIFO_CLR_OFFSET                15
#define CODEC_2_DN_FIFO_CLR_LEN                   1
#define AUDIO_0_DN_FIFO_AEMPTY_TH_OFFSET          16
#define AUDIO_0_DN_FIFO_AEMPTY_TH_LEN             7
#define AUDIO_0_DN_FIFO_AFULL_TH_OFFSET           24
#define AUDIO_0_DN_FIFO_AFULL_TH_LEN              7
#define AUDIO_0_DN_FIFO_CLR_OFFSET                31
#define AUDIO_0_DN_FIFO_CLR_LEN                   1

#define AUDIO_DN_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x130)
#define AUDIO_1_DN_FIFO_AEMPTY_TH_OFFSET          0
#define AUDIO_1_DN_FIFO_AEMPTY_TH_LEN             7
#define AUDIO_1_DN_FIFO_AFULL_TH_OFFSET           8
#define AUDIO_1_DN_FIFO_AFULL_TH_LEN              7
#define AUDIO_1_DN_FIFO_CLR_OFFSET                15
#define AUDIO_1_DN_FIFO_CLR_LEN                   1
#define AUDIO_2_DN_FIFO_AEMPTY_TH_OFFSET          16
#define AUDIO_2_DN_FIFO_AEMPTY_TH_LEN             7
#define AUDIO_2_DN_FIFO_AFULL_TH_OFFSET           24
#define AUDIO_2_DN_FIFO_AFULL_TH_LEN              7
#define AUDIO_2_DN_FIFO_CLR_OFFSET                31
#define AUDIO_2_DN_FIFO_CLR_LEN                   1

#define ULTR_AFIFO_CTRL_REG      (PAGE_SOCCODEC + 0x134)
#define ULTR_DN_FIFO_AEMPTY_TH_OFFSET             0
#define ULTR_DN_FIFO_AEMPTY_TH_LEN                7
#define ULTR_DN_FIFO_AFULL_TH_OFFSET              8
#define ULTR_DN_FIFO_AFULL_TH_LEN                 7
#define ULTR_DN_FIFO_CLR_OFFSET                   15
#define ULTR_DN_FIFO_CLR_LEN                      1

#define AUDIO_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x138)
#define AUDIO_R_UP_FIFO_AEMPTY_TH_OFFSET          5
#define AUDIO_R_UP_FIFO_AEMPTY_TH_LEN             5
#define AUDIO_R_UP_FIFO_AFULL_TH_OFFSET           10
#define AUDIO_R_UP_FIFO_AFULL_TH_LEN              5
#define AUDIO_R_UP_FIFO_CLR_OFFSET                15
#define AUDIO_R_UP_FIFO_CLR_LEN                   1
#define AUDIO_L_UP_FIFO_AEMPTY_TH_OFFSET          21
#define AUDIO_L_UP_FIFO_AEMPTY_TH_LEN             5
#define AUDIO_L_UP_FIFO_AFULL_TH_OFFSET           26
#define AUDIO_L_UP_FIFO_AFULL_TH_LEN              5
#define AUDIO_L_UP_FIFO_CLR_OFFSET                31
#define AUDIO_L_UP_FIFO_CLR_LEN                   1

#define VOICE_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x13C)
#define VOICE_R_UP_FIFO_AEMPTY_TH_OFFSET          5
#define VOICE_R_UP_FIFO_AEMPTY_TH_LEN             5
#define VOICE_R_UP_FIFO_AFULL_TH_OFFSET           10
#define VOICE_R_UP_FIFO_AFULL_TH_LEN              5
#define VOICE_R_UP_FIFO_CLR_OFFSET                15
#define VOICE_R_UP_FIFO_CLR_LEN                   1
#define VOICE_L_UP_FIFO_AEMPTY_TH_OFFSET          21
#define VOICE_L_UP_FIFO_AEMPTY_TH_LEN             5
#define VOICE_L_UP_FIFO_AFULL_TH_OFFSET           26
#define VOICE_L_UP_FIFO_AFULL_TH_LEN              5
#define VOICE_L_UP_FIFO_CLR_OFFSET                31
#define VOICE_L_UP_FIFO_CLR_LEN                   1

#define MIC34_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x140)
#define MIC3_UP_FIFO_AEMPTY_TH_OFFSET             5
#define MIC3_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC3_UP_FIFO_AFULL_TH_OFFSET              10
#define MIC3_UP_FIFO_AFULL_TH_LEN                 5
#define MIC3_UP_FIFO_CLR_OFFSET                   15
#define MIC3_UP_FIFO_CLR_LEN                      1
#define MIC4_UP_FIFO_AEMPTY_TH_OFFSET             21
#define MIC4_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC4_UP_FIFO_AFULL_TH_OFFSET              26
#define MIC4_UP_FIFO_AFULL_TH_LEN                 5
#define MIC4_UP_FIFO_CLR_OFFSET                   31
#define MIC4_UP_FIFO_CLR_LEN                      1

#define MIC56_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x144)
#define MIC5_UP_FIFO_AEMPTY_TH_OFFSET             5
#define MIC5_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC5_UP_FIFO_AFULL_TH_OFFSET              10
#define MIC5_UP_FIFO_AFULL_TH_LEN                 5
#define MIC5_UP_FIFO_CLR_OFFSET                   15
#define MIC5_UP_FIFO_CLR_LEN                      1
#define MIC6_UP_FIFO_AEMPTY_TH_OFFSET             21
#define MIC6_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC6_UP_FIFO_AFULL_TH_OFFSET              26
#define MIC6_UP_FIFO_AFULL_TH_LEN                 5
#define MIC6_UP_FIFO_CLR_OFFSET                   31
#define MIC6_UP_FIFO_CLR_LEN                      1

#define MIC78_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x148)
#define MIC7_UP_FIFO_AEMPTY_TH_OFFSET             5
#define MIC7_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC7_UP_FIFO_AFULL_TH_OFFSET              10
#define MIC7_UP_FIFO_AFULL_TH_LEN                 5
#define MIC7_UP_FIFO_CLR_OFFSET                   15
#define MIC7_UP_FIFO_CLR_LEN                      1
#define MIC8_UP_FIFO_AEMPTY_TH_OFFSET             21
#define MIC8_UP_FIFO_AEMPTY_TH_LEN                5
#define MIC8_UP_FIFO_AFULL_TH_OFFSET              26
#define MIC8_UP_FIFO_AFULL_TH_LEN                 5
#define MIC8_UP_FIFO_CLR_OFFSET                   31
#define MIC8_UP_FIFO_CLR_LEN                      1

#define ULTR_UP_AFIFO_CTRL_REG   (PAGE_SOCCODEC + 0x14C)
#define ULTR_UP_FIFO_AEMPTY_TH_OFFSET             5
#define ULTR_UP_FIFO_AEMPTY_TH_LEN                5
#define ULTR_UP_FIFO_AFULL_TH_OFFSET              10
#define ULTR_UP_FIFO_AFULL_TH_LEN                 5
#define ULTR_UP_FIFO_CLR_OFFSET                   15
#define ULTR_UP_FIFO_CLR_LEN                      1
#define AUDIOZOOM_UP_FIFO_AEMPTY_TH_OFFSET        21
#define AUDIOZOOM_UP_FIFO_AEMPTY_TH_LEN           5
#define AUDIOZOOM_UP_FIFO_AFULL_TH_OFFSET         26
#define AUDIOZOOM_UP_FIFO_AFULL_TH_LEN            5
#define AUDIOZOOM_UP_FIFO_CLR_OFFSET              31
#define AUDIOZOOM_UP_FIFO_CLR_LEN                 1

#define NOISE_METER_UP_AFIFO_CTRL_REG (PAGE_SOCCODEC + 0x150)
#define NOISE_METER_UP_FIFO_AEMPTY_TH_OFFSET      5
#define NOISE_METER_UP_FIFO_AEMPTY_TH_LEN         5
#define NOISE_METER_UP_FIFO_AFULL_TH_OFFSET       10
#define NOISE_METER_UP_FIFO_AFULL_TH_LEN          5
#define NOISE_METER_UP_FIFO_CLR_OFFSET            15
#define NOISE_METER_UP_FIFO_CLR_LEN               1

#define SIF_CTRL_REG             (PAGE_SOCCODEC + 0x154)
#define SIF_S2P_LOOP_OFFSET                       2
#define SIF_S2P_LOOP_LEN                          2
#define SIF_P2S_LOOP_OFFSET                       4
#define SIF_P2S_LOOP_LEN                          2
#define MIC7_DIN_SEL_OFFSET                       6
#define MIC7_DIN_SEL_LEN                          3
#define MIC8_DIN_SEL_OFFSET                       9
#define MIC8_DIN_SEL_LEN                          3

#define FS_CTRL0_REG             (PAGE_SOCCODEC + 0x158)
#define FS_SPA1_UP_AFIFO_OFFSET                   0
#define FS_SPA1_UP_AFIFO_LEN                      2
#define FS_SPA2_UP_AFIFO_OFFSET                   2
#define FS_SPA2_UP_AFIFO_LEN                      2
#define FS_SPA3_UP_AFIFO_OFFSET                   4
#define FS_SPA3_UP_AFIFO_LEN                      2
#define FS_SPA_4_UP_AFIFO_OFFSET                  6
#define FS_SPA_4_UP_AFIFO_LEN                     2
#define FS_CODEC_0_DN_AFIFO_OFFSET                8
#define FS_CODEC_0_DN_AFIFO_LEN                   3
#define FS_CODEC_1_DN_AFIFO_OFFSET                11
#define FS_CODEC_1_DN_AFIFO_LEN                   3
#define FS_CODEC_2_DN_AFIFO_OFFSET                14
#define FS_CODEC_2_DN_AFIFO_LEN                   2
#define FS_AUDIO_2_DN_AFIFO_OFFSET                16
#define FS_AUDIO_2_DN_AFIFO_LEN                   2
#define FS_BT_L_DN_AFIFO_OFFSET                   18
#define FS_BT_L_DN_AFIFO_LEN                      2
#define FS_BT_R_DN_AFIFO_OFFSET                   20
#define FS_BT_R_DN_AFIFO_LEN                      2
#define FS_AUDIO_L_UP_AFIFO_OFFSET                22
#define FS_AUDIO_L_UP_AFIFO_LEN                   2
#define FS_AUDIO_R_UP_AFIFO_OFFSET                24
#define FS_AUDIO_R_UP_AFIFO_LEN                   2
#define FS_MIC8_UP_PGA_OFFSET                     26
#define FS_MIC8_UP_PGA_LEN                        3

#define FS_CTRL1_REG             (PAGE_SOCCODEC + 0x15C)
#define FS_VOICE_L_UP_AFIFO_OFFSET                0
#define FS_VOICE_L_UP_AFIFO_LEN                   3
#define FS_VOICE_R_UP_AFIFO_OFFSET                3
#define FS_VOICE_R_UP_AFIFO_LEN                   3
#define FS_MIC3_UP_AFIFO_OFFSET                   6
#define FS_MIC3_UP_AFIFO_LEN                      3
#define FS_MIC4_UP_AFIFO_OFFSET                   9
#define FS_MIC4_UP_AFIFO_LEN                      3
#define FS_MIC5_UP_AFIFO_OFFSET                   12
#define FS_MIC5_UP_AFIFO_LEN                      3
#define FS_MIC6_UP_AFIFO_OFFSET                   15
#define FS_MIC6_UP_AFIFO_LEN                      3
#define FS_MIC7_UP_AFIFO_OFFSET                   18
#define FS_MIC7_UP_AFIFO_LEN                      3
#define FS_MIC8_UP_AFIFO_OFFSET                   21
#define FS_MIC8_UP_AFIFO_LEN                      3
#define FS_ULTR_UP_AFIFO_OFFSET                   24
#define FS_ULTR_UP_AFIFO_LEN                      2
#define FS_MIC7_UP_PGA_OFFSET                     28
#define FS_MIC7_UP_PGA_LEN                        3

#define FS_CTRL2_REG             (PAGE_SOCCODEC + 0x160)
#define FS_CODEC_0_DN_PGA_OFFSET                  0
#define FS_CODEC_0_DN_PGA_LEN                     2
#define FS_CODEC_1_DN_PGA_OFFSET                  2
#define FS_CODEC_1_DN_PGA_LEN                     2
#define FS_CODEC_2_DN_PGA_OFFSET                  4
#define FS_CODEC_2_DN_PGA_LEN                     2
#define FS_AUDIO_0_DN_PGA_OFFSET                  6
#define FS_AUDIO_0_DN_PGA_LEN                     2
#define FS_AUDIO_1_DN_PGA_OFFSET                  8
#define FS_AUDIO_1_DN_PGA_LEN                     2
#define FS_AUDIO_2_DN_PGA_OFFSET                  10
#define FS_AUDIO_2_DN_PGA_LEN                     2
#define FS_AUDIO_L_UP_PGA_OFFSET                  12
#define FS_AUDIO_L_UP_PGA_LEN                     2
#define FS_AUDIO_R_UP_PGA_OFFSET                  14
#define FS_AUDIO_R_UP_PGA_LEN                     2
#define FS_MIC3_UP_PGA_OFFSET                     16
#define FS_MIC3_UP_PGA_LEN                        3
#define FS_MIC4_UP_PGA_OFFSET                     19
#define FS_MIC4_UP_PGA_LEN                        3
#define FS_MIC5_UP_PGA_OFFSET                     22
#define FS_MIC5_UP_PGA_LEN                        3
#define FS_MIC6_UP_PGA_OFFSET                     25
#define FS_MIC6_UP_PGA_LEN                        3

#define FS_CTRL3_REG             (PAGE_SOCCODEC + 0x164)
#define FS_I2S2_RX_L_PGA_OFFSET                   0
#define FS_I2S2_RX_L_PGA_LEN                      3
#define FS_I2S2_RX_R_PGA_OFFSET                   3
#define FS_I2S2_RX_R_PGA_LEN                      3
#define FS_CODEC_0_DN_SRCUP_DIN_OFFSET            6
#define FS_CODEC_0_DN_SRCUP_DIN_LEN               2
#define FS_CODEC_0_DN_SRCUP_DOUT_OFFSET           8
#define FS_CODEC_0_DN_SRCUP_DOUT_LEN              3
#define FS_CODEC_1_DN_SRCUP_DIN_OFFSET            11
#define FS_CODEC_1_DN_SRCUP_DIN_LEN               2
#define FS_CODEC_1_DN_SRCUP_DOUT_OFFSET           13
#define FS_CODEC_1_DN_SRCUP_DOUT_LEN              3
#define FS_CODEC_2_DN_SRCUP_DIN_OFFSET            16
#define FS_CODEC_2_DN_SRCUP_DIN_LEN               2
#define FS_CODEC_2_DN_SRCUP_DOUT_OFFSET           18
#define FS_CODEC_2_DN_SRCUP_DOUT_LEN              3
#define FS_AUDIO_0_DN_SRCUP_DIN_OFFSET            21
#define FS_AUDIO_0_DN_SRCUP_DIN_LEN               2
#define FS_AUDIO_0_DN_SRCUP_DOUT_OFFSET           23
#define FS_AUDIO_0_DN_SRCUP_DOUT_LEN              3
#define FS_AUDIO_1_DN_SRCUP_DIN_OFFSET            26
#define FS_AUDIO_1_DN_SRCUP_DIN_LEN               2
#define FS_AUDIO_1_DN_SRCUP_DOUT_OFFSET           28
#define FS_AUDIO_1_DN_SRCUP_DOUT_LEN              3

#define FS_CTRL4_REG             (PAGE_SOCCODEC + 0x168)
#define FS_AUDIO_UP_L_SRCDN_DIN_OFFSET            14
#define FS_AUDIO_UP_L_SRCDN_DIN_LEN               2
#define FS_AUDIO_UP_L_SRCDN_DOUT_OFFSET           16
#define FS_AUDIO_UP_L_SRCDN_DOUT_LEN              2
#define FS_AUDIO_UP_R_SRCDN_DIN_OFFSET            18
#define FS_AUDIO_UP_R_SRCDN_DIN_LEN               2
#define FS_AUDIO_UP_R_SRCDN_DOUT_OFFSET           20
#define FS_AUDIO_UP_R_SRCDN_DOUT_LEN              2
#define FS_VOICE_UP_L_SRCDN_DIN_OFFSET            22
#define FS_VOICE_UP_L_SRCDN_DIN_LEN               2
#define FS_VOICE_UP_L_SRCDN_DOUT_OFFSET           24
#define FS_VOICE_UP_L_SRCDN_DOUT_LEN              3
#define FS_VOICE_UP_R_SRCDN_DIN_OFFSET            27
#define FS_VOICE_UP_R_SRCDN_DIN_LEN               2
#define FS_VOICE_UP_R_SRCDN_DOUT_OFFSET           29
#define FS_VOICE_UP_R_SRCDN_DOUT_LEN              3

#define FS_CTRL5_REG             (PAGE_SOCCODEC + 0x16C)
#define FS_MIC3_SRCDN_DOUT_OFFSET                 0
#define FS_MIC3_SRCDN_DOUT_LEN                    3
#define FS_MIC4_SRCDN_DOUT_OFFSET                 3
#define FS_MIC4_SRCDN_DOUT_LEN                    3
#define FS_MIC5_SRCDN_DOUT_OFFSET                 6
#define FS_MIC5_SRCDN_DOUT_LEN                    3
#define FS_MIC6_SRCDN_DOUT_OFFSET                 9
#define FS_MIC6_SRCDN_DOUT_LEN                    3
#define FS_MIC7_SRCDN_DOUT_OFFSET                 12
#define FS_MIC7_SRCDN_DOUT_LEN                    3
#define FS_MIC8_SRCDN_DOUT_OFFSET                 15
#define FS_MIC8_SRCDN_DOUT_LEN                    3
#define FS_I2S2_RX_L_SRCDN_DIN_OFFSET             18
#define FS_I2S2_RX_L_SRCDN_DIN_LEN                3
#define FS_I2S2_RX_L_SRCDN_DOUT_OFFSET            21
#define FS_I2S2_RX_L_SRCDN_DOUT_LEN               3
#define FS_VOICE_L_UP_PGA_OFFSET                  24
#define FS_VOICE_L_UP_PGA_LEN                     3
#define FS_VOICE_R_UP_PGA_OFFSET                  27
#define FS_VOICE_R_UP_PGA_LEN                     3

#define FS_CTRL6_REG             (PAGE_SOCCODEC + 0x170)
#define FS_I2S2_RX_R_SRCDN_DIN_OFFSET             0
#define FS_I2S2_RX_R_SRCDN_DIN_LEN                3
#define FS_I2S2_RX_R_SRCDN_DOUT_OFFSET            3
#define FS_I2S2_RX_R_SRCDN_DOUT_LEN               3
#define FS_I2S2_TX_L_SRCDN_DIN_OFFSET             6
#define FS_I2S2_TX_L_SRCDN_DIN_LEN                2
#define FS_I2S2_TX_L_SRCDN_DOUT_OFFSET            8
#define FS_I2S2_TX_L_SRCDN_DOUT_LEN               3
#define FS_I2S2_TX_R_SRCDN_DIN_OFFSET             11
#define FS_I2S2_TX_R_SRCDN_DIN_LEN                2
#define FS_I2S2_TX_R_SRCDN_DOUT_OFFSET            13
#define FS_I2S2_TX_R_SRCDN_DOUT_LEN               3
#define FS_I2S2_TX_MIXER2_OFFSET                  16
#define FS_I2S2_TX_MIXER2_LEN                     3
#define FS_S1_TDM_OFFSET                          19
#define FS_S1_TDM_LEN                             2
#define FS_SPA2_MIXER4_OFFSET                     21
#define FS_SPA2_MIXER4_LEN                        1
#define FS_SPA1_MIXER4_OFFSET                     22
#define FS_SPA1_MIXER4_LEN                        1
#define FS_SPA0_MIXER4_OFFSET                     23
#define FS_SPA0_MIXER4_LEN                        1
#define FS_AUDIO_0_DN_AFIFO_OFFSET                24
#define FS_AUDIO_0_DN_AFIFO_LEN                   3
#define FS_AUDIO_1_DN_AFIFO_OFFSET                27
#define FS_AUDIO_1_DN_AFIFO_LEN                   3

#define CODEC_DIN_MUX0_REG       (PAGE_SOCCODEC + 0x174)
#define I2S1_TX_L_SEL_OFFSET                      0
#define I2S1_TX_L_SEL_LEN                         2
#define I2S1_TX_R_SEL_OFFSET                      2
#define I2S1_TX_R_SEL_LEN                         2
#define I2S2_TX_R_SEL_OFFSET                      4
#define I2S2_TX_R_SEL_LEN                         1
#define SIDETONE_PGA_DIN_SEL_OFFSET               5
#define SIDETONE_PGA_DIN_SEL_LEN                  3
#define ADC1_DIN_SEL_OFFSET                       8
#define ADC1_DIN_SEL_LEN                          1
#define ADC2_DIN_SEL_OFFSET                       9
#define ADC2_DIN_SEL_LEN                          1
#define ADC3_DIN_SEL_OFFSET                       10
#define ADC3_DIN_SEL_LEN                          1
#define ADC4_DIN_SEL_OFFSET                       11
#define ADC4_DIN_SEL_LEN                          1
#define AU_UP_L_DIN_SEL_OFFSET                    12
#define AU_UP_L_DIN_SEL_LEN                       3
#define AU_UP_R_DIN_SEL_OFFSET                    15
#define AU_UP_R_DIN_SEL_LEN                       3
#define VO_UP_L_DIN_SEL_OFFSET                    18
#define VO_UP_L_DIN_SEL_LEN                       3
#define VO_UP_R_DIN_SEL_OFFSET                    21
#define VO_UP_R_DIN_SEL_LEN                       3
#define MIC3_DIN_SEL_OFFSET                       24
#define MIC3_DIN_SEL_LEN                          3
#define MIC4_DIN_SEL_OFFSET                       27
#define MIC4_DIN_SEL_LEN                          3
#define I2S2_TX_SEL_OFFSET                        30
#define I2S2_TX_SEL_LEN                           1

#define CODEC_DIN_MUX1_REG       (PAGE_SOCCODEC + 0x178)
#define MIC5_DIN_SEL_OFFSET                       0
#define MIC5_DIN_SEL_LEN                          4
#define MIC6_DIN_SEL_OFFSET                       4
#define MIC6_DIN_SEL_LEN                          4
#define BM_I2S1_TDM_TX_L_DIN_SEL_OFFSET           8
#define BM_I2S1_TDM_TX_L_DIN_SEL_LEN              1
#define BM_I2S1_TDM_TX_R_DIN_SEL_OFFSET           9
#define BM_I2S1_TDM_TX_R_DIN_SEL_LEN              1
#define NOISE_METER_PGA_DIN_OFFSET                10
#define NOISE_METER_PGA_DIN_LEN                   4
#define BT_TX_SRCDN_DIN_SEL_OFFSET                14
#define BT_TX_SRCDN_DIN_SEL_LEN                   1
#define ADC1_CIC_DOUT_SEL_OFFSET                  17
#define ADC1_CIC_DOUT_SEL_LEN                     2
#define ADC2_CIC_DOUT_SEL_OFFSET                  19
#define ADC2_CIC_DOUT_SEL_LEN                     2
#define ADC3_CIC_DOUT_SEL_OFFSET                  21
#define ADC3_CIC_DOUT_SEL_LEN                     2
#define ADC4_CIC_DOUT_SEL_OFFSET                  23
#define ADC4_CIC_DOUT_SEL_LEN                     2
#define ADC5_CIC_DOUT_SEL_OFFSET                  25
#define ADC5_CIC_DOUT_SEL_LEN                     1
#define ADC6_CIC_DOUT_SEL_OFFSET                  26
#define ADC6_CIC_DOUT_SEL_LEN                     1
#define ADC7_CIC_DOUT_SEL_OFFSET                  27
#define ADC7_CIC_DOUT_SEL_LEN                     1
#define ADC8_CIC_DOUT_SEL_OFFSET                  28
#define ADC8_CIC_DOUT_SEL_LEN                     1

#define CODEC_ADC1_DC_OFFSET_REG (PAGE_SOCCODEC + 0x17C)
#define ADC1_DC_OFFSET_OFFSET                     0
#define ADC1_DC_OFFSET_LEN                        24
#define ADC1_CIC_DIN_SEL_OFFSET                   24
#define ADC1_CIC_DIN_SEL_LEN                      2
#define ADC_ULTR_CIC_DIN_SEL_OFFSET               26
#define ADC_ULTR_CIC_DIN_SEL_LEN                  4

#define CODEC_ADC2_DC_OFFSET_REG (PAGE_SOCCODEC + 0x180)
#define ADC2_DC_OFFSET_OFFSET                     0
#define ADC2_DC_OFFSET_LEN                        24
#define ADC2_CIC_DIN_SEL_OFFSET                   24
#define ADC2_CIC_DIN_SEL_LEN                      2

#define CODEC_ADC3_DC_OFFSET_REG (PAGE_SOCCODEC + 0x184)
#define ADC3_DC_OFFSET_OFFSET                     0
#define ADC3_DC_OFFSET_LEN                        24
#define ADC3_CIC_DIN_SEL_OFFSET                   24
#define ADC3_CIC_DIN_SEL_LEN                      2

#define CODEC_ADC4_DC_OFFSET_REG (PAGE_SOCCODEC + 0x188)
#define ADC4_DC_OFFSET_OFFSET                     0
#define ADC4_DC_OFFSET_LEN                        24
#define ADC4_CIC_DIN_SEL_OFFSET                   24
#define ADC4_CIC_DIN_SEL_LEN                      2

#define CODEC_ADC5_DC_OFFSET_REG (PAGE_SOCCODEC + 0x18C)
#define ADC5_DC_OFFSET_OFFSET                     0
#define ADC5_DC_OFFSET_LEN                        24
#define ADC5_CIC_DIN_SEL_OFFSET                   24
#define ADC5_CIC_DIN_SEL_LEN                      2

#define CODEC_ADC6_DC_OFFSET_REG (PAGE_SOCCODEC + 0x190)
#define ADC6_DC_OFFSET_OFFSET                     0
#define ADC6_DC_OFFSET_LEN                        24
#define ADC6_CIC_DIN_SEL_OFFSET                   24
#define ADC6_CIC_DIN_SEL_LEN                      2

#define CODEC_ADC7_DC_OFFSET_REG (PAGE_SOCCODEC + 0x194)
#define ADC7_DC_OFFSET_OFFSET                     0
#define ADC7_DC_OFFSET_LEN                        24

#define CODEC_ADC8_DC_OFFSET_REG (PAGE_SOCCODEC + 0x198)
#define ADC8_DC_OFFSET_OFFSET                     0
#define ADC8_DC_OFFSET_LEN                        24

#define CODEC_ADC_ULTR_DC_OFFSET_REG (PAGE_SOCCODEC + 0x19C)
#define ADC_ULTR_DC_OFFSET_OFFSET                 0
#define ADC_ULTR_DC_OFFSET_LEN                    24

#define MEM_CTRL_S_REG           (PAGE_SOCCODEC + 0x1A0)
#define MEM_CTRL_S_OFFSET                         0
#define MEM_CTRL_S_LEN                            26

#define MEM_CTRL_1W2R_REG        (PAGE_SOCCODEC + 0x1A4)
#define MEM_CTRL_D1W2R_OFFSET                     0
#define MEM_CTRL_D1W2R_LEN                        16

#define ADC_CTRL_HPF_REG         (PAGE_SOCCODEC + 0x1A8)
#define ADC1_HPF_BYPASS_EN_OFFSET                 0
#define ADC1_HPF_BYPASS_EN_LEN                    1
#define ADC2_HPF_BYPASS_EN_OFFSET                 1
#define ADC2_HPF_BYPASS_EN_LEN                    1
#define ADC3_HPF_BYPASS_EN_OFFSET                 2
#define ADC3_HPF_BYPASS_EN_LEN                    1
#define ADC4_HPF_BYPASS_EN_OFFSET                 3
#define ADC4_HPF_BYPASS_EN_LEN                    1
#define ADC5_HPF_BYPASS_EN_OFFSET                 4
#define ADC5_HPF_BYPASS_EN_LEN                    1
#define ADC6_HPF_BYPASS_EN_OFFSET                 5
#define ADC6_HPF_BYPASS_EN_LEN                    1
#define ADC7_HPF_BYPASS_EN_OFFSET                 6
#define ADC7_HPF_BYPASS_EN_LEN                    1
#define ADC8_HPF_BYPASS_EN_OFFSET                 7
#define ADC8_HPF_BYPASS_EN_LEN                    1
#define DACR_UP16_HBF3_BYPASS_EN_OFFSET           8
#define DACR_UP16_HBF3_BYPASS_EN_LEN              1
#define DACR_UP16_HBF2_BYPASS_EN_OFFSET           9
#define DACR_UP16_HBF2_BYPASS_EN_LEN              1
#define DACR_UP16_HBF1_BYPASS_EN_OFFSET           10
#define DACR_UP16_HBF1_BYPASS_EN_LEN              1
#define DACR_UP16_HBF0_BYPASS_EN_OFFSET           11
#define DACR_UP16_HBF0_BYPASS_EN_LEN              1
#define DACL_UP16_HBF3_BYPASS_EN_OFFSET           12
#define DACL_UP16_HBF3_BYPASS_EN_LEN              1
#define DACL_UP16_HBF2_BYPASS_EN_OFFSET           13
#define DACL_UP16_HBF2_BYPASS_EN_LEN              1
#define DACL_UP16_HBF1_BYPASS_EN_OFFSET           14
#define DACL_UP16_HBF1_BYPASS_EN_LEN              1
#define DACL_UP16_HBF0_BYPASS_EN_OFFSET           15
#define DACL_UP16_HBF0_BYPASS_EN_LEN              1
#define ADC_ULTR_HPF_BYPASS_EN_OFFSET             16
#define ADC_ULTR_HPF_BYPASS_EN_LEN                1

#define BT_DN_AFIFO_CTRL_REG     (PAGE_SOCCODEC + 0x1AC)
#define BT_L_DN_FIFO_AEMPTY_TH_OFFSET             0
#define BT_L_DN_FIFO_AEMPTY_TH_LEN                7
#define BT_L_DN_FIFO_AFULL_TH_OFFSET              8
#define BT_L_DN_FIFO_AFULL_TH_LEN                 7
#define BT_L_DN_FIFO_CLR_OFFSET                   15
#define BT_L_DN_FIFO_CLR_LEN                      1
#define BT_R_DN_FIFO_AEMPTY_TH_OFFSET             16
#define BT_R_DN_FIFO_AEMPTY_TH_LEN                7
#define BT_R_DN_FIFO_AFULL_TH_OFFSET              24
#define BT_R_DN_FIFO_AFULL_TH_LEN                 7
#define BT_R_DN_FIFO_CLR_OFFSET                   31
#define BT_R_DN_FIFO_CLR_LEN                      1

#define CODEC_DACL_DC_OFFSET_REG (PAGE_SOCCODEC + 0x1B0)
#define DACL_DC_OFFSET_OFFSET                     0
#define DACL_DC_OFFSET_LEN                        24
#define DACL_SRCUP_DIN_SEL_OFFSET                 24
#define DACL_SRCUP_DIN_SEL_LEN                    4

#define CODEC_DACR_DC_OFFSET_REG (PAGE_SOCCODEC + 0x1B4)
#define DACR_DC_OFFSET_OFFSET                     0
#define DACR_DC_OFFSET_LEN                        24

#define DAC_FILTER_CTRL_REG      (PAGE_SOCCODEC + 0x1B8)
#define SDM_R_CALT_VLD_OFFSET                     0
#define SDM_R_CALT_VLD_LEN                        1
#define SDM_R_DITHER_OFFSET                       1
#define SDM_R_DITHER_LEN                          1
#define DACR_FIR2D_BYPASS_EN_OFFSET               2
#define DACR_FIR2D_BYPASS_EN_LEN                  1
#define DACR_FIR2C_BYPASS_EN_OFFSET               3
#define DACR_FIR2C_BYPASS_EN_LEN                  1
#define SDM_L_CALT_VLD_OFFSET                     4
#define SDM_L_CALT_VLD_LEN                        1
#define SDM_L_DITHER_OFFSET                       5
#define SDM_L_DITHER_LEN                          1
#define DACL_FIR2D_BYPASS_EN_OFFSET               6
#define DACL_FIR2D_BYPASS_EN_LEN                  1
#define DACL_FIR2C_BYPASS_EN_OFFSET               7
#define DACL_FIR2C_BYPASS_EN_LEN                  1
#define DACL_SRCUP_MUX_SEL_OFFSET                 8
#define DACL_SRCUP_MUX_SEL_LEN                    1

#define CIC_ULTRA_CTRL0_REG      (PAGE_SOCCODEC + 0x1BC)
#define ADC1_CIC_DN4_ULTRA_GAIN_OFFSET            0
#define ADC1_CIC_DN4_ULTRA_GAIN_LEN               8
#define ADC1_CIC_DN8_ULTRA_GAIN_OFFSET            8
#define ADC1_CIC_DN8_ULTRA_GAIN_LEN               4
#define ADC2_CIC_DN4_ULTRA_GAIN_OFFSET            16
#define ADC2_CIC_DN4_ULTRA_GAIN_LEN               8
#define ADC2_CIC_DN8_ULTRA_GAIN_OFFSET            24
#define ADC2_CIC_DN8_ULTRA_GAIN_LEN               4

#define CIC_ULTRA_CTRL1_REG      (PAGE_SOCCODEC + 0x1C0)
#define ADC3_CIC_DN4_ULTRA_GAIN_OFFSET            0
#define ADC3_CIC_DN4_ULTRA_GAIN_LEN               8
#define ADC3_CIC_DN8_ULTRA_GAIN_OFFSET            8
#define ADC3_CIC_DN8_ULTRA_GAIN_LEN               4
#define ADC4_CIC_DN4_ULTRA_GAIN_OFFSET            16
#define ADC4_CIC_DN4_ULTRA_GAIN_LEN               8
#define ADC4_CIC_DN8_ULTRA_GAIN_OFFSET            24
#define ADC4_CIC_DN8_ULTRA_GAIN_LEN               4

#define SIF0_CTRL_REG            (PAGE_SOCCODEC + 0x1C4)
#define SIF0_DACL_EN_OFFSET                       0
#define SIF0_DACL_EN_LEN                          1
#define SIF0_DACR_EN_OFFSET                       1
#define SIF0_DACR_EN_LEN                          1
#define SIF0_ADCL_EN_OFFSET                       2
#define SIF0_ADCL_EN_LEN                          1
#define SIF0_ADCR_EN_OFFSET                       3
#define SIF0_ADCR_EN_LEN                          1
#define SIF0_ADC_MIC3_EN_OFFSET                   4
#define SIF0_ADC_MIC3_EN_LEN                      1
#define SIF0_DAC_ULTR_EN_OFFSET                   5
#define SIF0_DAC_ULTR_EN_LEN                      1
#define SIF0_SPA_DAC_EN_OFFSET                    6
#define SIF0_SPA_DAC_EN_LEN                       1
#define SIF0_SPA_ADC_EN_OFFSET                    7
#define SIF0_SPA_ADC_EN_LEN                       1

#define HP_L_CTRL0_REG           (PAGE_SOCCODEC + 0x1C8)
#define HP_L_SRCUP_DIN_SEL_OFFSET                 0
#define HP_L_SRCUP_DIN_SEL_LEN                    1
#define HP_L_FIR2C_BYPASS_EN_OFFSET               1
#define HP_L_FIR2C_BYPASS_EN_LEN                  1
#define HP_L_FIR2D_BYPASS_EN_OFFSET               2
#define HP_L_FIR2D_BYPASS_EN_LEN                  1
#define HP_L_HBF0_BYPASS_EN_OFFSET                3
#define HP_L_HBF0_BYPASS_EN_LEN                   1
#define HP_L_HBF1_BYPASS_EN_OFFSET                4
#define HP_L_HBF1_BYPASS_EN_LEN                   1
#define HP_L_HBF2_BYPASS_EN_OFFSET                5
#define HP_L_HBF2_BYPASS_EN_LEN                   1
#define HP_L_HBF3_BYPASS_EN_OFFSET                6
#define HP_L_HBF3_BYPASS_EN_LEN                   1
#define HP_L_SDM_CALT_VLD_OFFSET                  7
#define HP_L_SDM_CALT_VLD_LEN                     1
#define HP_L_SDM_DITHER_OFFSET                    8
#define HP_L_SDM_DITHER_LEN                       1

#define HP_L_CTRL1_REG           (PAGE_SOCCODEC + 0x1CC)
#define HP_L_DC_OFFSET_OFFSET                     0
#define HP_L_DC_OFFSET_LEN                        24

#define HP_R_CTRL0_REG           (PAGE_SOCCODEC + 0x1D0)
#define HP_R_SRCUP_DIN_SEL_OFFSET                 0
#define HP_R_SRCUP_DIN_SEL_LEN                    1
#define HP_R_FIR2C_BYPASS_EN_OFFSET               1
#define HP_R_FIR2C_BYPASS_EN_LEN                  1
#define HP_R_FIR2D_BYPASS_EN_OFFSET               2
#define HP_R_FIR2D_BYPASS_EN_LEN                  1
#define HP_R_HBF0_BYPASS_EN_OFFSET                3
#define HP_R_HBF0_BYPASS_EN_LEN                   1
#define HP_R_HBF1_BYPASS_EN_OFFSET                4
#define HP_R_HBF1_BYPASS_EN_LEN                   1
#define HP_R_HBF2_BYPASS_EN_OFFSET                5
#define HP_R_HBF2_BYPASS_EN_LEN                   1
#define HP_R_HBF3_BYPASS_EN_OFFSET                6
#define HP_R_HBF3_BYPASS_EN_LEN                   1
#define HP_R_SDM_CALT_VLD_OFFSET                  7
#define HP_R_SDM_CALT_VLD_LEN                     1
#define HP_R_SDM_DITHER_OFFSET                    8
#define HP_R_SDM_DITHER_LEN                       1

#define HP_R_CTRL1_REG           (PAGE_SOCCODEC + 0x1D4)
#define HP_R_DC_OFFSET_OFFSET                     0
#define HP_R_DC_OFFSET_LEN                        24

#define ULTR_ADC_CTRL0_REG       (PAGE_SOCCODEC + 0x1D8)
#define ULTR_ADC_CIC_GAIN_OFFSET                  0
#define ULTR_ADC_CIC_GAIN_LEN                     6
#define ULTR_ADC_COMPD_BYPASS_EN_OFFSET           8
#define ULTR_ADC_COMPD_BYPASS_EN_LEN              1
#define ULTR_ADC_HBF2D_BYPASS_EN_OFFSET           9
#define ULTR_ADC_HBF2D_BYPASS_EN_LEN              1
#define ULTR_ADC_HBFVD_BYPASS_EN_OFFSET           10
#define ULTR_ADC_HBFVD_BYPASS_EN_LEN              1
#define ULTR_ADC_HPF_BYPASS_EN_OFFSET             11
#define ULTR_ADC_HPF_BYPASS_EN_LEN                1

#define ADC_ULTRA_CIC_CTRL_REG   (PAGE_SOCCODEC + 0x1DC)
#define ADC1_CIC_ULTRA_SEL_OFFSET                 0
#define ADC1_CIC_ULTRA_SEL_LEN                    1
#define ADC2_CIC_ULTRA_SEL_OFFSET                 1
#define ADC2_CIC_ULTRA_SEL_LEN                    1
#define ADC3_CIC_ULTRA_SEL_OFFSET                 2
#define ADC3_CIC_ULTRA_SEL_LEN                    1
#define ADC4_CIC_ULTRA_SEL_OFFSET                 3
#define ADC4_CIC_ULTRA_SEL_LEN                    1

#define FS_CTRL7_REG             (PAGE_SOCCODEC + 0x1E0)
#define FS_AUDIO_2_DN_SRCUP_DIN_OFFSET            0
#define FS_AUDIO_2_DN_SRCUP_DIN_LEN               2
#define FS_AUDIO_2_DN_SRCUP_DOUT_OFFSET           2
#define FS_AUDIO_2_DN_SRCUP_DOUT_LEN              3
#define FS_I2S2_RX_L_SRCUP_DIN_OFFSET             5
#define FS_I2S2_RX_L_SRCUP_DIN_LEN                3
#define FS_I2S2_RX_L_SRCUP_DOUT_OFFSET            8
#define FS_I2S2_RX_L_SRCUP_DOUT_LEN               3
#define FS_I2S2_RX_R_SRCUP_DIN_OFFSET             11
#define FS_I2S2_RX_R_SRCUP_DIN_LEN                3
#define FS_I2S2_RX_R_SRCUP_DOUT_OFFSET            14
#define FS_I2S2_RX_R_SRCUP_DOUT_LEN               3
#define FS_DACL_MIXER4_SRCUP_DIN_OFFSET           17
#define FS_DACL_MIXER4_SRCUP_DIN_LEN              2
#define FS_DACL_MIXER4_SRCUP_DOUT_OFFSET          19
#define FS_DACL_MIXER4_SRCUP_DOUT_LEN             2
#define FS_DACR_MIXER4_SRCUP_DIN_OFFSET           21
#define FS_DACR_MIXER4_SRCUP_DIN_LEN              2
#define FS_DACR_MIXER4_SRCUP_DOUT_OFFSET          23
#define FS_DACR_MIXER4_SRCUP_DOUT_LEN             2

#define PGA_GAINOFFSET_CTRL5_REG (PAGE_SOCCODEC + 0x1E4)
#define MIC5_UP_PGA_GAINOFFSET_OFFSET             0
#define MIC5_UP_PGA_GAINOFFSET_LEN                8
#define MIC6_UP_PGA_GAINOFFSET_OFFSET             8
#define MIC6_UP_PGA_GAINOFFSET_LEN                8
#define MIC7_UP_PGA_GAINOFFSET_OFFSET             16
#define MIC7_UP_PGA_GAINOFFSET_LEN                8
#define MIC8_UP__PGA_GAINOFFSET_OFFSET            24
#define MIC8_UP__PGA_GAINOFFSET_LEN               8

#define CODEC_DIN_MUX2_REG       (PAGE_SOCCODEC + 0x1E8)
#define AUDIOZOOM_DATA0_DIN_SEL_OFFSET            0
#define AUDIOZOOM_DATA0_DIN_SEL_LEN               3
#define AUDIOZOOM_DATA1_DIN_SEL_OFFSET            3
#define AUDIOZOOM_DATA1_DIN_SEL_LEN               3
#define AUDIOZOOM_DATA2_DIN_SEL_OFFSET            6
#define AUDIOZOOM_DATA2_DIN_SEL_LEN               3

#define NOISE_METER_CTRL0_REG    (PAGE_SOCCODEC + 0x1EC)
#define F_ALPHA_OFFSET                            0
#define F_ALPHA_LEN                               20

#define NOISE_METER_CTRL1_REG    (PAGE_SOCCODEC + 0x1F0)
#define S_ALPHA_OFFSET                            0
#define S_ALPHA_LEN                               20

#define NOISE_METER_CTRL2_REG    (PAGE_SOCCODEC + 0x1F4)
#define MIC_SENS_OFFSET_OFFSET                    0
#define MIC_SENS_OFFSET_LEN                       16
#define LA_OFFSET_OFFSET                          16
#define LA_OFFSET_LEN                             16

#define NOISE_METER_CTRL3_REG    (PAGE_SOCCODEC + 0x1F8)
#define LC_OFFSET_OFFSET                          0
#define LC_OFFSET_LEN                             16
#define LZ_OFFSET_OFFSET                          16
#define LZ_OFFSET_LEN                             16

#define NOISE_METER_CTRL4_REG    (PAGE_SOCCODEC + 0x1FC)
#define LAF_OFFSET_OFFSET                         0
#define LAF_OFFSET_LEN                            16
#define LAS_OFFSET_OFFSET                         16
#define LAS_OFFSET_LEN                            16

#define NOISE_METER_CTRL5_REG    (PAGE_SOCCODEC + 0x200)
#define LAF_MAX_OFFSET_OFFSET                     0
#define LAF_MAX_OFFSET_LEN                        16
#define LAS_MAX_OFFSET_OFFSET                     16
#define LAS_MAX_OFFSET_LEN                        16

#define NOISE_METER_CTRL6_REG    (PAGE_SOCCODEC + 0x204)
#define A_WT_GAIN_OFFSET                          0
#define A_WT_GAIN_LEN                             10
#define C_WT_GAIN_OFFSET                          12
#define C_WT_GAIN_LEN                             10

#define NOISE_METER_CTRL7_REG    (PAGE_SOCCODEC + 0x208)
#define Z_WT_GAIN_OFFSET                          0
#define Z_WT_GAIN_LEN                             10
#define OFL_INCR_OFFSET                           12
#define OFL_INCR_LEN                              6
#define OFL_DECR_OFFSET                           20
#define OFL_DECR_LEN                              6

#define NOISE_METER_CTRL8_REG    (PAGE_SOCCODEC + 0x20C)
#define NS_LVL_THR_OFFSET                         0
#define NS_LVL_THR_LEN                            16
#define NS_DUR_THR_OFFSET                         16
#define NS_DUR_THR_LEN                            8

#define NOISE_METER_CTRL9_REG    (PAGE_SOCCODEC + 0x210)
#define OFL_ALPHA_OFFSET                          0
#define OFL_ALPHA_LEN                             20
#define OFL_DUR_OFFSET                            20
#define OFL_DUR_LEN                               3
#define NSM_OFL_EN_OFFSET                         24
#define NSM_OFL_EN_LEN                            1
#define NSM_ONL_EN_OFFSET                         25
#define NSM_ONL_EN_LEN                            1

#define NOISE_METER_CTRL10_REG   (PAGE_SOCCODEC + 0x214)
#define NSM_ONL_INTR_CLR_OFFSET                   0
#define NSM_ONL_INTR_CLR_LEN                      1
#define NSM_OFL_INTR_CLR_OFFSET                   1
#define NSM_OFL_INTR_CLR_LEN                      1
#define OFL_LAF_OFFSET_OFFSET                     16
#define OFL_LAF_OFFSET_LEN                        16

#define NOISE_METER_OFL_REG_CTRL0_REG (PAGE_SOCCODEC + 0x218)
#define NM_OFL_STO_WR_ADDR_OFFSET                 0
#define NM_OFL_STO_WR_ADDR_LEN                    11
#define NM_OFL_STO_RD_ADDR_OFFSET                 12
#define NM_OFL_STO_RD_ADDR_LEN                    11

#define NOISE_METER_OFL_REG_CTRL1_REG (PAGE_SOCCODEC + 0x21C)
#define NM_OFL_WR_ADDR_INIT_OFFSET                0
#define NM_OFL_WR_ADDR_INIT_LEN                   11
#define NM_OFL_RD_ADDR_INIT_OFFSET                12
#define NM_OFL_RD_ADDR_INIT_LEN                   11

#define NOISE_METER_OFL_NOS_CNT_CTRL0_REG (PAGE_SOCCODEC + 0x220)
#define NM_OFL_STO_NOS_CNT_OFFSET                 0
#define NM_OFL_STO_NOS_CNT_LEN                    8

#define NOISE_METER_OFL_NOS_CNT_CTRL1_REG (PAGE_SOCCODEC + 0x224)
#define NM_OFL_NOS_CNT_INIT_OFFSET                0
#define NM_OFL_NOS_CNT_INIT_LEN                   8

#define NOISE_METER_OFL_RECOV_CTRL_REG (PAGE_SOCCODEC + 0x228)
#define NM_OFL_RECOV_PULSE_OFFSET                 0
#define NM_OFL_RECOV_PULSE_LEN                    1

#define NOISE_METER_ONLINE_DATA0_REG (PAGE_SOCCODEC + 0x22C)
#define NOISE_METER_ONLINE_LA_DATA_OFFSET         0
#define NOISE_METER_ONLINE_LA_DATA_LEN            16
#define NOISE_METER_ONLINE_LC_DATA_OFFSET         16
#define NOISE_METER_ONLINE_LC_DATA_LEN            16

#define NOISE_METER_ONLINE_DATA1_REG (PAGE_SOCCODEC + 0x230)
#define NOISE_METER_ONLINE_LZ_DATA_OFFSET         0
#define NOISE_METER_ONLINE_LZ_DATA_LEN            16

#define NOISE_METER_ONLINE_DATA2_REG (PAGE_SOCCODEC + 0x234)
#define NOISE_METER_ONLINE_LAS_DATA_OFFSET        0
#define NOISE_METER_ONLINE_LAS_DATA_LEN           16
#define NOISE_METER_ONLINE_LAF_DATA_OFFSET        16
#define NOISE_METER_ONLINE_LAF_DATA_LEN           16

#define NOISE_METER_ONLINE_DATA3_REG (PAGE_SOCCODEC + 0x238)
#define NOISE_METER_ONLINE_LASMAX_DATA_OFFSET     0
#define NOISE_METER_ONLINE_LASMAX_DATA_LEN        16
#define NOISE_METER_ONLINE_LAFMAX_DATA_OFFSET     16
#define NOISE_METER_ONLINE_LAFMAX_DATA_LEN        16

#define MEM_CTRL_SPC_REG         (PAGE_SOCCODEC + 0x23C)
#define MEM_CTRL_SPC_OFFSET                       0
#define MEM_CTRL_SPC_LEN                          26

#define CODEC_CLK_EN5_REG        (PAGE_SOCCODEC + 0x240)
#define CODEC_3_DN_SRCUP_CLKEN_OFFSET             0
#define CODEC_3_DN_SRCUP_CLKEN_LEN                1
#define AUDIO_3_DN_SRCUP_CLKEN_OFFSET             1
#define AUDIO_3_DN_SRCUP_CLKEN_LEN                1
#define CODEC_3_DN_PGA_CLKEN_OFFSET               2
#define CODEC_3_DN_PGA_CLKEN_LEN                  1
#define AUDIO_3_DN_PGA_CLKEN_OFFSET               3
#define AUDIO_3_DN_PGA_CLKEN_LEN                  1
#define CODEC_3_DN_AFIFO_CLKEN_OFFSET             4
#define CODEC_3_DN_AFIFO_CLKEN_LEN                1
#define AUDIO_3_DN_AFIFO_CLKEN_OFFSET             5
#define AUDIO_3_DN_AFIFO_CLKEN_LEN                1
#define SPA7_UP_AFIFO_CLKEN_OFFSET                6
#define SPA7_UP_AFIFO_CLKEN_LEN                   1
#define SPA8_UP_AFIFO_CLKEN_OFFSET                7
#define SPA8_UP_AFIFO_CLKEN_LEN                   1
#define MIXER4_SPA3_CLKEN_OFFSET                  8
#define MIXER4_SPA3_CLKEN_LEN                     1
#define MIXER2_ULTR3_CLKEN_OFFSET                 9
#define MIXER2_ULTR3_CLKEN_LEN                    1

#define FS_CTRL8_REG             (PAGE_SOCCODEC + 0x244)
#define FS_CODEC_3_DN_SRCUP_DIN_OFFSET            0
#define FS_CODEC_3_DN_SRCUP_DIN_LEN               2
#define FS_CODEC_3_DN_SRCUP_DOUT_OFFSET           2
#define FS_CODEC_3_DN_SRCUP_DOUT_LEN              3
#define FS_AUDIO_3_DN_SRCUP_DIN_OFFSET            5
#define FS_AUDIO_3_DN_SRCUP_DIN_LEN               2
#define FS_AUDIO_3_DN_SRCUP_DOUT_OFFSET           7
#define FS_AUDIO_3_DN_SRCUP_DOUT_LEN              3
#define FS_CODEC_3_DN_PGA_OFFSET                  10
#define FS_CODEC_3_DN_PGA_LEN                     2
#define FS_AUDIO_3_DN_PGA_OFFSET                  12
#define FS_AUDIO_3_DN_PGA_LEN                     2
#define FS_CODEC_3_DN_AFIFO_OFFSET                14
#define FS_CODEC_3_DN_AFIFO_LEN                   2
#define FS_AUDIO_3_DN_AFIFO_OFFSET                16
#define FS_AUDIO_3_DN_AFIFO_LEN                   2
#define FS_SPA3_MIXER4_OFFSET                     18
#define FS_SPA3_MIXER4_LEN                        1
#define FS_DACL_SRCUP_DIN_OFFSET                  19
#define FS_DACL_SRCUP_DIN_LEN                     2
#define FS_DACL_SRCUP_DOUT_OFFSET                 21
#define FS_DACL_SRCUP_DOUT_LEN                    3

#define CODEC_3_DN_AFIFO_CTRL_REG (PAGE_SOCCODEC + 0x248)
#define AUDIO_3_DN_FIFO_AEMPTY_TH_OFFSET          0
#define AUDIO_3_DN_FIFO_AEMPTY_TH_LEN             7
#define AUDIO_3_DN_FIFO_AFULL_TH_OFFSET           8
#define AUDIO_3_DN_FIFO_AFULL_TH_LEN              7
#define AUDIO_3_DN_FIFO_CLR_OFFSET                15
#define AUDIO_3_DN_FIFO_CLR_LEN                   1
#define CODEC_3_DN_FIFO_AEMPTY_TH_OFFSET          16
#define CODEC_3_DN_FIFO_AEMPTY_TH_LEN             7
#define CODEC_3_DN_FIFO_AFULL_TH_OFFSET           24
#define CODEC_3_DN_FIFO_AFULL_TH_LEN              7
#define CODEC_3_DN_FIFO_CLR_OFFSET                31
#define CODEC_3_DN_FIFO_CLR_LEN                   1

#define SPA78_UP_AFIFO_CTRL_REG  (PAGE_SOCCODEC + 0x24C)
#define SPA_7_UP_FIFO_AEMPTY_TH_OFFSET            5
#define SPA_7_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_7_UP_FIFO_AFULL_TH_OFFSET             10
#define SPA_7_UP_FIFO_AFULL_TH_LEN                5
#define SPA_7_UP_FIFO_CLR_OFFSET                  15
#define SPA_7_UP_FIFO_CLR_LEN                     1
#define SPA_8_UP_FIFO_AEMPTY_TH_OFFSET            21
#define SPA_8_UP_FIFO_AEMPTY_TH_LEN               5
#define SPA_8_UP_FIFO_AFULL_TH_OFFSET             26
#define SPA_8_UP_FIFO_AFULL_TH_LEN                5
#define SPA_8_UP_FIFO_CLR_OFFSET                  31
#define SPA_8_UP_FIFO_CLR_LEN                     1

#define CODEC_3_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x250)
#define CODEC_3_DN_PGA_LINEAR_SEL_OFFSET          1
#define CODEC_3_DN_PGA_LINEAR_SEL_LEN             1
#define CODEC_3_DN_PGA_ZERO_NUM_OFFSET            2
#define CODEC_3_DN_PGA_ZERO_NUM_LEN               5
#define CODEC_3_DN_PGA_THRE_ID_OFFSET             7
#define CODEC_3_DN_PGA_THRE_ID_LEN                2
#define CODEC_3_DN_PGA_NOISE_EN_OFFSET            9
#define CODEC_3_DN_PGA_NOISE_EN_LEN               1
#define CODEC_3_DN_PGA_BYPASS_OFFSET              10
#define CODEC_3_DN_PGA_BYPASS_LEN                 1
#define CODEC_3_DN_PGA_FADE_OUT_OFFSET            11
#define CODEC_3_DN_PGA_FADE_OUT_LEN               5
#define CODEC_3_DN_PGA_FADE_IN_OFFSET             16
#define CODEC_3_DN_PGA_FADE_IN_LEN                5
#define CODEC_3_DN_PGA_CFG_OFFSET                 21
#define CODEC_3_DN_PGA_CFG_LEN                    3
#define CODEC_3_DN_PGA_GAIN_OFFSET                24
#define CODEC_3_DN_PGA_GAIN_LEN                   8

#define AUDIO_3_DN_PGA_CTRL_REG  (PAGE_SOCCODEC + 0x254)
#define AUDIO_3_DN_PGA_LINEAR_SEL_OFFSET          1
#define AUDIO_3_DN_PGA_LINEAR_SEL_LEN             1
#define AUDIO_3_DN_PGA_ZERO_NUM_OFFSET            2
#define AUDIO_3_DN_PGA_ZERO_NUM_LEN               5
#define AUDIO_3_DN_PGA_THRE_ID_OFFSET             7
#define AUDIO_3_DN_PGA_THRE_ID_LEN                2
#define AUDIO_3_DN_PGA_NOISE_EN_OFFSET            9
#define AUDIO_3_DN_PGA_NOISE_EN_LEN               1
#define AUDIO_3_DN_PGA_BYPASS_OFFSET              10
#define AUDIO_3_DN_PGA_BYPASS_LEN                 1
#define AUDIO_3_DN_PGA_FADE_OUT_OFFSET            11
#define AUDIO_3_DN_PGA_FADE_OUT_LEN               5
#define AUDIO_3_DN_PGA_FADE_IN_OFFSET             16
#define AUDIO_3_DN_PGA_FADE_IN_LEN                5
#define AUDIO_3_DN_PGA_CFG_OFFSET                 21
#define AUDIO_3_DN_PGA_CFG_LEN                    3
#define AUDIO_3_DN_PGA_GAIN_OFFSET                24
#define AUDIO_3_DN_PGA_GAIN_LEN                   8

#define SRCUP_CTRL2_REG          (PAGE_SOCCODEC + 0x258)
#define CODEC_3_DN_SRCUP_SRC_MODE_OFFSET          0
#define CODEC_3_DN_SRCUP_SRC_MODE_LEN             3
#define CODEC_3_DN_SRCUP_FIFO_CLR_OFFSET          3
#define CODEC_3_DN_SRCUP_FIFO_CLR_LEN             1
#define AUDIO_3_SRCUP_SRC_MODE_OFFSET             4
#define AUDIO_3_SRCUP_SRC_MODE_LEN                3
#define AUDIO_3_DN_SRCUP_FIFO_CLR_OFFSET          7
#define AUDIO_3_DN_SRCUP_FIFO_CLR_LEN             1

#define SPA3_MIXER4_CTRL0_REG    (PAGE_SOCCODEC + 0x25C)
#define SPA3_MIXER4_GAIN1_OFFSET                  12
#define SPA3_MIXER4_GAIN1_LEN                     2
#define SPA3_MIXER4_GAIN2_OFFSET                  14
#define SPA3_MIXER4_GAIN2_LEN                     2
#define SPA3_MIXER4_GAIN3_OFFSET                  16
#define SPA3_MIXER4_GAIN3_LEN                     2
#define SPA3_MIXER4_GAIN4_OFFSET                  18
#define SPA3_MIXER4_GAIN4_LEN                     2
#define SPA3_MIXER4_IN1_MUTE_OFFSET               20
#define SPA3_MIXER4_IN1_MUTE_LEN                  1
#define SPA3_MIXER4_IN2_MUTE_OFFSET               21
#define SPA3_MIXER4_IN2_MUTE_LEN                  1
#define SPA3_MIXER4_IN3_MUTE_OFFSET               22
#define SPA3_MIXER4_IN3_MUTE_LEN                  1
#define SPA3_MIXER4_IN4_MUTE_OFFSET               23
#define SPA3_MIXER4_IN4_MUTE_LEN                  1
#define SPA3_MIXER4_IN1_ID_OFFSET                 24
#define SPA3_MIXER4_IN1_ID_LEN                    2
#define SPA3_MIXER4_IN2_ID_OFFSET                 26
#define SPA3_MIXER4_IN2_ID_LEN                    2
#define SPA3_MIXER4_IN3_ID_OFFSET                 28
#define SPA3_MIXER4_IN3_ID_LEN                    2
#define SPA3_MIXER4_IN4_ID_OFFSET                 30
#define SPA3_MIXER4_IN4_ID_LEN                    2

#define SPA3_MIXER4_CTRL1_REG    (PAGE_SOCCODEC + 0x260)
#define SPA3_MIXER4_ZERO_NUM_OFFSET               16
#define SPA3_MIXER4_ZERO_NUM_LEN                  5
#define SPA3_MIXER4_FADE_OUT_OFFSET               21
#define SPA3_MIXER4_FADE_OUT_LEN                  5
#define SPA3_MIXER4_FADE_IN_OFFSET                26
#define SPA3_MIXER4_FADE_IN_LEN                   5
#define SPA3_MIXER4_FADE_EN_OFFSET                31
#define SPA3_MIXER4_FADE_EN_LEN                   1

#define ULTR3_MIXER2_CTRL_REG    (PAGE_SOCCODEC + 0x264)
#define ULTR3_MIXER2_ZERO_NUM_OFFSET              6
#define ULTR3_MIXER2_ZERO_NUM_LEN                 5
#define ULTR3_MIXER2_FADE_OUT_OFFSET              11
#define ULTR3_MIXER2_FADE_OUT_LEN                 5
#define ULTR3_MIXER2_FADE_IN_OFFSET               16
#define ULTR3_MIXER2_FADE_IN_LEN                  5
#define ULTR3_MIXER2_FADE_EN_OFFSET               21
#define ULTR3_MIXER2_FADE_EN_LEN                  1
#define ULTR3_MIXER2_GAIN1_OFFSET                 22
#define ULTR3_MIXER2_GAIN1_LEN                    2
#define ULTR3_MIXER2_GAIN2_OFFSET                 24
#define ULTR3_MIXER2_GAIN2_LEN                    2
#define ULTR3_MIXER2_IN1_MUTE_OFFSET              26
#define ULTR3_MIXER2_IN1_MUTE_LEN                 1
#define ULTR3_MIXER2_IN2_MUTE_OFFSET              27
#define ULTR3_MIXER2_IN2_MUTE_LEN                 1
#define ULTR3_MIXER2_IN1_ID_OFFSET                28
#define ULTR3_MIXER2_IN1_ID_LEN                   2
#define ULTR3_MIXER2_IN2_ID_OFFSET                30
#define ULTR3_MIXER2_IN2_ID_LEN                   2

#define PGA_GAINOFFSET_CTRL6_REG (PAGE_SOCCODEC + 0x268)
#define CODEC_3_DN_PGA_GAINOFFSET_OFFSET          0
#define CODEC_3_DN_PGA_GAINOFFSET_LEN             8
#define AUDIO_3_DN_PGA_GAINOFFSET_OFFSET          8
#define AUDIO_3_DN_PGA_GAINOFFSET_LEN             8

#define SPA_AFIFO_UP_DIN_CTRL6_REG (PAGE_SOCCODEC + 0x26C)
#define SPA1_DIN_SEL_OFFSET                       0
#define SPA1_DIN_SEL_LEN                          1
#define SPA2_DIN_SEL_OFFSET                       1
#define SPA2_DIN_SEL_LEN                          1
#define SPA3_DIN_SEL_OFFSET                       2
#define SPA3_DIN_SEL_LEN                          1
#define SPA4_DIN_SEL_OFFSET                       3
#define SPA4_DIN_SEL_LEN                          1
#define BM_I2S1_TDM128_SLOT1_DIN_SEL_OFFSET       4
#define BM_I2S1_TDM128_SLOT1_DIN_SEL_LEN          1
#define BM_I2S1_TDM128_SLOT0_DIN_SEL_OFFSET       5
#define BM_I2S1_TDM128_SLOT0_DIN_SEL_LEN          1
#define SPA_TEST_FIFO_DIN_SEL_OFFSET              6
#define SPA_TEST_FIFO_DIN_SEL_LEN                 2
#define SPA_TEST_FIFO_CLR_OFFSET                  8
#define SPA_TEST_FIFO_CLR_LEN                     1
#define FS_SPA_TEST_UP_AFIFO_OFFSET               9
#define FS_SPA_TEST_UP_AFIFO_LEN                  3
#define SPA_TEST_FIFO_AFULL_TH_OFFSET             12
#define SPA_TEST_FIFO_AFULL_TH_LEN                5
#define SPA_TEST_FIFO_AEMPTY_TH_OFFSET            17
#define SPA_TEST_FIFO_AEMPTY_TH_LEN               5

#define SPA_AFIFO_UP_DIN_CTRL7_REG (PAGE_SOCCODEC + 0x270)
#define I2S1_TDM256_3IV_DIN_SEL_OFFSET            0
#define I2S1_TDM256_3IV_DIN_SEL_LEN               3
#define I2S1_TDM256_2IV_DIN_SEL_OFFSET            3
#define I2S1_TDM256_2IV_DIN_SEL_LEN               3
#define I2S1_TDM256_1IV_DIN_SEL_OFFSET            6
#define I2S1_TDM256_1IV_DIN_SEL_LEN               3
#define I2S1_TDM256_0IV_DIN_SEL_OFFSET            9
#define I2S1_TDM256_0IV_DIN_SEL_LEN               3

#define R_RST_CTRLEN                  (PAGE_ASPCFG + 0x0)
#define RST_EN_CODEC_N 9
#define RST_EN_AXI2TDM_N 25

#define R_RST_CTRLDIS                 (PAGE_ASPCFG + 0x4)
#define RST_DISEN_CODEC_N 9
#define RST_DISEN_AXI2TDM_N 25

#define R_RST_CTRLSTAT                (PAGE_ASPCFG + 0x8)
#define RST_DSP_N 4

#define R_GATE_EN                     (PAGE_ASPCFG + 0xC)
#define GT_CODEC_CLK 3
#define GT_AXI2TDM_CLK 30

#define R_GATE_CLKEN                  (PAGE_ASPCFG + 0x14)
#define GT_HIFIDSPCLK 0

#define R_DMA0_REQ_SEL                (PAGE_ASPCFG + 0x30)
#define AXI2TDM_IF1_REQ_UP_SEL_OFFSET 31
#define AXI2TDM_IF2_REQ_UP_SEL_OFFSET 29
#define AXI2TDM_IF1_REQ_DN_SEL_OFFSET 22

#define R_DMA_SEL                     (PAGE_ASPCFG + 0x54)
#define PA_CLK_OUT_SEL_OFFSET 11

#define R_CODEC_DMA_SEL               (PAGE_ASPCFG + 0x218)
#define CODEC_DMA_SEL 17

#define RESERVED_LOCK_ADDR            (PAGE_ASPCFG + 0x90)

#endif