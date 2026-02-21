/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition for elf implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 7 19:28:21 2018
 */
#ifndef LIBELF_ELF_IMPL_H
#define LIBELF_ELF_IMPL_H

#include <hongmeng/panic.h>
#include <libhmelf/elf.h>

struct elf_impl {
	int (*elf_ehdr)(struct elf_context *ctx,
			struct elf_generic_header *ghdr);
	int (*elf_phdr)(struct elf_context *ctx, struct elf_generic_phdr *gphdr,
			unsigned int index);
	int (*elf_shdr)(struct elf_context *ctx, struct elf_generic_shdr *gshdr,
			unsigned int index);
	int (*elf_set_ehdr)(struct elf_context *ctx,
			    const struct elf_generic_header *gehdr);
	int (*elf_set_phdr)(struct elf_context *ctx,
			    const struct elf_generic_phdr *gphdr);
	int (*elf_set_nhdr)(struct elf_context *ctx,
			    const struct elf_generic_nhdr *gnhdr);
	size_t (*elf_ehdr_size)(const struct elf_context *ctx);
	size_t (*elf_phdr_size)(const struct elf_context *ctx);
	size_t (*elf_shdr_size)(const struct elf_context *ctx);
	size_t (*elf_nhdr_size)(const struct elf_context *ctx);
	int64_t (*elf_dyn_tag)(const struct elf_context *ctx, void *dyn_ent);
	uint64_t (*elf_dyn_ptr)(const struct elf_context *ctx, void *dyn_ent);
	size_t (*elf_dyn_ent_size)(const struct elf_context *ctx);
};

extern struct elf_impl elf32_impl;
extern struct elf_impl elf64_impl;

#define elf_fop(ctx, func, ...) \
	({ BUG_ON(ctx->fops == NULL || ctx->fops->func == NULL); \
		  ctx->fops->func(ctx, ##__VA_ARGS__); })

#define elf_imp(ctx, func, ...) \
	({ BUG_ON(ctx->impl == NULL || ctx->impl->func == NULL); \
		  ctx->impl->func(ctx, ##__VA_ARGS__); })

#endif
