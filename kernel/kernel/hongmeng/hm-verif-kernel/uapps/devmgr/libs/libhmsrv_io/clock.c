/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: I/O service library of clock operation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar  5 14:07:48 2020
 */
#include <hongmeng/types.h>
#include <libsysif/devhost/api.h>
#include <libhmsrv_io/clock.h>

int hmsrv_io_clock_settime(const struct hmsrv_io_ctx *ctx,
			   const struct timespec *tp)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_clock_settime(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ptr_to_ulong(tp), ctx->filp);
}

int hmsrv_io_clock_gettime(const struct hmsrv_io_ctx *ctx,
			   struct timespec *tp)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_clock_gettime(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ptr_to_ulong(tp), ctx->filp);
}

int hmsrv_io_clock_getres(const struct hmsrv_io_ctx *ctx,
			  struct timespec *res)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_clock_getres(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ptr_to_ulong(res), ctx->filp);
}

int hmsrv_io_clock_adjtime(const struct hmsrv_io_ctx *ctx,
			   const void *tx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	return actvxactcapcall_drvcall_devhost_clock_adjtime(
			(ctx->flags & HMSRV_IO_CTX_FLAGS_CLOCK_FORWARD) != 0,
			true,	/* allow refwd */
			ctx->dh_ap_rref, ptr_to_ulong(tx), ctx->filp);
}
