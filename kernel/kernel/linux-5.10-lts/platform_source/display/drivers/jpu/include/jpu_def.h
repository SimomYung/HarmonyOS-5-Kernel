/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpeg jpu def
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef JPU_DEF_H
#define JPU_DEF_H

#define jpu_err_if_cond(cond, msg, ...) \
	do { \
		if (cond) { \
			jpu_err(msg, ##__VA_ARGS__); \
		} \
	} while (0)

#define jpu_check_null_return(in, out) do { \
	if ((in) == NULL) { \
		jpu_err("check null ptr at line %d\n", __LINE__); \
		return (out); \
	} \
} while (0)

#ifdef CONFIG_HISI_JPU_DUMP_REG
#define outp32(addr, val) \
	do { \
		writel(val, addr); \
		jpu_info("writel(0x%x, 0x%x);\n", val, (UINTPTR)addr); \
	} while (0)
#else
#define outp32(addr, val) writel(val, addr)
#endif

#define inp32(addr) readl(addr)

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#define JPG_CLK_ENABLE_DELAY 1
#define JPG_MEMORY_REMPAIR_DELAY 400

#define REG_SET_32_BIT 32
#define REG_SET_30_BIT 30
#define REG_SET_29_BIT 29
#define REG_SET_28_BIT 28
#define REG_SET_25_BIT 25
#define REG_SET_24_BIT 24
#define REG_SET_20_BIT 20
#define REG_SET_16_BIT 16
#define REG_SET_14_BIT 14
#define REG_SET_7_BIT 7
#define REG_SET_6_BIT 6
#define REG_SET_4_BIT 4
#define REG_SET_2_BIT 2
#define SHIFT_32_BIT 32
#define SHIFT_16_BIT 16
#define SHIFT_12_BIT 12
#define SHIFT_8_BIT 8
#define SHIFT_4_BIT 4

#define MEDIA1_CLK_ENABLE 0x08040040
#define MEDIA1_OTHERS_CLK_ENABLE 0x00040040
#define AXI_JPEG_CVDR_WR_QOS_CFG_VAL 0x10333311
#define AXI_JPEG_NR_RD_STOP 0x01000000
#define JPEGDEC_TPSRAM_2PRF_TRA 0x00010000
#define JPEGDEC_FAMA_PTW_MSB 0x0000007F
#define JPEGDEC_DECODE_START_DISABLE 0x80000001
#define JPEGDEC_FAMA_PTW_MID 0x80
#define JPGDEC_EVEN_NUM 2

enum CVDR_USER {
	JPEGD_ACPU,
	JPEGD_IOMCU,
	CVDR_BUTT,
};

enum VOTE_VALUE{
	VOTE_OFF,
	VOTE_ON,
};

#define JPGDEC_DONE_TIMEOUT_THRESHOLD_ASIC 400
#define JPGDEC_DONE_TIMEOUT_THRESHOLD_FPGA (15 * 1000)
#define VOTE_WAIT_TIMES		8
#define VOTE_BIT_MASK		16
#define VOTE_SLEEP_TIME		10

#define jpegd_stop_here(flag, fmt, arg...) \
do { \
	while (flag) { \
		msleep(1000); \
		jpu_info(fmt, ##arg); \
	} \
} while (0)

#endif /* JPU_DEF_H */
