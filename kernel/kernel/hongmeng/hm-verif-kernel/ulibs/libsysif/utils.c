/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The common sysif api
 * Author: Huawei OS Kernel Lab
 * Create: Sun Aug 11 19:17:51 2019
 */
#include <stdarg.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmasm/libsysif/utils.h>
#include <libsysif/utils.h>
#include <libmem/utils.h>
#include <libhwsecurec/securec.h>
#include <libhmucap/ucap.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmactv/actv.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_hmsignal.h>

/* sysif_actvhdlr_default_handler can tell client that there is no handler for its callnum */
int sysif_actvhdlr_default_handler(unsigned long long sender, unsigned long credential)
{
	UNUSED(sender, credential);
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	unsigned long callnum = __RPC_INFO_DECODE_CALLNO(local->rpcinfo);

	hm_info("No handler to handle this rpc call, callnum: %lu\n", callnum);
	return E_HM_NOSYS;
}

/* sysif_actvhdlr_reserved_default_handler can tell client that there is no handler for reserved methods */
int sysif_actvhdlr_reserved_default_handler(unsigned long long sender, unsigned long credential)
{
	UNUSED(sender, credential);
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	unsigned long callnum = __RPC_INFO_DECODE_CALLNO(local->rpcinfo);

	hm_info("No handler to handle this reserved method, callnum: %lu\n", callnum);
	return E_HM_NOSYS;
}

/* sysif_actvhdlr_noop_handler do nothing for its callnum */
int sysif_actvhdlr_noop_handler(unsigned long long sender, unsigned long credential)
{
	UNUSED(sender, credential);
	return E_HM_OK;
}

#ifndef __HOST_LLT__
static int
__actv_pool_set_cred(unsigned long long sender, unsigned long old_credential,
		     unsigned int u_slot_idx, unsigned long new_credential)
{
	__u32 cnode_idx = hm_ucap_self_cnode_idx();
	__u32 dst_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	uref_t pool_uref = (((__u64)(cnode_idx) << 32u) | (__u64)u_slot_idx);
	int err = hm_actv_pool_set_cred(0, pool_uref, dst_cnode_idx, new_credential);
	if (err < 0) {
		hm_error("Failed to set cred for: pool_uref=%llx cred=%lx err=%s\n",
			  pool_uref, new_credential, hmstrerror(err));
		/* set return err */
		hm_actvpool_set_kcall_retval(sender, old_credential,
					     (unsigned long)(long)err);
		err = __ACTV_WFERET_POOL_GRANT_CHANGE_RET;
	}
	return err;
}

static int __sysif_handle_rejected_default(unsigned long long sender,
					   unsigned int u_slot_idx,
					   unsigned long old_credential)
{
	/* Let actv cred state to be UNINITED */
	int err = __actv_pool_set_cred(sender, old_credential, u_slot_idx,
				       __ARCH_ACTV_CRED_UNINITED);
	return (err == E_HM_OK) ? __ACTV_WFERET_POOL_GRANT_CHANGE_REWIND : err;
}

static int __sysif_handle_set_cred_default(unsigned long long sender,
					   unsigned int u_slot_idx,
					   unsigned long old_credential)
{
	/*
	 * To be compatible with the use of `__ARCH_ACTV_CRED_UNDEFINED` in
	 * {devmgr, vfs, ...}, set credential into `__ARCH_ACTV_CRED_UNDEFINED`
	 * by default. In future, if `__ARCH_ACTV_CRED_UNDEFINED` was all
	 * replaced by `__ARCH_ACTV_CRED_UNINITED` in {devmgr, vfs, ...},
	 * the usage of `__actv_pool_set_cred` could be removed here.
	 */
	int err = __actv_pool_set_cred(sender, old_credential, u_slot_idx,
				       __ARCH_ACTV_CRED_UNDEFINED);
	return (err == E_HM_OK) ? __ACTV_WFERET_POOL_GRANT_CHANGE_REWIND : err;
}

DEFINE_UPCALL_ACTVHDLR(sysif_actv_pool_handle_grant_change)
{
	UNUSED(sender);
	int err = __ACTV_WFERET_POOL_GRANT_CHANGE_OK;

	if (credential != __ARCH_ACTV_CRED_INVAL_UPCALL) {
		err = E_HM_INVAL;
	} else {
		unsigned int event		= args.pool_grant_change.event;
		unsigned int u_slot_idx		= args.pool_grant_change.u_slot_idx;
		unsigned long old_credential	= args.pool_grant_change.old_credential;

		switch (event) {
		case __ACTV_POOL_GRANT_REJECTED:
			err = __sysif_handle_rejected_default(sender,
							      u_slot_idx,
							      old_credential);
			break;
		case __ACTV_POOL_GRANT_SET_CRED:
			err = __sysif_handle_set_cred_default(sender,
							      u_slot_idx,
							      old_credential);
			break;
		default:
			hm_error("Receive an invalid grant change event: event=%u\n",
				  event);
			break;
		}
	}

	return err;
}
#endif

static int
handle_elastic_add_actv_pure(struct actv_pure_attr *attr, rpc_entry_t entry)
{
	int err;

	if (attr == NULL) {
		hm_error("attr is NULL\n");
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	err = hm_actv_pure_create_ex(attr, entry);
	if (err < 0) {
		hm_error_ratelimited("create actv pure failed: %s\n", hmstrerror(err));
	}

	return err;
}

static int
handle_elastic_del_actv_pure(const struct __actv_pool_water_trigger_info *info,
			     struct actv_pure_attr *attr)
{
	int err;
	struct actv_pure *actv_pure = NULL;

	if (IS_REF_ERR(info->killed_actv)) {
		hm_error("killed actv ref is wrong: %"PRIxref"\n", info->killed_actv);
		__hmattr_sideff_resolved("no relative side effect");
		return REF_TO_ERR(info->killed_actv);
	}

	actv_pure = (struct actv_pure *)hm_ucap_actv_pure_res_of(info->killed_actv, attr->actv_kobj_pool);
	err = hm_actv_pure_destroy(info->killed_actv, ptr_to_ulong(actv_pure), attr, (info->is_user_inited != 0u));
	BUG_ON(err != E_HM_OK);

	return E_HM_OK;
}

/*
 * Note: Do not add log in this api. Because listen tcb with high prio
 *       will use vfs's or devhost's actv when handling their elastic
 *       event and thus may cause E_HM_ACTIVATIONPOOL_NORES.
 */
static int
actv_pool_handle_elastic_pure_core(const struct __actv_pool_water_trigger_info *info,
				   struct actv_pure_attr *attr, rpc_entry_t entry)
{
	int err;

	switch (info->type) {
	case __ACTV_POOL_WATER_TRIGGER_TYPE_ADD: {
		err = handle_elastic_add_actv_pure(attr, entry);
		break;
	}
	case __ACTV_POOL_WATER_TRIGGER_TYPE_DEL: {
		err = handle_elastic_del_actv_pure(info, attr);
		break;
	}
	case __ACTV_POOL_WATER_TRIGGER_TYPE_NONE: {
		/* nr_ready_actvs is not out of add/del water, so do nothing */
		err = E_HM_OK;
		break;
	}
	default:
		err = E_HM_INVAL;
		break;
	}

	return err;
}

int
sysif_actv_pool_handle_elastic_pure_ex(struct actv_pure_attr *attr, rpc_entry_t entry)
{
	int err;
	struct __actv_pool_water_trigger_info info = {0U, 0U, 0ULL};

	if (attr == NULL) {
		hm_error("attr is NULL\n");
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	err = hm_actv_pool_check_water(attr->actv_pool, &info);
	if (err < 0) {
		hm_error("fetch elastic info failed: err = %s\n", hmstrerror(err));
		__hmattr_sideff_resolved("no relative side effect");
		return err;
	}

	err = actv_pool_handle_elastic_pure_core(&info, attr, entry);
	return err;
}

#ifdef CONFIG_AUDIT
static uint8_t sysif_entry_sw_default = 0;
static uint8_t *sysif_entry_sw = &sysif_entry_sw_default;

static inline bool sysif_exit_need_default(void)
{
	return false;
}

bool need_sysif_entry(void)
{
	return !(*sysif_entry_sw == 0);
}

void sysif_entry_need_sw_set(uint8_t *sw)
{
	sysif_entry_sw = sw;
}

static sysif_need_hook_func_t sysif_exit_need_hook = sysif_exit_need_default;

bool need_sysif_exit(void)
{
	return sysif_exit_need_hook();
}

void sysif_exit_need_hook_set(sysif_need_hook_func_t func)
{
	sysif_exit_need_hook = func;
}

static sysif_hook_func_t g_sysif_entry_hook_func = NULL;
static sysif_hook_func_t g_sysif_exit_hook_func = NULL;

void sysif_set_entry_hook(sysif_hook_func_t func)
{
	g_sysif_entry_hook_func = func;
}

void sysif_entry_hook(long ret_code, int argc, ...)
{
	va_list args;

	if (g_sysif_entry_hook_func != NULL) {
		va_start(args, argc);
		g_sysif_entry_hook_func(ret_code, argc, args);
		va_end(args);
	}
}

void sysif_set_exit_hook(sysif_hook_func_t func)
{
	g_sysif_exit_hook_func = func;
}

void sysif_exit_hook(long ret_code, int argc, ...)
{
	va_list args;

	if (g_sysif_exit_hook_func != NULL) {
		va_start(args, argc);
		g_sysif_exit_hook_func(ret_code, argc, args);
		va_end(args);
	}
}
#else
bool need_sysif_entry(void)
{
	return false;
}

void sysif_entry_need_sw_set(uint8_t *sw)
{
	UNUSED(sw);
	return;
}

bool need_sysif_exit(void)
{
	return false;
}

void sysif_exit_need_hook_set(sysif_need_hook_func_t func)
{
	UNUSED(func);
	return;
}

void sysif_set_entry_hook(sysif_hook_func_t func)
{
	UNUSED(func);
	return;
}

void sysif_entry_hook(long ret_code, int argc, ...)
{
	UNUSED(ret_code, argc);
	return;
}

void sysif_set_exit_hook(sysif_hook_func_t func)
{
	UNUSED(func);
	return;
}

void sysif_exit_hook(long ret_code, int argc, ...)
{
	UNUSED(ret_code, argc);
	return;
}
#endif

__thread sysif_sec_dump_func_t sysif_exit_sec_dump = NULL;

void sysif_exit_sec_hook(unsigned long credential)
{
	if (unlikely(sysif_exit_sec_dump != NULL &&
		     sysif_actv_is_selfcall(credential) == false)) {
		sysif_exit_sec_dump();
		sysif_exit_sec_dump = NULL;
	}
}

void sysif_set_sec_dump_hook(sysif_sec_dump_func_t func)
{
	sysif_exit_sec_dump = func;
}

void sysif_mask_all(uint64_t *old)
{
	struct __actvret_hmcall_signal_set_sigmask ret;
	/*
	 * It is not possible to block SIGKILL or SIGSTOP.
	 * Attempts to do so are silently ignored in sysmgr.
	 */
	uint64_t mask_all = (uint64_t)(-1);

	if (old == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return;
	}

	/* This function will be removed after actvcall replacing EBB
	 * call, temporarily remove error log, otherwise too many logs
	 * will lead Stack overflow
	 */
	(void)actvcall_hmcall_signal_set_sigmask(SIGNAL_GETSET_SIGMASK, mask_all, &ret);
	*old = (uint64_t)ret.old_mask;
}

void sysif_mask_restore(const uint64_t *old)
{
	uint64_t mask_value;
	struct __actvret_hmcall_signal_set_sigmask ret;

	if (old == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return;
	}

	mask_value = *old;
	mem_zero_s(ret);
	/* This function will be removed after actvcall replacing EBB
	 * call, temporarily remove error log, otherwise too many logs
	 * will lead Stack overflow
	 */
	(void)actvcall_hmcall_signal_set_sigmask(SIGNAL_SET_SIGMASK, mask_value, &ret);
}
