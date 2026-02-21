/*
 * vcodec_mcu_intf.c
 *
 * Implement interface of vcodec mcu device
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
#include "vcodec_mcu.h"
#include "dbg.h"

static struct vcodec_dev_mcu* vcodec_get_mcu_dev_and_validate(struct vcodec_dev* dev)
{
	struct vcodec_dev_mcu* dev_mcu = (struct vcodec_dev_mcu*)dev;

	if (unlikely(!dev_mcu))
		return NULL;

	if (unlikely(dev_mcu->type != VCODEC_DEV_MCU))
		return NULL;

	if (unlikely(dev_mcu->size != sizeof(struct vcodec_dev_mcu)))
		return NULL;

	return dev_mcu;
}

static int32_t vcodec_mcu_open(struct vcodec_dev* dev, uint8_t *reg_base)
{
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu || !reg_base)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return vcodec_mcu_dev_open(dev_mcu, reg_base);
}

static int32_t vcodec_mcu_close(struct vcodec_dev* dev)
{
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	vcodec_mcu_dev_close(dev_mcu);
	return 0;
}

static int32_t vcodec_mcu_resume(struct vcodec_dev* dev)
{
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return  vcodec_mcu_dev_resume(dev_mcu);
}

static int32_t vcodec_mcu_suspend(struct vcodec_dev* dev)
{
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return vcodec_mcu_dev_suspend(dev_mcu);
};

static void vcodec_mcu_destroy(struct vcodec_dev* dev)
{
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	vcodec_mcu_dev_deinit(dev_mcu);

	kfree(dev_mcu);
	dev_mcu = NULL;
}

static int32_t vcodec_mcu_control(struct vcodec_dev* dev, uint32_t msg_id, void *msg, uint32_t msg_size)
{
	const struct vcodec_mcu_control_proc_tbl *tbl = NULL;
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	tbl = vcodec_mcu_dev_get_control_proc_tbl();
	while (tbl->cmd != MCU_INVALID_CMD_ID) {
		if (tbl->cmd == msg_id)
			break;

		tbl++;
	}
	if (tbl->cmd != msg_id || !tbl->proc)
		return -EFAULT;

	if ((tbl->msg_size) && ((tbl->msg_size != msg_size) || (!msg)))
		return -EFAULT;

	if (tbl->validate_mcu_status != VCODEC_MCU_STATUS_BUTT &&
		tbl->validate_mcu_status != dev_mcu->status)
		return -EFAULT;

	return tbl->proc(dev_mcu, msg);
}

static void vcodec_mcu_proc_read(struct vcodec_dev* dev, void* file)
{
	int used = 0;
	struct vcodec_dev_mcu* dev_mcu = vcodec_get_mcu_dev_and_validate(dev);
	if (unlikely(!dev_mcu)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	if (dev_mcu->status != VCODEC_MCU_STATUS_POWER_ON)
		return;

}

struct vcodec_dev* vcodec_mcu_create_dev(struct device *device)
{
	struct vcodec_dev_mcu* dev = NULL;
	if (unlikely(!device))
		return NULL;

	dev = kzalloc(sizeof(struct vcodec_dev_mcu), GFP_KERNEL);
	if (!dev)
		return NULL;
	
	vcodec_mcu_dev_init(dev);

	// init the callback function tabel
	dev->dev.open = vcodec_mcu_open;
	dev->dev.close = vcodec_mcu_close;
	dev->dev.suspend = vcodec_mcu_suspend;
	dev->dev.resume = vcodec_mcu_resume;
	dev->dev.reset = NULL;
	dev->dev.destroy = vcodec_mcu_destroy;
	dev->dev.control = vcodec_mcu_control;
	dev->dev.irq_proc = NULL;
	dev->dev.proc_read = vcodec_mcu_proc_read;

	// init basic mcu parameter
	dev->type = VCODEC_DEV_MCU;
	dev->size = sizeof(struct vcodec_dev_mcu);
	dev->device = device;

	return &dev->dev;
}
