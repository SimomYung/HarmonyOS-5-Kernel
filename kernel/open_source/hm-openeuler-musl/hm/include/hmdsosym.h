/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 23 14:53:17 2019
 */

#ifndef HM_INCLUDE_HMDSOSYM_H
#define HM_INCLUDE_HMDSOSYM_H

#include <bits/alltypes.h>

struct dso_sym_entry {
	uint32_t flag; /* Is entry valid or not, 0:invalid, 1:valid */
	uint32_t elf_type; /* Object file type */
	uint64_t fname; /* Object file name */
	uint64_t base; /* Address of dynamic shared object loaded */
	uint64_t tls_offset; /* Offset from TP of dso's TLS */
	uint64_t symtab_addr; /* Address of symbol section loaded */
	uint64_t strtab_addr; /* Address of strings section loaded */
	uint32_t fname_len; /* Length of file name, include last null character */
	uint32_t base_len; /* Length of dso */
	uint32_t symtab_len; /* Length of symbol section loaded */
	uint32_t strtab_len; /* Length of strings section loaded */
};

#define MEM_SIZE_FOR_DSO_INFO	(4096)
#define DSO_INFO_MAGIC	"HMDSO"
#define DSO_INFO_MAGIC_LEN	(16)
#define MAIN_ELF_NAME_LEN   (64)
#define HMLDSO_ELF_NAME_LEN (24)

/* dso_sym_info and dso_sym_entry memory structure, must align to 4k.
 * The data members in dso_sym_info and dso_sym_entry should be of fixed length.
 *
 *      base page                         cur_page                            next_page
 *  ------------------ <--    ------> ------------------ <--    ------>  ------------------
 * | ---------------- |   |  |       | ---------------- |   |  |        | ---------------- |
 * ||  dso_sym_info  ||    --|---    ||  dso_sym_info  ||    --|---     ||  dso_sym_info  ||
 * ||----------------|| NULL |   |   ||----------------||      |   |    ||----------------|| NULL
 * ||      ...       ||   ^  |   |   ||      ...       ||      |   |    ||      ...       ||   ^
 * ||   prev_page    ||___|  |    ---||   prev_page    ||      |    ----||   prev_page    ||   |
 * ||   next_page    ||------        ||   next_page    ||------         ||   next_page    ||___|
 * || main_elf_entry ||              || main_elf_entry ||               || main_elf_entry ||
 * ||hmldso_elf_entry||              ||hmldso_elf_entry||               ||hmldso_elf_entry||
 * | ---------------- |              | ---------------- |               | ---------------- |
 * | ---------------- |              | ---------------- |               | ---------------- |
 * || dso_sym_entry  ||              || dso_sym_entry  ||               || dso_sym_entry  ||
 * | ---------------- |              | ---------------- |               | ---------------- |
 * | ---------------- |              | ---------------- |               | ---------------- |
 * || dso_sym_entry  ||              || dso_sym_entry  ||               || dso_sym_entry  ||
 * | ---------------- |              | ---------------- |               | ---------------- |
 * |       ...        |              |       ...        |               |       ...        |
 *  ------------------                ------------------                 ------------------
 *
 */
struct dso_sym_info {
	char magic[DSO_INFO_MAGIC_LEN]; /* magic info, default is "HMDSO" */
	uint32_t nr_sym_entry; /* number of available entries  */
	uint32_t nr_valid_entry; /* current number of valid entries */
	uint64_t prev_page; /* point to the previous page of the current page */
	uint64_t next_page; /* point to the next page of the current page */
	char main_elf_name[MAIN_ELF_NAME_LEN]; /* save the name of main elf */
	char hmldso_name[HMLDSO_ELF_NAME_LEN]; /* save the name of hmldso elf */
	struct dso_sym_entry main_elf_entry; /* save the main elf information */
	struct dso_sym_entry hmldso_elf_entry; /* save the hmldso elf information */
};

int fill_dso_syminfo(struct dso_sym_entry *sym_entry);
void drop_dso_syminfo(const void *base);

#endif
