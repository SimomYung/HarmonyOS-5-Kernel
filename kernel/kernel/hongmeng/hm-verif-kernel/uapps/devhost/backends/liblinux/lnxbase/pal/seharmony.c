/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Pal for seharmony
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 02 11:50:30 2023
 */
#include <hongmeng/errno.h>
#include <liblinux/pal.h>
/* rwlock's macro redefined in pal.h */
#include <libsec/cred.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/permissions.h>
#include <libhmseharmony/services.h>
#include <devhost/log.h>

#ifdef CONFIG_HKIP_LDK_SEHARMONY_PROT
#include <libhmkip/prmem.h>
#include <libhmsrv_sys/hm_hkip.h>

#define SZ_1M					0x00100000
#define LDK_SEHARMONY_POOL_CAP    ((CONFIG_HKIP_LDK_SEHARMONY_POLDB_SIZE_MB) * SZ_1M)
#define SEHARMONY_POOL_ALIGNMENT  32

prmem_pool_declare(hm_ldk_seharmony_pool, PRMEM_POOL_RO_NO_RECL,
		   SEHARMONY_POOL_ALIGNMENT, PRMEM_UNDEFINED_REFILL, LDK_SEHARMONY_POOL_CAP);

prmem_pool_declare(hm_ldk_seharmony_prot_pool, PRMEM_POOL_RO_NO_RECL, sizeof(void *), PAGE_SIZE, PAGE_SIZE);

void *liblinux_pal_sel_seharmony_pool_init(void)
{
	prmem_aux_init();
	prmem_pool(&hm_ldk_seharmony_pool, &pool_data(hm_ldk_seharmony_pool));
	prmem_init_nodes_cache();
	hm_info("LDK SeHarmony hkip protect seharmony_pool init success\n");
	return hm_ldk_seharmony_pool;
}

void *liblinux_pal_sel_seharmony_prot_pool_init(void)
{
	prmem_aux_init();
	prmem_pool(&hm_ldk_seharmony_prot_pool, &pool_data(hm_ldk_seharmony_prot_pool));
	prmem_init_nodes_cache();
	hm_info("LDK SeHarmony hkip protect seharmony_prot_pool init success\n");
	return hm_ldk_seharmony_prot_pool;
}

#ifdef CONFIG_DEBUG_BUILD
void liblinux_pal_sel_verify_tampering(void *p, const char *msg)
{
	unsigned char *temp;
	volatile unsigned char tmp_char;
	if (!p) {
		return;
	}
	temp = (unsigned char *)p;
	tmp_char = *temp;
	if (hm_hkip_write_evil_data(temp, *temp + 1) != E_HM_OK) {
		hm_error("LDK SeHarmony hkip verify failed, Try change the '%s' failed.\n", msg);
		return;
	}
	if (tmp_char == *temp) {
		hm_info("LDK SeHarmony hkip verify success. '%s'.\n", msg);
		return;
	}
	if (hm_hkip_write_evil_data(temp, tmp_char) != E_HM_OK) {
		hm_error("LDK SeHarmony hkip verify failed, Try restore the '%s' failed.\n", msg);
		return;
	}
	hm_error("LDK SeHarmony hkip verify failed, The '%s' can be tampered with.\n", msg);
}
#endif
#endif

int liblinux_pal_sel_load_policy(const char *buffer, unsigned long size)
{
	int ret = hm_sel_load_policy_fwd(buffer, size);
	return -hmerrno2posix(ret);
}

void liblinux_pal_sel_init_ctx(struct task_security_struct *sel_cred)
{
	int err;
	struct sec_chk_cred cred_chk = {0};
	struct sec_cred_seharmony *seharmony_cred;
	__u64 idx = 0;
	err = sysfast_miscdata_read(&idx, __MISCDATA_TCB_CRED,
				    __MISCACCESS_SELF);

	if (err != E_HM_OK) {
		dh_error("failed to read tcb cred misc data\n");
		return;
	}

	struct base_cred *b_cred = (struct base_cred *)sec_cred_idx_to_ptr(idx);
	if (b_cred == NULL) {
		dh_error("failed to get base cred pointer\n");
		return;
	}

	cred_chk.base_cred = b_cred;
	cred_chk.cred_idx = idx;
	seharmony_cred = sel_cred_of_sec_cred(&cred_chk);
	sel_cred->sid = seharmony_cred->sid;
	sel_cred->exec_sid = seharmony_cred->exec_sid;
	sel_cred->create_sid = seharmony_cred->create_sid;
	sel_cred->keycreate_sid = seharmony_cred->keycreate_sid;
	sel_cred->sockcreate_sid = seharmony_cred->sockcreate_sid;
}

int liblinux_pal_sel_update_enforce(int value)
{
	int ret = hm_sec_shconf_set_field(HM_SEC_SHCONF_OFFSET_SEL_ENFORCING, value);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sel_avc_check(struct liblinux_pal_sel_sid_context *ccon, unsigned int requested)
{
	int ret = hmsel_avc_check(*((struct hmsel_sid_context *)ccon), requested);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sel_transition_sid(struct liblinux_pal_sel_sid_context *ccon, const char *objname, unsigned int *sidp)
{
	int ret = hmsel_transition_sid(*((struct hmsel_sid_context *)ccon), objname, sidp);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sel_policycap_extsockclass(void)
{
	return hmsel_policycap_extsockclass();
}

int liblinux_pal_sel_initialized(void)
{
	return hmsel_initialized();
}

int liblinux_pal_sel_allow_unknown(void)
{
	return (int)hmsel_allow_unknown();
}
