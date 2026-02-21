/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: String pattern matching
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */

#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <libhmsync/bitops.h>
#include <libhwsecurec/securec.h>

#include "fncmp.h"

#ifdef CONFIG_FNCMP_DP
#define BITMAP_SIZE ((PATH_MAX + 1) * 2)
static void raw_bitmap_set_bit_value(unsigned long long *bitmap,
				     unsigned int sz,
				     unsigned int bit_off,
				     int value)
{
	if (value != 1) {
		raw_bitmap_clear_bit(bitmap, sz, bit_off);
	} else {
		raw_bitmap_set_bit(bitmap, sz, bit_off);
	}
}

/* Dynamic program algorithm */
/* time complexity:  O(psz * ssz) */
/* space complexity: O(1), (PATH_MAX + 1) * 2 / (8 * sizeof(unsigned long long)) + 1 bytes */
bool fncmp(const char *p, size_t psz, const char *s, size_t ssz)
{
	int curr = 0;
	int prev = 1;
	int c_offset = 0;
	int p_offset = 0;
	int value;
	bool has_matched = false;

	DEFINE_RAW_BITMAP(map, BITMAP_SIZE);
	mem_zero_a(map);
	// empty pattern matches with empty string
	raw_bitmap_set_bit(map, BITMAP_SIZE, 0U);

	for (int pi = 1; pi <= psz; pi++) {
		if (p[pi - 1] == '*') {
			raw_bitmap_set_bit(map, BITMAP_SIZE, pi);
		} else {
			break;
		}
	}

	for (int si = 1; si <= ssz; si++) {
		// exchange curr and prev
		curr ^= 1;
		prev ^= 1;
		c_offset = (psz + 1) * curr;
		p_offset = (psz + 1) * prev;
		has_matched = false;
		for (int pi = 1; pi <= psz; pi++) {
			if (p[pi - 1] == '*') {
				value = raw_bitmap_test_bit(map, BITMAP_SIZE, pi - 1 + c_offset) == 1 ||
					raw_bitmap_test_bit(map, BITMAP_SIZE, pi + p_offset) == 1;
				raw_bitmap_set_bit_value(map, BITMAP_SIZE, pi + c_offset, value);
			} else if (p[pi - 1] == '?' || s[si - 1] == p[pi - 1]) {
				value = raw_bitmap_test_bit(map, BITMAP_SIZE, pi - 1 + p_offset);
				raw_bitmap_set_bit_value(map, BITMAP_SIZE, pi + c_offset, value);
			} else {
				raw_bitmap_clear_bit(map, BITMAP_SIZE, pi + c_offset);
			}
			if (raw_bitmap_test_bit(map, BITMAP_SIZE, pi + c_offset) == 1) {
				has_matched = true;
			}
		}
		if (has_matched == false) {
			return false;
		}
		// only empty s matches with empty p
		raw_bitmap_clear_bit(map, BITMAP_SIZE, 0);
	}
	return raw_bitmap_test_bit(map, BITMAP_SIZE, psz + c_offset) == 1;
}

#else
/* Check if the specified section [left, right) are all '*' character */
static bool is_all_stars(const char *p, size_t left, size_t right)
{
	bool ret = true;
	for (size_t i = left; i < right; i++) {
		if (p[i] != '*') {
			ret = false;
			break;
		}
	}

	return ret;
}

static bool fncmp_param_check(const char *p, size_t *psz, const char *s, size_t *ssz)
{
	bool ret = true;
	if (*psz > strlen(p) || *psz > PATH_MAX || *ssz > strlen(s) || *ssz > PATH_MAX) {
		ret = false;
	} else {
		// check the tail chars behind the last '*'
		while (*ssz > 0UL && *psz > 0UL && p[*psz - 1UL] != '*') {
			if (s[*ssz - 1UL] == p[*psz - 1UL] || p[*psz - 1UL] == '?') {
				(*ssz)--;
				(*psz)--;
			} else {
				ret = false;
				break;
			}
		}
	}

	return ret;
}

static bool fncmp_process(const char *p, size_t psz, const char *s, size_t ssz)
{
	size_t s_idx = 0;
	size_t p_idx = 0;
	size_t s_record = 0;
	size_t p_record = 0;
	bool has_recorded = false;
	bool ret = true;
	while (s_idx < ssz && p_idx < psz) {
		if (p[p_idx] == '*') {
			p_idx++;
			p_record = p_idx;
			s_record = s_idx;
			has_recorded = true;
		} else if (s[s_idx] == p[p_idx] || p[p_idx] == '?') {
			s_idx++;
			p_idx++;
		} else if (has_recorded && (s_record + 1UL) < ssz) {
			// rematch s from next char of s_record
			s_record++;
			s_idx = s_record;
			p_idx = p_record;
		} else {
			ret = false;
			break;
		}
	}
	if (ret) {
		ret = is_all_stars(p, p_idx, psz);
	}

	return ret;
}
/* Greedy algorithm */
/* asymptotic time complexity: O(psz * ssz) */
/* average time complexity:    O(ssz * log psz) */
/* space complexity:           O(1) */
bool fncmp(const char *pattern, size_t psz, const char *str, size_t ssz)
{
	bool ret = fncmp_param_check(pattern, &psz, str, &ssz);
	if (ret == true) {
		if (psz == 0UL) {
			ret = (ssz == 0UL);
		} else {
			ret = fncmp_process(pattern, psz, str, ssz);
		}
	}

	return ret;
}
#endif
