/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definitions of symbol related operations
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 11 20:30:46 2019
 */
#ifndef SYSMGR_INTAPI_PROCESS_SYMBOL_H
#define SYSMGR_INTAPI_PROCESS_SYMBOL_H

#include <stdbool.h>
#include <libmem/types.h>
#include <libhmelf/symbol.h>
#include <hmasm/registers.h>
#include <hmdsosym.h>
#include <intapi/process/process.h>

#define SYMBOL_FNAME_LEN_MAX 512U

struct vspace_s;

struct layout_info {
	uintptr_t stack_start;
	uintptr_t stack_end;
};

#define SYMBOL_FLAG_NO_SYMBOL (1U) /* bit 0 */
/* do not load symbols from external files */
#define SYMBOL_FLAG_NO_LOAD (1U << 1) /* bit 1 */
/* do not trigger OOM when preparing symbols */
#define SYMBOL_FLAG_NO_OOM (1U << 2) /* bit 2 */
#define SYMBOL_FLAG_NO_ADDRESS (1U << 3) /* bit 3 */
#define SYMBOL_FLAG_USE_RELATIVE (1U << 4) /* bit 4 */
#define SYMBOL_FLAG_NO_FRAMEPOINTER (1U << 5) /* bit 5 */

struct symbol_impl {
	enum symbol_mode mode;
	/* see SYMBOL_FLAG_XXX above */
	unsigned int flags;
	struct symbol_list_s *symbol_list;
	const struct arch_regs *regs;
	struct vspace_s *vspace;
	struct layout_info layout;
	unsigned long load_offset;
	struct dump_impl *dump_impl;

	int (*sym_lookup)(struct symbol_impl *sym_impl,
			  enum symbol_mode mode, unsigned long addr,
			  struct symbol_entry *entry);
};

enum symbol_source {
	SYMBOL_NOTLOAD,
	SYMBOL_FROM_FILE,
	SYMBOL_FROM_DSO,
	SYMBOL_FROM_MAP,
	SYMBOL_LOADFAIL,
	SYMBOL_SRC_MAX,
};

bool is_dso_base_exists(struct symbol_list_s *symbol_list, unsigned long base);

void add_into_symbol_info(struct symbol_desc *desc,
			  struct symbol_list_s *symbol_list);

void process_add_dso_symbol(const uint64_t dso_info_addr,
			    struct vspace_s *vspace,
			    struct symbol_list_s *symbol_list);

int process_symbol_lookup(struct symbol_impl *sym_impl,
			  enum symbol_mode mode, unsigned long addr,
			  struct symbol_entry *entry);

void process_symbol_list_init(struct symbol_list_s *symbol_list, bool emergency);
int load_symbol_desc_from_maps(struct symbol_impl *symbol_impl,
		unsigned long addr, struct symbol_desc **ret_sym_desc);

struct symbol_desc *retrieve_symbol_of_file(struct symbol_list_s *symbol_list, const char *fname);
#ifdef CONFIG_DYN_LOAD_SYMBOL_FROM_FILE
/* note: ret_sym_desc must be released from caller */
int load_symbol_desc_from_file(struct symbol_impl *symbol_impl, enum symbol_mode mode,
		unsigned long addr, struct symbol_desc **ret_sym_desc);
int load_symbol_desc_of_file(struct process_s *process, struct vspace_s *vspace,
			const char *fname, struct symbol_desc **ret_sym_desc);
#endif
#ifdef CONFIG_LOOKUP_SYMBOL_FROM_DB
int load_symbol_db(void);
int lookup_symbol_from_system_db(struct symbol_impl *sym_impl, struct symbol_desc *sym_desc,
				 unsigned long addr, struct symbol_entry *entry);
#endif

#endif
