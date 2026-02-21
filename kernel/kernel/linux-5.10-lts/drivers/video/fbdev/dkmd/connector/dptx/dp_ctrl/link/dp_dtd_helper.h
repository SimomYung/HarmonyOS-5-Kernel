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

#ifndef __DP_DTD_HELPER_H__
#define __DP_DTD_HELPER_H__

#ifndef dtd_array_size
#define dtd_array_size(_A) (sizeof(_A) / sizeof((_A)[0]))
#endif /* dtd_array_size */

#define detailed_timing_descriptor(his, vis, ha, va, hb, vb, hso, vso, hspw, vspw, hp, vp, il, pc, pri) \
	.h_image_size = his, .v_image_size = vis, .h_active = ha, .v_active = va, .h_blanking = hb, .v_blanking = vb, \
	.h_sync_offset = hso, .v_sync_offset = vso, .h_sync_pulse_width = hspw, .v_sync_pulse_width = vspw, \
	.h_sync_polarity = hp, .v_sync_polarity = vp, .interlaced = il, .pixel_clock = pc, .pixel_repetition_input = pri

/* sync polarity */
#define POSITIVE 1
#define NEGATIVE 0

/* vscan mode */
#define PROGRESSIVE 0
#define INTERLACED 1

static const struct dtd cea_modes_dtd[] = {
	/* 0 - dummy, cea_code start at 1 */
	{ },
	/* 1 - 640x480p @ 59.94/60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, NEGATIVE, NEGATIVE, PROGRESSIVE, 25175, 0) },
	/* 2 - 720x480p @ 59.94/60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 3 - 720x480p @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 4 - 1280x720p @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 370, 30, 110, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 5 - 1920x1080i @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 280, 22, 88, 2, 44, 5, POSITIVE, POSITIVE, INTERLACED, 74250, 0) },
	/* 6 - 1440x480i @ 59.94/60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 27000, 0) },
	/* 7 - 1440x480i @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 27000, 0) },
	/* 8 - 1440x240p @ 59.826/60.054/59.886/60.115Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 240, 276, 23, 38, 5, 124, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 9 - 1440x240p @59.826/60.054/59.886/60.115Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 240, 276, 23, 38, 5, 124, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 10 - 2880x480i @ 59.94/60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 240, 552, 22, 76, 4, 248, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 11 - 2880x480i @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 240, 552, 22, 76, 4, 248, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 12 - 2880x240p @ 59.826/60.054/59.886/60.115Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 240, 552, 23, 76, 5, 248, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 13 - 2880x240p @ 59.826/60.054/59.886/60.115Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 240, 552, 23, 76, 5, 248, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 14 - 1440x480p @ 59.94/60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 480, 276, 45, 32, 9, 124, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 15 - 1440x480p @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 480, 276, 45, 32, 9, 124, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 16 - 1920x1080p @ 59.94/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 17 - 720x576p @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 18 - 720x576p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 19 - 1280x720p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 700, 30, 440, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 20 - 1920x1080i @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 720, 22, 528, 2, 44, 5, POSITIVE, POSITIVE, INTERLACED, 74250, 0) },
	/* 21 - 1440x576i @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 27000, 0) },
	/* 22 - 1440x576i @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 27000, 0) },
	/* 23 - 1440x288p @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 288, 288, 26, 24, 4, 126, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 24 - 1440x288p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 288, 288, 26, 24, 4, 126, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 27000, 0) },
	/* 25 - 2880x576i @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 576, 576, 24, 48, 2, 252, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 26 - 2880x576i @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 576, 576, 24, 48, 2, 252, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 27 - 2880x288p @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 288, 576, 26, 48, 4, 252, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 28 - 2880x288p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 288, 576, 26, 48, 4, 252, 3, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 29 - 1440x576p @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 576, 288, 49, 24, 5, 128, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 30 - 1440x576p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 576, 288, 49, 24, 5, 128, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 31 - 1920x1080p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 32 - 1920x1080p @ 23.976/24Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 830, 45, 638, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 33 - 1920x1080p @ 25Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 34 - 1920x1080p @ 29.97/30Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 35 - 2880x480p @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 480, 552, 45, 64, 9, 248, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 36 - 2880x480p @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 480, 552, 45, 64, 9, 248, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 37 - 2880x576p @ 50Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 2880, 576, 576, 49, 48, 5, 256, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 38 - 2880x576p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2880, 576, 576, 49, 48, 5, 256, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 39 - 1920x1080i (1250 total) @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 384, 85, 32, 23, 168, 5, POSITIVE, NEGATIVE, INTERLACED, 72000, 0) },
	/* 40 - 1920x1080i @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 720, 22, 528, 2, 44, 5, POSITIVE, POSITIVE, INTERLACED, 148500, 0) },
	/* 41 - 1280x720p @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 700, 30, 440, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 42 - 720x576p @ 100Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 43 - 720x576p @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 44 - 1440x576i @ 100Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 45 - 1440x576i @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 46 - 1920x1080i @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 288, 22, 88, 2, 44, 5, POSITIVE, POSITIVE, INTERLACED, 148500, 0) },
	/* 47 - 1280x720p @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 370, 30, 110, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 48 - 720x480p @ 119.88/120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 49 - 720x480p @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 54000, 0) },
	/* 50 - 1440x480i @ 119.88/120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 51 - 1440x480i @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 54000, 0) },
	/* 52 - 720X576p @ 200Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 53 - 720X576p @ 200Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 576, 144, 49, 12, 5, 64, 5, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 54 - 1440x576i @ 200Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 108000, 0) },
	/* 55 - 1440x576i @ 200Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 576, 288, 24, 24, 2, 126, 3, NEGATIVE, NEGATIVE, INTERLACED, 108000, 0) },
	/* 56 - 720x480p @ 239.76/240Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 57 - 720x480p @ 239.76/240Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 720, 480, 138, 45, 16, 9, 62, 6, NEGATIVE, NEGATIVE, PROGRESSIVE, 108000, 0) },
	/* 58 - 1440x480i @ 239.76/240Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 108000, 0) },
	/* 59 - 1440x480i @ 239.76/240Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1440, 480, 276, 22, 38, 4, 124, 3, NEGATIVE, NEGATIVE, INTERLACED, 108000, 0) },
	/* 60 - 1280x720p @ 23.97/24Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 2020, 30, 1760, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 59400, 0) },
	/* 61 - 1280x720p @ 25Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 2680, 30, 2420, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 62 - 1280x720p @ 29.97/30Hz  16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 2020, 30, 1760, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 63 - 1920x1080p @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 64 - 1920x1080p @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 65 - 1280x720p @ 23.97/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 2020, 30, 1760, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 59400, 0) },
	/* 66 - 1280x720p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 2680, 30, 2420, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 67 - 1280x720p @ 29.97/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 2020, 30, 1760, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 68 - 1280x720p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 700, 30, 440, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 69 - 1280x720p @ 59.94/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 370, 30, 110, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 70 - 1280x720p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 700, 30, 440, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 71 - 1280x720p @ 119.88/120Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 370, 30, 110, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 72 - 1920x1080p @ 23.976/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 830, 45, 638, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 73 - 1920x1080p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 74 - 1920x1080p @ 29.97/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 74250, 0) },
	/* 75 - 1920x1080p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 76 - 1920x1080p @ 59.94/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 77 - 1920x1080p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 720, 45, 528, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 78 - 1920x1080p @ 119.88/120Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 79 - 1680x720p @ 23.98Hz/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 1620, 30, 1360, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 59400, 0) },
	/* 80 - 1680x720p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 1488, 30, 1228, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 59400, 0) },
	/* 81 - 1680x720p @ 29.97Hz/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 960, 30, 700, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 59400, 0) },
	/* 82 - 1680x720p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 520, 30, 260, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 82500, 0) },
	/* 83 - 1680x720p @ 59.94Hz/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 520, 30, 260, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 99000, 0) },
	/* 84 - 1680x720p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 320, 105, 60, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 165000, 0) },
	/* 85 - 1680x720p @ 119.88/120Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 320, 105, 60, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 198000, 0) },
	/* 86 - 2560x1080p @ 23.98Hz/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 1190, 20, 998, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 99000, 0) },
	/* 87 - 2560x1080p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 640, 45, 448, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 90000, 0) },
	/* 88 - 2560x1080p @ 29.97Hz/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 960, 45, 768, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 118800, 0) },
	/* 89 - 2560x1080p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 740, 45, 548, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 185625, 0) },
	/* 90 - 2560x1080p @ 59.94Hz/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 440, 20, 248, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 198000, 0) },
	/* 91 - 2560x1080p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 410, 170, 218, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 371250, 0) },
	/* 92 - 2560x1080p @ 119.88/120Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 740, 170, 548, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 495000, 0) },
	/* 93 - 3840x2160p @ 23.98Hz/24Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 1660, 90, 1276, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 94 - 3840x2160p @ 25Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 95 - 3840x2160p @ 29.97Hz/30Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 96 - 3840x2160p @ 50Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 97 - 3840x2160p @ 59.94Hz/60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 98 - 4096x2160p @ 23.98Hz/24Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 1404, 90, 1020, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 99 - 4096x2160p @ 25Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 1184, 90, 968, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 100 - 4096x2160p @ 29.97Hz/30Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 304, 90, 88, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 101 - 4096x2160p @ 50Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 1184, 90, 968, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 102 - 4096x2160p @ 59.94Hz/60Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 304, 90, 88, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 103 - 3840x2160p @ 23.98Hz/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 1660, 90, 1276, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 104 - 3840x2160p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 105 - 3840x2160p @ 29.97Hz/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 297000, 0) },
	/* 106 - 3840x2160p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 107 - 3840x2160p @ 59.94Hz/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 108 - 1280x720p @ 47.95/48Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 1220, 30, 960, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 90000, 0) },
	/* 109 - 1280x720p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1280, 720, 1220, 30, 960, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 90000, 0) },
	/* 110 - 1680x720p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1680, 720, 1070, 30, 810, 5, 40, 5, POSITIVE, POSITIVE, PROGRESSIVE, 99000, 0) },
	/* 111 - 1920x1080p @ 47.95/48Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 830, 45, 638, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 112 - 1920x1080p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 1920, 1080, 830, 45, 638, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 148500, 0) },
	/* 113 - 2560x1080p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 2560, 1080, 1190, 20, 998, 4, 44, 5, POSITIVE, POSITIVE, PROGRESSIVE, 198000, 0) },
	/* 114 - 3840x2160p @ 47.95/48Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 1660, 90, 1276, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 115 - 4096x2160p @ 47.95/48Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 1404, 90, 1020, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 116 - 3840x2160p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 1660, 90, 1276, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 594000, 0) },
	/* 117 - 3840x2160p @ 100Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 1188000, 0) },
	/* 118 - 3840x2160p @ 119.88/120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 1188000, 0) },
	/* 119 - 3840x2160p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 1440, 90, 1056, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 1188000, 0) },
	/* 120 - 3840x2160p @ 119.88/120Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 3840, 2160, 560, 90, 176, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 1188000, 0) },
	/* 121 - 5120x2160p @ 23.98/24Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 2380, 40, 1996, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 396000, 0) },
	/* 122 - 5120x2160p @ 25Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 2080, 40, 1696, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 396000, 0) },
	/* 123 - 5120x2160p @ 29.97/30Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 880, 40, 664, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 396000, 0) },
	/* 124 - 5120x2160p @ 47.95/48Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 1130, 315, 746, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 742500, 0) },
	/* 125 - 5120x2160p @ 50Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 1480, 90, 1096, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 742500, 0) },
	/* 126 - 5120x2160p @ 59.94/60Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 380, 90, 164, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 742500, 0) },
	/* 127 - 5120x2160p @ 100Hz 64:27 */
	{ detailed_timing_descriptor(64, 27, 5120, 2160, 1480, 90, 1096, 8, 88, 10, POSITIVE, POSITIVE, PROGRESSIVE, 1485000, 0) },
};

static const struct dtd cvt_modes_dtd[] = {
	/* 0 - dummy, cvt_code start at 1 */
	{ },
	/* 1 - */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 20, 8, 1, 32, 8, 1, 0, 0, 23750, 0) },
	/* 2 - */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 24, 31, 3, 81, 4, 1, 0, 0, 38250, 0) },
	/* 3 - */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 304, 30, 48, 3, 104, 4, 0, 1, 0, 63500, 0) },
	/* 4 - */
	{ detailed_timing_descriptor(4, 3, 1280, 960, 416, 36, 80, 3, 128, 4, 0, 1, 0, 101250, 0) },
	/* 5 - */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 464, 39, 88, 3, 144, 4, 0, 1, 0, 121750, 0) },
	/* 6 - */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 45, 112, 3, 68, 4, 0, 1, 0, 161000, 0) },
	/* 7 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 8 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 9 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 10 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 11 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 12 - */
	{ detailed_timing_descriptor(4, 3, 1280, 1024, 432, 39, 80, 3, 136, 7, 0, 1, 0, 109000, 0) },
	/* 13 - */
	{ detailed_timing_descriptor(4, 3, 1280, 768, 384, 30, 64, 3, 128, 7, 0, 1, 0, 79500, 0) },
	/* 14 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 15 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 16 - */
	{ detailed_timing_descriptor(4, 3, 1280, 720, 384, 28, 64, 3, 128, 5, 0, 1, 0, 74500, 0) },
	/* 17 - */
	{ detailed_timing_descriptor(4, 3, 1360, 768, 416, 30, 72, 3, 136, 5, 0, 1, 0, 84750, 0) },
	/* 18 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 19 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 20 - */
	{ detailed_timing_descriptor(4, 3, 1920, 1080, 656, 40, 128, 3, 200, 5, 0, 1, 0, 173000, 0) },
	/* 21 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 22 - */
	{ detailed_timing_descriptor(4, 3, 2560, 1440, 928, 53, 192, 3, 272, 5, 0, 1, 0, 312250, 0) },
	/* 23 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 24 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 25 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 26 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 27 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 28 - */
	{ detailed_timing_descriptor(4, 3, 1280, 800, 400, 31, 72, 3, 128, 6, 0, 1, 0, 83500, 0) },
	/* 29 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 30 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 31 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 32 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 33 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 34 - */
	{ detailed_timing_descriptor(4, 3, 1920, 1200, 672, 45, 136, 3, 200, 6, 0, 1, 0, 193250, 0) },
	/* 35 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 36 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 37 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 38 - */
	{ detailed_timing_descriptor(4, 3, 3840, 2400, 80, 69, 320, 3, 424, 6, 0, 1, 0, 580128, 0) },
	/* 39 - Details to be added. */
	{ detailed_timing_descriptor(4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) },
	/* 40 - */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 160, 35, 48, 3, 32, 4, 1, 0, 0, 124076, 0) },
	/* 41 - */
	{ detailed_timing_descriptor(4, 3, 2048, 1536, 160, 44, 48, 3, 32, 4, 1, 0, 0, 208000, 0) },
	/* 42 - */
	{ detailed_timing_descriptor(3, 2, 3120, 2080, 140, 128, 72, 36, 32, 6, 0, 0, 0, 863769, 0) },
};

static const struct dtd dmt_modes_dtd[] = {
	/* 0 - dummy, dmt_code start at 1 */
	{ },
	/* 0x01 - 640x350 @ 85Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 640, 350, 192, 95, 32, 32, 64, 3, POSITIVE, NEGATIVE, 0, 31500, 0) },
	/* 0x02 - 640x400 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 640, 400, 192, 45, 32, 1, 64, 3, NEGATIVE, POSITIVE, 0, 31500, 0) },
	/* 0x03 - 720x400 @ 85Hz 9:5 */
	{ detailed_timing_descriptor(9, 5, 720, 400, 216, 46, 36, 1, 72, 3, NEGATIVE, POSITIVE, 0, 35500, 0) },
	/* 0x04 - 640x480 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, NEGATIVE, NEGATIVE, 0, 25175, 0) },
	/* 0x05 - 640x480 @ 72Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 640, 480, 192, 40, 24, 9, 40, 3, NEGATIVE, NEGATIVE, 0, 31500, 0) },
	/* 0x06 - 640x480 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 640, 480, 200, 20, 16, 1, 64, 3, NEGATIVE, NEGATIVE, 0, 31500, 0) },
	/* 0x07 - 640x480 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 640, 480, 192, 29, 56, 1, 56, 3, NEGATIVE, NEGATIVE, 0, 36000, 0) },
	/* 0x08 - 800x600 @ 56Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 25, 24, 1, 72, 2, POSITIVE, POSITIVE, 0, 36000, 0) },
	/* 0x09 - 800x600 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 256, 28, 40, 1, 128, 4, POSITIVE, POSITIVE, 0, 40000, 0) },
	/* 0x0A - 800x600 @ 72Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 240, 66, 56, 37, 120, 6, POSITIVE, POSITIVE, 0, 50000, 0) },
	/* 0x0B - 800x600 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 256, 25, 16, 1, 80, 3, POSITIVE, POSITIVE, 0, 49500, 0) },
	/* 0x0C - 800x600 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 248, 31, 32, 1, 64, 3, POSITIVE, POSITIVE, 0, 56250, 0) },
	/* 0x0D - 800x600 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 800, 600, 160, 36, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 73250, 0) },
	/* 0x0E - 848x480 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 848, 480, 240, 37, 16, 6, 112, 8, POSITIVE, POSITIVE, 0, 33750, 0) },
	/* 0x0F - 1024x768 @ 43Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 240, 24, 8, 0, 176, 4, POSITIVE, POSITIVE, 1, 44900, 0) },
	/* 0x10 - 1024x768 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 320, 38, 24, 3, 136, 6, NEGATIVE, NEGATIVE, 0, 65000, 0) },
	/* 0x11 - 1024x768 @ 70Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 304, 38, 24, 3, 136, 6, NEGATIVE, NEGATIVE, 0, 75000, 0) },
	/* 0x12 - 1024x768 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 288, 32, 16, 1, 96, 3, POSITIVE, POSITIVE, 0, 78750, 0) },
	/* 0x13 - 1024x768 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 352, 40, 48, 1, 96, 3, POSITIVE, POSITIVE, 0, 94500, 0) },
	/* 0x14 - 1024x768 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 160, 45, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 115500, 0) },
	/* 0x15 - 1152x864 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1152, 864, 448, 36, 64, 1, 128, 3, POSITIVE, POSITIVE, 0, 108000, 0) },
	/* 0x16 - 1280x768 @ 60Hz 5:3 */
	{ detailed_timing_descriptor(5, 3, 1280, 768, 160, 22, 48, 3, 32, 7, POSITIVE, NEGATIVE, 0, 68250, 0) },
	/* 0x17 - 1280x768 @ 60Hz 5:3 */
	{ detailed_timing_descriptor(5, 3, 1280, 768, 384, 30, 64, 3, 128, 7, NEGATIVE, POSITIVE, 0, 79500, 0) },
	/* 0x18 - 1280x768 @ 75Hz 5:3 */
	{ detailed_timing_descriptor(5, 3, 1280, 768, 416, 37, 80, 3, 128, 7, NEGATIVE, POSITIVE, 0, 102250, 0) },
	/* 0x19 - 1280x768 @ 85Hz 5:3 */
	{ detailed_timing_descriptor(5, 3, 1280, 768, 432, 41, 80, 3, 136, 7, NEGATIVE, POSITIVE, 0, 117500, 0) },
	/* 0x1A - 1280x768 @ 120Hz 5:3 */
	{ detailed_timing_descriptor(5, 3, 1280, 768, 160, 45, 48, 3, 32, 7, POSITIVE, NEGATIVE, 0, 140250, 0) },
	/* 0x1B - 1280x800 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1280, 800, 160, 23, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 71000, 0) },
	/* 0x1C - 1280x800 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1280, 800, 400, 31, 72, 3, 128, 6, NEGATIVE, POSITIVE, 0, 83500, 0) },
	/* 0x1D - 1280x800 @ 75Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1280, 800, 416, 38, 80, 3, 128, 6, NEGATIVE, POSITIVE, 0, 106500, 0) },
	/* 0x1E - 1280x800 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1280, 800, 432, 43, 80, 3, 136, 6, NEGATIVE, POSITIVE, 0, 122500, 0) },
	/* 0x1F - 1280x800 @ 120Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1280, 800, 160, 47, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 146250, 0) },
	/* 0x20 - 1280x960 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1280, 960, 520, 40, 96, 1, 112, 3, POSITIVE, POSITIVE, 0, 108000, 0) },
	/* 0x21 - 1280x960 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1280, 960, 448, 51, 64, 1, 160, 3, POSITIVE, POSITIVE, 0, 148500, 0) },
	/* 0x22 - 1280x960 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1280, 960, 160, 57, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 175500, 0) },
	/* 0x23 - 1280x1024 @ 60Hz 5:4 */
	{ detailed_timing_descriptor(5, 4, 1280, 1024, 408, 42, 48, 1, 112, 3, POSITIVE, POSITIVE, 0, 108000, 0) },
	/* 0x24 - 1280x1024 @ 75Hz 5:4 */
	{ detailed_timing_descriptor(5, 4, 1280, 1024, 408, 42, 16, 1, 144, 3, POSITIVE, POSITIVE, 0, 135000, 0) },
	/* 0x25 - 1280x1024 @ 85Hz 5:4 */
	{ detailed_timing_descriptor(5, 4, 1280, 1024, 448, 48, 64, 1, 160, 3, POSITIVE, POSITIVE, 0, 157500, 0) },
	/* 0x26 - 1280x1024 @ 120Hz 5:4 */
	{ detailed_timing_descriptor(5, 4, 1280, 1024, 160, 60, 48, 3, 32, 7, POSITIVE, NEGATIVE, 0, 187250, 0) },
	/* 0x27 - 1360x768 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1360, 768, 432, 27, 64, 3, 112, 6, POSITIVE, POSITIVE, 0, 85500, 0) },
	/* 0x28 - 1360x768 @ 120Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1360, 768, 160, 45, 48, 3, 32, 5, POSITIVE, NEGATIVE, 0, 148250, 0) },
	/* 0x29 - 1400x1050 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 160, 30, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 101000, 0) },
	/* 0x2A - 1400x1050 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 464, 39, 88, 3, 144, 4, NEGATIVE, POSITIVE, 0, 121750, 0) },
	/* 0x2B - 1400x1050 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 496, 49, 104, 3, 144, 4, NEGATIVE, POSITIVE, 0, 156000, 0) },
	/* 0x2C - 1400x1050 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 512, 55, 104, 3, 152, 4, NEGATIVE, POSITIVE, 0, 179500, 0) },
	/* 0x2D - 1400x1050 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1400, 1050, 160, 62, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 208000, 0) },
	/* 0x2E - 1440x900 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1440, 900, 160, 26, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 88750, 0) },
	/* 0x2F - 1440x900 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1440, 900, 464, 34, 80, 3, 152, 6, NEGATIVE, POSITIVE, 0, 106500, 0) },
	/* 0x30 - 1440x900 @ 75Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1440, 900, 496, 42, 96, 3, 152, 6, NEGATIVE, POSITIVE, 0, 136750, 0) },
	/* 0x31 - 1440x900 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1440, 900, 512, 48, 104, 3, 152, 6, NEGATIVE, POSITIVE, 0, 157000, 0) },
	/* 0x32 - 1440x900 @ 120Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1440, 900, 160, 53, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 182750, 0) },
	/* 0x33 - 1600x1200 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 50, 64, 1, 192, 3, POSITIVE, POSITIVE, 0, 162000, 0) },
	/* 0x34 - 1600x1200 @ 65Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 50, 64, 1, 192, 3, POSITIVE, POSITIVE, 0, 175500, 0) },
	/* 0x35 - 1600x1200 @ 70Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 50, 64, 1, 192, 3, POSITIVE, POSITIVE, 0, 189000, 0) },
	/* 0x36 - 1600x1200 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 50, 64, 1, 192, 3, POSITIVE, POSITIVE, 0, 202500, 0) },
	/* 0x37 - 1600x1200 @ 85Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 560, 50, 64, 1, 192, 3, POSITIVE, POSITIVE, 0, 229500, 0) },
	/* 0x38 - 1600x1200 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1600, 1200, 160, 71, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 268250, 0) },
	/* 0x39 - 1680x1050 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1680, 1050, 160, 30, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 119000, 0) },
	/* 0x3A - 1680x1050 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1680, 1050, 560, 39, 104, 3, 176, 6, NEGATIVE, POSITIVE, 0, 146250, 0) },
	/* 0x3B - 1680x1050 @ 75Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1680, 1050, 592, 49, 120, 3, 176, 6, NEGATIVE, POSITIVE, 0, 187000, 0) },
	/* 0x3C - 1680x1050 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1680, 1050, 608, 55, 128, 3, 176, 6, NEGATIVE, POSITIVE, 0, 214750, 0) },
	/* 0x3D - 1680x1050 @ 120Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1680, 1050, 160, 62, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 245500, 0) },
	/* 0x3E - 1792x1344 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1792, 1344, 656, 50, 128, 1, 200, 3, NEGATIVE, POSITIVE, 0, 204750, 0) },
	/* 0x3F - 1792x1344 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1792, 1344, 664, 73, 96, 1, 216, 3, NEGATIVE, POSITIVE, 0, 261000, 0) },
	/* 0x40 - 1792x1344 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1792, 1344, 160, 79, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 333250, 0) },
	/* 0x41 - 1856x1392 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1856, 1392, 672, 47, 96, 1, 224, 3, NEGATIVE, POSITIVE, 0, 218250, 0) },
	/* 0x42 - 1856x1392 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1856, 1392, 704, 108, 128, 1, 224, 3, NEGATIVE, POSITIVE, 0, 288000, 0) },
	/* 0x43 - 1856x1392 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1856, 1392, 160, 82, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 356500, 0) },
	/* 0x44 - 1920x1200 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1920, 1200, 160, 35, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 154000, 0) },
	/* 0x45 - 1920x1200 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1920, 1200, 672, 45, 136, 3, 200, 6, NEGATIVE, POSITIVE, 0, 193250, 0) },
	/* 0x46 - 1920x1200 @ 75Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1920, 1200, 688, 55, 136, 3, 208, 6, NEGATIVE, POSITIVE, 0, 245250, 0) },
	/* 0x47 - 1920x1200 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1920, 1200, 704, 62, 144, 3, 208, 6, NEGATIVE, POSITIVE, 0, 281250, 0) },
	/* 0x48 - 1920x1200 @ 120Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 1920, 1200, 160, 71, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 317000, 0) },
	/* 0x49 - 1920x1440 @ 60Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1920, 1440, 680, 60, 128, 1, 208, 3, NEGATIVE, POSITIVE, 0, 234000, 0) },
	/* 0x4A - 1920x1440 @ 75Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1920, 1440, 720, 60, 144, 1, 224, 3, NEGATIVE, POSITIVE, 0, 297000, 0) },
	/* 0x4B - 1920x1440 @ 120Hz 4:3 */
	{ detailed_timing_descriptor(4, 3, 1920, 1440, 160, 85, 48, 3, 32, 4, POSITIVE, NEGATIVE, 0, 380500, 0) },
	/* 0x4C - 2560x1600 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 2560, 1600, 160, 46, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 268500, 0) },
	/* 0x4D - 2560x1600 @ 60Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 2560, 1600, 944, 58, 192, 3, 280, 6, NEGATIVE, POSITIVE, 0, 348500, 0) },
	/* 0x4E - 2560x1600 @ 75Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 2560, 1600, 976, 72, 208, 3, 280, 6, NEGATIVE, POSITIVE, 0, 443250, 0) },
	/* 0x4F - 2560x1600 @ 85Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 2560, 1600, 976, 82, 208, 3, 280, 6, NEGATIVE, POSITIVE, 0, 505250, 0) },
	/* 0x50 - 2560x1600 @ 120Hz 16:10 */
	{ detailed_timing_descriptor(16, 10, 2560, 1600, 160, 94, 48, 3, 32, 6, POSITIVE, NEGATIVE, 0, 552750, 0) },
	/* 0x51 - 1366x768 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1366, 768, 426, 30, 70, 3, 143, 3, POSITIVE, POSITIVE, 0, 85500, 0) },
	/* 0x52 - 1920x1080 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1920, 1080, 280, 45, 88, 4, 44, 5, POSITIVE, POSITIVE, 0, 148500, 0) },
	/* 0x53 - 1600x900 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1600, 900, 200, 100, 24, 1, 80, 3, POSITIVE, POSITIVE, 0, 108000, 0) },
	/* 0x54 - 2048x1152 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 2048, 1152, 202, 48, 26, 1, 80, 3, POSITIVE, POSITIVE, 0, 162000, 0) },
	/* 0x55 - 1280x720 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1280, 720, 370, 30, 110, 5, 40, 5, POSITIVE, POSITIVE, 0, 74250, 0) },
	/* 0x56 - 1366x768 @ 60Hz 16:9 */
	{ detailed_timing_descriptor(16, 9, 1366, 768, 134, 32, 14, 1, 56, 3, POSITIVE, POSITIVE, 0, 72000, 0) },
	/* 0x57 - 4096x2160 @ 60Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 80, 62, 8, 48, 32, 8, POSITIVE, NEGATIVE, 0, 556744, 0) },
	/* 0x58 - 4096x2160 @ 59.94Hz 256:135 */
	{ detailed_timing_descriptor(256, 135, 4096, 2160, 80, 62, 8, 48, 32, 8, POSITIVE, NEGATIVE, 0, 556188, 0) },
};

static const struct dtd edid_estab_timing[] = {
	/* 1 - 800x600p @ 60Hz  */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 24, 31, 3, 81, 4, 1, 0, 0, 38340, 0) },
	/* 2 - 800x600p @ 56Hz  */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 24, 31, 3, 81, 4, 1, 0, 0, 35790, 0) },
	/* 3 - 640x480p @ 75Hz  */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, 0, 0, 0, 31500, 0) },
	/* 4 - 640x480p @ 72Hz  */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, 0, 0, 0, 30240, 0) },
	/* 5 - 640x480p @ 67Hz  */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, 0, 0, 0, 28140, 0) },
	/* 6 - 640x480p @ 60Hz  */
	{ detailed_timing_descriptor(4, 3, 640, 480, 160, 45, 16, 10, 96, 2, 0, 0, 0, 25200, 0) },
	/* 7 - 720x400p @ 88Hz  */
	{ detailed_timing_descriptor(4, 3, 720, 400, 138, 45, 16, 9, 62, 6, 0, 0, 0, 33600, 0) },
	/* 8 - 720x400p @ 70Hz  */
	{ detailed_timing_descriptor(4, 3, 720, 400, 138, 45, 16, 9, 62, 6, 0, 0, 0, 26730, 0) },
	/* 9 - 1280x1024p 75Hz  */
	{ detailed_timing_descriptor(5, 4, 1280, 1024, 432, 39, 80, 3, 136, 7, 0, 1, 0, 136490, 0) },
	/* 10 - 1024x768p @ 75Hz  */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 320, 38, 24, 3, 136, 6, 0, 0, 0, 81250, 0) },
	/* 11 - 1024x768p @ 70Hz  */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 320, 38, 24, 3, 136, 6, 0, 0, 0, 75830, 0) },
	/* 12 - 1024x768p @ 60Hz  */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 320, 38, 24, 3, 136, 6, 0, 0, 0, 65000, 0) },
	/* 13 - 1024x768p @ 87Hz  */
	{ detailed_timing_descriptor(4, 3, 1024, 768, 320, 38, 24, 3, 136, 6, 0, 0, 0, 94250, 0) },
	/* 14 - 832x624p @ 75Hz  */
	{ detailed_timing_descriptor(4, 3, 832, 624, 256, 25, 16, 1, 80, 3, 0, 0, 0, 52960, 0) },
	/* 15 - 800x600p @ 75Hz  */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 24, 31, 3, 81, 4, 1, 0, 0, 47930, 0) },
	/* 16 - 800x600p @ 72Hz  */
	{ detailed_timing_descriptor(4, 3, 800, 600, 224, 24, 31, 3, 81, 4, 1, 0, 0, 46010, 0) },
	/* 17 - 1152x870p @ 75Hz  */
	{ detailed_timing_descriptor(4, 3, 1152, 870, 416, 36, 80, 3, 128, 4, 0, 1, 0, 106550, 0) },
};

#endif /* DP_DTD_HELPER_H */
