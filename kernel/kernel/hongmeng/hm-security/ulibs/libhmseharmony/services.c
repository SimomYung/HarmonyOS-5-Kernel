/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for exported interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */


#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <lib/utils.h>
#include <vsync/atomic.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/panic.h>
#include <hmkernel/barrier.h>

#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/policycap.h>

#include "services.h"
#include "policydb.h"
#include "classmapping.h"
#include "sidtab.h"
#include "context.h"
#include "avtab.h"
#include "ocontext.h"
#include "policycap.h"
#include "debug.h"
#include "filename_trans.h"
#ifdef CONFIG_SEHARMONY_AC_CACHE
#include "avc.h"
#endif
#ifdef CONFIG_HKIP_SEHARMONY_PROT
#include <libhmkip/prmem.h>
#include <libhmsrv_sys/hm_hkip.h>
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
#include <libsec/service_management.h>
#endif

#define SZ_1M					0x00100000
#define SEHARMONY_POOL_CAP        ((CONFIG_HKIP_SEHARMONY_POLDB_SIZE_MB) * SZ_1M)
#define SEHARMONY_POOL_ALIGNMENT  32

prmem_pool_declare(hm_seharmony_pool, PRMEM_POOL_RO_NO_RECL,
		   SEHARMONY_POOL_ALIGNMENT, PRMEM_UNDEFINED_REFILL, SEHARMONY_POOL_CAP);

#ifdef CONFIG_SEHARMONY_POLICY_SHARE
prmem_pool_declare(hm_seharmony_pool_shared, PRMEM_POOL_RO_NO_RECL,
		   SEHARMONY_POOL_ALIGNMENT, PRMEM_UNDEFINED_REFILL, SEHARMONY_POOL_CAP);
#endif

int hmsel_prmem_init(void)
{
	enum prmem_states prmem_status = hm_hkip_get_status() == E_HM_OK ? PRMEM_ENABLED : PRMEM_DISABLED;
	set_prmem_status(prmem_status);
	if (prmem_active()) {
		prmem_aux_init();
		prmem_pool(&hm_seharmony_pool, &pool_data(hm_seharmony_pool));
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
		prmem_pool(&hm_seharmony_pool_shared, &pool_data(hm_seharmony_pool_shared));
#endif
		prmem_init_nodes_cache();
	}
	hm_info("SeHarmony hkip protect status is %s, mprotect status is %d\n", get_prmem_status(), prmem_active());
	return E_HM_OK;
}

inline void *hmsel_prmem_malloc(size_t size)
{
	if (prmem_active())
		return pmalloc(hm_seharmony_pool, size, PRMEM_NO_FLAGS);
	else
		return malloc(size);
}

inline void *hmsel_prmem_calloc(size_t num, size_t size)
{
	if (prmem_active())
		return pcalloc(hm_seharmony_pool, num, size, PRMEM_NO_FLAGS);
	else
		return calloc(num, size);
}

inline char *hmsel_prmem_strdup(const char *s)
{
	if (prmem_active())
		return pstrdup(hm_seharmony_pool, s, PRMEM_NO_FLAGS);
	else
		return strdup(s);
}

inline void hmsel_prmem_free(void *p)
{
	if (prmem_active()) {
		if (prmem_free_addr(hm_seharmony_pool, p) != E_HM_OK) {
			hm_error("failed to free prmem addr\n");
		}
	} else {
		free(p);
	}
}

#ifdef CONFIG_SEHARMONY_POLICY_SHARE
inline void *hmsel_prmem_malloc_shared(size_t size)
{
	if (prmem_active()) {
		return pmalloc(hm_seharmony_pool_shared, size, PRMEM_NO_FLAGS);
	} else {
		return malloc(size);
	}
}

inline void *hmsel_prmem_calloc_shared(size_t num, size_t size)
{
	if (prmem_active())
		return pcalloc(hm_seharmony_pool_shared, num, size, PRMEM_NO_FLAGS);
	else
		return calloc(num, size);
}

inline char *hmsel_prmem_strdup_shared(const char *s)
{
	if (prmem_active())
		return pstrdup(hm_seharmony_pool_shared, s, PRMEM_NO_FLAGS);
	else
		return strdup(s);
}

inline void hmsel_prmem_free_shared(void *p)
{
	if (prmem_active()) {
		hm_error("hmsel_prmem_pool_share can not support free!\n");
	} else {
		free(p);
	}
}

void *policydb_shared_vaddr = NULL;
void hmsel_set_policydb_shared_vaddr(void *vaddr)
{
	policydb_shared_vaddr = vaddr;
}

void *hmsel_get_policydb_shared_vaddr(void)
{
	return policydb_shared_vaddr;
}

void hmsel_set_supervs_vaddr(unsigned long vaddr)
{
	if (prmem_active()) {
		hm_seharmony_pool_shared->supervs_vaddr = ulong_to_ptr(vaddr, void);
	}
}
#endif /* CONFIG_SEHARMONY_POLICY_SHARE */
#endif

DEFINE_ULIBS_MCACHE_VARIANT(security_variant)

struct hmsel_state hmsel_states = {0};

static bool hmsel_enable(void)
{
	return hmsel_initialized();
}

STATIC struct hmsel_policy *hmsel_policy_of_rlx(void)
{
	return hmsel_states.policy;
}

static struct hmsel_policy *hmsel_policy_of(void)
{
	struct hmsel_policy *temp = __read_once(hmsel_states.policy);
	__smp_rmb();
	return temp;
}

static void hmsel_policy_set(struct hmsel_policy *policy)
{
	__smp_wmb();
	__write_once(hmsel_states.policy, policy);
}

struct policydb *hmsel_get_policydb(void)
{
	struct hmsel_policy *policy = hmsel_policy_of();
	if (policy != NULL) {
		return hmsel_policydb_get_vaddr(&policy->policydb);
	}
	return NULL;
}

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static void hmsel_policycaps_set(bool *policycaps)
{
	vatomicptr_t *vptr = (vatomicptr_t *)(uintptr_t)(&hmsel_states.policycap);
	vatomicptr_write_rel(vptr, (void *)policycaps);
}
#endif

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static bool *hmsel_policycap_of(void)
{
	struct hmsel_state *state = hmsel_states_get();
	vatomicptr_t *vptr = (vatomicptr_t *)(uintptr_t)(&state->policycap);
	return (bool *)vatomicptr_read_acq(vptr);
}

bool hmsel_policycap_netpeer(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_NETPEER]);
	} else {
		return false;
	}
}

bool hmsel_policycap_openperm(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_OPENPERM]);
	} else {
		return false;
	}
}

bool hmsel_policycap_extsockclass(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_EXTSOCKCLASS]);
	} else {
		return false;
	}
}

bool hmsel_policycap_alwaysnetwork(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_ALWAYSNETWORK]);
	} else {
		return false;
	}
}

bool hmsel_policycap_cgroupseclabel(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		bool ret = __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_CGROUPSECLABEL]);
		return ret;
	} else {
		return false;
	}
}

bool hmsel_policycap_nnp_nosuid_transition(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_NNP_NOSUID_TRANSITION]);
	} else {
		return false;
	}
}

bool hmsel_policycap_genfs_seclabel_symlinks(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_GENFS_SECLABEL_SYMLINKS]);
	} else {
		return false;
	}
}

bool hmsel_policycap_ioctl_skip_cloexec(void)
{
	bool *policycap = hmsel_policycap_of();
	if (policycap != NULL) {
		return __read_once(policycap[HMSEL_POLICYDB_CAPABILITY_IOCTL_SKIP_CLOEXEC]);
	} else {
		return false;
	}
}
#else
bool hmsel_policycap_netpeer(void)
{
	struct hmsel_state *state = hmsel_states_get();

	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_NETPEER]);
}

bool hmsel_policycap_openperm(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_OPENPERM]);
}

bool hmsel_policycap_extsockclass(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_EXTSOCKCLASS]);
}

bool hmsel_policycap_alwaysnetwork(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_ALWAYSNETWORK]);
}

bool hmsel_policycap_cgroupseclabel(void)
{
	struct hmsel_state *state = hmsel_states_get();
	bool ret = __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_CGROUPSECLABEL]);
	return ret;
}

bool hmsel_policycap_nnp_nosuid_transition(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_NNP_NOSUID_TRANSITION]);
}

bool hmsel_policycap_genfs_seclabel_symlinks(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_GENFS_SECLABEL_SYMLINKS]);
}

bool hmsel_policycap_ioctl_skip_cloexec(void)
{
	struct hmsel_state *state = hmsel_states_get();
	return __read_once(state->policycap[HMSEL_POLICYDB_CAPABILITY_IOCTL_SKIP_CLOEXEC]);
}
#endif

int hmsel_init(void)
{
	NOFAIL(memset_s(&hmsel_states, sizeof(struct hmsel_state), 0, sizeof(struct hmsel_state)));
	raw_mutex_init(&hmsel_states.policy_lock);
	raw_mutex_init(&hmsel_states.load_lock);
	raw_mutex_init(&hmsel_states.load_fallback_lock);

	/* should set enforcing and checkreqprot from kernel parameter */

	return E_HM_OK;
}

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static int policy_resource_alloc(struct hmsel_policy **p, unsigned int flags)
{
	int rc = E_HM_OK;
	int rc1;
	struct hmsel_policy *policy = (struct hmsel_policy *)hmsel_prmem_malloc(sizeof(struct hmsel_policy));
	if (policy == NULL) {
		rc = E_HM_NOMEM;
		hm_error("failed to alloc hm seharmony policy memory\n");
	} else {
		rc1 = memset_s(policy, sizeof(struct hmsel_policy), 0, sizeof(struct hmsel_policy));
		if (rc1 != EOK) {
			hm_error("failed to init policy data\n");
			hmsel_prmem_free(policy);
			policy = NULL;
			rc = E_HM_INVAL;
		}
	}
	if (rc == E_HM_OK) {
		policy->sidtab = (struct hmsel_sidtab *)malloc(sizeof(struct hmsel_sidtab));
		if (policy->sidtab == NULL) {
			rc = E_HM_NOMEM;
			hm_error("failed to alloc hm seharmony sidtab memory\n");
			hmsel_prmem_free(policy);
			policy = NULL;
		} else {
			rc1 = memset_s(policy->sidtab, sizeof(struct hmsel_sidtab), 0, sizeof(struct hmsel_sidtab));
			if (rc1 != EOK) {
				hm_error("failed to init sidtab data\n");
				free(policy->sidtab);
				policy->sidtab = NULL;
				hmsel_prmem_free(policy);
				policy = NULL;
				rc = E_HM_INVAL;
			}
		}
	}
	if (rc == E_HM_OK) {
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
		if ((flags >> HMSEL_SERIVCE_SHIFT_BITS) & SEC_SERVICE_SYSMGR) {
#endif
			rc = hmsel_policydb_malloc(&policy->policydb);
			if (rc != E_HM_OK) {
				hm_error("failed to alloc hm seharmony policydb memory\n");
				free(policy->sidtab);
				policy->sidtab = NULL;
				hmsel_prmem_free(policy);
				policy = NULL;
				rc = E_HM_INVAL;
			} else {
				rc1 = hmsel_policydb_memset(&policy->policydb);
				if (rc1 != EOK) {
					hm_error("failed to init policydb data\n");
					hmsel_policydb_free(&policy->policydb);
					free(policy->sidtab);
					policy->sidtab = NULL;
					hmsel_prmem_free(policy);
					policy = NULL;
					rc = E_HM_INVAL;
				}
			}
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
		} else {
			hmsel_policydb_set_vaddr(&policy->policydb, hmsel_get_policydb_shared_vaddr());
			hm_info("other services skip malloc & memset.\n");
		}
#endif
	}

	*p = policy;
	return rc;
}
#else
static int policy_resource_alloc(struct hmsel_policy **p, unsigned int flags)
{
	int rc = E_HM_OK;
	int rc1;
	struct hmsel_policy *policy = (struct hmsel_policy *)malloc(sizeof(struct hmsel_policy));
	UNUSED(flags);
	if (policy == NULL) {
		rc = E_HM_NOMEM;
		hm_error("failed to alloc hm seharmony policy memory\n");
	} else {
		rc1 = memset_s(policy, sizeof(struct hmsel_policy), 0, sizeof(struct hmsel_policy));
		if (rc1 != EOK) {
			hm_error("failed to init policy data\n");
			free(policy);
			policy = NULL;
			rc = E_HM_INVAL;
		}
	}
	if (rc == E_HM_OK) {
		policy->sidtab = (struct hmsel_sidtab *)malloc(sizeof(struct hmsel_sidtab));
		if (policy->sidtab == NULL) {
			rc = E_HM_NOMEM;
			hm_error("failed to alloc hm seharmony sidtab memory\n");
			free(policy);
			policy = NULL;
		} else {
			rc1 = memset_s(policy->sidtab, sizeof(struct hmsel_sidtab), 0, sizeof(struct hmsel_sidtab));
			if (rc1 != EOK) {
				hm_error("failed to init sidtab data\n");
				free(policy->sidtab);
				policy->sidtab = NULL;
				free(policy);
				policy = NULL;
				rc = E_HM_INVAL;
			}
		}
	}
	if (rc == E_HM_OK) {
		rc = hmsel_policydb_malloc(&policy->policydb);
		if (rc != E_HM_OK) {
			hm_error("failed to alloc hm seharmony policydb memory\n");
			free(policy->sidtab);
			policy->sidtab = NULL;
			free(policy);
			policy = NULL;
			rc = E_HM_INVAL;
		} else {
			rc1 = hmsel_policydb_memset(&policy->policydb);
			if (rc1 != EOK) {
				hm_error("failed to init policydb data\n");
				hmsel_policydb_free(&policy->policydb);
				free(policy->sidtab);
				policy->sidtab = NULL;
				free(policy);
				policy = NULL;
				rc = E_HM_INVAL;
			}
		}
	}
	*p = policy;
	return rc;
}
#endif

static void policy_resource_release(struct hmsel_policy *policy)
{
	free(policy->sidtab);
#ifdef CONFIG_HKIP_SEHARMONY_PROT
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
	/* policydb share not support free */
	hmsel_prmem_free_shared(policy);
#else
	hmsel_prmem_free(policy);
#endif
#else
	free(policy);
#endif
#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
	class_bitmap_fini();
	hmsel_class_callback_deregister();
#endif
}

/* commit loaded policy to pending new policy for policy update */
static void policy_load_commit(struct hmsel_policy *policy)
{
	/* commit loaded policy to current policy for init load */
	hmsel_policy_set(policy);
}

/* uncommit loaded policy to pending new policy for policy update */
static void policy_load_uncommit(void)
{
	/* commit loaded policy to current policy for init load */
	hmsel_policy_set(NULL);
}

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static void policycaps_load_commit(bool *policycaps)
{
	hmsel_policycaps_set(policycaps);
}

static void policycaps_unload(void)
{
	hmsel_policycaps_set(NULL);
}
#endif

/* change state check logic when supporting policy update */
static int policy_load_state_check(void)
{
	int rc = E_HM_OK;
	if (hmsel_policy_of() != NULL) {
		rc = E_HM_INVAL;
		hm_error("can't override a new policy existed\n");
	}
	return rc;
}

/* change check logic when supporting policy update */
static int policy_load_fallback_state_check(void)
{
	int rc = E_HM_OK;
	/* get the guarantee from sysmgr that fallback is not called after inited */
	if (hmsel_policy_of() == NULL) {
		rc = E_HM_INVAL;
		hm_error("new loaded policy not existed\n");
	}
	return rc;
}

#ifdef CONFIG_HKIP_SEHARMONY_PROT
static int policycaps_resource_alloc(bool **policycaps)
{
	int rc = E_HM_OK;
	size_t caps_sz = sizeof(bool) * HMSEL_POLICYDB_CAPABILITY_MAX;
	bool *tmp = (bool *)hmsel_prmem_malloc(caps_sz);

	if (tmp == NULL) {
		rc = E_HM_NOMEM;
		hm_error("failed to alloc hm seharmony policycaps memory\n");
	} else {
		int ret = memset_s(tmp, caps_sz, 0, caps_sz);
		if (ret != EOK) {
			hm_error("failed to init policycaps data\n");
			hmsel_prmem_free(tmp);
			tmp = NULL;
			rc = E_HM_FAULT;
		}
	}

	*policycaps = tmp;
	return rc;
}
#endif

static int policy_load_policycaps(struct hmsel_policy *policy)
{
	int rc = E_HM_OK;
	unsigned int i;
	struct hmsel_policydb *db = &policy->policydb;
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	bool *policycaps = NULL;
	rc = policycaps_resource_alloc(&policycaps);
#endif
	if (rc == E_HM_OK) {
		for (i = 0; i < HMSEL_POLICYDB_CAPABILITY_MAX; i++) {
			int cap = hmsel_policydb_cap(db, i);
#ifdef CONFIG_HKIP_SEHARMONY_PROT
			__write_once(policycaps[i], cap);
#else
			__write_once(hmsel_states.policycap[i], cap);
#endif
		}
		hmsel_policydb_unknown_cap_log(db);
	}
	if (rc == E_HM_OK) {
#ifdef CONFIG_HKIP_SEHARMONY_PROT
		policycaps_load_commit(policycaps);
#endif
		for (i = 0; i < HMSEL_POLICYDB_CAPABILITY_MAX; i++) {
			int cap = __read_once(hmsel_states.policycap[i]);
			hm_info("seharmony policy capability %s=%d\n",
				hmsel_policycap_name(i), cap);
		}
	}

	return rc;
}

struct hmsel_state *hmsel_states_get(void)
{
	return &hmsel_states;
}

#ifdef CONFIG_SEHARMONY_AC_CACHE
static void do_policy_fini(struct hmsel_policy *policy)
{
	hmsel_sidtab_destroy(policy->sidtab);
	hmsel_free_mapping(&policy->map);
	hmsel_policydb_destroy(&policy->policydb);
	policy_resource_release(policy);
}
#endif

#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
static int hmsel_class_split_setup(unsigned char *map_policy, size_t len)
{
	return class_split_setup(map_policy, len);
}
#endif

#ifdef CONFIG_SEHARMONY_POLICY_SHARE
static int ocontext_to_sid(struct hmsel_sidtab *tab, struct hmsel_ocontext *oconp,
			   unsigned int idx, hmsel_sid_t *sidp);

static int do_ocon_sid_init(struct hmsel_policydb *db, struct hmsel_sidtab *sidtab)
{
	int rc = E_HM_OK;
	if (db == NULL || sidtab == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = hmsel_fsuse_ocontext_sid_init(db, sidtab, ocontext_to_sid);
	}

	if (rc == E_HM_OK) {
		rc = hmsel_genfs_ocontext_sid_init(db, sidtab, ocontext_to_sid);
	}

	return rc;
}
#endif

static int __do_load_policydb(const char *data, size_t len,
			      unsigned int flags, struct hmsel_policy *policy)
{
	int rc;
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	/* sysmgr register prmem_pool_shared function */
	if (prmem_active()) {
#ifdef CONFIG_SEHARMONY_POLICY_SHARE
		if ((flags >> HMSEL_SERIVCE_SHIFT_BITS) & SEC_SERVICE_SYSMGR) {
			hmsel_prmem_callback_register(hmsel_prmem_malloc_shared, hmsel_prmem_calloc_shared,
					hmsel_prmem_strdup_shared, hmsel_prmem_free_shared);
		} else {
			hmsel_prmem_callback_register(hmsel_prmem_malloc, hmsel_prmem_calloc, hmsel_prmem_strdup, hmsel_prmem_free);
		}
#else
		hmsel_prmem_callback_register(hmsel_prmem_malloc, hmsel_prmem_calloc, hmsel_prmem_strdup, hmsel_prmem_free);
#endif
	}
#endif
#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
	if ((flags >> HMSEL_SERIVCE_SHIFT_BITS) != SEC_SERVICE_UNKNOWN) {
		class_bitmap_init(flags >> HMSEL_SERIVCE_SHIFT_BITS, &policy->policydb);
		hmsel_class_callback_register(hmsel_class_split_setup);
	}
#endif
	struct hmsel_policy_file policy_file = {data, len};
	rc = hmsel_policydb_read(&policy->policydb, &policy_file);
	if (rc != E_HM_OK) {
		policy_resource_release(policy);
	}
	if (rc == E_HM_OK) {
		rc = hmsel_set_mapping(&policy->policydb, &policy->map);
		if (rc != E_HM_OK) {
			hmsel_policydb_destroy(&policy->policydb);
			policy_resource_release(policy);
		}
	}
	if (rc == E_HM_OK) {
		/* add sidtab convert when supporting policy update */
		rc = hmsel_sidtab_init(&policy->policydb, policy->sidtab, flags);
		if (rc != E_HM_OK) {
			hmsel_free_mapping(&policy->map);
			hmsel_policydb_destroy(&policy->policydb);
			policy_resource_release(policy);
		}
	}

#ifdef CONFIG_SEHARMONY_POLICY_SHARE
	if (rc == E_HM_OK) {
		rc = do_ocon_sid_init(&policy->policydb, policy->sidtab);
	}
#endif

#ifdef CONFIG_SEHARMONY_AC_CACHE
	if (rc == E_HM_OK) {
		rc = hmsel_ac_cache_init(flags >> HMSEL_SERIVCE_SHIFT_BITS);
		if (rc != E_HM_OK) {
			do_policy_fini(policy);
		}
	}
#endif
	return rc;
}

static int do_load_policy(const char *data, size_t len,
			  unsigned int flags, struct hmsel_policy *policy)
{
	int rc = __do_load_policydb(data, len, flags, policy);
	if (rc == E_HM_OK) {
		rc = policy_load_policycaps(policy);
		if (rc != E_HM_OK) {
			do_policy_fini(policy);
		}
	}
	if (rc == E_HM_OK) {
		policy_load_commit(policy);
		hm_info("seharmony policy load done, service is %u, avtab item num %u\n",
			flags >> HMSEL_SERIVCE_SHIFT_BITS, hmsel_avtab_num(&policy->policydb));
	}
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	if (rc == E_HM_OK && prmem_active())
		prmem_protect_pool(hm_seharmony_pool);
#endif
	return rc;
}

int hmsel_load_policy(const char *data, size_t len, unsigned int flags)
{
	int rc;
	struct hmsel_policy *policy = NULL;

	if (data == NULL) {
		hm_error("input policy buffer NULL\n");
		return E_HM_INVAL;
	}
	raw_mutex_lock(&hmsel_states.load_lock);
	rc = policy_load_state_check();
	if (rc == E_HM_OK) {
		rc = policy_resource_alloc(&policy, flags);
	}
	if (rc == E_HM_OK) {
		rc = do_load_policy(data, len, flags, policy);
	}
	raw_mutex_unlock(&hmsel_states.load_lock);
	return rc;
}

int hmsel_load_policy_fallback(void)
{
	int rc = E_HM_OK;
	struct hmsel_policy *policy;
	raw_mutex_lock(&hmsel_states.load_fallback_lock);
	rc = policy_load_fallback_state_check();
	if (rc == E_HM_OK) {
		/* free sidtab convert data, new sidtab entryies for policy update */
		policy = hmsel_policy_of_rlx();
#ifdef CONFIG_SEHARMONY_AC_CACHE
		hmsel_ac_cache_fini();
#endif
		hmsel_sidtab_destroy(policy->sidtab);
		hmsel_free_mapping(&policy->map);
		hmsel_policydb_destroy(&policy->policydb);
#ifdef CONFIG_HKIP_SEHARMONY_PROT
		hmsel_prmem_free(hmsel_states.policycap);
		policycaps_unload();
#endif
		policy_resource_release(policy);
		policy_load_uncommit();
		hm_info("fallback seharmony policy done\n");
	}
	raw_mutex_unlock(&hmsel_states.load_fallback_lock);
	return rc;
}

/* be called after hmsel_enable, return won't be NULL */
static struct hmsel_policy *hmsel_policy_get_current(void)
{
	/* modify policy get logic when supporting policy update using version num */
	return hmsel_policy_of();
}

STATIC hmsel_class_t class_unmap(const struct hmsel_map *map, hmsel_class_t tclass)
{
	hmsel_class_t ret = tclass;
	if (tclass < map->size) {
		ret = map->mapping[tclass].value;
	}
	return ret;
}

STATIC void hmsel_map_decision(const struct hmsel_map *map, hmsel_class_t tclass,
			       struct hmsel_av_decision *avd, unsigned int allow_unknown)
{
	struct hmsel_mapping *mapping = &map->mapping[tclass];
	unsigned int i, n = mapping->num_perms;
	uint32_t res;
	for (i = 0, res = 0; i < n; i++) {
		if ((avd->allowed & mapping->perms[i]) != 0) {
			res |= 1u<<i;
		}
		if ((allow_unknown != 0) && (mapping->perms[i] == 0)) {
			res |= 1u<<i;
		}
	}
	avd->allowed = res;

	for (i = 0, res = 0; i < n; i++) {
		if ((avd->auditallow & mapping->perms[i]) != 0) {
			res |= 1u<<i;
		}
	}
	avd->auditallow = res;

	for (i = 0, res = 0; i < n; i++) {
		if ((avd->auditdeny & mapping->perms[i]) != 0) {
			res |= 1u<<i;
		}
		if ((allow_unknown == 0) && (mapping->perms[i] == 0)) {
			res |= 1u<<i;
		}
	}
	for (; i < (sizeof(uint32_t) * BITS_PER_BYTE); i++) {
		res |= 1u<<i;
	}
	avd->auditdeny = res;
}

STATIC hmsel_class_t class_map(const struct hmsel_map *map, hmsel_class_t tclass)
{
	hmsel_class_t ret;
	for (ret = (hmsel_class_t)(1u); ret < map->size; ret++) {
		if (map->mapping[ret].value == tclass) {
			return ret;
		}
	}
	return HM_SECCLASS_NULL;
}

#define CONTEXT_INIT_OK    0
#define CONTEXT_INIT_DENY  1
#define CONTEXT_INIT_ALLOW 2

static int
hmsel_sid_context_init(struct hmsel_policy *policy,
		       struct hmsel_sid_context sidcon,
		       struct hmsel_chk_context *chk)
{
	if (policy == NULL || chk == NULL) {
		return CONTEXT_INIT_DENY;
	}
	struct hmsel_policydb *policydb = &policy->policydb;
	struct hmsel_sidtab *sidtab = policy->sidtab;
	struct hmsel_context *scon = hmsel_sidtab_search(sidtab, sidcon.ssid);
	if (scon == NULL) {
		hm_warn("failed to find source sid %u\n", sidcon.ssid);
		return CONTEXT_INIT_DENY;
	}
	struct hmsel_context *tcon = hmsel_sidtab_search(sidtab, sidcon.tsid);
	if (tcon == NULL) {
		hm_warn("failed to find target sid %u\n", sidcon.tsid);
		return CONTEXT_INIT_DENY;
	}
	hmsel_class_t tclass = class_unmap(&policy->map, sidcon.tclass);
	if (unlikely(sidcon.tclass != 0 && tclass == 0)) {
		if (hmsel_policydb_allow_unknown(policydb)) {
			return CONTEXT_INIT_ALLOW;
		}
		return CONTEXT_INIT_DENY;
	}
	chk->scon = scon;
	chk->tcon = tcon;
	chk->tclass = tclass;

	return CONTEXT_INIT_OK;
}

void hmsel_compute_av(struct hmsel_sid_context sidcon,
		      struct hmsel_av_decision *avd, hmsel_av_t req,
		      struct hmsel_xperms *xperms)
{
	if (avd == NULL) {
		return;
	}
	avd_init(avd);
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_chk_context chk_con = {0};
	int rc = hmsel_sid_context_init(policy, sidcon, &chk_con);
	if (rc == CONTEXT_INIT_DENY) {
		return;
	}
	if (rc == CONTEXT_INIT_ALLOW) {
		avd->allowed = HMSEL_AVD_ALL;
		return;
	}
	xperms->len = 0;
	hmsel_policydb_context_compute_av(&policy->policydb, chk_con, avd, req, xperms);
	if (sidcon.tclass < policy->map.size) {
		hmsel_map_decision(&policy->map, sidcon.tclass, avd,
				   hmsel_policydb_allow_unknown(&policy->policydb));
	}
}

void hmsel_compute_xperms_decisions(struct hmsel_sid_context sidcon,
				    uint8_t driver,
				    struct hmsel_xperms_decision *xpd)
{
	if (xpd == NULL) {
		return;
	}

	xpd->driver = driver;
	xpd->used = 0;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_chk_context chk_con = {0};
	int rc = hmsel_sid_context_init(policy, sidcon, &chk_con);
	if (rc == CONTEXT_INIT_DENY) {
		return;
	}
	if (rc == CONTEXT_INIT_ALLOW) {
		goto allow_all;
	}

	hmsel_policydb_context_compute_xpermsd(&policy->policydb, chk_con, xpd);
	return;
allow_all:
	NOFAIL(memset_s(xpd->allowed->p, sizeof(xpd->allowed->p), 0xff,
			sizeof(xpd->auditallow->p)));
	return;
}

static char *str_dupnul(const char *str, size_t len)
{
	char *buf = (char *)mcache_security_variant_malloc(len + 1);
	if (buf != NULL) {
		int rc = memcpy_s(buf, len, str, len);
		if (rc != EOK) {
			hm_error("failed to copy src str\n");
			mcache_security_variant_free(buf);
			buf = NULL;
		} else {
			buf[len] = '\0';
		}
	} else {
		hm_error("failed to alloc copy str\n");
	}
	return buf;
}

STATIC char *str_dup(const char *str, size_t len)
{
	char *buf = (char *)malloc(len);
	if (buf != NULL) {
		int rc = memcpy_s(buf, len, str, len);
		if (rc != EOK) {
			hm_error("failed to copy src str\n");
			free(buf);
			buf = NULL;
		}
	} else {
		hm_error("failed to alloc copy str\n");
	}
	return buf;
}

static int do_str_to_context(struct hmsel_policydb *db, struct hmsel_sidtab *tab,
			     char *strcon, struct hmsel_context *ctx, hmsel_sid_t def_sid)
{
	/* use def_sid to get default context for mls */
	int rc = E_HM_INVAL;
	uint32_t val = 0;
	const char *psep = ":";
	char *psave = NULL;
	char *now = strtok_r(strcon, psep, &psave);
	struct hmsel_context *def_ctx = NULL;
	if (now != NULL && psave != NULL) {
		rc = hmsel_policydb_user_val(db, now, &val);
		if (rc == E_HM_OK) {
			ctx->user = val;
			now = strtok_r(NULL, psep, &psave);
		} else {
			hm_info("invalid sel context user name %s\n", now);
		}
	}
	if (rc == E_HM_OK && now != NULL && psave != NULL) {
		rc = hmsel_policydb_role_val(db, now, &val);
		if (rc == E_HM_OK) {
			ctx->role = val;
			now = strtok_r(NULL, psep, &psave);
		} else {
			hm_info("invalid sel context role name %s\n", now);
		}
	}
	if (rc == E_HM_OK && now != NULL) {
		rc = hmsel_policydb_type_val(db, now, &val);
		if (rc == E_HM_OK) {
			ctx->type = val;
		} else {
			hm_info("invalid sel context type name %s\n", now);
		}
	}
	if (rc == E_HM_OK && now != NULL) {
		if (def_sid != 0) {
			def_ctx = sidtab_context_of(tab, def_sid);
		}
		rc = hmsel_policydb_mls_val(db, psave, ctx, def_ctx);
		if (rc != E_HM_OK) {
			hm_info("sel context mls parse failed err=%s\n", strerror(rc));
		}
	} else {
		rc = E_HM_INVAL;
	}
	return rc;
}

static bool hmsel_policydb_ctx_valid(const struct hmsel_policydb *policydb, const struct hmsel_context *c)
{
	if (c->user == 0 ||
	    c->user > hmsel_policydb_user_nprim(policydb)) {
		hm_error("context user val %"PRIu32" not valid\n", c->user);
		return false;
	}
	if (c->role == 0 ||
	    c->role > hmsel_policydb_role_nprim(policydb)) {
		hm_error("context role val %"PRIu32" not valid\n", c->role);
		return false;
	}
	if (c->type == 0 ||
	    c->type > hmsel_policydb_type_nprim(policydb)) {
		hm_error("context type val %"PRIu32" not valid\n", c->type);
		return false;
	}
	if (c->role != HMSEL_OBJECT_R_VAL) {
		if (!hmsel_policydb_type_ok(policydb, c->type, c->role)) {
			hm_error("context type %"PRIu32" not valid for role %"PRIu32"\n",
				 c->type, c->role);
			return false;
		}
		if (!hmsel_policydb_role_ok(policydb, c->role, c->user)) {
			hm_error("context role %"PRIu32" not valid for user %"PRIu32"\n",
				 c->role, c->user);
			return false;
		}
	}
	/* mls check */
	if (!hmsel_policydb_mls_ok(policydb, c)) {
		hm_error("context mls not valid for mls\n");
		return false;
	}
	return true;
}

STATIC int str_to_context(struct hmsel_policy *policy, struct hmsel_str_con str_con,
			  hmsel_sid_t def_sid, struct hmsel_context *context)
{
	int rc = do_str_to_context(&policy->policydb, policy->sidtab, str_con.con, context, def_sid);
	if (rc == E_HM_OK) {
		rc = hmsel_policydb_ctx_valid(&policy->policydb, context) ? E_HM_OK : E_HM_INVAL;
	}
	return rc;
}

static int do_str_to_sid(struct hmsel_str_con str_con, hmsel_sid_t *sidp,
			 hmsel_sid_t def_sid, int force, int global)
{
	int rc;
	char *str_force = NULL;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_context context = {0};
	char *str = str_con.con;
	struct hmmls_level levels = {0};
	context.levels = &levels;
	if (force != 0) {
		str_force = str_dup(str, strlen(str) + 1);
		if (str_force == NULL) {
			return E_HM_NOMEM;
		}
	}
	rc = str_to_context(policy, str_con, def_sid, &context);
	if (rc == E_HM_INVAL && force != 0) {
		context.str = str_force;
		context.len = (uint32_t)(strlen(str_force) + 1);
		rc = E_HM_OK;
	}
	if (rc == E_HM_OK) {
		PRINT_CON(&context);
		if (global == 0) {
			rc = hmsel_sidtab_context_to_sid(policy->sidtab, &context, sidp);
		} else {
			rc = hmsel_sidtab_global_context_to_sid(policy->sidtab, &context, sidp);
		}
	}
	if ((str_force != NULL) && (context.len == 0)) {
		free(str_force);
	}
	if (rc == E_HM_EXIST) {
		rc = E_HM_OK;
	}
	if (unlikely(context.levels != &levels)) {
		hmsel_context_free(&context);
	}
	return rc;
}

static int hmsel_context_to_sid_core(struct hmsel_str_con str_con, hmsel_sid_t *sidp,
				     hmsel_sid_t def_sid, int force, int global)
{
	int rc = E_HM_OK;
	char *str = str_dupnul(str_con.con, str_con.len);
	if (str == NULL) {
		return E_HM_NOMEM;
	}
	if (!hmsel_enable()) {
		uint32_t i;
		for (i = 1; i < HM_SECINITSID_NUM; i++) {
			const char *s = isid_to_context_str((hmsel_sid_t)i);
			if (s != NULL && strcmp(s, str) == 0) {
				*sidp = (hmsel_sid_t)i;
				break;
			}
		}
		if (i == HM_SECINITSID_NUM) {
			*sidp = HM_SECINITSID_KERNEL;
		}
	} else {
		struct hmsel_str_con new_str_con = {str, str_con.len + 1};
		rc = do_str_to_sid(new_str_con, sidp, def_sid, force, global);
	}
	if (rc != E_HM_OK) {
		hm_error("failed to convert %s to sid, ret=%s\n", str, hmstrerror(rc));
	}
	mcache_security_variant_free(str);
	return rc;
}

int hmsel_context_valid(struct hmsel_str_con str_con, hmsel_sid_t def_sid)
{
	int rc = E_HM_OK;
	char *str = str_dupnul(str_con.con, str_con.len);
	if (str == NULL) {
		return E_HM_NOMEM;
	}

	if (!hmsel_enable()) {
		rc = E_HM_FAULT;
	} else {
		struct hmsel_policy *policy = hmsel_policy_get_current();
		struct hmsel_context context = {0};
		struct hmmls_level levels = {0};
		context.levels = &levels;

		rc = do_str_to_context(&policy->policydb, policy->sidtab, str, &context, def_sid);
		if (rc == E_HM_OK) {
			rc = hmsel_policydb_ctx_valid(&policy->policydb, &context) ? E_HM_OK : E_HM_INVAL;
		}
		if (unlikely(context.levels != &levels)) {
			hmsel_context_free(&context);
		}
	}
	mcache_security_variant_free(str);

	return rc;
}

int hmsel_context_to_sid(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid)
{
	int rc;
	if (sidp == NULL) {
		hm_error("failed to check input params, sid ptr should not be NULL\n");
		rc = E_HM_INVAL;
	} else {
		rc = hmsel_context_to_sid_core(str_con, sidp, def_sid, 0, 0);
	}
	return rc;
}

int hmsel_context_to_sid_force(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid)
{
	int rc;
	if (sidp == NULL) {
		hm_error("failed to check input params, sid ptr should not be NULL\n");
		rc = E_HM_INVAL;
	} else {
		rc = hmsel_context_to_sid_core(str_con, sidp, def_sid, 1, 0);
	}
	return rc;
}

int hmsel_global_context_to_sid(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid)
{
	int rc;
	if (sidp == NULL) {
		hm_error("failed to check input params, sid ptr should not be NULL\n");
		rc = E_HM_INVAL;
	} else {
		rc = hmsel_context_to_sid_core(str_con, sidp, def_sid, 0, 1);
	}
	return rc;
}

int hmsel_global_context_to_sid_force(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid)
{
	int rc;
	if (sidp == NULL) {
		hm_error("failed to check input params, sid ptr should not be NULL\n");
		rc = E_HM_INVAL;
	} else {
		rc = hmsel_context_to_sid_core(str_con, sidp, def_sid, 1, 1);
	}
	return rc;
}

struct dcontext_info {
	struct hmsel_context *scon;
	struct hmsel_context *tcon;
	const char *objname;
	bool sock;
};

static void context_default_user_set(struct hmsel_context *conp, const struct dcontext_info *dinfop,
				     char duser, uint32_t specified, bool tclass_valid)
{
	switch (specified) {
	case HM_AVTAB_TRANSITION:
	case HM_AVTAB_CHANGE:
		if (tclass_valid && duser == HMSEL_DEFAULT_TARGET) {
			conp->user = dinfop->tcon->user;
		} else {
			conp->user = dinfop->scon->user;
		}
		break;
	case HM_AVTAB_MEMBER:
		conp->user = dinfop->tcon->user;
		break;
	default:
		hm_warn("invalid sid compute type %u\n", specified);
		break;
	}
}

static void context_default_prepare(const struct hmsel_policydb *db, struct hmsel_context *conp,
				    const struct dcontext_info *dinfop, hmsel_class_t tclass, uint32_t specified)
{
	char duser = HMSEL_DEFAULT_UND;
	char drole = HMSEL_DEFAULT_UND;
	char dtype = HMSEL_DEFAULT_UND;
	bool tclass_valid = hmsel_policydb_tclass_valid(db, tclass, &duser, &drole, &dtype);

	context_default_user_set(conp, dinfop, duser, specified, tclass_valid);

	if (tclass_valid && drole == HMSEL_DEFAULT_SOURCE) {
		conp->role = dinfop->scon->role;
	} else if (tclass_valid && drole == HMSEL_DEFAULT_TARGET) {
		conp->role = dinfop->tcon->role;
	} else {
		if (tclass == hmsel_policydb_proc_class(db) ||
		    dinfop->sock) {
			conp->role = dinfop->scon->role;
		} else {
			conp->role = HMSEL_OBJECT_R_VAL;
		}
	}
	if (tclass_valid && dtype == HMSEL_DEFAULT_SOURCE) {
		conp->type = dinfop->scon->type;
	} else if (tclass_valid && dtype == HMSEL_DEFAULT_TARGET) {
		conp->type = dinfop->tcon->type;
	} else {
		if (tclass == hmsel_policydb_proc_class(db) ||
		    dinfop->sock) {
			conp->type = dinfop->scon->type;
		} else {
			conp->type = dinfop->tcon->type;
		}
	}
	return;
}

static void do_filename_compute_type(const struct hmsel_policydb *db, struct hmsel_context *conp,
				    const struct dcontext_info *dinfop, hmsel_class_t tclass)
{
	struct hmsel_fname_trans_key key = {dinfop->tcon->type, tclass, dinfop->objname};
	const struct hmsel_fname_trans_datum *datum;
	datum = hmsel_fname_trans_search(db, &key);
	hmsel_fname_trans_find_set(datum, conp, dinfop->scon->type);
}

STATIC int do_compute_context(struct hmsel_policydb *db, struct hmsel_context *conp,
			      const struct dcontext_info *dinfop, hmsel_class_t tclass, uint32_t specified)
{
	int rc = E_HM_OK;
	struct hmsel_context *scon = dinfop->scon;
	struct hmsel_context *tcon = dinfop->tcon;
	struct hmsel_avtab_key avkey = {scon->type, tcon->type, tclass, specified};
	struct hmsel_chk_context ccontext = {0};

	context_default_prepare(db, conp, dinfop, tclass, specified);
	hmsel_avtab_find_set(db, conp, &avkey);
	/* compute filename type according to objname */
	if (dinfop->objname != NULL) {
		do_filename_compute_type(db, conp, dinfop, tclass);
	}
	if ((specified & HM_AVTAB_TRANSITION) != 0) {
		hmsel_policydb_role_trans(db, &conp->role, scon->role, tcon->type, tclass);
	}
	/* support mls sid compute */
	ccontext.scon = scon;
	ccontext.tcon = tcon;
	ccontext.tclass = tclass;
	rc = hmmls_compute_sid(db, ccontext, specified, conp);
	if (rc == E_HM_OK && !hmsel_policydb_ctx_valid(db, conp)) {
		rc = E_HM_INVAL;
	}
	return rc;
}

static int do_compute_sid(struct hmsel_sid_context ccon, uint32_t specified,
			  const char *objname, hmsel_sid_t *sidp, bool kern)
{
	int rc = E_HM_OK;
	struct hmsel_context newcon = {0};
	struct hmsel_policy *policy = hmsel_policy_get_current();
	hmsel_class_t tclass;
	struct hmsel_policydb *db = &policy->policydb;
	struct hmsel_sidtab *sidtab = policy->sidtab;
	struct dcontext_info dinfo;
	struct hmmls_level levels = {0};
	newcon.levels = &levels;
	if (kern) {
		tclass = class_unmap(&policy->map, ccon.tclass);
		dinfo.sock = hmsel_socket_class(ccon.tclass);
	} else {
		tclass = class_map(&policy->map, ccon.tclass);
		dinfo.sock = hmsel_socket_class(tclass);
		tclass = ccon.tclass;
	}
	dinfo.scon = hmsel_sidtab_search(sidtab, ccon.ssid);
	if (dinfo.scon == NULL) {
		hm_error("unrecognized seharmony ssid %u\n", ccon.ssid);
		return E_HM_INVAL;
	}
	dinfo.tcon = hmsel_sidtab_search(sidtab, ccon.tsid);
	if (dinfo.tcon == NULL) {
		hm_error("unrecognized seharmony tsid %u\n", ccon.tsid);
		return E_HM_INVAL;
	}
	dinfo.objname = objname;
	rc = do_compute_context(db, &newcon, &dinfo, tclass, specified);
	if (rc == E_HM_OK) {
		if (ccon.tclass == HM_SECCLASS_PROCESS) {
			/* generate global sid for process */
			rc = hmsel_sidtab_global_context_to_sid(sidtab, &newcon, sidp);
		} else {
			rc = hmsel_sidtab_context_to_sid(sidtab, &newcon, sidp);
		}
		/* use error code ESTALE for policydb update */
	}
	if (unlikely(newcon.levels != &levels)) {
		hmsel_context_free(&newcon);
	}
	return rc;
}

static int hmsel_compute_sid(struct hmsel_sid_context ccon, uint32_t specified,
			     const char *objname, hmsel_sid_t *sidp, bool kern)
{
	int rc = E_HM_OK;
	if (sidp == NULL) {
		hm_error("failed to check input params, sid ptr shouldn't be NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (!hmsel_enable()) {
			switch (ccon.tclass) {
			case HM_SECCLASS_PROCESS:
				*sidp = ccon.ssid;
				break;
			default:
				*sidp = ccon.tsid;
				break;
			}
		} else {
			rc = do_compute_sid(ccon, specified, objname, sidp, kern);
		}
	}
	return rc;
}

int hmsel_transition_sid(struct hmsel_sid_context ccon,
			 const char *objname, hmsel_sid_t *sidp)
{
	return hmsel_compute_sid(ccon, HM_AVTAB_TRANSITION, objname, sidp, true);
}

int hmsel_transition_sid_user(struct hmsel_sid_context ccon,
			      const char *objname, hmsel_sid_t *sidp)
{
	return hmsel_compute_sid(ccon, HM_AVTAB_TRANSITION, objname, sidp, false);
}

int hmsel_member_sid(struct hmsel_sid_context ccon, hmsel_sid_t *sidp)
{
	return hmsel_compute_sid(ccon, HM_AVTAB_MEMBER, NULL, sidp, false);
}

int hmsel_change_sid(struct hmsel_sid_context ccon, hmsel_sid_t *sidp)
{
	return hmsel_compute_sid(ccon, HM_AVTAB_CHANGE, NULL, sidp, false);
}

static int ocontext_to_sid(struct hmsel_sidtab *tab, struct hmsel_ocontext *oconp,
			   unsigned int idx, hmsel_sid_t *sidp)
{
	int rc = E_HM_OK;
	hmsel_sid_t sid = hmsel_ocon_sid(oconp, idx);
	if (sid == 0) {
		struct hmsel_context con = {0};
		struct hmmls_level levels = {0};
		con.levels = &levels;
		rc = hmsel_ocon_context(oconp, &con, idx);
		if (rc == E_HM_OK) {
			rc = hmsel_sidtab_context_to_sid(tab, &con, &sid);
		}
		if (rc == E_HM_OK) {
			hmsel_ocon_sid_set(oconp, idx, sid);
		}
		if (unlikely(con.levels != &levels)) {
			hmsel_context_free(&con);
		}
	}
	if (rc == E_HM_OK) {
		*sidp = sid;
	}
	return rc;
}

static const char *path_strip(const char *path)
{
	const char *ret = path;
	while (ret[0] == '/' && ret[1] == '/') {
		ret++;
	}
	return ret;
}

static int do_genfs_sid(struct hmsel_policy *policy, const char *fsname,
			const char *fpath, hmsel_class_t orig_class, uint32_t *sidp)
{
	int rc = E_HM_NOENT;
	const char *path = path_strip(fpath);
	hmsel_class_t sc = class_unmap(&policy->map, orig_class);
	*sidp = HM_SECINITSID_UNLABELED;
	struct hmsel_ocontext *oconp = hmsel_ocon_genfs(&policy->policydb, fsname, path, sc);
	if (oconp != NULL) {
		rc = ocontext_to_sid(policy->sidtab, oconp, 0, sidp);
	}
	return rc;
}

static int do_fs_use(hmsel_sid_t *sidp, unsigned int *behaviorp, const char *fsname)
{
	int rc = E_HM_OK;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *sidtab = policy->sidtab;
	struct hmsel_ocontext *oconp = hmsel_ocon_fs_use(&policy->policydb, fsname);
	if (oconp != NULL) {
		*behaviorp = hmsel_ocon_behavior(oconp);
		rc = ocontext_to_sid(sidtab, oconp, 0, sidp);
		/* use error code ESTALE for policydb update */
	} else {
		rc = do_genfs_sid(policy, fsname, "/", HM_SECCLASS_DIR, sidp);
		if (rc != E_HM_OK) {
			*behaviorp = HMSEL_FS_USE_NONE;
			rc = E_HM_OK;
		} else {
			*behaviorp = HMSEL_FS_USE_GENFS;
		}
		/* use error code ESTALE for policydb update */
	}
	return rc;
}

int hmsel_fs_use(hmsel_sid_t *sidp, unsigned int *behaviorp, const char *fsname)
{
	int rc = E_HM_OK;
	if (sidp == NULL || behaviorp == NULL || fsname == NULL) {
		hm_error("input ptr params should not be NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (!hmsel_enable()) {
			*sidp = HM_SECINITSID_UNLABELED;
			*behaviorp = HMSEL_FS_USE_NONE;
		} else {
			rc = do_fs_use(sidp, behaviorp, fsname);
		}
	}
	return rc;
}

int hmsel_genfs_sid(const char *fsname, const char *fpath,
		    hmsel_class_t orig_sclass, hmsel_sid_t *sidp)
{
	struct hmsel_policy *policy;
	int rc = E_HM_OK;
	if (fsname == NULL || fpath == NULL || sidp == NULL) {
		hm_error("input ptr params should not be NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (!hmsel_enable()) {
			*sidp = HM_SECINITSID_UNLABELED;
		} else {
			policy = hmsel_policy_get_current();
			rc = do_genfs_sid(policy, fsname, fpath, orig_sclass, sidp);
			/* use error code ESTALE for policydb update */
		}
	}
	return rc;
}

struct string {
	size_t len;
	const char *buf;
};

struct string_mls {
	size_t len;
	char *buf;
};

struct string_context {
	struct string user;
	struct string role;
	struct string type;
	struct string_mls mls;
};

/*
 * equal to sprintf_s(out, maxlen, "%s:%s:%s", user, role, type);
 * only call by context_to_str_valid->concat_context_str_mls, maxlen validated by context_to_str_valid
 */
static int concat_context_str(char *out,
			      uint32_t maxlen,
			      const struct string_context *scon)
{
	size_t p = 0;

	NOFAIL(memcpy_s(out + p, maxlen - p, scon->user.buf, scon->user.len));
	p += scon->user.len;

	out[p++] = ':';

	NOFAIL(memcpy_s(out + p, maxlen - p, scon->role.buf, scon->role.len));
	p += scon->role.len;

	out[p++] = ':';

	NOFAIL(memcpy_s(out + p, maxlen - p, scon->type.buf, scon->type.len));
	p += scon->type.len;

	out[p] = '\0';
	return p;
}

/*
 * equal to sprintf_s(out, maxlen, "%s:%s:%s:%s", user, role, type, mls);
 * only call by context_to_str_valid, maxlen validated by context_to_str_valid
 */
static int concat_context_str_mls(char *out,
				  uint32_t maxlen,
				  const struct string_context *scon)
{
	int ret = concat_context_str(out, maxlen, scon);

	out[ret++] = ':';

	NOFAIL(memcpy_s(out + ret, maxlen - ret, scon->mls.buf, scon->mls.len));
	ret += scon->mls.len;
	out[ret] = '\0';
	return ret;
}

#define STACK_BUF_LEN	((size_t)(64L))

static int context_to_str_valid(struct hmsel_policy *policy,
				struct hmsel_context *conp, char **scon, uint32_t *scon_len)
{
	int rc = E_HM_OK;
	uint32_t len = *scon_len;
	*scon_len = 0;
	struct string_context scontext = {0};
	char stack_buf[STACK_BUF_LEN];

	scontext.user.buf = hmsel_policydb_user_name(&policy->policydb, conp->user);
	scontext.role.buf = hmsel_policydb_role_name(&policy->policydb, conp->role);
	scontext.type.buf = hmsel_policydb_type_name(&policy->policydb, conp->type);
	scontext.user.len = strlen(scontext.user.buf);
	scontext.role.len = strlen(scontext.role.buf);
	scontext.type.len = strlen(scontext.type.buf);

	*scon_len += (uint32_t)(scontext.user.len + 1);
	*scon_len += (uint32_t)(scontext.role.len + 1);
	*scon_len += (uint32_t)(scontext.type.len + 1);
	if (hmsel_mls_enabled()) {
		scontext.mls.buf = hmsel_policydb_mls_str(&policy->policydb, conp,
				stack_buf, STACK_BUF_LEN);
		if (scontext.mls.buf == NULL) {
			rc = E_HM_NOMEM;
		} else {
			scontext.mls.len = strlen(scontext.mls.buf);
			*scon_len += (uint32_t)(scontext.mls.len + 1);
		}
	}
	/* add mls string info */
	if (scon != NULL && rc == E_HM_OK) {
		char *con_strp = NULL;
		bool need_free = false;

		if (*scon != NULL) {
			if (*scon_len > len) {
				rc = E_HM_FBIG;
			} else {
				con_strp = *scon;
			}
		} else {
			con_strp = malloc(*scon_len);
			if (con_strp == NULL) {
				hm_info("failed to alloc context string\n");
				rc = E_HM_NOMEM;
			}
			need_free = true;
		}

		if (rc == E_HM_OK) {
			if (hmsel_mls_enabled()) {
				rc = concat_context_str_mls(con_strp, *scon_len, &scontext);
			} else {
				rc = concat_context_str(con_strp, *scon_len, &scontext);
			}
			if (rc < 0) {
				hm_debug("failed to dup context string %s %s %s\n",
					 scontext.user.buf, scontext.role.buf, scontext.type.buf);
				rc = E_HM_INVAL;
				if (need_free) {
					free(con_strp);
				}
			} else {
				rc = E_HM_OK;
				*scon = con_strp;
			}
		}
	}
	if (scontext.mls.buf != NULL) {
		if (stack_buf != scontext.mls.buf) {
			(void)memset_s(scontext.mls.buf, scontext.mls.len, 0, scontext.mls.len);
			free(scontext.mls.buf);
		}
		scontext.mls.buf = NULL;
	}
	return rc;
}

#undef STACK_BUF_LEN

STATIC int context_to_str(struct hmsel_policy *policy,
			  struct hmsel_context *conp, char **scon, uint32_t *scon_len)
{
	int rc = E_HM_OK;

	if (scon == NULL) {
		return E_HM_INVAL;
	}

	if (conp->len != 0) {
		if (*scon != NULL) {
			uint32_t len = conp->len;
			if (len > *scon_len) {
				rc = E_HM_FBIG;
			} else {
				NOFAIL(memcpy_s(*scon, *scon_len, conp->str, len));
				*scon_len = len;
			}
		} else {
			*scon_len = conp->len;
			*scon = str_dup(conp->str, conp->len);
			if (*scon == NULL) {
				rc = E_HM_NOMEM;
			}
		}
	} else {
		rc = context_to_str_valid(policy, conp, scon, scon_len);
	}
	return rc;
}

static int do_sid_to_str(hmsel_sid_t sid, char **scon,
			 uint32_t *scon_len, int force, int invalid)
{
	int rc = E_HM_OK;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *tab = policy->sidtab;
	struct hmsel_context *con;

	if (force != 0) {
		con = hmsel_sidtab_search_force(tab, sid);
	} else {
		con = hmsel_sidtab_search(tab, sid);
	}
	if (con == NULL) {
		hm_error("unrecognized seharmony sid %"PRIu32"\n", sid);
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (invalid == 0 || con->len != 0) {
			rc = context_to_str(policy, con, scon, scon_len);
		}
	}
	return rc;
}

static int isid_to_name(hmsel_sid_t sid, char **scon, uint32_t *scon_len)
{
	int rc = E_HM_OK;
	const char *s = isid_to_context_str(sid);

	if (scon == NULL) {
		return E_HM_INVAL;
	}

	if (s == NULL) {
		hm_error("init sid has no name %"PRIu32"\n", sid);
		rc = E_HM_INVAL;
	} else {
		if (*scon != NULL) {
			uint32_t len = (uint32_t)(strlen(s) + 1);
			if (len > *scon_len) {
				rc = E_HM_FBIG;
			} else {
				NOFAIL(memcpy_s(*scon, *scon_len, s, len));
				*scon_len = len;
			}
		} else {
			*scon_len = (uint32_t)(strlen(s) + 1);
			char *str = str_dup(s, *scon_len);
			if (str == NULL) {
				rc = E_HM_NOMEM;
			} else {
				*scon = str;
			}
		}
	}
	return rc;
}

static int sid_to_con_core(hmsel_sid_t sid, char **scon,
			   uint32_t *scon_len, int force, int invalid)
{
	int rc = E_HM_OK;
	if (!hmsel_enable()) {
		if (sid <= HM_SECINITSID_NUM) {
			rc = isid_to_name(sid, scon, scon_len);
		} else {
			hm_error("sid to context str called with unknown sid before seharmony init\n");
			rc = E_HM_INVAL;
		}
	} else {
		rc = do_sid_to_str(sid, scon, scon_len, force, invalid);
	}
	return rc;
}

/*
 * scon: Alloc *scon to be allocated memory. So that in some cases we can
 * avoid malloc and free.
 * For those without allocated memory, it must be inited as NULL.
 */
int hmsel_sid_to_context(hmsel_sid_t sid, char **scon, uint32_t *scon_len)
{
	int rc;
	if (scon_len == NULL || scon == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = sid_to_con_core(sid, scon, scon_len, 0, 0);
	}
	return rc;
}

int hmsel_sid_to_context_force(hmsel_sid_t sid, char **scon, uint32_t *scon_len)
{
	int rc;
	if (scon_len == NULL || scon == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = sid_to_con_core(sid, scon, scon_len, 1, 0);
	}
	return rc;
}

int hmsel_sid_to_context_inval(hmsel_sid_t sid, char **scon, uint32_t *scon_len)
{
	int rc;
	if (scon_len == NULL || scon == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = sid_to_con_core(sid, scon, scon_len, 1, 1);
	}
	return rc;
}

static int hmsel_context_val_to_sid_core(struct hmsel_context *conp, hmsel_sid_t *sidp, int global)
{
	if (conp == NULL || sidp == NULL ||
	    conp->user == 0u || conp->role == 0u || conp->type == 0u) {
		hm_info("input parameter error\n");
		return E_HM_INVAL;
	}
	if (!hmsel_enable()) {
		hm_debug("seharmony not initialized for context convert\n");
		return E_HM_INVAL;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *sidtab = policy->sidtab;
	if (global == 0) {
		return hmsel_sidtab_context_to_sid(sidtab, conp, sidp);
	} else {
		return hmsel_sidtab_global_context_to_sid(sidtab, conp, sidp);
	}
}

int hmsel_context_val_to_sid(struct hmsel_context *conp, hmsel_sid_t *sidp)
{
	return hmsel_context_val_to_sid_core(conp, sidp, 0);
}

int hmsel_global_context_val_to_sid(struct hmsel_context *conp, hmsel_sid_t *sidp)
{
	return hmsel_context_val_to_sid_core(conp, sidp, 1);
}

static struct hmsel_context *sid_to_context_val_core(hmsel_sid_t sid, int force)
{
	if (!hmsel_enable()) {
		hm_debug("seharmony not initialized for context convert\n");
		return NULL;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *sidtab = policy->sidtab;

	return (force == 0) ? hmsel_sidtab_search(sidtab, sid) :
			      hmsel_sidtab_search_force(sidtab, sid);
}

struct hmsel_context *hmsel_sid_to_context_val(hmsel_sid_t sid)
{
	return sid_to_context_val_core(sid, 0);
}

struct hmsel_context *hmsel_sid_to_context_val_force(hmsel_sid_t sid)
{
	return sid_to_context_val_core(sid, 1);
}

int hmsel_sid_bounded_transition(hmsel_sid_t ssid, hmsel_sid_t tsid)
{
	struct hmsel_context *scon = NULL;
	struct hmsel_context *tcon = NULL;

	if (!hmsel_enable()) {
		hm_debug("seharmony not initialized for context convert\n");
		return E_HM_INVAL;
	}
	scon = sid_to_context_val_core(ssid, 0);
	if (scon == NULL) {
		hm_error("failed to find ssid context\n");
		return E_HM_INVAL;
	}

	tcon = sid_to_context_val_core(tsid, 0);
	if (tcon == NULL) {
		hm_error("failed to find tsid context\n");
		return E_HM_INVAL;
	}

	struct hmsel_policy *policy = hmsel_policy_get_current();
	return hmsel_policydb_sid_bounded_transition(&policy->policydb, scon->type, tcon->type);
}

static void transition_audit(hmsel_sid_t osid, hmsel_sid_t nsid, hmsel_sid_t tsid)
{
	char *ocon_str = NULL;
	char *ncon_str = NULL;
	char *tcon_str = NULL;
	uint32_t len = 0u;
	(void)hmsel_sid_to_context_force(osid, &ocon_str, &len);
	(void)hmsel_sid_to_context_force(nsid, &ncon_str, &len);
	(void)hmsel_sid_to_context_force(tsid, &tcon_str, &len);
	hm_error("failed to check transition, old context %"PRIu32"(%s), new context %"PRIu32"(%s),"
		 " task context %"PRIu32"(%s)\n", osid, ocon_str, nsid, ncon_str, tsid, tcon_str);
	free(ocon_str);
	free(ncon_str);
	free(tcon_str);
}

int hmsel_transition_check(hmsel_sid_t osid, hmsel_sid_t nsid, hmsel_sid_t tsid, hmsel_class_t oclass)
{
	if (!hmsel_enable()) {
		return E_HM_OK;
	}
	int ret;
	struct hmsel_context *ocon;
	struct hmsel_context *ncon;
	struct hmsel_context *tcon;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_policydb *db = &policy->policydb;

	ocon = hmsel_sid_to_context_val(osid);
	if (ocon == NULL) {
		hm_error("invalid old sid %"PRIu32" for transition check\n", osid);
		return E_HM_INVAL;
	}
	ncon = hmsel_sid_to_context_val(nsid);
	if (ncon == NULL) {
		hm_error("invalid new sid %"PRIu32" for transition check\n", nsid);
		return E_HM_INVAL;
	}
	tcon = hmsel_sid_to_context_val(tsid);
	if (tcon == NULL) {
		hm_error("invalid task sid %"PRIu32" for transition check\n", tsid);
		return E_HM_INVAL;
	}
	ret = hmsel_policydb_transition_check(db, ocon, ncon, tcon, class_unmap(&policy->map, oclass));
	if (ret != E_HM_OK) {
		transition_audit(osid, nsid, tsid);
	}
	return hmsel_enforcing() ? ret : E_HM_OK;
}

bool hmsel_allow_unknown(void)
{
	if(!hmsel_enable()) {
		return false;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_policydb *db = &policy->policydb;
	return hmsel_policydb_allow_unknown(db);
}

bool hmsel_reject_unknown(void)
{
	if(!hmsel_enable()) {
		return false;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_policydb *db = &policy->policydb;
	return hmsel_policydb_reject_unknown(db);
}

int hmsel_port_sid(uint8_t protocol, uint16_t port, hmsel_sid_t *sid)
{
	if (sid == NULL) {
		hm_error("input sid pointer should not be NULL\n");
		return E_HM_INVAL;
	}
	if (!hmsel_enable()) {
		*sid = HM_SECINITSID_PORT;
		return E_HM_OK;
	}

	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *sidtab = policy->sidtab;
	int rc = E_HM_OK;
	struct hmsel_ocontext *oconp = hmsel_ocon_port(&policy->policydb, protocol, port);

	if (oconp != NULL) {
		rc = ocontext_to_sid(sidtab, oconp, 0u, sid);
		/* should use error code ESTALE for policydb update */
	} else {
		*sid = HM_SECINITSID_PORT;
	}
	return rc;
}

#define INET_ADDR_LEN (sizeof(struct sockaddr_in))
static int do_node_sid_inet(struct hmsel_policy *policy, char *addr, uint32_t addrlen, hmsel_sid_t *sid)
{
	if ((size_t)addrlen < INET_ADDR_LEN) {
		hm_error("inet addrlen not right\n");
		return E_HM_INVAL;
	}
	uint32_t saddr = *((uint32_t *)(void *)addr);
	struct hmsel_ocontext *oconp = hmsel_ocon_node(&policy->policydb, saddr);
	struct hmsel_sidtab *sidtab;
	if (oconp != NULL) {
		sidtab = policy->sidtab;
		return ocontext_to_sid(sidtab, oconp, 0u, sid);
	} else {
		*sid = HM_SECINITSID_NODE;
	}
	return E_HM_OK;
}

#define SIN6_LEN_RFC2133 24
static int do_node_sid_inet6(struct hmsel_policy *policy, char *addr, uint32_t addrlen, hmsel_sid_t *sid)
{
	if ((size_t)addrlen < SIN6_LEN_RFC2133) {
		hm_error("inet6 addrlen not right\n");
		return E_HM_INVAL;
	}
	struct hmsel_ocontext *oconp = hmsel_ocon_node6(&policy->policydb, (uint32_t *)(void *)addr);
	struct hmsel_sidtab *sidtab;
	if (oconp != NULL) {
		sidtab = policy->sidtab;
		return ocontext_to_sid(sidtab, oconp, 0u, sid);
	} else {
		*sid = HM_SECINITSID_NODE;
	}
	return E_HM_OK;
}

int hmsel_node_sid(uint16_t dom, char *addr, uint32_t addrlen, hmsel_sid_t *sid)
{
	if (sid == NULL || addr == NULL) {
		hm_error("input pointer should not be NULL\n");
		return E_HM_INVAL;
	}
	if (!hmsel_enable()) {
		*sid = HM_SECINITSID_NODE;
		return E_HM_OK;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	int rc = E_HM_OK;
	switch (dom) {
	case AF_INET:
		rc = do_node_sid_inet(policy, addr, addrlen, sid);
		break;
	case AF_INET6:
		rc = do_node_sid_inet6(policy, addr, addrlen, sid);
		break;
	default:
		*sid = HM_SECINITSID_NODE;
		break;
	}
	/* should use error code ESTALE for policydb update */
	return rc;
}

bool hmsel_mls_enabled(void)
{
	if(!hmsel_enable()) {
		return false;
	}
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_policydb *db = &policy->policydb;
	return hmsel_policydb_mls_enabled(db);
}

/* In the Harmony kernel, Security Identifier (SID) is independent of processes. Therefore, cross-process security
 * context verification cannot be completed by comparing whether the SIDs are the same. This interface is added to check
 * whether the security context is consistent with the SID of the curent task during SELinux access control of the
 * tzdriver driver node.
 */
bool hmsel_is_context_match_sid(const char *context, hmsel_sid_t sid)
{
	if (context == NULL)
		return false;
	char *con_str = NULL;
	uint32_t len = 0u;
	int ret = hmsel_sid_to_context_force(sid, &con_str, &len);
	if (ret != E_HM_OK)
		return false;

	if (con_str != NULL) {
		ret = strncmp(context, con_str, len);
		if (ret != 0) {
			hm_error("current task sid=%s is not match with context=%s, len=%u\n", con_str, context, len);
			free(con_str);
			return false;
		}
		free(con_str);
		return true;
	}
	hm_error("the context convert from sid is NULL, sid=%u\n", sid);
	return false;
}

int hmsel_sid_mls_copy(hmsel_sid_t sid, hmsel_sid_t mls_sid, hmsel_sid_t *new_sid, bool global)
{
	if (!hmsel_enable() || !hmsel_mls_enabled()) {
		*new_sid = sid;
		return E_HM_OK;
	}

	int rc = E_HM_OK;
	struct hmsel_policy *policy = hmsel_policy_get_current();
	struct hmsel_sidtab *tab = policy->sidtab;
	struct hmsel_context *context1;
	struct hmsel_context *context2;
	struct hmsel_context newcon;

	rc = memset_s(&newcon, sizeof(struct hmsel_context), 0, sizeof(struct hmsel_context));
	if (rc != E_HM_OK) {
		hm_error("failed to init hmsel context\n");
		return E_HM_FAULT;
	}

	context1 = hmsel_sidtab_search(tab, sid);
	if (!context1) {
		hm_error("Seharmony: unrecognized SID %u\n", sid);
		return E_HM_INVAL;
	}
	context2 = hmsel_sidtab_search(tab, mls_sid);
	if (!context2) {
		hm_error("Seharmony: failed to recognize mls SID %u\n", sid);
		return E_HM_INVAL;
	}

	newcon.user = context1->user;
	newcon.role = context1->role;
	newcon.type = context1->type;
	rc = copy_mls_from_defcon(&newcon, context2);
	if (rc != E_HM_OK) {
		hm_error("copy mls failed, err=%s\n", hmstrerror(rc));
		hmsel_context_free(&newcon);
		return rc;
	}
	if (!hmsel_policydb_ctx_valid(&policy->policydb, &newcon)) {
		hmsel_context_free(&newcon);
		return E_HM_INVAL;
	}

	if (global) {
		rc = hmsel_sidtab_global_context_to_sid(tab, &newcon, new_sid);
	} else {
		rc = hmsel_sidtab_context_to_sid(tab, &newcon, new_sid);
	}

	hmsel_context_free(&newcon);
	return rc;
}
