/*
 * asp_codec_path.c -- codec driver
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

#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include <linux/clk.h>
#include "asp_codec_store.h"
#include "asp_codec_type.h"
#include "audio_log.h"
#include "soc_mad.h"
#include "audio_pinctrl.h"

#define LOG_TAG "Analog_less_path"

#define UPLINK_PLL \
	{ "AUDIO_UP_O_SWITCH",         NULL,   "PLL" }, \
	{ "VOICE_UP_O_SWITCH",         NULL,   "PLL" }, \
	{ "MIC34_UP_O_SWITCH",         NULL,   "PLL" }, \
	{ "MIC56_UP_O_SWITCH",         NULL,   "PLL" }, \
	{ "A2DP_OFFLOAD_SWITCH",       NULL,   "PLL" } \

#define AUDIO_VOICE_2MIC_ROUTE \
	{ "ADC1",                           NULL,                 "AUDIO_MIC12_INPUT" }, \
	{ "ADC2",                           NULL,                 "AUDIO_MIC12_INPUT" }, \
	{ "ADC1",                           NULL,                 "VOICE_MIC12_INPUT" }, \
	{ "ADC2",                           NULL,                 "VOICE_MIC12_INPUT" }, \
	{ "AUDIO_UP_L_MUX",                 "MIC1_ADC",           "ADC1" }, \
	{ "AUDIO_UP_L_MUX",                 "MIC2_ADC",           "ADC2" }, \
	{ "AUDIO_UP_L_MUX",                 "MIC3_ADC",           "ADC3" }, \
	{ "AUDIO_UP_L_MUX",                 "MIC4_ADC",           "ADC4" }, \
	{ "AUDIO_UP_R_MUX",                 "MIC1_ADC",           "ADC1" }, \
	{ "AUDIO_UP_R_MUX",                 "MIC2_ADC",           "ADC2" }, \
	{ "AUDIO_UP_R_MUX",                 "MIC3_ADC",           "ADC3" }, \
	{ "AUDIO_UP_R_MUX",                 "MIC4_ADC",           "ADC4" }, \
	{ "VOICE_UP_L_MUX",                 "MIC1_ADC",           "ADC1" }, \
	{ "VOICE_UP_L_MUX",                 "MIC2_ADC",           "ADC2" }, \
	{ "VOICE_UP_L_MUX",                 "MIC3_ADC",           "ADC3" }, \
	{ "VOICE_UP_L_MUX",                 "MIC4_ADC",           "ADC4" }, \
	{ "VOICE_UP_R_MUX",                 "MIC1_ADC",           "ADC1" }, \
	{ "VOICE_UP_R_MUX",                 "MIC2_ADC",           "ADC2" }, \
	{ "VOICE_UP_R_MUX",                 "MIC3_ADC",           "ADC3" }, \
	{ "VOICE_UP_R_MUX",                 "MIC4_ADC",           "ADC4" }, \
	{ "AUDIO_UP_O_SWITCH",              "ENABLE",             "AUDIO_UP_L_MUX" }, \
	{ "AUDIO_UP_O_SWITCH",              "ENABLE",             "AUDIO_UP_R_MUX" }, \
	{ "VOICE_UP_O_SWITCH",              "ENABLE",             "VOICE_UP_L_MUX" }, \
	{ "VOICE_UP_O_SWITCH",              "ENABLE",             "VOICE_UP_R_MUX" }, \
	{ "AUDIO_MIC12_OUTPUT",             NULL,                 "AUDIO_UP_O_SWITCH" }, \
	{ "VOICE_MIC12_OUTPUT",             NULL,                 "VOICE_UP_O_SWITCH" }, \
	{ "AUDIO_UP_2MIC_48K_O_SWITCH",     "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "VOICE_UP_2MIC_48K_O_SWITCH",     "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "VOICE_UP_2MIC_32K_O_SWITCH",     "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "UPLINK_MIC_OUTPUT",              NULL,                 "AUDIO_UP_2MIC_48K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",              NULL,                 "VOICE_UP_2MIC_48K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",              NULL,                 "VOICE_UP_2MIC_32K_O_SWITCH" } \

#define AUDIO_VOICE_4MIC_ROUTE \
	{ "ADC3",                       NULL,                 "AUDIO_MIC34_INPUT" }, \
	{ "ADC4",                       NULL,                 "AUDIO_MIC34_INPUT" }, \
	{ "MIC3_MUX",                   "MIC3_ADC",           "ADC3" }, \
	{ "MIC3_MUX",                   "MIC4_ADC",           "ADC4" }, \
	{ "MIC3_MUX",                   "MIC1_ADC",           "ADC1" }, \
	{ "MIC4_MUX",                   "MIC3_ADC",           "ADC3" }, \
	{ "MIC4_MUX",                   "MIC4_ADC",           "ADC4" }, \
	{ "MIC4_MUX",                   "MIC2_ADC",           "ADC2" }, \
	{ "MIC34_UP_O_SWITCH",          "ENABLE",             "MIC3_MUX" }, \
	{ "MIC34_UP_O_SWITCH",          "ENABLE",             "MIC4_MUX" }, \
	{ "AUDIO_MIC34_OUTPUT",         NULL,                 "MIC34_UP_O_SWITCH" }, \
	{ "BT_AUDIO_UP_48K_SWITCH",     "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "AUDIO_UP_4MIC_48K_O_SWITCH", "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "VOICE_UP_4MIC_48K_O_SWITCH", "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "VOICE_UP_4MIC_32K_O_SWITCH", "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "VOICE_UP_4MIC_8K_O_SWITCH",  "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "BT_VOICE_UP_32K_SWITCH",     "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "AUDIO_UP_4MIC_48K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "BT_AUDIO_UP_48K_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "VOICE_UP_4MIC_48K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "VOICE_UP_4MIC_32K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "BT_VOICE_UP_32K_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",          NULL,                 "VOICE_UP_4MIC_8K_O_SWITCH" } \

#define MIC34_MIC56_UP_ROUTE \
	{ "MIC3_MUX",            "MIC1_ADC",           "ADC1" }, \
	{ "MIC4_MUX",            "MIC2_ADC",           "ADC2" }, \
	{ "MIC4_MUX",            "MIC1_ADC",           "ADC1" }, \
	{ "MIC5_MUX",            "MIC1_ADC",           "ADC1" }, \
	{ "MIC6_MUX",            "MIC2_ADC",           "ADC2" }, \
	{ "MIC6_MUX",            "MIC1_ADC",           "ADC1" } \

#define AUDIO_VOICE_6MIC_ROUTE \
	{ "ADC5",                         NULL,                 "AUDIO_MIC56_INPUT" }, \
	{ "ADC6",                         NULL,                 "AUDIO_MIC56_INPUT" }, \
	{ "MIC5_MUX",                     "MIC3_ADC",           "ADC3" }, \
	{ "MIC5_MUX",                     "MIC5_ADC",           "ADC5" }, \
	{ "MIC6_MUX",                     "MIC4_ADC",           "ADC4" }, \
	{ "MIC6_MUX",                     "MIC6_ADC",           "ADC6" }, \
	{ "MIC56_UP_O_SWITCH",            "ENABLE",             "MIC5_MUX" }, \
	{ "MIC56_UP_O_SWITCH",            "ENABLE",             "MIC6_MUX" }, \
	{ "AUDIO_MIC56_OUTPUT",           NULL,                 "MIC56_UP_O_SWITCH" }, \
	{ "AUDIO_UP_6MIC_48K_O_SWITCH",   "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "WAKEUP_MAIN_MIC_16K_O_SWITCH", "ENABLE",             "UPLINK_MIC_INPUT" }, \
	{ "UPLINK_MIC_OUTPUT",            NULL,                 "WAKEUP_MAIN_MIC_16K_O_SWITCH" }, \
	{ "UPLINK_MIC_OUTPUT",            NULL,                 "AUDIO_UP_6MIC_48K_O_SWITCH" } \

#define AUDIO_VOICE_LOOP_ROUTE \
	{ "AUDIO_UP_L_MUX",  "MIXER4_SPA0_DOUT",        "SPA0_MIXER4" }, \
	{ "AUDIO_UP_L_MUX",  "AUDIO_0_SRCUP_DOUT",      "AUDIO_DN_0_SRC_EN" }, \
	{ "AUDIO_UP_L_MUX",  "SPA0_MIXER2_DOUT",        "ULTR0_MIXER2" }, \
	{ "AUDIO_UP_R_MUX",  "MIXER4_SPA1_DOUT",        "SPA1_MIXER4" }, \
	{ "AUDIO_UP_R_MUX",  "AUDIO_1_SRCUP_DOUT",      "AUDIO_DN_1_SRC_EN" }, \
	{ "AUDIO_UP_R_MUX",  "SPA1_MIXER2_DOUT",        "ULTR1_MIXER2" }, \
	{ "VOICE_UP_L_MUX",  "MIXER4_SPA0_DOUT",        "SPA0_MIXER4" }, \
	{ "VOICE_UP_L_MUX",  "CODEC_0_SRCUP_DOUT",      "CODEC_DN_0_SRC_EN" }, \
	{ "VOICE_UP_L_MUX",  "SPA2_MIXER2_DOUT",        "ULTR2_MIXER2" }, \
	{ "VOICE_UP_R_MUX",  "MIXER4_SPA1_DOUT",        "SPA1_MIXER4" }, \
	{ "VOICE_UP_R_MUX",  "CODEC_1_SRCUP_DOUT",      "CODEC_DN_1_SRC_EN" }, \
	{ "VOICE_UP_R_MUX",  "SPA3_MIXER2_DOUT",        "ULTR3_MIXER2" } \

#define DMIC12_ROUTE \
	{ "DMIC12_IN_SWITCH",     "ENABLE",              "DMIC12_INPUT" }, \
	{ "ADC1_CIC6_MUX",        "DMIC1_L",             "DMIC12_IN_SWITCH" }, \
	{ "ADC2_CIC6_MUX",        "DMIC1_R",             "DMIC12_IN_SWITCH" }, \
	{ "ADC1_CIC_OUT_DRV",     NULL,                  "ADC1_CIC6_MUX" }, \
	{ "ADC2_CIC_OUT_DRV",     NULL,                  "ADC2_CIC6_MUX" }, \
	{ "ADC1_DIN_MUX",         "ADC1_CIC",            "ADC1_CIC_OUT_DRV" }, \
	{ "ADC2_DIN_MUX",         "ADC2_CIC",            "ADC2_CIC_OUT_DRV" }, \
	{ "DMIC12_OUTPUT",        NULL,                  "ADC1_DIN_MUX" }, \
	{ "DMIC12_OUTPUT",        NULL,                  "ADC2_DIN_MUX" } \

#define DMIC34_ROUTE \
	{ "DMIC34_IN_SWITCH",     "ENABLE",             "DMIC34_INPUT" }, \
	{ "ADC3_CIC6_MUX",        "DMIC2_L",            "DMIC34_IN_SWITCH" }, \
	{ "ADC4_CIC6_MUX",        "DMIC2_R",            "DMIC34_IN_SWITCH" }, \
	{ "ADC3_CIC_OUT_DRV",     NULL,                 "ADC3_CIC6_MUX" }, \
	{ "ADC4_CIC_OUT_DRV",     NULL,                 "ADC4_CIC6_MUX" }, \
	{ "ADC3_DIN_MUX",         "ADC3_CIC",           "ADC3_CIC_OUT_DRV" }, \
	{ "ADC4_DIN_MUX",         "ADC4_CIC",           "ADC4_CIC_OUT_DRV" }, \
	{ "DMIC34_OUTPUT",        NULL,                 "ADC3_DIN_MUX" }, \
	{ "DMIC34_OUTPUT",        NULL,                 "ADC4_DIN_MUX" } \

#define DMIC56_ROUTE \
	{ "DMIC56_IN_SWITCH",     "ENABLE",             "DMIC56_INPUT" }, \
	{ "ADC5_CIC6_MUX",        "DMIC3_L",            "DMIC56_IN_SWITCH" }, \
	{ "ADC6_CIC6_MUX",        "DMIC3_R",            "DMIC56_IN_SWITCH" }, \
	{ "ADC5_CIC_OUT_DRV",     NULL,                 "ADC5_CIC6_MUX" }, \
	{ "ADC6_CIC_OUT_DRV",     NULL,                 "ADC6_CIC6_MUX" }, \
	{ "DMIC56_OUTPUT",        NULL,                 "ADC5_CIC_OUT_DRV" }, \
	{ "DMIC56_OUTPUT",        NULL,                 "ADC6_CIC_OUT_DRV" } \

#define MIC_CIC_PLL \
	{ "DMIC12_IN_SWITCH",         NULL,   "PLL" }, \
	{ "DMIC34_IN_SWITCH",         NULL,   "PLL" }, \
	{ "DMIC56_IN_SWITCH",         NULL,   "PLL" }, \
	{ "AU0_AMIC_INPUT",           NULL,   "PLL" }, \
	{ "DMIC12_IN_SWITCH",         NULL,   "DMIC_CLK" }, \
	{ "DMIC34_IN_SWITCH",         NULL,   "DMIC_CLK" }, \
	{ "DMIC56_IN_SWITCH",         NULL,   "DMIC_CLK" }, \
	{ "AU0_AMIC_INPUT",           NULL,   "SIF_CLK" } \

#define AUDIO_UP_2PA_EC \
	{ "SPA1_DIN_MUX",            "SPA0_DOUT",           "AUDIO_UP_EC_INPUT" }, \
	{ "SPA2_DIN_MUX",            "SPA1_DOUT",           "AUDIO_UP_EC_INPUT" }, \
	{ "EC_2PA_SWITCH",           "ENABLE",              "SPA1_DIN_MUX" }, \
	{ "EC_2PA_SWITCH",           "ENABLE",              "SPA2_DIN_MUX" }, \
	{ "SMARTPA_UP_FIFO_OUTPUT",  NULL,                  "EC_2PA_SWITCH" } \

#define AUDIO_UP_4PA_EC \
	{ "SPA3_DIN_MUX",            "SPA2_DOUT",           "AUDIO_UP_EC_INPUT" }, \
	{ "SPA4_DIN_MUX",            "SPA3_DOUT",           "AUDIO_UP_EC_INPUT" }, \
	{ "EC_4PA_SWITCH",           "ENABLE",              "SPA1_DIN_MUX" }, \
	{ "EC_4PA_SWITCH",           "ENABLE",              "SPA2_DIN_MUX" }, \
	{ "EC_4PA_SWITCH",           "ENABLE",              "SPA3_DIN_MUX" }, \
	{ "EC_4PA_SWITCH",           "ENABLE",              "SPA4_DIN_MUX" }, \
	{ "SMARTPA_UP_FIFO_OUTPUT",  NULL,                  "EC_4PA_SWITCH" } \

#define AUDIO_PLAY_2PA \
	{ "I2S1_RX_INPUT",           NULL,                  "I2S1_SUPPLY" }, \
	{ "I2S1_RX_INPUT",           NULL,                  "I2S1_RX_SUPPLY" }, \
	{ "IV_2PA_SLOT_SWITCH",      "ENABLE",              "I2S1_RX_INPUT" }, \
	{ "IV_2PA_48K_SWITCH",       "ENABLE",              "IV_2PA_SLOT_SWITCH" }, \
	{ "SMARTPA_UP_FIFO_OUTPUT",  NULL,                  "IV_2PA_48K_SWITCH" }, \
	{ "AUDIO_2PA_DN_SWITCH",     "ENABLE",              "AUDIO_DN_FIFO_INPUT" }, \
	{ "AUDIO_0_DN_PGA",          NULL,                  "AUDIO_2PA_DN_SWITCH" }, \
	{ "AUDIO_1_DN_PGA",          NULL,                  "AUDIO_2PA_DN_SWITCH" }, \
	{ "AUDIO_DN_0_SRC_EN",       NULL,                  "AUDIO_0_DN_PGA" }, \
	{ "AUDIO_DN_1_SRC_EN",       NULL,                  "AUDIO_1_DN_PGA" }, \
	{ "SPA0_MIXER4",             "AUDIO_DN_0",          "AUDIO_DN_0_SRC_EN" }, \
	{ "SPA1_MIXER4",             "AUDIO_DN_1",          "AUDIO_DN_1_SRC_EN" }, \
	{ "ULTR0_MIXER2",            "SPA0_DOUT",           "SPA0_MIXER4"  }, \
	{ "ULTR1_MIXER2",            "SPA1_DOUT",           "SPA1_MIXER4"  }, \
	{ "I2S1_TX_L_MUX",           "SPA0_MIXER2_DOUT",    "ULTR0_MIXER2"  }, \
	{ "I2S1_TX_R_MUX",           "SPA1_MIXER2_DOUT",    "ULTR1_MIXER2"  }, \
	{ "I2S1_TX_I2S_SWITCH",      "ENABLE",              "I2S1_TX_L_MUX" }, \
	{ "I2S1_TX_I2S_SWITCH",      "ENABLE",              "I2S1_TX_R_MUX" }, \
	{ "I2S1_TX_DRV",             NULL,                  "I2S1_TX_I2S_SWITCH" }, \
	{ "I2S1_TX_OUTPUT",          NULL,                  "I2S1_TX_DRV" }, \
	{ "I2S1_TX_OUTPUT",          NULL,                  "I2S1_SUPPLY" }, \
	{ "AUDIO_DN_FIFO_INPUT",     NULL,                  "PLL" } \

#define AUDIO_PLAY_4PA \
	{ "IV_4PA_SLOT_SWITCH",      "ENABLE",              "I2S1_RX_INPUT" }, \
	{ "ASP_CODEC_IV_4PA_SWITCH", "ENABLE",              "IV_4PA_SLOT_SWITCH" }, \
	{ "SMARTPA_UP_FIFO_OUTPUT",  NULL,                  "ASP_CODEC_IV_4PA_SWITCH" }, \
	{ "AUDIO_4PA_DN_SWITCH",   "ENABLE",                "AUDIO_DN_FIFO_INPUT" }, \
	{ "AUDIO_0_DN_PGA",        NULL,                    "AUDIO_4PA_DN_SWITCH" }, \
	{ "AUDIO_1_DN_PGA",        NULL,                    "AUDIO_4PA_DN_SWITCH" }, \
	{ "AUDIO_2_DN_PGA",        NULL,                    "AUDIO_4PA_DN_SWITCH" }, \
	{ "AUDIO_3_DN_PGA",        NULL,                    "AUDIO_4PA_DN_SWITCH" }, \
	{ "AUDIO_DN_0_SRC_EN",     NULL,                    "AUDIO_0_DN_PGA" }, \
	{ "AUDIO_DN_1_SRC_EN",     NULL,                    "AUDIO_1_DN_PGA" }, \
	{ "AUDIO_DN_2_SRC_EN",     NULL,                    "AUDIO_2_DN_PGA" }, \
	{ "AUDIO_DN_3_SRC_EN",     NULL,                    "AUDIO_3_DN_PGA" }, \
	{ "CODEC_3PA_DN_SWITCH",   "ENABLE",                "CODEC_DN_FIFO_INPUT" }, \
	{ "CODEC_0_DN_PGA",        NULL,                    "CODEC_3PA_DN_SWITCH" }, \
	{ "CODEC_1_DN_PGA",        NULL,                    "CODEC_3PA_DN_SWITCH" }, \
	{ "CODEC_2_DN_PGA",        NULL,                    "CODEC_3PA_DN_SWITCH" }, \
	{ "CODEC_DN_0_SRC_EN",     NULL,                    "CODEC_0_DN_PGA" }, \
	{ "CODEC_DN_1_SRC_EN",     NULL,                    "CODEC_1_DN_PGA" }, \
	{ "CODEC_DN_2_SRC_EN",     NULL,                    "CODEC_2_DN_PGA" }, \
	{ "I2S1_TDM_TX_0_MUX",     "CODEC_0_SRCUP_DOUT",    "CODEC_DN_0_SRC_EN" }, \
	{ "I2S1_TDM_TX_1_MUX",     "CODEC_1_SRCUP_DOUT",    "CODEC_DN_1_SRC_EN" }, \
	{ "I2S1_TDM_TX_2_MUX",     "AUDIO_0_SRCUP_DOUT",    "AUDIO_DN_0_SRC_EN" }, \
	{ "I2S1_TDM_TX_3_MUX",     "AUDIO_1_SRCUP_DOUT",    "AUDIO_DN_1_SRC_EN" }, \
	{ "SPA2_MIXER4",           "AUDIO_DN_2",            "AUDIO_DN_2_SRC_EN" }, \
	{ "SPA3_MIXER4",           "AUDIO_DN_3",            "AUDIO_DN_3_SRC_EN" }, \
	{ "SPA0_MIXER4",           "CODEC_DN_0",            "CODEC_DN_0_SRC_EN" }, \
	{ "SPA1_MIXER4",           "CODEC_DN_1",            "CODEC_DN_1_SRC_EN" }, \
	{ "SPA2_MIXER4",           "CODEC_DN_2",            "CODEC_DN_2_SRC_EN" }, \
	{ "ULTR2_MIXER2",          "SPA2_DOUT",             "SPA2_MIXER4" }, \
	{ "ULTR3_MIXER2",          "SPA3_DOUT",             "SPA3_MIXER4" }, \
	{ "I2S1_TDM_TX_0_MUX",     "SPA0_MIXER2_DOUT",      "ULTR0_MIXER2" }, \
	{ "I2S1_TDM_TX_1_MUX",     "SPA1_MIXER2_DOUT",      "ULTR1_MIXER2" }, \
	{ "I2S1_TDM_TX_2_MUX",     "SPA2_MIXER2_DOUT",      "ULTR2_MIXER2" }, \
	{ "I2S1_TDM_TX_3_MUX",     "SPA3_MIXER2_DOUT",      "ULTR3_MIXER2" }, \
	{ "I2S1_TX_TDM_SWITCH",    "ENABLE",                "I2S1_TDM_TX_0_MUX" }, \
	{ "I2S1_TX_TDM_SWITCH",    "ENABLE",                "I2S1_TDM_TX_1_MUX" }, \
	{ "I2S1_TX_TDM_SWITCH",    "ENABLE",                "I2S1_TDM_TX_2_MUX" }, \
	{ "I2S1_TX_TDM_SWITCH",    "ENABLE",                "I2S1_TDM_TX_3_MUX" }, \
	{ "I2S1_TX_DRV",           NULL,                    "I2S1_TX_TDM_SWITCH" }, \
	{ "I2S1_TX_OUTPUT",        NULL,                    "I2S1_TX_DRV" }, \
	{ "CODEC_DN_FIFO_INPUT",     NULL,                  "PLL" } \

#define AUDIO_CARTKIT \
	{ "CARKIT_48K_SWITCH",     "ENABLE", "CARKIT_INPUT" }, \
	{ "CARKIT_OUTPUT",         NULL,     "CARKIT_48K_SWITCH" } \

#define BT_DOWNLINK \
	{ "I2S2_TX_MIXER2",         "MIXER4_SPA0_DOUT",          "SPA0_MIXER4" }, \
	{ "I2S2_TX_MIXER2",         "MIXER4_SPA1_DOUT",          "SPA1_MIXER4" }, \
	{ "BT_TX_SRCDN_MUX",        "MIXER4_SPA0_DOUT",          "SPA0_MIXER4" }, \
	{ "I2S2_TX_R_SRCDN_SWITCH", "ENABLE",                    "SPA1_MIXER4" }, \
	{ "BT_TX_SRCDN_MUX",        "I2S2_MIXER2",               "I2S2_TX_MIXER2" }, \
	{ "I2S2_TX_L_SRCDN_SWITCH", "ENABLE",                    "BT_TX_SRCDN_MUX" }, \
	{ "I2S2_TX_L_SWITCH",       "ENABLE",                    "I2S2_TX_L_SRCDN_SWITCH" }, \
	{ "I2S2_TX_R_SEL_MUX",      "I2S2_TX_L_BTMATCH",         "I2S2_TX_L_SRCDN_SWITCH" }, \
	{ "I2S2_TX_R_SEL_MUX",      "I2S2_TX_R_BTMATCH",         "I2S2_TX_R_SRCDN_SWITCH" }, \
	{ "I2S2_TX_R_SWITCH",       "ENABLE",                    "I2S2_TX_R_SEL_MUX" }, \
	{ "I2S2_TX_DRV",            NULL,                        "I2S2_TX_L_SWITCH" }, \
	{ "I2S2_TX_DRV",            NULL,                        "I2S2_TX_R_SWITCH" }, \
	{ "I2S2_TX_OUTPUT",         NULL,                        "I2S2_TX_DRV" }, \
	{ "I2S2_TX_OUTPUT",         NULL,                        "I2S2_SUPPLY" }

#define A2DP_OFFLOAD_ROUTE \
	{ "A2DP_OFFLOAD_SWITCH",         "ENABLE",            "A2DP_OFFLOAD_INPUT" }, \
	{ "I2S2_TX_SEL_MUX",             "BT_FIFO_DN",        "A2DP_OFFLOAD_SWITCH" }, \
	{ "I2S2_TX_DRV",                 NULL,                "I2S2_TX_SEL_MUX" }, \
	{ "A2DP_OFFLOAD_OUTPUT",         NULL,                "I2S2_TX_DRV" }, \
	{ "A2DP_OFFLOAD_OUTPUT",         NULL,                "I2S2_SUPPLY" } \

#define ULTRA_ROUTE \
	{ "ULTRASONIC_DN_SWITCH",     "ENABLE",            "ULTRA_INPUT" }, \
	{ "ULTR_DN_PGA",              NULL,                "ULTRASONIC_DN_SWITCH" }, \
	{ "ULTR0_MIXER2",             "ULTR_DOUT",         "ULTR_DN_PGA" }, \
	{ "ULTR1_MIXER2",             "ULTR_DOUT",         "ULTR_DN_PGA" }, \
	{ "ULTR2_MIXER2",             "ULTR_DOUT",         "ULTR_DN_PGA" }, \
	{ "ULTR3_MIXER2",             "ULTR_DOUT",         "ULTR_DN_PGA" }, \
	{ "ULTR_UP_MUX",              "DMIC1_L",           "DMIC12_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "DMIC1_R",           "DMIC12_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "DMIC2_L",           "DMIC34_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "DMIC2_R",           "DMIC34_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "DMIC3_L",           "DMIC56_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "DMIC3_R",           "DMIC56_IN_SWITCH" }, \
	{ "ULTR_UP_MUX",              "AU0_L",             "DA_AD_LOOP_MUX" }, \
	{ "ULTR_UP_MUX",              "AU0_R",             "DA_AD_LOOP_MUX" }, \
	{ "ULTR_UP_MUX",              "AU0_MIC3",          "AU_EN_MIXER" }, \
	{ "ULTRASONIC_UP_SWITCH",     "ENABLE",            "ULTR_UP_MUX" }, \
	{ "ULTRA_OUTPUT",             NULL,                "ULTRASONIC_UP_SWITCH" } \

#define BT_UPLINK \
	{ "I2S2_RX_INPUT",             NULL,         "I2S2_SUPPLY" }, \
	{ "I2S2_RX_INPUT",             NULL,         "I2S2_RX_SUPPLY" }, \
	{ "I2S2_RX_L_SWITCH",          "ENABLE",     "I2S2_RX_INPUT" }, \
	{ "I2S2_RX_R_SWITCH",          "ENABLE",     "I2S2_RX_INPUT" }, \
	{ "I2S2_RX_L_PGA",             NULL,         "I2S2_RX_L_SWITCH" }, \
	{ "I2S2_RX_R_PGA",             NULL,         "I2S2_RX_R_SWITCH" }, \
	{ "I2S2_RX_L_SRC_SWITCH",      "ENABLE",     "I2S2_RX_L_PGA" }, \
	{ "I2S2_RX_R_SRC_SWITCH",      "ENABLE",     "I2S2_RX_R_PGA" }, \
	{ "I2S2_RX_SRCUP_8K_SWITCH",   "ENABLE",     "I2S2_RX_L_PGA" }, \
	{ "I2S2_RX_SRCUP_8K_SWITCH",   "ENABLE",     "I2S2_RX_R_PGA" }, \
	{ "I2S2_RX_SRCUP_16K_SWITCH",  "ENABLE",     "I2S2_RX_L_PGA" }, \
	{ "I2S2_RX_SRCUP_16K_SWITCH",  "ENABLE",     "I2S2_RX_R_PGA" }, \
	{ "I2S2_RX_SRCUP_32K_SWITCH",  "ENABLE",     "I2S2_RX_L_PGA" }, \
	{ "I2S2_RX_SRCUP_32K_SWITCH",  "ENABLE",     "I2S2_RX_R_PGA" }, \
	{ "VOICE_UP_L_MUX",            NULL,         "I2S2_RX_L_PGA" }, \
	{ "VOICE_UP_R_MUX",            NULL,         "I2S2_RX_R_PGA" }, \
	{ "VOICE_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_L_SRC_SWITCH" }, \
	{ "VOICE_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_R_SRC_SWITCH" }, \
	{ "AUDIO_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_L_SRC_SWITCH" }, \
	{ "AUDIO_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_R_SRC_SWITCH" }, \
	{ "VOICE_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_8K_SWITCH" }, \
	{ "VOICE_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_8K_SWITCH" }, \
	{ "AUDIO_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_8K_SWITCH" }, \
	{ "AUDIO_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_8K_SWITCH" }, \
	{ "VOICE_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_16K_SWITCH" }, \
	{ "VOICE_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_16K_SWITCH" }, \
	{ "AUDIO_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_16K_SWITCH" }, \
	{ "AUDIO_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_16K_SWITCH" }, \
	{ "VOICE_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_32K_SWITCH" }, \
	{ "VOICE_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_32K_SWITCH" }, \
	{ "AUDIO_UP_L_MUX",            "I2S2_RX_L",  "I2S2_RX_SRCUP_32K_SWITCH" }, \
	{ "AUDIO_UP_R_MUX",            "I2S2_RX_R",  "I2S2_RX_SRCUP_32K_SWITCH" } \

#define MMI_ROUTE \
	{ "I2S2_BLUETOOTH_LOOP_OUTPUT",   NULL,              "I2S2_SUPPLY" }, \
	{ "I2S2_BLUETOOTH_LOOP_SWITCH",   "ENABLE",          "I2S2_BLUETOOTH_LOOP_INPUT" }, \
	{ "I2S2_BLUETOOTH_LOOP_OUTPUT",   NULL,              "I2S2_BLUETOOTH_LOOP_SWITCH" } \

#define AXI_2_TDM_ROUTE \
	{ "AUDIO_INTF1_SWITCH",     NULL,        "PLL"}, \
	{ "AUDIO_INTF2_SWITCH",     NULL,        "PLL"}, \
	{ "AUDIO_INTF1_SWITCH",     "ENABLE",    "INTF1_INPUT" }, \
	{ "AUDIO_INTF2_SWITCH",     "ENABLE",    "INTF2_INPUT" }, \
	{ "INTF1_OUTPUT",           NULL,        "AUDIO_INTF1_SWITCH" }, \
	{ "INTF2_OUTPUT",           NULL,        "AUDIO_INTF2_SWITCH" } \

#define SPA_2PA_BYPASS_ROUTE \
	{ "SPK_2PA_BYPASS_SWITCH",   "ENABLE",      "SPK_SPA_INPUT" }, \
	{ "RCV_2PA_BYPASS_SWITCH",   "ENABLE",      "RCV_SPA_INPUT" }, \
	{ "SPA_OUTPUT",               NULL,         "SPK_2PA_BYPASS_SWITCH" }, \
	{ "SPA_OUTPUT",               NULL,         "RCV_2PA_BYPASS_SWITCH" } \

#define SPA_4PA_BYPASS_ROUTE \
	{ "SPK_4PA_BYPASS_SWITCH",   "ENABLE",      "SPK_SPA_INPUT" }, \
	{ "RCV_4PA_BYPASS_SWITCH",   "ENABLE",      "RCV_SPA_INPUT" }, \
	{ "SPA_OUTPUT",               NULL,         "SPK_4PA_BYPASS_SWITCH" }, \
	{ "SPA_OUTPUT",               NULL,         "RCV_4PA_BYPASS_SWITCH" } \

#define ANA_AMIC_ROUTE \
	{ "AU_EN_MIXER",          "AU0_L_EN",           "AU0_AMIC_INPUT" }, \
	{ "AU_EN_MIXER",          "AU0_R_EN",           "AU0_AMIC_INPUT" }, \
	{ "AU_EN_MIXER",          "AU0_MIC3_EN",        "AU0_AMIC_INPUT" }, \
	{ "DA_AD_LOOP_MUX",       "NORMAL",             "AU_EN_MIXER" }, \
	{ "CIC_ADC1_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC1_CIC6_MUX",        "AU0_L",              "CIC_ADC1_SWITCH" }, \
	{ "ADC1_CIC6_MUX",        "AU0_R",              "CIC_ADC1_SWITCH" }, \
	{ "ADC1_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC1_SWITCH" }, \
	{ "CIC_ADC2_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC2_CIC6_MUX",        "AU0_L",              "CIC_ADC2_SWITCH" }, \
	{ "ADC2_CIC6_MUX",        "AU0_R",              "CIC_ADC2_SWITCH" }, \
	{ "ADC2_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC2_SWITCH" }, \
	{ "CIC_ADC3_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC3_CIC6_MUX",        "AU0_L",              "CIC_ADC3_SWITCH" }, \
	{ "ADC3_CIC6_MUX",        "AU0_R",              "CIC_ADC3_SWITCH" }, \
	{ "ADC3_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC3_SWITCH" }, \
	{ "CIC_ADC4_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC4_CIC6_MUX",        "AU0_L",              "CIC_ADC4_SWITCH" }, \
	{ "ADC4_CIC6_MUX",        "AU0_R",              "CIC_ADC4_SWITCH" }, \
	{ "ADC4_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC4_SWITCH" }, \
	{ "CIC_ADC5_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC5_CIC6_MUX",        "AU0_L",              "CIC_ADC5_SWITCH" }, \
	{ "ADC5_CIC6_MUX",        "AU0_R",              "CIC_ADC5_SWITCH" }, \
	{ "ADC5_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC5_SWITCH" }, \
	{ "CIC_ADC6_SWITCH",      "ENABLE",             "DA_AD_LOOP_MUX" }, \
	{ "ADC6_CIC6_MUX",        "AU0_L",              "CIC_ADC6_SWITCH" }, \
	{ "ADC6_CIC6_MUX",        "AU0_R",              "CIC_ADC6_SWITCH" }, \
	{ "ADC6_CIC6_MUX",        "AU0_MIC3",           "CIC_ADC6_SWITCH" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC1_DIN_MUX" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC2_DIN_MUX" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC3_DIN_MUX" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC4_DIN_MUX" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC5_CIC_OUT_DRV" }, \
	{ "AU0_AMIC_OUTPUT",      NULL,                 "ADC6_CIC_OUT_DRV" } \

#define AUDIO_PLAY_EAR \
	{ "DACL_DIN_MUX",           "SPA0_MIXER2_DOUT",     "ULTR0_MIXER2" }, \
	{ "DACL_DIN_MUX",           "SPA1_MIXER2_DOUT",     "ULTR1_MIXER2" }, \
	{ "DACL_DIN_MUX",           "SPA2_MIXER2_DOUT",     "ULTR2_MIXER2" }, \
	{ "DACL_DIN_MUX",           "SPA3_MIXER2_DOUT",     "ULTR3_MIXER2" }, \
	{ "DACL_DIN_MUX",           "MIXER4_SPA0_DOUT",     "SPA0_MIXER4" }, \
	{ "DACL_DIN_MUX",           "MIXER4_SPA1_DOUT",     "SPA1_MIXER4" }, \
	{ "DACL_DIN_MUX",           "MIXER4_SPA2_DOUT",     "SPA2_MIXER4" }, \
	{ "DACL_DIN_MUX",           "I2S1_DIN",             "I2S1_RX_INPUT" }, \
	{ "DACL_SRCUP_SWITCH",      "ENABLE",               "DACL_DIN_MUX" }, \
	{ "DACL_SRCUP_ULTR_SWITCH", "ENABLE",               "DACL_DIN_MUX" }, \
	{ "DACL_SRCUP_MUX",         "DACL_SRCUP_DOUT",      "DACL_SRCUP_SWITCH" }, \
	{ "DACL_SRCUP_MUX",         "DACL_ULTR_DOUT",       "DACL_SRCUP_ULTR_SWITCH" }, \
	{ "SDML",                   NULL,                   "DACL_SRCUP_MUX" }, \
	{ "AD_DA_LOOP_MUX",         "NORMAL",               "SDML" }, \
	{ "AU_EN_MIXER",            "AU_DACL_EN",           "AD_DA_LOOP_MUX" }, \
	{ "AU_EN_MIXER",            "AU_DAC_ULTR_EN",       "AD_DA_LOOP_MUX" }, \
	{ "DACL_OUTPUT",            NULL,                   "AU_EN_MIXER" }, \
	{ "DACL_OUTPUT",            NULL,                   "PLL" }, \
	{ "DACL_OUTPUT",            NULL,                   "SIF_CLK" } \

#define MAD_LP_SINGLE_MIC_PATH \
	{ "MAD_LP_SINGLE_MIC_SWITCH",   "ENABLE",    "MAD_DMIC_INPUT" }, \
	{ "MAD_DMIC_OUTPUT",            NULL,        "MAD_LP_SINGLE_MIC_SWITCH" } \

#define MAD_NORM_SINGLE_MIC_PATH \
	{ "MAD_NORM_SINGLE_MIC_SWITCH", "ENABLE",    "MAD_CODECLESS_INPUT" }, \
	{ "MAD_CODECLESS_OUTPUT",       NULL,        "MAD_NORM_SINGLE_MIC_SWITCH" } \

#define AU_KCONTROLS \
	SOC_SINGLE("DACL_SRCUP_MODE", \
		SRCUP_CTRL0_REG, DACL_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(DACL_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("FS_DACL_SRCUP_DIN", \
		FS_CTRL8_REG, FS_DACL_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_DACL_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("FS_DACL_SRCUP_DOUT", \
		FS_CTRL8_REG, FS_DACL_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_DACL_SRCUP_DOUT_LEN), 0) \

static const struct snd_kcontrol_new g_kcontrols[] = {
	AU_KCONTROLS
};

static int i2s2_rx_srcup_8k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x0 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x0 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		snd_soc_component_update_bits(codec, SRCUP_CTRL1_REG,
			mask_on_bit(I2S2_RX_L_SRCUP_SRC_MODE_LEN, I2S2_RX_L_SRCUP_SRC_MODE_OFFSET) |
			mask_on_bit(I2S2_RX_R_SRCUP_SRC_MODE_LEN, I2S2_RX_R_SRCUP_SRC_MODE_OFFSET),
			0x5 << I2S2_RX_L_SRCUP_SRC_MODE_OFFSET | 0x5 << I2S2_RX_R_SRCUP_SRC_MODE_OFFSET);
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET),
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits(codec, SRCUP_CTRL1_REG,
			mask_on_bit(I2S2_RX_L_SRCUP_SRC_MODE_LEN, I2S2_RX_L_SRCUP_SRC_MODE_OFFSET) |
			mask_on_bit(I2S2_RX_R_SRCUP_SRC_MODE_LEN, I2S2_RX_R_SRCUP_SRC_MODE_OFFSET), 0);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x3 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x3 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	return 0;
}

static int i2s2_rx_srcup_16k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x1 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x1 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		snd_soc_component_update_bits(codec, SRCUP_CTRL1_REG,
			mask_on_bit(I2S2_RX_L_SRCUP_SRC_MODE_LEN, I2S2_RX_L_SRCUP_SRC_MODE_OFFSET) |
			mask_on_bit(I2S2_RX_R_SRCUP_SRC_MODE_LEN, I2S2_RX_R_SRCUP_SRC_MODE_OFFSET),
			0x1 << I2S2_RX_L_SRCUP_SRC_MODE_OFFSET | 0x1 << I2S2_RX_R_SRCUP_SRC_MODE_OFFSET);
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET),
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits(codec, SRCUP_CTRL1_REG,
			mask_on_bit(I2S2_RX_L_SRCUP_SRC_MODE_LEN, I2S2_RX_L_SRCUP_SRC_MODE_OFFSET) |
			mask_on_bit(I2S2_RX_R_SRCUP_SRC_MODE_LEN, I2S2_RX_R_SRCUP_SRC_MODE_OFFSET), 0);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x3 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x3 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	return 0;
}

static int i2s2_rx_srcup_32k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x2 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x2 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x3 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET),
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(I2S2_RX_L_SRCUP_CLKEN_OFFSET) | BIT(I2S2_RX_R_SRCUP_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits(codec, FS_CTRL7_REG,
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DIN_LEN, FS_I2S2_RX_L_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_L_SRCUP_DOUT_LEN, FS_I2S2_RX_L_SRCUP_DOUT_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DIN_LEN, FS_I2S2_RX_R_SRCUP_DIN_OFFSET) |
			mask_on_bit(FS_I2S2_RX_R_SRCUP_DOUT_LEN, FS_I2S2_RX_R_SRCUP_DOUT_OFFSET),
			0x3 << FS_I2S2_RX_L_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_L_SRCUP_DOUT_OFFSET |
			0x3 << FS_I2S2_RX_R_SRCUP_DIN_OFFSET | 0x4 << FS_I2S2_RX_R_SRCUP_DOUT_OFFSET);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	return 0;
}

static const struct snd_kcontrol_new dapm_i2s2_rx_srcup_8k_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, BT_SRCUP_8k, 1, 0);
static const struct snd_kcontrol_new dapm_i2s2_rx_srcup_16k_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, BT_SRCUP_16k, 1, 0);
static const struct snd_kcontrol_new dapm_i2s2_rx_srcup_32k_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, BT_SRCUP_32k, 1, 0);

static const char * const i2s2_tx_sel_mux_texts[] = {
	"I2S2_TX_BTMATCH",
	"BT_FIFO_DN",
};

static const struct soc_enum i2s2_tx_sel_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, I2S2_TX_SEL_OFFSET,
		ARRAY_SIZE(i2s2_tx_sel_mux_texts), i2s2_tx_sel_mux_texts);
static const struct snd_kcontrol_new dapm_i2s2_tx_sel_mux_controls =
	SOC_DAPM_ENUM("Mux", i2s2_tx_sel_mux_enum);

static const struct snd_kcontrol_new i2s2_tx_mixer2_controls[] = {
	SOC_DAPM_SINGLE("MIXER4_SPA0_DOUT", I2S2_TX_MIXER2_CTRL_REG,
		I2S2_TX_MIXER2_IN1_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("MIXER4_SPA1_DOUT", I2S2_TX_MIXER2_CTRL_REG,
		I2S2_TX_MIXER2_IN2_MUTE_OFFSET, 1, 1),
};

static const char * const bt_tx_srcdn_mux_texts[] = {
	"I2S2_MIXER2",
	"MIXER4_SPA0_DOUT",
};

static const struct soc_enum bt_tx_srcdn_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX1_REG, BT_TX_SRCDN_DIN_SEL_OFFSET,
		ARRAY_SIZE(bt_tx_srcdn_mux_texts), bt_tx_srcdn_mux_texts);

static const struct snd_kcontrol_new dapm_bt_tx_srcdn_mux_controls =
	SOC_DAPM_ENUM("Mux", bt_tx_srcdn_mux_enum);

#define PLATFORM_WIDGET \
	SND_SOC_DAPM_MUX("I2S2_TX_SEL_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_i2s2_tx_sel_mux_controls), \
	SND_SOC_DAPM_MUX("BT_TX_SRCDN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_bt_tx_srcdn_mux_controls), \
	SND_SOC_DAPM_MIXER("I2S2_TX_MIXER2", CODEC_CLK_EN0_REG, \
		I2S2_TX_MIXER2_CLKEN_OFFSET, 0, i2s2_tx_mixer2_controls, \
		ARRAY_SIZE(i2s2_tx_mixer2_controls)), \
	SND_SOC_DAPM_SWITCH_E("I2S2_RX_SRCUP_8K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_i2s2_rx_srcup_8k_switch_controls, i2s2_rx_srcup_8k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("I2S2_RX_SRCUP_16K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_i2s2_rx_srcup_16k_switch_controls, i2s2_rx_srcup_16k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("I2S2_RX_SRCUP_32K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_i2s2_rx_srcup_32k_switch_controls, i2s2_rx_srcup_32k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static int sdml_pga_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;
	AUDIO_LOGI("sdml_pga power event %d", event);

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/* src16 sdm enable */
		snd_soc_component_update_bits(codec, CODEC_CLK_EN4_REG,
			BIT(DACL_UP16_CLKEN_OFFSET) | BIT(DACL_SDM_CLKEN_OFFSET),
			BIT(DACL_UP16_CLKEN_OFFSET) | BIT(DACL_SDM_CLKEN_OFFSET));
		snd_soc_component_update_bits(codec, DAC_FILTER_CTRL_REG,
			BIT(SDM_L_CALT_VLD_OFFSET), BIT(SDM_L_CALT_VLD_OFFSET));

		/* au dacl enable */
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(DACL_CLKEN_OFFSET), BIT(DACL_CLKEN_OFFSET));
		
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/* src16 sdm disable */
		snd_soc_component_update_bits(codec, CODEC_CLK_EN4_REG,
			BIT(DACL_UP16_CLKEN_OFFSET) | BIT(DACL_SDM_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits(codec, DAC_FILTER_CTRL_REG,
			BIT(SDM_L_CALT_VLD_OFFSET), 0);
		
		/* au dacl disable */
		snd_soc_component_update_bits(codec, CODEC_CLK_EN2_REG,
			BIT(DACL_CLKEN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

static const struct snd_kcontrol_new dapm_dacl_srcup_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_CLK_EN2_REG, DACL_SRCUP_CLKEN_OFFSET, 1, 0);
static const struct snd_kcontrol_new dapm_dacl_srcup_ultr_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_CLK_EN4_REG, DACL_SRCUP_ULTR_CLKEN_OFFSET, 1, 0);

static const struct snd_kcontrol_new dapm_cic_adc3_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", ADC3_CIC_D16_CLKEN_REG, ADC3_CIC_D16_CLKEN_OFFSET, 1, 0);
static const struct snd_kcontrol_new dapm_cic_adc4_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", ADC4_CIC_D16_CLKEN_REG, ADC4_CIC_D16_CLKEN_OFFSET, 1, 0);
static const struct snd_kcontrol_new dapm_cic_adc5_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", ADC5_CIC_D16_CLKEN_REG, ADC5_CIC_D16_CLKEN_OFFSET, 1, 0);
static const struct snd_kcontrol_new dapm_cic_adc6_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", ADC6_CIC_D16_CLKEN_REG, ADC6_CIC_D16_CLKEN_OFFSET, 1, 0);

static const struct snd_kcontrol_new au_en_mixer[] = {
	SOC_DAPM_SINGLE("AU_DAC_ULTR_EN", SIF0_CTRL_REG, SIF0_DAC_ULTR_EN_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AU0_MIC3_EN",    SIF0_CTRL_REG, SIF0_ADC_MIC3_EN_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AU0_R_EN",       SIF0_CTRL_REG, SIF0_ADCR_EN_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AU0_L_EN",       SIF0_CTRL_REG, SIF0_ADCL_EN_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AU_DACL_EN",     SIF0_CTRL_REG, SIF0_DACL_EN_OFFSET, 1, 0),
};

static const char * const dacl_din_mux_texts[] = {
	"SPA0_MIXER2_DOUT",
	"SPA1_MIXER2_DOUT",
	"SPA2_MIXER2_DOUT",
	"SPA3_MIXER2_DOUT",
	"MIXER4_SPA0_DOUT",
	"MIXER4_SPA1_DOUT",
	"MIXER4_SPA2_DOUT",
	"MIXER4_SPA3_DOUT",
	"I2S1_DIN",
};

static const struct soc_enum dacl_din_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DACL_DC_OFFSET_REG, DACL_SRCUP_DIN_SEL_OFFSET,
		ARRAY_SIZE(dacl_din_mux_texts), dacl_din_mux_texts);

static const struct snd_kcontrol_new dacl_din_mux_controls =
	SOC_DAPM_ENUM("Mux", dacl_din_mux_enum);

static const char * const dacl_srcup_mux_texts[] = {
	"DACL_SRCUP_DOUT",
	"DACL_ULTR_DOUT",
};

static const struct soc_enum dacl_srcup_mux_enum =
	SOC_ENUM_SINGLE(DAC_FILTER_CTRL_REG, DACL_SRCUP_MUX_SEL_OFFSET,
		ARRAY_SIZE(dacl_srcup_mux_texts), dacl_srcup_mux_texts);

static const struct snd_kcontrol_new dacl_srcup_mux_controls =
	SOC_DAPM_ENUM("Mux", dacl_srcup_mux_enum);

static const char *const ad_da_loopback_mux_texts[] = {
	"NORMAL",
	"AD_DA_LOOP",
};

static const struct soc_enum ad_da_loopback_mux_enum =
	SOC_ENUM_SINGLE(SIF_CTRL_REG, SIF_P2S_LOOP_OFFSET,
		ARRAY_SIZE(ad_da_loopback_mux_texts), ad_da_loopback_mux_texts);

static const struct snd_kcontrol_new ad_da_loopback_mux =
	SOC_DAPM_ENUM("Mux", ad_da_loopback_mux_enum);

/* au da_ad loopback mux */
static const char *const da_ad_loopback_mux_texts[] = {
	"NORMAL",
	"DA_AD_LOOP",
	"DA_SOUT_AD_SIN_LOOP",
};

static const struct soc_enum da_ad_loopback_mux_enum =
	SOC_ENUM_SINGLE(SIF_CTRL_REG, SIF_S2P_LOOP_OFFSET,
		ARRAY_SIZE(da_ad_loopback_mux_texts), da_ad_loopback_mux_texts);

static const struct snd_kcontrol_new da_ad_loopback_mux =
	SOC_DAPM_ENUM("Mux", da_ad_loopback_mux_enum);

#define AU_WIDGET \
	SND_SOC_DAPM_OUTPUT("DACL_OUTPUT"), \
	SND_SOC_DAPM_SWITCH("CIC_ADC3_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_cic_adc3_switch_controls), \
	SND_SOC_DAPM_SWITCH("CIC_ADC4_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_cic_adc4_switch_controls), \
	SND_SOC_DAPM_SWITCH("CIC_ADC5_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_cic_adc5_switch_controls), \
	SND_SOC_DAPM_SWITCH("CIC_ADC6_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_cic_adc6_switch_controls), \
	SND_SOC_DAPM_SWITCH("DACL_SRCUP_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_dacl_srcup_switch_controls), \
	SND_SOC_DAPM_SWITCH("DACL_SRCUP_ULTR_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_dacl_srcup_ultr_switch_controls), \
	SND_SOC_DAPM_PGA_E("SDML", SND_SOC_NOPM, 0, 0, NULL, 0, \
		sdml_pga_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)), \
	SND_SOC_DAPM_MIXER("AU_EN_MIXER", CODEC_CLK_EN4_REG, SIF_MST_CLKEN_OFFSET, 0, \
		au_en_mixer, ARRAY_SIZE(au_en_mixer)), \
	SND_SOC_DAPM_MUX("DACL_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dacl_din_mux_controls), \
	SND_SOC_DAPM_MUX("DACL_SRCUP_MUX", \
		SND_SOC_NOPM, 0, 0, &dacl_srcup_mux_controls), \
	SND_SOC_DAPM_MUX("AD_DA_LOOP_MUX", \
		SND_SOC_NOPM, 0, 0, &ad_da_loopback_mux), \
	SND_SOC_DAPM_MUX("DA_AD_LOOP_MUX", \
		SND_SOC_NOPM, 0, 0, &da_ad_loopback_mux) \

static int sif_clk_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	IN_FUNCTION;
	AUDIO_LOGI("sif_clk power event %d", event);

	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	struct codec_platform_data *priv = snd_soc_component_get_drvdata(codec);
	int ret;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		soc_mad_request_pinctrl_state(HIGH_FREQ_MODE);
		if (!IS_ERR_OR_NULL(priv->sif_clk)) {
			ret = clk_prepare_enable(priv->sif_clk);
			if (ret != 0)
				AUDIO_LOGE("sif clk enable failed, ret: %d", ret);
		}
		break;
	case SND_SOC_DAPM_POST_PMD:
		soc_mad_release_pinctrl_state(HIGH_FREQ_MODE);
		if (!IS_ERR_OR_NULL(priv->sif_clk)) {
			clk_disable_unprepare(priv->sif_clk);
		}
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

#define AU_SUPPLY_WIDGET \
	SND_SOC_DAPM_SUPPLY("SIF_CLK", SND_SOC_NOPM, 0, 0, \
		sif_clk_power_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static void spa1_bypass_power_on_event(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
		BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
		max_val_on_bit(SPA1_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA1_SMARTPA_PROTECT_CLK_RST_EN_OFFSET, 0);
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
		BIT(SPA1_LTM_CLK_RST_EN_OFFSET) | BIT(SPA1_BCN_CLK_RST_EN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, SPA1BYPASS_CFG,
		max_val_on_bit(SPA1_BYPASS_EN_LEN), max_val_on_bit(SPA1_BYPASS_EN_LEN));
	snd_soc_component_update_bits_with_lock(codec, SPA1_PRE_CFG0,
		BIT(SPA1_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA1_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA1_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA1_CFG_PRE_PILOTTONE_CLKEN_OFFSET),
		BIT(SPA1_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA1_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA1_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA1_CFG_PRE_PILOTTONE_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
		(max_val_on_bit(SPA1_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA1_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA1_LTM_CLK_RST_EN_OFFSET) | BIT(SPA1_BCN_CLK_RST_EN_OFFSET),
		(max_val_on_bit(SPA1_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA1_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA1_LTM_CLK_RST_EN_OFFSET) | BIT(SPA1_BCN_CLK_RST_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
		BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
}

static int spa1_bypass_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("spa1 bypass power event enter %d", event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
        spa1_bypass_power_on_event(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
        snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		    BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static void spa2_bypass_power_on_event(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
		BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
		max_val_on_bit(SPA2_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA2_SMARTPA_PROTECT_CLK_RST_EN_OFFSET, 0);
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
		BIT(SPA2_LTM_CLK_RST_EN_OFFSET) | BIT(SPA2_BCN_CLK_RST_EN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, SPA2BYPASS_CFG,
		max_val_on_bit(SPA2_BYPASS_EN_LEN), max_val_on_bit(SPA2_BYPASS_EN_LEN));
	snd_soc_component_update_bits_with_lock(codec, SPA2_PRE_CFG0,
		BIT(SPA2_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA2_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA2_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA2_CFG_PRE_PILOTTONE_CLKEN_OFFSET),
		BIT(SPA2_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA2_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA2_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA2_CFG_PRE_PILOTTONE_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
		(max_val_on_bit(SPA2_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA2_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA2_LTM_CLK_RST_EN_OFFSET) | BIT(SPA2_BCN_CLK_RST_EN_OFFSET),
		(max_val_on_bit(SPA2_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA2_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA2_LTM_CLK_RST_EN_OFFSET) | BIT(SPA2_BCN_CLK_RST_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
		BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
}

static int spa2_bypass_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("spa2 bypass power event enter %d", event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
        spa2_bypass_power_on_event(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
        snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static void spa3_bypass_power_on_event(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
		BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
		max_val_on_bit(SPA3_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA3_SMARTPA_PROTECT_CLK_RST_EN_OFFSET, 0);
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
		BIT(SPA3_LTM_CLK_RST_EN_OFFSET) | BIT(SPA3_BCN_CLK_RST_EN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, SPA3BYPASS_CFG,
		max_val_on_bit(SPA3_BYPASS_EN_LEN), max_val_on_bit(SPA3_BYPASS_EN_LEN));
	snd_soc_component_update_bits_with_lock(codec, SPA3_PRE_CFG0,
		BIT(SPA3_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA3_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA3_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA3_CFG_PRE_PILOTTONE_CLKEN_OFFSET),
		BIT(SPA3_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA3_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA3_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA3_CFG_PRE_PILOTTONE_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
		(max_val_on_bit(SPA3_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA3_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA3_LTM_CLK_RST_EN_OFFSET) | BIT(SPA3_BCN_CLK_RST_EN_OFFSET),
		(max_val_on_bit(SPA3_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA3_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA3_LTM_CLK_RST_EN_OFFSET) | BIT(SPA3_BCN_CLK_RST_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
		BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
}

static int spa3_bypass_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("spa3 bypass power event enter %d", event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
        spa3_bypass_power_on_event(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
        snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		    BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static void spa4_bypass_power_on_event(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
		BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
		max_val_on_bit(SPA4_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA4_SMARTPA_PROTECT_CLK_RST_EN_OFFSET, 0);
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
		BIT(SPA4_LTM_CLK_RST_EN_OFFSET) | BIT(SPA4_BCN_CLK_RST_EN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, SPA4BYPASS_CFG,
		max_val_on_bit(SPA4_BYPASS_EN_LEN), max_val_on_bit(SPA4_BYPASS_EN_LEN));
	snd_soc_component_update_bits_with_lock(codec, SPA4_PRE_CFG0,
		BIT(SPA4_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA4_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA4_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA4_CFG_PRE_PILOTTONE_CLKEN_OFFSET),
		BIT(SPA4_CFG_PRE_PGA_BYPASS_OFFSET) | BIT(SPA4_CFG_PRE_HPS_BYPASS_OFFSET) |
		BIT(SPA4_CFG_PRE_LPS_BYPASS_OFFSET) | BIT(SPA4_CFG_PRE_PILOTTONE_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
		(max_val_on_bit(SPA4_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA4_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA4_LTM_CLK_RST_EN_OFFSET) | BIT(SPA4_BCN_CLK_RST_EN_OFFSET),
		(max_val_on_bit(SPA4_CLK_RST_CFG_CLK_RST_EN_LEN) << SPA4_SMARTPA_PROTECT_CLK_RST_EN_OFFSET) |
		BIT(SPA4_LTM_CLK_RST_EN_OFFSET) | BIT(SPA4_BCN_CLK_RST_EN_OFFSET));

	snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
		BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
}

static int spa4_bypass_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("spa4 bypass power event enter %d", event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
        spa4_bypass_power_on_event(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
        snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		    BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int spa3_close_passthroughen_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("power event enter %d", event);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
			BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int spa4_close_passthroughen_power_event(struct snd_soc_component *codec,
	struct snd_kcontrol *kcontrol, int event)
{
	AUDIO_LOGI("power event enter %d", event);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
			BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int spa_2pa_bypass_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	spa1_bypass_power_event(codec, kcontrol, event);
	spa2_bypass_power_event(codec, kcontrol, event);
	spa3_close_passthroughen_power_event(codec, kcontrol, event);
	spa4_close_passthroughen_power_event(codec, kcontrol, event);

	return 0;
}

static int spa_4pa_bypass_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	spa1_bypass_power_event(codec, kcontrol, event);
	spa2_bypass_power_event(codec, kcontrol, event);
	spa3_bypass_power_event(codec, kcontrol, event);
	spa4_bypass_power_event(codec, kcontrol, event);

	return 0;
}

static int iv_2pa_slot_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	AUDIO_LOGI("2pa iv slot power event enter %d", event);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
			BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
			BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, SPA1BYPASS_CFG,
			(max_val_on_bit(SPA1_SPA_V_DATA_SEL_OFFSET_LEN) << SPA1_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA1_SPA_I_DATA_SEL_OFFSET_LEN) << SPA1_SPA_I_DATA_SEL_OFFSET),
			0x10 << SPA1_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, SPA2BYPASS_CFG,
			(max_val_on_bit(SPA2_SPA_V_DATA_SEL_OFFSET_LEN) << SPA2_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA2_SPA_I_DATA_SEL_OFFSET_LEN) << SPA2_SPA_I_DATA_SEL_OFFSET),
			0x54 << SPA2_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
			BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
			BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int iv_4pa_slot_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	AUDIO_LOGI("4pa iv slot power event enter %d", event);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
			BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
			BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
			BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
			BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, SPA1BYPASS_CFG,
			(max_val_on_bit(SPA1_SPA_V_DATA_SEL_OFFSET_LEN) << SPA1_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA1_SPA_I_DATA_SEL_OFFSET_LEN) << SPA1_SPA_I_DATA_SEL_OFFSET),
			0x10 << SPA1_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, SPA2BYPASS_CFG,
			(max_val_on_bit(SPA2_SPA_V_DATA_SEL_OFFSET_LEN) << SPA2_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA2_SPA_I_DATA_SEL_OFFSET_LEN) << SPA2_SPA_I_DATA_SEL_OFFSET),
			0x32 << SPA2_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, SPA3BYPASS_CFG,
			(max_val_on_bit(SPA3_SPA_V_DATA_SEL_OFFSET_LEN) << SPA3_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA3_SPA_I_DATA_SEL_OFFSET_LEN) << SPA3_SPA_I_DATA_SEL_OFFSET),
			0x54 << SPA3_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, SPA4BYPASS_CFG,
			(max_val_on_bit(SPA4_SPA_V_DATA_SEL_OFFSET_LEN) << SPA4_SPA_V_DATA_SEL_OFFSET) |
			(max_val_on_bit(SPA4_SPA_I_DATA_SEL_OFFSET_LEN) << SPA4_SPA_I_DATA_SEL_OFFSET),
			0x76 << SPA4_SPA_V_DATA_SEL_OFFSET);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG1,
			BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG2,
			BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG3,
			BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CLK_RST_CFG4,
			BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA_PROTECT_3WAY_CLK_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int codec_3pa_dn_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	unused_parameter(kcontrol); 
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(CODEC_0_DN_AFIFO_CLKEN_OFFSET) | BIT(CODEC_1_DN_AFIFO_CLKEN_OFFSET) |
			BIT(CODEC_2_DN_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(CODEC_0_DN_AFIFO_CLKEN_OFFSET) | BIT(CODEC_1_DN_AFIFO_CLKEN_OFFSET) |
			BIT(CODEC_2_DN_AFIFO_CLKEN_OFFSET), BIT(CODEC_0_DN_AFIFO_CLKEN_OFFSET) |
			BIT(CODEC_1_DN_AFIFO_CLKEN_OFFSET) | BIT(CODEC_2_DN_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(CODEC_0_DN_AFIFO_CLKEN_OFFSET) | BIT(CODEC_1_DN_AFIFO_CLKEN_OFFSET) |
			BIT(CODEC_2_DN_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);
 
	return 0;
}

static int audio_intf1_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, AXI2TDM_INTF1_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF1_CORE_CLK_EN_OFFSET) | BIT(AXI2TDM_INTF1_CLK_EN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits(codec, AXI2TDM_CLK_CTRL_REG,
			BIT(AXI2TDM_CODEC_DP1_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP1_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET),
			BIT(AXI2TDM_CODEC_DP1_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP1_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET));
		/* dma select */
		snd_soc_component_update_bits(codec, R_DMA0_REQ_SEL,
			BIT(AXI2TDM_IF1_REQ_UP_SEL_OFFSET) | BIT(AXI2TDM_IF1_REQ_DN_SEL_OFFSET),
			BIT(AXI2TDM_IF1_REQ_UP_SEL_OFFSET) | BIT(AXI2TDM_IF1_REQ_DN_SEL_OFFSET));
		snd_soc_component_update_bits(codec, R_CODEC_DMA_SEL,
			BIT(CODEC_DMA_SEL), BIT(CODEC_DMA_SEL));
		snd_soc_component_update_bits(codec, AXI2TDM_INTF1_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF1_CORE_CLK_EN_OFFSET) | BIT(AXI2TDM_INTF1_CLK_EN_OFFSET),
			BIT(AXI2TDM_INTF1_CORE_CLK_EN_OFFSET) | BIT(AXI2TDM_INTF1_CLK_EN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, AXI2TDM_INTF1_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF1_CORE_CLK_EN_OFFSET) | BIT(AXI2TDM_INTF1_CLK_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, R_DMA0_REQ_SEL,
			BIT(AXI2TDM_IF1_REQ_UP_SEL_OFFSET) | BIT(AXI2TDM_IF1_REQ_DN_SEL_OFFSET), 0);
		snd_soc_component_update_bits(codec, R_CODEC_DMA_SEL, BIT(CODEC_DMA_SEL), 0);
		snd_soc_component_update_bits(codec, AXI2TDM_CLK_CTRL_REG,
			BIT(AXI2TDM_CODEC_DP1_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP1_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	OUT_FUNCTION;
	return 0;
}

static int audio_intf2_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL_REG,
			BIT(AXI2TDM_INTF2_CORE_CLK_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF2_CLK_EN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits(codec, R_DMA0_REQ_SEL,
			BIT(AXI2TDM_IF2_REQ_UP_SEL_OFFSET), BIT(AXI2TDM_IF2_REQ_UP_SEL_OFFSET));
		snd_soc_component_update_bits(codec, AXI2TDM_CLK_CTRL_REG2,
			BIT(AXI2TDM_CODEC_DP2_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP2_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET),
			BIT(AXI2TDM_CODEC_DP2_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP2_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET));
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL_REG,
			BIT(AXI2TDM_INTF2_CORE_CLK_EN_OFFSET), BIT(AXI2TDM_INTF2_CORE_CLK_EN_OFFSET));
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF2_CLK_EN_OFFSET), BIT(AXI2TDM_INTF2_CLK_EN_OFFSET));
		audio_pinctrl_set_pinctrl_mode(AUDIO_PINCTRL_I2S3, AUDIO_PINCTRL_DEFAULT);
		break;
	case SND_SOC_DAPM_POST_PMD:
		audio_pinctrl_set_pinctrl_mode(AUDIO_PINCTRL_I2S3, AUDIO_PINCTRL_DEFAULT);
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL0_REG,
			BIT(AXI2TDM_INTF2_CLK_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, AXI2TDM_INTF2_CLK_CTRL_REG,
			BIT(AXI2TDM_INTF2_CORE_CLK_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, AXI2TDM_CLK_CTRL_REG2,
			BIT(AXI2TDM_CODEC_DP2_UP_CLK_EN_OFFSET) | BIT(AXI2TDM_CODEC_DP2_DN_CLK_EN_OFFSET) |
			BIT(AXI2TDM_CODEC_DP_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	OUT_FUNCTION;
	return 0;
}

static int mad_lp_single_mic_switch_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	IN_FUNCTION;
	unused_parameter(w);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		soc_mad_request_pinctrl_state(LOW_FREQ_MODE);
		soc_single_path_mad_with_dmic_select_din(LOW_FREQ_MODE);

		soc_single_path_mad_with_dmic_hpf_config(LOW_FREQ_MODE, HPF_ENABLE);
		AUDIO_LOGI("mad lp switch PMU: %d", event);
		break;
	case SND_SOC_DAPM_POST_PMD:
		soc_single_path_mad_with_dmic_select_din(HIGH_FREQ_MODE);
		soc_mad_release_pinctrl_state(LOW_FREQ_MODE);
		soc_single_path_mad_with_dmic_hpf_config(LOW_FREQ_MODE, HPF_BYPASS);

		AUDIO_LOGI("mad lp switch PMD: %d", event);
		break;
	default:
		AUDIO_LOGE("mad power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;
	return 0;
}

static int mad_norm_single_mic_switch_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	IN_FUNCTION;
	unused_parameter(w);
	unused_parameter(kcontrol);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		/* data src select codecless */
		soc_single_path_mad_with_dmic_select_din(HIGH_FREQ_MODE);
		soc_single_path_mad_with_dmic_hpf_config(HIGH_FREQ_MODE, HPF_ENABLE);

		AUDIO_LOGI("mad norm switch PMU: %d", event);
		break;
	case SND_SOC_DAPM_POST_PMD:
		/* data src select DMIC */
		soc_single_path_mad_with_dmic_select_din(LOW_FREQ_MODE);
		soc_single_path_mad_with_dmic_hpf_config(HIGH_FREQ_MODE, HPF_BYPASS);

		AUDIO_LOGI("mad norm switch PMD: %d", event);
		break;
	default:
		AUDIO_LOGE("mad power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

static const struct snd_kcontrol_new dapm_spk_2pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, SPK_2PA_BYPASS_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_rcv_2pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, RCV_2PA_BYPASS_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_spk_4pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, SPK_4PA_BYPASS_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_rcv_4pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, RCV_4PA_BYPASS_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_iv_2pa_slot_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, IV_2PA_SLOT_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_iv_4pa_slot_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, IV_4PA_SLOT_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_codec_3pa_dn_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, CODEC_DN_3PA_IN_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_audio_intf1_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, AUDIO_AXI2TDM_INTF1_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_audio_intf2_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, AUDIO_AXI2TDM_INTF2_BIT, 1, 0);
static const struct snd_kcontrol_new mad_lp_single_mic_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, MAD_LP_SINGLE_MIC_BIT, 1, 0);
static const struct snd_kcontrol_new mad_norm_single_mic_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, MAD_NORM_SINGLE_MIC_BIT, 1, 0);

#define AXI2TDM_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("AUDIO_INTF1_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_audio_intf1_switch_controls, audio_intf1_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("AUDIO_INTF2_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_audio_intf2_switch_controls, audio_intf2_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define SPK_SPA_BYPASS_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("SPK_2PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_spk_2pa_bypass_switch_controls, spa_2pa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("SPK_4PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_spk_4pa_bypass_switch_controls, spa_4pa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define RCV_SPA_BYPASS_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("RCV_2PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_rcv_2pa_bypass_switch_controls, spa_2pa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("RCV_4PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_rcv_4pa_bypass_switch_controls, spa_4pa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define IV_2PA_SLOT_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("IV_2PA_SLOT_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_iv_2pa_slot_switch_controls, iv_2pa_slot_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define IV_4PA_SLOT_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("IV_4PA_SLOT_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_iv_4pa_slot_switch_controls, iv_4pa_slot_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define CODEC_3PA_DN_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("CODEC_3PA_DN_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_codec_3pa_dn_switch_controls, codec_3pa_dn_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define MAD_DMIC_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("MAD_LP_SINGLE_MIC_SWITCH", SND_SOC_NOPM, 0, 0, \
		&mad_lp_single_mic_switch_controls, mad_lp_single_mic_switch_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("MAD_NORM_SINGLE_MIC_SWITCH", SND_SOC_NOPM, 0, 0, \
		&mad_norm_single_mic_switch_controls, mad_norm_single_mic_switch_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static const struct snd_soc_dapm_widget g_widgets[] = {
	PLATFORM_WIDGET,
	AXI2TDM_SWITCH_WIDGET,
	AU_WIDGET,
	AU_SUPPLY_WIDGET,
	SPK_SPA_BYPASS_SWITCH_WIDGET,
	RCV_SPA_BYPASS_SWITCH_WIDGET,
	IV_2PA_SLOT_SWITCH_WIDGET,
	IV_4PA_SLOT_SWITCH_WIDGET,
	CODEC_3PA_DN_SWITCH_WIDGET,
	MAD_DMIC_SWITCH_WIDGET,
};

static const struct snd_soc_dapm_route g_route_map[] = {
	AUDIO_VOICE_2MIC_ROUTE,
	AUDIO_VOICE_4MIC_ROUTE,
	AUDIO_VOICE_6MIC_ROUTE,
	AUDIO_VOICE_LOOP_ROUTE,
	UPLINK_PLL,
	DMIC12_ROUTE,
	DMIC34_ROUTE,
	DMIC56_ROUTE,
	MIC_CIC_PLL,
	MIC34_MIC56_UP_ROUTE,
	AUDIO_PLAY_2PA,
	AUDIO_PLAY_4PA,
	AUDIO_CARTKIT,
	BT_DOWNLINK,
	A2DP_OFFLOAD_ROUTE,
	BT_UPLINK,
	ULTRA_ROUTE,
	MMI_ROUTE,
	AXI_2_TDM_ROUTE,
	ANA_AMIC_ROUTE,
	AUDIO_PLAY_EAR,
	SPA_2PA_BYPASS_ROUTE,
	SPA_4PA_BYPASS_ROUTE,
	AUDIO_UP_2PA_EC,
	AUDIO_UP_4PA_EC,
	MAD_LP_SINGLE_MIC_PATH,
	MAD_NORM_SINGLE_MIC_PATH,
};

static const struct snd_codec_resource g_codec_source = {
	NULL,
	g_kcontrols,
	ARRAY_SIZE(g_kcontrols),
	g_widgets,
	ARRAY_SIZE(g_widgets)
};

static const struct snd_codec_route g_codec_route = {
	g_route_map,
	ARRAY_SIZE(g_route_map)
};

static_t int __init asp_codec_path_init(void)
{
	add_codec_platform_dapm_list(&g_codec_source);
	add_codec_add_route(&g_codec_route);
	return 0;
}

device_initcall(asp_codec_path_init);
