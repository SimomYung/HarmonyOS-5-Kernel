/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: EL2 map functions
 * Author: Huawei OS Kernel Lab
 * Created: Fri Mar 17 2023
 */

#ifndef KLIBS_MAP_H
#define KLIBS_MAP_H

typedef unsigned long libmap_vaddr_t;
typedef unsigned long long libmap_paddr_t;
typedef unsigned long libmap_pte_t;

struct libmap_pgtable_context_s;

extern
int libmap_pgtable_zero_ll(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			   libmap_paddr_t pte_pa, libmap_vaddr_t vstart);

extern
int libmap_pgtable_zero_lb(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			   libmap_paddr_t pte_pa, libmap_vaddr_t vstart);

extern
int libmap_pgtable_populate_lb(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			       libmap_paddr_t pte_pa, libmap_vaddr_t vstart, libmap_pte_t ptev);

extern
int libmap_pgtable_populate_ll(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			       libmap_paddr_t pte_pa, libmap_vaddr_t vstart, libmap_pte_t ptev);

extern
int libmap_pgtable_populate_lt(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			       libmap_paddr_t pte_pa, libmap_vaddr_t vstart, libmap_pte_t ptev);

extern
int libmap_pgtable_refcnt_dec_curpa(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			       libmap_paddr_t cur_pa);

extern
int libmap_pgtable_populate_lt_pte(const struct libmap_pgtable_context_s *ctx_pa, void *((*pa2va)(libmap_paddr_t pa)),
			       libmap_paddr_t pte_pa, libmap_paddr_t cur_pa, libmap_pte_t ptev);
#endif
