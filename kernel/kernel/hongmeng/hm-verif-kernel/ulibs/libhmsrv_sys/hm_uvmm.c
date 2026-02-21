/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: API for hm-uvmm
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 02 16:54:32 2021
 */
#include <libhmsrv_sys/hm_uvmm.h>

#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_vm.h>
#include <libhwsecurec/securec.h>
#include <libmem/utils.h>
#include <libsysif/uvmm/api.h>

#include <string.h>

#define UVMM_PATH_LEN_MAX 64

static const struct uvmm_param_s uvmm_gicpt = {
	.vm_type = VM_GICPT,
};

#ifndef CONFIG_VM_MODE_UM
rref_t hm_uvmm_rref_acquire(const struct uvmm_param_s *uvmm)
{
	rref_t uvmm_rref = ERR_TO_REF(E_HM_INVAL);

	if (uvmm != NULL) {
		char uvmm_path[UVMM_PATH_LEN_MAX];
		int rc = hm_vm_get_name(uvmm_path, UVMM_PATH_LEN_MAX, uvmm->vm_type);
		if (rc <= 0) {
			hm_debug("get uvmm path failed, type=%d\n", uvmm->vm_type);
			uvmm_rref = ERR_TO_REF(E_HM_INVAL);
		} else {
			rc = hm_path_acquire(uvmm_path, &uvmm_rref);
			if (rc != E_HM_OK) {
				hm_debug("acquire uvmm_path failed with err=%s\n", hmstrerror(rc));
				uvmm_rref = ERR_TO_REF(rc);
			}
		}
	}
	return uvmm_rref;
}

void hm_uvmm_rref_release(const struct uvmm_param_s *uvmm, rref_t uvmm_rref)
{
	if (uvmm != NULL) {
		char uvmm_path[UVMM_PATH_LEN_MAX];
		int rc = hm_vm_get_name(uvmm_path, UVMM_PATH_LEN_MAX, uvmm->vm_type);
		if (rc <= 0) {
			hm_warn("get uvmm path failed, type=%d\n", uvmm->vm_type);
		} else {
			rc = hm_path_release(uvmm_path, uvmm_rref);
			if (rc != E_HM_OK) {
				hm_warn("release uvmm_path failed, err=%s\n", hmstrerror(rc));
			}
		}
	}
}
#else
rref_t hm_uvmm_rref_acquire(const struct uvmm_param_s *uvmm)
{
	UNUSED(uvmm);
	return (rref_t)(1ULL << 32U);
}
void hm_uvmm_rref_release(const struct uvmm_param_s *uvmm, rref_t uvmm_rref)
{
	UNUSED(uvmm, uvmm_rref);
}
#endif

/**
 * hm_uvmm_mclone_to_vm - clone contiguous pages to the VM
 * @src_va: The caller uses the virtual address to identify the start
 *          of the contiguous physical pages to be mapped.
 * @tgt_gpa: The guest physical address to be mapped.
 * @len: The length of the mapping.
 * @prot: The read, write or execute accesses are permitted to the
 *        data being mapped.
 */
int hm_uvmm_mclone_to_vm(uint64_t src_va, uint64_t tgt_gpa,
			 uint64_t len, uint32_t prot)
{
	rref_t rref = 0ULL;
	int rc = E_HM_OK;

	if (!PAGE_ALIGNED(src_va) || !PAGE_ALIGNED(tgt_gpa) || !PAGE_ALIGNED(len)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			rc = REF_TO_ERR(rref);
		}
	}
	if (rc == E_HM_OK) {
		rc = actvxactcapcall_uvmmcall_uvmm_mclone_to_vm(false, true, rref,
								src_va, tgt_gpa, len, prot);
		if (rc != E_HM_OK) {
			hm_debug("uvmm mclone to vm failed, err=%s\n", hmstrerror(rc));
		}
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return rc;
}

#ifdef CONFIG_DEBUG_BUILD
int hm_uvmm_vnotify_send_ctrl_msg(void)
{
	rref_t rref = 0ULL;
	int rc = E_HM_OK;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		rc = REF_TO_ERR(rref);
	}

	if (rc == E_HM_OK) {
		rc = actvxactcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(false, true, rref);
		if (rc != E_HM_OK) {
			hm_debug("uvmm process va mmap failed, err=%s\n", hmstrerror(rc));
		}
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return rc;
}
#else
int hm_uvmm_vnotify_send_ctrl_msg(void)
{
	return ENOSYS;
}
#endif
