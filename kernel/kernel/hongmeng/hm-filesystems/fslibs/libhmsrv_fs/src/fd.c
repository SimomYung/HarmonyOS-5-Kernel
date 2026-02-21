/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of fd
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 19 15:43:05 2019
 */

#include <fd.h>

#include <libhmsync/bitops/generic.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <hmnet/hm_socket.h>
#include <hmsysmgr/process/pid.h>
#include <hongmeng/errno.h>
#include <unistd.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhmsync/raw_rwlock.h>
#include <libfdtable/fdtable.h>
#include <vfs.h>
#include <vfs_epoll.h>
#include <fs_server.h>
#include <fslib_common.h>

#include "internal.h"

static const unsigned int spawn_clone_flags = EXEC_CLONE_FS;

unsigned int hm_spawn_clone_flags(void)
{
	return spawn_clone_flags;
}

int hm_get_fd_type(int fd)
{
	rref_t fsinfo_rref = FS_INVALID_CNODE_RREF;
	int err = E_HM_OK;

	err = hm_fdtable_require_grant(hm_ucap_self_cnode_idx(), &fsinfo_rref);
	if (err == E_HM_OK) {
		err = syscap_FDTableGetFdType(fsinfo_rref, (unsigned int)fd);
		(void)hm_ucap_reject(0, fsinfo_rref);
	}

	return err;
}
