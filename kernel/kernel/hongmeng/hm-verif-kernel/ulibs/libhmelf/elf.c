/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Provide ELF format operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 21 04:01:12 2018 -0400
 */
/* use musl elf header file */
#include <elf.h>
#include <errno.h>

#include <libhmelf/elf.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/const.h>

#include "elf_impl.h"

static int set_elf_impl(struct elf_context *ctx, unsigned int elf_class)
{
	int rc = E_HM_OK;

	switch (elf_class) {
	case ELFCLASS32:
		ctx->impl = &elf32_impl;
		break;
	case ELFCLASS64:
		ctx->impl = &elf64_impl;
		break;
	default:
		hm_warn("unknown ELF class: %u\n", elf_class);
		rc = E_HM_NOEXEC;
		break;
	}

	return rc;
}

static void clear_elf_impl(struct elf_context *ctx)
{
	if (ctx->impl != NULL) {
		ctx->impl = NULL;
	}
}

static int check_elf_magic(const void *ident, size_t len)
{
	int rc;

	if (len < __U(SELFMAG)) {
		rc = E_HM_NOEXEC;
	} else if (memcmp(ident, ELFMAG, SELFMAG) != 0) {
		rc = E_HM_NOEXEC;
	} else {
		rc = E_HM_OK;
	}
	return rc;
}

static int set_elf_header(struct elf_context *ctx)
{
	int err = E_HM_OK;
	unsigned char ident[EI_NIDENT];

	err = elf_file_read(ctx, ident, 0, sizeof(ident));
	if (err != E_HM_OK) {
		err = E_HM_NOEXEC;
	}
	if (err == E_HM_OK) {
		err = check_elf_magic((void *)ident, sizeof(ident));
	}
	if (err == E_HM_OK) {
		err = set_elf_impl(ctx, ident[EI_CLASS]);
		if (err != E_HM_OK) {
			hm_warn("set elf impl failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = elf_imp(ctx, elf_ehdr, &ctx->elf_header);
		if (err != E_HM_OK) {
			hm_warn("read elf header failed: %s\n", hmstrerror(err));
			clear_elf_impl(ctx);
		}
	}
	return err;
}

int elf_open_ctx(struct elf_context *ctx, struct elf_fops *fops,
		 const char *elf_file, void *private_data)
{
	int err = E_HM_OK;

	BUG_ON(ctx == NULL || fops == NULL);
	if (elf_file == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		ctx->fops = fops;
		if (private_data != NULL) {
			ctx->private_data = private_data;
		}
		err = elf_fop(ctx, open, elf_file);
		if (err != E_HM_OK) {
			hm_debug("open elf file '%s' failed: %s\n",
				 elf_file, hmstrerror(err));
			if (err == E_HM_ISDIR) {
				err = E_HM_ACCES;
			}
			ctx->fops = NULL;
		}
	}
	return err;
}

int elf_init_ctx(struct elf_context *ctx, struct elf_fops *fops,
		 const char *elf_file, void *private_data)
{
	int err;

	err = elf_open_ctx(ctx, fops, elf_file, private_data);
	if (err != E_HM_OK) {
		hm_debug("elf open ctx failed: %s\n", hmstrerror(err));
	}
	if (err == E_HM_OK) {
		err = set_elf_header(ctx);
		if (err != E_HM_OK) {
			elf_destroy_ctx(ctx);
		}
	}
	return err;
}

void elf_destroy_ctx(struct elf_context *ctx)
{
	int err;

	BUG_ON(ctx == NULL);

	err = elf_fop(ctx, close);
	if (err != E_HM_OK) {
		hm_warn("close elf context failed: %s\n", hmstrerror(err));
	}
	mem_zero_s(*ctx);
}

int elf_load_ehdr(struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);

	return elf_imp(ctx, elf_ehdr, &ctx->elf_header);
}

int elf_load_phdr(struct elf_context *ctx, struct elf_generic_phdr *phdr,
		  unsigned int index)
{
	BUG_ON(ctx == NULL || phdr == NULL);

	return elf_imp(ctx, elf_phdr, phdr, index);
}

int elf_load_shdr(struct elf_context *ctx, struct elf_generic_shdr *shdr,
		  unsigned int index)
{
	BUG_ON(ctx == NULL || shdr == NULL);

	return elf_imp(ctx, elf_shdr, shdr, index);
}

int elf_load_seg(struct elf_context *ctx, const struct elf_generic_phdr *phdr,
		 void *data, off_t offset, size_t length)
{
	int err = E_HM_OK;
	size_t diff, size;

	BUG_ON(ctx == NULL || phdr == NULL);
	if (data == NULL || offset < 0 || length == 0U) {
		err = E_HM_INVAL;
	}
	/* check data overflow */
	if (err == E_HM_OK && (uint64_t)offset >= phdr->filesz) {
		err = E_HM_OVERFLOW;
	}
	if (err == E_HM_OK) {
		diff = (size_t)phdr->filesz - (size_t)offset;
		size = length > diff ? diff : length;
		err = elf_file_read(ctx, data, offset + (off_t)phdr->offset, size);
	}

	return err;
}

int elf_load_sec(struct elf_context *ctx, const struct elf_generic_shdr *shdr,
		 void *data, off_t offset, size_t length)
{
	int err = E_HM_OK;
	size_t diff, size;

	BUG_ON(ctx == NULL || shdr == NULL);
	if (data == NULL || offset < 0 || length == 0U) {
		err = E_HM_INVAL;
	}

	/* check data overflow */
	if (err == E_HM_OK && (uint64_t)offset >= shdr->size) {
		err = E_HM_OVERFLOW;
	}
	if (err == E_HM_OK) {
		diff = (size_t)shdr->size - (size_t)offset;
		size = length > diff ? diff : length;
		err = elf_file_read(ctx, data, offset + (off_t)shdr->offset, size);
	}

	return err;
}

int elf_fill_ehdr(struct elf_context *ctx,
		  const struct elf_generic_header *ehdr)
{
	BUG_ON(ctx == NULL || ehdr == NULL);

	return elf_imp(ctx, elf_set_ehdr, ehdr);
}

int elf_fill_phdr(struct elf_context *ctx, const struct elf_generic_phdr *phdr)
{
	BUG_ON(ctx == NULL || phdr == NULL);

	return elf_imp(ctx, elf_set_phdr, phdr);
}

int elf_fill_nhdr(struct elf_context *ctx, const struct elf_generic_nhdr *nhdr)
{
	BUG_ON(ctx == NULL || nhdr == NULL);

	return elf_imp(ctx, elf_set_nhdr, nhdr);
}

int elf_file_lseek(struct elf_context *ctx, off_t offset, int whence,
		   off_t *pos)
{
	BUG_ON(ctx == NULL);

	return (offset < 0 || pos == NULL) ? E_HM_INVAL :
	       elf_fop(ctx, lseek, offset, whence, pos);
}

int elf_file_read(struct elf_context *ctx, void *data,
		  off_t offset, size_t length)
{
	ssize_t len;
	int err = E_HM_OK;

	BUG_ON(ctx == NULL);
	if (data == NULL || offset < 0 || length == 0U) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		len = elf_fop(ctx, read, data, offset, length);
		if (len < 0) {
			err = (int)len;
			hm_warn("read elf file failed: %s\n", hmstrerror(err));
		} else if ((size_t)len != length) {
			err = E_HM_INVAL;
		}
	}

	return err;
}

int elf_file_write(struct elf_context *ctx, const void *data, size_t length)
{
	int err = E_HM_OK;
	ssize_t len;

	BUG_ON(ctx == NULL);
	if (data == NULL || length == 0U) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		len = elf_fop(ctx, write, data, length);
		if (len < 0) {
			err = (int)len;
			hm_warn("write elf file failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}

int elf_file_size(struct elf_context *ctx, size_t *size)
{
	BUG_ON(ctx == NULL);
	return (size == NULL) ? E_HM_INVAL : elf_fop(ctx, filesz, size);
}

size_t elf_header_size(const struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);

	return elf_imp(ctx, elf_ehdr_size);
}

size_t elf_pgr_header_size(const struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);

	return elf_imp(ctx, elf_phdr_size);
}

size_t elf_sec_header_size(const struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);

	return elf_imp(ctx, elf_shdr_size);
}

size_t elf_note_header_size(const struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);

	return elf_imp(ctx, elf_nhdr_size);
}

int elf_check_magic(const void *ehdr, size_t len)
{
	return (ehdr == NULL) ? E_HM_INVAL : check_elf_magic(ehdr, len);
}

int64_t elf_dyn_tag(const struct elf_context *ctx, void *dyn_ent)
{
	BUG_ON(ctx == NULL || dyn_ent == NULL);
	return elf_imp(ctx, elf_dyn_tag, dyn_ent);
}

uint64_t elf_dyn_ptr(const struct elf_context *ctx, void *dyn_ent)
{
	BUG_ON(ctx == NULL || dyn_ent == NULL);
	return elf_imp(ctx, elf_dyn_ptr, dyn_ent);
}

size_t elf_dyn_ent_size(const struct elf_context *ctx)
{
	BUG_ON(ctx == NULL);
	return elf_imp(ctx, elf_dyn_ent_size);
}

int elf_set_class(struct elf_context *ctx, unsigned int elf_class)
{
	BUG_ON(ctx == NULL);

	return set_elf_impl(ctx, elf_class);
}

static const char *elf_type_names[ET_NUM] = {
	[ET_NONE]	= "ET_NONE",
	[ET_REL]	= "ET_REL",
	[ET_EXEC]	= "ET_EXEC",
	[ET_DYN]	= "ET_DYN",
	[ET_CORE]	= "ET_CORE",
};

const char *elf_type_name(unsigned int type)
{
	return (type >= __ARRAY_SIZE(elf_type_names)) ? "UNKNOWN" : elf_type_names[type];
}

int elf_get_start(struct elf_context *ctx, uintptr_t *start)
{
	BUG_ON(ctx == NULL);

	return elf_fop(ctx, get_load_start, start);
}
