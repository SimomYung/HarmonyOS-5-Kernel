/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: dfa scsi implement
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <securec.h>
#include <scsi/scsi.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_driver.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_host.h>

#include "scsi_debugfs.h"
#include "scsi_priv.h"
#include "scsi_logging.h"
#include "scsi_dfa.h"
#include "platform_include/basicplatform/linux/mas/mas_blk.h"

#define SCMD_RESULT_BIT_MASK 24
#define SREQ_RESULT_BIT_MASK 16

static bool dfa_cmd_retry_allowed(struct scsi_cmnd *cmd)
{
	if (cmd->allowed == SCSI_CMD_RETRIES_NO_LIMIT)
		return true;

	return cmd->retries < cmd->allowed;
}

static void dfa_uninit_cmd(struct scsi_cmnd *cmd)
{
	struct scsi_driver *drv = NULL;

	if (blk_rq_is_passthrough(cmd->request))
		return;

	drv = scsi_cmd_to_driver(cmd);
	if (drv->uninit_command)
		drv->uninit_command(cmd);
}

/**
 * dfa_queue_insert - private queue insertion
 * @cmd: The SCSI command being requeued
 * @reason:  The reason for the requeue
 */
void dfa_queue_insert(struct scsi_cmnd *cmd, int reason)
{
#ifdef CONFIG_MAS_BLK
	if (cmd->request)
		mas_blk_latency_req_check(cmd->request, REQ_PROC_STAGE_SCSI_INSERT);
#endif

	SCSI_LOG_MLQUEUE(1, scmd_printk(KERN_INFO, cmd,
		"Inserting command %p into mlqueue\n", cmd));

	/*
	 * Requeue this command.  It will go before all other commands
	 * that are already in the queue. Schedule requeue work under
	 * lock such that the kblockd_schedule_work() call happens
	 * before blk_cleanup_queue() finishes.
	 */
	cmd->result = 0;

	blk_mq_requeue_request(cmd->request, true);
}

void dfa_eh_scmnd(struct scsi_cmnd *scmd)
{
	struct Scsi_Host *shost = scmd->device->host;

	if (!scsi_device_online(scmd->device) ||
		 scsi_noretry_cmd(scmd)) {
		if (!scmd->result)
			scmd->result |= (DRIVER_TIMEOUT << SCMD_RESULT_BIT_MASK);
		SCSI_LOG_ERROR_RECOVERY(3,
			scmd_printk(KERN_ERR, scmd, "%s: flush finish cmd\n", current->comm));
#ifdef CONFIG_MAS_ORDER_PRESERVE
		scmd->eh_eflags |= SCSI_EH_IN_FLUSH_DONE_Q;
		scsi_finish_command(scmd);
		scmd->eh_eflags &= (~SCSI_EH_IN_FLUSH_DONE_Q);
#else
		scsi_finish_command(scmd);
#endif
		return;
	}

	dfa_queue_insert(scmd, SCSI_MLQUEUE_EH_RETRY);
	if (dfa_cmd_retry_allowed(scmd))
		pr_err("%s, cmd:0x%x retry %d times\n", __func__,
			scmd->cmnd[0], scmd->retries);
	else
		dfa_schedule_eh(shost);
	scmd->retries++;
}

enum blk_eh_timer_return dfa_times_out(struct request *req)
{
	struct scsi_cmnd *scmd = blk_mq_rq_to_pdu(req);
	enum blk_eh_timer_return rtn = BLK_EH_DONE;
	struct Scsi_Host *host = scmd->device->host;
	struct scsi_device *sdev = scmd->device;
	unsigned long cmd_age;
	int ret;

	scsi_log_completion(scmd, TIMEOUT_ERROR);

	cmd_age = (jiffies - scmd->jiffies_at_alloc) / HZ;
	if (cmd_age == 0) {
		pr_err("%s, [DFA] cmd_age error, current:%lu, alloc_at:%lu, HZ:%lu\n",
			__func__, jiffies, scmd->jiffies_at_alloc, HZ);
		return BLK_EH_RESET_TIMER;
	}

	if (host->eh_deadline != -1 && !host->last_reset)
		host->last_reset = jiffies;

	if (host->hostt->eh_timed_out)
		rtn = host->hostt->eh_timed_out(scmd);

	if (rtn != BLK_EH_DONE)
		return rtn;

	if (test_and_set_bit(SCMD_STATE_COMPLETE, &scmd->state))
		return BLK_EH_RESET_TIMER;

	set_host_byte(scmd, DID_TIME_OUT);
	ret = sdev->host->hostt->eh_abort_handler(scmd);
	if (ret != SUCCESS)
		SCSI_LOG_ERROR_RECOVERY(3,
			scmd_printk(KERN_ERR, scmd, "cmd abort %s\n",
			(ret == FAST_IO_FAIL) ? "not send" : "failed"));

	dfa_eh_scmnd(scmd);

	return rtn;
}

blk_status_t dfa_queue_rq(struct blk_mq_hw_ctx *hctx,
			 const struct blk_mq_queue_data *bd)
{
	struct request *req = bd->rq;
	struct request_queue *q = req->q;
	struct scsi_device *sdev = q->queuedata;
	struct scsi_cmnd *cmd = blk_mq_rq_to_pdu(req);
	blk_status_t ret;
	int reason;

	if (!(req->rq_flags & RQF_DONTPREP)) {
		ret = scsi_prepare_cmd(req);
		if (ret != BLK_STS_OK) {
#ifndef CONFIG_DFA_MCQ_OHOS
			mas_blk_latency_req_check(req, REQ_PROC_STAGE_SCSI_QUEUE_OUT);
#endif
			goto out;
		}
		req->rq_flags |= RQF_DONTPREP;
	} else {
		clear_bit(SCMD_STATE_COMPLETE, &cmd->state);
	}

	cmd->flags &= SCMD_PRESERVED_FLAGS;
	if (sdev->simple_tags)
		cmd->flags |= SCMD_TAGGED;
	if (bd->last)
		cmd->flags |= SCMD_LAST;

	scsi_set_resid(cmd, 0);
	(void)memset_s(cmd->sense_buffer, SCSI_SENSE_BUFFERSIZE, 0, SCSI_SENSE_BUFFERSIZE);
	cmd->scsi_done = scsi_mq_done;

	blk_mq_start_request(req);
	reason = scsi_dispatch_cmd(cmd);
	if (reason) {
#ifndef CONFIG_DFA_MCQ_OHOS
		mas_blk_latency_req_check(req, REQ_PROC_STAGE_SCSI_QUEUE_HOST);
#endif
		ret = BLK_STS_RESOURCE;
		goto out;
	}

	return BLK_STS_OK;

out:
	switch (ret) {
	case BLK_STS_OK:
	case BLK_STS_RESOURCE:
		break;
	case BLK_STS_ZONE_RESOURCE:
		if (scsi_device_blocked(sdev))
			ret = BLK_STS_DEV_RESOURCE;
		break;
	default:
		if (unlikely(!scsi_device_online(sdev)))
			scsi_req(req)->result = DID_NO_CONNECT << SREQ_RESULT_BIT_MASK;
		else
			scsi_req(req)->result = DID_ERROR << SREQ_RESULT_BIT_MASK;
		/*
		 * Make sure to release all allocated resources when
		 * we hit an error, as we will never see this command
		 * again.
		 */
		if (req->rq_flags & RQF_DONTPREP) {
			scsi_free_sgtables(cmd);
			dfa_uninit_cmd(cmd);
		}
		scsi_run_queue_async(sdev);
		break;
	}

	return ret;
}
