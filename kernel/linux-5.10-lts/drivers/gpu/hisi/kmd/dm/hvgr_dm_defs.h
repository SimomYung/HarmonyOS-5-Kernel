/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DM_DEFS_H
#define HVGR_DM_DEFS_H

#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/shrinker.h>
#ifdef CONFIG_HVGR_DFX_DMD_REPORT
#include <linux/pm_wakeup.h>
#endif
#include "hvgr_dm_driver_adapt_defs.h"
#include "hvgr_ioctl_dev.h"
#include "hvgr_version.h"
#include "hvgr_regmap.h"
#include "hvgr_cq_defs.h"

struct hvgr_fcp_data;

struct hvgr_reg_info {
	void __iomem *reg_base[HVGR_REG_BASE_SIZE];
	uint32_t gpu_id;
	uint32_t gpu_version;
	uint32_t l2_features;
	uint32_t gpc_present;
	uint32_t gpc_cores;
	uint32_t mmu_features;
	uint32_t thread_features;
	uint32_t thread_max_workgroup_size;
};

struct hvgr_fcp_register_base {
	uint64_t fcp_reg_base;
	uint64_t fcp_itcm_offset;
	uint64_t fcp_dtcm_offset;
	uint64_t fcp_itcm_cq_offset;
	uint64_t fcp_dtcm0_cq_offset;
	uint64_t fcp_dtcm1_cq_offset;
#if hvgr_version_ge(HVGR_V500)
	uint64_t fcp_reg_mb0_offset;
	uint64_t fcp_reg_mb1_offset;
	uint64_t fcp_reg_mb2d_offset;
#elif hvgr_version_eq(HVGR_V350)
	uint64_t fcp_reg_mb0_offset;
	uint64_t fcp_reg_mb1_offset;
#else
	uint64_t fcp_reg_mb_offset;
#endif
	uint64_t fcp_reg_dma_offset;
	uint64_t fcp_reg_timer_offset;
	uint64_t fcp_reg_uart_offset;
};

struct hvgr_register_module_base {
	uint64_t gpu_reg_base;
	uint64_t job_reg_base;
	uint64_t mmu_stage1_reg_base;
	uint64_t cq_reg_base;
	struct hvgr_fcp_register_base fcp_base;
};

#define HVGR_CRG_REG_PARA_NUM 6u

struct hvgr_crg_resource {
	uint32_t crg_start;
	uint32_t crg_size;
	/* reset enable */
	uint32_t rst_addr;
	uint32_t rst_val;
	/* reset disable */
	uint32_t rstdis_addr;
	uint32_t rstdis_val;
	void __iomem *reg_base;
};

#define HVGR_CTRL_REG_PARA_NUM 2u
#define HVGR_SCS_REG_PARA_NUM 2u

struct hvgr_ctrl_resource {
	uint32_t reg_start;
	uint32_t reg_size;
	void __iomem *reg_base;
};

struct hvgr_scs_resource {
	uint32_t reg_start;
	uint32_t reg_size;
	void __iomem *reg_base;
};

#define IRQ_REGISTER_DONE 1

struct hvgr_irq {
	u32 irq;  /* irq num */
	int reg_done;
	/*
	 * irq flag include: trigger mode,resource type
	 * ex: IORESOURCE_IRQ | IRQF_TRIGGER_HIGH
	 */
	unsigned long flags;
};

#define MAX_DMD_LOG_LENTH  160

#ifdef CONFIG_HVGR_DFX_DMD_REPORT
#define MAX_DMD_CHECK_CNT 4u
#ifdef CONFIG_HVGR_VIRTUAL_GUEST
#define MAX_DMD_CHECK_INTERVAL 86400UL
#else
#define MAX_DMD_CHECK_INTERVAL 1209600UL
#endif
struct gpu_dmd_msg {
	unsigned int dmd_id;
	struct mutex dmd_lock;
	atomic_t dmd_flag;
	u64 dmd_check_ts[MAX_DMD_CHECK_CNT];
	u32 dmd_check_cnt;
	u32 dmd_fault_cnt;
#ifdef CONFIG_HVGR_DFX_SH
	u32 dmd_sh_flag;
	struct wakeup_source *sr_wakeup_lock;
	atomic_t dmd_gaf;
	bool enable_sh_act;
	bool enable_sh_check;
	bool enable_sh_gaf;
	bool enable_sh_freq;
	bool enable_sh_mode;
	atomic_t dmd_changemode_flag;
#endif
	char dmd_log[MAX_DMD_LOG_LENTH];
};
#endif

#ifdef CONFIG_DFX_DEBUG_FS
struct hvgr_report_data {
	struct hvgr_ctx *ctx[CQ_CHANNEL_NUM];
	uint64_t address;
	uint32_t fault_code;
	uint32_t channel_id;
};

struct hvgr_fault_report {
	struct hvgr_report_data fault_data;
	spinlock_t report_lock;
	struct workqueue_struct *wq;
	struct work_struct post_work;
};
#endif
struct common_mcu {
	struct workqueue_struct *lowpa_wq;
	struct work_struct lowpa_work;
	uint64_t alloc_page_size;
	struct mutex mcu_lock;
	struct mutex mcu_shrink_mutex;
	struct shrinker mcu_shrink;
};

struct hvgr_dm_dev {
	struct hvgr_irq irqs[GPU_IRQ_CNT];
	struct hvgr_register_module_base reg_base;
	/* Register access data struct. */
	struct hvgr_reg_info dev_reg;
	struct hvgr_crg_resource crg_rst;
	struct hvgr_ctrl_resource ctrl_reg;
	struct hvgr_scs_resource scs_reg;
	/* fcp data struct */
	struct hvgr_fcp_data *fcp_data;
	struct list_head ctx_list;
	struct mutex ctx_list_mutex;
	struct list_head swapped_out_list;
	struct mutex swapped_out_list_mutex;
	struct list_head switch_fs_list;
	struct mutex switch_fs_mutex;
	u64 uio_reg_start;
#ifdef CONFIG_HVGR_DFX_DMD_REPORT
	struct gpu_dmd_msg *dmd_msg;
#endif
#ifdef CONFIG_HVGR_VIRTUAL_GUEST
	int report_irq;
	struct workqueue_struct *ecc_wq;
	struct work_struct ecc_work;
#endif
#ifdef CONFIG_DFX_DEBUG_FS
	struct hvgr_fault_report fault_report_data;
#endif
#if defined(HVGR_FACTORY_MODE) && HVGR_FACTORY_MODE != 0
	struct dentry *dm_dir;
	bool dm_bugon_enable;
	bool dm_socvolt_enable;
	atomic_t socvolt_check_cnt;
	atomic_t hebc_dmd_cnt;
#endif
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	struct mutex mcu_2d_proc_mutex;
#endif
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	struct common_mcu mcu_data;
#endif
};

#ifdef CONFIG_HVGR_DFX_SH
#define HVGR_DMD_NV_DATA_SIZE 52

struct hvgr_dmd_nv_data {
	union {
		uint8_t value;
		struct {
			uint8_t en        : 1;
			uint8_t freq_mark : 1;
			uint8_t gaf_mark  : 1;
			uint8_t check     : 1;
			uint8_t version   : 4;
		} bit;
	} info;

	uint8_t buf[HVGR_DMD_NV_DATA_SIZE];
};

union hvgr_dmd_nv_data_gpumode {
	uint8_t value;
	struct {
		uint8_t gpu_mode  : 7;
		uint8_t en        : 1;
	} bit;
};

union hvgr_dmd_nv_sh_data {
	uint8_t value;
	struct {
		uint8_t reserved  : 7;
		uint8_t en        : 1;
	} bit;
};
#endif

struct hvgr_last_time_record {
	uint64_t buck_on;
	uint64_t buck_off;
	uint64_t crg_reset;
	uint64_t gpu_idle;
	uint64_t gpu_running;
};

#endif
