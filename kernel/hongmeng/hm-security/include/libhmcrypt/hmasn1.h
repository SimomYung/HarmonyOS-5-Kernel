/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: asn.1 decoding
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 19:03:27 2022
 */
#ifndef ULIBS_HMASN1_H
#define ULIBS_HMASN1_H
#include <stdint.h>

#define ASN1_TAG_SEQUENCE		0x30
#define ASN1_TAG_BIT_STRING		0x03

struct asn1_data {
	uint8_t tag;
	uint32_t size;
	uint8_t *data;
};

/* parse one object from der buffer */
int asn1_parse(uint8_t *data, uint32_t size, struct asn1_data *d);

/* parse next child asn.1 item, if the first child c is NULL */
int asn1_next_child(struct asn1_data *d, struct asn1_data *c, struct asn1_data *n);

#endif
