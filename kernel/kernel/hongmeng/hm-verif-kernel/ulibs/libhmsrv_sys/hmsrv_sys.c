/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of sysif server hmsrv sys
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 08:11:52 2019
 */
#include <unistd.h>
#include <string.h>

#include <hmkernel/module_ops.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/syscall.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hmsrv_sys.h>

#define LIBHMSRV_SYS_VERSION_MAJOR	1
#define LIBHMSRV_SYS_VERSION_MINOR	0
#define LIBHMSRV_SYS_VERSION_PATCH	0
#define LIBHMSRV_SYS_VERSION_BUILD	0

#define SCHED_CLASS_NAME "sched_class"

static __u32 sysmgr_cnode_idx;
static rref_t module_sched_rref = 0;

void hmsrv_set_sysmgr_cnode_idx(__u32 cnode_idx)
{
	sysmgr_cnode_idx = cnode_idx;
}

__u32 hmsrv_sysmgr_cnode_idx(void)
{
	return sysmgr_cnode_idx;
}

rref_t hmsrv_module_sched_rref_of(void)
{
	return module_sched_rref;
}

void hmsrv_update_sched_class_module_rref(void)
{
	int err = 0;
	int ret = 0;
	rref_t module_rref = 0;
	rref_t __module_rref = 0;
	char module_name[CAP_MODULE_NAME_LEN] = {0};

	hm_ucap_for_each_rref(0, hmobj_Module, __module_rref, err) {
		if (err == E_HM_AGAIN) {
			continue;
		}
		ret = syscap_ModuleInfo(__module_rref, module_name, CAP_MODULE_NAME_LEN);
		if (ret < 0) {
			hm_error("syscal_ModuleInfo failed:%s\n", hmstrerror(ret));
			break;
		}
		if (strncmp(module_name, SCHED_CLASS_NAME,
			    sizeof(SCHED_CLASS_NAME) - (size_t)1) == 0) {
			module_rref = __module_rref;
			break;
		}
	}

	if (ret >= 0) {
		if (module_rref == 0ULL) {
			hm_panic("no module found, err:%s\n", hmstrerror(err));
		}
		module_sched_rref = module_rref;
	}
}

void hmsrv_sys_version_get(struct hmsrv_sys_version *version)
{
	if (version != NULL) {
		version->major = (uint16_t)LIBHMSRV_SYS_VERSION_MAJOR;
		version->minor = (uint16_t)LIBHMSRV_SYS_VERSION_MINOR;
		version->patch = (uint16_t)LIBHMSRV_SYS_VERSION_PATCH;
		version->build = (uint16_t)LIBHMSRV_SYS_VERSION_BUILD;
	}
}

int hmsrv_sys_version_check(const struct hmsrv_sys_version *expected)
{
	int rc;

	if (expected == NULL) {
		rc = E_HM_INVAL;
	} else if (expected->major > (uint16_t)LIBHMSRV_SYS_VERSION_MAJOR) {
		rc = E_HM_NOSYS;
	} else if (expected->minor > (uint16_t)LIBHMSRV_SYS_VERSION_MINOR) {
		rc = E_HM_NOSYS;
	} else {
		rc = E_HM_OK;
	}

	return rc;
}
