/*
 * vcodec_bsp_intf.c
 *
 * Implement interface of vcodec bsp device
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
#include <linux/slab.h>
#include "vcodec_bsp.h"
#include "dbg.h"

static struct vcodec_dev_bsp* vcodec_get_bsp_dev_and_validate(struct vcodec_dev* dev)
{
	struct vcodec_dev_bsp* dev_bsp = (struct vcodec_dev_bsp*)dev;

	if (unlikely(!dev_bsp))
		return NULL;

	if (unlikely(dev_bsp->type != VCODEC_DEV_BSP))
		return NULL;

	if (unlikely(dev_bsp->size != sizeof(struct vcodec_dev_bsp)))
		return NULL;

	return dev_bsp;
}

static int32_t vcodec_bsp_open(struct vcodec_dev* dev, uint8_t *reg_base)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp || !reg_base)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	dprint(PRN_ALWS, "begin open bsp device");
	if (vcodec_bsp_init_dev(dev_bsp, reg_base))
		return -EFAULT;

	if (vcodec_bsp_load_image(dev_bsp)) {
		vcodec_bsp_deinit_dev(dev_bsp);
		return -EFAULT;
	}

	if (vcodec_bsp_alloc_buffer(dev_bsp)) {
		vcodec_bsp_free_image(dev_bsp);
		vcodec_bsp_deinit_dev(dev_bsp);
		return -ENOMEM;
	}

	vcodec_bsp_set_irq_mask(dev_bsp);

	if (vcodec_bsp_open_dev(dev_bsp)) {
		vcodec_bsp_free_buffer(dev_bsp);
		vcodec_bsp_free_image(dev_bsp);
		vcodec_bsp_deinit_dev(dev_bsp);
		return -EFAULT;
	}
	dev_bsp->status = VCODEC_BSP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "bsp open success");
	return 0;
}

static int32_t vcodec_bsp_close(struct vcodec_dev* dev)
{
	int32_t ret = 0;
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	dprint(PRN_ALWS, "begin close bsp device");
	if (dev_bsp->status == VCODEC_BSP_STATUS_POWER_ON)
		ret = vcodec_bsp_close_dev(dev_bsp);
	dev_bsp->status = VCODEC_BSP_STATUS_POWER_OFF;

	vcodec_bsp_free_buffer(dev_bsp);
	vcodec_bsp_free_image(dev_bsp);
	vcodec_bsp_deinit_dev(dev_bsp);
	return ret;
}

static int32_t vcodec_bsp_resume(struct vcodec_dev* dev)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_bsp->status != VCODEC_BSP_STATUS_SUSPEND)
		return 0;

	dprint(PRN_ALWS, "begin resume bsp device");
	vcodec_bsp_set_irq_mask(dev_bsp);

	if (vcodec_bsp_open_dev(dev_bsp)) {
		dev_bsp->status = VCODEC_BSP_STATUS_FAULT;
		return -EFAULT;
	}

	dev_bsp->status = VCODEC_BSP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "resume bsp device success");
	return 0;
}

static int32_t vcodec_bsp_suspend(struct vcodec_dev* dev)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_bsp->status != VCODEC_BSP_STATUS_POWER_ON)
		return 0;
	
	dprint(PRN_ALWS, "begin suspend bsp device");
	(void)vcodec_bsp_close_dev(dev_bsp);

	dev_bsp->status = VCODEC_BSP_STATUS_SUSPEND;
	dprint(PRN_ALWS, "suspend bsp device success");
	return 0;
};


static int32_t vcodec_bsp_control(struct vcodec_dev* dev, uint32_t msg_id, void *msg, uint32_t msg_size)
{
	const struct vcodec_bsp_control_proc_tbl *tbl = NULL;
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	tbl = vcodec_bsp_get_control_proc_tbl();
	while (tbl->cmd != BSP_INVALID_CMD_ID) {
		if (tbl->cmd == msg_id)
			break;

		tbl++;
	}
	if (tbl->cmd != msg_id || !tbl->proc)
		return -EFAULT;

	if ((tbl->msg_size) && ((tbl->msg_size != msg_size) || (!msg)))
		return -EFAULT;

	if (tbl->validate_bsp_status != VCODEC_BSP_STATUS_BUTT &&
		tbl->validate_bsp_status != dev_bsp->status)
		return -EFAULT;

	return tbl->proc(dev_bsp, msg);
}

static int32_t vcodec_bsp_reset(struct vcodec_dev* dev, void* reset_info)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	struct bsp_reset_info* info = (struct bsp_reset_info*)reset_info;

	if (unlikely(!dev_bsp) || unlikely(!info)) {
		dprint(PRN_FATAL, "invalid device or rest info");
		return -EINVAL;
	}

	if (dev_bsp->status != VCODEC_BSP_STATUS_POWER_ON)
		return 0;

	dev_bsp->buffer_info.using_reg_index[0] = info->bsp_reg_decoding_index[0];
	dev_bsp->buffer_info.using_reg_index[1] = info->bsp_reg_decoding_index[1];

	dprint(PRN_ALWS, "begin reset bsp device chan %d frm %d", info->chan_id, info->frm_num);
	if (vcodec_bsp_reset_dev(dev_bsp)) {
		dev_bsp->status = VCODEC_BSP_STATUS_FAULT;
		return -EFAULT;
	}

	dev_bsp->status = VCODEC_BSP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "reset bsp device success");
	return 0;
};

static void vcodec_bsp_destroy(struct vcodec_dev* dev)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	vcodec_osal_event_exit(dev_bsp->bsp_power_off_ack);
	vcodec_osal_event_exit(dev_bsp->bsp_power_on_ack);
	vcodec_osal_event_exit(dev_bsp->bsp0_clear_ack);
	vcodec_osal_event_exit(dev_bsp->bsp1_clear_ack);

	kfree(dev_bsp);
	dev_bsp = NULL;
}

static irqreturn_t vcodec_bsp_irq_proc(struct vcodec_dev* dev)
{
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return vcodec_bsp_interrupt_handling(dev_bsp);
}

static void vcodec_bsp_proc_read(struct vcodec_dev* dev, void* file)
{
	int used = 0;
	struct vcodec_dev_bsp* dev_bsp = vcodec_get_bsp_dev_and_validate(dev);
	if (unlikely(!dev_bsp)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	if (dev_bsp->status != VCODEC_BSP_STATUS_POWER_ON)
		return;

}

struct vcodec_dev* vcodec_bsp_create_dev(struct device *device)
{
	struct vcodec_dev_bsp* dev = NULL;
	if (unlikely(!device))
		return NULL;

	dev = kzalloc(sizeof(struct vcodec_dev_bsp), GFP_KERNEL);
	if (!dev)
		return NULL;

	// init the callback function tabel
	dev->dev.open = vcodec_bsp_open;
	dev->dev.close = vcodec_bsp_close;
	dev->dev.suspend = vcodec_bsp_suspend;
	dev->dev.resume = vcodec_bsp_resume;
	dev->dev.reset = vcodec_bsp_reset;
	dev->dev.destroy = vcodec_bsp_destroy;
	dev->dev.control = vcodec_bsp_control;
	dev->dev.irq_proc = vcodec_bsp_irq_proc;
	dev->dev.proc_read = vcodec_bsp_proc_read;

	// init basic bsp parameter
	dev->type = VCODEC_DEV_BSP;
	dev->size = sizeof(struct vcodec_dev_bsp);
	dev->device = device;
	dev->use_ring_buffer_flag = 1;
	dev->retention_flag = 1;
	dev->reset_flag = 0;
	dev->next_idle_reg[0] = 0;
	dev->next_idle_reg[1] = 0;

	vcodec_osal_event_init(&dev->bsp_power_on_ack, 0);
	vcodec_osal_event_init(&dev->bsp_power_off_ack, 0);
	vcodec_osal_event_init(&dev->bsp0_clear_ack, 0);
	vcodec_osal_event_init(&dev->bsp1_clear_ack, 0);

	dev->status = VCODEC_BSP_STATUS_POWER_OFF;
	return &dev->dev;
}
