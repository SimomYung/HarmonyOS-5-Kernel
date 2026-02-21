/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */
#ifndef KLIBS_SYSEVENT_STRUCT_H
#define KLIBS_SYSEVENT_STRUCT_H

#define MAX_DOMAIN_LENGTH	16
#define MAX_EVENT_NAME_LENGTH	32

enum encode_type {
	VARINT = 0,
	LENGTH_DELIMITED = 1,
};

#pragma pack(1)
struct param_value_type {
	__u8 is_array: 1;
	__u8 value_type: 4;
	__u8 value_byte_cnt: 3;
};

struct sysevent_header {
	char domain[MAX_DOMAIN_LENGTH + 1];
	char name[MAX_EVENT_NAME_LENGTH + 1];
	__u64 timestamp;
	__u8 time_zone;
	__u32 uid;
	__u32 pid;
	__u32 tid;
	__u64 id;
	__u8 type: 2;
	__u8 is_open_trace: 1;
};
#pragma pack()

struct raw_data {
	char *buf;
	size_t capacity;
	size_t pos;
};

#endif
