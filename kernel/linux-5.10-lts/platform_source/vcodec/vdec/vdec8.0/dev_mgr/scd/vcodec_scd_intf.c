/*
 * vcodec_scd_intf.c
 *
 * Implement interface of vcodec scd device
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
#include "vcodec_scd.h"
#include "dbg.h"

static struct vcodec_dev_scd* vcodec_get_scd_dev_and_validate(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = (struct vcodec_dev_scd*)dev;

	if (unlikely(!dev_scd))
		return NULL;

	if (unlikely(dev_scd->type != VCODEC_DEV_SCD))
		return NULL;

	if (unlikely(dev_scd->size != sizeof(struct vcodec_dev_scd)))
		return NULL;

	return dev_scd;
}


static int32_t vcodec_scd_open(struct vcodec_dev* dev, uint8_t *reg_base)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd || !reg_base)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (vcodec_scd_init_dev(dev_scd, reg_base))
		return -EFAULT;

	dprint(PRN_ALWS, "begin open scd device");
	vcodec_scd_set_irq_mask(dev_scd);
	dev_scd->status = VCODEC_SCD_STATUS_POWER_ON;
	dprint(PRN_ALWS, "scd open success");

	return 0;
}

static int32_t vcodec_scd_close(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}
	vcodec_scd_deinit_dev(dev_scd);
	dev_scd->status = VCODEC_SCD_STATUS_POWER_OFF;
	return 0;
}

static int32_t vcodec_scd_resume(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_scd->status != VCODEC_SCD_STATUS_SUSPEND)
		return 0;

	vcodec_scd_set_irq_mask(dev_scd);
	dev_scd->status = VCODEC_SCD_STATUS_POWER_ON;
	return 0;
}

static int32_t vcodec_scd_suspend(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_scd->status != VCODEC_SCD_STATUS_POWER_ON)
		return 0;

	dev_scd->status = VCODEC_SCD_STATUS_SUSPEND;
	return 0;
};

static int32_t vcodec_scd_control(struct vcodec_dev* dev, uint32_t msg_id, void *msg, uint32_t msg_size)
{
	const struct vcodec_scd_control_proc_tbl *tbl = NULL;
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	tbl = vcodec_scd_get_control_proc_tbl();
	while (tbl->cmd != SCD_INVALID_CMD_ID) {
		if (tbl->cmd == msg_id)
			break;

		tbl++;
	}
	if (tbl->cmd != msg_id || !tbl->proc)
		return -EFAULT;

	if ((tbl->msg_size) && ((tbl->msg_size != msg_size) || (!msg)))
		return -EFAULT;

	if (tbl->validate_scd_status != VCODEC_SCD_STATUS_BUTT &&
		tbl->validate_scd_status != dev_scd->status)
		return -EFAULT;

	return tbl->proc(dev_scd, msg);
}

static int32_t vcodec_scd_reset(struct vcodec_dev* dev, void *reset_info)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_scd->status != VCODEC_SCD_STATUS_POWER_ON)
		return 0;

	if (vcodec_scd_reset_dev(dev_scd)) {
		dev_scd->status = VCODEC_SCD_STATUS_FAULT;
		return -EFAULT;
	}

	dev_scd->status = VCODEC_SCD_STATUS_POWER_ON;
	return 0;
};

static void vcodec_scd_destroy(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	kfree(dev_scd);
	dev_scd = NULL;
}

static irqreturn_t vcodec_scd_irq_proc(struct vcodec_dev* dev)
{
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return vcodec_scd_interrupt_handling(dev_scd);
}

static void vcodec_scd_proc_read(struct vcodec_dev* dev, void* file)
{
	int used = 0;
	struct vcodec_dev_scd* dev_scd = vcodec_get_scd_dev_and_validate(dev);
	if (unlikely(!dev_scd)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	if (dev_scd->status != VCODEC_SCD_STATUS_POWER_ON)
		return;

}

struct vcodec_dev* vcodec_scd_create_dev(struct device *device)
{
	struct vcodec_dev_scd* dev = NULL;
	if (unlikely(!device))
		return NULL;

	dev = kzalloc(sizeof(struct vcodec_dev_scd), GFP_KERNEL);
	if (!dev)
		return NULL;

	// init the callback function tabel
	dev->dev.open = vcodec_scd_open;
	dev->dev.close = vcodec_scd_close;
	dev->dev.suspend = vcodec_scd_suspend;
	dev->dev.resume = vcodec_scd_resume;
	dev->dev.reset = vcodec_scd_reset;
	dev->dev.destroy = vcodec_scd_destroy;
	dev->dev.control = vcodec_scd_control;
	dev->dev.irq_proc = vcodec_scd_irq_proc;
	dev->dev.proc_read = vcodec_scd_proc_read;

	// init basic scd parameter
	dev->type = VCODEC_DEV_SCD;
	dev->size = sizeof(struct vcodec_dev_scd);
	dev->device = device;

	// init scd attribute
	dev->poll_irq_enable = 0;

	vcodec_osal_event_init(&dev->event, 0);
	vcodec_osal_sema_init(&dev->sema);

	dev->status = VCODEC_SCD_STATUS_POWER_OFF;
	return &dev->dev;
}
