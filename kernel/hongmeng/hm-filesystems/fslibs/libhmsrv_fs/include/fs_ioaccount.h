/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of fs ioaccount
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 25 23:07:01 2020
 */

#ifndef FS_IOACCOUNT_H
#define FS_IOACCOUNT_H

#include <stdint.h>

#define IOACCOUNT_VERSION  (10)

/* fit with iotop-0.6 version */
#define BLKIO_DELAY_OFFSET    (40)
#define SWAPIN_DELAY_OFFSET   (56)
#define READ_BYTES_OFFSET     (248)
#define PAD_BYTES	      (4)

struct taskstats {
	uint16_t version;

	char pad_blkio_delay[BLKIO_DELAY_OFFSET - sizeof(uint16_t)];
	uint64_t blkio_delay_total; // offset 40

	char pad_swapin_delay[SWAPIN_DELAY_OFFSET - BLKIO_DELAY_OFFSET - sizeof(uint64_t)];
	uint64_t swapin_delay_total; // offset 56

	/* io data */
	char pad_read_bytes[READ_BYTES_OFFSET - SWAPIN_DELAY_OFFSET - sizeof(uint64_t)];
	uint64_t read_bytes; // offset 248
	uint64_t write_bytes;
	uint64_t cancelled_write_bytes;

	uint32_t ac_uid;
	char pad_task[PAD_BYTES];
}__attribute__((aligned(2)));

struct io_account {
	uint64_t blkio_delay_total;
	uint64_t read_bytes;
	uint64_t write_bytes;
};

enum {
	IO_DATA_OK = 0,
	IO_NOT_SUP,
	IO_EMPTY,
};

enum {
	TASKSTATS_CMD_UNSPEC = 0,
	TASKSTATS_CMD_GET,
	TASKSTATS_CMD_NEW,
	TASKSTATS_CMD_MAX,
};

enum {
	TASKSTATS_CMD_ATTR_UNSPEC = 0,
	TASKSTATS_CMD_ATTR_PID,
	TASKSTATS_CMD_ATTR_MAX,
};

enum {
	TASKSTATS_TYPE_UNSPEC = 0,
	TASKSTATS_TYPE_PID,
	TASKSTATS_TYPE_TGID,            /* Thread group id */
	TASKSTATS_TYPE_STATS,           /* taskstats structure */
	TASKSTATS_TYPE_AGGR_PID,        /* contains pid + stats */
	TASKSTATS_TYPE_AGGR_TGID,       /* contains tgid + stats */
};

int fs_get_pid_io_stats(uint32_t pid, struct taskstats *stats);

#endif
