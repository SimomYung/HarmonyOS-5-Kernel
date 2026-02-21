/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of jump_label
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 08 15:40:16 2023
 */
#ifndef MAPI_HMKERNEL_JUMP_LABEL_H
#define MAPI_HMKERNEL_JUMP_LABEL_H
#include <hmkernel/strict.h>
#include <hmkernel/errno.h>
#include <uapi/hmkernel/jump_label.h>

#ifdef CONFIG_JUMP_LABEL
int static_key_inc(struct static_key *key);
int static_key_dec(struct static_key *key);
#else
static inline int static_key_inc(struct static_key *key)
{
	BUG_ON((int)vatomic32_read(&key->enabled) < 0);
	vatomic32_inc(&key->enabled);
	return E_HM_OK;
}

static inline int static_key_dec(struct static_key *key)
{
	BUG_ON((int)vatomic32_read(&key->enabled) <= 0);
	vatomic32_dec(&key->enabled);
	return E_HM_OK;
}
#endif /* CONFIG_JUMP_LABEL */
#endif /* MAPI_HMKERNEL_JUMP_LABEL_H */
