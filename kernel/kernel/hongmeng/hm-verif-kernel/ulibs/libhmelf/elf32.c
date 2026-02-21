/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implementation for 32-bit ELF format operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 21 04:01:12 2018 -0400
 */

#include <elf.h>

#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include "elf_impl.h"

static int elf32_ehdr(struct elf_context *ctx, struct elf_generic_header *ghdr)
{
	size_t size;
	ssize_t len;
	Elf32_Ehdr ehdr;
	int rc = E_HM_OK;

	mem_zero_s(ehdr);
	size = sizeof(ehdr);
	len = elf_fop(ctx, read, &ehdr, 0, size);
	if ((size_t)len == size) {
		ghdr->elf_data        = (unsigned int)(ehdr.e_ident[EI_DATA]);
		ghdr->elf_class       = (unsigned int)(ehdr.e_ident[EI_CLASS]);
		ghdr->elf_type        = (unsigned int)(ehdr.e_type);
		ghdr->elf_machine     = (unsigned int)(ehdr.e_machine);
		ghdr->elf_version     = (unsigned int)(ehdr.e_version);
		ghdr->elf_flags       = (unsigned int)(ehdr.e_flags);
		ghdr->elf_entry_point = (uint64_t)(ehdr.e_entry);

		ghdr->phdr_offset     = (unsigned long)(ehdr.e_phoff);
		ghdr->shdr_offset     = (unsigned long)(ehdr.e_shoff);
		ghdr->ehdr_size       = (unsigned int)(ehdr.e_ehsize);
		ghdr->phdr_nr         = (unsigned int)(ehdr.e_phnum);
		ghdr->shdr_nr         = (unsigned int)(ehdr.e_shnum);
		ghdr->phdr_size       = (unsigned int)(ehdr.e_phentsize);
		ghdr->shdr_size       = (unsigned int)(ehdr.e_shentsize);
		ghdr->shstr_index     = (unsigned int)(ehdr.e_shstrndx);
	} else if (len < 0) {
		rc = (int)len;
	} else {
		rc = E_HM_IO;
	}
	return rc;
}

static int elf32_phdr(struct elf_context *ctx, struct elf_generic_phdr *gphdr,
		      unsigned int index)
{
	size_t size;
	ssize_t len;
	Elf32_Phdr phdr;
	int rc = E_HM_OK;
	struct elf_generic_header *ghdr = &ctx->elf_header;

	if (index >= ghdr->phdr_nr) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(phdr);
		size = sizeof(phdr);
		len = elf_fop(ctx, read, &phdr, (off_t)(ghdr->phdr_offset + size * index), size);
		if ((size_t)len == size) {
			gphdr->type     = (unsigned int)(phdr.p_type);
			gphdr->flags    = (unsigned int)(phdr.p_flags);
			gphdr->offset   = (uint64_t)(phdr.p_offset);
			gphdr->vaddr    = (uint64_t)(phdr.p_vaddr);
			gphdr->filesz   = (uint64_t)(phdr.p_filesz);
			gphdr->memsz    = (uint64_t)(phdr.p_memsz);
			gphdr->align    = (uint64_t)(phdr.p_align);
		} else if (len < 0) {
			rc = (int)len;
		} else {
			rc = E_HM_IO;
		}
	}

	return rc;
}

static int elf32_shdr(struct elf_context *ctx, struct elf_generic_shdr *gshdr,
		      unsigned int index)
{
	size_t size;
	ssize_t len;
	Elf32_Shdr shdr;
	int rc = E_HM_OK;
	struct elf_generic_header *ghdr = &ctx->elf_header;

	if (index >= ghdr->shdr_nr) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(shdr);
		size = sizeof(shdr);
		len = elf_fop(ctx, read, &shdr, (off_t)(ghdr->shdr_offset + size * index), size);
		if ((size_t)len == size) {
			gshdr->name     = (unsigned int)(shdr.sh_name);
			gshdr->type     = (unsigned int)(shdr.sh_type);
			gshdr->link	= (unsigned int)(shdr.sh_link);
			gshdr->addr     = (uint64_t)(shdr.sh_addr);
			gshdr->offset   = (uint64_t)(shdr.sh_offset);
			gshdr->size     = (uint64_t)(shdr.sh_size);
		} else if (len < 0) {
			rc = (int)len;
		} else {
			rc = E_HM_IO;
		}
	}

	return rc;
}

static int elf32_set_ehdr(struct elf_context *ctx,
			  const struct elf_generic_header *gehdr)
{
	size_t size;
	ssize_t len;
	Elf32_Ehdr ehdr;

	mem_zero_s(ehdr);
	NOFAIL(memcpy_s(&ehdr.e_ident[0], EI_NIDENT, ELFMAG, SELFMAG));

	ehdr.e_ident[EI_CLASS]   = ELFCLASS32;
	ehdr.e_ident[EI_DATA]    = (unsigned char)gehdr->elf_data;
	ehdr.e_ident[EI_VERSION] = EV_CURRENT;
	ehdr.e_ident[EI_OSABI]   = ELFOSABI_NONE;

	ehdr.e_entry             = (Elf32_Addr)gehdr->elf_entry_point;
	ehdr.e_type              = (Elf32_Half)gehdr->elf_type;
	ehdr.e_machine           = (Elf32_Half)gehdr->elf_machine;
	ehdr.e_version           = (Elf32_Word)gehdr->elf_version;
	ehdr.e_phoff             = (Elf32_Off)gehdr->phdr_offset;
	ehdr.e_shoff             = (Elf32_Off)gehdr->shdr_offset;
	ehdr.e_flags             = (Elf32_Word)gehdr->elf_flags;
	ehdr.e_ehsize            = (Elf32_Half)sizeof(Elf32_Ehdr);
	ehdr.e_phentsize         = (Elf32_Half)sizeof(Elf32_Phdr);
	ehdr.e_phnum             = (Elf32_Half)gehdr->phdr_nr;
	ehdr.e_shnum             = (Elf32_Half)gehdr->shdr_nr;
	ehdr.e_shstrndx          = (Elf32_Half)gehdr->shstr_index;

	size = sizeof(ehdr);
	len = elf_fop(ctx, write, &ehdr, size);
	return ((size_t)len != size) ? (len < 0 ? (int)len : E_HM_IO) : E_HM_OK;
}

static int elf32_set_phdr(struct elf_context *ctx,
			  const struct elf_generic_phdr *gphdr)
{
	size_t size;
	ssize_t len;
	Elf32_Phdr phdr;

	mem_zero_s(phdr);

	phdr.p_type     = (Elf32_Word)gphdr->type;
	phdr.p_offset   = (Elf32_Off)gphdr->offset;
	phdr.p_vaddr    = (Elf32_Addr)gphdr->vaddr;
	phdr.p_filesz   = (Elf32_Word)gphdr->filesz;
	phdr.p_memsz    = (Elf32_Word)gphdr->memsz;
	phdr.p_flags    = (Elf32_Word)gphdr->flags;
	phdr.p_paddr    = (Elf32_Addr)gphdr->paddr;
	phdr.p_align    = (Elf32_Word)gphdr->align;

	size = sizeof(phdr);
	len = elf_fop(ctx, write, &phdr, size);
	return ((size_t)len != size) ? (len < 0 ? (int)len : E_HM_IO) : E_HM_OK;
}

static int elf32_set_nhdr(struct elf_context *ctx,
			  const struct elf_generic_nhdr *gnhdr)
{
	size_t size;
	ssize_t len;
	Elf32_Nhdr nhdr;

	mem_zero_s(nhdr);

	nhdr.n_namesz   = (Elf32_Word)gnhdr->namesz;
	nhdr.n_descsz   = (Elf32_Word)gnhdr->descsz;
	nhdr.n_type     = (Elf32_Word)gnhdr->type;

	size = sizeof(nhdr);
	len = elf_fop(ctx, write, &nhdr, size);
	return ((size_t)len != size) ? (len < 0 ? (int)len : E_HM_IO) : E_HM_OK;
}

static size_t elf32_ehdr_size(const struct elf_context *ctx)
{
	UNUSED(ctx);
	return sizeof(Elf32_Ehdr);
}

static size_t elf32_phdr_size(const struct elf_context *ctx)
{
	UNUSED(ctx);
	return sizeof(Elf32_Phdr);
}

static size_t elf32_shdr_size(const struct elf_context *ctx)
{
	UNUSED(ctx);
	return sizeof(Elf32_Shdr);
}

static size_t elf32_nhdr_size(const struct elf_context *ctx)
{
	UNUSED(ctx);
	return sizeof(Elf32_Nhdr);
}

static int64_t elf32_dyn_tag(const struct elf_context *ctx, void *dyn_ent)
{
	UNUSED(ctx);
	Elf32_Dyn *ent = (Elf32_Dyn *)dyn_ent;
	return (int64_t)(ent->d_tag);
}

static uint64_t elf32_dyn_ptr(const struct elf_context *ctx, void *dyn_ent)
{
	UNUSED(ctx);
	Elf32_Dyn *ent = (Elf32_Dyn *)dyn_ent;
	return (uint64_t)(ent->d_un.d_ptr);
}

static size_t elf32_dyn_ent_size(const struct elf_context *ctx)
{
	UNUSED(ctx);
	return sizeof(Elf32_Dyn);
}

struct elf_impl elf32_impl = {
	.elf_ehdr         = elf32_ehdr,
	.elf_phdr         = elf32_phdr,
	.elf_shdr         = elf32_shdr,
	.elf_set_ehdr     = elf32_set_ehdr,
	.elf_set_phdr     = elf32_set_phdr,
	.elf_set_nhdr     = elf32_set_nhdr,
	.elf_ehdr_size    = elf32_ehdr_size,
	.elf_phdr_size    = elf32_phdr_size,
	.elf_shdr_size    = elf32_shdr_size,
	.elf_nhdr_size    = elf32_nhdr_size,
	.elf_dyn_tag      = elf32_dyn_tag,
	.elf_dyn_ptr      = elf32_dyn_ptr,
	.elf_dyn_ent_size = elf32_dyn_ent_size,
};
