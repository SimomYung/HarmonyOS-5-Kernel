/*
 * vcodec_vdec.h
 *
 * This is for vdec management
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

#ifndef VCODEC_VDEC
#define VCODEC_VDEC

#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/time64.h>
#include "drv_omxvdec.h"
#include "vcodec_types.h"
#include "vcodec_vdec_dbg.h"
#include "vcodec_vdec_memory.h"
#include "dev_mgr/dev_mgr.h"
#include "irq/vcodec_vdec_irq.h"
#include "dbg.h"
#include "vfmw.h"
#include "vdec_self_healing.h"

#ifdef PCIE_FPGA_VERIFY
#define VCODEC_ATTR_WEEK __attribute__((weak))
#else
#define VCODEC_ATTR_WEEK
#endif

#define VREG_OFS_DEC_STORE  0x408394
#define CHAN_REF_COUNT_MAX 8
struct mcu_log_info {
	void *log;
	uint32_t real_size;
	uint32_t buffer_size;
	time64_t time_stamp;
};

enum open_state {
	OPENING,
	OPENED_SUCCESS,
	OPENED_FAILED
};

typedef struct {
	uint32_t open_count;
	struct mutex vdec_mutex;
	struct mutex vdec_mutex_scd;
	struct mutex vdec_mutex_sec_scd;
	struct mutex vdec_pci_mutex;
	vcodec_bool device_locked;
	struct cdev cdev;
	struct device *class_dev;
	struct device *device;
	bool power_state; /* false: power off; true: power on */
	spinlock_t power_state_spin_lock;
	struct vdec_setting_info setting_info;
	void *work_ctx;
	void *async_open_node;
	void *root_ctx;
	uint8_t *reg_base_vir_addr;
	void __iomem *media1_ctrl_vir_addr; // MEDIA1_CTRL
	void __iomem *media1_crg_vir_addr;  // MEDIA1_CRG
	void __iomem *bisr_ctrl_vir_addr;   // BISR_CTRLMEDIA1
	struct vcodec_vdec_irq_ctx *irq_ctx;
	struct vcodec_vdec_memory_ctx *memory_ctx;
	struct completion mcu_alloc_done;
	struct completion mcu_report_event;
	struct completion async_open_done;
	enum open_state async_open_state;
	struct vcodec_dev *scd_dev;
	struct vcodec_dev *bsp_dev;
	struct vcodec_dev *pxp_dev;
	struct vcodec_dev *mcu_dev;
	struct mcu_log_info mcu_log;
	uint32_t vdec_chan_state[VFMW_CHAN_ID_MAX];
	bool is_panic_log;
	uint32_t qic_irq_cnt;
	vdec_sh sh;
} vdec_entry;

typedef struct {
	UADDR reg_reg_phy_base_addr;
	uint32_t reg_range;
} dev_reg_info;

struct vdec_chan_listnode {
	struct list_head head;
	int32_t chan_id;
	uint32_t chan_state;
	int32_t ref_num;
	struct completion chan_ref_num_equal_one;
};

struct vdec_chan_list {
	struct mutex lock;
	struct list_head chan_head;
};

typedef struct {
	uint64_t load;
	uint32_t base_clk_level;
	uint32_t is_release;
	uint32_t is_used;
	int32_t is_back_ground;
} vdec_load_param;

struct dec_file_handle {
	uint32_t instance_id; // kernel
	uint32_t is_used;
	uint32_t mcu_log_read_offset;
	struct vdec_fd_list fdlist;
	struct vdec_chan_list chanlist;
	vdec_load_param chanload[VFMW_CHAN_ID_MAX];
	vdec_entry *vdec_entry;
	spinlock_t global_reset_spin_lock;
	struct mutex chan_create_lock;
	bool hw_lock_flag;
	bool global_reset;
};

#define check_para_size_return(size, para_size, command) \
	do { \
		if ((size) != (para_size)) { \
			dprint(PRN_FATAL, "%s: prarameter_size is error\n", command); \
			return -EINVAL; \
		} \
	} while (0)

#define check_return(cond, else_print) \
	do { \
		if (!(cond)) { \
			dprint(PRN_FATAL, "%s\n", else_print); \
			return -EINVAL; \
		} \
	} while (0)

// 调试功能, 调用这个函数让代码停住, 方便查看寄存器等操作;
// 使用这个功能需要定义一个全局变量和设置全局变量值的函数, 通过ecall来修改全局变量的值,控制流程启停.
#define vdec_stop_here(flag, fmt, arg...) \
do { \
	while (flag) { \
		msleep(1000); \
		dprint(PRN_ALWS, fmt, ##arg); \
	} \
} while (0)

vdec_entry *vdec_get_entry(void);
struct platform_device *vdec_get_device(void);
void vdec_set_device(struct platform_device *dev);
int32_t vdec_tcu_connect(void);
void vdec_tcu_disconnect(void);

typedef int32_t(*fn_ioctl_handler)(struct file *file_handle, vdec_ioctl_msg *pmsg);
typedef struct {
	uint32_t cmd;
	fn_ioctl_handler handle;
} ioctl_command_node;

const ioctl_command_node *vcodec_vdec_get_ioctl_tbl(void);
int32_t vdec_ioctl_set_clk_rate(struct file *file_handle,
	vdec_ioctl_msg *pvdec_msg);
int32_t handle_lock_hw(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg);
int32_t handle_unlock_hw(struct file *file_handle,
	vdec_ioctl_msg *vdec_msg);
vdec_entry *vdec_get_entry_by_file_handle(struct file *file_handle);
int32_t vdec_dump_mcu_log_realtime(void *arg);
void vdec_reset_async(void);
void vdec_chan_state_clear(void);
bool vdec_is_async_open_success(vdec_entry* vdec);
bool vdec_chan_list_get(struct vdec_chan_list *list, uint32_t chanid);
int32_t vdec_chan_list_add(struct vdec_chan_list *list, int32_t chan_id);
int32_t vdec_chan_list_remove(struct vdec_chan_list *list, int32_t chan_id);
void vdec_set_reset_state(void);
void vdec_chan_list_put(struct vdec_chan_list *list, uint32_t chanid);
bool vdec_chan_list_destroy(struct vdec_chan_list *list, uint32_t chanid);
#endif
