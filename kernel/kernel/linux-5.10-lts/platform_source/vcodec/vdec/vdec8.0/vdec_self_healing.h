/*
 * vdec_self_healing.h
 *
 * This is for vdec self-healing
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef VCODEC_SELF_HEALING
#define VCODEC_SELF_HEALING

#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/time64.h>
#ifdef CONFIG_IMPROVE_YIELD
#include "platform_include/basicplatform/linux/improve_yield.h"
#define VDEC_IY_NV_INFO_MAX_LENGTH 256
#endif

#define SH_PV_ON   1
#define SH_PV_OFF  0

#define VDEC_MAX_DMD_MCU_ERR_CNT 3
#define VDEC_MAX_DMD_ERR_CNT 8
#define VDEC_MAX_DMD_CHECK_INTERVAL (60 * 60 * 24)
#define VDEC_MAX_DMD_REPORT_INTERVAL (60 * 60 * 1)

enum vdec_dmd_type {
	VDEC_DMD_MCU_PANIC,
	VDEC_DMD_BSP_TIMEOUT,
	VDEC_DMD_PXP_TIMEOUT,
	VDEC_DMD_BUTT
};
typedef struct {
	uint32_t dmd_err_cnt;
	time64_t last_report_ts;
	time64_t timestamp[VDEC_MAX_DMD_ERR_CNT];
} vdec_sh_node;

typedef struct {
	bool is_sh_nv_init;
	bool nv_flag;
	vdec_sh_node nodes[VDEC_DMD_BUTT];
} vdec_sh;

#ifdef CONFIG_IMPROVE_YIELD
typedef struct {
    unsigned char enabale_flag;
    unsigned char pv_flag;
	unsigned char test_case_number;
} RtbIyParam;

typedef union {
	unsigned char value;
	struct {
		unsigned char reserved : 6;       /* bit[0-5] reerved */
		unsigned char pv_flag : 1;        /* bit[6] is pv flag, 1:enable 0:disable */
		unsigned char enable_flag : 1;   /* bit[7] is enabale flag, 1:enable 0:disable */
	} bits;
} improve_yield_vdec_nv_flag;

typedef union {
	unsigned char value;
} improve_yield_vdec_error_flag;

typedef union {
	unsigned char value;
} improve_yield_vdec_reserved;

typedef struct {
    improve_yield_vdec_nv_flag nv_flag;
    improve_yield_vdec_error_flag error_flag;
	improve_yield_vdec_reserved reserved;
} improve_yield_vdec_info;

void vdec_init_iy_nvme(void);
#ifdef CONFIG_FACTORY_MODE
int32_t vdec_do_improve_yield(RtbIyParam *in_param, RtbIyParam *out_param);
int32_t vdec_register_iy_nvme(char *buffer, unsigned int *length);
#endif
#endif

void vdec_sh_init(void);
void vdec_sh_record_mcu_panic(void);
void vdec_sh_report_mcu_panic(void);
void vdec_sh_bsp(void);
void vdec_sh_pxp(void);
#endif
