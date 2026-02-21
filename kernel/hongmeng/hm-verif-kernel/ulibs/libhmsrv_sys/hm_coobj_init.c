/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define coobj related functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 9 20:48:28 2023
 */
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_coobj.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>

static vatomic32_t coobj_user_lock_inited = VATOMIC_INIT(0U);
#ifdef __aarch64__
static vatomic32_t coobj_resv_mem_inited = VATOMIC_INIT(0U);
extern uintptr_t coobj_resv_va;

static int coobj_resv_mem_init(void)
{
	int ret = E_HM_OK;
	void *resv_va = NULL;
	if (vatomic32_cmpxchg(&coobj_resv_mem_inited, 0U, 1U) == 1U) {
		do {} while (coobj_resv_va == 0);
		return E_HM_OK;
	}
	resv_va = hm_mem_mmap(NULL, COOBJ_RESERVED_MEM_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
	if (resv_va == MAP_FAILED) {
		ret = E_HM_NOMEM;
	} else {
		coobj_resv_va = ptr_to_type(resv_va, uintptr_t);
		ret = E_HM_OK;
	}
	return ret;
}
#endif

static int coobj_user_uholder_map(void *ctx, struct ____coobj_chunkid_s chunkid, __relpos_t size, __bool large_obj)
{
	UNUSED(ctx, chunkid, size, large_obj);
	return E_HM_OPNOTSUPP;
}
static int coobj_user_uholder_unmap(void *ctx, struct ____coobj_chunkid_s chunkid, __bool large_obj)
{
	UNUSED(ctx, chunkid, large_obj);
	return E_HM_OPNOTSUPP;
}
static int coobj_user_urenter_notify_noref(void *ctx, struct ____coobj_chunkid_s chunkid)
{
	UNUSED(ctx, chunkid);
	return E_HM_OPNOTSUPP;
}
static void coobj_user_uholder_lock_init(void *lockerptr)
{
	if (vatomic32_cmpxchg(&coobj_user_lock_inited, 0U, 1U) == 1U) {
		return;
	}
	raw_mutex_init((struct raw_mutex *)lockerptr);
}
static void coobj_user_uholder_lock(void *lockerptr)
{
	raw_mutex_lock((struct raw_mutex *)lockerptr);
}
static void coobj_user_uholder_unlock(void *lockerptr)
{
	raw_mutex_unlock((struct raw_mutex *)lockerptr);
}

static struct raw_mutex user_lock = RAW_MUTEX_INITIALIZER;

static struct ____coobj_base_user_import_s coobj_user_base_user_import = {
	.mem = {
		.uholder_map = coobj_user_uholder_map,
		.uholder_unmap = coobj_user_uholder_unmap,
		.urenter_notify_noref = coobj_user_urenter_notify_noref,
	},
	.sync = {
		.init = coobj_user_uholder_lock_init,
		.lock = coobj_user_uholder_lock,
		.unlock = coobj_user_uholder_unlock,
	},
};
static struct ____coobj_base_user_context_s coobj_user_base_user_context = {
	.memctx = NULL,
	.syncctx = &user_lock,
};
static struct ____coobj_base_user_s coobj_user = {
	.context = &coobj_user_base_user_context,
	.import = &coobj_user_base_user_import,
};

int coobj_init(const char *name, uintptr_t *ator, coobj_role role, __bool resv_mem, mode_t mode)
{
	int err = E_HM_OK;
	unsigned int flags = 0;
	struct coobj_name obj_name;
	struct __actvret_hmcall_ips_init fun_ret;

	mem_zero_s(fun_ret);
	if (name == NULL || ator == NULL || (role != COOBJ_ROLE_HOLDER && role != COOBJ_ROLE_RENTER)) {
		err = E_HM_INVAL;
	}
#ifdef __aarch64__
	if (err == E_HM_OK && resv_mem) {
		err = coobj_resv_mem_init();
	}
#endif
	if (err == E_HM_OK) {
		flags |= hm_sec_is_kasan_enabled() ? HM_COOBJ_FLAGS_KASAN_ENABLE : 0;
#ifdef CONFIG_HKIP_SECCOMP_PROT
		if (strcmp(name, "seccomp") == 0 && role == COOBJ_ROLE_HOLDER)
			flags |= HM_COOBJ_FLAGS_HKIP_ENABLE;
#endif
		uintptr_t shadow_pgref_va = hm_sec_get_shadow_pgref_va();

		NOFAIL(strncpy_s(obj_name.name, sizeof(obj_name.name),
			name, COOBJ_OBJ_NAME_MAX - 1));
#ifdef __aarch64__
		if (resv_mem) {
			err = actvcall_hmcall_ips_init(coobj_resv_va, obj_name, (int)role, mode, flags, shadow_pgref_va, &fun_ret);
		} else {
			err = actvcall_hmcall_ips_init((uintptr_t)0, obj_name, (int)role, mode, flags, shadow_pgref_va, &fun_ret);
		}
#else
		UNUSED(resv_mem);
		err = actvcall_hmcall_ips_init((uintptr_t)0, obj_name, (int)role, mode, flags, shadow_pgref_va, &fun_ret);
#endif
	}
	if (err == E_HM_OK) {
		*ator = coobj_var_user_init((struct ____coobj_uator_list_s *)(fun_ret.uator_list_uva),
			fun_ret.uator_typeid, flags, (int)role, &coobj_user, fun_ret.mapped);
	}

	return err;
}
