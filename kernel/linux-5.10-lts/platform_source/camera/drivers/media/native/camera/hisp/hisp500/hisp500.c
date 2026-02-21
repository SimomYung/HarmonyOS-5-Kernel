// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: platform config file for hisp500
 * Create: 2023-04-10
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 */

#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/rpmsg.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fh.h>
#include <media/v4l2-subdev.h>
#include <media/videobuf2-core.h>
#include <linux/pm_qos.h>
#include <clocksource/arm_arch_timer.h>
#include <asm/arch_timer.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/pm_wakeup.h>
#include <linux/iommu/mm_iommu.h>
#include <platform_include/isp/linux/hisp_remoteproc.h>
#include <platform_include/isp/linux/hisp_mempool.h>
#include <platform_include/isp/linux/hoisp.h>
#include <linux/iommu.h>
#include <linux/mutex.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/smart/linux/sdc.h>
#include <platform_include/camera/native/camera.h>
#include <platform_include/camera/native/hisp500_cfg.h>
#include <cam_buf.h>
#include "cam_log.h"
#include "hisp_intf.h"
#include "isp_msg_pool.h"
#include "platform/sensor_commom.h"
#include <platform_include/see/efuse_driver.h>

#if defined(CONFIG_SOC_PG_INFO) && !defined(SCOUT)
#include "soc_spec_info.h"
#include <platform_include/basicplatform/linux/soc_spec.h>
#endif


#define HISP_MSG_LOG_MOD 100

#define TIMEOUT_IS_FPGA_BOARD 500000
#define TIMEOUT_IS_NOT_FPGA_BOARD 15000

#define EFUSE_CHIP_VERSION_START_BIT 0
#define EFUSE_CHIP_VERSION_BIT_CNT 2

DEFINE_MUTEX(kernel_rpmsg_service_mutex);
DEFINE_MUTEX(kernel_fd_sgt_map_mutex);
static DEFINE_MUTEX(hisp_pinctrl_lock_mutex);

static struct wakeup_source *hisp_power_wakelock = NULL;
static struct mutex hisp_wake_lock_mutex;
static struct mutex hisp_power_lock_mutex;
static struct mutex hisp_mem_lock_mutex;
extern void hisp_boot_stat_dump(void);

#ifdef CONFIG_THERMAL_ISP_HOTPLUG
extern void set_block_isp_cb(int (*block_isp_cb)(bool));
static int handle_block_isp(bool block);
#endif

static void hisp500_deinit_isp_mem(void);

enum hisp500_chip_version : uint32_t {
	CHIP_VERSION_ES2 = 0,
	CHIP_VERSION_CS,
};

enum hisp500_mem_pool_attr
{
	MEM_POOL_ATTR_READ_WRITE_CACHE = 0,
	MEM_POOL_ATTR_READ_WRITE_SECURITY,
	MEM_POOL_ATTR_READ_WRITE_ISP_SECURITY,
	MEM_POOL_ATTR_READ_WRITE_CACHE_OFF_LINE,
	MEM_POOL_ATTR_MAX,
};

struct hisp500_mem_pool {
	unsigned int prot;
	unsigned long long cpu_addr;
	size_t size;
	size_t align_size;
	int active;
	unsigned int security_isp_mode;
	struct sg_table *sgt;
	unsigned int shared_fd;
	struct hisp_mem_type type;
};

struct isp_mem {
	int active;
	struct dma_buf *dmabuf;
};

struct hisp_fd_map {
	int shared_fd;
	unsigned long long cpu_addr;
	size_t size;
	struct hisp_mem_type type;
	struct sg_table *sgt;
	struct list_head nd;
	struct hisp_mem_info_t mem_info;
};

/*
 * @brief the instance to talk to hisp driver
 * When Histar ISP is probed, this sturcture will be initialized,
 * the object is used to notify hisp driver when needed.
 * @end
 */
typedef struct _tag_hisp500 {
	hisp_intf_t intf;
	hisp_notify_intf_t *notify;
	char const *name;
	atomic_t opened;
	struct platform_device *pdev; /* by used to get dts node */
	hisp_dt_data_t dt;
	struct hisp500_mem_pool mem_pool[MEM_POOL_ATTR_MAX];
	struct isp_mem mem;
	struct list_head fd_sgt_list;
} hisp500_t;

struct rpmsg_service_info {
	struct mutex sendlock;
	struct rpmsg_device *rpdev;
	struct isp_msg_pool *msg_pool[MAX_HISP_CLIENT];
	struct completion isp_comp;
};

static struct rpmsg_service_info rpmsg_local;
volatile static bool remote_processor_up[MAX_HISP_CLIENT];
volatile static int g_hisp_ref = 0;
static bool g_block_isp = false;
static bool g_hisp_probed = false;
static uint32_t g_chip_type = CAM_CT_UNKNOWN;
static bool g_has_oisp = false;
#define to_isp(i) container_of(i, hisp500_t, intf)

static void hisp500_notify_rpmsg_cb(uint8_t client_id);
static char const *hisp500_get_name(hisp_intf_t *i);
static int hisp500_config(hisp_intf_t *i, void *cfg);

static int hisp500_power_on(hisp_intf_t *i);
static int hisp500_power_off(hisp_intf_t *i);

static int hisp500_open(hisp_intf_t *i);
static int hisp500_close(hisp_intf_t *i);
static int hisp500_send_rpmsg(hisp_intf_t *i,
	struct isp_indirect_msg *msg, size_t len);
static int hisp500_recv_rpmsg(hisp_intf_t *i,
	struct isp_indirect_msg *msg, size_t len);

static int hisp500_set_sec_fw_buffer(struct hisp_cfg_data *cfg);
static int hisp500_release_sec_fw_buffer(void);
static void hisp500_get_chip_type(struct platform_device *pdev);


static bool hisp500_set_remote_processor_shutdown(hisp_intf_t *i)
{
	int client_id = hisp_get_client_id(current->tgid);
	if (i == NULL)
		return false;

	if (client_id < 0 || client_id >= MAX_HISP_CLIENT) {
		cam_warn("%s is not remote processor owner", __func__);
		return false;
	}

	/* check the remote processor boot flow */
	remote_processor_up[client_id] = false;

	return true;
}

static uint8_t hisp500_remote_processor_up(void)
{
	uint8_t up_count = 0;
	unsigned int i;
	for (i = 0; i < MAX_HISP_CLIENT; ++i)
		if (remote_processor_up[i])
			++up_count;
	return up_count;
}

static void hisp500_remote_processor_up_reset(void)
{
	unsigned int i;
	for (i = 0; i < MAX_HISP_CLIENT; ++i)
		remote_processor_up[i] = false;
}


bool modify_ack_timestamp_in_user(void)
{
	return true;
}

static hisp_vtbl_t s_vtbl_hisp500 = {
	.get_name = hisp500_get_name,
	.config = hisp500_config,
	.send_rpmsg = hisp500_send_rpmsg,
	.recv_rpmsg = hisp500_recv_rpmsg,
	.open = hisp500_open,
	.close = hisp500_close,
};

static hisp500_t s_hisp500 = {
	.intf = {.vtbl = &s_vtbl_hisp500,},
	.name = "hisp500",
};

static uint8_t get_client_id_by_msg_id(uint32_t message_id)
{
	return message_id >> HISP_MESSAGE_ID_CLIENT_BIT;
}
static void hisp500_notify_rpmsg_cb(uint8_t client_id)
{
	hisp_event_t isp_ev;
	isp_ev.kind = HISP_RPMSG_CB | (client_id << HISP_MESSAGE_ID_CLIENT_BIT);
	hisp_notify_intf_rpmsg_cb(s_hisp500.notify, &isp_ev);
}

/* Function declaration */
/**********************************************
 * Power up CSI/DPHY/sensor according to isp req
 * Only called by hisp500_rpmsg_ept_cb when api_name
 * is POWER_REQ, and will send a POWER_RSP to isp
 * after power request done.
 *********************************************/
static int hisp500_rpmsg_ept_cb(struct rpmsg_device *rpdev,
	void *data, int len, void *priv, u32 src)
{
	int rc;
	unsigned int i;

	(void)src;
	(void)rpdev;
	(void)priv;
	if (!data || len <= 0) {
		cam_err("func %s: data NULL or len:%d", __func__, len);
		return -EINVAL;
	}

	uint8_t client_id = get_client_id_by_msg_id(((struct isp_msg_header *)data)->message_id);
	((struct isp_msg_header *)data)->message_id &= (1 << HISP_MESSAGE_ID_CLIENT_BIT) - 1;
	unsigned api_name = ((struct isp_msg_header *)data)->api_name;
	if (client_id >= MAX_HISP_CLIENT) {
		cam_err("func %s: client id is %d, large than %d", __func__, client_id, MAX_HISP_CLIENT);
		return -EINVAL;
	}

	hisp_recvin(data);
	rc = queue_isp_msg(rpmsg_local.msg_pool[client_id], data, len);
	if (rc == 0) {
		hisp_recvx(data); // data saved to msg queue
		hisp500_notify_rpmsg_cb(client_id);
	}

#define MSG_EVENT_SENT 0x3000
	if (api_name == MSG_EVENT_SENT) {
		cam_debug("broadcast event to all client");
		int client_ids[MAX_HISP_CLIENT];
		unsigned client_num = hisp_get_current_client_ids(client_ids);
		for (i = 0; i < client_num; ++i)
			if (client_ids[i] != client_id && client_ids[i] >= 0 && client_ids[i] < MAX_HISP_CLIENT)
				if (queue_isp_msg(rpmsg_local.msg_pool[client_ids[i]], data, len) == 0)
					hisp500_notify_rpmsg_cb(client_ids[i]);
	}

	return rc;
}

static char const *hisp500_get_name(hisp_intf_t *itf)
{
	hisp500_t *isp = NULL;

	if (!itf) {
		cam_err("func %s: itf is NULL", __func__);
		return NULL;
	}

	isp = to_isp(itf);
	if (!isp) {
		cam_err("func %s: isp is NULL", __func__);
		return NULL;
	}
	return isp->name;
}

static int buffer_is_invalid(int share_fd, unsigned int req_addr,
	unsigned int req_size)
{
	int ret;
	struct iommu_format fmt = {0};

	ret = cam_buf_map_iommu(share_fd, &fmt);
	if (ret < 0) {
		cam_err("%s: fail to map iommu", __func__);
		return ret;
	}

	if (req_addr != fmt.iova || req_size > fmt.size) {
		cam_err("%s: req_iova:%#x,  req_size:%u",
			__func__, req_addr, req_size);
		cam_err("%s:real_iova:%#llx, real_size:%llu",
			__func__, fmt.iova, fmt.size);
		ret = -ERANGE;
	}
	cam_buf_unmap_iommu(share_fd, &fmt);
	return ret;
}

static int find_suitable_mem_pool(struct hisp_cfg_data *pcfg)
{
	int ipool;
	if (pcfg->param.type == MAP_TYPE_RAW2YUV) {
		ipool =  MEM_POOL_ATTR_READ_WRITE_CACHE_OFF_LINE;
	} else if (pcfg->param.type == MAP_TYPE_STATIC_ISP_SEC) {
		ipool =  MEM_POOL_ATTR_READ_WRITE_ISP_SECURITY;
	} else {
		for (ipool = 0; ipool < MEM_POOL_ATTR_MAX; ipool++) {
			if (s_hisp500.mem_pool[ipool].prot ==
				pcfg->param.prot) {
				break;
			}
		}

		if (ipool >= MEM_POOL_ATTR_MAX) {
			cam_err("func %s: no pool hit for prot:%d",
				__func__, pcfg->param.prot);
			return -EINVAL;
		}
	}
	return ipool;
}

#if defined(CONFIG_SENSOR_DIRECT_CHANNEL)
static struct sdc_mem_info *imu_info = NULL;
static imu_addr_param_t imu_map_param = {0};
static unsigned int imu_map_ref = 0;
static int hisp500_init_imu_buffer(void *cfg)
{
	int ret = 0;
	struct hisp_cfg_data *pcfg = NULL;
	struct hisp_mem_type type = { 0 };
	struct hisp_map_info_s map_info = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };
	if (cfg == NULL) {
		cam_err("%s: cfg is NULL", __FUNCTION__);
		return -EINVAL;
	}
	mutex_lock(&kernel_rpmsg_service_mutex);
	pcfg = (struct hisp_cfg_data*)cfg;
	if (imu_map_ref != 0) {
		pcfg->imu_param.iova = imu_map_param.iova;
		pcfg->imu_param.size = imu_map_param.size;
		imu_map_ref++;
		cam_info("imu already mapped ref = %d", imu_map_ref);
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return ret;
	}
	imu_info = sdc_buf_get();
	if (imu_info == NULL) {
		cam_err("%s: sdc buffer get fail", __FUNCTION__);
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return -EINVAL;
	}

	type.master = ISP_CPU;
	type.mem_type = ISP_PHY_MEM;
	map_info.size = imu_info->size;
	map_info.paddr = imu_info->phy_addr;
	map_info.prot = IOMMU_READ;

	ret = hisp_mem_map_addr(type, &map_info, &mem_info);
	if (!ret && mem_info.cpu_addr != 0) {
		pcfg->imu_param.iova = mem_info.cpu_addr;
		pcfg->imu_param.size = mem_info.cpu_size;
		imu_map_param.iova = mem_info.cpu_addr;
		imu_map_param.size = mem_info.cpu_size;
		imu_map_param.usage = pcfg->imu_param.usage;
		imu_map_ref++;
		cam_info("%s: iova = %x, size = %d", __FUNCTION__, pcfg->imu_param.iova, pcfg->imu_param.size);
	} else {
		ret = -ENOMEM;
		cam_err("%s: hisp_mem_map_addr failed", __FUNCTION__);
	}
	mutex_unlock(&kernel_rpmsg_service_mutex);
	return ret;
}

static int hisp500_deinit_imu_buffer(void *cfg)
{
	int ret = 0;
	struct hisp_cfg_data *pcfg = NULL;
	struct hisp_mem_info_s mem_info = { 0 };
	struct hisp_mem_type type = { 0 };

	if (cfg == NULL) {
		cam_err("func %s: cfg is NULL", __FUNCTION__);
		return -EINVAL;
	}
	mutex_lock(&kernel_rpmsg_service_mutex);
	if (imu_map_ref == 0) {
		cam_info("imu is not mapped");
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return -EINVAL;
	}
	if (imu_map_ref > 1) {
		imu_map_ref--;
		cam_info("imu already mapped ref = %d", imu_map_ref);
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return ret;
	}
	pcfg = (struct hisp_cfg_data*)cfg;
	cam_info("%s: iova = %x, size = %d", __FUNCTION__, pcfg->imu_param.iova, pcfg->imu_param.size);
	if ((pcfg->imu_param.iova != imu_map_param.iova) || (pcfg->imu_param.size != imu_map_param.size)) {
		cam_err("deinit param error");
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return -EINVAL;
	}
	mem_info.cpu_addr = pcfg->imu_param.iova;
	mem_info.cpu_size = pcfg->imu_param.size;
	type.master = ISP_CPU;
	type.mem_type = ISP_PHY_MEM;
	ret = hisp_mem_unmap_addr(type, mem_info);
	if (ret != 0) {
		cam_err("%s: hisp_mem_unmap_addr failed", __FUNCTION__);
		ret = -EINVAL;
	}
	imu_map_ref = 0;
	imu_map_param.iova = 0;
	imu_map_param.size = 0;
	sdc_buf_put(imu_info);
	imu_info = NULL;
	mutex_unlock(&kernel_rpmsg_service_mutex);
	return ret;
}
#endif

static int hisp500_init_r8isp_memory_pool(void *cfg)
{
	int ipool;
	int ret;
	struct hisp_cfg_data *pcfg = NULL;
	struct sg_table *sgt = NULL;
	struct hisp_mem_type type = { 0 };
	struct hisp_map_info_s map_info = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };

	if (cfg == NULL) {
		cam_err("%s: cfg is NULL", __func__);
		return -EINVAL;
	}

	pcfg = (struct hisp_cfg_data*)cfg;
	cam_info("%s: pool cfg vaddr=0x%pK, iova=0x%x, size=0x%x", __func__,
		pcfg->param.vaddr, pcfg->param.iova, pcfg->param.size);
	cam_info("%s: type=%d, prot=0x%x align=0x%zx sec=0x%x", __func__,
		pcfg->param.type, pcfg->param.prot,
		pcfg->param.pool_align_size, pcfg->param.security_isp_mode);

	if (!pcfg->param.security_isp_mode &&
		buffer_is_invalid(pcfg->param.sharedFd,
		pcfg->param.iova, pcfg->param.size)) {
		cam_err("check buffer fail");
		return -EINVAL;
	}

	ipool = find_suitable_mem_pool(pcfg);
	if (ipool < 0)
		return ipool;

	/* take care of putting sgtable. */
	sgt = cam_buf_get_sgtable(pcfg->param.sharedFd);
	if (IS_ERR_OR_NULL(sgt)) {
		cam_err("%s: fail to get sgtable", __func__);
		return -ENOENT;
	}

	mutex_lock(&kernel_rpmsg_service_mutex);

	if (pcfg->param.type == MAP_TYPE_DYNAMIC_SEC ||
		pcfg->param.type == MAP_TYPE_STATIC_SEC ||
		pcfg->param.type == MAP_TYPE_STATIC_ISP_SEC) {
		map_info.fd = pcfg->param.sharedFd;
		map_info.size = pcfg->param.size;
		map_info.prot = pcfg->param.prot;

		ret = hisp_mem_map_addr_sec((unsigned int)ipool,
			&map_info, &mem_info);
		if (ret != 0) {
			cam_err("%s: hisp_mem_map_addr_sec failed", __func__);
			mutex_unlock(&kernel_rpmsg_service_mutex);
			cam_buf_put_sgtable(sgt);
			return -ENOMEM;
		}

		pcfg->param.iova = mem_info.cpu_addr;
		/* hold sg_table things, release at deinit. */
		s_hisp500.mem_pool[ipool].sgt = sgt;
		s_hisp500.mem_pool[ipool].cpu_addr = mem_info.cpu_addr;
		s_hisp500.mem_pool[ipool].size = pcfg->param.size;
		s_hisp500.mem_pool[ipool].align_size = pcfg->param.pool_align_size;
		s_hisp500.mem_pool[ipool].security_isp_mode =
			pcfg->param.security_isp_mode;
		s_hisp500.mem_pool[ipool].shared_fd = pcfg->param.sharedFd;
		s_hisp500.mem_pool[ipool].type = type;
		/*
		* ion iova isn't equal r8 iova, security or unsecurity, align etc
		* return r8 iova to daemon, and send to r8 later
		*/
		s_hisp500.mem_pool[ipool].active = 1;

		cam_info("%s: r8_iova_pool_base=%#llx",
			__func__, s_hisp500.mem_pool[ipool].cpu_addr);
	}

	mutex_unlock(&kernel_rpmsg_service_mutex);
	return 0;
}

static int hisp500_deinit_r8isp_memory_pool(void *cfg)
{
	int ipool;
	struct hisp_cfg_data *pcfg = NULL;
	int ret;
	unsigned int prot;

	if (cfg == NULL) {
		cam_err("func %s: cfg is NULL", __func__);
		return -EINVAL;
	}

	ipool = find_suitable_mem_pool(cfg);
	if (ipool < 0)
		return ipool;

	mutex_lock(&kernel_rpmsg_service_mutex);
	if (s_hisp500.mem_pool[ipool].active) {
		s_hisp500.mem_pool[ipool].active = 0;
		pcfg = (struct hisp_cfg_data*)cfg;
		cam_info("%s: hisp_mem_unmap_addr_sec ipool=%d", __func__, (unsigned int)ipool);

		if (pcfg->param.type == MAP_TYPE_DYNAMIC_SEC ||
			pcfg->param.type == MAP_TYPE_STATIC_SEC ||
			pcfg->param.type == MAP_TYPE_STATIC_ISP_SEC) {
			ret = hisp_mem_unmap_addr_sec((unsigned int)ipool);
			if (ret != 0)
				cam_err("%s: hisp_secmem_ca_unmap failed",
					__func__);
		}

		/* release sg_table things. */
		cam_buf_put_sgtable(s_hisp500.mem_pool[ipool].sgt);
	}
	prot = s_hisp500.mem_pool[ipool].prot; // prot should not be memset to 0
	ret = memset_s(&(s_hisp500.mem_pool[ipool]), sizeof(struct hisp500_mem_pool),
		0, sizeof(struct hisp500_mem_pool));
	if (ret != 0) {
		cam_err("memset_s failed %d", __LINE__);
		mutex_unlock(&kernel_rpmsg_service_mutex);
		return -EINVAL;
	}
	s_hisp500.mem_pool[ipool].prot = prot;
	mutex_unlock(&kernel_rpmsg_service_mutex);

	return 0;
}

static int hisp500_alloc_isp_cpu_addr(void *cfg)
{
	struct hisp_cfg_data *pcfg = NULL;
	struct sg_table *sgt = NULL;
	struct hisp_fd_map *fdmap = NULL;
	struct hisp_mem_type type = { 0 };
	struct hisp_map_info_s map_info = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };
	int ret;

	if (cfg == NULL) {
		cam_err("%s: cfg is NULL", __func__);
		return -EINVAL;
	}

	fdmap = (struct hisp_fd_map*)kzalloc(sizeof(struct hisp_fd_map),
		GFP_KERNEL);
	if (fdmap == NULL) {
		cam_err("%s: alloc fd map list node error", __func__);
		return -EINVAL;
	}

	pcfg = (struct hisp_cfg_data*)cfg;
	sgt = cam_buf_get_sgtable(pcfg->param.sharedFd);
	if (IS_ERR_OR_NULL(sgt)) {
		cam_err("%s: fail to get sgtable", __func__);
		kfree(fdmap);
		return -ENOENT;
	}

	mutex_lock(&kernel_rpmsg_service_mutex);
	cam_info("%s: size=%x, prot=0x%x align=0x%zx, usage=%d", __func__,
		pcfg->param.size,
		pcfg->param.prot, pcfg->param.pool_align_size,
		pcfg->param.usage);

	type.master = pcfg->param.mem_master;
	type.mem_type = ISP_SHARE_MEM;
	map_info.fd = pcfg->param.sharedFd;
	map_info.size = pcfg->param.size;
	map_info.prot = pcfg->param.prot;

	ret = hisp_mem_map_addr(type, &map_info, &mem_info);
	if (ret != 0) {
		cam_err("%s: hisp_alloc_cpu_map_addr failed", __func__);
		mutex_unlock(&kernel_rpmsg_service_mutex);
		cam_buf_put_sgtable(sgt);
		kfree(fdmap);
		return ret;
	}
	pcfg->param.iova = mem_info.cpu_addr;
	// /* AP alloc, ion info */
	fdmap->sgt = sgt;
	fdmap->shared_fd = (int)pcfg->param.sharedFd;
	fdmap->cpu_addr = mem_info.cpu_addr;
	fdmap->size = pcfg->param.size;
	fdmap->type = type;
	fdmap->mem_info.cpu_addr = mem_info.cpu_addr;
	fdmap->mem_info.cpu_size = pcfg->param.size;

	if (pcfg->param.map_info.fw_mem_size != 0) {
		type.master = ISP_CPU;
		type.mem_type = ISP_PRIVATE_MEM;
		map_info.fd = -1;
		map_info.size = pcfg->param.map_info.fw_mem_size;
		map_info.prot = pcfg->param.map_info.fw_mem_prot;
		ret = hisp_mem_map_addr(type, &map_info, &mem_info);
		if (ret == 0) {
			pcfg->param.mem_info.fw_mem.cpu_addr = mem_info.cpu_addr;
			pcfg->param.mem_info.fw_mem.size = mem_info.cpu_size;
			fdmap->mem_info.fw_mem.cpu_addr = pcfg->param.mem_info.fw_mem.cpu_addr;
			fdmap->mem_info.fw_mem.size = pcfg->param.mem_info.fw_mem.size;
		} else {
			cam_err("%s: hisp_alloc_cpu_map_addr failed", __func__);
		}
	}

	if (pcfg->param.map_info.ispfw_mem_size != 0) {
		type.master = ISP_CPU | ISP_CORE;
		type.mem_type = ISP_PRIVATE_MEM;
		map_info.fd = -1;
		map_info.size = pcfg->param.map_info.ispfw_mem_size;
		map_info.prot = pcfg->param.map_info.ispfw_mem_prot;
		ret = hisp_mem_map_addr(type, &map_info, &mem_info);
		if (ret == 0) {
			pcfg->param.mem_info.ispfw_mem.cpu_addr = mem_info.cpu_addr;
			pcfg->param.mem_info.ispfw_mem.size = mem_info.cpu_size;
			fdmap->mem_info.ispfw_mem.cpu_addr = pcfg->param.mem_info.ispfw_mem.cpu_addr;
			fdmap->mem_info.ispfw_mem.size = pcfg->param.mem_info.ispfw_mem.size;
		} else {
			cam_err("%s: hisp_alloc_cpu_map_addr failed", __func__);
		}
	}
	mutex_unlock(&kernel_rpmsg_service_mutex);

	mutex_lock(&kernel_fd_sgt_map_mutex);
	cam_info("%s: add list shared_fd %d", __func__, fdmap->shared_fd);
	list_add(&fdmap->nd, &s_hisp500.fd_sgt_list);
	mutex_unlock(&kernel_fd_sgt_map_mutex);
	return 0;
}

static int hisp500_free_isp_cpu_addr(void *cfg)
{
	struct hisp_cfg_data *pcfg = NULL;
	int ret;
	struct hisp_fd_map *fdmap = NULL;
	struct hisp_fd_map *n = NULL;
	struct hisp_mem_type type = { 0 };
	struct hisp_mem_info_s mem_info = { 0 };

	if (cfg == NULL) {
		cam_err("func %s: cfg is NULL", __func__);
		return -EINVAL;
	}

	mutex_lock(&kernel_rpmsg_service_mutex);
	pcfg = (struct hisp_cfg_data*)cfg;
	cam_info("%s: size=%x", __func__, pcfg->param.size);
	type.master = pcfg->param.mem_master;
	type.mem_type = ISP_SHARE_MEM;
	mem_info.cpu_addr = pcfg->param.iova;
	mem_info.cpu_size = pcfg->param.size;
	ret = hisp_mem_unmap_addr(type, mem_info);
	if (ret != 0)
		cam_err("%s: hisp_mem_unmap_addr failed", __func__);

	if (pcfg->param.map_info.fw_mem_size != 0) {
		type.master = ISP_CPU;
		type.mem_type = ISP_PRIVATE_MEM;
		mem_info.cpu_addr = pcfg->param.mem_info.fw_mem.cpu_addr;
		mem_info.cpu_size = pcfg->param.mem_info.fw_mem.size;
		ret = hisp_mem_unmap_addr(type, mem_info);
		if(ret != 0)
			cam_err("%s: hisp_mem_unmap_addr failed", __func__);
	}
	if (pcfg->param.map_info.ispfw_mem_size != 0) {
		type.master = ISP_CPU | ISP_CORE;
		type.mem_type = ISP_PRIVATE_MEM;
		mem_info.cpu_addr = pcfg->param.mem_info.ispfw_mem.cpu_addr;
		mem_info.cpu_size = pcfg->param.mem_info.ispfw_mem.size;
		ret = hisp_mem_unmap_addr(type, mem_info);
		if(ret != 0)
			cam_err("%s: hisp_mem_unmap_addr failed", __func__);
	}
	mutex_unlock(&kernel_rpmsg_service_mutex);

	mutex_lock(&kernel_fd_sgt_map_mutex);
	list_for_each_entry_safe(fdmap, n, &s_hisp500.fd_sgt_list, nd) {
		if (fdmap->shared_fd == (int)pcfg->param.sharedFd) {
			cam_info("%s: find shared_fd %d to del",
				__func__, fdmap->shared_fd);
			cam_buf_put_sgtable(fdmap->sgt);
			list_del_init(&fdmap->nd);
			kfree(fdmap);
		}
	}
	mutex_unlock(&kernel_fd_sgt_map_mutex);
	return 0;
}

static int hisp500_free_isp_cpu_addr_force(void)
{
	int ret;
	struct hisp_fd_map *fdmap = NULL;
	struct hisp_fd_map *n = NULL;
	struct hisp_mem_info_s mem_info = { 0 };
	cam_warn("func %s", __func__);

	mutex_lock(&kernel_fd_sgt_map_mutex);
	list_for_each_entry_safe(fdmap, n, &s_hisp500.fd_sgt_list, nd) {
		if (fdmap->cpu_addr != 0 && fdmap->sgt != NULL) {
			cam_info("%s: find shared_fd %d to del",
				__func__, fdmap->shared_fd);
			mutex_lock(&kernel_rpmsg_service_mutex);
			mem_info.cpu_addr = fdmap->cpu_addr;
			mem_info.cpu_size = fdmap->size;
			ret = hisp_mem_unmap_addr(fdmap->type, mem_info);
			if (ret != 0)
				cam_err("%s: hisp_mem_unmap_addr failed",
					__func__);
			if (fdmap->mem_info.fw_mem.cpu_addr != 0) {
				struct hisp_mem_type type = { 0 };
				type.master = ISP_CPU;
				type.mem_type = ISP_PRIVATE_MEM;
				struct hisp_mem_info_s mem_info = { 0 };
				mem_info.cpu_addr = fdmap->mem_info.fw_mem.cpu_addr;
				mem_info.cpu_size = fdmap->mem_info.fw_mem.size;
				ret = hisp_mem_unmap_addr(type, mem_info);
				if (ret != 0)
					cam_err("%s: hisp_mem_unmap_addr failed", __func__);
			}
			if (fdmap->mem_info.ispfw_mem.cpu_addr != 0) {
				struct hisp_mem_type type = { 0 };
				type.master = ISP_CPU | ISP_CORE;
				type.mem_type = ISP_PRIVATE_MEM;
				struct hisp_mem_info_s mem_info = { 0 };
				mem_info.cpu_addr = fdmap->mem_info.ispfw_mem.cpu_addr;
				mem_info.cpu_size = fdmap->mem_info.ispfw_mem.size;
				ret = hisp_mem_unmap_addr(type, mem_info);
				if (ret != 0)
					cam_err("%s: hisp_mem_unmap_addr failed", __func__);
			}
			mutex_unlock(&kernel_rpmsg_service_mutex);
			cam_buf_put_sgtable(fdmap->sgt);
			list_del_init(&fdmap->nd);
			kfree(fdmap);
		}
	}
	mutex_unlock(&kernel_fd_sgt_map_mutex);
	return 0;
}

static int hisp500_free_imu_addr_force(void)
{
	int rc = 0;
#if defined(CONFIG_SENSOR_DIRECT_CHANNEL)
	struct hisp_mem_info_s mem_info = { 0 };
	struct hisp_mem_type type = { 0 };
	mutex_lock(&kernel_rpmsg_service_mutex);
	if (imu_map_ref != 0) {
		type.master = ISP_CPU;
		type.mem_type = ISP_PHY_MEM;
		mem_info.cpu_addr = imu_map_param.iova;
		mem_info.cpu_size = imu_map_param.size;
		rc = hisp_mem_unmap_addr(type, mem_info);
		imu_map_ref = 0;
		imu_map_param.iova = 0;
		imu_map_param.size = 0;
		sdc_buf_put(imu_info);
		imu_info = NULL;
	}
	mutex_unlock(&kernel_rpmsg_service_mutex);
#endif
	return rc;
}

/*
 * handle daemon carsh
 * miss ispmanager poweroff
 * miss memory pool deinit
 */
static int hisp500_deinit_r8isp_memory_pool_force(void)
{
	int ipool = 0;
	int ret = 0;
	unsigned int prot;
	cam_warn("func %s", __func__);

	mutex_lock(&kernel_rpmsg_service_mutex);
	for (ipool = 0; ipool < MEM_POOL_ATTR_MAX; ipool++) {
		if (s_hisp500.mem_pool[ipool].active) {
			cam_warn("%s: force deiniting pool:%d",
				__func__, ipool);
			s_hisp500.mem_pool[ipool].active = 0;
			ret = hisp_mem_unmap_addr_sec((unsigned int)ipool);
			if (ret != 0)
				cam_err("%s: hisp_mem_unmap_addr failed", __func__);
			cam_buf_put_sgtable(s_hisp500.mem_pool[ipool].sgt);
		}
		prot = s_hisp500.mem_pool[ipool].prot; // prot should not be memset to 0
		ret = memset_s(&(s_hisp500.mem_pool[ipool]),
			sizeof(struct hisp500_mem_pool),
			0,
			sizeof(struct hisp500_mem_pool));
		if (ret != 0) {
			cam_err("memset_s failed %d", __LINE__);
			mutex_unlock(&kernel_rpmsg_service_mutex);
			return -EINVAL;
		}
		s_hisp500.mem_pool[ipool].prot = prot;
	}
	mutex_unlock(&kernel_rpmsg_service_mutex);
	return 0;
}

static int hisp500_mem_pool_pre_init(void)
{
	int ret = 0;
	int ipool = 0;
	int prot = 0;

	for (ipool = 0; ipool < MEM_POOL_ATTR_MAX; ipool++) {
		ret = memset_s(&(s_hisp500.mem_pool[ipool]),
			sizeof(struct hisp500_mem_pool),
			0,
			sizeof(struct hisp500_mem_pool));
		if (ret != 0) {
			cam_err("memset_s failed %d", __LINE__);
			return -EINVAL;
		}
		switch (ipool) {
		case MEM_POOL_ATTR_READ_WRITE_CACHE:
		case MEM_POOL_ATTR_READ_WRITE_CACHE_OFF_LINE:
			prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE;
			break;

		case MEM_POOL_ATTR_READ_WRITE_SECURITY:
		case MEM_POOL_ATTR_READ_WRITE_ISP_SECURITY:
			prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE |
				IOMMU_SEC;
			break;

		default:
			prot = -1;
			break;
		}

		cam_debug("%s  ipool %d prot 0x%x", __func__, ipool, prot);

		if (prot < 0) {
			cam_err("%s unkown ipool %d prot 0x%x",
				__func__, ipool, prot);
			return -EINVAL;
		}

		s_hisp500.mem_pool[ipool].prot = (unsigned int)prot;
	}

	return 0;
}

static int hisp500_mem_pool_later_deinit(void)
{
	int ipool = 0;
	int ret = 0;
	unsigned int prot;
	cam_debug("%s", __func__);

	for (ipool = 0; ipool < MEM_POOL_ATTR_MAX; ipool++) {
		if (ipool == MEM_POOL_ATTR_READ_WRITE_CACHE_OFF_LINE)
			continue;
		if (s_hisp500.mem_pool[ipool].active) {
			cam_warn("%s: force deiniting pool:%d",
				__func__, ipool);
			s_hisp500.mem_pool[ipool].active = 0;
			ret = hisp_mem_unmap_addr_sec((unsigned int)ipool);
			if (ret != 0)
				cam_err("%s: hisp_mem_unmap_addr failed", __func__);
			cam_buf_put_sgtable(s_hisp500.mem_pool[ipool].sgt);
		}
		prot = s_hisp500.mem_pool[ipool].prot; // prot should not be memset to 0
		ret = memset_s(&s_hisp500.mem_pool[ipool],
			sizeof(struct hisp500_mem_pool),
			0,
			sizeof(struct hisp500_mem_pool));
		if (ret != 0) {
			cam_err("memset_s failed %d", __LINE__);
			return -EINVAL;
		}
		s_hisp500.mem_pool[ipool].prot = prot;
	}

	return 0;
}

unsigned int ispfuncclks_es[] = {
	279000000,
	360000000,
	418000000,
	480000000,
};
unsigned int ispfunc2clks_es[] = {
	168000000,
	279000000,
	360000000,
	418000000,
	480000000,
	558000000,
	720000000,
};
unsigned int ispfunc3clks_es[] = {
	168000000,
	279000000,
	360000000,
	418000000,
	482000000,
	558000000,
	720000000,
};
unsigned int ispcpuclks_es[] = {
	836000000,
	1280000000,
	1618000000,
};
// CS
unsigned int ispfuncclks[] = {
	168000000,
	209000000,
	240000000,
	279000000,
	320000000,
	360000000,
	418000000,
	480000000,
};
unsigned int ispfunc2clks[] = {
	168000000,
	209000000,
	240000000,
	279000000,
	320000000,
	360000000,
	418000000,
	480000000,
	558000000,
	640000000,
	720000000,
};
unsigned int ispfunc3clks[] = {
	168000000,
	209000000,
	240000000,
	279000000,
	320000000,
	335000000,
	360000000,
	418000000,
	482000000,
	558000000,
	600000000,
	640000000,
	720000000,
};
unsigned int ispcpuclks[] = {
	836000000,
	1280000000,
	1672000000,
};

static int hisp500_set_ispfunc_clk_rate_self_adapt(unsigned int type, int clk_level)
{
	int rc;
	unsigned int*clks_array;
	int array_size;
	switch (type) {
		case ISPFUNC_CLK:
			if (g_chip_type == CAM_CT_ES) {
				clks_array = ispfuncclks_es;
				array_size = (int)ARRAY_SIZE(ispfuncclks_es);
			} else {
				clks_array = ispfuncclks;
				array_size = (int)ARRAY_SIZE(ispfuncclks);
			}
			break;
		case ISPFUNC2_CLK:
			if (g_chip_type == CAM_CT_ES) {
				clks_array = ispfunc2clks_es;
				array_size = (int)ARRAY_SIZE(ispfunc2clks_es);
			} else {
				clks_array = ispfunc2clks;
				array_size = (int)ARRAY_SIZE(ispfunc2clks);
			}
			break;
		case ISPFUNC3_CLK:
			if (g_chip_type == CAM_CT_ES) {
				clks_array = ispfunc3clks_es;
				array_size = (int)ARRAY_SIZE(ispfunc3clks_es);
			} else {
				clks_array = ispfunc3clks;
				array_size = (int)ARRAY_SIZE(ispfunc3clks);
			}
			break;
		case ISPCPU_CLK:
			if (g_chip_type == CAM_CT_ES) {
				clks_array = ispcpuclks_es;
				array_size = (int)ARRAY_SIZE(ispcpuclks_es);
			} else {
				clks_array = ispcpuclks;
				array_size = (int)ARRAY_SIZE(ispcpuclks);
			}
			break;
		default:
			cam_err("%s: unsupported type:%#x", __func__, type);
			return -1;
	}

	if (clk_level >= array_size || clk_level < 0) {
		cam_warn("voting invalid level: %d", clk_level);
		return -EINVAL;
	}

	do {
		rc = hisp_set_clk_rate(type, clks_array[clk_level]);
		if (rc == 0)
			break;
		if (clk_level != 0)
			cam_info("%s: set to clk level:%d fail, try level:%d", __func__, clk_level, clk_level - 1);
		clk_level -= 1; /* attention: minus one for a lower clk level. */
	} while (clk_level >= 0);
	return rc;
}

static int hisp500_phy_csi_connect(void *pdata)
{
	int rc = 0;
	struct phy_csi_connect_info *conn;

	conn = pdata;
	if (!conn) {
		cam_err("%s: data is NULL", __func__);
		return -1;
	}
	WARN_ON(sizeof(struct hisp_phy_info_t) != sizeof(conn->phy_info));
	rc = hisp_phy_csi_connect((struct hisp_phy_info_t *)&conn->phy_info,
		conn->csi_index);
	if (rc != 0)
		cam_err("%s: phy csi connect fail:%d", __func__, rc);
	return rc;
}

static int hisp500_config_power_on(hisp_intf_t *i, struct hisp_cfg_data *pcfg)
{
	int rc = 0;
	int client_id = hisp_get_client_id(current->tgid);
	g_has_oisp = (pcfg->cfgdata[0] != 0) ? true : false;
	if (hisp500_remote_processor_up()) {
		cam_warn("%s hisp500 is still on power-on state", __func__);
		if (client_id >= 0 && client_id < MAX_HISP_CLIENT)
			remote_processor_up[client_id] = true;
		return rc;
	}

	if (pcfg->isSecure == 0) {
		hisp_set_boot_mode(NONSEC_CASE);
	} else if (pcfg->isSecure == 1) {
		hisp_set_boot_mode(SEC_CASE);
	} else {
		cam_info("%s invalid mode", __func__);
	}
	cam_notice("%s power on the hisp500", __func__);
	rc = hisp500_power_on(i);

	if (g_has_oisp) {
		int ret = hoisp_start_prepare();
		cam_info("%s oisp power on, ret = %d", __func__, ret);  // oisp power on
	}

	return rc;
}

static int hisp500_config_power_off(hisp_intf_t *i, struct hisp_cfg_data *pcfg)
{
    (void)pcfg;
    int rc = 0;
	if (hisp500_remote_processor_up()) {
		cam_notice("%s power off the hisp500", __func__);
		if (hisp500_set_remote_processor_shutdown(i)) {
			rc = hisp500_power_off(i);
			if (rc != 0)
				cam_err("failed to hisp500 power off");
		}
	}
	return rc;
}

static int hisp500_config_op_isp(void *cfg)
{
	int rc = 0;
	struct hisp_cfg_data *pcfg = (struct hisp_cfg_data*)(cfg);
	int level = 0;
	switch (pcfg->cfgtype) {
	case HISP_CONFIG_ISP_FE:
		cam_info("%s HISP_CONFIG_ISP_FE, level: %d", __func__, pcfg->cfgdata[0]);
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPFUNC_CLK, pcfg->cfgdata[0]);
		break;
	case HISP_CONFIG_ISP_BE:
		cam_info("%s HISP_CONFIG_ISP_BE, level: %d", __func__, pcfg->cfgdata[0]);
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPFUNC3_CLK, pcfg->cfgdata[0]);
		break;
	case HISP_CONFIG_ISP_PE:
		cam_info("%s HISP_CONFIG_ISP_PE, level: %d", __func__, pcfg->cfgdata[0]);
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPFUNC2_CLK, pcfg->cfgdata[0]);
		break;
	case HISP_CONFIG_R8_TURBO:
		cam_info("%s HISP_CONFIG_R8_TURBO", __func__);
		level = 2;
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPCPU_CLK, level);
		break;
	case HISP_CONFIG_R8_NORMAL:
		cam_info("%s HISP_CONFIG_R8_NORMAL", __func__);
		level = 1;
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPCPU_CLK, level);
		break;
	case HISP_CONFIG_R8_LOWPOWER:
		cam_info("%s HISP_CONFIG_R8_LOWPOWER", __func__);
		level = 0;
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPCPU_CLK, level);
		break;
	case HISP_CONFIG_R8_ULTRALOW:
		cam_info("%s HISP_CONFIG_R8_ULTRALOW", __func__);
		level = 0;
		rc = hisp500_set_ispfunc_clk_rate_self_adapt(ISPCPU_CLK, level);
		break;
	case HISP_CONFIG_GET_SEC_ISPFW_SIZE:
		rc = hisp_secboot_memsize_get_from_type(
			pcfg->secMemType, &pcfg->buf_size);
		break;
	case HISP_CONFIG_SET_SEC_ISPFW_BUFFER:
		rc = hisp500_set_sec_fw_buffer(cfg);
		break;
	case HISP_CONFIG_RELEASE_SEC_ISPFW_BUFFER:
		rc = hisp500_release_sec_fw_buffer();
		break;
	case HISP_CONFIG_SECBOOT_PREPARE:
		rc = hisp_secboot_prepare();
		break;
	case HISP_CONFIG_SECBOOT_UNPREPARE:
		rc = 0;
		break;
	default:
		cam_err("%s: unsupported cmd:%#x", __func__, pcfg->cfgtype);
		break;
	}
	return rc;
}

static int hisp500_config_op(hisp_intf_t *i, void *cfg, int* pflag)
{
	int rc = 0;
	struct hisp_cfg_data *pcfg = (struct hisp_cfg_data*)(cfg);
	*pflag = 0;
	switch (pcfg->cfgtype) {
	case HISP_CONFIG_POWER_ON:
		rc = hisp500_config_power_on(i, pcfg);
		break;
	case HISP_CONFIG_POWER_OFF:
		rc = hisp500_config_power_off(i, pcfg);
		break;
	case HISP_CONFIG_INIT_MEMORY_POOL:
		rc = hisp500_init_r8isp_memory_pool(cfg);
		break;
	case HISP_CONFIG_DEINIT_MEMORY_POOL:
		rc = hisp500_deinit_r8isp_memory_pool(cfg);
		break;
	case HISP_CONFIG_PROC_TIMEOUT: {
		int client_id = hisp_get_client_id(current->tgid);
		unsigned int message_id = pcfg->cfgdata[0] | (client_id << HISP_MESSAGE_ID_CLIENT_BIT);
		cam_info("%s timeout message_id.0x%x", __func__, message_id);
		hisp_dump_rpmsg_with_id(message_id);
		if (client_id >= 0 && client_id < MAX_HISP_CLIENT)
			dump_isp_ack_ids(rpmsg_local.msg_pool[client_id]);
		break;
	}
	case HISP_CONFIG_SET_MDC_BUFFER:
		rc = hisp_set_mdc_buffer(cfg);
		break;
	case HISP_CONFIG_RELEASE_MDC_BUFFER:
		rc = hisp_release_mdc_buffer();
		break;
	case HISP_CONFIG_PHY_CSI_CONNECT:
		rc = hisp500_phy_csi_connect((void*)(pcfg->cfgdata));
		break;
	case HISP_CONFIG_LOCK_VOLTAGE:
		cam_warn("%s: lock voltage need develop!", __func__);
		break;
	case HISP_CONFIG_ALLOC_ISP_CPU_MEM:
		rc = hisp500_alloc_isp_cpu_addr(cfg);
		break;
	case HISP_CONFIG_FREE_ISP_CPU_MEM:
		rc = hisp500_free_isp_cpu_addr(cfg);
		break;
#if defined(CONFIG_SENSOR_DIRECT_CHANNEL)
	case HISP_CONFIG_INIT_IMU_BUFFER:
		rc = hisp500_init_imu_buffer(cfg);
		break;
	case HISP_CONFIG_DEINIT_IMU_BUFFER:
		rc = hisp500_deinit_imu_buffer(cfg);
		break;
#endif
	default:
		*pflag = 1; /* need continue to judge */
		break;
	}
	return rc;
}

static int hisp500_config(hisp_intf_t *i, void *cfg)
{
	int rc = 0;
	int flag = 0;
	hisp500_t *hi = NULL;
	struct hisp_cfg_data *pcfg = NULL;
	hisp_assert(i != NULL);
	if (cfg == NULL) {
		cam_err("func %s: cfg is NULL", __func__);
		return -1;
	}
	pcfg = (struct hisp_cfg_data*)cfg;
	hi = to_isp(i);
	hisp_assert(hi != NULL);

	mutex_lock(&hisp_power_lock_mutex);
	if (g_block_isp) {
		cam_err("ioctl blocked due to isp thermal limit");
		mutex_unlock(&hisp_power_lock_mutex);
		return -1;
	}
	rc = hisp500_config_op(i, cfg, &flag);
	if (flag == 1)
		rc = hisp500_config_op_isp(cfg);
	mutex_unlock(&hisp_power_lock_mutex);

	if (rc < 0)
		cam_err("%s: cmd:%#x fail, rc:%u", __func__, pcfg->cfgtype, rc);
	return rc;
}

static int hisp500_open(hisp_intf_t *i)
{
	(void)i;
	cam_info("%s hisp500 device open", __func__);

	mutex_lock(&hisp_power_lock_mutex);
	g_hisp_ref++;
	mutex_unlock(&hisp_power_lock_mutex);
	return 0;
}

static int hisp500_close(hisp_intf_t *i)
{
	int rc = 0;
	cam_info("%s hisp500 device close", __func__);
	mutex_lock(&hisp_power_lock_mutex);

	if (g_hisp_ref)
		g_hisp_ref--;

	if (hisp_get_client_refs(current->tgid) == 1 && hisp500_remote_processor_up()) {
		cam_warn("%s hisp500 is still on power-on state, power off it",
			__func__);

		if (hisp500_set_remote_processor_shutdown(i)) {
			rc = hisp500_power_off(i);
			if (rc != 0)
				cam_err("failed to hisp500 power off");
		}

		if (!hisp500_remote_processor_up()) {
			hisp500_deinit_r8isp_memory_pool_force();
			hisp500_free_isp_cpu_addr_force();
			hisp500_free_imu_addr_force();
		}
	}

	if (g_hisp_ref == 0) {
		hisp500_deinit_isp_mem();
		cam_buf_close_security_ta();
	}

	mutex_unlock(&hisp_power_lock_mutex);
	return rc;
}

static int hisp500_pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *state)
{
	int rc;

	mutex_lock(&hisp_pinctrl_lock_mutex);
	rc = pinctrl_select_state(p, state);
	mutex_unlock(&hisp_pinctrl_lock_mutex);

	return rc;
}

static int hisp500_power_on_rpproc(hisp500_t *hi)
{
	int rc = 0;
	if (hi == NULL)
		goto FAILED_RET;

	if ((!hw_is_fpga_board()) && (!IS_ERR(hi->dt.pinctrl_default))) {
		rc = hisp500_pinctrl_select_state(hi->dt.pinctrl,
			hi->dt.pinctrl_default);
		if (rc != 0) {
			cam_err("pinctrl select state fail");
			goto FAILED_RET;
		}
	}

	hisp_rpmsgrefs_reset();
	rc = hisp_rproc_enable();

FAILED_RET:
	return rc;
}

static int hisp500_power_on(hisp_intf_t *i)
{
	int rc = 0;
	bool rproc_enabled = false;
	bool hi_opened = false;
	hisp500_t *hi = NULL;
	int client_id = hisp_get_client_id(current->tgid);
	unsigned long current_jiffies = jiffies;
	uint32_t timeout = hw_is_fpga_board() ?
		TIMEOUT_IS_FPGA_BOARD : TIMEOUT_IS_NOT_FPGA_BOARD;

	if (i == NULL)
		return -1;
	if (client_id < 0 || client_id >= MAX_HISP_CLIENT)
		return -1;
	hi = to_isp(i);

	if (g_block_isp) {
		cam_err("isp blocked due to thermal");
		return -1;
	}
	cam_info("%s enter ... ", __func__);

	mutex_lock(&hisp_wake_lock_mutex);
	if (!hisp_power_wakelock->active) {
		__pm_stay_awake(hisp_power_wakelock);
		cam_info("%s hisp power on enter, wake lock", __func__);
	}
	mutex_unlock(&hisp_wake_lock_mutex);

	mutex_lock(&kernel_rpmsg_service_mutex);
	if (!atomic_read((&hi->opened))) {
		reinit_completion(&rpmsg_local.isp_comp);
		rc = hisp500_power_on_rpproc(hi);
		cam_info("%s() %d after hisp500_power_on_rpproc, rc = %d", __func__, __LINE__, rc);
		if (rc != 0)
			goto FAILED_RET;
		rproc_enabled = true;

		cam_info("%s() %d before wait completion", __func__, __LINE__);
		rc = (int)wait_for_completion_timeout(&rpmsg_local.isp_comp,
			msecs_to_jiffies(timeout));
		if (rc == 0) {
			rc = -ETIME;
			hisp_boot_stat_dump();
			cam_err("wait completion timeout");
			goto FAILED_RET;
		}
		cam_info("%s() %d after wait completion, rc = %d",
			__func__, __LINE__, rc);
		rc = 0;

		atomic_inc(&hi->opened);
		hi_opened = true;
	} else {
		cam_notice("%s isp has been opened", __func__);
	}
	remote_processor_up[client_id] = true;
	if (!rpmsg_local.rpdev) { // rpmsg_device not probed
		rc = -ENODEV;
		cam_err("%s: remote processor not connected", __func__);
		goto FAILED_RET;
	}

	if (hisp500_mem_pool_pre_init()) {
		cam_err("failed to pre init mem pool ");
		rc = -ENOMEM;
		goto FAILED_RET;
	}

	mutex_unlock(&kernel_rpmsg_service_mutex);
	cam_info("%s exit ,power on time:%d... ", __func__,
		jiffies_to_msecs(jiffies - current_jiffies));
	return rc;

FAILED_RET:
	if (hi_opened)
		atomic_dec(&hi->opened);

	if (rproc_enabled)
		hisp_rproc_disable();

	hisp500_mem_pool_later_deinit();
	remote_processor_up[client_id] = false;

	mutex_unlock(&kernel_rpmsg_service_mutex);

	mutex_lock(&hisp_wake_lock_mutex);
	if (hisp_power_wakelock->active) {
		__pm_relax(hisp_power_wakelock);
		cam_err("%s hisp power on failed, wake unlock", __func__);
	}
	mutex_unlock(&hisp_wake_lock_mutex);
	return rc;
}

static int hisp500_power_off(hisp_intf_t *i)
{
	int rc = 0;
	hisp500_t *hi = NULL;
	unsigned long current_jiffies = jiffies;
	struct hisp_mem_info_s mem_info = { 0 };
	struct hisp_mem_type type = { 0 };

	if (i == NULL)
		return -1;

	hi = to_isp(i);

	cam_info("%s enter ... ", __func__);

	if (hisp500_remote_processor_up()) {
		rc = 0;
		goto RET;
	}

	mutex_lock(&kernel_rpmsg_service_mutex);
	if (atomic_read((&hi->opened))) {
		hisp_phy_csi_disconnect();
		hisp_rproc_disable();

		if (g_has_oisp) {
			int ret = hoisp_stop_cleanup();
			cam_info("%s oisp power off, ret = %d", __func__, ret);  // oisp power off
		}

		if (!hw_is_fpga_board()) {
			if (!IS_ERR(hi->dt.pinctrl_idle)) {
				rc = hisp500_pinctrl_select_state(hi->dt.pinctrl,
					hi->dt.pinctrl_idle);
				if (rc != 0) {
					// Empty.
				}
			}
		}
		atomic_dec(&hi->opened);
	} else {
		cam_notice("%s isp hasn't been opened", __func__);
	}
	hisp500_mem_pool_later_deinit();

#if defined(CONFIG_SENSOR_DIRECT_CHANNEL)
	if (imu_map_ref != 0) {
		type.master = ISP_CPU;
		type.mem_type = ISP_PHY_MEM;
		mem_info.cpu_addr = imu_map_param.iova;
		mem_info.cpu_size = imu_map_param.size;
		rc = hisp_mem_unmap_addr(type, mem_info);
		imu_map_ref = 0;
		imu_map_param.iova = 0;
		imu_map_param.size = 0;
		sdc_buf_put(imu_info);
		imu_info = NULL;
	}
#endif
	mutex_unlock(&kernel_rpmsg_service_mutex);
RET:
	cam_info("%s exit ,power 0ff time:%d... ", __func__,
		jiffies_to_msecs(jiffies - current_jiffies));

	mutex_lock(&hisp_wake_lock_mutex);
	if (hisp_power_wakelock->active) {
		__pm_relax(hisp_power_wakelock);
		cam_info("%s hisp power off exit, wake unlock", __func__);
	}
	mutex_unlock(&hisp_wake_lock_mutex);
	return rc;
}

void hisp_clear_msgs(int client_id)
{
	if (client_id >= 0 && client_id < MAX_HISP_CLIENT)
		clear_isp_msg_pool(rpmsg_local.msg_pool[client_id]);
}
static void hisp500_rpmsg_remove(struct rpmsg_device *rpdev)
{
	unsigned int i;
	cam_info("%s enter ... ", __func__);
	hisp_rpmsgrefs_dump(); // dump rpmsg refs when power off'ed
	for (i = 0; i < MAX_HISP_CLIENT; ++i)
		clear_isp_msg_pool(rpmsg_local.msg_pool[i]); // release isp msg nodes

	mutex_lock(&rpmsg_local.sendlock);
	if (rpdev != rpmsg_local.rpdev)
		cam_warn("%s: removing rpdev not same as probed", __func__);
	rpmsg_local.rpdev = NULL;
	mutex_unlock(&rpmsg_local.sendlock);
	cam_info("%s: rpmsg kernel driver is removed", __func__);
}

#define RPMSG_MAX_PAYLOAD 512
struct isp_msg_holder {
	// rpmsg_hdr takes away some bytes
	uint8_t data[RPMSG_MAX_PAYLOAD - sizeof(struct rpmsg_hdr)];
};

static int hisp500_power_off_rpmsg(struct isp_msg_header* msg_hdr, uint32_t len, int client_id)
{
#define COMMAND_ISP_CPU_POWER_OFF_REQUEST 4113
	int rc;
	if (!msg_hdr || msg_hdr->api_name != COMMAND_ISP_CPU_POWER_OFF_REQUEST)
		return -EINVAL;
	if (client_id >= MAX_HISP_CLIENT || client_id < 0) {
		cam_err("func %s: client id is %d", __func__, client_id);
		return -EINVAL;
	}

	mutex_lock(&hisp_power_lock_mutex);
	if (hisp500_remote_processor_up() <= 1) {
		mutex_unlock(&hisp_power_lock_mutex);
		return -1;
	}

	cam_warn("two daemon run, skip power off rpmsg");
	rc = queue_isp_msg(rpmsg_local.msg_pool[client_id], msg_hdr, len);
	if (rc == 0)
		hisp500_notify_rpmsg_cb(client_id);

	remote_processor_up[client_id] = false;

	mutex_unlock(&hisp_power_lock_mutex);
	return 0;
}
static int hisp500_send_rpmsg(hisp_intf_t *itf, struct isp_indirect_msg *imsg, size_t len)
{
	int rc = 0;
	int msg_type = 0;
	struct isp_msg_header *msg_hdr = NULL;
	struct isp_msg_holder msg_data = {};
	int client_id = hisp_get_client_id(current->tgid);

	(void)itf;
	if (!imsg || len < sizeof(*imsg)) {
		cam_err("%s: itf or imsg NULL, or len:%zu", __func__, len);
		return -EINVAL;
	}
	if (!imsg->isp_msg || imsg->msg_size > sizeof(msg_data)) {
		cam_err("%s: isp_msg NULL or msg_size:%zu too big, max:%zu",
			__func__, len, sizeof(msg_data));
		return -EINVAL;
	}
	if (client_id >= MAX_HISP_CLIENT || client_id < 0) {
		cam_err("func %s: client id is %d", __func__, client_id);
		return -EINVAL;
	}

	rc = copy_from_user(&msg_data, (void __user *)imsg->isp_msg, imsg->msg_size);
	if (rc) {
		cam_err("%s: copy msg from user fail, len:%u",
			__func__, imsg->msg_size);
		return -EFAULT;
	}

	if ((unlikely(hisp_msg_type_check(&msg_data, sizeof(msg_data), &msg_type)))) {
		cam_err("%s: mst_type check failed\n", __func__);
		return -EINVAL;
	}
	if (msg_type) {
		cam_err("%s: isp kernel msg id, forbiden to send\n", __func__);
		return -EINVAL;
	}

	msg_hdr = (struct isp_msg_header *)&msg_data;
	if (!hisp500_power_off_rpmsg(msg_hdr, imsg->msg_size, client_id))
		return rc;

	msg_hdr->message_id |= (client_id << HISP_MESSAGE_ID_CLIENT_BIT);
	cam_debug("%s: client_id:%d, api_name:%#x, message_id:%#x", __func__,
		client_id, msg_hdr->api_name, msg_hdr->message_id);

	hisp_sendin(&msg_data);
	mutex_lock(&rpmsg_local.sendlock); // protect against async remove
	if (rpmsg_local.rpdev)
		rc = rpmsg_send(rpmsg_local.rpdev->ept,
			(void *)&msg_data, imsg->msg_size);
	else
		rc = -ENODEV;
	mutex_unlock(&rpmsg_local.sendlock);
	if (rc)
		cam_err("%s: rpmsg_send ret is %d", __func__, rc);
	return rc;
}

static int hisp500_recv_rpmsg(hisp_intf_t *itf, struct isp_indirect_msg *imsg, size_t len)
{
	int rc;
	struct isp_msg_header msg_hdr;
	int client_id = hisp_get_client_id(current->tgid);

	(void)itf;
	if (!imsg || len < sizeof(*imsg)) {
		cam_err("%s: user imsg NULL or len:%u", __func__, len);
		return -EINVAL;
	}
	if (!imsg->isp_msg) {
		cam_err("%s: user imsg->isp_msg NULL", __func__);
		return -EINVAL;
	}

	if (client_id >= MAX_HISP_CLIENT || client_id < 0) {
		cam_err("func %s: client id is %d", __func__, client_id);
		return -EINVAL;
	}

	rc = dequeue_isp_msg(rpmsg_local.msg_pool[client_id], imsg->isp_msg,
		imsg->msg_size, &msg_hdr);
	if (rc > 0)
		hisp_recvdone((void *)&msg_hdr);

	cam_debug("%s: client_id:%d, recved api_name:%#x, message_id:%#x, rc:%d", __func__,
		client_id, msg_hdr.api_name, msg_hdr.message_id, rc);
	return rc;
}

static int hisp500_set_sec_fw_buffer(struct hisp_cfg_data *cfg)
{
	int rc;
	mutex_lock(&hisp_mem_lock_mutex);
	rc = hisp_set_sec_fw_buffer(cfg);
	if (rc < 0)
		cam_err("%s: fail, rc:%d", __func__, rc);

	if (s_hisp500.mem.active) {
		s_hisp500.mem.active = 0;
		dma_buf_put(s_hisp500.mem.dmabuf);
	}

	s_hisp500.mem.dmabuf = dma_buf_get(cfg->share_fd);
	if (IS_ERR_OR_NULL(s_hisp500.mem.dmabuf)) {
		cam_err("Fail: dma buffer error");
		mutex_unlock(&hisp_mem_lock_mutex);
		return -EFAULT;
	}
	s_hisp500.mem.active = 1;
	mutex_unlock(&hisp_mem_lock_mutex);
	return rc;
}

static int hisp500_release_sec_fw_buffer(void)
{
	int rc;
	int ret;
	mutex_lock(&hisp_mem_lock_mutex);
	rc = hisp_release_sec_fw_buffer();
	if (rc < 0)
		cam_err("%s: fail, rc:%d", __func__, rc);

	if (s_hisp500.mem.active) {
		s_hisp500.mem.active = 0;
		dma_buf_put(s_hisp500.mem.dmabuf);
	}
	ret = memset_s(&(s_hisp500.mem),
		sizeof(struct isp_mem),
		0,
		sizeof(struct isp_mem));
	if (ret != 0)
		cam_warn("%s: fail for memset_s mem_pool", __func__);
	mutex_unlock(&hisp_mem_lock_mutex);
	return rc;
}
static void hisp500_deinit_isp_mem(void)
{
	int ret;
	cam_info("func %s", __func__);
	mutex_lock(&hisp_mem_lock_mutex);
	if (s_hisp500.mem.active) {
		cam_err("sec isp ex,put dmabuf");
		s_hisp500.mem.active = 0;
		dma_buf_put(s_hisp500.mem.dmabuf);
	}

	ret = memset_s(&(s_hisp500.mem),
		sizeof(struct isp_mem),
		0,
		sizeof(struct isp_mem));
	if (ret != 0)
		cam_warn("%s: fail for memset_s mem_pool", __func__);
	mutex_unlock(&hisp_mem_lock_mutex);
	return;
}


int hisp_extra_pinctrl_index(const char *name)
{
	int i;
	struct extra_pinctrl *ep = s_hisp500.dt.extra_pinctrl;

	if (!name)
		return -EINVAL;

	for (i = 0; i < (int)ARRAY_SIZE(s_hisp500.dt.extra_pinctrl); ++i)
		if (ep->name && strcmp(ep->name, name) == 0)
			return i;
	return -1;
}

int hisp_extra_pinctrl_select_state(int idx, const char *state)
{
	struct extra_pinctrl *ep = s_hisp500.dt.extra_pinctrl;

	if (idx < 0 || idx >= (int)ARRAY_SIZE(s_hisp500.dt.extra_pinctrl) || !state)
		return -EINVAL;

	if (strcmp(state, "default") == 0)
		return hisp500_pinctrl_select_state(s_hisp500.dt.pinctrl, ep[idx].pinctrl_default);
	if (strcmp(state, "idle") == 0)
		return hisp500_pinctrl_select_state(s_hisp500.dt.pinctrl, ep[idx].pinctrl_idle);
	return -1;
}

static ssize_t hisp_power_state_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	(void)dev;
	(void)attr;
	cam_info("enter %s, isp power on:%d",
		__func__, hisp500_remote_processor_up());
	return scnprintf(buf, PAGE_SIZE, "%d\n", hisp500_remote_processor_up());
}

static ssize_t hisp_chip_type_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	(void)dev;
	(void)attr;
	cam_info("enter %s, chip_type:%u", __func__, g_chip_type);
	return scnprintf(buf, PAGE_SIZE, "%d\n", g_chip_type);
}

static int32_t hisp500_rpmsg_probe(struct rpmsg_device *rpdev)
{
	cam_info("%s enter", __func__);
	if (rpmsg_local.rpdev) {
		cam_warn("%s rpdev is already up", __func__);
		return -EBUSY; // if need support multi rpmsg_device, need revise
	}
	if (!rpdev || !rpdev->ept) {
		cam_err("%s: rpdev NULL or rpdev->ept not created!", __func__);
		return -ENOENT;
	}

	rpmsg_local.rpdev = rpdev;
	dev_set_drvdata(&rpdev->dev, &rpmsg_local);

	complete(&rpmsg_local.isp_comp);
	cam_info("new KERNEL connection srv channel: %u -> %u",
		rpdev->src, rpdev->dst);
	return 0;
}

static struct rpmsg_device_id rpmsg_hisp500_id_table[] = {
	{.name = "rpmsg-isp"},
	{},
};

MODULE_DEVICE_TABLE(platform, rpmsg_hisp500_id_table);

static const struct of_device_id s_hisp500_dt_match[] = {
	{
		.compatible = "vendor,chip_isp500",
		.data = &s_hisp500.intf,
	},
	{},
};

MODULE_DEVICE_TABLE(of, s_hisp500_dt_match);
#pragma GCC visibility push(default)
static struct rpmsg_driver rpmsg_hisp500_driver = {
	.drv.name = KBUILD_MODNAME,
	.drv.owner = THIS_MODULE,
	.id_table = rpmsg_hisp500_id_table,
	.probe = hisp500_rpmsg_probe,
	.callback = hisp500_rpmsg_ept_cb,
	.remove = hisp500_rpmsg_remove,
};
#pragma GCC visibility pop


static struct device_attribute hisp_power_state_attr =
	__ATTR(isp_power_on, 0444, hisp_power_state_show, NULL);

static struct device_attribute hisp_chip_type_attr =
	__ATTR(chip_type, 0444, hisp_chip_type_show, NULL);

static int32_t hisp500_platform_probe(struct platform_device* pdev)
{
	int32_t ret = 0;
	unsigned int i;
	cam_info("%s: enter", __func__);
	if (g_hisp_probed) {
		cam_err("%s: hisp has been probed once", __func__); /* temp fix kernel unknown reason call probe twice */
		return -1;
	}
	hisp_power_wakelock = wakeup_source_register(&pdev->dev, "hisp_power_wakelock");
	if (!hisp_power_wakelock) {
		cam_err("%s: wakeup source register failed", __func__);
		return -1;
	}
	mutex_init(&hisp_wake_lock_mutex);
	mutex_init(&hisp_power_lock_mutex);
	mutex_init(&hisp_mem_lock_mutex);
	mutex_init(&rpmsg_local.sendlock);
	init_completion(&rpmsg_local.isp_comp);

	ret = hisp_get_dt_data(pdev, &s_hisp500.dt);
	if (ret < 0) {
		cam_err("%s: get dt failed", __func__);
		goto error;
	}

	hisp500_get_chip_type(pdev);
	for (i = 0; i < MAX_HISP_CLIENT; ++i) {
		rpmsg_local.msg_pool[i] = create_isp_msg_pool();
		if (!rpmsg_local.msg_pool[i]) {
			cam_err("%s: create msg pool failed", __func__);
			ret = -ENOMEM;
			goto error;
		}
	}
	ret = hisp_register(pdev, &s_hisp500.intf, &s_hisp500.notify);
	if (ret == 0) {
		atomic_set(&s_hisp500.opened, 0);
	} else {
		cam_err("%s() %d hisp_register failed with ret %d",
			__func__, __LINE__, ret);
		goto error;
	}

	ret = register_rpmsg_driver(&rpmsg_hisp500_driver);
	if (ret != 0) {
		cam_err("%s() %d register_rpmsg_driver failed with ret %d",
			__func__, __LINE__, ret);
		goto error;
	}

	s_hisp500.pdev = pdev;

	ret = memset_s(&(s_hisp500.mem),
		sizeof(struct isp_mem),
		0,
		sizeof(struct isp_mem));
	if (ret != 0)
		cam_warn("%s: fail for memset_s mem_pool", __func__);

	ret = device_create_file(&pdev->dev, &hisp_power_state_attr);
	if (ret < 0) {
		cam_err("%s failed to creat hisp _power_state attribute",
			__func__);
		unregister_rpmsg_driver(&rpmsg_hisp500_driver);
		hisp_unregister(s_hisp500.pdev);
		goto error;
	}

	ret = device_create_file(&pdev->dev, &hisp_chip_type_attr);
	if (ret < 0) {
		cam_err("%s failed to creat hisp_chip_type attribute",
			__func__);
		unregister_rpmsg_driver(&rpmsg_hisp500_driver);
		hisp_unregister(s_hisp500.pdev);
		goto error;
	}
	mutex_lock(&kernel_fd_sgt_map_mutex);
	INIT_LIST_HEAD(&s_hisp500.fd_sgt_list);
	mutex_unlock(&kernel_fd_sgt_map_mutex);
#ifdef CONFIG_THERMAL_ISP_HOTPLUG
	set_block_isp_cb(handle_block_isp);
#endif
	g_hisp_probed = true;
	cam_info("%s: exit", __func__);
	return 0;
error:
	wakeup_source_unregister(hisp_power_wakelock);
	hisp_power_wakelock = NULL;
	for (i = 0; i < MAX_HISP_CLIENT; ++i) {
		if (rpmsg_local.msg_pool[i])
			destroy_isp_msg_pool(rpmsg_local.msg_pool[i]);
		rpmsg_local.msg_pool[i] = NULL;
	}
	mutex_destroy(&rpmsg_local.sendlock);
	mutex_destroy(&hisp_wake_lock_mutex);
	mutex_destroy(&hisp_power_lock_mutex);
	mutex_destroy(&hisp_mem_lock_mutex);
	cam_notice("%s exit with ret = %d", __func__, ret);
	return ret;
}

static struct platform_driver s_hisp500_driver = {
	.probe = hisp500_platform_probe,
	.driver = {
		.name = "vendor,chip_isp500",
		.owner = THIS_MODULE,
		.of_match_table = s_hisp500_dt_match,
	},
};

static bool bypass_isp_init_by_soc_spec(void)
{
#if defined(CONFIG_SOC_PG_INFO) && !defined(SCOUT)
	struct soc_component_pg_info pg_info;
	get_soc_pg_info_by_index(SOC_COMPONENT_MEDIA2_ISP_INFO, &pg_info);

	if (pg_info.tailor_strategy == SOC_COMPONENT_TAILOR_STRATEGY_1) {
		cam_info("%s: is pg chip, bypass isp\n", __func__);
		return true;
	}
#endif

	return false;
}

static int __init hisp500_init_module(void)
{
	if (bypass_isp_init_by_soc_spec()) {
		cam_info("%s: bypass isp init\n", __func__);
		return 0;
	}
	cam_notice("%s enter", __func__);
	return platform_driver_register(&s_hisp500_driver);
}

static void __exit hisp500_exit_module(void)
{
	if (bypass_isp_init_by_soc_spec()) {
		cam_info("%s: bypass isp init\n", __func__);
		return;
	}
	cam_notice("%s enter", __func__);
	hisp_unregister(s_hisp500.pdev);
	platform_driver_unregister(&s_hisp500_driver);
	wakeup_source_unregister(hisp_power_wakelock);
	hisp_power_wakelock = NULL;
	mutex_destroy(&hisp_wake_lock_mutex);
}

#ifdef CONFIG_THERMAL_ISP_HOTPLUG
static int handle_block_isp(bool block)
{
	int rc;
	mutex_lock(&hisp_power_lock_mutex);
	if (block == g_block_isp) {
		mutex_unlock(&hisp_power_lock_mutex);
		return 0;
	}
	if (block) {
		cam_warn("%s block isp", __func__);
		g_block_isp = true;
		hisp500_remote_processor_up_reset();
		rc = hisp500_power_off(&s_hisp500.intf);
		if (rc != 0)
			cam_err("failed to hisp500 power off");
		hisp500_deinit_r8isp_memory_pool_force();
		hisp500_free_isp_cpu_addr_force();
		hisp500_free_imu_addr_force();

		hisp500_deinit_isp_mem();
		cam_buf_close_security_ta();
	} else {
		cam_warn("%s unblock isp", __func__);
		g_block_isp = false;
	}
	mutex_unlock(&hisp_power_lock_mutex);
	return 0;
}
#endif

static void hisp500_get_chip_type(struct platform_device *pdev)
{
	struct device *dev = NULL;
	struct device_node *dev_node = NULL;
    struct efuse_desc desc;
	uint32_t chip_version;

	int ret = 0;
	if (!pdev) {
		cam_err("%s: pdev is NULL", __func__);
		return;
	}

	dev = &pdev->dev;
	if (!dev) {
		cam_err("%s: dev NULL", __func__);
		return;
	}

	dev_node = dev->of_node;
	if (!dev_node) {
		cam_err("%s: of node NULL", __func__);
		return;
	}

	ret = of_property_read_u32(dev_node, "vendor,chip_type", &g_chip_type);
	if (ret < 0) {
		cam_err("%s: get chip type failed", __func__);
		return;
	}

	if (g_chip_type == CAM_CT_ES) {
		cam_info("%s: chip_type [ES] ", __func__);
		return;
	} else if (g_chip_type == CAM_CT_UNKNOWN) {
		cam_info("%s: chip_type [UNKNOWN] ", __func__);
		return;
	}

	desc.buf = &chip_version;
	desc.buf_size = sizeof(chip_version) / sizeof(uint32_t);
	desc.start_bit = EFUSE_CHIP_VERSION_START_BIT;
	desc.bit_cnt = EFUSE_CHIP_VERSION_BIT_CNT;
	desc.item_vid = EFUSE_KERNEL_CHIP_VERSION;

	ret = efuse_read_value_t(&desc);
	if (ret == EFUSE_OK) {
		g_chip_type = (chip_version == CHIP_VERSION_ES2) ? CAM_CT_ES2 : CAM_CT_CS;
		cam_info("%s: chip_type [%s]", __func__, (g_chip_type == CAM_CT_ES2) ? "ES2" : "CS");
	} else {
		g_chip_type = CAM_CT_CS;
		cam_warn("%s: get chip version failed, set chip_type [CS]", __func__);
	}
}
module_init(hisp500_init_module);
module_exit(hisp500_exit_module);
MODULE_DESCRIPTION("hisp500 driver");
MODULE_LICENSE("GPL v2");
