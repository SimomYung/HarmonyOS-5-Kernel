/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __DP_EDID_H__
#define __DP_EDID_H__

#include <linux/string.h>
#include "dp_ctrl.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmacro-redefined"

#define EDID_BLOCK_LINE_LENGTH 16
#define EDID_BLOCK_LENGTH 128
#define EDID_NUM 256
#define MAX_EDID_RETRY_COUNT 2
#define EDID_RETRY_DELAY_TIME 100
#define EDID_BASE_FPS 60

#ifndef max
#define max(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

#define IMAGE_RATIO(a, b) 	(((a) * 100) / (b))
#define VALUE_DIFF(a, b)	(abs((a) - (b)))

struct image_ration_t {
	uint8_t h_ratio;
	uint8_t v_ratio;
};

static const uint8_t edid_v1_header[8] = {0x00, 0xff, 0xff, 0xff,
								   0xff, 0xff, 0xff, 0x00};

#define upper_nibble(x)  ((0xF0 & (x)) >> 4)

#define lower_nibble(x)  (0xF & (x))

#define combine_hi_8lo(hi, lo)  ((((uint8_t)hi) << 8) | (uint8_t)lo)

#define combine_hi_4lo(hi, lo)  ((((uint8_t)hi) << 4) | (uint8_t)lo)

#define get_array_len(array)  (ARRAY_SIZE(array))

#define get_cea_data_block_tag(x) (((uint8_t)0xE0 & x[0]) >> 5)

#define get_cea_data_block_len(x) ((uint8_t)0x1F & x[0])

#define EDID_LENGTH				   0x80 //lint !e547

#define EDID_HEADER				   0x00
#define EDID_HEADER_END			   0x07

#define EDID_FACTORY_START			   0x08
#define EDID_FACTORY_END			0x11

#define EDID_COLOR_GAMUT_START	   0x19
#define EDID_COLOR_GAMUT_END	   0x22

#define EDID_STRUCT_VERSION		   0x12
#define EDID_STRUCT_REVISION	   0x13

#define EXTENSION_FLAG			   0x7e

#define DETAILED_TIMING_DESCRIPTIONS_START	0x36
#define DETAILED_TIMING_DESCRIPTION_SIZE	18
#define MONTIOR_NAME_DESCRIPTION_SIZE	 14
#define DETAILED_TIMING_DESCRIPTION_COUNT	4

#define HORIZONTAL_AND_VERTICAL_SCREEN_SIZE 0x15

#define PIXEL_CLOCK_LO	(uint8_t)(dtd[0])
#define PIXEL_CLOCK_HI	(uint8_t)(dtd[1])
#define PIXEL_CLOCK		(combine_hi_8lo(PIXEL_CLOCK_HI, PIXEL_CLOCK_LO))

#define PIXEL_CLOCK_LO_V21	(uint8_t)(dtd[3])
#define PIXEL_CLOCK_MI_V21	(uint8_t)(dtd[4])
#define PIXEL_CLOCK_HI_V21	(uint8_t)(dtd[5])
#define PIXEL_CLOCK_V21		((((((uint32_t)dtd[5]) << 16) | ((uint32_t)dtd[4]) << 8 | (uint32_t)dtd[3]) + 1) / 10)

#define H_ACTIVE_LO		(uint8_t)(dtd[2])

#define H_BLANKING_LO	(uint8_t)(dtd[3])

#define H_ACTIVE_HI		upper_nibble((uint8_t)dtd[4])

#define H_ACTIVE		combine_hi_8lo(H_ACTIVE_HI, H_ACTIVE_LO)

#define H_ACTIVE_LO_V21		(uint8_t)(dtd[7])

#define H_ACTIVE_HI_V21		((uint8_t)dtd[8])

#define H_ACTIVE_V21		(combine_hi_8lo(H_ACTIVE_HI_V21, H_ACTIVE_LO_V21) + 1)

#define H_BLANKING_HI	lower_nibble((uint8_t)dtd[4])

#define H_BLANKING		combine_hi_8lo(H_BLANKING_HI, H_BLANKING_LO)

#define H_BLANKING_LO_V21	(uint8_t)(dtd[9])

#define H_BLANKING_HI_V21	((uint8_t)dtd[10])

#define H_BLANKING_V21	(combine_hi_8lo(H_BLANKING_HI_V21, H_BLANKING_LO_V21) + 1)

#define V_ACTIVE_LO	   (uint8_t)(dtd[5])

#define V_BLANKING_LO  (uint8_t)(dtd[6])

#define V_ACTIVE_HI	   upper_nibble((uint8_t)dtd[7])

#define V_ACTIVE	   combine_hi_8lo(V_ACTIVE_HI, V_ACTIVE_LO)

#define V_ACTIVE_LO_V21	   (uint8_t)(dtd[15])

#define V_ACTIVE_HI_V21	   ((uint8_t)dtd[16])

#define V_ACTIVE_V21	   (combine_hi_8lo(V_ACTIVE_HI_V21, V_ACTIVE_LO_V21) + 1)

#define V_BLANKING_HI  lower_nibble((uint8_t)dtd[7])

#define V_BLANKING	   combine_hi_8lo(V_BLANKING_HI, V_BLANKING_LO)

#define V_BLANKING_LO_V21  (uint8_t)(dtd[17])

#define V_BLANKING_HI_V21  ((uint8_t)dtd[18])

#define V_BLANKING_V21	   (combine_hi_8lo(V_BLANKING_HI_V21, V_BLANKING_LO_V21) + 1)

#define H_SYNC_OFFSET_LO	(uint8_t)(dtd[8])
#define H_SYNC_WIDTH_LO		(uint8_t)(dtd[9])

#define V_SYNC_OFFSET_LO	upper_nibble((uint8_t)dtd[10])
#define V_SYNC_WIDTH_LO		lower_nibble((uint8_t)dtd[10])

#define V_SYNC_WIDTH_HI		((uint8_t)dtd[11] & (0x1 | 0x2))
#define V_SYNC_OFFSET_HI	(((uint8_t)dtd[11] & (0x4 | 0x8)) >> 2)

#define H_SYNC_WIDTH_HI		(((uint8_t)dtd[11] & (0x10 | 0x20)) >> 4)
#define H_SYNC_OFFSET_HI	(((uint8_t)dtd[11] & (0x40 | 0x80)) >> 6)

#define V_SYNC_WIDTH		combine_hi_4lo(V_SYNC_WIDTH_HI, V_SYNC_WIDTH_LO)
#define V_SYNC_OFFSET		combine_hi_4lo(V_SYNC_OFFSET_HI, V_SYNC_OFFSET_LO)

#define V_SYNC_WIDTH_LO_V21	(uint8_t)(dtd[21])
#define V_SYNC_WIDTH_HI_V21	(uint8_t)(dtd[22])
#define V_SYNC_WIDTH_V21		(combine_hi_8lo(V_SYNC_WIDTH_HI_V21, V_SYNC_WIDTH_LO_V21) + 1)

#define V_SYNC_OFFSET_LO_V21	(uint8_t)(dtd[19])
#define V_SYNC_OFFSET_HI_V21	(uint8_t)(dtd[20])
#define V_SYNC_OFFSET_V21	(combine_hi_8lo(V_SYNC_OFFSET_HI_V21, V_SYNC_OFFSET_LO_V21) + 1)

#define H_SYNC_WIDTH		combine_hi_8lo(H_SYNC_WIDTH_HI, H_SYNC_WIDTH_LO)
#define H_SYNC_OFFSET		combine_hi_8lo(H_SYNC_OFFSET_HI, H_SYNC_OFFSET_LO)

#define H_SYNC_WIDTH_LO_V21	(uint8_t)(dtd[13])
#define H_SYNC_WIDTH_HI_V21	(uint8_t)(dtd[14])
#define H_SYNC_WIDTH_V21		(combine_hi_8lo(H_SYNC_WIDTH_HI_V21, H_SYNC_WIDTH_LO_V21) + 1)

#define H_SYNC_OFFSET_LO_V21	(uint8_t)(dtd[11])
#define H_SYNC_OFFSET_HI_V21	(uint8_t)(dtd[12])
#define H_SYNC_OFFSET_V21	(combine_hi_8lo(H_SYNC_OFFSET_HI_V21, H_SYNC_OFFSET_LO_V21) + 1)

#define H_SIZE_LO			(uint8_t)(dtd[12])
#define V_SIZE_LO			(uint8_t)(dtd[13])

#define H_SIZE_HI			upper_nibble((uint8_t)dtd[14])
#define V_SIZE_HI			lower_nibble((uint8_t)dtd[14])

#define H_SIZE				combine_hi_8lo(H_SIZE_HI, H_SIZE_LO)
#define V_SIZE				combine_hi_8lo(V_SIZE_HI, V_SIZE_LO)

#define H_BORDER			(uint8_t)(dtd[15])
#define V_BORDER			(uint8_t)(dtd[16])

#define FLAGS				(uint8_t)(dtd[17])

#define VSCAN_MODE			((FLAGS & 0x80) >> 7)
#define V_SYNC_POLARITY ((FLAGS & 0x4) >> 2)
#define H_SYNC_POLARITY ((FLAGS & 0x2) >> 1)
#define INPUT_TYPE			(((FLAGS & 0x60) >> 4) | ((FLAGS & 0x1)))
#define SYNC_SCHEME			((FLAGS & 0x18) >> 3)
#define SCHEME_DETAIL		((FLAGS & 0x6) >> 1)

#define OTHER_DESCRIPTOR_DATA	5
#define MONITOR_NAME			0xfc
#define MONITOR_LIMITS			0xfd
#define MONITOR_SERIAL_NUMBER	0xff
#define UNKNOWN_DESCRIPTOR		-1
#define DETAILED_TIMING_BLOCK	-2

#define V_MIN_RATE				block[5]
#define V_MAX_RATE				block[6]
#define H_MIN_RATE				block[7]
#define H_MAX_RATE				block[8]
#define MAX_PIXEL_CLOCK			(((int)block[9]) * 10)
#define VIDEO_TIMING_SUPPORT_FLAG	block[10]
#define GTF2_START_FREQ			(block[12] * 2)
#define GTF2_C					block[13]
#define GTF2_M					(block[14] | (block[15] << 8))
#define GTF2_K					block[16]
#define GTF2_J					block[17]
#define H_MAX_IMAGE_SIZE		0x15
#define V_MAX_IMAGE_SIZE		0x16

#define EXTENSION_HEADER		   0x02
#define EXTENSION_HEADER_TIMING    0x70

#define EXTENSION_AUDIO_TAG		   1
#define EXTENSION_VIDEO_TAG		   2
#define EXTENSION_VENDOR_TAG	   3
#define EXTENSION_SPEAKER_TAG	   4

#define EXTEN_AUDIO_FORMAT		   ((0x78 & cea_data_block[0]) >> 3)
#define EXTEN_AUDIO_MAX_CHANNELS   ((0x7 & cea_data_block[0]) + 1)
#define EXTEN_AUDIO_SAMPLING	   (0x7F & cea_data_block[1])
#define EXTEN_AUDIO_LPCM_BIT	   (0x7 & cea_data_block[2])
#define EXTEN_AUDIO_BITRATE		   ((uint8_t)cea_data_block[2] * 8)

#define EXTEN_VIDEO_NATIVE		   (((uint8_t)0x80 & *cea_data_block) >> 7)
#define EXTEN_VIDEO_CODE		   ((uint8_t)0x7F & *cea_data_block)

#define EXTEN_SPEAKER			   (0x7F & cea_data_block[0])

#define LATENCY_PRESENT	(latency_fields + interlaced_latency_fields)

/* EDID audio Data Block */
#define AUDIO_TAG		1
#define VIDEO_TAG		2
#define EDID_TAG_MASK		GENMASK(7, 5)
#define EDID_TAG_SHIFT		5
#define EDID_SIZE_MASK		GENMASK(4, 0)
#define EDID_SIZE_SHIFT		0

/* Established timing blocks */
#define ET1_800X600_60HZ	BIT(0)
#define ET1_800X600_56HZ	BIT(1)
#define ET1_640X480_75HZ	BIT(2)
#define ET1_640X480_72HZ	BIT(3)
#define ET1_640X480_67HZ	BIT(4)
#define ET1_640X480_60HZ	BIT(5)
#define ET1_720X400_88HZ	BIT(6)
#define ET1_720X400_70HZ	BIT(7)

#define ET2_1280X1024_75HZ	BIT(0)
#define ET2_1024X768_75HZ	BIT(1)
#define ET2_1024X768_70HZ	BIT(2)
#define ET2_1024X768_60HZ	BIT(3)
#define ET2_1024X768_87HZ	BIT(4)
#define ET2_832X624_75HZ	BIT(5)
#define ET2_800X600_75HZ	BIT(6)
#define ET2_800X600_72HZ	BIT(7)

#define ET3_1152X870_75HZ	BIT(7)

#define ESTABLISHED_TIMINGS_START	0x23
#define ESTABLISHED_TIMINGS(x)		BIT(x)

#define EDID_FEATURE_SUPPORT		0x18
#define EDID_FEATURE_SUPPORT_TIMING_MODE	BIT(0)
#define DEFAULT_GTF_SUPPORTED		0x00
#define RANGE_LIMITS_ONLY			0x01
#define SECONDARY_GTF_SUPPORTED		0x02
#define CVT_SUPPORTED				0x04
#define STANDARD_TIMINGS_START		0x26
#define IMAGE_ASPECT_RATIO_INDEX(x)		((0xC0 & (x)) >> 6)
#define REFRESH_RATE(x)				(0x3F & (x))

/* CVT Reduced Blanking Timing Parameter */
/* In pixel */
#define CVT_RBV2_MIN_H_BLANK		80
#define CVT_RBV2_H_SYNC				32
#define CVT_RBV2_H_FRONT_PORCH		8
/* In us */
#define CVT_RBV2_MIN_V_BLANK		460
/* In line */
#define CVT_RBV2_V_SYNC				8
#define CVT_RBV2_MIN_V_FRONT_PORCH	1
#define CVT_RBV2_MIN_V_BACK_PORCH	6
/* 1 indicates positive, 0 indicates negative */
#define CVT_RBV2_H_POLARITY			1
#define CVT_RBV2_V_POLARITY			0

/* GTF Blanking Timing Parameter */
/* The size of the top and bottom overscan margin as a percentage(1.8%) of the active vertical image */
#define	GTF_MARGIN_PERCENT			18
/* The assumed character cell granularity of the graphics system (in pixel) */
#define GTF_CELL_GRAN				8
/* The minimum vertical front porch in lines */
#define GTF_MIN_V_FRONT_PORCH		1
/* The width of the V sync in lines */
#define GTF_V_SYNC					3
/* The width of the H sync as a percentage(8%) of the total line period */
#define GTF_H_SYNC_PERCENT			8
/* Minimum time of vertical sync + back porch interval (us) */
#define GTF_MIN_V_SYNC_PLUS_BP		550
/* The default blanking formula gradient(600%/kHz) */
#define GTF_DEFAULT_M            	600
/* The default blanking formula offset(40%) */
#define GTF_DEFAULT_C				40
/* The default blanking formula scaling factor */
#define GTF_DEFAULT_K				128
/* The blanking formula scaling factor weighting(20%) */
#define GTF_DEFAULT_J				20

enum timing_mode {
	VESA_DMT_MODE = 0,
	VESA_CVT_MODE,
	VESA_GTF_MODE,
    VESA_GTF2_MODE
};

enum cvt_version {
	CVT_CRT = 0,
	CVT_RBV1,
	CVT_RBV2,
	CVT_RBV3
};

enum gtf_version {
	GTF_DEFAULT = 0,
	GTF_SECONDARY,
};

int parse_edid(struct dp_ctrl *dptx, uint16_t len);
int dptx_calc_fps(struct timing_info *dptx_timing_node, uint32_t *fps);
int release_edid_info(struct dp_ctrl *dptx);
bool convert_code_to_dtd(struct dtd *mdtd, uint8_t code, uint32_t refresh_rate, uint8_t video_format);
bool convert_code_to_dtd_by_user(struct dp_ctrl *dptx, struct dtd *mdtd, uint8_t *fps);

int parse_main(struct dp_ctrl *dptx);
int parse_extension(struct dp_ctrl *dptx, uint8_t *exten);
int parse_extension_v21(struct dp_ctrl *dptx, uint8_t *exten);
int parse_timing_description(struct dp_ctrl *dptx, uint8_t *dtd);
int parse_timing_description_v21(struct dp_ctrl *dptx, uint8_t *dtd);
int parse_cea_data_block(struct dp_ctrl *dptx, uint8_t *cea_data, uint8_t dtd_start);
int parse_monitor_limits(struct dp_ctrl *dptx, uint8_t *block);
int parse_monitor_name(struct dp_ctrl *dptx, uint8_t *blockname, uint32_t size);
int block_type(uint8_t *block);

int parse_established_timing(struct dp_ctrl *dptx);
int parse_standard_timing(struct dp_ctrl *dptx);

#endif /* DP_EDID_H */
#pragma GCC diagnostic pop
