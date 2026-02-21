/*
 * video_log.c
 *
 * video log operations
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#include "video_log.h"
#include <linux/sort.h>
#include <linux/io.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/rtc.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/stat.h>
#include <linux/types.h>
#include <linux/dirent.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include "securec.h"
#include "mntn_public_interface.h"

#define ROOT_UID 0
#define SYSTEM_GID 1000
#define DIR_NAME_MAX_LEN 1024
#define FILE_NAME_MAX_LEN 256
#define MAX_NUMBER_OF_LOG_FILES 10
#define ABNORMAL_NUMBER_OF_LOG_FILES (-1)
#define VDEC_LOG_FILE_NAME_MAX_LEN 256
#define FILE_NAME_PREFIX "vdec_mcore_log_"
#define FILE_NAME_POSTFIX ".txt"
#ifndef CONFIG_BBOX_LOG_DIR
#define MCU_VDEC_LOG_PATH "/data/hisi_logs/vdec_mcore_log/"
#else
#define MCU_VDEC_LOG_PATH (CONFIG_BBOX_LOG_DIR "vdec_mcore_log/")
#endif

#define MCU_PANIC_LOG_MAX_SIZE 0x20000
#define LOG_ALIGN_DOWN(val, align) (((uint32_t)(val)) & (~((uint32_t)((align)-1))))

struct linux_dirent {
	unsigned long d_ino; /* inode number */
	unsigned long d_off; /* offset to next linux_dirent */
	unsigned short d_reclen; /* length of this linux_dirent */
	char d_name[0]; /* filename max length 255 */
};

struct directory_info {
	int32_t fd; /* dirent descriptor */
	int32_t dirents_size; /* sum of linux_dirents length */
	char dirents[DIR_NAME_MAX_LEN]; /* store linux_dirents information */
	char path[FILE_NAME_MAX_LEN]; /* full path */
};

struct directory {
	struct directory_info dir_info;
	long long (*operator_folder)(struct directory *folder);
};

struct video_file {
	struct directory file;
};

struct video_folder {
	struct directory folder;
};

typedef struct {
	char *g_oldest_file;
	char **str_array;
	int32_t file_count;
	int8_t *dst_log_virtual;
} log_file_entry;

log_file_entry g_log_file_entry;

static log_file_entry *log_file_get_entry(void)
{
	return &g_log_file_entry;
}

static char *get_oldest_file(void)
{
	log_file_entry *log_file = log_file_get_entry();
	return log_file->g_oldest_file;
}

/* create folder object for folder operation */
static struct directory *create_folder(void)
{
	int32_t ret;
	mm_segment_t old_fs;
	struct video_folder *folder = NULL;

	folder = kzalloc(sizeof(struct video_folder), GFP_KERNEL);
	if (!folder) {
		dprint(PRN_ERROR, "malloc folder failed");
		return NULL;
	}

	ret = strcpy_s(folder->folder.dir_info.path, sizeof(folder->folder.dir_info.path), MCU_VDEC_LOG_PATH);
	if (ret != 0) {
		dprint(PRN_ERROR, "copy path name failed");
		kfree(folder);
		return NULL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	folder->folder.dir_info.fd = (int32_t)ksys_open(MCU_VDEC_LOG_PATH, O_RDONLY, 0700);
	if (folder->folder.dir_info.fd < 0) {
		dprint(PRN_ERROR, "open failed");
		set_fs(old_fs);
		kfree(folder);
		return NULL;
	}

	set_fs(old_fs);

	return (struct directory *)folder;
}

/* release the space occupied by folder object */
static void destory_folder(const struct directory *folder)
{
	mm_segment_t old_fs;

	if (!folder)
		return;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	if (folder->dir_info.fd > 0)
		ksys_close((uint32_t)folder->dir_info.fd);

	set_fs(old_fs);
	kfree(folder);
}

static int32_t compare(const void *left_value, const void *right_value)
{
    return strcmp(*(const char **)left_value, *(const char **)right_value);
}
/*
 * loop through to get dirent,
 * operator the dirent with *operator_file or *operator_folder
 */

static int32_t video_remove_file(const char *path)
{
	int32_t ret;
	mm_segment_t old_fs;
	if (!path) {
		dprint(PRN_ERROR, "file name is null");
		return -EINVAL;
	}
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	ret = (int32_t)ksys_unlink(path);
	if (ret != 0)
		dprint(PRN_ERROR, "unlink file fail");
	set_fs(old_fs);
	return ret;
}

static long long operate_folder_impl(const struct directory *folder)
{
	int32_t ret;
	uint32_t pos;
	struct linux_dirent *dirent = NULL;
	log_file_entry *log_file = log_file_get_entry();
	uint32_t file_index = 0;
	log_file->file_count = 0;
	dirent = (struct linux_dirent *)(folder->dir_info.dirents);
	for (pos = 0; pos < (uint32_t)folder->dir_info.dirents_size;
		pos += dirent->d_reclen) {
		char file_name[256];
		dirent = (struct linux_dirent *)(folder->dir_info.dirents + pos);
		/* d_name + 1 because first character of filename getting from system is an invalid char '\x08' or '\x04' */
 		ret = strcpy_s(file_name, sizeof(file_name), dirent->d_name + 1);
		if (ret != 0) {
			dprint(PRN_ERROR, "strcpy_s fail:%d", ret);
		}
		if (strncmp(file_name, FILE_NAME_PREFIX, strlen(FILE_NAME_PREFIX)) == 0 &&
			log_file->file_count >= MAX_NUMBER_OF_LOG_FILES) {
			char file_name_fullpath[VDEC_LOG_FILE_NAME_MAX_LEN] = MCU_VDEC_LOG_PATH;
			log_file->file_count++;
			ret = strcat_s(file_name_fullpath, sizeof(file_name_fullpath), file_name);
			if (ret != 0) {
				dprint(PRN_ERROR, "strcat log fullpath fail");
				return ABNORMAL_NUMBER_OF_LOG_FILES;
			}
			ret = video_remove_file(file_name_fullpath);
			if (ret != 0) {
				dprint(PRN_ERROR, "remove file fail:%d", ret);
				return ABNORMAL_NUMBER_OF_LOG_FILES;
			}
		} else if (strncmp(file_name, FILE_NAME_PREFIX, strlen(FILE_NAME_PREFIX)) == 0 &&
			log_file->file_count < MAX_NUMBER_OF_LOG_FILES) {
			log_file->file_count++;
        	ret = strcpy_s(log_file->str_array[file_index], FILE_NAME_MAX_LEN, file_name);
			if (ret != 0) {
				dprint(PRN_ERROR, "strcpy_s fail:%d", ret);
				return file_index;
			}
			file_index++;
		}
	}
	log_file->file_count = (int32_t)file_index;
	if (log_file->file_count > 0) {
		sort(log_file->str_array, (size_t)log_file->file_count, sizeof(char *), compare, NULL);
		log_file->g_oldest_file = log_file->str_array[0];
	}
	return log_file->file_count;
}

/*
 * get all dirents in current folder,
 * and operate this dirents with function operate_folder_impl
 *
 */
static long long operate_folder(struct directory *folder)
{
	mm_segment_t old_fs;
	log_file_entry *log_file = log_file_get_entry();
	log_file->file_count = 0;
	if (folder->dir_info.dirents_size >= 0) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		folder->dir_info.dirents_size = ksys_getdents64(
			(uint32_t)folder->dir_info.fd,
			(struct linux_dirent64 *)folder->dir_info.dirents,
			sizeof(folder->dir_info.dirents));
		set_fs(old_fs);
		log_file->file_count = (int32_t)operate_folder_impl(folder);
	}

	return log_file->file_count;
}

/* get the number of files under the current folder */
static long long get_file_count(struct directory *folder)
{
	if (!folder) {
		dprint(PRN_ERROR, "create folder object fail");
		return -EINVAL;
	}

	folder->operator_folder = get_file_count;

	return operate_folder(folder);
}

/*
 * get_dirents_size - purpose is to determine path type based on return value
 * path:  file or folder path
 *
 * this function calls sys_getdents to get return
 *
 */
static int32_t get_dirents_size(void)
{
	int32_t fd;
	int32_t dirents_size;
	mm_segment_t old_fs;
	char dirents[DIR_NAME_MAX_LEN] = {0};

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	fd = ksys_open(MCU_VDEC_LOG_PATH, O_RDONLY, 0700);
	if (fd < 0) {
		dprint(PRN_ERROR, "open failed");
		set_fs(old_fs);
		return -EPERM;
	}

	dirents_size = ksys_getdents64((uint32_t)fd, (struct linux_dirent64 *)dirents, sizeof(dirents));
	ksys_close((uint32_t)fd);
	set_fs(old_fs);

	return dirents_size;
}

static int32_t video_create_dir(void)
{
	long ret;
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	/* if dir already exist, ksys_access return 0 */
	ret = ksys_access(MCU_VDEC_LOG_PATH, 0);
	if (ret == 0) {
		set_fs(old_fs);
		return 0;
	}
	ret = ksys_mkdir(MCU_VDEC_LOG_PATH, 0770);
	if (ret < 0) {
        dprint(PRN_ERROR, "create dir failed, fd: %ld", ret);
		set_fs(old_fs);
		return -EPERM;
	}
	ret = ksys_chown(MCU_VDEC_LOG_PATH, ROOT_UID, SYSTEM_GID);
	if (ret) {
		dprint(PRN_ERROR, "chown dir failed, error code: %ld", ret);
		set_fs(old_fs);
		return -EPERM;
	}
	set_fs(old_fs);
	return 0;
}

static int32_t video_get_dir_count(void)
{
	int32_t count;
	int32_t dirents_size;
	struct directory *folder = NULL;
	dirents_size = get_dirents_size();
	if (dirents_size <= 0) {
		dprint(PRN_ERROR, "get dirents failed");
		return -1;
	}
	folder = create_folder();
	count = (int32_t)get_file_count(folder);
	destory_folder(folder);
	return count;
}

static int32_t write_mcu_log_with_timestamp(char *log_file_name, int8_t *buffer, uint32_t size)
{
	mm_segment_t old_fs;
	struct file *fp = NULL;
	int32_t ret = 0;
	int32_t file_count;
	char *oldest_log;
	char oldest_log_fullpath[VDEC_LOG_FILE_NAME_MAX_LEN] = MCU_VDEC_LOG_PATH;
	ret = video_create_dir();
	if (ret != 0) {
		dprint(PRN_ERROR, "video create log dir fail");
		return -EINVAL;
	}
	file_count = video_get_dir_count();
	if (file_count < 0) {
		dprint(PRN_ERROR, "file count error:%d", file_count);
		return -EINVAL;
	}
	if (file_count >= MAX_NUMBER_OF_LOG_FILES) {
		oldest_log = get_oldest_file();
		ret = strcat_s(oldest_log_fullpath, sizeof(oldest_log_fullpath), oldest_log);
		if (ret != 0) {
			dprint(PRN_ERROR, "strcat oldest log fullpath fail");
			return -EINVAL;
		}
		ret = video_remove_file(oldest_log_fullpath);
		if (ret != 0) {
			dprint(PRN_ERROR, "video remove oldest log fail");
			return -EINVAL;
		}
	}
 	old_fs = get_fs();
	set_fs(KERNEL_DS);
	fp = filp_open(log_file_name, O_RDWR | O_CREAT, 0660);
	if (IS_ERR(fp)) {
		dprint(PRN_ERROR, "create file error, error code:%ld", PTR_ERR(fp));
		set_fs(old_fs);
		return -EIO;
	}

	vfs_write(fp, buffer, size, &fp->f_pos);
	filp_close(fp, NULL);

	ret = (int32_t)ksys_chown(log_file_name, ROOT_UID, SYSTEM_GID);
	if (ret) {
		dprint(PRN_ERROR, "chown file failed, error code: %d", ret);
		set_fs(old_fs);
		return -EPERM;
	}
	set_fs(old_fs);
	return 0;
}

int32_t vdec_log_init_entry(void)
{
	uint32_t index;
	uint32_t subscript;
	log_file_entry *log_file = log_file_get_entry();
	log_file->dst_log_virtual = (int8_t *)ioremap(RDR_VDEC_MEM_MAP_ADDR, MCU_PANIC_LOG_MAX_SIZE);
	if (!log_file->dst_log_virtual) {
		dprint(PRN_ERROR, "log area map virtual error\n");
		return -ENOMEM;
	}

	log_file->str_array = kzalloc(MAX_NUMBER_OF_LOG_FILES * sizeof(char *), GFP_KERNEL);
	if (!log_file->str_array) {
		dprint(PRN_ERROR, "kzalloc str_array failed");
		goto unmap_log_virtual;
	}
	for (index = 0; index < MAX_NUMBER_OF_LOG_FILES; index++) {
		log_file->str_array[index] = kzalloc(FILE_NAME_MAX_LEN, GFP_KERNEL);
		if (!log_file->str_array[index]) {
			dprint(PRN_ERROR, "kzalloc str_array index failed");
			goto free_array;
		}
	}
	return 0;

free_array:
	for (subscript = 0; subscript < index; subscript++) {
		kfree(log_file->str_array[subscript]);
		log_file->str_array[subscript] = NULL;
	}
	kfree(log_file->str_array);
	log_file->str_array = NULL;

unmap_log_virtual:
	iounmap(log_file->dst_log_virtual);
	log_file->dst_log_virtual = NULL;
	return -ENOMEM;
}

void vdec_log_deinit_entry(void)
{
	uint32_t index;
	log_file_entry *log_file = log_file_get_entry();
	if (!log_file->str_array)
		return;

	for (index = 0; index < MAX_NUMBER_OF_LOG_FILES; index++) {
    	if (log_file->str_array[index]) {
        	kfree(log_file->str_array[index]);
            log_file->str_array[index] = NULL;
        }
    }
    kfree(log_file->str_array);
    log_file->str_array = NULL;
	if (log_file->dst_log_virtual) {
		iounmap(log_file->dst_log_virtual);
		log_file->dst_log_virtual = NULL;
	}
}

static int32_t vdec_write_panic_log(int8_t *buffer, uint32_t size)
{
	int32_t ret = 0;
	log_file_entry *log_file = log_file_get_entry();
	uint32_t log_size = (size > MCU_PANIC_LOG_MAX_SIZE) ? MCU_PANIC_LOG_MAX_SIZE : size;
	int8_t *src_log_addr = (size > MCU_PANIC_LOG_MAX_SIZE) ?
		(buffer + LOG_ALIGN_DOWN((size - log_size), 64)) : buffer;

	if (!log_file->dst_log_virtual) {
		dprint(PRN_ERROR, "log virtual area is NULL\n");
		return -ENOMEM;
	}

	if (memcpy_s(log_file->dst_log_virtual, log_size, src_log_addr, log_size) != EOK) {
		dprint(PRN_ERROR, "copy mcu log error\n");
		ret = -ENOMEM;
	}

	return ret;
}

int32_t vdec_write_log(int8_t *buffer, uint32_t size, bool is_panic)
{
	int32_t ret;
	struct rtc_time tm = {0};
	struct timespec64 tv = {0};
	char log_file_name[VDEC_LOG_FILE_NAME_MAX_LEN] = {0};

	if (is_panic)
		return vdec_write_panic_log(buffer, size);

	ktime_get_real_ts64(&tv);
	tv.tv_sec -= (long)sys_tz.tz_minuteswest * 60;
	rtc_time64_to_tm(tv.tv_sec, &tm);
	ret = snprintf_s(log_file_name, sizeof(log_file_name),
					sizeof(log_file_name) - 1, "%s%s%04d%02d%02d%02d%02d%02d%s",
					MCU_VDEC_LOG_PATH, FILE_NAME_PREFIX, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec, FILE_NAME_POSTFIX);
	if (ret == -1) {
		dprint(PRN_ERROR, "rename file name fail\n");
		return -EINVAL;
	}
	dprint(PRN_ALWS, "mcu log time stamp: %04d%02d%02d%02d%02d%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);
	return write_mcu_log_with_timestamp(log_file_name, buffer, size);
}