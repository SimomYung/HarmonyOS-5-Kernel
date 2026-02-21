/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide function of symbol lookup
 * Author: Huawei OS Kernel Lab
 * Create: Wed 11 Dec 2019 11:41:08 AM CST
 */
#include <elf.h>
#include <hongmeng/errno.h>
#include <libhmelf/vsymbol.h>
#include <libhmelf/symbol.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/const.h>

static unsigned int sym_item_size(enum symbol_mode mode)
{
	return (mode == SYMBOL_MODE_32) ? (unsigned int)sizeof(Elf32_Sym) : (unsigned int)sizeof(Elf64_Sym);
}

static struct generic_sym_item get_sym_item(enum symbol_mode mode,
					    uintptr_t addr, uint64_t index)
{
	struct generic_sym_item item;

	if (mode == SYMBOL_MODE_32) {
		Elf32_Sym *sym = &((Elf32_Sym *)addr)[index];

		item.type = (unsigned char)ELF32_ST_TYPE(sym->st_info);
		item.value = (unsigned long)sym->st_value;
		item.index = sym->st_name;
		item.size = (unsigned long)sym->st_size;
		item.shndx = (unsigned short)sym->st_shndx;
	} else {
		Elf64_Sym *sym = &((Elf64_Sym *)addr)[index];

		item.type = (unsigned char)ELF64_ST_TYPE(sym->st_info);
		item.value = (unsigned long)sym->st_value;
		item.index = sym->st_name;
		item.size = (unsigned long)sym->st_size;
		item.shndx = (unsigned short)sym->st_shndx;
	}

	return item;
}

static int __vsymbol_info_lookup_by_addr(struct symbol_desc *sym_desc, enum symbol_mode mode,
					 unsigned long _addr, struct vsymbol_info *info)
{
	struct section_mem *sym_mem = NULL;
	struct section_mem *str_mem = NULL;
	uint64_t i, n;
	char *strtab = NULL;
	unsigned long addr = _addr;

	/* fill fname always so path can be find even when symbol is not found */
	info->fname = sym_desc->fname;
	info->load_offset = (unsigned long)sym_desc->base;

	addr = addr - sym_desc->offset;
	sym_mem = &sym_desc->secmem[SECTION_SYMTAB];
	str_mem = &sym_desc->secmem[SECTION_STRTAB];
	strtab = (char *)str_mem->addr;
	if (strtab == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}
	n = sym_mem->size / sym_item_size(mode);
	for (i = 0; i < n; i++) {
		struct generic_sym_item item;
		unsigned long distance;
		size_t count;

		item = get_sym_item(mode, sym_mem->addr, i);
		if (item.type != __U(STT_FUNC) && item.type != __U(STB_GLOBAL)) {
			continue;
		}
		if (item.index >= str_mem->size) {
			continue;
		}
		if (addr < item.value) {
			continue;
		}
		distance = addr - item.value;
		if (distance >= item.size) {
			continue;
		}
		count = (size_t)((str_mem->size - item.index) >= (SYMBOL_NAME_MAX_LEN - 1U) ?
				 (SYMBOL_NAME_MAX_LEN - 1U) : (str_mem->size - item.index));
		info->sname_size = count;
		info->sname = strtab + item.index;
		info->offset = distance;
		info->size = item.size;
		info->addr = sym_desc->offset + item.value;
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_OK;
	}

	return E_HM_POSIX_FAULT;
}

int vsymbol_info_lookup_by_addr(struct symbol_desc *sym_desc, enum symbol_mode mode,
				unsigned long _addr, struct vsymbol_info *info)
{
	int ret = E_HM_OK;

	if (info == NULL || sym_desc == NULL || _addr < sym_desc->offset) {
		ret =  E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = __vsymbol_info_lookup_by_addr(sym_desc, mode, _addr, info);
	}
	return ret;
}

static void make_symbol_entry(struct symbol_entry *entry,
			      const struct symbol_desc *sym_desc,
			      const struct vsymbol_info *info)
{
	entry->fname = info->fname;
	NOFAIL(strncpy_s(entry->name, SYMBOL_NAME_MAX_LEN, info->sname,
			 info->sname_size));
	entry->name[info->sname_size] = '\0';
	entry->load_offset = info->load_offset;
	entry->offset = info->offset;
	entry->size = info->size;
	entry->value = info->addr - sym_desc->offset;
}

int symbol_lookup_by_addr(struct symbol_desc *sym_desc, enum symbol_mode mode,
			  unsigned long _addr, struct symbol_entry *entry)
{
	int ret = E_HM_OK;
	struct vsymbol_info info = {0};

	if (entry == NULL || sym_desc == NULL || _addr < sym_desc->offset) {
		ret =  E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = __vsymbol_info_lookup_by_addr(sym_desc, mode, _addr, &info);
	}
	if (ret == E_HM_OK) {
		make_symbol_entry(entry, sym_desc, &info);
	} else {
		if (entry != NULL && sym_desc != NULL) {
			/* fill fname always so path can be find even when symbol is not found */
			entry->fname = sym_desc->fname;
			entry->load_offset = (unsigned long)sym_desc->base;
		}
	}
	return ret;
}

static int __vsymbol_info_lookup_by_name_and_type(struct symbol_desc *sym_desc,
						  enum symbol_mode mode,
						  const char *name, unsigned int type_mask,
						  struct vsymbol_info *info)
{
	struct section_mem *sym = NULL;
	struct section_mem *str = NULL;
	uint64_t i, n;
	char *strtab = NULL;

	sym = &sym_desc->secmem[SECTION_SYMTAB];
	str = &sym_desc->secmem[SECTION_STRTAB];
	strtab = (char *)str->addr;
	if (strtab == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}
	n = sym->size / sym_item_size(mode);
	for (i = 0; i < n; i++) {
		struct generic_sym_item item;
		size_t count;

		item = get_sym_item(mode, sym->addr, i);
		if (item.value == __UL(0) || item.shndx == (unsigned short)SHN_UNDEF) {
			continue;
		}
		if (((__U(1) << (unsigned int)item.type) & type_mask) == __U(0)) {
			continue;
		}
		if (item.index >= str->size) {
			continue;
		}

		if ((strlen(name) + 1) > (str->size - item.index)) {
			continue;
		}

		if (strcmp(name, strtab + item.index) != 0) {
			continue;
		}
		count = (str->size - item.index) >= (SYMBOL_NAME_MAX_LEN - 1U) ?
			(SYMBOL_NAME_MAX_LEN - 1U) : (str->size - item.index);
		info->sname_size = count;
		info->sname = strtab + item.index;
		info->size = item.size;
		info->addr = sym_desc->offset + item.value;
		info->fname = sym_desc->fname;
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_OK;
	}

	return E_HM_POSIX_FAULT;
}

int symbol_lookup_by_name_and_type(struct symbol_desc *sym_desc, enum symbol_mode mode,
				   const char *name, unsigned int type_mask, struct symbol_entry *entry)
{
	int ret = E_HM_OK;
	struct vsymbol_info info = {0};

	if (entry == NULL || sym_desc == NULL || name == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = __vsymbol_info_lookup_by_name_and_type(sym_desc, mode, name, type_mask, &info);
	}
	if (ret == E_HM_OK) {
		make_symbol_entry(entry, sym_desc, &info);
	}
	return ret;
}

#define VALID_TYPES ((1U << __U(STT_OBJECT)) | (1U << __U(STT_FUNC)) | (1U << __U(STT_TLS)))
int symbol_lookup_by_name(struct symbol_desc *sym_desc, enum symbol_mode mode,
			  const char *name, struct symbol_entry *entry)
{
	return symbol_lookup_by_name_and_type(sym_desc, mode, name, VALID_TYPES, entry);
}

int vsymbol_info_lookup_by_name(struct symbol_desc *sym_desc, enum symbol_mode mode,
				const char *name, struct vsymbol_info *info)
{
	int ret = E_HM_OK;

	if (info == NULL || sym_desc == NULL || name == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = __vsymbol_info_lookup_by_name_and_type(sym_desc, mode, name, VALID_TYPES, info);
	}
	return ret;
}
