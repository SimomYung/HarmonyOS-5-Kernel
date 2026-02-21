/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: Extract and Save Sensorhub Dump Log
 * Author: Shengzexuan Shengzexuan@huawei.com
 * Create: 2024-05-13
 */

#include <linux/namei.h>
#include <linux/syscalls.h>
#include "contexthub_extract_save_dump_log.h"

#define FILE_PATH_MAX_LENGTH        128
#define DEFAULT_LOG_DIC_PATH        "/dev/block/by-name/"
#define DEFAULT_LOG_DIC_NAME        "sensorhub_log_dic"
#define LOG_IMAGE_START_NEW_MAGIC   "SENSORHUB_LOG_IMAGE_INFO"
#define DICT_BUFF_SIZE              (1024 * 1024 * 3)
#define LOG_MAXLEN                  1024

#define FRAME_MAGIC_OFFSET          0
#define FRAME_PARA_CNT_OFFSET       1
#define FRAME_HEAD_SIZE             4
#define FRAME_PARA_SIZE             4
#define FRAME_LABLE_SIZE            4
#define FRAME_TIMESTAMP_SIZE        4
#define FRAME_TIMESTAMP_OFFSET      (FRAME_HEAD_SIZE+FRAME_LABLE_SIZE)
#define FRAME_OVERHEAD              (FRAME_HEAD_SIZE+FRAME_LABLE_SIZE+FRAME_TIMESTAMP_SIZE)
#define FRAME_MAGIC                 0xA5
#define COMMIT_STAMP_SIZE           4
#define COMMIT_BIT_SHIFT            4

struct dump_zone_head {
	uint32_t cnt;
	uint32_t len;
	uint32_t tuncate;
};

uint8_t *g_sh_dict = NULL;

static int judge_access(const char *filename)
{
	struct path path;
	int error;

	error = kern_path(filename, LOOKUP_FOLLOW, &path);
	if (error)
		return error;
	error = inode_permission(d_inode(path.dentry), MAY_ACCESS);
	path_put(&path);
	return error;
}

static char *SearchBuffer(char *haystack, int haystacklen, const char *needle, int needlelen)
{
	/* warning: O(n^2) */
	int searchlen = haystacklen - needlelen + 1;

	for (; searchlen-- > 0; haystack++) {
		if (!memcmp(haystack, needle, needlelen)) {
			return haystack;
		}
	}

	return NULL;
}

static uint32_t get_dict_size(struct file *fp, origin_dict_info *ori_dict_info)
{
	uint8_t *head = NULL;
	uint32_t totalsize;
	uint32_t buffsize = DICT_BUFF_SIZE;
	uint8_t *ori_dict_buffer = NULL;
	uint32_t bytes_read;

	ori_dict_buffer = kmalloc(buffsize, GFP_KERNEL);
	if (!ori_dict_buffer) {
		pr_err("Failed to allocate buffer\n");
		return -1;
	}

	bytes_read = vfs_read(fp, ori_dict_buffer, buffsize, &(fp->f_pos));
	if (bytes_read < 0) {
		pr_err("Error reading sh dict to buffer: %u\n", bytes_read);
		kfree(ori_dict_buffer);
		return -1;
	}
	head = (uint8_t *)SearchBuffer((char *)ori_dict_buffer, buffsize, LOG_IMAGE_START_NEW_MAGIC,
		strlen(LOG_IMAGE_START_NEW_MAGIC));
	if (head == NULL || (head - ori_dict_buffer + strlen(LOG_IMAGE_START_NEW_MAGIC) >= buffsize)) {
		pr_err("cannot find magic word!\n");
		kfree(ori_dict_buffer);
		return -1;
	}
	head += strlen(LOG_IMAGE_START_NEW_MAGIC);
	if (memcpy_s(ori_dict_info, sizeof(origin_dict_info), head, sizeof(origin_dict_info))) {
		pr_err("memcpy_s info_new failed\n");
		return -1;
	}
	kfree(ori_dict_buffer);

	totalsize = ori_dict_info->log_size + sizeof(origin_dict_info);
	return totalsize;
}

int load_sh_dict(void)
{
	struct file *fp = NULL;
	char path[PATH_MAXLEN];
	int flags;
	mm_segment_t old_fs;
	int read_size;
	origin_dict_info ori_dict_info;
	uint32_t totalsize;

	if (sprintf_s(path, PATH_MAXLEN, "%s%s", DEFAULT_LOG_DIC_PATH, DEFAULT_LOG_DIC_NAME) < 0) {
		pr_err("%s():path length is too large\n", __func__);
		return -1;
	}
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#ifdef CONFIG_LIBLINUX
    int ret = ksys_access(path, 0);
#else
    int ret = judge_access(path);
#endif
	if (ret != 0) {
		pr_err("failed to judge access, errno:%d\n", ret);
		goto fail_set_fs;
	}

	flags = O_RDONLY;
	fp = filp_open(path, flags, FILE_LIMIT);
	if (IS_ERR(fp)) {
		pr_err("failed to open dict, errno: = %d\n", fp);
		goto fail_set_fs;
	}

	totalsize = get_dict_size(fp, &ori_dict_info);
	if (totalsize < 0) {
		pr_err("failed to get dict size\n");
		goto fail_close_file;
	}

	g_sh_dict = kmalloc(totalsize, GFP_KERNEL);
	if (!g_sh_dict) {
		pr_err("load_sh_dict malloc fail!\n");
		goto fail_close_file;
	}
	sh_dict_info *dict = (sh_dict_info *)g_sh_dict;
	dict->base = ori_dict_info.addr_start;
	dict->size = ori_dict_info.log_size;

	vfs_llseek(fp, 0L, SEEK_SET);
	read_size = vfs_read(fp, dict->log_strings, dict->size, &(fp->f_pos));
	if (read_size != dict->size) {
		pr_err("%s:read file exception with ret %d.\n", __func__, read_size);
		kfree(g_sh_dict);
		g_sh_dict = NULL;
		goto fail_close_file;
	}
	filp_close(fp, NULL);
	set_fs(old_fs);
	return 0;

fail_close_file:
	filp_close(fp, NULL);
fail_set_fs:
	set_fs(old_fs);
	return -1;
}

static unsigned char add_image_commit(uint32_t index, uint32_t next_index, uint8_t *log_buff, struct file *fp) {
	char buf[LOG_MAXLEN];
	unsigned char next_head = log_buff[next_index];
	uint32_t *next = (uint32_t *)&log_buff[next_index];
	if (0 == index) {
		uint32_t *commit = (uint32_t *)&log_buff[index];
		if (sprintf_s(buf, LOG_MAXLEN, "image commit is %x\n", *commit >> COMMIT_BIT_SHIFT) < 0)
			pr_err("%s():image commit is too large\n", __func__);
		vfs_llseek(fp, 0L, SEEK_END);
		vfs_write(fp, buf, strlen(buf), &(fp->f_pos));
		vfs_fsync(fp, 0);
	} else if (0 == next_index % 0x1000) {
		unsigned char tmp = log_buff[next_index + COMMIT_STAMP_SIZE];
		if (tmp == FRAME_MAGIC) {
			next_head = FRAME_MAGIC;
			if (sprintf_s(buf, LOG_MAXLEN, "image commit is %x\n", *next >> COMMIT_BIT_SHIFT) < 0)
				pr_err("%s():image commit is too large\n", __func__);
			vfs_llseek(fp, 0L, SEEK_END);
			vfs_write(fp, buf, strlen(buf), &(fp->f_pos));
			vfs_fsync(fp, 0);
		}
	}
	return next_head;
}

int extract_sh_dump_log(uint8_t *sensorhub_dump_buff, union dump_info dump_info, uint32_t dump_index)
{
	uint8_t *log_buff = NULL;
	uint32_t index = FRAME_MAGIC_OFFSET;
	char *fmt;
	int flags;
	struct file *fp = NULL;
	mm_segment_t old_fs;
	int ret;
	log_buff = sensorhub_dump_buff;
	if (g_sh_dict == NULL) {
		pr_err("g_sh_dict is NULL!\n");
		return -1;
	}

	sh_dict_info *dict = (sh_dict_info *)g_sh_dict;

	struct dump_config *dump = NULL;
	struct dump_zone_head *dzh = NULL;
	dump = iomcu_config_get(IOMCU_CONFIG_USR_DUMP, sizeof(struct dump_config));
	dzh = (struct dump_zone_head *)sensorhub_dump_buff;
	size_t log_buff_size = min(dump->dump_size, dzh->len);
 
	char filename[PATH_MAXLEN];
	if (sprintf_s(filename, PATH_MAXLEN, "sensorhub-%02u_extracted.txt", dump_index) < 0) {
		pr_err("%s():filename is too long\n", __func__);
		goto errfree;
	}

	char path[PATH_MAXLEN];
	if (sprintf_s(path, PATH_MAXLEN, "%s%s", SH_DMP_DIR, filename) < 0) {
		pr_err("%s():path length is too large\n", __func__);
		goto errfree;
	}

	old_fs = get_fs();
	set_fs(KERNEL_DS);
    flags = O_CREAT | O_RDWR | O_TRUNC;
	fp = filp_open(path, flags, FILE_LIMIT);
	if (IS_ERR(fp)) {
		pr_err("failed to create extract dump result file\n");
		set_fs(old_fs);
		goto errfree;
	}

	do {
		int para_cnt;
		unsigned int log_lv;
		unsigned char head, next_head;
		uint32_t *next;
		uint32_t next_index;
		uint32_t lable;
		if (index >= (log_buff_size - FRAME_HEAD_SIZE - FRAME_TIMESTAMP_SIZE - 1))
			break;
		para_cnt = log_buff[index + FRAME_PARA_CNT_OFFSET] & 0xf;
		log_lv = log_buff[index + FRAME_PARA_CNT_OFFSET] >> 4;
		if (index + (para_cnt * FRAME_PARA_SIZE) + FRAME_OVERHEAD >= log_buff_size)
			break;
		next_index = index + para_cnt * FRAME_PARA_SIZE + FRAME_OVERHEAD;
		head = log_buff[index];
		next_head = log_buff[next_index];
		next = (uint32_t *)&log_buff[next_index];
		lable = *(uint32_t *)(&log_buff[index + FRAME_HEAD_SIZE]) - dict->base;

		char buf[LOG_MAXLEN];
		next_head = add_image_commit(index, next_index, log_buff, fp);
		if (log_lv <= LOG_LOWEST_LEVEL && lable < dict->size &&
			((head == FRAME_MAGIC && next_head == FRAME_MAGIC) || (head == FRAME_MAGIC && *next == 0))) {
			fmt = &dict->log_strings[lable];
			if (sprintf_s(buf, LOG_MAXLEN, "[%10u][%6s]: ",
				*(uint32_t *)&log_buff[index + FRAME_TIMESTAMP_OFFSET], log_prompts[log_lv]) < 0) {
				pr_err("%s():core is too large\n", __func__);
				filp_close(fp, NULL);
				set_fs(old_fs);
				goto errfree;
			}
			vfs_llseek(fp, 0L, SEEK_END);
			vfs_write(fp, buf, strlen(buf), &(fp->f_pos));
			vfs_fsync(fp, 0);
			progprintf(fp, fmt, (uint32_t *)&log_buff[index + FRAME_OVERHEAD]);
			index += (FRAME_OVERHEAD + para_cnt * FRAME_PARA_SIZE);
		} else {
			index++;
		}
	} while(1);

	ret = (int)vfs_fchown(fp, ROOT_UID, SYSTEM_GID);
	if (ret)
		pr_err("[%s], chown %s uid [%d] gid [%d] failed err [%d]!\n", __func__, path, ROOT_UID, SYSTEM_GID, ret);
	filp_close(fp, NULL);
	set_fs(old_fs);
	kfree(g_sh_dict);
	g_sh_dict = NULL;

	return 0;
errfree:
		kfree(g_sh_dict);	
		g_sh_dict = NULL;
		return -1;
}