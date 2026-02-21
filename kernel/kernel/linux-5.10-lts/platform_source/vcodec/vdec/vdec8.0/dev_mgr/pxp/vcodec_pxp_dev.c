/*
 * vcodec_pxp_dev.c
 *
 * Implement vcodec pxp function which is used only by
 * formal version
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "vcodec_pxp.h"
#include "vcodec_pxp_hal.h"
#include "vcodec_vdec_regulator.h"
#include "vcodec_vdec_plat.h"
#include "vdec_sc_regulator.h"
#include "vcodec_dev_mgr_util.h"
#include "vcodec_dev_mgr_formal.h"
#include "tvp_adapter.h"
#include "dbg.h"

int32_t vcodec_pxp_init_dev(struct vcodec_dev_pxp* dev, uint8_t *reg_base)
{
	return vcodec_pxp_init_hal(dev, reg_base);
}

void vcodec_pxp_deinit_dev(struct vcodec_dev_pxp *dev)
{
	vcodec_pxp_deinit_hal(dev);
}

int32_t vcodec_pxp_open_dev(struct vcodec_dev_pxp* dev)
{
	return vcodec_pxp_hal_poweron(dev);
}

int32_t vcodec_pxp_close_dev(struct vcodec_dev_pxp* dev)
{
	return vcodec_pxp_hal_poweroff(dev);
}

int32_t vcodec_pxp_reset_dev(struct vcodec_dev_pxp* dev)
{
	// reset operation
	dev->reset_flag = 1;
	(void)vcodec_pxp_hal_poweroff(dev);

	if (vcodec_pxp_hal_poweron(dev)) {
		dev->reset_flag = 0;
		return -EFAULT;
	}

	vcodec_pxp_hal_unreset_host_process(dev);
	dev->reset_flag = 0;
	return 0;
}

static const char *vcodec_pxp_get_image_name(void)
{
	if (vdec_plat_is_es())
		return VDEC_PXP_IMAGE_PATH;
	else
		return VDEC_CS_PXP_IMAGE_PATH;
}

static int32_t vcodec_pxp_read_image(struct vcodec_dev_pxp* dev)
{
	const size_t max_image_size = PXP_IMG_DATA_SIZE;
	const size_t min_image_size = VDEC_CERT_SIZE;

	return vcodec_dev_mgr_read_image(vcodec_pxp_get_image_name(),
		&dev->image_buffer, &dev->image_size,
		min_image_size, max_image_size);
}

void vcodec_pxp_free_image(struct vcodec_dev_pxp* dev)
{
	if (dev->image_buffer) {
		vfree(dev->image_buffer);
		dev->image_buffer = NULL;
		dev->image_size = 0;
	}
}

int32_t vcodec_pxp_load_image(struct vcodec_dev_pxp* dev)
{
	static uint32_t image_load_count = 0;

	if (image_load_count) {
		image_load_count = (image_load_count + 1) % LOAD_IMAGE_TO_TEE_PERIOD;
		return 0;
	}

	if (vcodec_pxp_read_image(dev))
		return -EIO;

	if (load_vdec_image(dev->type, dev->image_buffer, dev->image_size)) {
		vcodec_pxp_free_image(dev);
		return -EFAULT;
	}

	image_load_count = (image_load_count + 1) % LOAD_IMAGE_TO_TEE_PERIOD;
	return 0;
}

/* When the PXP has dual core, they needs to use sc or ddr to interact
 * data with each other. If sc allocation fail, we must allocate ddr
 * instead, otherwise pxp will not work. */
int32_t vcodec_pxp_alloc_syscache(struct vcodec_dev_pxp* dev)
{
	int32_t ret = -ENOMEM;
	vdec_plat_ops *ops = get_vdec_plat_ops();
	uint32_t syscache_size = ops->pxp_powerup_get_syscache_size();

	// if pxp used only single core, it doesn't need sc
	if (!dev->multi_core_enable)
		return 0;

#ifdef CONFIG_VCODEC_OHOS
	ret = vdec_alloc_dma_and_map(dev->device, syscache_size, &dev->pxp_sc);
#else
	if (dev->syscache_enable)
		ret = vdec_mem_alloc_with_sc(dev->device, syscache_size, &dev->pxp_sc);

	if (ret)
		ret = vdec_alloc_dma_and_map(dev->device, syscache_size, &dev->pxp_sc);
	else
		dev->is_sc_allocated = 1;
#endif

	if (ret) {
		dprint(PRN_ERROR, "pxp alloc syscache error");
		return ret;
	}

	dev->buffer_info.syscache_start_addr = dev->pxp_sc.iova;
	dev->buffer_info.syscache_len = syscache_size;

	return 0;
}

void vcodec_pxp_free_syscache(struct vcodec_dev_pxp* dev)
{
	if (!dev->multi_core_enable)
		return;

	if (dev->is_sc_allocated)
		vdec_mem_free_with_sc(dev->device, &dev->pxp_sc);
	else
		vdec_free_dma_and_unmap(dev->device, &dev->pxp_sc);

	dev->buffer_info.syscache_start_addr = 0;
	dev->buffer_info.syscache_len = 0;
}

void vcodec_pxp_config_sc(struct vcodec_dev_pxp* dev)
{
	if (!dev->is_sc_allocated)
		return;

	vdec_hal_cfg_pxpc_syscache();
}

void vcodec_pxp_set_irq_mask(struct vcodec_dev_pxp* dev)
{
	vcodec_pxp_hal_irq_mask(dev);
}

irqreturn_t vcodec_pxp_interrupt_handling(struct vcodec_dev_pxp* dev)
{
	return vcodec_pxp_hal_interrupt_handling(dev);
}

const struct vcodec_pxp_control_proc_tbl *vcodec_pxp_get_control_proc_tbl(void)
{
	static struct vcodec_pxp_control_proc_tbl tbl[] = {
		/* tbl end flag */
		{PXP_INVALID_CMD_ID, 0, VCODEC_PXP_STATUS_BUTT, NULL},
	};
	return tbl;
}