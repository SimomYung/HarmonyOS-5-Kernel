/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm std ringbuf
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_STD_RINGBUF_H
#define ULIBS_LIBHMPSF_XVM_STD_RINGBUF_H

#include <libhmpsf/xvm/std/map/map.h>
#include <xvm/xvm_types/std/map/ringbuf.h>

/*
 * Create globally-shared ringbuf. If an existent one is found, it will be reused.
 * If successful, handle of allocated metadata of the map will be returned through `map`.
 */
int xvm_ringbuf_create_global_shared(xvm_map_handle_t *map, const char *name, unsigned short participants,
				     unsigned int capacity);

/*
 * Consume data in the ringbuf, and copy it to @data.
 */
int xvm_ringbuf_consume(xvm_map_handle_t map, unsigned char *data, unsigned int *data_size);

#endif
