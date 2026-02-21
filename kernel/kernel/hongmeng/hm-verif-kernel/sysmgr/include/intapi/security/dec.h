/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: dec ctrl header
 * Author: Huawei
 * Create: Wen Mar 17 09:43:10 2024
*/

#ifndef SYSMGR_INTAPI_DEC_H
#define SYSMGR_INTAPI_DEC_H
#include <stdint.h>
/* MODE FOR ENABLE OR DISABLE */
#define DEC_MODE_OFF		0
#define DEC_MODE_ON		1
/* MODE FOR FILE OPERATION */
#define DEC_MODE_READ 1
#define DEC_MODE_WRITE 2

struct hmdec_audit_context {
	const char *path;
	uint32_t mode;
};

struct hmdec_audit_info {
	char *buf;
	size_t len;
};

/* update audit info buffer */
static inline void dec_ai_update(int len, struct hmdec_audit_info *ai)
{
	size_t size;
	if (len > 0 && (size_t)len <= ai->len) {
		ai->buf = (char *)ai->buf + (size_t)len;
		ai->len -= (size_t)len;  
	}
}

int hmdec_avc_check(struct hmdec_audit_context *con);

int dec_mode_of(void);

void dec_mode_enable(void);

void dec_mode_disable(void);

#endif
