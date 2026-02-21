/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide self relocate interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 20:31:36 2019
 */
#include <string.h>
#include <hmkernel/const.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libhmelf/elf.h>
#include <hongmeng/syscall.h>
#include <libself_rel/relocate.h>

enum {
	RELOCATE_OFFSET = 0,
	RELOCATE_TYPE,
	RELCOATE_ADDEND
};

#define STRIDE_LEN_WITHOUT_ADDEND	2U
#define STRIDE_LEN_WITH_ADDEND		3U

#ifndef __HOST_LLT__
#ifdef __clang__

size_t *dynamic_of_elf(void)
{
	extern void *_DYNAMIC __attribute((weak)) __attribute__((visibility("hidden")));
	return &_DYNAMIC;
}

#else

#define __stringfy__(s) #s
#define _stringfy_(s) __stringfy__(s)
#ifdef CONFIG_ARM
__asm__(
".section .rodata \n"
".weak _DYNAMIC \n"
"_DYNAMIC: \n"
".word "_stringfy_(RELOCATE_NO_DYNAMIC)"\n"
".hidden _DYNAMIC \n"
".text \n"
".global dynamic_of_elf \n"
".type dynamic_of_elf,%function \n"
"dynamic_of_elf: \n"
"	ldr a2, 1f \n"
"	add a2, pc, a2 \n"
"	mov a1, a2 \n"
"2:	mov pc, lr \n"
".align 2 \n"
"1:	.word _DYNAMIC-2b \n"
);
#elif defined CONFIG_AARCH64
__asm__(
".section .rodata \n"
".weak _DYNAMIC\n"
"_DYNAMIC: \n"
".quad "_stringfy_(RELOCATE_NO_DYNAMIC)"\n"
".hidden _DYNAMIC\n"
".text \n"
".global  dynamic_of_elf \n"
".type  dynamic_of_elf ,%function\n"
"dynamic_of_elf :\n"
"	adrp x0, _DYNAMIC\n"
"	add x0, x0, #:lo12:_DYNAMIC\n"
"	ret\n"
);
#else
#error "not supported arch"
#endif
#endif
#else

uintptr_t llt_dynamic_addr;

size_t *dynamic_of_elf(void)
{
	return (size_t *)llt_dynamic_addr;
}

#endif
static void relocate(uintptr_t load_start, const size_t *rel_entry, size_t size, size_t stride)
{
	size_t tls_offset;
	size_t align = 0UL;
	const size_t tls_val = 0UL;
	int rc;
	const char *p1 = "tls segment not found\n";
	const char *p2 = "relocation type invalid\n";
	size_t rel_size = size;
	const size_t *rel = rel_entry;

	for (; rel_size > 0; rel_size -= stride * sizeof(size_t)) {
		size_t *rel_addr = ulong_to_ptr(load_start + rel[RELOCATE_OFFSET], size_t);
		size_t addend = stride > STRIDE_LEN_WITHOUT_ADDEND ? rel[RELCOATE_ADDEND] : *rel_addr;

		switch (r_type(rel[RELOCATE_TYPE])) {
		case REL_RELATIVE:
			*rel_addr = load_start + addend;
			break;
		case REL_TPOFF:
			rc = elf_phdr_info(load_start, PT_TLS, NULL, NULL, &align, NULL);
			if (rc != E_HM_OK) {
				(void)sysfast_syslog((unsigned long)HMLOG_PANIC, p1, strlen(p1), NULL);
				__panic_impl(NULL, __FUNCTION__, __LINE__);
			}
			tls_offset = (size_t)(GAP_ABOVE_TP);
			tls_offset += (size_t)(-GAP_ABOVE_TP) & (size_t)(align - __U(1));
			/*
			 * Since the current sysmgr does not support dynamic libraries,
			 * tls_val is 0
			 */
			*rel_addr = tls_val + tls_offset + addend;
			break;
		case REL_NONE:
			break;
		default:
			(void)sysfast_syslog((unsigned)HMLOG_PANIC, p2, strlen(p2), NULL);
			__panic_impl(NULL, __FUNCTION__, __LINE__);
			break;
		}
		rel += stride;
	}
}

void selfreloc_relocate(uintptr_t load_start)
{
	size_t dyn[DYN_COUNT];
	size_t *rel = NULL;
	size_t rel_size;
	size_t i;
	size_t *dynv = dynamic_of_elf();

	if (!((dynv == NULL) || (*dynv == RELOCATE_NO_DYNAMIC))) {
		for (i = 0U; i < DYN_COUNT; i++) {
			dyn[i] = 0U;
		}
		for (i = 0U; dynv[i] != 0UL; i += STRIDE_LEN_WITHOUT_ADDEND) {
			if (dynv[i] < DYN_COUNT) {
				dyn[dynv[i]] = dynv[i + __U(1)];
			}
		}
		rel = ulong_to_ptr(load_start + dyn[DT_REL], size_t);
		rel_size = dyn[DT_RELSZ];
		relocate(load_start, rel, rel_size, STRIDE_LEN_WITHOUT_ADDEND);

		rel = ulong_to_ptr(load_start + dyn[DT_RELA], size_t);
		rel_size = dyn[DT_RELASZ];
		relocate(load_start, rel, rel_size, STRIDE_LEN_WITH_ADDEND);
	}
}
