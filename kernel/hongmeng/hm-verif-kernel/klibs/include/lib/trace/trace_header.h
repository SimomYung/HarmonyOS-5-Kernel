/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definitions of trace header
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 25 10:40:11 2023
 */

#ifndef KLIBS_TRACE_HEADER_H
#define KLIBS_TRACE_HEADER_H

#define HMCAP_THREAD_NAME_SIZE 16

/* this struct should synchronize with the struct trace_entry in ldk */
struct trace_header {
	unsigned short common_type;
	unsigned char common_flags;
	unsigned char common_preempt_count;
	int common_pid;
};

#endif
