/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide function of load/unload sections in elf file
 * Author: Huawei OS Kernel Lab
 * Create: Tue 19 Nov 2019 09:41:49 PM CST
 */
#include <libhmelf/elf.h>
#include <libstrict/overflow.h>
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <libhmelf/symbol.h>
#include <libhmelf/elf_load.h>
#include <hmkernel/const.h>

#include "elf_ex.h"

int load_section(struct elf_context *ctx, const struct elf_generic_shdr *shdr,
		 struct section_mem *mem)
{
	int rc = E_HM_OK;
	size_t size = (size_t)PAGE_ALIGN_UP(shdr->size);
	if (size == 0U) {
		hm_warn("invalid section header size\n");
		rc = E_HM_INVAL;
	} else {
		void *mapped_addr = malloc(size);

		if (mapped_addr == NULL) {
			hm_warn("alloc memory for mapped_addr failed\n");
			rc = E_HM_NOMEM;
		} else {
			rc = elf_load_sec(ctx, shdr, mapped_addr, 0, size);
			if (rc != 0) {
				hm_warn("load section failed\n");
				free(mapped_addr);
			} else {
				mem->addr = (uintptr_t)mapped_addr;
				mem->size = size;
			}
		}
	}
	return rc;
}

void unload_section(struct section_mem *mem)
{
	BUG_ON(mem == NULL);
	if ((mem->addr != 0U) && (mem->size != 0U)) {
		free((void *)mem->addr);
		mem->addr = 0;
		mem->size = 0;
	}
}

int load_shstr_section(struct elf_context *ctx, struct section_mem *mem)
{
	int rc = E_HM_OK;
	struct elf_generic_shdr shdr;

	mem_zero_s(shdr);
	rc = elf_load_shdr(ctx, &shdr, ctx->elf_header.shstr_index);
	if (rc != 0) {
		hm_warn("load shstrtab section header failed: %s\n", hmstrerror(rc));
	} else {
		rc = load_section(ctx, &shdr, mem);
		if (rc == 0) {
			*((char *)mem->addr + mem->size - 1) = '\0';
		}
	}
	return rc;
}

int load_section_by_name(const struct section_mem *shstr_mem, const char *name,
			 struct elf_context *ctx, struct section_mem *mem)
{
	int rc = E_HM_OK;
	unsigned int i = 0;
	struct elf_generic_shdr shdr;
	char *shstr = NULL;

	if (shstr_mem == NULL || name == NULL || ctx == NULL || mem == NULL) {
		rc = E_HM_INVAL;
	} else {
		shstr = (char *)shstr_mem->addr;
		for (i = 0; i < ctx->elf_header.shdr_nr; i++) {
			mem_zero_s(shdr);
			rc = elf_load_shdr(ctx, &shdr, i);
			if (rc != 0) {
				break;
			}
			if (shdr.name >= shstr_mem->size) {
				rc = E_HM_INVAL;
				break;
			}
			if ((strlen(name) + 1) > (shstr_mem->size - shdr.name)) {
				continue;
			}
			if (strcmp(name, shstr + shdr.name) != 0) {
				continue;
			}
			rc = load_section(ctx, &shdr, mem);
			if (rc != E_HM_OK) {
				hm_warn("load symtab section failed: %s\n",
					hmstrerror(rc));
				break;
			}
			rc = E_HM_OK;
			break;
		}
	}

	if (rc == E_HM_OK && i == ctx->elf_header.shdr_nr) {
		rc = E_HM_SRCH;
	}

	return rc;
}

int process_load_symbol(struct elf_context *ctx, struct symbol_desc *sym_desc)
{
	int rc = E_HM_OK;
	struct section_mem shstr_mem;
	struct section_mem *symtab_mem = NULL;
	struct section_mem *strtab_mem = NULL;

	mem_zero_s(shstr_mem);
	if (ctx == NULL || sym_desc == NULL) {
		hm_warn("invalid arguments\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = load_shstr_section(ctx, &shstr_mem);
		if (rc != E_HM_OK) {
			hm_warn("load shstrtab section failed: %s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		symtab_mem = &sym_desc->secmem[SECTION_SYMTAB];
		symtab_mem->addr = 0;
		rc = load_section_by_name(&shstr_mem, ".symtab", ctx, symtab_mem);
		if (rc != E_HM_OK) {
			hm_debug("load symtab section failed: %s\n", hmstrerror(rc));
			unload_section(&shstr_mem);
		}
	}
	if (rc == E_HM_OK) {
		strtab_mem = &sym_desc->secmem[SECTION_STRTAB];
		strtab_mem->addr = 0;
		rc = load_section_by_name(&shstr_mem, ".strtab", ctx, strtab_mem);
		if (rc != E_HM_OK) {
			hm_warn("load strtab section failed: %s\n", hmstrerror(rc));
			unload_section(&sym_desc->secmem[SECTION_SYMTAB]);
			unload_section(&shstr_mem);
		}
	}
	if (rc == E_HM_OK) {
		unload_section(&shstr_mem);
	}
	return rc;
}

void process_unload_symbol(struct symbol_desc *sym_desc)
{
	int i;

	if (sym_desc != NULL) {
		for (i = 0; i < (int)(SECTION_MAX); i++) {
			unload_section(&sym_desc->secmem[i]);
		}
	}
}

static int check_ehdr_validity(struct elf_context *elf_ctx)
{
	int rc = E_HM_OK;
	size_t ehdr_size;
	unsigned int type;
	struct elf_generic_header *elf_header = &elf_ctx->elf_header;

	type = elf_header->elf_type;
	if (type != __U(ET_EXEC) && type != __U(ET_DYN)) {
		hm_warn("invalid elf type: %u\n", type);
		rc = E_HM_NOEXEC;
	} else {
		/* check elf header size */
		ehdr_size = elf_header->ehdr_size;
		if (ehdr_size != elf_header_size(elf_ctx)) {
			hm_warn("invalid elf header size: %zu\n", ehdr_size);
			rc = E_HM_NOEXEC;
		}
	}

	return rc;
}

static int check_shdr_validity(struct elf_context *elf_ctx)
{
	int rc = E_HM_OK;
	unsigned int shdr_offset;
	unsigned int shdr_size;
	unsigned int shdr_nr = 0;
	size_t shdr_len = 0;
	size_t elf_size = 0;
	struct elf_generic_header *elf_header = &elf_ctx->elf_header;

	/* check section header size */
	shdr_size = elf_header->shdr_size;
	if ((size_t)shdr_size != elf_sec_header_size(elf_ctx)) {
		hm_warn("invalid section header size: %u\n", shdr_size);
		rc = E_HM_NOEXEC;
	}
	if (rc == E_HM_OK) {
		/* shdr offset must be greater than elf header size */
		shdr_offset = (unsigned int)(elf_header->shdr_offset);
		if (shdr_offset < elf_header->ehdr_size) {
			hm_warn("invalid section header offset: %u\n", shdr_offset);
			rc = E_HM_NOEXEC;
		}
	}
	if (rc == E_HM_OK) {
		/* shdr number must be greater than 0 */
		shdr_nr = elf_header->shdr_nr;
		if (shdr_nr == 0U) {
			hm_warn("section header number is zero\n");
			rc = E_HM_NOEXEC;
		}
	}
	if (rc == E_HM_OK) {
		/* shdr length must be less than elf file size */
		noovfl_umul(shdr_len, shdr_size, shdr_nr,
			    "shdr length is out of limit of long long\n");
		rc = elf_file_size(elf_ctx, &elf_size);
		if (rc != E_HM_OK) {
			hm_warn("get elf file size failed: %s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK && shdr_len >= elf_size) {
		hm_warn("too many section headers: %u\n", shdr_nr);
		rc = E_HM_NOEXEC;
	}

	return rc;
}

static int init_elf_context(const char *name, struct elf_context *elf_ctx)
{
	int rc;
#ifdef CONFIG_ARM64_BTI_USER
	uint32_t ext_prot;
#endif

	mem_zero_s(*elf_ctx);
	rc = elf_init_ctx(elf_ctx, &elf_ex_fops, name, NULL);
	if (rc != E_HM_OK) {
		hm_warn("Failed to open elf %s\n", name);
	} else {
		rc = check_ehdr_validity(elf_ctx);
		if (rc != E_HM_OK) {
			hm_warn("check elf header failed\n");
			elf_destroy_ctx(elf_ctx);
			rc = E_HM_NOEXEC;
		}
	}
#ifdef CONFIG_ARM64_BTI_USER
	/* parse ext prot BTI while init elf_context */
	if (rc == E_HM_OK) {
		rc = elf_parse_ext_prot(elf_ctx, &ext_prot);
		if (rc == E_HM_OK) {
			elf_ctx->elf_ext_prot = ext_prot;
		} else {
			elf_destroy_ctx(elf_ctx);
			rc = E_HM_NOEXEC;
		}
	}
#endif
	return rc;
}

int process_parse_elf(const char *name, struct elf_context *ctx)
{
	int rc = E_HM_OK;

	if (name == NULL || ctx == NULL) {
		hm_warn("invalid arguments\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = init_elf_context(name, ctx);
		if (rc != E_HM_OK) {
			hm_warn("init elf context failed: %s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		rc = check_shdr_validity(ctx);
		if (rc != E_HM_OK) {
			hm_warn("check elf shdr failed: %s\n", hmstrerror(rc));
			elf_destroy_ctx(ctx);
		}
	}

	return rc;
}

void process_unload_ftrace_stub_section(struct ftrace_stub_section *ftrace_stub)
{
	if (ftrace_stub != NULL) {
		free(ftrace_stub->stub_site_insn);
		free(ftrace_stub->stub_site_vaddr);
		ftrace_stub->stub_site_insn = NULL;
		ftrace_stub->stub_site_vaddr = NULL;
		ftrace_stub->stub_site_num = 0;
	}
}

static int load_ftrace_stub_vaddr(const struct section_mem *mem,
				  struct ftrace_stub_section *ftrace_stub)
{
	int rc = E_HM_OK;
	uint64_t stub_site_num;
	uint64_t stub_site_vaddr_size;
	uintptr_t *stub_site_vaddr = NULL;
	uintptr_t *stub_site_memcpy_addr = NULL;

	ftrace_stub->stub_site_vaddr = NULL;
	/* stub_site_num put at first of .ftrace_stub_site section */
	stub_site_num = *(uint64_t *)mem->addr;
	if (stub_site_num == 0 ||
	    stub_site_num > (UINT64_MAX / sizeof(uintptr_t))) {
		hm_warn("invalid ftrace stub site num\n");
		rc = E_HM_POSIX_FAULT;
	}
	if (rc == E_HM_OK) {
		/* read stub site vaddr list from .ftrace_stub_site section */
		stub_site_vaddr_size = stub_site_num * sizeof(uintptr_t);
		stub_site_vaddr = (uintptr_t *)malloc(stub_site_vaddr_size);
		if (stub_site_vaddr == NULL) {
			rc = E_HM_NOMEM;
			hm_warn("malloc ftrace stub site vaddr failed\n");
		}
	}
	if (rc == E_HM_OK) {
		/* calculate the start address of memory copy */
		stub_site_memcpy_addr = (uintptr_t *)ptr_add(mem->addr,
							     sizeof(stub_site_num));
		rc = memcpy_s((void *)stub_site_vaddr, stub_site_vaddr_size,
			      (void *)stub_site_memcpy_addr, stub_site_vaddr_size);
		if (rc != E_HM_OK) {
			hm_warn("memcpy ftrace stub site vaddr failed\n");
			rc = E_HM_POSIX_FAULT;
			free(stub_site_vaddr);
		}
	}
	if (rc == E_HM_OK) {
		/* save stub site list info in ftrace_stub_section */
		ftrace_stub->stub_site_num = stub_site_num;
		ftrace_stub->stub_site_vaddr = stub_site_vaddr;
	}
	return rc;
}

static int load_ftrace_stub_insn(const struct section_mem *mem,
				 struct ftrace_stub_section *ftrace_stub)
{
	int rc = E_HM_OK;
	uint64_t stub_site_num;
	uint64_t stub_site_insn_size;
	uint32_t *stub_site_insn = NULL;
	uintptr_t *stub_site_memcpy_addr = NULL;

	ftrace_stub->stub_site_insn = NULL;
	/* stub_site_num put at first of .ftrace_stub_site section */
	stub_site_num = *(uint64_t *)mem->addr;
	if (stub_site_num == 0 ||
	    stub_site_num > (UINT64_MAX / sizeof(uintptr_t))) {
		hm_warn("invalid ftrace stub site num\n");
		rc = E_HM_POSIX_FAULT;
	}
	if (rc == E_HM_OK) {
		/* read stub site insn list from .ftrace_stub_site section */
		stub_site_insn_size = stub_site_num * sizeof(uint32_t);
		stub_site_insn = (uint32_t *)malloc(stub_site_insn_size);
		if (stub_site_insn == NULL) {
			hm_warn("malloc ftrace stub site insn failed\n");
			rc = E_HM_NOMEM;
		}
	}
	if (rc == E_HM_OK) {
		/* calculate the start address of memory copy */
		stub_site_memcpy_addr = (uintptr_t *)ptr_add(mem->addr,
							     sizeof(stub_site_num));
		stub_site_memcpy_addr = (uintptr_t *)ptr_add(stub_site_memcpy_addr,
							     stub_site_num * sizeof(uintptr_t));
		rc = memcpy_s((void *)stub_site_insn, stub_site_insn_size,
			      (void *)stub_site_memcpy_addr, stub_site_insn_size);
		if (rc != E_HM_OK) {
			hm_warn("memcpy ftrace stub site insn failed\n");
			free(stub_site_insn);
			rc = E_HM_POSIX_FAULT;
		}
	}
	if (rc == E_HM_OK) {
		/* save stub site list info in ftrace_stub_section */
		ftrace_stub->stub_site_num = stub_site_num;
		ftrace_stub->stub_site_insn = stub_site_insn;
	}
	return rc;
}

static int check_ftrace_stub_site_section(const char *shstr, struct elf_context *ctx,
					  struct section_mem *ftrace_sec_mem)
{
	unsigned int i;
	int rc = E_HM_OK;
	uint64_t obj_section_size;
	uint64_t stub_site_num;
	uint64_t stub_site_vaddr_size;
	uint64_t stub_site_insn_size;
	uintptr_t *stub_site_section_addr = NULL;
	struct elf_generic_shdr shdr;

	for (i = 0; i < ctx->elf_header.shdr_nr; i++) {
		mem_zero_s(shdr);
		rc = elf_load_shdr(ctx, &shdr, i);
		if (rc != E_HM_OK) {
			break;
		}
		if (strcmp(FTRACE_STUB_SITE_SECTION, shstr + shdr.name) == 0) {
			/* stub_site_num put at first of .ftrace_stub_site section */
			stub_site_section_addr = (uintptr_t *)(ftrace_sec_mem->addr);
			stub_site_num = *stub_site_section_addr;

			stub_site_vaddr_size = stub_site_num * sizeof(uintptr_t);
			stub_site_insn_size = stub_site_num * sizeof(uint32_t);
			obj_section_size = sizeof(stub_site_num) +
					   stub_site_vaddr_size +
					   stub_site_insn_size;

			if (obj_section_size == shdr.size) {
				break;
			}
		}
	}
	if (rc == E_HM_OK && i == ctx->elf_header.shdr_nr) {
		rc = E_HM_POSIX_FAULT;
	}

	return rc;
}

int load_ftrace_stub_sec_mem(struct elf_context *ctx,
			     struct section_mem *ftrace_sec_mem)
{
	int rc;
	struct section_mem shstr_mem;

	mem_zero_s(shstr_mem);
	rc = load_shstr_section(ctx, &shstr_mem);
	if (rc != E_HM_OK) {
		hm_warn("load shstrtab section for ftrace failed: %s\n",
			hmstrerror(rc));
	}
	if (rc == E_HM_OK) {
		rc = load_section_by_name(&shstr_mem, FTRACE_STUB_SITE_SECTION, ctx, ftrace_sec_mem);
		if (rc != E_HM_OK) {
			hm_debug("load FTRACE_STUB_SITE_SECTION from elf_context failed: %s\n",
				 hmstrerror(rc));
			unload_section(&shstr_mem);
		}
	}
	if (rc == E_HM_OK) {
		rc = check_ftrace_stub_site_section((char *)shstr_mem.addr, ctx, ftrace_sec_mem);
		if (rc != E_HM_OK) {
			hm_warn("invalid ftrace_stub_site_section\n");
			unload_section(&shstr_mem);
			unload_section(ftrace_sec_mem);
		}
	}
	if (rc == E_HM_OK) {
		unload_section(&shstr_mem);
	}
	return rc;
}

int process_load_ftrace_stub_section(struct elf_context *ctx,
				     struct ftrace_stub_section *ftrace_stub)
{
	int rc = E_HM_OK;
	struct section_mem ftrace_sec_mem;

	mem_zero_s(ftrace_sec_mem);
	if (ctx == NULL || ftrace_stub == NULL) {
		hm_warn("invalid arguments\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = load_ftrace_stub_sec_mem(ctx, &ftrace_sec_mem);
		if (rc != E_HM_OK) {
			hm_debug("load_ftrace_stub_sec_mem failed\n");
		}
	}
	if (rc == E_HM_OK) {
		ftrace_stub->stub_site_num = 0;
		rc = load_ftrace_stub_vaddr(&ftrace_sec_mem, ftrace_stub);
		if (rc != E_HM_OK) {
			hm_warn("load ftrace stub vaddr failed: %s\n", hmstrerror(rc));
			unload_section(&ftrace_sec_mem);
		}
	}
	if (rc == E_HM_OK) {
		rc = load_ftrace_stub_insn(&ftrace_sec_mem, ftrace_stub);
		if (rc != E_HM_OK) {
			hm_warn("load ftrace stub insn failed: %s\n", hmstrerror(rc));
			unload_section(&ftrace_sec_mem);
			free(ftrace_stub->stub_site_vaddr);
			ftrace_stub->stub_site_vaddr = NULL;
			ftrace_stub->stub_site_num = 0;
		}
	}
	if (rc == E_HM_OK) {
		unload_section(&ftrace_sec_mem);
	}
	return rc;
}

static uint32_t parse_elf_property(uint32_t type, const char* data, size_t datasz)
{
	uint32_t prot = 0;
	if (type == GNU_PROPERTY_AARCH64_FEATURE_1_AND) {
		const uint32_t *p = (const void*)data;
		if (datasz != sizeof(*p)) {
			return prot;
		}
		if((*p & GNU_PROPERTY_AARCH64_FEATURE_1_BTI) != 0) {
			prot |= MPROT_BTI;
		}
	}
	return prot;
}
static uint32_t parse_prot(const char *data, size_t off, size_t datasz)
{
	size_t local_offset;
	size_t gnu_datasz;
	const struct gnu_property *pr;

	local_offset = off;
	gnu_datasz = datasz - off;

	if(gnu_datasz < sizeof(*pr))
		return 0;

	/*
	 * | struct gnu_property | ---- |
	 * | pr_type | pr_datasz | data |
	 * |--------datasz -------------|
	 */

	pr = (const struct gnu_property *)(data + local_offset);
	local_offset += sizeof(*pr);

	if (pr->pr_datasz > (gnu_datasz - sizeof(*pr)))
		return 0;

	return parse_elf_property(pr->pr_type, data + local_offset, pr->pr_datasz);
}

/* parse BTI info, default return value is 0 */
int elf_parse_ext_prot(struct elf_context *ctx, uint32_t *ext_prot)
{
	if (ctx == NULL || ext_prot == NULL) {
		hm_warn("invaild params\n");
		return E_HM_INVAL;
	}

	union {
		struct elf_generic_nhdr note;
		char data[0x400];
	} gnu_data;

	*ext_prot = 0;
	unsigned int nph;
	nph = ctx->elf_header.phdr_nr;
	for (unsigned int i = 0; i < nph; i++) {
		struct elf_generic_phdr phdr;
		int err;
		mem_zero_s(phdr);
		err = elf_load_phdr(ctx, &phdr, i);
		if (err != E_HM_OK) {
			return err;
		}
		if (phdr.type != PT_NOTE && phdr.type != PT_GNU_PROPERTY) {
			continue;
		}
		if (phdr.filesz > sizeof(gnu_data.data)) {
			continue;
		}
		ssize_t len = ctx->fops->read(ctx, gnu_data.data, phdr.offset, phdr.filesz);
		if (len != (ssize_t)phdr.filesz) {
			mem_zero_s(gnu_data);
			continue;
		}
		/* maybe more than one NOTE section */
		if (gnu_data.note.type != NT_GNU_PROPERTY_TYPE_0 || gnu_data.note.namesz != NOTE_NAME_SZ) {
			mem_zero_s(gnu_data);
			continue;
		}
		size_t off = ALIGN_UP(sizeof(gnu_data.note) + NOTE_NAME_SZ, ELF_GNU_PROPERTY_ALIGN);
		size_t datasz = off + gnu_data.note.descsz;
		*ext_prot = parse_prot((char *)gnu_data.data, off, datasz);
		return E_HM_OK;
	}
	return E_HM_OK;
}
