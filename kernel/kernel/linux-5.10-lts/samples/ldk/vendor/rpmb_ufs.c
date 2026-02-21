// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: HACK RPMB IN LIBLINUX
 * Create: 2023-06-21
 */

#include <linux/delay.h>
#include <linux/syscalls.h>
#include <linux/bootdevice/bootdevice.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/jiffies.h>
#include <linux/percpu.h>
#include <linux/uio.h>
#include <linux/idr.h>
#include <linux/bsg.h>
#include <linux/types.h>
#include <linux/rpmb/rpmb.h>
#ifndef CONFIG_DFX_TIME
#include <linux/sched/clock.h>
#endif

#include <asm/uaccess.h>
#include <scsi/sg.h>
#include <scsi/scsi.h>
#include <scsi/scsi_ioctl.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_driver.h>
#include <scsi/scsi_request.h>

#include <platform_include/see/bl31_smc.h>
#include <trace/hooks/liblinux.h>

#include "vendor_rpmb.h"
#ifdef CONFIG_RPMB_UFS_VERIFY
#include "ufshcd.h"
#endif
#if defined(CONFIG_MAS_ORDER_PRESERVE) || defined(CONFIG_MAS_UNISTORE_PRESERVE)
#include "mas_blk.h"
#endif

#define UFS_RPMB_BLOCK_DEVICE_NAME_WITHOUT_PATH "49476"

extern struct rpmb_time_record rpmb_time_point[RPMB_FROM_MAX];
struct bsg_class_device *rpmb_bcd;


void hook_get_rpmb_bcd(void *__data, const char *devname, struct bsg_class_device *bcd)
{
	if (strstr(devname, UFS_RPMB_BLOCK_DEVICE_NAME_WITHOUT_PATH) != NULL) {
		rpmb_bcd = bcd;
	}
}

static struct bsg_device *rpmb_alloc_device(void)
{
	struct bsg_device *bd;

	bd = kzalloc(sizeof(*bd), GFP_KERNEL);
	if (unlikely(!bd))
		return NULL;
	return bd;
}

static struct bsg_device *get_rpmb_private_data(struct request_queue *rq)
{
	struct bsg_device *bd;

	if (!blk_queue_scsi_passthrough(rq)) {
		WARN_ONCE(true, "Attempt to register a non-SCSI queue\n");
		return ERR_PTR(-EINVAL);
	}

	if (!blk_get_queue(rq))
		return ERR_PTR(-ENXIO);

	bd = rpmb_alloc_device();
	if (!bd) {
		blk_put_queue(rq);
		return ERR_PTR(-ENOMEM);
	}

	bd->queue = rq;

	return bd;
}

#ifdef CONFIG_RPMB_UFS_VERIFY
static int rpmb_send_ufs_verify(struct bsg_device *bd)
{
	struct request_queue *q = bd->queue;
	struct scsi_device *sdev = q->queuedata;
	struct Scsi_Host *shost = sdev->host;
	struct ufs_hba *hba = shost_priv(shost);
	int time_out = 1500; /* 1.5 seconds */
	int ret;

	mutex_lock(&hba->dev_cmd.lock);
	ret = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_VERIFY, time_out);
	mutex_unlock(&hba->dev_cmd.lock);
#ifdef CONFIG_RPMB_DEBUG_FS
	pr_err("[%s] ohos verify ret %d\n", __func__, ret);
#endif

	return ret;
}
#endif

static int ufs_bsg_ioctl_rpmb_driver(struct bsg_device *bd, struct ufs_blk_ioc_rpmb_data *rdata,
				     bool is_write, enum func_id id, enum rpmb_req_source source)
{
	int32_t i;
	int32_t ret = -RPMB_ERR_GENERAL;
	int32_t retry_times = 0;
#ifdef CONFIG_RPMB_UFS_VERIFY
	u8 read_command = 0;
	unsigned short read_request = 0;
#endif

	for (i = 0; i < UFS_IOC_MAX_RPMB_CMD; i++) {
		if (!rdata->data[i].siv.request_len)
			break;
#ifdef CONFIG_RPMB_UFS_VERIFY
		/* check thr935 */
		if (memcmp(g_rpmb_ufs_model, UFS_MODEL_THOR935, sizeof(UFS_MODEL_THOR935) - 1) == 0) {
			read_command = rdata->sdb_command[i][0];
			read_request = be16_to_cpu(((struct rpmb_frame *)rdata->data[0].buf)->request);
			if ((read_command == 0xA2) && (read_request == RPMB_REQ_READ_DATA))
				rpmb_send_ufs_verify(bd);
		}
#endif

		while (retry_times < RPMB_IOCTL_RETRY_TIMES) {
			ret = (int32_t)blk_scsi_kern_ioctl((void *)bd,
				(uintptr_t)(&(rdata->data[i].siv)),
				(!i && is_write), (unsigned int)source, i);
			if (!ret && !rdata->data[i].siv.info) {
				break;
			} else {
				pr_err("rpmb ioctl [%d], ret %d, status code: "
				       "driver_status = %u "
				       "transport_status = %u "
				       "device_status = %u!\n",
				       i, ret, rdata->data[i].siv.driver_status,
				       rdata->data[i].siv.transport_status,
				       rdata->data[i].siv.device_status);
				pr_err("ufs rpmb sense data is %u\n",
				       (*((u8*)(uintptr_t)rdata->data[i].siv.response + 2)));
			}
			retry_times++;
		}

		if (retry_times == RPMB_IOCTL_RETRY_TIMES) {
			pr_err("rpmb ufs ioctl retry failed, total retry "
			       "times is %d!\n", retry_times);
			return RPMB_ERR_IOCTL;
		}
		if (id == RPMB_FUNC_ID_SE) {
			if (rdata->data[i].siv.din_xfer_len)
				memcpy(rdata->data[i].buf, /* unsafe_function_ignore: memcpy */
				       (void *)(uintptr_t)rdata->data[i].siv.din_xferp,
				       rdata->data[i].buf_bytes); /* [false alarm]: buf size is same when alloc */
			else
				memcpy(rdata->data[i].buf, /* unsafe_function_ignore: memcpy */
				       (void *)(uintptr_t)rdata->data[i].siv.dout_xferp,
				       rdata->data[i].buf_bytes); /* [false alarm]: buf size is same when alloc */
		}
		retry_times = 0;
		rpmb_time_point[source].rpmb_ufs_start_time[i] = g_rpmb_ufs_start_time;
		rpmb_time_point[source].rpmb_ufs_end_time[i] = g_rpmb_ufs_end_time;
	}
	return ret;
}

static void hook_ufs_bsg_ioctl_rpmb(void *__data, struct ufs_blk_ioc_rpmb_data *rdata,
				   bool is_write, int id, int source, int *ret)
{
	int32_t rc = -RPMB_ERR_GENERAL;

	if (rpmb_bcd == NULL)
		return;

	struct bsg_device *bd = get_rpmb_private_data(rpmb_bcd->queue);
	rc = ufs_bsg_ioctl_rpmb_driver(bd, rdata, is_write, (enum func_id)id, (enum rpmb_req_source)source);
	if (rc != 0)
		pr_err("hook ufs rpmb ioctl ret is 0x%x\n", rc);
	kfree(bd);
	if (ret != NULL)
		*ret = rc;

	return;
}

// sys_open & sys_access are not supported, record bsg_class_device in bcd when create it, and operate bcd straightly
INIT_VENDOR_HOOK(ldk_rvh_get_rpmb_bcd, hook_get_rpmb_bcd);
INIT_VENDOR_HOOK(ldk_rvh_ufs_bsg_ioctl_rpmb, hook_ufs_bsg_ioctl_rpmb);
