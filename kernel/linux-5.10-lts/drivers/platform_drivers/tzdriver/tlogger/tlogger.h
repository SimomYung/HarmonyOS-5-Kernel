/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 * Description: TEE Logging Subsystem, read the tee os log from rdr memory
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
#ifndef TLOGGER_H
#define TLOGGER_H

#include <linux/types.h>
#include <linux/poll.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/miscdevice.h>

#define OPEN_FILE_MODE          0640U
#define ROOT_UID                0
#define ROOT_GID                0
#define SYSTEM_GID              1000
#ifdef LAST_TEE_MSG_ROOT_GID
#define FILE_CHOWN_GID                0
#else
/* system gid for last_teemsg file sys chown */
#define FILE_CHOWN_GID                1000
#endif

#define LOG_BUFFER_RESERVED_LEN    11U
#define VERSION_INFO_LEN           156U
/*
 * Log's buffer flag info, size: 64 bytes head + 156 bytes's version info.
 * For filed description:
 * last_pos : current log's end position, last log's start position.
 * write_loops: Write cyclically. Init value is 0, when memory is used
 *              up, the value add 1.
 */
struct log_buffer_flag {
	uint32_t reserved0;
	uint32_t last_pos;
	uint32_t write_loops;
	uint32_t log_level;
	/* [0] is magic filed, [1] is serial_no filed, uesd for log retention feature */
	uint32_t reserved[LOG_BUFFER_RESERVED_LEN];
	uint32_t max_len;
	unsigned char version_info[VERSION_INFO_LEN];
};

struct log_buffer {
	struct log_buffer_flag flag;
	unsigned char buffer_start[];
};

struct tlogger_group {
	struct list_head node;
	uint32_t nsid;
	volatile uint32_t reader_cnt;
	volatile uint32_t tlogf_stat;
};

struct tlogger_log {
	unsigned char *buffer_info; /* ring buffer info */
	struct mutex mutex_info; /* this mutex protects buffer_info */
	struct list_head logs; /* log channels list */
	struct mutex mutex_log_chnl; /* this mutex protects log channels */
	struct miscdevice misc_device; /* misc device log */
	struct list_head readers; /* log's readers */
};

struct tlogger_reader {
	struct tlogger_log *log; /* tlogger_log info data */
	struct tlogger_group *group; /* tlogger_group info data */
	struct pid *pid; /* current process pid */
	struct list_head list; /* log entry in tlogger_log's list */
	wait_queue_head_t wait_queue_head; /* wait queue head for reader */
	/* Current reading position, start position of next read again */
	uint32_t r_off;
	uint32_t r_loops;
	uint32_t r_sn;
	uint32_t r_failtimes;
	uint32_t r_from_cur;
	uint32_t r_is_tlogf;
	bool r_all; /* whether this reader can read all entries */
	uint32_t r_ver;
};

unsigned int process_tlogger_poll(struct file *file, poll_table *wait);
#ifdef CONFIG_TEELOG
void tz_log_write(void);
int tlogger_store_msg(const char *file_path, uint32_t file_path_len);
int register_mem_to_teeos(uint64_t mem_addr, uint32_t mem_len, bool is_cache_mem);

#ifdef CONFIG_TZDRIVER_MODULE
int init_tlogger_service(void);
void free_tlogger_service(void);
int register_tloger_mem(void);
#endif

#else
static inline void tz_log_write(void)
{
	return;
}

static inline int tlogger_store_msg(const char *file_path, uint32_t file_path_len)
{
	(void)file_path;
	(void)file_path_len;
	return 0;
}
static inline int register_mem_to_teeos(uint64_t mem_addr, uint32_t mem_len,
	bool is_cache_mem)
{
	(void)mem_addr;
	(void)mem_len;
	return 0;
}
static inline int init_tlogger_service(void)
{
	return 0;
}
static inline void free_tlogger_service(void)
{
}
#endif
#endif
