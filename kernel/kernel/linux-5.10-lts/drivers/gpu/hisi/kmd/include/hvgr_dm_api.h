/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DM_API_H
#define HVGR_DM_API_H

#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/kref.h>

#include "hvgr_ioctl.h"
#include "apicmd/hvgr_ioctl_dev.h"
#include "hvgr_dm_defs.h"
#include "hvgr_regmap.h"

struct hvgr_ctx;
struct hvgr_device;

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
#define MALEOON_2D_MCU_ENABLE       1
#define MALEOON_2D_MCU_DISABLE      1

/* reserve 128k itcm 64k + dtcm 32k + stack32k */
#define MALEOON_2D_MCU_SIZE        0x20000
#endif

#define INVALID_DMD_ID             0u

typedef enum hvgr_dmd_id {
	DMD_INVALID = 0,
	DMD_JOB_FAIL,
	DMD_JOB_HANG,
	DMD_FCP_LOAD_FAIL,
	DMD_BUS_FAULT,
	DMD_PAGE_FAULT,
	DMD_PW_ONOFF_FAIL,
	DMD_HARD_RESET,
	DMD_SH_NV_MARK,
	DMD_SH_HANG_NV_MARK,
	DMD_ENTRY_TIMEOUT
} hvgr_gpu_fault_id;

#define gpu_control_reg(gdev, offset) \
	(((u64)(uintptr_t)((gdev)->dm_dev.dev_reg.reg_base[0])) + (u64)(offset))

#define mmu_control_reg(gdev, offset) \
	(((u64)(uintptr_t)((gdev)->dm_dev.dev_reg.reg_base[1])) + (u64)(offset))

#define misc_control_reg(gdev, offset) \
	(((u64)(uintptr_t)((gdev)->dm_dev.dev_reg.reg_base[2])) - HVGR_REG_BASE2_LEN + (u64)(offset))

#define job_control_reg(gdev, offset) \
	(((u64)(uintptr_t)((gdev)->dm_dev.dev_reg.reg_base[2])) + JOB_CONTROL_BASE - HVGR_REG_BASE2_LEN + \
	(u64)(offset))

#define cq_control_reg(gdev, offset) \
	(((u64)(uintptr_t)((gdev)->dm_dev.dev_reg.reg_base[2])) + CQ_REG_BASE - HVGR_REG_BASE2_LEN + \
	(u64)(offset))

#define fcp_control_reg(gdev, offset) \
	((uint64_t)((gdev)->dm_dev.dev_reg.reg_base[2]) + FCP_2D_MB_REG_BASE - HVGR_REG_BASE2_LEN + \
	(uint64_t)(offset))

/* Used for datan */
#define datan_mmu_control_reg(gdev, offset) \
	((uint64_t)((gdev)->dm_dev.reg_base.mmu_stage1_reg_base) + (uint64_t)(offset))

#define datan_cq_control_reg(gdev, offset) \
	((uint64_t)((gdev)->dm_dev.reg_base.cq_reg_base) + (uint64_t)(offset))

#define u64_to_ptr_user(x) (void __user *)((uintptr_t)(x))

#define hvgr_ioctl_handle(_ctx, _cmd, _func) \
	do { \
		ret = _func((_ctx)); \
		goto exit; \
	} while (0)

#define hvgr_ioctl_handle_w(_ctx, _cmd, _func, _arg, _type) \
	do { \
		_type _para = {0}; \
		long _ret; \
		unsigned long cp_ret; \
		void __user *_upara = u64_to_ptr_user(_arg); \
		if (_upara == NULL) { \
			ret = -EINVAL; \
			goto exit; \
		} \
		cp_ret = \
			copy_from_user(&_para, _upara, sizeof(_para)); \
		if (cp_ret != 0U) { \
			ret = -EFAULT; \
			goto exit; \
		} \
		_ret = _func((_ctx), &_para); \
		ret = _ret; \
		goto exit; \
	} while (0)

#define hvgr_ioctl_handle_r(_ctx, _cmd, _func, _arg, _type) \
	do { \
		_type _para = {0}; \
		unsigned long cp_ret; \
		long _ret; \
		void __user *_upara = u64_to_ptr_user(_arg); \
		if (_upara == NULL)  { \
			ret = -EINVAL; \
			goto exit; \
		} \
		_ret = _func((_ctx), &_para); \
		if (_ret != 0) { \
			ret = _ret; \
			goto exit; \
		} \
		cp_ret = \
			copy_to_user(_upara, &_para, sizeof(_para)); \
		if (cp_ret != 0U) { \
			ret = -EFAULT; \
			goto exit; \
		} \
		ret = _ret; \
		goto exit; \
	} while (0)

#define hvgr_ioctl_handle_wr(_ctx, _cmd, _func, _arg, _type) \
	do { \
		_type _para = {0}; \
		long _ret; \
		unsigned long cp_ret; \
		void __user *_upara = u64_to_ptr_user(_arg); \
		if (_upara == NULL) { \
			ret = -EINVAL; \
			goto exit; \
		} \
		cp_ret = \
			copy_from_user(&_para, _upara, sizeof(_para)); \
		if (cp_ret != 0U) { \
			ret = -EFAULT; \
			goto exit; \
		} \
		_ret = _func((_ctx), &_para); \
		if (_ret != 0) { \
			ret = _ret; \
			goto exit; \
		} \
		cp_ret = copy_to_user(_upara, &_para, sizeof(_para)); \
		if (cp_ret != 0U) { \
			ret = -EFAULT; \
			goto exit; \
		} \
		ret = _ret; \
		goto exit; \
	} while (0)

/*
 * hvgr_ioctl_get_driver_info - Get driver info
 *
 * @ctx:     The hvgr context info
 * @para:    The driver info
 *
 * Return: 0 on success or error code
 */
long hvgr_ioctl_get_driver_info(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_version * const para);

/*
 * hvgr_ioctl_get_gpu_info - Get driver gpu info
 *
 * @ctx:     The hvgr context info
 * @para:    The driver gpu info
 *
 * Return: 0 on success or error code
 */
long hvgr_ioctl_get_gpu_info(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_gpu_info * const para);

/*
 * hvgr_ioctl_cfg_driver - Configure ctx resource and only this ioctl complete
 * then other ioctl could set.
 *
 * @ctx:     The hvgr context info
 * @para:    The driver ctx cfg info
 *
 * Return: 0 on success or error code
 */
long hvgr_ioctl_cfg_driver(struct hvgr_ctx * const ctx, hvgr_ioctl_para_drv_cfg * const para);

/*
 * hvgr_ioctl_decfg_driver - Deconfigure ctx resource and make sure release release ctx success.
 *
 * @ctx:     The hvgr context info
 * @para:    The driver ctx cfg info
 *
 * Return: 0 on success or error code
 */
long hvgr_ioctl_decfg_driver(struct hvgr_ctx * const ctx, hvgr_ioctl_para_drv_cfg * const para);

/*
 * hvgr_ioctl_dm - device manage ioctl
 *
 * @ctx:     The hvgr context info
 * @cmd:     cmdcode
 * @arg:     para from user mode
 *
 * Copy arg from user mode to kerenl mode and invoke function by cmd.
 * Return: 0 on success or error code
 */
long hvgr_ioctl_dm(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg);

/*
 * hvgr_crg_reset - Reset gpu region from soc register.
 *
 * @gdev:    The hvgr device
 *
 * Return: 0 on success or error code
 */
int hvgr_crg_reset(struct hvgr_device *gdev);

/*
 * hvgr_is_fpga - Gpu is fpga or soc.
 *
 * @gdev:    The hvgr device
 *
 */
bool hvgr_is_fpga(struct hvgr_device * const gdev);

/*
 * hvgr_rt_is_enable - rt is enable.
 *
 * @gdev:    The hvgr device
 *
 */
bool hvgr_rt_is_enable(struct hvgr_device * const gdev);

/*
 * hvgr_synchronize_irqs - Sync irqs before the gpu powered off.
 *
 * @gdev:    The hvgr device
 *
 */
void hvgr_synchronize_irqs(struct hvgr_device *gdev);

/*
 * hvgr_register_irq - Register interrupt to kernel.
 *
 * @gdev:    The hvgr device
 * @type:    irqs type likes gpu, cq, memory, fcp and so on.
 * @handler  irq handle function.
 *
 * Return: 0 on success or error code
 */
int hvgr_register_irq(struct hvgr_device *gdev, enum hvgr_irqs_type type, irq_handler_t handler);

/*
 * hvgr_read_reg - Read gpu register
 *
 * @gdev:    The hvgr device
 * @addr:    The gpu register address
 *
 * Return: value of gpu register
 */
uint32_t hvgr_read_reg(struct hvgr_device * const gdev, uint64_t addr);

/*
 * hvgr_write_reg - Write gpu register
 *
 * @gdev:    The hvgr device
 * @addr:    The gpu register address
 * @value:   The value of gpu register
 *
 */
void hvgr_write_reg(struct hvgr_device * const gdev, uint64_t addr, uint32_t value);

/*
 * hvgr_fast_pwr_write_reg - If process in the first entire booting phase, store register value
 * into ddr instead of set value directly. HW will loal those register bundled together.
 *
 * @gdev:    The hvgr device
 * @addr:    The gpu register address
 * @value:   The value of gpu register
 *
 */
void hvgr_fast_pwr_write_reg(struct hvgr_device * const gdev, uint64_t addr, uint32_t value);

/*
 * hvgr_get_device - Get hvgr_device pointer
 *
 *
 * Return: hvgr_device pointer
 */
struct hvgr_device *hvgr_get_device(void);

/*
 * hvgr_gpu_irq - gpu irq handler
 *
 * @irq:    The irq type
 * @data:   gdev
 *
 * Return: irq handle result
 */
irqreturn_t hvgr_gpu_irq(int irq, void *data);

/*
 * hvgr_device_probe - gpu device level init
 *
 * @gdev:    The hvgr device
 *
 * init gpu device level resource.
 *
 * Return: 0 on success or error code
 */
int hvgr_device_probe(struct hvgr_device *gdev);

/*
 * hvgr_device_remove - gpu device level uninit
 *
 * @gdev:    The hvgr device
 *
 * remove gpu device level resource.
 *
 * Return: 0 on success or error code
 */
int hvgr_device_remove(struct hvgr_device *gdev);

/*
 * hvgr_get_core_nums - Get gpc cores numbers
 *
 * @gdev:    The hvgr device
 *
 * Return: numbers of gpc cores
 */
uint32_t hvgr_get_core_nums(struct hvgr_device *gdev);

/*
 * hvgr_get_cores - Get GPC bit map
 *
 * @param gdev - The gdev object pointer.
 *
 * Return: GPC bit map.
 */
uint32_t hvgr_get_cores(struct hvgr_device *gdev);

/*
 * hvgr_init_control_baseaddr - get baseaddr from different hw version
 *
 * @gdev:    The hvgr device
 *
 * Return: 0 on success or error code
 */
int hvgr_init_control_baseaddr(struct hvgr_device * const gdev);

/*
 * hvgr_fcp_load_by_poll - Load fcp firmware by polling INT status
 *
 * @gdev:    The hvgr device
 *
 * Return: 0 on success or error code
 */
int hvgr_fcp_load_by_poll(struct hvgr_device *gdev);

/*
 * hvgr_disable_interrupts - Disable gpu interrupts
 *
 * @gdev:    The hvgr device
 *
 * mask off gpu interrupts and clear it.
 */
void hvgr_disable_interrupts(struct hvgr_device *gdev);

/*
 * hvgr_enable_interrupts - Enable gpu interrupts
 *
 * @gdev:    The hvgr device
 *
 * clear gpu interrupts and mask it.
 *
 */
void hvgr_enable_interrupts(struct hvgr_device *gdev);

/*
 * hvgr_dmd_msg_init - init dmd message
 *
 * @gdev:    The hvgr device
 *
 * Return: 0 on success or error code
 */
int hvgr_dmd_msg_init(struct hvgr_device *gdev);

/*
 * hvgr_dmd_msg_term - terminate dmd message
 *
 * @gdev:    The hvgr device
 */
void hvgr_dmd_msg_term(struct hvgr_device *gdev);

/*
 * hvgr_dmd_msg_set - set dmd message
 *
 * @gdev:    The hvgr device
 * @dmd_id:  The dmd id
 * @dmd_log: The dmd log
 *
 */
void hvgr_dmd_msg_set(struct hvgr_device *gdev,  enum hvgr_dmd_id dmd_id,
	const char *dmd_log);

/*
 * hvgr_dmd_msg_report - report dmd message
 *
 * @gdev:    The hvgr device
 */
void hvgr_dmd_msg_report(struct hvgr_device *gdev);

/*
 * hvgr_dmd_id_check - check weather dmd id is valid
 *
 * @gdev:    The hvgr device
 */
bool hvgr_dmd_id_check(struct hvgr_device *gdev);

/*
 * hvgr_dmd_msg_clear - Clear dmd msg after gpu reset done if dmdmsg is not null.
 *
 * @gdev:    The hvgr device
 */
void hvgr_dmd_msg_clear(struct hvgr_device *gdev);

void hvgr_dmd_sr_awake(struct hvgr_device *gdev);

void hvgr_dmd_sr_relax(struct hvgr_device *gdev);

/*
 * hvgr_get_ctx_compat - get ctx 32bit or 64bit
 *
 * @ctx:    The hvgr context info
 */
int hvgr_get_ctx_compat(struct hvgr_ctx *ctx);

/*
 * hvgr_get_main_process_name - get ctx tgid process name
 *
 * @ctx:    The hvgr context info
 */
char *hvgr_get_main_process_name(struct hvgr_ctx *ctx);

/*
 * hvgr_dm_get_soc_timestamp - get soc timestamp
 *
 * @gdev:    The hvgr device
 */
uint64_t hvgr_dm_get_soc_timestamp(struct hvgr_device * const gdev);

/*
 * hvgr_ctx_kref_release - hvgr_ctx
 *
 * @ctx_kref:    The hvgr_ctx ref count
 */
void hvgr_ctx_kref_release(struct kref *ctx_kref);

bool hvgr_is_fake_hang(struct hvgr_device * const gdev);

#if hvgr_version_ge(500)
bool hvgr_dm_is_2mcu(struct hvgr_device * const gdev);
#endif

bool hvgr_is_report_dmd(struct hvgr_device * const gdev);

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
long hvgr_dev_ioctl_set_uni_render(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_set_uni_render * const para);
#endif

/*
 * hvgr_fcp_load_protect_exit - Load fcp firmware when protect mode exit.
 *
 * @gdev:    The hvgr device
 *
 * Return: 0 on success or error code
 */
int hvgr_fcp_load_protect_exit(struct hvgr_device *gdev);

#ifdef CONFIG_HVGR_DFX_SH
void hvgr_dmd_set_sid(struct hvgr_ctx *ctx, uint32_t sid);

void hvgr_dmd_config_ctx_gaf(struct hvgr_ctx *ctx, uint32_t flag);

void hvgr_dmd_init_sh(struct hvgr_device *gdev);

int hvgr_dmd_nv_read(struct hvgr_device *gdev, struct hvgr_dmd_nv_data *data);

int hvgr_dmd_nv_write(struct hvgr_device *gdev, struct hvgr_dmd_nv_data *data);
#endif

#if defined(HVGR_VK_EXT_DEVICE_FAULT) && HVGR_VK_EXT_DEVICE_FAULT != 0
void hvgr_fault_info_fill(struct hvgr_ctx * const ctx, enum hvgr_fault_addr_type type,
	char *desc, uint64_t addr, uint64_t code, uint64_t data);

void hvgr_fault_info_fill_all_running_ctx(struct hvgr_device * const gdev,
	enum hvgr_fault_addr_type type, char *desc, uint64_t addr, uint64_t code, uint64_t data);

void hvgr_fault_info_fill_all_running_ctx_nolock(struct hvgr_device * const gdev,
	enum hvgr_fault_addr_type type, char *desc, uint64_t addr, uint64_t code, uint64_t data);
#endif

#endif
