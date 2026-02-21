/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide type declaration of symbol and interfaces of symbol_lookup
 * Author: Huawei OS Kernel Lab
 * Create: Wed 11 Dec 2019 11:41:08 AM CST
 */
#ifndef ULIBS_LIBHMELF_SYMBOL_H
#define ULIBS_LIBHMELF_SYMBOL_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <hongmeng/panic.h>

#define SYMBOL_NAME_MAX_LEN		128U
#define UNKNOWN_NAME			"?????"

enum symbol_mode {
	SYMBOL_MODE_32,
	SYMBOL_MODE_64,
};

#define MAIN_ELF_SYMBOL			1U
#define DSO_ELF_SYMBOL			2U
#define HMLDSO_ELF_SYMBOL               3U

struct symbol_entry {
	const char *fname; /* symbol in which binary file */
	unsigned long load_offset; /* load offset of this binary file */

	char name[SYMBOL_NAME_MAX_LEN];
	unsigned long offset; /* offset to start of the function */
	unsigned long size;
	unsigned long value;
};

struct generic_sym_item {
	unsigned long value;
	unsigned long size;
	unsigned int index;
	unsigned short shndx;
	unsigned char type;
};

enum section_type {
	SECTION_SYMTAB,
	SECTION_STRTAB,
	SECTION_MAX,
};

struct section_mem {
	uintptr_t addr;
	size_t size;
};

struct dso_sym_range {
	unsigned long symtab_addr;
	unsigned long strtab_addr;
	unsigned int symtab_len;
	unsigned int strtab_len;
};

struct symbol_desc {
	const char *fname; /* binary file name */
	unsigned int flags;
	uintptr_t base;
	unsigned int base_len;
	unsigned int elf_type;
	uintptr_t offset;
	unsigned int load_state; /* mark load state */
	struct section_mem secmem[SECTION_MAX];
	struct dso_sym_range dso_sym; /* come from dso symbol info */
	struct symbol_desc *next; /* link all parsed symbol_desc */
	char data[0]; /* put file name content here if needed */
};

static inline struct symbol_desc *symbol_desc_next(struct symbol_desc *desc)
{
	return (desc != NULL) ? (desc->next) : NULL;
}

struct symbol_list_s {
	struct symbol_desc *head;
	void *(*malloc_func)(size_t size);
	void (*free_func)(uintptr_t ptr);
};

#define SYMBOL_LIST_INITIALIZER {.head = NULL, .malloc_func = NULL, .free_func = NULL}

static inline void symbol_list_init_malloc_free(struct symbol_list_s *symbol_list,
		void *(*malloc_func)(size_t size), void (*free_func)(uintptr_t ptr))
{
	BUG_ON(malloc_func == NULL || free_func == NULL);
	symbol_list->malloc_func = malloc_func;
	symbol_list->free_func = free_func;
}

static inline void *symbol_list_malloc(struct symbol_list_s *symbol_list, size_t size,
		void *(*default_malloc_func)(size_t size))
{
	void *ptr = NULL;
	void *(*func)(size_t size) = NULL;
	func = (symbol_list == NULL) ? NULL : symbol_list->malloc_func;
	if (func == NULL) {
		func = default_malloc_func;
	}
	if (func != NULL) {
		ptr = func(size);
	}

	return ptr;
}

static inline void symbol_list_free(struct symbol_list_s *symbol_list, uintptr_t ptr,
		void (*default_free_func)(uintptr_t ptr))
{
	void (*func)(uintptr_t ptr) = NULL;
	if (ptr != 0) {
		func = (symbol_list == NULL) ? NULL : symbol_list->free_func;
		if (func == NULL) {
			func = default_free_func;
		}
		if (func != NULL) {
			func(ptr);
		}
	}
}

static inline struct symbol_desc *symbol_list_head_of(struct symbol_list_s *symbol_list)
{
	return ((symbol_list != NULL) ? symbol_list->head : NULL);
}

static inline void symbol_list_init(struct symbol_list_s *symbol_list, struct symbol_desc *desc)
{
	BUG_ON(symbol_list == NULL || symbol_list->head != NULL);
	symbol_list->head = desc;
}

static inline void symbol_list_reset(struct symbol_list_s *symbol_list)
{
	if (symbol_list != NULL) {
		symbol_list->head = NULL;
	}
}

static inline bool symbol_list_empty(const struct symbol_list_s *symbol_list)
{
	return (symbol_list == NULL || symbol_list->head == NULL);
}

#define foreach_symbol_desc(symbol_list, sym_desc) \
	for ((sym_desc) = (((symbol_list) == NULL) ? NULL : (symbol_list)->head);  \
			(sym_desc) != NULL; \
			(sym_desc) = (sym_desc)->next)

#define foreach_symbol_desc_safe(symbol_list, sym_desc, tmp) \
	for ((sym_desc) = symbol_list_head_of(symbol_list), (tmp) = symbol_desc_next(sym_desc); \
			(sym_desc) != NULL; \
			(sym_desc) = (tmp), (tmp) = symbol_desc_next(tmp))

struct vsymbol_info;

int symbol_lookup_by_addr(struct symbol_desc *sym_desc, enum symbol_mode mode,
			  unsigned long _addr, struct symbol_entry *entry);
int vsymbol_info_lookup_by_addr(struct symbol_desc *sym_desc, enum symbol_mode mode,
				unsigned long _addr, struct vsymbol_info *info);
int symbol_lookup_by_name(struct symbol_desc *sym_desc, enum symbol_mode mode,
			  const char *name, struct symbol_entry *entry);
int symbol_lookup_by_name_and_type(struct symbol_desc *sym_desc, enum symbol_mode mode,
				   const char *name, unsigned int type_mask, struct symbol_entry *entry);
int vsymbol_info_lookup_by_name(struct symbol_desc *sym_desc, enum symbol_mode mode,
				const char *name, struct vsymbol_info *info);
#endif
