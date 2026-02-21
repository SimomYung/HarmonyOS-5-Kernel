/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of str
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 24 16:32:43 2019
 */

#include <utils/str.h>

#include <errno.h>
#include <stdlib.h>
#include <wchar.h>
#include <fslib_common.h>
#include <hongmeng/panic.h>

#include "internal.h"

/* Get length of UTF-16 string. */
size_t utf16_len(const uint16_t *src, size_t max_len)
{
	size_t len = 0;

	BUG_ON(src == NULL);
	for (; *src != 0; src++) {
		if (++len >= max_len) {
			break;
		}
	}

	return len;
}

static uint32_t cont(const char *str, size_t *index)
{
	uint32_t c = (uint32_t)(unsigned char)str[(*index)++];
	return ((c & UTF8_CONT_MASK) == UTF8_CONT_VALUE) ? (c & UTF8_CH_MASK) : 0;
}

uint32_t to_wchr(const char *str, size_t *index, size_t len)
{
	uint32_t c;
	uint32_t r;

	BUG_ON(str == NULL || index == NULL);
	c = (uint32_t)str[*index];

	if (*index + 1 >= len) {
		return 0;
	}
	(*index)++;

	if ((c & UTF8_1B_MASK) == 0) {
		return c;
	} else if ((c & UTF8_2B_MASK) == UTF8_2B_VALUE) {
		uint32_t c1 = cont(str, index);
		if (c1 == 0) {
			return STR_ERR;
		}
		r = ((c & UTF8_SHIFT1_MASK) << CONT_SHIFT1) | c1;
		return r >= UTF8_2B_MIN ? r : STR_ERR;
	} else if ((c & UTF8_3B_MASK) == UTF8_3B_VALUE) {
		uint32_t c1 = cont(str, index);
		uint32_t c2 = ((c1 == 0) ? 0 : cont(str, index));
		if (c1 == 0 || c2 == 0) {
			return STR_ERR;
		}
		r = ((c & UTF8_SHIFT2_MASK) << CONT_SHIFT2) | (c1 << CONT_SHIFT1) | c2;
		return (r >= UTF8_3B_MIN && r < UTF8_3B_MAX) ? r : STR_ERR;
	} else if ((c & UTF8_4B_MASK) == UTF8_4B_VALUE) {
		uint32_t c1 = cont(str, index);
		uint32_t c2 = ((c1 == 0) ? 0 : cont(str, index));
		uint32_t c3 = ((c2 == 0) ? 0 : cont(str, index));
		if (c1 == 0 || c2 == 0 || c3 == 0) {
			return STR_ERR;
		}
		r = ((c & UTF8_SHIFT3_MASK) << CONT_SHIFT3) | (c1 << CONT_SHIFT2) | (c2 << CONT_SHIFT1) | c3;
		return (r >= UTF8_4B_MIN && r <= UTF16_MAX) ? r : STR_ERR;
	}

	return STR_ERR;
}

/* Convert string src to utf16 string. */
int utf8_to_utf16(const char *src, size_t slen, uint16_t *dest, size_t dlen)
{
	int rc = E_HM_OK;
	size_t offset = 0;
	size_t i = 0;
	size_t cnt = slen;
	uint32_t wch;

	/* If src is "", slen maybe 0, it's ok and dest will be '\0'. */
	if (src == NULL || dlen == 0 || dest == NULL) {
		return E_HM_INVAL;
	}

	while ((wch = to_wchr(src, &offset, STR_SIZE_MAX)) != 0) {
		if (cnt == 0) {
			break;
		}

		if (i >= dlen - 1) {
			rc = E_HM_OVERFLOW;
			break;
		}

		if (wch < UTF8_4B_MIN) {
			dest[i] = (uint16_t)wch;
		} else {
			if (i + 1 >= dlen - 1) {
				rc = E_HM_OVERFLOW;
				break;
			}
			wch -= UTF8_4B_MIN;
			dest[i] = (uint16_t)(UTF8_1W_PRE | (wch >> UTF_SHIFT));
			dest[i + 1] = (uint16_t)(UTF8_2W_PRE | (wch & UTF8_WCH_MASK));
			i++;
		}

		i++;
		cnt--;
	}

	dest[i] = '\0';
	return rc;
}

static int utf16_decode_next(const uint32_t ch, char *dst, size_t *offset, size_t len)
{
	if (dst == NULL || offset == NULL) {
		return E_HM_INVAL;
	}

	if (ch < UTF8_2B_MIN) {
		if (*offset >= len) {
			return E_HM_OVERFLOW;
		}
		dst[(*offset)++] = (char)ch;
		return E_HM_OK;
	} else if (ch >= UTF8_2B_MIN && ch < UTF8_3B_MIN) {
		if (*offset >= len - UTF8_NEED_1B) {
			return E_HM_OVERFLOW;
		}
		dst[(*offset)++] = (char)(UTF8_2B_VALUE | (ch >> CONT_SHIFT1));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | (ch & UTF8_CH_MASK));
		return E_HM_OK;
	} else if (ch >= UTF8_3B_MIN && ch < UTF8_3B_MAX) {
		if (*offset >= len - UTF8_NEED_2B) {
			return E_HM_OVERFLOW;
		}
		dst[(*offset)++] = (char)(UTF8_3B_VALUE | (ch >> CONT_SHIFT2));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | ((ch >> CONT_SHIFT1) & UTF8_CH_MASK));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | (ch & UTF8_CH_MASK));
		return E_HM_OK;
	} else if (ch >= UTF8_4B_MIN && ch <= UTF16_MAX) {
		if (*offset >= len - UTF8_NEED_3B) {
			return E_HM_OVERFLOW;
		}
		dst[(*offset)++] = (char)(UTF8_4B_VALUE | (ch >> CONT_SHIFT3));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | ((ch >> CONT_SHIFT2) & UTF8_CH_MASK));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | ((ch >> CONT_SHIFT1) & UTF8_CH_MASK));
		dst[(*offset)++] = (char)(UTF8_CONT_VALUE | (ch & UTF8_CH_MASK));
		return E_HM_OK;
	}
	return E_HM_INVAL;
}

/* Convert UTF16 string to UTF8. */
int utf16_to_utf8(const uint16_t *src, char *dst, size_t len)
{
	size_t i = 0;
	size_t offset = 0;
	uint32_t ch;
	uint32_t ch1;
	int rc = E_HM_OK;

	if (src == NULL || len == 0 || dst == NULL) {
		return E_HM_INVAL;
	}

	while (src[i] != 0) {
		ch = (uint32_t)src[i];
		if ((src[i] & UTF8_1W_MASK) == UTF8_1W_PRE) {
			ch1 = (uint32_t)src[i + 1];
			if ((ch1 != 0) && (ch1 & UTF8_1W_MASK) == UTF8_2W_PRE) {
				ch = ((ch - UTF8_1W_PRE) << UTF_SHIFT) + (ch1 - UTF8_2W_PRE) + UTF8_4B_MIN;
				i += 2; // Here should be offset 2, because 1 has been added before.
			} else {
				break;
			}
		} else {
			ch = src[i];
			i++;
		}
		rc = utf16_decode_next(ch, dst, &offset, len - 1);
		if (rc != E_HM_OK) {
			break;
		}
	}
	dst[offset] = '\0';
	return rc;
}

/* str here must be valid number string */
static bool parse_sign(const char *_str, unsigned int *pos)
{
	const char *str = _str;
	const char *start = _str;
	bool negative = false;

	while (*str == ' ') {
		str++;
	}

	if (*str == '-') {
		negative = true;
		str++;
	} else if (*str == '+') {
		str++;
	}

	*pos += (unsigned int)(str - start);
	return negative;
}

/* str here must be valid number string */
static int parse_base(const char *_str, int _bs, unsigned int *pos)
{
	int bs = _bs;
	int base;
	const char *str = _str;
	const char *start = _str;

	if (*str == '0') {
		str++;
		base = 8; /* 8 begins with '0' */
		if ((*str == 'x') || (*str == 'X')) {
			str++;
			base = 16; /* 16 begins with '0x' */
		}
		if (bs == 0) {
			bs = base;
		}
	}

	if (bs == 0) {
		bs = 10; /* 10: defalut is DEC */
	}

	*pos += (unsigned int)(str - start);
	return bs;
}

static bool check_value(const char *_str, int bs, unsigned long long _ull)
{
	unsigned long long tmp;
	unsigned long long v;
	const char *start = _str;
	const char *str = _str;
	unsigned long long ull = _ull;

	/* bs must no be zero, is convert in parse_base */
	if (bs == 0) {
		return false;
	}

	while (*str != '\0') {
		str++;
	}

	/* cmp chars one by one from the back */
	while (str != start) {
		str--;
		if (*str >= 'a' && *str <= 'z') {
			v = (unsigned long long)(unsigned)(((int)*str - (int)'a') + 10); /* 10: base convert to DEC */
		} else if (*str >= 'A' && *str <= 'Z') {
			v = (unsigned long long)(unsigned)(((int)*str - (int)'A') + 10); /* 10: base convert to DEC */
		} else if (*str >= '0' && *str <= '9') {
			v = (unsigned long long)(unsigned)((int)*str - (int)'0');
		} else {
			return false;
		}

		/* get ull the last bit, and cmp with str */
		tmp = ull % (unsigned long long)(unsigned int)bs;
		if (tmp != v) {
			return false;
		}

		/* renew ull the last bit */
		ull = ull / (unsigned long long)(unsigned int)bs;
	}

	return true;
}

/* str here must be valid number string, it is safe to check its value */
static bool slow_check_strtol(const char *str, long val, int bs)
{
	unsigned long long ull;
	int base;
	bool negative = false;
	unsigned int pos = 0;

	negative = parse_sign(str, &pos);
	base = parse_base(str + pos, bs, &pos);
	if (negative) {
		ull = (val == LONG_MIN) ?
		      (unsigned long long)LONG_MAX + 1 : (unsigned long long)(unsigned long)(-val);
	} else {
		ull = (unsigned long long)(unsigned long)val;
	}

	return check_value(str + pos, base, ull);
}

/* str here must be valid number string, it is safe to check its value */
static bool slow_check_strtoull(const char *str, unsigned long long val, int bs)
{
	int base;
	bool negative = false;
	unsigned int pos = 0;

	negative = parse_sign(str, &pos);
	if (negative == true) {
		return false;
	}
	base = parse_base(str + pos, bs, &pos);
	return check_value(str + pos, base, val);
}

int fs_strtol(const char *str, long *val, int bs)
{
	char *ep = NULL;
	long l;
	int ret;

	ret = str_check(str);
	if (ret != E_HM_OK) {
		hm_debug("input str error %s\n", hmstrerror(ret));
		return ret;
	}

	l = strtol(str, &ep, bs);
	if (*ep != '\0') {
		return E_HM_INVAL;
	} else if (l == LONG_MAX &&
		   !slow_check_strtol(str, LONG_MAX, bs)) {
		return E_HM_RANGE;
	} else if (l == LONG_MIN &&
		   !slow_check_strtol(str, LONG_MIN, bs)) {
		return E_HM_RANGE;
	}
	*val = l;

	return E_HM_OK;
}

int fs_strtoi(const char *str, int *val, int bs)
{
	char *ep = NULL;
	long l;
	int ret;

	ret = str_check(str);
	if (ret != E_HM_OK) {
		hm_debug("strtoi input str error %s\n", hmstrerror(ret));
		return ret;
	}

	l = strtol(str, &ep, bs);
	if (*ep != '\0') {
		return E_HM_INVAL;
	} else if (l > INT_MAX || l < INT_MIN) {
		return E_HM_RANGE;
	} else if (l == INT_MAX &&
		   !slow_check_strtol(str, INT_MAX, bs)) {
		return E_HM_RANGE;
	} else if (l == INT_MIN &&
		   !slow_check_strtol(str, INT_MIN, bs)) {
		return E_HM_RANGE;
	}
	*val = (int)l;

	return E_HM_OK;
}

int fs_strtoui(const char *str, unsigned int *val, int bs)
{
	char *ep = NULL;
	unsigned long ul;
	int ret;

	ret = str_check(str);
	if (ret != E_HM_OK) {
		hm_debug("input str error %s\n", hmstrerror(ret));
		return ret;
	}

	ul = strtoul(str, &ep, bs);
	if (*ep != '\0') {
		return E_HM_INVAL;
	} else if (ul > UINT_MAX) {
		return E_HM_RANGE;
	} else if (ul == UINT_MAX &&
		   !slow_check_strtoull(str, (unsigned long long)UINT_MAX, bs)) {
		return E_HM_RANGE;
	}
	*val = (unsigned int)ul;

	return E_HM_OK;
}

int fs_strtoul(const char *str, unsigned long *val, int bs)
{
	char *ep = NULL;
	unsigned long ul;
	int ret;

	ret = str_check(str);
	if (ret != E_HM_OK) {
		hm_debug("strtoul input str error %s\n", hmstrerror(ret));
		return ret;
	}

	ul = strtoul(str, &ep, bs);
	if (*ep != '\0') {
		return E_HM_INVAL;
	} else if (ul == ULONG_MAX &&
		   !slow_check_strtoull(str, (unsigned long long)ULONG_MAX, bs)) {
		return E_HM_RANGE;
	}
	*val = ul;

	return E_HM_OK;
}

int fs_strtoull(const char *str, unsigned long long *val, int bs)
{
	char *ep = NULL;
	unsigned long long ull;
	int ret;

	ret = str_check(str);
	if (ret != E_HM_OK) {
		hm_debug("input str error %s\n", hmstrerror(ret));
		return ret;
	}

	ull = strtoull(str, &ep, bs);
	if (*ep != '\0') {
		return E_HM_INVAL;
	} else if (ull == ULLONG_MAX &&
		   !slow_check_strtoull(str, ULLONG_MAX, bs)) {
		return E_HM_RANGE;
	}
	*val = ull;

	return E_HM_OK;
}
