/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: asn.1 decoding
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 19:05:42 2022
 */
#include "libhmcrypt/hmasn1.h"

#include <stdlib.h>
#include <hongmeng/errno.h>

#define ASN1_HEADER_SHORT	2U
#define BITS_PER_BYTE		8U

static uint32_t bytes_to_int(uint8_t *data, int cnt)
{
	uint32_t len = 0;

	for (int i = 0; i < cnt; i++) {
		len <<= BITS_PER_BYTE;
		len += data[i];
	}

	return len;
}

int asn1_parse(uint8_t *data, uint32_t size, struct asn1_data *d)
{
	uint32_t len = 0;
	uint8_t ext_len = 0;

	if (size < ASN1_HEADER_SHORT) {
		return E_HM_POSIX_FAULT;
	}

	len = data[1];
	/* parse length byte, 0x80 is top bit(sign bit) of first byte */
	if ((len & 0x80) != 0U) {
		ext_len = len & 0xf;
		if (size < ext_len + ASN1_HEADER_SHORT) {
			return E_HM_POSIX_FAULT;
		}
		len = bytes_to_int(data + ASN1_HEADER_SHORT, ext_len);
	}

	d->tag = data[0];
	d->size = len;
	d->data = data + ASN1_HEADER_SHORT + ext_len;

	if (d->tag == ASN1_TAG_BIT_STRING) {
		if (d->size < 1 || d->data[0] != 0) {
			/* only support bitstring with byte aligned */
			return E_HM_POSIX_FAULT;
		}
		d->data++;
		d->size--;
	}

	return E_HM_OK;
}

int asn1_next_child(struct asn1_data *d, struct asn1_data *c, struct asn1_data *n)
{
	uint8_t *data = NULL;
	uint32_t size = 0U;

	if (c == NULL) {
		data = d->data;
		size = d->size;
	} else {
		data = c->data + c->size;
		size = (uint32_t)(d->data + d->size - data);
	}

	return asn1_parse(data, size, n);
}
