/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions of line buffer in tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:52:20 2019
 */

#include "tty_line_buf.h"
#include "tty_wrap.h"

static int __tty_line_buf_read(struct tty_line_buf *line_buf, char *out_buf,
			    unsigned int sz)
{
	unsigned int i;
	unsigned int size = sz;
	unsigned int line_buf_total_size;

	line_buf_total_size = line_buf->max_len;
	/* if size oversize, we only read line_buf_total_size at most */
	if (size > line_buf_total_size) {
		size = line_buf_total_size;
	}
	for (i = 0; i < size; i++) {
		unsigned int new_head;
		char c;

		/* if len == 0, just return i as real read size */
		if (line_buf->len == 0) {
			break;
		}
		/* read the head char and write to out_buf */
		c = line_buf->data[line_buf->head % line_buf_total_size];
		out_buf[i] = c;

		/* update head and len */
		new_head = (line_buf->head + 1) % line_buf_total_size;
		line_buf->head = new_head;
		line_buf->len--;
	}

	return (int)i;
}

/*
 * Read from tty input buf to out_buf,
 * return real read char size.
 */
int tty_line_buf_read(struct tty_line_buf *line_buf, char *out_buf,
		      unsigned int sz)
{
	int ret;

	if (line_buf == NULL || out_buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	ret = __tty_line_buf_read(line_buf, out_buf, sz);
	(void)tty_mutex_unlock(&line_buf->mutex);

	return ret;
}
tty_export_symbol(tty_line_buf_read);

static int __tty_line_buf_write(struct tty_line_buf *line_buf, const char *buf,
				unsigned int size, bool override)
{
	unsigned int line_buf_total_size;
	unsigned int idx;
	unsigned int i;

	line_buf_total_size = line_buf->max_len;
	for (i = 0; i < size; i++) {
		/* buf is not full */
		if (line_buf->len < line_buf_total_size) {
			idx = (line_buf->head + line_buf->len) % line_buf_total_size;
			line_buf->data[idx] = buf[i];
			line_buf->len = line_buf->len + 1;
			continue;
		}
		/* buf is already full */
		if (override) {
			/* override the head */
			idx = (line_buf->head + line_buf->len) % line_buf_total_size;
			line_buf->data[idx] = buf[i];
			line_buf->head = (line_buf->head + 1) % line_buf_total_size;
		} else {
			break;
		}
	}

	return (int)i;
}

/*
 * Write to line_buf, if there is not enough space left, only the remaining
 * space is written
 */
int tty_line_buf_write(struct tty_line_buf *line_buf, const char *buf,
		       unsigned int size)
{
	int ret;

	if (line_buf == NULL || buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	ret = __tty_line_buf_write(line_buf, buf, size, false);
	(void)tty_mutex_unlock(&line_buf->mutex);

	return ret;
}
tty_export_symbol(tty_line_buf_write);

/*
 * Write to line_buf, If there is not enough space left, the header characters
 * will be override
 */
int tty_line_buf_write_override(struct tty_line_buf *line_buf, const char *buf,
				unsigned int size)
{
	int ret;

	if (line_buf == NULL || buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	ret = __tty_line_buf_write(line_buf, buf, size, true);
	(void)tty_mutex_unlock(&line_buf->mutex);

	return ret;
}
tty_export_symbol(tty_line_buf_write_override);

int tty_line_buf_write_maybe_swapout(struct tty_line_buf *line_buf,
	const char *input, unsigned int input_len,
	char *swap_output, unsigned int output_len)
{
	int swap_len = 0;
	unsigned int space_avail;
	unsigned int total_size;

	if (line_buf == NULL || input == NULL || swap_output == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	total_size = line_buf->max_len;
	space_avail = total_size - line_buf->len;
	if (space_avail + output_len < input_len) {
		(void)tty_mutex_unlock(&line_buf->mutex);
		return -EFAULT;
	}
	if (space_avail < input_len) {
		swap_len = __tty_line_buf_read(line_buf, swap_output,
					       input_len - space_avail);
	}
	(void)__tty_line_buf_write(line_buf, input, input_len, false);
	(void)tty_mutex_unlock(&line_buf->mutex);

	return swap_len;
}
tty_export_symbol(tty_line_buf_write_maybe_swapout);

static int __tty_line_buf_copy(struct tty_line_buf *line_buf, char *new_data,
			       unsigned int new_len)
{
	int err = 0;
	unsigned int head, len, max_len, part_size;
	char *data;

	head = line_buf->head;
	len = line_buf->len;
	max_len = line_buf->max_len;
	data = line_buf->data;
	if (head + len <= max_len) {
		err = memcpy_s(new_data, new_len, data + head, len);
		if (err != 0) {
			return -err;
		}
	} else {
		/*
		 * old data is in two parts.
		 * 1. copy old_data[head:end], totally `part_size` bytes;
		 * 2. copy old_data[0:len-part_size], totally `len-part_size` bytes.
		 */
		part_size = max_len - head;
		err = memcpy_s(new_data, new_len, data + head, part_size);
		if (err != 0) {
			return -err;
		}
		err = memcpy_s(new_data + part_size, max_len - part_size, data, len - part_size);
		if (err != 0) {
			return -err;
		}
	}

	return err;
}

static int __tty_line_buf_resize(struct tty_line_buf *line_buf, unsigned int new_len)
{
	int err;
	char *new_data;

	/* don't need resize */
	if (new_len == line_buf->max_len) {
		return 0;
	}
	new_data = (char *)tty_mem_zalloc(new_len);
	if (new_data == NULL) {
		tty_err("tty: malloc for tty line buf failed\n");
		return -ENOMEM;
	}
	/*
	 * if new space is smaller (flush) or there is no space allocated before,
	 * just set head and len; otherwise copy old data to new data
	 */
	if (new_len < line_buf->max_len) {
		/* bytes alloced by `zalloc` are all zero, don't need zero again */
		line_buf->head = 0;
		line_buf->len = 0;
	} else {
		err = __tty_line_buf_copy(line_buf, new_data, new_len);
		if (err != 0) {
			tty_mem_free(new_data);
			tty_err("tty: mem copy from old data to new data failed, err: %s\n",
				strerror(err));
			return err;
		}
		line_buf->head = 0;
		tty_mem_free(line_buf->data);
	}
	tty_info("tty: line buf size changed: [%uB -> %uB]\n", line_buf->max_len, new_len);
	line_buf->data = new_data;
	line_buf->max_len = new_len;

	return 0;
}

int tty_line_buf_write_extend(struct tty_line_buf *line_buf, const char *buf,
			      unsigned int size)
{
	int ret;
	unsigned int space_avail, new_size, new_avail;

	if (line_buf == NULL || buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	space_avail = line_buf->max_len - line_buf->len;
	if (space_avail < size) {
		new_size = line_buf->max_len;
		new_avail = space_avail;
		while (new_avail < size &&
		       new_size < MAX_LINE_BUF_SIZE * MAX_LINE_BUF_EXTEND_TIMES) {
			new_size += MAX_LINE_BUF_SIZE;
			new_avail += MAX_LINE_BUF_SIZE;
		}
		ret = __tty_line_buf_resize(line_buf, new_size);
		if (ret < 0) {
			(void)tty_mutex_unlock(&line_buf->mutex);
			return ret;
		}
	}
	ret = __tty_line_buf_write(line_buf, buf, size, true);
	(void)tty_mutex_unlock(&line_buf->mutex);

	return ret;
}
tty_export_symbol(tty_line_buf_write_extend);

/*
 * Flush tty buffer
 */
int tty_line_buf_flush(struct tty_line_buf *line_buf)
{
	int err;

	if (line_buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	/* resize to the default size */
	err = __tty_line_buf_resize(line_buf, MAX_LINE_BUF_SIZE);
	if (err < 0) {
		tty_err("tty: line buf resize failed, err: %s\n", strerror(err));
		tty_mutex_unlock(&line_buf->mutex);
		return err;
	}
	mem_zero_b(line_buf->data, line_buf->max_len);
	line_buf->head = 0;
	line_buf->len = 0;
	(void)tty_mutex_unlock(&line_buf->mutex);
	return 0;
}
tty_export_symbol(tty_line_buf_flush);

/*
 * Return line buf status
 * LINE_BUF_STATUS_EMPTY
 * LINE_BUF_STATUS_PARTIAL
 * LINE_BUF_STATUS_EMPTY
 */
int tty_line_buf_state(struct tty_line_buf *line_buf)
{
	unsigned int total_size;
	int status = (int)LINE_BUF_STATUS_EMPTY;

	if (line_buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	if (line_buf->len > 0) {
		total_size = line_buf->max_len;
		status = (line_buf->len < total_size) ?
			 LINE_BUF_STATUS_PARTIAL : LINE_BUF_STATUS_FULL;
	}
	(void)tty_mutex_unlock(&line_buf->mutex);
	return status;
}
tty_export_symbol(tty_line_buf_state);

void tty_line_buf_init(struct tty_line_buf *line_buf)
{
	char *data = NULL;

	if (line_buf == NULL) {
		tty_warn("tty: tty_line_buf_init line_buf is NULL\n");
		return;
	}
	mem_zero_s(*line_buf);
	tty_mutex_init(&line_buf->mutex);
	(void)tty_sem_init(&line_buf->read_sem, 0);
	(void)tty_sem_init(&line_buf->write_sem, 0);
	data = (char *)tty_mem_zalloc(MAX_LINE_BUF_SIZE);
	if (data == NULL) {
		tty_err("tty: malloc for tty line buf failed\n");
		return;
	}
	line_buf->head = 0;
	line_buf->len = 0;
	line_buf->data = data;
	line_buf->max_len = MAX_LINE_BUF_SIZE;

	return;
}
tty_export_symbol(tty_line_buf_init);

void tty_line_buf_destroy(struct tty_line_buf *line_buf)
{
	if (line_buf == NULL) {
		tty_warn("tty: line buf is NULL in tty line buf destroy\n");
		return;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	tty_mem_free(line_buf->data);
	line_buf->data = NULL;
	line_buf->max_len = 0U;
	line_buf->head = 0U;
	line_buf->len = 0U;
	(void)tty_mutex_unlock(&line_buf->mutex);
}
tty_export_symbol(tty_line_buf_destroy);

/* Only reset buf and head, len. Do not reset mutex */
void tty_line_buf_reset(struct tty_line_buf *line_buf)
{
	int ret;

	if (line_buf == NULL || line_buf->data == NULL) {
		return;
	}
	ret = tty_line_buf_flush(line_buf);
	if (ret < 0) {
		tty_warn("tty: tty_line_buf_flush failed, err: %s\n", strerror(-ret));
	}
}
tty_export_symbol(tty_line_buf_reset);

void tty_line_buf_wake_up_reader(struct tty_line_buf *line_buf)
{
	if (line_buf == NULL) {
		tty_warn("tty: tty_line_buf_wake_up_reader line_buf is NULL\n");
		return;
	}
	(void)tty_sem_post(&line_buf->read_sem);
}
tty_export_symbol(tty_line_buf_wake_up_reader);

int tty_line_buf_schedule_reader(struct tty_line_buf *line_buf)
{
	int ret = 0;
	if (line_buf == NULL) {
		return -EINVAL;
	}
	/* loop until tty_sem_post is called and someone writes to line_buf */
	ret = tty_sem_wait(&line_buf->read_sem);
	return ret;
}
tty_export_symbol(tty_line_buf_schedule_reader);

int tty_line_buf_space_used(struct tty_line_buf *line_buf)
{
	int s;
	if (line_buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	s = (int)line_buf->len;
	(void)tty_mutex_unlock(&line_buf->mutex);
	return s;
}
tty_export_symbol(tty_line_buf_space_used);

int tty_line_buf_space_avail(struct tty_line_buf *line_buf)
{
	int s;
	unsigned int total_size;

	if (line_buf == NULL) {
		return -EINVAL;
	}
	(void)tty_mutex_lock(&line_buf->mutex);
	total_size = line_buf->max_len;
	s = (int)(total_size - line_buf->len);
	(void)tty_mutex_unlock(&line_buf->mutex);
	return s;
}
tty_export_symbol(tty_line_buf_space_avail);

void tty_line_buf_wake_up_writer(struct tty_line_buf *line_buf)
{
	if (line_buf == NULL) {
		tty_warn("tty: wake up writer line_buf is NULL\n");
		return;
	}
	(void)tty_sem_post(&line_buf->write_sem);
}
tty_export_symbol(tty_line_buf_wake_up_writer);

int tty_line_buf_schedule_writer(struct tty_line_buf *line_buf)
{
	int ret = 0;
	if (line_buf == NULL) {
		return -EINVAL;
	}
	ret = tty_sem_wait(&line_buf->write_sem);
	return ret;
}
tty_export_symbol(tty_line_buf_schedule_writer);

int tty_line_buf_info(struct tty_line_buf *line_buf, struct tty_line_buf_info_s *line_buf_info)
{
	int ret = 0;
	if (line_buf == NULL || line_buf_info == NULL) {
		return -EINVAL;
	}

	tty_mutex_lock(&line_buf->mutex);
	line_buf_info->data_len = line_buf->len;
	line_buf_info->max_len = line_buf->max_len;
	line_buf_info->read_sem_waiters = (unsigned int)tty_sem_waiters(&line_buf->read_sem);
	line_buf_info->write_sem_waiters = (unsigned int)tty_sem_waiters(&line_buf->write_sem);
	tty_mutex_unlock(&line_buf->mutex);

	return ret;
}
tty_export_symbol(tty_line_buf_info);
