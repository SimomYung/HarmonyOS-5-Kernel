/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implement of lookup symbol virtual address
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 20 16:04:54 2020
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmelf/elf.h>
#include <libhmelf/symbol.h>
#include <libhmelf/elf_load.h>
#include <libbunch/bunch_ipc.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmelf/vsymbol.h>
#include <hmkernel/const.h>
#include <libcrt/hmcrt.h>
#include <hmdsosym.h>

enum lookup_type {
	LOOKUP_BY_NAME,
	LOOKUP_BY_ADDR,
};

struct lookup_option {
	enum lookup_type type;
	union {
		const char *name;
		unsigned long addr;
	} arg;
};

static enum symbol_mode get_symbol_mode(void)
{
	return (sizeof(int) == sizeof(long)) ? SYMBOL_MODE_32 : SYMBOL_MODE_64;
}

static int lookup_vsymbol(struct symbol_desc *sym_desc,
			  const struct lookup_option *opt,
			  struct vsymbol_info *info)
{
	enum symbol_mode sym_mode = get_symbol_mode();
	int ret;

	switch (opt->type) {
	case LOOKUP_BY_NAME:
		ret = vsymbol_info_lookup_by_name(sym_desc, sym_mode,
						  opt->arg.name, info);
		break;
	case LOOKUP_BY_ADDR:
		ret = vsymbol_info_lookup_by_addr(sym_desc, sym_mode,
						  opt->arg.addr, info);
		break;
	default:
		ret = E_HM_NOSYS;
		break;
	}

	return ret;
}

static void fill_sym_desc(const struct dso_sym_entry *entry,
			  struct symbol_desc *sym_desc)
{
	sym_desc->fname = (char *)(uintptr_t)entry->fname;
	sym_desc->base = (uintptr_t)entry->base;
	if (entry->elf_type == __U(ET_DYN)) {
		sym_desc->offset = (uintptr_t)entry->base;
	}
	sym_desc->base_len = entry->base_len;
	sym_desc->dso_sym.symtab_addr = (unsigned long)entry->symtab_addr;
	sym_desc->dso_sym.strtab_addr = (unsigned long)entry->strtab_addr;
	sym_desc->dso_sym.symtab_len = entry->symtab_len;
	sym_desc->dso_sym.strtab_len = entry->strtab_len;
	sym_desc->secmem[SECTION_SYMTAB].addr = (uintptr_t)entry->symtab_addr;
	sym_desc->secmem[SECTION_SYMTAB].size = (size_t)entry->symtab_len;
	sym_desc->secmem[SECTION_STRTAB].addr = (uintptr_t)entry->strtab_addr;
	sym_desc->secmem[SECTION_STRTAB].size = (size_t)entry->strtab_len;
}

static int lookup_vsymbol_by_entry(const struct lookup_option *opt,
				   struct vsymbol_info *info,
				   const struct dso_sym_entry *entry,
				   bool *need_break)
{
	int ret = E_HM_OK;
	struct symbol_desc sym_desc;

	mem_zero_s(sym_desc);
	fill_sym_desc(entry, &sym_desc);
	if (sym_desc.fname == NULL || sym_desc.fname[0] == '\0') {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK &&
	    (sym_desc.dso_sym.strtab_addr == 0ULL ||
	     sym_desc.dso_sym.symtab_addr == 0ULL)) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = lookup_vsymbol(&sym_desc, opt, info);
		if (ret == E_HM_OK) {
			*need_break = true;
		}
	}

	return ret;
}

static int check_dso_info(const struct dso_sym_info *cur_dso_info)
{
	int ret = E_HM_OK;
	unsigned long max_sym_entrys;

	if (strcmp(cur_dso_info->magic, DSO_INFO_MAGIC) != 0) {
		hm_warn("failed to match the magic, magic=%s\n", cur_dso_info->magic);
		ret = E_HM_POSIX_FAULT;
	}
	if (ret == E_HM_OK) {
		max_sym_entrys = (MEM_SIZE_FOR_DSO_INFO -
				  (ptr_to_ulong(&(cur_dso_info->main_elf_entry)) -
				   ptr_to_ulong(cur_dso_info))) /
				 (sizeof(struct dso_sym_entry));
		if (cur_dso_info->nr_sym_entry > max_sym_entrys) {
			hm_warn("sym entry num exceeded, curr_num=%u, max_num=%lu\n",
			cur_dso_info->nr_sym_entry, max_sym_entrys);
			ret = E_HM_INVAL;
		}
	}
	return ret;
}

static int lookup_vsymbol_in_proc(const char *filename,
				  const struct lookup_option *opt,
				  struct vsymbol_info *info)
{
	int ret = E_HM_OK;
	bool need_break = false;
	struct dso_sym_info *cur_dso_info = NULL;
	struct dso_sym_entry *entries = NULL;

	cur_dso_info = dso_syminfo;
	while (cur_dso_info != NULL) {
		ret = check_dso_info(cur_dso_info);
		if (ret != E_HM_OK) {
			hm_debug("check dso info failed\n");
			break;
		}
		entries = &cur_dso_info->main_elf_entry;
		for (unsigned int i = 0; i < cur_dso_info->nr_sym_entry; ++i) {
			if (entries[i].flag == 0) {
				continue;
			}
			if (filename != NULL &&
			    strncmp(filename, (char *)(uintptr_t)entries[i].fname, strlen(filename)) != 0) {
				continue;
			}
			ret = lookup_vsymbol_by_entry(opt, info, &entries[i], &need_break);
			if (need_break) {
				break;
			}
		}
		if (need_break) {
			break;
		}
		cur_dso_info = (struct dso_sym_info *)(uintptr_t)cur_dso_info->next_page;
	}

	return ret;
}

int hm_elf_lookup_vsymbol_by_name(const char *filename,
				  const char *name,
				  struct vsymbol_info *info)
{
	struct lookup_option opt;
	int rc;

	if (name == NULL || name[0] == '\0' || info == NULL) {
		hm_debug("invalid symbol name or output info argument\n");
		rc = E_HM_INVAL;
	} else {
		opt.type = LOOKUP_BY_NAME;
		opt.arg.name = name;
		rc = lookup_vsymbol_in_proc(filename, &opt, info);
	}
	return rc;
}

int hm_elf_lookup_vsymbol_by_addr(unsigned long addr,
				  struct vsymbol_info *info)
{
	struct lookup_option opt;
	int rc;

	if (addr == 0UL || info == NULL) {
		hm_debug("invalid symbol address or output info argument\n");
		rc = E_HM_INVAL;
	} else {
		opt.type = LOOKUP_BY_ADDR;
		opt.arg.addr = addr;
		rc = lookup_vsymbol_in_proc(NULL, &opt, info);
	}
	return rc;
}
