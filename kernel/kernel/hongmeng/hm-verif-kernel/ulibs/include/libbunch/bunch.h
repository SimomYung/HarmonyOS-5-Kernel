/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for bunch api
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 30 18:10:40 2019
 */
#ifndef ULIBS_LIBBUNCH_BUNCH_H
#define ULIBS_LIBBUNCH_BUNCH_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define BUNCH_SIZE_MAX (0x1U << 26) /* 64M */
#define BUNCH_DEF_SIZE (0x1U << 12) /* 4K */

#define	BUNCH_STATUS_OK		0x0U
	/* bunch invalid */
#define	BUNCH_STATUS_INVALID	0x01U
	/* no memory */
#define	BUNCH_STATUS_NOMEM	0x02U
	/* write overflow */
#define	BUNCH_STATUS_WOF	0x04U
	/* read overflow */
#define	BUNCH_STATUS_ROF	0x08U
	/* position overflow */
#define	BUNCH_STATUS_POF	0x10U

struct bunch {
	void *payload;
	size_t size;
	size_t position;
	void *private_data;
	unsigned int status;
};

/*
 * alloc memory by the way of customize function.
 * user use customize function to alloc and free memory,
 * this interface only install the allocated memory to bunch.
 */
void *bunch_payload_of(const struct bunch *bunch);
size_t bunch_size_of(const struct bunch *bunch);
int bunch_payload_install(struct bunch *bunch, void *payload, size_t size);

/*
 * alloc memory by the way of malloc and free.
 * these interfaces malloc, free and install the memory.
 */
int bunch_create(struct bunch *bunch, size_t size);
void bunch_destroy(struct bunch *bunch);

void bunch_write_byte(struct bunch *bunch, uint8_t val);
void bunch_write_int32(struct bunch *bunch, int32_t val);
void bunch_write_uint32(struct bunch *bunch, uint32_t val);
void bunch_write_int64(struct bunch *bunch, int64_t val);
void bunch_write_uint64(struct bunch *bunch, uint64_t val);
void bunch_write_uintptr(struct bunch *bunch, uintptr_t val);
void bunch_write_bool(struct bunch *bunch, bool val);
void bunch_write_string(struct bunch *bunch, const char *s);

uint8_t bunch_read_byte(struct bunch *bunch);
int32_t bunch_read_int32(struct bunch *bunch);
uint32_t bunch_read_uint32(struct bunch *bunch);
int64_t bunch_read_int64(struct bunch *bunch);
uint64_t bunch_read_uint64(struct bunch *bunch);
uintptr_t bunch_read_uintptr(struct bunch *bunch);
bool bunch_read_bool(struct bunch *bunch);
const char *bunch_read_string(struct bunch *bunch);

void bunch_override_byte(struct bunch *bunch, size_t position, uint8_t val);
void bunch_override_int32(struct bunch *bunch, size_t position, int32_t val);
void bunch_override_uint32(struct bunch *bunch, size_t position, uint32_t val);
void bunch_override_int64(struct bunch *bunch, size_t position, int64_t val);
void bunch_override_bool(struct bunch *bunch, size_t position, bool val);
void bunch_override_string(struct bunch *bunch, size_t position,
			   const char *s);
void bunch_override_add_int32(struct bunch *bunch, size_t position,
			      int32_t val);
void bunch_override_add_int64(struct bunch *bunch, size_t position,
			      int64_t val);

void bunch_seek(struct bunch *bunch, size_t position);
size_t bunch_get_position(const struct bunch *bunch);
size_t bunch_get_capacity(const struct bunch *bunch);
void bunch_set_private_data(struct bunch *bunch, void *private_data);
void *bunch_get_private_data(struct bunch *bunch);
void bunch_duplicate(struct bunch *bunch, const struct bunch *origin);
bool bunch_is_empty(const struct bunch *bunch);
bool bunch_is_healthy(const struct bunch *bunch);
unsigned int bunch_get_status(const struct bunch *bunch);

#endif
