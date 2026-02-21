/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces of load/unload sections in elf file
 * Author: Huawei OS Kernel Lab
 * Create: Wed 20 Nov 2019 03:08:55 PM CST
 */
#ifndef ULIBS_LIBHMELF_ELF_LOAD_H
#define ULIBS_LIBHMELF_ELF_LOAD_H

#include <elf.h>
#include <libhmelf/elf.h>
#include <libhmelf/symbol.h>

#include <libhmtrace/ftrace/function_trace.h>

int load_section(struct elf_context *ctx, const struct elf_generic_shdr *shdr,
		 struct section_mem *mem);
void unload_section(struct section_mem *mem);
int load_shstr_section(struct elf_context *ctx, struct section_mem *mem);
int load_section_by_name(const struct section_mem *shstr_mem, const char *name,
			 struct elf_context *ctx,  struct section_mem *mem);

void process_unload_symbol(struct symbol_desc *sym_desc);
int process_load_symbol(struct elf_context *ctx, struct symbol_desc *sym_desc);

int process_parse_elf(const char *name, struct elf_context *ctx);

int load_ftrace_stub_sec_mem(struct elf_context *ctx,
			     struct section_mem *ftrace_sec_mem);

void process_unload_ftrace_stub_section(struct ftrace_stub_section *ftrace_stub);
int process_load_ftrace_stub_section(struct elf_context *ctx,
				     struct ftrace_stub_section *ftrace_stub);
int elf_parse_ext_prot(struct elf_context *ctx, uint32_t *ext_prot);
#endif
