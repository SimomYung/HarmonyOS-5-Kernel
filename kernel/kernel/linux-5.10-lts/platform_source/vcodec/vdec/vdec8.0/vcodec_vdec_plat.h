/*
 * vcodec_vdec_plat.h
 *
 * This is for vdec platform
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#ifndef VCODEC_VDEC_PLAT_H
#define VCODEC_VDEC_PLAT_H
#include "dbg.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"

#define vdec_check_ret(cond, ret) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "assert condition %s not match\n", #cond); \
			return ret; \
		} \
	} while (0)

#define vdec_init_mutex(lock) \
	do { \
		mutex_init(lock); \
	} while (0)

#define vdec_mutex_lock(lock) \
	do { \
		mutex_lock(lock); \
	} while (0)

#define vdec_mutex_lock_interruptible(lock) \
	({int mutex_lock_ret; \
		do { \
			mutex_lock_ret = mutex_lock_interruptible(lock); \
		} while (0); \
	mutex_lock_ret;})

#define vdec_mutex_unlock(lock) \
	do { \
		mutex_unlock(lock); \
	} while (0)

typedef struct {
	uint32_t target_value;
	uint32_t expect_value;
	uint32_t offset;
} bus_idle_cfg;

typedef struct {
	bool (*vcodec_vdec_device_bypass)(void);
	bool (*smmu_need_cfg_max_tok_trans)(void);
	void (*config_bus_idle)(bus_idle_cfg* bus_idle, bool set);
	void (*pxp_config_bus_idle)(bus_idle_cfg* bus_idle, bool set);
	void (*pxp_reset_config_subctrl_rst)(U_VDEC_RST* reg_rst, bool enable);
	void (*pxp_reset_config_subctrl_crg)(U_VDEC_CRG* reg_crg, bool enable);
	void (*pxp_powerup_softrst_bit)(U_VDEC_RST* reg_rst);
	void (*pxp_powerup_config_syscache)(uint8_t *reg_base_addr,
		uint32_t multi_core_enable, UADDR syscache_start_addr, uint32_t syscache_len);
	uint32_t (*pxp_powerup_get_syscache_size)(void);
	void (*set_acpu_irq_mask)(S_VDE_SUB_CTRL_NS_REGS_TYPE *subctrl_vir_addr);
	bool (*is_open_qic_core_cfg_intr)(void);
	uint32_t (*get_qic_core_data_start_addr)(void);
	void (*set_qic_irq_mask)(uint8_t *reg_base_addr);
	bool (*bsp_need_sw_buffer)(void);
	void (*smmu_cfg_mcu_ssid)(uint32_t reg_base_offset);
	bool (*is_need_power_up_vdec_cal)(void);
	void (*get_media1_crg)(uint8_t *media1_crg_vir_addr);
} vdec_plat_ops;

int32_t vdec_device_probe(void *plt);

vdec_plat_ops *get_vdec_plat_ops(void);
#endif
