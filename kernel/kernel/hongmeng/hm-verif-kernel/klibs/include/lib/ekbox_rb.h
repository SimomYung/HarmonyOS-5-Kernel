/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 21 10:06:04 2022
 */

#ifndef KLIBS_EKBOX_RB_H
#define KLIBS_EKBOX_RB_H

#include <hmkernel/types.h>

struct ekbox_head_s {
	__u64 magic_num;
	__u32 buf_size;
	__u32 widx; /* widx points to writer index */
	__bool full;
};

typedef int (*ekbox_read_func_t)(const char *data, unsigned int len, __uptr_t read_arg);

extern void ekbox_rb_init(struct ekbox_head_s *hd, __u32 size);
extern void ekbox_rb_puts(struct ekbox_head_s *hd, const char *str, __u32 str_len);
extern __bool ekbox_rb_is_inited(const struct ekbox_head_s *hd, __u32 size);
extern int ekbox_rb_read(const struct ekbox_head_s *hd, __u32 ekbox_size,
		ekbox_read_func_t read_func, __uptr_t read_arg);

#endif
