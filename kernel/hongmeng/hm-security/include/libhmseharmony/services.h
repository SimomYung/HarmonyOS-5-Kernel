/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony head file for exported interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_EXPORT_SERVICES_H
#define HMSEL_EXPORT_SERVICES_H

#include <stdbool.h>
#include <sys/types.h>
#include <libmem/mcache.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_mutex.h>

#include <libhmseharmony/flask.h>
#include <libhmseharmony/pac.h>

#define HMSEL_SERIVCE_SHIFT_BITS (16u)

#define HMSEL_LOAD_FLAGS_GUEST 0x00000001

#define HMSEL_AVD_NONE 0
#define HMSEL_AVD_ALL 0xFFFFFFFF

#define HMSEL_AVD_FLAGS_PERMISSIVE 0x0001

#define HMSEL_FS_USE_XATTR		(1u) /* use xattr */
#define HMSEL_FS_USE_TRANS		(2u) /* use transition SIDs, e.g. devpts/tmpfs */
#define HMSEL_FS_USE_TASK		(3u) /* use task SIDs, e.g. pipefs/sockfs */
#define HMSEL_FS_USE_GENFS		(4u) /* use the genfs support */
#define HMSEL_FS_USE_NONE		(5u) /* no labeling support */
#define HMSEL_FS_USE_MNTPOINT		(6u) /* use mountpoint labeling */
#define HMSEL_FS_USE_NATIVE		(7u) /* use native label support */
#define HMSEL_FS_USE_MAX		(8u) /* Highest HMSEL_FS_USE_XXX */

DECLARE_ULIBS_MCACHE_VARIANT(security_variant)

typedef uint32_t hmsel_sid_t;
typedef uint32_t hmsel_av_t;

struct hmsel_policy;

struct hmsel_str_con {
	char *con;
	size_t len;
};

struct hmsel_audit_data {
	hmsel_av_t requested;
	hmsel_av_t audited;
	hmsel_av_t denied;
	int result;
};

struct hmsel_sid_context {
	hmsel_sid_t ssid;
	hmsel_sid_t tsid;
	hmsel_class_t tclass;
};

#define HMSEL_XPERM_CACHE_MAX 4
struct hmsel_av_decision {
	__sec_data(avc, cntx) uint32_t allowed;
	__sec_data(avc, cntx) uint32_t auditallow;
	__sec_data(avc, cntx) uint32_t auditdeny;
	__sec_data(avc, cntx) uint32_t flags;
	__sec_data(avc, cntx) uint32_t audited;
	uint16_t cmd[HMSEL_XPERM_CACHE_MAX];
	uint8_t cmd_idx;
	uint8_t cmd_len;
};

#define HMSEL_XPERMS_ALLOWED 1
#define HMSEL_XPERMS_AUDITALLOW 2
#define HMSEL_XPERMS_DONTAUDIT 4

#define hmsel_xperm_set(perms, x) ((perms)[(x) >> 5] |= 1 << ((x) & 0x1f))
#define hmsel_xperm_test(perms, x) (1 & ((perms)[(x) >> 5] >> ((x) & 0x1f)))
struct hmsel_xperms_data {
	uint32_t p[8];
};

struct hmsel_xperms_decision {
	uint8_t used;
	uint8_t driver;
	__sec_data(seharmony, ptr) struct hmsel_xperms_data *allowed;
	__sec_data(seharmony, ptr) struct hmsel_xperms_data *auditallow;
	__sec_data(seharmony, ptr) struct hmsel_xperms_data *dontaudit;
};

struct hmsel_xperms {
	uint16_t len;
	struct hmsel_xperms_data drivers;
};

#define DYNAMIC_BUFF 1
struct hm_catmap_node {
	uint16_t startbit; /* starting position in the total bitmap */
	uint64_t map; /* this node's portion of the bitmap */
};
struct hmmls_level {
	uint32_t sens; /* sensitivity */
	uint16_t cat_num; /* bitmap numbers */
	uint16_t highbit;
	struct hm_catmap_node node[DYNAMIC_BUFF]; /* bitmap */
};

#define MLS_LEVEL_NUM 2
struct hmsel_context {
	union {
		struct {
			uint32_t shadow_len; /* need be 0 */
			uint32_t user;
			uint32_t role;
			uint32_t type;
			uint32_t flag; /* 1 - levels : 0 - level */
			union {
				struct hmmls_level *levels;
				struct hmmls_level level[DYNAMIC_BUFF];
			};
		};
		struct {
			uint32_t len;
			char *str;
		};
	};
};

#ifdef CONFIG_HKIP_SEHARMONY_PROT
int hmsel_prmem_init(void);
void *hmsel_prmem_malloc(size_t size);
void *hmsel_prmem_calloc(size_t num, size_t size);
char *hmsel_prmem_strdup(const char *s);
void hmsel_prmem_free(void *p);
#endif
#ifdef CONFIG_DEBUG_BUILD
int hmsel_policydb_test(void);
#endif
int hmsel_init(void);
int hmsel_load_policy(const char *data, size_t len, unsigned int flags);
int hmsel_load_policy_fallback(void);
int hmsel_context_val_to_sid(struct hmsel_context *conp, hmsel_sid_t *sidp);
int hmsel_global_context_val_to_sid(struct hmsel_context *conp, hmsel_sid_t *sidp);
int hmsel_context_valid(struct hmsel_str_con str_con, hmsel_sid_t def_sid);
int hmsel_context_to_sid(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid);
int hmsel_context_to_sid_force(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid);
int hmsel_global_context_to_sid(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid);
int hmsel_global_context_to_sid_force(struct hmsel_str_con str_con, hmsel_sid_t *sidp, hmsel_sid_t def_sid);
int hmsel_sid_to_context(hmsel_sid_t sid, char **scon, uint32_t *scon_len);
int hmsel_sid_to_context_force(hmsel_sid_t sid, char **scon, uint32_t *scon_len);
int hmsel_sid_to_context_inval(hmsel_sid_t sid, char **scon, uint32_t *scon_len);
int hmsel_transition_sid(struct hmsel_sid_context ccon, const char *objname, hmsel_sid_t *sidp);
int hmsel_transition_sid_user(struct hmsel_sid_context ccon, const char *objname, hmsel_sid_t *sidp);
int hmsel_member_sid(struct hmsel_sid_context ccon, hmsel_sid_t *sidp);
int hmsel_change_sid(struct hmsel_sid_context ccon, hmsel_sid_t *sidp);
int hmsel_fs_use(hmsel_sid_t *sidp, unsigned int *behaviorp, const char *fsname);
int hmsel_genfs_sid(const char *fsname, const char *fpath, hmsel_class_t orig_sclass, hmsel_sid_t *sidp);
struct hmsel_state *hmsel_states_get(void);
struct hmsel_context *hmsel_sid_to_context_val(hmsel_sid_t sid);
struct hmsel_context *hmsel_sid_to_context_val_force(hmsel_sid_t sid);
int hmsel_sid_bounded_transition(hmsel_sid_t ssid, hmsel_sid_t tsid);
int hmsel_transition_check(hmsel_sid_t osid, hmsel_sid_t nsid, hmsel_sid_t tsid, hmsel_class_t oclass);
bool hmsel_is_context_match_sid(const char *context, hmsel_sid_t sid);
bool hmsel_allow_unknown(void);
bool hmsel_reject_unknown(void);
int hmsel_port_sid(uint8_t protocol, uint16_t port, hmsel_sid_t *sid);
int hmsel_node_sid(uint16_t dom, char *addr, uint32_t addrlen, hmsel_sid_t *sid);
bool hmsel_mls_enabled(void);
bool hmsel_policycap_netpeer(void);
bool hmsel_policycap_openperm(void);
bool hmsel_policycap_extsockclass(void);
bool hmsel_policycap_alwaysnetwork(void);
bool hmsel_policycap_cgroupseclabel(void);
bool hmsel_policycap_nnp_nosuid_transition(void);
bool hmsel_policycap_genfs_seclabel_symlinks(void);
bool hmsel_policycap_ioctl_skip_cloexec(void);

static inline bool hmsel_initialized(void)
{
	return hm_sec_shconf_sel_state()->initialized;
}

static inline void hmsel_initialized_set(bool val)
{
	hm_sec_shconf_sel_state()->initialized = val;
}

#ifdef CONFIG_HMSEL_DISABLE
static inline bool hmsel_disabled(void)
{
	return hm_sec_shconf_sel_state()->disabled;
}

static inline void hmsel_disabled_set(bool val)
{
	hm_sec_shconf_sel_state()->disabled = val;
}
#else
static inline bool hmsel_disabled(void)
{
	return false;
}

static inline void hmsel_disabled_set(bool val)
{
	(void)val;
}
#endif

#ifdef CONFIG_HMSEL_DEVELOP
static inline bool hmsel_enforcing(void)
{
	return __read_once(hm_sec_shconf()->seharmony_enforcing);
}

static inline void hmsel_enforcing_set(bool val)
{
	hm_sec_shconf()->seharmony_enforcing = val;
}
#else
static inline bool hmsel_enforcing(void)
{
#ifdef CONFIG_SEHARMONY_ENFORCING
	return true;
#else
	return false;
#endif /* CONFIG_SEHARMONY_ENFORCING */
}

static inline void hmsel_enforcing_set(bool val)
{
	(void)val;
}
#endif

static inline bool hmsel_log_restrict(void)
{
	return __read_once(*hm_sec_shconf_sel_log_restrict()) != 0U;
}
#endif
