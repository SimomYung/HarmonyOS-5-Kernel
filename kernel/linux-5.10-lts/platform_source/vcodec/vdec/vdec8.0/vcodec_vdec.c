/*
 * vcodec_vdec.c
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

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include "vcodec_osal.h"
#include "vcodec_vdec.h"
#include "vcodec_vdec_regulator.h"
#include "vcodec_vdec_memory.h"
#include "vcodec_vdec_plat.h"
#include "video_log.h"
#include "smmu/smmu.h"
#include "vdec_memory_map.h"
#include "vcodec_mcu_ddr_map.h"
#include "vfmw_ipc.h"
#include "vfmw_pdt.h"
#include "tvp_adapter.h"
#include "vdec/VDE_SUB_CTRL_NS_c_union_define.h"
#include "vdec/vdec_base_address.h"
#include "soc_iomcu_baseaddr_interface.h"
#include "vcodec_vdec_dbg.h"
#include "platform_include/basicplatform/linux/dfx_bbox_diaginfo.h"
#include "bbox_diaginfo_id_def.h"

#define MAX_OPEN_COUNT (VFMW_CHAN_ID_MAX)

static int32_t g_is_normal_init;

static struct class *g_vdec_class = NULL;
static const char g_vdec_drv_name[] = "vcodec_vdec";
static dev_t g_vdec_dev_num;

vdec_entry g_vdec_entry;
static struct dec_file_handle g_file_handle[MAX_OPEN_COUNT];

typedef enum {
	T_IOCTL_ARG,
	T_IOCTL_ARG_COMPAT,
	T_BUTT,
} compat_type_e;

static uint32_t get_clk_rate(void)
{
	uint32_t i, j;
	uint32_t max_base_clk_level = 0;
	uint32_t clk_level = 0;
	uint64_t total_load = 0;
	uint32_t chan_num = 0;
	vdec_regulator *regulator_info = &(vdec_plat_get_entry()->regulator_info);

	for (i = 0; i < MAX_OPEN_COUNT; i++) {
		if (g_file_handle[i].is_used) {
			for (j = 0; j < VFMW_CHAN_ID_MAX; j++) {
				if (g_file_handle[i].chanload[j].is_used && !g_file_handle[i].chanload[j].is_back_ground) {
					chan_num++;
					max_base_clk_level = g_file_handle[i].chanload[j].base_clk_level > max_base_clk_level ?
						g_file_handle[i].chanload[j].base_clk_level : max_base_clk_level;
					if (ULLONG_MAX - total_load >= g_file_handle[i].chanload[j].load)
						total_load += g_file_handle[i].chanload[j].load;
				}
			}
		}
	}
	clk_level = VDEC_CLK_RATE_LEVEL_MAX - 1;
	for (i = 0; i < VDEC_CLK_LOAD_LIMIT_MAX; i++) {
		if (total_load <= regulator_info->clk_load_limit_values[i]) {
			clk_level = i;
			break;
		}
	}
	dprint(PRN_ALWS, "chan_num is %u total_load %llu, clk_level %u, max_base_clk_level %u",
		chan_num, total_load, clk_level, max_base_clk_level);
	clk_level = max_base_clk_level > clk_level ? max_base_clk_level : clk_level;

	return clk_level;
}

vdec_entry *vdec_get_entry(void)
{
	return &g_vdec_entry;
}

static void vdec_chan_list_init(struct vdec_chan_list *list)
{
	mutex_init(&list->lock);
	INIT_LIST_HEAD(&list->chan_head);
}

bool vdec_chan_list_get(struct vdec_chan_list *list, uint32_t chanid)
{
	struct vdec_chan_listnode *pos = NULL;

	mutex_lock(&list->lock);
	list_for_each_entry(pos, &list->chan_head, head) {
		if (pos->chan_id == (int32_t)chanid) {
			mutex_unlock(&list->lock);
			return true;
		}
	}
	mutex_unlock(&list->lock);
	return false;
}

int32_t vdec_chan_list_add(struct vdec_chan_list *list, int32_t chan_id)
{
	struct vdec_chan_listnode *pos = NULL;
	struct vdec_chan_listnode *node = NULL;

	dprint(PRN_DBG, "add chan id is %d", chan_id);
	if (!list) {
		dprint(PRN_ALWS, "add chan list null ptr");
		return -EFAULT;
	}

	mutex_lock(&list->lock);
	list_for_each_entry(pos, &list->chan_head, head) {
		if (pos->chan_id == chan_id) {
			dprint(PRN_ERROR, "duplicate chanid :%d", chan_id);
			mutex_unlock(&list->lock);
			return -EFAULT;;
		}
	}

	node = vmalloc(sizeof(struct vdec_chan_listnode));
	if (!node) {
		dprint(PRN_ERROR, "malloc fail");
		mutex_unlock(&list->lock);
		return -ENOMEM;
	}
	node->chan_id = chan_id;
	node->chan_state = VFMW_CHAN_STATE_CREATE;
	dprint(PRN_ALWS, "create chan %d", chan_id);

	list_add_tail(&node->head, &list->chan_head);
	mutex_unlock(&list->lock);
	return 0;
}

int32_t vdec_chan_list_remove(struct vdec_chan_list *list, int32_t chan_id)
{
	struct vdec_chan_listnode *pos = NULL;
	struct vdec_chan_listnode *node = NULL;
	dprint(PRN_DBG, "remove chan id is %d", chan_id);

	if (!list) {
		dprint(PRN_ALWS, "add chan list null ptr");
		return -EFAULT;
	}

	mutex_lock(&list->lock);
	list_for_each_entry_safe(pos, node, &list->chan_head, head) {
		if (pos->chan_id == chan_id) {
			dprint(PRN_ALWS, "destroy chan_id :%d", chan_id);
			pos->chan_id = -1;
			pos->chan_state = VFMW_CHAN_STATE_NULL;
			list_del(&pos->head);
			vfree(pos);
			mutex_unlock(&list->lock);
			return 0;
		}
	}

	dprint(PRN_ERROR, "remove invalid chan_id:%d", chan_id);
	mutex_unlock(&list->lock);
	return -EINVAL;
}

static void vdec_clear_chan_load(vdec_load_param *load_node)
{
	load_node->base_clk_level = 0;
	load_node->is_release = 0;
	load_node->is_used = 0;
	load_node->load = 0;
}

static void vdec_init_all_file_handle(void)
{
	uint32_t instance_id, index;

	for (instance_id = 0; instance_id < MAX_OPEN_COUNT; instance_id++) {
		g_file_handle[instance_id].instance_id = instance_id;
		g_file_handle[instance_id].is_used = 0;
		g_file_handle[instance_id].mcu_log_read_offset = 0;
		g_file_handle[instance_id].vdec_entry = NULL;
		g_file_handle[instance_id].hw_lock_flag = false;
		g_file_handle[instance_id].global_reset = false;
		for (index = 0; index < VFMW_CHAN_ID_MAX; index++) {
			vdec_clear_chan_load(&g_file_handle[instance_id].chanload[index]);
		}
		vdec_share_fd_list_init(&g_file_handle[instance_id].fdlist);
		vdec_chan_list_init(&g_file_handle[instance_id].chanlist);
		spin_lock_init(&g_file_handle[instance_id].global_reset_spin_lock);
	}
}

static int32_t vdec_drv_acquire_file_handle(struct dec_file_handle **file)
{
	uint32_t instance_id;

	check_return(file, "input param is null");

	for (instance_id = 0; instance_id < MAX_OPEN_COUNT; instance_id++)
		if (g_file_handle[instance_id].is_used == 0)
			break;

	if (instance_id >= MAX_OPEN_COUNT) {
		dprint(PRN_ERROR, "failed to find idle chan");
		return -ENOSPC;
	}

	g_file_handle[instance_id].is_used = 1;
	*file = &g_file_handle[instance_id];

	return 0;
}

static void vdec_close_notify_mcu_destroy(struct dec_file_handle *handle)
{
	int32_t ret = 0;
	vdec_entry *vdec = handle->vdec_entry;
	struct ipc_destroy_msg msg = {0};
	struct vdec_chan_list *chan_list = &handle->chanlist;
	struct vdec_chan_listnode *chan_pos = NULL;
	struct vdec_chan_listnode *chan_node = NULL;

	mutex_lock(&chan_list->lock);
	list_for_each_entry_safe(chan_pos, chan_node, &chan_list->chan_head, head) {
		if (chan_pos->chan_id < VFMW_NO_SEC_CHAN_START || chan_pos->chan_id >= VFMW_CHAN_ID_MAX) {
			dprint(PRN_FATAL, "chan_id %d error", chan_pos->chan_id);
			chan_pos->chan_id = -1;
			chan_pos->chan_state = VFMW_CHAN_STATE_NULL;
			list_del(&chan_pos->head);
			vfree(chan_pos);
			continue;
		}

		dprint(PRN_ALWS, "release chan id:%d", chan_pos->chan_id);
		msg.chan_id = (uint32_t)chan_pos->chan_id;
		/* ipc timeout no need to send another message to MCU */
		if (!ret && chan_pos->chan_state == VFMW_CHAN_STATE_CREATE) {
			ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_DESTORY_MSG, &msg, sizeof(struct ipc_destroy_msg));
			if (ret || msg.output.res)
				dprint(PRN_ERROR, "send destroy msg failed %d %d", ret, msg.output.res);
		}
		chan_pos->chan_id = -1;
		chan_pos->chan_state = VFMW_CHAN_STATE_NULL;
		list_del(&chan_pos->head);
		vfree(chan_pos);
	}
	mutex_unlock(&chan_list->lock);
}

static int32_t vdec_drv_release_file_handle(struct file *file_handle, vdec_entry *vdec)
{
	struct dec_file_handle *handle = (struct dec_file_handle *)file_handle->private_data;
	struct vdec_fd_list *list = &handle->fdlist;
	struct vdec_fd_listnode *pos = NULL;
	struct vdec_fd_listnode *node = NULL;
	uint32_t index;

	if (handle->is_used == 0) {
		dprint(PRN_ERROR, "file handle is not used");
		return -EINVAL;
	}

	/* The vdec abnormal close notifies the MCU to destroy the corresponding channel
	   before freeing the memory requested by the user mode. */
	vdec_close_notify_mcu_destroy(handle);

	mutex_lock(&list->lock);
	list_for_each_entry_safe(pos, node, &list->fdhead, head) {
		(void)vdec_mem_iommu_unmap((void *)handle->vdec_entry->device,
			pos->share_fd, file_handle, pos->dmabuf, pos->iova);

		dprint(PRN_DBG, "release share_fd:%d", pos->share_fd);
		pos->share_fd = -1;
		pos->dmabuf = VCODEC_NULL;
		pos->iova = VCODEC_NULL;
		pos->iova_size = 0;
		list_del(&pos->head);
		vfree(pos);
	}
	mutex_unlock(&list->lock);
	handle->instance_id = 0;
	for (index = 0; index < VFMW_CHAN_ID_MAX; index++) {
		vdec_clear_chan_load(&handle->chanload[index]);
	}
	handle->is_used = 0;
	handle->mcu_log_read_offset = 0;
	handle->vdec_entry = NULL;

	return 0;
}

static ssize_t vdec_freq_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t count;
	uint32_t clk_level = VDEC_CLK_RATE_LEVEL_MAX;

	if (!buf)
		return 0;

	vdec_plat_get_dynamic_clk_rate(&clk_level);
	count = sprintf_s(buf, PAGE_SIZE, "%u\n", clk_level);

	return count;
}

static ssize_t vdec_freq_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	long val = 0;

	if (kstrtol(buf, 0, &val) < 0)
		return (ssize_t)count;

	if (val >= VDEC_CLK_RATE_LEVEL_MAX || val < 0)
		return (ssize_t)count;

	vdec_plat_set_dynamic_clk_rate((uint32_t)val);
	dprint(PRN_ALWS, "set clk is %u", (uint32_t)val);

	return (ssize_t)count;
}


static int32_t vdec_create_class(void)
{
	int32_t rc;

	g_vdec_class = class_create(THIS_MODULE, "vdec_class");
	if (IS_ERR(g_vdec_class)) {
		rc = PTR_ERR(g_vdec_class);
		g_vdec_class = NULL;
		dprint(PRN_FATAL, "call class_create failed, rc = %d\n", rc);
		return rc;
	}

	return 0;
}

static void vdec_destroy_class(void)
{
	class_destroy(g_vdec_class);
	g_vdec_class = NULL;
}

static int32_t vdec_create_device_file(struct device *dev)
{
	int32_t rc;


	return 0;
}

static int32_t vdec_setup_cdev(vdec_entry *vdec, const struct file_operations *fops)
{
	int32_t rc;

	rc = vdec_create_class();
	if (rc) {
		dprint(PRN_FATAL, "call vdec_create_class failed, rc = %d\n", rc);
		return rc;
	}

	rc = alloc_chrdev_region(&g_vdec_dev_num, 0, 1, "vcodec_decoder");
	if (rc) {
		dprint(PRN_FATAL, "call alloc_chrdev_region failed, rc = %d\n", rc);
		goto cls_destroy;
	}

	vdec->class_dev = device_create(g_vdec_class, NULL, g_vdec_dev_num, "%s", "vcodec_vdec");
	if (IS_ERR(vdec->class_dev)) {
		rc = PTR_ERR(vdec->class_dev);
		dprint(PRN_FATAL, "call device_create failed, rc = %d\n", rc);
		goto unregister_region;
	}

	cdev_init(&vdec->cdev, fops);
	vdec->cdev.owner = THIS_MODULE;
	rc = cdev_add(&vdec->cdev, g_vdec_dev_num, 1);
	if (rc < 0) {
		dprint(PRN_FATAL, "call cdev_add failed, rc = %d\n", rc);
		goto dev_destroy;
	}

	rc = vdec_create_device_file(vdec->class_dev);
	if (rc < 0) {
		dprint(PRN_FATAL, "call vdec_create_device_file failed, rc = %d\n", rc);
		goto dev_del;
	}

	return 0;

dev_del:
	cdev_del(&vdec->cdev);
dev_destroy:
	device_destroy(g_vdec_class, g_vdec_dev_num);
unregister_region:
	unregister_chrdev_region(g_vdec_dev_num, 1);
cls_destroy:
	vdec_destroy_class();

	return rc;
}

static int32_t vdec_cleanup_cdev(vdec_entry *vdec)
{
	if (!g_vdec_class) {
		dprint(PRN_FATAL, "vdec class is NULL");
		return -EINVAL;
	} else {
		cdev_del(&vdec->cdev);
		device_destroy(g_vdec_class, g_vdec_dev_num);
		unregister_chrdev_region(g_vdec_dev_num, 1);
		class_destroy(g_vdec_class);
		g_vdec_class = VCODEC_NULL;
		return 0;
	}
}

static void vdec_init_power_state(vdec_entry *vdec)
{
	unsigned long flag;

	spin_lock_irqsave(&vdec->power_state_spin_lock, flag);
	vdec->power_state = true;
	spin_unlock_irqrestore(&vdec->power_state_spin_lock, flag);
}

vdec_entry *vdec_get_entry_by_file_handle(struct file *file_handle)
{
	struct dec_file_handle *vdec_file = NULL;

	if (!file_handle) {
		dprint(PRN_FATAL, "file_handle is null");
		return NULL;
	}

	if (!file_handle->private_data) {
		dprint(PRN_FATAL, "private_data is null");
		return NULL;
	}

	vdec_file = (struct dec_file_handle *)file_handle->private_data;
	return vdec_file->vdec_entry;
}

static int32_t vdec_open_mcu(void)
{
#if VDEC_MCORE_ENABLE
	vdec_entry *vdec = vdec_get_entry();
	return vdec->mcu_dev->open(vdec->mcu_dev, vdec->reg_base_vir_addr);
#else
	return 0;
#endif
}

static void vdec_close_mcu(void)
{
#if VDEC_MCORE_ENABLE
	vdec_entry *vdec = vdec_get_entry();
	(void)vdec->mcu_dev->close(vdec->mcu_dev);
#endif
}

static int32_t vdec_suspend_mcu(void)
{
#if VDEC_MCORE_ENABLE
	vdec_entry *vdec = vdec_get_entry();
	return vdec->mcu_dev->suspend(vdec->mcu_dev);
#else
	return 0;
#endif
}

static int32_t vdec_resume_mcu(void)
{
#if VDEC_MCORE_ENABLE
	vdec_entry *vdec = vdec_get_entry();
	return vdec->mcu_dev->resume(vdec->mcu_dev);
#else
	return 0;
#endif
}

dev_reg_info g_dev_reg_info;

static void set_dev_reg_phy_base_addr(UADDR addr, uint32_t range)
{
	g_dev_reg_info.reg_reg_phy_base_addr = addr;
	g_dev_reg_info.reg_range = range;
}

static int32_t vdec_map_reg_addr(void)
{
	vdec_dts *dts_info = &(vdec_plat_get_entry()->dts_info);
	UADDR reg_addr = dts_info->reg_base_info.reg_phy_addr;
	uint32_t reg_range = dts_info->reg_base_info.reg_range;
	set_dev_reg_phy_base_addr(reg_addr, reg_range);

	g_vdec_entry.reg_base_vir_addr = (uint8_t *)vdec_mmap(reg_addr, reg_range);
	if (!g_vdec_entry.reg_base_vir_addr)
		goto exit;

	g_vdec_entry.media1_ctrl_vir_addr = ioremap(SOC_IOMCU_MEDIA1_CTRL_BASE_ADDR, 0x1000);
	if (!g_vdec_entry.media1_ctrl_vir_addr)
		goto media1_ctrl;

	g_vdec_entry.media1_crg_vir_addr = ioremap(SOC_IOMCU_MEDIA1_CRG_BASE_ADDR, 0x1000);
	if (!g_vdec_entry.media1_crg_vir_addr)
		goto media1_crg;

	g_vdec_entry.bisr_ctrl_vir_addr = ioremap(SOC_ACPU_BISR_BASE_ADDR, 0x1000);
	if (!g_vdec_entry.bisr_ctrl_vir_addr)
		goto bisr_ctrl;

	return 0;

bisr_ctrl:
	iounmap(g_vdec_entry.media1_crg_vir_addr);
media1_crg:
	iounmap(g_vdec_entry.media1_ctrl_vir_addr);
media1_ctrl:
	vdec_munmap((uint32_t*)g_vdec_entry.reg_base_vir_addr);
exit:
	return -EIO;
}

static void vdec_unmap_reg_addr(void)
{
	if (g_vdec_entry.bisr_ctrl_vir_addr) {
		iounmap(g_vdec_entry.bisr_ctrl_vir_addr);
		g_vdec_entry.bisr_ctrl_vir_addr = NULL;
	}
	if (g_vdec_entry.media1_crg_vir_addr) {
		iounmap(g_vdec_entry.media1_crg_vir_addr);
		g_vdec_entry.media1_crg_vir_addr = NULL;
	}
	if (g_vdec_entry.media1_ctrl_vir_addr) {
		iounmap(g_vdec_entry.media1_ctrl_vir_addr);
		g_vdec_entry.media1_ctrl_vir_addr = NULL;
	}

	if (g_vdec_entry.reg_base_vir_addr) {
		vdec_munmap((uint32_t *)g_vdec_entry.reg_base_vir_addr);
		g_vdec_entry.reg_base_vir_addr = VCODEC_NULL;
	}
}

#ifdef VDEC_MCORE_ENABLE
static int32_t vdec_async_bsp_reset_work(void * msg)
{
	int32_t ret;
	vdec_entry *vdec = vdec_get_entry();

	vdec_mutex_lock(&vdec->vdec_mutex);
	if (!vdec->power_state) {
		dprint(PRN_WARN, "vdec is poweroff");
		vdec_mutex_unlock(&vdec->vdec_mutex);
		return 0;
	}
	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed\n");
		vdec_mutex_unlock(&vdec->vdec_mutex);
		return -EFAULT;
	}

	ret = vdec->bsp_dev->reset(vdec->bsp_dev, msg);
	if (ret) {
		dprint(PRN_ERROR, "reset bsp fail");
	}

	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");
	vdec_mutex_unlock(&vdec->vdec_mutex);
	return ret;
}

static int32_t vdec_async_pxp_reset_work(void * msg)
{
	int32_t ret;
	vdec_entry *vdec = vdec_get_entry();

	vdec_mutex_lock(&vdec->vdec_mutex);
	if (!vdec->power_state) {
		dprint(PRN_WARN, "vdec is poweroff");
		vdec_mutex_unlock(&vdec->vdec_mutex);
		return 0;
	}
	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed\n");
		vdec_mutex_unlock(&vdec->vdec_mutex);
		return -EFAULT;
	}

	ret = vdec->pxp_dev->reset(vdec->pxp_dev, msg);
	if (ret) {
		dprint(PRN_ERROR, "reset pxp fail");
	}
	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");
	vdec_mutex_unlock(&vdec->vdec_mutex);
	return ret;
}

static int32_t vdec_bsp_reset(void * msg)
{
	vdec_entry *vdec = vdec_get_entry();
	void *node = NULL;
	int32_t work_ret;

	node = vcodec_work_submit(vdec->work_ctx, vdec_async_bsp_reset_work, msg);
	if (!node) {
		dprint(PRN_ERROR, "bsp reset node error");
		return -EFAULT;
	}
	work_ret = vcodec_work_finish(node);
	if (work_ret)
		dprint(PRN_ERROR, "bsp reset error");
	return work_ret;
}

static int32_t vdec_pxp_reset(void * msg)
{
	vdec_entry *vdec = vdec_get_entry();
	void *node = NULL;
	int32_t work_ret;

	node = vcodec_work_submit(vdec->work_ctx, vdec_async_pxp_reset_work, msg);
	if (!node) {
		dprint(PRN_ERROR, "pxp reset node error");
		return -EFAULT;
	}
	work_ret = vcodec_work_finish(node);
	if (work_ret)
		dprint(PRN_ERROR, "pxp reset error");
	return work_ret;
}

void vdec_set_reset_state(void)
{
	unsigned long flag;
	uint32_t instance_id;

	for (instance_id = 0; instance_id < MAX_OPEN_COUNT; instance_id++) {
		if (g_file_handle[instance_id].is_used) {
			spin_lock_irqsave(&g_file_handle->global_reset_spin_lock, flag);
			g_file_handle[instance_id].global_reset = true;
			spin_unlock_irqrestore(&g_file_handle->global_reset_spin_lock, flag);
		}
	}
}

static void vdec_recv_event_notify(enum vcodec_dev_event_notify event, void * msg)
{
	switch (event) {
	case VCODEC_DEV_MCU_EVENT_ALLOC_ION_DONE:
		complete_all(&vdec_get_entry()->mcu_alloc_done);
		break;
	case VCODEC_DEV_MCU_RESET_BSP:
		if (vdec_bsp_reset(msg))
			vdec_set_reset_state();
		vdec_sh_bsp();
		break;
	case VCODEC_DEV_MCU_RESET_PXP:
		if (vdec_pxp_reset(msg))
			vdec_set_reset_state();
		vdec_sh_pxp();
		break;
	case VCODEC_DEV_MCU_EVENT_REPORT:
		complete(&vdec_get_entry()->mcu_report_event);
		break;
	case VCODEC_DEV_MCU_RESET_GLOBAL:
		vdec_set_reset_state();
		dprint(PRN_WARN, "set vdec global reset");
		break;
	default:
		return;
	}
}

// dump mcu log from mcu_log memory where logs are kept
static int32_t vdec_dump_mcu_log(void *arg)
{
	vdec_entry *vdec = vdec_get_entry();

	if (!vdec->mcu_log.log || vdec->mcu_log.real_size > vdec->mcu_log.buffer_size)
		return -EINVAL;

	return vdec_write_log(vdec->mcu_log.log, vdec->mcu_log.real_size, vdec->is_panic_log);
}

static void vdec_transfer_mcu_log(uint8_t *buffer, uint32_t size)
{
	vdec_entry *vdec = vdec_get_entry();
	void *work_node = NULL;
	int work_ret = 0;
	uint32_t real_size;

	dprint(PRN_ALWS, "enter mcu log dump, transfer time:%ld, current time:%ld, size:%u",
		vdec->mcu_log.time_stamp, ktime_get_real_seconds(), size);

	if (size > MCU_LOG_BUFFER_SIZE) {
		dprint(PRN_WARN, "mcu log size error:%u>%d", size, MCU_LOG_BUFFER_SIZE);
		real_size = MCU_LOG_BUFFER_SIZE;
	} else {
		real_size = size;
	}

	if (!vdec->mcu_log.log) {
		vdec->mcu_log.log = vmalloc(MCU_LOG_BUFFER_SIZE);
		if (!vdec->mcu_log.log) {
			dprint(PRN_ERROR, "alloc mcu log buffer fail");
			return;
		}
		vdec->mcu_log.buffer_size = MCU_LOG_BUFFER_SIZE;
		dprint(PRN_ALWS, "alloc mcu log buffer size:%u", vdec->mcu_log.buffer_size);
	}

	if (memcpy_s(vdec->mcu_log.log, (size_t)vdec->mcu_log.buffer_size, buffer, real_size)) {
		dprint(PRN_ERROR, "copy mcu log fail");
		return;
	}
	vdec->mcu_log.time_stamp = ktime_get_real_seconds();
	vdec->mcu_log.real_size = real_size;

	work_node = vcodec_work_submit(vdec->root_ctx, vdec_dump_mcu_log, NULL);
	if (!work_node) {
		dprint(PRN_ERROR, "vdec submit log task error");
		return;
	}
	work_ret = vcodec_work_finish(work_node);
	if (work_ret != 0)
		dprint(PRN_ERROR, "vdec dump mcu logbuffer error");
}

static void vdec_free_mcu_logbuffer(void)
{
	vdec_entry *vdec = vdec_get_entry();
	if (vdec->mcu_log.log) {
		vfree(vdec->mcu_log.log);
		vdec->mcu_log.log = NULL;
		vdec->mcu_log.buffer_size = 0;
	}
}

// realtime dump mcu logs based on pointers and sizes in mcu exec mem
int32_t vdec_dump_mcu_log_realtime(void *arg)
{
	vdec_entry *vdec = vdec_get_entry();
	struct mcu_real_time_log log = {0};

	if (!vdec->mcu_dev)
		return -EINVAL;

	if (vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_DUMP_REALTIME_LOG, &log,
		sizeof(struct mcu_real_time_log)))
		return -EFAULT;

	return vdec_write_log(log.buffer, log.size, vdec->is_panic_log);
}

void vdec_betaclub_dump_mculog(void)
{
	vdec_entry *vdec = vdec_get_entry();
	void *work_node = NULL;
	int work_ret = 0;

	if (!vdec->root_ctx)
		return;

	dprint(PRN_ALWS, "vdec betaclub dump mcu log start");
	work_node = vcodec_work_submit(vdec->root_ctx, vdec_dump_mcu_log_realtime, NULL);
	if (!work_node) {
		dprint(PRN_ERROR, "vdec submit log task error");
		return;
	}
	work_ret = vcodec_work_finish(work_node);
	if (work_ret != 0) {
		dprint(PRN_ERROR, "vdec dump mcu logbuffer error");
		return;
	}
	dprint(PRN_ALWS, "vdec betaclub dump mcu log end");
}

EXPORT_SYMBOL(vdec_betaclub_dump_mculog);

static void vdec_dump_bsp_irq_status(void)
{
	uint32_t status_reg_value;
	vdec_entry *vdec = vdec_get_entry();
	if (!vdec->reg_base_vir_addr)
		return;

	// sub ctrl INTR_NS_BSP1
	rd_vreg(vdec->reg_base_vir_addr, 0x408078, status_reg_value);
	dprint(PRN_ALWS, "bsp irq status:0x%x, after power off", status_reg_value);
}

static void vdec_dump_pxp_irq_status(void)
{
	uint32_t status_reg_value;
	vdec_entry *vdec = vdec_get_entry();
	if (!vdec->reg_base_vir_addr)
		return;

	// sub ctrl INTR_NS_PXPC1
	rd_vreg(vdec->reg_base_vir_addr, 0x408068, status_reg_value);
	dprint(PRN_ALWS, "pxp irq status:0x%x, after power off", status_reg_value);
}

static int32_t vdec_create_mcu_dev(void)
{
	vdec_entry *vdec = vdec_get_entry();
	if (unlikely(vdec->mcu_dev)) {
		dprint(PRN_ERROR, "invalid vdec entry");
		return -EFAULT;
	}

	vdec->mcu_dev = create_vcodec_dev(VCODEC_DEV_MCU, vdec->device);
	if (!vdec->mcu_dev)
		return -EFAULT;

	if (vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SET_EVENT_NOTIFY,
		(void *)vdec_recv_event_notify, sizeof(void *))) {
		destroy_vcodec_dev(vdec->mcu_dev);
		vdec->mcu_dev = NULL;
		return -EFAULT;
	}

	if (vdec->mcu_dev->control(vdec->mcu_dev,
		VCODEC_DEV_MCU_SET_LOG_TRANSFER,
		(void *)vdec_transfer_mcu_log, sizeof(void *))) {
		destroy_vcodec_dev(vdec->mcu_dev);
		vdec->mcu_dev = NULL;
		return -EFAULT;
	}
	return 0;
}

static void vdec_destroy_mcu_dev(void)
{
	vdec_entry* vdec = vdec_get_entry();
	if (vdec->mcu_dev) {
		destroy_vcodec_dev(vdec->mcu_dev);
		vdec->mcu_dev = NULL;
	}
}
#else
static int32_t vdec_create_mcu_dev(void)
{
	return 0;
}
static void vdec_destroy_mcu_dev(void)
{
}

#endif

static int32_t vdec_create_sub_dev(void)
{
	vdec_entry *vdec = vdec_get_entry();
	if (unlikely(vdec->bsp_dev || vdec->pxp_dev || vdec->scd_dev)) {
		dprint(PRN_ERROR, "invalid vdec entry");
		return -EFAULT;
	}

	vdec->bsp_dev = create_vcodec_dev(VCODEC_DEV_BSP, vdec->device);
	if (!vdec->bsp_dev)
		return -EFAULT;

	vdec->pxp_dev = create_vcodec_dev(VCODEC_DEV_PXP, vdec->device);
	if (!vdec->pxp_dev)
		goto create_pxp_error;

	vdec->scd_dev = create_vcodec_dev(VCODEC_DEV_SCD, vdec->device);
	if (!vdec->scd_dev)
		goto create_scd_error;

	if (vdec_create_mcu_dev())
		goto create_mcu_error;

	return 0;

create_mcu_error:
	destroy_vcodec_dev(vdec->scd_dev);
	vdec->scd_dev = NULL;
create_scd_error:
	destroy_vcodec_dev(vdec->pxp_dev);
	vdec->pxp_dev = NULL;
create_pxp_error:
	destroy_vcodec_dev(vdec->bsp_dev);
	vdec->bsp_dev = NULL;

	return -EFAULT;
}

static void vdec_destroy_sub_dev(void)
{
	vdec_entry* vdec = vdec_get_entry();

	vdec_destroy_mcu_dev();

	if (vdec->bsp_dev) {
		destroy_vcodec_dev(vdec->bsp_dev);
		vdec->bsp_dev = NULL;
	}

	if (vdec->pxp_dev) {
		destroy_vcodec_dev(vdec->pxp_dev);
		vdec->pxp_dev = NULL;
	}

	if (vdec->scd_dev) {
		destroy_vcodec_dev(vdec->scd_dev);
		vdec->scd_dev = NULL;
	}
}

static int32_t vdec_close_sub_dev(void* arg)
{
	vdec_entry *vdec = (vdec_entry *)arg;
	int32_t ret_bsp, ret_pxp;
	int32_t ret = 0;

	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed\n");
		return -EFAULT;
	}
	vdec_close_mcu();
	(void)vdec->scd_dev->close(vdec->scd_dev);
	if (!vcodec_work_finish(vdec->async_open_node)) {
		ret_pxp = vdec->pxp_dev->close(vdec->pxp_dev);
		ret_bsp = vdec->bsp_dev->close(vdec->bsp_dev);
		if (ret_pxp || ret_bsp) {
			dprint(PRN_ERROR, "close pxp ret %d, bsp ret %d", ret_pxp, ret_bsp);
			vdec_set_reset_state();
			ret = -EFAULT;
		} else {
			dprint(PRN_ALWS, "close pxp and bsp device success");
		}
	}

	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");

	return ret;
}

static int32_t vdec_async_open_bsp(void * arg)
{
	vdec_entry *vdec = (vdec_entry *)arg;
    unsigned long timeout;

    timeout = wait_for_completion_timeout(&vdec->mcu_alloc_done, msecs_to_jiffies(1000));
    if (timeout == 0) {
        dprint(PRN_ERROR, "wait mcu done timeout");
        return -EFAULT;
    }

	if (vdec->bsp_dev->open(vdec->bsp_dev, vdec->reg_base_vir_addr)) {
		dprint(PRN_ERROR, "open bsp fail");
		return -EFAULT;
	}

	return 0;
}

static int32_t vdec_async_open_pxp(void * arg)
{
	int32_t ret;
	vdec_entry *vdec = (vdec_entry *)arg;
    unsigned long timeout;

    timeout = wait_for_completion_timeout(&vdec->mcu_alloc_done, msecs_to_jiffies(1000));
    if (timeout == 0) {
        dprint(PRN_ERROR, "wait mcu done timeout");
        return -EFAULT;
    }

	ret = vdec->pxp_dev->open(vdec->pxp_dev, vdec->reg_base_vir_addr);
	if (ret) {
		dprint(PRN_ERROR, "open pxp fail");
		return -EFAULT;
	}

	return 0;
}

bool vdec_is_async_open_success(vdec_entry* vdec)
{
	if (vdec->async_open_state == OPENING) {
		dprint(PRN_WARN, "async open state is opening");
		wait_for_completion(&vdec->async_open_done);
	}

	return vdec->async_open_state == OPENED_SUCCESS;
}

static int32_t vdec_async_open(void * arg)
{
	int32_t bsp_ret, pxp_ret;
	void *work, *node;
	vdec_entry *vdec = (vdec_entry *)arg;

	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed");
		return -EFAULT;
	}

	dprint(PRN_ALWS, "tvp vdec init success");
	work = vcodec_work_ctx_create("vcodec_temp");
	vfmw_assert_goto_prnt(work, tvp_exit, "work create failed");

	node = vcodec_work_submit(work, vdec_async_open_bsp, vdec);
	vfmw_assert_goto_prnt(node, work_destroy, "work submit failed");

	DBG_TIMER_SATRT();
	pxp_ret = vdec_async_open_pxp(vdec);
	DBG_TIMER_SHOW("pxp open");

	bsp_ret = vcodec_work_finish(node);
	DBG_TIMER_SHOW("bxp open");
	if (bsp_ret != 0 || pxp_ret != 0) {
		dprint(PRN_FATAL, "async open failed");
		vdec->async_open_state = OPENED_FAILED;
		if (!bsp_ret)
			(void)vdec->bsp_dev->close(vdec->bsp_dev);

		if (!pxp_ret) {
			(void)vdec->pxp_dev->close(vdec->pxp_dev);
		}
	} else {
		dprint(PRN_ALWS, "async open success");
		vdec->async_open_state = OPENED_SUCCESS;
	}

	complete_all(&vdec->async_open_done);

work_destroy:
	vcodec_work_ctx_destroy(work);
tvp_exit:
	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed");
	return (vdec->async_open_state != OPENED_SUCCESS) ? -EFAULT : 0;
}

static int32_t vdec_sync_open_scd(void * arg)
{
	int32_t ret;
	vdec_entry *vdec = (vdec_entry *)arg;
	ret = vdec->scd_dev->open(vdec->scd_dev, vdec->reg_base_vir_addr);
	if (ret) {
		dprint(PRN_ERROR, "open scd fail");
		return -EFAULT;
	}

	return 0;
}

static int32_t vdec_sync_open_mcu(vdec_entry* vdec)
{
	if (vdec_open_mcu()) {
		dprint(PRN_ERROR, "open mcu fail");
		return -EFAULT;
	}
	return 0;
}

/* Optimized for open subdevices:
	  + current process: SCD + MCU
	  + vcodec_async   : PXP + BSP (witch create once in all lifecycle until driver remove)
*/
static int32_t vdec_open_sub_dev(void)
{
	vdec_entry* vdec = vdec_get_entry();
	int32_t ret;
	DBG_TIMER_SATRT();

	// create once in all lifecycle until driver remove
	if (!vdec->work_ctx) {
		vdec->work_ctx = vcodec_work_ctx_create("vcodec_async");
		vfmw_assert_goto_prnt(vdec->work_ctx, exit_error, "create async work failed");
	}

	init_completion(&vdec->mcu_alloc_done);
	init_completion(&vdec->mcu_report_event);
	init_completion(&vdec->async_open_done);

	vdec->async_open_state = OPENING;
	vdec->async_open_node = vcodec_work_submit(vdec->work_ctx, vdec_async_open, vdec);
	vfmw_assert_goto_prnt(vdec->async_open_node, exit_error, "submit async open work failed");
	DBG_TIMER_SHOW("submit async open work");

	DBG_TIMER_RESATRT();
	ret = vdec_sync_open_scd(vdec);
	vfmw_assert_goto_prnt(ret == 0, open_scd_error, "open scd failed");
	DBG_TIMER_SHOW("open scd");

	DBG_TIMER_RESATRT();
	ret = vdec_sync_open_mcu(vdec);
	vfmw_assert_goto_prnt(ret == 0, open_mcu_error, "open mcu failed");
	DBG_TIMER_SHOW("open mcu");
	dprint(PRN_ALWS, "open mcu success");

	return 0;

open_mcu_error:
	(void)vdec->scd_dev->close(vdec->scd_dev);
open_scd_error:
	ret = vcodec_work_finish(vdec->async_open_node);
	if (ret == 0) {
		// close bsp/pxp/scd when async open success
		(void)vdec->bsp_dev->close(vdec->bsp_dev);
		(void)vdec->pxp_dev->close(vdec->pxp_dev);
	}

exit_error:
	return -EFAULT;
}

static int vdec_panic_notify(struct notifier_block *nb, unsigned long action, void *data)
{
	vdec_entry *vdec = vdec_get_entry();

	dprint(PRN_ALWS, "system panic then notice vdec");
	unused_param(nb);
	unused_param(action);
	unused_param(data);
	vdec->is_panic_log = true;
	if (vdec_dump_mcu_log_realtime(NULL)) {
		dprint(PRN_ERROR, "panic dump mculog failed");
		return NOTIFY_BAD;
	}
	vdec->is_panic_log = false;
	return NOTIFY_OK;
}

static struct notifier_block vdec_panic_block = {
	.notifier_call = vdec_panic_notify,
};

static int32_t vdec_open_global_resource(void)
{
	int32_t ret;
	vdec_plat* plt = vdec_plat_get_entry();
	vdec_entry* vdec = vdec_get_entry();

	dprint(PRN_ALWS, "begin open vdec global resource");
    vdec_sh_init();
    vdec_init_reserved_iova();
	if (vdec_map_reg_addr()) {
		dprint(PRN_FATAL, "map vdec register fail");
		return -EFAULT;
	}
	ret = vdec_log_init_entry();
	vfmw_assert_goto_prnt(ret == 0, vdec_log_init_error,
		"vdec_log_init_entry failed");

	plt->clk_ctrl.open_in_high_clk = 1;

	ret = power_up_vdec_top();
	vfmw_assert_goto_prnt(ret == 0, power_up_top_error,
		"power_up_vdec_top failed");

	ret = power_up_vdec_cal();
	vfmw_assert_goto_prnt(ret == 0, power_up_vdec_error,
		"power_up_vdec_cal failed");

	vdec->memory_ctx = vcodec_vdec_memory_request_ctx();
	vfmw_assert_goto_prnt(vdec->memory_ctx, get_memory_ctx_error,
		"get_memory_ctx failed");

	vdec->irq_ctx = vcodec_vdec_irq_request_ctx(&plt->dts_info,
		vdec->reg_base_vir_addr);
	vfmw_assert_goto_prnt(vdec->irq_ctx, get_irq_request_ctx_error,
		"get_irq_request_ctx failed");

	ret = vdec_open_sub_dev();
	vfmw_assert_goto_prnt(ret == 0, open_dev_error,
		"vdec_open_sub_dev failed");

	spin_lock_init(&vdec->power_state_spin_lock);

	g_is_normal_init = 1;
	vdec_init_power_state(vdec);

	plt->clk_ctrl.clk_flag = 1;
	plt->clk_ctrl.static_clk_level = 0;

	vdec_init_all_file_handle();
	vdec_chan_state_clear();
	(void)atomic_notifier_chain_register(&panic_notifier_list, &vdec_panic_block);
	vdec->qic_irq_cnt = 0;
	dprint(PRN_ALWS, "open vdec global resource success");
	return 0;

open_dev_error:
	vcodec_vdec_irq_free_ctx(vdec->irq_ctx);
	vdec->irq_ctx = NULL;
get_irq_request_ctx_error:
	vcodec_vdec_memory_free_ctx(vdec->memory_ctx);
	vdec->memory_ctx = NULL;
get_memory_ctx_error:
	power_down_vdec_cal(false);
power_up_vdec_error:
	power_down_vdec_top_mcu();
power_up_top_error:
	vdec_log_deinit_entry();
vdec_log_init_error:
	vdec_unmap_reg_addr();
	return -EFAULT;
}

static void vdec_close_global_resource(void)
{
	unsigned long flag;
	void* node = NULL;
	vdec_entry* vdec = vdec_get_entry();

	(void)atomic_notifier_chain_unregister(&panic_notifier_list, &vdec_panic_block);
	node = vcodec_work_submit(vdec->work_ctx, vdec_close_sub_dev, vdec);
	vcodec_work_finish(node);

	vcodec_vdec_irq_free_ctx(vdec->irq_ctx);
	vdec->irq_ctx = NULL;

	vcodec_vdec_memory_free_ctx(vdec->memory_ctx);
	vdec->memory_ctx = NULL;

	if (vdec->power_state) {
		vdec_dump_bsp_irq_status();
		vdec_dump_pxp_irq_status();
		spin_lock_irqsave(&vdec->power_state_spin_lock, flag);
		vdec->power_state = false;
		spin_unlock_irqrestore(&vdec->power_state_spin_lock, flag);
		power_down_vdec_cal(false);
		power_down_vdec_top_mcu();
		dprint(PRN_ALWS, "power down success");
	}

	vdec_log_deinit_entry();

	if (vdec->device_locked) {
		vdec->device_locked = VCODEC_FALSE;
	}

	g_is_normal_init = 0;
	vdec->setting_info.ion_alloc_total_size = 0;

	vdec_unmap_reg_addr();

    vdec_deinit_reserved_iova();
	dprint(PRN_ALWS, "after close global resource");
}

static int32_t vdec_open(struct inode *inode, struct file *file_handle)
{
	dprint(PRN_ALWS, "begin to open\n");
	int32_t ret = 0;
	vdec_entry *vdec = NULL;
	struct dec_file_handle *vdec_file = NULL;
	uint64_t temp = vcodec_osal_get_time_in_us();

	check_return(inode != NULL, "inode is null");
	check_return(file_handle != NULL, "file_handle is null");

	vdec = container_of(inode->i_cdev, vdec_entry, cdev);

	vdec_mutex_lock(&vdec->vdec_mutex);
	if (vdec->open_count >= MAX_OPEN_COUNT) {
		dprint(PRN_FATAL, "open vdec instance too much");
		ret = -EBUSY;
		goto exit;
	}

	if (vdec->open_count == 0) {
		if (vdec_open_global_resource()) {
			ret = -EFAULT;
			goto exit;
		}
	}

	if (vdec_drv_acquire_file_handle(&vdec_file)) {
		ret = -EBUSY;
		goto acquire_fail;
	}
	vdec_file->vdec_entry = vdec;
	file_handle->private_data = vdec_file;
	vdec->open_count++;
	dprint(PRN_ALWS, "open_count: %u, spend time: %lld us", vdec->open_count,
		vcodec_osal_get_time_in_us() - temp);
	vdec_mutex_unlock(&vdec->vdec_mutex);
	return 0;

acquire_fail:
	if (vdec->open_count == 0)
		vdec_close_global_resource();
exit:
	vdec_mutex_unlock(&vdec->vdec_mutex);
	return ret;
}

static int32_t vdec_close(struct inode *inode, struct file *file_handle)
{
	uint32_t msg;
	int32_t ret;
	vdec_entry *vdec = VCODEC_NULL;
	struct dec_file_handle *vdec_file = NULL;

	check_return(inode, "inode is null");
	check_return(file_handle->private_data, "private_data is null");

	vdec_file = file_handle->private_data;
	vdec = vdec_file->vdec_entry;
	check_return(vdec, "vdec is null");

	vdec_mutex_lock(&vdec->vdec_mutex);
	if (vdec->open_count > 0)
		vdec->open_count--;

	vdec_drv_release_file_handle(file_handle, vdec);
	file_handle->private_data = NULL;

	if (vdec_file->hw_lock_flag) {
		ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_UNLOCK_SCD_MSG, &msg, sizeof(uint32_t));
		if (ret) {
			dprint(PRN_DBG, "unlock hw failed");
		}
		vdec->device_locked = VCODEC_FALSE;
		vdec_file->hw_lock_flag = false;
	}

	vdec_sh_report_mcu_panic();

	if (vdec->open_count == 0)
		vdec_close_global_resource();

	dprint(PRN_ALWS, "close vdec success, open_count: %u", vdec->open_count);
	vdec_mutex_unlock(&vdec->vdec_mutex);

	return 0;
}

static int32_t vdec_compat_get_data(compat_type_e eType, void __user *pUser,
	void *pData)
{
	int32_t ret = 0;
	int32_t get_user_data_ret = 0;
	int32_t s32Data = 0;
	compat_ulong_t CompatData = 0;
	vdec_ioctl_msg *pIoctlMsg = (vdec_ioctl_msg *)pData;

	if (!pUser || !pData) {
		dprint(PRN_FATAL, "param is null\n");
		return -EINVAL;
	}

	switch (eType) {
	case T_IOCTL_ARG:
		if (copy_from_user(pIoctlMsg, pUser, sizeof(*pIoctlMsg))) {
			dprint(PRN_FATAL, "puser copy failed\n");
			ret = -EFAULT;
		}
		break;
	case T_IOCTL_ARG_COMPAT: {
		compat_ioctl_msg __user *pCompatMsg = pUser;

		get_user_data_ret |= get_user(s32Data, &pCompatMsg->chan_num);
		pIoctlMsg->chan_num = s32Data;

		get_user_data_ret |= get_user(s32Data, &pCompatMsg->in_size);
		pIoctlMsg->in_size = (uint32_t)s32Data;

		get_user_data_ret |= get_user(s32Data, &pCompatMsg->out_size);
		pIoctlMsg->out_size = (uint32_t)s32Data;

		get_user_data_ret |= get_user(CompatData, &pCompatMsg->in);
		pIoctlMsg->in = (void *)(uintptr_t)((vcodec_virt_addr_t)CompatData);

		get_user_data_ret |= get_user(CompatData, &pCompatMsg->out);
		pIoctlMsg->out = (void *)(uintptr_t)((vcodec_virt_addr_t)CompatData);

		ret = (get_user_data_ret != 0) ? -EFAULT : 0;
	}
		break;
	default:
		dprint(PRN_FATAL, "unknown type %d\n", eType);
		ret = -EINVAL;
		break;
	}

	return ret;
}

int32_t vdec_ioctl_set_clk_rate(struct file *file_handle,
	vdec_ioctl_msg *pvdec_msg)
{
	uint32_t clk_level = VDEC_CLK_RATE_LEVEL_MAX;
	uint32_t target_clk_level;
	performance_params_s performance_params;
	int32_t chan_id;
	struct dec_file_handle *vdec_file = (struct dec_file_handle *)file_handle->private_data;
	dprint(PRN_ALWS, "start VDEC_IOCTL_SET_CLK_RATE");

	check_return(pvdec_msg->in != NULL,
		"VDEC_IOCTL_SET_CLK_RATE, invalid input prarameter");
	check_para_size_return(sizeof(performance_params), pvdec_msg->in_size,
		"VDEC_IOCTL_SET_CLK_RATE");
	if (copy_from_user(&performance_params, pvdec_msg->in, sizeof(performance_params))) {
		dprint(PRN_FATAL, "VDEC_IOCTL_SET_CLK_RATE: copy_from_user failed");
		return -EFAULT;
	}

	check_return(performance_params.base_clk_level < VDEC_CLK_RATE_LEVEL_MAX,
		"VDEC_IOCTL_SET_CLK_RATE, invalid prarameter");
	chan_id = performance_params.chan_id;
	if (chan_id < 0 || chan_id >= VFMW_CHAN_ID_MAX) {
		dprint(PRN_FATAL, "invalid chanid %d", chan_id);
		return -EFAULT;
	}

	vdec_mutex_lock(&g_vdec_entry.vdec_mutex);
	if (performance_params.is_release) {
		vdec_file->chanload[chan_id].load = 0;
		vdec_file->chanload[chan_id].base_clk_level = 0;
		vdec_file->chanload[chan_id].is_used = 0;
		vdec_file->chanload[chan_id].is_release = 1;
		vdec_file->chanload[chan_id].is_back_ground = 0;
	} else {
		vdec_file->chanload[chan_id].load = performance_params.load;
		vdec_file->chanload[chan_id].base_clk_level = performance_params.base_clk_level;
		vdec_file->chanload[chan_id].is_used = 1;
		vdec_file->chanload[chan_id].is_release = 0;
		vdec_file->chanload[chan_id].is_back_ground = performance_params.is_back_ground;
	}

	dprint(PRN_ALWS, "chanid %d load is %lu base_clk_level is %u is_release %u is_back_ground %d", chan_id,
		vdec_file->chanload[chan_id].load, vdec_file->chanload[chan_id].base_clk_level,
		vdec_file->chanload[chan_id].is_release, vdec_file->chanload[chan_id].is_back_ground);

	clk_level = get_clk_rate();
	target_clk_level = clk_level;
	vdec_plat_get_target_clk_rate(&target_clk_level);
	if (vdec_plat_regulator_set_clk_rate(target_clk_level) != 0) {
		vdec_mutex_unlock(&g_vdec_entry.vdec_mutex);
		return -EFAULT;
	}
	vdec_plat_set_static_clk_rate(clk_level);
	vdec_mutex_unlock(&g_vdec_entry.vdec_mutex);
	dprint(PRN_ALWS, "end VDEC_IOCTL_SET_CLK_RATE");

	return 0;
}

static uint32_t dec_hal_get_pid_info(void)
{
	uint32_t pid = 0;
	vdec_entry *entry = vdec_get_entry();
	pid = readl(entry->reg_base_vir_addr + VREG_OFS_DEC_STORE);

	return pid;
}

static bool vfmw_scene_ident(void)
{
	uint32_t ident_info = dec_hal_get_pid_info();
	if (ident_info != 0 && ident_info == (uint32_t)current->tgid)
		return VCODEC_TRUE;

	return VCODEC_FALSE;
}

int32_t handle_lock_hw(struct file *file_handle, vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	uint32_t msg;
	bool scene_ident_res;
	struct dec_file_handle *vdec_file = (struct dec_file_handle *)file_handle->private_data;
	vdec_entry* vdec = vdec_get_entry();
	check_return(vdec, "vdec is null");
	if (!vdec->power_state) {
		return -EIO;
	}

	if (!vdec_is_async_open_success(vdec)) {
		dprint(PRN_ERROR, "async open state is not success");
		return -EFAULT;
	}

	vdec_mutex_lock(&vdec->vdec_mutex_sec_scd);
	if (vdec->device_locked) {
		vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
		return -EIO;
	}
	scene_ident_res = vfmw_scene_ident();
	if (scene_ident_res == VCODEC_TRUE) {
		ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_LOCK_SCD_MSG, &msg, sizeof(uint32_t));
		if (ret) {
			dprint(PRN_ERROR, "%s : lock hw error", __func__);
			vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
			return -EIO;
		}
		vdec->device_locked = VCODEC_TRUE;
		vdec_file->hw_lock_flag = true;
	}

	dprint(PRN_DBG, "lock");
	vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
	return 0;
}

int32_t handle_unlock_hw(struct file *file_handle, vdec_ioctl_msg *vdec_msg)
{
	int32_t ret;
	uint32_t msg;
	bool scene_ident_res;
	struct dec_file_handle *vdec_file = (struct dec_file_handle *)file_handle->private_data;
	vdec_entry* vdec = vdec_get_entry();
	check_return(vdec, "vdec is null");
	if (!vdec->power_state) {
		return -EIO;
	}
	vdec_mutex_lock(&vdec->vdec_mutex_sec_scd);
	if (!vdec->device_locked) {
		vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
		return -EIO;
	}
	scene_ident_res = vfmw_scene_ident();
	if (scene_ident_res == VCODEC_TRUE) {
		ret = vdec->mcu_dev->control(vdec->mcu_dev, VCODEC_DEV_MCU_SEND_UNLOCK_SCD_MSG, &msg, sizeof(uint32_t));
		if (ret) {
			dprint(PRN_ERROR, "%s : unlock hw error", __func__);
			vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
			return -EIO;
		}
		vdec->device_locked = VCODEC_FALSE;
		vdec_file->hw_lock_flag = false;
	}

	dprint(PRN_DBG, "unlock");
	vdec_mutex_unlock(&vdec->vdec_mutex_sec_scd);
	return 0;
}

static int32_t vdec_subdev_suspend(void *arg)
{
	vdec_entry *vdec = (vdec_entry *)arg;
	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed\n");
		return -EFAULT;
	}

	tvp_vdec_suspend();
	(void)vdec_suspend_mcu();
	(void)vdec->scd_dev->suspend(vdec->scd_dev);
	if (vdec->async_open_state == OPENED_SUCCESS) {
		(void)vdec->pxp_dev->suspend(vdec->pxp_dev);
		(void)vdec->bsp_dev->suspend(vdec->bsp_dev);
	}

	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");

	return 0;
}

static int32_t vdec_subdev_resume(void *arg)
{
	int32_t ret = 0;
	vdec_entry *vdec = (vdec_entry *)arg;

	if (tvp_vdec_secure_init()) {
		dprint(PRN_FATAL, "tvp vdec init failed\n");
		return -EFAULT;
	}

	if (vdec->async_open_state == OPENED_SUCCESS) {
		ret = vdec->pxp_dev->resume(vdec->pxp_dev);
		vfmw_assert_goto_prnt(ret == 0, resume_pxp_error, "resume pxp error");
		ret = vdec->bsp_dev->resume(vdec->bsp_dev);
		vfmw_assert_goto_prnt(ret == 0, resume_bsp_error, "resume pxp error");
	}

	ret = vdec->scd_dev->resume(vdec->scd_dev);
	vfmw_assert_goto_prnt(ret == 0, resume_scd_error, "resume scd error");
	ret = vdec_resume_mcu();
	vfmw_assert_goto_prnt(ret == 0, resume_mcu_error, "resume mcu error");

	tvp_vdec_resume();
	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");

	return 0;

resume_mcu_error:
	(void)vdec->scd_dev->suspend(vdec->scd_dev);
resume_scd_error:
	if (vdec->async_open_state == OPENED_SUCCESS)
		(void)vdec->bsp_dev->suspend(vdec->bsp_dev);
resume_bsp_error:
	if (vdec->async_open_state == OPENED_SUCCESS)
		(void)vdec->pxp_dev->suspend(vdec->pxp_dev);
resume_pxp_error:
	if (tvp_vdec_secure_exit())
		dprint(PRN_FATAL, "tvp vdec exit failed\n");

	return ret;
}

static void vdec_suspend_process(void)
{
	vdec_plat *plt = vdec_plat_get_entry();
	vdec_entry *vdec = vdec_get_entry();
	void *work_node = NULL;
	unsigned long flag;

	spin_lock_irqsave(&vdec->power_state_spin_lock, flag);
	vdec->power_state = false;
	spin_unlock_irqrestore(&vdec->power_state_spin_lock, flag);

	work_node = vcodec_work_submit(vdec->work_ctx, vdec_subdev_suspend, vdec);
	if (work_node)
		vcodec_work_finish(work_node);
	else
		dprint(PRN_FATAL, "submit suspend sub device work fail");

	smmu_deinit();
	/* SR need retention bsp/pxp itcm img */
	power_down_vdec_cal(true);
	power_down_vdec_top_mcu();
	plt->clk_ctrl.clk_flag = 0;
}

static int32_t vdec_resume_process(void)
{
	int32_t ret;
	vdec_entry *vdec = vdec_get_entry();
	uint32_t clk_level = VDEC_CLK_RATE_LEVEL_MAX;
	vdec_plat *plt = vdec_plat_get_entry();
	void *work_node = NULL;
	unsigned long flag;

	ret = power_up_vdec_top();
	check_return(ret == 0, "power on vdec top failed");

	ret = power_up_vdec_cal();
	vfmw_assert_goto_prnt(ret == 0, power_up_vdec_error, "power_up_vdec_cal failed\n");

	ret = smmu_init();
	vfmw_assert_goto_prnt(ret == 0, smmu_init_error, "smmu init failed\n");

	work_node = vcodec_work_submit(vdec->work_ctx, vdec_subdev_resume, vdec);
	vfmw_assert_goto_prnt(work_node != NULL, resume_subdev_error, "submit subdev resume work fail");
	ret = vcodec_work_finish(work_node);
	vfmw_assert_goto_prnt(ret == 0, resume_subdev_error, "subdev resume fail");

	spin_lock_irqsave(&vdec->power_state_spin_lock, flag);
	vdec->power_state = true;
	spin_unlock_irqrestore(&vdec->power_state_spin_lock, flag);
	vdec_plat_get_target_clk_rate(&clk_level);
	dprint(PRN_ALWS, "resume clk rate %u", clk_level);
	vdec_plat_regulator_set_clk_rate(clk_level);
	plt->clk_ctrl.clk_flag = 1;

	return 0;

resume_subdev_error:
	smmu_deinit();
smmu_init_error:
	power_down_vdec_cal(true);
power_up_vdec_error:
	power_down_vdec_top_mcu();
	return -EFAULT;
}

static fn_ioctl_handler vdec_ioctl_get_handler(uint32_t cmd)
{
	const ioctl_command_node * node = vcodec_vdec_get_ioctl_tbl();

	dprint(PRN_DBG, "recieved cmd %x", cmd);
	while (node->cmd) {
		if (cmd == node->cmd) {
			return node->handle;
		}
		node++;
	}

	return NULL;
}

static long vdec_ioctl_common(struct file *file_handle, unsigned int cmd,
	unsigned long arg, compat_type_e type)
{
	int32_t ret;
	long ioctl_ret;
	vdec_ioctl_msg vdec_msg;
	void *u_arg = (void *)(uintptr_t)arg;
	fn_ioctl_handler ioctl_handler = VCODEC_NULL;
	vdec_entry *vdec = vdec_get_entry_by_file_handle(file_handle);

	check_return(vdec, "vdec is null");

	(void)memset_s(&vdec_msg, sizeof(vdec_msg), 0, sizeof(vdec_msg));

	if ((cmd != VDEC_IOCTL_UNLOCK_HW) && (cmd != VDEC_IOCTL_LOCK_HW)) {
		check_return(u_arg != VCODEC_NULL, "arg is null");
		ret = vdec_compat_get_data(type, u_arg, &vdec_msg);
		check_return(ret == 0, "compat data get failed");
	}

	ioctl_handler = vdec_ioctl_get_handler(cmd);
	if (ioctl_handler == VCODEC_NULL) {
		dprint(PRN_ERROR, "error cmd %d is not supported\n", _IOC_NR(cmd));
		return -ENOTTY;
	}

	ioctl_ret = ioctl_handler(file_handle, &vdec_msg);
	return ioctl_ret;
}

static long vdec_ioctl(struct file *file_handle, unsigned int cmd,
	unsigned long arg)
{
	return vdec_ioctl_common(file_handle, cmd, arg, T_IOCTL_ARG);
}

#ifdef CONFIG_COMPAT
static long vdec_compat_ioctl(struct file *file_handle, unsigned int cmd,
	unsigned long arg)
{
	void *user_ptr = compat_ptr(arg);

	return vdec_ioctl_common(file_handle, cmd, (unsigned long)(uintptr_t)user_ptr, T_IOCTL_ARG_COMPAT);
}
#endif


static const struct file_operations g_vdec_fops = {
	.owner = THIS_MODULE,
	.open = vdec_open,
	.unlocked_ioctl = vdec_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl = vdec_compat_ioctl,
#endif
	.release = vdec_close,
};

struct platform_device *g_dec_pdev;
struct platform_device *vdec_get_device(void)
{
	return g_dec_pdev;
}

void vdec_set_device(struct platform_device *dev)
{
	g_dec_pdev = dev;
}

static int32_t vdec_probe(struct platform_device *pltdev)
{
	vdec_entry *vdec = NULL;
	vcodec_osal_intf_init();

	if (vdec_device_probe(pltdev)) {
		dprint(PRN_FATAL, "vdec device probe failed");
		return -EFAULT;
	}

	if (vdec_mem_probe(&pltdev->dev)) {
		dprint(PRN_ERROR, "vdec memory probe failed");
		return -EFAULT;
	}

	platform_set_drvdata(pltdev, VCODEC_NULL);
	(void)memset_s(&g_vdec_entry, sizeof(vdec_entry), 0, sizeof(vdec_entry));
	vdec_init_mutex(&g_vdec_entry.vdec_mutex);
	vdec_init_mutex(&g_vdec_entry.vdec_mutex_scd);
	vdec_init_mutex(&g_vdec_entry.vdec_mutex_sec_scd);
	vdec_init_mutex(&g_vdec_entry.vdec_pci_mutex);

	if (vdec_setup_cdev(&g_vdec_entry, &g_vdec_fops)) {
		dprint(PRN_FATAL, "vdec_setup_cdev failed");
		return -EFAULT;
	}

	g_vdec_entry.device = &pltdev->dev;
	platform_set_drvdata(pltdev, &g_vdec_entry);
	vdec_set_device(pltdev);

	if (vdec_plat_init(pltdev)) {
		dprint(PRN_FATAL, "vdec plat init failed");
		goto init_plat_error;
	}

	vdec_init_setting_info(&g_vdec_entry.setting_info);

	if (vdec_create_sub_dev())
		goto create_sub_dev_error;

	dprint(PRN_ALWS, "vdec probe success");
	vdec = vdec_get_entry();
	vdec->root_ctx = vcodec_work_ctx_create("vdec_root_task");
	if (!vdec->root_ctx)
		dprint(PRN_ERROR, "vdec create vdec root work ctx failed");
	return 0;

create_sub_dev_error:
	vdec_plat_deinit();
init_plat_error:
	vdec_cleanup_cdev(&g_vdec_entry);

	return -EFAULT;
}

static int32_t vdec_remove(struct platform_device *pltdev)
{
	vdec_entry *vdec = VCODEC_NULL;

	vdec = platform_get_drvdata(pltdev);
	if (IS_ERR_OR_NULL(vdec)) {
		dprint(PRN_ERROR, "errno = %ld\n", PTR_ERR(vdec));
		return -EFAULT;
	}
	vdec_free_mcu_logbuffer();
	vdec_destroy_sub_dev();
	vcodec_work_ctx_destroy(vdec->root_ctx);
	if (vdec->work_ctx != NULL) {
		vcodec_work_ctx_destroy(vdec->work_ctx);
		vdec->work_ctx = NULL;
	}
	vdec_cleanup_cdev(vdec);
	vdec_plat_deinit();

	platform_set_drvdata(pltdev, VCODEC_NULL);

	dprint(PRN_ALWS, "remove vdec success\n");
	return 0;
}

static int32_t vdec_suspend(struct platform_device *pltdev, pm_message_t state)
{
	dprint(PRN_ALWS, "+\n");
	vdec_mutex_lock(&g_vdec_entry.vdec_mutex);

	if ((g_is_normal_init != 0) && (g_vdec_entry.power_state))
		vdec_suspend_process();
	else
		dprint(PRN_ALWS, "vdec already power off\n");

	vdec_mutex_unlock(&g_vdec_entry.vdec_mutex);

	dprint(PRN_ALWS, "-\n");

	return 0;
}

static int32_t vdec_resume(struct platform_device *pltdev)
{
	dprint(PRN_ALWS, "+\n");
	vdec_mutex_lock(&g_vdec_entry.vdec_mutex);
	if ((g_is_normal_init != 0) && (!g_vdec_entry.power_state)) {
		if (vdec_resume_process()) {
			vdec_mutex_unlock(&g_vdec_entry.vdec_mutex);
			dprint(PRN_ALWS, "failed.\n");
			return -EFAULT;
		}
	}

	vdec_mutex_unlock(&g_vdec_entry.vdec_mutex);

	dprint(PRN_ALWS, "-\n");

	return 0;
}

void vdec_tcu_disconnect(void)
{
	smmu_disconnect();
}

int32_t vdec_tcu_connect(void)
{
	if (smmu_connect() != 0) {
		dprint(PRN_ERROR, "smmu_init fail");
		return -EFAULT;
	}
	return 0;
}

void vdec_chan_state_clear(void)
{
	vdec_entry *vdec = vdec_get_entry();
	int32_t index;

	for (index = 0; index < VFMW_CHAN_NUM; ++index) {
		vdec->vdec_chan_state[index] = VFMW_CID_MAX;
	}
}

static const struct of_device_id g_vdec_dt_match[] = {
	{ .compatible = "hisilicon,VCodec-vdec", },
	{ }
};

MODULE_DEVICE_TABLE(of, g_vdec_dt_match);

static struct platform_driver g_vdec_driver = {
	.probe = vdec_probe,
	.remove = vdec_remove,
	.suspend = vdec_suspend,
	.resume = vdec_resume,
	.driver = {
		.name = (char *)g_vdec_drv_name,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(g_vdec_dt_match),
	},
};

module_platform_driver(g_vdec_driver)

MODULE_DESCRIPTION("vdec driver");
MODULE_LICENSE("GPL");
