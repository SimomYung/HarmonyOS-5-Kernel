/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Export static shm API to driver.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 07 10:23:44 2023
 */

#include <libhmsrv_sys/hm_vm.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_uvmm.h>
#include <libdevhost/vpipe/ipc_shm.h>

static const struct uvmm_param_s uvmm_gicpt = {
	.vm_type = VM_GICPT,
};

raw_static_assert(sizeof(struct hm_vm_shm_info) == sizeof(struct dh_shm_info),
		  hm_vm_shm_info_is_not_compatible_with_dh_shm_info);

int libdh_alloc_static_shm(int id, unsigned int flags, struct dh_shm_info *ret_info)
{
	int err = E_HM_OK;
	rref_t rref;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = hm_vm_alloc_static_shm_with_flags(rref, id, UVMM_SHM_O_RDWR, UVMM_SHM_DEFAULT_MODE,
							flags, (struct hm_vm_shm_info *)ret_info);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	return err;
}

int libdh_free_static_shm(int id, struct dh_shm_info *ret_info)
{
	int err = E_HM_OK;
	rref_t rref;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}
	if (err == E_HM_OK) {
		err = hm_vm_free_static_shm(rref, id, (struct hm_vm_shm_info *)ret_info);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	return err;
}
