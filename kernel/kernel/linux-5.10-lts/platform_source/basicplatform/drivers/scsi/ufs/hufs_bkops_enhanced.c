/*
 * hufs_bkops_enhanced.c
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

#include "hufs_bkops_enhanced.h"

#include <platform_include/basicplatform/linux/mas/mas_enhanced_gc_core.h>
#include <linux/version.h>

#include <scsi/scsi_host.h>
#include "ufs.h"
#include "ufshcd.h"
#include "ufshcd_debug.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0))
#include "ufs-quirks.h"
#else
#include "ufs_quirks.h"
#endif

#ifdef CONFIG_MAS_UNISTORE_PRESERVE
#include <linux/blkdev.h>
#endif

#ifdef CONFIG_MAS_BLK_HM
#include "basicplatform/linux/mas/mas_blk_hm.h"
#endif

#define QUERY_RES_MASK 0xFFFFFFFF
#define QUERY_REQ_TIMEOUT 1300 /* msec */

static int mas_bkops_upiu_query_opcode(struct ufs_query_req *request,
					struct ufs_hba *hba,
					enum query_opcode opcode,
					u32 *bkops_status)
{ 
	switch (opcode) {
	case UPIU_QUERY_OPCODE_M_GC_START:
		/* fall-through */
	case UPIU_QUERY_OPCODE_M_GC_STOP:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_WRITE_REQUEST;
		break;
	case UPIU_QUERY_OPCODE_M_GC_CHECK:
		request->query_func = UPIU_QUERY_FUNC_STANDARD_READ_REQUEST;
		if (!bkops_status) {
			dev_err(hba->dev,
			    "%s: Invalid argument for read request\n",
			    __func__);
			return -EINVAL;
		}
		break;
	default:
		dev_err(hba->dev,
		    "%s: Expected query bkops opcode but got = %d\n",
		    __func__, opcode);
		return -EINVAL;
	}
 
	return 0;
}

static void ufshcd_enhancegc_start_init_query(struct ufs_hba *hba, enum query_opcode opcode,
	struct ufs_query_req *request, u32 *target_size)
{
	u32 apsize = 0x14;
	if (opcode == UPIU_QUERY_OPCODE_M_GC_START && target_size && *target_size > 0) {
		request->upiu_req.value = cpu_to_be32(apsize);
		request->upiu_req.reserved[0] = cpu_to_be32(*target_size);
	}
	dev_err(hba->dev, "enhancegc_query length:0x%x, value:0x%x, reserved[0]:0x%x,reserved[1]:0x%x\n",
		request->upiu_req.length, request->upiu_req.value,
		request->upiu_req.reserved[0], request->upiu_req.reserved[1]);
}
 
static void ufshcd_enhancegc_response_check(struct ufs_hba *hba,
	enum query_opcode opcode, struct ufs_query_res *response,
	struct ufshcd_query_response *ufshcd_resp)
{
	u32 res_status = 0;
	switch (opcode) {
	case UPIU_QUERY_OPCODE_M_GC_START:
	    if (ufshcd_resp->device_target) {
		    *ufshcd_resp->device_target =
			    be32_to_cpu(response->upiu_res.reserved[0]) & QUERY_RES_MASK;
			dev_err(hba->dev,
				"%s: start querye response, dev_target_size: 0x%xMB\n",
				__func__, *ufshcd_resp->device_target);
		}
		break;
	case UPIU_QUERY_OPCODE_M_GC_STOP:
	    if (ufshcd_resp->now_free_cnt) {
		    *ufshcd_resp->now_free_cnt =
			    be32_to_cpu(response->upiu_res.reserved[0]) & QUERY_RES_MASK;
			dev_err(hba->dev,
				"%s: stop quuery response, dev_now_free_cnt: 0x%xMB\n",
				__func__, *ufshcd_resp->now_free_cnt);
		}
		break;
	case UPIU_QUERY_OPCODE_M_GC_CHECK:
		res_status =
			be32_to_cpu(response->upiu_res.value) & 0xFFFF;
		if (ufshcd_resp->device_target) {
			*ufshcd_resp->device_target =
				be32_to_cpu(response->upiu_res.reserved[0]) & QUERY_RES_MASK;
			dev_err(hba->dev,
				"%s: check status, dev_target_size: 0x%xMB\n",
				__func__, *ufshcd_resp->device_target);
		}
        if (ufshcd_resp->now_free_cnt) {
			*ufshcd_resp->now_free_cnt =
				be32_to_cpu(response->upiu_res.reserved[1]) & QUERY_RES_MASK;
			dev_err(hba->dev,
				"%s: check status, dev_now_target_cnt: 0x%xMB\n",
				__func__, *ufshcd_resp->now_free_cnt);
		}
		break;
	default:
		dev_err(hba->dev,
		    "%s: Expected query bkops opcode but got = %d\n",
		    __func__, opcode);
	}
	if (ufshcd_resp->bkops_status) {
		*ufshcd_resp->bkops_status = res_status;
	}
}

static int ufshcd_query_thor950_bkops(struct ufs_hba *hba,
	enum query_opcode opcode, u32 *target_size, struct ufshcd_query_response *ufshcd_resp)
{
	struct ufs_query_req *request = NULL;
	struct ufs_query_res *dev_response = NULL;
	int err;
	unsigned long flags;
 
	BUG_ON(!hba);
 
	spin_lock_irqsave(hba->host->host_lock, flags);
	if (hba->ufshcd_state != UFSHCD_STATE_OPERATIONAL) {
		spin_unlock_irqrestore(hba->host->host_lock, flags);
		return -EBUSY;
	}
	spin_unlock_irqrestore(hba->host->host_lock, flags);
 
	/* hold error handler, reset may cause send command a noc error */
	mutex_lock(&hba->eh_mutex);
	mutex_lock(&hba->dev_cmd.lock);
 
	ufshcd_init_query(hba, &request, &dev_response, opcode, 0, 0, 0);
    ufshcd_enhancegc_start_init_query(hba, opcode, request, target_size);
	err = mas_bkops_upiu_query_opcode(request, hba, opcode, ufshcd_resp->bkops_status);
	if (err)
		goto out_unlock;
 
	err = ufshcd_exec_dev_cmd(hba, DEV_CMD_TYPE_QUERY, QUERY_REQ_TIMEOUT);
	if (err) {
		dev_err(hba->dev, "%s: Sending bkops query failed, err = %d\n",
		    __func__, err);
		goto out_unlock;
	}
 
    ufshcd_enhancegc_response_check(hba, opcode, dev_response, ufshcd_resp);

out_unlock:
	mutex_unlock(&hba->dev_cmd.lock);
	mutex_unlock(&hba->eh_mutex);
	return err;
}

static int ufshcd_thor950_enhanced_stop(struct ufs_hba *hba)
{
	struct ufshcd_query_response ufshcd_resp = {
	    .bkops_status = NULL,
	    .device_target = NULL,
	    .now_free_cnt = NULL,
    };
	return ufshcd_query_thor950_bkops(
		hba, UPIU_QUERY_OPCODE_M_GC_STOP, NULL, &ufshcd_resp);
}

static int ufshcd_thor950_enhanced_query(struct ufs_hba *hba, u32 *status,
   u32 *device_target, u32 *now_free_cnt)
{
	struct ufshcd_query_response ufshcd_resp = {
	    .bkops_status = status,
	    .device_target = device_target,
	    .now_free_cnt = now_free_cnt,
    };

	if (!status)
		return -EINVAL;

	return ufshcd_query_thor950_bkops(
		hba, UPIU_QUERY_OPCODE_M_GC_CHECK, NULL, &ufshcd_resp);
}

static int ufshcd_thor950_enhanced_start(struct ufs_hba *hba, u32 *target_size)
{
	struct ufshcd_query_response ufshcd_resp = {
	    .bkops_status = NULL,
	    .device_target = NULL,
	    .now_free_cnt = NULL,
    };
	return ufshcd_query_thor950_bkops(
		hba, UPIU_QUERY_OPCODE_M_GC_START, target_size, &ufshcd_resp);
}

static int ufshcd_enhanced_gc_status_query(void *bkops_data, u32 *status, u32 *device_target, u32 *now_free_cnt)
{
	int err;
	struct ufs_hba *hba = NULL;
	if ((!bkops_data) || (!status))
		return -EINVAL;

	hba = (struct ufs_hba *)bkops_data;
	pm_runtime_get_sync(hba->dev);
	err = ufshcd_thor950_enhanced_query(hba, status, device_target, now_free_cnt);
	pm_runtime_put_sync(hba->dev);
	if (err)
		pr_err("UFS bkops status query failed!\n");

	return err;
}

static int ufshcd_enhanced_gc_start(struct ufs_hba *hba, u32 *target_size)
{
	int ret;
	pm_runtime_get_sync(hba->dev);
	ret = ufshcd_thor950_enhanced_start(hba, target_size);
	pm_runtime_put_sync(hba->dev);
	if (ret)
		pr_err("UFS bkops start failed!\n");

	return ret;
}

static int ufshcd_enhanced_gc_stop(struct ufs_hba *hba)
{
	int ret = 0;
	pm_runtime_get_sync(hba->dev);
	ret = ufshcd_thor950_enhanced_stop(hba);
	pm_runtime_put_sync(hba->dev);
	if (ret)
		pr_err("UFS bkops stop failed!\n");
	return ret;
}

static int ufshcd_enhanced_gc_start_stop(void *bkops_data, int start, u32 *target_size)
{
	struct ufs_hba *hba = NULL;
	int ret;

	if (!bkops_data)
		return -EINVAL;

	hba = bkops_data;
	if (start)
		ret = ufshcd_enhanced_gc_start(hba, target_size);
	else
		ret = ufshcd_enhanced_gc_stop(hba);

	return ret;
}

static struct enhanced_ops ufs_enhanced_ops = {
	.bkops_enhanced_gc_start_stop = ufshcd_enhanced_gc_start_stop,
	.bkops_enhanced_gc_status_query = ufshcd_enhanced_gc_status_query,
};


static bool hufs_is_bkops_id_match(struct hufs_bkops_enhanced_id *bkops_id,
	uint16_t man_id, const char *ufs_model, const char *ufs_rev)
{
	if (man_id != bkops_id->manufacturer_id)
		return false;

	if ((strncmp(ufs_model, bkops_id->ufs_model,
		    strlen(bkops_id->ufs_model)) != 0) &&
	    (strcmp(HUFS_MODEL_ANY, bkops_id->ufs_model) != 0))
		return false;

	if ((strcmp(ufs_rev, bkops_id->ufs_rev) != 0) &&
	    (strcmp(HUFS_REV_ANY, bkops_id->ufs_rev) != 0))
		return false;

	return true;
}

static bool hufs_is_bkops_supported(struct scsi_device *sdev)
{
	struct ufs_hba *hba = shost_priv(sdev->host);
	struct list_head *node = NULL;
	struct hufs_bkops_enhanced_id *bkops_id = NULL;

	/* blacklist first */
	list_for_each(node, &hba->bkops_blacklist)
	{
		bkops_id = list_entry(node, struct hufs_bkops_enhanced_id, p);
		if (hufs_is_bkops_id_match(bkops_id, hba->manufacturer_id,
		    sdev->model, sdev->rev))
			return false;
	}

	/* then whitelist */
	list_for_each(node, &hba->bkops_whitelist)
	{
		bkops_id = list_entry(node, struct hufs_bkops_enhanced_id, p);
		if (hufs_is_bkops_id_match(bkops_id, hba->manufacturer_id,
		    sdev->model, sdev->rev))
			return true;
	}

	return false;
}

int hufs_enhanced_manual_bkops_config(struct scsi_device *sdev)
{
	struct ufs_hba *hba = NULL;
	struct mas_bkops *ufs_bkops = NULL;

	if (!sdev)
		return -EINVAL;

	hba = shost_priv(sdev->host);

	if ((sdev->host->hufs_dev_quirk_flag &
		SHOST_HUFS_DEV_QUIRK(SHOST_QUIRK_BKOPS_ENABLE)) == 0)
		return 0;

	if (!hufs_is_bkops_supported(sdev) || hba->ufs_bkops_enabled) {
		pr_err("bkops not supported or has been enabled already!\n");
		return 0;
	}

	ufs_bkops = mas_bkops_alloc();
	if (!ufs_bkops)
		return -ENOMEM;

	hba->ufs_bkops = ufs_bkops;
    ufs_enhanced_ops.bkops_data = hba;
    if (mas_enhanced_gc_enable(&ufs_enhanced_ops))
		goto free_ufs_bkops;

	hba->ufs_bkops_enabled = 1;
	return 0;

free_ufs_bkops:
	kfree(ufs_bkops);
	return -EAGAIN;
}
