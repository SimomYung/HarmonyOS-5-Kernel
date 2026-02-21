/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Read policy file and get policy info to init mode
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */

#include <fcntl.h>
#include <stdio.h>
#include <endian.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <policy_def.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_log.h>
#include <libhwsecurec/securec.h>
#include "libpolicy_api.h"
#include "fncmp.h"

#define SEC_LIB_PORT_RANGE_MAX_NUM 65535u
#ifdef CONFIG_INNER_MAC
#define BIT_NUMBER_OF_BYTE 8u
#endif

#define PLC_PERM_CACHE	"permissive_cache"
#define PLC_PERM	"permissive"
#define PLC_DISABLE	"disable"
#define PLC_ENFORCE	"enforce"
/*
 * Check use-after-free:
 * The base will init in policy_init(), and it point to memory which mmaped
 * by policy file. So caller should use local_sec between policy_init() and
 * munmap() to avoid UAF.
 */
struct local_sec {
	void *base;
	uint64_t sz;
};

static struct local_sec __ro_after_init sec_base[MAX_SEC] ;
static uint32_t _property_of(uint64_t type_id);

#ifdef SEC_CHK_PERMISSIVE
static unsigned int __ro_after_init g_sec_plc_mode = SEC_PLC_PERMISSIVE;
#else
static unsigned int __ro_after_init g_sec_plc_mode = SEC_PLC_ENFORCE;
#endif

#define eprintf(...) (void)fprintf(stderr, __VA_ARGS__)

int sec_plc_set_mode(unsigned int mode)
{
	g_sec_plc_mode = mode;
	return E_HM_OK;
}

unsigned int sec_plc_mode(void)
{
	return g_sec_plc_mode;
}

bool sec_plc_mode_is_disable(void)
{
	return ____IS(g_sec_plc_mode & SEC_PLC_DISABLE);
}

bool sec_plc_mode_is_permissive(void)
{
	return ____IS(g_sec_plc_mode & SEC_PLC_PERMISSIVE);
}

bool sec_plc_mode_is_pmsv_cache(void)
{
	return (g_sec_plc_mode == (SEC_PLC_PERMISSIVE | SEC_PLC_CACHE));
}

static bool is_reserved_type(enum SEC_TYPE type)
{
	if (type >= EXTYPE && type <= EXACTION) {
		return true;
	}

	return false;
}

static bool is_file_header_valid(const void *file, uint64_t sec_num, size_t filesize)
{
	uint64_t file_head_len;
	uint64_t sec_hdr_size = le64toh(((struct hdr*)file)->sec_hdr.sz);

	/* Unlikely, sec_size * sec_num will overflow */
	if (sec_num > UINT64_MAX / sizeof(struct sec)) {
		return false;
	}
	/*
	 * We check the correctness of sec_hdr.sz and make sure
	 * no out of boundary access here
	 */
	file_head_len = sec_num * sizeof(struct sec);
	if (file_head_len > (uint64_t)(filesize - sizeof(struct hdr)) ||
	    file_head_len != sec_hdr_size) {
		return false;
	}

	return true;
}

static bool is_sec_data_valid(const struct sec *cur_sec, uint64_t filesize)
{
	uint64_t sec_type, sec_off, sec_size;

	if (cur_sec == NULL) {
		return false;
	}
	sec_type = le64toh(cur_sec->type);
	sec_off = le64toh(cur_sec->off);
	sec_size = le64toh(cur_sec->sz);
	if (sec_type >= (uint64_t)MAX_SEC) {
		return false;
	}

	if (filesize < sec_size || sec_off == 0U ||
	    sec_off > filesize - sec_size) {
		return false;
	}

	return true;
}

static int locate_secs(void *file, size_t filesize)
{
	uint64_t i;
	int rc = E_HM_OK;
	struct sec *_sec = NULL;
	uint64_t sec_num = 0;

	if (filesize < sizeof(struct hdr)) {
		return -1;
	}
	_sec = u64_to_ptr(ptr_to_u64(file) + sizeof(struct hdr), void);
	sec_num = le64toh(((struct hdr*)file)->sec_hdr.sec_num);
	/*
	 * It is necessary to check all variables in file's header before use
	 * them, if we assume the source file is untrusted
	 */
	if (!is_file_header_valid(file, sec_num, filesize)) {
		return -1;
	}

	sec_base[HDR].base = file;
	sec_base[SECTBL].sz = le64toh(((struct hdr *)file)->sec_hdr.sz);
	sec_base[SECTBL].base = ulong_to_ptr(ptr_to_type(file, uintptr_t) + sizeof(struct hdr), void);
	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach sec_num. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: sec_num is less than 20 according to enum SEC_TYPE,
	 * so loop condition is reasonable.
	 */
	for (i = 0; i < sec_num; i++) {
		/* Check each sec before load them to local sec_base */
		if (!is_sec_data_valid(&_sec[i], (uint64_t)filesize)) {
			hm_error("policy file is corrupted\n");
			rc = -1;
			break;
		} else {
			sec_base[le64toh(_sec[i].type)].sz = le64toh(_sec[i].sz);
			sec_base[le64toh(_sec[i].type)].base = (void *)((char *)file + le64toh(_sec[i].off));
		}
	}

	if (rc == E_HM_OK) {
		for (unsigned type = HDR; type < MAX_SEC; type++) {
			if (sec_base[type].base == NULL && !is_reserved_type(type)) {
				rc = -1;
				break;
			}
		}
	}

	return rc;
}

/*
 * If tgt == null_type, it means the search filed is not set by cmd line.
 * @src: field set by rules in policy db
 * @tgt: criteria field set by command line
 */
static bool cmp_rule_field(uint64_t src, uint64_t tgt)
{
	if (tgt == null_type) {
		return true;
	} else {
		return src == tgt;
	}
}

static bool cmp_rule_str(const char *src, const char *tgt)
{
	if (tgt == NULL) {
		return true;
	}
	if (src == NULL) {
		return false;
	}
	return strcmp(src, tgt) == 0;
}

/*
 * Returns true when all rule fields can match with the search criteria
 * @rule: rule from policydb
 * @search: criteria set by command line
 */
static bool rule_filter(const struct sec_plc_rule_criteria *rule,
			const struct sec_plc_rule_criteria *search)
{
	return cmp_rule_field(rule->sub, search->sub) &&
	       cmp_rule_field(rule->obj, search->obj) &&
	       cmp_rule_field(rule->target, search->target) &&
	       (cmp_rule_field(rule->sub, search->generic) ||
		cmp_rule_field(rule->obj, search->generic) ||
		cmp_rule_field(rule->target, search->generic) ||
		cmp_rule_field(rule->generic, search->generic)) &&
	       cmp_rule_str(rule->obj_name, search->obj_name);
}

static void reset_criteria(struct sec_plc_rule_criteria *criteria)
{
	mem_zero_b(criteria, sizeof(*criteria));
	criteria->sub = null_type;
	criteria->obj = null_type;
	criteria->target = null_type;
	criteria->generic = null_type;
	criteria->obj_name = NULL;
}

static void dump_te_rule(const struct sec_plc_rule_criteria *criteria)
{
	struct policy_hdr_unit *phu = NULL, *upper_phu = NULL;
	struct policy_unit *pu = NULL, *upper_pu = NULL;
	struct sec_plc_rule_criteria rule;

	phu = (struct policy_hdr_unit *)(sec_base[POLICY_HDR].base);
	upper_phu = phu + (sec_base[POLICY_HDR].sz / sizeof(*phu)) - 1 ;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when phu reach upper_phu. Since phu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_phu is counted base on the size of POLICY_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; phu <= upper_phu; phu++) {
		pu = (struct policy_unit *)(uintptr_t)((char *)sec_base[POLICY_UNIT].base +
			le64toh(phu->off));
		upper_pu = pu + (le64toh(phu->sz) / sizeof(*pu)) - 1 ;
		/*
		 * Loop termination condition:
		 * Following loop will terminate when pu reach upper_pu. Since pu is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: upper_pu is counted base on the size of phu,
		 * which is limited, so loop condition is reasonable.
		 */
		for (; pu <= upper_pu; pu++) {
			reset_criteria(&rule);
			rule.sub = le64toh(phu->sub_id);
			rule.obj = le64toh(pu->obj_id);
			if (rule_filter(&rule, criteria) == false) {
				continue;
			}
			(void)hm_log("action: %s %s %s\n",
				     sec_plc_def2str(le64toh(phu->sub_id), TYPY_DEF_DESC),
				     sec_plc_def2str(le64toh(pu->obj_id), TYPY_DEF_DESC),
				     sec_plc_def2str(le64toh(pu->act_id), ACTION_DEF_DESC));
		}
	}
	return;
}

static void print_abilities(uint64_t sub_id, const ability_t *ab)
{
	uint32_t i;
	uint64_t one_idx = ABILITY_BITS_PER_ELEMENT;

	for (i = 0; i < ABILITY_WIDTH; i++) {
		uint64_t tmp = le64toh(ab->ability[i]);
		one_idx = raw_bitmap_find_first_one(ptr_to_type(&tmp, unsigned long long*),
						    (unsigned int)ABILITY_BITS_PER_ELEMENT);
		while (one_idx < ABILITY_BITS_PER_ELEMENT) {
			(void)hm_log("ability:%s %s%s\n",
				     sec_plc_def2str(sub_id, TYPY_DEF_DESC),
				     sec_plc_get_ability_name(one_idx + ABILITY_BITS_PER_ELEMENT * i),
				     sec_plc_is_internal_ab((enum ability_def)
				     (one_idx + ABILITY_BITS_PER_ELEMENT * i)) ? " (internal)" : "");
			tmp = le64toh(ab->ability[i]);
			one_idx = raw_bitmap_find_next_one(
					ptr_to_type(&tmp, unsigned long long*),
					(unsigned int)ABILITY_BITS_PER_ELEMENT, (unsigned int)one_idx);
		}
	}
}

static void dump_ab_rule(const struct sec_plc_rule_criteria *criteria)
{
	struct ability_hdr_unit *ahu = NULL, *upper_ahu = NULL;
	struct mrange_unit *mu = NULL, *upper_mu = NULL;
	struct sec_plc_port_range_unit *pu = NULL, *upper_pu = NULL;
	struct sec_plc_rule_criteria rule;

	ahu = (struct ability_hdr_unit *)(sec_base[ABILITY_HDR].base);
	upper_ahu = ahu + (sec_base[ABILITY_HDR].sz / sizeof(*ahu)) - 1 ;

	for (; ahu <= upper_ahu; ahu++) {
		reset_criteria(&rule);
		rule.sub = le64toh(ahu->sub_id);
		if (rule_filter(&rule, criteria) == false) {
			continue;
		}
		print_abilities(le64toh(ahu->sub_id), &ahu->ability);
		mu = (struct mrange_unit *)(uintptr_t)((char *)sec_base[MRANGE_UNIT].base +
			le64toh(ahu->mrange_off));
		upper_mu = mu + (le64toh(ahu->mrange_sz) / sizeof(*mu)) - 1 ;
		for (; mu <= upper_mu; mu++) {
			(void)hm_log("ability:%s mem_range[0x%"PRIx64", 0x%"PRIx64")\n",
				     sec_plc_def2str(le64toh(ahu->sub_id), TYPY_DEF_DESC),
				     le64toh(mu->start), le64toh(mu->end));
		}
		pu = (struct sec_plc_port_range_unit *)(uintptr_t)((char *)sec_base[PORT_RANGE_UNIT].base +
			le64toh(ahu->port_range_off));
		upper_pu = pu + (le64toh(ahu->port_range_sz) / sizeof(*pu)) - 1 ;
		for (; pu <= upper_pu; pu++) {
			(void)hm_log("ability:%s port_range[%"PRIu64", %"PRIu64")\n",
				     sec_plc_def2str(le64toh(ahu->sub_id), TYPY_DEF_DESC),
				     le64toh(pu->start), le64toh(pu->end));
		}
	}
	return;
}

static char *extend_str(const char *prefix, const char *s, const char *suffix)
{
	size_t size = strlen(s) + strlen(prefix) + strlen(suffix) + 1u;
	char *tmp = malloc(size);
	int rc;
	if (tmp != NULL) {
		*tmp = '\0';
		rc = strcat_s(tmp, size, prefix);
		BUG_ON(rc != EOK);
		rc = strcat_s(tmp, size, s);
		BUG_ON(rc != EOK);
		rc = strcat_s(tmp, size, suffix);
		BUG_ON(rc != EOK);
	}
	return tmp;
}

static void print_trans_rule_based_on_fileobj(const struct sec_plc_rule_criteria *criteria,
					      const char *fileobj, size_t len, uint64_t tgt)
{
	struct sec_plc_rule_criteria rule;
	char *token = NULL;
	char *tmp = NULL;
#define TOKEN_SUB 0
#define TOKEN_OBJ 1
#define TOKEN_ACT 2
#define TOKEN_NAME 3
#define TOKEN_NUM 4
	char *tokens[TOKEN_NUM] = {NULL};
	int i;

	token = strdup(fileobj + len);
	if (token == NULL) {
		eprintf("failed to allocate memory, skip obj=%s\n", fileobj);
		return;
	}
	tokens[0] = strtok_r(token, "|", &tmp);
	for (i = 1; i < TOKEN_NUM; i++) {
		tokens[i] = strtok_r(NULL, "|", &tmp);
		if (tokens[i] == NULL) {
			eprintf("invalid transition-name rule, obj=%s\n", fileobj);
			goto free_token;
		}
	}
	tokens[TOKEN_SUB] = extend_str("", tokens[TOKEN_SUB], "_t");
	tokens[TOKEN_OBJ] = extend_str("", tokens[TOKEN_OBJ], "_t");
	tokens[TOKEN_ACT] = extend_str("ac_", tokens[TOKEN_ACT], "");
	if (tokens[TOKEN_SUB] == NULL || tokens[TOKEN_OBJ] == NULL || tokens[TOKEN_ACT] == NULL) {
		eprintf("failed to allocate memory, skip obj=%s\n", fileobj);
		goto free_tokens;
	}
	reset_criteria(&rule);
	rule.sub = sec_plc_str2def(tokens[TOKEN_SUB]);
	rule.obj = sec_plc_str2def(tokens[TOKEN_OBJ]);
	rule.obj_name = tokens[TOKEN_NAME];
	rule.target = tgt;
	if (rule_filter(&rule, criteria)) {
		(void)hm_log("type transition name: %s %s %s %s %s\n",
			     tokens[TOKEN_SUB], tokens[TOKEN_OBJ],
			     tokens[TOKEN_NAME], tokens[TOKEN_ACT],
			     sec_plc_def2str(tgt, TYPY_DEF_DESC));
	}
free_tokens:
	free(tokens[TOKEN_SUB]);
	free(tokens[TOKEN_OBJ]);
	free(tokens[TOKEN_ACT]);
free_token:
	free(token);
}

#define TRANS_NAME_OBJ_PREFIX "__hmitn:"
static void dump_trans_rule_based_on_fileobj(const struct sec_plc_rule_criteria *criteria)
{
	struct fileobj_path_unit *fpu = NULL, *upper_fpu = NULL;
	const char *fileobj = NULL;
	size_t len;

	fpu = (struct fileobj_path_unit *)(sec_base[FILEOBJ].base);
	upper_fpu = fpu + (sec_base[FILEOBJ].sz / sizeof(*fpu)) - 1;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when fpu reach upper_fpu. Since fpu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_fpu is counted base on the size of TYPE_TRANSITION_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; fpu <= upper_fpu; fpu++) {
		fileobj = (const char *)sec_base[STRTBL].base + le64toh(fpu->name_off);
		len = sizeof(TRANS_NAME_OBJ_PREFIX) - 1u;
		if (strncmp(TRANS_NAME_OBJ_PREFIX, fileobj, len) != 0) {
			continue;
		}
		print_trans_rule_based_on_fileobj(criteria, fileobj, len, le64toh(fpu->sub_id));
	}
}

static void dump_trans_rule(const struct sec_plc_rule_criteria *criteria)
{
	struct type_transition_hdr_unit *thu = NULL, *upper_thu = NULL;
	struct type_transition_unit *tu = NULL, *upper_tu = NULL;
	struct sec_plc_rule_criteria rule;

	thu = (struct type_transition_hdr_unit *)(sec_base[TYPE_TRANSITION_HDR].base);
	upper_thu = thu + (sec_base[TYPE_TRANSITION_HDR].sz / sizeof(*thu)) - 1 ;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when thu reach upper_thu. Since thu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_thu is counted base on the size of TYPE_TRANSITION_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; thu <= upper_thu; thu++) {
		tu = ptr_to_type((char *)sec_base[TYPE_TRANSITION_UNIT].base +
			le64toh(thu->off), struct type_transition_unit *);
		upper_tu = tu + (le64toh(thu->sz) / sizeof(*tu)) - 1 ;
		/*
		 * Loop termination condition:
		 * Following loop will terminate when tu reach upper_tu. Since tu is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: upper_tu is counted base on the size of thu,
		 * which is limited, so loop condition is reasonable.
		 */
		for (; tu <= upper_tu; tu++) {
			reset_criteria(&rule);
			rule.sub = le64toh(thu->sub_id);
			rule.obj = le64toh(tu->obj_id);
			rule.target = le64toh(tu->tgt_id);
			if (rule_filter(&rule, criteria) == false) {
				continue;
			}
			(void)hm_log("type transition: %s %s %s %s\n",
				     sec_plc_def2str(le64toh(thu->sub_id), TYPY_DEF_DESC),
				     sec_plc_def2str(le64toh(tu->obj_id), TYPY_DEF_DESC),
				     sec_plc_def2str(le64toh(tu->act_id), ACTION_DEF_DESC),
				     sec_plc_def2str(le64toh(tu->tgt_id), TYPY_DEF_DESC));
		}
	}
	dump_trans_rule_based_on_fileobj(criteria);
	return;
}

static void dump_pattern_rule(const struct sec_plc_rule_criteria *criteria)
{
	struct pattern_unit *pu = NULL, *upper_pu = NULL;
	struct sec_plc_rule_criteria rule;

	pu = (struct pattern_unit *)(sec_base[PATTERN_UNIT].base);
	upper_pu = pu + (sec_base[PATTERN_UNIT].sz / sizeof(*pu)) - 1 ;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when pu reach upper_pu. Since pu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_pu is counted base on the size of TYPE_TRANSITION_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; pu <= upper_pu; pu++) {
		reset_criteria(&rule);
		rule.obj = le64toh(pu->sub_id);
		if (rule_filter(&rule, criteria) == false) {
			continue;
		}
		(void)hm_log("fileobj def:%s %s\n",
			     sec_plc_def2str(le64toh(pu->sub_id), TYPY_DEF_DESC),
			     le64toh(pu->name_off) + (char *)sec_base[STRTBL].base);
	}
	return;
}

static void dump_fileobj_rule(const struct sec_plc_rule_criteria *criteria)
{
	struct fileobj_path_unit *fpu = NULL, *upper_fpu = NULL;
	struct sec_plc_rule_criteria rule;

	fpu = (struct fileobj_path_unit *)(sec_base[FILEOBJ].base);
	upper_fpu = fpu + (sec_base[FILEOBJ].sz / sizeof(*fpu)) - 1 ;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when fpu reach upper_fpu. Since fpu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_fpu is counted base on the size of TYPE_TRANSITION_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; fpu <= upper_fpu; fpu++) {
		reset_criteria(&rule);
		rule.obj = le64toh(fpu->sub_id);
		if (rule_filter(&rule, criteria) == false) {
			continue;
		}
		(void)hm_log("fileobj def:%s %s\n",
			     sec_plc_def2str(le64toh(fpu->sub_id), TYPY_DEF_DESC),
			     le64toh(fpu->name_off) + (char *)sec_base[STRTBL].base);
	}
	dump_pattern_rule(criteria);
}

static void log_print(uint64_t type_id, uint32_t property)
{
	const char *label = sec_plc_def2str(type_id, TYPY_DEF_DESC);
	if (sec_plc_type_is_permissive_subj(type_id, &property)) {
		(void)hm_log("%-16s %s\n", "permissive subj:", label);
	}
	if (sec_plc_type_is_permissive_obj(type_id, &property)) {
		(void)hm_log("%-16s %s\n","permissive obj:", label);
	}
	if (sec_plc_type_is_no_log_subj(type_id, &property)) {
		(void)hm_log("%-16s %s\n", "no log subj:", label);
	}
	if (sec_plc_type_is_no_log_obj(type_id, &property)) {
		(void)hm_log("%-16s %s\n", "no log obj:", label);
	}
	if (sec_plc_type_is_cache_subj(type_id, &property)) {
		(void)hm_log("%-16s %s\n", "log cache subj:", label);
	}
	if (sec_plc_type_is_cache_obj(type_id, &property)) {
		(void)hm_log("%-16s %s\n", "log cache obj:", label);
	}
}

static void dump_property_rule(const struct sec_plc_rule_criteria *criteria)
{
	uint64_t type_id;
	struct policy_desc_idx_unit *pdu = NULL, *upper_pdu = NULL;
	struct sec_plc_rule_criteria rule;

	pdu = (struct policy_desc_idx_unit *)(sec_base[POLICY_DESC_IDX].base);
	upper_pdu = pdu + (sec_base[POLICY_DESC_IDX].sz / sizeof(*pdu)) - 1;

	/*
	 * Loop termination condition:
	 * Following loop will terminate when fpu reach upper_fpu. Since fpu is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: upper_fpu is counted base on the size of TYPE_TRANSITION_HDR section,
	 * which is limited, so loop condition is reasonable.
	 */
	for (; pdu <= upper_pdu; pdu++) {
		reset_criteria(&rule);
		type_id = le64toh(pdu->type_id);
		rule.generic = type_id;
		if (rule_filter(&rule, criteria) == false) {
			continue;
		}
		uint32_t property = _property_of(type_id);
		if (property != INVALID_PROPERTY) {
			log_print(type_id, property);
		}
	}
	return;
}

typedef void(*dump_func)(const struct sec_plc_rule_criteria *criteria);

struct dump_handle {
	unsigned int code;
	dump_func handle_func;
};

static struct dump_handle dump_tbl[] = {
	{SEC_PLC_RULE_AC, dump_te_rule},
	{SEC_PLC_RULE_AB, dump_ab_rule},
	{SEC_PLC_RULE_FILEOBJ, dump_fileobj_rule},
	{SEC_PLC_RULE_TRANS, dump_trans_rule},
	{SEC_PLC_RULE_PROPERTY, dump_property_rule},
};

void sec_plc_dump(const struct sec_plc_rule_criteria *criteria)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(dump_tbl); i++) {
		if (sec_plc_rule_is_on(criteria->type, dump_tbl[i].code) == 1U) {
			dump_tbl[i].handle_func(criteria);
		}
	}
}

void *policy_mmap(const char *file_name, size_t *sz)
{
	size_t ret_len = 0U;
	void *addr = NULL;
	int fd;

	if ((file_name == NULL) ||
	    (sz == NULL)) {
		hm_error("filename or size is NULL\n");
		addr = MAP_FAILED;
	} else {
		fd = hm_mem_shm_open(file_name, 0, O_RDONLY, &ret_len);
		if (fd < 0) {
			hm_error("failed to open shared memory of security policy\n");
			addr = MAP_FAILED;
		} else {
			addr = hm_mem_mmap(NULL, ret_len, PROT_READ, (uint32_t)MAP_SHARED | (uint32_t)MAP_ANONYMOUS,
					   (int32_t)fd, 0);
			if (addr == MAP_FAILED) {
				hm_error("failed to do mmap for shared memory\n");
			} else {
				*sz = ret_len;
			}
			(void)hm_mem_shm_close(fd);
		}
	}

	return addr;
}

void policy_munmap(void *addr, size_t sz)
{
	(void)hm_mem_munmap(addr, sz);
	return;
}

static void sec_plc_set_type_mode(uint64_t def_mode, unsigned int mode_value, int *cnt)
{
	int ret;

	ret = check_type_id(def_mode);
	if (ret == E_HM_OK) {
		*cnt += 1;
		(void)sec_plc_set_mode(mode_value);
	}

	return;
}

const char* sec_plc_mode2str(unsigned int plc_mode)
{
	const char* ret = NULL;
	switch (plc_mode) {
	case (SEC_PLC_PERMISSIVE | SEC_PLC_CACHE):
		ret = PLC_PERM_CACHE;
		break;
	case SEC_PLC_PERMISSIVE:
		ret = PLC_PERM;
		break;
	case SEC_PLC_DISABLE:
		ret = PLC_DISABLE;
		break;
	default:
		ret = PLC_ENFORCE;
		break;
	}
	return ret;
}

int sec_policy_chksum(uint8_t *chksum, size_t len)
{
	int rc = E_HM_OK;
	struct hdr *_hdr = (struct hdr *)sec_base[HDR].base;
	if (chksum == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK && len < CHKSUM_ARRAY_SIZE) {
		rc = E_HM_RANGE;
	}
	if (rc == E_HM_OK) {
		int err = memcpy_s(chksum, len, _hdr->chksum, CHKSUM_ARRAY_SIZE);
		if (err != EOK) {
			rc = E_HM_INVAL;
		}
	}
	return rc;
}

int sec_u8_to_hex_str(char* str_chksum, size_t str_len, const uint8_t *chksum, size_t len)
{
	int rc = E_HM_OK;
	if (str_chksum == NULL || chksum == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK && str_len <= len * HEX_CHARS_PER_U8) {
		rc = E_HM_RANGE;
	}
	if (rc == E_HM_OK) {
		for (size_t i = 0; i < len; i++) {
			int err = snprintf_s(str_chksum + HEX_CHARS_PER_U8 * i,
					str_len - HEX_CHARS_PER_U8 * i,
					HEX_CHARS_PER_U8,
					"%0*x", HEX_CHARS_PER_U8, chksum[i]);
			if (err != HEX_CHARS_PER_U8) {
				rc = E_HM_INVAL;
				break;
			}
		}
	}
	return rc;
}

static int mode_init(void)
{
	int ret = E_HM_OK;
	int cnt = 0;
	uint64_t def_disable = sec_plc_str2def("builtin_mac_disable_t");
	uint64_t def_permissive = sec_plc_str2def("builtin_mac_permissive_t");
	uint64_t def_permissive_cache = sec_plc_str2def("builtin_mac_permissive_cache_t");

	sec_plc_set_type_mode(def_permissive_cache, SEC_PLC_PERMISSIVE | SEC_PLC_CACHE, &cnt);
	sec_plc_set_type_mode(def_permissive, SEC_PLC_PERMISSIVE, &cnt);
	sec_plc_set_type_mode(def_disable, SEC_PLC_DISABLE, &cnt);

	if (cnt > 1) {
		hm_error("there are %d policy modes in policy files, but only one is required\n", cnt);
		ret = E_HM_INVAL;
	}

	return ret;
}

typedef struct {
	unsigned int max_ability;
	unsigned int max_action;
} max_id_set;

static int find_max_id(max_id_set *id_set)
{
	struct policy_desc_unit *base_ndu = NULL;
	struct policy_desc_unit *upper_ndu = NULL;
	struct policy_desc_unit *tmp_ndu = NULL;

	base_ndu = ptr_to_type(sec_base[POLICY_DESC].base, struct policy_desc_unit *);
	upper_ndu = base_ndu + (sec_base[POLICY_DESC].sz / sizeof(*base_ndu)) - 1;
	for (tmp_ndu = upper_ndu; tmp_ndu >= base_ndu; tmp_ndu--) {
		switch(le64toh(tmp_ndu->policy_type)) {
		case TYPY_DEF_DESC:
			break;
		case ABILITY_DEF_DESC:
			if (id_set->max_ability == 0) {
				id_set->max_ability = le32toh(tmp_ndu->type_id);
			}
			break;
		case ACTION_DEF_DESC:
			if (id_set->max_action == 0) {
				id_set->max_action = le32toh(tmp_ndu->type_id);
			}
			break;
		default:
			hm_error("policy type is invalid, value=%"PRIu64"\n", le64toh(tmp_ndu->policy_type));
			return E_HM_INVAL;
		}
	}
	return E_HM_OK;
}

static int get_max_id_set(max_id_set *id_set)
{
	int ret;

	if (sec_base[POLICY_DESC].sz >= sizeof(struct policy_desc_unit)) {
		ret = find_max_id(id_set);
		if (ret != E_HM_OK) {
			return ret;
		}
	}
	/* max_ability(action) = max_id + 1 */
	id_set->max_ability++;
	id_set->max_action++;
	return E_HM_OK;
}

int sec_plc_check_policy_consistency(void)
{
	int ret;
	max_id_set id_set = {0};

	ret = get_max_id_set(&id_set);
	if (ret != E_HM_OK) {
		return ret;
	}

	int cnt = 0;
	if (id_set.max_ability != (unsigned int)max_ability) {
		hm_error("inconsistent definition of ability exists! "
			 "the max_ability defined in policy.db is %u, "
			 "but the max_ability defined in policy_def.h is %u\n",
			 id_set.max_ability, (unsigned int)max_ability);
		cnt++;
	}
	if (id_set.max_action != (unsigned int)max_action) {
		hm_error("inconsistent definition of action exists! "
			 "the max_action defined in policy.db is %u, "
			 "but the max_action defined in policy_def.h is %u\n",
			 id_set.max_action, (unsigned int)max_action);
		cnt++;
	}

	return (cnt == 0) ? E_HM_OK : E_HM_INVAL;
}

#ifndef __HOST_LLT__
static vatomic32_t policy_inited = VATOMIC_INIT(0U);;
#endif
int policy_init(void *policy, size_t sz)
{
#ifndef __HOST_LLT__
	if (vatomic32_cmpxchg(&policy_inited, 0U, 1U) == 1U) {
		return E_HM_OK;
	}
#endif
	int ret;

	if (policy == NULL) {
		hm_error("policy address is invalid\n");
		return E_HM_INVAL;
	}
	ret = locate_secs(policy, sz);
	if (ret < 0) {
		hm_error("failed to locate section of policy\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = mode_init();
		if (ret != E_HM_OK) {
			hm_error("failed to initialize policy mode\n");
		}
	}

	return (ret == 0) ? E_HM_OK : E_HM_INVAL;
}

static int ability_hdr_unit_cmp(const struct ability_hdr_unit *nd1,
				const struct ability_hdr_unit *tgt)
{
	int rc;
	if (le64toh(nd1->sub_id) == tgt->sub_id) {
		rc = 0;
	} else if (le64toh(nd1->sub_id) > tgt->sub_id) {
		rc = 1;
	} else {
		rc = -1;
	}
	return rc;
}

static int policy_hdr_unit_cmp(const struct policy_hdr_unit *nd1,
			       const struct policy_hdr_unit *tgt)
{
	int rc;
	if (le64toh(nd1->sub_id) == tgt->sub_id) {
		rc = 0;
	} else if (le64toh(nd1->sub_id) < tgt->sub_id) {
		rc = -1;
	} else {
		rc = 1;
	}
	return rc;
}

static int policy_unit_cmp(const struct policy_unit *pu1,
			   const struct policy_unit *tgt)
{
	int rc;
	if (le64toh(pu1->obj_id) < tgt->obj_id) {
		rc = -1;
	} else if (le64toh(pu1->obj_id) > tgt->obj_id) {
		rc = 1;
	} else if (le64toh(pu1->act_id) > tgt->act_id) {
		rc = 1;
	} else if (le64toh(pu1->act_id) < tgt->act_id) {
		rc = -1;
	} else {
		rc = 0;
	}
	return rc;
}

static int type_transition_hdr_unit_cmp(const struct type_transition_hdr_unit *nd1,
					const struct type_transition_hdr_unit *tgt)
{
	int ret;

	if (le64toh(nd1->sub_id) == tgt->sub_id) {
		ret = 0;
	} else if (le64toh(nd1->sub_id) < tgt->sub_id) {
		ret = -1;
	} else {
		ret = 1;
	}
	return ret;
}

static int type_transition_unit_cmp(const struct type_transition_unit *nd1,
				    const struct type_transition_unit *tgt)
{
	int rc;
	if (le64toh(nd1->obj_id) < tgt->obj_id) {
		rc = -1;
	} else if (le64toh(nd1->obj_id) > tgt->obj_id) {
		rc = 1;
	} else if (le64toh(nd1->act_id) > tgt->act_id) {
		rc = 1;
	} else if (le64toh(nd1->act_id) < tgt->act_id) {
		rc = -1;
	} else {
		rc = 0;
	}
	return rc;
}

static int mrange_unit_cmp(const struct mrange_unit *nd1,
			   const struct mrange_unit *tgt)
{
	uint64_t start = le64toh(nd1->start);
	uint64_t end = le64toh(nd1->end);
	int rc;
	if ((start <= tgt->start) && (end >= tgt->end)) {
		rc = 0;
	} else if (start >= tgt->start) {
		rc = 1;
	} else {
		rc = -1;
	}
	return rc;
}

static int sec_plc_port_range_unit_cmp(const struct sec_plc_port_range_unit *curr,
				       const struct sec_plc_port_range_unit *target)
{
	uint64_t start = le64toh(curr->start);
	uint64_t end = le64toh(curr->end);
	int rc;
	if ((start <= target->start) && (end >= target->end)) {
		rc = 0;
	} else if (start >= target->start) {
		rc = 1;
	} else {
		rc = -1;
	}
	return rc;
}

static int fileobj_path_unit_cmp(const struct fileobj_path_unit *nd1, const char *tgt_path)
{
	char *file1 = le64toh(nd1->name_off) + (char *)sec_base[STRTBL].base;

	return strcmp(file1, tgt_path);
}

static int pattern_path_cmp(const struct pattern_path *nd1, const char *tgt_path)
{
	char *file1 = le64toh(nd1->stem_off) + (char *)sec_base[STRTBL].base;

	return strncmp(file1, tgt_path, (size_t)le64toh(nd1->stem_size));
}

static int policy_desc_unit_cmp(const struct policy_desc_unit *pdu1,
				const struct policy_desc_unit *pdu2)
{
	int rc;
	if (le64toh(pdu1->policy_type) < pdu2->policy_type) {
		rc = -1;
	} else if (le64toh(pdu1->policy_type) > pdu2->policy_type) {
		rc = 1;
	} else if (le32toh(pdu1->type_id) < pdu2->type_id) {
		rc = -1;
	} else if (le32toh(pdu1->type_id) > pdu2->type_id) {
		rc = 1;
	} else {
		rc = 0;
	}

	return rc;
}

#define binary_search(base_nd, upper_nd, tgt_nd, nd_name) \
	({ \
		struct nd_name *__tmp = NULL; \
		uint64_t __upper = (uint64_t)(int64_t)((upper_nd) - (base_nd)); \
		uint64_t __lower = 0u; \
		int __rc; \
		/*												\
		 * Loop termination condition:									\
		 * Following loop terminate when __lower is larger than __upper, Since __lower keep increasing	\
		 * in loop body, it has an upper bound.								\
		 * Loop condition is constant and reasonable: __upper may be the number of policy_hdr/ability_hdr,\
		 * which is limited, so the loop condition is reasonable.					\
		 */												\
		while (__lower <= __upper) { \
			uint64_t mid = (__upper + __lower) / 2U; \
			__tmp = ((base_nd) + (mid));		\
			__rc = nd_name##_cmp(__tmp, (tgt_nd)); \
			if (__rc == 0) { \
				break; \
			}		\
			if (__rc < 0) { \
				if (mid == __upper) {\
					__tmp = NULL; \
					break; \
				} \
				__lower = mid + 1U; \
			} else { \
				if (mid == __lower) { \
					__tmp = NULL; \
					break; \
				} \
				__upper = mid - 1U; \
			} \
		} \
		__tmp; \
	 })

bool check_te_policy(uint64_t sub_type, uint64_t obj_type,
		    uint64_t act)
{
	struct policy_hdr_unit *phu = NULL, *upper_phu = NULL, tgt_phu, *tmp_phu = NULL;
	struct policy_unit *pu = NULL, tgt_pu, *upper_pu = NULL, *tmp_pu = NULL;
	bool check = true;

	phu = (struct policy_hdr_unit *)(sec_base[POLICY_HDR].base);
	upper_phu = phu + (sec_base[POLICY_HDR].sz / sizeof(*phu)) - 1 ;

	if (sec_base[POLICY_HDR].sz < sizeof(*phu)) {
		hm_debug("policy section size is invalid, size=0x%"PRIx64"\n", sec_base[POLICY_HDR].sz);
		check = false;
	}

	if (check) {
		tgt_phu.sub_id = sub_type;
		tmp_phu = binary_search(phu, upper_phu, &tgt_phu, policy_hdr_unit);
		if (tmp_phu == NULL) {
			check = false;
		}
	}

	if (check) {
		tgt_pu.obj_id = obj_type;
		tgt_pu.act_id = act;
		pu = (struct policy_unit *)(uintptr_t)((char *)sec_base[POLICY_UNIT].base +
					    le64toh(tmp_phu->off));
		if (le64toh(tmp_phu->sz) < sizeof(*pu)) {
			hm_debug("policy header unit size is invalid, size=0x%"PRIx64"\n", le64toh(tmp_phu->sz));
			check = false;
		}
	}
	if (check) {
		upper_pu = pu + (le64toh(tmp_phu->sz) / sizeof(*pu)) - 1;
		tmp_pu = binary_search(pu, upper_pu, &tgt_pu, policy_unit);
		if (tmp_pu != NULL) {
			check = true;
		} else {
			check = false;
		}
	}
	return check;
}

static bool _chk_ability(const ability_t *ablty, enum ability_def tgt_ablty)
{
	bool rc = true;
	uint64_t i;
	i = (uint64_t)tgt_ablty / ABILITY_BITS_PER_ELEMENT;
	if ((le64toh(ablty->ability[i]) &
	     ((uint64_t)1 << ((unsigned int)tgt_ablty % ABILITY_BITS_PER_ELEMENT))) != 0U) {
		rc = true;
	} else {
		rc = false;
	}
	return rc;
}

static void _set_ability(ability_t *ablty, enum ability_def tgt_ablty)
__attribute__((unused));

static void _set_ability(ability_t *ablty, enum ability_def tgt_ablty)
{
	uint64_t i;
	i = (uint64_t)tgt_ablty / ABILITY_BITS_PER_ELEMENT;
	ablty->ability[i] |= htole64((uint64_t)1 << ((unsigned int)tgt_ablty % ABILITY_BITS_PER_ELEMENT));
	return;
}

static int search_ability_hdr_unit(uint64_t sub_id,
				   struct ability_hdr_unit **ability)
{
	int rc = E_HM_OK;

	struct ability_hdr_unit *upper_unit = NULL, *base_unit = NULL;
	struct ability_hdr_unit target = {0};

	if (sec_base[ABILITY_HDR].sz < sizeof(struct ability_hdr_unit)) {
		hm_debug("ability header size is too small, size=0x%"PRIx64"\n", sec_base[ABILITY_HDR].sz);
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		base_unit = (struct ability_hdr_unit *)(sec_base[ABILITY_HDR].base);
		upper_unit = base_unit + (sec_base[ABILITY_HDR].sz /
					  sizeof(struct ability_hdr_unit)) - 1;

		target.sub_id = sub_id;
		*ability = binary_search(base_unit, upper_unit,
					 &target, ability_hdr_unit);
		if (*ability == NULL) {
			hm_debug("[ability] failed to search ability, "
				 "sub_id=0x%"PRIx64"\n", sub_id);
			rc = E_HM_POSIX_NOENT;
		}
	}
	return rc;
}

bool check_ability(uint64_t sub_type, enum ability_def ablty)
{
	bool rc = true;
	struct ability_hdr_unit *tmp_ahu = NULL;
	int err = search_ability_hdr_unit(sub_type, &tmp_ahu);
	if (err < 0) {
		rc = false;
	} else {
		rc = _chk_ability(&tmp_ahu->ability, ablty);
	}
	return rc;
}

static bool _valid_mrange(const mrange_t *mrange)
{
	bool rc = false;
	if ((mrange != NULL) && (mrange->start < mrange->end)) {
		rc = true;
	}
	return rc;
}

bool check_mrange(uint64_t sub_type, mrange_t *mrange)
{
	struct ability_hdr_unit *ability = NULL;
	struct mrange_unit *mu = NULL, *upper_mu = NULL, *tmp_mu = NULL;
	int ret;
	bool rc = true;

	if (!_valid_mrange(mrange)) {
		hm_debug("memory range is invalid\n");
		rc = false;
	}

	if (rc == true) {
		ret = search_ability_hdr_unit(sub_type, &ability);
		if (ret != 0) {
			rc = false;
		}
	}

	if (rc == true) {
		mu = (struct mrange_unit*)(uintptr_t)((char *)sec_base[MRANGE_UNIT].base +
			le64toh(ability->mrange_off));
		if (le64toh(ability->mrange_sz) < sizeof(struct mrange_unit)) {
			hm_warn("the size of memory range is invalid, size=0x%"PRIx64"\n", le64toh(ability->mrange_sz));
			rc = false;
		}
	}

	if (rc == true) {
		upper_mu = mu + (le64toh(ability->mrange_sz) / sizeof(*mu)) - 1;
		tmp_mu = binary_search(mu, upper_mu, mrange, mrange_unit);
		if (tmp_mu == NULL) {
			rc = false;
		}
	}
	return rc;
}

static bool _valid_port_range(const sec_plc_port_range_t *range)
{
	bool ret_val = false;
	/* start can be equal to end, indicate one port */
	if (range != NULL &&
	    range->start <= range->end &&
	    range->end <= SEC_LIB_PORT_RANGE_MAX_NUM) {
		ret_val = true;
	}
	return ret_val;
}

static int check_port_range(const struct ability_hdr_unit *ability,
			    const sec_plc_port_range_t *range)
{
	struct sec_plc_port_range_unit *base_unit = NULL;
	struct sec_plc_port_range_unit *upper_unit = NULL;
	struct sec_plc_port_range_unit *ret = NULL;
	int rc = 0;

	if (le64toh(ability->port_range_sz) <
	    sizeof(struct sec_plc_port_range_unit)) {
		hm_warn("[port range] size of port range is invalid, size=0x%"PRIx64"\n", le64toh(ability->port_range_sz));
		rc = -1;
	} else {
		base_unit = (struct sec_plc_port_range_unit*)
			    (uintptr_t)((char *)sec_base[PORT_RANGE_UNIT].base +
			    le64toh(ability->port_range_off));
		upper_unit = base_unit +
			     (le64toh(ability->port_range_sz) /
			     sizeof(struct sec_plc_port_range_unit)) - 1;

		ret = binary_search(base_unit, upper_unit,
				    range, sec_plc_port_range_unit);
		if (ret == NULL) {
			hm_debug("[port range] failed to search port range, sub_id=0x%"PRIx64", "
				 "start=0x%"PRIx64", end=0x%"PRIx64"\n",
				 le64toh(ability->sub_id), range->start, range->end);
			rc = -1;
		}
	}

	return rc;
}

bool sec_plc_check_port_range(uint64_t sub_type, const sec_plc_port_range_t *range)
{
	struct ability_hdr_unit *ability = NULL;
	int ret;
	bool rc = true;

	if (range == NULL) {
		hm_debug("[port range] port range is null\n");
		rc = false;
	}

	if (rc == true) {
		if (!_valid_port_range(range)) {
			hm_debug("[port range] port range is invalid, "
				 "start=0x%"PRIx64", end=0x%"PRIx64"\n",
				 range->start, range->end);
			rc = false;
		}
	}

	if (rc == true) {
		/*
		 * since we increase end by 1 when parsing policy,
		 * do the same process to the check target
		 */
		sec_plc_port_range_t range_internal;
		range_internal.start = range->start;
		range_internal.end = range->end + 1U;

		ret = search_ability_hdr_unit(sub_type, &ability);
		if (ret != 0) {
			hm_debug("[port range] failed to search ability, "
				 "ret=%d, sub_id=0x%"PRIx64"\n", ret, sub_type);
			rc = false;
		} else {
			ret = check_port_range(ability, &range_internal);
			if (ret != 0) {
				hm_debug("[port range] failed to search port range, "
					 "ret=%d, sub_id=0x%"PRIx64", "
					 "start=0x%"PRIx64", end=0x%"PRIx64"\n",
					 ret, sub_type, range->start, range->end);
				rc = false;
			}
		}
	}

	return rc;
}

static int get_sub_id_by_pattern(const char *path, uint64_t *sub_id)
{
	int rc = 0;
	struct pattern_path *base_p = NULL, *upper_p = NULL, *tmp = NULL;
	struct pattern_unit *base_pu = NULL, *upper_pu = NULL;

	if (sec_base[PATTERN].sz < sizeof(*base_p)) {
		hm_warn("pattern size is invalid, size=0x%"PRIx64"\n", sec_base[PATTERN].sz);
		rc = -1;
	}

	if (rc == 0) {
		base_p = ptr_to_type(sec_base[PATTERN].base, struct pattern_path *);
		upper_p = base_p + (sec_base[PATTERN].sz / sizeof(*base_p)) - 1 ;
		tmp = binary_search(base_p, upper_p, path, pattern_path);
		if (tmp == NULL ||
		    le64toh(tmp->size) < sizeof(*base_pu)) {
			rc = -1;
		}
	}

	if (rc == 0) {
		rc = -1;
		base_pu = ptr_to_type((ptr_to_type(sec_base[PATTERN_UNIT].base, char *) +
				      le64toh(tmp->unit_off)), struct pattern_unit *);
		upper_pu = base_pu + (le64toh(tmp->size) / sizeof(*base_pu)) - 1;
		/*
		 * Loop termination condition:
		 * Following loop terminate when base_pu is larger than upper_pu, Since base_pu keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: upper_pu is size of the pattern,
		 * which is limited, so the loop condition is reasonable.
		 */
		while (base_pu <= upper_pu) {
			char *pattern = ptr_to_type(sec_base[STRTBL].base, char *) +
				    le64toh(base_pu->name_off);
			size_t pattern_len = strlen(pattern);
			if (fncmp(pattern, pattern_len, path, strlen(path))) {
				*sub_id = le64toh(base_pu->sub_id);
				rc = 0;
				break;
			}
			base_pu++;
		}
	}
	return rc;
}

int get_sub_id_by_path(const char *path, uint64_t *sub_id)
{
	struct fileobj_path_unit *base_p = NULL, *upper_p = NULL, *tmp = NULL;
	int rc = E_HM_OK;
	if (sub_id == NULL || path == NULL) {
		hm_error("sub_id or path is NULL\n");
		rc = E_HM_INVAL;
	} else if (sec_base[FILEOBJ].sz < sizeof(*base_p)) {
		hm_error("section size is invalid\n");
		rc = E_HM_INVAL;
	} else {
		base_p = (struct fileobj_path_unit *)(sec_base[FILEOBJ].base);
		upper_p = base_p + (sec_base[FILEOBJ].sz / sizeof(*base_p)) - 1 ;
		tmp = binary_search(base_p, upper_p, path, fileobj_path_unit);
		if (tmp == NULL) {
			rc = get_sub_id_by_pattern(path, sub_id);
		} else {
			*sub_id = le64toh(tmp->sub_id);
		}
	}
	return rc;
}

int sec_plc_get_sid_with_prefix(const char *path, const char *prefix, uint64_t *sub_id)
{
	size_t len = 0;
	size_t path_len = 0;
	size_t prefix_len = 0;
	int rc = E_HM_OK;
	char *alias = NULL;

	if (path == NULL || prefix == NULL || sub_id == NULL) {
		hm_error("input parameters is invalid\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		path_len = strlen(path);
		prefix_len = strlen(prefix);
		if (SIZE_MAX - path_len <= prefix_len) {
			hm_error("path len %zu and prefix len %zu overflow\n", path_len, prefix_len);
			rc = E_HM_INVAL;
		}
	}

	if (rc == E_HM_OK) {
		len = path_len + prefix_len;
		alias = (char *)malloc(len + 1);
		if (alias == NULL) {
			hm_debug("malloc len %zu failed\n", (len + 1));
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		NOFAIL(memcpy_s(alias, len + 1, prefix, prefix_len));
		NOFAIL(strncpy_s(alias + prefix_len, path_len + 1, path, path_len));
		rc = get_sub_id_by_path(alias, sub_id);
	}

	if (alias != NULL) {
		free(alias);
	}

	return rc;
}

bool get_type_transition(uint64_t sub_id, uint64_t obj_id, enum action_def act_id, uint64_t *tgt_id)
{
	bool ret = true;
	struct type_transition_hdr_unit *phu = NULL;
	struct type_transition_hdr_unit *upper_phu = NULL;
	struct type_transition_hdr_unit *tmp_phu = NULL;
	struct type_transition_hdr_unit tgt_phu;
	struct type_transition_unit *pu = NULL;
	struct type_transition_unit *upper_pu = NULL;
	struct type_transition_unit *tmp_pu = NULL;
	struct type_transition_unit tgt_pu;

	if (tgt_id == NULL) {
		hm_error("param is invalid\n");
		ret = false;
	}
	if (ret && sec_base[TYPE_TRANSITION_HDR].sz < sizeof(*phu)) {
		hm_warn("section size is invalid, size=0x%"PRIx64"\n", sec_base[TYPE_TRANSITION_HDR].sz);
		ret = false;
	}

	if (ret) {
		phu = (struct type_transition_hdr_unit *)(sec_base[TYPE_TRANSITION_HDR].base);
		upper_phu = phu + (sec_base[TYPE_TRANSITION_HDR].sz / sizeof(*phu)) - 1;

		tgt_phu.sub_id = sub_id;
		tmp_phu = binary_search(phu, upper_phu, &tgt_phu, type_transition_hdr_unit);
		if (tmp_phu == NULL) {
			ret = false;
		}
	}

	if (ret) {
		tgt_pu.obj_id = obj_id;
		tgt_pu.act_id = (uint64_t)(unsigned int)act_id;
		if (le64toh(tmp_phu->sz) < sizeof(*pu)) {
			hm_warn("pattern size is invalid, size=0x%"PRIx64"\n", le64toh(tmp_phu->sz));
			ret = false;
		}
	}
	if (ret) {
		pu = (struct type_transition_unit *)(uintptr_t)((char *)sec_base[TYPE_TRANSITION_UNIT].base +
								le64toh(tmp_phu->off));
		upper_pu = pu + (le64toh(tmp_phu->sz) / sizeof(*pu)) - 1 ;
		tmp_pu = binary_search(pu, upper_pu, &tgt_pu, type_transition_unit);
		if (tmp_pu != NULL) {
			*tgt_id = le64toh(tmp_pu->tgt_id);
		} else {
			ret = false;
		}
	}

	return ret;
}

int check_type_id(type_id_t id)
{
	return (id == (type_id_t)null_type) ?
	       E_HM_INVAL : E_HM_OK;
}

static int get_valid_ability(uint64_t sub_type, const ability_t *ablty,
			     ability_t *must_ablty, ability_t *avail_ablty)
{
	struct ability_hdr_unit *ahu = NULL;
	struct ability_hdr_unit *tmp_ahu = NULL;
	struct ability_hdr_unit tgt_ahu;
	uint64_t i;
	int rc = E_HM_OK;
	ability_t empty_ability = {0};
	ability_t *tmp_ablty = &empty_ability;

	if (sec_base[ABILITY_HDR].sz >= sizeof(*ahu)) {
		ahu = (struct ability_hdr_unit *)(sec_base[ABILITY_HDR].base);

		tgt_ahu.sub_id = sub_type;
		tmp_ahu = binary_search(ahu, (ahu + (sec_base[ABILITY_HDR].sz / sizeof(*ahu)) - 1),
					&tgt_ahu, ability_hdr_unit);
		if (tmp_ahu != NULL) {
			tmp_ablty = &(tmp_ahu->ability);
		}

		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach ABILITY_WIDTH. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: ABILITY_WIDTH is 2 according to libsecplc/policy_fmt.h,
		 * so loop condition is reasonable.
		 */
		for (i = 0; i < ABILITY_WIDTH; i++) {
			must_ablty->ability[i] = le64toh(tmp_ablty->ability[i]) & ablty->ability[i];
			if (avail_ablty != NULL) {
				avail_ablty->ability[i] = le64toh(tmp_ablty->ability[i]);
			}
		}
	} else {
		hm_warn("ability header size is invalid, size=0x%"PRIx64"\n", sec_base[ABILITY_HDR].sz);
		rc = E_HM_INVAL;
	}

	return rc;
}

int sec_plc_get_avail_abilities(uint64_t sub_type, ability_t *avail_ablty)
{
	ability_t exp_ablty = {0};
	ability_t must_ablty = {0};
	return get_valid_ability(sub_type, &exp_ablty, &must_ablty, avail_ablty);
}

static bool _check_all_ability(const ability_t *exp_ablty, const ability_t *ablty)
{
	uint64_t i;
	bool ret = true;
	/*
	 * Loop termination condition:
	 * Following loop will terminate when i reach ABILITY_WIDTH. Since i is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: ABILITY_WIDTH is 2 according to libsecplc/policy_fmt.h,
	 * so loop condition is reasonable.
	 */
	for (i = 0; i < ABILITY_WIDTH; i++) {
		if (exp_ablty->ability[i] != ablty->ability[i]) {
			ret = false;
			break;
		}
	}
	return ret;
}

int sec_plc_check_abilities(uint64_t sub_type, ability_t *exp_ablty, ability_t *avail_ablty)
{
	ability_t ability = {0};
	int ret = E_HM_OK;

	if (exp_ablty == NULL || avail_ablty == NULL) {
		hm_error("param is invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK && get_valid_ability(sub_type, exp_ablty, &ability, avail_ablty) != E_HM_OK) {
		hm_error("failed to get valid abilities\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK && !_check_all_ability(exp_ablty, &ability)) {
		ret = E_HM_ACCES;
	}

	return ret;
}

/*
 * sub_type: sub type id to be searched
 * exp_ablty: expected abilites of this operation
 * avail_ablty: available abilities that the subject appears to show depending on the ENFORCE/PERMISSIVE mode
 * log_avail_ablty: available abilities of the subject used when logging
 */
bool check_and_export_abilities(uint64_t sub_type, ability_t *exp_ablty, ability_t *avail_ablty,
				ability_t *log_avail_ablty)
{
	int ret = E_HM_OK;

	if (exp_ablty == NULL || avail_ablty == NULL || log_avail_ablty == NULL) {
		hm_error("param is invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = sec_plc_check_abilities(sub_type, exp_ablty, avail_ablty);
		/* If parameters are invalid, it will show the log with empty log_avail_ablty.
		 * Else fix the result of avail_ablty with plc mode.
		 */
		if (ret != E_HM_INVAL) {
			NOFAIL(memcpy_s(log_avail_ablty, sizeof(ability_t), avail_ablty, sizeof(ability_t)));
			if (sec_plc_mode() != SEC_PLC_ENFORCE) {
				NOFAIL(memset_s(avail_ablty, sizeof(ability_t), 0xFF, sizeof(ability_t)));
			}
		}
	}

	return ret == E_HM_OK;
}

const char *sec_plc_def2str(uint64_t sub_id, enum POLICY_DESC_TYPE policy_type)
{
	struct policy_desc_unit ndu = {0};
	struct policy_desc_unit *base_ndu = NULL;
	struct policy_desc_unit *upper_ndu = NULL;
	struct policy_desc_unit *tmp_ndu = NULL;
	char *str = NULL;

	if (sec_base[POLICY_DESC].sz >= sizeof(*base_ndu)) {
		base_ndu = ptr_to_type(sec_base[POLICY_DESC].base, struct policy_desc_unit *);
		upper_ndu = base_ndu + (sec_base[POLICY_DESC].sz / sizeof(*base_ndu)) - 1;

		ndu.policy_type = (uint64_t)policy_type;
		ndu.type_id = (uint32_t)sub_id;

		tmp_ndu = binary_search(base_ndu, upper_ndu, &ndu, policy_desc_unit);
		if (tmp_ndu != NULL) {
			str = (ptr_to_type(sec_base[STRTBL].base, char *) + le64toh(tmp_ndu->unit_off));
		}
	}

	return str;
}

/* Similar with sec_plc_def2str */
static uint32_t _property_of(uint64_t type_id)
{
	struct policy_desc_unit *base_ndu = NULL;
	uint32_t property = INVALID_PROPERTY;
	/* The max_num of sec_base[POLICY_DESC] with item size of policy_desc_unit */
	uint64_t max_num = sec_base[POLICY_DESC].sz / sizeof(struct policy_desc_unit);

	/* Because the policy_desc_unit is stored with type_id start from 1,and increased one by one */
	if (sec_base[POLICY_DESC].sz >= sizeof(*base_ndu) && type_id >= 1 && type_id < max_num) {
		base_ndu = ptr_to_type(sec_base[POLICY_DESC].base, struct policy_desc_unit *);
		if (base_ndu[type_id - 1].policy_type == TYPY_DEF_DESC) {
			property = le32toh(base_ndu[type_id - 1].property);
		}
	}
	/* Some error with sec_base data or invalid type_id */
	return property;
}

static inline bool sec_plc_type_has_property(uint64_t type_id, uint32_t *p_cached_property, uint32_t tprop)
{
	if (p_cached_property == NULL) {
		return (_property_of(type_id) & tprop) != 0U;
	}
	if (*p_cached_property == INVALID_PROPERTY) {
		*p_cached_property = _property_of(type_id);
	}
	return ((*p_cached_property) & tprop) != 0U;
}

bool sec_plc_type_is_permissive_subj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, PERMISSIVE_SUBJ);
}

bool sec_plc_type_is_permissive_obj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, PERMISSIVE_OBJ);
}

bool sec_plc_type_is_no_log_subj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, NO_LOG_SUBJ);
}

bool sec_plc_type_is_no_log_obj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, NO_LOG_OBJ);
}

bool sec_plc_type_is_cache_subj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, CACHE_SUBJ);
}

bool sec_plc_type_is_cache_obj(uint64_t type_id, uint32_t *p_cached_property)
{
	return sec_plc_type_has_property(type_id, p_cached_property, CACHE_OBJ);
}

static struct policy_desc_idx_unit *bin_search_type_name(const char *type_name,
	struct policy_desc_idx_unit *base_nd, uint64_t nr_pdu)
{
	struct policy_desc_idx_unit *tmp_nd = NULL;
	char *tmp_name = NULL;
	uint64_t lower = 0;
	uint64_t upper = nr_pdu;
	uint64_t mid;
	int rc = -1;

	/*
	 * Loop termination condition:
	 * Following loop terminate when lower is equal to upper, Since lower keep increasing
	 * or the upper keep decreasing in loop body, it has a bound.
	 * Loop condition is constant and reasonable: upper is size of POLICY_DESC_IDX,
	 * which is limited, so the loop condition is reasonable.
	 */
	while (lower < upper) {
		mid = lower + (upper - lower) / 2U; /* 2 for binary search */
		tmp_nd = base_nd + mid;
		tmp_name = le64toh(tmp_nd->unit_off) +
			   (char *)sec_base[STRTBL].base;
		rc = strcmp(tmp_name, type_name);
		if (rc == 0) {
			break;
		}
		if (rc < 0) {
			lower = mid + 1u;
		} else {
			upper = mid;
		}
	}
	if (rc != 0) {
		tmp_nd = (lower == upper) ? NULL : (base_nd + lower);
	}
	return tmp_nd;
}

uint64_t sec_plc_str2def(const char *type_name)
{
	struct policy_desc_idx_unit *base_pdu = NULL;
	struct policy_desc_idx_unit *tmp_pdu = NULL;
	uint64_t nr_pdu;
	uint64_t rc;

	if (type_name == NULL) {
		hm_warn("type_name is NULL\n");
		rc = (uint64_t)null_type;
	} else if (sec_base[POLICY_DESC_IDX].sz < sizeof(*base_pdu)) {
		hm_warn("policy section size is invalid\n");
		rc = (uint64_t)null_type;
	} else {
		base_pdu = (struct policy_desc_idx_unit *)(sec_base[POLICY_DESC_IDX].base);
		nr_pdu = sec_base[POLICY_DESC_IDX].sz / sizeof(*base_pdu);

		tmp_pdu = bin_search_type_name(type_name, base_pdu, nr_pdu);
		rc = (tmp_pdu == NULL) ? (uint64_t)null_type : le64toh(tmp_pdu->type_id);
	}

	return rc;
}

static void port_range_unit_get_num(const struct ability_hdr_unit *ability,
				    uint64_t *num)
{
	uint64_t sz = le64toh(ability->port_range_sz);
	uint64_t unit_sz = sizeof(struct sec_plc_port_range_unit);
	uint64_t check = sz % unit_sz;
	BUG_ON(check != 0U);
	*num = sz / unit_sz;
}

static int port_range_iterator_init(uint64_t subj,
				    struct sec_plc_port_range_iterator *it)
{
	struct ability_hdr_unit *ability = NULL;
	int ret;
	uint64_t num = 0;

	it->unit_curr = NULL;
	it->unit_last = NULL;

	ret = search_ability_hdr_unit(subj, &ability);
	if (ret != 0) {
		hm_debug("[port range] get ability fail, sub_id=0x%"PRIx64"\n",
			 subj);
	} else {
		port_range_unit_get_num(ability, &num);

		it->unit_curr = (struct sec_plc_port_range_unit*)(uintptr_t)((char *)sec_base[PORT_RANGE_UNIT].base +
								  le64toh(ability->port_range_off));
		it->unit_last = it->unit_curr + num;
	}

	return ret;
}

int sec_plc_iter_port_range_start(enum type_def subj,
				  struct sec_plc_port_range_iterator *it)
{
	int ret = E_HM_OK;
	if (it == NULL) {
		hm_error("[port range] iterator is null\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = port_range_iterator_init((uint64_t)subj, it);
		if (ret != 0) {
			hm_warn("[port range] failed to initialize iterator,"
				"ret=%d, sub_id=0x%x\n", ret, (unsigned int)subj);
		}
	}
	return ret;
}

void sec_plc_iter_port_range_next(struct sec_plc_port_range_iterator *it)
{
	if (it != NULL) {
		it->unit_curr++;
	}
}

static bool is_port_range_iterator_valid(const struct sec_plc_port_range_iterator *it)
{
	return (it->unit_curr != NULL && it->unit_last != NULL);
}

int sec_plc_iter_port_range_get(const struct sec_plc_port_range_iterator *it,
				sec_plc_port_range_t *range)
{
	int rc = E_HM_OK;

	if (it == NULL) {
		hm_error("[port range] iterator is null\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		if (!is_port_range_iterator_valid(it)) {
			hm_warn("[port range] iterator is not valid\n");
			rc = E_HM_INVAL;
		}
	}

	if (rc == E_HM_OK) {
		if (it->unit_curr >= it->unit_last) {
			rc = E_HM_OVERFLOW;
		}
	}

	if (rc == E_HM_OK) {
		if (range != NULL) {
			range->start = le64toh(it->unit_curr->start);
			range->end = le64toh(it->unit_curr->end);

			/*
			 * since we increase the end when parsing the policy
			 * restore it here
			 */
			BUG_ON(range->end == 0U);
			range->end--;
		}
	}

	return rc;
}

int sec_plc_port_range_get_reserved(struct sec_plc_port_range_iterator *it)
{
	int err = sec_plc_iter_port_range_start(reserved_port_range_t, it);
	if (err != E_HM_OK) {
		hm_warn("[port range] get all reserved fail\n");
	}
	return err;
}

int sec_plc_get_internal_abs(ability_t *ability)
{
	int ret = E_HM_OK;
	if (ability == NULL) {
		ret =  E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ability_t *abs_in_sec =
			(ability_t *)sec_base[INNER_ABILITY].base;
		for (unsigned int i = 0; i < ABILITY_WIDTH; i++) {
#ifdef CONFIG_INNER_MAC
			(ability->ability)[i] =
				le64toh((abs_in_sec->ability)[i]);
#else
			UNUSED(abs_in_sec);
			(ability->ability)[i] = 0;
#endif
		}
	}

	return ret;
}

bool sec_plc_is_internal_ab(enum ability_def ability)
{
#ifdef CONFIG_INNER_MAC
	uint64_t index = (uint64_t)ability / (sizeof(uint64_t) * BIT_NUMBER_OF_BYTE);
	uint64_t shift = (uint64_t)ability % (sizeof(uint64_t) * BIT_NUMBER_OF_BYTE);
	if (index >= ABILITY_WIDTH) {
		hm_warn("ability index overflow\n");
		return false;
	}

	ability_t *abs_in_sec = (ability_t *)sec_base[INNER_ABILITY].base;
	uint64_t ab = le64toh((abs_in_sec->ability)[index]);
	return (ab & ((uint64_t)1u << shift)) != 0u;
#else
	UNUSED(ability);
	return false;
#endif
}
