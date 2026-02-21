/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Provide interfaces for kcov
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 28 17:44:03 2023-2024
 */

#ifndef ULIBS_LIBKCOV_H
#define ULIBS_LIBKCOV_H

#include <sys/ioctl.h>
#include <stddef.h>
#include <hmkernel/kcov.h>

struct kcov_remote_arg {
	unsigned int           trace_mode;     /* KCOV_TRACE_PC or KCOV_TRACE_CMP */
	unsigned int           area_size;      /* Length of coverage buffer in words */
	unsigned int           num_handles;    /* Size of handles array */
	unsigned long   common_handle;
	unsigned long   handles[0];
};

#define KCOV_REMOTE_MAX_HANDLES	0x100

#define KCOV_INIT_TRACE		_IOR('c', 1, unsigned long)
#define KCOV_ENABLE		_IO('c', 100)
#define KCOV_DISABLE		_IO('c', 101)
#define KCOV_REMOTE_ENABLE	_IOW('c', 102, struct kcov_remote_arg)

enum {
	KCOV_TRACE_PC = 0,
	KCOV_TRACE_CMP = 1,
};

#define MAX_KCOV_THREAD_COUNT ((int)(sizeof(unsigned int) * 8))
#define KCOV_CMP_CONST          (1U << 0)
#define KCOV_CMP_SIZE(n)        ((n) << 1)

struct kcov_service_data {
	uintptr_t manage_data_vaddr;
	uintptr_t data_vaddr;
	int sidx;
};

struct kcov_manager_data_s {
	size_t size;
};

struct kcov_data_s {
	unsigned long data[0];
};

void __sanitizer_cov_trace_pc(void);
void __sanitizer_cov_trace_cmp1(__u8 arg1, __u8 arg2);
void __sanitizer_cov_trace_cmp2(__u16 arg1, __u16 arg2);
void __sanitizer_cov_trace_cmp4(__u32 arg1, __u32 arg2);
void __sanitizer_cov_trace_cmp8(__u64 arg1, __u64 arg2);
void __sanitizer_cov_trace_const_cmp1(__u8 arg1, __u8 arg2);
void __sanitizer_cov_trace_const_cmp2(__u16 arg1, __u16 arg2);
void __sanitizer_cov_trace_const_cmp4(__u32 arg1, __u32 arg2);
void __sanitizer_cov_trace_const_cmp8(__u64 arg1, __u64 arg2);
void __sanitizer_cov_trace_switch(__u64 val, __u64 *cases);

void kcov_init_finished(void);

#endif
