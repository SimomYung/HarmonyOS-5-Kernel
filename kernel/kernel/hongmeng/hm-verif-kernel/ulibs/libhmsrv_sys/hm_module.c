/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define cap_modlue interface for user
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 10:32:19 2019
 */

#include <hmkernel/const.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>

#include <libhmucap/ucap.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_sys/hm_module.h>

/* all modules are in sysmgr */
static uint32_t sysmgr_idx = 0;

static int do_sched_module_require(rref_t *rref)
{
	int ret = E_HM_OK;
	struct __actvret_hmcall_module_sched_require hmcall_ret;

	mem_zero_s(hmcall_ret);
	ret = actvcall_hmcall_module_sched_require(&hmcall_ret);
	if (ret != E_HM_OK) {
		hm_error("actvcall_hmcall_module_sched_require failed, err=%s\n", hmstrerror(ret));
	} else {
		*rref = hmcall_ret.rref;
	}

	return ret;
}

rref_t hm_sched_module_rref_of(void)
{
	int err = E_HM_OK;
	rref_t rref = 0ULL;

	err = do_sched_module_require(&rref);
	if (err != E_HM_OK) {
		hm_error("do_sched_module_require failed\n");
		rref = ERR_TO_REF(err);
	}

	return rref;
}

static void hm_module_init_idx(void)
{
	/* Sysmgr should not rely on this func */
	sysmgr_idx = hmsrv_sysmgr_cnode_idx();
}

rref_t hm_module_open(const char *module_name)
{
	int err = E_HM_OK;
	rref_t module_rref = (rref_t)0;
	struct module_grant_arg arg;

	if (module_name == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK && sysmgr_idx == 0U) {
		hm_module_init_idx();
	}

	if (err == E_HM_OK) {
		module_rref = hm_ucap_require(0, sysmgr_idx, 0);
		if (IS_REF_ERR(module_rref)) {
			err = REF_TO_ERR(module_rref);
		}
	}
	if (err == E_HM_OK) {
		err = strncpy_s(arg.mod_name, sizeof(arg.mod_name), module_name,
				strlen(module_name));
		if (err != 0) {
			hm_error("strncpy_s failed.\n");
			(void)hm_ucap_reject(0, module_rref);
			err = E_HM_RESFAULT;
		}
	}
	if (err == E_HM_OK) {
		arg.mod_rref = module_rref;
		/* call modulemgr for grant */
		err = actvcall_hmcall_module_grant_ctrl(arg);
		if (err < 0) {
			/* impossilbe to failed */
			(void)hm_ucap_reject(0, module_rref);
		}
	}

	return (err < 0) ? ERR_TO_REF(err) : module_rref;
}

int hm_module_close(rref_t module_rref)
{
	int err;

	err = hm_ucap_reject(0, module_rref);
	return (err < 0) ? err : 0;
}

/* Cap_Module use cap_ioctl interface, so we change the ulibs interface name */
int hm_module_ioctl(rref_t module_rref, uint32_t cmd,
		    void *cfg, size_t cfg_len,
		    void *rlt, size_t rlt_len)
{
	return syscap_ModuleIoctl(module_rref, cmd,
				  cfg, cfg_len, rlt, rlt_len);
}

int hm_module_bind_modif(rref_t module_rref)
{
	return syscap_ModuleBindModif(module_rref);
}

int hm_module_unbind_modif(rref_t module_rref)
{
	return syscap_ModuleUnbindModif(module_rref);
}
