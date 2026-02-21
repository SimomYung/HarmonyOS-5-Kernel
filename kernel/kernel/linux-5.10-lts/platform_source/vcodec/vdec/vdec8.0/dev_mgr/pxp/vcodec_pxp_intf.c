/*
 * vcodec_pxp_intf.c
 *
 * Implement interface of vcodec pxp device
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
#include "vcodec_pxp.h"
#include "dbg.h"

static struct vcodec_dev_pxp* vcodec_get_pxp_dev_and_validate(struct vcodec_dev* dev)
{
	struct vcodec_dev_pxp* dev_pxp = (struct vcodec_dev_pxp*)dev;

	if (unlikely(!dev_pxp))
		return NULL;

	if (unlikely(dev_pxp->type != VCODEC_DEV_PXP))
		return NULL;

	if (unlikely(dev_pxp->size != sizeof(struct vcodec_dev_pxp)))
		return NULL;

	return dev_pxp;
}


static int32_t vcodec_pxp_open(struct vcodec_dev* dev, uint8_t *reg_base)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp || !reg_base)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	dprint(PRN_ALWS, "begin open pxp device");
	if (vcodec_pxp_init_dev(dev_pxp, reg_base))
		return -EFAULT;

	if (vcodec_pxp_load_image(dev_pxp)) {
		vcodec_pxp_deinit_dev(dev_pxp);
		return -EFAULT;
	}

	if (vcodec_pxp_alloc_syscache(dev_pxp)) {
		vcodec_pxp_free_image(dev_pxp);
		vcodec_pxp_deinit_dev(dev_pxp);
		return -ENOMEM;
	}

	vcodec_pxp_config_sc(dev_pxp);

	vcodec_pxp_set_irq_mask(dev_pxp);

	if (vcodec_pxp_open_dev(dev_pxp)) {
		vcodec_pxp_free_syscache(dev_pxp);
		vcodec_pxp_free_image(dev_pxp);
		vcodec_pxp_deinit_dev(dev_pxp);
		return -EFAULT;
	}

	dev_pxp->status = VCODEC_PXP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "open pxp device success");

	return 0;
}

static int32_t vcodec_pxp_close(struct vcodec_dev* dev)
{
	int32_t ret = 0;
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	dprint(PRN_ALWS, "begin close pxp device");
	if (dev_pxp->status == VCODEC_PXP_STATUS_POWER_ON)
		ret = vcodec_pxp_close_dev(dev_pxp);
	dev_pxp->status = VCODEC_PXP_STATUS_POWER_OFF;

	vcodec_pxp_free_syscache(dev_pxp);
	vcodec_pxp_free_image(dev_pxp);
	vcodec_pxp_deinit_dev(dev_pxp);
	return ret;
}

static int32_t vcodec_pxp_resume(struct vcodec_dev* dev)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_pxp->status != VCODEC_PXP_STATUS_SUSPEND)
		return 0;

	dprint(PRN_ALWS, "begin resume pxp device");
	vcodec_pxp_set_irq_mask(dev_pxp);

	if (vcodec_pxp_open_dev(dev_pxp)) {
		dev_pxp->status = VCODEC_PXP_STATUS_FAULT;
		return -EFAULT;
	}

	dev_pxp->status = VCODEC_PXP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "resume pxp device success");
	return 0;
}

static int32_t vcodec_pxp_suspend(struct vcodec_dev* dev)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_pxp->status != VCODEC_PXP_STATUS_POWER_ON)
		return 0;

	dprint(PRN_ALWS, "begin suspend pxp device");
	(void)vcodec_pxp_close_dev(dev_pxp);

	dev_pxp->status = VCODEC_PXP_STATUS_SUSPEND;
	dprint(PRN_ALWS, "suspend pxp device success");
	return 0;
};


static int32_t vcodec_pxp_control(struct vcodec_dev* dev, uint32_t msg_id, void *msg, uint32_t msg_size)
{
	const struct vcodec_pxp_control_proc_tbl *tbl = NULL;
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	tbl = vcodec_pxp_get_control_proc_tbl();
	while (tbl->cmd != PXP_INVALID_CMD_ID) {
		if (tbl->cmd == msg_id)
			break;

		tbl++;
	}
	if (tbl->cmd != msg_id || !tbl->proc)
		return -EFAULT;

	if ((tbl->msg_size) && ((tbl->msg_size != msg_size) || (!msg)))
		return -EFAULT;

	if (tbl->validate_pxp_status != VCODEC_PXP_STATUS_BUTT &&
		tbl->validate_pxp_status != dev_pxp->status)
		return -EFAULT;

	return tbl->proc(dev_pxp, msg);
}

static int32_t vcodec_pxp_reset(struct vcodec_dev* dev, void* reset_info)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	struct pxp_reset_info* info = (struct pxp_reset_info*)reset_info;
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	if (dev_pxp->status != VCODEC_PXP_STATUS_POWER_ON)
		return 0;

	dprint(PRN_ALWS, "begin reset pxp device chan %d frm %d", info->chan_id, info->frm_num);
	if (vcodec_pxp_reset_dev(dev_pxp)) {
		dev_pxp->status = VCODEC_PXP_STATUS_FAULT;
		return -EFAULT;
	}

	dev_pxp->status = VCODEC_PXP_STATUS_POWER_ON;
	dprint(PRN_ALWS, "reset pxp device success");
	return 0;
};

static void vcodec_pxp_destroy(struct vcodec_dev* dev)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	vcodec_osal_event_exit(dev_pxp->pxp_power_on_ack);
	vcodec_osal_event_exit(dev_pxp->pxp_power_off_ack);

	kfree(dev_pxp);
	dev_pxp = NULL;
}

static irqreturn_t vcodec_pxp_irq_proc(struct vcodec_dev* dev)
{
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return -EINVAL;
	}

	return vcodec_pxp_interrupt_handling(dev_pxp);
}

static void vcodec_pxp_proc_read(struct vcodec_dev* dev, void* file)
{
	int used = 0;
	struct vcodec_dev_pxp* dev_pxp = vcodec_get_pxp_dev_and_validate(dev);
	if (unlikely(!dev_pxp)) {
		dprint(PRN_FATAL, "invalid device");
		return;
	}

	if (dev_pxp->status != VCODEC_PXP_STATUS_POWER_ON)
		return;

}

struct vcodec_dev* vcodec_pxp_create_dev(struct device *device)
{
	struct vcodec_dev_pxp* dev = NULL;
	if (unlikely(!device))
		return NULL;

	dev = kzalloc(sizeof(struct vcodec_dev_pxp), GFP_KERNEL);
	if (!dev)
		return NULL;

	// init the callback function tabel
	dev->dev.open = vcodec_pxp_open;
	dev->dev.close = vcodec_pxp_close;
	dev->dev.suspend = vcodec_pxp_suspend;
	dev->dev.resume = vcodec_pxp_resume;
	dev->dev.reset = vcodec_pxp_reset;
	dev->dev.destroy = vcodec_pxp_destroy;
	dev->dev.control = vcodec_pxp_control;
	dev->dev.irq_proc = vcodec_pxp_irq_proc;
	dev->dev.proc_read = vcodec_pxp_proc_read;

	// init basic pxp parameter
	dev->type = VCODEC_DEV_PXP;
	dev->size = sizeof(struct vcodec_dev_pxp);
	dev->device = device;

	// init pxp attribute
	dev->multi_core_enable = 1;
	dev->syscache_enable = 1;
	dev->is_sc_allocated = 0;

	vcodec_osal_event_init(&dev->pxp_power_on_ack, 0);
	vcodec_osal_event_init(&dev->pxp_power_off_ack, 0);

	dev->status = VCODEC_PXP_STATUS_POWER_OFF;
	return &dev->dev;
}
