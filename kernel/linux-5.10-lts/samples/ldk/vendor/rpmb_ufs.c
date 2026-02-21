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

#ifdef CONFIG_RPMB_SET_MULTI_KEY
#define MAX_KEY_STATUS_NUM MAX_RPMB_REGION_NUM
#else
#define MAX_KEY_STATUS_NUM 1
#endif

struct combine_cmd {
	struct ufs_blk_ioc_data *data;
	struct rpmb_frame *frame;
	int is_write;
	unsigned short blocks;
	unsigned short blksz;
	u8 *sdb_command;
	u8 *sense_buffer;
	struct rpmb_frame *transfer_frame;
};

#define CMD_PARA_SET(_cmd, _data, _frame, _is_write, _blocks, _blksz,	\
		     _sdb_command, _sense_buffer, _transfer_frame)	\
	do {								\
		(_cmd)->data = (_data);					\
		(_cmd)->frame = (_frame);				\
		(_cmd)->is_write = (_is_write);				\
		(_cmd)->blocks = (_blocks);				\
		(_cmd)->blksz = (_blksz);				\
		(_cmd)->sdb_command = (_sdb_command);			\
		(_cmd)->sense_buffer = (_sense_buffer);			\
		(_cmd)->transfer_frame = (_transfer_frame);		\
	} while (0)

static u64 g_hisee_counter_lock = 0;

struct rpmb_time_record {
	int state;
	u64 resource_alloc;
	u64 cmd_lock_done;
	u64 cmd_lock_release;
};

enum rpmb_req_source {
	RPMB_FROM_SECURE_OS,
	RPMB_FROM_MSPC,
	RPMB_FROM_AUTH_KEY,
	RPMB_FROM_MAX,
};

#define BSG_DEFAULT_CMDS	64
#define UFS_RPMB_BLOCK_DEVICE_NAME_WITHOUT_PATH "49476"

struct bsg_class_device *rpmb_bcd;

// hack blk_scsi_kern.c
#define TIME_RECORD_COUNT 3
#define IO_RECORD_COUNT 3
struct blk_scsi_device {
	struct request_queue *queue;
	spinlock_t lock;
	struct list_head busy_list;
	struct list_head done_list;
	struct hlist_node dev_list;
	atomic_t ref_count;
	int queued_cmds;
	int done_cmds;
	wait_queue_head_t wq_done;
	wait_queue_head_t wq_free;
	char name[20];
	int max_queue;
	unsigned long flags;
};

struct rpmb_time_blk_record {
	u64 req_get_done[IO_RECORD_COUNT];
	u64 req_exec_start[IO_RECORD_COUNT];
	u64 req_exec_end[IO_RECORD_COUNT];
};

static struct rpmb_time_blk_record rpmb_time_blk_point[TIME_RECORD_COUNT];

enum time_record_type {
	REQ_GET_DONE,
	REQ_EXEC_START,
	REQ_EXEC_END,
};

static void blk_scsi_kern_time_record(enum time_record_type type,
				      unsigned int source_id,
				      unsigned int io_cnt)
{
	if (source_id >= TIME_RECORD_COUNT || io_cnt >= IO_RECORD_COUNT) {
		pr_err("%s: out of bound\n", __func__);
		return;
	}
	switch(type) {
	case REQ_GET_DONE:
		rpmb_time_blk_point[source_id].req_get_done[io_cnt] =
			dfx_getcurtime();
		break;
	case REQ_EXEC_START:
		rpmb_time_blk_point[source_id].req_exec_start[io_cnt] =
			dfx_getcurtime();
		break;
	case REQ_EXEC_END:
		rpmb_time_blk_point[source_id].req_exec_end[io_cnt] =
			dfx_getcurtime();
		break;
	}
}

static int blk_kern_fill_sgv4_hdr_rq(struct request_queue *q,
				     struct request *rq,
				     struct sg_io_v4 *hdr)
{
	struct scsi_request *req = scsi_req(rq);

	if (hdr->request_len > BLK_MAX_CDB) {
		req->cmd = kzalloc((size_t)hdr->request_len, GFP_KERNEL);
		if (!req->cmd)
			return -ENOMEM;
	}

	memcpy((void *)req->cmd, /* unsafe_function_ignore: memcpy */
	       (void *)(uintptr_t)hdr->request,
	       (unsigned long)hdr->request_len);

	/*
	 * fill in request structure
	 */
	req->cmd_len = (unsigned short)hdr->request_len;

	rq->timeout = (unsigned int)msecs_to_jiffies(hdr->timeout);
	if (!rq->timeout)
		rq->timeout = q->sg_timeout;
	if (!rq->timeout)
		rq->timeout = BLK_DEFAULT_SG_TIMEOUT;
	if (rq->timeout < BLK_MIN_SG_TIMEOUT)
		rq->timeout = BLK_MIN_SG_TIMEOUT;

	return 0;
}

/*
 * Check if sg_io_v4 from kernel is allowed and valid
 */
static int blk_kern_validate_sgv4_hdr(struct sg_io_v4 *hdr, int *op)
{
	int ret = 0;

	if (hdr->guard != 'Q')
		return -EINVAL;

	switch (hdr->protocol) {
	case BSG_PROTOCOL_SCSI:
		switch (hdr->subprotocol) {
		case BSG_SUB_PROTOCOL_SCSI_CMD:
		case BSG_SUB_PROTOCOL_SCSI_TRANSPORT:
			break;
		default:
			ret = -EINVAL;
		}
		break;
	default:
		ret = -EINVAL;
	}

	*op = hdr->dout_xfer_len ? REQ_OP_SCSI_OUT : REQ_OP_SCSI_IN;
	return ret;
}

/*
 * map sg_io_v4 to a request.
 */
static struct request *blk_kern_map_hdr(struct blk_scsi_device *bd,
					struct sg_io_v4 *hdr,
					unsigned int source_id, unsigned int io_cnt)
{
	struct request_queue *q = bd->queue;
	struct request *rq = NULL;
	int ret, op;
	unsigned int dxfer_len;
	void *dxferp = NULL;
	struct bsg_class_device *bcd = &q->bsg_dev;

	/* if the LLD has been removed then the bsg_unregister_queue will
	 * eventually be called and the class_dev was freed, so we can no
	 * longer use this request_queue. Return no such address.
	 */
	if (!bcd->class_dev)
		return ERR_PTR(-ENXIO);

	ret = blk_kern_validate_sgv4_hdr(hdr, &op);
	if (ret)
		return ERR_PTR(ret);

	/*
	 * map scatter-gather elements separately and string them to request
	 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	rq = blk_get_request(q, op, 0);
#else
	rq = blk_get_request(q, op, GFP_KERNEL);
#endif
	if (IS_ERR(rq))
		return rq;

	blk_scsi_kern_time_record(REQ_GET_DONE, source_id, io_cnt);
	ret = blk_kern_fill_sgv4_hdr_rq(q, rq, hdr);
	if (ret)
		goto out;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
	if (op == REQ_OP_SCSI_OUT && hdr->din_xfer_len) {
		if (!test_bit(QUEUE_FLAG_BIDI, &q->queue_flags)) {
			ret = -EOPNOTSUPP;
			goto out;
		}
	}
#endif
	if (hdr->dout_xfer_len) {
		dxfer_len = hdr->dout_xfer_len;
		dxferp = (void *)(uintptr_t)hdr->dout_xferp;
	} else if (hdr->din_xfer_len) {
		dxfer_len = hdr->din_xfer_len;
		dxferp = (void *)(uintptr_t)hdr->din_xferp;
	} else {
		dxfer_len = 0;
	}

	if (dxfer_len) {
		ret = blk_rq_map_kern(q, rq, dxferp, dxfer_len, GFP_KERNEL);
		if (ret)
			goto out;
	}

	return rq;
out:
	scsi_req_free_cmd(scsi_req(rq));
	blk_put_request(rq);
	return ERR_PTR(ret);
}

static int blk_kern_complete_hdr_rq(struct request *rq, struct sg_io_v4 *hdr)
{
	struct scsi_request *req = scsi_req(rq);
	int ret = 0;

	/*
	 * fill in all the output members
	 */
	hdr->device_status = (unsigned int)req->result & 0xff;
	hdr->transport_status = host_byte((unsigned int)(req->result));
	hdr->driver_status = driver_byte((unsigned int)(req->result));
	hdr->info = 0;
	if (hdr->device_status || hdr->transport_status || hdr->driver_status)
		hdr->info |= SG_INFO_CHECK;
	hdr->response_len = 0;

	if (req->sense_len && hdr->response) {
		unsigned int len = min_t(unsigned int, hdr->max_response_len,
				req->sense_len);

		memcpy((void *)(uintptr_t)hdr->response, /* unsafe_function_ignore: memcpy */
		       req->sense,
		       (unsigned long)len);
		hdr->response_len = len;
	}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	if (rq_data_dir(rq) == READ) {
#else
	if (rq->next_rq) {
		hdr->dout_resid = (__s32)req->resid_len;
		hdr->din_resid = (__s32)scsi_req(rq->next_rq)->resid_len;
		blk_put_request(rq->next_rq);
	} else if (rq_data_dir(rq) == READ) {
#endif
		hdr->din_resid = (__s32)req->resid_len;
	} else {
		hdr->dout_resid = (__s32)req->resid_len;
	}

	/*
	 * If the request generated a negative error number, return it
	 * (providing we aren't already returning an error); if it's
	 * just a protocol response (i.e. non negative), that gets
	 * processed above.
	 */
	if (!ret && req->result < 0)
		ret = req->result;

	scsi_req_free_cmd(req);
	blk_put_request(rq);

	return ret;
}

static long blk_scsi_kern_ioctl_for_hook(struct blk_scsi_device *bd, unsigned long arg, bool need_order,
					 unsigned int source_id, unsigned int io_cnt)
{
	int *uarg = (int *)(uintptr_t)arg;
	int ret;
	struct request *rq = NULL;
	struct sg_io_v4 hdr;
	int at_head;

	memcpy(&hdr, uarg, sizeof(hdr)); /* unsafe_function_ignore: memcpy */
	rq = blk_kern_map_hdr(bd, &hdr, source_id, io_cnt);
	if (IS_ERR(rq))
		return PTR_ERR(rq);
#if defined(CONFIG_MAS_ORDER_PRESERVE) || defined(CONFIG_MAS_UNISTORE_PRESERVE)
	if (need_order)
		blk_req_set_make_req_nr(rq);
#endif
	at_head = (0 == (hdr.flags & BSG_FLAG_Q_AT_TAIL));
	blk_scsi_kern_time_record(REQ_EXEC_START, source_id, io_cnt);
	blk_execute_rq(bd->queue, NULL, rq, at_head);
	blk_scsi_kern_time_record(REQ_EXEC_END, source_id, io_cnt);
	ret = blk_kern_complete_hdr_rq(rq, &hdr);

	memcpy(uarg, &hdr, sizeof(hdr)); /* unsafe_function_ignore: memcpy */

	return ret;
}
// end hack for blk_scsi_kern.c

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
static int rpmb_send_ufs_verify(struct blk_scsi_device *bd)
{
	struct request_queue *q = bd->queue;
	struct scsi_device *sdev = q->queuedata;
	struct Scsi_Host *shost = sdev->host;
	struct ufs_hba *hba = shost_priv(shost);
	int time_out = 1500; /* 1.5 seconds */
	int ret;

	/* if not 935, return */
#ifdef CONFIG_RPMB_DEBUG_FS
	pr_err("[%s] %s\n", __func__, hba->model);
#endif
	if (memcmp(hba->model, UFS_MODEL_THOR935, sizeof(UFS_MODEL_THOR935) - 1) != 0)
		return 0;

	mutex_lock(&hba->dev_cmd.lock);
	ret = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_VERIFY, time_out);
	mutex_unlock(&hba->dev_cmd.lock);
#ifdef CONFIG_RPMB_DEBUG_FS
	pr_err("[%s] verify ret %d\n", __func__, ret);
#endif

	return ret;
}
#endif

static int ufs_bsg_ioctl_rpmb_driver(struct blk_scsi_device *bd, struct ufs_blk_ioc_rpmb_data *rdata,
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
		read_command = rdata->sdb_command[i][0];
		read_request = be16_to_cpu(((struct rpmb_frame *)rdata->data[0].buf)->request);
		if ((read_command == 0xA2) && (read_request == RPMB_REQ_READ_DATA))
			rpmb_send_ufs_verify(bd);
#endif

		while (retry_times < RPMB_IOCTL_RETRY_TIMES) {
			ret = (int32_t)blk_scsi_kern_ioctl_for_hook(bd,
				(uintptr_t)(&(rdata->data[i].siv)),
				(!i && is_write), (unsigned int)source, i);
			if (!ret && !rdata->data[i].siv.info) {
				break;
			} else {
				pr_err("rpmb ioctl [%d], ret %d, status code: "
				       "driver_status = %d "
				       "transport_status = %d "
				       "device_status = %d!\n",
				       i, ret, rdata->data[i].siv.driver_status,
				       rdata->data[i].siv.transport_status,
				       rdata->data[i].siv.device_status);
				pr_err("ufs rpmb sense data is %d\n",
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
	}
	return ret;
}

static void hook_ufs_bsg_ioctl_rpmb(void *__data, struct ufs_blk_ioc_rpmb_data *rdata,
				   bool is_write, int id, int source, int *ret)
{
	int32_t rc = -RPMB_ERR_GENERAL;

	if (rpmb_bcd == NULL)
		return;

	struct blk_scsi_device *bd = (struct blk_scsi_device *)get_rpmb_private_data(rpmb_bcd->queue);
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
