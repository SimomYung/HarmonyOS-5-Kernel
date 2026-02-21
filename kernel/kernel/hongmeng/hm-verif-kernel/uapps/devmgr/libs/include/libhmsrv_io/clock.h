/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: I/O service APIs of clock operation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar  5 14:07:48 2020
 */
#ifndef ULIBS_LIBHMSRV_IO_CLOCK_H
#define ULIBS_LIBHMSRV_IO_CLOCK_H

#include <time.h>	/* for `struct timespec` */
#include <libhmsrv_io/type.h>

int hmsrv_io_clock_settime(const struct hmsrv_io_ctx *ctx,
			   const struct timespec *tp);

int hmsrv_io_clock_gettime(const struct hmsrv_io_ctx *ctx,
			   struct timespec *tp);

int hmsrv_io_clock_getres(const struct hmsrv_io_ctx *ctx,
			  struct timespec *res);

int hmsrv_io_clock_adjtime(const struct hmsrv_io_ctx *ctx,
			   const void *tx);

#endif	/* ULIBS_LIBHMSRV_IO_CLOCK_H */
