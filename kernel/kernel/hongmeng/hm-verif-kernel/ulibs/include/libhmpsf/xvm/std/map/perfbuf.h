/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: libhmpsf xvm std perf event buffer
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 10:56:27 2023
 */

#ifndef ULIBS_LIBHMPSF_XVM_STD_PERFBUF_H
#define ULIBS_LIBHMPSF_XVM_STD_PERFBUF_H

#include <libhmpsf/xvm/std/map/map.h>
#include <xvm/xvm_types/std/map/perfbuf.h>

typedef void (*xvm_perfbuf_sample_fn_t)(void *ctx, unsigned int cpu, void *data, unsigned int size);

struct xvm_perfbuf_params_s {
	xvm_perfbuf_sample_fn_t sample_cb;
	void *ctx; /* passed into callbacks */
	unsigned int buf_size; /* must be multiple of PAGE_SIZE */
	unsigned int nr_cpus; /* number of cpus calculated base on "/sys/devices/system/cpu/possible" */
	unsigned int wakeup_watermark; /* bytes before wakeup, 0 means half of buf_size */
};

struct perfbuf_cpu_buf_s {
	void *base; /* userspace mmap address */
	int fd;
};

struct perfbuf_s {
	xvm_map_t base;
	xvm_perfbuf_sample_fn_t sample_cb;
	void *ctx;

	unsigned int page_size;
	unsigned int buf_size;
	struct perfbuf_cpu_buf_s **cpu_bufs;
	struct pollfd *pollfds;
	void *cat_buf;
	unsigned int nr_cpus;
};

/*
 * Create globally-shared perf event buffer. If an existent one is found, it will be reused.
 * If successful, handle of allocated metadata of the map will be returned through `map`.
 */
int xvm_perfbuf_create(xvm_map_handle_t *map, const char *name, struct xvm_perfbuf_params_s *params);


/*
 * Accessors.
 */
int xvm_perfbuf_poll(xvm_map_handle_t map, int timeout);
void xvm_perfbuf_free(xvm_map_handle_t map);

#endif
