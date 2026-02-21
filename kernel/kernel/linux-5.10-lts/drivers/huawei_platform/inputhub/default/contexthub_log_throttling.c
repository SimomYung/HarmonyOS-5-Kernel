/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: Log dictionaries are grouped by module headers
 * Author: Yujianfeng @huawei.com
 * Create: 2024-12-31
 */

#include <linux/string.h>
#include <linux/module.h>
#include <linux/fs.h>
#include "securec.h"
#include "contexthub_extract_save_dump_log.h"
#include "contexthub_log_throttling.h"
#include "contexthub_route.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/time64.h>
#include <linux/namei.h>
#include <linux/syscalls.h>

#define SHIFT_24                    24
#define MAX_LOG_NUM                 3000
#define FRAME_MAGIC_OFFSET          0
#define FRAME_HEAD_SIZE_VER2        8
#define FRAME_TIMESTAMP_SIZE        4
#define FRAME_MAGIC                 0xA5
#define LOW_NIBBLE_MASK             0x0F
#define ARRAY_OFFSET_1              0x1
#define ARRAY_OFFSET_2              0x2
#define ARRAY_OFFSET_3              0x3
#define BIT_SHIFT_8                 8
#define BIT_SHIFT_16                16
#define BIT_SHIFT_24                24
#define FRAME_CNT_OFFSET            1
#define FRAME_PARA_SIZE             4
#define FRAME_ADDR_OFFSET           4
#define SSH_LOG_MAX_NUM             3000
#define SSH_LOG_THRO_TIMES_MS       (1000 * 300)
#define SSH_LOG_THRO_TIMES_S        300
#define array_size(arr)		(sizeof(arr) / sizeof((arr)[0]))
#define DEFAULT_LOG_DIC_PATH        "/dev/block/by-name/"
#define DEFAULT_LOG_DIC_NAME        "sensorhub_log_dic"
#define DICT_BUFF_SIZE              (1024 * 1024 * 3)
#define LOG_IMAGE_START_NEW_MAGIC   "SENSORHUB_LOG_IMAGE_INFO"

static log_label_tabel g_ssh_log_label_tabel = {0};
static bool g_ssh_log_label_tabel_is_enable = false;
static uint32_t g_logStrStartAddr = 0;
static uint32_t g_logStrSize = 0;
static uint32_t g_ssh_label_num[SENSORHUB_TAG_MAX];
static bool g_is_log_thro_list[SENSORHUB_TAG_MAX];
static char g_log_head_array[SENSORHUB_TAG_MAX][SENSORHUB_HEAD_MAX];
static uint16_t g_log_tag_nums = 0;
static char g_log_label_whitelist[][SENSORHUB_HEAD_MAX] = {
    "2[log]",
};
static time64_t ssh_last_print_log_time;
static uint8_t *g_sh_dict_for_logthro = NULL;
static int g_logbuff_out_counter = 0;

static void print_ssh_log_num(void)
{
    if (g_ssh_log_label_tabel_is_enable == false) {
        pr_info("%s g_ssh_log_label_tabel_is_enable = %d\n", __func__, g_ssh_log_label_tabel_is_enable);
        return;
    }

    pr_info("********print_ssh_log_num start********\n");
    pr_info("%-54s %-20s\n", "ssh_log_label", "ssh_log_num");

    for (int i = 0; i < g_log_tag_nums; i++) {
        if (g_ssh_label_num[i] != 0) {
            pr_info("ssh_log_label:%-30s, ssh_log_num:%-8d.\n", g_log_head_array[i], g_ssh_label_num[i]);
        }
    }

    pr_info("********print_ssh_log_num finished********\n");
}

static char* get_log_head(const char* log_entry)
{
    if (log_entry == NULL) {
        pr_err("contexthub_log_throttling.c get_log_head get log_entry is NULL\n");
        return NULL;
    }

    const char* start = log_entry;
    const char* bracket_end = strchr(start, ']');
 
    if (start == NULL || bracket_end == NULL || start >= bracket_end) {
        pr_err("start or bracket_end wrong!\n");
        return NULL;
    }
 
    size_t len = bracket_end - log_entry + 1;
    char* log_head = (char*)kmalloc(len + 1, GFP_KERNEL);
 
    if (log_head == NULL) {
        return NULL;
    }
 
    if (memcpy_s(log_head, len + 1, start, len) != 0) {
        kfree(log_head);
        pr_err("log head memcpy failed\n");
        return NULL;
    }
    log_head[len] = '\0';
 
    return log_head;
}

static int get_log_head_tag(const char* log_head)
{
    if (log_head == NULL) {
        pr_err("log_head get NULL!\n");
        return -1;
    }
    
    int ret = 0;
    if (g_log_tag_nums >= SENSORHUB_TAG_MAX) {
        pr_err("Log head storage is full\n");
        return -1;
    }

    for (int i = 0; i < g_log_tag_nums; i++) {
        if (strcmp(log_head, g_log_head_array[i]) == 0) {
            return i;
        }
    }

    if (memcpy_s(g_log_head_array[g_log_tag_nums], SENSORHUB_HEAD_MAX, log_head, strlen(log_head) + 1) != 0) {
        pr_err("log head save in label failed\n");
        return -1;
    }
    pr_info("%s add log label[%d]: %s!\n", __func__, g_log_tag_nums, g_log_head_array[g_log_tag_nums]);

    ret = g_log_tag_nums;
    g_log_tag_nums++;
    
    return ret;
}

static uint32_t compare_log_heads(const char* current_log_head, const char* prev_log_head)
{
    if (current_log_head != NULL && prev_log_head != NULL) {
        if (strcmp(prev_log_head, current_log_head) != 0) {
            return 1;
        }
    }
        
    if (prev_log_head == NULL && current_log_head != NULL) {
            pr_info("first prev_log_head is null\n");
            return 1;
    }
    return 0;
}

static int update_log_config(const sh_dict_info *dict, log_label_tabel *label_addr_spr)
{
    const char* log_entry = dict->log_strings;
    char* prev_log_head = NULL;
    char* current_log_head = NULL;
    uint16_t len = 0;

    while (log_entry < (dict->log_strings + dict->size)) {
        if (log_entry == NULL) {
            pr_err("contexthub_log_throttling %s get log_entry is NULL!\n",  __func__);
            break;
        }
        current_log_head = get_log_head(log_entry); //malloc
        if (compare_log_heads(current_log_head, prev_log_head) == 1) {
            label_addr_spr->addr_group[len] = log_entry - dict->log_strings + g_logStrStartAddr;
            int tag = get_log_head_tag(current_log_head);
            if (tag >= 0 && tag < SENSORHUB_TAG_MAX) {
                label_addr_spr->label_tag[len] = (uint16_t)tag;
            }

            pr_info("subscript:%d, Log head changed at: 0X%x, before :%s, now:%s\n", len,
                    label_addr_spr->addr_group[len], prev_log_head, g_log_head_array[tag]);
            len++;

            if (prev_log_head != NULL) {
                kfree(prev_log_head);
                prev_log_head = NULL;
            }
            prev_log_head = current_log_head;
        }
        
        log_entry = strchr(log_entry, '\0');
        if (log_entry != NULL) {
            log_entry++;
        }

        if (len >= SENSORHUB_TAG_MAX) {
            pr_err("update_log_config log_label number is over %d\n!", SENSORHUB_TAG_MAX);
            break;
        }
    }

    kfree(prev_log_head);
    kfree(current_log_head);
    prev_log_head = NULL;
    current_log_head = NULL;
    label_addr_spr->lenth = len;
    return 0;
}

static void do_collect_ssh_log_num(uint32_t addr)
{
    if (addr > g_logStrStartAddr + g_logStrSize || addr < g_logStrStartAddr) {
        pr_err("%s get unvalid addr : 0X%x\n", __func__, addr);
        return;
    }

    char log_label[SENSORHUB_HEAD_MAX] = {0};
    for (int i = g_ssh_log_label_tabel.lenth - 1; i >= 0; i--) {
        if (addr >= g_ssh_log_label_tabel.addr_group[i]) {
            g_ssh_label_num[g_ssh_log_label_tabel.label_tag[i]] += 1;
            break;
        }
    }
}

static void check_print_ssh_log_num_time(void)
{
    time64_t get_data_current_time;
    struct timespec64 tv;
	memset_s(&tv, sizeof(struct timespec64), 0, sizeof(struct timespec64));
	ktime_get_real_ts64(&tv);
	get_data_current_time = tv.tv_sec;
    if ((get_data_current_time - ssh_last_print_log_time) >= SSH_LOG_THRO_TIMES_S) {
        pr_info("%s now_time_is %llds, ssh_last_print_log_time:%lld.\n", 
            __func__, get_data_current_time, ssh_last_print_log_time);
        print_ssh_log_num();
        ssh_last_print_log_time = get_data_current_time;
        for (int i = 0; i < g_log_tag_nums; i++) {
            g_ssh_label_num[i] = 0;
            g_is_log_thro_list[i] = false;
        }
    }
}

static void increment_logbuff_out_counter(size_t len, uint32_t index, uint32_t index_max_offset)
{
    g_logbuff_out_counter++;

    // 检查计数器是否达到100
    if (g_logbuff_out_counter == 100) {
        // 打印日志
        pr_warn("%s len: %u is not enough for index + max_offset=%u + %u, 100times\n",
            __func__, len, index, index_max_offset);
        // 将计数器清零
        g_logbuff_out_counter = 0;
    }
}

int collect_ssh_log_num(const uint8_t *logbuff, size_t len)
{
    if (g_ssh_log_label_tabel_is_enable == false) {
        return -1;
    }

    check_print_ssh_log_num_time();

    if (logbuff == NULL || len == 0) {
        pr_err("%s get unvalid data!\n", __func__);
        return -1;
    }

    uint32_t index = FRAME_MAGIC_OFFSET;
    uint32_t addr;
    uint32_t index_max_offset = 0;

    while (len - FRAME_HEAD_SIZE_VER2 - FRAME_TIMESTAMP_SIZE + 1 > index) {
        index_max_offset = (logbuff[index + FRAME_CNT_OFFSET] & LOW_NIBBLE_MASK) * FRAME_PARA_SIZE +
            FRAME_TIMESTAMP_SIZE + FRAME_HEAD_SIZE_VER2 + ARRAY_OFFSET_3;

        if ((logbuff[index] == FRAME_MAGIC) && (len < (index + index_max_offset + 1))) {
            increment_logbuff_out_counter(len, index, index_max_offset);
        }

        if ((logbuff[index] == FRAME_MAGIC && len < (index + index_max_offset + 1)) ||
            (logbuff[index] == FRAME_MAGIC && (logbuff[index + index_max_offset - ARRAY_OFFSET_3] == FRAME_MAGIC)) ||
            (logbuff[index] == FRAME_MAGIC && (logbuff[index + index_max_offset - ARRAY_OFFSET_3] == 0x00) &&
            (logbuff[index + index_max_offset - ARRAY_OFFSET_2] == 0x00) &&
            (logbuff[index + index_max_offset - ARRAY_OFFSET_1] == 0x00) &&
            (logbuff[index + index_max_offset] == 0x00))) {
            addr = logbuff[index + FRAME_ADDR_OFFSET] +
                ((uint32_t)logbuff[index + FRAME_ADDR_OFFSET + ARRAY_OFFSET_1] << BIT_SHIFT_8) + /* 8 bits */
                ((uint32_t)logbuff[index + FRAME_ADDR_OFFSET + ARRAY_OFFSET_2] << BIT_SHIFT_16) + /* 16 bits */
                ((uint32_t)logbuff[index + FRAME_ADDR_OFFSET + ARRAY_OFFSET_3] << BIT_SHIFT_24); /* 24 bits */

            if (addr > g_logStrStartAddr + g_logStrSize) {
                pr_err("%s get log_label overflow : 0X%x\n", __func__, addr);
                index++;
                continue;
            }

            do_collect_ssh_log_num(addr);
            index += (FRAME_TIMESTAMP_SIZE + FRAME_HEAD_SIZE_VER2 +
                    (logbuff[index + FRAME_CNT_OFFSET] & LOW_NIBBLE_MASK) * FRAME_PARA_SIZE);
        } else {
            index++;
        }
    }

    return 0;
}

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

static bool shh_log_thro_check_file_access(char *path)
{
    return sprintf_s(path, PATH_MAXLEN, "%s%s", DEFAULT_LOG_DIC_PATH, DEFAULT_LOG_DIC_NAME) >= 0 &&
           (
#ifdef CONFIG_LIBLINUX
               ksys_access(path, 0) == 0
#else
               judge_access(path) == 0
#endif
           );
}

static int shh_log_thro_read_file(struct file *fp, sh_dict_info *dict)
{
    int read_size = vfs_read(fp, dict->log_strings, dict->size, &(fp->f_pos));
    if (read_size != dict->size) {
        return -1;
    }
    return 0;
}

static int shh_log_thro_load_dict(void)
{
    struct file *fp = NULL;
    char path[PATH_MAXLEN];
    int flags;
    mm_segment_t old_fs;
    origin_dict_info ori_dict_info;
    uint32_t totalsize;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    if (!shh_log_thro_check_file_access(path)) {
        goto fail;
    }

    flags = O_RDONLY;
    fp = filp_open(path, flags, FILE_LIMIT);
    if (IS_ERR(fp)) {
        goto fail;
    }

    totalsize = get_dict_size(fp, &ori_dict_info);
    if (totalsize < 0) {
        goto fail_close_file;
    }

    g_sh_dict_for_logthro = kmalloc(totalsize, GFP_KERNEL);
    if (!g_sh_dict_for_logthro) {
        goto fail_close_file;
    }

    sh_dict_info *dict = (sh_dict_info *)g_sh_dict_for_logthro;
    dict->base = ori_dict_info.addr_start;
    dict->size = ori_dict_info.log_size;

    vfs_llseek(fp, 0L, SEEK_SET);
    if (shh_log_thro_read_file(fp, dict) != 0) {
        kfree(g_sh_dict_for_logthro);
        g_sh_dict_for_logthro = NULL;
        goto fail_close_file;
    }

    filp_close(fp, NULL);
    set_fs(old_fs);
    return 0;

fail_close_file:
    if (!IS_ERR(fp)) {
        filp_close(fp, NULL);
    }
    return -1;

fail:
    set_fs(old_fs);
    return -1;
}

int ssh_log_label_tabel_generate(void)
{
    pr_info("ssh_log_label_tabel_generate start\n");
    if (shh_log_thro_load_dict() == 0) {
		pr_info("ssh_log_label_tabel_generate load dict succ\n");
	} else {
		pr_err("ssh_log_label_tabel_generate failed to load dict\n");
        goto out;
	}
 
    if (g_sh_dict_for_logthro == NULL) {
		pr_err("ssh_log_label_tabel_generate g_sh_dict_for_logthro is NULL!\n");
        goto out;
	}

    sh_dict_info *dict = (sh_dict_info *)g_sh_dict_for_logthro;

    g_logStrStartAddr = dict->base;
    g_logStrSize = dict->size;
    pr_info("%s sensorhub log dict size: 0X%x bytes, base_addr 0X%x\n",
        __func__, dict->size, dict->base);

    if (update_log_config(dict, &g_ssh_log_label_tabel)) {
        pr_err("%s update_log_config failed\n", __func__);
        kfree(g_sh_dict_for_logthro);
	    g_sh_dict_for_logthro = NULL;
        goto out;
    }

    kfree(g_sh_dict_for_logthro);
	g_sh_dict_for_logthro = NULL;

    g_ssh_log_label_tabel_is_enable = true;
    for (int i = 0; i < g_log_tag_nums; i++) {
        pr_info("%s, g_log_head_array[%d]: %s \n", __func__, i, g_log_head_array[i]);
    }

    pr_info("ssh_log_label_tabel_generate work success!\n");
	return 0;

out:
    g_ssh_log_label_tabel_is_enable = false;
	return -1;
}

