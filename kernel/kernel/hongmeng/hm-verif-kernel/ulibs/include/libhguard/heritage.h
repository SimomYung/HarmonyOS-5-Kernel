/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Init data sharing interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 4 18:48:37 2022
 */

#ifndef UAPP_INIT_HERITAGE_H
#define UAPP_INIT_HERITAGE_H

#include <stdint.h>

#define PACKED_SYSTEM_SERVICE_NAME_MAX 255

struct packed_system_service_s {
	int32_t pid;
	char name[PACKED_SYSTEM_SERVICE_NAME_MAX];
};

struct init_heritage_s {
	uint32_t size;
	uint32_t payload_cnt;
	struct packed_system_service_s payload[];
};

void init_heritage_sender_open(uint32_t size, int32_t *out_id, void **out_addr);
void init_heritage_sender_close(uint32_t size, int32_t id, void *addr);
void init_heritage_receiver_open(uint32_t *out_size, int32_t *out_id, void **out_addr);
void init_heritage_receiver_close(uint32_t size, int32_t id, void *addr);

#endif
