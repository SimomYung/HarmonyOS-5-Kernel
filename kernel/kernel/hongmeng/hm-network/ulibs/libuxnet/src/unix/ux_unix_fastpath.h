/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: fastpath function headfile for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 10 10:21:00 2024
 */

#ifndef __UX_UNIX_FASTPATH_H__
#define __UX_UNIX_FASTPATH_H__

#define HILOG_PATH "/dev/unix/socket/hilogInput"

enum unix_fastpath_type {
	FP_INVAL = -1,
	FP_SHM_TYPE,
	FP_MAX
};

struct ux_unix_sock;
#ifdef CONFIG_UNIX_FASTPATH
void ux_unix_fastpath_init(ux_proto_ops_t *usk_ops);
void ux_unix_fastpath_disable(struct ux_unix_sock *usk);
void ux_unix_try_server_fastpath(struct ux_unix_sock *usk, char *path);
int ux_unix_try_client_fastpath(struct ux_unix_sock *usk, enum unix_fastpath_type fp_type);
#else
static inline void ux_unix_fastpath_init(ux_proto_ops_t *usk_ops)
{
	UNUSED(usk_ops);
	return;
}

static inline void ux_unix_fastpath_disable(struct ux_unix_sock *usk)
{
	UNUSED(usk);
	return;
}

static inline void ux_unix_try_server_fastpath(struct ux_unix_sock *usk, char *path)
{
	UNUSED(usk, path);
	return;
}

static inline int ux_unix_try_client_fastpath(struct ux_unix_sock *usk, enum unix_fastpath_type fp_type)
{
	UNUSED(usk, fp_type);
	return E_HM_OK;
}
#endif

#endif /* __UX_UNIX_FASTPATH_H__ */
