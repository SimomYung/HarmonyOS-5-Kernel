/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: devmgr mgmt helper APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 16 16:46:07 2021
 */
#ifndef ULIBS_LIBHMSRV_IO_MGMT_H
#define ULIBS_LIBHMSRV_IO_MGMT_H

#include <libsysif/devmgr/api.h>

/* helpers to build mgmt target */
#define DM_MGMT_TARGET(t, f, d) ({		\
	struct devmgr_mgmt_target tgt = {	\
		.type = (t),			\
		.f = (d) };			\
	tgt; })
#define DM_MGMT_DEVID_TARGET(id)  \
		DM_MGMT_TARGET(DM_MGMT_TARGET_DEVID, devid, id)
#define DM_MGMT_CIDX_TARGET(cidx) \
		DM_MGMT_TARGET(DM_MGMT_TARGET_CNODEIDX, cnode_idx, cidx)
#define DM_MGMT_ROOT_TARGET DM_MGMT_DEVID_TARGET(0)

int hmsrv_io_mgmt_load_external_module(struct devmgr_mgmt_target target,
				       const char *pathname, const char *args);

/* mgmt APIs wrapper */
int hmsrv_io_mgmt_load_module(struct devmgr_mgmt_target target,
			      const char *modname, const char *args);

int hmsrv_io_mgmt_unload_module(struct devmgr_mgmt_target target,
				const char *modname);

int hmsrv_io_mgmt_sync(void);

typedef int (*hmsrv_io_mgmt_dump_callback_fn)(void *ctx, const char *data);
int hmsrv_io_mgmt_dump(const hmsrv_io_mgmt_dump_callback_fn fn, void *ctx);

#endif /* ULIBS_LIBHMSRV_IO_MGMT_H */
