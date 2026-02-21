/*
 * vcodec_dev_mgr_formal.h
 *
 * Declare vcodec dev mgr function which is used only by formal version
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
#ifndef VCODEC_DEV_MGR_FORMAL_H
#define VCODEC_DEV_MGR_FORMAL_H
#include "vcodec_types.h"

#define SEC_AUTH_BSP_QUEUE_NAME  "bsp_sec_auth_tc"
#define SEC_AUTH_PXP_QUEUE_NAME  "pxp_sec_auth_tc"
#define IMAGE_ID_BSP_NAME        "bsps_eng"
#define IMAGE_ID_PXP_NAME        "pxpc_eng"
#define SEC_AUTH_NAME            "sec_auth"

#define load_vdec_min(a, b)      ((a) < (b) ? (a) : (b))

#define SEC_AUTH_SEND_MAX_SIZE   0x100000 // 1M

/* Load bsp and pxp image from the file to tee every open LOAD_IMAGE_TO_TEE_PERIOD times,
   and from tee to tcm every open */
#define LOAD_IMAGE_TO_TEE_PERIOD 64

#define LOAD_VDEC_OK             0
#define LOAD_VDEC_ERROR          1

struct work_ctx {
	int result;
	bool is_bsp_image;
	void *image_buffer;
	size_t image_size;
	struct completion completion;
	struct work_struct work;
	struct workqueue_struct *queue;
};

int load_vdec_image(int vdec_dev_type, void *image_buffer, size_t image_size);

#endif