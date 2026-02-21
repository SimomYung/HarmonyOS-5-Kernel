/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide structure and interfaces for reading event
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 27 10:58:03 2020
 */

#ifndef ULIBS_LIBHMPROBE_H
#define ULIBS_LIBHMPROBE_H

enum hmprobe_event_type {
	HMPROBE_KEV_TYPE = 0,
	HMPROBE_UEV_TYPE = 1,
	HMPROBE_UPROBE_TYPE = 2,
	HMPROBE_RET_UPROBE_TYPE = 3,
};

struct hmprobe_event_attr_s {
	enum hmprobe_event_type type;
	union {
		struct {
			const char *file_path;
			const char *symbol_name;
			unsigned long file_offset;
			unsigned int pid;
		} uprobe;
		struct {
			const char *tp_name;
		} trace_point;
	} event_descriptor;
};

int hmprobe_open(struct hmprobe_event_attr_s *attr, unsigned int *event_id);
int hmprobe_close(unsigned int event_id);
int hmprobe_attach(unsigned int module_id, const char *func_name, unsigned int event_id, unsigned int *prog_fd);
int hmprobe_detach(unsigned int prog_fd);
#endif
