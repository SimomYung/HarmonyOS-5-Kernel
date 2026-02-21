/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Definitions related to livepatch module
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 29 18:00:55 2020
 */
#ifndef KLIBS_LIVEPATCH_LOADER_H
#define KLIBS_LIVEPATCH_LOADER_H

#include <hmasm/types.h>
#include <hmkernel/types.h>
#include <hmkernel/klog_level.h>
#ifdef __KERNEL__
#include <lib/elf.h>
#else
#include <elf.h>
#include <hongmeng/types.h>
#endif
#define SHN_LIVEPATCH		0xff20
#define SHF_RELA_LIVEPATCH	0x00100000

struct __livepatch_call_plt_entry_s {
	__u32 mov0;  /* movn x16, #0x... */
	__u32 mov1;  /* movk x16, #0x..., lsl #16 */
	__u32 mov2;  /* movk x16, #0x..., lsl #32 */
	__u32 mov3;  /* movk x16, #0x..., lsl #48 */
	__u32 br;    /* br   x16 */
	__u32 resv;
};

struct __livepatch_adrp_plt_entry_s {
	__u32 adrp;  /* adrp reg, off */
	__u32 mov0;  /* movz x16, #0x..., lsl #16 */
	__u32 mov1;  /* movk x16, #0x..., lsl #32 */
	__u32 mov2;  /* movk x16, #0x..., lsl #48 */
	__u32 add;   /* add  reg, reg, x16 */
	__u32 br;    /* br   x16 */
};

struct __livepatch_got_entry_s {
	__u64 slot;
};

struct symbol_desc;
struct livepatch_load_info_s {
	/*
	 * target process info.
	 * this part of struct is input for livepatch_loader
	 */
	int pid; /* pid of process, not use in kernel. */
	unsigned long func_nr; /* func nr of patch */
	__u64 load_start; /* object(elf, so) load addr */
	struct symbol_desc *symbols; /* target process's symbol list head */

	Elf64_Ehdr *ehdr; /* patch elf context */
	unsigned long len; /* patch elf size */

	/*
	 * this part of struct is used internally by livepatch_loader,
	 * elf basic info, such as symtab index and strtab index,
	 * plt/got addr.
	 * they are calculated by ehdr and len.
	 */
	Elf64_Shdr *sechdrs;
	char *secstrings;
	unsigned int i_str;
	char *strtab;
	unsigned int i_sym;
	union {
		struct __livepatch_call_plt_entry_s *call_plts;
		struct __livepatch_adrp_plt_entry_s *adrp_plts;
	} plts;
	unsigned int nrplt;
	unsigned int plt_inuse;
	struct __livepatch_got_entry_s *gots;
	unsigned int nrgot;
	unsigned int got_inuse;

	/* struct layout is for output. */
	struct {
		/* patch code addr in target process vspace */
		char *base;
		/*
		 * patch code backup addr in sysmgr, we will relocate patch
		 * in this sysmgr vspace, and copy this context to target process
		 * vspace after relocated.
		 * in kernel and sysmgr, sys_base is same as base.
		 */
		char *sys_base;
		/* the offset of target base and sys_base. */
		__s64 offset;

		/* rx/rw/ro should aligned to page */
		unsigned long rx_size; /* patch code sections and plt, got */
		unsigned long rw_size; /* patch data */
		unsigned long ext_size; /* extend data, used for kpatch funcs. service patch not use */
		unsigned long ro_size; /* patch ro data */
		unsigned long size;    /* total size */
	} layout;
};

struct livepatch_modinfo {
	int major;      /* hmpatch-build tool major version */
	int minor;      /* hmpatch-build tool minor version */
	char *target;   /* the livepatch module's patch target */
	char *version;  /* the source code version which construct the livepatch module */
	int nr_plt;
};

/* functions that livepatch loader needed */
struct livepatch_loader_ops_s {
	const char *hmkernel_release;
	int (*symbol_lookup)(struct symbol_desc *sym_desc, int pid,
			     const char *name, unsigned long *offset);
	void *(*lp_malloc)(size_t size); /* malloc tmp mem for plggot calculate */
	void (*lp_free)(void *mem, size_t size); /* free tmp mem */

	/* alloc patch code mem in target process */
	int (*lp_alloc_target_mem)(int pid, size_t size, __uptr_t *tar_addr, __uptr_t *sys_addr);
	/* do mprotect for patch code/data mem */
	int (*lp_mprotect_target_mem)(const struct livepatch_load_info_s *info);

	void __attribute__((format(printf, 2, 3)))
		(*livepatch_printf)(enum klog_level level, const char *fmt, ...);
};

/* register functions that lp loader needed */
int livepatch_loader_ops_register(struct livepatch_loader_ops_s *ops);
/* get livepatch_loader functions, for lp_loader internal use. */
struct livepatch_loader_ops_s *livepatch_loader_ops(void);

/*
 * load livepatch to layout, and do relocate.
 *
 * before call this function, you need fill 'pid', 'load_start', 'ehdr', 'len' of
 * struct livepatch_load_info_s.
 * in functin:
 * 1. elf basic info in struct livepatch_load_info_s would be calculated first.
 * 2. lp_loader will using funstion lp_alloc_target_mem in livepatch_loader_ops_s to
 *    prepare layout memory to store patch code and data.
 * 3. move code in ehdr to layout memory and do relocate.
 * 4. finalize protect layout. make patch code has exec right.
 */
int livepatch_loader_load(struct livepatch_load_info_s *info);

/*
 * find elf section shdr from elf.
 * input: info's ehdr and len, section name
 * output: addr of shdr.
 */
Elf64_Shdr *livepatch_loader_find_section(struct livepatch_load_info_s *info, const char *sec_name);

/* generate br insntructions.
 * insns: output insntructions, len >= 5
 * val: the addr need jump to
 */
void livepatch_gen_br_insns(__u32 *insns, unsigned int len, __u64 val);

#endif
