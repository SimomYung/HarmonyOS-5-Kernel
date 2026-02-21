/*
 * ams_tcs3410.h
 *
 * code for AMS tcs3410 sensor
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Description: code for AMS tcs3410 sensor
 * Author: caozhipeng
 * Create: 2024-09-18
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

#ifndef TCS3410_HW_H
#define TCS3410_HW_H

#include "color_sensor.h"

#define ENABLE_DUMP_REGS
#define DEBUG_FIFO_FD_DATA
#define TCS3410_I2C_ADDR (0x39)

//init parameter
#define GAIN_TBL_SIZE               14
#define TCS3410_CHIP_VALUE          0x5c
#define TCS3410_SAMPLE_TIME         719
#define ALS_NR_SAMPLES              99
#define TCS3410_WAIT_TIME           0
#define TCS3410_AGC_MODE            0
#define TCS3410_REPORT_DATA_LEN     4
#define AGC_NUM_SAMPLES             19
#define FD_NR_SAMPLES               100
#define TCS3410_PERSIST             0
#define PD_GAIN_DEFAULT             8
#define FIFO_THRESHOLD              16
#define TCS3410_DEVICE_ON           0x01
#define TCS3410_ENABLE_ON           0x01
#define TCS3410_ENABLE_OFF          0x00
#define ALS_NUM_CH                  3
#define ALS_CH_SIZE                 (sizeof(uint8_t) * 2)
#define MOD_CLOCK                   1389
#define ALS_ACCUMULATE_NO           3
#define SCALER                      1000
#define TCS3410_MASK_AGAIN          0x03
#define INDOOR_LUX_TRIGGER	        6000
#define OUTDOOR_LUX_TRIGGER	        10000
#define TCS3410_MAX_LUX		        0xffff
#define TCS3410_MAX_ALS_VALUE	    0xffff
#define TCS3410_ALS_LOW_THRESH      0x000000
#define TCS3410_ALS_HIGH_THRESH     0xFFFFFF
#define TCS3410_MIN_ALS_VALUE	    3
#define MAX_I2C_READ_LEN            128
#define MAX_DEVICE_FIFO_LEN         512
#define GC_AGAIN                    256
#define GAIN_RATIO                  100
#define MAX_ALS_GAIN_LEVEL          12
#define MIN_ALS_GAIN_LEVEL          1
#define MAX_FIFO_DATA_LEN           2048
#define FFT_MAX_SAMPLE_SIZE         1024
#define FFT_SAMPLE_FREQ             2000
#define FFT_BIN_COUNT               256
#define MAX_BUFFER_SIZE             256
#define FIRST_FLK_TIMER             6
#define FD_POLLING_TIME             22
#define BUF_RATIO_8_16BIT           2
#define ASTEP_LEN                   2
#define REPORT_FIFO_LEN             128
#define TCS3410_CHAN_NUM            3
#define FLCKR_BUFFER_SZ             1024
#define FD_PACKET_COMPRESSED_SZ     7 // MOD_FD_FIFO_DATA7_WIDTH + 1(base 0) + 1(HW bit flag)
#define FD_FOOTER_SZ_BYTES          5 // size of the footer -  3 end marker bytes, 2 bytes for gain
#define FLICKER_BYTES_PER_SAMPLE    2
#define MAX_FD_NR_SAMPLES           (0x07ff + 1)
#define TCS3410_DEFAULT_MODULATOR_PERIOD_TO_MS 1000
#define READ_TWO_BITS               2
#define READ_NINE_BITS              9
#define CHIPID_GET_TIME             3
#define SEQUENCER_INIT_VALUE        3
#define SAMPLE_TIME_LOW_MASK        0x00FF
#define SAMPLE_TIME_HIGH_MASK       0x0700
#define NUM_SAMPLES_LOW_MASK        0x00FF
#define NUM_SAMPLES_HIGH_MASK       0x0700
#define SHIFT_8                     8
#define SHIFT_4                     4
#define ALS_THRESH_MASK_0           0x000000FF
#define ALS_THRESH_MASK_1           0x0000FF00
#define ALS_THRESH_MASK_2           0x00FF0000
#define MOD_PHD_MASK                0x0F
#define HIGH_TIMER_OFFSET           5
#define TCS3410_RBITS_GAIN_1		128
#define TCS3410_RBITS_GAIN_2		256
#define TCS3410_RBITS_GAIN_3		512
#define TCS3410_RBITS_GAIN_4		1024
#define TCS3410_RBITS_VALUE_4		1
#define TCS3410_RBITS_VALUE_3		2
#define TCS3410_RBITS_VALUE_2		3
#define TCS3410_RBITS_VALUE_1		4
#define TCS3410_MEAS_STEP_0			0
#define TCS3410_MEAS_STEP_1			1
// TCS3410 FIFO settings
#define ALS_DATA_LENGTH             0
#define END_MARKER_SZ               3
#define AMSDRIVER_FD_ENABLE         1
#define AMSDRIVER_FD_DISABLE        0
#define AMSDRIVER_ALS_ENABLE        1
#define AMSDRIVER_ALS_DISABLE       0
#define GAIN_LEVEL_1                1
#define GAIN_LEVEL_2                4
#define GAIN_LEVEL_3                16
#define GAIN_LEVEL_4                64
#define GAIN_LEVEL_5                256
#define GAIN_LEVEL_6                512
#define CAL_RATIO_DEFAULT			10000
#define AMS_FLOAT_TO_FIX			10000
#define AMS_REPORT_LOG_COUNT_NUM	10
#define MAX_REPORT_LOGCOUNT         10000
// Register map
#define TCS3410_ENABLE                          0x80
#define TCS3410_MEAS_MODE0                      0x81
#define TCS3410_MEAS_MODE1                      0x82
#define TCS3410_SAMPLE_TIME0                    0x83
#define TCS3410_SAMPLE_TIME1                    0x84
#define TCS3410_ALS_NR_SAMPLES0                 0x85
#define TCS3410_ALS_NR_SAMPLES1                 0x86
#define TCS3410_FD_NR_SAMPLES0                  0x87
#define TCS3410_FD_NR_SAMPLES1                  0x88
#define TCS3410_WTIME                           0x89
#define TCS3410_AILT0                           0x8A
#define TCS3410_AILT1                           0x8B
#define TCS3410_AILT2                           0x8C
#define TCS3410_AIHT0                           0x8D
#define TCS3410_AIHT1                           0x8E
#define TCS3410_AIHT2                           0x8F
#define TCS3410_AUXID                           0x90
#define TCS3410_REVID                           0x91
#define TCS3410_ID                              0x92
#define TCS3410_STATUS                          0x93
#define TCS3410_ALS_STATUS                      0x94
#define TCS3410_DATAL0                          0x95
#define TCS3410_DATAH0                          0x96
#define TCS3410_DATAL1                          0x97
#define TCS3410_DATAH1                          0x98
#define TCS3410_DATAL2                          0x99
#define TCS3410_DATAH2                          0x9A
#define TCS3410_ALS_STATUS2                     0x9B
#define TCS3410_ALS_STATUS3                     0x9C
#define TCS3410_STATUS2                         0x9D
#define TCS3410_STATUS3                         0x9E
#define TCS3410_STATUS4                         0x9F
#define TCS3410_STATUS5                         0xA0
#define TCS3410_CFG0                            0xA1
#define TCS3410_CFG1                            0xA2
#define TCS3410_CFG2                            0xA3
#define TCS3410_CFG3                            0xA4
#define TCS3410_CFG4                            0xA5
#define TCS3410_CFG5                            0xA6
#define TCS3410_CFG6                            0xA7
#define TCS3410_CFG7                            0xA8
#define TCS3410_CFG8                            0xA9
#define TCS3410_CFG9                            0xAA
#define TCS3410_AGC_NR_SAMPLES_LO               0xAC
#define TCS3410_AGC_NR_SAMPLES_HI               0xAD
#define TCS3410_TRIGGER_MODE                    0xAE
#define TCS3410_CONTROL                         0xB1
#define TCS3410_INTENAB                         0xBA
#define TCS3410_SIEN                            0xBB
#define TCS3410_MOD_COMP_CFG1                   0xCE
#define TCS3410_MEAS_SEQR_FD_0                  0xCF
#define TCS3410_MEAS_SEQR_ALS_FD_1              0xD0
#define TCS3410_MEAS_SEQR_APERS_AND_VSYNC_WAIT  0xD1
#define TCS3410_MEAS_SEQR_RESIDUAL_0            0xD2
#define TCS3410_MEAS_SEQR_RESIDUAL_1_AND_WAIT   0xD3
#define TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_L     0xD4
#define TCS3410_MEAS_SEQR_STEP0_MOD_GAINX_H     0xD5
#define TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_L     0xD6
#define TCS3410_MEAS_SEQR_STEP1_MOD_GAINX_H     0xD7
#define TCS3410_MEAS_SEQR_STEP2_MOD_GAINX_L     0xD8
#define TCS3410_MEAS_SEQR_STEP3_MOD_GAINX_L     0xDA
#define TCS3410_MEAS_SEQR_STEP0_MOD_PHDX_SMUX_L 0xDC
#define TCS3410_MEAS_SEQR_STEP0_MOD_PHDX_SMUX_H 0xDD
#define TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_L 0xDE
#define TCS3410_MEAS_SEQR_STEP1_MOD_PHDX_SMUX_H 0xDF
#define TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_L 0xE0
#define TCS3410_MEAS_SEQR_STEP2_MOD_PHDX_SMUX_H 0xE1
#define TCS3410_MEAS_SEQR_STEP3_MOD_PHDX_SMUX_L 0xE2
#define TCS3410_MEAS_SEQR_STEP3_MOD_PHDX_SMUX_H 0xE3
#define TCS3410_MOD_CALIB_CFG0                  0xE4
#define TCS3410_MOD_CALIB_CFG2                  0xE6
#define TCS3410_VSYNC_PERIOD_L                  0xF2
#define TCS3410_VSYNC_PERIOD_H                  0xF3
#define TCS3410_VSYNC_PERIOD_TARGET_L           0xF4
#define TCS3410_VSYNC_PERIOD_TARGET_H           0xF5
#define TCS3410_VSYNC_CONTROL                   0xF6
#define TCS3410_VSYNC_CFG                       0xF7
#define TCS3410_VSYNC_GPIO_INT                  0xF8
#define TCS3410_MOD_FIFO_DATA_CFG0              0xF9
#define TCS3410_MOD_FIFO_DATA_CFG1              0xFA
#define TCS3410_MOD_FIFO_DATA_CFG2              0xFB
#define TCS3410_FIFO_THR                        0xFC
#define TCS3410_FIFO_LEVEL                      0xFD
#define TCS3410_FIFO_STATUS0                    0xFE
#define TCS3410_FIFO_DATA                       0xFF
/* Mask Macros */
#define TCS3410_PON_SHIFT 0
#define TCS3410_MASK_PON (1 << TCS3410_PON_SHIFT)

#define TCS3410_AEN_SHIFT 1
#define TCS3410_MASK_AEN (1 << TCS3410_AEN_SHIFT)

#define TCS3410_FDEN_SHIFT 6
#define TCS3410_MASK_FDEN (1 << TCS3410_FDEN_SHIFT)

#define TCS3410_MINT_SHIFT 7
#define TCS3410_MASK_MINT (1 << TCS3410_MINT_SHIFT)

#define TCS3410_AINT_SHIFT 3
#define TCS3410_MASK_AINT (1 << TCS3410_AINT_SHIFT)

#define TCS3410_FINT_SHIFT 2
#define TCS3410_MASK_FINT (1 << TCS3410_FINT_SHIFT)

#define TCS3410_SINT_SHIFT 0
#define TCS3410_MASK_SINT (1 << TCS3410_SINT_SHIFT)

#define TCS3410_MIEN_SHIFT 7
#define TCS3410_MASK_MIEN (1 << TCS3410_MIEN_SHIFT)

#define TCS3410_AIEN_SHIFT 3
#define TCS3410_MASK_AIEN (1 << TCS3410_AIEN_SHIFT)

#define TCS3410_FIEN_SHIFT 2
#define TCS3410_MASK_FIEN (1 << TCS3410_FIEN_SHIFT)

#define TCS3410_SIEN_SHIFT 0
#define TCS3410_MASK_SIEN (1 << TCS3410_SIEN_SHIFT)

#define TCS3410_APERS_SHIFT 0
#define TCS3410_MASK_APERS (0xF << TCS3410_APERS_SHIFT)

#define TCS3410_ASAT_SHIFT 7
#define TCS3410_MASK_ASAT (1 << TCS3410_ASAT_SHIFT)

// Register bits map
// ENABLE @ 0x80
#define PON                                      (0x01 << 0)
#define AEN                                      (0x01 << 1)
#define PEN                                      (0x01 << 2)
#define WEN                                      (0x01 << 3)
#define SMUXEN                                   (0x01 << 4)
#define FDEN                                     (0x01 << 6)

#define TCS3410_MEAS_MODE0_SHIFT_STOP_AFTER_NTH_ITER                (7)
#define TCS3410_MEAS_MODE0_SHIFT_DOUBLE_STEP_DOWN   (6)
#define TCS3410_MEAS_MODE0_MASK_ENABLE_AGC_ASAT_DOUBLE_STEP_DOWN (1 << TCS3410_MEAS_MODE0_SHIFT_DOUBLE_STEP_DOWN)
#define TCS3410_MEAS_MODE0_SHIFT_MEAS_SEQR_SINGLE_SHOT_MODE         (5)
#define TCS3410_MEAS_MODE0_SHIFT_FIFO_ALS_STATUS_WRITE_ENABLE       (4)
#define TCS3410_MEAS_MODE0_SHIFT_ALS_SCALE                          (0)
#define TCS3410_MEAS_MODE0_MASK_ALS_SCALE   (0x0f << TCS3410_MEAS_MODE0_SHIFT_ALS_SCALE)


#define TCS3410_SHIFT_FD_END_MARKER_WRITE_EN    (7)
#define TCS3410_FD_END_MARKER_WRITE_EN (1 << TCS3410_SHIFT_FD_END_MARKER_WRITE_EN)
#define TCS3410_SHIFT_FD_CHKSUM_MARKER_WRITE_EN (6)
#define TCS3410_FD_CHKSUM_MARKER_WRITE_EN (1 << TCS3410_SHIFT_FD_CHKSUM_MARKER_WRITE_EN)
#define TCS3410_SHIFT_FD_GAIN_WRITE_EN          (5)
#define TCS3410_FD_GAIN_WRITE_EN (1 << TCS3410_SHIFT_FD_GAIN_WRITE_EN)
#define TCS3410_MEAS_MODE1_SHIFT_ALS_MSB_POSITION                   (0)
#define TCS3410_MEAS_MODE1_MASK_ALS_MSB_POSITION                    (0x1F << TCS3410_MEAS_MODE1_SHIFT_ALS_MSB_POSITION)
#define DEFAULT_ALS_MSB_POSITION                                    (0x0C)

#define TCS3410_MOD_CALIB_NTH_ITERATION_AGC_ENABLE_SHIFT 5
#define TCS3410_MASK_MOD_CALIB_NTH_ITERATION_AGC_ENABLE (1 << TCS3410_MOD_CALIB_NTH_ITERATION_AGC_ENABLE_SHIFT)

#define TCS3410_MEASUREMENT_SEQ_AGC_ASAT_PATTERN_SHIFT 4
#define TCS3410_MASK_MEASUREMENT_SEQ_AGC_ASAT_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQ_AGC_ASAT_PATTERN_SHIFT)

#define TCS3410_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN_SHIFT 4
#define TCS3410_MASK_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQ_AGC_PREDICT_PATTERN_SHIFT)

#define TCS3410_FIFO_OV_SHIFT 7
#define TCS3410_MASK_FIFO_OV (1 << TCS3410_FIFO_OV_SHIFT)

#define TCS3410_FIFO_UF_SHIFT 6
#define TCS3410_MASK_FIFO_UF (1 << TCS3410_FIFO_UF_SHIFT)

#define TCS3410_MOD_ALS_FIFO_DATAX_WRITE_ENABLE_SHIFT 7
#define TCS3410_MASK_MOD_ALS_FIFO_DATAX_WRITE_ENABLE (0x01 << TCS3410_MOD_ALS_FIFO_DATAX_WRITE_ENABLE_SHIFT)

#define TCS3410_FIFO_THR_LSB_SHIFT 0
#define TCS3410_MASK_FIFO_THR_LSB (0x0001 << TCS3410_FIFO_THR_LSB_SHIFT)

#define TCS3410_ALS_DATA0_SCALED_STATUS_SHIFT 2
#define TCS3410_MASK_ALS_DATA0_SCALED_STATUS (0x01 << TCS3410_ALS_DATA0_SCALED_STATUS_SHIFT)

#define TCS3410_ALS_DATA1_SCALED_STATUS_SHIFT 1
#define TCS3410_MASK_ALS_DATA1_SCALED_STATUS (0x01 << TCS3410_ALS_DATA1_SCALED_STATUS_SHIFT)

#define TCS3410_ALS_DATA2_SCALED_STATUS_SHIFT 0
#define TCS3410_MASK_ALS_DATA2_SCALED_STATUS (0x01 << TCS3410_ALS_DATA2_SCALED_STATUS_SHIFT)

#define TCS3410_FD_NR_SAMPLES_HI_SHIFT 0
#define TCS3410_MASK_FD_NR_SAMPLES_HI (0x07 << TCS3410_FD_NR_SAMPLES_HI_SHIFT)

#define TCS3410_FIFO_CLR_SHIFT 1
#define TCS3410_MASK_FIFO_CLR (0x01 << TCS3410_FIFO_CLR_SHIFT)

#define TCS3410_MOD_CALIB_NTH_ITERATION_SHIFT 0
#define TCS3410_MASK_MOD_CALIB_NTH_ITERATION (0xFF << TCS3410_MOD_CALIB_NTH_ITERATION_SHIFT)

#define TCS3410_SINT_MEASUREMENT_SEQUENCER_SHIFT 1
#define TCS3410_MASK_SINT_MEASUREMENT_SEQUENCER (0x01 << TCS3410_SINT_MEASUREMENT_SEQUENCER_SHIFT)

#define TCS3410_SAI_ACTIVE_SHIFT 1
#define TCS3410_MASK_SAI_ACTIVE (0x01 << TCS3410_SAI_ACTIVE_SHIFT)

#define TCS3410_CLEAR_SAI_ACTIVE_SHIFT 0
#define TCS3410_MASK_CLEAR_SAI_ACTIVE (0x01 << TCS3410_CLEAR_SAI_ACTIVE_SHIFT)

#define TCS3410_SAI_SHIFT 6
#define TCS3410_MASK_SAI (0x01 << TCS3410_SAI_SHIFT)

#define TCS3410_SIEN_MEASUREMENT_SEQUENCER_SHIFT 1
#define TCS3410_MASK_SIEN_MEASUREMENT_SEQUENCER (0x01 << TCS3410_SIEN_MEASUREMENT_SEQUENCER_SHIFT)

#define TCS3410_MEASUREMENT_SEQUENCER_SINGLE_SHOT_SHIFT 5
#define TCS3410_MASK_MEASUREMENT_SEQUENCER_SINGLE_SHOT (0x01 << TCS3410_MEASUREMENT_SEQUENCER_SINGLE_SHOT_SHIFT)

#define TCS3410_MEASUREMENT_SEQUENCER_MOD0_FD_PATTERN_SHIFT 0
#define TCS3410_MASK_MEASUREMENT_SEQUENCER_MOD0_FD_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQUENCER_MOD0_FD_PATTERN_SHIFT)

#define TCS3410_MEASUREMENT_SEQUENCER_MOD1_FD_PATTERN_SHIFT 4
#define TCS3410_MASK_MEASUREMENT_SEQUENCER_MOD1_FD_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQUENCER_MOD1_FD_PATTERN_SHIFT)

#define TCS3410_MEASUREMENT_SEQUENCER_MOD2_FD_PATTERN_SHIFT 4
#define TCS3410_MASK_MEASUREMENT_SEQUENCER_MOD2_FD_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQUENCER_MOD2_FD_PATTERN_SHIFT)

#define TCS3410_MEASUREMENT_SEQUENCER_ALS_PATTERN_SHIFT 0
#define TCS3410_MASK_MEASUREMENT_SEQUENCER_ALS_PATTERN (0x0F << TCS3410_MEASUREMENT_SEQUENCER_ALS_PATTERN_SHIFT)

// TCS3410_REG_FIFO_LEVEL    0xFD
#define TCS3410_SHIFT_FIFO_LEVEL                 (2)  // upper 2 bits of a 10 bit number
#define TCS3410_SHIFT_STATUS0_FIFO_LVL_L    (0)
#define TCS3410_MASK_STATUS0_FIFO_LVL_L     (3 << TCS3410_SHIFT_STATUS0_FIFO_LVL_L)

/* TCS3410_REG_CFG2    0xA3 */
#define TCS3410_SHIFT_CFG2_FIFO_THRESH_L          (0)
#define TCS3410_MASK_CFG2_FIFO_THRESH_L           (1 << TCS3410_SHIFT_CFG2_FIFO_THRESH_L)

// TCS3410_REG_FIFO_STATUS0     0xFE
#define TCS3410_SHIFT_FIFO_STATUS0_FIFO_OVERFLOW      (7)
#define TCS3410_MASK_FIFO_STATUS0_FIFO_OVERFLOW       (1 << TCS3410_SHIFT_FIFO_STATUS0_FIFO_OVERFLOW)

#define TCS3410_SHIFT_FIFO_STATUS0_FIFO_UNDERFLOW     (6)
#define TCS3410_MASK_FIFO_STATUS0_FIFO_UNDERFLOW      (1 << TCS3410_SHIFT_FIFO_STATUS0_FIFO_UNDERFLOW)

#define TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_SHIFT 7
#define TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_MASK (1 << TCS3410_REG_MOD_FIFO_DATA_WRITE_ENABLE_SHIFT)

#define TCS3410_SHIFT_ALS_STATUS_MEAS_SEQR_STEP      (6)
#define TCS3410_MASK_ALS_STATUS_MEAS_SEQR_STEP       (3 << TCS3410_SHIFT_ALS_STATUS_MEAS_SEQR_STEP)

#define TCS3410_SHIFT_GAIN_MOD0         (0)
#define TCS3410_SHIFT_GAIN_MOD1         (4)
#define TCS3410_SHIFT_GAIN_MOD2         (0)

// TCS3410_REG_STATUS     0x9D      STATUS2
#define STATUS2_ALS_DATA_VALID_SHIFT             6
#define STATUS2_ALS_DATA_VALID_MASK              (1 << STATUS2_ALS_DATA_VALID_SHIFT)

#define TCS3410_SHIFT_CFG4_MEAS_SEQR_SINT_PER_STEP       (4)
#define TCS3410_MASK_CFG4_MEAS_SEQR_SINT_PER_STEP        (1 << TCS3410_SHIFT_CFG4_MEAS_SEQR_SINT_PER_STEP)

#define TCS3410_SHIFT_CFG4_MOD_ALS_FIFO_DATA_FORMAT      (0)
#define TCS3410_MASK_CFG4_MOD_ALS_FIFO_DATA_FORMAT       (3 << TCS3410_SHIFT_CFG4_MOD_ALS_FIFO_DATA_FORMAT)

// TCS3410_REG_STATUS4    0x9F
#define TCS3410_REG_STATUS4_MOD_SAMPLE_TRIGGER_ERROR_SHIFT                  3
#define TCS3410_MOD_SAMPLE_TRIGGER_ERROR     (1 << TCS3410_REG_STATUS4_MOD_SAMPLE_TRIGGER_ERROR_SHIFT)

// TCS3410_REG_SIEN     0xBB
#define TCS3410_SHIFT_SIEN_SIEN_MEASR_SEQR               (1)
#define TCS3410_MASK_SIEN_SIEN_MEASR_SEQR                (1 << TCS3410_SHIFT_SIEN_SIEN_MEASR_SEQR)

// tcs3410_als_chan_asat_mask
#define TCS3410_ALS_STATUS_ALS_DATA2_SATURATION_SHIFT            3
#define TCS3410_ALS_STATUS_ALS_DATA2_SATURATION_MASK             (1 << TCS3410_ALS_STATUS_ALS_DATA2_SATURATION_SHIFT)

#define TCS3410_ALS_STATUS_ALS_DATA1_SATURATION_SHIFT            4
#define TCS3410_ALS_STATUS_ALS_DATA1_SATURATION_MASK             (1 << TCS3410_ALS_STATUS_ALS_DATA1_SATURATION_SHIFT)

#define TCS3410_ALS_STATUS_ALS_DATA0_SATURATION_SHIFT            5
#define TCS3410_ALS_STATUS_ALS_DATA0_SATURATION_MASK             (1 << TCS3410_ALS_STATUS_ALS_DATA0_SATURATION_SHIFT)

// TCS3410_REG_MOD_COMP_CFG1     0xCE
#define TCS3410_SHIFT_MOD_COMP_CFG1_IDAC_RANGE           (6)
#define TCS3410_MASK_MOD_COMP_CFG1_IDAC_RANGE            (3 << TCS3410_SHIFT_MOD_COMP_CFG1_IDAC_RANGE)

//supported fifo formats
typedef enum {
	FIFO_FORMAT_UNCOMPRESSED = 1,
	FIFO_FORMAT_DIFFERENCE,
	FIFO_FORMAT_COMPRESSED,
	FIFO_FORMAT_DIFFERENCE_COMPRESSED,
	FIFO_FORMAT_MULTI_CHL_COMPRESSED,
	FIFO_FORMAT_MULTI_CHL_DIFFERENCE_COMPRESSED
} tcs3410_fifo_format_t;

typedef enum{
	AGC_HW_DISABLE_ALL = 0,
	AGC_Sat_ONLY,
	AGC_Pre_ONLY,
	AGC_HW_ENABLE_ALL,
} tcs3410_agc_hw_mode;

typedef enum{
	AGC_SW_DISABLE = 0,
	AGC_SW_ENABLE,
} tcs3410_AGC_SW_mode;

typedef enum {
	PD_CLEAR,
	PD_RED,
	PD_GREEN,
	PD_BLUE,
	PD_END,
} tcs3410_pd_t;

typedef enum {
	MODULATOR_0,
	MODULATOR_1,
	MODULATOR_2,
	NUM_MODULATORS,
} tcs3410_modulator_t;

typedef enum {
	STAT,
	ALS_STAT,
	ALS_STAT2,
	ALS_STAT3,
	STAT2,
	STAT3,
	STAT4,
	STAT5,
	STAT_FIFO,
	STAT_END
} tcs3410_stat_type_t;

typedef enum {
	TCS3410_HALF_X_GAIN            = 0x00,
	TCS3410_1_X_GAIN               = 0x01,
	TCS3410_2_X_GAIN               = 0x02,
	TCS3410_4_X_GAIN               = 0x03,
	TCS3410_8_X_GAIN               = 0x04,
	TCS3410_16_X_GAIN              = 0x05,
	TCS3410_32_X_GAIN              = 0x06,
	TCS3410_64_X_GAIN              = 0x07,
	TCS3410_128_X_GAIN             = 0x08,
	TCS3410_256_X_GAIN             = 0x09,
	TCS3410_512_X_GAIN             = 0x0A,
	TCS3410_1024_X_GAIN            = 0x0B,
	TCS3410_2048_X_GAIN            = 0x0C,
	TCS3410_4096_X_GAIN            = 0x0D,
} tcs3410_gains;

enum tcs3410_mod_idac_ranges {
	TCS3410_IDAC_58UV,
	TCS3410_IDAC_38UV,
	TCS3410_IDAC_18UV,
	TCS3410_IDAC_9UV,
};

enum tcs3410_power_mode_t {
	AMS_TCS3410_FEATURE_ALS = (1 << 0),
	AMS_TCS3410_FEATURE_LAST
};

enum tcs3410_sensor_id_t {
	UNKNOWN_DEVICE,
	TCS3410_REV0
};

struct tcs3410_als {
	int32_t raw_vis;
	int32_t raw_ir;
	int32_t raw_uv;
	uint8_t step;
	uint8_t analog_saturation[PD_END];
	uint8_t gains[PD_END];
	uint32_t agc_thr_high;
	uint32_t agc_thr_low;
	uint16_t cct;
	uint32_t raw_pds[NUM_MODULATORS];
	uint8_t pd_gain[NUM_MODULATORS];
	uint16_t saturation_high;
	uint16_t saturation_low;
};

struct tcs3410_cal_ref{
	uint32_t channel_target[ALS_NUM_CH];
	uint8_t gain[ALS_NUM_CH];
};

// data struct for processing
struct tcs3410_flicker
{
	uint8_t flicker_data_h;
	uint8_t flicker_data_l;
	uint8_t flicker_data_l_filled;
	uint16_t flicker_data_number;
	uint16_t flicker_data[MAX_FD_NR_SAMPLES];
};

struct tcs3410_als_ctx_t {
	UINT16 saturation;
	UINT8 gain_id;
	UINT32 gain;
	UINT32 itime_ms;
	UINT16 astep;
	UINT8 atime;
	UINT8 status;
	UINT8 status2;
	UINT8 als_update;
	bool load_calibration;
	UINT8 smux_mode;
	UINT8 enabled;
    UINT32 data[10];
};

#define ams_mutex_lock(m) { \
	mutex_lock(m); \
}
#define ams_mutex_unlock(m) { \
	mutex_unlock(m); \
}

struct tcs3410_calib_ctx_t {
	uint32_t target[TCS3410_REPORT_DATA_LEN];
	uint32_t ratio[TCS3410_REPORT_DATA_LEN];
	uint8_t reserved[16];
};

struct tcs3410_ctx {
	uint8_t i2c_addr;
	struct i2c_client *handle;
	struct mutex lock;
	uint16_t sample_time;
	uint8_t wait_time;
	uint16_t atime_ms;
	uint8_t agc_mode;
	tcs3410_agc_hw_mode hw_agc_mode;
	uint16_t agc_num_samples;
	uint8_t als_scale;
	uint16_t fifo_threshold;
	uint16_t fifo_level;
	uint16_t fd_nr_samples;
	uint16_t als_nr_samples;
	uint8_t persist;
	uint8_t gain_max;
	uint8_t gain[PD_END];
	uint8_t  gain_reg[NUM_MODULATORS];  /* actual register value - only 1 modulator for flicker */
	uint8_t  reg_status[STAT_END];
	uint32_t gain_mod[NUM_MODULATORS];  /* actual channel gain.. 2x, 4x, 8x, 16x... */
	uint32_t norm_pds[NUM_MODULATORS];
	uint32_t norm_data[PD_END];
	uint8_t enable;
	struct tcs3410_als als;
	struct tcs3410_flicker fd;
	struct tcs3410_als_ctx_t alg_ctx;
	struct tcs3410_calib_ctx_t cal_ctx
};

#endif /* TCS3410_H */