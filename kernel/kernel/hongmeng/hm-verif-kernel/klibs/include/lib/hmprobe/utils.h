/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide hmprobe basic util structure
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 15:29:10 2023
 */

#ifndef HMPROBE_UTILS_H
#define HMPROBE_UTILS_H

#define HMPROBE_EVENT_NAME_MAX_SIZE 64
#define HMPROBE_EVENT_MAX_ATTACH_TIMES 50
#define HMPROBE_FUNC_NAME_MAX_SIZE 64
#define HMPROBE_LOCK_SIZE 64

#define HMPROBE_FILE_PATH_MAX_SIZE 128
#define HMPROBE_SYMBOL_NAME_MAX_SIZE 256

enum hmprobe_cmd {
	HMPROBE_EVENT_OPEN,
	HMPROBE_EVENT_CLOSE,
	HMPROBE_EVENT_ATTACH,
	HMPROBE_EVENT_DETACH,
};

typedef enum hmprobe_event_host_type {
	HMPROBE_TRACEPOINT_EVENT = 0,
	HMPROBE_PROBE_EVENT = 1,
	NR_HMPROBE_EVENT_TYPE
} hmprobe_event_type_t;

struct hmprobe_prog_attr_s {
	unsigned int module_id;
	const char *func_name;
	unsigned long name_len;
	unsigned int prog_fd;
};

struct hmprobe_module_attach_attr_s {
	unsigned int module_id;
	unsigned int event_id;
	char func_name[HMPROBE_FUNC_NAME_MAX_SIZE];
};

struct hmprobe_module_pre_alloc_pfn_s {
	unsigned long long module_pfn;
};

struct hmprobe_event_pre_alloc_pfn_s {
	unsigned long long event_pfn;
	unsigned long long runtime_pfn;
};

struct hmprobe_uprobe_point_s {
	char file_path[HMPROBE_FILE_PATH_MAX_SIZE];
	char symbol_name[HMPROBE_SYMBOL_NAME_MAX_SIZE];
	unsigned long file_offset;
	unsigned int pid;
	unsigned char is_ret;
};

struct hmprobe_kev_point_s {
	char kev_name[HMPROBE_EVENT_NAME_MAX_SIZE];
};

struct hmprobe_uev_point_s {
	char uev_name[HMPROBE_EVENT_NAME_MAX_SIZE];
};

union hmprobe_event_point_s {
	struct hmprobe_uprobe_point_s uprobe_point;
	struct hmprobe_kev_point_s kev_point;
	struct hmprobe_uev_point_s uev_point;
};

struct hmprobe_event_host_attr_s {
	int vspaceid;
	hmprobe_event_type_t event_type;
	union hmprobe_event_point_s ev_point;
	unsigned int pid;
};

#endif
