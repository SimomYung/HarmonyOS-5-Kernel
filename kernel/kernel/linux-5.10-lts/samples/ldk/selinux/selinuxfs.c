// SPDX-License-Identifier: GPL-2.0
#include <trace/hooks/liblinux.h>
#include <liblinux/prmem.h>
#include <security.h>

#include "sec.h"

static void hook_sel_load_policy(void *args, const char *buffer, unsigned long size, ssize_t *ret)
{
	int rc = sec_load_policy(buffer, size);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_load_policy, hook_sel_load_policy);

#ifdef CONFIG_SECURITY_SELINUX_DEVELOP
static void hook_sel_update_enforce(void *args, int value, ssize_t *ret)
{
	int rc = sec_update_enforce(value);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_update_enforce, hook_sel_update_enforce);
#endif /* CONFIG_SECURITY_SELINUX_DEVELOP */

static void hook_sel_update_cache_threshold(void *args, int value, ssize_t *ret)
{
	int rc = sec_update_cache_threshold(value);
	if (ret != NULL) {
		*ret = rc;
	}
}
INIT_VENDOR_HOOK(ldk_vh_sel_update_cache_threshold, hook_sel_update_cache_threshold);

struct common_audit_data;

int __override avc_has_perm(struct selinux_state *state,
			    u32 ssid, u32 tsid,
			    u16 tclass, u32 requested,
			    struct common_audit_data *auditdata)
{
	return sec_avc_perm(tsid, tclass, requested);
}

#ifdef CONFIG_HM_HKIP_SEHARMONY_PROT
void __override selinux_protection_init(struct selinux_state *state)
{
	selinux_pool.hm_prmem_pool = liblinux_pal_sel_seharmony_pool_init();
	selinux_prot_pool.hm_prmem_pool = liblinux_pal_sel_seharmony_prot_pool_init();

	state->prot = (struct selinux_prot *)pzalloc(&selinux_prot_pool, sizeof(struct selinux_prot *), PRMEM_NO_FLAGS);
	if (!state->prot) {
		pr_err("SELinux: hkip protection init err\n");
		BUG();
	}
}
#endif
