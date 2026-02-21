/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 21 10:06:04 2022
 */

#ifndef KLIBS_EKBOX_RB_H
#define KLIBS_EKBOX_RB_H

#include <hmkernel/types.h>

struct ekbox_head_s {
	u64 magic_num;
	u32 buf_size;
	u32 widx; /* widx points to writer index */
	bool full;
};

extern void ekbox_rb_init(struct ekbox_head_s *hd, u32 size);
extern void ekbox_rb_puts(struct ekbox_head_s *hd, const char *str, u32 str_len);
extern bool ekbox_rb_is_inited(const struct ekbox_head_s *hd, u32 size);

#endif
