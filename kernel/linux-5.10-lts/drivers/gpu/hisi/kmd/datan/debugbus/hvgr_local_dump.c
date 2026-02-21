/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_local_dump.h"

#include <securec.h>

#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_regmap_debug_bus.h"
#include "hvgr_debug_bus_dump.h"
#include "hvgr_file.h"

static bool hvgr_local_debugbus_get_duplicated_file_name(char *file_name, char *time)
{
	struct kstat fstat;
	int ret;

	ret = snprintf_s(file_name, MAX_FILE_NAME_SIZE, MAX_FILE_NAME_SIZE - 1,
		HVGR_DEBUG_BUS_DATA_FILE_DUPLICATED,
		HVGR_RELEASE_NAME, time);
	if (ret < 0)
		return false;

	if (vfs_stat(file_name, &fstat) < 0)
		return true;

	/* If file exits, delete it first */
	if (hvgr_file_delete_file(file_name) < 0)
		return false;

	return true;
}

static void hvgr_local_debugbus_get_first_file_name(char *file_name, char *time)
{
	struct rtc_time tm;
	int ret;
	struct hvgr_device *gdev;

	gdev = hvgr_get_device();
	rtc_time64_to_tm((time64_t)get_seconds(), &tm);
	ret = snprintf_s(time, MAX_TIME_SIZE, MAX_TIME_SIZE + 1, HVGR_DEBUG_BUS_TIME,
		tm.tm_year + BASE_YEAR, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
		tm.tm_min, tm.tm_sec);
	if (ret < 0)
		hvgr_err(gdev, HVGR_DATAN, "%s %s", __func__, time);

	ret = snprintf_s(file_name, MAX_FILE_NAME_SIZE, MAX_FILE_NAME_SIZE - 1,
		HVGR_DEBUG_BUS_DATA_FILE, HVGR_RELEASE_NAME, time);
	if (ret < 0)
		hvgr_err(gdev, HVGR_DATAN, "%s %s", __func__, file_name);
}

static bool hvgr_local_debugbus_get_file_name(char *file_name, int dump_mode, char *time)
{
	struct kstat fstat;

	if (dump_mode == HVGR_DEBUG_BUS_DUMP_SEC)
		return hvgr_local_debugbus_get_duplicated_file_name(file_name, time);

	if (dump_mode != HVGR_DEBUG_BUS_DUMP_FIR)
		return false;

	hvgr_local_debugbus_get_first_file_name(file_name, time);

	if (vfs_stat(file_name, &fstat) < 0)
		return true;

	/* If file exits, delete it first */
	if (hvgr_file_delete_file(file_name) < 0)
		return false;

	return true;
}

u32 hvgr_debugbus_get_buffer_size(struct hvgr_device * const gdev)
{
	u32 buffer_size;
	u32 core_number;

	if (gdev == NULL)
		return 0u;

	core_number = hvgr_get_core_nums(gdev);
	if (core_number == 0u) {
		hvgr_err(gdev, HVGR_DATAN, "debugbus use default gpc nums");
		core_number = (u32)HVGR_DEBUG_BUS_MAX_GPC_CORE_ID + 1u;
	}
	buffer_size = HVGR_DEBUG_BUS_BUFFER_SIZE_PRE_GPC * core_number +
		HVGR_DEBUG_BUS_BUFFER_SIZE_MISC;
	return buffer_size;
}

void hvgr_local_debugbus_write_to_file(char *buffer, char *file_name, size_t buf_len)
{
	struct file *data_file = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	mm_segment_t old_fs;
#endif
	loff_t pos = 0;

	if (buf_len == 0 || file_name == NULL)
		return;

	data_file = filp_open(file_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUGO | S_IWUSR);
	if (IS_ERR_OR_NULL(data_file))
		return;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	(void)vfs_write(data_file, buffer, buf_len, &pos);
	set_fs(old_fs);
#else
	(void)kernel_write(data_file, buffer, buf_len, &pos);
#endif

	(void)vfs_fsync(data_file, 0);

	(void)filp_close(data_file, NULL);
}

static void hvgr_local_debugbus_write_buffer_to_file(char *buffer, int dump_mode, char *time)
{
	char file_name[MAX_FILE_NAME_SIZE] = {0};

	if (strlen(buffer) == 0)
		return;

	if (!hvgr_local_debugbus_get_file_name(file_name, dump_mode, time))
		return;

	hvgr_local_debugbus_write_to_file(buffer, file_name, strlen(buffer));
}

bool hvgr_local_debugbus_check_disk(void)
{
	int num;


	if ((hvgr_file_get_free_space(HVGR_LOG_ROOT) >> HVGR_FILE_BYTE2MB) < MIN_FREE_SPACE)
		return false;

	num = hvgr_file_get_file_num(HVGR_DEBUG_BUS_DATA_DIR, HVGR_DEBUG_BUS_FILE_PREFIX);
	if (num == -ENOENT)
		return true;
	else if (num < 0)
		return false;

	return num < HVGR_DEBUG_BUS_MAX_NUMBER;
}

void hvgr_local_debugbus_dump(struct hvgr_device *gdev)
{
	int ret = 0;
	u32 buffer_size = 0;
	char time[MAX_TIME_SIZE] = {0};
	struct hvgr_datan_dev *datan_dev = NULL;
	struct hvgr_debugbus_dev *bus_dev = NULL;

	if (gdev == NULL)
		return;

	hvgr_info(gdev, HVGR_DATAN, "%s IN", __func__);

	datan_dev = &gdev->datan_dev;
	if (datan_dev == NULL)
		return;

	bus_dev = &datan_dev->datan_bus_dev;
	if (bus_dev == NULL)
		return;

	mutex_lock(&datan_dev->dump_dir_lock);
	if (!hvgr_file_make_dir(HVGR_DEBUG_BUS_DATA_DIR, HVGR_DEBUG_BUS_DATA_DIR_MODE)) {
		hvgr_err(gdev, HVGR_DATAN, "%s mkdir fail", __func__);
		ret = -1;
	}
	mutex_unlock(&datan_dev->dump_dir_lock);

	if (!hvgr_local_debugbus_check_disk())
		ret = -1;

	mutex_lock(&bus_dev->debug_bus_mutex);
	if (bus_dev->debugbus_buf != NULL) {
		if (ret == 0) {
			hvgr_local_debugbus_write_buffer_to_file(bus_dev->debugbus_buf,
				HVGR_DEBUG_BUS_DUMP_FIR, time);

			/*
			 * Dump twice for file comparison
			 * Same files indicate that gpu job hangs
			 * Different files indicate that gpu job timeouts
			 */
			buffer_size = hvgr_debugbus_get_buffer_size(gdev);
			hvgr_local_debugbus_write_buffer_to_file(&bus_dev->debugbus_buf[buffer_size],
				HVGR_DEBUG_BUS_DUMP_SEC, time);
		}
		vfree(bus_dev->debugbus_buf);
		bus_dev->debugbus_buf = NULL;
	}
	bus_dev->debugbus_cnt--;
	mutex_unlock(&bus_dev->debug_bus_mutex);
}
