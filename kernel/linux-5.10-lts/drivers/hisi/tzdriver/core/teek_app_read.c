/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "teek_app_load.h"
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/kthread.h>
#include <securec.h>
#include "session_manager.h"
#include "ko_adapt.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

static int32_t teek_open_app_file(struct file *fp, char **file_buf, uint32_t total_img_len)
{
	loff_t pos = 0;
	uint32_t read_size;
	char *file_buffer = NULL;

	if (total_img_len == 0 || total_img_len > MAX_IMAGE_LEN) {
		tloge("img len is invalied %u\n", total_img_len);
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	file_buffer = vmalloc(total_img_len);
	if (!file_buffer) {
		tloge("alloc TA file buffer(size=%u) failed\n", total_img_len);
		return TEEC_ERROR_GENERIC;
	}

	read_size = (uint32_t)kernel_read(fp, file_buffer, total_img_len, &pos);
	if (read_size != total_img_len) {
		tloge("read ta file failed, read size/total size=%u/%u\n", read_size, total_img_len);
		vfree(file_buffer);
		return TEEC_ERROR_GENERIC;
	}

	*file_buf = file_buffer;

	return TEEC_SUCCESS;
}

int32_t teek_read_app(const char *load_file, char **file_buf, uint32_t *file_len)
{
	int32_t ret;
	struct file *fp = NULL;

	/* ta path is NULL means no need to load TA */
	if (!load_file)
		return TEEC_SUCCESS;

	if (!file_buf || !file_len) {
		tloge("load app params invalied\n");
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	fp = filp_open(load_file, O_RDONLY, 0);
	if (!fp || IS_ERR(fp)) {
		tloge("open file error %ld\n", PTR_ERR(fp));
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	if (!fp->f_inode) {
		tloge("node is NULL\n");
		filp_close(fp, 0);
		return TEEC_ERROR_BAD_PARAMETERS;
	}

	*file_len = (uint32_t)(fp->f_inode->i_size);

	ret = teek_open_app_file(fp, file_buf, *file_len);
	if (ret != TEEC_SUCCESS)
		tloge("do read app fail\n");

	filp_close(fp, 0);
	fp = NULL;

	return ret;
}
