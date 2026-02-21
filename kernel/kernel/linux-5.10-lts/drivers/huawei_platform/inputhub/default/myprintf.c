/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Contexthub dump extract myprintf head file
 * Author: Shengzexuan Shengzexuan@huawei.com
 * Create: 2024-05-13
 */

#include "myprintf.h"
#include <linux/slab.h>

#define PF(fmt, fmt_argv, fp) do { \
	char buff[1024]; \
	if (sprintf_s(buff, sizeof(buff), fmt, fmt_argv) < 0) { \
		pr_err("PF fail!\n"); \
		return NULL; \
	} \
	vfs_llseek(fp, 0L, SEEK_END); \
	vfs_write(fp, buff, strlen(buff), &((fp)->f_pos)); \
	vfs_fsync(fp, 0); \
	} while (0)

static void float_to_string_manual(float num, char *float_str, size_t str_size) {
	union {
		float f;
		struct {
			uint32_t frac : 23;
			uint32_t exp : 8;
			uint32_t sign : 1;
		} parts;
	} float_cast;

	float_cast.f = num;

	int sign = float_cast.parts.sign ? -1 : 1;
	int exp = float_cast.parts.exp - 127;
	uint32_t frac = float_cast.parts.frac;
	uint64_t mantissa = (1UL << 23) | frac;
	uint64_t value;
	if (exp > -23) {
		if (exp >= 0)
			value = mantissa << exp;
		else
			value = mantissa >> (-exp);
	} else {
		value = 0;
	}

	uint64_t int_part = value >> 23;
	uint64_t frac_part = (value & ((1UL << 23) - 1)) * 1000000UL >> 23;
 
	if (sign == -1) {
		if (snprintf_s(float_str, str_size, str_size - 1, "-%llu.%06llu", int_part, frac_part) < 0) {
			pr_err("snprintf_s float_str fail!\n");
		}
	} else {
		if (snprintf_s(float_str, str_size, str_size - 1, "%llu.%06llu", int_part, frac_part) < 0) {
			pr_err("snprintf_s float_str fail!\n");
		}
	}
}

static void PfFlt(char *fmt, float func, struct file *fp)
{
	char float_str[32] = { 0, };
	float_to_string_manual(func, float_str, sizeof(float_str));
	vfs_llseek(fp, 0L, SEEK_END);
	vfs_write(fp, float_str, strlen(float_str), &(fp->f_pos));
	vfs_fsync(fp, 0);
}

static uint32_t *gargv;
static const char *getstr(void)
{
	gargv++;
	return ("");
}

static void escape_num(char *fmt, int c, int percent, char *store) {
	int value;
	for (c = *fmt == '0' ? 4 : 3, value = 0; c-- && *fmt >= '0' && *fmt <= '7'; ++fmt) {
		value <<= 3;
		value += *fmt - '0';
	}
	--fmt;
	if (percent && value == '%') {
		*store++ = '%';
		*store = '%';
	} else {
		*store = value;
	}
}

static int escape(char *fmt, int percent, size_t *len)
{
	char *save, *store;
	int value, c;
	for (save = store = fmt; (c = *fmt); ++fmt, ++store) {
		if (c != '\\') {
			*store = c;
			continue;
		}
		switch (*++fmt) {
		case '\0':
			*store = '\\';
			*++store = '\0';
			*len = store - save;
			return 0;
		case '\\':
		case '\'':
			*store = *fmt;
			break;
		case 'a':
			*store = '\a';
			break;
		case 'b':
			*store = '\b';
			break;
		case 'c':
			*store = '\0';
			*len = store - save;
			return 1;
		case 'f':
			*store = '\f';
			break;
		case 'n':
			*store = '\n';
			break;
		case 'r':
			*store = '\r';
			break;
		case 't':
			*store = '\t';
			break;
		case 'v':
			*store = '\v';
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			escape_num(fmt, c, percent, store);
			break;
		default:
			*store = *fmt;
			break;
		}
	}
	*store = '\0';
	*len = store - save;
	return 0;
}

static char *mknum(char *str, int ch)
{
	static char *copy;
	static size_t copy_size;
	char *newcopy;
	size_t len, newlen;
	len = strlen(str) + 1;
	if (len > copy_size) {
		newlen = ((len + 1023) >> 10) << 10;
		newcopy = krealloc(copy, newlen, GFP_KERNEL);
		if (!newcopy) {
			pr_err("mknum malloc fail!\n");
			return NULL;
		}
		copy = newcopy;
		copy_size = newlen;
	}
	if (memmove_s(copy, sizeof(copy), str, len - 2) != EOK) {
		pr_err("mknum memmove fail!\n");
		return NULL;
	}
	copy[len - 2] = ch;
	copy[len - 1] = '\0';
	return (copy);
}

static char *doformat(char *start, int *rval, struct file *fp)
{
	static const char skip1[] = "#'-+ 0";
	static const char skip2[] = "0123456789";
	char *fmt;
	char convch, nextch;
	fmt = start + 1;
	fmt += strspn(fmt, skip1);
	fmt += strspn(fmt, skip2);
	if (*fmt == '.') {
		++fmt;
		size_t digits = strspn(fmt, skip2);
		if (digits == 0) {
			fmt--;
		} else {
			fmt += digits;
		}
	} else if (!*fmt) {
		pr_err("missing format character\n");
		return NULL;
	}
	convch = *fmt;
	nextch = *++fmt;
	*fmt = '\0';

	switch (convch) {
	case 's':
		{
			const char *p;
			p = getstr();
			PF(start, p, fp);
			break;
		}
	case 'c':
	case 'd':
	case 'i':
	case 'o':
	case 'u':
	case 'x':
	case 'X':
		{
			char *f;
			int32_t val = *(int32_t *) gargv;
			uint32_t uval = *(uint32_t *) gargv;
			int signedconv;
			gargv++;
			signedconv = (convch == 'd' || convch == 'i');
			if ((f = mknum(start, convch)) == NULL)
				return NULL;
			if (signedconv)
				PF(f, val, fp);
			else
				PF(f, uval, fp);
			break;
		}
	case 'e':
	case 'E':
	case 'f':
	case 'F':
	case 'g':
	case 'G':
	case 'a':
	case 'A':
		{
			float p = *(float *)gargv;
			gargv++;
			PfFlt(start, (float)p, fp);
			break;
		}
	default:
		pr_err("illegal format character %c\n", convch);
		return NULL;
	}
	*fmt = nextch;
	return fmt;
}

int progprintf(struct file *fp, char *fmt_in, uint32_t argv[])
{
	size_t len;
	int rval;
	char *format, *start;
	char buff[1024];
	char *fmt;
	pr_debug("progprintf %d\n", (int)strlen(fmt_in));
	if(memcpy_s(buff, sizeof(buff), fmt_in, strlen(fmt_in) + 1) != EOK) {
		pr_err("%s(): memcpy fmt_in fail\n", __func__);
		return -1;
	}

	format = fmt = buff;
	escape(fmt, 1, &len);
	rval = 0;
	gargv = argv;
	start = fmt;
	while (fmt < format + len) {
		if (fmt[0] == '%') {
			vfs_llseek(fp, 0L, SEEK_END);
			vfs_write(fp, start, fmt - start, &(fp->f_pos));
			vfs_fsync(fp, 0);
			if (fmt[1] == '%') {
				char percent_sign = '%';
				vfs_llseek(fp, 0L, SEEK_END);
				vfs_write(fp, &percent_sign, 1, &(fp->f_pos));
				vfs_fsync(fp, 0);
				fmt += 2;
			} else {
				fmt = doformat(fmt, &rval, fp);
				if (fmt == NULL)
					return -1;
			}
			start = fmt;
		} else {
			fmt++;
		}
	}
	vfs_llseek(fp, 0L, SEEK_END);
	vfs_write(fp, start, fmt - start, &(fp->f_pos));
	vfs_fsync(fp, 0);

	return (rval);
}