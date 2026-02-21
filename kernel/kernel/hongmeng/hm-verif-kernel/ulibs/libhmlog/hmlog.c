/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implementations of hongmeng kernel log api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 19 03:43:55 2018
 */
#include <libhmlog/hmlog.h>
#include <libhmlog/klogctl.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_log.h>

#define LOG_BUF_SIZE 512U
#define ERR_MSG "[hmlog truncated]"

raw_static_assert((int)HMLOG_INVALID == HMLOG_LEVEL_NR, hmlog_level_numbers_error);

static enum hmlog_output_type g_output_type = HMLOG_DEFAULT_OUTPUT;
static struct raw_mutex log_mutex = RAW_MUTEX_INITIALIZER;

static const char *level_names[HMLOG_LEVEL_NR] = {
	[HMLOG_DEBUG]	= "DEBUG",
	[HMLOG_INFO]	= "INFO",
	[HMLOG_WARN]	= "WARN",
	[HMLOG_ERROR]	= "ERROR",
	[HMLOG_PANIC]	= "PANIC",
	[HMLOG_NOLOCK]	= "NOLOCK",
};

struct buf_info {
	char *buf;
	size_t size;
};

const char *hmlog_name_of_level(unsigned int level)
{
	if (level > (unsigned int)HMLOG_PANIC) {
		return "UNKNOWN";
	}
	return level_names[level];
}

unsigned int hmlog_level_of_name(const char *name)
{
	unsigned int level;

	if (name == NULL) {
		return UINT_MAX;
	}
	if (strcmp(name, "DEBUG") == 0) {
		level = (unsigned int)LIBLINUX_LOGLEVEL_DEBUG;
	} else if (strcmp(name, "INFO") == 0) {
		level = (unsigned int)LIBLINUX_LOGLEVEL_INFO;
	} else if (strcmp(name, "WARN") == 0) {
		level = (unsigned int)LIBLINUX_LOGLEVEL_WARNING;
	} else if (strcmp(name, "ERROR") == 0) {
		level = (unsigned int)LIBLINUX_LOGLEVEL_ERR;
	} else if (strcmp(name, "PANIC") == 0) {
		level = (unsigned int)LIBLINUX_LOGLEVEL_EMERG;
	} else {
		level = UINT_MAX;
	}
	return level;
}

void hmlog_setoutput(enum hmlog_output_type output)
{
	if (output >= HMLOG_OUTPUT_INVAL) {
		return;
	}
	raw_mutex_lock(&log_mutex);
	g_output_type = output;
	raw_mutex_unlock(&log_mutex);
}

#ifdef __HOST_LLT__

#define TEST_BUFFER_LEN 4096

static char llt_buffer[TEST_BUFFER_LEN];

char *get_hmlog_buffer(void)
{
	return llt_buffer;
}

void clear_hmlog_buffer(void)
{
	llt_buffer[0] = '\0';
}

void hmlog_output(enum hmlog_output_type output_type, enum hmlog_level level,
		  const char *__log, size_t log_size)
{
	int err;

	UNUSED(level);
	UNUSED(log_size);

	switch (output_type) {
	case HMLOG_OUTPUT_BUFFER:
		err = snprintf_s(llt_buffer, TEST_BUFFER_LEN,
				 TEST_BUFFER_LEN - 1, "%s", __log);
		if (err <= 0) {
			llt_buffer[0] = '\0';
		}

		break;
	case HMLOG_OUTPUT_PRINTF:
		/* pass-through */
	case HMLOG_OUTPUT_CONSOLE:
		/* pass-through */
	default:
		printf("%s", __log);
		break;
	}
}

#else

void hmlog_output(enum hmlog_output_type output_type, enum hmlog_level level,
		  const char *__log, size_t log_size)
{
	switch (output_type) {
	case HMLOG_OUTPUT_KBOX:
		(void)sysfast_syslog((unsigned int)level, __log, log_size, NULL);
		break;
	case HMLOG_OUTPUT_CONSOLE:
		(void)hm_log("%s", __log);
		break;
	case HMLOG_OUTPUT_INVAL:
	/* fall through */
	default:
		/* do nothing */
		break;
	}
}

#endif /* __HOST_LLT__ */

static size_t copy_data(char *buf, size_t buf_size,
			const char *data, size_t data_size)
{
	size_t size = 0U;

	if (buf_size > 0U && data_size > 0U) {
		size = data_size;
		if (size > buf_size) {
			size = buf_size;
		}
		NOFAIL(memcpy_s(buf, buf_size, data, size));
	}
	return size;
}

static size_t min(size_t a, size_t b)
{
	return a < b ? a : b;
}

#define TMP_BUF_SIZE	24
#define DECIMAL		10

static size_t copy_line(char *buf, size_t buf_size, int line)
{
	char tmp[TMP_BUF_SIZE] = { 0 };
	int cnt = 0;
	int n = line;
	size_t offset = 0U;

	if (n < 0) {
		offset += copy_data(&buf[offset], buf_size - offset, "-", 1);
		n = -n;
	}
	if (n == 0) {
		tmp[cnt++] = '0';
	} else {
		while ((n > 0) && (cnt < TMP_BUF_SIZE)) {
			tmp[cnt++] = '0' + n % DECIMAL;
			n /= DECIMAL;
		}
	}
	while (cnt > 0) {
		cnt--;
		offset += copy_data(&buf[offset], buf_size - offset, &tmp[cnt], 1);
	}
	return offset;
}

#define MAX_FUNC_SIZE	64U

static size_t mark_data_truncted(char *buf, size_t buf_size, const char *func, int line)
{
	size_t offset = 0U;

	offset += copy_data(&buf[offset], buf_size - offset, ERR_MSG, strlen(ERR_MSG));
	offset += copy_data(&buf[offset], buf_size - offset, func,
			    min(strlen(func), MAX_FUNC_SIZE));
	offset += copy_data(&buf[offset], buf_size - offset, ":", 1);
	offset += copy_line(&buf[offset], buf_size - offset, line);
	offset += copy_data(&buf[offset], buf_size - offset, "\n", sizeof("\n"));
	return offset;
}

static size_t __attribute__((format(printf, 4, 0)))
do_hmlog_printf(struct buf_info *log_buf, const char *func, int line,
		const char *fmt, va_list args)
{
	int prefix_len = 0;
	int content_len = 0;
	size_t output_size = 0U;
	char *buf = log_buf->buf;
	size_t buf_size = log_buf->size;

	/* return value of snprintf_s excluding the terminating byte '\0' */
	prefix_len = snprintf_s(buf, buf_size, buf_size - 1U,
				"[%s:%d] ", func, line);
	if (prefix_len >= 0 && prefix_len < (int)buf_size) {
		/* return value of vsnprintf_s excluding the terminating byte '\0' */
		content_len = vsnprintf_s(&buf[prefix_len],
					  buf_size - (unsigned int)prefix_len,
					  (buf_size - (unsigned int)prefix_len) - 1U,
					  fmt, args);
		if (content_len >= 0) {
			/* output size including the terminating byte '\0' */
			output_size = (size_t)(unsigned int)prefix_len + (size_t)(unsigned int)content_len + 1UL;
		}
	}
	if (output_size == 0U) {
		/* output size including the terminating byte '\0' */
		output_size = mark_data_truncted(buf, buf_size, func, line);
	}
	buf[buf_size - 1U] = '\0'; /* defensive terminating null byte */

	return output_size;
}

void __attribute__((__noinline__)) __attribute__((format(printf, 4, 0)))
__hmlog_vprintf(enum hmlog_level level, const char *func,
		int line, const char *fmt, va_list args)
{
	size_t output_size = 0U;
	char output_buf[LOG_BUF_SIZE];
	struct buf_info log_buf = {
		.buf = output_buf,
		.size = LOG_BUF_SIZE,
	};

	output_size = do_hmlog_printf(&log_buf, func, line, fmt, args);
	hmlog_output(g_output_type, level, output_buf, output_size);
}

void __attribute__((format(printf, 5, 6)))
__hmlog_printf_syslog(enum hmlog_level level, const char *file, const char *func,
		      int line, const char *fmt, ...)
{
	UNUSED(file);
	va_list args;
	size_t output_size = 0U;
	char output_buf[LOG_BUF_SIZE];
	struct buf_info log_buf = {
		.buf = output_buf,
		.size = LOG_BUF_SIZE,
	};

	va_start(args, fmt);
	output_size = do_hmlog_printf(&log_buf, func, line, fmt, args);
	va_end(args);

	(void)sysfast_syslog((unsigned int)level, output_buf, output_size, NULL);
}
