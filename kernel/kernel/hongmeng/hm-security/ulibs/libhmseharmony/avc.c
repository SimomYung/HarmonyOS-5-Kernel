/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for avc
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <inttypes.h>

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_scopedptr.h>

#include <libhmseharmony/avc.h>
#include <libhmseharmony/flask.h>
#include <libhmseharmony/services.h>
#include <libhmsrv_sys/hm_timer.h>

#include "services.h"
#include "classmapping.h"
#include "sidtab.h"
#ifdef CONFIG_SEHARMONY_AC_CACHE
#include <libhmsrv_sys/hm_mem.h>
#include <libsec/service_management.h>
#include "avc.h"
#endif

struct hmsel_avc_xperms_node {
	struct hmsel_xperms xp;
	struct dlist_node xpd_head; /* list head of hmsel_avc_xpermd_node */
	struct raw_rwlock dlist_lock; /* for xpd_head */
};

struct hmsel_avc_pac_result {
	uint64_t pac;
	uint64_t pac_expect;
};

DEFINE_ULIBS_MCACHE(hmsel_avc_xperms_node, sizeof(struct hmsel_avc_xperms_node))
struct hmsel_avc_xperms_decision_node {
	struct hmsel_xperms_decision xpd;
	struct dlist_node xpd_list; /* list of hmsel_xperms_decision */
};

static int hmsel_avc_denied(unsigned flags, struct hmsel_av_decision *avd)
{
	if ((flags & HMSEL_AVC_STRICT) != 0) {
		return E_HM_ACCES;
	}
	if (hmsel_enforcing()) {
#ifdef	CONFIG_SEHARMONY_TYPE_PERMISSIVE
		if (((avd->flags & HMSEL_AVD_FLAGS_PERMISSIVE) == 0)) {
		    return E_HM_ACCES;
		}
#else
		return E_HM_ACCES;
#endif
	}
	return E_HM_OK;
}

#ifdef CONFIG_SEHARMONY_LOG_RESTRICT
static void avc_log_rate_limit(struct hmsel_av_decision *avd,
			  struct hmsel_sid_context scon, hmsel_av_t *audited);
static void avc_log_rate_limit_xperm(struct hmsel_av_decision *avd,
				     struct hmsel_sid_context scon, uint16_t cmd, hmsel_av_t *audited);
#endif


static hmsel_av_t
hmsel_avc_audit_required(const struct hmsel_audit_data *audit_data, struct hmsel_av_decision *avd,
			 hmsel_av_t auditdeny, hmsel_av_t *deniedp, struct hmsel_sid_context scon)
{
	hmsel_av_t requested = audit_data->requested;
	int res = audit_data->result;
	hmsel_av_t denied = requested & ~(avd->allowed);
	hmsel_av_t audited;
	if (unlikely(denied != 0)) {
		audited = denied & avd->auditdeny;
		if (auditdeny != 0 && (auditdeny & avd->auditdeny) == 0) {
			audited = 0;
		}
	} else if (res != E_HM_OK) {
		audited = denied = requested;
	} else {
		audited = requested & avd->auditallow;
	}
#ifdef CONFIG_SEHARMONY_LOG_RESTRICT
	if (audited != 0 && hmsel_log_restrict()) {
		avc_log_rate_limit(avd, scon, &audited);
	}
#else
	UNUSED(scon);
#endif
	*deniedp = denied;
	return audited;
}

#ifdef CONFIG_SEHARMONY_LOG
#define PERM_BUF_LEN 64
#define BUF_LEN 512

/* record pid and 'exe' of process */
static void do_audit_proc(const struct sec_sel_audit *ad, struct hmsel_audit_info *ai)
{
	const struct sec_chk_cred *cred = NULL;
	struct sec_chk_cred chk = {0};
	char *exe = NULL;
	size_t len = ai->len;
	pid_t pid = 0;
	int ret;
	if (ad != NULL && ad->cred != NULL) {
		cred = ad->cred;
	} else {
		ret = sec_chk_cred_init_requester(&chk);
		if (ret == E_HM_OK) {
			cred = &chk;
		}
	}
	if (cred != NULL) {
		(void)sec_log_read_pid_node(cred, &pid, "exe", &exe);
	}
	if (exe != NULL) {
		ret = snprintf_s(ai->buf, len, len - 1, "for pid=%d, comm=\"%s\" ", pid, exe);
	} else {
		if (cred == NULL) {
			ret = snprintf_s(ai->buf, len, len - 1, "%s", "for pid=unknown, comm=unknown ");
		} else {
			ret = snprintf_s(ai->buf, len, len - 1, "for pid=unknown, comm=unknown, cidx=0x%x ", cred->cnode_idx);
		}
	}
	free(exe);
	ai_update(ret, ai);
}

/* record check result and audited rights */
static void do_audit_core(hmsel_class_t class, hmsel_av_t requested,
			  hmsel_av_t audited, hmsel_av_t denied, struct hmsel_audit_info *ai)
{
	UNUSED(requested);

	int ret;
	char *audited_buf;
	size_t len = ai->len;
	hmsel_av_t av_perm = audited;

	if (audited == 0) {
		ret = snprintf_s(ai->buf, len, len - 1, "avc: %s null ", denied ? "denied" : "granted");
		ai_update(ret, ai);
		return;
	}

	char *buf = malloc(PERM_BUF_LEN);
	if (buf == NULL) {
		hm_debug("failed to alloc seharmony perm log buf\n");
		audited_buf = "unknown";
	} else {
		audited_buf = buf;
		hmsel_perms(class, &av_perm, audited_buf, PERM_BUF_LEN);
		buf[PERM_BUF_LEN - 1] = 0;
	}

	ret = snprintf_s(ai->buf, len, len - 1, "avc: %s { %s", denied ? "denied" : "granted", audited_buf);
	ai_update(ret, ai);

	if (av_perm != 0) { // Remaining audited perms
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " 0x%x", av_perm);
		ai_update(ret, ai);
	}
	ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " } ");
	ai_update(ret, ai);
	free(buf);
}

/* record ssid/tsid or scontext/tcontext */
static void do_audit_detail(struct hmsel_sid_context scon, hmsel_av_t denied, int res, struct hmsel_audit_info *ai)
{
	char *scontext = NULL;
	char *tcontext = NULL;
	uint32_t olen;
	int ret = 0;

	(void)hmsel_sid_to_context(scon.ssid, &scontext, &olen);
	(void)hmsel_sid_to_context(scon.tsid, &tcontext, &olen);

	if (scontext == NULL) {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " ssid=%"PRIu32, scon.ssid);
	} else {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " scontext=%s", scontext);
	}
	ai_update(ret, ai);

	if (tcontext == NULL) {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " tsid=%"PRIu32, scon.tsid);
	} else {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " tcontext=%s", tcontext);
	}
	ai_update(ret, ai);
	free(scontext);
	free(tcontext);
	scontext = NULL;
	tcontext = NULL;

	ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " tclass=%s", hmsel_class_name(scon.tclass));
	ai_update(ret, ai);

	if (denied != 0) {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " permissive=%u", (res != 0) ? 0 : 1);
		ai_update(ret, ai);
	}

	/* if sid is valid, len of context is 0, nothing will be print */
	ret = hmsel_sid_to_context_inval(scon.ssid, &scontext, &olen);
	if (ret == 0 && scontext != NULL) {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " srawcon=%s", scontext);
		ai_update(ret, ai);
	}
	/* if sid is valid, len of context is 0, nothing will be print */
	ret = hmsel_sid_to_context_inval(scon.tsid, &tcontext, &olen);
	if (ret == 0 && tcontext != NULL) {
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " trawcon=%s", tcontext);
		ai_update(ret, ai);
	}
	free(scontext);
	free(tcontext);
}
#endif

// Currently the deny scenario will print the log
static int avc_audit_slow(struct hmsel_sid_context scon, struct hmsel_audit_data *aud_data,
	const struct sec_sel_audit *ad)
{
#ifdef CONFIG_SEHARMONY_LOG
	char *buf = NULL;
	struct hmsel_audit_info ai = {0};
#ifdef CONFIG_TEST_SEHARMONY_ENFORCE
	char *scontext = NULL;
	char *ocontext = NULL;
	uint32_t olen;
	(void)hmsel_sid_to_context(scon.ssid, &scontext, &olen);
	(void)hmsel_sid_to_context(scon.tsid, &ocontext, &olen);
	bool test_seharmony_log = false;
	if (scontext != NULL && ocontext != NULL) {
		const char test_prefix[] = ":test_";
		test_seharmony_log = (strstr(scontext, test_prefix) != NULL) ||
							(strstr(ocontext, test_prefix) != NULL);
	}
	free(scontext);
	free(ocontext);
#endif
	if (aud_data->denied) {
		buf = malloc(BUF_LEN);
		if (buf == NULL) {
			hm_error("failed to alloc seharmony log buffer\n");
			return E_HM_NOMEM;
		}
		ai.buf = buf;
		ai.len = BUF_LEN;
		do_audit_core(scon.tclass, aud_data->requested, aud_data->audited, aud_data->denied, &ai);
		do_audit_proc(ad, &ai);
		if (ad != NULL && ad->func != NULL) {
			ad->func(ad->cred, ad->data, &ai);
		}
		do_audit_detail(scon, aud_data->denied, aud_data->result, &ai);
		buf[BUF_LEN - 1] = 0;
#ifdef CONFIG_TEST_SEHARMONY_ENFORCE
		if (test_seharmony_log) {
			hm_error("%s\n", buf);
		}
#else
		hm_error("%s\n", buf);
#endif
		free(buf);
	} else {
		hm_debug("seharmony allow: ssid %"PRIu32", tsid %"PRIu32", tclass %"PRIu16"(%s), "
			 "request %"PRIx32", audited %"PRIx32"\n",
			 scon.ssid, scon.tsid, scon.tclass, hmsel_class_name(scon.tclass),
			 aud_data->requested, aud_data->audited);
	}

	return E_HM_OK;
#else
	UNUSED(scon);
	UNUSED(aud_data);
	UNUSED(ad);
	return E_HM_OK;
#endif
}

int hmsel_avc_audit(struct hmsel_sid_context scon, hmsel_av_t requested,
		    struct hmsel_av_decision *avd, int res, const struct sec_sel_audit *ad,
		    hmsel_av_t auditdeny)
{
	/*  implement formal seharmony audit using err code res */
	struct hmsel_audit_data aud_data = {0};

	aud_data.result = res;
	aud_data.requested = requested;
	aud_data.audited = hmsel_avc_audit_required(&aud_data, avd, auditdeny, &aud_data.denied, scon);
	if (likely(aud_data.audited == 0)) {
		return E_HM_OK;
	}
	return avc_audit_slow(scon, &aud_data, ad);
}

#ifdef CONFIG_SEHARMONY_AC_CACHE

#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT_XPERM
#define ac_cache_stat_xperm(fmt...) __hmlog(HMLOG_INFO, fmt)
#else
#define ac_cache_stat_xperm(fmt...)
#endif

#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
struct ac_cache_stat ac_cache_stats;

#define HMSEL_STAT_LIMIT 1000
#define HMSEL_STAT_CLASS_LIMIT 1000

static void ac_cache_print(void)
{
	uint16_t i, j;
	for (i = 1u; i < hmsel_class_num(); i++) {
		j = i - 1u;
		if ((ac_cache_stats.hits[j] + ac_cache_stats.misses[j]) > HMSEL_STAT_CLASS_LIMIT) {
			hm_info("seharmony stat: class (%"PRIu16")%s\t: hit %"PRIu64",\tmiss %"PRIu64",\toverride %"PRIu64",\t"
				"override_fail %"PRIu64",\toverride_read_fail %"PRIu64",\tallocate %"PRIu64"\n",
				i, hmsel_class_name(i), ac_cache_stats.hits[j], ac_cache_stats.misses[j],
				ac_cache_stats.overrides[j], ac_cache_stats.override_fails[j],
				ac_cache_stats.override_read_fails[j], ac_cache_stats.allocates[j]);
		}
	}
	hm_info("\n");

	uint64_t idx;
	for (idx = 0; idx < ac_cache_stats.override_num; idx++) {
		struct override_entry *oentry = &ac_cache_stats.override_data[idx];
		if (oentry->ready) {
			char *scon1 = NULL, *scon2 = NULL, *tcon1 = NULL, *tcon2 = NULL;
			uint32_t strlen = 0;
			(void)hmsel_sid_to_context(oentry->ssid1, &scon1, &strlen);
			(void)hmsel_sid_to_context(oentry->ssid2, &scon2, &strlen);
			(void)hmsel_sid_to_context(oentry->tsid1, &tcon1, &strlen);
			(void)hmsel_sid_to_context(oentry->tsid2, &tcon2, &strlen);
			hm_info("seharmony stat: override from %s(%"PRIu32") %s(%"PRIu32"):%s(%"PRIu16") "
				"to %s(%"PRIu32") %s(%"PRIu32"):%s(%"PRIu16")\n",
				scon1, oentry->ssid1, tcon1, oentry->tsid1, hmsel_class_name(oentry->tclass1), oentry->tclass1,
				scon1, oentry->ssid2, tcon1, oentry->tsid2, hmsel_class_name(oentry->tclass2), oentry->tclass2);
			free(scon1);
			free(scon2);
			free(tcon1);
			free(tcon2);
		}
	}
	hm_info("\n");
}

#define ac_cache_stat_add(member, class)	\
do {					\
	uint64_t __tmp_stat = vatomic64_inc_get_rlx((vatomic64_t *)(uintptr_t)(&ac_cache_stats.member[class - 1])); \
	if ((__tmp_stat % HMSEL_STAT_LIMIT) == 0) { \
		ac_cache_print();	\
	}				\
} while(false)

#else
#define ac_cache_stat_add(...)
#endif

#define INVALID_SHM_ID (-1)
static struct hmsel_ac_cache ac_cache = {0};
static struct hmsel_ac_cache ac_cache_fast[HM_SECCLASS_FAST + 1] = {0};

static int hmsel_ac_cache_alloc(struct hmsel_ac_cache *cache)
{
	size_t len = HMSEL_BUCKET_SLOTS;
	struct hhead *head = hm_mem_mmap(0, len * sizeof(struct hhead), PROT_READ | PROT_WRITE,
					 MAP_ANON | MAP_PRIVATE, INVALID_SHM_ID, 0);
	if (head == MAP_FAILED) {
		hm_error("failed to alloc hmsel ac cache memory\n");
		return E_HM_NOMEM;
	}
	cache->hheads = head;
	return E_HM_OK;
}

static void hmsel_ac_cache_destroy(struct hmsel_ac_cache *cache)
{
	size_t len = HMSEL_BUCKET_SLOTS;
	(void)hm_mem_munmap(cache->hheads, len * sizeof(struct hhead));
}

static int hmsel_ac_cache_alloc_fast(unsigned int service)
{
	int ret = E_HM_OK;
	if ((service & SEC_SERVICE_VFS) != 0u) {
		ret = hmsel_ac_cache_alloc(&ac_cache_fast[HM_SECCLASS_FILE]);
		if (ret == E_HM_OK) {
			ret = hmsel_ac_cache_alloc(&ac_cache_fast[HM_SECCLASS_DIR]);
			if (ret != E_HM_OK) {
				hmsel_ac_cache_destroy(&ac_cache_fast[HM_SECCLASS_FILE]);
			}
		}
	}
	return ret;
}

int hmsel_ac_cache_init(unsigned int service)
{
#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
	mem_zero_b(&ac_cache_stats, sizeof(struct ac_cache_stat));
	ac_cache_stats.override_data = malloc(sizeof(struct override_entry) * OVERRIDE_ENTRY_MAX);
	if (ac_cache_stats.override_data == NULL) {
		hm_error("failed to alloc hmsel ac cache stat memory\n");
		return E_HM_NOMEM;
	}
#endif
	int ret = hmsel_ac_cache_alloc(&ac_cache);
	if (ret == E_HM_OK) {
		ret = hmsel_ac_cache_alloc_fast(service);
		if (ret != E_HM_OK) {
			hmsel_ac_cache_destroy(&ac_cache);
		}
	}
	return ret;
}

void hmsel_ac_cache_fini(void)
{
#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
	free(ac_cache_stats.override_data);
#endif
	hmsel_ac_cache_destroy(&ac_cache);
}

#define TARGET_SHIFT 2
#define TARGET_CLASS_SHIFT 4
STATIC unsigned int ac_hash_fast(hmsel_sid_t ssid, hmsel_sid_t tsid)
{
	return (ssid ^ (tsid << TARGET_SHIFT)) & (HMSEL_BUCKET_SLOTS - 1);
}

STATIC struct hhead *ac_cache_head_fast(hmsel_class_t idx, unsigned int bucket)
{
	return &ac_cache_fast[idx].hheads[bucket];
}

STATIC unsigned int ac_hash(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass)
{
	return (ssid ^ (tsid << TARGET_SHIFT) ^ (tclass << TARGET_CLASS_SHIFT)) &
	       (HMSEL_BUCKET_SLOTS - 1);
}

STATIC struct hhead *ac_cache_head(unsigned int bucket)
{
	return &ac_cache.hheads[bucket];
}

static bool entry_seq_match(hmsel_seq_t seq_pre, hmsel_seq_t seq_post)
{
	return (seq_pre == seq_post);
}

static void entry_avd_read(struct hmsel_av_decision *avd, struct ac_entry *entry)
{
	*avd = entry->avd;
}

static hmsel_seq_t entry_seq_read(struct ac_entry *entry)
{
	vatomic32_t *v32ptr = (vatomic32_t *)(uintptr_t)(&entry->eseq);
	return vatomic32_read_rlx(v32ptr);
}

static bool entry_seq_prepared(hmsel_seq_t seq)
{
	return (seq & (hmsel_seq_t)1) == 0;
}

static hmsel_seq_t entry_seq_read_pre(struct ac_entry *entry)
{
#define MAX_LOOP_NO_SLEEP 10
#define SLEEP_USEC 1
#define SLEEP_MAX 100000
	hmsel_seq_t seq;
	vatomic32_t *v32ptr = (vatomic32_t *)(uintptr_t)(&entry->eseq);
	unsigned int loop_count = 0;
	unsigned int useconds = SLEEP_USEC;
	bool wait_so_long_log_once = true;
	do {
		loop_count++;
		if (loop_count > MAX_LOOP_NO_SLEEP) {
			useconds = (useconds >= SLEEP_MAX) ? SLEEP_MAX : (useconds << 1);
			hm_usleep(useconds);
		}
		if (useconds >= SLEEP_MAX && wait_so_long_log_once) {
			wait_so_long_log_once = false;
			hm_warn("it has been so long time to wait for avc entry write over!\n");
		}
		seq = vatomic32_read_acq(v32ptr);
	} while (!entry_seq_prepared(seq));
	return seq;
}

static hmsel_seq_t entry_seq_read_post(struct ac_entry *entry)
{
#ifndef __HOST_LLT__
	__smp_rmb();
#endif
	hmsel_seq_t seq = entry_seq_read(entry);
	return seq;
}

#if defined(ENABLE_PAC_HARD_CODE) && defined(PAC_DFI_AC_ENTRY)
static inline uint64_t hm_pacga_ac_entry(struct ac_entry *entry, bool fast)
{
	uint64_t res = (uintptr_t)entry;
	res = hm_pacga(entry->ssid, res);
	res = hm_pacga(entry->tsid, res);
	if (!fast) {
		res = hm_pacga(entry->tclass, res);
	}
	return res;
}

static inline void hm_pacga_ac_entry_check(struct ac_entry *entry, struct hmsel_avc_pac_result *result)
{
	if (result->pac != result->pac_expect) {
		hm_error("[PAC AC DFI] ac entry check fail, pac %llx %llx\n",
				result->pac, result->pac_expect);
		_hm_pac_record_fault(entry->ssid, (uint64_t)(uintptr_t)entry,
			result->pac, result->pac_expect);
	}
}

#else

static inline uint64_t hm_pacga_ac_entry(struct ac_entry *entry, bool fast)
{
	UNUSED(entry);
	UNUSED(fast);
	return 0;
}

static inline void hm_pacga_ac_entry_check(struct ac_entry *entry, struct hmsel_avc_pac_result *result)
{
	UNUSED(entry);
	UNUSED(result);
}
#endif

static bool entry_cmp_fast(hmsel_sid_t ssid, hmsel_sid_t tsid, struct ac_entry *entry,
	struct hmsel_avc_pac_result *result)
{
	hmsel_sid_t ssid_tmp = entry->ssid;
	hmsel_sid_t tsid_tmp = entry->tsid;
	result->pac_expect = entry->pac_fast;
	uint64_t res = (uintptr_t)entry;
	res = hm_pacga(ssid_tmp, res);
	result->pac = hm_pacga(tsid_tmp, res);
	return (ssid == ssid_tmp) && (tsid == tsid_tmp);
}

static bool entry_cmp(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
		      struct ac_entry *entry, struct hmsel_avc_pac_result *result)
{
	hmsel_sid_t ssid_tmp = entry->ssid;
	hmsel_sid_t tsid_tmp = entry->tsid;
	hmsel_class_t tclass_tmp = entry->tclass;
	result->pac_expect = entry->pac;
	uint64_t res = (uintptr_t)entry;
	res = hm_pacga(ssid_tmp, res);
	res = hm_pacga(tsid_tmp, res);
	result->pac = hm_pacga(tclass_tmp, res);
	return (ssid == ssid_tmp) && (tsid == tsid_tmp) && (tclass == tclass_tmp);
}


static bool entry_match_fast(hmsel_sid_t ssid, hmsel_sid_t tsid,
			     struct hmsel_av_decision *avd,
			     struct ac_entry *entry, struct ac_entry **dest,
			     bool is_read)
{
	hmsel_seq_t seq_pre, seq_post;
	struct hmsel_avc_pac_result result;

	seq_pre = entry_seq_read_pre(entry);

	if (entry_cmp_fast(ssid, tsid, entry, &result)) {
		if (dest != NULL) {
			/* xp_node might be freed during ac_add */
			if (is_read) {
				(void)raw_rwlock_rdlock(&entry->xpn_lock);
			} else {
				(void)raw_rwlock_wrlock(&entry->xpn_lock);
			}
			*dest = entry;
		}
		entry_avd_read(avd, entry);
	} else {
		return false;
	}

	seq_post = entry_seq_read_post(entry);

	bool ret = entry_seq_match(seq_pre, seq_post);
	if (ret == false && dest != NULL) {
		(void)raw_rwlock_unlock(&entry->xpn_lock);
	}
	if (ret == true) {
		hm_pacga_ac_entry_check(entry, &result);
	}
	return ret;
}

static bool entry_match(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
			struct hmsel_av_decision *avd, struct ac_entry *entry,
			struct ac_entry **dest,
			bool is_read)
{
	hmsel_seq_t seq_pre, seq_post;
	struct hmsel_avc_pac_result result;

	seq_pre = entry_seq_read_pre(entry);

	if (entry_cmp(ssid, tsid, tclass, entry, &result)) {
		if (dest != NULL) {
			/* xp_node might be freed during ac_add */
			if (is_read) {
				(void)raw_rwlock_rdlock(&entry->xpn_lock);
			} else {
				(void)raw_rwlock_wrlock(&entry->xpn_lock);
			}
			*dest = entry;
		}
		entry_avd_read(avd, entry);
	} else {
		return false;
	}

	seq_post = entry_seq_read_post(entry);

	bool ret = entry_seq_match(seq_pre, seq_post);
	if (ret == false && dest != NULL) {
		(void)raw_rwlock_unlock(&entry->xpn_lock);
	}
	if (ret == true) {
		hm_pacga_ac_entry_check(entry, &result);
	}
	return ret;
}

#define ACTIVE_NUM_ENTRY_IDX 0
#define ENTRY_IDX_ADD_HINT 1
STATIC uint8_t bucket_active_entry_num(struct hhead *head)
{
	vatomic8_t *v8ptr = (vatomic8_t *)(uintptr_t)(&head->entrys[ACTIVE_NUM_ENTRY_IDX].actives);
	return vatomic8_read_rlx(v8ptr);
}

static uint8_t bucket_get_add_hint(struct hhead *head)
{
	vatomic8_t *v8ptr = (vatomic8_t *)(uintptr_t)(&head->entrys[ENTRY_IDX_ADD_HINT].actives);
	return vatomic8_get_inc_rlx(v8ptr);
}

static struct ac_entry *bucket_entry_of(struct hhead *head, uint8_t idx)
{
	return &head->entrys[idx];
}

static int ac_query_normal(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
			   struct hmsel_av_decision *avd, struct ac_entry **dest)
{
	unsigned int bucket = ac_hash(ssid, tsid, tclass);
	struct hhead *head = ac_cache_head(bucket);
	struct ac_entry *entry;
	uint8_t active = bucket_active_entry_num(head);
	uint8_t idx;
	for (idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		if (entry_match(ssid, tsid, tclass, avd, entry, dest, true)) {
			ac_cache_stat_add(hits, tclass);
			return E_HM_OK;
		}
	}
	ac_cache_stat_add(misses, tclass);
	return E_HM_NOENT;
}

static int ac_query_fast(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
			 struct hmsel_av_decision *avd, struct ac_entry **dest)
{
	unsigned int bucket = ac_hash_fast(ssid, tsid);
	struct hhead *head = ac_cache_head_fast(tclass, bucket);
	struct ac_entry *entry;
	uint8_t active = bucket_active_entry_num(head);
	uint8_t idx;
	for (idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		if (entry_match_fast(ssid, tsid, avd, entry, dest, true)) {
			ac_cache_stat_add(hits, tclass);
			return E_HM_OK;
		}
	}
	ac_cache_stat_add(misses, tclass);
	return E_HM_NOENT;
}

/* if @dest is not NULL, ac_query would rdlock ac_entry.lock,
 * the caller should unlock it later
 */
STATIC int ac_query(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
		    struct hmsel_av_decision *avd, struct ac_entry **dest)
{
	int ret;
	if (tclass <= HM_SECCLASS_FAST && ac_cache_fast[tclass].hheads != NULL) {
		ret = ac_query_fast(ssid, tsid, tclass, avd, dest);
	} else {
		ret = ac_query_normal(ssid, tsid, tclass, avd, dest);
	}
	return ret;
}

static void __free_xpn(struct hmsel_avc_xperms_node *xpn)
{
	struct hmsel_avc_xperms_decision_node *pos = NULL, *next = NULL;
	if (xpn != NULL) {
		dlist_for_each_entry_safe(pos, next, &xpn->xpd_head,
					  struct hmsel_avc_xperms_decision_node, xpd_list) {
			dlist_delete(&pos->xpd_list);
			ac_cache_stat_xperm("xpd node is released, driver=0x%x\n", pos->xpd.driver);
			free(pos->xpd.allowed);
			free(pos->xpd.auditallow);
			free(pos->xpd.dontaudit);
			free(pos);
		}
		ac_cache_stat_xperm("xpn is released\n");
		mcache_hmsel_avc_xperms_node_free(xpn);
	}
}

#define ENTRY_SEQ_FREE (0u)
static int entry_obtain_free(struct ac_entry *entry)
{
	vatomic32_t *v32ptr = (vatomic32_t *)(uintptr_t)(&entry->eseq);
	if (vatomic32_cmpxchg_rlx(v32ptr, ENTRY_SEQ_FREE, 1u) == ENTRY_SEQ_FREE) {
#ifndef __HOST_LLT__
		__smp_wmb();
#endif
		return E_HM_OK;
	}
	return E_HM_AGAIN;
}

static int entry_obtain(struct ac_entry *entry, hmsel_seq_t seq)
{
	vatomic32_t *v32ptr = (vatomic32_t *)(uintptr_t)(&entry->eseq);
	if (vatomic32_cmpxchg_rlx(v32ptr, seq, seq + 1u) == seq) {
#ifndef __HOST_LLT__
		__smp_wmb();
#endif
		return E_HM_OK;
	}
	return E_HM_AGAIN;
}

static inline void
entry_populate_fast(struct ac_entry *entry, hmsel_sid_t ssid, hmsel_sid_t tsid,
		    struct hmsel_av_decision *avd,
		    struct hmsel_avc_xperms_node *xpn)
{
	entry->ssid = ssid;
	entry->tsid = tsid;
	(void)raw_rwlock_wrlock(&entry->xpn_lock);
	struct hmsel_avc_xperms_node *tmp = entry->xp_node;
	entry->xp_node = xpn;
	(void)raw_rwlock_unlock(&entry->xpn_lock);
	if (tmp != NULL) {
		__free_xpn(tmp);
	}
	entry->avd = *avd;
	entry->pac_fast = hm_pacga_ac_entry(entry, true);
}

static void
entry_populate(struct ac_entry *entry, hmsel_sid_t ssid, hmsel_sid_t tsid,
	       hmsel_class_t tclass, struct hmsel_av_decision *avd,
	       struct hmsel_avc_xperms_node *xpn)
{
	entry_populate_fast(entry, ssid, tsid, avd, xpn);
	entry->tclass = tclass;
	entry->pac = hm_pacga_ac_entry(entry, false);
}

static void entry_write_post(struct ac_entry *entry)
{
	vatomic32_t *v32ptr = (vatomic32_t *)(uintptr_t)(&entry->eseq);
	hmsel_seq_t seq = vatomic32_read_rlx(v32ptr);
	vatomic32_write_rel(v32ptr, seq + 1u);
}

static void bucket_add_new_post(struct hhead *head)
{
	vatomic8_t *v8ptr = (vatomic8_t *)(uintptr_t)(&head->entrys[ACTIVE_NUM_ENTRY_IDX].actives);
	vatomic8_inc_rlx(v8ptr);
}

#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
static void override_entry_populate1(struct override_entry *oentry,
				     hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass)
{
	oentry->ssid1 = ssid;
	oentry->tsid1 = tsid;
	oentry->tclass1 = tclass;
}

static void override_entry_populate2(struct override_entry *oentry, struct ac_entry *entry)
{
	oentry->ssid2 = entry->ssid;
	oentry->tsid2 = entry->tsid;
	oentry->tclass2 = entry->tclass;
	oentry->ready = true;
}
#endif

/* update will skip if every entry race fail */
static void bucket_update(struct hhead *head, hmsel_sid_t ssid, hmsel_sid_t tsid,
			  hmsel_class_t tclass, struct hmsel_av_decision *avd,
			  struct hmsel_avc_xperms_node *xpn)
{
	uint8_t idx;
	hmsel_seq_t seq;
	struct ac_entry *entry;
	uint8_t hint = bucket_get_add_hint(head);
	for (idx = 0; idx < HMSEL_ENTRY_SLOTS; idx++, hint++) {
		entry = bucket_entry_of(head, hint % HMSEL_ENTRY_SLOTS);
		seq = entry_seq_read(entry);
		/* someone else is writing this entry, skip */
		if (!entry_seq_prepared(seq)) {
			ac_cache_stat_add(override_read_fails, tclass);
			continue;
		}
		/* someone else is writing this entry, skip */
		if (entry_obtain(entry, seq) != E_HM_OK) {
			ac_cache_stat_add(override_fails, tclass);
			continue;
		}
		ac_cache_stat_add(overrides, tclass);
#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
		uint32_t idx_override = vatomic32_get_inc_rlx((vatomic32_t *)(uintptr_t)(&ac_cache_stats.override_num));
		struct override_entry *oentry = &ac_cache_stats.override_data[idx_override];
		override_entry_populate1(oentry, ssid, tsid, tclass);
		override_entry_populate2(oentry, entry);
#endif
		entry_populate(entry, ssid, tsid, tclass, avd, xpn);
		entry_write_post(entry);
		break;
	}
}

static void ac_add_normal(hmsel_sid_t ssid, hmsel_sid_t tsid,
			  hmsel_class_t tclass, struct hmsel_av_decision *avd,
			  struct hmsel_avc_xperms_node *xpn)
{
	uint8_t idx;
	struct ac_entry *entry;
	unsigned int bucket = ac_hash(ssid, tsid, tclass);
	struct hhead *head = ac_cache_head(bucket);
retry:
	idx = bucket_active_entry_num(head);
	if (idx < HMSEL_ENTRY_SLOTS) {
		entry = bucket_entry_of(head, idx);
		if (likely(entry_obtain_free(entry) == E_HM_OK)) {
			ac_cache_stat_add(allocates, tclass);
			raw_rwlock_init(&entry->xpn_lock, NULL);
			entry_populate(entry, ssid, tsid, tclass, avd, xpn);
			entry_write_post(entry);
			bucket_add_new_post(head);
			return;
		} else {
			goto retry;
		}
	} else {
		bucket_update(head, ssid, tsid, tclass, avd, xpn);
	}
}

static void ac_add_fast(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
			struct hmsel_av_decision *avd,
			struct hmsel_avc_xperms_node *xpn)
{
	uint8_t idx;
	struct ac_entry *entry;
	unsigned int bucket = ac_hash_fast(ssid, tsid);
	struct hhead *head = ac_cache_head_fast(tclass, bucket);
retry:
	idx = bucket_active_entry_num(head);
	if (idx < HMSEL_ENTRY_SLOTS) {
		entry = bucket_entry_of(head, idx);
		if (likely(entry_obtain_free(entry) == E_HM_OK)) {
			ac_cache_stat_add(allocates, tclass);
			raw_rwlock_init(&entry->xpn_lock, NULL);
			entry_populate_fast(entry, ssid, tsid, avd, xpn);
			entry_write_post(entry);
			bucket_add_new_post(head);
			return;
		} else {
			goto retry;
		}
	} else {
		bucket_update(head, ssid, tsid, tclass, avd, xpn);
	}
}

static void __avc_xperms_node_alloc(struct hmsel_avc_xperms_node **xpn)
{
	struct hmsel_avc_xperms_node *tmp = mcache_hmsel_avc_xperms_node_malloc();
	if (tmp == NULL) {
		hm_error("no mem for xperms node\n");
		return;
	}
	mem_zero_s(*tmp);
	dlist_init(&tmp->xpd_head);
	raw_rwlock_init(&tmp->dlist_lock, NULL);
	*xpn = tmp;
}

static void ac_add(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
		   struct hmsel_av_decision *avd,
		   struct hmsel_avc_xperms_node *xpn)
{
	struct hmsel_avc_xperms_node *dest = NULL;
	if (xpn != NULL && xpn->xp.len != 0) {
		__avc_xperms_node_alloc(&dest);
		if (dest != NULL) {
			NOFAIL(memcpy_s(dest->xp.drivers.p,
					sizeof(dest->xp.drivers.p),
					xpn->xp.drivers.p,
					sizeof(xpn->xp.drivers.p)));
			dest->xp.len = xpn->xp.len;
		}
	}
	if (tclass <= HM_SECCLASS_FAST && ac_cache_fast[tclass].hheads != NULL) {
		ac_add_fast(ssid, tsid, tclass, avd, dest);
	} else {
		ac_add_normal(ssid, tsid, tclass, avd, dest);
	}
}

/* only copy decisions of the xperms */
static void __copy_xpermsd(uint8_t xperms,
			   struct hmsel_xperms_decision *dest,
			   struct hmsel_xperms_decision *src)
{
	if (src == NULL || dest == NULL) {
		return;
	}
	uint8_t i = xperms >> 5; // 5: get the index of xperms array
	dest->used = src->used;
	dest->driver = src->driver;
	if (____IS(dest->used & HMSEL_XPERMS_ALLOWED)) {
		dest->allowed->p[i] = src->allowed->p[i];
	}
	if (____IS(dest->used & HMSEL_XPERMS_AUDITALLOW)) {
		dest->auditallow->p[i] = src->auditallow->p[i];
	}
	if (____IS(dest->used & HMSEL_XPERMS_DONTAUDIT)) {
		dest->dontaudit->p[i] = src->dontaudit->p[i];
	}
}

static int
__xperms_decision_node_alloc(struct hmsel_xperms_decision *src,
			     struct hmsel_avc_xperms_decision_node **dest)
{
	struct hmsel_avc_xperms_decision_node *local = malloc(sizeof(*local));
	if (local == NULL) {
		hm_error("no mem for xperms decision node\n");
		return E_HM_NOMEM;
	}
	mem_zero_s(*local);
	dlist_init(&local->xpd_list);

	local->xpd.allowed = malloc(sizeof(struct hmsel_xperms_data));
	if (local->xpd.allowed == NULL) {
		hm_error("no mem for xperms data\n");
		goto free_xpd_node;
	}
	mem_zero_s(*local->xpd.allowed);

	local->xpd.auditallow = malloc(sizeof(struct hmsel_xperms_data));
	if (local->xpd.auditallow == NULL) {
		hm_error("no mem for xperms data\n");
		goto free_allow;
	}
	mem_zero_s(*local->xpd.auditallow);

	local->xpd.dontaudit = malloc(sizeof(struct hmsel_xperms_data));
	if (local->xpd.dontaudit == NULL) {
		hm_error("no mem for xperms data\n");
		goto free_auditallow;
	}
	mem_zero_s(*local->xpd.dontaudit);

	local->xpd.used = src->used;
	local->xpd.driver = src->driver;
	NOFAIL(memcpy_s(local->xpd.allowed->p, sizeof(local->xpd.allowed->p),
			src->allowed->p, sizeof(src->allowed->p)));
	NOFAIL(memcpy_s(local->xpd.auditallow->p, sizeof(local->xpd.auditallow->p),
			src->auditallow->p, sizeof(src->auditallow->p)));
	NOFAIL(memcpy_s(local->xpd.dontaudit->p, sizeof(local->xpd.dontaudit->p),
			src->dontaudit->p, sizeof(src->dontaudit->p)));
	*dest = local;
	return E_HM_OK;

free_auditallow:
	free(local->xpd.auditallow);
free_allow:
	free(local->xpd.allowed);
free_xpd_node:
	free(local);
	return E_HM_NOMEM;
}

static void
ac_update_xpn_normal(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
		     struct hmsel_xperms_decision *src)
{
	unsigned int bucket = ac_hash(ssid, tsid, tclass);
	struct hhead *head = ac_cache_head(bucket);
	struct ac_entry *entry, *tmp_entry;
	struct hmsel_av_decision avd = {0};
	struct hmsel_avc_xperms_decision_node *dest = NULL;
	uint8_t active = bucket_active_entry_num(head);
	for (uint8_t idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		if (entry_match(ssid, tsid, tclass, &avd, entry, &tmp_entry, false) == false) {
			continue;
		}
		if (entry->xp_node == NULL) {
			(void)raw_rwlock_unlock(&entry->xpn_lock); /* rdlock during entry_match */
			break;
		}
		int rc = __xperms_decision_node_alloc(src, &dest);
		if (rc == E_HM_OK) {
			(void)raw_rwlock_wrlock(&entry->xp_node->dlist_lock);
			dlist_insert_head(&entry->xp_node->xpd_head, &dest->xpd_list);
			ac_cache_stat_xperm("add xpd to xpn successfully, driver=0x%x\n",
					    src->driver);
			(void)raw_rwlock_unlock(&entry->xp_node->dlist_lock);
			(void)raw_rwlock_unlock(&entry->xpn_lock); /* rdlock during entry_match */
		}
	}
}

static void
ac_update_xpn_fast(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
		   struct hmsel_xperms_decision *src)
{
	unsigned int bucket = ac_hash_fast(ssid, tsid);
	struct hhead *head = ac_cache_head_fast(tclass, bucket);
	struct ac_entry *entry, *tmp_entry;
	struct hmsel_av_decision avd = {0};
	struct hmsel_avc_xperms_decision_node *dest = NULL;
	uint8_t active = bucket_active_entry_num(head);
	for (uint8_t idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		if (entry_match_fast(ssid, tsid, &avd, entry, &tmp_entry, false) == false) {
			continue;
		}
		if (entry->xp_node == NULL) {
			(void)raw_rwlock_unlock(&entry->xpn_lock); /* wrlock during entry_match_fast */
			break;
		}
		int rc = __xperms_decision_node_alloc(src, &dest);
		if (rc == E_HM_OK) {
			(void)raw_rwlock_wrlock(&entry->xp_node->dlist_lock);
			dlist_insert_head(&entry->xp_node->xpd_head, &dest->xpd_list);
			ac_cache_stat_xperm("add xpd to xpn successfully, driver=0x%x\n",
					    src->driver);
			(void)raw_rwlock_unlock(&entry->xp_node->dlist_lock);
			(void)raw_rwlock_unlock(&entry->xpn_lock); /* wrlock during entry_match_fast */
		}
	}
}

/*
 * Add xperms decision node to xperms node's dlist
 */
static void
ac_update_xpn(struct hmsel_sid_context scon, struct hmsel_xperms_decision *src)
{
	hmsel_class_t tclass = scon.tclass;
	if (tclass <= HM_SECCLASS_FAST && ac_cache_fast[tclass].hheads != NULL) {
		ac_update_xpn_fast(scon.ssid, scon.tsid, scon.tclass, src);
	} else {
		ac_update_xpn_normal(scon.ssid, scon.tsid, scon.tclass, src);
	}
}

#endif

static int do_check_noaudit(struct hmsel_sid_context scon, hmsel_av_t requested,
			    unsigned flags, struct hmsel_av_decision *avd)
{
	int rc = E_HM_OK;
	hmsel_av_t denied;
	if (requested == HMSEL_AVD_NONE) {
		hm_error("requested perms should not be none\n");
		return E_HM_ACCES;
	}
	struct hmsel_avc_xperms_node tmp_xpn = {0};
	struct hmsel_avc_xperms_node *xpn = &tmp_xpn;
	if (!hmsel_initialized()) {
		avd->allowed = HMSEL_AVD_ALL;
		avd->auditallow = HMSEL_AVD_NONE;
		avd->flags = 0;
		return E_HM_OK;
	}
#ifndef CONFIG_SEHARMONY_AC_CACHE
	hmsel_compute_av(scon, avd, requested, &xpn->xp);
#else
	int rc2 = ac_query(scon.ssid, scon.tsid, scon.tclass, avd, NULL);
	if (rc2 != E_HM_OK) {
		hmsel_compute_av(scon, avd, requested, &xpn->xp);
		ac_add(scon.ssid, scon.tsid, scon.tclass, avd, xpn);
	}
#endif
	denied = requested & ~(avd->allowed);
	if (unlikely(denied != 0)) {
		rc = hmsel_avc_denied(flags, avd);
	}
	return rc;
}

int hmsel_avc_check_noaudit(struct hmsel_sid_context scon, hmsel_av_t requested,
			       unsigned flags, struct hmsel_av_decision *avd)
{
	if (avd == NULL) {
		hm_error("failed to check param, avd is NULL\n");
		return E_HM_INVAL;
	}
	return do_check_noaudit(scon, requested, flags, avd);
}

bool hmsel_sid_context_is_equal(hmsel_sid_t ssid, hmsel_sid_t tsid)
{
	struct hmsel_context *scon = hmsel_sid_to_context_val(ssid);
	struct hmsel_context *tcon = hmsel_sid_to_context_val(tsid);
	if (scon != NULL && tcon != NULL) {
		return (hmsel_context_cmp(scon, tcon) == 0);
	}
	return false;
}

static int do_avc_check(struct hmsel_sid_context scon, hmsel_av_t requested, const struct sec_sel_audit *ad,
			hmsel_av_t auditdeny)
{
	struct hmsel_av_decision avd;
	int rc = do_check_noaudit(scon, requested, 0, &avd);
	int rc2 = hmsel_avc_audit(scon, requested, &avd, rc, ad, auditdeny);
	return (rc2 != E_HM_OK) ? rc2 : rc;
}

int hmsel_avc_check(struct hmsel_sid_context scon, hmsel_av_t requested)
{
	return do_avc_check(scon, requested, NULL, 0);
}

static unsigned int
__xperms_has_perm(const struct hmsel_xperms_decision *xpd, uint8_t xperms, uint8_t type)
{
	unsigned int rc = 0;
	switch (type) {
	case HMSEL_XPERMS_ALLOWED:
		if (____IS(xpd->used & HMSEL_XPERMS_ALLOWED)) {
			rc = hmsel_xperm_test(xpd->allowed->p, xperms);
		}
		break;
	case HMSEL_XPERMS_AUDITALLOW:
		if (____IS(xpd->used & HMSEL_XPERMS_AUDITALLOW)) {
			rc = hmsel_xperm_test(xpd->auditallow->p, xperms);
		}
		break;
	case HMSEL_XPERMS_DONTAUDIT:
		if (____IS(xpd->used & HMSEL_XPERMS_DONTAUDIT)) {
			rc = hmsel_xperm_test(xpd->dontaudit->p, xperms);
		}
		break;
	default:
		rc = 0;
	}
	return rc;
}


static hmsel_av_t
hmsel_avc_xperms_audit_required(hmsel_av_t requested,
				struct hmsel_av_decision *avd,
				int res, struct hmsel_xperms_decision *xpd,
				uint16_t cmd, struct hmsel_sid_context scon, hmsel_av_t *deniedp)
{
	hmsel_av_t denied = requested & ~(avd->allowed);
	hmsel_av_t audited = 0;
	uint8_t xperms = cmd & 0xff;
	if (unlikely(denied != 0)) {
		audited = denied & avd->auditdeny;
		if ((audited != 0 && xpd != 0) &&
		    __xperms_has_perm(xpd, xperms, HMSEL_XPERMS_DONTAUDIT) != 0) {
			audited &= ~requested;
		}
	} else if (res != E_HM_OK) {
		audited = denied = requested;
	} else {
		audited = requested & avd->auditallow;
		if ((audited != 0 && xpd != 0) &&
		    __xperms_has_perm(xpd, xperms, HMSEL_XPERMS_AUDITALLOW) != 0) {
			audited &= ~requested;
		}
	}
	*deniedp = denied;
#ifdef CONFIG_SEHARMONY_LOG_RESTRICT
	if (audited != 0 && hmsel_log_restrict()) {
		avc_log_rate_limit_xperm(avd, scon, cmd, &audited);
	}
#endif
	return audited;
}

static int
hmsel_avc_xperms_audit(struct hmsel_sid_context scon, hmsel_av_t requested,
		       struct hmsel_av_decision *avd, int res,
		       const struct sec_sel_audit *ad,
		       struct hmsel_xperms_decision *xpd,
		       uint16_t cmd)
{
	struct hmsel_audit_data aud_data = {0};

	aud_data.result = res;
	aud_data.requested = requested;
	aud_data.audited = hmsel_avc_xperms_audit_required(aud_data.requested, avd, aud_data.result, xpd,
							   cmd, scon, &aud_data.denied);
	if (likely(aud_data.audited == 0)) {
		return E_HM_OK;
	}
	return avc_audit_slow(scon, &aud_data, ad);
}

static struct hmsel_xperms_decision *
__xperms_decision_lookup(uint8_t driver, struct hmsel_avc_xperms_node *xpn)
{
	struct hmsel_avc_xperms_decision_node *xpd_node = NULL;

	dlist_for_each_entry(xpd_node, &xpn->xpd_head,
			     struct hmsel_avc_xperms_decision_node, xpd_list) {
		if (xpd_node->xpd.driver == driver) {
			return &xpd_node->xpd;
		}
	}

	return NULL;
}

int hmsel_avc_xperms_query(struct hmsel_sid_context scon, hmsel_av_t req,
			   uint8_t driver, uint8_t xperms,
			   const struct sec_sel_audit *ad)
{
	int rc = E_HM_ACCES;
	int rc2 = 0;
	hmsel_av_t denied;
	struct hmsel_av_decision avd = {0};
	struct hmsel_avc_xperms_node *xpn = NULL;
	struct hmsel_avc_xperms_node tmp_xpn = {0};
	struct hmsel_xperms_decision *xpd = NULL;
	struct hmsel_xperms_decision tmp_xpd = {0};
	struct hmsel_xperms_data allowed = {0};
	struct hmsel_xperms_data auditallow = {0};
	struct hmsel_xperms_data dontaudit = {0};
	uint16_t cmd = xperms + ((uint16_t)driver << 8);
	if (!hmsel_initialized()) {
		return E_HM_OK;
	}
	if (req == HMSEL_AVD_NONE) {
		hm_error("req perms should not be none\n");
		return E_HM_ACCES;
	}
	xpn = &tmp_xpn;
#ifdef CONFIG_SEHARMONY_AC_CACHE
	struct ac_entry *entry = NULL;
	rc = ac_query(scon.ssid, scon.tsid, scon.tclass, &avd, &entry);
	if (rc != E_HM_OK) {
		dlist_init(&xpn->xpd_head);
		raw_rwlock_init(&xpn->dlist_lock, NULL);
		/* compute drivers, xp.len is set to 1 initially if allowxperm existed */
		hmsel_compute_av(scon, &avd, req, &xpn->xp);
		ac_add(scon.ssid, scon.tsid, scon.tclass, &avd, xpn); // no lock
		ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, add entry to avc\n",
				    scon.ssid, scon.tsid, driver, xperms);
	} else {
		/* entry->xpn_lock is rdlocked during ac_query */
		xpn = entry->xp_node;
		ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, get xpn from avc\n",
				    scon.ssid, scon.tsid, driver, xperms);
	}
#else
	hmsel_compute_av(scon, &avd, req, &xpn->xp);
#endif

	/* no driver is flagged */
	if (xpn == NULL || xpn->xp.len == 0) {
#ifdef CONFIG_SEHARMONY_AC_CACHE
		if (rc == E_HM_OK) { /* xpn is from avc */
			(void)raw_rwlock_unlock(&entry->xpn_lock);
			ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, no driver is flagged\n",
					    scon.ssid, scon.tsid, driver, xperms);
		}
#endif
		goto out;
	}
	tmp_xpd.allowed = &allowed;
	tmp_xpd.auditallow = &auditallow;
	tmp_xpd.dontaudit = &dontaudit;
#ifdef CONFIG_SEHARMONY_AC_CACHE
	if (rc == E_HM_OK) { // xpn is from ac_query, xpn_lock is rdlocked
		(void)raw_rwlock_rdlock(&xpn->dlist_lock);
	}
#endif
	xpd = __xperms_decision_lookup(driver, xpn); // xpn is from ac_query rdlocked, or is a tmp_xpd
	if (unlikely(xpd == NULL)) {
		/* check if the driver is flagged in the bitmap */
		rc2 = (int)hmsel_xperm_test(xpn->xp.drivers.p, driver);
#ifdef CONFIG_SEHARMONY_AC_CACHE
		if (rc == E_HM_OK) {
			(void)raw_rwlock_unlock(&xpn->dlist_lock);
			(void)raw_rwlock_unlock(&entry->xpn_lock);
			ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, xpd not found in xpn\n",
					    scon.ssid, scon.tsid, driver, xperms);
		}
#endif
		if (rc2 == 0) { // driver is not matched in bitmap
			ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, no driver is flagged\n",
					    scon.ssid, scon.tsid, driver, xperms);
			avd.allowed &= ~req;
			goto out;
		}
		hmsel_compute_xperms_decisions(scon, driver, &tmp_xpd);
#ifdef CONFIG_SEHARMONY_AC_CACHE
		ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, insert xpd to xpn\n",
				    scon.ssid, scon.tsid, driver, xperms);
		ac_update_xpn(scon, &tmp_xpd);
#endif
	} else {
		/* copy from xpd to tmp_xpd */
		__copy_xpermsd(xperms, &tmp_xpd, xpd);
#ifdef CONFIG_SEHARMONY_AC_CACHE
		if (rc == E_HM_OK) {
			(void)raw_rwlock_unlock(&xpn->dlist_lock);
			(void)raw_rwlock_unlock(&entry->xpn_lock);
			ac_cache_stat_xperm("ssid=%u, tsid=%u, driver=0x%x, xperms=0x%x, found xpd in xpn\n",
					    scon.ssid, scon.tsid, driver, xperms);
		}
#endif
	}
	xpd = &tmp_xpd;
	/* 5. check if xperm has perms */
	if (__xperms_has_perm(xpd, xperms, HMSEL_XPERMS_ALLOWED) == 0) {
		avd.allowed &= ~req;
	}
out:
	rc = E_HM_OK;
	denied = req & ~(avd.allowed);
	if (unlikely(denied != 0)) {
		rc = hmsel_avc_denied(0, &avd);
	}
	/* 6. audit xperms */
	rc2 = hmsel_avc_xperms_audit(scon, req, &avd, rc, ad, xpd, cmd);
	return (rc2 != E_HM_OK) ? rc2 : rc;
}

int hmsel_avc_query(struct hmsel_sid_context scon, hmsel_av_t requested, const struct sec_sel_audit *ad)
{
	return do_avc_check(scon, requested, ad, 0);
}

int hmsel_avc_query_audit(struct hmsel_sid_context scon, hmsel_av_t requested, const struct sec_sel_audit *ad,
			  hmsel_av_t auditdeny)
{
	return do_avc_check(scon, requested, ad, auditdeny);
}

#define POSIX_CAP_BIT 5
#define POSIX_CAP_MASK 31
#define POSIX_CAP_IDX(cap) ((unsigned int)(cap) >> POSIX_CAP_BIT)
#define POSIX_CAP_AV(cap) (1u << ((cap) & POSIX_CAP_MASK))

static hmsel_class_t cap_class_of(int cap, bool initns)
{
	if (POSIX_CAP_IDX(cap) == 0u) {
		return initns ? HM_SECCLASS_CAPABILITY : HM_SECCLASS_CAP_USERNS;
	} else if (POSIX_CAP_IDX(cap) == 1u) {
		return initns ? HM_SECCLASS_CAPABILITY2 : HM_SECCLASS_CAP2_USERNS;
	} else {
		hm_error("invalid seharmony capability %d\n", cap);
	}
	return HM_SECCLASS_NULL;
}

static void cap_logging(const void *cred, const void *data, struct hmsel_audit_info *ai)
{
	UNUSED(cred);
	int ret;
	int key;

	if (data != NULL && ai != NULL) {
		key = *(int *)data;
		ret = snprintf_s(ai->buf, ai->len, ai->len - 1, " capability=%d", key);
		ai_update(ret, ai);
	}
}

int hmsel_cap_check(const struct sec_chk_cred *cred, hmsel_sid_t sid, int cap, unsigned int opts, bool initns)
{
	UNUSED(cred);
	int rc, capability;
	int rcaudit;
	struct hmsel_av_decision avd;
	struct hmsel_sid_context scon = {
		.ssid = sid,
		.tsid = sid,
		.tclass = cap_class_of(cap, initns),
	};
	rc = do_check_noaudit(scon, POSIX_CAP_AV((unsigned int)cap), 0, &avd);

	capability = cap;
	const struct sec_sel_audit ad = {
		.cred = NULL,
		.func = cap_logging,
		.data = &capability,
	};
	if ((opts & HM_CAP_OPT_NOAUDIT) == 0) {
		rcaudit = hmsel_avc_audit(scon, POSIX_CAP_AV(cap), &avd, rc, &ad, 0);
		if (rcaudit != E_HM_OK) {
			return rcaudit;
		}
	}

	return rc;
}

#ifdef CONFIG_SEHARMONY_LOG_RESTRICT
static void ac_update_audited_xperm(struct ac_entry *entry, uint16_t cmd)
{
	if (cmd == 0) {
		return;
	}
	if (entry->avd.cmd_len < HMSEL_XPERM_CACHE_MAX) {
		entry->avd.cmd[entry->avd.cmd_len] = cmd;
		entry->avd.cmd_len++;
		return;
	}
	entry->avd.cmd[entry->avd.cmd_idx] = cmd;
	entry->avd.cmd_idx++;
	if (entry->avd.cmd_idx > HMSEL_XPERM_CACHE_MAX - 1) {
		entry->avd.cmd_idx = 0;
	}
}

static void ac_update_audited_normal(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
				     struct hmsel_av_decision *src, uint16_t cmd)
{
	unsigned int bucket = ac_hash(ssid, tsid, tclass);
	struct hhead *head = ac_cache_head(bucket);
	struct ac_entry *entry = NULL;
	struct hmsel_avc_pac_result result;
	uint8_t active = bucket_active_entry_num(head);
	hmsel_seq_t seq;
	for (uint8_t idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		seq = entry_seq_read(entry);
#ifndef __HOST_LLT__
		__smp_rmb();
#endif
		if (!entry_cmp(ssid, tsid, tclass, entry, &result)) {
			/* entry_cmp failed, skip pac checking of this entry */
			continue;
		}
		/* someone else is writing this entry, skip */
		if (!entry_seq_prepared(seq) ||
		    entry_obtain(entry, seq) != E_HM_OK) {
			break;
		}
		hm_pacga_ac_entry_check(entry, &result);
		entry->avd.audited = src->audited;
		ac_update_audited_xperm(entry, cmd);
		entry_write_post(entry);
		break;
	}
}

static void ac_update_audited_fast(hmsel_sid_t ssid, hmsel_sid_t tsid, hmsel_class_t tclass,
				   struct hmsel_av_decision *src, uint16_t cmd)
{
	unsigned int bucket = ac_hash_fast(ssid, tsid);
	struct hhead *head = ac_cache_head_fast(tclass, bucket);
	struct ac_entry *entry = NULL;
	struct hmsel_avc_pac_result result;
	uint8_t active = bucket_active_entry_num(head);
	hmsel_seq_t seq;
	for (uint8_t idx = 0; idx < active; idx++) {
		entry = bucket_entry_of(head, idx);
		seq = entry_seq_read(entry);
#ifndef __HOST_LLT__
		__smp_rmb();
#endif
		if (!entry_cmp_fast(ssid, tsid, entry, &result)) {
			/* entry_cmp failed, skip pac checking of this entry */
			continue;
		}
		/* someone else is writing this entry, skip */
		if (!entry_seq_prepared(seq) ||
		    entry_obtain(entry, seq) != E_HM_OK) {
			break;
		}

		hm_pacga_ac_entry_check(entry, &result);
		entry->avd.audited = src->audited;
		ac_update_audited_xperm(entry, cmd);
		entry_write_post(entry);
		break;
	}
}

static void update_avc_audited(struct hmsel_av_decision *avd, struct hmsel_sid_context scon, uint16_t cmd)
{
	hmsel_class_t tclass = scon.tclass;
	if (tclass <= HM_SECCLASS_FAST && ac_cache_fast[tclass].hheads != NULL) {
		ac_update_audited_fast(scon.ssid, scon.tsid, scon.tclass, avd, cmd);
	} else {
		ac_update_audited_normal(scon.ssid, scon.tsid, scon.tclass, avd, cmd);
	}
}

static void avc_log_rate_limit(struct hmsel_av_decision *avd,
			  struct hmsel_sid_context scon, hmsel_av_t *audited)
{
	hmsel_av_t audit = *audited;
	if ((audit & avd->audited) == audit) {
		*audited = 0;
		return;
	}
	avd->audited |= audit;
	update_avc_audited(avd, scon, 0);
}

static void avc_log_rate_limit_xperm(struct hmsel_av_decision *avd,
				     struct hmsel_sid_context scon, uint16_t cmd, hmsel_av_t *audited)
{
	for (uint8_t i = 0; i < avd->cmd_len; i++) {
		if (avd->cmd[i] == cmd) {
			*audited = 0;
			return;
		}
	}
	update_avc_audited(avd, scon, cmd);
}
#endif
