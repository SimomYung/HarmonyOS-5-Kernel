/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide structure and interfaces for reading kev
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 27 10:58:03 2020
 */

#ifndef ULIBS_LIBKEV_KEV_H
#define ULIBS_LIBKEV_KEV_H

#include "kev_types.h"
#include <hmasm/kern_syscall.h>
#include <hmkernel/capability.h>

struct kev_config {
	bool enable;
	/*
	 * 'buffer' is a bitmap vector, bit 0 for ring buffer 0,
	 * bit 1 for ring buffer 1, etc.
	 */
	uint32_t buffer;
	uint64_t paddr;
	uint64_t boot_offset;
	uint32_t size;
};

typedef int (*kev_iter_func_t)(const struct kev_s *kev, uintptr_t arg);
typedef int (*klog_dump_func_t)(const char *log, unsigned int len, uintptr_t dump_arg);

/*
 * read event configuration specified by 'type'. the event configuration readed
 * by this function include the index of ring buffer that contains the event
 * and the information whether the event is enabled in system.
 */
int kev_read_event_config(enum kev_type type, struct kev_config *config);

/* read the kev ringbuffer index specified by @kev_type */
int kev_buffer_index_of(unsigned int kev_type, unsigned int *buffer_index);

/* read the kev ringbuffer size specified by @kev_type */
int kev_buffer_size_of(unsigned int kev_type, unsigned int *buf_size);

/* read the kev ringbuffer index specified by @config */
int kev_buffer_config_index_of(unsigned int kev_type, struct kev_config *config,
			       unsigned int *buffer_index);

/* read the current write head of ring buffer specified by 'index' */
int kev_read_head_of_buffer(unsigned int index, uint64_t *head);

/*
 * read up to 'size' bytes from position 'head' in ring buffer
 * specified by 'index' directly to buffer 'buf'.
 * NOTE: if 'head' eqauls to zero, data copy will start from current
 * write head in ring buffer.
 */
ssize_t kev_read_data(unsigned int index, uint64_t head,
		      char *buf, size_t size);

/*
 * access each kev stored in buffer 'buf' in order of time and
 * pass kev as a parameter to the callback function 'action'.
 */
int kev_for_each(const char *buf, size_t size,
		 int (*action)(const struct kev_s *event, uintptr_t arg),
		 uintptr_t arg);

/* enable or disable the event tracing,
 * example: kev_set_enable(KEV_OUTPUT_DISABLE, 2, sched_switch, klog);
 */
int kev_set_enable(unsigned int enable, int num, ...);

int kev_set_filter(const char *event_name, const char *filter);
int kev_iter_last_data(unsigned int kev_type, size_t max_read_len,
		kev_iter_func_t it_func, uintptr_t arg, size_t *kev_data_len);
int kev_iter_current_data(unsigned int kev_type, size_t max_read_len,
		kev_iter_func_t it_func, uintptr_t arg, size_t *kev_data_len);
int kev_klog_format(const struct kev_klog_s *klog, char *buf, unsigned int buf_len);
int kev_iter_last_klog(unsigned long max_read_len, klog_dump_func_t dump_func, uintptr_t arg);
int kev_iter_current_klog(unsigned long max_read_len, klog_dump_func_t dump_func, uintptr_t arg);

int kev_iter_on_klog_data(const char *buffer, size_t buflen, klog_dump_func_t dump_func, uintptr_t arg);
int kev_read_klog_data(char *buffer, size_t buflen, ssize_t offset, size_t *ret_read_len);

#endif
