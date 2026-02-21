/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Elf
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 14 04:12:13 2018
 */
#ifndef KLIBS_ELF_H
#define KLIBS_ELF_H

#include <hmasm/types.h>
#include <hmkernel/types.h>

/* ELF field index in e_indent */
#define EI_MAG0		0	/* magic number 0 */
#define ELFMAG0		0x7FU

#define EI_MAG1		1	/* magic number 1 */
#define ELFMAG1		'E'

#define EI_MAG2		2	/* magic number 2 */
#define ELFMAG2		'L'

#define EI_MAG3		3	/* magic number 3 */
#define ELFMAG3		'F'

#define ELFMAG		"\x7f""ELF"
#define SELFMAG		4UL

#define EI_CLASS	4	/* elf class, 32 or 64 bit */
#define ELFCLASS32	1U
#define ELFCLASS64	2U

#define EI_DATA		5	/* elf endianess */

#define EI_VERSION	6	/* must be EV_CURRENT */

#define EI_OSABI	7	/* target operating system ABI */

#define EI_ABIVERSION	8	/* the ABI version */

#define EI_PAD		9	/* padding bytes */
#define EI_NIDENT	16

#define ELFDATANONE	0
#define ELFDATA2LSB	1
#define ELFDATA2MSB	2

/* ELF type */
#define ET_NONE		0
#define ET_REL		1
#define ET_EXEC		2
#define ET_DYN		3
#define ET_CORE		4
#define ET_NUM		5	/* number of defined types */
#define ET_LOPROC	0xff00
#define ET_HIPROC	0xffff

/* ELF Machine types: (EM_*) */
#define EM_NONE		0
#define EM_ARM		40
#define EM_AARCH64	183

/* ELF file format version numbers */
#define	EV_NONE		0	/* invalid elf version */
#define	EV_CURRENT	1	/* original and current version of elf */
#define EV_NUM		2	/* number of defiend version number */

/* PHDR type */
#define PT_LOAD		1U
#define PT_DYNAMIC	2U
#define PT_NOTE     4U
#define PT_PHDR		6U	/* segment containing PHDR itself */
/* randomdata segment, should fill with real random data on load */
#define PT_RANDOMDATA  0x6788fc60
#define PT_RANDOMDATA_SIZE_LIMIT (1024 * 128)
#define PT_GNU_PROPERTY  0x6474e553

/* dynamic entry type */
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_REL		17
#define DT_RELSZ	18
#define DT_RELACOUNT	0x6ffffff9
#define DT_RELCOUNT	0x6ffffffa

/* AArch64 relocs */
#define R_AARCH64_NONE		0U
#define R_AARCH64_RELATIVE	1027U

/* AArch64 relocate type, used by livepatch */
#define R_AARCH64_ABS64			257 /* Direct 64 bit. */
#define R_AARCH64_ADR_PREL_PG_HI21	275 /* Page-rel. ADRP imm. from 32:12.  */
#define R_AARCH64_ADD_ABS_LO12_NC	277 /* Dir. ADD imm. from bits 11:0.  */
#define R_AARCH64_LDST8_ABS_LO12_NC	278 /* Likewise for LD/ST; no check. */
#define R_AARCH64_JUMP26		282 /* PC-rel. B imm. from bits 27:2.  */
#define R_AARCH64_CALL26		283 /* Likewise for CALL.  */
#define R_AARCH64_ADR_GOT_PAGE		311 /* P-page-rel. GOT off. ADRP 32:12.  */
#define R_AARCH64_LD64_GOT_LO12_NC	312 /* Dir. GOT off. LD/ST imm. 11:3.  */
#define R_AARCH64_LDST16_ABS_LO12_NC	284 /* Likewise for bits 11:1.  */
#define R_AARCH64_LDST32_ABS_LO12_NC	285 /* Likewise for bits 11:2.  */
#define R_AARCH64_LDST64_ABS_LO12_NC	286 /* Likewise for bits 11:3.  */

/* ARM relocs */
#define R_ARM_NONE	0U
#define R_ARM_RELATIVE	23U

#define PF_R		0x4U
#define PF_W		0x2U
#define PF_X		0x1U

#define SHT_SYMTAB	  2	/* Symbol table */
#define SHT_STRTAB	  3	/* String table */
#define SHT_RELA	  4	/* Relocation entries with addends */
#define SHT_NOBITS	  8	/* Program space with no data (bss) */
#define SHT_REL		  9	/* Relocation entries, no addends */

#define SHF_WRITE	(1 << 0)	/* Writable */
#define SHF_ALLOC	(1 << 1)	/* Occupies memory during execution */
#define SHF_EXECINSTR	(1 << 2)	/* Executable */

#define SHN_UNDEF	0	/* Undefined section */
#define SHN_ABS		0xfff1	/* Associated symbol is absolute */
#define SHN_COMMON	0xfff2	/* Associated symbol is common */

#define ELF64_R_SYM(i)			((i) >> 32)
#define ELF64_R_TYPE(i)			((i) & 0xffffffff)

/* elf64 */
typedef u64	__Elf64_Addr;	/* Program address */
typedef u8	__Elf64_Byte;	/* Unsigned tiny integer */
typedef u16	__Elf64_Half;	/* Unsigned medium integer */
typedef u64	__Elf64_Off;	/* File offset */
typedef u16	__Elf64_Section;	/* Section index */
typedef s32	__Elf64_Sword;	/* Signed integer */
typedef u32	__Elf64_Word;	/* Unsigned integer */
typedef u64	__Elf64_Lword;	/* Unsigned long integer */
typedef u64	__Elf64_Xword;	/* Unsigned long integer */
typedef s64	__Elf64_Sxword;	/* Signed long integer */

/* 64 bit EHDR */
struct elf64_hdr {
	unsigned char	e_ident[EI_NIDENT];	/* ELF identification */
	__Elf64_Half	e_type;			/* Object file type (ET_*) */
	__Elf64_Half	e_machine;		/* Machine type (EM_*) */
	__Elf64_Word	e_version;		/* File format version (EV_*) */
	__Elf64_Addr	e_entry;		/* Start address */
	__Elf64_Off 	e_phoff;		/* File offset to the PHDR table */
	__Elf64_Off 	e_shoff;		/* File offset to the SHDR table */
	__Elf64_Word	e_flags;		/* Flags (EF_*) */
	__Elf64_Half	e_ehsize;		/* Elf header size in bytes */
	__Elf64_Half	e_phentsize;		/* PHDR table entry size in bytes */
	__Elf64_Half	e_phnum;		/* Number of PHDR entries */
	__Elf64_Half	e_shentsize;		/* SHDR table entry size in bytes */
	__Elf64_Half	e_shnum;		/* Number of SHDR entries */
	__Elf64_Half	e_shstrndx;		/* Index of section name string table */
};

/* 64 bit PHDR entry */
struct elf64_phdr {
	__Elf64_Word	p_type;		/* Type of segment */
	__Elf64_Word	p_flags;	/* Segment flags */
	__Elf64_Off	p_offset;	/* File offset to segment */
	__Elf64_Addr	p_vaddr;	/* Virtual address in memory */
	__Elf64_Addr	p_paddr;	/* Physical address (if relevant) */
	__Elf64_Xword	p_filesz;	/* Size of segment in file */
	__Elf64_Xword	p_memsz;	/* Size of segment in memory */
	__Elf64_Xword	p_align;	/* Alignment constraints */
};

struct elf64_shdr {
	__Elf64_Word	sh_name;	/* Section name (string tbl index) */
	__Elf64_Word	sh_type;	/* Section type */
	__Elf64_Xword	sh_flags;	/* Section flags */
	__Elf64_Addr	sh_addr;	/* Section virtual addr at execution */
	__Elf64_Off	sh_offset;	/* Section file offset */
	__Elf64_Xword	sh_size;	/* Section size in bytes */
	__Elf64_Word	sh_link;	/* Link to another section */
	__Elf64_Word	sh_info;	/* Additional section information */
	__Elf64_Xword	sh_addralign;	/* Section alignment */
	__Elf64_Xword	sh_entsize;	/* Entry size if section holds table */
};

struct elf64_rel {
	__Elf64_Addr	r_offset;	/* Address */
	__Elf64_Xword	r_info;		/* Relocation type and symbol index */
};

struct elf64_rela {
	__Elf64_Addr	r_offset;		/* Address */
	__Elf64_Xword	r_info;			/* Relocation type and symbol index */
	__Elf64_Sxword	r_addend;		/* Addend */
};

struct elf64_sym {
	__Elf64_Word	st_name;		/* Symbol name (string tbl index) */
	unsigned char	st_info;		/* Symbol type and binding */
	unsigned char st_other;		/* Symbol visibility */
	__Elf64_Section	st_shndx;		/* Section index */
	__Elf64_Addr	st_value;		/* Symbol value */
	__Elf64_Xword	st_size;		/* Symbol size */
};

typedef struct elf64_hdr Elf64_Ehdr;
typedef struct elf64_shdr Elf64_Shdr;
typedef struct elf64_rel Elf64_Rel;
typedef struct elf64_rela Elf64_Rela;
typedef struct elf64_sym Elf64_Sym;

/* elf32 */
typedef u32	__Elf32_Addr;	/* Program address */
typedef u8	__Elf32_Byte;	/* Unsigned tiny integer */
typedef u16	__Elf32_Half;	/* Unsigned medium integer */
typedef u32	__Elf32_Off;	/* File offset */
typedef u16	__Elf32_Section;	/* Section index */
typedef s32	__Elf32_Sword;	/* Signed integer */
typedef u32	__Elf32_Word;	/* Unsigned integer */
typedef u32	__Elf32_Lword;	/* Unsigned long integer */
typedef u32	__Elf32_Xword;	/* Unsigned long integer */
typedef s32	__Elf32_Sxword;	/* Signed long integer */

/* 32 bit EHDR */
struct elf32_hdr {
	unsigned char	e_ident[EI_NIDENT];	/* ELF identification */
	__Elf32_Half	e_type;			/* Object file type (ET_*) */
	__Elf32_Half	e_machine;		/* Machine type (EM_*) */
	__Elf32_Word	e_version;		/* File format version (EV_*) */
	__Elf32_Addr	e_entry;		/* Start address */
	__Elf32_Off	e_phoff;		/* File offset to the PHDR table */
	__Elf32_Off	e_shoff;		/* File offset to the SHDR table */
	__Elf32_Word	e_flags;		/* Flags (EF_*) */
	__Elf32_Half	e_ehsize;		/* Elf header size in bytes */
	__Elf32_Half	e_phentsize;		/* PHDR table entry size in bytes */
	__Elf32_Half	e_phnum;		/* Number of PHDR entries */
	__Elf32_Half	e_shentsize;		/* SHDR table entry size in bytes */
	__Elf32_Half	e_shnum;		/* Number of SHDR entries */
	__Elf32_Half	e_shstrndx;		/* Index of section name string table */
};

/* 32 bit PHDR entry */
struct elf32_phdr {
	__Elf32_Word	p_type;		/* Type of segment */
	__Elf32_Off	p_offset;	/* File offset to segment */
	__Elf32_Addr	p_vaddr;	/* Virtual address in memory */
	__Elf32_Addr	p_paddr;	/* Physical address (if relevant) */
	__Elf32_Xword	p_filesz;	/* Size of segment in file */
	__Elf32_Xword	p_memsz;	/* Size of segment in memory */
	__Elf32_Word	p_flags;	/* Segment flags */
	__Elf32_Xword	p_align;	/* Alignment constraints */
};

/* 32 bit SHDR entry */
struct elf32_shdr {
	__Elf32_Word	sh_name;	/* Section name (string tbl index) */
	__Elf32_Word	sh_type;	/* Section type */
	__Elf32_Word	sh_flags;	/* Section flags */
	__Elf32_Addr	sh_addr;	/* Section virtual addr at execution */
	__Elf32_Off	sh_offset;	/* Section file offset */
	__Elf32_Word	sh_size;	/* Section size in bytes */
	__Elf32_Word	sh_link;	/* Link to another section */
	__Elf32_Word	sh_info;	/* Additional section information */
	__Elf32_Word	sh_addralign;	/* Section alignment */
	__Elf32_Word	sh_entsize;	/* Entry size if section holds table */
};

#endif
