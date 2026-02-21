/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: APIs of libpolicy provided by security service
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 19 11:01:37 CST 2019
 */
#ifndef __LIBPOLICY_API_H_
#define __LIBPOLICY_API_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <policy_def.h>
#include <hmkernel/strict.h>
#include <hongmeng/panic.h>
#include <libsecplc/policy_fmt.h>
#include <inttypes.h>
#include <libhmsync/atomic.h>

typedef uint64_t type_id_t;
#define SEC_PLC_RULE_AC		1U // print action rules
#define SEC_PLC_RULE_AB		2U // print ability rules
#define SEC_PLC_RULE_FILEOBJ	3U // print file object defition rules
#define SEC_PLC_RULE_TRANS	4U // print type transition rules
#define SEC_PLC_RULE_PROPERTY	5U // print type transition rules
#define SEC_PLC_RULE_ALL	0xFF

#define sec_plc_rule_set(val, n) ((val) |= 1U << (n))
#define sec_plc_rule_is_on(val, n) (((val) >> (n)) & 1U)
struct sec_plc_rule_criteria {
	uint8_t type;
	uint64_t sub;
	uint64_t obj;
	uint64_t target;
	uint64_t generic; // type can be applied to sub, obj, target
	const char *obj_name;
};

void sec_plc_dump(const struct sec_plc_rule_criteria *criteria);

int get_sub_id_by_path(const char *path, uint64_t *sub_id);
int sec_plc_get_sid_with_prefix(const char *path, const char *prefix, uint64_t *sub_id);
bool get_type_transition(uint64_t sub_id, uint64_t obj_id, enum action_def act_id, uint64_t *tgt_id);
bool check_te_policy(uint64_t sub_type, uint64_t obj_type,
		     uint64_t act);
bool check_ability(uint64_t sub_type, enum ability_def ablty);
bool check_mrange(uint64_t sub_type, mrange_t *mrange);
int sec_plc_check_policy_consistency(void);
int policy_init(void *policy, size_t sz);
void *policy_mmap(const char *file_name, size_t *sz);
void policy_munmap(void *addr, size_t sz);
int check_type_id(type_id_t id);
bool check_and_export_abilities(uint64_t sub_type, ability_t *exp_ablty, ability_t *avail_ablty,
				ability_t *log_avial_ablty);
const char *sec_plc_def2str(uint64_t sub_id, enum POLICY_DESC_TYPE policy_type);
uint64_t sec_plc_str2def(const char *type_name);
bool sec_plc_type_is_permissive_subj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_type_is_permissive_obj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_type_is_no_log_subj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_type_is_no_log_obj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_type_is_cache_subj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_type_is_cache_obj(uint64_t type_id, uint32_t *p_cached_property);
bool sec_plc_is_internal_ab(enum ability_def ability);
int sec_plc_get_internal_abs(ability_t *ability);

static inline bool ability_set_check(const ability_t *ablty, enum ability_def ability)
{
	uint64_t i = (uint64_t)ability / ABILITY_BITS_PER_ELEMENT;

	return ____IS(ablty->ability[i] & ((uint64_t)1 << ((uint64_t)ability % ABILITY_BITS_PER_ELEMENT)));
}

static inline void ability_set_add(ability_t *ablty, enum ability_def ability)
{
	uint64_t i = (uint64_t)ability / ABILITY_BITS_PER_ELEMENT;

	ablty->ability[i] = ablty->ability[i] | ((uint64_t)1 << ((uint64_t)ability % ABILITY_BITS_PER_ELEMENT));

	return;
}

static inline const char* sec_plc_get_type_name(uint64_t type_id)
{
	return sec_plc_def2str(type_id, TYPY_DEF_DESC);
}

static inline const char* sec_plc_get_ability_name(uint64_t ability_id)
{
	return sec_plc_def2str(ability_id, ABILITY_DEF_DESC);
}

static inline const char* sec_plc_get_action_name(uint64_t action_id)
{
	return sec_plc_def2str(action_id, ACTION_DEF_DESC);
}

bool sec_plc_check_port_range(uint64_t sub_type, const sec_plc_port_range_t *range);
int sec_plc_iter_port_range_start(enum type_def subj, struct sec_plc_port_range_iterator *it);
void sec_plc_iter_port_range_next(struct sec_plc_port_range_iterator *it);
int sec_plc_iter_port_range_get(const struct sec_plc_port_range_iterator *it,
				sec_plc_port_range_t *range);
int sec_plc_port_range_get_reserved(struct sec_plc_port_range_iterator *it);

int sec_plc_set_mode(unsigned int mode);
int sec_policy_chksum(uint8_t* chksum, size_t len);
int sec_u8_to_hex_str(char* str_chksum, size_t str_len, const uint8_t* chksum, size_t len);
unsigned int sec_plc_mode(void);
const char* sec_plc_mode2str(unsigned int plc_mode);
bool sec_plc_mode_is_disable(void);
bool sec_plc_mode_is_permissive(void);
bool sec_plc_mode_is_pmsv_cache(void);  // permissive_cache mode

int sec_plc_check_abilities(uint64_t sub_type, ability_t *exp_ablty, ability_t *avail_ablty);

int sec_plc_get_avail_abilities(uint64_t sub_type, ability_t *avail_ablty);

static inline void hmmac_type_get(uint64_t *cache, const char *type_name)
{
	if (cache != NULL) {
		if (*cache == 0) {
			uint64_t t = sec_plc_str2def(type_name);
			raw_atomic_xchg64(cache, t);
		}
	}
}
#define hmmac_type(type) ({ static uint64_t cache = 0; hmmac_type_get(&cache, #type); cache;})

#define POLICY_NAME "policy.db"
#define POLICY_SHM "policydb"
#endif
