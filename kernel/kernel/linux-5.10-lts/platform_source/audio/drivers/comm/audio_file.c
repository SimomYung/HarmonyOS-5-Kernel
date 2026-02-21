/*
 * audio_file.c
 *
 * audio file operation common
 *
 * Copyright (c) 2015 Huawei Technologies Co., Ltd.
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

#include "audio_file.h"

#include <linux/io.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/rtc.h>
#include <linux/err.h>
#include <linux/time.h>
#include <linux/stat.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#ifdef CONFIG_VENDOR_FS
#include <platform_include/basicplatform/linux/fs/vendor_fs_interface.h>
#endif

#include "audio_log.h"
#include "securec.h"

#define LOG_TAG "audio_file"
#define DIR_NAME_MAX_LEN 1024
#define FILE_NAME_MAX_LEN 200
#define DUMP_DIR_LEN 128
#define ROOT_UID 0
#define SYSTEM_GID 1000
#define MAX_PCM_SIZE 0x80000000 /* 2G */

/*
 * Before adding a whitelist, make sure that
 * the path length added is not more than FILE_NAME_MAX_LEN
 * or modify FILE_NAME_MAX_LEN
 */
const static char * const white_list_path[] = {
	"/data/log/wakeup_pcm/",
	"/data/log/codec_dsp/default/",
	"/data/log/codec_dsp/beta_club/",
	"/data/log/codec_dsp/",
	"/data/log/codechifi_total_logs/",
	"/data/log/codec_dsp/visual_tool/",
	SOCDSP_TOTAL_LOG_PATH,
};

struct linux_dirent {
	unsigned long d_ino; /* inode number */
	unsigned long d_off; /* offset to next linux_dirent */
	unsigned short d_reclen; /* length of this linux_dirent */
	char d_name[0]; /* filename max length 255 */
};

struct directory_info {
#ifdef CONFIG_VENDOR_FS
	struct file *fp; /* dirent descriptor */
#else
	int fd; /* dirent descriptor */
#endif
	int dirents_size; /* sum of linux_dirents length */
	char dirents[DIR_NAME_MAX_LEN]; /* store linux_dirents information */
	char path[FILE_NAME_MAX_LEN]; /* full path */
};

struct directory {
	struct directory_info dir_info;

	/*
	 * operator_file - operating interface to file
	 * file: file object, store file operator and file information
	 *
	 * operator_file is a universal operating interface to file,
	 * it can be get_file_size, get_file_count, remove_file_dir,
	 * and you can add new operation to file whatever you like
	 *
	 */
	long long (*operator_file)(const struct directory *file);

	/*
	 * operator_folder - operating interface to folder
	 * folder: folder object,
	 *           store folder operator and folder information
	 *
	 * operator_folder is a universal operating interface to folder,
	 * it can be get_folder_size, get_folder_count, remove_folder_dir,
	 * and you can add new operation to folder whatever you like
	 */
	long long (*operator_folder)(struct directory *folder);
};

struct audio_file {
	struct directory file;
};

struct audio_folder {
	struct directory folder;
};

/* create file object for file operation */
static struct directory *create_file(const char *cur_path, unsigned int size)
{
	int ret;
	mm_segment_t old_fs;
	struct audio_file *file = NULL;

	if (cur_path == NULL) {
		AUDIO_LOGE("path is null, create file failed");
		return NULL;
	}

	if (strlen(cur_path) >= FILE_NAME_MAX_LEN || strlen(cur_path) > size) {
		AUDIO_LOGE("cur path len[%zu] err, size is %u",
			strlen(cur_path), FILE_NAME_MAX_LEN);
		return NULL;
	}

	file = kzalloc(sizeof(struct audio_file), GFP_KERNEL);
	if (file == NULL) {
		AUDIO_LOGE("malloc file failed");
		return NULL;
	}

	ret = strcpy_s(file->file.dir_info.path, sizeof(file->file.dir_info.path), cur_path);
	if (ret != 0) {
		AUDIO_LOGE("copy path name failed");
		kfree(file);
		return NULL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	file->file.dir_info.fp = filp_open(cur_path, O_RDONLY, 0770);
	if (IS_ERR_OR_NULL(file->file.dir_info.fp)) {
#else
	file->file.dir_info.fd = ksys_open(cur_path, O_RDONLY, 0770);
	if (file->file.dir_info.fd < 0) {
#endif
		AUDIO_LOGE("open failed");
		set_fs(old_fs);
		kfree(file);
		return NULL;
	}

	set_fs(old_fs);

	return (struct directory *)file;
}

/* release the space occupied by file object */
static void destory_file(const struct directory *file)
{
	mm_segment_t old_fs;

	if (file == NULL)
		return;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	if (!IS_ERR_OR_NULL(file->dir_info.fp))
		filp_close(file->dir_info.fp, NULL);
#else
	if (file->dir_info.fd > 0)
		ksys_close(file->dir_info.fd);
#endif

	set_fs(old_fs);
	kfree(file);
}

/* create folder object for folder operation */
static struct directory *create_folder(const char *path, unsigned int size)
{
	int ret;
	mm_segment_t old_fs;
	struct audio_folder *folder = NULL;

	if (path == NULL) {
		AUDIO_LOGE("path is null, create file failed");
		return NULL;
	}

	if (strlen(path) >= FILE_NAME_MAX_LEN || strlen(path) > size) {
		AUDIO_LOGE("path len: %zu err, size is %u",
			strlen(path), FILE_NAME_MAX_LEN);
		return NULL;
	}

	folder = kzalloc(sizeof(struct audio_folder), GFP_KERNEL);
	if (folder == NULL) {
		AUDIO_LOGE("malloc folder failed");
		return NULL;
	}

	ret = strcpy_s(folder->folder.dir_info.path, sizeof(folder->folder.dir_info.path), path);
	if (ret != 0) {
		AUDIO_LOGE("copy path name failed");
		kfree(folder);
		return NULL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	folder->folder.dir_info.fp = filp_open(path, O_RDONLY, 0770);
	if (IS_ERR_OR_NULL(folder->folder.dir_info.fp)) {
#else
	folder->folder.dir_info.fd = ksys_open(path, O_RDONLY, 0770);
	if (folder->folder.dir_info.fd < 0) {
#endif
		AUDIO_LOGE("open failed");
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

	if (folder == NULL)
		return;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	if (!IS_ERR_OR_NULL(folder->dir_info.fp))
		filp_close(folder->dir_info.fp, NULL);
#else
	if (folder->dir_info.fd > 0)
		ksys_close(folder->dir_info.fd);
#endif

	set_fs(old_fs);
	kfree(folder);
}

/* create file or folder objects based on type */
static struct directory *create_sub_dir_obj(char type, const char *path,
	unsigned int size)
{
	struct directory *direct = NULL;

	if (type == DT_DIR)
		direct = create_folder(path, size);
	else if (type == DT_REG)
		direct = create_file(path, size);
	else
		AUDIO_LOGE("err dir type: %d", type);

	return direct;
}

/* destory file or folder objects based on type */
static void destory_sub_dir_obj(char type, const struct directory *direct)
{
	if (type == DT_DIR)
		destory_folder(direct);
	else if (type == DT_REG)
		destory_file(direct);
	else
		AUDIO_LOGE("err dir type: %d", type);
}

/*
 * loop through to get dirent,
 * operator the dirent with *operator_file or *operator_folder
 *
 */
static long long operate_sub_folder_impl(const struct directory *folder)
{
	int ret;
	char d_type;
	char *sub_path = NULL;
	unsigned int pos;
	long long comm_value = 0;
	struct linux_dirent *dirent = NULL;
	struct directory *sub_folder = NULL;

	sub_path = kzalloc((unsigned long)FILE_NAME_MAX_LEN, GFP_KERNEL);
	if (sub_path == NULL) {
		AUDIO_LOGE("sub path alloc fail");
		return 0;
	}

	dirent = (struct linux_dirent *)(folder->dir_info.dirents);
	/* loop through to get dirent */
	for (pos = 0; pos < (unsigned int)folder->dir_info.dirents_size;
		pos += dirent->d_reclen) {
		dirent = (struct linux_dirent *)(folder->dir_info.dirents + pos);

		/* file type, offset is (d_reclen - 1) */
		d_type = *(folder->dir_info.dirents + pos +
			dirent->d_reclen - 1);

		ret = snprintf_s(sub_path, FILE_NAME_MAX_LEN, FILE_NAME_MAX_LEN - 1, "%s/%s",
			folder->dir_info.path,
			dirent->d_name);
		if (ret == -1) {
			AUDIO_LOGE("copy sub_path failed");
			break;
		}

		if (d_type == DT_DIR &&
			(strncmp(dirent->d_name, ".", sizeof(".")) == 0 ||
			strncmp(dirent->d_name, "..", sizeof("..")) == 0 ||
			strncmp(dirent->d_name, "...", sizeof("...")) == 0))
			continue;

		sub_folder = create_sub_dir_obj(d_type, sub_path, FILE_NAME_MAX_LEN - 1);
		if (sub_folder == NULL)
			continue;

		if (d_type == DT_DIR)
			comm_value += folder->operator_folder(sub_folder);
		else if (d_type == DT_REG)
			comm_value += folder->operator_file(sub_folder);
		else
			AUDIO_LOGE("err dir type: %d", d_type);

		destory_sub_dir_obj(d_type, sub_folder);
	}
	kfree(sub_path);

	return comm_value;
}

/*
 * get all dirents in current folder,
 * and operate this dirents with function operate_sub_folder_impl
 *
 */
static long long operate_sub_folder(struct directory *folder)
{
	long long comm_value = 0;
	mm_segment_t old_fs;

	while (folder->dir_info.dirents_size >= 0) {
		old_fs = get_fs();
		set_fs(KERNEL_DS);

		/* get a batch of dirents */
#ifdef CONFIG_VENDOR_FS
		folder->dir_info.dirents_size = (int)vendor_getdents64(
			folder->dir_info.fp,
			(struct linux_dirent64 *)folder->dir_info.dirents,
			sizeof(folder->dir_info.dirents));
#else
		folder->dir_info.dirents_size = ksys_getdents64(
			folder->dir_info.fd,
			(struct linux_dirent64 *)folder->dir_info.dirents,
			sizeof(folder->dir_info.dirents));
#endif

		set_fs(old_fs);
		if (folder->dir_info.dirents_size <= 0)
			break;

		comm_value += operate_sub_folder_impl(folder);
	}

	return comm_value;
}

static long long get_file_size(const struct directory *file)
{
	struct kstat stat;
	mm_segment_t old_fs;

	if (file == NULL) {
		AUDIO_LOGW("create file object fail");
		return -EINVAL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	(void)memset_s(&stat, sizeof(struct kstat), 0, sizeof(struct kstat)); /* unsafe_function_ignore: memset */
	if (vfs_stat(file->dir_info.path, &stat) != 0) {
		AUDIO_LOGE("stat failed");
		set_fs(old_fs);
		return 0;
	}

	set_fs(old_fs);

	return stat.size;
}

/* get all file and folder size under the current folder and subfolder */
static long long get_folder_size(struct directory *folder)
{
	struct kstat stat;
	unsigned long long size;
	mm_segment_t old_fs;

	if (folder == NULL) {
		AUDIO_LOGW("create file object fail");
		return -EINVAL;
	}

	folder->operator_file = get_file_size;
	folder->operator_folder = get_folder_size;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	(void)memset_s(&stat, sizeof(struct kstat), 0, sizeof(struct kstat)); /* unsafe_function_ignore: memset */
	if (vfs_stat(folder->dir_info.path, &stat) != 0)
		AUDIO_LOGE("stat failed");

	set_fs(old_fs);

	size = stat.size;
	size += operate_sub_folder(folder);

	return size;
}

/* if file exist, file count increase 1 */
static long long get_file_count(const struct directory *file)
{
	if (file == NULL) {
		AUDIO_LOGE("create file object fail");
		return -EINVAL;
	}

	return 1;
}

/* get the number of files under the current folder and subfolder */
static long long get_folder_count(struct directory *folder)
{
	if (folder == NULL) {
		AUDIO_LOGE("create file object fail");
		return -EINVAL;
	}

	folder->operator_file = get_file_count;
	folder->operator_folder = get_folder_count;

	return operate_sub_folder(folder);
}

static int rm_dir(char *path)
{
	int ret;
	char *pdst = path;
	mm_segment_t old_fs;

	old_fs = get_fs();
	set_fs(KERNEL_DS);

	while (*pdst)
		pdst++;

	pdst--;
	if (*pdst == '/')
		*pdst = '\0';

#ifdef CONFIG_VENDOR_FS
	ret = (int)vendor_rmdir(path);
#else
	ret = ksys_rmdir(path);
#endif
	if (ret != 0)
		AUDIO_LOGE("del failed ret: %d", ret);

	*pdst = '/';

	set_fs(old_fs);

	return ret;
}

/*
 * dlelete hard links to files, when the reference number is 0,
 * the underlying files can be deleted
 *
 */
static long long remove_file_dir(const struct directory *file)
{
	long long ret;
	mm_segment_t old_fs;

	if (file == NULL) {
		AUDIO_LOGE("create file object fail");
		return -EINVAL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	ret = vendor_unlink(file->dir_info.path);
#else
	ret = ksys_unlink(file->dir_info.path);
#endif
	if (ret != 0)
		AUDIO_LOGE("unlink dir fail");

	set_fs(old_fs);

	return ret;
}

/*
 * delete hard links to files in current folders and subfolders
 */
static long long remove_folder_dir(struct directory *folder)
{
	long long ret;

	if (folder == NULL) {
		AUDIO_LOGE("create file object fail");
		return -EINVAL;
	}

	folder->operator_file = remove_file_dir;
	folder->operator_folder = remove_folder_dir;

	ret = operate_sub_folder(folder);
	rm_dir(folder->dir_info.path);

	return ret;
}

static int check_valid_path(const char *path, unsigned int size)
{
	int ret;
	unsigned int i;
	char dir_name[DUMP_DIR_LEN] = {0};

	/* null pointer check */
	if (path == NULL) {
		AUDIO_LOGE("path is null");
		return -EINVAL;
	}

	/* path length check */
	if (strlen(path) >= FILE_NAME_MAX_LEN || strlen(path) > size) {
		AUDIO_LOGE("path is too long, len is: %zu", strlen(path));
		return -EINVAL;
	}

	/* absolute path check */
	for (i = 0; i < strlen(path); i++) {
		/* sizeof("xx")-1 is remove the effect of '\0' */
		if (path[i] == '.' &&
			(strncmp(path + i, "..", strlen("..")) == 0 ||
			strncmp(path + i, "...", strlen("...")) == 0)) {
			AUDIO_LOGE("path contain err, invalid path");
			return -EINVAL;
		}
	}

	/* white list check */
	for (i = 0; i < ARRAY_SIZE(white_list_path); i++) {
		if (strncmp(path, white_list_path[i],
			strlen(white_list_path[i])) == 0) {
			AUDIO_LOGI("white list check, success");
			return 0;
		}
	}
	ret = snprintf_s(dir_name, sizeof(dir_name), sizeof(dir_name) - 1, "%s%s", PATH_ROOT, "codecdump/");
	if (ret == -1) {
		AUDIO_LOGI("copy dir name failed");
		return -EINVAL;
	}
	if (strncmp(path, dir_name, strlen(dir_name)) == 0) {
		AUDIO_LOGI("white list check, success");
		return 0;
	}

	return -EINVAL;
}

/*
 * get_dirents_size - purpose is to determine path type based on return value
 * path:  file or folder path
 *
 * this function calls sys_getdents to get return
 *
 */
static int get_dirents_size(const char *path)
{
#ifdef CONFIG_VENDOR_FS
	struct file *pfile = NULL;
#else
	int fd;
#endif
	int dirents_size;
	mm_segment_t old_fs;
	char dirents[DIR_NAME_MAX_LEN] = {0};

	if (path == NULL) {
		AUDIO_LOGE("path is null");
		return -EINVAL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	pfile = filp_open(path, O_RDONLY, 0770);
	if (IS_ERR_OR_NULL(pfile)) {
		AUDIO_LOGW("open failed");
		set_fs(old_fs);
		return -EINVAL;
	}

	dirents_size = (int)vendor_getdents64(pfile, (struct linux_dirent64 *)dirents,
			sizeof(dirents));
	filp_close(pfile, NULL);
#else
	fd = ksys_open(path, O_RDONLY, 0770);
	if (fd < 0) {
		AUDIO_LOGW("open failed");
		set_fs(old_fs);
		return -EINVAL;
	}

	dirents_size = ksys_getdents64(fd, (struct linux_dirent64 *)dirents,
			sizeof(dirents));
	ksys_close(fd);
#endif

	set_fs(old_fs);

	return dirents_size;
}

static int create_dir_with_chown(const char *path)
{
	int ret;
	mm_segment_t old_fs;
#ifdef CONFIG_VENDOR_FS
	struct file *pfile = NULL;
#endif

	if (path == NULL) {
		AUDIO_LOGE("path is null");
		return -EINVAL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	ret = (int)vendor_access(path, 0);
	if (ret) {
		ret = (int)vendor_mkdir(path, 0770);
		if (ret < 0) {
			AUDIO_LOGE("create dir: %s failed", path);
			set_fs(old_fs);
			return -EPERM;
		}
		AUDIO_LOGI("create dir: %s successed", path);
	}

	pfile = filp_open(path, O_RDONLY, 0770);
	if (IS_ERR_OR_NULL(pfile)) {
		AUDIO_LOGE("open dir %s failed", path);
		vendor_rmdir(path);
		set_fs(old_fs);
		return -EINVAL;
	}

	if (vfs_fchown(pfile, ROOT_UID, SYSTEM_GID)) {
		AUDIO_LOGE("chown failed");
		filp_close(pfile, NULL);
		vendor_rmdir(path);
		set_fs(old_fs);
		return -EPERM;
	}

	filp_close(pfile, NULL);
#else
	ret = ksys_access(path, 0);
	if (ret) {
		ret = ksys_mkdir(path, 0770);
		if (ret < 0) {
			AUDIO_LOGE("create dir failed, fd: %d", ret);
			set_fs(old_fs);
			return -EPERM;
		}
		AUDIO_LOGI("create dir successed fd: %d", ret);
	}

	if ((int)ksys_chown((const char __user *)path, ROOT_UID, SYSTEM_GID)) {
		AUDIO_LOGE("chown failed");
		ksys_rmdir(path);
		set_fs(old_fs);
		return -EPERM;
	}
#endif

	set_fs(old_fs);

	return 0;
}

int audio_create_dir(const char *path, unsigned int size)
{
	char cur_path[FILE_NAME_MAX_LEN] = {0};
	int index = 0;

	if (check_valid_path(path, size) != 0) {
		AUDIO_LOGE("invalid err");
		return -EINVAL;
	}

	if (*path != '/')
		return -EINVAL;

	cur_path[index++] = *path++;

	while (*path != '\0') {
		if (index >= FILE_NAME_MAX_LEN) {
			AUDIO_LOGE("path is too long");
			return -EINVAL;
		}

		if (*path == '/')
			create_dir_with_chown(cur_path);

		cur_path[index++] = *path++;
	}

	return 0;
}

int audio_rename_dir(const char *old_name, unsigned int old_size,
	const char *new_name, unsigned int new_size)
{
	mm_segment_t old_fs;
	int ret;
#ifdef CONFIG_VENDOR_FS
	struct file *fp = NULL;
#endif

	if (check_valid_path(old_name, old_size) != 0 ||
		check_valid_path(new_name, new_size) != 0) {
		AUDIO_LOGE("invalid err");
		return -EINVAL;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);

#ifdef CONFIG_VENDOR_FS
	ret = (int)vendor_rename(old_name, new_name);
#else
	ret = ksys_rename(old_name, new_name);
#endif
	if (ret != 0) {
		AUDIO_LOGE("rename err");
		set_fs(old_fs);
		return ret;
	}

#ifdef CONFIG_VENDOR_FS
	fp = filp_open(new_name, O_RDONLY, 0770);
	if (IS_ERR_OR_NULL(fp)) {
		AUDIO_LOGE("chown dir fail");
		set_fs(old_fs);
		return -EINVAL;
	}

	ret = vfs_fchown(fp, ROOT_UID, SYSTEM_GID);
	if (ret != 0) {
		AUDIO_LOGE("chown err");
		filp_close(fp, NULL);
		set_fs(old_fs);
		return ret;
	}
	filp_close(fp, NULL);
#else
	ret = ksys_chown(new_name, ROOT_UID, SYSTEM_GID);
	if (ret != 0) {
		AUDIO_LOGE("chown dir fail");
		set_fs(old_fs);
		return ret;
	}
#endif

	set_fs(old_fs);

	return 0;
}

int audio_remove_dir(const char *path, unsigned int size)
{
	int ret = 0;
	int dirents_size;
	struct directory *file = NULL;
	struct directory *folder = NULL;

	if (check_valid_path(path, size) != 0) {
		AUDIO_LOGE("invalid err");
		return -EINVAL;
	}

	dirents_size = get_dirents_size(path);
	/* judge path type based on dirents_size, file or folder */
	if (dirents_size == 0) {
		file = create_file(path, size);
		ret = (int)remove_file_dir(file);
		destory_file(file);
	} else if (dirents_size > 0) {
		folder = create_folder(path, size);
		ret = (int)remove_folder_dir(folder);
		destory_folder(folder);
	} else {
		AUDIO_LOGE("get dirents failed");
	}

	return ret;
}

int audio_remove_file(const char *path, unsigned int size)
{
	long long ret;
	mm_segment_t old_fs;
	struct directory *file = NULL;

	if (path == NULL) {
		AUDIO_LOGE("file name is null");
		return -EINVAL;
	}

	AUDIO_LOGI("remove file");

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	file = create_file(path, size);

#ifdef CONFIG_VENDOR_FS
	ret = vendor_unlink(path);
#else
	ret = ksys_unlink(path);
#endif
	if (ret != 0)
		AUDIO_LOGE("unlink file fail");

	set_fs(old_fs);
	destory_file(file);

	return ret;
}

long long audio_get_dir_size(const char *path, unsigned int size)
{
	int dirents_size;
	long long dir_size = 0;
	struct directory *file = NULL;
	struct directory *folder = NULL;

	if (check_valid_path(path, size) != 0) {
		AUDIO_LOGE("invalid err");
		return -1;
	}

	dirents_size = get_dirents_size(path);
	/* judge path type based on dirents_size, file or folder */
	if (dirents_size == 0) {
		file = create_file(path, size);
		dir_size = get_file_size(file);
		destory_file(file);
	} else if (dirents_size > 0) {
		folder = create_folder(path, size);
		dir_size = get_folder_size(folder);
		destory_folder(folder);
	} else {
		AUDIO_LOGW("get dirents failed");
	}

	return dir_size;
}

int audio_get_dir_count(const char *path, unsigned int size)
{
	int count = 0;
	int dirents_size;
	struct directory *file = NULL;
	struct directory *folder = NULL;

	if (check_valid_path(path, size) != 0) {
		AUDIO_LOGE("invalid err");
		return -1;
	}

	dirents_size = get_dirents_size(path);
	/* judge path type based on dirents_size, file or folder */
	if (dirents_size == 0) {
		file = create_file(path, size);
		count = (int)get_file_count(file);
		destory_file(file);
	} else if (dirents_size > 0) {
		folder = create_folder(path, size);
		count = (int)get_folder_count(folder);
		destory_folder(folder);
	} else {
		AUDIO_LOGE("get dirents failed");
	}

	return count;
}

/* append root path by system time */
int audio_append_dir_systime(char *full_path, const char *root_path,
	size_t full_path_size)
{
	int ret;
	struct rtc_time tm;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0))
	struct timespec64 tv;
#else
	struct timeval tv;
#endif

	if (full_path == NULL) {
		AUDIO_LOGE("full path is null");
		return -EINVAL;
	}

	if (root_path == NULL) {
		AUDIO_LOGE("root path is null");
		return -EINVAL;
	}

	if (full_path_size == 0 || full_path_size > FILE_NAME_MAX_LEN) {
		AUDIO_LOGE("input dir size error, size: %zd", full_path_size);
		return -EINVAL;
	}

	(void)memset_s(&tm, sizeof(tm), 0, sizeof(tm)); /* unsafe_function_ignore: memset */
	(void)memset_s(&tv, sizeof(tv), 0, sizeof(tv)); /* unsafe_function_ignore: memset */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0))
	ktime_get_real_ts64(&tv);
#else
	do_gettimeofday(&tv);
#endif
	tv.tv_sec -= (long)sys_tz.tz_minuteswest * 60;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
	rtc_time64_to_tm(tv.tv_sec, &tm);
#else
	rtc_time_to_tm(tv.tv_sec, &tm);
#endif

	ret = snprintf_s(full_path, full_path_size, full_path_size - 1, "%s%04d%02d%02d%02d%02d%02d/",
		root_path, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);
	if (ret == -1) {
		AUDIO_LOGE("copy full_path failed");
		return -EINVAL;
	}

	return 0;
}

/* append system time to root path, saved in full path and create full path */
int audio_create_rootpath(char *full_path, const char *root_path,
	size_t full_path_size)
{
	if (full_path == NULL) {
		AUDIO_LOGE("full_path is null");
		return -EINVAL;
	}

	if (root_path == NULL) {
		AUDIO_LOGE("root full_path is null");
		return -EINVAL;
	}

	if (full_path_size == 0 || full_path_size > FILE_NAME_MAX_LEN) {
		AUDIO_LOGE("input dir size error, size: %zu", full_path_size);
		return -EINVAL;
	}

	if (audio_get_dir_size(root_path, full_path_size) > MAX_PCM_SIZE)
		audio_remove_dir(root_path, full_path_size);

	if (audio_append_dir_systime(full_path, root_path, full_path_size)) {
		AUDIO_LOGE("append dir name failed");
		return -EINVAL;
	}

	if (audio_create_dir(full_path, full_path_size)) {
		AUDIO_LOGE("create dir or chown failed");
		return -EINVAL;
	}

	return 0;
}

