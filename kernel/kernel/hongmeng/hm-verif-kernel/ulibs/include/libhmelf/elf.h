/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Definition for ELF format operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 7 19:28:21 2018
 */
#ifndef ULIBS_LIBHMELF_ELF_H
#define ULIBS_LIBHMELF_ELF_H

#include <stdint.h>
#include <unistd.h>

#include <hongmeng/errno.h>

/* randomdata segment, should fill with real random data on load */
#define PT_RANDOMDATA  0x6788fc60
#define PT_RANDOMDATA_SIZE_LIMIT (1024 * 128)
#define PT_GNU_RELRO 0x6474e552
#define PT_HKIP  0x60000008

#define BINFMT_HEAD_BUF_SIZE 256

struct elf_context;
struct elf_impl;

struct elf_generic_header {
	unsigned int elf_data;
	unsigned int elf_class;
	unsigned int elf_type;
	unsigned int elf_machine;
	unsigned int elf_version;
	unsigned int elf_flags;
	uint64_t elf_entry_point;

	/* for internal use */
	unsigned long phdr_offset;
	unsigned long shdr_offset;
	unsigned int ehdr_size;
	unsigned int phdr_nr;
	unsigned int shdr_nr;
	unsigned int phdr_size;
	unsigned int shdr_size;
	unsigned int shstr_index;
};

struct elf_generic_phdr {
	unsigned int type;
	unsigned int flags;

	uint64_t offset;
	uint64_t vaddr;
	uint64_t paddr;
	uint64_t memsz;
	uint64_t filesz;
	uint64_t align;
};

struct elf_generic_shdr {
	unsigned int name;
	unsigned int type;
	unsigned int link;

	uint64_t addr;
	uint64_t offset;
	uint64_t size;
};

struct elf_generic_nhdr {
	unsigned int namesz;
	unsigned int descsz;
	unsigned int type;
};

struct elf_fops {
	int (*open)(struct elf_context *ctx, const char *elf_file);
	ssize_t (*read)(struct elf_context *ctx, void *data,
			off_t offset, size_t length);
	ssize_t (*write)(struct elf_context *ctx, const void *data, size_t length);
	int (*close)(struct elf_context *ctx);
	int (*lseek)(struct elf_context *ctx, off_t offset,
		     int whence, off_t *pos);
	int (*filesz)(struct elf_context *ctx, size_t *size);
	int (*get_load_start)(struct elf_context *ctx, uintptr_t *start);
};

struct elf_context {
	struct elf_fops *fops;
	struct elf_impl *impl;
	struct elf_generic_header elf_header;
	uint32_t elf_ext_prot;
	void *private_data;
	/*
	 * XXX: head_buf and elf_header are redundant, elf_header will remove
	 * later. This is an intermediate state.
	 */
	char head_buf[BINFMT_HEAD_BUF_SIZE];
};

struct gnu_property {
	uint32_t pr_type;
	uint32_t pr_datasz;
};
#define GNU_PROPERTY_TYPE_0_NAME "GNU"
/* .note.gnu.property types for EM_AARCH64: */
#define GNU_PROPERTY_AARCH64_FEATURE_1_AND   0xc0000000
/* Bits for GNU_PROPERTY_AARCH64_FEATURE_1_BTI */
#define GNU_PROPERTY_AARCH64_FEATURE_1_BTI  0x1
#define NOTE_NAME_SZ (sizeof(GNU_PROPERTY_TYPE_0_NAME))
/* Note types with note name GNU */
#define NT_GNU_PROPERTY_TYPE_0 5
#define ELF_GNU_PROPERTY_ALIGN 8

int elf_open_ctx(struct elf_context *ctx, struct elf_fops *fops,
		 const char *elf_file, void *private_data);
int elf_init_ctx(struct elf_context *ctx, struct elf_fops *fops,
		 const char *elf_file, void *private_data);
void elf_destroy_ctx(struct elf_context *ctx);

int elf_load_ehdr(struct elf_context *ctx);
int elf_load_phdr(struct elf_context *ctx, struct elf_generic_phdr *phdr,
		  unsigned int index);
int elf_load_shdr(struct elf_context *ctx, struct elf_generic_shdr *shdr,
		  unsigned int index);

int elf_load_seg(struct elf_context *ctx, const struct elf_generic_phdr *phdr,
		 void *data, off_t offset, size_t length);
int elf_load_sec(struct elf_context *ctx, const struct elf_generic_shdr *shdr,
		 void *data, off_t offset, size_t length);

int elf_fill_ehdr(struct elf_context *ctx,
		  const struct elf_generic_header *ehdr);
int elf_fill_phdr(struct elf_context *ctx, const struct elf_generic_phdr *phdr);
int elf_fill_nhdr(struct elf_context *ctx, const struct elf_generic_nhdr *nhdr);
int elf_file_lseek(struct elf_context *ctx, off_t offset,
		   int whence, off_t *pos);
int elf_file_read(struct elf_context *ctx, void *data,
		  off_t offset, size_t length);
int elf_file_write(struct elf_context *ctx, const void *data, size_t length);
int elf_file_size(struct elf_context *ctx, size_t *size);

size_t elf_header_size(const struct elf_context *ctx);
size_t elf_pgr_header_size(const struct elf_context *ctx);
size_t elf_sec_header_size(const struct elf_context *ctx);
size_t elf_note_header_size(const struct elf_context *ctx);

int elf_check_magic(const void *ehdr, size_t len);
int elf_set_class(struct elf_context *ctx, unsigned int elf_class);

int64_t elf_dyn_tag(const struct elf_context *ctx, void *dyn_ent);
uint64_t elf_dyn_ptr(const struct elf_context *ctx, void *dyn_ent);
size_t elf_dyn_ent_size(const struct elf_context *ctx);

int elf_phdr_info(uintptr_t load_start, unsigned int type, uintptr_t *addr,
		  size_t *memsz, size_t *align, size_t *filesz);

const char *elf_type_name(unsigned int type);

int elf_get_start(struct elf_context *ctx, uintptr_t *start);
#endif
