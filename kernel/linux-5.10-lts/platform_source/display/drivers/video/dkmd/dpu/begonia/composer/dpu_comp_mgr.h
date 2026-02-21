/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef COMPOSER_MGR_H
#define COMPOSER_MGR_H

#include <linux/kthread.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/of_reserved_mem.h>
#include <linux/hrtimer.h>
#include <linux/spinlock.h>
#include <dpu/soc_dpu_define.h>
#include <securec.h>
#include "dkmd_comp.h"
#include "online/dpu_comp_online.h"
#include "offline/dpu_comp_offline.h"
#include "present/dpu_comp_present.h"
#include "peri/dkmd_peri.h"
#include "aod/dpu_sh_aod.h"
#include "bl/dpu_comp_bl.h"
#include "esd/dpu_comp_esd.h"
#include "lowtemp/dpu_comp_low_temp_handler.h"
#include "fusa/dpu_comp_fusa_handler.h"
#include "secure/dpu_comp_tui.h"
#include "dkmd_lcd_interface.h"
#include "effect/alsc/dpu_effect_alsc.h"
#include "effect/hiace/dpu_effect_hiace.h"
#include "effect/hdr/dpu_effect_hdr.h"
#include "dkmd_aod_itf.h"

#define DTS_PATH_LOGO_BUFFER "/reserved-memory/logo-buffer"
#define DTS_PATH_CMDLIST_BUFFER	"/reserved-memory/cmdlist-buffer"
/* if continuously clear succ in COMP_CLEAR_FAIL_NUM, which means some error cannot be cleared by
 * chip clear procedure, we need reset hardware
 */
#define COMP_CLEAR_CONTINUOUS_NUM 3
#define CHIP_TIMEOUT_FRAME_COUNT 3
#define COMP_TIMEOUT_FRAME_COUNT ((COMP_CLEAR_CONTINUOUS_NUM - 1) * (CHIP_TIMEOUT_FRAME_COUNT))
#define COMP_STATUS_COUNT 2 // (8 * DEVICE_COMP_MAX_COUNT) / 64

typedef union {
	volatile uint64_t status[COMP_STATUS_COUNT];
	struct {
		uint8_t value[DEVICE_COMP_MAX_COUNT];
	} refcount;
} dpu_comp_status_t;

enum DPU_POWER_ON_STAGE {
	DPU_POWER_ON_NONE = 0,
	DPU_POWER_ON_STAGE1,
};

enum DPU_ACTION_TYPE {
	DPU_POWER_ON = 0,
	DPU_POWER_OFF,
	DPU_CHANGE_ACT_DISPLAY,
};

enum INIT_PANEL_DISPLAY_STATUS {
	ONE_PANEL_DISPLAY = 0,
	MULTI_PANEL_DISPLAY = 1
};

typedef struct {
	uint64_t vsync_count;
	uint32_t frame_count;
} cursor_stats_t;

typedef struct {
	int32_t x;
	int32_t y;
	bool update;
	spinlock_t lock;
} cursor_coord_t;

#ifndef round1
#define round1(x, y)  ((x) / (y) + ((x) % (y)  ? 1 : 0))
#endif

struct dpu_composer;
struct composer_manager {
	/* private data */
	struct platform_device *device;

	struct device_node *parent_node;

	/* Base address register, power supply, clock is public */
	char __iomem *dpu_base;
	char __iomem *actrl_base;
	char __iomem *media1_ctrl_base;
	char __iomem *pctrl_base;

	struct regulator *disp_ch1subsys_regulator;
	struct regulator *dsssubsys_regulator;
	struct regulator *vivobus_regulator;
	struct regulator *media1_subsys_regulator;
	struct regulator *regulator_smmu_tcu;

	struct regulator *dsisubsys_regulator;
	struct regulator *dpsubsys_regulator;
	struct regulator *dss_lite1_regulator;

	struct dpu_composer *dpu_comps[DEVICE_COMP_MAX_COUNT];

	struct regulator *vivobus_autodiv_regulator;

	/* For exclusive display device power reference count */
	dpu_comp_status_t power_status;
	struct semaphore power_sem;
	struct list_head isr_list;
	uint32_t hardware_reseted;

	int32_t mdp_irq;
	struct ukmd_isr mdp_isr_ctrl;

	int32_t sdp_irq;
	struct ukmd_isr sdp_isr_ctrl;

	int32_t tbu_err_detect_irq;
	struct ukmd_isr tbu_err_detect_isr_ctrl;

	int32_t tbu_err_multpl_irq;
	struct ukmd_isr tbu_err_multpl_isr_ctrl;
#ifdef CONFIG_DKMD_DEBUG_ENABLE
	int32_t dvfs_irq;
	struct ukmd_isr dvfs_isr_ctrl;
#endif

	int32_t m1_qic_irq;
	struct ukmd_isr m1_qic_isr_ctrl;

	struct mutex tbu_sr_lock;

	struct mutex idle_lock;
	bool idle_enable;
	uint32_t idle_func_flag;
	dpu_comp_status_t active_status;

	spinlock_t dimming_spin_lock;
	dpu_comp_status_t dimming_status;

	uint32_t effect_ctrl_flag;

	bool aod_enable;
	uint32_t power_on_stage;
	uint32_t init_panel_display_status;
	bool is_power_restarting; // when power restarting no need start aod

	/* link initialize cmdlist id */
	uint32_t init_scene_cmdlist;

	/* Receive each scene registration */
	struct composer_scene *scene[DPU_SCENE_OFFLINE_2 + 1];

	/* Regist lcd ops for ioctl */
	struct product_display_ops *dpu_lcdkit_ops;
};

/**
 * @brief Each device has its own composer's data structures, interrupt handling,
 * realization of vsync, timeline process is not the same;
 * so, this data structure need to dynamic allocation, and at the same time keep
 * saving composer manager pointer and link next connector pointer.
 *
 */
struct dpu_composer {
	/* composer ops_handle function pointer */
	struct composer comp;

	/* record every time processing data */
	void *present_data;

	/* pointer for connector which will be used for composer */
	struct dkmd_connector_info *conn_info;

	/* save composer manager pointer */
	struct composer_manager *comp_mgr;

	/* public each scene kernel processing threads */
	struct kthread_worker handle_worker;
	struct task_struct *handle_thread;

	struct ukmd_isr isr_ctrl;
	struct ukmd_attr attrs;

	void *hdr_ctrl;
	struct dpu_alsc *alsc;

	struct kthread_work wbuf_work;

	// underflow/timeout clear fail, need reset hardware
	uint32_t clear_succ_frame_index_queue[COMP_CLEAR_CONTINUOUS_NUM - 1];

	// hiace
	struct dkmd_hiace_ctrl *hiace_ctrl;
	struct kthread_work hiace_work;
	struct kthread_worker effect_worker;
	struct task_struct *effect_thread;

	// aod
	struct sh_aod sh_aod_info;
	struct dpu_bl_ctrl bl_ctrl;
	// esd
	struct dpu_esd_ctrl esd_ctrl;
	// low temp
	struct dpu_low_temp_ctrl low_temp_ctrl;
	// tui
	struct dpu_secure secure_ctrl;
	// fusa
	struct dpu_fusa_ctrl fusa_ctrl;
	// poweric flag
	bool pmic_abnormal_flag;

	// used for clear logo buffer
	bool dpu_boot_complete;

	/* comp idle params */
	uint32_t comp_idle_flag;
	uint32_t comp_idle_enable;
	int32_t comp_idle_expire_count;

	/* debug dvfs freq dump timer */
	struct hrtimer dkmd_dvfs_dump_hrtimer;

	/* debug dvfs stat */
	uint32_t curr_active_level;

	// used for cur scene
	int32_t comp_scene_id;

	/* vsync_count init as 0 and then mono increase */
	uint64_t vsync_count;
	/* hardware cursor stats */
	cursor_stats_t cursor_stats;
	/* hardware cursor position */
	cursor_coord_t cursor_coord;

	/* indicates whether dfr related listeners have been registered */
	bool has_dfr_related_listener_registered;
	struct dpu_self_healing_ctrl self_healing_ctrl;
	int32_t (*overlay)(struct dpu_composer *dpu_comp, struct disp_frame *frame);
	int32_t (*create_fence)(struct dpu_composer *dpu_comp, struct disp_present_fence *fence);
	int32_t (*set_safe_frm_rate)(struct dpu_composer *dpu_comp, uint32_t safe_frm_rate);
	int32_t (*release_fence)(struct dpu_composer *dpu_comp, int32_t fd);
	void (*vsync_deinit)(struct dpu_composer *dpu_comp, struct comp_online_present *present);
	void (*timeline_deinit)(struct dpu_composer *dpu_comp, struct comp_online_present *present);
};

static inline bool dpu_comp_status_is_disable(const dpu_comp_status_t* comp_status)
{
	uint64_t res = 0;
	unsigned int index;
	dpu_check_and_return((!comp_status), false, err, "comp_status is NULL");

	for (index = 0; index < COMP_STATUS_COUNT; index++)
		res |= comp_status->status[index];

	return res == 0;
}

static inline uint32_t dpu_comp_power_on_count(const dpu_comp_status_t* comp_status)
{
	uint32_t cnt = 0;
	unsigned int index;
	dpu_check_and_return((!comp_status), 0, err, "comp_status is NULL");

	for (index = 0; index < DEVICE_COMP_MAX_COUNT; index++)
		if (comp_status->refcount.value[index] > 0)
			cnt++;

	return cnt;
}

static inline bool dpu_comp_status_default_setup(dpu_comp_status_t* comp_status)
{
	unsigned int index;
	dpu_check_and_return((!comp_status), false, err, "comp_status is NULL");

	for (index = 0; index < COMP_STATUS_COUNT; index++)
		comp_status->status[index] = 0;
	return true;
}

static inline void dpu_comp_status_debug(const dpu_comp_status_t* comp_status)
{
	unsigned int status_index;
	for (status_index = 0; status_index < COMP_STATUS_COUNT; status_index++)
		dpu_pr_debug("dpu_comp_status_t%u=0x%llx ", status_index, comp_status->status[status_index]);
}

static inline void dpu_comp_status_info(const dpu_comp_status_t* comp_status)
{
	unsigned int status_index;
	for (status_index = 0; status_index < COMP_STATUS_COUNT; status_index++)
		dpu_pr_info("dpu_comp_status_t%u=0x%llx ", status_index, comp_status->status[status_index]);
}

static inline void dpu_comp_status_warn(const dpu_comp_status_t* comp_status)
{
	unsigned int status_index;
	for (status_index = 0; status_index < COMP_STATUS_COUNT; status_index++)
		dpu_pr_warn("dpu_comp_status_t%u=0x%llx ", status_index, comp_status->status[status_index]);
}

static inline struct dpu_composer *to_dpu_composer(struct composer *comp)
{
	return container_of(comp, struct dpu_composer, comp);
}

static inline struct ukmd_timeline *get_online_timeline(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct comp_online_present *present = (struct comp_online_present *)dpu_comp->present_data;
	return &(present->timeline);
}

static inline void composer_mgr_enable_dimming(struct composer_manager *comp_mgr, uint32_t comp_idx)
{
	unsigned long flags = 0;

	spin_lock_irqsave(&comp_mgr->dimming_spin_lock, flags);
	comp_mgr->dimming_status.refcount.value[comp_idx] = 1;
	spin_unlock_irqrestore(&comp_mgr->dimming_spin_lock, flags);
}

static inline void composer_mgr_disable_dimming(struct composer_manager *comp_mgr, uint32_t comp_idx)
{
	unsigned long flags = 0;

	if (unlikely(comp_idx >= DEVICE_COMP_MAX_COUNT)) {
		dpu_pr_err("comp_idx=%u is out of range", comp_idx);
		return;
	}

	spin_lock_irqsave(&comp_mgr->dimming_spin_lock, flags);
	comp_mgr->dimming_status.refcount.value[comp_idx] = 0;
	spin_unlock_irqrestore(&comp_mgr->dimming_spin_lock, flags);
}

static inline bool composer_mgr_dimming_status_is_off(struct composer_manager *comp_mgr)
{
	bool ret = 0;
	unsigned long flags = 0;

	spin_lock_irqsave(&comp_mgr->dimming_spin_lock, flags);
	ret = dpu_comp_status_is_disable(&comp_mgr->dimming_status);
	spin_unlock_irqrestore(&comp_mgr->dimming_spin_lock, flags);

	return ret;
}

static inline void trace_buffer_num(uint32_t buffers)
{
	char buf[128];
	(void)snprintf_s(buf, sizeof(buf), sizeof(buf) - 1, "C|%d|%s-%d|%lu\n", 9999, "buffer num", 9999, buffers);
	trace_printk(buf);
}

static inline bool is_multi_device_active(struct composer_manager *comp_mgr)
{
	return ((comp_mgr->active_status.refcount.value[DEVICE_COMP_DP_ID] != 0) || 
		(comp_mgr->active_status.refcount.value[DEVICE_COMP_HDMI_ID] != 0) ||
		(comp_mgr->active_status.refcount.value[DEVICE_COMP_VIRTUAL_ID] != 0));
}

/*
 * async present will retain present task at queue when suspend,
 * we can't resync timeline, cause the fence and cmdlist would be released too early.
 * such as:
 * 1, before suspend: timeline: 10, (task1:fence pt 11), (task2:fence pt 12)
 * 2, suspend: timeline: 12
 * 3, resume: present task1, lock cmdlist pt: 11 + 1
 * 4, vsync irq: timeline : 13, release task1 cmdlist, then underflow
 */
static inline void composer_manager_timeline_resync(struct dpu_composer *dpu_comp, int32_t off_mode)
{
	switch (off_mode) {
	case COMPOSER_OFF_MODE_BLANK:
		composer_present_timeline_resync(dpu_comp);
		break;
	case COMPOSER_OFF_MODE_SUSPEND:
		if (dpu_comp->comp.base.enable_async_online == 0)
			composer_present_timeline_resync(dpu_comp);
		break;
	default:
		dpu_pr_err("not support off_mode %d", off_mode);
		return;
	}
}

static inline void dpu_pr_enter_log(struct dpu_composer *dpu_comp, int32_t off_mode)
{
	if (is_offline_panel(&dpu_comp->conn_info->base)) {
		dpu_comp_status_debug(&dpu_comp->comp_mgr->power_status);
		dpu_pr_debug("index:%u enter, off_mode = %d", dpu_comp->comp.index, off_mode);
	} else {
		dpu_comp_status_warn(&dpu_comp->comp_mgr->power_status);
		dpu_pr_warn("index:%u enter, off_mode = %d", dpu_comp->comp.index, off_mode);
	}
}

static inline void dpu_pr_exit_log(struct dpu_composer *dpu_comp)
{
	if (is_offline_panel(&dpu_comp->conn_info->base)) {
		dpu_comp_status_debug(&dpu_comp->comp_mgr->power_status);
		dpu_pr_debug("dpu_pr_exit_log exit");
	} else {
		dpu_comp_status_info(&dpu_comp->comp_mgr->power_status);
		dpu_pr_info("dpu_pr_exit_log exit");
	}
}

static inline bool composer_manager_get_scene_switch(struct dpu_composer *dpu_comp)
{
	return dpu_comp->conn_info->dsc_switch_enable;
}

static inline bool is_fake_power_off(struct dpu_composer *dpu_comp)
{
	dpu_pr_info("aod:%d is_fake_power_off:%d", dpu_aod_get_lcd_always_on(), dpu_comp->comp.is_fake_power_off);
	return (dpu_comp->comp.is_fake_power_off || dpu_aod_get_lcd_always_on());
}

struct dpu_bl_ctrl *get_bl_ctrl_from_device(struct device *dev);
int32_t composer_manager_set_fastboot(struct composer *comp);
int32_t composer_manager_on(struct composer *comp);
int32_t composer_manager_on_no_lock(struct composer *comp);
int32_t composer_wait_for_blank(struct composer *comp);
void composer_restore_fast_unblank_status(struct composer *comp);
int32_t composer_blank_peri_handle(struct composer *comp, int32_t blank_mode);
int32_t composer_manager_off(struct composer *comp, int32_t off_mode);
int32_t composer_manager_off_no_lock(struct composer *comp, int32_t off_mode);
int32_t composer_manager_present(struct composer *comp, void *in_frame);
void composer_manager_release(struct composer *comp);

void composer_manager_power_restart_no_lock(struct dpu_composer *dpu_comp);
void composer_manager_reset_hardware(struct dpu_composer *dpu_comp);
void composer_dpu_power_on_sub(struct dpu_composer *dpu_comp);
void composer_dpu_power_off_sub(struct dpu_composer *dpu_comp);
bool composer_dpu_free_logo_buffer(struct dpu_composer *dpu_comp);
bool composer_dpu_free_cmdlist_buffer(struct dpu_composer *dpu_comp);
bool composer_check_power_status(struct dpu_composer *dpu_comp);

int32_t media_regulator_enable(struct composer_manager *comp_mgr);
int32_t media_regulator_disable(struct composer_manager *comp_mgr);
void vivobus_autodiv_regulator_enable(struct composer_manager *comp_mgr);
void vivobus_autodiv_regulator_disable(struct composer_manager *comp_mgr);
void composer_dpu_power_off(struct composer_manager *comp_mgr, struct composer *comp);
void composer_dpu_power_on(struct composer_manager *comp_mgr, struct composer *comp);

void composer_manager_power_down(struct dpu_composer *dpu_comp);
void composer_manager_power_up(struct dpu_composer *dpu_comp);

bool composer_manager_v740_is_bypass_by_pg(void);
bool composer_manager_v741_is_bypass_by_pg(void);
struct composer_manager *get_dpu_composer_manager(void);
void composer_dpu_pipesw_disconnect(struct dpu_composer *dpu_comp);

void composer_dvfs_dump_timer_init(struct dpu_composer *dpu_comp);
void composer_cancel_dvfs_dump_timer(struct dpu_composer *dpu_comp);

int32_t composer_manager_update_hardware_cursor_coord(struct composer *comp, int32_t scene_id, int32_t x, int32_t y);
int32_t composer_manager_enable_hardware_cursor_stats(struct composer *comp, int32_t scene_id, bool enable);
int32_t composer_manager_get_hardware_cursor_stats(
    struct composer *comp, int32_t scene_id, uint32_t *frame_count, uint32_t *vsync_count);
bool is_video_panel_type(void);
#ifdef CONFIG_LEDS_CLASS
void bl_lcd_set_backlight(struct led_classdev *led_cdev,
	enum led_brightness value);
#endif

#endif
