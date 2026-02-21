/*
 * vcodec_bsp_dev.c
 *
 * Implement vcodec bsp function which is used only by
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

#include "vfmw_pdt.h"
#include "vcodec_bsp.h"
#include "vcodec_bsp_hal.h"
#include "vcodec_vdec_regulator.h"
#include "vcodec_dev_mgr_util.h"
#include "vcodec_dev_mgr_formal.h"
#include "dbg.h"
#include "vcodec_mcu_ddr_map.h"
#include "vcodec_vdec_plat.h"

int32_t vcodec_bsp_init_dev(struct vcodec_dev_bsp* dev, uint8_t *reg_base)
{
	return vcodec_bsp_init_hal(dev, reg_base);
}

void vcodec_bsp_deinit_dev(struct vcodec_dev_bsp* dev)
{
	vcodec_bsp_deinit_hal(dev);
}

static bool vcodec_bsp_need_sw_buffer(void)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();

	return ops->bsp_need_sw_buffer();
}

int32_t vcodec_bsp_open_dev(struct vcodec_dev_bsp* dev)
{
	dprint(PRN_ALWS, "power on bsp");

	if (vcodec_bsp_need_sw_buffer())
		(void)memcpy_s(dev->sw_buffer.virt_addr, dev->sw_buffer.iova_size,
			vcodec_bsp_sw_data(), BSP_SW_DATA_SIZE);

	mb();
	if (vcodec_bsp_hal_poweron(dev))
		return -EFAULT;

	dprint(PRN_ALWS, "power on bsp success");
	return 0;
}

int32_t vcodec_bsp_close_dev(struct vcodec_dev_bsp* dev)
{
	dprint(PRN_ALWS, "power off bsp");

	if (vcodec_bsp_hal_poweroff(dev))
		return -EFAULT;

	dprint(PRN_ALWS, "power off bsp success");
	return 0;
}

void vcodec_bsp_set_irq_mask(struct vcodec_dev_bsp* dev)
{
	vcodec_bsp_hal_irq_mask(dev);
}

int32_t vcodec_bsp_reset_dev(struct vcodec_dev_bsp* dev)
{
	vcodec_bsp_hal_reset_pre_process(dev);
	(void)vcodec_bsp_close_dev(dev);

	vcodec_bsp_set_irq_mask(dev);
	if (vcodec_bsp_open_dev(dev)) {
		dprint(PRN_ERROR, "bsp poweron failed.");
		return -EFAULT;
	}

	vcodec_bsp_hal_unreset_host_process(dev);
	return 0;
}

static const char *vcodec_bsp_get_image_name(void)
{
	if (vdec_plat_is_es())
		return VDEC_BSP_IMAGE_PATH;
	else
		return VDEC_CS_BSP_IMAGE_PATH;
}

static int32_t vcodec_bsp_read_image(struct vcodec_dev_bsp* dev)
{
	const size_t max_image_size = BSP_IMG_DATA_SIZE;
	const size_t min_image_size = vcodec_bsp_need_sw_buffer() ? VDEC_CERT_SIZE + BSP_SW_DATA_SIZE : VDEC_CERT_SIZE;

	return vcodec_dev_mgr_read_image(vcodec_bsp_get_image_name(),
		&dev->image_buffer, &dev->image_size,
		min_image_size, max_image_size);
}

void vcodec_bsp_free_image(struct vcodec_dev_bsp *dev)
{
	if (dev->image_buffer) {
		vfree(dev->image_buffer);
		dev->image_buffer = NULL;
		dev->image_size = 0;
	}
}

int32_t vcodec_bsp_load_image(struct vcodec_dev_bsp* dev)
{
	static uint32_t image_load_count = 0;

	if (image_load_count) {
		image_load_count = (image_load_count + 1) % LOAD_IMAGE_TO_TEE_PERIOD;
		return 0;
	}

	if (vcodec_bsp_read_image(dev))
		return -EIO;

	if (load_vdec_image(dev->type, dev->image_buffer, dev->image_size)) {
		vcodec_bsp_free_image(dev);
		return -EFAULT;
	}

	image_load_count = (image_load_count + 1) % LOAD_IMAGE_TO_TEE_PERIOD;
	return 0;
}

static int32_t vcodec_bsp_alloc_sw_buffer(struct vcodec_dev_bsp* dev)
{
	if (!vcodec_bsp_need_sw_buffer())
		return 0;

	if (vdec_alloc_iova_with_va(BSP_SW_BUFFER_BASE, BSP_SW_BUFFER_SIZE, &(dev->sw_buffer))) {
		dprint(PRN_ERROR, "Alloc memory for bsp sw fail");
		return -ENOMEM;
	}
	dev->buffer_info.sw_addr = dev->sw_buffer.iova;

	return 0;
}

static void vcodec_bsp_free_sw_buffer(struct vcodec_dev_bsp* dev)
{
	if (!vcodec_bsp_need_sw_buffer())
		return;

	vdec_free_iova_with_va(&(dev->sw_buffer));
}

int32_t vcodec_bsp_alloc_buffer(struct vcodec_dev_bsp* dev)
{
	UADDR phy_addr = 0;
	uint32_t index = 0;

	// use_ring_buffer_flag MUST NOT be false in user version
	if (unlikely(!dev->use_ring_buffer_flag))
		return -EFAULT;

	// Alloc sw buffer
	if (vcodec_bsp_alloc_sw_buffer(dev))
		return -ENOMEM;

	// Alloc ring buffer
	if (vdec_alloc_iova(VDEC_BSP_RING_BUFFER_BASE, VDEC_BSP_RING_BUFFER_SIZE, &(dev->ring_buffer))) {
		dprint(PRN_ERROR, "vcodec_bsp_alloc_ring_buf error");
		vcodec_bsp_free_sw_buffer(dev);
		return -ENOMEM;
	}

	// fill buffer info
	phy_addr = dev->ring_buffer.iova;
	dev->buffer_info.ctu_info_len = BSP_CTU_INFO_RING_SIZE;
	dev->buffer_info.ctu_res_len = BSP_CTU_RES_RING_SIZE;
	for (index = 0; index < VDH_BSP_NUM; index++) {
		dev->buffer_info.ctu_info_addr[index] = phy_addr;
		phy_addr += dev->buffer_info.ctu_info_len;
		dev->buffer_info.ctu_res_addr[index] = phy_addr;
		phy_addr += dev->buffer_info.ctu_res_len;
	}

	return 0;
}

void vcodec_bsp_free_buffer(struct vcodec_dev_bsp* dev)
{
	// use_ring_buffer_flag MUST NOT be false in user version
	if (unlikely(!dev->use_ring_buffer_flag))
		return;

	vdec_free_iova(&(dev->ring_buffer));
	vcodec_bsp_free_sw_buffer(dev);

	// clear buffer info
	if (memset_s(&dev->buffer_info, sizeof(struct bsp_buffer_cfg),
		0, sizeof(struct bsp_buffer_cfg)))
		dprint(PRN_ERROR, "memset_s clear buffer info fail");
}

irqreturn_t vcodec_bsp_interrupt_handling(struct vcodec_dev_bsp* dev)
{
	return vcodec_bsp_hal_interrupt_handling(dev);
}

const struct vcodec_bsp_control_proc_tbl* vcodec_bsp_get_control_proc_tbl(void)
{
	static struct vcodec_bsp_control_proc_tbl tbl[] = {
		/* tbl end flag */
		{BSP_INVALID_CMD_ID, 0, VCODEC_BSP_STATUS_BUTT, NULL},
	};

	return tbl;
}