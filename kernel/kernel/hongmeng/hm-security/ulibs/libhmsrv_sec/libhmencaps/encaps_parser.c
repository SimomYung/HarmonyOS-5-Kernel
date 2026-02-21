/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Encaps parser implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 01:19:14 2024
 */
#include <ctype.h>
#include <string.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmencaps/encaps_parser.h>
#include <libhmencaps/encaps_str2int_hash.h>

#define ENCAPS_HEAD_STR "\"encaps\""
#define ENCAPS_HEAD_LEN 8
#define ENCAPS_COUNT_HEAD_STR "\"ohos.encaps.count\""
#define ENCAPS_COUNT_HEAD_LEN 19
#define TRUE_STR_LEN 4
#define FALSE_STR_LEN 5

#define NUMBER_BASE 10
#define MAX_VALUE_LEN 32
#define MAX_KEY_LEN 64
#define MAX_ENCAPS_STR_LEN (MAX_ENCAPS_COUNT * MAX_KEY_LEN * 2)

#define JSON_BEGIN 0
#define ENCAP_HEADER 1
#define ENCAP_BODY_BIGIN 2
#define ENCAP_COLON 3

#define ENCAP_COUNT 4
#define ENCAP_COUNT_HEADER 5
#define ENCAP_COUNT_VALUE 6

#define ENCAP_KEY_VALUE 7
#define ENCAP_INT 8
#define ENCAP_BOOL 9

#define ENCAP_NEXT_KV_OR_END 10
#define ENCAP_END 11
#define JSON_END 12

#define ENCAP_COUNT_CHECK_DONE 13
#define ENCAP_SORT_DONE 14
#define ENCAP_BOOL_PARSE_DONE 15
#define ENCAP_INT_PARSE_DONE 16
#define ENCAP_STR_PARSE_DONE 17
#define ENCAP_REDUND_CHECK_DONE 18
#define ENCAP_VALUE_FOUND 19

#define ENCAP_FMT_ERROR 20
#define ENCAP_KEY_ERROR 21
#define ENCAP_VALUE_ERROR 22
#define ENCAP_INTL_ERROR 23
#define ENCAP_COUNT_ERROR 24
#define ENCAP_REDUND_ERROR 25
#define ENCAP_PARAM_ERROR 26

#define VALUE_ERROR (-2)
#define MALLOC_ERROR (-1)
#define MERGE_SUCCESS 0

/* Function to merge two sorted subarrays */
static int merge(struct sec_encaps arr[], uint32_t l, uint32_t m, uint32_t r, uint32_t *mrg_len)
{
	uint32_t n1 = m - l + 1;
	uint32_t n2 = r - m;
	/* Create temporary arrays */
	struct sec_encaps *L = malloc(n1 * sizeof(struct sec_encaps));
	struct sec_encaps *R = malloc(n2 * sizeof(struct sec_encaps));
	if (L == NULL || R == NULL) {
		hm_warn("Error: unable to allocate memory for array L or array R.\n");
		if (L != NULL) {
			free(L);
		}
		if (R != NULL) {
			free(R);
		}
		return MALLOC_ERROR;
	}
	/* Copy data to temporary arrays */
	for (uint32_t i = 0; i < n1; i++) {
		L[i] = arr[l + i];
	}
	for (uint32_t j = 0; j < n2; j++) {
		R[j] = arr[m + 1 + j];
	}
	uint32_t i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		if (L[i].key < R[j].key) {
			arr[k] = L[i++];
		} else if (L[i].key > R[j].key) {
			arr[k] = R[j++];
		} else {
			/* keys are equal, check values */
			if (L[i].value != R[j].value) {
				hm_warn("Error: keys are equal but values are not.\n");
				free(L);
				free(R);
				return VALUE_ERROR;
			}
			arr[k] = L[i++];
			/* skip the duplicate in R */
			++j;
		}
		++k;
	}
	/* copy remaining items */
	while (i < n1) {
		arr[k++] = L[i++];
	}
	while (j < n2) {
		arr[k++] = R[j++];
	}
	/* update mrg_len to the actual size */
	*mrg_len = k;
	/* Free allocated memory */
	free(L);
	free(R);
	return MERGE_SUCCESS;
}
/* Main recursive merge sort function */
static int encaps_merge_sort(struct sec_encaps arr[], uint32_t l, uint32_t r, uint32_t *mrg_len)
{
	int ret = MERGE_SUCCESS;
	if (l == r) {
		*mrg_len = 1;
		return ret;
	}
	if (l < r) {
		uint32_t m = l + (r - l) / 2;

		ret = encaps_merge_sort(arr, l, m, mrg_len);
		if (ret != MERGE_SUCCESS) {
			return ret;
		}

		ret = encaps_merge_sort(arr, m + 1, r, mrg_len);
		if (ret != MERGE_SUCCESS) {
			return ret;
		}

		ret = merge(arr, l, m, r, mrg_len);
	}
	return ret;
}

struct sec_encaps *encaps_list_merge(
	struct sec_encaps *encaps1, uint32_t len1, struct sec_encaps *encaps2, uint32_t len2, uint32_t *mrg_len)
{
	*mrg_len = len1 + len2;
	struct sec_encaps *encaps_mrg = (struct sec_encaps *)malloc((*mrg_len) * sizeof(struct sec_encaps));
	if (encaps_mrg == NULL) {
		hm_warn("Error: unable to allocate memory for array encaps_mrg.");
		return NULL;
	}
	uint32_t i = 0, j = 0, k = 0;
	while (i < len1 && j < len2) {
		if (k >= MAX_ENCAPS_COUNT) {
			goto merge_error;
		} else if (encaps1[i].key < encaps2[j].key) {
			encaps_mrg[k] = encaps1[i++];
		} else if (encaps1[i].key > encaps2[j].key) {
			encaps_mrg[k] = encaps2[j++];
		} else {
			/* keys are equal, check values */
			if (encaps1[i].value != encaps2[j].value) {
				goto merge_error;
			}
			encaps_mrg[k] = encaps1[i++];
			/* skip the duplicate in encaps2 */
			++j;
		}
		++k;
	}
	/* copy remaining items */
	while (i < len1) {
		if (k >= MAX_ENCAPS_COUNT) {
			goto merge_error;
		}
		encaps_mrg[k++] = encaps1[i++];
	}
	while (j < len2) {
		if (k >= MAX_ENCAPS_COUNT) {
			goto merge_error;
		}
		encaps_mrg[k++] = encaps2[j++];
	}
	/* update mrg_len to the actual size */
	*mrg_len = k;
	return encaps_mrg;

merge_error:
	hm_warn("Error: merge error\n");
	free(encaps_mrg);
	encaps_mrg = NULL;
	*mrg_len = 0;
	return NULL;
}

static inline void white_space_eater(const char *encap_str, uint32_t *encap_str_idx)
{
	while (isspace(*(encap_str + *encap_str_idx))) {
		(*encap_str_idx) = ((*encap_str_idx) + 1);
	}
}

static int encap_value_type(const char *encap_str, uint32_t vl_start_index)
{
	char first_char = *(encap_str + vl_start_index);
	if (first_char >= '0' && first_char <= '9') {
		return ENCAP_INT;
	}
	if (first_char == 't' || first_char == 'f') {
		return ENCAP_BOOL;
	}
	hm_warn("Error: Invalid char, expect digit char or 't' or 'f' at %d, \
	current char: '%c'\n", vl_start_index, first_char);
	return ENCAP_FMT_ERROR;
}

static int encap_vl_int(const char *encap_str, uint32_t vl_start_index, uint32_t vl_len, uint32_t *vl_int)
{
	const char *vl_start_ptr = encap_str + vl_start_index;
	const char *vl_end_ptr = vl_start_ptr + vl_len;
	char *end_ptr = NULL;
	if (vl_len > MAX_VALUE_LEN) {
		hm_warn("Error: string is too long, can not be safely parse as number, \
		at %d\n", vl_start_index);
		return ENCAP_VALUE_ERROR;
	}
	unsigned long result = strtoul(vl_start_ptr, &end_ptr, NUMBER_BASE);
	if (end_ptr == vl_start_ptr || end_ptr != vl_end_ptr) {
		hm_warn("Error: Invalid value, the value string can not be safely parse as number, \
		at %d\n", vl_start_index);
		return ENCAP_FMT_ERROR;
	}
	if (result >= UINT32_MAX) {
		hm_warn("Error: Invalid value, the value is too large, at %d\n", vl_start_index);
		return ENCAP_VALUE_ERROR;
	}
	*vl_int = (uint32_t)result;
	return ENCAP_INT_PARSE_DONE;
}

static inline int encaps_colon_parser(const char *encap_str, uint32_t *encap_str_idx, int encap_state)
{
	white_space_eater(encap_str, encap_str_idx);
	if (*(encap_str + *encap_str_idx) != ':') {
		hm_warn("Error: Invalid char, expect ':' at %d, \
		current char: '%c'", (*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	++(*encap_str_idx);
	white_space_eater(encap_str, encap_str_idx);
	return encap_state;
}

static int encap_vl_bool(const char *encap_str, uint32_t vl_start_index, uint32_t vl_len, uint32_t *parsed_bool)
{
	char true_str[5] = "true";
	char false_str[6] = "false";
	if (vl_len == TRUE_STR_LEN && memcmp(encap_str + vl_start_index, true_str, TRUE_STR_LEN) == 0) {
		*parsed_bool = 1;
		return ENCAP_BOOL_PARSE_DONE;
	}
	if (vl_len == FALSE_STR_LEN && memcmp(encap_str + vl_start_index, false_str, FALSE_STR_LEN) == 0) {
		*parsed_bool = 0;
		return ENCAP_BOOL_PARSE_DONE;
	}
	hm_warn("Error: Invalid value string, expect \"true\" or \"false\" at %d\n", vl_start_index);
	return ENCAP_FMT_ERROR;
}

static int encap_str_parser(const char *encap_str, uint32_t *encap_str_idx, char *str)
{
	if (*(encap_str + *encap_str_idx) != '"') {
		hm_warn("Error: Invalid char, expect '\"' at %d, current char: '%c'\n",
				(*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	++(*encap_str_idx);
	int str_start_index = *encap_str_idx;
	while (*(encap_str + *encap_str_idx) != '\0') {
		if (*(encap_str + *encap_str_idx) == '"') {
			break;
		}
		++(*encap_str_idx);
	}
	if (*(encap_str + *encap_str_idx) != '"') {
		hm_warn("Error: Invalid char, expect '\"' at %d, current char: '%c'\n",
				(*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	uint32_t str_end_idx = *encap_str_idx;
	uint32_t str_len = str_end_idx - str_start_index;
	if (str_len > MAX_KEY_LEN) {
		hm_warn("Error: encap key length more than MAX_KEY_LEN, key length is %d\n", str_len);
		return ENCAP_KEY_ERROR;
	}
	errno_t ret = memcpy_s(str, MAX_KEY_LEN, encap_str + str_start_index, str_len);
	if (ret != 0) {
		hm_warn("Error: memory copy failed\n");
		return ENCAP_KEY_ERROR;
	}
	*(str + str_len) = '\0';
	++(*encap_str_idx);
	return ENCAP_STR_PARSE_DONE;
}

static inline void encap_vl_str_parser(
	const char *encap_str, uint32_t *encap_str_idx, uint32_t *vl_start_index, uint32_t *vl_len)
{
	*vl_start_index = *encap_str_idx;
	while (*(encap_str + *encap_str_idx) != '\0') {
		if (isspace(*(encap_str + *encap_str_idx)) || *(encap_str + *encap_str_idx) == ','
			|| *(encap_str + *encap_str_idx) == '}') {
			break;
		}
		++(*encap_str_idx);
	}
	*vl_len = *encap_str_idx - *vl_start_index;
}

static inline int json_begin_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	white_space_eater(encap_str, encap_str_idx);
	if (*(encap_str + *encap_str_idx) != '{') {
		hm_warn("Error: Invalid char, expect '{' at %d, current char: '%c'\n",
				(*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	++(*encap_str_idx);
	return ENCAP_HEADER;
}

static int encap_body_begin_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	int encap_state = ENCAP_BODY_BIGIN;
	encap_state = encaps_colon_parser(encap_str, encap_str_idx, encap_state);
	if (encap_state == ENCAP_FMT_ERROR) {
		return encap_state;
	}
	if (*(encap_str + *encap_str_idx) != '{') {
		hm_warn("Error: Invalid char, expect '{' at %d, current char: '%c'\n",
				(*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	++(*encap_str_idx);
	return ENCAP_COUNT;
}

static inline int encap_header_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	white_space_eater(encap_str, encap_str_idx);
	char header[9] = ENCAPS_HEAD_STR;
	if (strncmp(encap_str + (*encap_str_idx), header, ENCAPS_HEAD_LEN) != 0) {
		hm_warn("Error: Invalid char, expect 'e' at %d ,current char: '%c'\n",
				*encap_str_idx, *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	} else {
		(*encap_str_idx) = (*encap_str_idx) + ENCAPS_HEAD_LEN;
		return ENCAP_BODY_BIGIN;
	}
}

static inline int encap_count_header_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	white_space_eater(encap_str, encap_str_idx);
	char header[20] = ENCAPS_COUNT_HEAD_STR;
	if (strncmp(encap_str + (*encap_str_idx), header, ENCAPS_COUNT_HEAD_LEN) != 0) {
		hm_warn("Error: Invalid count header at %d, current char: '%c'\n",
				*encap_str_idx, *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	} else {
		(*encap_str_idx) = (*encap_str_idx) + ENCAPS_COUNT_HEAD_LEN;
		return ENCAP_COLON;
	}
}

static inline int encap_count_vl_parser(const char *encap_str, uint32_t *encap_str_idx, uint *count)
{
	uint32_t vl_start_idx = 0;
	uint32_t vl_len = 0;
	encap_vl_str_parser(encap_str, encap_str_idx, &vl_start_idx, &vl_len);
	return encap_vl_int(encap_str, vl_start_idx, vl_len, count);
}

static inline int encaps_count_format_parser(const char *encap_str, uint32_t *encap_str_idx, uint32_t *count)
{
	int encap_state = encap_count_header_parser(encap_str, encap_str_idx);
	if (encap_state != ENCAP_COLON) {
		return encap_state;
	}
	encap_state = encaps_colon_parser(encap_str, encap_str_idx, encap_state);
	if (encap_state == ENCAP_FMT_ERROR) {
		return encap_state;
	}
	return encap_count_vl_parser(encap_str, encap_str_idx, count);
}

static inline int alloc_encaps(struct sec_encaps **encap_list, uint32_t *count, int encap_state)
{
	if (*count > MAX_ENCAPS_COUNT) {
		hm_warn("Error: count should not be more than MAX_ENCAPS_COUNT");
		return ENCAP_INTL_ERROR;
	}
	*encap_list = (struct sec_encaps *)malloc((*count) * sizeof(struct sec_encaps));
	if (*encap_list == NULL) {
		hm_warn("Error: malloc encap_list failed");
		return ENCAP_INTL_ERROR;
	}
	return encap_state;
}

static inline int encap_count_parser(
	const char *encap_str, uint32_t *encap_str_idx, struct sec_encaps **encap_list, uint32_t *count)
{
	int encap_state = encaps_count_format_parser(encap_str, encap_str_idx, count);
	if (encap_state != ENCAP_INT_PARSE_DONE) {
		return encap_state;
	}
	encap_state = alloc_encaps(encap_list, count, encap_state);
	if (encap_state == ENCAP_INTL_ERROR) {
		return encap_state;
	}
	return ENCAP_NEXT_KV_OR_END;
}

static inline int encap_key_parser(const char *encap_str, uint32_t *encap_str_idx, char *key_str)
{
	int encap_state = encap_str_parser(encap_str, encap_str_idx, key_str);
	if (encap_state != ENCAP_STR_PARSE_DONE) {
		return encap_state;
	}
	return ENCAP_COLON;
}

static int encap_value_parser(const char *encap_str, uint32_t *encap_str_idx, uint32_t *type, uint32_t *value)
{
	uint32_t vl_start_index = 0;
	uint32_t vl_len = 0;
	encap_vl_str_parser(encap_str, encap_str_idx, &vl_start_index, &vl_len);
	int encap_state = encap_value_type(encap_str, vl_start_index);
	switch (encap_state) {
		case ENCAP_BOOL:
			*type = BOOL_VALUE;
			encap_state = encap_vl_bool(encap_str, vl_start_index, vl_len, value);
			break;
		case ENCAP_INT:
			*type = INT_VALUE;
			encap_state = encap_vl_int(encap_str, vl_start_index, vl_len, value);
			break;
		default:
			break;
	}
	return encap_state;
}

static inline int create_encap(struct sec_encaps *const encaps, char *key_str,
	int value, int type, uint32_t *encap_count_atc)
{
	struct sec_encaps *new_encap = encaps + (*encap_count_atc);
	new_encap->type = type;
	struct encaps_str2int *str2int = find_encaps_str2int(key_str, strlen(key_str));
	if (str2int == NULL) {
		return ENCAP_KEY_ERROR;
	}
	new_encap->key = str2int->hash_vl;
	new_encap->value = value;
	++(*encap_count_atc);
	return ENCAP_NEXT_KV_OR_END;
}

static inline int get_encap_key_value(const char *encap_str, uint32_t *encap_str_idx,
   char *key_str, uint32_t *type, uint32_t *value)
{
	white_space_eater(encap_str, encap_str_idx);
	int encap_state = encap_key_parser(encap_str, encap_str_idx, key_str);
	if (encap_state != ENCAP_COLON) {
		return encap_state;
	}
	encap_state = encaps_colon_parser(encap_str, encap_str_idx, encap_state);
	if (encap_state == ENCAP_FMT_ERROR) {
		return encap_state;
	}
	return encap_value_parser(encap_str, encap_str_idx, type, value);
}

static inline int encap_kv_count_check(uint32_t encap_str_idx, uint32_t encap_count_dec, uint32_t encap_count_atc)
{
	if (encap_count_atc >= encap_count_dec) {
		hm_warn("Error: Too many permission at %d, declare count is %d, \
		but actually at least %d\n", encap_str_idx, encap_count_dec, encap_count_atc + 1);
		return ENCAP_COUNT_ERROR;
	}
	return ENCAP_KEY_VALUE;
}

static int encap_key_value_parser(const char *encap_str, uint32_t *encap_str_idx, uint32_t encap_count_dec,
	struct sec_encaps *const encaps, uint32_t *encap_count_atc)
{
	uint32_t type = 0, value = 0;
	char key_str[MAX_KEY_LEN + 1] = {'\0'};
	int encap_state = encap_kv_count_check(*encap_str_idx, encap_count_dec, *encap_count_atc);
	if (encap_state != ENCAP_KEY_VALUE) {
		return encap_state;
	}
	encap_state = get_encap_key_value(encap_str, encap_str_idx, key_str, &type, &value);
	if (encap_state != ENCAP_BOOL_PARSE_DONE && encap_state != ENCAP_INT_PARSE_DONE) {
		return encap_state;
	}
	return create_encap(encaps, key_str, value, type, encap_count_atc);
}

static inline int match_encaps_key_and_set_value(const char *key_str, uint32_t type,
	uint32_t value, struct sec_encaps *encap)
{
	struct encaps_str2int *str2int = find_encaps_str2int(key_str, strlen(key_str));
	if (str2int == NULL) {
		return ENCAP_KEY_ERROR;
	}
	if (str2int->hash_vl == encap->key) {
		encap->type = type;
		encap->value = value;
		return ENCAP_VALUE_FOUND;
	}
	return ENCAP_NEXT_KV_OR_END;
}

static int encap_key_value_match(const char *encap_str, uint32_t *encap_str_idx, uint32_t encap_count_dec,
	struct sec_encaps *encap, uint32_t *encap_count_atc)
{
	char key_str[MAX_KEY_LEN + 1] = {'\0'};
	uint32_t type = 0, value = 0;
	int encap_state = encap_kv_count_check(*encap_str_idx, encap_count_dec, *encap_count_atc);
	if (encap_state != ENCAP_KEY_VALUE) {
		return encap_state;
	}
	encap_state = get_encap_key_value(encap_str, encap_str_idx, key_str, &type, &value);
	if (encap_state != ENCAP_BOOL_PARSE_DONE && encap_state != ENCAP_INT_PARSE_DONE) {
		return encap_state;
	}
	return match_encaps_key_and_set_value(key_str, type, value, encap);
}

static int encap_next_or_end_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	white_space_eater(encap_str, encap_str_idx);
	switch (*(encap_str + *encap_str_idx)) {
		case ',':
			++(*encap_str_idx);
			return ENCAP_KEY_VALUE;
		case '}':
			++(*encap_str_idx);
			return ENCAP_END;
		default:
			hm_warn("Error: Invalid char, expect ',' or '}' at %d, \
			current char: '%c'\n",
				(*encap_str_idx),
				*(encap_str + *encap_str_idx));
			return ENCAP_FMT_ERROR;
	}
}

static inline int encaps_count_check(uint32_t encap_count_dec, uint32_t encap_count_atc)
{
	if (encap_count_dec != encap_count_atc) {
		hm_warn("Error: Encap count is illegal, declare count is %d, \
		but actually is %d\n", encap_count_dec, encap_count_atc);
		return ENCAP_COUNT_ERROR;
	}
	return ENCAP_COUNT_CHECK_DONE;
}

static int encaps_sort(struct sec_encaps *const encap_list, uint32_t encap_count, uint32_t *mrg_len)
{
	int ret = 0;
	if (encap_count == 0) {
		goto encaps_sort_done;
	}
	ret = encaps_merge_sort(encap_list, 0, encap_count - 1, mrg_len);
	switch (ret) {
		case MALLOC_ERROR:
			return ENCAP_INTL_ERROR;
		case VALUE_ERROR:
			return ENCAP_VALUE_ERROR;
		default:
			break;
	}
encaps_sort_done:
	return ENCAP_SORT_DONE;
}

static inline int encap_redund_check(uint32_t encap_count_dec, uint32_t mrg_len, uint32_t *count)
{
	*count = mrg_len;
	if (encap_count_dec != mrg_len) {
		return ENCAP_REDUND_ERROR;
	}
	return ENCAP_REDUND_CHECK_DONE;
}

static inline int json_end_parser(const char *encap_str, uint32_t *encap_str_idx)
{
	white_space_eater(encap_str, encap_str_idx);
	if (*(encap_str + *encap_str_idx) != '}') {
		hm_warn("Error: Invalid char, expect '}' at %d, current char: '%c'\n",
				(*encap_str_idx), *(encap_str + *encap_str_idx));
		return ENCAP_FMT_ERROR;
	}
	return JSON_END;
}

static void encaps_warn(int encap_state)
{
	switch (encap_state) {
		case ENCAP_KEY_ERROR:
			hm_warn("Error: Parse encaps failed due to encap key error\n");
			break;
		case ENCAP_VALUE_ERROR:
			hm_warn("Error: Parse encaps failed due to encap value error\n");
			break;
		case ENCAP_COUNT_ERROR:
			hm_warn("Error: Parse encaps failed due to declared count of encaps can't match actual count\n");
			break;
		case ENCAP_REDUND_ERROR:
			hm_warn("Error: Parse encaps failed due to redunt encaps are declared in file\n");
			break;
		case ENCAP_FMT_ERROR:
			hm_warn("Error: Parse encaps failed, please check file format\n");
			break;
		case ENCAP_INTL_ERROR:
			hm_warn("Error: Parse encaps failed due to internal error\n");
			break;
		case ENCAP_PARAM_ERROR:
			hm_warn("Error: Parse encaps failed due to param error\n");
			break;
		default:
			hm_warn("Error: Unknown internal error occurs in Parsing encaps \n");
			break;
	}
}

static inline int process_parser_error_state(const char *encaps_str, int encap_state,
												struct sec_encaps **encap_list, uint32_t *count)
{
	hm_warn("Error: encaps state error state is %d, encaps str is %s\n", encap_state, encaps_str);
	encaps_warn(encap_state);
	if (*encap_list != NULL) {
		free(*encap_list);
	}
	*encap_list = NULL;
	*count = 0;
	return E_HM_INVAL;
}

static inline int process_get_encaps_value_error_state(const char *encaps_str, const char *key, int encap_state)
{
	hm_warn("Error: encaps state error state is %d, encaps str is %s, key is %s\n",
				encap_state, encaps_str, key);
	if(encap_state == JSON_END) {
		hm_warn("Error: can't find value for this key \n");
	} else {
		encaps_warn(encap_state);
	}
	return E_HM_INVAL;
}

static int encaps_begin_format_parser(const char *encaps_str, uint32_t *encap_str_idx)
{
	int state = JSON_BEGIN;
	while (*(encaps_str + *encap_str_idx) != '\0') {
		switch (state) {
			case JSON_BEGIN:
				state = json_begin_parser(encaps_str, encap_str_idx);
				break;
			case ENCAP_HEADER:
				state = encap_header_parser(encaps_str, encap_str_idx);
				break;
			case ENCAP_BODY_BIGIN:
				state = encap_body_begin_parser(encaps_str, encap_str_idx);
				break;
			default:
				return state;
		}
	}
	return state;
}

static inline int encaps_begin_parser(const char *encap_str, uint32_t *encap_str_idx,
	struct sec_encaps **const encap_list, uint32_t *encap_count_dec)
{
	int state =  encaps_begin_format_parser(encap_str, encap_str_idx);
	if (state != ENCAP_COUNT) {
		return ENCAP_FMT_ERROR;
	}
	return encap_count_parser(encap_str, encap_str_idx, encap_list, encap_count_dec);
}

static inline int encaps_begin_check(const char *encap_str, uint32_t *encap_str_idx,
	uint32_t *encap_count_dec)
{
	int encap_state =  encaps_begin_format_parser(encap_str, encap_str_idx);
	if (encap_state != ENCAP_COUNT) {
		return encap_state;
	}
	encap_state = encaps_count_format_parser(encap_str, encap_str_idx, encap_count_dec);
	if (encap_state != ENCAP_INT_PARSE_DONE) {
		return encap_state;
	}
	return ENCAP_NEXT_KV_OR_END;
}

static int encaps_kv_to_end_parser(const char *encap_str, uint32_t *encap_str_idx,
	struct sec_encaps *const encap_list, uint32_t encap_count_dec, uint32_t *encap_count_atc)
{
	int state = ENCAP_NEXT_KV_OR_END;
	while (*(encap_str + *encap_str_idx) != '\0') {
		switch (state) {
			case ENCAP_KEY_VALUE:
				state =
					encap_key_value_parser(encap_str, encap_str_idx, encap_count_dec, encap_list, encap_count_atc);
				break;
			case ENCAP_NEXT_KV_OR_END:
				state = encap_next_or_end_parser(encap_str, encap_str_idx);
				break;
			case ENCAP_END:
				state = json_end_parser(encap_str, encap_str_idx);
				break;
			default:
				return state;
		}
	}
	return state;
}

static int encaps_kv_to_end_search(const char *encap_str, uint32_t *encap_str_idx,
	struct sec_encaps *encap, uint32_t encap_count_dec, uint32_t *encap_count_atc)
{
	int state = ENCAP_NEXT_KV_OR_END;
	while (*(encap_str + *encap_str_idx) != '\0') {
		switch (state) {
			case ENCAP_KEY_VALUE:
				state =
					encap_key_value_match(encap_str, encap_str_idx, encap_count_dec, encap, encap_count_atc);
				break;
			case ENCAP_NEXT_KV_OR_END:
				state = encap_next_or_end_parser(encap_str, encap_str_idx);
				break;
			case ENCAP_END:
				state = json_end_parser(encap_str, encap_str_idx);
				break;
			default:
				return state;
		}
	}
	return state;
}

static int encaps_check_after_end(struct sec_encaps *const encap_list, uint32_t encap_count_dec,
	uint32_t encap_count_atc, uint32_t *count)
{
	int state = JSON_END;
	uint32_t mrg_len = 0;
	state = encaps_count_check(encap_count_dec, encap_count_atc);
	if (state != ENCAP_COUNT_CHECK_DONE) {
		return state;
	}
	state = encaps_sort(encap_list, encap_count_atc, &mrg_len);
	if (state != ENCAP_SORT_DONE) {
		return state;
	}
	return encap_redund_check(encap_count_dec, mrg_len, count);
}

static inline int encaps_parser_params_check(const char *encap_str,
	struct sec_encaps **const encap_list, uint32_t *count)
{
	if (encap_str == NULL || encap_list == NULL || *encap_list != NULL || count == NULL
		|| strlen(encap_str) > MAX_ENCAPS_STR_LEN) {
		return ENCAP_PARAM_ERROR;
	}
	return JSON_BEGIN;
}

int encaps_parser(const char *encaps_str, struct sec_encaps **const encap_list, uint32_t *count)
{
	uint32_t encap_str_idx = 0, encap_count_dec = 0, encap_count_atc = 0;
	int encap_state = encaps_parser_params_check(encaps_str, encap_list, count);
	if (encap_state != JSON_BEGIN) {
		return process_parser_error_state(encaps_str, encap_state, encap_list, count);
	}
	encap_state = encaps_begin_parser(encaps_str, &encap_str_idx, encap_list, &encap_count_dec);
	if (encap_state != ENCAP_NEXT_KV_OR_END) {
		return process_parser_error_state(encaps_str, encap_state, encap_list, count);
	}
	encap_state = encaps_kv_to_end_parser(encaps_str, &encap_str_idx,
						*encap_list, encap_count_dec, &encap_count_atc);
	if (encap_state != JSON_END) {
		return process_parser_error_state(encaps_str, encap_state, encap_list, count);
	}
	encap_state = encaps_check_after_end(*encap_list, encap_count_dec, encap_count_atc, count);
	if (encap_state != ENCAP_REDUND_CHECK_DONE) {
		return process_parser_error_state(encaps_str, encap_state, encap_list, count);
	}
	return E_HM_OK;
}

static inline int get_encaps_value_params_check(const char *encaps_str,
	char *key, uint32_t *value, enum encaps_type *type, uint32_t *key_hash)
{
	if (encaps_str == NULL || key == NULL || value == NULL || type == NULL
		|| strlen(encaps_str) > MAX_ENCAPS_STR_LEN) {
		return ENCAP_PARAM_ERROR;
	}
	struct encaps_str2int *str2int = find_encaps_str2int(key, strlen(key));
	if (str2int == NULL) {
		return ENCAP_PARAM_ERROR;
	}
	*key_hash = str2int->hash_vl;
	return JSON_BEGIN;
}

int get_encaps_value_from_str(const char *encaps_str, char *key, uint32_t *value, enum encaps_type *type)
{
	uint32_t encap_str_idx = 0, encap_count_dec = 0, encap_count_atc = 0, key_hash = 0;
	struct sec_encaps encap = {0};
	int encap_state = get_encaps_value_params_check(encaps_str, key, value, type, &key_hash);
	if (encap_state != JSON_BEGIN) {
		return process_get_encaps_value_error_state(encaps_str, key, encap_state);
	}
	encap_state = encaps_begin_check(encaps_str, &encap_str_idx, &encap_count_dec);
	if (encap_state != ENCAP_NEXT_KV_OR_END) {
		return process_get_encaps_value_error_state(encaps_str, key, encap_state);
	}
	encap.key = key_hash;
	encap_state = encaps_kv_to_end_search(encaps_str, &encap_str_idx,
						&encap, encap_count_dec, &encap_count_atc);
	if (encap_state != ENCAP_VALUE_FOUND) {
		return process_get_encaps_value_error_state(encaps_str, key, encap_state);
	}
	*value = encap.value;
	*type = encap.type;
	return E_HM_OK;
}
