/*
 * hufs_bkops_enhanced.h
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

#ifndef HUFS_BKOPS_ENHANCED_H
#define HUFS_BKOPS_ENHANCED_H
#include "scsi/ufs/ufshcd.h"

#define HUFS_MODEL_ANY "HUFS_MODEL_ANY"
#define HUFS_REV_ANY "HUFS_REV_ANY"

struct hufs_bkops_enhanced_id {
	uint16_t manufacturer_id;
	char *ufs_model;
	char *ufs_rev;
	struct list_head p;
};

struct ufshcd_query_response {
    u32 *bkops_status;
    u32 *device_target;
    u32 *now_free_cnt;
};

int hufs_enhanced_manual_bkops_config(struct scsi_device *sdev);
#endif
