/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: base64 encoding and decoding
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 09:21:10 2022
 */
#include <libhmcrypt/hmbase64.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>

/* each four 6-bit binary numbers are combined into three 8-bit binary numbers */
#define BASE64_DECODE_NUM 4

static const char b64_tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char b64_rev_tbl[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
	-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};
static const char b64_padding = '=';


static int __value_of(char c)
{
	return b64_rev_tbl[(int)(unsigned char)c] == (char)-1 ? E_HM_FAULT : (int)b64_rev_tbl[(int)(unsigned char)c];
}

static void __decode(const char *src, uint8_t *dst, int cnt)
{
	/* base64 decode number count */
	switch (cnt) {
	case 4:
		/* decode the third byte */
		dst[2] = ((src[2] & 0x3) << 6U) + src[3];
		/* fallthrough */
	case 3:
		/* decode the second byte */
		dst[1] = ((src[1] & 0xf) << 4U) + ((src[2] & 0x3c) >> 2U);
		/* fallthrough */
	case 2:
		/* decode the frist byte */
		dst[0] = (src[0] << 2U) + ((src[1] & 0x30) >> 4U);
		break;
	default:
		hm_error("wrong cnt in base64 decode\n");
		break;
	}
}

static int __decode4(char data[BASE64_DECODE_NUM], uint8_t *dst, int *cnt)
{
	int i;
	int ret = E_HM_OK;

	/* the last byte is padding */
	if (data[3] == b64_padding) {
		(*cnt)--;
	}

	/* the third byte is padding */
	if (data[2] == b64_padding) {
		(*cnt)--;
	}

	for (i = 0; i < *cnt; i++) {
		if (__value_of(data[i]) == E_HM_FAULT) {
			ret = E_HM_FAULT;
			break;
		}
		data[i] = (char)__value_of(data[i]);
	}

	if (ret == E_HM_OK) {
		__decode(data, dst, *cnt);
	}

	return ret;
}

int base64_decode(const char *src, size_t len, uint8_t *dst, size_t *out_size)
{
	int ret = E_HM_OK;
	int cnt = 0;
	char data[BASE64_DECODE_NUM] = {0};
	bool fin = false;
	size_t j = 0;

	for (size_t i = 0; i < len; i++) {
		if (isspace(src[i])) {
			/* skip spaces and CRLF */
			continue;
		}

		if (fin) {
			/* already meet the padding, should not process further */
			ret = E_HM_FAULT;
			break;
		}

		data[cnt++] = src[i];
		/* each four 6-bit binary numbers are combined into three 8-bit binary numbers */
		if (cnt == BASE64_DECODE_NUM) {
			/* found one base64 digits group */
			if (__decode4(data, dst + j, &cnt) != E_HM_OK) {
				ret = E_HM_FAULT;
				break;
			}

			if (cnt < BASE64_DECODE_NUM) {
				fin = true;
			}

			/* for all three valid pattern aaaa aaa= aa==,
			   cnt -1 will always be the number of decoded bytes */
			j += (size_t)(unsigned int)(cnt - 1);
			cnt = 0;
		}
	}

	*out_size = j;
	if ((ret == E_HM_OK) && (cnt != 0)) {
		/* left some extra characters */
		ret = E_HM_FAULT;
		*out_size = 0;
	}

	return ret;
}

int base64_encode(const uint8_t *src, size_t len, char *dst, size_t *out_size)
{
	size_t s, i, j;
	uint8_t c;
	uint8_t l;
	*out_size = 0;

	s = 0;
	l = 0;
	for (i = j = 0; i < len; i++) {
		c = src[i];

		switch (s) {
		case 0:
			s = 1;
			dst[j++] = b64_tbl[(c >> 2) & 0x3F];
			break;
		case 1:
			s = 2;
			dst[j++] = b64_tbl[((l & 0x3) << 4) | ((c >> 4) & 0xF)];
			break;
		case 2:
			s = 0;
			dst[j++] = b64_tbl[((l & 0xF) << 2) | ((c >> 6) & 0x3)];
			dst[j++] = b64_tbl[c & 0x3F];
			break;
		default:
			/* never */
			break;
		}
		l = c;
	}

	if (s == 1) {
		dst[j++] = b64_tbl[(l & 0x3) << 4];
		dst[j++] = b64_padding;
		dst[j++] = b64_padding;
	} else if (s == 2) {
		dst[j++] = b64_tbl[(l & 0xF) << 2];
		dst[j++] = b64_padding;
	}

	dst[j] = 0;
	*out_size = j;

	return E_HM_OK;
}
