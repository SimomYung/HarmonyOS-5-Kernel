/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Privilged services access uapps memory implementation on SPIN.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 5 10:33:25 2020
 */
#ifndef ULIBS_LIBMEM_UMEM_H
#define ULIBS_LIBMEM_UMEM_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define SPIN_MEM_SAFE_COPY 0x1U

void hm_setup_spin_flags(void);
int hm_copy_to_user(void *dst_ori, const void *src_ori, size_t bytes);
int hm_copy_from_user(void *dst_ori, const void *src_ori, size_t bytes);
int hm_copy_from_user_nopf(void *dst_ori, const void *src_ori, size_t bytes);
int hm_strcpy_from_user(void *dst_ori, const void *src_ori, size_t bytes);
int hm_copy_to_caller(void *dst_ori, const void *src_ori, size_t bytes);
int hm_copy_from_caller(void *dst_ori, const void *src_ori, size_t bytes);
int hm_umem_get_user_pages(bool is_fwd, uintptr_t vaddr, size_t len,
			   unsigned long long *ret_pa_array, size_t array_size);
int hm_umem_put_user_pages(bool is_fwd, uintptr_t vaddr, size_t len,
			   unsigned long long *ret_pa_array, size_t array_size);
int hm_umem_charge_mlock(unsigned long mlock_size);
int hm_umem_uncharge_mlock(unsigned long mlock_size);
int hm_umem_safe_copy_nopf_with_pan(void *dst, const void *src, size_t bytes);

#ifdef CONFIG_HARDENED_USERCOPY
__attribute__((unused))
static bool hm_addr_cross_border(const unsigned long addr, const size_t size,
		const unsigned long start, const unsigned long end,
		bool end_included)
{
	bool is_cross_border = false;

	if (addr < start) {
		if (addr + size >= start) {
			is_cross_border = true;
		}
	} else {
		if (end_included) {
			if (addr + size > end && addr <= end) {
				is_cross_border = true;
			}
		} else if (addr + size >= end && addr < end) {
			is_cross_border = true;
		}
	}
	return is_cross_border;
}

__attribute__((unused))
static inline bool hm_addr_overlaps(const unsigned long addr, const size_t size,
		const unsigned long start, const unsigned long end)
{
	bool is_overlap = false;

	if (addr < end && addr + size >= start) {
		is_overlap = true;
	}
	return is_overlap;
}

#ifdef __hmbuild_target_is_static__
bool hm_addr_clear_of_text(const void *addr, const size_t size);
#else
typedef bool (*hm_addr_clear_of_text_t)(const void *, const size_t);
void hm_register_addr_clear_of_text_check_func(hm_addr_clear_of_text_t func);
#endif
#endif /* CONFIG_HARDENED_USERCOPY */

#endif /* ULIBS_LIBMEM_UMEM_H */
