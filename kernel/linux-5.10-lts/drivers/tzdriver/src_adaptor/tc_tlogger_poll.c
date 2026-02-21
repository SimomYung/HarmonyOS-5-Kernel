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
#include "tlogger.h"
#include <linux/poll.h>
#include <linux/fs.h>
#include "tc_ns_log.h"
#include "tc_current_info.h"

unsigned int process_tlogger_poll(struct file *file, poll_table *wait)
{
	struct tlogger_reader *reader = NULL;
	struct tlogger_log *log = NULL;
	struct log_buffer *buffer = NULL;
	uint32_t ret = POLLOUT | POLLWRNORM;

	tlogd("logger_poll ++\n");
	if (!file || !wait) {
		tloge("param is null\n");
		return ret;
	}

	reader = file->private_data;
	if (!reader) {
		tloge("the private data is null\n");
		return ret;
	}

	log = reader->log;
	if (!log) {
		tloge("log is null\n");
		return ret;
	}

	buffer = (struct log_buffer*)log->buffer_info;
	if (!buffer) {
		tloge("buffer is null\n");
		return ret;
	}

	poll_wait(file, &reader->wait_queue_head, wait);

	if (buffer->flag.last_pos != reader->r_off)
		ret |= POLLIN | POLLRDNORM;

	return ret;
}
