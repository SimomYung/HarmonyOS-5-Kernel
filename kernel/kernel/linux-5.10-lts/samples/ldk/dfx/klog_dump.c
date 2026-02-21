// SPDX-License-Identifier: GPL-2.0
#include <sys/param.h>
#include <liblinux/pal.h>
#include <libhmlog/hmlog.h>
#include <stdint.h>
#include <hongmeng/errno.h>

struct klog_buf_s {
	void *buffer;
	unsigned int size;
	unsigned int pos;
	unsigned int total_copied;
	bool wrap;
};

static int dump_klog(const char *buffer, unsigned int len, uintptr_t arg)
{
	struct klog_buf_s *buf = (struct klog_buf_s *)arg;
	bool wrap = buf->wrap;
	unsigned int copy_len;
	int ret = -1;

	while (len > 0) {
		copy_len = MIN(len, buf->size - buf->pos);
		ret = memcpy_s(buf->buffer + buf->pos, buf->size - buf->pos, buffer, copy_len);
		if (ret != 0) {
			ret = -1;
			break;
		}
		buffer += copy_len;
		len -= copy_len;
		buf->pos = (buf->pos + copy_len) % buf->size;
		buf->total_copied += copy_len;

		if (!wrap && (buf->total_copied >= buf->size)) {
			/* return -1 to break liblinux_iter_current_klog since no more space to hold any log */
			ret = -1;
			break;
		}
	}

	return ret;
}

static int reorder_buffer(struct klog_buf_s *buf)
{
	char *tmp = NULL;
	size_t right_size = 0U;
	size_t left_size = 0U;
	int ret = 0;

	if (buf->total_copied > buf->size) {
		left_size = (buf->total_copied % buf->size);
		right_size = buf->size - left_size;
		if (left_size > 0) {
			tmp = malloc(left_size);
			if (tmp == NULL) {
				ret = -1;
			}
		}
	}

	if (ret == 0 && tmp != NULL) {
		ret = memcpy_s(tmp, left_size, buf->buffer, left_size);
		if (ret == 0) {
			ret = memmove_s(buf->buffer, buf->size, buf->buffer + left_size, right_size);
		}
		if (ret == 0) {
			ret = memcpy_s(buf->buffer + right_size, left_size, tmp, left_size);
		}
		if (ret != 0) {
			ret = -1;
		}
	}
	free(tmp);

	return ret;
}

void liblinux_save_hm_log(char *pbuf, unsigned int buf_size)
{
	int ret;
	struct klog_buf_s buf = {
		.buffer = pbuf,
		.size = buf_size,
		.pos = 0,
		.total_copied = 0,
		.wrap = true,
	};

	if (pbuf != NULL && buf_size > 0) {
		(void)liblinux_iter_current_klog(buf_size, dump_klog, (uintptr_t)&buf);
		if (buf.total_copied > 0) {
			/* newest log may write at head of the buffer since here wrap is used,
			 * reorder it to make the newest log to end of the log */
			ret = reorder_buffer(&buf);
			if (ret != 0) {
				hm_error("Failed to reorder buffer. buf_size:%u, total:%u\n",
						buf_size, buf.total_copied);
			}
		}
	}
}
EXPORT_SYMBOL(liblinux_save_hm_log);

bool liblinux_get_hm_log_buffer(char *pbuf, unsigned int buf_size, size_t *retlen)
{
	struct klog_buf_s buf = {
		.buffer = pbuf,
		.size = buf_size,
		.pos = 0,
		.total_copied = 0,
		.wrap = true,
	};

	if (pbuf == NULL || buf_size == 0) {
		return false;
	}

	(void)liblinux_iter_current_klog(buf_size, dump_klog, (uintptr_t)&buf);
	if (buf.total_copied == 0) {
		return false;
	}

	if (retlen != NULL) {
		*retlen = (size_t)buf.total_copied;
	}

	return true;
}
EXPORT_SYMBOL(liblinux_get_hm_log_buffer);
