/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: sequence file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 17 11:23:07 2023
 */
#ifndef SYSMGR_INTAPI_SEQ_FILE_H
#define SYSMGR_INTAPI_SEQ_FILE_H

#include <stddef.h>
#include <stdbool.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <intapi/vfs/file.h>
#include <intapi/mem/mcache.h>
#include <libhmsync/raw_mutex.h>

#define SEQ_WIDTH2	2
#define SEQ_WIDTH8	8
#define SEQ_WIDTH16	16
#define SEQ_OCTAL_NUM	4

DECLARE_MCACHE(seq_file)

struct maps_private_s {
	struct vspace_s *vspace;
	char *vr_segname;
};

struct seq_operations;

struct seq_file_s {
	char *buf;
	void *private;
	const struct seq_operations *op;
	const struct file *file;
	struct raw_mutex mutex;
	size_t size;
	size_t used;
	size_t pad_until;

	size_t from;
	/* already read */
	off_t read_pos;
	/* current index for iter */
	off_t index;
	int err;
};

struct seq_operations {
	void* (*start) (struct seq_file_s *s, off_t *pos);
	void (*stop) (struct seq_file_s *s, void *vr);
	void* (*next) (struct seq_file_s *s, void *vr, off_t *pos);
	int (*show) (struct seq_file_s *s, void *vr);
};

#define SEQ_INIT(seq, _buf, _size) \
	struct seq_file_s seq##_s = { \
		.buf = _buf,	\
		.size = _size,	\
		.used = 0,	\
		.pad_until = 0,	\
		.err = 0,	\
	};			\
	struct seq_file_s *seq = &seq##_s;

static inline void seq_init(struct seq_file_s *seq_buf, char *buf, size_t size)
{
	seq_buf->buf = buf;
	seq_buf->size = size;
	seq_buf->used = 0;
	seq_buf->pad_until = 0;
	seq_buf->err = 0;
}

static inline bool seq_is_overflow(const struct seq_file_s *seq_buf)
{
	return seq_buf->used >= seq_buf->size;
}

/* check the errno first, then check if overflow */
static inline int seq_err_of(const struct seq_file_s *seq_buf)
{
	if (seq_buf->err < 0) {
		return seq_buf->err;
	} else {
		return seq_is_overflow(seq_buf) ? E_HM_FBIG : (int)(unsigned int)seq_buf->used;
	}
}

static inline void seq_set_overflow(struct seq_file_s *seq_buf)
{
	seq_buf->used = seq_buf->size;
}

static inline void seq_put_char(struct seq_file_s *seq_buf, char c)
{
	if (seq_buf->used < seq_buf->size) {
		seq_buf->buf[seq_buf->used++] = c;
	}
}

/*
 * get a buffer from the seq_file buffer.
 * If buffer available, return the buffer size, or set NULL & return 0.
 *
 * pair with seq_commit
 *
 * 1. seq_get_buf
 * 2. write some data
 * 3. commit the data written to the buffer acquired by 1).
 */
static inline size_t seq_get_buf(struct seq_file_s *seq_buf, char **pbuf)
{
	/* when overflow set, can be equal */
	BUG_ON(seq_buf->used > seq_buf->size);
	size_t size = 0;

	if (seq_buf->used < seq_buf->size) {
		*pbuf = seq_buf->buf + seq_buf->used;
		size = seq_buf->size - seq_buf->used;
	} else {
		/* equal, overflow */
		*pbuf = NULL;
	}

	return size;
}

/* use num < 0 to issue a error */
static inline void seq_commit(struct seq_file_s *seq_buf, int num)
{
	if (num > 0) {
		BUG_ON(seq_buf->used + num > seq_buf->size);
		/* still can be overflow */
		seq_buf->used += num;
	} else {
		/* set overflow */
		seq_buf->used = seq_buf->size;
		seq_buf->err = num;
	}
}

/*
 * set padding width
 *
 * pair with the seq_pad
 *
 * 1. call seq_setwidth to set the max width
 * 2. write some data
 * 3. call seq_pad to pad the remaining bytes
 */
static inline void seq_setwidth(struct seq_file_s *seq_buf, size_t size)
{
	/* set_pad will check the pad_until when pad */
	seq_buf->pad_until = seq_buf->used + size;
}

/*
 * e.g. '\n' -> "\\012"
 * out[5]: need four char and one '\0'
 */
void char_to_octal(char c, char out[5]);
size_t str_count_esc(const char *str, const char *esc);

/**
 *	seq_escape_special_char - replace char in esc and save to buf
 *	@buf: structure for storing data
 *	@len: length of the buf
 *	@str: string will be processed
 *	@esc: string include char which will be replaced
 *
 *	Check whether the char in @str are in @esc, if yes, perform octal conversion.
 *	and then save to @seq_buf->buf
*/
void seq_escape_special_char(char *buf, size_t len, const char* str, const char* esc);

/* Escape the characters in the esc parameter, caller should make sure str is valid */
void seq_put_str_with_esc(struct seq_file_s *seq_buf, const char *str, const char *esc);
void seq_put_str(struct seq_file_s *seq_buf, const char *str);
void seq_put_hex_ll(struct seq_file_s *seq_buf, const char *delimiter,
		    unsigned long long v, unsigned int width);
void seq_put_decimal_ull_width(struct seq_file_s *seq_buf, const char *delimiter,
			       unsigned long long v, unsigned int width);
void seq_put_decimal_ll(struct seq_file_s *seq_buf, const char *delimiter, long long v);
void seq_pad(struct seq_file_s *seq_buf, char c);
void seq_set_pad(struct seq_file_s *seq, char c, size_t used);
int seq_write(struct seq_file_s *seq_buf, const void *data, size_t len);
void seq_vprintf(struct seq_file_s *seq_buf, const char *f, va_list args);
void seq_printf(struct seq_file_s *seq_buf, const char *f, ...);

static inline void seq_put_decimal_ull(struct seq_file_s *seq_buf,
				       const char *delimiter, unsigned long long v)
{
	return seq_put_decimal_ull_width(seq_buf, delimiter, v, 0);
}

#endif
