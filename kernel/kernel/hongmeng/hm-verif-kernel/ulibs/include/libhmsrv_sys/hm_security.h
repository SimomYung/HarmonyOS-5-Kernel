/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Wrappers for methods of secmgr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 17:12:05 2019
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SECURITY_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SECURITY_H

#include <lib/utils.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <hmasm/page.h>
#include <hongmeng/errno.h>
struct sec_cred_cfg {
	size_t proc_num;
	size_t groups_num;
	uint32_t cfg_mask;
};

#define MAX_SEC_MOD_NUM 6
#define MAX_SEC_COMP_NUM 6

int hm_sec_cred_cfg(struct sec_cred_cfg *cfg);

int hm_sec_setexeccon(const char *context);

int hm_sec_set_debug(const char *context, unsigned cnode_idx,
		     unsigned *log_enable);

int hm_sec_umask(int mask);

#define HM_XPM_MODE_ACTION_READ 1
#define HM_XPM_MODE_ACTION_WRITE 2

int hm_xpm_mode_write(char *buf, size_t len);

int hm_xpm_mode_read(char *buf, size_t len);

int hm_sec_time_set_clock(void);

/* shared configurations */
int hm_sec_shconf_init(void);
int hm_sec_shconf_set_field(unsigned int field, unsigned int value);

#define HM_SEC_SHCONF_SHM_NAME "sec_shconf"
#define HM_SEC_SHCONF_SHM_RO_NAME "sec_shconf_ro"

extern uintptr_t _hm_sec_shconf_base;
extern uintptr_t _hm_sec_shconf_ro_base;

struct hm_sec_shconf_sel_state {
	bool disabled;
	bool initialized;
};

#define HM_SEC_SHCONF_OFFSET_KPTR_RESTRICT	offset_of(struct hm_sec_shconf, kptr_restrict_mode)
#define HM_SEC_SHCONF_OFFSET_AUDIT		offset_of(struct hm_sec_shconf, audit_mode)
#define HM_SEC_SHCONF_OFFSET_AUDIT_SW		offset_of(struct hm_sec_shconf, has_audit_feature)
#define HM_SEC_SHCONF_OFFSET_DEBUG_ENABLE	offset_of(struct hm_sec_shconf, debug_enabled)
#define HM_SEC_SHCONF_OFFSET_AUDIT_PID		offset_of(struct hm_sec_shconf, audit_pid)
#define HM_SEC_SHCONF_OFFSET_AUDIT_FEATURES	offset_of(struct hm_sec_shconf, audit_features)
#define HM_SEC_SHCONF_OFFSET_SEL_LOG_RESTRICT	offset_of(struct hm_sec_shconf, seharmony_log_restrict)

#define HM_SEC_SHCONF_OFFSET_SIG_VERIF	\
	(offset_of(struct hm_sec_shconf_sig_cfg, mode) + sizeof(struct hm_sec_shconf))

#define HM_SEC_SHCONF_OFFSET_SEL_DISABLE	\
	(offset_of(struct hm_sec_shconf_ro, sel_state) + offset_of(struct hm_sec_shconf_sel_state, disabled))
#define HM_SEC_SHCONF_OFFSET_SEL_ENFORCING	\
	(offset_of(struct hm_sec_shconf, seharmony_enforcing))

#define HM_SEC_SHCONF_SEL_LOG_RESTRICT_ENABLE	1U
#define HM_SEC_SHCONF_SEL_LOG_RESTRICT_DISABLE	0U

/* crl control occupies hm_sec_shconf_crl_cfg width bytes */
#define HM_SEC_SHCONF_OFFSET_CRL_CTRL		offset_of(struct hm_sec_shconf, crl)

struct hm_sec_shconf_crl_cfg {
	uint32_t seq_lock;
	uint32_t raw_shm_num;
	size_t raw_crl_sz;
	size_t real_space;
	bool has_default_crl;
};

struct hm_sec_shconf {
	uint8_t kptr_restrict_mode;
	uint8_t debug_enabled;
	uint8_t has_audit_feature;
	uint8_t audit_mode;
	uint8_t seharmony_mode;
	uint32_t audit_pid;
	uint32_t audit_features;
	uint32_t seharmony_log_restrict;
	uid_t kernel_overflowuid;
	gid_t kernel_overflowgid;
	uint32_t seharmony_policy_seq;
	struct hm_sec_shconf_crl_cfg crl;
	size_t root_usr_ns_idx;
	bool seharmony_enforcing;
};

struct hm_sec_shconf_ro {
	struct hm_sec_shconf_sel_state sel_state;
};

struct hm_sec_shconf_sig_cfg {
	uint32_t mode; /* permissive, enforcing, disabled */
	uint32_t type; /* cms, sha256, cms_builtin_crl, pcsk7 */
};

#define HM_SEC_SHCONF_SIZE \
	(ALIGN_UP(sizeof(struct hm_sec_shconf), PAGE_SIZE) + ALIGN_UP(sizeof(struct hm_sec_shconf_sig_cfg), PAGE_SIZE))

#define HM_SEC_SHCONF_SIG_CFG_OFFSET \
	(ALIGN_UP(sizeof(struct hm_sec_shconf), PAGE_SIZE))

static inline struct hm_sec_shconf *hm_sec_shconf(void)
{
	return (struct hm_sec_shconf *)(_hm_sec_shconf_base);
}

static inline struct hm_sec_shconf_ro *hm_sec_shconf_ro(void)
{
	return (struct hm_sec_shconf_ro *)(_hm_sec_shconf_ro_base);
}

static inline struct hm_sec_shconf_sig_cfg *hm_sec_shconf_sig_ro(void)
{
	uintptr_t va = _hm_sec_shconf_ro_base + HM_SEC_SHCONF_SIG_CFG_OFFSET;
	return (struct hm_sec_shconf_sig_cfg *)va;
}

#define HM_SEC_SHCONF_ROOT_USR_NS_IDX_INIT(ptr) ((*(ptr)) = ((size_t)____COOBJ_INVALID_POS))
static inline size_t *hm_sec_shconf_root_usr_ns_idx(void)
{
	return &hm_sec_shconf()->root_usr_ns_idx;
}

static inline struct hm_sec_shconf_crl_cfg *hm_sec_shconf_crl_ctrl(void)
{
	return &hm_sec_shconf()->crl;
}

static inline struct hm_sec_shconf_sel_state *hm_sec_shconf_sel_state(void)
{
	return &hm_sec_shconf_ro()->sel_state;
}

#define HM_SEC_SHCONF_KPTR_RESTRICT_DISABLE	0U
#define HM_SEC_SHCONF_KPTR_RESTRICT_ENABLE	1U
#define HM_SEC_SHCONF_KPTR_RESTRICT_FORCE	2U
#define HM_SEC_SHCONF_KPTR_RESTRICT_MAX		HM_SEC_SHCONF_KPTR_RESTRICT_FORCE

static inline uint8_t *hm_sec_shconf_kptr_restrict(void)
{
	return &hm_sec_shconf()->kptr_restrict_mode;
}

#define HM_SEC_SHCONF_AUDIT_DISABLE	0U
#define HM_SEC_SHCONF_AUDIT_ENABLE	1U
#define HM_SEC_SHCONF_AUDIT_ENABLE_LOCK	2U

#define HM_SEC_SHCONF_AUDIT_SW_DISABLE		0U
#define HM_SEC_SHCONF_AUDIT_SW_ENABLE		1U

#define HM_SEC_AUDIT_SW_ENABLED		"enabled"

static inline uint8_t *hm_sec_shconf_audit(void)
{
	return &hm_sec_shconf()->audit_mode;
}

static inline uint32_t *hm_sec_shconf_audit_pid(void)
{
	return &hm_sec_shconf()->audit_pid;
}

static inline uint32_t *hm_sec_shconf_audit_features(void)
{
	return &hm_sec_shconf()->audit_features;
}

static inline uid_t *hm_sec_shconf_kernel_overflowuid(void)
{
	return &hm_sec_shconf()->kernel_overflowuid;
}

static inline gid_t *hm_sec_shconf_kernel_overflowgid(void)
{
	return &hm_sec_shconf()->kernel_overflowgid;
}

static inline uint32_t *hm_sec_shconf_sel_log_restrict(void)
{
	return &hm_sec_shconf()->seharmony_log_restrict;
}

#define HM_SEC_POLICY_MODE_LEN 32U
#define HM_SEC_POLICY_MODE_NORMAL_STR "normal"
#define HM_SEC_POLICY_MODE_BYPASS_STR "bypass"
#define HM_SEC_POLICY_MODE_NORMAL 0U
#define HM_SEC_POLICY_MODE_BYPASS 1U

#define HM_SEC_SHCONF_DTS_STRING_LEN 32U
#define HM_SEC_SHCONF_DTS_SIG_TYPE_SHA256 "sha256"
#define HM_SEC_SHCONF_DTS_SIG_TYPE_CMS "cms"
#define HM_SEC_SHCONF_DTS_SIG_TYPE_CMS_CRL "cms_builtin_crl"
#define HM_SEC_SHCONF_DTS_SIG_TYPE_PKCS7 "pcsk7"

#define HM_SEC_SHCONF_DTS_SIG_MODE_DISABLED "disabled"
#define HM_SEC_SHCONF_DTS_SIG_MODE_ENFORCING "enforcing"
#define HM_SEC_SHCONF_DTS_SIG_MODE_PERMISSIVE "permissive"

// type
#define HM_SEC_SHCONF_SIG_SHA256 0x0
#define HM_SEC_SHCONF_SIG_CMS 0x1
#define HM_SEC_SHCONF_SIG_CMS_BUILTIN_CRL 0x3
#define HM_SEC_SHCONF_SIG_PKCS7 0x4
// mode
#define HM_SEC_SHCONF_SIG_DISABLED 0U
#define HM_SEC_SHCONF_SIG_PERMISSIVE 2U
#define HM_SEC_SHCONF_SIG_ENFORCING 1U

static inline uint32_t hm_sec_shconf_sig_mode(void)
{
	return hm_sec_shconf_sig_ro()->mode;
}

static inline uint32_t hm_sec_shconf_sig_type(void)
{
	return hm_sec_shconf_sig_ro()->type;
}

static inline void hm_sec_shconf_sig_mode_set(uint32_t value)
{
	hm_sec_shconf_sig_ro()->mode = value;
}

static inline void hm_sec_shconf_sig_type_set(uint32_t value)
{
	hm_sec_shconf_sig_ro()->type = value;
}

static inline void hm_sec_shconf_sig_conf_init(void)
{
	hm_sec_shconf_sig_mode_set(HM_SEC_SHCONF_SIG_DISABLED);
	hm_sec_shconf_sig_type_set(HM_SEC_SHCONF_SIG_SHA256);
}

static inline bool hm_sec_shconf_cms_builtin_crl(void)
{
	if (hm_sec_shconf_sig_mode() != HM_SEC_SHCONF_SIG_DISABLED &&
	    hm_sec_shconf_sig_type() == HM_SEC_SHCONF_SIG_CMS) {
		return false;
	}
	return true;
}

/* get dynamic cms verify mode: depends on enable from dts and choose cms from dts */
static inline uint32_t hm_sec_shconf_cms_verif_mode(void)
{
	uint32_t ret = HM_SEC_SHCONF_SIG_DISABLED;
	if ((hm_sec_shconf_sig_type() & HM_SEC_SHCONF_SIG_CMS) != 0) {
		// when cms sig enabled
		ret = hm_sec_shconf_sig_mode();
	}
	return ret;
}

/* get dynamic sha256 verify mode: depends on enable from dts and choose sha256 from dts */
static inline uint32_t hm_sec_shconf_sha256_verif_mode(void)
{
	uint32_t ret = HM_SEC_SHCONF_SIG_DISABLED;
	if (hm_sec_shconf_sig_type() == HM_SEC_SHCONF_SIG_SHA256) {
		// when sha256 sig enabled
		ret = hm_sec_shconf_sig_mode();
	}
	return ret;
}

/* get dynamic pkcs7 verify mode: depends on enable from dts and choose pkcs7 from dts */
static inline uint32_t hm_sec_shconf_pkcs7_verif_mode(void)
{
	uint32_t ret = HM_SEC_SHCONF_SIG_DISABLED;
	if ((hm_sec_shconf_sig_type() & HM_SEC_SHCONF_SIG_PKCS7) != 0) {
		/* when pkcs7 sig enabled */
		ret = hm_sec_shconf_sig_mode();
	}
	return ret;
}

static inline uint8_t *hm_sec_shconf_audit_sw(void)
{
	return &hm_sec_shconf()->has_audit_feature;
}

#define HM_SEC_SHCONF_SEC_DEBUG_DISABLE		0U
#define HM_SEC_SHCONF_SEC_DEBUG_RESTRICT	1U
#define HM_SEC_SHCONF_SEC_DEBUG_ENABLE		2U
#define HM_SEC_SHCONF_SEC_DEBUG_MAX		HM_SEC_SHCONF_SEC_DEBUG_ENABLE

/*
 * NOTE: Don't call this directly, use hm_sec_debug_get/set_mode
 */
static inline uint8_t *hm_sec_shconf_sec_debug_enable(void)
{
	return &hm_sec_shconf()->debug_enabled;
}

static inline uint8_t hm_sec_debug_get_mode(void)
{
	int ret = hm_sec_shconf_init();
	if (ret != E_HM_OK) {
		return HM_SEC_SHCONF_SEC_DEBUG_DISABLE;
	} else {
		return *hm_sec_shconf_sec_debug_enable();
	}
}

static inline void hm_sec_debug_set_mode(uint8_t mode)
{
	int ret = hm_sec_shconf_init();
	if (ret == E_HM_OK) {
		*hm_sec_shconf_sec_debug_enable() = mode;
	}
}
#ifdef __HOST_UT__
bool hm_sec_debug_is_enabled(void);
#else
static inline bool hm_sec_debug_is_enabled(void)
{
	uint8_t mode = hm_sec_debug_get_mode();
	return mode != HM_SEC_SHCONF_SEC_DEBUG_DISABLE;
}
#endif

void hm_sec_policy_set_mode(uint32_t mode);
uint32_t hm_sec_policy_get_mode(void);

int hm_sec_register_service(const char *name, cref_t actv_cref,
			    unsigned int method_id, uref_t *actv_uref);

int hm_sel_load_policy(const void *policy, size_t size);
int hm_sel_load_policy_fwd(const void *policy, size_t size);

static inline void hm_sec_sel_mode_set(uint8_t mode)
{
	hm_sec_shconf()->seharmony_mode = mode;
}

static inline uint8_t hm_sec_sel_mode_get(void)
{
	return hm_sec_shconf()->seharmony_mode;
}

static inline void hm_sec_sel_plc_seq_inc(uint8_t mode)
{
	hm_sec_shconf()->seharmony_policy_seq++;
}

static inline uint8_t hm_sec_sel_plc_seq_get(void)
{
	return hm_sec_shconf()->seharmony_policy_seq;
}

void hm_sec_set_kasan_state(bool val);
bool hm_sec_is_kasan_enabled(void);

void hm_sec_set_shadow_pgref_va(uintptr_t addr);
uintptr_t hm_sec_get_shadow_pgref_va(void);

#define HM_PR_PAC_SET_RESTRICT		0x706163

#endif
