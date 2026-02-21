/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define structure and prototypes of line buffer in tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:56:50 2019
 */
#ifndef TTY_LINE_BUF_H
#define TTY_LINE_BUF_H

#include "tty_wrap.h"

#define MAX_LINE_BUF_SIZE 4096U
#define MAX_LINE_BUF_EXTEND_TIMES 4U

enum LINE_BUF_STATUS {
	LINE_BUF_STATUS_EMPTY = 1,
	LINE_BUF_STATUS_PARTIAL,
	LINE_BUF_STATUS_FULL
};

struct tty_line_buf_info_s {
	unsigned int data_len;
	unsigned int max_len;
	unsigned int read_sem_waiters;
	unsigned int write_sem_waiters;
};

struct tty_line_buf {
	char *data;
	unsigned int max_len;
	unsigned int head; /* point to the first readable char */
	unsigned int len;
	struct tty_mutex_t mutex; /* the mutex lock to protect linebuf */
	struct tty_sem_t read_sem; /* the redaer wait at sem of linebuf */
	struct tty_sem_t write_sem; /* the writer wait at sem of linebuf */
};

void tty_line_buf_init(struct tty_line_buf *line_buf);

void tty_line_buf_destroy(struct tty_line_buf *line_buf);

void tty_line_buf_reset(struct tty_line_buf *line_buf);

int tty_line_buf_read(struct tty_line_buf *line_buf, char *out_buf,
		      unsigned int sz);

int tty_line_buf_write(struct tty_line_buf *line_buf, const char *buf,
		       unsigned int size);

int tty_line_buf_write_override(struct tty_line_buf *line_buf, const char *buf,
				unsigned int size);

int tty_line_buf_write_maybe_swapout(struct tty_line_buf *line_buf,
	const char *input, unsigned int input_len,
	char *swap_output, unsigned int output_len);

int tty_line_buf_write_extend(struct tty_line_buf *line_buf, const char *buf,
			      unsigned int size);

int tty_line_buf_flush(struct tty_line_buf *line_buf);

int tty_line_buf_state(struct tty_line_buf *line_buf);

void tty_line_buf_wake_up_reader(struct tty_line_buf *line_buf);

int tty_line_buf_schedule_reader(struct tty_line_buf *line_buf);

int tty_line_buf_space_used(struct tty_line_buf *line_buf);

int tty_line_buf_space_avail(struct tty_line_buf *line_buf);

void tty_line_buf_wake_up_writer(struct tty_line_buf *line_buf);

int tty_line_buf_schedule_writer(struct tty_line_buf *line_buf);

int tty_line_buf_info(struct tty_line_buf *line_buf, struct tty_line_buf_info_s *line_buf_info);

#endif
