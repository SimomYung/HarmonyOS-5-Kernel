/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Self-reloc procedure of hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#include <dynlink.h>
#include <hm_ldso.h>

#include <hongmeng/types.h>
#include <libstrict/overflow.h>
#include <libhwsecurec/securec.h>

#define LDSO_START "_hm_ldso_start"
#include "crt_start.h"

#define SYSV_HASH_BASE 2U
#define VEC_TRAVEL_STRIDE 2
#define DT_REL_STRIDE 2
#define DT_RELA_STRIDE 3
#define ST_BIND_SHIFT 4
#define BIT_SHIFT 24
#define HASH_MULT 16
#define ldso_abort(...) __builtin_trap()
#define ldso_laddr(info, addr) ((void *)(uintptr_t)((info)->base + (addr)))

enum {
	RELOCATE_OFFSET,
	RELOCATE_INFO,
	RELOCATE_ADDEND
};

struct reloc_info {
	size_t base;
	uint32_t *hashtab;
	int16_t *versym;
	size_t *got;
	Elf_Sym *syms;
	char *strings;
	size_t *jmprel;
	size_t pltrelsz;
	size_t pltrel;
	size_t *rel;
	size_t relsz;
	size_t *rela;
	size_t relasz;
};

#if defined(__clang__)
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)
#else
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif

static uint32_t sym_hash(const char *s0)
{
	uint_fast32_t h = 0;
	const unsigned char *s = (void *)s0;
	while (*s != 0) {
		h = HASH_MULT * h + *s++;
		h ^= ((h >> BIT_SHIFT) & 0xf0);
	}
	return h & 0xfffffff;
}

static Elf_Sym *lookup_sym(const char *s, uint32_t h, struct reloc_info *info)
{
	size_t i;
	Elf_Sym *syms = NULL;
	char *strings = NULL;
	uint32_t *hashtab = NULL;

	if (s == NULL || info == NULL || info->syms == NULL || info->strings == NULL ||
	    info->hashtab == NULL)
		return NULL;

	syms = info->syms;
	strings = info->strings;
	hashtab = info->hashtab;

	for (i = hashtab[SYSV_HASH_BASE + h % hashtab[0]]; i != 0; i = hashtab[SYSV_HASH_BASE + hashtab[0] + i]) {
		if ((info->versym == NULL || info->versym[i] >= 0)
		    && (strcmp(s, strings + syms[i].st_name) == 0))
			return syms + i;
	}
	return NULL;
}

static Elf_Sym *find_sym(struct reloc_info *info, const char *s)
{
	uint32_t h = sym_hash(s);
	return lookup_sym(s, h, info);
}

static inline bool is_rel_type(int type)
{
	return (type == LDSO_REL_GOT || type == LDSO_REL_PLT || type == LDSO_REL_COPY);
}

static size_t fix_addend(const size_t *rel, const size_t *reloc_addr, size_t stride, int type)
{
	size_t addend;
	if (stride > DT_REL_STRIDE) {
		addend = rel[RELOCATE_ADDEND];
	} else if (is_rel_type(type)) {
		addend = 0;
	} else {
		addend = *reloc_addr;
	}
	return addend;
}

static void reloc_addr_init(size_t *reloc_addr, const Elf_Sym *sym, size_t sym_val, size_t addend, int type)
{
	switch (type) {
	case LDSO_REL_SYMBOLIC:
	case LDSO_REL_GOT:
	case LDSO_REL_PLT:
		*reloc_addr = sym_val + addend;
		break;
	case LDSO_REL_COPY:
		BUG_ON(sym == NULL);
		NOFAIL((int)memcpy_s(reloc_addr, (size_t)sym->st_size,
				     size_t_to_ptr(sym_val, void), (size_t)sym->st_size));
		break;
	default:
		ldso_abort("Abort: unsupported type: %d\n", type);
		break;
	}
}

static void __do_relocs(struct reloc_info *info, const size_t *rel, size_t rel_size,
			size_t stride)
{
	int type = 0;
	size_t sym_index = 0;
	const char *strings = NULL;
	Elf_Sym *syms = NULL;
	Elf_Sym *sym = NULL;
	Elf_Sym *def = NULL;
	size_t *reloc_addr = NULL;
	size_t sym_val = 0;
	size_t addend = 0;

	BUG_ON(info == NULL);
	BUG_ON(rel == NULL);
	BUG_ON(info->syms == NULL);
	BUG_ON(info->strings == NULL);
	BUG_ON(info->base == 0);

	syms = info->syms;
	strings = info->strings;

	for (; rel_size != 0; rel += stride, rel_size -= stride * sizeof(size_t)) {
		if (ldso_is_relative(rel[RELOCATE_INFO], info->syms)) {
			continue;
		}
		type = (int)R_TYPE(rel[RELOCATE_INFO]);
		if (type == 0) {
			continue;
		}
		reloc_addr = ldso_laddr(info, rel[RELOCATE_OFFSET]);
		addend = fix_addend(rel, reloc_addr, stride, type);

		sym_index = R_SYM(rel[RELOCATE_INFO]);
		if (sym_index != 0) {
			const char *name = NULL;

			sym = syms + sym_index;
			name = strings + sym->st_name;
			def = (sym->st_info & 0xf) == STT_SECTION ? sym : find_sym(info, name);
			if (def == NULL && ((sym->st_shndx != SHN_UNDEF)
					    || ((sym->st_info >> ST_BIND_SHIFT) != STB_WEAK))) {
				ldso_abort("Error relocating %s: symbol not found\n", name);
				break;
			}
		} else {
			sym = 0;
			def = 0;
		}
		sym_val = def != NULL ? (size_t)ldso_laddr(info, def->st_value) : 0;

		reloc_addr_init(reloc_addr, sym, sym_val, addend, type);
	}
}

static void do_relocs(struct reloc_info *info)
{
	if (info->jmprel != NULL) {
		__do_relocs(info, info->jmprel, info->pltrelsz,
			    (info->pltrel == DT_RELA ? DT_RELA_STRIDE : DT_REL_STRIDE));
	}
	if (info->rel != NULL) {
		__do_relocs(info, info->rel, info->relsz, DT_REL_STRIDE);
	}
	if (info->rela != NULL) {
		__do_relocs(info, info->rela, info->relasz, DT_RELA_STRIDE);
	}
}

static void do_self_reloc(size_t base, const size_t *dynv)
{
	struct reloc_info info;
	size_t dyn_tag;
	size_t dyn_val;

	mem_zero_s(info);

	info.base = base;
	info.versym = NULL;
	for (dyn_tag = dynv[0]; dyn_tag != DT_NULL; dynv += VEC_TRAVEL_STRIDE, dyn_tag = dynv[0]) {
		dyn_val = dynv[1];
		if (dyn_tag == DT_SYMTAB) {
			info.syms = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_STRTAB) {
			info.strings = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_HASH) {
			info.hashtab = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_PLTGOT) {
			info.got = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_VERSYM) {
			info.versym = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_JMPREL) {
			info.jmprel = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_PLTRELSZ) {
			info.pltrelsz = dyn_val;
		} else if (dyn_tag == DT_PLTREL) {
			info.pltrel = dyn_val;
		} else if (dyn_tag == DT_REL) {
			info.rel = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_RELSZ) {
			info.relsz = dyn_val;
		} else if (dyn_tag == DT_RELA) {
			info.rela = ldso_laddr(&info, dyn_val);
		} else if (dyn_tag == DT_RELASZ) {
			info.relasz = dyn_val;
		} else {
			/* do nothing */
		}
	}
	/* do relocations */
	do_relocs(&info);
}

static void self_reloc_relative(const size_t *rel, size_t rel_size, size_t step, size_t ldso_base)
{
	for (; rel_size != 0; rel += step, rel_size -= step * sizeof(size_t)) {
		if (!ldso_is_relative(rel[RELOCATE_INFO], 0)) {
			continue;
		}
		size_t *rel_addr = ulong_to_ptr(ldso_base + rel[RELOCATE_OFFSET], size_t);
		size_t addend = step > DT_REL_STRIDE ? rel[RELOCATE_ADDEND] : *rel_addr;
		*rel_addr = ldso_base + addend;
	}
}

static __attribute__((used)) void _hm_ldso_start_c(size_t *sp, const size_t *ldso_dynv)
{
	size_t *rel = NULL;
	size_t rel_size;
	size_t ldso_base;
	size_t i;
	size_t aux[LDSO_AUX_CNT];
	size_t dyn[LDSO_DYN_CNT];

	size_t argc = (size_t)(ssize_t)*sp;
	char **argv = (void *)(sp + 1);

	i = argc + 1;
	while (argv[i]) {
		i++;
	}
	size_t *auxv = (void *)(argv + i + 1);
	for (i = 0; i < LDSO_AUX_CNT; i++) {
		aux[i] = 0;
	}

	for (i = 0; auxv[i] != 0; i += VEC_TRAVEL_STRIDE) {
		if (auxv[i] < LDSO_AUX_CNT) {
			aux[auxv[i]] = auxv[i + 1];
		}
	}

	for (i = 0; i < LDSO_DYN_CNT; i++) {
		dyn[i] = 0;
	}
	for (i = 0; ldso_dynv[i] != 0; i += VEC_TRAVEL_STRIDE) {
		if (ldso_dynv[i] < LDSO_DYN_CNT) {
			dyn[ldso_dynv[i]] = ldso_dynv[i + 1];
		}
	}

	ldso_base = aux[AT_BASE];
	rel = ulong_to_ptr(ldso_base + dyn[DT_REL], size_t);
	rel_size = dyn[DT_RELSZ];
	self_reloc_relative(rel, rel_size, DT_REL_STRIDE, ldso_base);

	rel = ulong_to_ptr(ldso_base + dyn[DT_RELA], size_t);
	rel_size = dyn[DT_RELASZ];
	self_reloc_relative(rel, rel_size, DT_RELA_STRIDE, ldso_base);

	/* Do self-relocate here */
	do_self_reloc(ldso_base, ldso_dynv);

	/* jump to hm ldso main function */
	_hm_ldso_main(sp, (int)argc, argv);
	__builtin_unreachable();
}

#if defined(__clang__)
#pragma clang attribute pop
#else
#pragma GCC pop_options
#endif
