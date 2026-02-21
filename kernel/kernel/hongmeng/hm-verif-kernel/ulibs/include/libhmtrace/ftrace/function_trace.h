/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of function trace API
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 24 19:54:44 2019
 */
#ifndef ULIBS_LIBHMTRACE_FTRACE_FUNC_TRACE_H
#define ULIBS_LIBHMTRACE_FTRACE_FUNC_TRACE_H
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <time.h>

#define FTRACE_STUB_SITE_SECTION	".ftrace_stub_site"

struct ftrace_stub_section {
	uint64_t stub_site_num;
	uintptr_t *stub_site_vaddr;
	uint32_t *stub_site_insn;
};

struct function_trace_record {
	uintptr_t ip;
	uintptr_t parent_ip;
	struct timespec time;
};

struct rb* hm_ftrace_ring_buffer_of(void);
int hm_ftrace_init(void);
int hm_ftrace_lookup_symbol(const struct function_trace_record *record,
			    char* buf, size_t buf_size);
int hm_ftrace_parse_elf_file(pid_t pid);
void hm_ftrace_release_elf_context(void);
void hm_ftrace_release_sym_desc(void);
#endif
