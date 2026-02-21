/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Calculate address space layout
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 22 21:04:34 2019
 */
#ifndef KLIBS_VSPACE_LAYOUT_H
#define KLIBS_VSPACE_LAYOUT_H

#include <hmasm/types.h>
#include <hmkernel/types.h>

struct vspace_layout {
	__uptr_t load_start;		/* start of loaded image range */
	__uptr_t load_end;		/* end of loaded image range */
	__uptr_t stack_start;		/* start of stack range */
	__uptr_t stack_end;		/* end of stack range */
	__uptr_t heap_start;		/* start of heap range */
	__uptr_t heap_end;		/* end of heap range */
	__uptr_t mmap_base;		/* base of mmap range */
	__uptr_t mmap_top;		/* top of mmap mmap range */
	__uptr_t auxv_start;		/* start of auxv data */
	__uptr_t auxv_end;		/* end of auxv data */
	__uptr_t argc_start;		/* start of argument data */
	__uptr_t arg_start;		/* start of argument data */
	__uptr_t arg_end;		/* end of argument data */
	__uptr_t env_start;		/* start of environment range */
	__uptr_t env_end;		/* end of environment range */
	__uptr_t interp_base;		/* base of PT_LOAD segment of the interpreter */
	__uptr_t interp_end;		/* end of PT_LOAD segment of the interpreter */
	__uptr_t rodata_start;          /* base of read-only data segment */
	__uptr_t rodata_end;            /* end of read-only data segment */
	__uptr_t data_start;		/* base of writable segment */
	__uptr_t data_end;		/* end of writable segment */
	__uptr_t code_start;		/* base of exectuable segment */
	__uptr_t code_end;		/* end of executable segment */
	__uptr_t dynamic_start;         /* start of .dynamic section */
	__uptr_t dynamic_end;           /* end of .dynamic section */
	__uptr_t got_start;             /* start of .got section */
	__uptr_t got_end;               /* end of .got section */
	__uptr_t phdr_start;            /* start of PHDR segment */
	__uptr_t phdr_end;              /* end of PHDR segment */
	__uptr_t vdso_start;            /* start of vdso */
	__uptr_t vdso_end;              /* end of vdso */
	__uptr_t pcpinfo_start;            /* start of pcpinfo */
	__uptr_t pcpinfo_end;              /* end of pcpinfo */
	__uptr_t gpuheap_start;		/* start of gpuheap(4gva) */
	__uptr_t gpuheap_end;		/* end of gpuheap(4gva) */
	__uptr_t load_offset;           /* offset of load */
};

enum {
	ASLR_DYN_OFFSET,
	ASLR_LOAD_OFFSET,
	ASLR_HEAP_OFFSET,
	ASLR_STACK_OFFSET,
	ASLR_MMAP_OFFSET,
	ASLR_OFFSET_NR
};

struct vspace_rnd_arg {
	__uptr_t layout_start;
	__uptr_t layout_end;
	__uptr_t load_offset;
	bool is_pie;
	size_t load_align;
};

typedef int (*rnd_off_gen_t)(__u8, unsigned long *);

struct layout_param {
	size_t stack_size;
	size_t heap_size;
};

struct vspace_rnd_gen {
	rnd_off_gen_t generate;
	__u8 rnd_bit;
};

struct vspace_range {
	__uptr_t start;
	__uptr_t end;
};

int vspace_layout_init(struct vspace_layout *layout, struct layout_param params,
		       __uptr_t load_offset, __uptr_t layout_end);

int vspace_layout_randomize(struct vspace_layout *layout, const __u8 r_bits[ASLR_OFFSET_NR],
			    rnd_off_gen_t generate, struct vspace_rnd_arg *rnd_arg);

int vspace_randomsize_segment(struct vspace_range *avail_range, __uptr_t segment_sz, struct vspace_rnd_gen gen);

#endif
