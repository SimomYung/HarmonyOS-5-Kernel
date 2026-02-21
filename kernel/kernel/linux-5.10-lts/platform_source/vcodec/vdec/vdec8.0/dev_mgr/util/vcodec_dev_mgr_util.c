/*
 * vcodec_dev_mgr_util.c
 *
 * Implement util dev mgr function, it used by both formal version
 * and FPGA verification version
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
#include <linux/vmalloc.h>
#include <linux/kernel_read_file.h>
#include "vcodec_dev_mgr_util.h"

int32_t vcodec_dev_mgr_read_image(const char* path, void **buffer,
	size_t *size, size_t min_size, size_t max_size)
{
	int32_t ret = 0;
	ret = kernel_read_file_from_path(path, 0, buffer, max_size, size, READING_FIRMWARE);
	if (ret <= 0)
		return -EFAULT;

	if (*size % 4 != 0) {
		vfree(*buffer);
		*buffer = NULL;
		*size = 0;
		return -EFAULT;
	}

	if (*size > max_size || *size <= min_size) {
		vfree(*buffer);
		*buffer = NULL;
		*size = 0;
		return -EFAULT;
	}

	return 0;
}