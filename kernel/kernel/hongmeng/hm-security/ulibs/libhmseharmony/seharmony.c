/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Init the module of seharmony and provide hints for check failure
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 06 11:27:14 2023
 */
#include "libsec/utility.h"

#include <stdbool.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsec/posix_cap.h>
#include <libsec/server_env.h>
#include <libsec/service_management.h>
#include <libsec/service_communication.h>
#include <libhmactv/actv.h>
#include <libhmseharmony/avc.h>
#include <libhmseharmony/seharmony.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/avc.h>

static int hm_secs_mgmt_seharmony(unsigned int command, const struct actvpool_recv *data, unsigned int service);
static int seharmony_init(const struct sec_server_env *env)
{
	UNUSED(env);
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	hmsel_prmem_init();
#endif
	sec_set_sel_cap_check_func(hmsel_cap_check);
	return hm_secs_mgmt_register(HM_SEC_MODULE_SEHARMONY, hm_secs_mgmt_seharmony);
}

static void seharmony_fini(void)
{
}

struct sec_mod sec_mod_seharmony = {
	.name = "seharmony",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = seharmony_init,
		.fini = seharmony_fini,
	},
};

static sel_load_policy_custom_ft custom_load_policy_fn;
static sel_load_policy_custom_ft custom_post_load_fn;

void sel_load_policy_custom(sel_load_policy_custom_ft fn, unsigned int command)
{
	switch (command) {
	case HM_SECS_SEL_LOAD_POLICY:
		custom_load_policy_fn = fn;
		break;
	case HM_SECS_SEL_POST_LOAD:
		custom_post_load_fn = fn;
		break;
	default:
		hm_warn("unsupport command %u\n", command);
		break;
	}
}

static int _sel_load_policy(const void *data, size_t size, unsigned int flags, unsigned int service)
{
	int ret;
	/* The data is the shared memory from the sysmgr
	 * and can be directly used without being copied
	 * to the local memory. */
	ret = hmsel_load_policy(data, size, (service << HMSEL_SERIVCE_SHIFT_BITS) | flags);
	if (ret != E_HM_OK) {
		hm_debug("failed to load policy, err=%s\n", hmstrerror(ret));
	}
	return ret;
}

int sel_load_policy(const void *data, size_t size, unsigned int service)
{
	int ret;

	ret= _sel_load_policy(data, size, 0u, service);
	if (ret == E_HM_OK && custom_load_policy_fn != NULL) {
		custom_load_policy_fn();
	}
	return ret;
}

int sel_load_policy_remote(const void *data, size_t size, unsigned int service)
{
	int ret;

	ret= _sel_load_policy(data, size, HMSEL_LOAD_FLAGS_GUEST, service);
	if (ret == E_HM_OK && custom_load_policy_fn != NULL) {
		custom_load_policy_fn();
	}
	return ret;
}

int sel_unload_policy(void)
{
	hm_debug("unload\n");
	(void)hmsel_load_policy_fallback();
	return E_HM_OK;
}

static int seharmony_load_policy(const struct actvpool_recv *data, unsigned int service)
{
	int ret = E_HM_OK;
	struct hm_secs_data policy;
	struct hm_secs_data_desc *desc = NULL;

	desc = hm_secs_recv_simple_data(data, struct hm_secs_data_desc,
					(ret) = E_HM_INVAL);
	if (ret == E_HM_OK) {
		ret = hm_secs_data_recvfrom(&policy, desc, sysif_actv_src_cnode_idx());
	}
	if (ret == E_HM_OK) {
		hm_debug("policy info: size: %zu\n", hm_secs_data_size(&policy));
		if (hm_secs_data_size(&policy) != 0u) {
			ret = sel_load_policy_remote(hm_secs_data_addr(&policy), hm_secs_data_size(&policy), service);
		} else {
			ret = sel_unload_policy();
		}
		hm_secs_data_destroy(&policy);
	}
	return ret;
}

static void seharmony_post_load(void)
{
	if (custom_post_load_fn != NULL) {
		custom_post_load_fn();
	}
	return;
}

static int hm_secs_mgmt_seharmony(unsigned int command, const struct actvpool_recv *data, unsigned int service)
{
	int ret = E_HM_OK;
	switch (command) {
	case HM_SECS_SEL_LOAD_POLICY:
		ret = seharmony_load_policy(data, service);
		if (ret != E_HM_OK) {
			hm_error("failed to load seharmony policy for service, err=%s\n", hmstrerror(ret));
		}
		break;
	case HM_SECS_SEL_POST_LOAD:
		seharmony_post_load();
		break;
	default:
		ret = E_HM_OPNOTSUPP;
		break;
	}
	return ret;
}

SEC_DEFINE_SEHARMONY_DEFAULT(proc_avc_check, ARGS(hmsel_sid_t, tsid,
		hmsel_class_t, tclass, hmsel_av_t, av_perm))
{
	UNUSED(cred);
	struct hmsel_sid_context scon = {
		.ssid = sel_cred->sid,
		.tsid = (int32_t)tsid > 0 ? tsid : sel_cred->sid,
		.tclass = tclass,
	};

	/* no additional audit information */
	return hmsel_avc_query(scon, av_perm, NULL);
}
int hmsel_proc_avc_check(const struct sec_chk_cred *cred, hmsel_sid_t tsid,
		hmsel_class_t tclass, hmsel_av_t av_perm)
{
	return sec_chk_seharmony_proc_avc_check(cred, tsid, tclass, av_perm);
}
