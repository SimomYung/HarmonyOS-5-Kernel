/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Struct of kconsole
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 15:31:43 2019
 */

#ifndef HMKERNEL_KLOGPORT_H
#define HMKERNEL_KLOGPORT_H

#include <lib/dlist.h>
#include <hmkernel/errno.h>
#include <hmkernel/klogport_type_flag.h>

struct klogport_s {
	struct dlist_node list;
	unsigned long type_flag;
	unsigned int level;
	void (*puts)(const char *content, __kernel_size_t content_length,
		     const char *prefix, __kernel_size_t prefix_length);
	void (*puts_nolock)(const char *content, __kernel_size_t content_length,
			    const char *prefix, __kernel_size_t prefix_length);
};

void klogport_register(struct klogport_s *klogport);
void klogport_unregister(struct klogport_s *klogport);
void klogport_unregister_all(void);
void klogport_puts(unsigned int level,
		   const char *content, size_t content_length,
		   const char *prefix, size_t prefix_length);
void klogport_puts_nolock(unsigned int level,
			  const char *content, size_t content_length,
			  const char *prefix, size_t prefix_length);
int klogport_set_level(unsigned long type_flags, unsigned int level);
unsigned int klogport_min_level(void);

#ifdef __HOST_LLT__
int klogport_counts(void);
#endif

#endif
