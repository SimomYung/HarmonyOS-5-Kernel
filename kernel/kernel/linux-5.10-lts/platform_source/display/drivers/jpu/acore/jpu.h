/* Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 *
 * jpeg jpu
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

#ifndef JPU_H
#define JPU_H

#include "os_adapt.h"
#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
#include <platform_include/isp/linux/hipp_common.h>
#else
#include <platform_include/isp/linux/hipp.h>
#endif
#include "jpgdec_platform.h"
#include "jpu_common.h"

#define CONFIG_FB_DEBUG_USED

enum jpeg_dec_platform {
	DSS_V400 = 1,
	DSS_V500,
	DSS_V501,
	DSS_V510,
	DSS_V510_CS,
	DSS_V600,
	DSS_V700,
	UNSUPPORT_PLATFORM,
};

enum jpeg_reg_base {
	JPEG_DECODER_REG = 0,
	JPEG_TOP_REG,
	JPEG_CVDR_REG,
	JPEG_SMMU_REG,
	JPEG_MEDIA1_REG,
	JPEG_PERI_REG,
	JPEG_PMCTRL_REG,
	JPEG_SCTRL_REG,
	JPEG_SECADAPT_REG,
};

struct jpu_data_type {
	uint32_t index;
	uint32_t ref_cnt;

	struct mutex jpu_mutex;

	struct platform_device *pdev;
	uint32_t jpu_major;
	struct class *jpu_class;
	struct device *jpu_dev;

	char __iomem *jpu_cvdr_base;
	char __iomem *jpu_top_base;
	char __iomem *jpu_dec_base;
	char __iomem *jpu_smmu_base;
	char __iomem *media1_crg_base;
	char __iomem *peri_crg_base;
	char __iomem *pmctrl_base;
	char __iomem *sctrl_base;
#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	char __iomem *secadapt_base;
#endif

	uint32_t jpu_irq_num[JPGDEC_IRQ_NUM];
	uint32_t jpu_dec_done_flag;
	uint32_t fpga_flag;
	wait_queue_head_t jpu_dec_done_wq;

	struct regulator *jpu_regulator;
	struct regulator *media1_regulator;

	const char *jpg_func_clk_name;
	struct clk *jpg_func_clk;

	const char *jpg_platform_name;
	enum jpeg_dec_platform jpu_support_platform;

	struct sg_table *lb_sg_table;
	uint32_t lb_buf_base;

	uint32_t lb_addr; /* line buffer addr */
	uint32_t power_on;
	struct semaphore blank_sem;

	jpu_dec_reg_t jpu_dec_reg_default;
	jpu_dec_reg_t jpu_dec_reg;
	bool jpu_res_initialized;

	struct jpu_data_t jpu_req;

#if defined(CONFIG_DPU_FB_V501) || defined(CONFIG_DPU_FB_V510) || \
	defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	struct hipp_common_s *jpgd_drv;
#endif

#if defined(CONFIG_DPU_FB_V600) || defined(CONFIG_DKMD_DPU_VERSION)
	jpgd_secadapt_prop_t jpgd_secadapt_prop;
#endif
};

struct jpu_dma_buf {
	void *in_buf;
	void *out_buf;
};

extern int g_debug_jpu_dec;
extern int g_debug_jpu_dec_job_timediff;
extern struct wakeup_source *g_ws;

int32_t jpu_job_decode(struct jpu_data_type *jpu_device,
	const void __user *argp);

bool jpu_device_need_bypass(void);
#endif /* JPU_H */
