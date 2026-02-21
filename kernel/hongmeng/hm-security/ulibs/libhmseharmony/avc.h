/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for ac cache
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 08 11:49:29 2023
 */

#ifndef HMSEL_AVC_H
#define HMSEL_AVC_H

#include <libhmsync/raw_rwlock.h>
#include <libhmseharmony/pac.h>
#define HMSEL_BUCKET_SLOTS 512
#define HMSEL_ENTRY_SLOTS 4

typedef uint32_t hmsel_seq_t;

/* size of ac_entry is 32, so every ac_entry will fall into a single cache line(64B) */
struct ac_entry {
	hmsel_seq_t eseq; /* hash list entry sequence number */
	hmsel_sid_t ssid;
	hmsel_sid_t tsid;
	hmsel_class_t tclass;

	/* actives lies in struct padding space because of alignment,
	 * so add an uint8_t member won't increase total struct size.
	 * we use this member to record the bucket's active entry num. */
	uint8_t actives;
	struct hmsel_av_decision avd;
	__sec_data(avc, ptr) struct hmsel_avc_xperms_node *xp_node;
	struct raw_rwlock xpn_lock;
	uint32_t pac;
	uint32_t pac_fast;
};

struct hhead {
	struct ac_entry entrys[HMSEL_ENTRY_SLOTS];
};

struct hmsel_ac_cache {
	__sec_data(avc, ptr) struct hhead *hheads;
};

int hmsel_ac_cache_init(unsigned int service);
void hmsel_ac_cache_fini(void);

static inline struct hmsel_av_decision *ac_entry_avd_of(struct ac_entry *entry)
{
	return &entry->avd;
}

#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT

#define OVERRIDE_ENTRY_MAX 512
struct override_entry {
	hmsel_sid_t ssid1;
	hmsel_sid_t tsid1;
	hmsel_sid_t ssid2;
	hmsel_sid_t tsid2;
	hmsel_class_t tclass1;
	hmsel_class_t tclass2;
	bool ready;
};

struct ac_cache_stat {
	uint64_t misses[HM_SECCLASS_NUM];
	uint64_t hits[HM_SECCLASS_NUM];
	uint64_t allocates[HM_SECCLASS_NUM];
	uint64_t overrides[HM_SECCLASS_NUM];
	uint64_t override_fails[HM_SECCLASS_NUM];
	uint64_t override_read_fails[HM_SECCLASS_NUM];
	uint64_t entry_reads[HM_SECCLASS_NUM];
	uint64_t entry_read_fails[HM_SECCLASS_NUM];
	uint32_t override_num;
	__sec_data(seharmony, ptr) struct override_entry *override_data;
};
#endif

#endif
